/**********
© Copyright 2022 Xilinx, Inc.

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
    // Graph ports
    port<input> in;
    port<output> out;

    // polar_clip connection ports
    port<output> clip_in;
    port<input> clip_out;

    clipped() {
      interpolator = kernel::create(fir_27t_sym_hb_2i);
      classify     = kernel::create(classifier);

      connect< window<INTERPOLATOR27_INPUT_BLOCK_SIZE, INTERPOLATOR27_INPUT_MARGIN> >(in, interpolator.in[0]);
      connect< window<POLAR_CLIP_INPUT_BLOCK_SIZE>, stream >(interpolator.out[0], clip_in);
      connect< stream >(clip_out, classify.in[0]);
      connect< window<CLASSIFIER_OUTPUT_BLOCK_SIZE> >(classify.out[0], out);

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


class Lab8 : public graph {
public:
    // Connection to the testbench:
    //  - standard one with files
    //  - or External Traffic Generator
    input_plio plin;
    output_plio clip_in;
    input_plio clip_out;
    output_plio plout;

    clipped AIE;

    Lab8()
    {
#ifdef EXTERNAL_IO
        plin = input_plio::create("DataIn1",adf::plio_32_bits);
        clip_in = output_plio::create("clip_in",adf::plio_32_bits);
        clip_out = input_plio::create("clip_out",adf::plio_32_bits);
        plout = output_plio::create("DataOut1",adf::plio_32_bits);
#else
        plin = input_plio::create("DataIn1",adf::plio_32_bits,"data/mm2s.txt");
        clip_in = output_plio::create("clip_in",adf::plio_32_bits, "data/polar_clip_in.txt");
        clip_out = input_plio::create("clip_out",adf::plio_32_bits,"data/polar_clip_out.txt");
        plout = output_plio::create("DataOut1",adf::plio_32_bits, "data/DataOut1.txt");
#endif

        connect<> (plin.out[0],AIE.in);
        connect<> (AIE.clip_in,clip_in.in[0]);
        connect<> (clip_out.out[0],AIE.clip_out);
        connect<> (AIE.out,plout.in[0]);

    };

};

#endif /**********__GRAPH_H__**********/
