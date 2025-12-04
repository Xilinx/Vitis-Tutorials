//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "max_pool1d_w2.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class max_pool1d_w2_graph : public graph {
public:
  // Declare Kernels
  kernel kk;

  port<input>                            data_i;
  port<output>                           data_o;
  shared_buffer<bfloat16>                MT_output;

  max_pool1d_w2_graph( void )
  {
    // Tiling parameters for outputs
    tiling_parameters bdw_output = { .buffer_dimension   = {max_pool1d_w2::NNODES,max_pool1d_w2::NSAMP/2},
                                     .tiling_dimension   = {2,32},
                                     .offset             = {0,0},
                                     .tile_traversal     = {{.dimension=1, .stride=32,.wrap=max_pool1d_w2::NSAMP/2/32},
				                            {.dimension=0, .stride=2, .wrap=max_pool1d_w2::NNODES/2}}
                                                      };
    tiling_parameters bdr_output = { .buffer_dimension   = {max_pool1d_w2::NNODES*max_pool1d_w2::NSAMP/2},
                                     .tiling_dimension   = {max_pool1d_w2::NNODES*max_pool1d_w2::NSAMP/2},
                                     .offset             = {0} };
    MT_output = shared_buffer<bfloat16>::create({max_pool1d_w2::NNODES*max_pool1d_w2::NSAMP/2}, 1, 1);
    write_access(MT_output.in[0]) = tiling(bdw_output);
    read_access(MT_output.out[0]) = tiling(bdr_output);
    repetition_count(MT_output) = 1;

    kk = kernel::create_object<max_pool1d_w2>();
    source(kk) = "max_pool1d_w2.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = max_pool1d_w2::NSPLIT;

    // Connect inputs:
    connect<> ( data_i,            kk.in[0] );         dimensions(kk.in[0])  = {max_pool1d_w2::NSAMP  /max_pool1d_w2::NSPLIT*max_pool1d_w2::NNODES};
    connect<> ( kk.out[0],         MT_output.in[0] );
    connect<> ( MT_output.out[0],  data_o );           dimensions(kk.out[0]) = {max_pool1d_w2::NSAMP/2/max_pool1d_w2::NSPLIT*max_pool1d_w2::NNODES};

  }
};

