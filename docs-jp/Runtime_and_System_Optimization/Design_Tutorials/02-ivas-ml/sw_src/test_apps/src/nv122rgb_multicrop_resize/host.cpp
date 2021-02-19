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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "xcl2.hpp"

#include <iostream>
#include <random>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

#define NUM_PARALLEL_ROIS 5
#define HEIGHT 1080
#define WIDTH 1920
#define RESIZE_H 80
#define RESIZE_W 80
#define STRIDE_IN_W 2048
#define STRIDE_IN_H 1088


ssize_t read_file_to_buf(const char *file, uint8_t **buf, size_t *len)
{
    ssize_t ret;
    FILE *fp;

    fp = fopen(file, "r");
    if (fp == NULL) {
        return -1;
    }

    ret = fseek(fp, 0, SEEK_END);
    if (ret < 0) {
        return -1;
    }

    *len = (size_t)ftell(fp);
    if ((*len) < 0) {
        return -1;
    }

    rewind(fp);

    *buf = (uint8_t *)malloc(sizeof(uint8_t) * (*len));
    if (*buf == NULL) {
        return -1;
    }

    fread(*buf, (*len), sizeof(char), fp);
    fclose(fp);

    return 0;
}

ssize_t write_buf_to_file(const uint8_t *buf, const size_t len, const char *file)
{
    size_t ret;
    FILE *fp;

    fp = fopen(file, "w");
    if (fp == NULL) {
        return -1;
    }

    ret = fwrite(buf, len, sizeof(char), fp);
    if (ret < 0) {
        return -1;
    }

    fclose(fp);

    return 0;
}

uint32_t roi_size(cv::Rect _roi)
{
    // Size in term of bytes is "height x width x no.of channels". In RGB case it is "h x w x 3"
    return (uint32_t)((((_roi.height * _roi.width) + 1) * 3) / 4); // This gives ceil ((h * w * 3)/4)
}

