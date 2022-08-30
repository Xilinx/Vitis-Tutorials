/**********
© Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/

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

      //Window connection between port 'in' and input port of the kernel
      connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in.out[0], interpolator.in[0]);

      connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip.in[0]);
      
      //stream connection between kernels
      connect< stream>(clip.out[0], classify.in[0]);
      
      //window connection between output port of the kernel and port 'out'
      connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE> >(classify.out[0], out.in[0]);

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
