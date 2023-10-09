//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<class TT_DATA, class TT_TWIDDLE, class TT_ACC, unsigned NFFT>
class dft16_compute {
public:
  static constexpr unsigned NSAMP_O = NFFT*4; // 1 transform x 4 samples per tile
  static constexpr unsigned DNSHIFT = 15;
  static constexpr unsigned COEFF_DEPTH = 16*4;
private:
  // DFT coefficients:
  alignas(16) TT_TWIDDLE (&coeff0)[COEFF_DEPTH/2];
  alignas(16) TT_TWIDDLE (&coeff1)[COEFF_DEPTH/2];
public:
  // Constructor:
  dft16_compute( TT_TWIDDLE (&coeff0_i)[COEFF_DEPTH/2], TT_TWIDDLE (&coeff1_i)[COEFF_DEPTH/2] );

  // Run:
  void run( input_stream<TT_DATA>* restrict sig0_i,
            input_stream<TT_DATA>* restrict sig1_i,
            output_buffer<TT_DATA,extents<NSAMP_O> >& restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft16_compute::run );
    REGISTER_PARAMETER( coeff0 );
    REGISTER_PARAMETER( coeff1 );
  }
};