int main(int argc, char **argv)
{

    uint8_t *input_img;
    uint8_t *y_img, *uv_img;

    if (argc != 2) {
        std::cout << "Usage: nv12_multicrop_resize_test <filename>" << std::endl;
        std::cout << "    Note input should be raw NV12 data" << std::endl;
    }

    std::ifstream test_data(argv[1], std::ifstream::binary);
    if (!test_data) {
        std::cout << "ERROR: Unable to open input data file!" << std::endl;
        return EXIT_FAILURE;
    }
    test_data.seekg(0, test_data.end);
    unsigned int nb = test_data.tellg();

    printf("Size of file is %d bytes\n", nb);
    input_img = (uint8_t *)malloc(sizeof(uint8_t) * nb);

    test_data.seekg(0, test_data.beg);
    test_data.read((char *)input_img, nb);
    test_data.close();
    y_img  = input_img;
    uv_img = input_img + (STRIDE_IN_W * STRIDE_IN_H);

    printf("Y at %p, UV at %p\n", y_img, uv_img);

    uint32_t roi_info_size = (NUM_PARALLEL_ROIS * 5 + 1) * sizeof(uint32_t);
    uint32_t *roi_info;
    roi_info = (uint32_t *)malloc(roi_info_size);

    roi_info[0] = NUM_PARALLEL_ROIS;

    for (uint32_t i = 0, j = 0; j < NUM_PARALLEL_ROIS; j++, i += 5) {
        roi_info[i + 1] = (uint32_t)640; // Start column of ROI
        roi_info[i + 2] = (uint32_t)0;   // Start row of ROI
        roi_info[i + 3] = (uint32_t)360; // Height of ROI
        roi_info[i + 4] = (uint32_t)640; // Width of ROI
        roi_info[i + 5] = (uint32_t)0;   // Offset of ROI
    }

    uint32_t y_plane_size  = (STRIDE_IN_W * STRIDE_IN_H);
    uint32_t uv_plane_size = (STRIDE_IN_H / 2) * STRIDE_IN_W;
    uint32_t rgb_size      = (RESIZE_H * RESIZE_W * 3);

    uint8_t *out_buf_0 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_1 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_2 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_3 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_4 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_5 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_6 = (uint8_t *)malloc(rgb_size);
    uint8_t *out_buf_7 = (uint8_t *)malloc(rgb_size);

    cl::Event event_sp;
    cl_int err;

    auto devices = xcl::get_xil_devices();
    auto device  = devices[0];
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err, cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    OCL_CHECK(err, std::string device_name = device.getInfo<CL_DEVICE_NAME>(&err));
    auto fileBuf = xcl::read_binary_file("/media/card/dpu.xclbin");
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    devices.resize(1);

    printf("Kernel Stage\n");
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
    OCL_CHECK(err, cl::Kernel krnl(program, "nv122rgb_multicrop_resize", &err));

    printf("Buffer Stage\n");
    OCL_CHECK(err, cl::Buffer yToDevice(context, CL_MEM_READ_ONLY, y_plane_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer uvToDevice(context, CL_MEM_READ_ONLY, uv_plane_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer imagesFromDevice0(context, CL_MEM_WRITE_ONLY, rgb_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer imagesFromDevice1(context, CL_MEM_WRITE_ONLY, rgb_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer imagesFromDevice2(context, CL_MEM_WRITE_ONLY, rgb_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer imagesFromDevice3(context, CL_MEM_WRITE_ONLY, rgb_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer imagesFromDevice4(context, CL_MEM_WRITE_ONLY, rgb_size, NULL, &err));
    OCL_CHECK(err, cl::Buffer roiToDevice(context, CL_MEM_READ_ONLY, roi_info_size, NULL, &err));


    printf("Arg Stage\n");
    OCL_CHECK(err, err = krnl.setArg(0, yToDevice));
    OCL_CHECK(err, err = krnl.setArg(1, uvToDevice));
    //
    OCL_CHECK(err, err = krnl.setArg(2, imagesFromDevice0));
    OCL_CHECK(err, err = krnl.setArg(3, imagesFromDevice1));
    OCL_CHECK(err, err = krnl.setArg(4, imagesFromDevice2));
    OCL_CHECK(err, err = krnl.setArg(5, imagesFromDevice3));
    OCL_CHECK(err, err = krnl.setArg(6, imagesFromDevice4));
    //
    OCL_CHECK(err, err = krnl.setArg(7, HEIGHT));
    OCL_CHECK(err, err = krnl.setArg(8, WIDTH));
    OCL_CHECK(err, err = krnl.setArg(9, STRIDE_IN_W));
    OCL_CHECK(err, err = krnl.setArg(10, roiToDevice));

    printf("Enqueue Stage\n");
    q.enqueueWriteBuffer(yToDevice, CL_TRUE, 0, y_plane_size, y_img);
    q.enqueueWriteBuffer(uvToDevice, CL_TRUE, 0, uv_plane_size, uv_img);
    q.enqueueWriteBuffer(roiToDevice, CL_TRUE, 0, roi_info_size, roi_info);

    printf("Task Stage\n");
    q.enqueueTask(krnl, NULL, &event_sp);
    printf("Wait Stage\n");
    clWaitForEvents(1, (const cl_event *)&event_sp);

    cl_ulong start   = 0;
    cl_ulong end     = 0;
    double diff_prof = 0.0f;
    event_sp.getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
    event_sp.getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
    diff_prof = end - start;
    std::cout << "Cropped and Scaled " << NUM_PARALLEL_ROIS << " ROIs in " << (diff_prof / 1000000) << "ms"
              << " (Avg: " << (diff_prof / 1000000) / NUM_PARALLEL_ROIS << " ms per ROI)" << std::endl;

    q.enqueueReadBuffer(imagesFromDevice0, CL_TRUE, 0, rgb_size, out_buf_0);
    q.enqueueReadBuffer(imagesFromDevice1, CL_TRUE, 0, rgb_size, out_buf_1);
    q.enqueueReadBuffer(imagesFromDevice2, CL_TRUE, 0, rgb_size, out_buf_2);
    q.enqueueReadBuffer(imagesFromDevice3, CL_TRUE, 0, rgb_size, out_buf_3);
    q.enqueueReadBuffer(imagesFromDevice4, CL_TRUE, 0, rgb_size, out_buf_4);

    q.finish();

    std::cout << "Done with CL session" << std::endl;

    cv::Mat roi_mat0(RESIZE_H, RESIZE_W, CV_8UC3, out_buf_0);
    cv::Mat roi_mat1(RESIZE_H, RESIZE_W, CV_8UC3, out_buf_1);
    cv::Mat roi_mat2(RESIZE_H, RESIZE_W, CV_8UC3, out_buf_2);
    cv::Mat roi_mat3(RESIZE_H, RESIZE_W, CV_8UC3, out_buf_3);
    cv::Mat roi_mat4(RESIZE_H, RESIZE_W, CV_8UC3, out_buf_4);

    cv::cvtColor(roi_mat0, roi_mat0, CV_RGB2BGR);
    cv::cvtColor(roi_mat1, roi_mat1, CV_RGB2BGR);
    cv::cvtColor(roi_mat2, roi_mat2, CV_RGB2BGR);
    cv::cvtColor(roi_mat3, roi_mat3, CV_RGB2BGR);
    cv::cvtColor(roi_mat4, roi_mat4, CV_RGB2BGR);

    cv::imwrite("0.png", roi_mat0);
    cv::imwrite("1.png", roi_mat1);
    cv::imwrite("2.png", roi_mat2);
    cv::imwrite("3.png", roi_mat3);
    cv::imwrite("4.png", roi_mat4);


    free(out_buf_0);
    free(out_buf_1);
    free(out_buf_2);
    free(out_buf_3);
    free(out_buf_4);
    free(out_buf_5);
    free(out_buf_6);
    free(out_buf_7);
    ;
    free(input_img);
    free(roi_info);

    printf("DONE\n");

    return 0;
}
