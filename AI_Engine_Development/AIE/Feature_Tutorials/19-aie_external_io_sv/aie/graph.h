/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

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
     adf::input_plio in;
     adf::input_plio clip_out; 
     adf::output_plio clip_in;
     adf::output_plio out; 
  
    clipped() {
    
      in = adf::input_plio::create("in_interpolator",adf::plio_32_bits,"data/in_interpolator.txt");
      clip_out = adf::input_plio::create("in_classifier",adf::plio_32_bits);        
      clip_in = adf::output_plio::create("out_interpolator",adf::plio_32_bits); 
      out = adf::output_plio::create("out_classifier",adf::plio_32_bits,"data/out_classifier.txt");

      interpolator = kernel::create(fir_27t_sym_hb_2i);
      classify     = kernel::create(classifier);

      adf::source(interpolator) = "kernels/interpolators/hb27_2i.cc";
      adf::source(classify)    = "kernels/classifiers/classify.cc";

      connect(in.out[0], interpolator.in[0]);
      connect(interpolator.out[0], clip_in.in[0]);
      connect(clip_out.out[0], classify.in[0]);
      connect(classify.out[0],out.in[0]);

      dimensions(interpolator.in[0]) = {INTERPOLATOR27_INPUT_SAMPLES};
      dimensions(interpolator.out[0]) = {INTERPOLATOR27_OUTPUT_SAMPLES};
      dimensions(classify.out[0]) = {CLASSIFIER_OUTPUT_SAMPLES};
      std::vector<std::string> myheaders;
      myheaders.push_back("include.h");

      adf::headers(interpolator) = myheaders;
      adf::headers(classify) = myheaders;
      runtime<ratio>(interpolator) = 0.8;
      runtime<ratio>(classify) = 0.8;

    };
};

#endif /* __GRAPH_H__ */
