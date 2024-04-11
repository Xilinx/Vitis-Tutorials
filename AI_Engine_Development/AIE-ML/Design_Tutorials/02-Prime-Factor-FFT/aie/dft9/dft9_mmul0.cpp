//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft9_mmul0.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dft9_0::dft9_0( TT_TWID (&coeff_i)[COEFF_DEPTH] ) : coeff(coeff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dft9_0::run( input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig_i,
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
  aie::vector<TT_TWID,32> vc1;

  // Twiddles for first half of samples:
  vc0.insert<8>(0,aie::load_v<8>(&coeff[ 0]));
  vc0.insert<8>(1,aie::load_v<8>(&coeff[ 8]));
  vc0.insert<8>(2,aie::load_v<8>(&coeff[16]));
  vc0.insert<8>(3,aie::load_v<8>(&coeff[24]));

  vc1.insert<8>(0,aie::load_v<8>(&coeff[32]));
  vc1.insert<8>(1,aie::load_v<8>(&coeff[40]));
  vc1.insert<8>(2,aie::load_v<8>(&coeff[48]));
  vc1.insert<8>(3,aie::load_v<8>(&coeff[56]));

  // Lane buffers
  aie::vector<TT_DATA,32> data0;
  aie::vector<TT_DATA,32> data1;
  aie::vector<TT_DATA,32> data2;

  // Input buffer pointer:
  TT_DATA* __restrict ptr = sig_i.data();

  // Loop over complete set of DFT-9 required for PFA-1008 transform:
  // --> We will run 7*16 = 112 transforms in total for a complete PFA-1008 (one kernel invocation)
  //
  // The loop body will perform 8 transforms since 9 x 8 = 72 is the smallest multiple of 8

  for (unsigned rr=0; rr < NUM_FFT/8; rr++)
    chess_prepare_for_pipelining
  {
    data0.insert<8>(0,aie::load_v<8>((ptr   )));
    data0.insert<8>(1,aie::load_v<8>((ptr+ 8)));
    data0.insert<8>(2,aie::load_v<8>((ptr+16)));
    data0.insert<8>(3,aie::load_v<8>((ptr+24)));
    data1.insert<8>(0,aie::load_v<8>((ptr+32)));
    data1.insert<8>(1,aie::load_v<8>((ptr+40)));
    data1.insert<8>(2,aie::load_v<8>((ptr+48)));
    data1.insert<8>(3,aie::load_v<8>((ptr+56)));
    data2.insert<8>(0,aie::load_v<8>((ptr+56)));
    data2.insert<8>(1,aie::load_v<8>((ptr+64)));

    compute0.mul(                        data0.extract<4>(0), vc0 );
    compute1.mul(                        data0.extract<4>(0), vc1 );
    compute2.mul( (aie::shuffle_down(data0,1)).extract<4>(2), vc0 );
    compute3.mul( (aie::shuffle_down(data0,1)).extract<4>(2), vc1 );
    compute4.mul( (aie::shuffle_down(data0,2)).extract<4>(4), vc0 );
    compute5.mul( (aie::shuffle_down(data0,2)).extract<4>(4), vc1 );
    compute6.mul( (aie::shuffle_down(data0,3)).extract<4>(6), vc0 );
    compute7.mul( (aie::shuffle_down(data0,3)).extract<4>(6), vc1 );

    writeincr( acc_o, compute0.to_accum() );
    writeincr( acc_o, compute1.to_accum() );
    writeincr( acc_o, compute2.to_accum() );
    writeincr( acc_o, compute3.to_accum() );
    writeincr( acc_o, compute4.to_accum() );
    writeincr( acc_o, compute5.to_accum() );
    writeincr( acc_o, compute6.to_accum() );
    writeincr( acc_o, compute7.to_accum() );

    compute0.mul(                        data1.extract<4>(1), vc0 );
    compute1.mul(                        data1.extract<4>(1), vc1 );
    compute2.mul( (aie::shuffle_down(data1,1)).extract<4>(3), vc0 );
    compute3.mul( (aie::shuffle_down(data1,1)).extract<4>(3), vc1 );
    compute4.mul( (aie::shuffle_down(data1,2)).extract<4>(5), vc0 );
    compute5.mul( (aie::shuffle_down(data1,2)).extract<4>(5), vc1 );
    compute6.mul( (aie::shuffle_down(data2,3)).extract<4>(1), vc0 );
    compute7.mul( (aie::shuffle_down(data2,3)).extract<4>(1), vc1 );

    writeincr( acc_o, compute0.to_accum() );
    writeincr( acc_o, compute1.to_accum() );
    writeincr( acc_o, compute2.to_accum() );
    writeincr( acc_o, compute3.to_accum() );
    writeincr( acc_o, compute4.to_accum() );
    writeincr( acc_o, compute5.to_accum() );
    writeincr( acc_o, compute6.to_accum() );
    writeincr( acc_o, compute7.to_accum() );

    // Update pointer:
    ptr += 72;
  }
}


