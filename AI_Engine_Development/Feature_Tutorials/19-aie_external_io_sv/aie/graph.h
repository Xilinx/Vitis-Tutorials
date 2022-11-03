#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "include.h"
#include "kernels.h"

using namespace adf;

class clipped : public graph {  

  private:
    kernel interpolator;
    kernel classify;

  public:
     adf::input_plio in0;
     adf::input_plio pl_in0;
     adf::output_plio out0;
     adf::output_plio pl_out0;
  
    clipped() {
    
      in0 = adf::input_plio::create("in_interpolator",adf::plio_32_bits,"data/in_interpolator.txt");
      pl_in0 = adf::input_plio::create("in_classifier",adf::plio_32_bits);
      out0 = adf::output_plio::create("out_interpolator",adf::plio_32_bits);
      pl_out0 = adf::output_plio::create("out_classifier",adf::plio_32_bits,"data/out_classifier.txt");

      interpolator = kernel::create(fir_27t_sym_hb_2i);
      classify     = kernel::create(classifier);

      connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in0.out[0], interpolator.in[0]);

      connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], out0.in[0]);

      connect< stream, stream >(pl_in0.out[0], classify.in[0]);
      connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE>, stream >(classify.out[0], pl_out0.in[0]);
       
      std::vector<std::string> myheaders;
      myheaders.push_back("include.h");

      adf::headers(interpolator) = myheaders;
      adf::headers(classify) = myheaders;

      source(interpolator) = "kernels/interpolators/hb27_2i.cc";
      source(classify)    = "kernels/classifiers/classify.cc";

      runtime<ratio>(interpolator) = 0.8;
      runtime<ratio>(classify) = 0.8;
    };
};

#endif /* __GRAPH_H__ */
