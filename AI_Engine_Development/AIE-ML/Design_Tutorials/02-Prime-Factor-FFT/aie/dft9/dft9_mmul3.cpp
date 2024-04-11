//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft9_mmul3.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dft9_3::dft9_3( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dft9_3::run(  input_cascade<TT_ACC>* __restrict acc_i,
                   output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o )

{
  aie::vector<TT_DATA,8> data0;
  aie::vector<TT_DATA,8> data1;
  aie::vector<TT_DATA,8> data2;
  aie::vector<TT_DATA,8> data3;
  aie::vector<TT_DATA,8> data4;
  aie::vector<TT_DATA,8> data5;
  aie::vector<TT_DATA,8> data6;
  aie::vector<TT_DATA,8> data7;
  aie::vector<TT_DATA,8> data8;
  aie::vector<TT_DATA,8> data9;
  aie::vector<TT_DATA,8> dataA;
  aie::vector<TT_DATA,8> dataB;
  aie::vector<TT_DATA,8> dataC;
  aie::vector<TT_DATA,8> dataD;
  aie::vector<TT_DATA,8> dataE;
  aie::vector<TT_DATA,8> dataF;
  auto itw = aie::begin_vector<8>(sig_o);

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

    data8 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    data9 = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    dataA = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    dataB = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    dataC = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    dataD = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    dataE = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);
    dataF = readincr_v<8>(acc_i).to_vector<TT_DATA>(DNSHIFT);

    *itw++ = data0;                                                               //  0 to  7
    *itw++ = aie::shuffle_up_fill(data2,aie::shuffle_down_fill(data0,data1,1),1); //  8 to 15
    *itw++ = aie::shuffle_up_fill(data4,aie::shuffle_down_fill(data2,data3,1),2); // 16 to 23
    *itw++ = aie::shuffle_up_fill(data6,aie::shuffle_down_fill(data4,data5,1),3); // 24 to 31
    *itw++ = aie::shuffle_up_fill(data8,aie::shuffle_down_fill(data6,data7,1),4); // 32 to 39
    *itw++ = aie::shuffle_up_fill(dataA,aie::shuffle_down_fill(data8,data9,1),5); // 40 to 47
    *itw++ = aie::shuffle_up_fill(dataC,aie::shuffle_down_fill(dataA,dataB,1),6); // 48 to 56
    *itw++ = aie::shuffle_up_fill(dataE,aie::shuffle_down_fill(dataC,dataD,1),7); // 56 to 63
    *itw++ = aie::shuffle_down_fill(dataE,dataF,1);                               // 64 to 71
  }
}


