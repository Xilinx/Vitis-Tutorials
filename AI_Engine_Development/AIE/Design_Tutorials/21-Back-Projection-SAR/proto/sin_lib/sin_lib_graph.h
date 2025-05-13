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
#include "sin_lib_lut_init.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class sin_lib_graph : public graph {
public:
  input_plio  sig_i;
  output_plio sig_o;

  typedef float TT_DATA;
  static constexpr unsigned  TP_COARSE_BITS = 10;
  static constexpr unsigned    TP_FINE_BITS = 13;
  static constexpr unsigned  TP_DOMAIN_MODE = 0;
  static constexpr unsigned TP_WINDOW_VSIZE = 512;
  static constexpr unsigned        TP_SHIFT = 0;
  static constexpr unsigned          TP_RND = rnd_floor;
  static constexpr unsigned          TP_SAT = 1;
  static constexpr unsigned        LUT_SIZE = 2*(1 << TP_COARSE_BITS);

  std::array<TT_DATA,LUT_SIZE> LUT_init;

  // Constructor:
  sin_lib_graph()
  {
    sig_i  =  input_plio::create("PLIO_i", plio_64_bits, "data/sig_i.txt" );
    sig_o  = output_plio::create("PLIO_o", plio_64_bits, "data/sig_o.txt" );

    // Initialize LUT:
    initialize_LUT<TT_DATA>( (TT_DATA*)&LUT_init[0],TP_COARSE_BITS,TP_DOMAIN_MODE);

    // Initialize graph:
    // Throughput:     1740 MB/sec or ~420 Msps
    // Total cycles:   1478
    xf::dsp::aie::func_approx::func_approx_graph<TT_DATA,TP_COARSE_BITS,TP_FINE_BITS,
                                                 TP_DOMAIN_MODE,TP_WINDOW_VSIZE,TP_SHIFT,
                                                 TP_RND,TP_SAT> sin_dsp( LUT_init );
    connect<>( sig_i.out[0],  sin_dsp.in[0] );
    connect<>( sin_dsp.out[0],  sig_o.in[0] );
  }
};


