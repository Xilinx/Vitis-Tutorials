#ifndef FRAMEINFO
#define FRAMEINFO

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
namespace vitis{
    namespace ai{
      
struct FrameInfo {
  int channel_id;
  unsigned long frame_id;
  cv::Mat dpu_mat;
  cv::Mat show_mat;
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


}
}



#endif
