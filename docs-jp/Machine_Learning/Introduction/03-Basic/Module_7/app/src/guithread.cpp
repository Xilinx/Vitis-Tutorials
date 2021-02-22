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
#include "guithread.hpp"
#include <glog/logging.h>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "frameinfo.hpp"

namespace vitis {
namespace ai {

std::shared_ptr<GuiThread> GuiThread::instance() {
  static std::weak_ptr<GuiThread> the_instance;
  std::shared_ptr<GuiThread> ret;
  if (the_instance.expired()) {
    ret = std::make_shared<GuiThread>();
    the_instance = ret;
  }
  ret = the_instance.lock();
  assert(ret != nullptr);
#if USE_DRM
  vitis::ai::imshow_open();
  //cv::Mat img(1080, 1920, CV_8UC3);
  imshow_set_background(img);
#endif
  return ret;
}

GuiThread::GuiThread()
    : MyThread{},
      queue_{
          new queue_t{10}  // assuming GUI is not bottleneck, 10 is high enough
      },
      inactive_counter_{0} {
  LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT GUI";
}
void GuiThread::clean_up_queue() {
  FrameInfo frame_info;
  while (!queue_->empty()) {
    queue_->pop(frame_info);
    frames_[frame_info.channel_id].frame_info = frame_info;
    frames_[frame_info.channel_id].dirty = true;
  }
}
int GuiThread::run() {
auto gui_start = std::chrono::system_clock::now();
  FrameInfo frame_info;
  if (!queue_->pop(frame_info, std::chrono::milliseconds(500))) {
    inactive_counter_++;
    if (inactive_counter_ > 150) {
      // inactive for 5 second, stop
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "no frame_info to show";
      return 1;
    } else {
      return 0;
    }
  }
  inactive_counter_ = 0;
  frames_[frame_info.channel_id].frame_info = frame_info;
  frames_[frame_info.channel_id].dirty = true;
  LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
      << " gui queue size " << queue_->size()
      << ", state = " << (is_stopped() ? "stopped" : "running");
  clean_up_queue();
#if USE_DRM
  bool all_dirty = true;
  for (auto& f : frames_) {
    all_dirty = all_dirty && f.second.dirty;
  }
  if (!all_dirty) {  
    // only show frames until all channels are dirty
    return 0;
  }
  auto width = modeset_get_fb_width();
  auto height = modeset_get_fb_height();
  auto screen_size = cv::Size{width, height};
  auto sizes = std::vector<cv::Size>(frames_.size());

  std::transform(frames_.begin(), frames_.end(), sizes.begin(),
                 [](const decltype(frames_)::value_type& a) {
                   return a.second.frame_info.show_mat.size();
                 });
  LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "rects size is  " << rects.size();

  for (const auto& rect : rects) {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "screen " << screen_size << "; r = " << rect;
    if ((rect.x + rect.width > width) || (rect.y + rect.height > height) ||
        (rect.x + rect.width < 1) || (rect.y + rect.height < 1)) {
      LOG(FATAL) << "out of boundary";
    }
  }
  int c = 0;
  for (auto& f : frames_) {
    vitis::ai::imshow(rects[c], f.second.frame_info.show_mat);
    f.second.dirty = false;
    c++;
  }
  vitis::ai::imshow_update();
#else
  bool any_dirty = false;
  for (auto& f : frames_) {
    if (f.second.dirty) {
      cv::imshow(
          std::string{"CH-"} + std::to_string(f.second.frame_info.channel_id),
          f.second.frame_info.mat);
      f.second.dirty = false;
      any_dirty = true;
    }
  }
  if (any_dirty) {
    auto key = cv::waitKey(1);
    if (key == 27) {
      return 1;
    }
  }
#endif
LOG(INFO)<<"Gui duration :"<<std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::system_clock::now() - gui_start).count()/1000<<"ms";

  GuiThread::clean_up_queue();
  return 0;
}

std::string GuiThread::name() { return std::string{"GUIThread"}; }

queue_t* GuiThread::getQueue() { return queue_.get(); }

cv::Mat GuiThread::img(1080, 1920, CV_8UC3, cv::Scalar(0,215,255));
std::vector<cv::Rect> GuiThread::rects{{0, 0, 1920, 1080}};

}  // namespace ai
}  // namespace vitis
