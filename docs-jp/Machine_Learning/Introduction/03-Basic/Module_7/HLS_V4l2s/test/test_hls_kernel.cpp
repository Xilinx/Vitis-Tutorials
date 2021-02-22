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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <random>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "xcl2.hpp"

#define IN_HEIGHT 2160
#define IN_WIDTH 3840
#define OUT_HEIGHT 1080
#define OUT_WIDTH 1920
#define OUT_RESIZE_HEIGHT 360
#define OUT_RESIZE_WIDTH 640

ssize_t read_file_to_buf(const char *file, uint8_t **buf, size_t *len)
{
    ssize_t ret;
    FILE *fp;

    fp = fopen(file, "r");
    if (fp == NULL)
    {
        return -1;
    }

    ret = fseek(fp, 0, SEEK_END);
    if (ret < 0)
    {
        return -1;
    }

    *len = (size_t)ftell(fp);
    if ((*len) < 0)
    {
        return -1;
    }

    rewind(fp);

    *buf = (uint8_t *)malloc(sizeof(uint8_t) * (*len));
    if (*buf == NULL)
    {
        return -1;
    }

    fread(*buf, (*len), sizeof(char), fp);
    fclose(fp);

    return 0;
}

ssize_t write_buf_to_file(const uint8_t *buf, const size_t len,
                          const char *file)
{
    size_t ret;
    FILE *fp;

    fp = fopen(file, "w");
    if (fp == NULL)
    {
        return -1;
    }

    ret = fwrite(buf, len, sizeof(char), fp);
    if (ret < 0)
    {
        return -1;
    }

    fclose(fp);

    return 0;
}

int main(int argc, char **argv)
{
    uint8_t *input_img;
    unsigned int resize_size = (OUT_RESIZE_WIDTH * OUT_RESIZE_HEIGHT * 3);
    unsigned int full_size = (OUT_WIDTH * OUT_HEIGHT * 3);
    size_t yuv_size;
    read_file_to_buf("test.yuv", &input_img, &yuv_size);
    uint8_t *out_buf_0 = (uint8_t *)malloc(resize_size);
    uint8_t *out_buf_1 = (uint8_t *)malloc(full_size);
    // Initialize OpenCL instance
    xcl::XilinxOcl xocl;
    if (std::getenv("DPU_XCLBIN_PATH"))
    {
        xocl.initialize(std::getenv("DPU_XCLBIN_PATH"));
    }
    else
    {
        xocl.initialize("/usr/lib/dpu.xclbin");
    }

    cl::CommandQueue q = xocl.get_command_queue();
    cl::Kernel krnl = xocl.get_kernel("pre_processor");
    auto imgToDevice = xocl.create_buffer(yuv_size, CL_MEM_READ_ONLY);
    auto resizeFromDevice = xocl.create_buffer(resize_size, CL_MEM_WRITE_ONLY);
    auto fullFromDevice = xocl.create_buffer(full_size, CL_MEM_WRITE_ONLY);

krnl.setArg(0, imgToDevice);
krnl.setArg(1, resizeFromDevice);
krnl.setArg(2,  fullFromDevice);
krnl.setArg(3,  IN_WIDTH);
krnl.setArg(4,  IN_HEIGHT);
krnl.setArg(5,  OUT_RESIZE_WIDTH);
krnl.setArg(6,  OUT_RESIZE_HEIGHT);

cl::Event event_sp;


printf("Enqueue Stage\n");
q.enqueueWriteBuffer(imgToDevice, CL_TRUE, 0, yuv_size, input_img);

printf("Task Stage\n");
q.enqueueTask(krnl, NULL, &event_sp);
printf("Wait Stage\n");
clWaitForEvents(1, (const cl_event *)&event_sp);

cl_ulong start = 0;
cl_ulong end = 0;
double diff_prof = 0.0f;
// event_sp.getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
// event_sp.getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
// diff_prof = end - start;
std::cout << "Latency: " << (diff_prof / 1000000) << "ms" << std::endl;

q.enqueueReadBuffer(resizeFromDevice, CL_TRUE, 0, resize_size, out_buf_0);
q.enqueueReadBuffer(fullFromDevice, CL_TRUE, 0, full_size, out_buf_1);

q.finish();

std::cout << "Done with CL session" << std::endl;

//    write_buf_to_file(out_buf_0, resize_size, "resize.bgr");

cv::Mat roi_mat0(OUT_RESIZE_HEIGHT, OUT_RESIZE_WIDTH, CV_8UC3, out_buf_0);
cv::Mat roi_mat1(OUT_HEIGHT, OUT_WIDTH, CV_8UC3, out_buf_1);

cv::imwrite("scaled.png", roi_mat0);
cv::imwrite("full.png", roi_mat1);

free(out_buf_0);
free(out_buf_1);
free(input_img);

printf("DONE\n");

return 0;
}
