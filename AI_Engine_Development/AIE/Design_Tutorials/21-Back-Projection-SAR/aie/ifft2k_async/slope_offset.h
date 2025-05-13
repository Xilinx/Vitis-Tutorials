//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class slope_offset {
public:
  static constexpr unsigned      NSAMP = 2048;
  static constexpr float       scale_x = 2048.0;
  static constexpr float       delta_x = 4.8828125e-04; // 1/2048
  static constexpr float        step_x = 3.90625e-3;    // 8/2048

  alignas(16) float x_init[8] = {0,delta_x,2*delta_x,3*delta_x,4*delta_x,5*delta_x,6*delta_x,7*delta_x };

  slope_offset(void);

  void run( input_buffer<cfloat>& sig_i,
            output_async_buffer<float>& real_o, output_async_buffer<float>& imag_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( slope_offset::run );
  }
};

