//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class fmod_floor {
public:
  static constexpr unsigned NSAMP  = 512; // Note: 512 x 512 --> Need 512 graph iteration for full image
  static constexpr unsigned NPULSE = 128;

  fmod_floor(void);

  // Run:
  void run( input_buffer<float>& sig_i, output_buffer<float>& sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( fmod_floor::run );
  }
};

