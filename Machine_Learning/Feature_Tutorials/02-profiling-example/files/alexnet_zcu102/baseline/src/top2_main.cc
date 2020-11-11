/*
## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.
*/

#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#include <dnndk/dnndk.h>
#include <opencv2/opencv.hpp>

// header file for Caffe input images APIs
#include "dputils.h"

using namespace cv;
using namespace std;
using namespace std::chrono;

int threadnum;

#define KERNEL_CONV "alexnetBNnoLRN_0"
#define CONV_INPUT_NODE "conv1"
#define CONV_OUTPUT_NODE "fc8"

const string baseImagePath = "./test_images/";

//#define SHOWTIME

#ifdef SHOWTIME
#define _T(func)                                                              \
    {                                                                         \
        auto _start = system_clock::now();                                    \
        func;                                                                 \
        auto _end = system_clock::now();                                      \
        auto duration = (duration_cast<microseconds>(_end - _start)).count(); \
        string tmp = #func;                                                   \
        tmp = tmp.substr(0, tmp.find('('));                                   \
        cout << "[TimeTest]" << left << setw(30) << tmp;                      \
        cout << left << setw(10) << duration << "us" << endl;                 \
    }
#else
#define _T(func) func;
#endif

/**
 * @brief put image names to a vector
 *
 * @param path - path of the image direcotry
 * @param images - the vector of image name
 *
 * @return none
 */
void ListImages(string const &path, vector<string> &images) {
    images.clear();
    struct dirent *entry;

    /*Check if path is a valid directory path. */
    struct stat s;
    lstat(path.c_str(), &s);
    if (!S_ISDIR(s.st_mode)) {
        fprintf(stderr, "Error: %s is not a valid directory!\n", path.c_str());
        exit(1);
    }

    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
        fprintf(stderr, "Error: Open %s path failed.\n", path.c_str());
        exit(1);
    }

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN) {
            string name = entry->d_name;
	    //cout << "DBG ListImages: " << name << endl;
            string ext = name.substr(name.find_last_of(".") + 1);
            if ((ext == "JPEG") || (ext == "jpeg") || (ext == "JPG") || (ext == "jpg") ||
                (ext == "PNG") || (ext == "png")) {
                images.push_back(name);
            }
        }
    }

    closedir(dir);
}

/**
 * @brief load kinds from file to a vector
 *
 * @param path - path of the kind file
 * @param kinds - the vector of kinds string
 *
 * @return none
 */
void LoadWords(string const &path, vector<string> &kinds) {
    kinds.clear();
    fstream fkinds(path);
    if (fkinds.fail()) {
        fprintf(stderr, "Error : Open %s failed.\n", path.c_str());
        exit(1);
    }
    string kind;
    while (getline(fkinds, kind)) {
        kinds.push_back(kind);
    }

    fkinds.close();
}

/**
 * @brief softmax operation
 *
 * @param data - pointer to input buffer
 * @param size - size of input buffer
 * @param result - calculation result
 *
 * @return none
 */
void CPUCalcSoftmax(const float *data, size_t size, float *result) {
    assert(data && result);
    double sum = 0.0f;

    for (size_t i = 0; i < size; i++) {
        result[i] = exp(data[i]);
        sum += result[i];
    }

    for (size_t i = 0; i < size; i++) {
        result[i] /= sum;
    }
}

/**H
 * @brief Get top k results according to its probability
 *
 * @param d - pointer to input data
 * @param size - size of input data
 * @param k - calculation result
 * @param vkinds - vector of kinds
 *
 * @return none
 */
void TopK(const float *d, int size, int k, vector<string> &vkind) {
    assert(d && size > 0 && k > 0);
    priority_queue<pair<float, int>> q;

    for (auto i = 0; i < size; ++i) {
        q.push(pair<float, int>(d[i], i));
    }

    for (auto i = 0; i < k; ++i)
    {
      pair<float, int> ki = q.top();
      printf("[Top]%d prob = %-8f  name = %s\n", i, d[ki.second], vkind[ki.second].c_str());
      q.pop();
    }
}


/**
 * @brief Run CONV Task for miniVggNet
 *
 * @param taskConv - pointer to miniVggNet CONV Task
 *
 * @return none
 */
// daniele.bagni@xilinx.com

