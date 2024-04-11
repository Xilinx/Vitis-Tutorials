//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft16_mmul1.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dft16_1::dft16_1( TT_TWID (&coeff_i)[COEFF_DEPTH] ) : coeff(coeff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dft16_1::run(  input_cascade<TT_ACC>* __restrict acc_i,
                    input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig_i,
                    output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o )
{
  // Iterators for I/O buffers:
  auto itr0 = aie::begin_vector<4>(sig_i);
  auto itw0 = aie::begin_vector<8>(sig_o);
  auto itw1 = aie::begin_vector<8>(sig_o);
  itr0 += 2;                    // Processing 'odd' vectors in this tile
  itw1 += 1;                    // Use for 'odd' vectors

  // Define matrix multiplication:
  using MMUL = aie::mmul<1,4,8,TT_DATA,TT_TWID>;
  MMUL compute;

  // Vector registers for DFT coefficients:
  aie::vector<TT_TWID,32> vc0;
  aie::vector<TT_TWID,32> vc1;
  aie::vector<TT_TWID,32> vc2;
  aie::vector<TT_TWID,32> vc3;

  // Twiddles for first half of samples:
  vc0 = aie::load_v<32>(&coeff[64]);
  vc1 = aie::load_v<32>(&coeff[96]);
  vc2 = aie::load_v<32>(&coeff[192]);
  vc3 = aie::load_v<32>(&coeff[224]);

  // Loop over complete set of DFT-16 required for PFA-1008 transform:
  // --> We will run 7*9 = 63 transforms in total for a complete PFA-1008 (one kernel invocation)
  for (unsigned rr=0; rr < NUM_FFT; rr++)
    chess_loop_range(NUM_FFT/8,)
    chess_prepare_for_pipelining
  {
    compute = readincr_v<8>( acc_i );
    compute.mac( *(itr0+0), vc0 );
    compute.mac( *(itr0+1), vc1 );
    *itw0 = compute.template to_vector<TT_DATA>(DNSHIFT);
    itw0 += 2;                  // Hop to next even vector
    compute = readincr_v<8>( acc_i );
    compute.mac( *(itr0+0), vc2 );
    compute.mac( *(itr0+1), vc3 );
    *itw1 = compute.template to_vector<TT_DATA>(DNSHIFT);
    itw1 += 2;                  // Hop to next odd vector
    itr0 += 4;                  // Hop to next input vector
  }
}


