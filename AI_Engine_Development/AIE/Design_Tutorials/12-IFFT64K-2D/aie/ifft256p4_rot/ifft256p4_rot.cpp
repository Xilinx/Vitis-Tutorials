//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ifft256p4_rot.h"

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

template <unsigned NSTREAM, unsigned PHASE>
ifft256p4_rot<NSTREAM,PHASE>::ifft256p4_rot( TT_DATA (&tbuff_i)[NFFT_1D] ) : tbuff(tbuff_i)
{
  aie::set_rounding(aie::rounding_mode::conv_even);
  aie::set_saturation(aie::saturation_mode::saturate);
  COUNT = PHASE;
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template <unsigned NSTREAM, unsigned PHASE>
void ifft256p4_rot<NSTREAM,PHASE>::run(  input_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_i,
                                         output_buffer<TT_DATA,extents<NSAMP> >& __restrict sig_o )
{
  // Set pointers to windows:
  TT_DATA* ibuff = sig_i.data();
  TT_DATA* obuff = sig_o.data();

  // Iterators to point to output buffer for twiddle multiplication:
  auto itr = aie::begin_restrict_vector<4>( sig_i );
  auto itw = aie::begin_restrict_vector<4>( sig_o );

  // Perform FFT:
  radix4_dit<64,TT_DATA,TT_TWID>(ibuff,  tw1,  tw2,   NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  radix4_dit<16,TT_DATA,TT_TWID>(tbuff,  tw4,  tw8,   NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);
  radix4_dit< 4,TT_DATA,TT_TWID>(ibuff,  tw16, tw32,  NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  radix4_dit< 1,TT_DATA,TT_TWID>(tbuff,  tw64, tw128, NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);

  // ==================== Twiddle Rotation ====================

  // Declare registers:
  aie::vector<TT_TWID,8> twid_use;
  aie::vector<TT_TWID,8> twid;
  aie::vector<TT_DATA,8> buff;

  // Compute initial phase for this transform:
  unsigned int phase = COUNT << ALIGN;

  // Compute vectorized phase ramp:
  unsigned int phase_step = 0;
  for (unsigned ii=0; ii < 8; ii++) {
    twid.push(aie::sincos_complex(phase_step));
    phase_step += phase;
  }
  phase = 0;

  // Loop over data, multiplying by twiddles eight at a time:
  for (int ii=0; ii < NFFT_1D/8; ii++)
    chess_prepare_for_pipelining
    chess_unroll_loop(8)
  {
    // Compute coarse phase and fine phase:
    twid_use = (aie::mul(aie::reverse(twid),aie::sincos_complex(phase))).to_vector<TT_TWID>(15);
    phase += phase_step;

    // Multiply data x twiddle:
    buff.insert(0,*itr++);
    buff.insert(1,*itr++);
    *itw++ = (aie::mul(buff.extract<4>(0),twid_use.extract<4>(0))).to_vector<TT_DATA>(15);
    *itw++ = (aie::mul(buff.extract<4>(1),twid_use.extract<4>(1))).to_vector<TT_DATA>(15);
  }
  COUNT = ( COUNT >= NSAMP-NSTREAM ) ? PHASE : COUNT+NSTREAM;

  // Write extra 4 samples:
  *itw++ = aie::zeros<TT_DATA,4>();

}
