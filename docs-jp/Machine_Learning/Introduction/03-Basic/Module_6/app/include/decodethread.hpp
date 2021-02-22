/*
 * Copyright 2019 Xilinx Inc.
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
#ifndef DECODETHREAD
#define DECODETHREAD
#include "frameinfo.hpp"
#include <vitis/ai/bounded_queue.hpp>
#include <memory>
#include "mythread.hpp"
#include <linux/videodev2.h>    
#include <V4l2Device.hpp>
#include <V4l2Capture.hpp>

namespace vitis{
    namespace ai{

/**
 * @brief queue_t the safe bounded queue for FrameInfo. BoundedQueue offers operation APIs such as push, pop , etc.
 * 
 */
using queue_t = vitis::ai::BoundedQueue<FrameInfo>;
/**
 * @brief Responsible for decode MJPEG or YUV data to BGR
 * 
 */
struct DecodeThread : public MyThread {
  /**
   * @brief Construct a new Decode Thread object
   * 
   * @param channel_id the input channel id.
   * @param video_file usb camera or video file
   * @param queue output queue, the queue of FrameInfo will be forward to DPU thread.
   */
  DecodeThread(int channel_id, const std::string& video_file, queue_t* queue);     
        

  virtual ~DecodeThread() {}
 /**
  * @brief master function ot run Decode thread task
  * 
  * @return int  0 for success, -1 for stopped
  */
  virtual int run() override;

  /**
   * @brief naming an decode thread.
   * 
   * @return std::string 
   */
  virtual std::string name() override;
  /**
   * @brief open the stream by using opencv videoCapture directly
   * 
   */
  void open_stream();

  /**
   * @brief Open the stream by using V4L2 libs to get the yuv data from USB-Camera and convert into BGR.
   * 
   * @return std::vector<cv::Mat> contains two cv::Mat, one is 1080P for showing the result, the other is 640x360 for DPU processing.
   *
   */
  std::vector<cv::Mat> v4l2_videoCapture();
  /**
   * @brief input channel ID
   * 
   */
  int channel_id_;
  /**
   * @brief video file input. 
   * 
   */
  std::string video_file_;
  /**
   * @brief frame_id_ using for sort the sequence.
   * 
   */
  unsigned long frame_id_;
  /**
   * @brief using for video input.
   * 
   */
  std::unique_ptr<cv::VideoCapture> video_stream_;
  /**
   * @brief the frames get from USB-Camera/video file will be emplace back to this queue, this queue_ will be output to DPU thread
   * 
   */
  queue_t* queue_;
  /**
   * @brief true for camera, false for video file
   * 
   */
  bool is_camera_;
  /**
   * @brief device name of V4L2
   * 
   */
  const char * in_devname;
  /**
   * @brief The param to initialize an v4l2 device.
   * 
   */
  V4L2DeviceParameters param;
  /**
   * @brief The method to get the camera video from usb camera by using V4L2
   * 
   */
  V4l2Capture* videoCapture;
};
}}

#endif
