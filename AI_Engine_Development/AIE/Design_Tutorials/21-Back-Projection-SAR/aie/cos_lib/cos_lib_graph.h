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
#include "cos_lib_lut_init.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
class cos_lib_graph : public graph {
public:
  port<input>  sig_i;
  port<output> sig_o;

  typedef float TT_DATA;
  static constexpr unsigned  TP_COARSE_BITS = 10;
  static constexpr unsigned    TP_FINE_BITS = 13;
  static constexpr unsigned  TP_DOMAIN_MODE = 0;
  static constexpr unsigned TP_WINDOW_VSIZE = NSAMP;
  static constexpr unsigned        TP_SHIFT = 0;
  static constexpr unsigned          TP_RND = rnd_floor;
  static constexpr unsigned          TP_SAT = 1;
  static constexpr unsigned        LUT_SIZE = 2*(1 << TP_COARSE_BITS);

  std::array<TT_DATA,LUT_SIZE> LUT_init;
  xf::dsp::aie::func_approx::func_approx_graph<TT_DATA,TP_COARSE_BITS,TP_FINE_BITS,
                                               TP_DOMAIN_MODE,TP_WINDOW_VSIZE,TP_SHIFT,
                                               TP_RND,TP_SAT> cos_dsp;

  // Constructor:
  cos_lib_graph() : LUT_init({initialize_cos_LUT<TT_DATA,LUT_SIZE,TP_COARSE_BITS,TP_DOMAIN_MODE>()}),
                    cos_dsp(LUT_init)
  {
    // Initialize graph:
    // Throughput:     1740 MB/sec or ~420 Msps
    // Total cycles:   1478
    connect<>( sig_i,          cos_dsp.in[0] );
    connect<>( cos_dsp.out[0], sig_o         );
    repetition_count(cos_dsp.m_kernel[0]) = REPEAT;
  }
};


