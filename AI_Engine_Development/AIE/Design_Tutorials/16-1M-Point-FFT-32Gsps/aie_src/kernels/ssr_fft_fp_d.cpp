//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_d.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_d::ssr_fft_fp_d( int (&lut)[256] )
  : fft_lut_tw4(lut)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_d::run( input_buffer <int,extents<2048> >& __restrict inputx,
                        output_buffer<int,extents<2048> >& __restrict outputy )
{
  cfloat * restrict xbuf =  (cfloat * restrict ) inputx.data();
  cfloat * restrict ybuf =  (cfloat *restrict  ) outputy.data();
  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw4;
  // Radix-2 stages
  stage7_radix2_dit_a(  xbuf,       1024,     ybuf);    // s7a
  stage7_radix2_dit_b(  ybuf, twa,  1024,     xbuf);    // s7b
  stage8_radix2_dit_a(  xbuf,       1024,     ybuf);    // s8a
}


