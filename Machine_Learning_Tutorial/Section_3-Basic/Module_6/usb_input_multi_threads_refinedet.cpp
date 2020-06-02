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
#include <glog/logging.h>
#include <signal.h>
#include <unistd.h>
#include <vart/dpu/dpu_runner.hpp>
#include <vart/dpu/dpu_runner_ext.hpp>
#include <vart/dpu/tensor_buffer.hpp>
#include <string>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vitis/ai/library/tensor.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <type_traits>
#include <vitis/ai/bounded_queue.hpp>
#include <vitis/ai/env_config.hpp>
#include <vector>
#include <vitis/ai/refinedet.hpp>
//using v4l2 setting the camera
#include <linux/videodev2.h>    
#include <V4l2Device.h>
#include <V4l2Capture.h>
#include <sys/types.h>                      
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstring>

DEF_ENV_PARAM(DEBUG_DEMO, "1")

#ifndef USE_DRM
#define USE_DRM 0
#endif
#if USE_DRM
#include <vitis/ai/dpdrm.hpp>
#endif



// get runners parameters
static int idx=0;
 auto runners_ = vitis::ai::DpuRunner::create_dpu_runner("/usr/share/vitis_ai_library/models/refinedet_pruned_0_8");
  auto inputs =
      dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[idx].get())->get_inputs();
  auto scales = dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[idx].get())
                    ->get_input_scale();
// set the layout
inline std::vector<cv::Rect>& gui_layout() {
  static std::vector<cv::Rect> rects;
  return rects;
}
// set the wallpaper
inline cv::Mat& gui_background() {
  static cv::Mat img;
  return img;
}

static std::vector<int> g_num_of_threads;
static std::vector<std::string> g_avi_file;
inline void usage_video(const char* progname) {
  std::cout << "usage: " << progname << "      -t <num_of_threads>\n"
            << "      <video file name>\n"
            << std::endl;
  return;
}

inline void parse_opt(int argc, char* argv[], int start_pos = 1) {
  int opt = 0;
  optind = start_pos;
  while ((opt = getopt(argc, argv, "c:t:")) != -1) {
    switch (opt) {
      case 't':
        g_num_of_threads.emplace_back(std::stoi(optarg));
        break;
      case 'c':  // how many channels
        break;   // do nothing. parse it in outside logic.
      default:
        usage_video(argv[0]);
        exit(1);
    }
  }
  for (int i = optind; i < argc; ++i) {
    g_avi_file.push_back(std::string(argv[i]));
  }
  if (g_avi_file.empty()) {
    std::cerr << "Expected argument after options\n";
    exit(EXIT_FAILURE);
  }
  if (g_num_of_threads.empty()) {
    // by default, all channels has at least one thread
    g_num_of_threads.emplace_back(1);
  }
  return;
}
inline std::vector<cv::Size>& each_channel_mosaik_size() {
  static std::vector<cv::Size> msize;
  return msize;
}

namespace vitis {
namespace ai {

// A struct that can storage data and info for each frame
struct FrameInfo {
  int channel_id;
  unsigned long frame_id;
  cv::Mat mat;
  float max_fps;
  float fps;
  int belonging;
  int mosaik_width;
  int mosaik_height;
  int horizontal_num;
  int vertical_num;
  cv::Rect_<int> local_rect;
  cv::Rect_<int> page_layout;
  std::string channel_name;
};

using queue_t = vitis::ai::BoundedQueue<FrameInfo>;
struct MyThread {
  // static std::vector<MyThread *> all_threads_;
  static inline std::vector<MyThread*>& all_threads() {
    static std::vector<MyThread*> threads;
    return threads;
  }
  static void signal_handler(int) { stop_all(); }
  static void stop_all() {
    for (auto& th : all_threads()) {
      th->stop();
    }
  }
  static void wait_all() {
    for (auto& th : all_threads()) {
      th->wait();
    }
  }
  static void start_all() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "Thread num " << all_threads().size();
    for (auto& th : all_threads()) {
      th->start();
    }
  }

