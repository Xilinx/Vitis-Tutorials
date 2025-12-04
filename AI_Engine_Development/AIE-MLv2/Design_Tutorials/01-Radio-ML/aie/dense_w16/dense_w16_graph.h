//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "dense_w16.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class dense_w16_graph : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>                 data_i;
  port<output>                data_o;

  port<direction::in>         weights_0;
  port<direction::in>         weights_1;
  port<direction::in>         biases;

  static constexpr unsigned NSAMP       = 512;
  static constexpr unsigned NNODES      = 128;
  static constexpr unsigned NWEIGHTS    = NSAMP*NNODES;
  static constexpr unsigned NBIASES     = NNODES;

  dense_w16_graph( void )
  {

    kk = kernel::create_object<dense_w16<NSAMP,NNODES,NWEIGHTS,NBIASES>>();
    source(kk) = "dense_w16.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = 1;
    
    single_buffer(kk.in[1]);
    single_buffer(kk.in[2]);
    single_buffer(kk.in[3]);

    // Connect inputs:
    connect<> ( data_i,     kk.in[0] );      dimensions(kk.in[0])  = {NSAMP};
    connect<parameter> ( weights_0, async(kk.in[1]) );
    connect<parameter> ( weights_1, async(kk.in[2]) );
    connect<parameter> ( biases,  async(kk.in[3]) );
    connect<> ( kk.out[0],   data_o );        dimensions(kk.out[0]) = {NNODES};
    
  }
};

