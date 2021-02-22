#include "dputhread.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include "frameinfo.hpp"
#include <glog/logging.h>

namespace vitis {
    namespace ai{
    
   DpuThread::DpuThread(std::unique_ptr<Filter>&& filter, queue_t* queue_in,
            queue_t* queue_out, const std::string& suffix)
      : MyThread{},
        filter_{std::move(filter)},
        queue_in_{queue_in},
        queue_out_{queue_out},
        suffix_{suffix}
 {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT DPU";
  }

   int DpuThread::run()  {
    auto dpu_start = std::chrono::system_clock::now();
    FrameInfo frame;
    if (!queue_in_->pop(frame, std::chrono::milliseconds(500))) {
      return 0;
    }

    if (filter_) {
      frame.show_mat = filter_->run(frame);
    }
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "dpu queue size " << queue_out_->size();
    LOG(INFO)<<"DPU :"<<std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::system_clock::now() - dpu_start).count()<<"ms";
    while (!queue_out_->push(frame, std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
    return 0;
  }

std::string DpuThread::name() 
{ 
  return std::string("DPU-") + suffix_;
  
 }

   
    }//end ai
}//end vitis