  static void main_proxy(MyThread* me) { return me->main(); }
  void main() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] is started";
    while (!stop_) {
      auto run_ret = run();
      if (!stop_) {
        stop_ = run_ret != 0;
      }
    }
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "thread [" << name() << "] is ended";
  }

  virtual int run() = 0;

  virtual std::string name() = 0;

  explicit MyThread() : stop_(false), thread_{nullptr} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT A Thread";
    all_threads().push_back(this);
  }

  virtual ~MyThread() {  //
    all_threads().erase(
        std::remove(all_threads().begin(), all_threads().end(), this),
        all_threads().end());
  }

  void start() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] is starting";
    thread_ = std::unique_ptr<std::thread>(new std::thread(main_proxy, this));
  }

  void stop() {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] is stopped.";
    stop_ = true;
  }

  void wait() {
    if (thread_ && thread_->joinable()) {
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
          << "waiting for [" << name() << "] ended";
      thread_->join();
    }
  }
  bool is_stopped() { return stop_; }

  bool stop_;
  std::unique_ptr<std::thread> thread_;
};

// std::vector<MyThread *> MyThread::all_threads_;
struct DecodeThread : public MyThread {
  DecodeThread(int channel_id, const std::string& video_file, queue_t* queue)
      : MyThread{},
        channel_id_{channel_id},
        video_file_{video_file},
        frame_id_{0},
        in_devname{"/dev/video0"},
        param(in_devname, V4L2_PIX_FMT_UYVY , 1920, 1080, 30, 0,0),
        video_stream_{},
        queue_{queue},
        videoCapture{V4l2Capture::create(param, V4l2Access::IOTYPE_MMAP)}
        {}

  virtual ~DecodeThread() {}
 
  virtual int run() override {
   
    //auto& cap = *video_stream_.get();
    auto image_from_device_start = std::chrono::system_clock::now();
    auto image=v4l2_videoCapture();
    std::cout<<"get_image+convert+resize time :"<<std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::system_clock::now() - image_from_device_start).count()<<std::endl;
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "decode queue size " << queue_->size();
    if (queue_->size() > 0 && is_camera_ == true) {
      return 0;
    }
    while (!queue_->push(FrameInfo{channel_id_, ++frame_id_, image},
                         std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
    return 0;
  }

  virtual std::string name() override {
    return std::string{"DedodeThread-"} + std::to_string(channel_id_);
  }

  void open_stream() {
    is_camera_ = video_file_.size() == 1 && video_file_[0] >= '0' &&
                 video_file_[0] <= '9';
    video_stream_ = std::unique_ptr<cv::VideoCapture>(
        is_camera_ ? new cv::VideoCapture(std::stoi(video_file_))
                   : new cv::VideoCapture(video_file_));
    if (!video_stream_->isOpened()) {
      LOG(ERROR) << "cannot open file " << video_file_;
      stop();
    }
  }

  cv::Mat v4l2_videoCapture()
  {
    cv::Mat v4l2Mat;
    timeval tv;
    tv.tv_sec=1;
    tv.tv_usec=0;
    int ret = videoCapture->isReadable(&tv);
    if(ret==1)
    {
    auto ret1=videoCapture->read(v4l2Mat);
    
   }
   cv::Mat image;
   auto size=cv::Size(640,360);
   cv::resize(v4l2Mat, image, size);
  return image;
  }

  int channel_id_;
  int fd;
  std::string video_file_;
  unsigned long frame_id_;
  std::unique_ptr<cv::VideoCapture> video_stream_;
  queue_t* queue_;
  bool is_camera_;
  const char * in_devname;
  V4L2DeviceParameters param;
  V4l2Capture* videoCapture;
  

};

struct GuiThread : public MyThread {
  static std::shared_ptr<GuiThread> instance() {
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
    cv::Mat img = gui_background();
    imshow_set_background(img);
#endif
    return ret;
  }

  GuiThread()
      : MyThread{},
        queue_{
            new queue_t{
                10}  // assuming GUI is not bottleneck, 10 is high enough
        },
        inactive_counter_{0} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT GUI";
  }
  virtual ~GuiThread() {  //
#if USE_DRM
    vitis::ai::imshow_close();
#endif
  }
  void clean_up_queue() {
    FrameInfo frame_info;
    while (!queue_->empty()) {
      queue_->pop(frame_info);
      frames_[frame_info.channel_id].frame_info = frame_info;
      frames_[frame_info.channel_id].dirty = true;
    }
  }
  virtual int run() override {
    FrameInfo frame_info;
    if (!queue_->pop(frame_info, std::chrono::milliseconds(500))) {
      inactive_counter_++;
      if (inactive_counter_ > 10) {
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
                     return a.second.frame_info.mat.size();
                   });
    std::vector<cv::Rect> rects;
    rects = gui_layout();
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
      vitis::ai::imshow(rects[c], f.second.frame_info.mat);
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
    clean_up_queue();
    return 0;
  }

