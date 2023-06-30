/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
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
	
    input_plio  in;
    output_plio out;	

    output_plio clip_in;
    input_plio clip_out;

    clipped() {
      in  = input_plio::create("DataIn1", plio_32_bits,"data/input.txt");
      out = output_plio::create("DataOut1", plio_32_bits,"data/output.txt");
      
      clip_out = input_plio::create("polar_clip_in",plio_32_bits,"data/input1.txt");
      clip_in = output_plio::create("polar_clip_out", plio_32_bits,"data/output1.txt");

      interpolator = kernel::create(fir_27t_sym_hb_2i);
      classify     = kernel::create(classifier);

      connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in.out[0], interpolator.in[0]);
      connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip_in.in[0]);
      
      connect< stream >(clip_out.out[0], classify.in[0]);
      connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE> >(classify.out[0], out.in[0]);
       
      std::vector<std::string> myheaders;
      myheaders.push_back("include.h");

      adf::headers(interpolator) = myheaders;
      adf::headers(classify) = myheaders;

      source(interpolator) = "hb27_2i.cc";
      source(classify)    = "classify.cc";

      runtime<ratio>(interpolator) = 0.8;
      runtime<ratio>(classify) = 0.8;
    };
};

#endif /* __GRAPH_H__ */
