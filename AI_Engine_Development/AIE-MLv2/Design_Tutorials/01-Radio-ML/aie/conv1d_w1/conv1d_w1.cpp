//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>

#include "conv1d_w1.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned KERNEL_SIZE, unsigned NWEIGHTS, unsigned NSPLIT>
conv1d_w1<NSAMP,NNODES,KERNEL_SIZE,NWEIGHTS,NSPLIT>::conv1d_w1(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP, unsigned NNODES, unsigned KERNEL_SIZE, unsigned NWEIGHTS, unsigned NSPLIT>
void conv1d_w1<NSAMP,NNODES,KERNEL_SIZE,NWEIGHTS,NSPLIT>::run(  input_buffer<TT_DATA>&        __restrict data_iq,
                                                                const TT_DATA                (&weights)[NWEIGHTS],
                                                                const TT_DATA                (&biases)[NNODES],
                                                                output_async_buffer<TT_DATA>& __restrict data_o)
{
  auto p_data_iq  = aie::begin_restrict_vector<64>(data_iq);
  
  aie::accum<accfloat,64>  acc;
  aie::vector<TT_DATA,64>  data_iq_0;
  aie::vector<TT_DATA,64>  data_iq_1;
  aie::vector<TT_DATA,64>  data_reg;
  aie::vector<TT_DATA,64>  weights_reg;

  auto p_data_o  = aie::begin_restrict_vector<64>(data_o);

  // Loop over nodes
  for ( unsigned n=0; n < NNODES; n++)
  {
    if ( (n%(NNODES/NSPLIT))==0 )
    {
      data_o.acquire();
      p_data_o  = aie::begin_restrict_vector<64>(data_o);
    }
    // Loop over samples
    for ( unsigned jj=0; jj < NSAMP/128; jj++)
      chess_loop_range(NSAMP/128,)
      chess_prepare_for_pipelining
    {
      data_iq_0 = *p_data_iq++;
      data_iq_1 = *p_data_iq++;
      auto [data_i_0,data_q_0] = aie::interleave_unzip(data_iq_0,data_iq_1,1);
      data_iq_0 = *p_data_iq++;
      data_iq_1 = *p_data_iq++;
      auto [data_i_1,data_q_1] = aie::interleave_unzip(data_iq_0,data_iq_1,1);
      p_data_iq-=2;
      acc  = aie::zeros<accfloat,64>();
      for ( unsigned kk=0; kk < KERNEL_SIZE; kk++)
        chess_unroll_loop(*)
      {
        data_reg  = aie::shuffle_down_fill(data_i_0,data_i_1,kk+1);               // Add +1 to kk since we pre-pad 4 0s, but python model only uses 3 0s.
        weights_reg = aie::broadcast<TT_DATA,64>(weights[kk*NNODES*2+n]);
        acc = mac_elem_64(data_reg,weights_reg,acc);
      
        data_reg  = aie::shuffle_down_fill(data_q_0,data_q_1,kk+1);               // Add +1 to kk since we pre-pad 4 0s, but python model only uses 3 0s.
        weights_reg = aie::broadcast<TT_DATA,64>(weights[kk*NNODES*2+n+NNODES]);
        acc = mac_elem_64(data_reg,weights_reg,acc);
      }
      acc = aie::add(acc,biases[n]);
      *p_data_o++ = aie::max(acc.to_vector<TT_DATA>(),TT_DATA(0));
    }
    p_data_iq -= NSAMP/64;
    if ( ((n+1)%(NNODES/NSPLIT))==0 ) data_o.release();
  }
}