  virtual std::string name() override { return std::string{"GUIThread"}; }

  queue_t* getQueue() { return queue_.get(); }

  std::unique_ptr<queue_t> queue_;
  int inactive_counter_;
  struct FrameCache {
    bool dirty;
    FrameInfo frame_info;
  };
  std::map<int, FrameCache> frames_;
};
/// GuiThread end

struct GridGuiThread : public MyThread {
  static std::shared_ptr<GridGuiThread> instance() {
    static std::weak_ptr<GridGuiThread> the_instance;
    std::shared_ptr<GridGuiThread> ret;
    if (the_instance.expired()) {
      ret = std::make_shared<GridGuiThread>();
      the_instance = ret;
    }
    ret = the_instance.lock();
    assert(ret != nullptr);
#if USE_DRM
    vitis::ai::imshow_open();
    cv::Mat img = gui_background();
    imshow_set_background(img);
#endif
    return ret;
  }

  GridGuiThread()
      : MyThread{},
        queue_{
            new queue_t{
                60}  // assuming GUI is not bottleneck, 10 is high enough
        },
        inactive_counter_{0} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT GUI";
  }
  virtual ~GridGuiThread() {  //
#if USE_DRM
    vitis::ai::imshow_close();
#endif
  }
  virtual int run() override {  // GRID
    while (!queue_->empty()) {
      FrameInfo frame_info;
      if (!queue_->pop(frame_info, std::chrono::milliseconds(500))) {
        inactive_counter_++;
        if (inactive_counter_ > 50) {
          // inactive for 5 second, stop
          LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "no frame_info to show";
          return 1;
        } else {
          return 0;
        }
      }
      inactive_counter_ = 0;
      frames_[frame_info.channel_id].all_frame_info.push_back(frame_info);
      // frames_[frame_info.channel_id].dirty = true;
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
          << " gridgui queue size " << queue_->size()
          << ", state = " << (is_stopped() ? "stopped" : "running");
    }
#if USE_DRM
    auto width = modeset_get_fb_width();
    auto height = modeset_get_fb_height();
    auto screen_size = cv::Size{width, height};
    std::vector<cv::Rect> rects;
    rects = gui_layout();
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "rects size is  " << rects.size();

    for (const auto& rect : rects) {
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
          << "screen " << screen_size << "; r = " << rect;
      if ((rect.x + rect.width > width) || (rect.y + rect.height > height) ||
          (rect.x + rect.width < 1) || (rect.y + rect.height < 1)) {
        LOG(FATAL) << "out of boundary";
      }
    }
	
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "current frame size is " << frames_.size();
    for (auto& f : frames_) {
      for (auto& frame_info : f.second.all_frame_info) {
        LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
            << "local rect: " << frame_info.local_rect;
        vitis::ai::imshow(frame_info.local_rect, frame_info.mat);
      }
      auto tmp_frame_info = f.second.all_frame_info[0];
      auto tmp_width = 96;
      cv::Mat info_mat(cv::Size(tmp_width * 3, 16), CV_8UC3,
                       cv::Scalar(255, 255, 255));
      cv::Mat name_mat = info_mat(cv::Rect_<int>(0, 0, tmp_width * 1.5, 16));
      cv::Mat fps_mat =
          info_mat(cv::Rect_<int>(tmp_width * 1.7, 0, tmp_width, 16));
      cv::putText(name_mat, tmp_frame_info.channel_name, cv::Point(3, 11),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, 1);
      cv::putText(fps_mat,
                  std::string("fps: ") + std::to_string(tmp_frame_info.fps),
                  cv::Point(3, 11), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                  cv::Scalar(0, 0, 0), 1, 1);
      vitis::ai::imshow(
          cv::Rect_<int>(tmp_frame_info.page_layout.x,
                         tmp_frame_info.page_layout.y, tmp_width * 3, 16),
          info_mat);
    }
    vitis::ai::imshow_update();
    for (auto& f : frames_) {
      for (auto& frame_info : f.second.all_frame_info) {
        LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
            << "local rect: " << frame_info.local_rect;
        vitis::ai::imshow(frame_info.local_rect, frame_info.mat);
      }
      auto tmp_frame_info = f.second.all_frame_info[0];
      auto tmp_width = 96;
      cv::Mat info_mat(cv::Size(tmp_width * 3, 16), CV_8UC3,
                       cv::Scalar(255, 255, 255));
      cv::Mat name_mat = info_mat(cv::Rect_<int>(0, 0, tmp_width * 1.5, 16));
      cv::Mat fps_mat =
          info_mat(cv::Rect_<int>(tmp_width * 1.7, 0, tmp_width, 16));
      cv::putText(name_mat, tmp_frame_info.channel_name, cv::Point(3, 11),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, 1);
      cv::putText(fps_mat,
                  std::string("fps: ") + std::to_string(tmp_frame_info.fps),
                  cv::Point(3, 11), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                  cv::Scalar(0, 0, 0), 1, 1);
      vitis::ai::imshow(
          cv::Rect_<int>(tmp_frame_info.page_layout.x,
                         tmp_frame_info.page_layout.y, tmp_width * 3, 16),
          info_mat);
    }

    // vitis::ai::imshow(cv::Rect_<int>(0, 0, mosaik_width, 16), fps_mat);
    for (auto& f : frames_) {
      f.second.all_frame_info.clear();
    }
#else

#endif
    return 0;
  }

  virtual std::string name() override { return std::string{"GUIThread"}; }

  queue_t* getQueue() { return queue_.get(); }

  std::unique_ptr<queue_t> queue_;
  int inactive_counter_;
  struct FrameCache {
    bool dirty;
    std::vector<FrameInfo> all_frame_info;
  };
  std::map<int, FrameCache> frames_;
};
////////////////////////GridGuiThread
struct Filter {
  explicit Filter() {}
  virtual ~Filter() {}
  virtual cv::Mat run(cv::Mat& input) = 0;
};

