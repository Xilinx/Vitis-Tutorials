//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_a.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_a::ssr_fft_fp_a( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_a::run( input_buffer <int,extents<1024> >& __restrict inputx0,
                        input_buffer <int,extents<1024> >& __restrict inputx1,
                        output_buffer<int,extents<2048> >& __restrict outputy )
{
  cfloat * restrict xbuf0 =  (cfloat * restrict) inputx0.data();
  cfloat * restrict xbuf1 =  (cfloat * restrict) inputx1.data();
  cfloat * restrict ybuff =  (cfloat * restrict) outputy.data();
  // Radix-2 stages
  stage0_radix2_dit(  xbuf0, xbuf1, 1024,  ybuff);         // s0
  stage1_radix2_dit(  ybuff,        1024,  xbuf0, xbuf1);  // s1
  stage2_radix2_dit(  xbuf0, xbuf1, 1024,  ybuff);         // s2

}


