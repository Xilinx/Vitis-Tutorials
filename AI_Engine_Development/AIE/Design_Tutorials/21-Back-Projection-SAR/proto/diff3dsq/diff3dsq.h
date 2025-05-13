//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class diff3dsq {
public:
  typedef float    TT_DATA;
  typedef accfloat TT_ACC;
  static constexpr unsigned NSAMP  = 512; // Note: 512 x 512 --> Need 512 graph iteration for full image
  static constexpr unsigned NPULSE = 128;

  // Lookup table for (xA,yA,zA aperture coordinates:
  alignas(16) TT_DATA (&coord)[3*NPULSE];

  // Current pulse:
  unsigned pulse;
  unsigned count;

  diff3dsq( TT_DATA (&coord_i)[3*NPULSE] );

  // Run:
  void run( input_buffer<TT_DATA>& sig_i, output_buffer<TT_DATA>& sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( diff3dsq::run );
    REGISTER_PARAMETER( coord );
  }
};

