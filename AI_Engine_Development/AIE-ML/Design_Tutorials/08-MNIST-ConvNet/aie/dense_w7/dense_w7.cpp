//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dense_w7.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dense_w7::dense_w7( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  fsm_state = 0;
  aie::store_v(scratch,aie::broadcast<bfloat16,16>(-2^127));
}

// ------------------------------------------------------------
// Grab Weights
// ------------------------------------------------------------

void dense_w7::grab_wts( input_async_buffer<bfloat16>& wts_i )
{
  if (fsm_state == 0) {
    wts_i.acquire();
    fsm_state = 1;
  }
}

// ------------------------------------------------------------
// Flatten
// ------------------------------------------------------------

inline __attribute__((noinline))
void dense_w7::flatten( input_buffer<bfloat16>& ifm_i )
{
  // Input arriving (3,8,128) --> pack into (1152,)
  auto itr = aie::begin_restrict_vector<16>(ifm_i);
  auto itw = aie::begin_restrict_vector<16>(data);
  for (unsigned rr=0; rr < 3; rr++)
    chess_prepare_for_pipelining
  {
    for (unsigned cc=0; cc < 3*128; cc+=16)
      chess_prepare_for_pipelining
    {
      *itw++ = *itr++;
    } // cc
    itr += 40; // skip the zero padding
  } // rr
}

// ------------------------------------------------------------
// Fully Connected Layer
// ------------------------------------------------------------

inline __attribute__((noinline))
void dense_w7::fully_connected( input_async_buffer<bfloat16>& wts_i )
{
  // Input format is (1152,):
  auto itw = aie::begin(scratch);
  auto itap = aie::begin_restrict_vector<16>(wts_i);
  std::array<aie::vector<bfloat16,32>,4>  comp;
  std::array<aie::vector<bfloat16,32>,4>  taps;
  std::array<aie::accum<accfloat,16>,4>   acc;
  // Loop over 10 outputs:
  for (unsigned oc=0; oc < 10; oc++)
    chess_prepare_for_pipelining
  {
    // Reset pointer to start of inputs:
    auto idat = aie::begin_restrict_vector<16>(data);

    acc[0] = aie::zeros<accfloat,16>();
    acc[1] = aie::zeros<accfloat,16>();
    acc[2] = aie::zeros<accfloat,16>();
    acc[3] = aie::zeros<accfloat,16>();

    bfloat16 bias = *(wts_i.data()+11520+oc);

    // Loop over 1152 inputs 32x4=128 at a time:
    for (unsigned ic=0; ic < 1152; ic+=128)
      chess_prepare_for_pipelining
    {
      comp[0].insert(0,*idat++);      taps[0].insert(0,*itap++);
      comp[0].insert(1,*idat++);      taps[0].insert(1,*itap++);
      comp[1].insert(0,*idat++);      taps[1].insert(0,*itap++);
      comp[1].insert(1,*idat++);      taps[1].insert(1,*itap++);
      comp[2].insert(0,*idat++);      taps[2].insert(0,*itap++);
      comp[2].insert(1,*idat++);      taps[2].insert(1,*itap++);
      comp[3].insert(0,*idat++);      taps[3].insert(0,*itap++);
      comp[3].insert(1,*idat++);      taps[3].insert(1,*itap++);
      acc[0] = mac_elem_16_2(comp[0],taps[0],acc[0]);
      acc[1] = mac_elem_16_2(comp[1],taps[1],acc[1]);
      acc[2] = mac_elem_16_2(comp[2],taps[2],acc[2]);
      acc[3] = mac_elem_16_2(comp[3],taps[3],acc[3]);
    } // ic
    acc[0] = aie::add(acc[0],acc[1]);
    acc[2] = aie::add(acc[2],acc[3]);
    acc[0] = aie::add(acc[0],acc[2]);
    // The 'chess_separator_scheduler()' below avoid a back-end compiler scheduling issue that introduces
    // functional errors in the output. Filed CRVO-12023. This temporary workaround may have a
    // throughput impact.
    *itw++ = chess_copy(bias + aie::reduce_add(acc[0].to_vector<bfloat16>()));
    chess_separator_scheduler();
  } // oc
}

// ------------------------------------------------------------
// Softmax
// ------------------------------------------------------------

inline __attribute__((noinline))
void dense_w7::softmax( output_buffer<bfloat16>& ofm_o )
{
  auto itw = aie::begin_vector<16>(ofm_o);
  aie::vector<bfloat16,16> vecA;
  aie::vector<bfloat16,16> vecB;
  aie::vector<bfloat16,16> vecC;
  aie::vector<bfloat16,16> vecD;
  aie::vector<bfloat16,16> vecE;
  // Load initial values:
  vecA = aie::load_v<16>(scratch);

  // Find maximum value:
  bfloat16 max_val = aie::reduce_max(vecA);

  // Subtract max value from all values:
  aie::accum<accfloat,16> accM;
  aie::accum<accfloat,16> accA;
  accM.from_vector(aie::broadcast<float,16>(max_val));
  accA.from_vector(vecA);
  vecB = (aie::sub(accA,accM)).to_vector<bfloat16>();

  // Start computation of exponentials of all input values:
  aie::accum<accfloat,16> accB;
  accB.from_vector(aie::broadcast<float,16>(exp_B));
  accA = aie::mac(accB,vecB,exp_S);
  vecC = accA.to_vector<bfloat16>();

  // Integer values should be in the range [0,16256], find outliers and set to zero:
  aie::vector<int16,16>   exp_i = aie::to_fixed<int16>(vecC,0);
  aie::mask<16>         msk_neg = aie::lt(exp_i,int16(0));
  aie::vector<int16,16> exp_bnd = aie::select(exp_i, aie::zeros<int16,16>(), msk_neg);
  aie::mask<16>         msk_pos = aie::gt(exp_bnd, int16(16256));
  exp_bnd = aie::select(exp_bnd, aie::zeros<int16,16>(), msk_pos);

  // Accumulate all vectors to determine scale factor:
  vecD = exp_bnd.cast_to<bfloat16>();
  bfloat16 scale_factor = aie::inv(aie::reduce_add(vecD));
  vecE = aie::mul(vecD,scale_factor);
  // Write result:
  *itw = vecE;
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dense_w7::run( input_buffer      <bfloat16>& ifm_i,
                    input_async_buffer<bfloat16>& wts_i,
                    output_buffer     <bfloat16>& ofm_o )
{
  grab_wts( wts_i );
  flatten( ifm_i );
  fully_connected( wts_i );
  softmax( ofm_o );
}


