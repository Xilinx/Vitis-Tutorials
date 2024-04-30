//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_b.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_b::ssr_fft_fp_b( int (&fftbuf_i)[2048], int (&lut0)[16], int (&lut1)[32] )
  : fftbuf(fftbuf_i), fft_lut_tw0(lut0), fft_lut_tw1(lut1)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_b::run( input_buffer <int,extents<2048> >& __restrict inputx,
                        output_buffer<int,extents<2048> >& __restrict outputy )
{
  cfloat * restrict xbuf =  (cfloat * restrict)  inputx.data();
  cfloat * restrict ybuf =  (cfloat * restrict) outputy.data();

  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw0;
  cfloat * restrict twb = (cfloat * restrict) fft_lut_tw1;

  stage3_radix2_dit(  xbuf,   twa,  1024,   (cfloat * restrict) fftbuf);  // s3
  stage4_radix2_dit(  (cfloat * restrict) fftbuf, twb,  1024,       ybuf);    // s4
}


