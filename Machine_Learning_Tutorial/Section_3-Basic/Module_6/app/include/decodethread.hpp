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
      
using queue_t = vitis::ai::BoundedQueue<FrameInfo>;
struct DecodeThread : public MyThread {
  DecodeThread(int channel_id, const std::string& video_file, queue_t* queue);     
        

  virtual ~DecodeThread() {}
 
  virtual int run() override;

  virtual std::string name() override;

  void open_stream();

  std::vector<cv::Mat> v4l2_videoCapture();
  int channel_id_;
  int fd;
  std::string video_file_;
  unsigned long frame_id_;
  std::unique_ptr<cv::VideoCapture> video_stream_;
  queue_t* queue_;
 
  bool is_camera_;
  const char * in_devname;
  V4L2DeviceParameters param;
  V4l2Capture* videoCapture;
};
}}

#endif
