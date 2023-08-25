//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft9_compute.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<class TT_DATA, class TT_TWIDDLE, class TT_ACC, unsigned NFFT>
dft9_compute<TT_DATA,TT_TWIDDLE,TT_ACC,NFFT>::dft9_compute( TT_TWIDDLE (&coeff_i)[COEFF_DEPTH] )
: coeff(coeff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<class TT_DATA, class TT_TWIDDLE, class TT_ACC, unsigned NFFT>
void dft9_compute<TT_DATA,TT_TWIDDLE,TT_ACC,NFFT>
::run(  input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig0_i,
        input_buffer<TT_DATA,extents<NSAMP_I> >& __restrict sig1_i,
        output_buffer<TT_DATA,extents<NSAMP_O> >& __restrict sig_o )
{
  // Iterators for I/O buffers:
  auto itr0a = aie::begin<aie_dm_resource::a>(sig0_i);
  auto itr0b = aie::begin<aie_dm_resource::b>(sig0_i);
  auto itr1a = aie::begin<aie_dm_resource::a>(sig1_i);
  auto itr1b = aie::begin<aie_dm_resource::b>(sig1_i);
  auto itw  = aie::begin_vector<8>(sig_o);

  // Declare buffer for input signals [1x2] (use 16 lanes to hold 14 samples from two transforms):
  aie::vector<TT_DATA,16> buff_sigA = aie::zeros<TT_DATA,16>();
  aie::vector<TT_DATA,16> buff_sigB = aie::zeros<TT_DATA,16>();

  // Declare buffer for coefficients [1x4];
  aie::vector<TT_TWIDDLE,8> buff_coeff = aie::zeros<TT_TWIDDLE,8>();

  // Declare accumulators:
  aie::accum<TT_ACC,8> accAA;
  aie::accum<TT_ACC,8> accBB;

  // Loop over complete set of DFT-9 required for PFA-1008 transform:
  // --> We will run 7*16 = 112 transforms in total for a complete PFA-1008
  for (unsigned rr=0; rr < NFFT/8; rr++)
    chess_loop_range(NFFT/8,)
    chess_prepare_for_pipelining
  {
    // ------------------------------------------------------------
    // Process 4 transforms on SS0
    // ------------------------------------------------------------

    buff_sigA.insert(0,aie::load_v<8>(itr0a   ));
    buff_sigA.insert(1,aie::load_v<8>(itr0b+ 8));
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |a0|a1|a2|a3 |a4|a5|a6|a7 |a8|c0|c1|c2 |c3|c4|c5|c6
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[0] );
    accAA = mul8(      as_v16cint16(buff_sigA), 0,0x99990000,as_v8cint16(buff_coeff),0,0x32103210); // a0/c0
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xAAAA1111,as_v8cint16(buff_coeff),4,0x32103210); // a1/c1
    buff_coeff = aie::load_v<8,aie_dm_resource::b>( &coeff[8] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xBBBB2222,as_v8cint16(buff_coeff),0,0x32103210); // a2/c2
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xCCCC3333,as_v8cint16(buff_coeff),4,0x32103210); // a3/c3
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[16] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xDDDD4444,as_v8cint16(buff_coeff),0,0x32103210); // a4/c4
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xEEEE5555,as_v8cint16(buff_coeff),4,0x32103210); // a5/c5
    buff_sigA.insert(0,aie::load_v<4>(itr0a+16));
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |c7|c8|e0|e1 |a4|a5|a6|a7 |a8|c0|c1|c2 |c3|c4|c5|c6
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[24] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xFFFF6666,as_v8cint16(buff_coeff),0,0x32103210); // a6/c6
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0x00007777,as_v8cint16(buff_coeff),4,0x32103210); // a7/c7
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[28] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0x11118888,as_v8cint16(buff_coeff),4,0x32103210); // a8/c8
    *itw++ = accAA.template to_vector<TT_DATA>( DNSHIFT );
    // ------------------------------------------------------------
    buff_sigB.insert(0,aie::load_v<8>(itr0a+16));
    buff_sigB.insert(1,aie::load_v<8>(itr0b+24));
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |c7|c8|e0|e1 |e2|e3|e4|e5 |e6|e7|e8|g0 |g1|g2|g3|g4
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[0] );
    accBB = mul8(      as_v16cint16(buff_sigB),0,0xBBBB2222,as_v8cint16(buff_coeff),0,0x32103210); // e0/g0
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xCCCC3333,as_v8cint16(buff_coeff),4,0x32103210); // e1/g1
    buff_coeff = aie::load_v<8,aie_dm_resource::b>( &coeff[8] );
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xDDDD4444,as_v8cint16(buff_coeff),0,0x32103210); // e2/g2
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xEEEE5555,as_v8cint16(buff_coeff),4,0x32103210); // e3/g3
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[16] );
    buff_sigB.insert(0,aie::load_v<4>(itr0a+32));    itr0a += 36;  itr0b += 36;
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |g5|g6|g7|g8 |e2|e3|e4|e5 |e6|e7|e8|g0 |g1|g2|g3|g4
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xFFFF6666,as_v8cint16(buff_coeff),0,0x32103210); // e4/g4
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x00007777,as_v8cint16(buff_coeff),4,0x32103210); // e5/g5
    buff_coeff = aie::load_v<8,aie_dm_resource::b>( &coeff[24] );
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x11118888,as_v8cint16(buff_coeff),0,0x32103210); // e6/g6
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x22229999,as_v8cint16(buff_coeff),4,0x32103210); // e7/g7
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[28] );
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x3333AAAA,as_v8cint16(buff_coeff),4,0x32103210); // e8/g8
    *itw++ = accBB.template to_vector<TT_DATA>( DNSHIFT );

    // ------------------------------------------------------------
    // Process 4 transforms on SS1
    // ------------------------------------------------------------
    buff_sigA.insert(0,aie::load_v<8>(itr1a   ));
    buff_sigA.insert(1,aie::load_v<8>(itr1b+ 8));
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |b0|b1|b2|b3 |b4|b5|b6|b7 |b8|d0|d1|d2 |d3|d4|d5|d6
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[0] );
    accAA = mul8(      as_v16cint16(buff_sigA), 0,0x99990000,as_v8cint16(buff_coeff),0,0x32103210); // b0/d0
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xAAAA1111,as_v8cint16(buff_coeff),4,0x32103210); // b1/d1
    buff_coeff = aie::load_v<8,aie_dm_resource::b>( &coeff[8] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xBBBB2222,as_v8cint16(buff_coeff),0,0x32103210); // b2/d2
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xCCCC3333,as_v8cint16(buff_coeff),4,0x32103210); // b3/d3
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[16] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xDDDD4444,as_v8cint16(buff_coeff),0,0x32103210); // b4/d4
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xEEEE5555,as_v8cint16(buff_coeff),4,0x32103210); // b5/d5
    buff_sigA.insert(0,aie::load_v<4>(itr1a+16));
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |d7|d8|f0|f1 |b4|b5|b6|b7 |b8|d0|d1|d2 |d3|d4|d5|d6
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[24] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0xFFFF6666,as_v8cint16(buff_coeff),0,0x32103210); // a6/d6
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0x00007777,as_v8cint16(buff_coeff),4,0x32103210); // a7/d7
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[28] );
    accAA = mac8(accAA,as_v16cint16(buff_sigA), 0,0x11118888,as_v8cint16(buff_coeff),4,0x32103210); // a8/d8
    *itw++ = accAA.template to_vector<TT_DATA>( DNSHIFT );
    // ------------------------------------------------------------
    buff_sigB.insert(0,aie::load_v<8>(itr1a+16));
    buff_sigB.insert(1,aie::load_v<8>(itr1b+24));
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |d7|d8|f0|f1 |f2|f3|f4|f5 |f6|f7|f8|h0 |h1|h2|h3|h4
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[0] );
    accBB = mul8(      as_v16cint16(buff_sigB),0,0xBBBB2222,as_v8cint16(buff_coeff),0,0x32103210); // f0/h0
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xCCCC3333,as_v8cint16(buff_coeff),4,0x32103210); // f1/h1
    buff_coeff = aie::load_v<8,aie_dm_resource::b>( &coeff[8] );
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xDDDD4444,as_v8cint16(buff_coeff),0,0x32103210); // f2/h2
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xEEEE5555,as_v8cint16(buff_coeff),4,0x32103210); // f3/h3
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[16] );
    buff_sigB.insert(0,aie::load_v<4>(itr1a+32));    itr1a += 36;  itr1b += 36;
    // |0 |1 |2 |3  |4 |5 |6 |7  |8 |9 |10|11 |12|13|14|15
    // |h5|h6|h7|h8 |f2|f3|f4|f5 |f6|f7|f8|h0 |h1|h2|h3|h4
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0xFFFF6666,as_v8cint16(buff_coeff),0,0x32103210); // f4/h4
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x00007777,as_v8cint16(buff_coeff),4,0x32103210); // f5/h5
    buff_coeff = aie::load_v<8,aie_dm_resource::b>( &coeff[24] );
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x11118888,as_v8cint16(buff_coeff),0,0x32103210); // f6/h6
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x22229999,as_v8cint16(buff_coeff),4,0x32103210); // f7/h7
    buff_coeff = aie::load_v<8,aie_dm_resource::a>( &coeff[28] );
    accBB = mac8(accBB,as_v16cint16(buff_sigB),0,0x3333AAAA,as_v8cint16(buff_coeff),4,0x32103210); // e8/g8
    *itw++ = accBB.template to_vector<TT_DATA>( DNSHIFT );

  }
}


