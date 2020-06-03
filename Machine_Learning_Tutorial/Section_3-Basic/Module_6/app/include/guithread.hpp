#ifndef GUITHREAD
#define GUITHREAD
#include "frameinfo.hpp"
#include <vitis/ai/bounded_queue.hpp>
#include <memory>
#include "mythread.hpp"
#include <map>
#include <opencv2/core.hpp>


#ifndef USE_DRM
#define USE_DRM 0
#endif

#if USE_DRM
#include <vitis/ai/dpdrm.hpp>
#endif


namespace vitis{
    namespace ai{
using queue_t = vitis::ai::BoundedQueue<FrameInfo>;

struct GuiThread : public MyThread {
  static std::shared_ptr<GuiThread> instance();
  GuiThread();
  virtual ~GuiThread() {  //
#if USE_DRM
    vitis::ai::imshow_close();
#endif
  }
  void clean_up_queue();
  virtual int run();

  virtual std::string name() override; 

  queue_t* getQueue(); 

  std::unique_ptr<queue_t> queue_;
  int inactive_counter_;
  
  struct FrameCache {
    bool dirty;
    FrameInfo frame_info;
  };
 std::map<int, FrameCache> frames_;
 static std::vector<cv::Rect> rects;
 static cv::Mat img;
};

    }//end ai
}//end vitis

#endif
