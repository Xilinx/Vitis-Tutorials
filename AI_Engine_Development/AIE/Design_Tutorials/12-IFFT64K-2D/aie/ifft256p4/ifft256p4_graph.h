//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "ifft256p4.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class ifft256p4_graph : public graph {
public:
  kernel k_kernel;

  port<input>  sig_i;
  port<output> sig_o;

  // Constructor:
  ifft256p4_graph( void )
  {
    k_kernel = kernel::create_object<ifft256p4>(std::vector<ifft256p4::TT_DATA>(ifft256p4::NFFT));
    source(k_kernel) = "ifft256p4.cpp";
    runtime<ratio>(k_kernel) = 0.9;
    connect<>( sig_i, k_kernel.in[0] );
    connect<>( k_kernel.out[0], sig_o );
  }
};

