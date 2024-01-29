//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
// #include "dft16.h"
#include "dft16_mmul0.h"
#include "dft16_mmul1.h"
#include "dft16_twiddle.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dft16_graph : public graph {
public:

  // Kernels:
  kernel kk0;
  kernel kk1;

  port<input> sig_i;
  port<output> sig_o;

  dft16_graph( void )
  {
    // kk0 = kernel::create_object<dft16>( std::vector<typename dft16::TT_TWID>{DFT16_TWID} );
    kk0 = kernel::create_object<dft16_0>( std::vector<typename dft16_0::TT_TWID>{DFT16_TWID} );
    kk1 = kernel::create_object<dft16_1>( std::vector<typename dft16_1::TT_TWID>{DFT16_TWID} );
    // source(kk0) = "dft16.cpp";
    source(kk0) = "dft16_mmul0.cpp";
    source(kk1) = "dft16_mmul1.cpp";
    runtime<ratio>(kk0) = 0.9;
    runtime<ratio>(kk1) = 0.9;

    connect<>  n0 (sig_i,kk0.in[0]);
    connect<>  n1 (sig_i,kk1.in[1]);
    connect<cascade>(kk0.out[0],kk1.in[0]);
    connect<>(kk1.out[0],sig_o);
    fifo_depth(n0) = 32;
    fifo_depth(n1) = 32;
  }
};

