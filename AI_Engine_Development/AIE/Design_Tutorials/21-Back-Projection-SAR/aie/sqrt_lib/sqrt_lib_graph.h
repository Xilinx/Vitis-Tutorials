//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <vector>
#include <array>
#include <func_approx_fns.hpp>
#include <func_approx_graph.hpp>

using namespace adf;
namespace dsplib = xf::dsp::aie::func_approx;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class sqrt_lib_graph : public graph {
public:
  port<input>  sig_i;
  port<output> sig_o;

  typedef float TT_DATA;
  static constexpr int  TP_COARSE_BITS = 10;
  static constexpr int    TP_FINE_BITS = 13;
  static constexpr int  TP_DOMAIN_MODE = 2;
  static constexpr int TP_WINDOW_VSIZE = NSAMP;
  static constexpr int        TP_SHIFT = 0;
  static constexpr int          TP_RND = rnd_floor;
  static constexpr int          TP_SAT = 1;
  static constexpr int        LUT_SIZE = 2*(1 << TP_COARSE_BITS);

  std::array<TT_DATA,LUT_SIZE> LUT_init;
  dsplib::func_approx_graph<TT_DATA,TP_COARSE_BITS,TP_FINE_BITS,TP_DOMAIN_MODE,TP_WINDOW_VSIZE,TP_SHIFT,
                            TP_RND,TP_SAT> sqrt_dsp;

  // Initialize LUT:
  std::array<TT_DATA,LUT_SIZE> init_LUT(void)
  {
    std::array<TT_DATA,LUT_SIZE> LUT;
    dsplib::getSqrt<TT_DATA,TT_DATA>(&LUT[0],TP_COARSE_BITS,TP_FINE_BITS,TP_DOMAIN_MODE,TP_SHIFT);
    return(LUT);
  }

  // Constructor:
  sqrt_lib_graph() : LUT_init({init_LUT()}), sqrt_dsp(LUT_init)
  {
    // Initialize graph:
    // Throughput:     1740 MB/sec or ~420 Msps
    // Total cycles:   1486
    connect<>( sig_i,            sqrt_dsp.in[0] );
    connect<>( sqrt_dsp.out[0],  sig_o          );
    repetition_count(sqrt_dsp.m_kernel[0]) = REPEAT;
  }
};


