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
#include "dpdrm.hpp"
#endif


namespace vitis{
    namespace ai{
using queue_t = vitis::ai::BoundedQueue<FrameInfo>;
/**
 * @struct  GuiThread
 * 
 * @brief   display thread
 * 
 */
struct GuiThread : public MyThread {
  /**
   * @brief create an instance of GuiThread
   * 
   * @return std::shared_ptr<GuiThread> 
   */
  static std::shared_ptr<GuiThread> instance();
  GuiThread();
  virtual ~GuiThread() {  //
#if USE_DRM
    vitis::ai::imshow_close();
#endif
  }
  /**
   * @brief clean up the queue set the dirty flag of the FrameInfo true.
   * 
   */
  void clean_up_queue();

  /**
   * @brief run the display function
   * 
   * @return int 
   */
  virtual int run();

/**
 * @brief naming a Gui thread
 * 
 * @return std::string 
 */
  virtual std::string name() override; 

  /**
   * @brief get queue function, the display frame need to be copy the this queue.
   * 
   * @return queue_t* 
   */
  queue_t* getQueue(); 

  /**
   * @brief the FrameInfo queue to display
   * 
   */
  std::unique_ptr<queue_t> queue_;
  /**
   * @brief setting the time out counter
   * 
   */
  int inactive_counter_;
  
  /**
   * @brief add dirty flag to FrameInfo
   * 
   */
  struct FrameCache {
    bool dirty;
    FrameInfo frame_info;
  };
 std::map<int, FrameCache> frames_;


 /**
  * @brief display windows for DRM
  * 
  */
 static std::vector<cv::Rect> rects;

 /**
  * @brief backgourd image for DRM
  * 
  */
 static cv::Mat img;
};

    }//end ai
}//end vitis

#endif
