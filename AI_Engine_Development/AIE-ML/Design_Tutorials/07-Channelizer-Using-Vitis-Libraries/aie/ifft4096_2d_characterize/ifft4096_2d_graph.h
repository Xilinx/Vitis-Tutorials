//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <vss_fft_ifft_1d_graph.hpp>

using namespace adf;
namespace dsplib_fft = xf::dsp::aie::fft::vss_1d;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class ifft4096_2d_graph : public graph {
public:
  typedef cint32            TT_DATA;
  typedef cint16            TT_TWIDDLE;
  static constexpr unsigned TP_POINT_SIZE = 4096;
  static constexpr unsigned TP_FFT_NIFFT = 0;
  static constexpr unsigned TP_SHIFT = 0;
  static constexpr unsigned TP_API = 0;
  static constexpr unsigned TP_SSR = 1;
  static constexpr unsigned TP_RND = 12;
  static constexpr unsigned TP_SAT = 1;
  static constexpr unsigned TP_TWIDDLE_MODE = 0;

  std::array< port< input>,TP_SSR>  front_i;
  std::array< port< input>,TP_SSR>  back_i;
  std::array<port< output>,TP_SSR>  front_o;
  std::array<port< output>,TP_SSR>  back_o;

  using TT_2D_IFFT = dsplib_fft::vss_fft_ifft_1d_graph<TT_DATA,TT_TWIDDLE,TP_POINT_SIZE,TP_FFT_NIFFT,
                                                       TP_SHIFT,TP_API,TP_SSR,
                                                       TP_RND,TP_SAT,TP_TWIDDLE_MODE>;

  TT_2D_IFFT ifft4096_2d;
  
  // Constructor:
  ifft4096_2d_graph( void )
  {
    for ( unsigned ff=0; ff < TP_SSR; ff++) {
      connect<>( front_i[ff],                 ifft4096_2d.front_i[ff] );
      connect<>( ifft4096_2d.front_o[ff],       front_o[ff] );
      connect<>( back_i[ff],                  ifft4096_2d.back_i[ff] );
      connect<>( ifft4096_2d.back_o[ff],        back_o[ff] );
    }
  }
};
