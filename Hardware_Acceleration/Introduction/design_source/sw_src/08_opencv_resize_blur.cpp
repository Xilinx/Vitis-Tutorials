/**********
Copyright (c) 2019, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/


#include "event_timer.hpp"

#include <chrono>
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/mman.h>

// Xilinx OCL
#include "xilinx_ocl_helper.hpp"

uint32_t nearest_resolution_div8(int32_t n)
{
    int32_t q = n / 8;

    int32_t n1 = 8 * q;
    int32_t n2 = 8 * (q + 1);

    if (abs(n - n1) < abs(n - n2)) {
        return (uint32_t)n1;
    }
    return (uint32_t)n2;
}


int main(int argc, char *argv[])
{
    EventTimer et;
    if (argc != 2) {
        std::cout << "Usage: 08_opencv_resize_blur <input image>" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "-- Example 8: OpenCV Image Resize and Blur --" << std::endl
              << std::endl;

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (!image.data) {
        std::cout << "ERROR: Unable to load image " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    // Bounds checking in the input image. The kernel in the hardware must have a width
    // evenly divisible by eight, and the resolution must be 4k or lower
    unsigned short in_height = image.rows;
    unsigned short in_width  = image.cols;
    if (image.cols % 8 != 0) {
        std::cout << "ERROR: Input image width must be divisible by 8, "
                  << image.cols << " is not." << std::endl;
        return EXIT_FAILURE;
    }
    if ((in_height > 2160) || (in_width > 3840)) {
        std::cout << "ERROR: Input image must be <= 4k (3840x2160), current image is "
                  << in_width << "x" << in_height << std::endl;
        return EXIT_FAILURE;
    }

    // Output images have the same restrictions as the input image
    uint32_t out_width  = image.cols / 3;
    uint32_t out_height = image.rows / 3;
    if (out_width % 8 != 0) {
        std::cout << "WARNING: Output image width must be divisible by 8, "
                  << out_width << " is not." << std::endl;
        out_width = nearest_resolution_div8(out_width);
        std::cout << "Adjusting to " << out_width << "x" << out_height << std::endl;
    }
    if ((out_height > 2160) || (out_width > 3840)) {
        std::cout << "ERROR: Output image must be <= 4k (3840x2160), current setting is "
                  << out_width << "x" << out_height << std::endl;
        return EXIT_FAILURE;
    }


    et.add("OpenCV resize and blur operation");

    cv::Mat resize_ocv, result_ocv;
    resize_ocv.create(cv::Size(out_width, out_height), image.type());
    result_ocv.create(cv::Size(out_width, out_height), image.type());
    cv::resize(image, resize_ocv, cv::Size(out_width, out_height), 0, 0, CV_INTER_LINEAR);
    cv::GaussianBlur(resize_ocv, result_ocv, cv::Size(7, 7), 3.0f, 3.0f, cv::BORDER_CONSTANT);
    et.finish();

    cv::imwrite("blur_out.png", result_ocv);
    cv::imwrite("blur_no_blur.png", resize_ocv);

    std::cout << "OpenCV conversion done! Image resized " << in_width
              << "x" << in_height << " to " << out_width << "x" << out_height
              << " and blurred 7x7!" << std::endl;

    std::cout << "Starting Xilinx OpenCL implementation..." << std::endl;

    std::cout << "Matrix has " << image.channels() << " channels" << std::endl;

    et.add("OpenCL initialization");
    xilinx::example_utils::XilinxOclHelper xocl;
    xocl.initialize("alveo_examples.xclbin");

    cl::CommandQueue q = xocl.get_command_queue();
    cl::Kernel krnl    = xocl.get_kernel("resize_blur_rgb");
    et.finish();
    et.add("OCL input buffer initialization");

    cl::Buffer imageToDevice = xocl.create_buffer(in_width * in_height * image.channels(),
                                                  (CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR));

    et.add("OCL output buffer initialization");
    cl::Buffer imageFromDevice = xocl.create_buffer(in_width * in_height * image.channels(),
                                                    (CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR));
    et.finish();

    // We set the kernel args before mapping the buffers to allow the runtime to
    // determine external DDR connectivity
    krnl.setArg(0, imageToDevice);
    krnl.setArg(1, imageFromDevice);
    krnl.setArg(2, image.cols);
    krnl.setArg(3, image.rows);
    krnl.setArg(4, out_width);
    krnl.setArg(5, out_height);
    krnl.setArg(6, 3.0f);

    et.add("Map output buffer and populate data");
    uint8_t *buf_to_device = (uint8_t *)q.enqueueMapBuffer(imageToDevice,
                                                           CL_TRUE,
                                                           CL_MAP_WRITE,
                                                           0,
                                                           image.rows * image.cols * image.channels());
    cv::Mat source_hw(image.rows, image.cols, image.type(), (void *)buf_to_device);
    image.copyTo(source_hw);
    q.enqueueUnmapMemObject(imageToDevice, buf_to_device);

    et.add("FPGA Kernel resize operation");

    cl::Event event_migrate_in, event_migrate_out, event_kernel;
    std::vector<cl::Event> events;
    q.enqueueMigrateMemObjects({imageFromDevice},
                               CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED,
                               NULL,
                               &event_migrate_out);
    events.push_back(event_migrate_out);
    q.enqueueMigrateMemObjects({imageToDevice}, 0, &events, &event_migrate_in); // From host
    events.push_back(event_migrate_in);

    // Launch the kernel
    q.enqueueTask(krnl, &events, &event_kernel);
    events.push_back(event_kernel);
    clWaitForEvents(events.size(), (const cl_event *)events.data());

    // Map the buffer back from the device (blocking)
    uint8_t *buf_from_device = (uint8_t *)q.enqueueMapBuffer(imageFromDevice,
                                                             CL_TRUE,
                                                             CL_MAP_READ,
                                                             0,
                                                             out_height * out_width * image.channels());

    cv::Mat result_hw(out_height, out_width, image.type(), (void *)buf_from_device);
    et.finish();

    et.print();

    // Write hardware output image
    cv::imwrite("hw_blur_out.png", result_hw);

    q.enqueueUnmapMemObject(imageFromDevice, buf_from_device);


    q.finish();

    return EXIT_SUCCESS;
}
