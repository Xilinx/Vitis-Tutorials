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
#pragma once

#include <glog/logging.h>
#include <signal.h>
#include <memory>
#include <vector>
#include <thread>
#include "vitis/ai/env_config.hpp"



#ifndef USE_DRM
#define USE_DRM 0
#endif

#if USE_DRM
#include "dpdrm.hpp"
#endif

namespace vitis {
namespace ai {
DEF_ENV_PARAM(DEBUG_DEMO, "1")
struct MyThread {
  /**
 * @brief create a threads pool
 * 
 * @return std::vector<MyThread*>& 
 */
  static inline std::vector<MyThread*>& all_threads();
  /**
 * @brief signal_handler for singal handle
 * 
 */
  static void signal_handler(int);
    /**
  * @brief interrupt all the threads
  * 
  */
  static void stop_all();
  /**
   * @brief wait all threads finish
   * 
   */
  static void wait_all();

  /**
   * @brief create threads and with main_proxy()
   * 
   */
  static void start_all();

  /**
   * @brief proxy function for real main() 
   * 
   * @param me the object pointer which derived from MyThread.
   */
  static void main_proxy(MyThread* me);

  /**
   * @brief real main() function, wrap the run() function.
   * 
   */
  void main();

  virtual int run() = 0;

  virtual std::string name() = 0;

  /**
   * @brief Construct a new My Thread:: My Thread object. Once the construt function called the all_threads() function
   * will create an static thread pool"std::vector<Mythread*> threads""
   * 
   */
  explicit MyThread();

  /**
   * @brief deconstruct function
   * 
   * erase all the elements form the std::vector<Mythread*> threads 
   */
  virtual ~MyThread();

  /**
   * @brief create threads and with main_proxy()
   * 
   */
  void start();
  /**
   * @brief stop all threads, set the stop_ flag true.
   * 
   */
  void stop();

  /**
   * @brief wait for all the threads finish.
   * 
   */
  void wait();
  
  /**
   * @brief Determine the status of the stop flag
   * 
   * @return true 
   * @return false 
   */
  bool is_stopped();
  /**
   * @brief thread flag 
   * 
   */
  bool stop_;

  /**
   * @brief thread safe pointer
   * 
   */
  std::unique_ptr<std::thread> thread_;
};
}//end ai
}//end vitis
