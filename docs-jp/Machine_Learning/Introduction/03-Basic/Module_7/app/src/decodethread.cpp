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
#include "decodethread.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <glog/logging.h>
#include <iostream>
#include <memory>

#ifndef USE_KERNEL
#define USE_KERNEL 0
#endif

namespace vitis{namespace ai{
std::vector<cv::Mat> DecodeThread::v4l2Mats(2);
//std::vector<cv::Mat> DecodeThread::v4l2Mats_0(2);
//int DecodeThread::count=0;

  DecodeThread::DecodeThread(int channel_id, const std::string& video_file, queue_t* queue)
      : MyThread{},
        channel_id_{channel_id},
        video_file_{video_file},
        frame_id_{0},
        in_devname{"/dev/video0"},
        param(in_devname, V4L2_PIX_FMT_UYVY , 2304, 1296, 30, 0,0),
        video_stream_{},
        queue_{queue},
        videoCapture{V4l2Capture::create(param, V4l2Access::IOTYPE_MMAP)}
        
        {
          
        }


   int DecodeThread::run() {
   
    //auto& cap = *video_stream_.get();
    auto image_from_device_start = std::chrono::system_clock::now();
    v4l2_videoCapture(v4l2Mats);
        while (!queue_->push(FrameInfo{channel_id_, ++frame_id_, v4l2Mats[1], v4l2Mats[0]},
                         std::chrono::milliseconds(10000))) {
      if (is_stopped()) {
        return -1;
      }}

    
    LOG(INFO)<<"Decode and Resize :"<<std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::system_clock::now() - image_from_device_start).count()/1000<<"ms";
    return 0;
  }

  std::string DecodeThread::name() {
    return std::string{"DedodeThread-"} + std::to_string(channel_id_);
  }

  void DecodeThread::open_stream() {
    is_camera_ = video_file_.size() == 1 && video_file_[0] >= '0' &&
                 video_file_[0] <= '9';
    video_stream_ = std::unique_ptr<cv::VideoCapture>(
        is_camera_ ? new cv::VideoCapture(std::stoi(video_file_))
                   : new cv::VideoCapture(video_file_));
    if (!video_stream_->isOpened()) {
      LOG(ERROR) << "cannot open file " << video_file_;
      stop();
    }
  }

 int DecodeThread::v4l2_videoCapture(std::vector<cv::Mat>& mats)
  {
    //std::vector<cv::Mat> v4l2Mats;
  //  v4l2Mats.reserve(2);
    timeval tv;
    tv.tv_sec=1;
    tv.tv_usec=0;
    int ret = videoCapture->isReadable(&tv);
    if(ret==1)
    {
    #if USE_KERNEL
    auto ret1=videoCapture->read_images_with_kernel(mats);
    #else
    auto ret1=videoCapture->read_images(mats);
    #endif
   // CHECK_EQ(ret, 0)<<"get frame from device failed ";
    
   }
    return 0;
  }



}//end ai 
}//end vitis
