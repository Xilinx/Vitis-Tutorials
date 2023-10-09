//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<class TT_DATA, unsigned NFFT>
class dft16_combine {
public:
  static constexpr unsigned NSAMP_I = NFFT*4; // 1 transforms x 4 samples per tile
  // Constructor:
  dft16_combine( void );

  // Run:
  void run( input_buffer<TT_DATA,extents<NSAMP_I> >& restrict sig0_i,
            input_buffer<TT_DATA,extents<NSAMP_I> >& restrict sig1_i,
            input_buffer<TT_DATA,extents<NSAMP_I> >& restrict sig2_i,
            input_buffer<TT_DATA,extents<NSAMP_I> >& restrict sig3_i,
            output_stream<TT_DATA>* restrict sig0_o,
            output_stream<TT_DATA>* restrict sig1_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft16_combine::run );
  }
};

