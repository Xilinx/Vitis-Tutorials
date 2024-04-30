//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_c.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_c::ssr_fft_fp_c( int (&fftbuf_i)[2048], int (&lut2)[64], int (&lut3)[128] )
  : fftbuf(fftbuf_i), fft_lut_tw2(lut2), fft_lut_tw3(lut3)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_c::run( input_buffer <int,extents<2048> >& __restrict inputx,
                        output_buffer<int,extents<2048> >& __restrict outputy )
{
  cfloat * restrict xbuf =  (cfloat * restrict)inputx.data();
  cfloat * restrict ybuf =  (cfloat * restrict)outputy.data();

  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw2;
  cfloat * restrict twb = (cfloat * restrict) fft_lut_tw3;

  // Radix-2 stages
  stage5_radix2_dit(  xbuf,   twa,  1024,   (cfloat * restrict)    fftbuf);  // s5
  stage6_radix2_dit(  (cfloat * restrict) fftbuf, twb,  1024,       ybuf);    // s6
}


