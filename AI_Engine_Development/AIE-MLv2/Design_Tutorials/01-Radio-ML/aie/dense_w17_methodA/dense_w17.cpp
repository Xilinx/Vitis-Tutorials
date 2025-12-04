

//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/utils.hpp>
#include "dense_w17.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
dense_w17<NNODES,NWEIGHTS,NBIASES>::dense_w17(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}
// ------------------------------------------------------------
// SeLu
// ------------------------------------------------------------

template<unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
aie::vector<typename dense_w17<NNODES,NWEIGHTS,NBIASES>::TT_DATA,64> dense_w17<NNODES,NWEIGHTS,NBIASES>::SeLu( aie::vector<TT_DATA,64> data )
{
  aie::accum<accfloat,64> accB;
  accB.from_vector(aie::broadcast<float,64>(exp_B));

  // Compute 'pos':
  auto pos = aie::mul(data,Cpos).to_vector<TT_DATA>();

  // Compute 'neg' -- first part is exp(x) approximation
  auto data_t = (aie::mac(accB,data,exp_S)).to_vector<TT_DATA>();
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
  return aie::select(pos,neg,aie::le(data,aie::broadcast<TT_DATA,64>(0.0f)));
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NNODES, unsigned NWEIGHTS, unsigned NBIASES>
void dense_w17<NNODES,NWEIGHTS,NBIASES>::run( input_buffer<TT_DATA>&     __restrict data_i,
                                              const TT_DATA              (&weights)[NWEIGHTS],
                                              const TT_DATA              (&biases)[NBIASES],
                                              output_buffer<TT_DATA>&    __restrict data_o)
{

  aie::vector<TT_DATA,64>  v_reg0 = aie::zeros<TT_DATA,64>();
  aie::vector<TT_DATA,64>  v_reg1 = aie::zeros<TT_DATA,64>();
  aie::vector<TT_DATA,64>  bias_reg = aie::zeros<TT_DATA,64>();

  aie::accum<accfloat,64>  acc_reg0, acc_reg1;

  auto p_data_o = aie::begin_restrict_vector<64>(data_o);

  auto p_biases = aie::begin_restrict_vector<64>(biases);
  // auto p_biases = aie::begin_restrict_vector<16>(biases);
  
  auto p_weights = aie::begin_restrict_vector<64>(weights);

  auto p_data = aie::begin_restrict_vector<64>(data_i);
  auto data_0 = *p_data++;
  auto data_1 = *p_data;

  // Total of 128 nodes 
  // First - 64 nodes ouptut
  for ( unsigned n=0; n < 64; n++)
      chess_loop_range(64,)
      chess_prepare_for_pipelining
  {
      // First 64 samples computation - node (n) For example - Node 0 in first loop
      acc_reg0 = mul_elem_64(data_0, *p_weights++); 
      
      acc_reg0 = mac_elem_64(data_1, *p_weights++, acc_reg0); // Next 64 samples - First node0 computation done (128 samples processed for Node0) in loop 0

      v_reg0.set(aie::reduce_add(acc_reg0.to_vector<TT_DATA>()),n);
    
  }

  // Last - 64 nodes output 
  for ( unsigned n=0; n < 64; n++)
      chess_loop_range(64,)
      chess_prepare_for_pipelining
  {
      acc_reg1 = mul_elem_64(data_0, *p_weights++);

      acc_reg1 = mac_elem_64(data_1, *p_weights++, acc_reg1);
      
      v_reg1.set(aie::reduce_add(acc_reg1.to_vector<TT_DATA>()),n);
  }

  bias_reg = *p_biases++;

  // First - 64 nodes
  // Add accumulated data with bias 
  auto add_bias0 = aie::add(v_reg0,bias_reg);

  // Call SELU function - First 64 nodes donw with SELU
  *p_data_o++ = SeLu(add_bias0);
  
  bias_reg = *p_biases;
  // Last - 64 nodes
  // Add accumulated data with bias 
  auto add_bias1 = aie::add(v_reg1,bias_reg);

  // Call SELU function - - Last 64 nodes donw with SELU
  *p_data_o++ = SeLu(add_bias1);

} 