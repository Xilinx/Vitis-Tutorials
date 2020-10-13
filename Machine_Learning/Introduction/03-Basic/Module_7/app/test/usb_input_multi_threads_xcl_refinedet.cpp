/*
 * Copyright 2020 Xilinx Inc.
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
#include <fcntl.h>
#include <glog/logging.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <type_traits>
#include <vector>
#include <vitis/ai/bounded_queue.hpp>
#include <vitis/ai/nnpp/refinedet.hpp>
#include <vitis/ai/refinedet.hpp>

#include "decodethread.hpp"
#include "dputhread.hpp"
#include "filter.hpp"
#include "frameinfo.hpp"
#include "guithread.hpp"
#include "mythread.hpp"
#include "sortthread.hpp"
#include <hlsV4l2Capture.hpp>
#include <hlsV4l2Device.hpp>

#ifndef USE_DRM
#define USE_DRM 0
#endif
#if USE_DRM
#include "dpdrm.hpp"
#endif

// the multithreads number setting for DPU Thread
static std::vector<int> g_num_of_threads;

// video file name, "0" for camera
static std::vector<std::string> g_avi_file;

// index number for the DPU runners we create
static int idx = 0;

// using the queue type with FrameInfo, using safe BoundedQueue
using queue_t = vitis::ai::BoundedQueue<vitis::ai::FrameInfo>;

// usage help for this application.
inline void usage_video(const char* progname) {
  std::cout << "usage: " << progname << "      -t <num_of_threads>\n"
            << "      <video file name>\n"
            << std::endl;
  return;
}


// parse args 
inline void parse_opt(int argc, char* argv[], int start_pos = 1) {
  int opt = 0;
  optind = start_pos;
  while ((opt = getopt(argc, argv, "c:t:")) != -1) {
    switch (opt) {
      case 't':
        g_num_of_threads.emplace_back(std::stoi(optarg));
        break;
      case 'c':  // how many channels
        break;   // do nothing. parse it in outside logic.
      default:
        usage_video(argv[0]);
        exit(1);
    }
  }
  for (int i = optind; i < argc; ++i) {
    g_avi_file.push_back(std::string(argv[i]));
  }
  if (g_avi_file.empty()) {
    std::cerr << "Expected argument after options\n";
    exit(EXIT_FAILURE);
  }
  if (g_num_of_threads.empty()) {
    // by default, all channels has at least one thread
    g_num_of_threads.emplace_back(1);
  }
  return;
}


/**
 * @brief Drawing overlay after getting the result and the display image from the DPU thread. This function is encapsure into DpuFilter.
 * 
 * @param image input image, in this case we using 1080p image
 * @param result input result
 * @param is_jpeg 
 * @return cv::Mat return the iamge with result overlay.
 */
static cv::Mat process_result(cv::Mat& image_show,
                              const vitis::ai::RefineDetResult& result,
                              bool is_jpeg);

using namespace vitis::ai;
int main(int argc, char* argv[]) {
  int start_pos = 2;
  // register the singal handler for SIGINT
  signal(SIGINT, vitis::ai::MyThread::signal_handler);
  parse_opt(argc, argv, start_pos);
  {
#if USE_DRM
    
    /**
     * @brief determine the input source.
     * in this v4l2 for usb camera input, cv::VideoCapture for video file input
     * 
     */
    std::string file_name(g_avi_file[0]);
    bool is_camera =
        file_name.size() == 1 && file_name[0] >= '0' && file_name[0] <= '9';
    if (is_camera) {
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "Using camera";
    } else {
      cv::VideoCapture video_cap(g_avi_file[0]);
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "Using file";
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "width " << video_cap.get(3);
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "height " << video_cap.get(4);
      cv::Rect rect2(0, 0, (int)video_cap.get(3), (int)video_cap.get(4));
      video_cap.release();
    }
    
#else
    cv::moveWindow(std::string{"CH-"} + std::to_string(0), 500, 500);
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "Window name " << std::string{"CH-"} + std::to_string(0);
#endif

   auto decode_queue = std::unique_ptr<queue_t>{new queue_t{5}};
    
  /**
   * @brief Create a decode_thread for getting image from usb camera.
   * Only one channel input, channel_id=0
   */
    auto decode_thread = std::unique_ptr<DecodeThread>(
        new DecodeThread{0, file_name, decode_queue.get()});

  /**
   * @brief Create a Dpu Thread which will contain g_num_of_threads DPU Filter.  
   * Decode thread is a producer and DPU is a consumer with "g_num_of_threads" Filters.
   * 
   */
    auto dpu_thread = std::vector<std::unique_ptr<DpuThread>>{};

    /**
     * @brief using frame_id in FrameInfo to sort the queue
     * 
     */
    auto sorting_queue =
        std::unique_ptr<queue_t>(new queue_t(5 * g_num_of_threads[0]));

    /**
     * @brief create an GUI thread, copy the image to the drm frame buffer to show the result.
     * 
     */
    auto gui_thread = vitis::ai::GuiThread::instance();

    /**
     * @brief the FrameInfo need to be display should copy to gui_queue.
     * 
     */
    auto gui_queue = gui_thread->getQueue();

    /**
     * @brief setting the model_name 
     * 
     */
    const auto model_name = argv[1];
    /**
     * @brief push back the g_num_of_threads of DPU Filters into DPU THREAD
     * 
     */
    for (int i = 0; i < g_num_of_threads[0]; ++i) {
      dpu_thread.emplace_back(new DpuThread(
          create_dpu_filter( [model_name]() {return vitis::ai::RefineDet::create(model_name, 0);}, process_result), 
          decode_queue.get(),sorting_queue.get(), std::to_string(i)));
    }

    auto sorting_thread = std::unique_ptr<vitis::ai::SortingThread>(
        new SortingThread(sorting_queue.get(), gui_queue, std::to_string(0)));
   /**
    * @brief run the MyThread::start_all function
    * 
    */
    MyThread::start_all();

    /**
     * @brief wait for all Gui thread finished
     * 
     */
    gui_thread->wait();

    /**
     * @brief stop all the threads.
     * 
     */
    MyThread::stop_all();
    /**
     * @brief Stop the decode thread/dpu thread/sort thread.
     * 
     */
    MyThread::wait_all();
  }
  LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "BYEBYE";
  return 0;
}

static cv::Mat process_result(cv::Mat& image_show,
                              const vitis::ai::RefineDetResult& result,
                              bool is_jpeg) {
  //auto img = image.clone();
  for (auto& box : result.bboxes) {
    float x = box.x * (image_show.cols);
    float y = box.y * (image_show.rows);
    int xmin = x;
    int ymin = y;
    int xmax = x + (box.width) * (image_show.cols);
    int ymax = y + (box.height) * (image_show.rows);
    float score = box.score;
    xmin = std::min(std::max(xmin, 0), image_show.cols);
    xmax = std::min(std::max(xmax, 0), image_show.cols);
    ymin = std::min(std::max(ymin, 0), image_show.rows);
    ymax = std::min(std::max(ymax, 0), image_show.rows);

    LOG_IF(INFO, is_jpeg) << "RESULT2: "
                          << "\t" << xmin << "\t" << ymin << "\t" << xmax
                          << "\t" << ymax << "\t" << score << "\n";
    auto label = 2;
    if (label == 1) {
      cv::rectangle(image_show, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(0, 255, 0), 1, 1, 0);
    } else if (label == 2) {
      cv::rectangle(image_show, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(255, 0, 0), 1, 1, 0);
    } else if (label == 3) {
      cv::rectangle(image_show, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(0, 0, 255), 1, 1, 0);
    }
  }

  return image_show;
}
