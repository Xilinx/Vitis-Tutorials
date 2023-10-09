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
class dft9_compute {
public:
  static constexpr unsigned NSAMP_I = 9*NFFT/2; // # of samples on each input stream
  static constexpr unsigned NSAMP_O = 4*NFFT; // # of samples delivered to combiner from each tile
  static constexpr unsigned DNSHIFT = 15;
  static constexpr unsigned COEFF_DEPTH = 9*4;
private:
  // DFT coefficients:
  // --> Stores left half of twiddles 9 x 4, taken in row-major order
  alignas(16) TT_TWIDDLE (&coeff)[COEFF_DEPTH];
public:
  // Constructor:
  dft9_compute( TT_TWIDDLE (&coeff_i)[COEFF_DEPTH] );

  // Run:
  void run(  input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig0_i,
             input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig1_i,
             output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft9_compute::run );
    REGISTER_PARAMETER( coeff );
  }
};

