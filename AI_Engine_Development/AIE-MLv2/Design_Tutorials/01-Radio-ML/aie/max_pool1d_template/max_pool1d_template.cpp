//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>

#include "max_pool1d_template.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template <unsigned NSAMP, unsigned NNODES, unsigned NSPLIT>
max_pool1d_template<NSAMP,NNODES,NSPLIT>::max_pool1d_template()
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template <unsigned NSAMP, unsigned NNODES, unsigned NSPLIT>
void max_pool1d_template<NSAMP,NNODES,NSPLIT>::run(  input_buffer<TT_DATA>&  __restrict data_i,
                                                     output_buffer<TT_DATA>& __restrict data_o)
{
  
  aie::vector<TT_DATA,64>  data_i_0;
  aie::vector<TT_DATA,64>  data_i_1;

  // Loop over some samples:
  //for ( unsigned n=0; n < NSPLIT; n++)
  //{
    auto p_data_i  = aie::begin_vector<64>(data_i);
    auto p_data_o  = aie::begin_vector<64>(data_o);
    // Loop over some nodes and samples
    for ( unsigned ii=0; ii < NNODES*NSAMP/64/2/NSPLIT; ii++)
        chess_loop_range(NNODES*NSAMP/64/2/NSPLIT,)
        chess_prepare_for_pipelining
    {
      data_i_0  = *p_data_i++;
      data_i_1  = *p_data_i++;
      *p_data_o++ = aie::max(data_i_0,data_i_1);
    }
  //}
}