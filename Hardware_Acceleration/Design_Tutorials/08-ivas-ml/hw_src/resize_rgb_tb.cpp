/*
 * Copyright 2019 Xilinx, Inc.
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


#include "resize_rgb.h"

#include <ap_int.h>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unistd.h>

#define IN_W 2048
#define IN_H 1088
#define OUT_W 640
#define OUT_H 360

int main(int argc, char **argv)
{

    // Get the path to the input data file
    if (argc != 2) {
        std::cout << "ERROR: Please provide the path to the input source data file" << std::endl;
    }

    static unsigned char file_in[4147200];
    static unsigned char mat_out_dat[691200];

    std::ifstream test_data(argv[1], std::ifstream::binary);
    if (test_data) {
        test_data.seekg(0, test_data.end);
        unsigned int nb = test_data.tellg();
        //file_in = new unsigned char[nb];

        test_data.seekg(0, test_data.beg);
        test_data.read((char *)file_in, nb);
        test_data.close();
    }
    else {
        std::cout << "Unable to open file " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    static ap_uint<AXI_WIDTH> buf_in_y[IN_W * IN_H / 16];
    static ap_uint<AXI_WIDTH> buf_in_uv[IN_W * IN_H / 2 / 16];
    static ap_uint<AXI_WIDTH> buf_out[OUT_W * OUT_H * 3 / 16];

    int n = 0;
    for (int i = 0; i < 2048 * 1088 / 16; i++) {
        for (int j = 0; j < 16; j++) {
            buf_in_y[i](8 * j + 7, 8 * j) = file_in[n];
            n++;
        }
    }

    n = 2048 * 1088;

    for (int i = 0; i < 2048 * 1088 / 2 / 16; i++) {
        for (int j = 0; j < 16; j++) {
            buf_in_uv[i](8 * j + 7, 8 * j) = file_in[n];
            n++;
        }
    }


    resize_accel_rgb(buf_in_y,
                     buf_in_uv,
                     buf_out,
                     1920,
                     1080,
                     2048,
                     OUT_W,
                     OUT_H,
                     0,
                     128);


    n = 0;
    for (int i = 0; i < OUT_W * OUT_H * 3 / 16; i++) {
        for (int j = 0; j < 16; j++) {
            mat_out_dat[n] = buf_out[i](8 * j + 7, 8 * j);
            n++;
        }
    }

    cv::Mat mat_out(OUT_H, OUT_W, CV_8UC3, (void *)mat_out_dat);

    cv::imwrite("out.png", mat_out);

    return EXIT_SUCCESS;
}
