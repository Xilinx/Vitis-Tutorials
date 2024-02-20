//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "fft32_r2_twiddle.h"

using namespace adf;

class fft32_r2_kernel {
public:
  typedef cint16 TT_DATA;
  typedef cint16 TT_TWID;
  static constexpr unsigned N = 32;
  static constexpr unsigned SHIFT_TW = 15;
  static constexpr unsigned SHIFT_DT = 15;
  static constexpr bool     INVERSE  = false;
  static constexpr unsigned REPEAT   = 128;
  static constexpr unsigned WIN_SIZE = N * REPEAT;

private:

  // Temporary Buffers:
  alignas(aie::vector_decl_align) TT_DATA tbuff[N];

  // Twiddle factors:
  alignas(aie::vector_decl_align) static constexpr TT_TWID    tw1[ 1] = TWID1;
  alignas(aie::vector_decl_align) static constexpr TT_TWID    tw2[ 2] = TWID2;
  alignas(aie::vector_decl_align) static constexpr TT_TWID    tw4[ 4] = TWID4;
  alignas(aie::vector_decl_align) static constexpr TT_TWID    tw8[ 8] = TWID8;
  alignas(aie::vector_decl_align) static constexpr TT_TWID   tw16[16] = TWID16;

public:
  // Constructor:
  fft32_r2_kernel(void);

  // Run:
  void run(  input_buffer<TT_DATA,extents<WIN_SIZE> >& __restrict sig_i,
            output_buffer<TT_DATA,extents<WIN_SIZE> >& __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( fft32_r2_kernel::run );
  }
};


