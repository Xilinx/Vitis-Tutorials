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
#ifndef FILTER
#define FILTER
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <type_traits>
#include <vart/dpu/vitis_dpu_runner_factory.hpp>
#include <vart/dpu/dpu_runner_ext.hpp>
#include <vart/tensor_buffer.hpp>
#include <vector>
#include <vitis/ai/library/tensor.hpp>

#include "frameinfo.hpp"

namespace vitis {
namespace ai {

/**
 * @brief base struct Filter
 *
 */
struct Filter {
  explicit Filter() {}
  virtual ~Filter() {}
  virtual cv::Mat run(FrameInfo& frame) = 0;
};

/**
 * @brief DpuFilter the element required by DpuThread
 *
 * @tparam dpu_model_type_t dpu_model struct type such as vitis::ai::Refinedet
 * @tparam ProcessResult Function to process the result, such as drawing the
 * overlay
 */
template <typename dpu_model_type_t, typename ProcessResult>
struct DpuFilter : public Filter {
  /**
   * @brief Construct a new Dpu Filter object
   *
   * @param dpu_model dpu_model_type_t in vitis ai library contains the method
   * of dpu runner XRT based.
   * @param processor process the result, such as drawing the overlay.
   * @param inputs the inputs needed by DPU pre-processing.
   */
  DpuFilter(std::unique_ptr<dpu_model_type_t>&& dpu_model,
            const ProcessResult& processor)
      : Filter{}, dpu_model_{std::move(dpu_model)}, processor_{processor} {}

  virtual ~DpuFilter() {}

  /**
   * @brief Function to mean and scale the input image and copy the data into
   * input address of DPU.
   *
   * @param input the input image cv::Mat.data
   * @param rows  rows of input image
   * @param cols  colums of input image
   * @param channels channels of input image
   * @param stride  the step of input image cv::Mat.step
   * @param mean    pre-processing mean value
   * @param scale   pre-processing scale value
   *
   */
  void NormalizeInputData(uint8_t* input, int rows, int cols, int channels,
                          int stride, const std::vector<float>& mean,
                          const std::vector<float>& scale) {
    for (auto h = 0; h < rows; ++h) {
      for (auto w = 0; w < cols; ++w) {
        for (auto c = 0; c < channels; ++c) {
          auto value = (input[h * stride + w * channels + c] * 1.0f - mean[c]) *
                       scale[c];
          input[h * cols * channels + w * channels + c] = (uchar)value;
        }
      }
    }
  }
  /**
   * @brief Get the fixpos from the InputTensor.
   *
   * @param tensor get from DpuRunnerExt
   * @return float real_scale= scale*fixpos
   */
  float my_tensor_scale(vitis::ai::library::InputTensor& tensor) {
    return std::exp2f(1.0f * (float)tensor.fixpos);
  }
  /*
  vitis::ai::library::InputTensor convert_tensor_buffer_to_input_tensor(
      vart::TensorBuffer* tb, float scale) {
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

  /**
   * @brief Function to run dpu task
   *
   * @param frame Input Frame
   * @return cv::Mat the image with result.
   */
  cv::Mat run(FrameInfo& frame) override {
    auto result = dpu_model_->run(frame.dpu_mat);
    return processor_(frame.show_mat, result, false);
  }
  /**
   * @brief In vitis ai library different algorithm model was wrappered in a
   * different struct, contains the pre-processing/run DPU task/post-processing
   *
   */
  std::unique_ptr<dpu_model_type_t> dpu_model_;
  /**
   * @brief Function pointer to process the result, such as drawing overlay.
   *
   */
  const ProcessResult& processor_;
  /**
   * @brief Dpu runner index
   *
   */
  static int id;
};
template <typename dpu_model_type_t, typename ProcessResult>
int DpuFilter<dpu_model_type_t, ProcessResult>::id = 0;

/**
 * @brief Create a dpu filter object
 *
 * @tparam FactoryMethod method template
 * @tparam ProcessResult  process function template
 * @param factory_method  the method to create an dpu_model struct
 * @param process_result  the process result function pointer
 * @return std::unique_ptr<vitis::ai::Filter>
 */
template <typename FactoryMethod, typename ProcessResult>
std::unique_ptr<vitis::ai::Filter> create_dpu_filter(
    const FactoryMethod& factory_method, const ProcessResult& process_result) {
  using dpu_model_type_t = typename decltype(factory_method())::element_type;
  return std::unique_ptr<Filter>(new DpuFilter<dpu_model_type_t, ProcessResult>(
      factory_method(), process_result));
}

}  // namespace ai
}  // namespace vitis

#endif
