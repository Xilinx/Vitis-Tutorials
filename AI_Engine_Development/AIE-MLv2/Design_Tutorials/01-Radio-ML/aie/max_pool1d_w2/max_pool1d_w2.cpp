//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/utils.hpp>

#include "max_pool1d_w2.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

max_pool1d_w2::max_pool1d_w2(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void max_pool1d_w2::run(  input_buffer<TT_DATA>&  __restrict data_i,
                          output_buffer<TT_DATA>& __restrict data_o)
{
  
  aie::vector<TT_DATA,32>  data_i_0;
  aie::vector<TT_DATA,32>  data_i_1;
  aie::vector<TT_DATA,32>  data_i_2;
  aie::vector<TT_DATA,32>  data_i_3;

  aie::vector<TT_DATA,32>  data_o_0;
  aie::vector<TT_DATA,32>  data_o_1;

  auto p_data_i_0  = aie::begin_vector<32>(data_i);              // Works on even rows
  auto p_data_i_1  = aie::begin_vector<32>(data_i.data()+NSAMP); // Works on odd rows
  auto p_data_o    = aie::begin_vector<64>(data_o);
  // Loop over nodes
  for ( unsigned ii=0; ii < NNODES/NSPLIT/2; ii++)
  {
    // Loop over samples
    for ( unsigned jj=0; jj < NSAMP/64; jj++)
        chess_prepare_for_pipelining
    {
      data_i_0  = *p_data_i_0++;
      data_i_1  = *p_data_i_0++;
      auto [v1, v2] = aie::interleave_unzip(data_i_0,data_i_1,1);
      data_o_0 = aie::max(v1,v2);

      data_i_2  = *p_data_i_1++;
      data_i_3  = *p_data_i_1++;
      auto [v3, v4] = aie::interleave_unzip(data_i_2,data_i_3,1);
      data_o_1 = aie::max(v3,v4);

      auto rv = aie::interleave_zip(data_o_0,data_o_1,1);
      *p_data_o++ = aie::concat(rv.first,rv.second);
    }
    p_data_i_0+=NSAMP/32;
    p_data_i_1+=NSAMP/32;
  }
}