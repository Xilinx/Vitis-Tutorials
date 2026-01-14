//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <iostream>
#include "ifft4096_2d_graph.h"

class dut_graph : public graph {
public:
  ifft4096_2d_graph dut;
  std::array< input_plio,ifft4096_2d_graph::TP_SSR>  front_i;
  std::array< input_plio,ifft4096_2d_graph::TP_SSR>  back_i;
  std::array<output_plio,ifft4096_2d_graph::TP_SSR>  front_o;
  std::array<output_plio,ifft4096_2d_graph::TP_SSR>  back_o;
  dut_graph(void)
  {
    for ( unsigned ff=0; ff < ifft4096_2d_graph::TP_SSR; ff++) {
      std::string fname_i0 = "data/front_i_" + std::to_string(ff) + ".txt";
      std::string fname_i1 = "data/back_i_" + std::to_string(ff) + ".txt";
      std::string fname_o0 = "data/front_o_" + std::to_string(ff) + ".txt";
      std::string fname_o1 = "data/back_o_" + std::to_string(ff) + ".txt";
      std::string pname_i0 = "PLIO_front_in_" + std::to_string(ff) + "";
      std::string pname_i1 = "PLIO_back_in_" + std::to_string(ff) + "";
      std::string pname_o0 = "PLIO_front_out_" + std::to_string(ff) + "";
      std::string pname_o1 = "PLIO_back_out_" + std::to_string(ff) + "";
      front_i[ff] =  input_plio::create(pname_i0,plio_64_bits,fname_i0);
      back_i[ff] =  input_plio::create(pname_i1,plio_64_bits,fname_i1);
      front_o[ff] = output_plio::create(pname_o0,plio_64_bits,fname_o0);
      back_o[ff] = output_plio::create(pname_o1,plio_64_bits,fname_o1);
      connect<>( front_i[ff].out[0],  dut.front_i[ff]   );
      connect<>( back_i[ff].out[0],  dut.back_i[ff]   );
      connect<>( dut.front_o[ff],     front_o[ff].in[0] );
      connect<>( dut.back_o[ff],     back_o[ff].in[0] );

#ifndef __X86SIM__
      if (ff<ifft4096_2d_graph::TP_SSR/2){
            location<kernel>   (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])           = tile(15+ff,1);
            location<kernel>   (dut.ifft4096_2d.m_fftTwRotKernels[ff])                                  = location<kernel>(dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0]);
            single_buffer      (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>   (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])     = bank(15+ff,1,0);
            single_buffer      (dut.ifft4096_2d.m_fftTwRotKernels[ff].out[0]);
            location<buffer>   (dut.ifft4096_2d.m_fftTwRotKernels[ff].out[0])                           = bank(15+ff,1,2);
            single_buffer      (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0]);
            location<buffer>   (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])    = bank(15+ff,1,1);
            location<stack>    (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])           = bank(15+ff,1,3);
            location<parameter>(dut.ifft4096_2d.m_fftTwRotKernels[ff].param[0])                         = bank(15+ff,1,3);
            location<parameter>(dut.ifft4096_2d.m_fftTwRotKernels[ff].param[1])                         = bank(15+ff,1,3);
            location<PLIO>(front_i[ff])                                                                 = shim(16);
            location<PLIO>(front_o[ff])                                                                 = shim(16);

            location<kernel>(dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = tile(11+ff,1);
            single_buffer   (dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>(dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])         = bank(11+ff,1,0);
            //single_buffer   (dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0]);
            location<buffer>(dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])        = {bank(11+ff,1,1),bank(11+ff,1,2)};
            location<stack> (dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = bank(11+ff,1,3);
            location<PLIO>(back_i[ff])                                                                  = shim(14);
            location<PLIO>(back_o[ff])                                                                  = shim(14);
      }
      else{
            location<kernel>(dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])              = tile(15+ff-4,2);
            location<kernel>(dut.ifft4096_2d.m_fftTwRotKernels[ff])                                     = location<kernel>(dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0]);
            single_buffer   (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>(dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])        = bank(15+ff-4,2,0);
            single_buffer   (dut.ifft4096_2d.m_fftTwRotKernels[ff].out[0]);
            location<buffer>(dut.ifft4096_2d.m_fftTwRotKernels[ff].out[0])                              = bank(15+ff-4,2,2);
            single_buffer   (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0]);
            location<buffer>(dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])       = bank(15+ff-4,2,1);
            location<stack> (dut.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])              = bank(15+ff-4,2,3);
            location<parameter>(dut.ifft4096_2d.m_fftTwRotKernels[ff].param[0])                         = bank(15+ff-4,2,3);
            location<parameter>(dut.ifft4096_2d.m_fftTwRotKernels[ff].param[1])                         = bank(15+ff-4,2,3);
            location<PLIO>(front_i[ff])                                                                 = shim(15);
            location<PLIO>(front_o[ff])                                                                 = shim(15);

            location<kernel>(dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = tile(11+ff-4,2);
            single_buffer   (dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>(dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])         = bank(11+ff-4,2,0);
            //single_buffer   (dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0]);
            location<buffer>(dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])        = {bank(11+ff-4,2,1),bank(11+ff-4,2,2)};
            location<stack> (dut.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = bank(11+ff-4,2,3);
            location<PLIO>(back_i[ff])                                                                  = shim(13);
            location<PLIO>(back_o[ff])                                                                  = shim(13);
      }
#endif

    }
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(8);
  aie_dut.end();

  return 0;
}

