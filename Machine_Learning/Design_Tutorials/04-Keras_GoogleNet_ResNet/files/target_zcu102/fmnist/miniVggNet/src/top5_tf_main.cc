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
#include <sstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <mutex>
#include <string>
#include <vector>
#include <thread>


// header file OpenCV for image processing
#include <opencv2/opencv.hpp>

// header files for DNNDK APIs
#include <dnndk/dnndk.h>

//// header file for Caffe input images APIs
//#include "dputils.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

int threadnum;

#define KERNEL_CONV "miniVggNet_0"

#define CONV_INPUT_NODE "conv2d_1_convolution"
#define CONV_OUTPUT_NODE "dense_2_MatMul"

const string baseImagePath = "./test/";


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


/*List all images's name in path.*/
void ListImages(std::string const &path, std::vector<std::string> &images) {
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
      std::string name = entry->d_name;
      std::string ext = name.substr(name.find_last_of(".") + 1);
      if ((ext == "JPEG") || (ext == "jpeg") || (ext == "JPG") || (ext == "jpg") ||
          (ext == "bmp") ||  (ext == "BMP") || (ext == "PNG") || (ext == "png")) {
        images.push_back(name);
      }
    }
  }

  closedir(dir);
}

/*Load all kinds*/
void LoadWords(std::string const &path, std::vector<std::string> &kinds) {
  kinds.clear();
  std::fstream fkinds(path);
  if (fkinds.fail()) {
    fprintf(stderr, "Error : Open %s failed.\n", path.c_str());
    exit(1);
  }
  std::string kind;
  while (getline(fkinds, kind)) {
    kinds.push_back(kind);
  }

  fkinds.close();
}

/* Calculate softmax.*/
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

/* Get top k results and show them based on kinds. */
void TopK(const float *d, int size, int k, std::vector<std::string> &vkind) {
  assert(d && size > 0 && k > 0);
  std::priority_queue<std::pair<float, int>> q;

  for (auto i = 0; i < size; ++i) {
    q.push(std::pair<float, int>(d[i], i));
  }

  for (auto i = 0; i < k; ++i)
    {
      std::pair<float, int> ki = q.top();
      int real_ki = ki.second;
      fprintf(stdout, "top[%d] prob = %-8f  name = %s\n", i, d[ki.second], vkind[real_ki].c_str());
      q.pop();
  }
}

void central_crop(const Mat& image, int height, int width, Mat& img) {
  int offset_h = (image.rows - height)/2;
  int offset_w = (image.cols - width)/2;
  Rect box(offset_w, offset_h, width, height);
  img = image(box);
}

//void change_bgr(const Mat& image, int8_t* data, float scale, float* mean) {
//  for(int i = 0; i < 3; ++i)
//    for(int j = 0; j < image.rows; ++j)
//      for(int k = 0; k < image.cols; ++k) {
//		    data[j*image.rows*3+k*3+2-i] = (image.at<Vec3b>(j,k)[i] - (int8_t)mean[i]) * scale;
//      }
//}

void normalize_image(const Mat& image, int8_t* data, float scale, float* mean) {
  for(int i = 0; i < 3; ++i) {
    for(int j = 0; j < image.rows; ++j) {
      for(int k = 0; k < image.cols; ++k) {
	//data[j*image.rows*3+k*3+2-i] = (float(image.at<Vec3b>(j,k)[i])/255.0 ) * scale; //DB: original code
	data[j*image.cols*3+k*3+i] = (float(image.at<Vec3b>(j,k)[i])/255.0 - 0.5)*2 * scale;
      }
     }
   }
}


inline void set_input_image(DPUTask *task, const string& input_node, const cv::Mat& image, float* mean)
{
  //Mat cropped_img;
  DPUTensor* dpu_in = dpuGetInputTensor(task, input_node.c_str());
  float scale = dpuGetTensorScale(dpu_in);
  int width = dpuGetTensorWidth(dpu_in);
  int height = dpuGetTensorHeight(dpu_in);
  int size = dpuGetTensorSize(dpu_in);
  int8_t* data = dpuGetTensorAddress(dpu_in);

  //cout << "SET INPUT IMAGE: scale = " << scale  << endl;
  //cout << "SET INPUT IMAGE: width = " << width  << endl;
  //cout << "SET INPUT IMAGE: height= " << height << endl;
  //cout << "SET INPUT IMAGE: size  = " << size   << endl;

  normalize_image(image, data, scale, mean);
}


