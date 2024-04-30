//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_e {
public:

  alignas(16) int (&fftbuf)[2048];
  alignas(16) int (&fft_lut_tw5)[512];
  alignas(16) int (&fft_lut_tw6)[1024];

  // Constructor:
  ssr_fft_fp_e( int (&fftbuf_i)[2048], int (&lut5)[512], int (&lut6)[1024] );

  // Run:
  void run( input_buffer <int,extents<2048> >& __restrict inputx,
            output_buffer<int,extents<1024> >& __restrict outputy0,
            output_buffer<int,extents<1024> >& __restrict outputy1 );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_e::run );
    REGISTER_PARAMETER( fft_lut_tw5 );
    REGISTER_PARAMETER( fft_lut_tw6 );
    REGISTER_PARAMETER( fftbuf );
  }
};

