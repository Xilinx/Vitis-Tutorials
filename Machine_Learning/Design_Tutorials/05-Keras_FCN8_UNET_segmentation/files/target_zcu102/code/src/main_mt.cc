/*
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Author: Daniele Bagni, Xilinx Inc.
// date: 13 May 2021

#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <thread>

#include "common.h"
/* header file OpenCV for image processing */
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace std::chrono;

GraphInfo shapes;

//const string baseImagePath = "./src/img_test/";
string baseImagePath;  // they will get their values via argv[]

int num_threads   = 0;
int is_running_0  = 1;
int num_of_images = 0;
int num_images_x_thread = 0;

unsigned long long int max_addr_imags;
unsigned long long int max_addr_FCres;

uint8_t colorB[] = {128, 232, 70, 156, 153, 153, 30,  0,   35, 152,
                    180, 60,  0,  142, 70,  100, 100, 230, 32};
uint8_t colorG[] = {64,  35, 70, 102, 153, 153, 170, 220, 142, 251,
                    130, 20, 0,  0,   0,   60,  80,  0,   11};
uint8_t colorR[] = {128, 244, 70,  102, 190, 153, 250, 220, 107, 152,
                    70,  220, 255, 0,   0,   0,   0,   0,   119};


/**
 * @brief put image names to a vector
 *
 * @param path - path of the image direcotry
 * @param images_list - the vector of image name
 *
 * @return none
 */
void ListImages(string const& path, vector<string>& images_list) {
  images_list.clear();
  struct dirent* entry;

  /*Check if path is a valid directory path. */
  struct stat s;
  lstat(path.c_str(), &s);
  if (!S_ISDIR(s.st_mode)) {
    fprintf(stderr, "Error: %s is not a valid directory!\n", path.c_str());
    exit(1);
  }

  DIR* dir = opendir(path.c_str());
  if (dir == nullptr) {
    fprintf(stderr, "Error: Open %s path failed.\n", path.c_str());
    exit(1);
  }

  while ((entry = readdir(dir)) != nullptr) {
    if (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN) {
      string name = entry->d_name;
      string ext = name.substr(name.find_last_of(".") + 1);
      if ((ext == "JPEG") || (ext == "jpeg") || (ext == "JPG") ||
          (ext == "jpg") || (ext == "PNG") || (ext == "png")) {
        images_list.push_back(name);
      }
    }
  }

  closedir(dir);
}


/**
 * @brief Run DPU Task for CNN
 *
 * @param taskFCN8 - pointer to FCN8 Task
 *
 * @return none
 */
void runCNN(vart::Runner* runner, float *imageInputs, float *FCResult)
{

  // get in/out tensors and dims
  auto outputTensors = runner->get_output_tensors();
  auto inputTensors = runner->get_input_tensors();
  auto out_dims = outputTensors[0]->get_dims();
  auto in_dims = inputTensors[0]->get_dims();
  // get shape info
  int outSize   = shapes.outTensorList[0].size;
  int outHeight = shapes.outTensorList[0].height;
  int outWidth  = shapes.outTensorList[0].width;
  int inSize    = shapes.inTensorList[0].size;
  int inHeight  = shapes.inTensorList[0].height;
  int inWidth   = shapes.inTensorList[0].width;
  int batchSize = in_dims[0];
  int num_of_classes = outSize/(inSize/3);

  std::vector<std::unique_ptr<vart::TensorBuffer>> inputs, outputs;
  std::vector<vart::TensorBuffer*> inputsPtr, outputsPtr;
  std::vector<std::shared_ptr<xir::Tensor>> batchTensors;

  float *loc_imageInputs = imageInputs;
  float *loc_FCResult    = FCResult;

  // debug messages
  cout << "outSize  " << outSize   << endl;
  cout << " inSize  " <<  inSize   << endl;
  cout << "outW     " << outWidth  << endl;
  cout << "outH     " << outHeight << endl;
  cout << "inpW     " <<  inWidth  << endl;
  cout << "inpH     " <<  inHeight << endl;
  cout << "# class  " <<  num_of_classes  << endl;
  cout << "batchSize " << batchSize << endl; // alway 1 for Edge

  for (unsigned int n = 0; n < num_images_x_thread; n += batchSize)
  {
      unsigned int i = 0;
      loc_imageInputs = imageInputs+(n + i) * inSize;
      loc_FCResult    = FCResult+   (n + i) * outSize;
      //cout << "DPU processing image #" << n  << endl;

      // in/out tensor refactory for batch inout/output
      batchTensors.push_back(std::shared_ptr<xir::Tensor>(xir::Tensor::create(inputTensors[0]->get_name(),
          in_dims, xir::DataType{xir::DataType::FLOAT, sizeof(float) * 8u})));
      inputs.push_back(std::make_unique<CpuFlatTensorBuffer>(loc_imageInputs, batchTensors.back().get()));
      batchTensors.push_back(std::shared_ptr<xir::Tensor>(xir::Tensor::create(outputTensors[0]->get_name(),
          out_dims,xir::DataType{xir::DataType::FLOAT, sizeof(float) * 8u})));
      outputs.push_back(std::make_unique<CpuFlatTensorBuffer>( loc_FCResult, batchTensors.back().get()));

      //tensor buffer input/output
      inputsPtr.clear();
      outputsPtr.clear();
      inputsPtr.push_back(inputs[0].get());
      outputsPtr.push_back(outputs[0].get());

      //run
      auto job_id = runner->execute_async(inputsPtr, outputsPtr);
      runner->wait(job_id.first, -1);

      inputs.clear();
      outputs.clear();
  }
}



