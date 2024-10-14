//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include <adf.h>
#include <aie_api/aie.hpp>

#include "farrow_kernel1.h"

#include <aie_api/utils.hpp>

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

farrow_kernel1::farrow_kernel1( int16 (&taps)[8] ) : f_taps(taps)
{
 
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  
  for(int i=0;i<STATE_LEN;i++)
  {
    f_state[i] = (cint16){0,0};
  }

}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void farrow_kernel1::run(  input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict sig_i,
                           output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict y )
{
  aie::accum<TT_ACC,16>           acc_f3; 

  aie::vector<cint16,32> v_buff; 
  v_buff.insert(3,aie::load_v<8>( f_state ) ); 
  aie::vector<int16,8> f_coeffs = aie::load_v<8>(f_taps);

  auto p_sig_i = aie::begin_vector<16>(sig_i); 
  auto p_y    = aie::begin_vector<16>(y);    

  // Loop over some samples:
  for ( unsigned rr=0; rr < BUFFER_SIZE/32; rr++) 
    chess_loop_range(BUFFER_SIZE/32,BUFFER_SIZE/32)
    chess_prepare_for_pipelining
  {
    v_buff.insert(0,*p_sig_i++);
    acc_f3 = aie::sliding_mul_ops< 16, 8, 1, 1, 1, int16, cint16>::mul(f_coeffs,0,v_buff,25); 
    *p_y++ = acc_f3.to_vector<TT_SIG>(DNSHIFT);

    v_buff.insert(1,*p_sig_i++);
    acc_f3 = aie::sliding_mul_ops<16,8,1,1,1,int16,cint16>::mul(f_coeffs,0,v_buff,9);  
    *p_y++ = acc_f3.to_vector<TT_SIG>(DNSHIFT);
  }

 *(v8cint16*)f_state = v_buff.extract<8>(3); 

}

