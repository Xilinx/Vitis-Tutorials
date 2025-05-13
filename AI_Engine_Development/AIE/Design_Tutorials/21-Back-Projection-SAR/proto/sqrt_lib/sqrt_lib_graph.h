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

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class sqrt_lib_graph : public graph {
public:
  input_plio  sig_i;
  output_plio sig_o;

  typedef float TT_DATA;
  static constexpr int  TP_COARSE_BITS = 10;
  static constexpr int    TP_FINE_BITS = 13;
  static constexpr int  TP_DOMAIN_MODE = 2;
  static constexpr int TP_WINDOW_VSIZE = 512;
  static constexpr int        TP_SHIFT = 0;
  static constexpr int          TP_RND = rnd_floor;
  static constexpr int          TP_SAT = 1;
  static constexpr int        LUT_SIZE = 2*(1 << TP_COARSE_BITS);

  std::array<TT_DATA,LUT_SIZE> LUT_init;

  // Constructor:
  sqrt_lib_graph()
  {
    sig_i  =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o  = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );

    // Initialize LUT:
    xf::dsp::aie::func_approx::getSqrt<TT_DATA,TT_DATA>( (TT_DATA*)&LUT_init[0],
                                                         TP_COARSE_BITS,TP_FINE_BITS,TP_DOMAIN_MODE,TP_SHIFT);

    // Initialize graph:
    // Throughput:     1740 MB/sec or ~420 Msps
    // Total cycles:   1486
    xf::dsp::aie::func_approx::func_approx_graph<TT_DATA,TP_COARSE_BITS,TP_FINE_BITS,
                                                 TP_DOMAIN_MODE,TP_WINDOW_VSIZE,TP_SHIFT,
                                                 TP_RND,TP_SAT> sqrt_dsp( LUT_init );
    connect<>( sig_i.out[0],  sqrt_dsp.in[0] );
    connect<>( sqrt_dsp.out[0],  sig_o.in[0] );
    location<graph>(*this) = bounding_box(24,0,25,0);
  }
};