/**
 * @brief Entry for running FCN8 neural network
 *
 * @note Runner APIs prefixed with "dpu" are used to easily program &
 *       deploy FCN8 on DPU platform.
 *
 */
int main(int argc, char* argv[])
{
  //using std::chrono::system_clock;
  system_clock::time_point t_start, t_end;

  // Check args
  if (argc != 5) {
    cout << "Usage: run_cnn xmodel_path test_images_path thread_num (from 1 to 6) use_post_proc(1:yes, 0:no)" << endl;
    return -1;
  }
  baseImagePath = std::string(argv[2]); //path name of the folder with test images
  num_threads = stoi(argv[3]);
  assert( (num_threads<=6)&(num_threads>=1) );
  int use_post_processing = stoi(argv[4]);

  /////////////////////////////////////////////////////////////////////////////////////////////
  // PREPARE DPU STUFF

  auto graph = xir::Graph::deserialize(argv[1]);
  auto subgraph = get_dpu_subgraph(graph.get());
  CHECK_EQ(subgraph.size(), 1u)
      << "CNN should have one and only one dpu subgraph.";
  LOG(INFO) << "create running for subgraph: " << subgraph[0]->get_name();

  //create runners
  auto runner  = vart::Runner::create_runner(subgraph[0], "run");
  auto runner1 = vart::Runner::create_runner(subgraph[0], "run");
  auto runner2 = vart::Runner::create_runner(subgraph[0], "run");
  auto runner3 = vart::Runner::create_runner(subgraph[0], "run");
  auto runner4 = vart::Runner::create_runner(subgraph[0], "run");
  auto runner5 = vart::Runner::create_runner(subgraph[0], "run");

  // get in/out tensors and dims
  auto inputTensors = runner->get_input_tensors();
  auto outputTensors = runner->get_output_tensors();
  auto out_dims = outputTensors[0]->get_shape();
  auto in_dims = inputTensors[0]->get_shape();

  // get in/out tensor shape
  int inputCnt = inputTensors.size();
  int outputCnt = outputTensors.size();
  TensorShape inshapes[inputCnt];
  TensorShape outshapes[outputCnt];
  shapes.inTensorList = inshapes;
  shapes.outTensorList = outshapes;
  getTensorShape(runner.get(), &shapes, inputCnt, outputCnt);

  // get shape info
  int outSize   = shapes.outTensorList[0].size;
  int outHeight = shapes.outTensorList[0].height;
  int outWidth  = shapes.outTensorList[0].width;
  int inSize    = shapes.inTensorList[0].size;
  int inHeight  = shapes.inTensorList[0].height;
  int inWidth   = shapes.inTensorList[0].width;
  int batchSize = in_dims[0];
  int num_of_classes = outSize/(inSize/3);

  // debug messages
  cout << "outSize  " << outSize   << endl;
  cout << " inSize  " <<  inSize   << endl;
  cout << "outW     " << outWidth  << endl;
  cout << "outH     " << outHeight << endl;
  cout << "inpW     " <<  inWidth  << endl;
  cout << "inpH     " <<  inHeight << endl;
  cout << "# class  " <<  num_of_classes  << endl;
  cout << "batchSize " << batchSize << endl; // alway 1 for Edge


  /////////////////////////////////////////////////////////////////////////////////////////////
  // PREPROCESSING ALL IMAGES

  // Load all image filenames
  vector<string>  image_filename;
  ListImages(baseImagePath, image_filename);
  if (image_filename.size() == 0) {
    cerr << "\nError: No images existing under " << baseImagePath << endl;
    exit(-1);
  }
  else {
    num_of_images = image_filename.size();
    cout << "\n max num of images to read " << num_of_images << endl;
  }

  //number of images per thread
  num_images_x_thread = num_of_images/num_threads;
  num_images_x_thread = (num_images_x_thread/batchSize)*batchSize;
  cout << "\n number of images per thread: " << num_images_x_thread << endl;
  //effective number of images as multiple of num_threads and batchSize
  num_of_images = num_images_x_thread * num_threads;

  vector<Mat> imageList;
  float* imageInputs = new float[(num_of_images)*inSize];
  float* FCResult    = new float[(num_of_images)*outSize];
  //float* softmax     = new float[outSize];

  // preprocess all images at once
  for (unsigned int n = 0; n < num_of_images; n++)
  {
      Mat image = imread(baseImagePath + image_filename[n]);
      Mat image2 = cv::Mat(inHeight, inWidth, CV_8SC3);
      resize(image, image2, Size(inHeight, inWidth), 0, 0, INTER_LINEAR); //0);
      for (int y = 0; y < inHeight; y++) {
        for (int x = 0; x < inWidth; x++) {
          for (int c = 0; c < 3; c++) {
            //imageInputs[n*inSize + 3*(y*inWidth+x) + 2-c] = ((float)image2.at<Vec3b>(y,x)[c])/127.5 -1.0; //RGB conversion
            imageInputs[n*inSize + 3*(y*inWidth+x) + c] = ((float)image2.at<Vec3b>(y,x)[c])/127.5 -1.0;     //BGR format
          }
        }
      }
      imageList.push_back(image2);
  }

    // split images in chunks, each chunks for its own thead
    // avoid pointing to wrong memory locations
    float *imagesInput0 = imageInputs+ inSize*(num_threads==1 ? 0*num_images_x_thread : 0);
    float *imagesInput1 = imageInputs+ inSize*(num_threads==2 ? 1*num_images_x_thread : 0);
    float *imagesInput2 = imageInputs+ inSize*(num_threads==3 ? 2*num_images_x_thread : 0);
    float *imagesInput3 = imageInputs+ inSize*(num_threads==4 ? 3*num_images_x_thread : 0);
    float *imagesInput4 = imageInputs+ inSize*(num_threads==5 ? 4*num_images_x_thread : 0);
    float *imagesInput5 = imageInputs+ inSize*(num_threads==6 ? 5*num_images_x_thread : 0);

    float *FCResult0    = FCResult+   outSize*(num_threads==1 ? 0*num_images_x_thread : 0);
    float *FCResult1    = FCResult+   outSize*(num_threads==2 ? 1*num_images_x_thread : 0);
    float *FCResult2    = FCResult+   outSize*(num_threads==3 ? 2*num_images_x_thread : 0);
    float *FCResult3    = FCResult+   outSize*(num_threads==4 ? 3*num_images_x_thread : 0);
    float *FCResult4    = FCResult+   outSize*(num_threads==5 ? 4*num_images_x_thread : 0);
    float *FCResult5    = FCResult+   outSize*(num_threads==6 ? 5*num_images_x_thread : 0);
    /*
    fprintf(stderr, "imagesInput0         = %llu\n", (long long unsigned int) imagesInput0);
    fprintf(stderr, "imagesInput1         = %llu\n", (long long unsigned int) imagesInput1);
    fprintf(stderr, "imagesInput2         = %llu\n", (long long unsigned int) imagesInput2);
    fprintf(stderr, "imagesInput3         = %llu\n", (long long unsigned int) imagesInput3);
    fprintf(stderr, "FCResult0            = %llu\n", (long long unsigned int) FCResult0);
    fprintf(stderr, "FCResult1            = %llu\n", (long long unsigned int) FCResult1);
    fprintf(stderr, "FCResult2            = %llu\n", (long long unsigned int) FCResult2);
    fprintf(stderr, "FCResult3            = %llu\n", (long long unsigned int) FCResult3);
    max_addr_imags = ((unsigned long long int) imageInputs) + num_of_images *  inSize;
    max_addr_FCres = ((unsigned long long int) FCResult   ) + num_of_images * outSize;
    fprintf(stderr, "max imags            = %llu\n", max_addr_imags);
    fprintf(stderr, "max FCRes            = %llu\n", max_addr_FCres);
    */

    /////////////////////////////////////////////////////////////////////////////////////////////
    // MULTITHREADING DPU EXECUTION WITH BATCH
    thread workers[num_threads];

    t_start = system_clock::now();

    for (auto i = 0; i < num_threads; i++)
    {
        if (i == 0) workers[i] = thread(runCNN, runner.get(),   ref(imagesInput0), ref(FCResult0) );
        if (i == 1) workers[i] = thread(runCNN, runner1.get(),  ref(imagesInput1), ref(FCResult1) );
        if (i == 2) workers[i] = thread(runCNN, runner2.get(),  ref(imagesInput2), ref(FCResult2) );
        if (i == 3) workers[i] = thread(runCNN, runner3.get(),  ref(imagesInput3), ref(FCResult3) );
        if (i == 4) workers[i] = thread(runCNN, runner4.get(),  ref(imagesInput4), ref(FCResult4) );
        if (i == 5) workers[i] = thread(runCNN, runner5.get(),  ref(imagesInput5), ref(FCResult5) );
    }
    // Release thread resources.
    for (auto &w : workers) {
      if (w.joinable()) w.join();
    }

    t_end = system_clock::now();
    auto duration = (duration_cast<microseconds>(t_end - t_start)).count();
    cout << "\n" << endl;
    cout << "[Time] " << duration << "us" << endl;
    cout << "[FPS ] "  << num_of_images*1000000.0/duration  << endl;
    cout << "\n" << endl;

    /////////////////////////////////////////////////////////////////////////////////////////////
    // POSTPROCESSING ALL THE IMAGES
    if (use_post_processing==1)
    {
      for (unsigned int n = 0; n < num_of_images; n ++)
      {
          cout << "\nImage : " << image_filename[n] << endl;
          Mat image3 = imageList[n].clone();

          // Calculate softmax on CPU and display TOP-5 classification results
          //CPUCalcSoftmax(&FCResult[n * outSize], outSize, softmax);
          //TopK(softmax, outSize, 5, kinds);
          //Display the image
          //cv::imshow("Segmentation", imageList[n]);
          //cv::waitKey(1000);
          //save the image
          //cv::imwrite(filename[n], imageList[n]);

  	      /* post processing starts here and should be out of the fps performance countes */ //DB:04Mar2021
  	      Mat segMat(outHeight, outWidth, CV_8UC3);
  	      Mat showMat(inHeight,  inWidth, CV_8UC3);
          float *OutData = &FCResult[n * outSize];
  	      for (int row = 0; row < outHeight; row++) {
            for (int col = 0; col < outWidth; col++) {
              int ii = row * outWidth * num_of_classes + col * num_of_classes;
              auto max_ind = max_element(OutData + ii, OutData + ii + num_of_classes);
              int posit = distance(OutData + ii, max_ind);
              segMat.at<Vec3b>(row, col) = Vec3b(colorB[posit], colorG[posit], colorR[posit]);
            }
          }
    	    // resize to original scale and overlay for displaying
    	    //resize(segMat, showMat, Size(inWidth, inHeight), 0, 0, INTER_NEAREST); //NOT NEEDED IN THIS CASE
    	    for ( int ii = 0; ii < showMat.rows * showMat.cols * 3; ii++) {
            showMat.data[ii] = segMat.data[ii]; //copy SegMat into showMat
            image3.data[ii] = image3.data[ii] * 0.4 + showMat.data[ii] * 0.6;
          }
    	    if (n<20)
          {
            char s[20]; sprintf(s, "image %03d", n);
            // putText(image3, s, Point(10,10), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);

            Mat dst;
            cv::hconcat(imageList[n], image3, dst); // horizontal
            //numpy_vertical = np.vstack((imageList[n], image3))
            //numpy_horizontal = np.hstack((imageList[n], image3))
            cv::imshow(s, dst);

            //cv::imshow("input", imageList[n]);
            cv::waitKey(1000);
    		    //cv::imshow("segmented",image3);
    		    //cv::imshow(s, image3);
            //cv::waitKey(1000);
    		    //cv::imshow(s, numpy_horizontal);
    		    cv::imwrite(format("%03d.png",n), image3);
            //cv::waitKey(1000);
            cv::destroyAllWindows();
    	    }
      }
      imageList.clear();
    }



    /////////////////////////////////////////////////////////////////////////////////////////////


    //cout << "deleting softmax     memory" << endl;
    //delete[] softmax;
    cout << "deleting imageInputs memory" << endl;
    delete[] imageInputs;
    cout << "deleting FCResult    memory" << endl;
    delete[] FCResult;

    return 0;
  }
