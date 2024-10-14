//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "farrow_kernel1.h"
#include "farrow_kernel2.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class farrow_graph : public graph {
public:
  // Declare Kernels
  kernel farrow_kernel1_ins;
  kernel farrow_kernel2_ins;

  std::array<port<input>,1>   sig_i;
  std::array<port<input>,1>   del_i;
  std::array<port<output>,1>  sig_o;

  farrow_graph( void )
  {
    farrow_kernel1_ins = kernel::create_object<farrow_kernel1>();
    source(farrow_kernel1_ins) = "farrow_kernel1.cpp";
    runtime<ratio>(farrow_kernel1_ins) = 0.9;
    
    farrow_kernel2_ins = kernel::create_object<farrow_kernel2>();
    source(farrow_kernel2_ins) = "farrow_kernel2.cpp";
    runtime<ratio>(farrow_kernel2_ins) = 0.9;

    location<stack>(farrow_kernel1_ins) = location<kernel>(farrow_kernel1_ins);
    location<stack>(farrow_kernel2_ins) = location<kernel>(farrow_kernel2_ins);
    
    // Connect inputs:
    connect<> ( sig_i[0], farrow_kernel1_ins.in[0] );
    connect<> ( del_i[0], farrow_kernel2_ins.in[0] );

    connect<> ( farrow_kernel1_ins.out[0], farrow_kernel2_ins.in[1] );
    connect<> ( farrow_kernel1_ins.out[1], farrow_kernel2_ins.in[2] );
    connect<> ( farrow_kernel1_ins.out[2], farrow_kernel2_ins.in[3] );
    connect<> ( farrow_kernel1_ins.out[3], farrow_kernel2_ins.in[4] );

   // Connect outputs:
    connect<>( farrow_kernel2_ins.out[0], sig_o[0] );
  }
};

