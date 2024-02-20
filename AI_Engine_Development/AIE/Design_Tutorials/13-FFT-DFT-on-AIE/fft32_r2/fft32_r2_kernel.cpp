//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "fft32_r2_kernel.h"

fft32_r2_kernel::fft32_r2_kernel( void )
{
  aie::set_rounding(aie::rounding_mode::positive_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

void fft32_r2_kernel::run(  input_buffer<TT_DATA,extents<WIN_SIZE> >& __restrict sig_i,
                           output_buffer<TT_DATA,extents<WIN_SIZE> >& __restrict sig_o )
{
  // Set pointers to windows:
  TT_DATA* ibuff = sig_i.data();
  TT_DATA* obuff = sig_o.data();
  // Perform FFT:
  for (int rr=0; rr < REPEAT; rr++)
    chess_prepare_for_pipelining
    chess_loop_range(REPEAT,)
  {
    aie::fft_dit_r2_stage<16>(ibuff, tw1,  N, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
    aie::fft_dit_r2_stage< 8>(tbuff, tw2,  N, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);
    aie::fft_dit_r2_stage< 4>(ibuff, tw4,  N, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
    aie::fft_dit_r2_stage< 2>(tbuff, tw8,  N, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);
    aie::fft_dit_r2_stage< 1>(ibuff, tw16, N, SHIFT_TW, SHIFT_DT, INVERSE, obuff);
    ibuff += N;
    obuff += N;
  }
}

