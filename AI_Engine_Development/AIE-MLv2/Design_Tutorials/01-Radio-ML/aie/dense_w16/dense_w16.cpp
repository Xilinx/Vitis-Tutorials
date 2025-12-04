//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/utils.hpp>
#include "dense_w16.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
dense_w16<NSAMP,NNODES,NWEIGHTS,NBIASES>::dense_w16(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// SeLu
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
aie::vector<typename dense_w16<NSAMP,NNODES,NWEIGHTS,NBIASES>::TT_DATA,64> dense_w16<NSAMP,NNODES,NWEIGHTS,NBIASES>::SeLu( aie::vector<TT_DATA,64> data_i )
{
  aie::accum<accfloat,64> accB;
  accB.from_vector(aie::broadcast<float,64>(exp_B));

  // Compute 'pos':
  auto pos = aie::mul(data_i,Cpos).to_vector<TT_DATA>();

  // Compute 'neg' -- first part is exp(x) approximation
  auto data_t = (aie::mac(accB,data_i,exp_S)).to_vector<TT_DATA>();
  // Integer values should be in the range [0,16256], find outliers and set to zero:
  aie::vector<int16,64>     exp_i = aie::to_fixed<int16>(data_t,0);
  aie::mask<64>           msk_neg = aie::lt(exp_i,int16(0));
  aie::vector<int16,64>   exp_bnd = aie::select(exp_i, aie::zeros<int16,64>(), msk_neg);
  aie::mask<64>           msk_pos = aie::gt(exp_bnd, int16(16256));
  exp_bnd = aie::select(exp_bnd, aie::zeros<int16,64>(), msk_pos);
  auto neg_tmp = exp_bnd.cast_to<TT_DATA>();

  // Compute 'neg' -- second part is remaining math
  auto neg = (aie::mul(aie::sub(neg_tmp,TT_DATA(1.0f)),Cneg)).to_vector<TT_DATA>();

  // Select between 'pos' and 'neg':
  return aie::select(pos,neg,aie::le(data_i,aie::broadcast<TT_DATA,64>(0.0f)));
}
// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
void dense_w16<NSAMP,NNODES,NWEIGHTS,NBIASES>::run(  input_buffer<TT_DATA>&  __restrict data_i,
                      const TT_DATA              (&weights_0)[NWEIGHTS/2],
                      const TT_DATA              (&weights_1)[NWEIGHTS/2],
                      const TT_DATA              (&biases)[NBIASES],
                      output_buffer<TT_DATA>& __restrict data_o)
{
  auto p_data_i     = aie::begin_vector_circular<64>(data_i);
  auto p_data_o     = aie::begin_restrict_vector<64>(data_o);
  auto p_weights_0  = aie::begin_restrict_vector<64>(weights_0);
  auto p_weights_1  = aie::begin_restrict_vector<64>(weights_1);
  auto p_biases     = aie::begin_restrict_vector<64>(biases);

  aie::vector<TT_DATA,64>  data_reg0,data_reg1,data_reg2;
  aie::vector<TT_DATA,64>  weights_reg0,weights_reg1;
  aie::accum<accfloat,64>  acc_reg0,acc_reg1;
  aie::accum<accfloat,64>  bias_acc0,bias_acc1;

  acc_reg0  = aie::zeros<accfloat,64>();
  acc_reg1  = aie::zeros<accfloat,64>();
  for ( unsigned iter=0; iter < NSAMP/64; iter++)
  {
    data_reg0 = *p_data_i++;
    data_reg1 = *p_data_i;
    // Loop over samples
    // Had to break into two loops due to CR-1242134
    for ( unsigned n=0; n < 32; n++)
        chess_loop_range(32,)
        chess_prepare_for_pipelining
    {
        data_reg2 = aie::shuffle_down_fill(data_reg0,data_reg1,n);

        weights_reg0 = *p_weights_0++;
        acc_reg0     = mac_elem_64(data_reg2,weights_reg0,acc_reg0);
        weights_reg1 = *p_weights_1++;
        acc_reg1     = mac_elem_64(data_reg2,weights_reg1,acc_reg1);
    }
    for ( unsigned n=0; n < 32; n++)
        chess_loop_range(32,)
        chess_prepare_for_pipelining
    {
        data_reg2 = aie::shuffle_down_fill(data_reg0,data_reg1,n+32);

        weights_reg0 = *p_weights_0++;
        acc_reg0     = mac_elem_64(data_reg2,weights_reg0,acc_reg0);
        weights_reg1 = *p_weights_1++;
        acc_reg1     = mac_elem_64(data_reg2,weights_reg1,acc_reg1);
    }
  }
  
  bias_acc0.from_vector(*p_biases++);
  acc_reg0 = aie::add(acc_reg0,bias_acc0);
  *p_data_o++ = SeLu(acc_reg0.to_vector<TT_DATA>());
  
  bias_acc1.from_vector(*p_biases++);
  acc_reg1 = aie::add(acc_reg1,bias_acc1);
  *p_data_o++ = SeLu(acc_reg1.to_vector<TT_DATA>());
}