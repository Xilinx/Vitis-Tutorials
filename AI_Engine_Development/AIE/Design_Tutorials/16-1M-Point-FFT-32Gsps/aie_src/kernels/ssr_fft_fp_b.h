//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_b {
public:

  alignas(16) int (&fftbuf)[2048];
  alignas(16) int (&fft_lut_tw0)[16];
  alignas(16) int (&fft_lut_tw1)[32];

  // Constructor:
  ssr_fft_fp_b( int (&fftbuf_i)[2048], int (&lut0)[16], int (&lut1)[32] );

  // Run:
  void run( input_buffer <int,extents<2048> >& __restrict inputx,
            output_buffer<int,extents<2048> >& __restrict outputy );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_b::run );
    REGISTER_PARAMETER( fft_lut_tw0 );
    REGISTER_PARAMETER( fft_lut_tw1 );
    REGISTER_PARAMETER( fftbuf );
  }
};

