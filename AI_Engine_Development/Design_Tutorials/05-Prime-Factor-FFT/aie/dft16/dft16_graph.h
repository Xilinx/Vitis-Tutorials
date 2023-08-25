//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "dft16_compute.h"
#include "dft16_combine.h"
#include "dft16_twiddle.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dft16_graph : public graph {
public:
  static constexpr unsigned NFFT  = 7*9;
  typedef cint16 TT_DATA;
  typedef cint16 TT_TWIDDLE;
  typedef cacc48 TT_ACC;

  // Kernels:
  kernel k_tile0;
  kernel k_tile1;
  kernel k_tile2;
  kernel k_tile3;
  kernel k_tile4;

  std::array<port< input>,2> sig_i;
  std::array<port<output>,2> sig_o;

  using TT_COMPUTE = dft16_compute<TT_DATA,TT_TWIDDLE,TT_ACC,NFFT>;
  using TT_COMBINE = dft16_combine<TT_DATA,NFFT>;

  dft16_graph( void )
  {
    k_tile0 = kernel::create_object<TT_COMPUTE>( std::vector<TT_TWIDDLE>{DFT16_TWID0a},
                                                 std::vector<TT_TWIDDLE>{DFT16_TWID0b} );
    k_tile1 = kernel::create_object<TT_COMPUTE>( std::vector<TT_TWIDDLE>{DFT16_TWID1a},
                                                 std::vector<TT_TWIDDLE>{DFT16_TWID1b} );
    k_tile2 = kernel::create_object<TT_COMPUTE>( std::vector<TT_TWIDDLE>{DFT16_TWID2a},
                                                 std::vector<TT_TWIDDLE>{DFT16_TWID2b} );
    k_tile3 = kernel::create_object<TT_COMPUTE>( std::vector<TT_TWIDDLE>{DFT16_TWID3a},
                                                 std::vector<TT_TWIDDLE>{DFT16_TWID3b} );
    k_tile4 = kernel::create_object<TT_COMBINE>();
    source(k_tile0) = "dft16_compute.cpp";
    source(k_tile1) = "dft16_compute.cpp";
    source(k_tile2) = "dft16_compute.cpp";
    source(k_tile3) = "dft16_compute.cpp";
    source(k_tile4) = "dft16_combine.cpp";
    runtime<ratio>(k_tile0) = 0.9;
    runtime<ratio>(k_tile1) = 0.9;
    runtime<ratio>(k_tile2) = 0.9;
    runtime<ratio>(k_tile3) = 0.9;
    runtime<ratio>(k_tile4) = 0.9;

    // Connect stream I/O:
    connect<stream,stream> n0 (sig_i[0],k_tile0.in[0]);
    connect<stream,stream> n1 (sig_i[1],k_tile0.in[1]);
    connect<stream,stream> n2 (sig_i[0],k_tile1.in[0]);
    connect<stream,stream> n3 (sig_i[1],k_tile1.in[1]);
    connect<stream,stream> n4 (sig_i[0],k_tile2.in[0]);
    connect<stream,stream> n5 (sig_i[1],k_tile2.in[1]);
    connect<stream,stream> n6 (sig_i[0],k_tile3.in[0]);
    connect<stream,stream> n7 (sig_i[1],k_tile3.in[1]);
    // fifo_depth(n0) = 96;
    // fifo_depth(n1) = 96;
    // fifo_depth(n2) = 96;
    // fifo_depth(n3) = 96;
    // fifo_depth(n4) = 96;
    // fifo_depth(n5) = 96;
    // fifo_depth(n6) = 96;
    // fifo_depth(n7) = 96;

    // Connect to combiner tile:
    connect<>(k_tile0.out[0],k_tile4.in[0]);
    connect<>(k_tile1.out[0],k_tile4.in[1]);
    connect<>(k_tile2.out[0],k_tile4.in[2]);
    connect<>(k_tile3.out[0],k_tile4.in[3]);
    connect<stream,stream>(k_tile4.out[0],sig_o[0]);
    connect<stream,stream>(k_tile4.out[1],sig_o[1]);
  }
};

