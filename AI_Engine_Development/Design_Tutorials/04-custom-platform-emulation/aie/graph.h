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

class clipped : public graph {  

  private:
    kernel interpolator;
    kernel classify;
   
  public:
    //kernel clip;
    port<input> in;
    port<output> clip_in;
    port<output> out;
    port<input> clip_out;

    clipped() {
      interpolator = kernel::create(fir_27t_sym_hb_2i);
      //clip         = kernel::create(polar_clip);
      classify     = kernel::create(classifier);

      //fabric<pl>(clip);

      connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in, interpolator.in[0]);
      //connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip.in[0]);
      connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip_in);
      //connect< stream >(clip.out[0], classify.in[0]);
      connect< stream >(clip_out, classify.in[0]);
      connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE> >(classify.out[0], out);

      std::vector<std::string> myheaders;
      myheaders.push_back("include.h");

      adf::headers(interpolator) = myheaders;
      adf::headers(classify) = myheaders;

      source(interpolator) = "kernels/interpolators/hb27_2i.cc";
      //source(clip)         = "polar_clip.cpp";
      source(classify)    = "kernels/classifiers/classify.cc";

      runtime<ratio>(interpolator) = 0.8;
      runtime<ratio>(classify) = 0.8;
    };
};

#endif /**********__GRAPH_H__**********/
