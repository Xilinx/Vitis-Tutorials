//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ifft256p4.h"

// ------------------------------------------------------------
// Radix-4 Stage
// ------------------------------------------------------------

template <unsigned Vectorization, class TT_DATA, class TT_TWID>
inline void radix4_dit(const TT_DATA * __restrict x,
                       const TT_TWID * __restrict tw1,
                       const TT_TWID * __restrict tw2,
                       unsigned n, unsigned shift_tw, unsigned shift, bool inv,
                       TT_DATA * __restrict y)
{
  const TT_TWID* dummy = NULL;  // Currently not used
  const unsigned Radix = 4;
  using FFT = aie::fft_dit<Vectorization, Radix, TT_DATA>;

  FFT fft;

  // int block_size = FFT::block_size(n);
  int block_size = n / ( Radix * FFT::out_vector_size);

  auto it_stage  = fft.begin_stage(x, tw1, tw2, dummy);
  auto it_out0 = aie::begin_restrict_vector<FFT::out_vector_size>(y);
  auto it_out1 = aie::begin_restrict_vector<FFT::out_vector_size>(y + 2*n / Radix);

  for (int j = 0; j < block_size; ++j)
    chess_prepare_for_pipelining
    chess_loop_range(1,)
  {
    const auto out = fft.dit(*it_stage++, shift_tw, shift, inv);
    *it_out0 = out[0]; it_out0 +=  block_size;
    *it_out0 = out[1]; it_out0 += -block_size + 1;
    *it_out1 = out[2]; it_out1 +=  block_size;
    *it_out1 = out[3]; it_out1 += -block_size + 1;
  }
}

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

  // Perform FFT:
  radix4_dit<64,TT_DATA,TT_TWID>(ibuff,  tw1,  tw2,   NFFT, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  radix4_dit<16,TT_DATA,TT_TWID>(tbuff,  tw4,  tw8,   NFFT, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);
  radix4_dit< 4,TT_DATA,TT_TWID>(ibuff,  tw16, tw32,  NFFT, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  radix4_dit< 1,TT_DATA,TT_TWID>(tbuff,  tw64, tw128, NFFT, SHIFT_TW, SHIFT_DT, INVERSE, obuff);

  // Write EXTRA samples:
  itw += NFFT/4;
  *itw++ = aie::zeros<TT_DATA,4>();
}
