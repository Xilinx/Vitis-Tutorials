//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ifft256p4.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ifft256p4::ifft256p4( TT_DATA (&tbuff_i)[NFFT] ) : tbuff(tbuff_i)
{
  aie::set_rounding(aie::rounding_mode::conv_even);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ifft256p4::run(  input_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_i,
                     output_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_o )
{
  // Set pointers to windows:
  auto itw = aie::begin_vector<4>(sig_o);
  TT_DATA* ibuff = sig_i.data();
  TT_DATA* obuff = sig_o.data();

  aie::fft_dit_r4_stage<64>(ibuff, tw4a_1, tw4a_0, tw4a_2, NFFT, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  aie::fft_dit_r4_stage<16>(tbuff, tw4b_1, tw4b_0, tw4b_2, NFFT, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);
  aie::fft_dit_r4_stage< 4>(ibuff, tw4c_1, tw4c_0, tw4c_2, NFFT, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  aie::fft_dit_r4_stage< 1>(tbuff, tw4d_1, tw4d_0, tw4d_2, NFFT, SHIFT_TW, SHIFT_DT, INVERSE, obuff);

  // Write EXTRA samples:
  itw += NFFT/4;
  *itw++ = aie::zeros<TT_DATA,4>();
}
