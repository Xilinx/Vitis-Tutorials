//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class bp_update {
public:
  static constexpr unsigned NSAMP  = 512; // Note: 512 x 512 --> Need 512 graph iteration for full image
  static constexpr unsigned NPULSE = 128;

  bp_update(void);

  // Run:
  void run( input_buffer <cfloat>& pixel_i,
            input_buffer <cfloat>& dist_i,
            input_buffer <cfloat>& phcorr_i,
            output_buffer<cfloat>& pixel_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( bp_update::run );
  }
};

