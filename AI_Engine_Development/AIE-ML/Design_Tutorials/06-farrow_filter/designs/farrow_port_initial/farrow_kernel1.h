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
  alignas(32) static constexpr int16 f_taps[32] ={
   		206,-1264,6606,-14835,14835,-6606,1264,-206,  // anti-sym 
      906,-3543,10352,-7628,-7628,10352,-3543,906,  // sym
      -51,316,-1652,20093,-20093,1652,-316,51,      // anti-sym
      -226,886,-2588,10099,10099,-2588,886,-226     // sym    // flip(f3_taps.int(end-3:end)),flip(f2_taps.int(end-3:end)),flip(f1_taps.int(end-3:end)),flip(f0_taps.int(end-3:end))
  };

  static constexpr unsigned BUFFER_SIZE = 1024;
  static constexpr unsigned DNSHIFT = 14;
private:
  // State:
  static constexpr unsigned STATE_LEN = 8;
  alignas(32) TT_SIG f_state[STATE_LEN];

public:
  // Constructor:
  farrow_kernel1( void );

  // Run:
  void run( input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict sig_i,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict y3,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict y2,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict y1,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict y0);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( farrow_kernel1::run );
  }
};
