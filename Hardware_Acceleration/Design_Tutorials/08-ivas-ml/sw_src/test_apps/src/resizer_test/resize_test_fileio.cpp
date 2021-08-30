/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "event_timer.hpp"
#include "xilinx_ocl.hpp"

#include <CL/cl2.hpp>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#define IN_WIDTH 1920
#define IN_HEIGHT 1080
#define OUT_WIDTH 640
#define OUT_HEIGHT 360
#define FRAMERATE 60
#define NUM_BUFS 3

struct capture_buffer
{
    int fd;
    cl::Buffer buffer;
    size_t length;
    void *data;
    bool initialized;
    bool ocl_initialized;
    uint64_t count;
    int id;
    int metadata;
};

capture_buffer allocate_buffer(swm::XilinxOcl &xocl, cl::CommandQueue &q, cl::Kernel &krnl, size_t size)
{
    capture_buffer cbuf;
    cl_int err;

    cbuf.buffer = xocl.create_buffer(size, CL_MEM_READ_ONLY);

    krnl.setArg(0, cbuf.buffer);

    err = q.enqueueMigrateMemObjects({cbuf.buffer},
                                     CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED | CL_MIGRATE_MEM_OBJECT_HOST);
    if (err != CL_SUCCESS) {
        std::cout << "ERROR: Unable to enqueue memory migration: err=" << err << std::endl;
    }
    q.finish();

    cbuf.length = size;
    cbuf.fd     = xocl.get_fd_for_buffer(cbuf.buffer);

    cbuf.data = q.enqueueMapBuffer(cbuf.buffer, CL_TRUE, CL_MAP_READ, 0, size, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        std::cout << "ERROR: Unable to map userspace buffer pointer: err=" << err << std::endl;
    }
    cbuf.initialized = false;
    cbuf.count       = 0;
    return cbuf;
}

int main(int argc, char *argv[])
{

    if (argc != 3) {
        std::cout << "Usage: resize_test_fileio <filename> <stride>" << std::endl;
        std::cout << "    Note input should be raw NV12 data" << std::endl;
    }

    // Initialize OpenCL instance
    swm::XilinxOcl xocl;
    if (std::getenv("DPU_XCLBIN_PATH")) {
        xocl.initialize(std::getenv("DPU_XCLBIN_PATH"));
    }
    else {
        xocl.initialize("zcu102_dpu.xclbin");
    }

    cl_int err;
    cl::CommandQueue q = xocl.get_command_queue();
    cl::Kernel krnl    = xocl.get_kernel("resize_accel_rgb");

    // Allocate three buffers for capture
    capture_buffer buf;

    int stride = std::atoi(argv[2]);

    std::cout << "Processing file " << argv[1] << " with stride " << stride << std::endl;

    // Allocate memory for the scaled vector (we'll downscale to 720p for the test)
    //std::vector<cl::Memory> bufvec;
    std::ifstream test_data(argv[1], std::ifstream::binary);
    if (!test_data) {
        std::cout << "ERROR: Unable to open input data file!" << std::endl;
        return EXIT_FAILURE;
    }
    test_data.seekg(0, test_data.end);
    unsigned int nb = test_data.tellg();

    // Allocate input buffer based on size
    buf = allocate_buffer(xocl, q, krnl, nb);

    test_data.seekg(0, test_data.beg);
    test_data.read((char *)buf.data, nb);
    test_data.close();

    int rows = IN_HEIGHT;
    if (stride != IN_WIDTH) {
        rows = 1088;
    }

    cl::Buffer subBuf = xocl.create_sub_buffer(buf.buffer,
                                               stride * rows,
                                               xocl.get_buffer_size(buf.buffer) - (stride * rows),
                                               CL_MEM_READ_ONLY);

    cl::Buffer image_out = xocl.create_buffer(OUT_WIDTH * OUT_HEIGHT * 3,
                                              CL_MEM_READ_WRITE);


    std::vector<cl::Event> events;

    // Set kernel arguments aside from the input buffer pointer
    krnl.setArg(2, image_out);
    krnl.setArg(3, IN_WIDTH);
    krnl.setArg(4, IN_HEIGHT);
    krnl.setArg(5, stride);
    krnl.setArg(6, OUT_WIDTH);
    krnl.setArg(7, OUT_HEIGHT);

    // Map the output buffer to userspace memory
    uint8_t *img_from_device = (uint8_t *)q.enqueueMapBuffer(image_out,
                                                             CL_TRUE,
                                                             CL_MAP_READ,
                                                             0,
                                                             OUT_WIDTH * OUT_HEIGHT * 3,
                                                             NULL,
                                                             NULL,
                                                             &err);


    krnl.setArg(0, buf.buffer);
    krnl.setArg(1, subBuf);
    krnl.setArg(8, 1);
    krnl.setArg(9, 128);

    std::array<cl::Event, 4> cl_events;
    EventTimer et;

    et.add("Kernel operation");
    err |= q.enqueueMigrateMemObjects({buf.buffer}, 0, NULL, &cl_events[0]);
    events.push_back(cl_events[0]);
    err |= q.enqueueTask(krnl, &events, &cl_events[1]);
    events.push_back(cl_events[1]);
    err |= q.enqueueMigrateMemObjects({image_out}, CL_MIGRATE_MEM_OBJECT_HOST, &events, &cl_events[2]);

    if (err != CL_SUCCESS) {
        std::cout << "ERROR: Unable to successfully enqueue task! err=" << err << std::endl;
    }
    clWaitForEvents(3, (const cl_event *)&cl_events);
    et.finish();
    events.clear();

    // Create an OpenCV Mat object and save the image to disk
    cv::Mat result_hw(OUT_HEIGHT, OUT_WIDTH, CV_8UC3, (void *)img_from_device);
    cv::imwrite("result_hw.png", result_hw);

    cv::Mat normalized = result_hw.clone();

    for (int i = 0; i < OUT_WIDTH * OUT_HEIGHT * 3; i++) {
        normalized.data[i] = (char)(result_hw.data[i] + 128);
    }
    cv::imwrite("subbed.png", normalized);


    q.enqueueUnmapMemObject(buf.buffer, buf.data);
    q.enqueueUnmapMemObject(image_out, img_from_device);
    q.finish();

    et.print();

    return EXIT_SUCCESS;
}
