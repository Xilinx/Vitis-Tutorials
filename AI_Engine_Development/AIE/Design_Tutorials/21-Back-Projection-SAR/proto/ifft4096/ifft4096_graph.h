//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <array>
#include <adf.h>
#include <fft_ifft_dit_1ch_graph.hpp>

using namespace adf;
namespace dsplib = xf::dsp::aie::fft::dit_1ch;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class ifft4096_graph : public graph {
public:
  typedef cfloat                              TT_TYPE;
  typedef cfloat                              TT_TWIDDLE;
  static constexpr int  TP_POINT_SIZE         = 4096;
  static constexpr int  TP_FFT_NIFFT          = 0;
  static constexpr int  TP_SHIFT              = 0;
  static constexpr int  TP_CASC_LEN           = 1;
  static constexpr int  TP_DYN_PT_SIZE        = 0;
  static constexpr int  TP_WINDOW_SIZE        = TP_POINT_SIZE;
  static constexpr int  TP_API                = 0;
  static constexpr int  TP_PARALLEL_POWER     = 1;
  static constexpr int  NPORTS_IO             = (TP_API == 0) ? (1<<TP_PARALLEL_POWER) : (1 << (TP_PARALLEL_POWER+1));

  std::array< input_plio,NPORTS_IO> sig_i;
  std::array<output_plio,NPORTS_IO> sig_o;

  using TT_FFT = dsplib::fft_ifft_dit_1ch_graph<TT_TYPE,TT_TWIDDLE,TP_POINT_SIZE,TP_FFT_NIFFT,TP_SHIFT,
                                                TP_CASC_LEN,TP_DYN_PT_SIZE,TP_WINDOW_SIZE,TP_API,
                                                TP_PARALLEL_POWER>;

  TT_FFT ifft;

  // Constructor:
  ifft4096_graph()
  {
    for (unsigned ii=0; ii < NPORTS_IO; ii++) {
      sig_i[ii] =  input_plio::create("PLIO_i_"+std::to_string(ii),plio_64_bits,"data/sig_i_"+std::to_string(ii)+".txt");
      sig_o[ii] = output_plio::create("PLIO_o_"+std::to_string(ii),plio_64_bits,"data/sig_o_"+std::to_string(ii)+".txt");
      connect<>( sig_i[ii].out[0],  ifft.in[ii] );
      connect<>( ifft.out[ii],  sig_o[ii].in[0] );
    }
  }
};


