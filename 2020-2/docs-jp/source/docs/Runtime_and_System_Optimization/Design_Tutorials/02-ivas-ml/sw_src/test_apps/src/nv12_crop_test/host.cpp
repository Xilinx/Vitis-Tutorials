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
#include <vector>

#define NUM_PARALLEL_ROIS 10
#define HEIGHT 1080
#define WIDTH 1920
#define RESIZE_H 80
#define RESIZE_W 80
#define STRIDE_IN_W 2048
#define STRIDE_IN_H 1088

#define GST_ROUND_UP_8(num) (((num) + 7) & ~7)
#define GST_ROUND_UP_2(num) (((num) + 1) & ~1)

#define OVERRIDE

EventTimer et;

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


void event_cb(cl_event event, cl_int cmd_status, void *id)
{
    if (getenv("XCL_EMULATION_MODE") != NULL) {
        std::cout << "  kernel finished processing request " << *(int *)id << std::endl;
    }
    cl_ulong start;
    cl_ulong end;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    double diff_prof = 0.0f;
    diff_prof        = end - start;
    std::cout << "Crop took " << (diff_prof / 1000000) << "ms" << std::endl;
}


// -------------------------------------------------------------------------------------------
// Struct returned by cropDispatcher() and used to keep track of the request sent to the kernel
// The sync() method waits for completion of the request. After it returns, results are ready
// -------------------------------------------------------------------------------------------
struct CropRequest
{

    cl_event mEvent[3];
    int mId;

    CropRequest(int id)
    {
        mId = id;
    }

    void sync()
    {
        // Wait until the outputs have been read back
        clWaitForEvents(1, &mEvent[2]);
        clReleaseEvent(mEvent[0]);
        clReleaseEvent(mEvent[1]);
        clReleaseEvent(mEvent[2]);
    }
};

// -------------------------------------------------------------------------------------------
// Class used to dispatch requests to the kernel
// The CropDispatcher() method schedules the necessary operations (write, kernel, read) and
// returns a CropRequest* struct which can be used to track the completion of the request.
// The dispatcher has its own OOO command queue allowing multiple requests to be scheduled
// and executed independently by the OpenCL runtime.
// -------------------------------------------------------------------------------------------
class CropDispatcher
{

public:
    CropDispatcher(
        cl_device_id &Device,
        cl_context &Context,
        cl_program &Program)
    {
        mKernel  = clCreateKernel(Program, "nv12_crop", &mErr);
        mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE, &mErr);
        mContext = Context;
        mCounter = 0;
    }

    CropRequest *operator()(
        uint8_t *y_img,
        uint8_t *uv_img,
        uint8_t *y_crop,
        uint8_t *uv_crop,
        uint32_t roi_x,
        uint32_t roi_y,
        uint32_t roi_w,
        uint32_t roi_h,
        uint32_t stride)
    {

        CropRequest *req = new CropRequest(mCounter++);

        et.add("Buffer Creation");
        // Y-Input Buffer
        mSrcBuf[0] = clCreateBuffer(mContext, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, STRIDE_IN_W * STRIDE_IN_H * sizeof(uint8_t), y_img, &mErr);
        // UV-Input Buffer
        mSrcBuf[1] = clCreateBuffer(mContext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, STRIDE_IN_W * STRIDE_IN_H / 2 * sizeof(uint8_t), uv_img, &mErr);
        // Y-Output Buffer
        mDstBuf[0] = clCreateBuffer(mContext, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, roi_h * roi_w * sizeof(uint8_t), y_crop, &mErr);
        // UV-Output Buffer
        mDstBuf[1] = clCreateBuffer(mContext, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, roi_w * roi_h / 2 * sizeof(uint8_t), uv_crop, &mErr);
        et.add("Set Kernel Args");
        clSetKernelArg(mKernel, 0, sizeof(cl_mem), &mSrcBuf[0]);
        clSetKernelArg(mKernel, 1, sizeof(cl_mem), &mSrcBuf[1]);
        clSetKernelArg(mKernel, 2, sizeof(cl_mem), &mDstBuf[0]);
        clSetKernelArg(mKernel, 3, sizeof(cl_mem), &mDstBuf[1]);
        clSetKernelArg(mKernel, 4, sizeof(uint32_t), &roi_x);
        clSetKernelArg(mKernel, 5, sizeof(uint32_t), &roi_y);
        clSetKernelArg(mKernel, 6, sizeof(uint32_t), &roi_w);
        clSetKernelArg(mKernel, 7, sizeof(uint32_t), &roi_h);
        clSetKernelArg(mKernel, 8, sizeof(uint32_t), &stride);

        et.add("Enqueue Mem and Tasks");
        clEnqueueMigrateMemObjects(mQueue, 2, mSrcBuf, 0, 0, nullptr, &req->mEvent[0]);

        clEnqueueTask(mQueue, mKernel, 1, &req->mEvent[0], &req->mEvent[1]);

        clEnqueueMigrateMemObjects(mQueue, 2, mDstBuf, CL_MIGRATE_MEM_OBJECT_HOST, 1, &req->mEvent[1], &req->mEvent[2]);

        et.finish();
        et.print();
        //        clSetEventCallback(req->mEvent[1], CL_COMPLETE, event_cb, &req->mId);

        return req;
    };

    ~CropDispatcher()
    {
        clReleaseCommandQueue(mQueue);
        clReleaseKernel(mKernel);
    };

