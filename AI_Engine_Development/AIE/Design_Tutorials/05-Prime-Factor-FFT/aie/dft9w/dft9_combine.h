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
class dft9_combine {
public:
  static constexpr unsigned NSAMP_I = 4*NFFT; // # of samples delivered to combiner by each tile
  // Constructor:
  dft9_combine( void );

  // Run:
  void run(  input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig0_i,
             input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig1_i,
             input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig2_i,
            output_stream<TT_DATA>* __restrict sig0_o,
            output_stream<TT_DATA>* __restrict sig1_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft9_combine::run );
  }
};

