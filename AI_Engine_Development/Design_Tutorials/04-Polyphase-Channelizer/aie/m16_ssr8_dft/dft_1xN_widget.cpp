//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dft_1xN_widget.h"

// ============================================================
// Input Tile
// ============================================================

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
dft_1xN_input<TT_DATA,TT_COEFF,TT_ACC,NSAMP>::dft_1xN_input( TT_COEFF (&coeff0_i)[8], TT_COEFF (&coeff1_i)[8] )
  : coeff0( coeff0_i ), coeff1( coeff1_i )
{
  aie::set_rounding(aie::rounding_mode::positive_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
void dft_1xN_input<TT_DATA,TT_COEFF,TT_ACC,NSAMP>::run_input( input_stream<TT_DATA>* __restrict sig0_i,
                                                              input_stream<TT_DATA>* __restrict sig1_i,
                                                              output_stream<TT_ACC>* __restrict acc_o )

{
  // Iterator to access DFT coefficients:
  auto it0 = aie::cbegin_vector_circular<8,8,aie_dm_resource::a>( coeff0 );
  auto it1 = aie::cbegin_vector_circular<8,8,aie_dm_resource::b>( coeff1 );

 // Declare buffer for input signals [1x2]:
  aie::vector<TT_DATA,8> buff_sig = aie::zeros<TT_DATA,8>();

  // Declare buffer for coefficients [1x4]:
  aie::vector<TT_COEFF,16> buff_coeff = aie::zeros<TT_COEFF,16>();

  // Declare accumulator:
  aie::accum<TT_ACC,4> accA;    // Output for 1st DFT
  aie::accum<TT_ACC,4> accB;    // Output for 2nd DFT

  // Loop over some number of samples:
  for ( unsigned rr=0; rr < NSAMP/4; rr++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
  {
    buff_sig.insert(0,aie::concat(readincr_v<4,aie_stream_resource_in::a>(sig0_i),
                                  readincr_v<4,aie_stream_resource_in::b>(sig1_i)));
    buff_coeff.insert(0,*it0++);
    buff_coeff.insert(1,*it1++);
    // Compute result [1x2] x [2x4]:
    // --> aie::sliding_mul_ops<Lanes,Points,CoeffStep,DataStepX,DataStepY,CoeffType,DataType,AccumTag>
    //         ::mul(coeff,coeff_start,data,data_start)
    // --> Note: use coeff for input samples, use data for DFT coefficients (assume stored in row-major order)
    accA = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mul(     buff_sig,0,buff_coeff,0);
    accA = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accA,buff_sig,4,buff_coeff,8);
    writeincr(acc_o,accA);
    accB = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mul(     buff_sig,2,buff_coeff,0);
    accB = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accB,buff_sig,6,buff_coeff,8);
    writeincr(acc_o,accB);
  } // rr
}

// ============================================================
// Middle Tile
// ============================================================

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
dft_1xN_middle<TT_DATA,TT_COEFF,TT_ACC,NSAMP>::dft_1xN_middle( TT_COEFF (&coeff0_i)[8], TT_COEFF (&coeff1_i)[8] )
  : coeff0( coeff0_i ), coeff1( coeff1_i )
{
  aie::set_rounding(aie::rounding_mode::positive_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
void dft_1xN_middle<TT_DATA,TT_COEFF,TT_ACC,NSAMP>::run_middle( input_stream<TT_DATA>* __restrict sig0_i,
                                                                input_stream<TT_DATA>* __restrict sig1_i,
                                                                input_stream<TT_ACC>* __restrict acc_i,
                                                                output_stream<TT_ACC>* __restrict acc_o  )
{
  // Iterator to access DFT coefficients:
  auto it0 = aie::cbegin_vector_circular<8,8,aie_dm_resource::a>( coeff0 );
  auto it1 = aie::cbegin_vector_circular<8,8,aie_dm_resource::b>( coeff1 );

 // Declare buffer for input signals [1x2]:
  aie::vector<TT_DATA,8> buff_sig = aie::zeros<TT_DATA,8>();

  // Declare buffer for coefficients [1x4]:
  aie::vector<TT_COEFF,16> buff_coeff = aie::zeros<TT_COEFF,16>();

  // Declare accumulator:
  aie::accum<TT_ACC,4> accA;    // Output for 1st DFT
  aie::accum<TT_ACC,4> accB;    // Output for 2nd DFT

  // Loop over some number of samples:
  for ( unsigned rr=0; rr < NSAMP/4; rr++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
  {
    buff_sig.insert(0,aie::concat(readincr_v<4,aie_stream_resource_in::a>(sig0_i),
                                  readincr_v<4,aie_stream_resource_in::b>(sig1_i)));
    buff_coeff.insert(0,*it0++);
    buff_coeff.insert(1,*it1++);
    // Compute result [1x2] x [2x4]:
    // --> aie::sliding_mul_ops<Lanes,Points,CoeffStep,DataStepX,DataStepY,CoeffType,DataType,AccumTag>
    //         ::mul(coeff,coeff_start,data,data_start)
    // --> Note: use coeff for input samples, use data for DFT coefficients (assume stored in row-major order)
    accA = readincr_v4(acc_i);
    accA = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accA,buff_sig,0,buff_coeff,0);
    accA = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accA,buff_sig,4,buff_coeff,8);
    writeincr(acc_o,accA);
    accB = readincr_v4(acc_i);
    accB = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accB,buff_sig,2,buff_coeff,0);
    accB = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accB,buff_sig,6,buff_coeff,8);
    writeincr(acc_o,accB);
  } // rr
}

