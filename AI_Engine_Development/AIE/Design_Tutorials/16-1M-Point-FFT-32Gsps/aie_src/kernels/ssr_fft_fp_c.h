//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_c {
public:

  alignas(16) int (&fftbuf)[2048];
  alignas(16) int (&fft_lut_tw2)[64];
  alignas(16) int (&fft_lut_tw3)[128];

  // Constructor:
  ssr_fft_fp_c( int (&fftbuf_i)[2048], int (&lut2)[64], int (&lut3)[128] );

  // Run:
  void run( input_buffer <int,extents<2048> >& __restrict inputx,
            output_buffer<int,extents<2048> >& __restrict outputy );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_c::run );
    REGISTER_PARAMETER( fft_lut_tw2 );
    REGISTER_PARAMETER( fft_lut_tw3 );
    REGISTER_PARAMETER( fftbuf );
  }
};

