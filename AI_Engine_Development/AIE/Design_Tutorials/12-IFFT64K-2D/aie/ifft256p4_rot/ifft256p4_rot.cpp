//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ifft256p4_rot.h"

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
  aie::fft_dit_r4_stage<64>(ibuff, tw4a_1, tw4a_0, tw4a_2, NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  aie::fft_dit_r4_stage<16>(tbuff, tw4b_1, tw4b_0, tw4b_2, NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);
  aie::fft_dit_r4_stage< 4>(ibuff, tw4c_1, tw4c_0, tw4c_2, NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, tbuff);
  aie::fft_dit_r4_stage< 1>(tbuff, tw4d_1, tw4d_0, tw4d_2, NFFT_1D, SHIFT_TW, SHIFT_DT, INVERSE, ibuff);

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
