//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "selu.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP>
selu<NSAMP>::selu(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP>
void selu<NSAMP>::run( input_buffer<bfloat16>& sig_i, output_buffer<bfloat16>& sig_o )
{
  auto itr = aie::begin_restrict_vector<32>(sig_i);
  auto itw = aie::begin_restrict_vector<32>(sig_o);
  auto itp = aie::begin_restrict_vector<32>(SCRATCH_POS);
  auto itn = aie::begin_restrict_vector<32>(SCRATCH_NEG);

  aie::accum<accfloat,32> accB;
  accB.from_vector(aie::broadcast<float,32>(exp_B));

  // Compute 'pos':
  for (unsigned ii=0; ii < NSAMP/32; ii++)
    chess_prepare_for_pipelining
  {
    *itp++ = aie::mul(*itr++,Cpos);
  }
  chess_separator();

  // Compute 'neg' -- first part is exp(x) approximation
  itr = aie::begin_restrict_vector<32>(sig_i);
  for (unsigned ii=0; ii < NSAMP/32; ii++)
    chess_prepare_for_pipelining
  {
    auto data_i = *itr++;
    auto data_t = (aie::mac(accB,data_i,exp_S)).to_vector<bfloat16>();
    // Integer values should be in the range [0,16256], find outliers and set to zero:
    aie::vector<int16,32>     exp_i = aie::to_fixed<int16>(data_t,0);
    aie::mask<32>           msk_neg = aie::lt(exp_i,int16(0));
    aie::vector<int16,32>   exp_bnd = aie::select(exp_i, aie::zeros<int16,32>(), msk_neg);
    aie::mask<32>           msk_pos = aie::gt(exp_bnd, int16(16256));
    exp_bnd = aie::select(exp_bnd, aie::zeros<int16,32>(), msk_pos);
    *itn++ = exp_bnd.cast_to<bfloat16>();
  }
  chess_separator();

  // Compute 'neg' -- second part is remaining math
  itr = aie::begin_restrict_vector<32>(SCRATCH_NEG);
  itn = aie::begin_restrict_vector<32>(SCRATCH_NEG);
  for (unsigned ii=0; ii < NSAMP/32; ii++)
    chess_prepare_for_pipelining
  {
    auto data_i = *itr++;
    *itn++ = (aie::mul(aie::sub(data_i,bfloat16(1.0f)),Cneg)).to_vector<bfloat16>();
  }
  chess_separator();

  // Select between 'pos' and 'neg':
  itr = aie::begin_restrict_vector<32>(sig_i);
  itp = aie::begin_restrict_vector<32>(SCRATCH_POS);
  itn = aie::begin_restrict_vector<32>(SCRATCH_NEG);
  for (unsigned ii=0; ii < NSAMP/32; ii++)
    chess_prepare_for_pipelining
  {
    auto data_i = *itr++;
    auto data_p = *itp++;
    auto data_n = *itn++;
    *itw++ = aie::select(data_p,data_n,aie::le(data_i,aie::broadcast<bfloat16,32>(0.0f)));
  }
}