vector<string> kinds, images; //DB


void run_CNN(DPUTask *taskConv, Mat img)
{
  assert(taskConv);

  // Get the output Tensor
  int8_t *outAddr = (int8_t *)dpuGetOutputTensorAddress(taskConv, CONV_OUTPUT_NODE);
  // Get size of the output Tensor
  int size = dpuGetOutputTensorSize(taskConv, CONV_OUTPUT_NODE);
  // Get channel count of the output Tensor
  int channel = dpuGetOutputTensorChannel(taskConv, CONV_OUTPUT_NODE);
  // Get scale of the output Tensor
  float out_scale = dpuGetOutputTensorScale(taskConv, CONV_OUTPUT_NODE);

  float *softmax = new float[size];

  // Set image into Conv Task with mean value
  float mean[3] = {0.0f, 0.0f, 0.0f};
  set_input_image(taskConv, CONV_INPUT_NODE, img, mean);

  //cout << "\nRun CNN ..." << endl;
  _T(dpuRunTask(taskConv));

  // Calculate softmax on CPU and show TOP5 classification result
  _T(dpuRunSoftmax(outAddr, softmax, channel, size/channel, out_scale));
  TopK(softmax, channel, 5, kinds);

  //// Show the image
  //cv::imshow("Image", image);
  //cv::waitKey(1);

  delete[] softmax;

}


/**
 * @brief Run DPU CONV Task for Keras Net
 *
 * @param taskConv - pointer to CONV Task
 *
 * @return none
 */
void classifyEntry(DPUKernel *kernelConv)
{

  //  vector<string> kinds, images;

  /*Load all image names */
  ListImages(baseImagePath, images);
  if (images.size() == 0) {
    cerr << "\nError: Not images exist in " << baseImagePath << endl;
    return;
  } else {
    cout << "total image : " << images.size() << endl;
  }

  /*Load all kinds words.*/
  LoadWords(baseImagePath + "labels.txt", kinds);
  if (kinds.size() == 0) {
    cerr << "\nError: Not words exist in labels.txt." << endl;
    return;
  }

  /* ************************************************************************************** */
  //DB added multi-threding code

#define DPU_MODE_NORMAL 0
#define DPU_MODE_PROF   1
#define DPU_MODE_DUMP   2

  thread workers[threadnum];
  auto _start = system_clock::now();

  for (auto i = 0; i < threadnum; i++)
  {
  workers[i] = thread([&,i]()
  {

    /* Create DPU Tasks for CONV  */
    DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_NORMAL); // profiling not enabled
    //DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_PROF); // profiling enabled
    //enable profiling
    //int res1 = dpuEnableTaskProfile(taskConv);
    //if (res1!=0) printf("ERROR IN ENABLING TASK PROFILING FOR CONV KERNEL\n");

    for(unsigned int ind = i  ;ind < images.size();ind+=threadnum)
      {

	cout << "\nLoad image : " << images.at(ind) << endl;
	Mat img = imread(baseImagePath + images.at(ind));
	//cout << "DBG imread " << baseImagePath + images.at(ind) << endl;
	//Size sz(32,32);
	//Mat img2; resize(img, img2, sz); //DB
	run_CNN(taskConv, img);
      }
    // Destroy DPU Tasks & free resources
    dpuDestroyTask(taskConv);
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
 * @return 0 on success, or error message dispalyed in case of failure.
 */
int main(int argc, char *argv[])
{

  DPUKernel *kernelConv;

  if(argc == 2) {
    threadnum = stoi(argv[1]);
    cout << "now running " << argv[0] << " " << argv[1] << endl;
  }
  else
      cout << "now running " << argv[0] << endl;


  /* Attach to DPU driver and prepare for running */
  dpuOpen();

  /* Create DPU Kernel for MNIST */
  kernelConv = dpuLoadKernel(KERNEL_CONV); //DB

  /* run CIFAR10 Classification  */
  classifyEntry(kernelConv);

  /* Destroy DPU Kernel  */
  dpuDestroyKernel(kernelConv);

  /* Dettach from DPU driver & release resources */
  dpuClose();

  return 0;
}
