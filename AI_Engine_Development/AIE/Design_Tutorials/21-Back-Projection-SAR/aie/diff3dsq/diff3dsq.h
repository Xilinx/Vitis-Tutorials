//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned REPEAT,unsigned NSAMP>
class diff3dsq {
public:
  typedef float    TT_DATA;
  typedef accfloat TT_ACC;
  static constexpr unsigned NPULSE = 586;
  static constexpr unsigned NCOORD = 3*NPULSE;
  static constexpr float    sqrt_compress = 1.025640994e-08; // Equals 1.0/97.5e6. Compress input to (1,4)

  // Current pulse:
  unsigned pulse;
  unsigned count;

  diff3dsq();

  // Run:
  void run( input_buffer<TT_DATA>& sig_i, const TT_DATA (&coord)[NCOORD], output_buffer<TT_DATA>& sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( diff3dsq::run );
  }
};

