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
#ifndef SORTTHREAD
#define SORTTHREAD

#include "frameinfo.hpp"
#include <vitis/ai/bounded_queue.hpp>
#include "mythread.hpp"


namespace vitis{
    namespace ai{
using queue_t = vitis::ai::BoundedQueue<FrameInfo>;

struct SortingThread : public MyThread {
  /**
   * @brief Construct a new Sorting Thread object
   * 
   * @param queue_in the queue get from DPU thread
   * @param queue_out output the thread with sorted frame_id
   * @param suffix thread name suffix
   */
  SortingThread(queue_t* queue_in, queue_t* queue_out,
                const std::string& suffix);
  virtual ~SortingThread() {}

  /**
   * @brief run function of sortthread
   * 
   * @return int 
   */
  virtual int run() override ;

  /**
   * @brief nameing a sort thread
   * 
   * @return std::string 
   */
  virtual std::string name() override ;
  queue_t* queue_in_;
  queue_t* queue_out_;
  unsigned long frame_id_;
  /**
   * @brief the timestamp
   * 
   */
  std::deque<std::chrono::time_point<std::chrono::steady_clock>> points_;
  std::string suffix_;
  /**
   * @brief fps bumber
   * 
   */
  float fps_;

  /**
   * @brief pick the max fps number to store in max_fps_
   * 
   */
  float max_fps_;
};

    }//end ai
}//end vitis


#endif
