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

#include "bn_arrays.hpp"
#include "dputils.h"
#include "event_timer.hpp"

#include <algorithm>
#include <dnndk/dnndk.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

// DPU input & output Node name for sort_reID
#define NODE_INPUT "conv1"
#define NODE_OUTPUT "ave_pool1"

EventTimer et;

float calc_norm(std::vector<float> &vals)
{
    float norm = 0.0;
    for (int i = 0; i < vals.size(); i++) {
        norm += vals[i] * vals[i];
    }
    return sqrtf32(norm);
}

void run_sort_reid(DPUTask *task, cv::Mat &image, std::vector<float> &result)
{
    DPUTensor *conv_in_tensor = dpuGetInputTensor(task, NODE_INPUT);
    int inHeight              = dpuGetTensorHeight(conv_in_tensor);
    int inWidth               = dpuGetTensorWidth(conv_in_tensor);
    int inCH                  = dpuGetTensorChannel(conv_in_tensor);
    float scaleFix            = dpuGetTensorScale(conv_in_tensor);
    int8_t *inputAddr         = dpuGetTensorAddress(conv_in_tensor);
    int value;

    cv::Mat img;
    et.add("Set input tensor and preprocess");
    cv::resize(image, img, cv::Size(80, 80));

    float scale   = 0.017429;
    float mean[3] = {103.0, 116.0, 123.0}; //bgr

    for (int idx_h = 0; idx_h < img.rows; idx_h++) {
        for (int idx_w = 0; idx_w < img.cols; idx_w++) {
            for (int idx_c = 0; idx_c < 3; idx_c++) {
                value     = (int)((img.at<cv::Vec3b>(idx_h, idx_w)[idx_c] - mean[idx_c]) * scaleFix * scale);
                int idx_r = idx_c;
                if (idx_c == 0) {
                    idx_r = 2;
                }
                else if (idx_c == 2) {
                    idx_r = 0;
                }

                inputAddr[idx_h * img.cols * 3 + idx_w * 3 + idx_r] = (char)value;
            }
        }
    }
    et.finish();
    //cv::imwrite("scaled_image.png", img);

    //et.add("DPU Set input image");
    //dpuSetInputImage2(task, NODE_INPUT, img);
    et.add("DPU run task");
    dpuRunTask(task);
    et.add("Get output tensors");

    // Get output data and postprocess
    et.add("Postprocess");
    DPUTensor *conv_out_tensor = dpuGetOutputTensor(task, NODE_OUTPUT);
    int outSize                = dpuGetTensorSize(conv_out_tensor);
    int8_t *outAddr            = (int8_t *)dpuGetTensorAddress(conv_out_tensor);
    int outCH                  = dpuGetTensorChannel(conv_out_tensor);
    float out_scale            = dpuGetTensorScale(conv_out_tensor);

    for (int i = 0; i < outSize; i++) {
        float val = outAddr[i] * out_scale;
        result[i] = (val - bn_mean[i]) / sqrtf32(bn_var[i] + 0.00001) * bn_weight[i];
    }

    float norm = calc_norm(result);

    for (int i = 0; i < result.size(); i++) {
        result[i] = result[i] / norm;
    }

    et.finish();
}

template <class Iter_T, class Iter2_T>
float euclidian(Iter_T first, Iter_T last, Iter2_T first2)
{
    float dist = 0.0;
    while (first != last) {
        float d = (*first++) - (*first2++);
        dist += d * d;
    }
    return dist > 0.0 ? sqrt(dist) : 0.0;
}

void face_reid(DPUKernel *kernel)
{
    std::cout << "--== sort_reID Test Application ==--"
              << std::endl
              << std::endl;

    const char *home;
    if ((home = getenv("HOME")) == NULL) {
        home = getpwuid(getuid())->pw_dir;
    }

    std::string homedir(home);

    std::string img_root      = homedir + "/test_data/reid/";
    std::string image_names[] = {"1.jpg",
                                 "2.jpg",
                                 "3.jpg",
                                 "4.jpg",
                                 "5.jpg",
                                 "6.jpg",
                                 "7.jpg"};

    std::vector<std::string> images(image_names, image_names + sizeof(image_names) / sizeof(std::string));
    std::vector<std::vector<float>> results(7);
    for (int j = 0; j < 7; j++) {
        results[j].resize(128);
    }

    et.add("DPU create task");
    DPUTask *task = dpuCreateTask(kernel, 0);

    cv::Mat image;
    int i = 0;
    for (std::string image_name : images) {
        et.add("Load image " + image_name);
        std::string filename = img_root + image_name;


        image = cv::imread(filename);
        if (image.data == NULL) {
            std::cout << "ERROR: Unable to read " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        et.finish();


        run_sort_reid(task, image, results[i]);
        i++;
    }

    float distances[6];
    for (int j = 1; j < 7; j++) {
        distances[j - 1] = euclidian(results[0].begin(), results[0].end(), results[j].begin());
    }

    std::cout << "Resulting distance matrix: " << std::endl;
    std::cout << "    [ ";
    for (int j = 0; j < 6; j++) {
        std::cout << distances[j];
        if (j != 5) {
            std::cout << ", ";
        }
        else {
            std::cout << " ]"
                      << std::endl
                      << std::endl;
        }
    }

    // Now that we have all of the results, calculate the distance matrix

    et.print();
    exit(0);

    cv::imwrite("out.png", image);
}


int main(int argc, char **argv)
{
    // Attach to DPU runtime and prepare for running
    dpuOpen();

    // Load DPU Kernel for DenseBox neural network
    DPUKernel *kernel = dpuLoadKernel("reid");

    // Doing face detection.
    face_reid(kernel);

    // Destroy DPU Kernel & free resources
    dpuDestroyKernel(kernel);

    // Dettach from DPU driver & release resources
    dpuClose();

    et.print();

    return EXIT_SUCCESS;
}
