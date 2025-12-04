//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/utils.hpp>
#include "dense_w18.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
dense_w18<NSAMP,NNODES,NWEIGHTS,NBIASES>::dense_w18(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// softmax
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
void dense_w18<NSAMP,NNODES,NWEIGHTS,NBIASES>::softmax( aie::vector<TT_DATA,32> vecA, output_buffer<TT_DATA>& data_o )
{
  auto itw = aie::begin_vector<8>(data_o);
  aie::vector<TT_DATA,32> vecB;
  aie::vector<TT_DATA,32> vecC;
  aie::vector<TT_DATA,32> vecD;
  aie::vector<TT_DATA,32> vecE;

  // Find maximum value:
  TT_DATA max_val = aie::reduce_max(vecA);

  // Subtract max value from all values:
  aie::accum<accfloat,32> accM;
  aie::accum<accfloat,32> accA;
  accM.from_vector(aie::broadcast<float,32>(max_val));
  accA.from_vector(vecA);
  vecB = (aie::sub(accA,accM)).to_vector<TT_DATA>();

  // Start computation of exponentials of all input values:
  aie::accum<accfloat,32> accB;
  accB.from_vector(aie::broadcast<float,32>(exp_B));
  accA = aie::mac(accB,vecB,exp_S);
  vecC = accA.to_vector<TT_DATA>();

  // Integer values should be in the range [0,16256], find outliers and set to zero:
  aie::vector<int16,32>   exp_i = aie::to_fixed<int16>(vecC,0);
  aie::mask<32>         msk_neg = aie::lt(exp_i,int16(0));
  aie::vector<int16,32> exp_bnd = aie::select(exp_i, aie::zeros<int16,32>(), msk_neg);
  aie::mask<32>         msk_pos = aie::gt(exp_bnd, int16(16256));
  exp_bnd = aie::select(exp_bnd, aie::zeros<int16,32>(), msk_pos);

  // Accumulate all vectors to determine scale factor:
  vecD = exp_bnd.cast_to<TT_DATA>();
  TT_DATA scale_factor = aie::inv(aie::reduce_add(vecD));
  vecE = aie::mul(vecD,scale_factor);
  // Write result:
  *itw++ = vecE.extract<8>(0);
  *itw++ = vecE.extract<8>(1);
  *itw++ = vecE.extract<8>(2);
}
// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
void dense_w18<NSAMP,NNODES,NWEIGHTS,NBIASES>::run(  input_buffer<TT_DATA>&  __restrict data_i,
                                                     const TT_DATA              (&weights)[NWEIGHTS],
                                                     const TT_DATA              (&biases)[NNODES],
                                                     output_buffer<TT_DATA>& __restrict data_o)
{
  auto p_data_i       = aie::begin_vector_circular<64>(data_i);
  auto p_weights      = aie::begin_restrict_vector<64>(weights);
  auto p_biases       = aie::begin_restrict_vector<8>(biases);
  auto p_data_o       = aie::begin_restrict_vector<8>(data_o);

  aie::vector<TT_DATA,32>  v_reg0 = aie::zeros<TT_DATA,32>();
  aie::vector<TT_DATA,32>  bias_reg = aie::zeros<TT_DATA,32>();;
  aie::accum<accfloat,64>  acc_reg0,acc_reg1,acc_reg2,acc_reg3;

  for ( unsigned iter=0; iter < NNODES; iter+=4)
    chess_loop_range(NNODES,)
    chess_prepare_for_pipelining
  {
    acc_reg0         = mul_elem_64(*p_data_i++,*p_weights++);
    acc_reg0         = mac_elem_64(*p_data_i++,*p_weights++,acc_reg0);
    v_reg0.set(aie::reduce_add(acc_reg0.to_vector<TT_DATA>()),iter);

    acc_reg1         = mul_elem_64(*p_data_i++,*p_weights++);
    acc_reg1         = mac_elem_64(*p_data_i++,*p_weights++,acc_reg1);
    v_reg0.set(aie::reduce_add(acc_reg1.to_vector<TT_DATA>()),iter+1);

    acc_reg2         = mul_elem_64(*p_data_i++,*p_weights++);
    acc_reg2         = mac_elem_64(*p_data_i++,*p_weights++,acc_reg2);
    v_reg0.set(aie::reduce_add(acc_reg2.to_vector<TT_DATA>()),iter+2);

    acc_reg3         = mul_elem_64(*p_data_i++,*p_weights++);
    acc_reg3         = mac_elem_64(*p_data_i++,*p_weights++,acc_reg3);
    v_reg0.set(aie::reduce_add(acc_reg3.to_vector<TT_DATA>()),iter+3);
  }
  bias_reg.insert<8>(0,*p_biases++);
  bias_reg.insert<8>(1,*p_biases++);
  bias_reg.insert<8>(2,*p_biases++);
  softmax(aie::add(v_reg0,bias_reg),data_o);
}
