//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ifft2k_async.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ifft2k_async::ifft2k_async( cfloat (&tw1024_i)[1024], cfloat (&tw512_i)[512] )
  : tw1024(tw1024_i), tw512(tw512_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ifft2k_async::run( input_buffer<cfloat>& sig_i, output_buffer<cfloat>& sig_o )
{
  cfloat* ibuff = sig_i.data();
  cfloat* obuff = sig_o.data();
  // Compute IFFT:
  aie::fft_dit_r2_stage<1024>(ibuff,tw1,   NSAMP,true,obuff);
  aie::fft_dit_r2_stage< 512>(obuff,tw2,   NSAMP,true,ibuff);
  aie::fft_dit_r2_stage< 256>(ibuff,tw4,   NSAMP,true,obuff);
  aie::fft_dit_r2_stage< 128>(obuff,tw8,   NSAMP,true,ibuff);
  aie::fft_dit_r2_stage<  64>(ibuff,tw16,  NSAMP,true,obuff);
  aie::fft_dit_r2_stage<  32>(obuff,tw32,  NSAMP,true,ibuff);
  aie::fft_dit_r2_stage<  16>(ibuff,tw64,  NSAMP,true,obuff);
  aie::fft_dit_r2_stage<   8>(obuff,tw128, NSAMP,true,ibuff);
  aie::fft_dit_r2_stage<   4>(ibuff,tw256, NSAMP,true,obuff);
  aie::fft_dit_r2_stage<   2>(obuff,tw512, NSAMP,true,ibuff);
  aie::fft_dit_r2_stage<   1>(ibuff,tw1024,NSAMP,true,obuff);
  // The loop below applies a 1/N scaling and effects the fftshift() reordering:
  auto itr0 = aie::begin_restrict_vector<4>(sig_o);
  auto itr1 = aie::begin_restrict_vector<4>(sig_o);  itr1 += NSAMP/8;
  auto itw0 = aie::begin_vector<4>(sig_o);
  auto itw1 = aie::begin_vector<4>(sig_o);  itw1 += NSAMP/8;
  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    auto vec_pos = aie::mul(*itr0++,SCALE);
    auto vec_neg = aie::mul(*itr1++,SCALE);
    *itw0++ = vec_neg;
    *itw1++ = vec_pos;
  } // rr
}


