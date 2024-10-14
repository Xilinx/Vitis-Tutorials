//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class farrow_kernel1 {
public:
  typedef cint16  TT_SIG;
  typedef cacc64  TT_ACC; 
  
  static constexpr unsigned BUFFER_SIZE = 1024;
  static constexpr unsigned DNSHIFT = 14;

private:
  // State:
  static constexpr unsigned STATE_LEN = 8;
  alignas(32) TT_SIG f_state[STATE_LEN];
  alignas(32) int16 (&f_taps)[8];

public:
  // Constructor:
  farrow_kernel1( int16 (&taps)[8] );
 
  // Run:
  void run( input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict sig_i,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict y );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( farrow_kernel1::run );
    REGISTER_PARAMETER( f_taps );
  }
};