// Execute each lib run function and processor your implement
template <typename dpu_model_type_t, typename ProcessResult>
struct DpuFilter : public Filter {
  DpuFilter(std::unique_ptr<dpu_model_type_t>&& dpu_model,
            const ProcessResult& processor, std::vector<vitis::ai::TensorBuffer *>& inputs )
      : Filter{}, dpu_model_{std::move(dpu_model)}, processor_{processor},inputs_{inputs} {}
  virtual ~DpuFilter() {}

  
void NormalizeInputData(uint8_t* input, int rows, int cols, int channels,
                        int stride, const std::vector<float>& mean,
                        const std::vector<float>& scale) {
  //#if 1
  for (auto h = 0; h < rows; ++h) {
    for (auto w = 0; w < cols; ++w) {
      for (auto c = 0; c < channels; ++c) {
        auto value =
            (int)((input[h * stride + w * channels + c] * 1.0f - mean[c]) *
                  scale[c]);
        input[h * cols * channels + w * channels + c] = (char)value;
      }
    }
  }
}

  float my_tensor_scale(vitis::ai::library::InputTensor& tensor) {return std::exp2f(1.0f * (float)tensor.fixpos);}
  vitis::ai::library::InputTensor convert_tensor_buffer_to_input_tensor(
  vitis::ai::TensorBuffer* tb, float scale) {
  auto ret = vitis::ai::library::InputTensor{};
  auto tensor = tb->get_tensor();
  auto dim_num = tensor->get_dim_num();
  ret.size =
      tensor->get_element_num() * vitis::ai::size_of(tensor->get_data_type());
  ret.batch = dim_num <= 0 ? 1 : tensor->get_dim_size(0);
  ret.height = dim_num <= 1 ? 1 : tensor->get_dim_size(1);
  ret.width = dim_num <= 2 ? 1 : tensor->get_dim_size(2);
  ret.channel = dim_num <= 3 ? 1 : tensor->get_dim_size(3);
  ret.fixpos = (int8_t)log2f(scale);
  ret.dtype = vitis::ai::library::DT_INT8;
  ret.name = tensor->get_name();
  auto dims = tensor->get_dims();
  auto index = dims;
  auto size = 0ul;
  // CHECK_LT(dims[0], ret.data.size());
  for (auto batch_idx = 0; batch_idx < dims[0]; ++batch_idx) {
    std::tie(ret.get_data(batch_idx), size) = tb->data({batch_idx, 0, 0, 0});
    CHECK_GE(size, ret.height * ret.width * ret.channel);}

  return ret;
}
  cv::Mat run(cv::Mat& image) override {
  
  auto ret = std::vector<vitis::ai::library::InputTensor>{};
  ret.reserve(inputs.size());
  int c = 0;
  for (auto& t : inputs) {
    ret.emplace_back(convert_tensor_buffer_to_input_tensor(t, scales[c]));
    c++;
  }

  float input_fixed_scale = my_tensor_scale(ret[0]);
  std::vector<float> mean_{104, 117, 123};
  std::vector<float> scale_{1.0, 1.0, 1.0};
  std::vector<float> real_scale{scale_[0] * input_fixed_scale,
                           scale_[1] * input_fixed_scale,
                           scale_[2] * input_fixed_scale};

  auto layer_data = ret[0];
  auto rows = layer_data.height;
  auto cols = layer_data.width;
  auto channels = layer_data.channel;

  ///end add the preprocessing part 
    auto result = dpu_model_->run(image);
    return processor_(image, result, false);
  }
  std::unique_ptr<dpu_model_type_t> dpu_model_;
  const ProcessResult& processor_;
  std::vector<vitis::ai::TensorBuffer *> inputs_;
};
template <typename FactoryMethod, typename ProcessResult>
std::unique_ptr<Filter> create_dpu_filter(const FactoryMethod& factory_method,
                                          const ProcessResult& process_result,
                                          std::vector<vitis::ai::TensorBuffer *>& inputs) {
  using dpu_model_type_t = typename decltype(factory_method())::element_type;
  return std::unique_ptr<Filter>(new DpuFilter<dpu_model_type_t, ProcessResult>(
      factory_method(), process_result, inputs));
}

