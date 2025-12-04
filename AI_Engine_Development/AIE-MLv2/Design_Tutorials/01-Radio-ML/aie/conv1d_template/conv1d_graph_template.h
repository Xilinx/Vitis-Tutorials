//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "conv1d_template.h"
#include "next_pow_2_constexpr.h"
#include "ceil_constexpr.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

// NSAMP is number of samples from each node, assumed to be a multiple of 8 and 16 <= NSAMP <= 512 
// NNODES is number nodes, assumed to be equal to 64.
// KERNEL_SIZE is the number of weights per node, assumed to be 7

template<unsigned NSAMP, unsigned NNODES, unsigned KERNEL_SIZE>
class conv1d_graph_template : public graph {
public:
  // Declare Kernels
  kernel kk;

  // Declare IOs
  port<input>             data_i;
  port<direction::in>     weights;
  port<direction::in>     biases;
  port<output>            data_o;
  
  static constexpr unsigned NWEIGHTS = NNODES*NNODES*KERNEL_SIZE;                // Number of weights, dependent on NNODES and KERNEL_SIZE
  static constexpr unsigned NBIASES  = NNODES;                                   // Number of biases, dependent on NNODES
  static constexpr int      NZERO_PAD = (KERNEL_SIZE-1)/2;                       // pre/post pad input buffer so output has same NSAMP as input. KERNEL_SIZE assumed to be odd.

  static constexpr double   BUFFER_SIZE_MULTIPLE = NSAMP*NNODES*22/16*2*2*2.0/65536;  // Ratio of IO buffer size to local tile memory
                                                                                      // 22/16  -> Bandwidth and kernel input buffer grows by a factor of 22/16 due to nature of processing.
                                                                                      // *2     -> 2 bytes per sample
                                                                                      // *2     -> ping-pong
                                                                                      // *2     -> Margin to ensure input buffer uses only two banks (same applies for output buffer)
                                                                                      // 65536  -> Local tile memory (64KB)
  static constexpr unsigned NSPLIT = (BUFFER_SIZE_MULTIPLE>1) ?
                                     next_pow_2_constexpr(BUFFER_SIZE_MULTIPLE) :
                                     ceil_constexpr(BUFFER_SIZE_MULTIPLE);
                                                                                 // Process incoming samples in NSPLIT chunks so IO buffers fit in local tile memory.
                                                                                 // Return type of ceil in c++17 is not constexpr

  // Declare memory tiles
  shared_buffer<bfloat16> MT_input;
  shared_buffer<bfloat16> MT_output;
  
  conv1d_graph_template( void )
  {
    // Tiling parameters for inputs
    tiling_parameters bdw_input = { .buffer_dimension   = {NNODES*NSAMP},
                                    .tiling_dimension   = {NNODES*NSAMP},
                                    .offset             = {0} };
    tiling_parameters bdr_input = { .buffer_dimension   = {NNODES  ,NSAMP},
                                    .tiling_dimension   = {8,22},
                                    .offset             = {0,-NZERO_PAD},
                                    .tile_traversal     = {{.dimension=0, .stride=8,.wrap=NNODES/8}, 
                                                           {.dimension=1, .stride=16,.wrap=NSAMP/16}}
                                                          };
    MT_input = shared_buffer<bfloat16>::create({NSAMP*NNODES}, 1, 1);
    write_access(MT_input.in[0]) = tiling(bdw_input);
    read_access(MT_input.out[0]) = tiling(bdr_input);
    repetition_count(MT_input) = 1;

    // Tiling parameters for outputs
    tiling_parameters bdw_output = { .buffer_dimension   = {NNODES,NSAMP},
                                     .tiling_dimension   = {8,16},
                                     .offset             = {0,0},
                                     .tile_traversal     = {{.dimension=0, .stride=8, .wrap=NNODES/8},
                                                            {.dimension=1, .stride=16, .wrap=NSAMP/16}}
                                                           };
    tiling_parameters bdr_output = { .buffer_dimension   = {NNODES*NSAMP},
                                     .tiling_dimension   = {NNODES*NSAMP},
                                     .offset             = {0} };
    MT_output = shared_buffer<bfloat16>::create({NNODES*NSAMP}, 1, 1);
    write_access(MT_output.in[0]) = tiling(bdw_output);
    read_access(MT_output.out[0]) = tiling(bdr_output);
    repetition_count(MT_output) = 1;

    kk = kernel::create_object<conv1d_template<NSAMP,NNODES,KERNEL_SIZE,NWEIGHTS,NSPLIT>>();
    source(kk) = "conv1d_template.cpp";
    runtime<ratio>(kk) = 0.9;
    repetition_count(kk) = NSPLIT;
    
    connect<> ( data_i,            MT_input.in[0] );
    connect<> ( MT_input.out[0],   kk.in[0] );           dimensions(kk.in[0])  = {NNODES*NSAMP/NSPLIT*22/16};
    connect<parameter> ( weights,  async(kk.in[1]) );
    connect<parameter> ( biases,   async(kk.in[2]) );
    connect<> ( kk.out[0],         MT_output.in[0] );
    connect<> ( MT_output.out[0],  data_o );             dimensions(kk.out[0]) = {NNODES*NSAMP/NSPLIT};
    
    single_buffer(kk.in[1]);
    single_buffer(kk.in[2]);

  }
};

