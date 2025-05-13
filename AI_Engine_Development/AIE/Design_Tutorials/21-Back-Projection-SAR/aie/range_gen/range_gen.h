//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned REPEAT,unsigned NSAMP,unsigned ID>
class range_gen {
public:
  typedef float    TT_DATA;
  typedef accfloat TT_ACC;
  static constexpr unsigned      NPIXEL = 512; // # of pixels for square image
  static constexpr unsigned     NENGINE = NPIXEL*NPIXEL/REPEAT/NSAMP;
  static constexpr TT_DATA    RANGE_MIN = -50.0;
  static constexpr TT_DATA    RANGE_MAX = +50.0;
  static constexpr TT_DATA    RANGE_INC = (RANGE_MAX-RANGE_MIN)/float(NPIXEL);
  static constexpr TT_DATA   RANGE_STEP = 8*RANGE_INC;
  static constexpr unsigned    TOT_SAMP = NPIXEL*NPIXEL/NENGINE;
  static constexpr TT_DATA       X_INIT = RANGE_MIN + float(NPIXEL/NENGINE*ID)*RANGE_INC;

  static_assert(NSAMP*REPEAT*NENGINE==NPIXEL*NPIXEL,"Incorrect image parameters");

  alignas(16) TT_DATA Y_INIT[8] = { RANGE_MAX-0*RANGE_INC, RANGE_MAX-1*RANGE_INC,
                                    RANGE_MAX-2*RANGE_INC, RANGE_MAX-3*RANGE_INC,
                                    RANGE_MAX-4*RANGE_INC, RANGE_MAX-5*RANGE_INC,
                                    RANGE_MAX-6*RANGE_INC, RANGE_MAX-7*RANGE_INC } ;
  range_gen(void);

  // Run:
  void run( output_stream<TT_DATA>* sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( range_gen::run );
  }
};

