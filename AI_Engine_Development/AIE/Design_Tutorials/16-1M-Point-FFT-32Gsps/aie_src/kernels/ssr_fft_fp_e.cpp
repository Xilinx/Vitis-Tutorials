//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_e.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_e::ssr_fft_fp_e( int (&fftbuf_i)[2048], int (&lut5)[512], int (&lut6)[1024] )
  : fftbuf(fftbuf_i), fft_lut_tw5(lut5), fft_lut_tw6(lut6)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_e::run( input_buffer <int,extents<2048> >& __restrict inputx,
                        output_buffer<int,extents<1024> >& __restrict outputy0,
                        output_buffer<int,extents<1024> >& __restrict outputy1 )
{
  cfloat * restrict xbuf =  (cfloat * restrict) inputx.data();
  cfloat * restrict ybuf0=  (cfloat * restrict) outputy0.data();
  cfloat * restrict ybuf1=  (cfloat * restrict) outputy1.data();

  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw5;
  cfloat * restrict twb = (cfloat * restrict) fft_lut_tw6;

  // Radix-2 stages
  stage8_radix2_dit_b( xbuf,     twa,      1024,  (cfloat * restrict)  fftbuf);  // s8b
  stage9_radix2_dit(   (cfloat * restrict) fftbuf,       twb,      512,   ybuf0);  // s9
  stage9_radix2_dit(  ((cfloat * restrict) fftbuf)+8,   twb+4,    512,   ybuf1);  // s9
}


