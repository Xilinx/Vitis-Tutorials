//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft7_mmul0.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dft7_0::dft7_0( TT_TWID (&coeff_i)[COEFF_DEPTH] ) : coeff(coeff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dft7_0::run( input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig_i,
                  output_cascade<TT_ACC>* __restrict acc_o )
{
  // Define matrix multiplication:
  using MMUL = aie::mmul<1,4,8,TT_DATA,TT_TWID>;
  MMUL compute0;
  MMUL compute1;
  MMUL compute2;
  MMUL compute3;
  MMUL compute4;
  MMUL compute5;
  MMUL compute6;
  MMUL compute7;

  // Vector registers for DFT coefficients:
  aie::vector<TT_TWID,32> vc0;

  // Twiddles for first half of samples:
  vc0.insert<8>(0,aie::load_v<8>(&coeff[ 0]));
  vc0.insert<8>(1,aie::load_v<8>(&coeff[ 8]));
  vc0.insert<8>(2,aie::load_v<8>(&coeff[16]));
  vc0.insert<8>(3,aie::load_v<8>(&coeff[24]));

  // Lane buffers
  aie::vector<TT_DATA,32> data0;
  aie::vector<TT_DATA,32> data1;

  // Input buffer pointer:
  TT_DATA* __restrict ptr = sig_i.data();

  // Loop over complete set of DFT-7 required for PFA-1008 transform:
  // --> We will run 9*16 = 144 transforms in total for a complete PFA-1008 (one kernel invocation)
  //
  // The loop body will perform 8 transforms since 7 x 8 = 56 is the smallest multiple of 8

  // |a0|a1|a2|a3|a4|a5|a6|b0|
  // |b1|b2|b3|b4|b5|b6|c0|c1|
  // |c2|c3|c4|c5|c6|d0|d1|d2|
  // |d3|d4|d5|d6|e0|e1|e2|e3|
  // |e4|e5|e6|f0|f1|f2|f3|f4|
  // |f5|f6|g0|g0|g2|g3|g4|g5|
  // |g6|h0|h1|h2|h3|h4|h5|h6|
  //
  // We process 8 transforms (cascade writes) from 7 input vectors (56 samples in total)

  for (unsigned rr=0; rr < NUM_FFT/8; rr++)
    chess_prepare_for_pipelining
  {
    data0.insert<8>(0,aie::load_v<8>((ptr)));
    data0.insert<8>(1,aie::load_v<8>((ptr+8)));
    data0.insert<8>(2,aie::load_v<8>((ptr+16)));
    data0.insert<8>(3,aie::load_v<8>((ptr+24)));
    data1.insert<8>(0,aie::load_v<8>((ptr+32)));
    data1.insert<8>(1,aie::load_v<8>((ptr+40)));
    data1.insert<8>(2,aie::load_v<8>((ptr+48)));

    compute0.mul(                    data0.extract<4>(0), vc0 );
    compute1.mul( aie::shuffle_up(data0,1).extract<4>(2), vc0 );
    compute2.mul( aie::shuffle_up(data0,2).extract<4>(4), vc0 );
    compute3.mul( aie::shuffle_up(data0,3).extract<4>(6), vc0 );
    compute4.mul(                    data0.extract<4>(7), vc0 );
    compute5.mul( aie::shuffle_up(data1,1).extract<4>(1), vc0 );
    compute6.mul( aie::shuffle_up(data1,2).extract<4>(3), vc0 );
    compute7.mul( aie::shuffle_up(data1,3).extract<4>(5), vc0 );

    writeincr(acc_o,compute0.to_accum());
    writeincr(acc_o,compute1.to_accum());
    writeincr(acc_o,compute2.to_accum());
    writeincr(acc_o,compute3.to_accum());
    writeincr(acc_o,compute4.to_accum());
    writeincr(acc_o,compute5.to_accum());
    writeincr(acc_o,compute6.to_accum());
    writeincr(acc_o,compute7.to_accum());

    // Update pointer:
    ptr += 56;
  }
}


