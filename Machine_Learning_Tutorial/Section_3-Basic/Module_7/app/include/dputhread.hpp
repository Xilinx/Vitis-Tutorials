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


#ifndef DPUTHREAD
#define DPUTHREAD

#include <memory>
#include <string>
#include <vitis/ai/bounded_queue.hpp>

#include "decodethread.hpp"
#include "filter.hpp"
#include "frameinfo.hpp"
#include "mythread.hpp"

namespace vitis {
namespace ai {

using queue_t = vitis::ai::BoundedQueue<FrameInfo>;
struct DpuThread : public MyThread {
  /**
   * @brief Construct a new Dpu Thread object
   * 
   * @param filter DpuFilter which contains the model algorithm level pre-processing/run dpu task/post-processing show_result.
   * @param queue_in queue get from decode thread
   * @param queue_out output queue for sort thread
   * @param suffix the thread name suffix
   */
  DpuThread(std::unique_ptr<Filter>&& filter, queue_t* queue_in,
            queue_t* queue_out,const std::string& suffix);

  virtual ~DpuThread() {}

  /**
   * @brief Thead run() function and will call the DpuFilter run()
   * 
   * @return int 
   */
  virtual int run() override;
  /**
   * @brief name the thread
   * 
   * @return std::string 
   */
  virtual std::string name() override;
  /**
   * @brief Filter of DPU
   * 
   */
  std::unique_ptr<Filter> filter_;

  /**
   * @brief frame queue getting from decode thread
   * 
   */
  queue_t* queue_in_;
  /**
   * @brief output queue for sortthread
   * 
   */
  queue_t* queue_out_;

  /**
   * @brief DpuThread name suffix
   * 
   */
  std::string suffix_;
};
}  // namespace ai
}  // namespace vitis

#endif
