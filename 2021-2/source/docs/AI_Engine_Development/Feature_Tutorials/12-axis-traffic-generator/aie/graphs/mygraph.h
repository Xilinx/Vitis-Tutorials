/**********
© Copyright 2021 Xilinx, Inc.

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
#include <adf.h>
#include "include.h"
#include "../kernels/kernels.h"
#include "fft_ifft_dit_1ch_graph.hpp"


using namespace adf;

template <const int IN_SIZE>
class mygraph : public graph {

  private:
    kernel  passth_i;
    xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<cint16, cint16, 128, 1, 1> fwd_fft;

  public:
    port<input>   in;
    port<output>  out;

    mygraph()
    {
      // specify kernels
      passth_i                   = kernel::create(passth_cint16);
      
      source(passth_i)           = "../kernels/pass_thru.cc";
      runtime<ratio>(passth_i)   = 0.9;
      
      /* No FFT */
      /*
      connect< stream, window<IN_SIZE*4> > net0  (in, passth_i.in[0]);
      connect< window<IN_SIZE * 4> > net1 (passth_i.out[0], out);
      */

      connect< stream, window<IN_SIZE*4> > net0  (in, passth_i.in[0]);
      connect< window<IN_SIZE*4> > net1 (passth_i.out[0], fwd_fft.in[0]);
      connect< window<IN_SIZE*4> > net2 (fwd_fft.out[0], out);
      
    }
};

template <const int IN_SIZE>
class mygraph_float : public graph {

  private:
    kernel  passth_i;
    //xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<cint16, cint16, 128, 1> fwd_fft;

  public:
    port<input>   in;
    port<output>  out;

    mygraph_float()
    {
      // specify kernels
      passth_i                   = kernel::create(passth_cfloat);
      
      source(passth_i)           = "../kernels/pass_thru.cc";
      runtime<ratio>(passth_i)   = 0.9;
      

      /* No FFT */	
      connect< stream, window<IN_SIZE*4> > net0  (in, passth_i.in[0]);
      connect< window<IN_SIZE * 4> > net1 (passth_i.out[0], out);
      

      /*
      connect< stream, window<IN_SIZE*4> > net0  (in, passth_i.in[0]);
      connect< window<IN_SIZE*4> > net1 (passth_i.out[0], fwd_fft.in);
      connect< window<IN_SIZE*4> > net2 (fwd_fft.out, out);
      */

    }
};


