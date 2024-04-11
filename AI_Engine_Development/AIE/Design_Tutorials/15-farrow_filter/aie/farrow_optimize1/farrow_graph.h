//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "farrow_kernel.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class farrow_graph : public graph {
public:
  // Declare Kernels
  kernel farrow_kernel_ins;

  std::array<port<input>,1>   sig_i;
  std::array<port<input>,1>   del_i;
  std::array<port<output>,1>  sig_o;

  farrow_graph( void )
  {
    farrow_kernel_ins = kernel::create_object<farrow_kernel>();
    source(farrow_kernel_ins) = "farrow_kernel.cpp";
    runtime<ratio>(farrow_kernel_ins) = 0.9;
    // Connect inputs:
    connect<> ( sig_i[0], farrow_kernel_ins.in[0] );
    connect<> ( del_i[0], farrow_kernel_ins.in[1] );
    // Connect outputs:
    connect<>( farrow_kernel_ins.out[0], sig_o[0] );
  }
};