private:
    cl_kernel mKernel;
    cl_command_queue mQueue;
    cl_context mContext;
    cl_mem mSrcBuf[2];
    cl_mem mDstBuf[2];
    cl_int mErr;
    int mCounter;
};

int main(int argc, char **argv)
{

    if (argc != 3) {
        fprintf(stderr, "Invalid Number of Arguments!\nUsage: ");
        fprintf(stderr, "crop_accel.exe <1080p nv12 image> <num_crops>\n");
        return -1;
    }

    //------------------------------------------------------------------------------------------------------------------
    //    cl_int err;
    //    std::cout << "Programming FPGA" << std::endl;
    //    cl_context    	context;
    //    cl_program    	program;
    //    cl_device_id	device;
    //    load_xclbin_file("./binary_container_1.xclbin", context, device, program);

    //------------------------------------------------------------------------------------------------------------------
    uint32_t num_crops = atol(argv[2]);

    uint32_t img_height = STRIDE_IN_H;
    uint32_t img_width  = STRIDE_IN_W;

    printf("NV12 Crop Test Application\n");
    printf("This application will crop %d random ROIs from a 1080p (%4d x %4d) NV12 frame\n\n", num_crops, img_width,
           img_height);

    cv::Rect ROIs[num_crops];
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

    for (uint32_t i = 0; i < num_crops; i++) {
        // X Y
        std::uniform_int_distribution<> distr(0, img_height - 128);
        ROIs[i].y = distr(eng);
        distr     = std::uniform_int_distribution<>(0, img_width - 256);
        ROIs[i].x = distr(eng);
        // Width
        do {
            std::uniform_int_distribution<> distr(40, 200);
            ROIs[i].width = distr(eng);
        } while ((ROIs[i].x + ROIs[i].width >= (int)img_width) || (ROIs[i].width == 0));
        // Height
        do {
            std::uniform_int_distribution<> distr(40, 200);
            ROIs[i].height = distr(eng);
        } while ((ROIs[i].y + ROIs[i].height >= (int)img_height) || ROIs[i].height == 0 ||
                 (ROIs[i].height > 80 && ROIs[i].width <= 80) || (ROIs[i].height <= 80 && ROIs[i].width > 80));

#ifdef OVERRIDE
        ROIs[i].x      = 0;
        ROIs[i].y      = 0;
        ROIs[i].width  = 208;
        ROIs[i].height = 208;
#endif

        printf("Original ROI %3d: at (x:%4d, y:%4d) with dimension (w:%4d x h:%4d)\n",
               i, ROIs[i].x, ROIs[i].y, ROIs[i].width, ROIs[i].height);
    }
    printf("\n");

    //------------------------------------------------------------------------------------------------------------------
    uint8_t *y_crop_bufs[num_crops];
    uint8_t *uv_crop_bufs[num_crops];
    void *ptr;
    uint32_t roiSize[num_crops];


    for (uint32_t i = 0; i < num_crops; i++) {
        // X and Y Need to Be Even
        ROIs[i].x = GST_ROUND_UP_2(ROIs[i].x);
        ROIs[i].y = GST_ROUND_UP_2(ROIs[i].y);

        // Alignment Check
        ROIs[i].width  = GST_ROUND_UP_8(ROIs[i].width);
        ROIs[i].height = GST_ROUND_UP_8(ROIs[i].height);

        // Overflow Check
        if ((ROIs[i].x + ROIs[i].width > WIDTH)) {
            ROIs[i].x -= (ROIs[i].width + ROIs[1].x) - WIDTH;
            ROIs[i].x = GST_ROUND_UP_8(ROIs[i].x);
        }
        if ((ROIs[i].y + ROIs[i].height > HEIGHT)) {
            ROIs[i].y -= (ROIs[i].y + ROIs[i].height) - HEIGHT;
            ROIs[i].y = GST_ROUND_UP_8(ROIs[i].y);
        }

        roiSize[i] = ROIs[i].width * ROIs[i].height * 3;
        if (posix_memalign(&ptr, 4096, roiSize[i] * sizeof(uint8_t)))
            throw std::bad_alloc();
        y_crop_bufs[i] = reinterpret_cast<uint8_t *>(ptr);
        if (posix_memalign(&ptr, 4096, (roiSize[i] / 2) * sizeof(uint8_t)))
            throw std::bad_alloc();
        uv_crop_bufs[i] = reinterpret_cast<uint8_t *>(ptr);

        printf("Adjusted ROI %3d: at (x:%4d, y:%4d) with dimension (w:%4d x h:%4d)\n",
               i, ROIs[i].x, ROIs[i].y, ROIs[i].width, ROIs[i].height);
    }

    //------------------------------------------------------------------------------------------------------------------
    uint8_t *input_img;
    uint8_t *y_img, *uv_img;

    std::ifstream test_data(argv[1], std::ifstream::binary);
    if (!test_data) {
        std::cout << "ERROR: Unable to open input data file!" << std::endl;
        return EXIT_FAILURE;
    }
    test_data.seekg(0, test_data.end);
    unsigned int nb = test_data.tellg();

    printf("Size of input file is %d bytes\n", nb);
    input_img = (uint8_t *)malloc(sizeof(uint8_t) * nb);

    test_data.seekg(0, test_data.beg);
    test_data.read((char *)input_img, nb);
    test_data.close();

    if (posix_memalign(&ptr, 4096, STRIDE_IN_H * STRIDE_IN_W * sizeof(uint8_t)))
        throw std::bad_alloc();
    y_img = reinterpret_cast<uint8_t *>(ptr);
    if (posix_memalign(&ptr, 4096, STRIDE_IN_W * STRIDE_IN_H / 2 * sizeof(uint8_t)))
        throw std::bad_alloc();
    uv_img = reinterpret_cast<uint8_t *>(ptr);

    memcpy(y_img, input_img, (STRIDE_IN_W * STRIDE_IN_H) * sizeof(uint8_t));
    memcpy(uv_img, input_img + (STRIDE_IN_W * STRIDE_IN_H), (STRIDE_IN_W * STRIDE_IN_H / 2) * sizeof(uint8_t));

    //------------------------------------------------------------------------------------------------------------------
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

    uint32_t *roi_info;
    uint32_t roi_size = ((num_crops * 4) + 1) * sizeof(uint32_t);
    roi_info          = (uint32_t *)malloc(roi_size);
    roi_info[0]       = num_crops;
    for (int i = 0; i < num_crops; i++) {
        roi_info[(i * 4) + 1] = ROIs[i].x;
        roi_info[(i * 4) + 2] = ROIs[i].y;
        roi_info[(i * 4) + 3] = ROIs[i].width;
        roi_info[(i * 4) + 4] = ROIs[i].height;
    }

    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
    OCL_CHECK(err, cl::Kernel krnl(program, "nv12_crop", &err));

    uint32_t yInSize  = STRIDE_IN_W * STRIDE_IN_H;
    uint32_t uvInSize = STRIDE_IN_W * STRIDE_IN_H / 2;
    uint32_t tempSize;
    cl::Buffer cropBufArray[10];

    et.add("Buffers");
    OCL_CHECK(err, cl::Buffer yToDevice(context, CL_MEM_WRITE_ONLY, yInSize, NULL, &err));
    OCL_CHECK(err, cl::Buffer uvToDevice(context, CL_MEM_WRITE_ONLY, uvInSize, NULL, &err));
    OCL_CHECK(err, cl::Buffer roiInfo(context, CL_MEM_WRITE_ONLY, roi_size, NULL, &err));
    for (int i = 0; i < num_crops; i++) {
        tempSize = ROIs[i].width * ROIs[i].height * 3;
        OCL_CHECK(err, cropBufArray[i] = cl::Buffer(context, CL_MEM_READ_ONLY, tempSize, NULL, &err));
    }
    //    tempSize = 1920*1080*3;
    //    OCL_CHECK(err, cl::Buffer rgb_out0(context, CL_MEM_WRITE_ONLY, tempSize, NULL, &err));

    et.add("Args");
    OCL_CHECK(err, err = krnl.setArg(0, yToDevice));
    OCL_CHECK(err, err = krnl.setArg(1, uvToDevice));
    OCL_CHECK(err, err = krnl.setArg(2, cropBufArray[0]));
    OCL_CHECK(err, err = krnl.setArg(3, cropBufArray[1]));
    OCL_CHECK(err, err = krnl.setArg(4, cropBufArray[2]));
    OCL_CHECK(err, err = krnl.setArg(5, cropBufArray[3]));
    OCL_CHECK(err, err = krnl.setArg(6, cropBufArray[4]));
    OCL_CHECK(err, err = krnl.setArg(7, cropBufArray[5]));
    OCL_CHECK(err, err = krnl.setArg(8, cropBufArray[6]));
    OCL_CHECK(err, err = krnl.setArg(9, cropBufArray[7]));
    OCL_CHECK(err, err = krnl.setArg(10, cropBufArray[8]));
    OCL_CHECK(err, err = krnl.setArg(11, cropBufArray[9]));
    OCL_CHECK(err, err = krnl.setArg(12, roiInfo));
    OCL_CHECK(err, err = krnl.setArg(13, STRIDE_IN_W));
    OCL_CHECK(err, err = krnl.setArg(14, 0));
    et.add("Enqueue Write Buffers");
    q.enqueueWriteBuffer(yToDevice, CL_TRUE, 0, yInSize, y_img);
    q.enqueueWriteBuffer(uvToDevice, CL_TRUE, 0, uvInSize, uv_img);
    q.enqueueWriteBuffer(roiInfo, CL_TRUE, 0, roi_size, roi_info);

    et.add("Task");
    q.enqueueTask(krnl, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    cl_ulong start   = 0;
    cl_ulong end     = 0;
    double diff_prof = 0.0f;
    event_sp.getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
    event_sp.getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
    diff_prof = end - start;
    std::cout << "Cropped and Scaled " << NUM_PARALLEL_ROIS << " ROIs in " << (diff_prof / 1000000) << "ms"
              << " (Avg: " << (diff_prof / 1000000) / NUM_PARALLEL_ROIS << " ms per ROI)" << std::endl;

    et.add("Enqueue Read Buffers");

    for (int i = 0; i < num_crops; i++) {
        tempSize = ROIs[i].width * ROIs[i].height * 3;
        q.enqueueReadBuffer(cropBufArray[0], CL_TRUE, 0, tempSize, y_crop_bufs[i]);
    }

    et.finish();
    et.print();
    //
    q.finish();
    //------------------------------------------------------------------------------------------------------------------
    //
    //    CropDispatcher Crop(device, context, program);
    //
    //
    //    CropRequest* request[num_crops];
    ////    et.add("Requests");
    //    for (int i=0; i < num_crops; i++)
    //    {
    //        request[i] = Crop(y_img, uv_img, y_crop_bufs[i], uv_crop_bufs[i], ROIs[i].x, ROIs[i].y, ROIs[i].width, ROIs[i].height, STRIDE_IN_W);
    ////    }
    ////
    //////    et.add("Syncs");
    ////    for (int i=0; i < num_crops; i++)
    ////    {
    //        request[i]->sync();
    //    }
    ////    et.finish();
    //    et.print();

    uint8_t *out_img;
    cv::Mat bgr_mat;
    char filename[20];
    for (int i = 0; i < num_crops; i++) {
        bgr_mat.release();
        out_img = (uint8_t *)malloc((ROIs[i].height * ROIs[i].width * 3) * sizeof(uint8_t));
        memcpy(out_img, y_crop_bufs[i], ROIs[i].width * ROIs[i].height * 3 * sizeof(uint8_t));
        bgr_mat = cv::Mat(ROIs[i].height, ROIs[i].width, CV_8UC3, (void *)out_img);
        sprintf(filename, "ROI_%d_%dx%d.png", i, ROIs[i].width, ROIs[i].height);
        cv::imwrite(filename, bgr_mat);
        free(out_img);
    }

    free(input_img);
    free(y_img);
    free(uv_img);
    for (uint32_t i = 0; i < num_crops; i++) {
        free(y_crop_bufs[i]);
        free(uv_crop_bufs[i]);
    }
    return 0;
}
