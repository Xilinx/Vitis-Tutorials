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
class bp_update {
public:

  bp_update(void);

  // Run:
  void run( input_buffer <cfloat>& image_i,
            input_buffer <cfloat>& interp_i,
            input_buffer <cfloat>& phase_i,
            output_buffer<cfloat>& image_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( bp_update::run );
  }
};

