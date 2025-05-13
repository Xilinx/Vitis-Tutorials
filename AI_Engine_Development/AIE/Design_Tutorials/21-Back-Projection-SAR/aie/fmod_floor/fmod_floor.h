//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned NSAMP>
class fmod_floor {
public:
  typedef float TT_DATA;
  static constexpr float MAXVAL = 0.9990234375; // This is lower edge of last bin in sin/cos LUTs to follow

  alignas(16) float SCRATCH[NSAMP];
  fmod_floor(void);

  // Run:
  void run( input_buffer<TT_DATA>& sig_i, output_buffer<TT_DATA>& sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( fmod_floor::run );
  }
};

