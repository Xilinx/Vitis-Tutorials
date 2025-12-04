//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class max_pool1d_w2 {
public:
  typedef bfloat16  TT_DATA;
  static constexpr unsigned NSAMP       = 1024;              // 1024 from each node
  static constexpr unsigned NNODES      = 64;                // 64 nodes
  static constexpr double   BUFFER_SIZE_MULTIPLE = NSAMP*NNODES*2*2*2.0/65536;  // Ratio of IO buffer size to local tile memory
                                                                                // 22/16  -> Bandwidth and kernel input buffer grows by a factor of 22/16 due to nature of processing.
                                                                                // *2     -> 2 bytes per sample
                                                                                // *2     -> ping-pong
                                                                                // *2     -> Margin to ensure input buffer uses only two banks (same applies for output buffer)
                                                                                // 65536  -> Local tile memory (64KB)
  static constexpr unsigned NSPLIT = BUFFER_SIZE_MULTIPLE;                      // Process incoming samples in NSPLIT chunks so IO buffers fit in local tile memory.

private:

public:
  // Constructor:
  max_pool1d_w2( void );

  // Run:
  void run( input_buffer<TT_DATA>&  __restrict data_i,
            output_buffer<TT_DATA>& __restrict data_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( max_pool1d_w2::run );
  }
};

