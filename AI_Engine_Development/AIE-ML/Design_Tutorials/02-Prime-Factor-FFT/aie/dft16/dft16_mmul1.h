//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class dft16_1 {
public:
  typedef cint16 TT_DATA;
  typedef cint16 TT_TWID;
  typedef cacc48 TT_ACC;
  static constexpr unsigned NUM_FFT = 4*7*9;
  static constexpr unsigned NSAMP_I = 16*NUM_FFT; // 16 samples per transform
  static constexpr unsigned NSAMP_O = 16*NUM_FFT; // 16 samples per transform
  static constexpr unsigned DNSHIFT = 15;
  static constexpr unsigned COEFF_DEPTH = 16*16; // 16 x 16
private:
  // DFT coefficients:
  alignas(16) TT_TWID (&coeff)[COEFF_DEPTH];
public:
  // Constructor:
  dft16_1( TT_TWID (&coeff_i)[COEFF_DEPTH] );

  // Run:
  void run( input_cascade<TT_ACC>* __restrict acc_i,
            input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig_i,
            output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft16_1::run );
    REGISTER_PARAMETER( coeff );
  }
};

