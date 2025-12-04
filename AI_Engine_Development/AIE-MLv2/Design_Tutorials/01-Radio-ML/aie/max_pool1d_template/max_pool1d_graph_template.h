//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "max_pool1d_template.h"
#include "ceil_constexpr.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template <unsigned NSAMP, unsigned NNODES>
class max_pool1d_graph_template : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>                            data_i;
  port<output>                           data_o;
  
  static constexpr double   BUFFER_SIZE_MULTIPLE = NSAMP*NNODES*2*2*2.0/65536;  // Ratio of output buffer size to local tile memory
                                                                                // *2     -> 2 bytes per sample
                                                                                // *2     -> ping-pong
                                                                                // *2     -> Margin to ensure input buffer uses only two banks (same applies for output buffer)
                                                                                // 65536  -> Local tile memory (64KB)
  static constexpr unsigned NSPLIT = ceil_constexpr(BUFFER_SIZE_MULTIPLE);      // Process incoming samples in NSPLIT chunks so IO buffers fit in local tile memory. Return type of ceil in c++17 is not constexpr.

  max_pool1d_graph_template( void )
  {
    kk = kernel::create_object<max_pool1d_template<NSAMP,NNODES,NSPLIT>>();
    source(kk) = "max_pool1d_template.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = NSPLIT;

    // Connect inputs:
    connect<> ( data_i,     kk.in[0] );       dimensions(kk.in[0])  = {NSAMP/NSPLIT  *NNODES};
    connect<> ( kk.out[0],  data_o );         dimensions(kk.out[0]) = {NSAMP/NSPLIT/2*NNODES};
  }
};

