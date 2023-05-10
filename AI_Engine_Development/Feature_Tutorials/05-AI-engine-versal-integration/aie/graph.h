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

class clipped : public adf::graph {  

  private:
    kernel interpolator;
    kernel classify;
    kernel clip;
   
  public:
      
    //Declaring PLIO objects
    adf::input_plio in;
    adf::output_plio out;

    clipped() {
      //kernel instantiation
      interpolator = adf::kernel::create(fir_27t_sym_hb_2i);
      clip         = adf::kernel::create(polar_clip);
      classify     = adf::kernel::create(classifier);
      
      adf::source(interpolator) = "kernels/interpolators/hb27_2i.cc";
      adf::source(clip)         = "kernels/polar_clip.cpp";
      adf::source(classify)    = "kernels/classifiers/classify.cc";
      
      //Input PLIO object that specifies the file containing input data
      in = adf::input_plio::create("DataIn1", adf::plio_32_bits,"data/input.txt");
      
      //Output PLIO object that specifies the file containing output data
      out = adf::output_plio::create("DataOut1",adf::plio_32_bits, "data/output.txt");

      //connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in.out[0], interpolator.in[0]);
      //connection between buffer port 'in' and 'input port of the kernel
      connect(in.out[0], interpolator.in[0]);

      //connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip.in[0]);
      connect(interpolator.out[0], clip.in[0]);
      
      //stream connection between kernels
      //connect< stream>(clip.out[0], classify.in[0]);
      connect(clip.out[0], classify.in[0]);
      
      //connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE> >(classify.out[0], out.in[0]);
      //connection between output buffer port of the kernel and port 'out'
      connect(classify.out[0], out.in[0]);

      //Dimensions - Specifying the size of the buffers
      dimensions(interpolator.in[0]) = {INTERPOLATOR27_INPUT_SAMPLES};
      dimensions(interpolator.out[0]) = {POLAR_CLIP_INPUT_SAMPLES};
      dimensions(classify.out[0]) = {CLASSIFIER_OUTPUT_SAMPLES};


      std::vector<std::string> myheaders;
      myheaders.push_back("include.h");

      adf::headers(interpolator) = myheaders;
      adf::headers(classify) = myheaders;
      
      //Specifying core usage fraction for a kernel
      runtime<ratio>(interpolator) = 0.8;
      runtime<ratio>(clip) = 0.8;
      runtime<ratio>(classify) = 0.8;
    };
};

#endif /**********__GRAPH_H__**********/
