//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "conv1d_w1.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class conv1d_w1_graph : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>                 data_iq;
  port<direction::in>         weights;
  port<direction::in>         biases;
  port<output>                data_o;
  shared_buffer<bfloat16>     MT;

  static constexpr unsigned NSAMP       = 2048;                  // 1024 I/Q samples
  static constexpr unsigned NNODES      = 64;                    // 64 nodes
  static constexpr unsigned NBIASES     = NNODES;                // 64 nodes
  static constexpr unsigned KERNEL_SIZE = 7;                     // Matches KERNEL_SIZE in Python
  static constexpr unsigned NWEIGHTS    = KERNEL_SIZE*2*NNODES;  // KERNEL_SIZE=7, I/Q paths, 64 nodes

  static constexpr double   BUFFER_SIZE_MULTIPLE = NSAMP/2*NNODES*2*2*2.0/65536;  // Ratio of output buffer size to local tile memory
                                                                                  // *2     -> 2 bytes per sample
                                                                                  // *2     -> ping-pong
                                                                                  // *2     -> Margin to ensure input buffer uses only two banks (same applies for output buffer)
                                                                                  // 65536  -> Local tile memory (64KB)
  static constexpr unsigned NSPLIT = BUFFER_SIZE_MULTIPLE;                        // Process incoming samples in NSPLIT chunks so output buffers fit in local tile memory.

  conv1d_w1_graph( void )
  {

    tiling_parameters bdw = { .buffer_dimension   = {NSAMP},
                              .tiling_dimension   = {NSAMP},
                              .offset             = {0} };
    // These tiling parameters pre-pads I/Q paths with 4 complex 0s and post-pads with 4 complex 0s, as opposed to minimum required of 3 and 3.
    // This is done to ensure the kernel input buffer size is a multiple of 16 bytes.
    tiling_parameters bdr = { .buffer_dimension   = {NSAMP},
                              .tiling_dimension   = {NSAMP+4*2*2},
                              .offset             = {-4*2},
                              .tile_traversal     = {{.dimension=0, .stride=NSAMP+4*2*2,.wrap=1}}
                            };

    MT = shared_buffer<bfloat16>::create({NSAMP}, 1, 1);
    write_access(MT.in[0]) = tiling(bdw);
    read_access(MT.out[0]) = tiling(bdr);
    repetition_count(MT) = 1;

    kk = kernel::create_object<conv1d_w1<NSAMP,NNODES,KERNEL_SIZE,NWEIGHTS,NSPLIT>>();
    source(kk) = "conv1d_w1.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = 1;
    stack_size(kk)       = 2048;
    
    single_buffer(kk.in[1]);
    single_buffer(kk.in[2]);

    // Connect inputs:
    connect<> ( data_iq,     MT.in[0] );
    connect<> ( MT.out[0],   kk.in[0] );      dimensions(kk.in[0])  = {NSAMP+4*2*2};
    connect<parameter> ( weights, async(kk.in[1]) );
    connect<parameter> ( biases,  async(kk.in[2]) );
    connect<> ( kk.out[0],   data_o );        dimensions(kk.out[0]) = {NSAMP/2*NNODES/NSPLIT};
    
  }
};

