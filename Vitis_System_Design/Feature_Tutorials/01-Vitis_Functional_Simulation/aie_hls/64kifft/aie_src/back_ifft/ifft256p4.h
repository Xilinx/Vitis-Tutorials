//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ifft256p4 {
public:
  typedef cint32 TT_DATA;
  typedef cint16 TT_TWID;
  static constexpr unsigned     NFFT = 256;
  static constexpr unsigned    EXTRA = 4;
  static constexpr unsigned    NSAMP = NFFT + EXTRA;
  static constexpr bool      INVERSE = true;
  static constexpr unsigned SHIFT_TW = 15;
  static constexpr unsigned SHIFT_DT = 15;


private:

  // Temporary Buffers:
  alignas(32) TT_DATA (&tbuff)[NFFT];

  // Twiddle factors:
#include "ifft256p4_twiddle.h"

public:
  // Constructor:
  ifft256p4( TT_DATA (&tbuff_i)[NFFT] );

  // Run:
  void run(  input_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_i,
            output_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ifft256p4::run );
    REGISTER_PARAMETER( tbuff );
  }
};
