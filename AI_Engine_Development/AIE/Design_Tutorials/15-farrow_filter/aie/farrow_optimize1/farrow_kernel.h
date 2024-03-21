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
  static constexpr unsigned F_TAPS = 16;
  alignas(32) static constexpr int16 f_taps[F_TAPS] ={
		206,-1264,6606,-14835,906,-3543,10352,-7628,-51,316,-1652,20093,-226,886,-2588,10099 // flip(f3_taps.int(end-3:end)),flip(f2_taps.int(end-3:end)),flip(f1_taps.int(end-3:end)),flip(f0_taps.int(end-3:end))
  };

  static constexpr unsigned BUFFER_SIZE = 1024;
  static constexpr unsigned DNSHIFT = 14;

  // State:
  static constexpr unsigned STATE_LEN = 8;
  alignas(32) TT_SIG f_state[STATE_LEN];
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
