//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_d {
public:

  alignas(16) int (&fft_lut_tw4)[256];

  // Constructor:
  ssr_fft_fp_d( int (&lut)[256] );

  // Run:
  void run( input_buffer <int,extents<2048> >& __restrict inputx,
            output_buffer<int,extents<2048> >& __restrict outputy );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_d::run );
    REGISTER_PARAMETER( fft_lut_tw4 );
  }
};

