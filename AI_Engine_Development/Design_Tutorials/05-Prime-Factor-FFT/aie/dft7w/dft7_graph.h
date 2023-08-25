//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "dft7_compute.h"
#include "dft7_combine.h"
#include "dft7_twiddle.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dft7_graph : public graph {
public:
  static constexpr unsigned NFFT = 9*16;
  typedef cint16 TT_DATA;
  typedef cint16 TT_TWIDDLE;
  typedef cacc48 TT_ACC;

  // Kernels:
  kernel k_tile0;
  kernel k_tile1;
  kernel k_tile2;

  std::array<port< input>,2> sig_i;
  std::array<port<output>,2> sig_o;

  using TT_COMPUTE = dft7_compute<TT_DATA,TT_TWIDDLE,TT_ACC,NFFT>;
  using TT_COMBINE = dft7_combine<TT_DATA,NFFT>;

  dft7_graph( void )
  {
    k_tile0 = kernel::create_object<TT_COMPUTE>( std::vector<TT_TWIDDLE>{DFT7_TWID0} );
    k_tile1 = kernel::create_object<TT_COMPUTE>( std::vector<TT_TWIDDLE>{DFT7_TWID1} );
    k_tile2 = kernel::create_object<TT_COMBINE>();
    source(k_tile0) = "dft7_compute.cpp";
    source(k_tile1) = "dft7_compute.cpp";
    source(k_tile2) = "dft7_combine.cpp";
    runtime<ratio>(k_tile0) = 0.9;
    runtime<ratio>(k_tile1) = 0.9;
    runtime<ratio>(k_tile2) = 0.9;

    // Connect stream I/O:
    connect<>(sig_i[0],k_tile0.in[0]);
    connect<>(sig_i[1],k_tile0.in[1]);
    connect<>(sig_i[0],k_tile1.in[0]);
    connect<>(sig_i[1],k_tile1.in[1]);

    // adf::dimensions(k_tile0.in[0]) = { TT_COMPUTE::NSAMP_I };
    // adf::dimensions(k_tile0.in[1]) = { TT_COMPUTE::NSAMP_I };
    // adf::dimensions(k_tile1.in[0]) = { TT_COMPUTE::NSAMP_I };
    // adf::dimensions(k_tile1.in[1]) = { TT_COMPUTE::NSAMP_I };

    // Connect to combiner tile:
    connect<>(k_tile0.out[0],k_tile2.in[0]);
    connect<>(k_tile1.out[0],k_tile2.in[1]);
    connect<>(k_tile2.out[0],sig_o[0]);
    connect<>(k_tile2.out[1],sig_o[1]);
  }
};

