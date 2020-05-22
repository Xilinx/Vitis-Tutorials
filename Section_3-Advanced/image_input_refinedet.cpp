#include <glog/logging.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vart/dpu/dpu_runner.hpp>
#include <vart/dpu/dpu_runner_ext.hpp>
#include <vart/dpu/tensor_buffer.hpp>
#include <vector>
#include <vitis/ai/env_config.hpp>
#include <vitis/ai/library/tensor.hpp>
#include <vitis/ai/refinedet.hpp>
DEF_ENV_PARAM(DEBUG_DPBASE, "0");
using namespace std;
using namespace std;
static cv::Mat process_result(cv::Mat& image,
                              const vitis::ai::RefineDetResult& result,
                              bool is_jpeg);
static vitis::ai::library::InputTensor convert_tensor_buffer_to_input_tensor(
    vitis::ai::TensorBuffer* tb, float scale);
float my_tensor_scale(vitis::ai::library::InputTensor& tensor);
void NormalizeInputData(uint8_t* input, int rows, int cols, int channels,
                        int stride, const std::vector<float>& mean,
                        const std::vector<float>& scale);

int main(int argc, char* argv[]) {
  int idx = 0;
  string model_dir = argv[1];
  auto runners_ = vitis::ai::DpuRunner::create_dpu_runner(model_dir);
  auto inputs =
      dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[idx].get())->get_inputs();
  auto scales = dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[idx].get())
                    ->get_input_scale();
  auto ret = std::vector<vitis::ai::library::InputTensor>{};
  ret.reserve(inputs.size());
  int c = 0;
  for (auto& t : inputs) {
    ret.emplace_back(convert_tensor_buffer_to_input_tensor(t, scales[c]));
    LOG_IF(INFO, ENV_PARAM(DEBUG_DPBASE))
        << "input tensor[" << c << "]: " << ret.back();
    c++;
  }
  float input_fixed_scale = my_tensor_scale(ret[0]);

  auto model_name = "refinedet_pruned_0_8";
  const auto& model = [model_name] {
    return vitis::ai::RefineDet::create(model_name, false);
  }();
  cv::Mat image;
  for (auto& x : scales) {
    cout << x << endl;
  }

  auto input_image = cv::imread("sample_refinedet.jpg");
  auto size = cv::Size(model->getInputWidth(), model->getInputHeight());
  if (size != input_image.size()) {
    cv::resize(input_image, image, size);
  } else {
    image = input_image;
  }

  // mean and scale
  std::vector<float> mean_{104, 117, 123};
  std::vector<float> scale_{1.0, 1.0, 1.0};
  vector<float> real_scale{scale_[0] * input_fixed_scale,
                           scale_[1] * input_fixed_scale,
                           scale_[2] * input_fixed_scale};

  auto layer_data = ret[0];
  auto rows = layer_data.height;
  auto cols = layer_data.width;
  auto channels = layer_data.channel;

  NormalizeInputData(image.data, rows, cols, channels, image.step, mean_,
                     real_scale);

  auto result = model->run(image);
  process_result(image, result, true);

  return 0;
}

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

float my_tensor_scale(vitis::ai::library::InputTensor& tensor) {
  return std::exp2f(1.0f * (float)tensor.fixpos);
}

static vitis::ai::library::InputTensor convert_tensor_buffer_to_input_tensor(
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
    CHECK_GE(size, ret.height * ret.width * ret.channel);
  }

  return ret;
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
