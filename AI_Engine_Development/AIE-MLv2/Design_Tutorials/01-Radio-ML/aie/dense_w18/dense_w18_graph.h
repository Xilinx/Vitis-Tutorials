//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "dense_w18.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dense_w18_graph : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>                 data_i;
  port<output>                data_o;

  port<direction::in>         weights;
  port<direction::in>         biases;

  static constexpr unsigned NSAMP       = 128;
  static constexpr unsigned NNODES      = 24;
  static constexpr unsigned NWEIGHTS    = NSAMP*NNODES;
  static constexpr unsigned NBIASES     = NNODES;

  dense_w18_graph( void )
  {

    kk = kernel::create_object<dense_w18<NSAMP,NNODES,NWEIGHTS,NBIASES>>();
    source(kk) = "dense_w18.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = 1;
    
    single_buffer(kk.in[1]);
    single_buffer(kk.in[2]);

    // Connect inputs:
    connect<> ( data_i,     kk.in[0] );      dimensions(kk.in[0])  = {NSAMP};
    connect<parameter> ( weights, async(kk.in[1]) );
    connect<parameter> ( biases,  async(kk.in[2]) );
    connect<> ( kk.out[0],   data_o );       dimensions(kk.out[0]) = {NNODES};
    
  }
};
