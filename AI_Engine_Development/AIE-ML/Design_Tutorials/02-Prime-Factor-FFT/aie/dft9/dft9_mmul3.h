//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class dft9_3 {
public:
  typedef cint16 TT_DATA;
  typedef cint16 TT_TWID;
  typedef cacc64 TT_ACC;
  static constexpr unsigned NUM_FFT = 4*7*16;
  static constexpr unsigned NSAMP_O = 9*NUM_FFT;
  static constexpr unsigned DNSHIFT = 15;

  // Constructor:
  dft9_3(void);

  // Run:
  void run(  input_cascade<TT_ACC>* __restrict acc_i,
             output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft9_3::run );
  }
};

