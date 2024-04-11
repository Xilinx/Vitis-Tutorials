//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

class sincos_test {
public:
  typedef unsigned TT_PHASE;
  typedef cint16   TT_TWID;
  static constexpr unsigned NUM_BITS = 20;
  static constexpr unsigned SHIFT = 32 - NUM_BITS;

  // Constructor:
  sincos_test( void );

  // Run:
  void run( output_stream<TT_PHASE>* restrict phase_o,
            output_stream<TT_TWID>* restrict twid_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( sincos_test::run );
  }
};

