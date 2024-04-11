//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>

#include "farrow_kernel2.h"

#include <aie_api/utils.hpp>
// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

farrow_kernel2::farrow_kernel2( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void farrow_kernel2::run(  input_buffer<TT_DEL,adf::extents<BUFFER_SIZE>>&  __restrict del_i,
                           input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y3,
                           input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y2,
                           input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y1,
                           input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict y0,
                           output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict sig_o)
{
  aie::vector<int16,8>           del;
  aie::accum<TT_ACC,8>           acc_x;
  aie::accum<TT_ACC,8>           acc_y;
  aie::accum<TT_ACC,8>           acc_z;

  auto p_del_i = aie::begin_vector<4>(del_i);
  auto p_y3 = aie::begin_vector<8>(y3);
  auto p_y2 = aie::begin_vector<8>(y2);
  auto p_y1 = aie::begin_vector<8>(y1);
  auto p_y0 = aie::begin_vector<8>(y0);
  auto p_sig_o = aie::begin_vector<8>(sig_o);

  auto p_z2    = aie::begin_restrict_vector<8>(z);
  auto p_z1    = aie::begin_restrict_vector<8>(z);

  // Loop over some samples:
  for ( unsigned rr=0; rr < BUFFER_SIZE/16; rr++)
    chess_loop_range(1,0)
    chess_prepare_for_pipelining
  {
    del = aie::vector_cast<int16>(*p_del_i++);*p_del_i++;
    acc_x = aie::mac(aie::from_vector<TT_ACC>(*p_y2++,DNSHIFT), *p_y3++,del);
    *p_z2++ = acc_x.to_vector<TT_SIG>(DNSHIFT);
    
    del = aie::vector_cast<int16>(*p_del_i++);*p_del_i++;
    acc_x = aie::mac(aie::from_vector<TT_ACC>(*p_y2++,DNSHIFT), *p_y3++,del);
    *p_z2++ = acc_x.to_vector<TT_SIG>(DNSHIFT);
  }

  p_del_i = aie::begin_vector<4>(del_i);
  p_z2 = aie::begin_restrict_vector<8>(z);

  for ( unsigned rr=0; rr < BUFFER_SIZE/16; rr++)
    chess_loop_range(1,0)
    chess_prepare_for_pipelining
  {
    del = aie::vector_cast<int16>(*p_del_i++);*p_del_i++;
    acc_y = aie::mac(aie::from_vector<TT_ACC>(*p_y1++,DNSHIFT), *p_z2++,del);
    *p_z1++ = acc_y.to_vector<TT_SIG>(DNSHIFT);

    del = aie::vector_cast<int16>(*p_del_i++);*p_del_i++;
    acc_y = aie::mac(aie::from_vector<TT_ACC>(*p_y1++,DNSHIFT), *p_z2++,del);
    *p_z1++ = acc_y.to_vector<TT_SIG>(DNSHIFT);
  }

  p_del_i = aie::begin_vector<4>(del_i);
  p_z1 = aie::begin_restrict_vector<8>(z);

  for ( unsigned rr=0; rr < BUFFER_SIZE/16; rr++)
    chess_loop_range(1,0)
    chess_prepare_for_pipelining
  {
    del = aie::vector_cast<int16>(*p_del_i++);*p_del_i++;
    acc_z = aie::mac(aie::from_vector<TT_ACC>(*p_y0++,DNSHIFT), *p_z1++,del);
    *p_sig_o++ = acc_z.to_vector<TT_SIG>(DNSHIFT);

    del = aie::vector_cast<int16>(*p_del_i++);*p_del_i++;
    acc_z = aie::mac(aie::from_vector<TT_ACC>(*p_y0++,DNSHIFT), *p_z1++,del);
    *p_sig_o++ = acc_z.to_vector<TT_SIG>(DNSHIFT);
  }
}
