//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template< unsigned NSTREAM, unsigned PHASE>
class ifft256p4_rot {
public:
  typedef cint32 TT_DATA;
  typedef cint16 TT_TWID;
  static constexpr unsigned  NFFT_1D = 256;
  static constexpr unsigned     NFFT = NFFT_1D * NFFT_1D;
  static constexpr unsigned    EXTRA = 4;
  static constexpr unsigned    NSAMP = NFFT_1D + EXTRA;
  static constexpr bool      INVERSE = true;
  static constexpr unsigned SHIFT_TW = 15;
  static constexpr unsigned SHIFT_DT = 15;

  // Twiddle factor argument:
  // exp(+1i*2*pi*r*c/NFFT) --> 2*pi*r*c/NFFT = pi*r*c/(2^15)
  // aie::sincos(arg) is normalized by 1/pi and 'arg' is provided in Q1.31 format.
  // --> we need to shift by (31-15) = 16 in order to normalize phase step "r" by 2^15
  static constexpr unsigned ALIGN = 16;

  // Count off the rows:
  unsigned COUNT;

private:

  // Temporary Buffers:
  alignas(32) TT_DATA (&tbuff)[NFFT_1D];

  // Twiddle factors:
#include "ifft256p4_rot_twiddle.h"

public:
  // Constructor:
  ifft256p4_rot( TT_DATA (&tbuff_i)[NFFT_1D] );

  // Run:
  void run(  input_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_i,
            output_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ifft256p4_rot::run );
    REGISTER_PARAMETER( tbuff );
  }
};
