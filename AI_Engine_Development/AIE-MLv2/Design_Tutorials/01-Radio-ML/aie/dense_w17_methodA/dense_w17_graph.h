//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#pragma once

#include <adf.h>
#include "dense_w17.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dense_w17_graph : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>               data_i;
  port<output>              data_o;
  
  port<direction::in>       weights;
  port<direction::in>       biases;

  static constexpr unsigned NSAMP       = 128;          // 128 I/Q samples
  static constexpr unsigned NNODES      = 128;          // 128 nodes
  static constexpr unsigned NBIASES     = NNODES;       // 128 nodes
  static constexpr unsigned NWEIGHTS    = NSAMP*NNODES; // KERNEL_SIZE*2*NNODES;  // KERNEL_SIZE=7, I/Q paths, 64 nodes
  
  dense_w17_graph( void )
  {

    kk = kernel::create_object<dense_w17<NNODES,NWEIGHTS,NBIASES>>();
    source(kk) = "dense_w17.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = 1;
    
    single_buffer(kk.in[1]);
    single_buffer(kk.in[2]);

    // Connect inputs:
    connect<> ( data_i,     kk.in[0] ); dimensions(kk.in[0])  = {NNODES};
    connect<parameter> ( weights,    async(kk.in[1]) ); 
    connect<parameter> ( biases,     async(kk.in[2]) ); 

    // Connect outputs:
    connect<> ( kk.out[0],   data_o );  dimensions(kk.out[0]) = {NNODES};
    
  }
};

