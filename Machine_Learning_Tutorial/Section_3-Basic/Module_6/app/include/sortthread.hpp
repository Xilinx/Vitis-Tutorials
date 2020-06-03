#ifndef SORTTHREAD
#define SORTTHREAD

#include "frameinfo.hpp"
#include <vitis/ai/bounded_queue.hpp>
#include "mythread.hpp"


namespace vitis{
    namespace ai{
using queue_t = vitis::ai::BoundedQueue<FrameInfo>;

struct SortingThread : public MyThread {
  SortingThread(queue_t* queue_in, queue_t* queue_out,
                const std::string& suffix);
  virtual ~SortingThread() {}
  virtual int run() override ;
  virtual std::string name() override ;
  queue_t* queue_in_;
  queue_t* queue_out_;
  unsigned long frame_id_;
  std::deque<std::chrono::time_point<std::chrono::steady_clock>> points_;
  std::string suffix_;
  float fps_;
  float max_fps_;
};

    }//end ai
}//end vitis


#endif


