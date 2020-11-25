#include "decodethread.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <glog/logging.h>
#include <iostream>
#include <memory>

namespace vitis{namespace ai{

  DecodeThread::DecodeThread(int channel_id, const std::string& video_file, queue_t* queue)
      : MyThread{},
        channel_id_{channel_id},
        video_file_{video_file},
        frame_id_{0},
        in_devname{"/dev/video0"},
        param(in_devname, V4L2_PIX_FMT_UYVY , 1920, 1080, 30, 0,0),
        video_stream_{},
        queue_{queue},
        videoCapture{V4l2Capture::create(param, V4l2Access::IOTYPE_MMAP)}
        
        {}


   int DecodeThread::run() {
   
    //auto& cap = *video_stream_.get();
    auto image_from_device_start = std::chrono::system_clock::now();
    auto image=v4l2_videoCapture();
    LOG(INFO)<<"Decode and Resize :"<<std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::system_clock::now() - image_from_device_start).count()<<"ms";
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "1080p decode queue size " << queue_->size();
    while (!queue_->push(FrameInfo{channel_id_, ++frame_id_, image[1], image[0]},
                         std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
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

  std::vector<cv::Mat> DecodeThread::v4l2_videoCapture()
  {
    std::vector<cv::Mat> v4l2Mats;
    timeval tv;
    tv.tv_sec=1;
    tv.tv_usec=0;
    int ret = videoCapture->isReadable(&tv);
    if(ret==1)
    {
    auto ret1=videoCapture->read_images(v4l2Mats);
   // CHECK_EQ(ret, 0)<<"get frame from device failed ";
    
   }
    return v4l2Mats;
  }



}//end ai 
}//end vitis
