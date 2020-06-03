#include <fcntl.h>
#include <glog/logging.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <type_traits>
#include <vector>
#include <vitis/ai/bounded_queue.hpp>
#include <vitis/ai/nnpp/refinedet.hpp>
#include <vitis/ai/refinedet.hpp>

#include "decodethread.hpp"
#include "dputhread.hpp"
#include "filter.hpp"
#include "frameinfo.hpp"
#include "guithread.hpp"
#include "mythread.hpp"
#include "sortthread.hpp"
#include <V4l2Capture.hpp>
#include <V4l2Device.hpp>

#ifndef USE_DRM
#define USE_DRM 0
#endif
#if USE_DRM
#include <vitis/ai/dpdrm.hpp>
#endif

// get runners parameters
static std::vector<int> g_num_of_threads;
static std::vector<std::string> g_avi_file;
static int idx = 0;

using queue_t = vitis::ai::BoundedQueue<vitis::ai::FrameInfo>;
// usage video
inline void usage_video(const char* progname) {
  std::cout << "usage: " << progname << "      -t <num_of_threads>\n"
            << "      <video file name>\n"
            << std::endl;
  return;
}

/*// set the wallpaper
inline cv::Mat& gui_background() {
  static cv::Mat img;
  return img;
}*/

// parse args
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

static cv::Mat process_result(cv::Mat& image,
                              const vitis::ai::RefineDetResult& result,
                              bool is_jpeg);

using namespace vitis::ai;
int main(int argc, char* argv[]) {
  int start_pos = 2;
  signal(SIGINT, vitis::ai::MyThread::signal_handler);
  parse_opt(argc, argv, start_pos);
  {
#if USE_DRM
    cv::VideoCapture video_cap(g_avi_file[0]);

    std::string file_name(g_avi_file[0]);
    bool is_camera =
        file_name.size() == 1 && file_name[0] >= '0' && file_name[0] <= '9';
    if (is_camera) {
      // cv::Rect rect1(0,0,640,360);
      // GuiThread::rects.emplace_back(rect1);
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "Using camera";
    } else {
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "Using file";
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "width " << video_cap.get(3);
      LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO)) << "height " << video_cap.get(4);
      cv::Rect rect2(0, 0, (int)video_cap.get(3), (int)video_cap.get(4));
      //  GuiThread::rects.emplace_back(rect2);
    }
    video_cap.release();
#else
    cv::moveWindow(std::string{"CH-"} + std::to_string(0), 500, 500);
    LOG_IF(INFO, ENV_PARAM(DEBUG_DEMO))
        << "Window name " << std::string{"CH-"} + std::to_string(0);
#endif
    auto runners_ = vitis::ai::DpuRunner::create_dpu_runner(
        "/usr/share/vitis_ai_library/models/refinedet_pruned_0_8");
    auto inputs = dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[idx].get())
                      ->get_inputs();
    auto scales = dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[idx].get())
                      ->get_input_scale();

    auto channel_id = 0;
    std::vector<std::string> g_avi_file{"0"};
    auto decode_queue = std::unique_ptr<queue_t>{new queue_t{5}};
    

    auto decode_thread = std::unique_ptr<DecodeThread>(
        new DecodeThread{channel_id, g_avi_file[0], decode_queue.get()});
    auto dpu_thread = std::vector<std::unique_ptr<DpuThread>>{};
    auto sorting_queue =
        std::unique_ptr<queue_t>(new queue_t(5 * g_num_of_threads[0]));
    auto gui_thread = vitis::ai::GuiThread::instance();
    auto gui_queue = gui_thread->getQueue();
    const auto model_name = "refinedet_pruned_0_8";
    for (int i = 0; i < g_num_of_threads[0]; ++i) {
      dpu_thread.emplace_back(new DpuThread(
          create_dpu_filter(
              [model_name]() {
                return vitis::ai::RefineDet::create(model_name, 0);
              },
              process_result, inputs),
          decode_queue.get(),sorting_queue.get(), std::to_string(i)));
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

static cv::Mat process_result(cv::Mat& image_show,
                              const vitis::ai::RefineDetResult& result,
                              bool is_jpeg) {
  //auto img = image.clone();
  for (auto& box : result.bboxes) {
    float x = box.x * (image_show.cols);
    float y = box.y * (image_show.rows);
    int xmin = x;
    int ymin = y;
    int xmax = x + (box.width) * (image_show.cols);
    int ymax = y + (box.height) * (image_show.rows);
    float score = box.score;
    xmin = std::min(std::max(xmin, 0), image_show.cols);
    xmax = std::min(std::max(xmax, 0), image_show.cols);
    ymin = std::min(std::max(ymin, 0), image_show.rows);
    ymax = std::min(std::max(ymax, 0), image_show.rows);

    LOG_IF(INFO, is_jpeg) << "RESULT2: "
                          << "\t" << xmin << "\t" << ymin << "\t" << xmax
                          << "\t" << ymax << "\t" << score << "\n";
    auto label = 2;
    if (label == 1) {
      cv::rectangle(image_show, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(0, 255, 0), 1, 1, 0);
    } else if (label == 2) {
      cv::rectangle(image_show, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(255, 0, 0), 1, 1, 0);
    } else if (label == 3) {
      cv::rectangle(image_show, cv::Point(xmin, ymin), cv::Point(xmax, ymax),
                    cv::Scalar(0, 0, 255), 1, 1, 0);
    }
  }

  return image_show;
}
