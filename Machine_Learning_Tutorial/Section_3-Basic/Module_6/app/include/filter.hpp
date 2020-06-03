#ifndef FILTER
#define FILTER
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <vart/dpu/dpu_runner.hpp>
#include <vart/dpu/dpu_runner_ext.hpp>
#include <vart/dpu/tensor_buffer.hpp>
#include <memory>
#include <vitis/ai/library/tensor.hpp>
#include <type_traits>

namespace vitis{
    namespace ai{

struct Filter {
  explicit Filter() {}
  virtual ~Filter() {}
  virtual cv::Mat run(FrameInfo& frame) = 0;
};

template <typename dpu_model_type_t, typename ProcessResult>
struct DpuFilter : public Filter {
  DpuFilter(std::unique_ptr<dpu_model_type_t>&& dpu_model,
            const ProcessResult& processor, std::vector<vitis::ai::TensorBuffer *>& inputs)
      : Filter{}, dpu_model_{std::move(dpu_model)}, processor_{processor},inputs_{inputs} {



      }


  virtual ~DpuFilter() {}

  
void NormalizeInputData(uint8_t* input, int rows, int cols, int channels,
                        int stride, const std::vector<float>& mean,
                        const std::vector<float>& scale){
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

  float my_tensor_scale(vitis::ai::library::InputTensor& tensor){return std::exp2f(1.0f * (float)tensor.fixpos);} 
  vitis::ai::library::InputTensor convert_tensor_buffer_to_input_tensor(
  vitis::ai::TensorBuffer* tb, float scale){
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

  
  virtual cv::Mat run(FrameInfo& frame) override
  {
    auto runners_ = vitis::ai::DpuRunner::create_dpu_runner("/usr/share/vitis_ai_library/models/refinedet_pruned_0_8");
    auto inputs =
    dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[id].get())->get_inputs();
    auto scales = dynamic_cast<vart::dpu::DpuRunnerExt*>(runners_[id].get())
              ->get_input_scale();
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
      auto result = dpu_model_->run(frame.dpu_mat);
      return processor_(frame.show_mat, result, false);
  }


  std::unique_ptr<dpu_model_type_t> dpu_model_;
  const ProcessResult& processor_;
  std::vector<vitis::ai::TensorBuffer *> inputs_;
  static int id;
};
template<typename dpu_model_type_t, typename ProcessResult>
int DpuFilter<dpu_model_type_t, ProcessResult>::id=0;

//create dpu filter

template <typename FactoryMethod, typename ProcessResult>
std::unique_ptr<vitis::ai::Filter> create_dpu_filter(const FactoryMethod& factory_method,
                                          const ProcessResult& process_result,
                                          std::vector<vitis::ai::TensorBuffer *>& inputs) {
  using dpu_model_type_t = typename decltype(factory_method())::element_type;
  return std::unique_ptr<Filter>(new DpuFilter<dpu_model_type_t, ProcessResult>(
      factory_method(), process_result, inputs));
}


  }//end ai
}//end vitis

#endif
