//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class farrow_kernel2 {
public:
  typedef cint16  TT_SIG;
  typedef int32   TT_DEL;
  typedef cacc48  TT_ACC;

  static constexpr unsigned BUFFER_SIZE = 1024;
  static constexpr unsigned DNSHIFT = 14;
private:
  // State:

  alignas(32) TT_SIG z[BUFFER_SIZE];

public:
  // Constructor:
  farrow_kernel2( void );

  // Run:
  void run( input_buffer<TT_DEL,adf::extents<BUFFER_SIZE>>&  __restrict del_i,
            input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y3,
            input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y2,
            input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y1,
            input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y0,
            output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict sig_o);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( farrow_kernel2::run );
  }
};