// ============================================================
// Output Tile
// ============================================================

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
dft_1xN_output<TT_DATA,TT_COEFF,TT_ACC,NSAMP>::dft_1xN_output( TT_COEFF (&coeff0_i)[8], TT_COEFF (&coeff1_i)[8] )
  : coeff0( coeff0_i ), coeff1( coeff1_i )
{
  aie::set_rounding(aie::rounding_mode::positive_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}


// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
void dft_1xN_output<TT_DATA,TT_COEFF,TT_ACC,NSAMP>::run_output( input_stream<TT_DATA>* __restrict sig0_i,
                                                                input_stream<TT_DATA>* __restrict sig1_i,
                                                                input_stream<TT_ACC>* __restrict acc_i,
                                                                output_stream<TT_DATA>* __restrict sig0_o,
                                                                output_stream<TT_DATA>* __restrict sig1_o )
{
  // Iterator to access DFT coefficients:
  auto it0 = aie::cbegin_vector_circular<8,8,aie_dm_resource::a>( coeff0 );
  auto it1 = aie::cbegin_vector_circular<8,8,aie_dm_resource::b>( coeff1 );

 // Declare buffer for input signals [1x2]:
  aie::vector<TT_DATA,8> buff_sig = aie::zeros<TT_DATA,8>();

  // Declare buffer for coefficients [1x4]:
  aie::vector<TT_COEFF,16> buff_coeff = aie::zeros<TT_COEFF,16>();

  // Declare accumulator:
  aie::accum<TT_ACC,4> accA;    // Output for 1st DFT
  aie::accum<TT_ACC,4> accB;    // Output for 2nd DFT

  // Loop over some number of samples:
  for ( unsigned rr=0; rr < NSAMP/4; rr++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
  {
    buff_sig.insert(0,aie::concat(readincr_v<4,aie_stream_resource_in::a>(sig0_i),
                                  readincr_v<4,aie_stream_resource_in::b>(sig1_i)));
    buff_coeff.insert(0,*it0++);
    buff_coeff.insert(1,*it1++);
    // Compute result [1x2] x [2x4]:
    // --> aie::sliding_mul_ops<Lanes,Points,CoeffStep,DataStepX,DataStepY,CoeffType,DataType,AccumTag>
    //         ::mul(coeff,coeff_start,data,data_start)
    // --> Note: use coeff for input samples, use data for DFT coefficients (assume stored in row-major order)
    accA = readincr_v4(acc_i);
    accA = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accA,buff_sig,0,buff_coeff,0);
    accA = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accA,buff_sig,4,buff_coeff,8);
    writeincr<aie_stream_resource_out::a>(sig0_o,accA.template to_vector<TT_DATA>( BIT_SHIFT ));

    accB = readincr_v4(acc_i);
    accB = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accB,buff_sig,2,buff_coeff,0);
    accB = aie::sliding_mul_ops<4,2,1,4,1,TT_DATA,TT_COEFF,TT_ACC>::mac(accB,buff_sig,6,buff_coeff,8);
    writeincr<aie_stream_resource_out::b>(sig1_o,accB.template to_vector<TT_DATA>( BIT_SHIFT ));
  } // rr
}

