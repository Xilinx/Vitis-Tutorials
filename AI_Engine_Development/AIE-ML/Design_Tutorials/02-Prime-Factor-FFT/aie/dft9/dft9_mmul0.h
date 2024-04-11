//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class dft9_0 {
public:
  typedef cint16 TT_DATA;
  typedef cint16 TT_TWID;
  typedef cacc64 TT_ACC;
  static constexpr unsigned NUM_FFT = 4*7*16;
  static constexpr unsigned NSAMP_I = 9*NUM_FFT; // 9 samples per transform
  static constexpr unsigned DNSHIFT = 15;
  static constexpr unsigned COEFF_DEPTH = 4*16;
private:
  // DFT coefficients:
  alignas(16) TT_TWID (&coeff)[COEFF_DEPTH];
public:
  // Constructor:
  dft9_0( TT_TWID (&coeff_i)[COEFF_DEPTH] );

  // Run:
  void run(  input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig_i,
             output_cascade<TT_ACC>* __restrict acc_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft9_0::run );
    REGISTER_PARAMETER( coeff );
  }
};

