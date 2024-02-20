//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "fft32_r2_kernel.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class fft32_r2_graph : public graph {
private:
  kernel k_kernel;

public:
  port<input>  sig_i;
  port<output> sig_o;

  // Constructor:
  fft32_r2_graph( void )
  {
    k_kernel = kernel::create_object<fft32_r2_kernel>();
    source(k_kernel) = "fft32_r2_kernel.cpp";
    runtime<ratio>(k_kernel) = 0.9;

    connect( sig_i, k_kernel.in[0] );
    connect( k_kernel.out[0], sig_o );
  }
};

