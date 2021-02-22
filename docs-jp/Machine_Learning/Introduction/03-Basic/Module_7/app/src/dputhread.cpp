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

    while (!queue_out_->push(frame, std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
    LOG(INFO)<<"DPU in single thread duration :"<<std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::system_clock::now() - dpu_start).count()/1000<<"ms";
    return 0;
  }

std::string DpuThread::name() 
{ 
  return std::string("DPU-") + suffix_;
  
 }

   
    }//end ai
}//end vitis
