//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class farrow_kernel {
public:
  typedef cint16  TT_SIG;
  typedef int32   TT_DEL;
  typedef cacc48  TT_ACC;
  static constexpr unsigned F3_TAPS = 8;
  alignas(32) static constexpr int16 f3_taps[F3_TAPS] ={
		206,-1264,6606,-14835,0,0,0,0        // flip(f3_taps.int(end-3:end))
  };
  static constexpr unsigned F2_TAPS = 8;
  alignas(32) static constexpr int16 f2_taps[F2_TAPS] ={
		906,-3543,10352,-7628,0,0,0,0        // flip(f2_taps.int(end-3:end))
  };
  static constexpr unsigned F1_TAPS = 8;
  alignas(32) static constexpr int16 f1_taps[F1_TAPS] ={
		-51,316,-1652,20093,0,0,0,0          // flip(f1_taps.int(end-3:end))
  };
  static constexpr unsigned F0_TAPS = 8;
  alignas(32) static constexpr int16 f0_taps[F0_TAPS] ={
		-226,886,-2588,10099,0,0,0,0         // flip(f0_taps.int(end-3:end))
  };

  static constexpr unsigned BUFFER_SIZE = 1024;
  static constexpr unsigned DNSHIFT = 14;

  // State:
  static constexpr unsigned STATE_LEN = 8;
  alignas(32) TT_SIG f3_state[STATE_LEN];
  alignas(32) TT_SIG f2_state[STATE_LEN];
  alignas(32) TT_SIG f1_state[STATE_LEN];
  alignas(32) TT_SIG f0_state[STATE_LEN];
private:

public:
  // Constructor:
  farrow_kernel( void );

  // Run:
  void run( input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict sig_i,
            input_buffer<TT_DEL,adf::extents<BUFFER_SIZE>>&  __restrict del_i,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict sig_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( farrow_kernel::run );
  }
};