vector<string> kinds, images;
void run_alexnetBNnoLRN(DPUTask *taskConv, Mat img) {
    assert(taskConv );

    float MEAN_VALUES[3] = {106.4051, 116.038956, 124.462036};
    
    // Get the output Tensor
    int8_t *outAddr = (int8_t *)dpuGetOutputTensorAddress(taskConv, CONV_OUTPUT_NODE);
    // Get size of the output Tensor
    int size = dpuGetOutputTensorSize(taskConv, CONV_OUTPUT_NODE);
    // Get channel count of the output Tensor
    int channel = dpuGetOutputTensorChannel(taskConv, CONV_OUTPUT_NODE);
    // Get scale of the output Tensor
    float out_scale = dpuGetOutputTensorScale(taskConv, CONV_OUTPUT_NODE);

    float *softmax = new float[channel];
    float *FCResult = new float[channel];
    //_T(dpuSetInputImage2(taskConv, CONV_INPUT_NODE, img));
    _T(dpuSetInputImage(taskConv, CONV_INPUT_NODE, img, &MEAN_VALUES[0]));


    _T(dpuRunTask(taskConv));
    _T(dpuRunSoftmax(outAddr, softmax, channel, size/channel, out_scale));
    //_T(dpuGetOutputTensorInHWCFP32(taskConv, CONV_OUTPUT_NODE, FCResult, channel));
    //_T(CPUCalcSoftmax(FCResult, channel, softmax));
    _T(TopK(softmax, channel, 2, kinds));

    delete[] softmax;
    delete[] FCResult;
}

//void classifyEntry(DPUKernel *kernelconv, DPUKernel *kernelfc) {
void classifyEntry(DPUKernel *kernelconv)
{

    ListImages(baseImagePath, images);
    if (images.size() == 0) {
        cerr << "\nError: Not images exist in " << baseImagePath << endl;
        return;
    } else {
        cout << "total image : " << images.size() << endl;
    }

    /* Load all kinds words.*/
    LoadWords(baseImagePath + "labels.txt", kinds);
    if (kinds.size() == 0) {
        cerr << "\nError: Not words exist in words.txt." << endl;
        return;
    }

    #define DPU_MODE_NORMAL 0
    #define DPU_MODE_PROF   1
    #define DPU_MODE_DUMP   2

    thread workers[threadnum];
    auto _start = system_clock::now();

    for (auto i = 0; i < threadnum; i++)
    {
    workers[i] = thread([&,i]()
    {
      // Create DPU Tasks from DPU Kernel
      DPUTask *taskconv = dpuCreateTask(kernelconv, DPU_MODE_NORMAL); // profiling not enabled
      //DPUTask *taskconv = dpuCreateTask(kernelconv, DPU_MODE_PROF); // profiling enabled
      //enable profiling
      //int res1 = dpuEnableTaskProfile(taskconv);
      //if (res1!=0) printf("ERROR IN ENABLING TASK PROFILING FOR CONV KERNEL\n");
      for(unsigned int ind = i  ;ind < images.size();ind+=threadnum)
      {
	Mat img = imread(baseImagePath + images.at(ind));
	cout << "DBG imread " << baseImagePath + images.at(ind) << endl;
	run_alexnetBNnoLRN(taskconv,img);
      }
      // Destroy DPU Tasks & free resources
      dpuDestroyTask(taskconv);
    });
    }

    // Release thread resources.
    for (auto &w : workers) {
        if (w.joinable()) w.join();
    }

    auto _end = system_clock::now();
    auto duration = (duration_cast<microseconds>(_end - _start)).count();
    cout << "[Time]" << duration << "us" << endl;
    cout << "[FPS]" << images.size()*1000000.0/duration  << endl;
}

/**
 * @brief Entry for running miniVggNet neural network
 *
 */
int main(int argc ,char** argv)
{

  if(argc == 2) {
    threadnum = stoi(argv[1]);
    cout << "now running " << argv[0] << " " << argv[1] << endl;
  }
  else
    cout << "now running " << argv[0] << endl;

  DPUKernel *kernelConv;

  dpuOpen();
  kernelConv = dpuLoadKernel(KERNEL_CONV);
  classifyEntry(kernelConv);
  dpuDestroyKernel(kernelConv);
  dpuClose();

  return 0;
}
