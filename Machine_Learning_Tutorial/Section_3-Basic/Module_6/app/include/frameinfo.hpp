
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
#ifndef FRAMEINFO
#define FRAMEINFO

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
namespace vitis{
    namespace ai{
/**
 * @brief FrameInfo struct the data flow struct, used in decode_thread/dpu_thread/sort_thread/Gui_thread
 * 
 */
struct FrameInfo {
  int channel_id;
  unsigned long frame_id;
  cv::Mat dpu_mat;// run dpu thread task required
  cv::Mat show_mat;// run gui thread task required
  float max_fps;// maximum fps
  float fps;// realtime fps
  int belonging;
  int mosaik_width;
  int mosaik_height;
  int horizontal_num;
  int vertical_num;
  cv::Rect_<int> local_rect;
  cv::Rect_<int> page_layout;
  std::string channel_name;
};


}
}



#endif
