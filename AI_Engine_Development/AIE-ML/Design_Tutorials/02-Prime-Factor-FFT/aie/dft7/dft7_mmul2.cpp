//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft7_mmul2.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dft7_2::dft7_2( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dft7_2::run(  input_cascade<TT_ACC>* __restrict acc_i,
                   output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o )

{
  aie::vector<TT_DATA,8>  data0;
  aie::vector<TT_DATA,8>  data1;
  aie::vector<TT_DATA,8>  data2;
  aie::vector<TT_DATA,8>  data3;
  aie::vector<TT_DATA,8>  data4;
  aie::vector<TT_DATA,8>  data5;
  aie::vector<TT_DATA,8>  data6;
  aie::vector<TT_DATA,8>  data7;
  auto itw = aie::begin_vector<8>(sig_o);

  // |a0|a1|a2|a3|a4|a5|a6|b0|
  // |b1|b2|b3|b4|b5|b6|c0|c1|
  // |c2|c3|c4|c5|c6|d0|d1|d2|
  // |d3|d4|d5|d6|e0|e1|e2|e3|
  // |e4|e5|e6|f0|f1|f2|f3|f4|
  // |f5|f6|g0|g0|g2|g3|g4|g5|
  // |g6|h0|h1|h2|h3|h4|h5|h6|
  //
  // We process 8 transforms (cascade reads) to produce 7 output vectors (56 samples in total)

  for (unsigned rr=0; rr < NUM_FFT/8; rr++)
    chess_prepare_for_pipelining
  {
    data0 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data1 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data2 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data3 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data4 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data5 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data6 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data7 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);

    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data0,1),data1),1)).extract<8>(0);
    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data1,1),data2),2)).extract<8>(0);
    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data2,1),data3),3)).extract<8>(0);
    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data3,1),data4),4)).extract<8>(0);
    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data4,1),data5),5)).extract<8>(0);
    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data5,1),data6),6)).extract<8>(0);
    *itw++ = (aie::shuffle_down(aie::concat(aie::shuffle_up(data6,1),data7),7)).extract<8>(0);
  }
}