// Execute dpu filter
struct DpuThread : public MyThread {
  DpuThread(std::unique_ptr<Filter>&& filter, queue_t* queue_in,
            queue_t* queue_out, const std::string& suffix)
      : MyThread{},
        filter_{std::move(filter)},
        queue_in_{queue_in},
        queue_out_{queue_out},
        suffix_{suffix} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT DPU";
  }
  virtual ~DpuThread() {}

  virtual int run() override {
    FrameInfo frame;
    if (!queue_in_->pop(frame, std::chrono::milliseconds(500))) {
      return 0;
    }
    if (filter_) {
      frame.mat = filter_->run(frame.mat);
    }
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "dpu queue size " << queue_out_->size();
    while (!queue_out_->push(frame, std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
    return 0;
  }

  virtual std::string name() override { return std::string("DPU-") + suffix_; }
  std::unique_ptr<Filter> filter_;
  queue_t* queue_in_;
  queue_t* queue_out_;
  std::string suffix_;
};

// Implement sorting thread
struct SortingThread : public MyThread {
  SortingThread(queue_t* queue_in, queue_t* queue_out,
                const std::string& suffix)
      : MyThread{},
        queue_in_{queue_in},
        queue_out_{queue_out},
        frame_id_{0},
        suffix_{suffix},
        fps_{0.0f},
        max_fps_{0.0f} {
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "INIT SORTING";
  }
  virtual ~SortingThread() {}
  virtual int run() override {
    FrameInfo frame;
    frame_id_++;
    auto frame_id = frame_id_;
    auto cond =
        std::function<bool(const FrameInfo&)>{[frame_id](const FrameInfo& f) {
          // sorted by frame id
          return f.frame_id <= frame_id;
        }};
    if (!queue_in_->pop(frame, cond, std::chrono::milliseconds(500))) {
      return 0;
    }
    auto now = std::chrono::steady_clock::now();
    float fps = -1.0f;
    long duration = 0;
    if (!points_.empty()) {
      auto end = points_.back();
      duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - end)
              .count();
      float duration2 = (float)duration;
      float total = (float)points_.size();
      fps = total / duration2 * 1000.0f;
      auto x = 10;
      auto y = 20;
      fps_ = fps;
      frame.fps = fps;
      max_fps_ = std::max(max_fps_, fps_);
      frame.max_fps = max_fps_;
      if (frame.mat.cols > 200)
        cv::putText(frame.mat, std::string("FPS: ") + std::to_string(fps),
                    cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar(20, 20, 180), 2, 1);
    }
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "thread [" << name() << "] "
        << " frame id " << frame.frame_id << " sorting queue size "
        << queue_out_->size() << "   FPS: " << fps;
    points_.push_front(now);
    if (duration > 2000) {  // sliding window for 2 seconds.
      points_.pop_back();
    }
    while (!queue_out_->push(frame, std::chrono::milliseconds(500))) {
      if (is_stopped()) {
        return -1;
      }
    }
    return 0;
  }

  virtual std::string name() override { return std::string{"SORT-"} + suffix_; }
  queue_t* queue_in_;
  queue_t* queue_out_;
  unsigned long frame_id_;
  std::deque<std::chrono::time_point<std::chrono::steady_clock>> points_;
  std::string suffix_;
  float fps_;
  float max_fps_;
};


}// namespace ai
}// namespace vitis

