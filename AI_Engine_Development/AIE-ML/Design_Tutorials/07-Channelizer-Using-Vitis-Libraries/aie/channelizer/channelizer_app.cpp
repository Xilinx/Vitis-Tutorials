//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <iostream>
#include "channelizer_graph.h"

class dut_graph : public graph {
public:
  channelizer_graph dut;
  std::array< input_plio,firbank_graph::NPORT_I>     sig_i;
  std::array<output_plio,firbank_graph::NPORT_O>     sig_o;
  std::array< input_plio,ifft4096_2d_graph::TP_SSR>  front_i;
  std::array< input_plio,ifft4096_2d_graph::TP_SSR>   back_i;
  std::array<output_plio,ifft4096_2d_graph::TP_SSR>  front_o;
  std::array<output_plio,ifft4096_2d_graph::TP_SSR>   back_o;
  dut_graph(void)
  {
    // Goal is to reduce routing congestion by preventing routing of all split/merge units to all columns
    //
    // Col:    0    1   2    3   4   5  6  7
    //        ------------------------------
    // Row: 3 |  12 28  14  30   13 29 15 31
    //      2 |  8  24  10  26   9  25 11 27
    //      1 |  4  20  06  22   5  21  7 23
    //      0 |  0  16  02  18   1  17  3 19
    //        ------------------------------
    std::vector<int> FBX = { 0, 4, 2, 6, 0, 4, 2, 6,
                             0, 4, 2, 6, 0, 4, 2, 6,
                             1, 5, 3, 7, 1, 5, 3, 7,
                             1, 5, 3, 7, 1, 5, 3, 7 };

    std::vector<int> FBY = { 0, 0, 0, 0, 1, 1, 1, 1,
                             2, 2, 2, 2, 3, 3, 3, 3,
                             0, 0, 0, 0, 1, 1, 1, 1,
                             2, 2, 2, 2, 3, 3, 3, 3 };

    int start_fb = 30;
    for (unsigned ii=0; ii < firbank_graph::TP_SSR; ii+=4) {

      single_buffer(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+0].in[0]);
      location<kernel>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii])                 =      tile(start_fb+FBX[ii],FBY[ii]);
      location<stack>    (dut.firbank.tdmfir.graph_instance.m_firKernels[ii])                 =      bank(start_fb+FBX[ii],FBY[ii],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii].param[0])        =      bank(start_fb+FBX[ii],FBY[ii],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii].param[1])        =   address(start_fb+FBX[ii],FBY[ii],0x4C00);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii].in[0])           =      bank(start_fb+FBX[ii],FBY[ii],0);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii].out[0])          = {    bank(start_fb+FBX[ii],FBY[ii],1), bank(start_fb+FBX[ii],FBY[ii],3) };

      single_buffer(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1].in[0]);
      location<kernel>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1])                 =      tile(start_fb+FBX[ii+1],FBY[ii+1]);
      location<stack>    (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1])                 =      bank(start_fb+FBX[ii+1],FBY[ii+1],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1].param[0])        =      bank(start_fb+FBX[ii+1],FBY[ii+1],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1].param[1])        =   address(start_fb+FBX[ii+1],FBY[ii+1],0x4C00);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1].in[0])           =      bank(start_fb+FBX[ii+1],FBY[ii+1],0);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+1].out[0])          = {    bank(start_fb+FBX[ii+1],FBY[ii+1],1), bank(start_fb+FBX[ii+1],FBY[ii+2],3) };

      single_buffer(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2].in[0]);
      location<kernel>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2])                 =      tile(start_fb+FBX[ii+2],FBY[ii+2]);
      location<stack>    (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2])                 =      bank(start_fb+FBX[ii+2],FBY[ii+2],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2].param[0])        =      bank(start_fb+FBX[ii+2],FBY[ii+2],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2].param[1])        =   address(start_fb+FBX[ii+2],FBY[ii+2],0x4C00);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2].in[0])           =      bank(start_fb+FBX[ii+2],FBY[ii+2],0);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+2].out[0])          = {    bank(start_fb+FBX[ii+2],FBY[ii+2],1), bank(start_fb+FBX[ii+2],FBY[ii+2],3) };

      single_buffer(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3].in[0]);
      location<kernel>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3])                 =      tile(start_fb+FBX[ii+3],FBY[ii+3]);
      location<stack>    (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3])                 =      bank(start_fb+FBX[ii+3],FBY[ii+3],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3].param[0])        =      bank(start_fb+FBX[ii+3],FBY[ii+3],3);
      location<parameter>(dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3].param[1])        =   address(start_fb+FBX[ii+3],FBY[ii+3],0x4C00);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3].in[0])           =      bank(start_fb+FBX[ii+3],FBY[ii+3],0);
      location<buffer>   (dut.firbank.tdmfir.graph_instance.m_firKernels[ii+3].out[0])          = {    bank(start_fb+FBX[ii+3],FBY[ii+3],1), bank(start_fb+FBX[ii+3],FBY[ii+3],3) };
    }
    for (unsigned ii=0; ii < firbank_graph::NPORT_I; ii++) {
      std::string file_i0 = "data/filterbank_i_" + std::to_string(ii) + ".csv";
      sig_i[ii] =  input_plio::create("PLIO_i_"+std::to_string(ii), plio_64_bits, file_i0 );
      connect<>(     sig_i[ii].out[0], dut.sig_i[ii] );
      location<PLIO>(sig_i[ii]) = shim(32);
    }
    for (unsigned ii=0; ii < firbank_graph::NPORT_O; ii++) {
      std::string file_o0 = "data/filterbank_o_" + std::to_string(ii) + ".txt";
      sig_o[ii] = output_plio::create("PLIO_o_"+std::to_string(ii), plio_64_bits, file_o0 );
      connect<>( dut.sig_o[ii],            sig_o[ii].in[0] );
      location<PLIO>(sig_o[ii]) = shim(32);
    }
    for ( unsigned ff=0; ff < ifft4096_2d_graph::TP_SSR; ff++) {
      std::string fname_i0 = "data/fft_front_i_" + std::to_string(ff) + ".txt";
      std::string fname_o0 = "data/fft_front_o_" + std::to_string(ff) + ".txt";
      std::string pname_i0 = "PLIO_front_in_" + std::to_string(ff) + "";
      std::string pname_o0 = "PLIO_front_out_" + std::to_string(ff) + "";
      std::string fname_i1 = "data/fft_back_i_" + std::to_string(ff) + ".txt";
      std::string fname_o1 = "data/fft_back_o_" + std::to_string(ff) + ".txt";
      std::string pname_i1 = "PLIO_back_in_" + std::to_string(ff) + "";
      std::string pname_o1 = "PLIO_back_out_" + std::to_string(ff) + "";
      front_i[ff] =  input_plio::create(pname_i0,plio_64_bits,fname_i0);
      front_o[ff] = output_plio::create(pname_o0,plio_64_bits,fname_o0);
      back_i[ff] =  input_plio::create(pname_i1,plio_64_bits,fname_i1);
      back_o[ff] = output_plio::create(pname_o1,plio_64_bits,fname_o1);
      connect<>( front_i[ff].out[0],  dut.front_i[ff]   );
      connect<>( dut.front_o[ff],     front_o[ff].in[0] );
      connect<>( back_i[ff].out[0],  dut.back_i[ff]   );
      connect<>( dut.back_o[ff],     back_o[ff].in[0] );
#ifndef __X86SIM__
      if (ff<ifft4096_2d_graph::TP_SSR/2){
            location<kernel>   (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])           = tile(34+ff,4);
            location<kernel>   (dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff])                                  = location<kernel>(dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0]);
            single_buffer      (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>   (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])     = bank(34+ff,4,0);
            single_buffer      (dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].out[0]);
            location<buffer>   (dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].out[0])                           = bank(34+ff,4,2);
            single_buffer      (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0]);
            location<buffer>   (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])    = bank(34+ff,4,1);
            location<stack>    (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])           = bank(34+ff,4,3);
            location<parameter>(dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].param[0])                         = bank(34+ff,4,3);
            location<parameter>(dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].param[1])                         = bank(34+ff,4,3);
            location<PLIO>(front_i[ff])                                                                             = shim(31);
            location<PLIO>(front_o[ff])                                                                             = shim(31);

            location<kernel>(dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = tile(30+ff,4);
            single_buffer   (dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>(dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])         = bank(30+ff,4,0);
            location<buffer>(dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])        = {bank(30+ff,4,1),bank(30+ff,4,2)};
            location<stack> (dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = bank(30+ff,4,3);
            location<PLIO>(back_i[ff])                                                                              = shim(29);
            location<PLIO>(back_o[ff])                                                                              = shim(29);
      }
      else{
            location<kernel>   (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])           = tile(34+ff-4,5);
            location<kernel>   (dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff])                                  = location<kernel>(dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0]);
            single_buffer      (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>   (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])     = bank(34+ff-4,5,0);
            single_buffer      (dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].out[0]);
            location<buffer>   (dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].out[0])                           = bank(34+ff-4,5,2);
            single_buffer      (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0]);
            location<buffer>   (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])    = bank(34+ff-4,5,1);
            location<stack>    (dut.ifft4096_2d.ifft4096_2d.frontFFTGraph[ff].FFTwinproc.m_fftKernels[0])           = bank(34+ff-4,5,3);
            location<parameter>(dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].param[0])                         = bank(34+ff-4,5,3);
            location<parameter>(dut.ifft4096_2d.ifft4096_2d.m_fftTwRotKernels[ff].param[1])                         = bank(34+ff-4,5,3);
            location<PLIO>(front_i[ff])                                                                             = shim(30);
            location<PLIO>(front_o[ff])                                                                             = shim(30);

            location<kernel>(dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = tile(30+ff-4,5);
            single_buffer   (dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0]);
            location<buffer>(dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].in[0])         = bank(30+ff-4,5,0);
            location<buffer>(dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0].out[0])        = {bank(30+ff-4,5,1),bank(30+ff-4,5,2)};
            location<stack> (dut.ifft4096_2d.ifft4096_2d.backFFTGraph[ff].FFTwinproc.m_fftKernels[0])               = bank(30+ff-4,5,3);
            location<PLIO>(back_i[ff])                                                                              = shim(28);
            location<PLIO>(back_o[ff])                                                                              = shim(28);
      }
#endif
    }
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

#if defined(__AIESIM__) || defined(__X86SIM__)
// Initialize and run the graph:
int main(void)
{
  aie_dut.init();
  aie_dut.run(4);
  aie_dut.end();

  return 0;
}
#endif
