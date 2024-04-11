//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "dft7_mmul0.h"
#include "dft7_mmul1.h"
#include "dft7_mmul2.h"
#include "dft7_twiddle.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dft7_graph : public graph {
public:

  // Kernels:
  kernel kk0;
  kernel kk1;
  kernel kk2;

  port<input> sig_i;
  port<output> sig_o;

  dft7_graph( void )
  {
    kk0 = kernel::create_object<dft7_0>( std::vector<typename dft7_0::TT_TWID>{DFT7_TWID0} );
    kk1 = kernel::create_object<dft7_1>( std::vector<typename dft7_1::TT_TWID>{DFT7_TWID1} );
    kk2 = kernel::create_object<dft7_2>();
    source(kk0) = "dft7_mmul0.cpp";
    source(kk1) = "dft7_mmul1.cpp";
    source(kk2) = "dft7_mmul2.cpp";
    runtime<ratio>(kk0) = 0.9;
    runtime<ratio>(kk1) = 0.9;
    runtime<ratio>(kk2) = 0.9;

    connect<>  n0   (sig_i,kk0.in[0]);
    connect<>  n1   (sig_i,kk1.in[1]);
    connect<cascade>(kk0.out[0],kk1.in[0]);
    connect<cascade>(kk1.out[0],kk2.in[0]);
    connect<>       (kk2.out[0],sig_o);
    fifo_depth(n0) = 32;
    fifo_depth(n1) = 32;
  }
};