/// show result
    static cv::Mat
    process_result(cv::Mat& image, const vitis::ai::RefineDetResult& result,
                   bool is_jpeg);

/// end show result
using namespace vitis::ai;

int main(int argc, char* argv[]) {
  int start_pos = 2;
  signal(SIGINT, MyThread::signal_handler);

  parse_opt(argc, argv, start_pos);
  {
#if USE_DRM
    cv::VideoCapture video_cap(g_avi_file[0]);

    std::string file_name(g_avi_file[0]);
    bool is_camera =
        file_name.size() == 1 && file_name[0] >= '0' && file_name[0] <= '9';
    if (is_camera) {
      
      gui_layout() = {{0, 0, 640, 360}};
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "Using camera";
    } 
    else {
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "Using file";
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "width " << video_cap.get(3);
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "height " << video_cap.get(4);
      gui_layout() = {{0, 0, (int)video_cap.get(3), (int)video_cap.get(4)}};
    }
    video_cap.release();
#else
    cv::moveWindow(std::string{"CH-"} + std::to_string(0), 500, 500);
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "Window name " << std::string{"CH-"} + std::to_string(0);
#endif
    auto channel_id = 0;
    std::vector<std::string>g_avi_file{"0"};
    auto decode_queue = std::unique_ptr<queue_t>{new queue_t{5}};
    auto decode_thread = std::unique_ptr<DecodeThread>(
        new DecodeThread{channel_id, g_avi_file[0], decode_queue.get()});
    auto dpu_thread = std::vector<std::unique_ptr<DpuThread>>{};
    auto sorting_queue =
        std::unique_ptr<queue_t>(new queue_t(5 * g_num_of_threads[0]));
    auto gui_thread = GuiThread::instance();
    auto gui_queue = gui_thread->getQueue();
    const auto model_name = "refinedet_pruned_0_8";
    for (int i = 0; i < g_num_of_threads[0]; ++i) {
      dpu_thread.emplace_back(new DpuThread(
          create_dpu_filter(
              [model_name]() {
                return vitis::ai::RefineDet::create(model_name, 0);
              },
              process_result,inputs),
          decode_queue.get(), sorting_queue.get(), std::to_string(i)));
    }

    auto sorting_thread = std::unique_ptr<vitis::ai::SortingThread>(
        new SortingThread(sorting_queue.get(), gui_queue, std::to_string(0)));
    // start everything
    MyThread::start_all();
    gui_thread->wait();
    MyThread::stop_all();
    MyThread::wait_all();
     }
  LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "BYEBYE";
  return 0;
}




static inline void usage_jpeg(const char* progname) {
  std::cout << "usage : " << progname << " <img_url> [<img_url> ...]"
            << std::endl;
}


static cv::Mat process_result(cv::Mat& image,
                              const vitis::ai::RefineDetResult& result,
                              bool is_jpeg) {
  auto img = image.clone();
  for (auto& box : result.bboxes) {
    float x = box.x * (img.cols);
    float y = box.y * (img.rows);
    int xmin = x;
    int ymin = y;
    int xmax = x + (box.width) * (img.cols);
    int ymax = y + (box.height) * (img.rows);
    float score = box.score;
    xmin = std::min(std::max(xmin, 0), img.cols);
    xmax = std::min(std::max(xmax, 0), img.cols);
    ymin = std::min(std::max(ymin, 0), img.rows);
    ymax = std::min(std::max(ymax, 0), img.rows);

    LOG_IF(INFO, is_jpeg) << "RESULT2: "
                          << "\t" << xmin << "\t" << ymin << "\t" << xmax
                          << "\t" << ymax << "\t" << score << "\n";
    auto label = 2;
    if (label == 1) {
      cv::rectangle(img, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(0, 255, 0), 1, 1, 0);
    } else if (label == 2) {
      cv::rectangle(img, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(255, 0, 0), 1, 1, 0);
    } else if (label == 3) {
      cv::rectangle(img, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(0, 0, 255), 1, 1, 0);
    }
  }

  return img;
}

