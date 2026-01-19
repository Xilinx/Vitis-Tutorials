//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>

#include "conv1d_template.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned KERNEL_SIZE, unsigned NWEIGHTS, unsigned NSPLIT>
conv1d_template<NSAMP,NNODES,KERNEL_SIZE,NWEIGHTS,NSPLIT>::conv1d_template(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned KERNEL_SIZE, unsigned NWEIGHTS, unsigned NSPLIT>
void conv1d_template<NSAMP,NNODES,KERNEL_SIZE,NWEIGHTS,NSPLIT>::run(  input_buffer<TT_DATA>&     __restrict data_i,
                                                                      const TT_DATA              (&weights)[NWEIGHTS],
                                                                      const TT_DATA              (&biases)[NNODES],
                                                                      output_buffer<TT_DATA>&    __restrict data_o)
{
  auto p_data_o  = aie::begin_restrict_vector<32>(data_o);
  
  aie::vector<TT_DATA,32>  data_reg0;
  aie::vector<TT_DATA,32>  data_reg1;
  aie::vector<TT_DATA,32>  data_reg2;
  aie::vector<TT_DATA,32>  data_reg3;
  aie::vector<TT_DATA,32>  data_patch_reg0;
  aie::vector<TT_DATA,32>  data_patch_reg1;
  aie::vector<TT_DATA,32>  data_patch_reg2;
  aie::vector<TT_DATA,32>  data_patch_reg3;
  aie::vector<TT_DATA,32>  data_patch_reg4;
  aie::vector<TT_DATA,32>  data_patch_reg5;
  aie::accum<accfloat,32>  acc_reg0;
  aie::accum<accfloat,32>  acc_reg1;
  aie::accum<accfloat,32>  acc_reg2;
  aie::accum<accfloat,32>  acc_reg3;
  aie::vector<TT_DATA,64>  weights_reg;
  aie::vector<TT_DATA,32>  bias_reg;
  aie::accum<accfloat,32>  bias_acc;


  auto p_biases  = aie::begin_restrict_vector<8>(biases);
  unsigned index_offset;
  auto p_weights = aie::begin_restrict_vector<64>(weights);

  // Loop over some samples and output nodes:
  for ( unsigned iter=0,ii=0; iter < NSAMP/16/NSPLIT*NNODES/8; iter++)
  {
    index_offset=1408*ii;   // 1408=8*22*8
    auto p_data0 = aie::begin_restrict_vector<32>(data_i.data()+index_offset        );  // Input nodes 0-7,  16-23, 32-39, 48-55
    auto p_data1 = aie::begin_restrict_vector<32>(data_i.data()+index_offset+176-16 );  // Input nodes 8-15, 24-31, 40-47, 56-63
                                                                                        // 176=8*22. Subtracting 16 allows doing an aligned read. We add this 16 offset later when doing shuffle_down_fill

    acc_reg0  = aie::zeros<accfloat,32>();
    acc_reg1  = aie::zeros<accfloat,32>();
    acc_reg2  = aie::zeros<accfloat,32>();
    acc_reg3  = aie::zeros<accfloat,32>();
    // Loop over input nodes 0-7, 16-23, 32-39, 48-55
    for ( unsigned kk=0; kk < NNODES/8/2; kk++)
        chess_loop_range(NNODES/8/2,)
        chess_prepare_for_pipelining
    {
      data_patch_reg0 = *p_data0++;
      data_patch_reg1 = *p_data0++;
      data_patch_reg2 = *p_data0++;
      data_patch_reg3 = *p_data0++;
      data_patch_reg4 = *p_data0++;
      data_patch_reg5 = *p_data0++;
      // weight0
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,0)
      // weight1
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,8)
      // weight2
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,16)
      // weight3
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,24)
      // weight4
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,32)
      // weight5
      COMPUTE_PATTERN(data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,data_patch_reg5,8)
      // weight6
      COMPUTE_PATTERN(data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,data_patch_reg5,16)
      p_weights+=KERNEL_SIZE;
      p_data0 += 5;
    }
    
    p_weights -= (NNODES/8-1)*KERNEL_SIZE;
    
    // Loop over odd input nodes 8-15, 24-31, 40-47, 56-63
    for ( unsigned kk=0; kk < NNODES/8/2; kk++)
        chess_loop_range(NNODES/8/2,)
        chess_prepare_for_pipelining
    {
      data_patch_reg0 = *p_data1++;
      data_patch_reg1 = *p_data1++;
      data_patch_reg2 = *p_data1++;
      data_patch_reg3 = *p_data1++;
      data_patch_reg4 = *p_data1++;
      data_patch_reg5 = *p_data1++;
      // weight0
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,16)
      // weight1
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,24)
      // weight2
      COMPUTE_PATTERN(data_patch_reg0,data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,32)
      // weight3
      COMPUTE_PATTERN(data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,data_patch_reg5,8)
      // weight4
      COMPUTE_PATTERN(data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,data_patch_reg5,16)
      // weight5
      COMPUTE_PATTERN(data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,data_patch_reg5,24)
      // weight6
      COMPUTE_PATTERN(data_patch_reg1,data_patch_reg2,data_patch_reg3,data_patch_reg4,data_patch_reg5,32)
      p_weights+=KERNEL_SIZE;
      p_data1 += 5;
    }
    p_weights -= KERNEL_SIZE;
    bias_reg = aie::concat(*p_biases,*p_biases,*p_biases,*p_biases);p_biases++;
    bias_acc.from_vector(bias_reg);
    
    acc_reg0 = aie::add(acc_reg0,bias_acc);
    *p_data_o++ = aie::max(acc_reg0.to_vector<TT_DATA>(),TT_DATA(0));
    
    acc_reg1 = aie::add(acc_reg1,bias_acc);
    *p_data_o++ = aie::max(acc_reg1.to_vector<TT_DATA>(),TT_DATA(0));
    
    acc_reg2 = aie::add(acc_reg2,bias_acc);
    *p_data_o++ = aie::max(acc_reg2.to_vector<TT_DATA>(),TT_DATA(0));
    
    acc_reg3 = aie::add(acc_reg3,bias_acc);
    *p_data_o++ = aie::max(acc_reg3.to_vector<TT_DATA>(),TT_DATA(0));
    if ( ((iter+1)%(NNODES/8))==0 )
    {
      ii++;
      p_weights = aie::begin_restrict_vector<64>(weights);
      p_biases = aie::begin_restrict_vector<8>(biases);
    }
  }
}
