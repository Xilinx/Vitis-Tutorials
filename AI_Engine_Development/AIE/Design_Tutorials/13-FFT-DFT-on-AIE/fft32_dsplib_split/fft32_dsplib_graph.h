//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <vector>
#include <fft_ifft_dit_1ch_graph.hpp>
using namespace adf;
using namespace xf::dsp::aie::fft::dit_1ch;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<int ORIGIN_X, int ORIGIN_Y>
class fft32_dsplib_graph : public graph {
public:
  typedef cint16                              TT_TYPE;
  typedef cint16                              TT_TWIDDLE;
  static constexpr int  REPEAT                = 1;
  static constexpr int  TP_POINT_SIZE         = 32;
  static constexpr int  TP_FFT_NIFFT          = 1;
  static constexpr int  TP_SHIFT              = 0;
  static constexpr int  TP_CASC_LEN           = 3;
  static constexpr int  TP_DYN_PT_SIZE        = 0;
  static constexpr int  TP_WINDOW_SIZE        = TP_POINT_SIZE * REPEAT;
  static constexpr int  TP_API                = 0;
  static constexpr int  TP_PARALLEL_POWER     = 0;
  static constexpr int  Nports = (TP_API == 1 ) ? (1 << (TP_PARALLEL_POWER+1)) : 1;

  std::array<input_plio,Nports>  fft_i;
  std::array<output_plio,Nports> fft_o;

  fft_ifft_dit_1ch_graph<TT_TYPE,TT_TWIDDLE,TP_POINT_SIZE,TP_FFT_NIFFT,TP_SHIFT,
                         TP_CASC_LEN,TP_DYN_PT_SIZE,TP_WINDOW_SIZE,TP_API,
                         TP_PARALLEL_POWER> dut_fft;
  fft32_dsplib_graph(void)
  {
    for (int ii=0; ii < Nports; ii++) {
      std::string fname_i = "data/sig" + std::to_string(ii) + "_i.txt";
      std::string fname_o = "data/sig" + std::to_string(ii) + "_o.txt";
      fft_i[ii] =  input_plio::create("PLIO_i_"+std::to_string(ii),plio_64_bits,fname_i);
      fft_o[ii] = output_plio::create("PLIO_o_"+std::to_string(ii),plio_64_bits,fname_o);
      connect<stream>( fft_i[ii].out[0], dut_fft.in[ii]  );
      connect<stream>( dut_fft.out[ii],  fft_o[ii].in[0] );
    }
  }
};


