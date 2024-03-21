//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <adf.h>
#include <aie_api/aie.hpp>

#include "farrow_kernel.h"

#include <aie_api/utils.hpp>
// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

farrow_kernel::farrow_kernel( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  for(int i=0;i<STATE_LEN;i++)
  {
    f3_state[i] = (cint16){0,0};
    f2_state[i] = (cint16){0,0};
    f1_state[i] = (cint16){0,0};
    f0_state[i] = (cint16){0,0};
  }
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void farrow_kernel::run(  input_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>&  __restrict sig_i,
			  input_buffer<TT_DEL,adf::extents<BUFFER_SIZE>>&  __restrict del_i,
			  output_buffer<TT_SIG,adf::extents<BUFFER_SIZE>>& __restrict sig_o)
{
  aie::vector<int16,8>        del;
  aie::accum<TT_ACC,8>        acc_f3;
  aie::accum<TT_ACC,8>        acc_f2;
  aie::accum<TT_ACC,8>        acc_f1;
  aie::accum<TT_ACC,8>        acc_f0;
  aie::vector<TT_SIG,8>       y3;
  aie::vector<TT_SIG,8>       y2;
  aie::vector<TT_SIG,8>       y1;
  aie::vector<TT_SIG,8>       y0;
  aie::accum<TT_ACC,8>        acc_x;
  aie::accum<TT_ACC,8>        acc_y;
  aie::accum<TT_ACC,8>        acc_z;
  aie::vector<TT_SIG,8>       z2;
  aie::vector<TT_SIG,8>       z1;
  aie::vector<cint16,16>      v_buff3;
  aie::vector<cint16,16>      v_buff2;
  aie::vector<cint16,16>      v_buff1;
  aie::vector<cint16,16>      v_buff0;
  aie::vector<int16,8>        f3_coeffs;
  aie::vector<int16,8>        f2_coeffs;
  aie::vector<int16,8>        f1_coeffs;
  aie::vector<int16,8>        f0_coeffs;

  v_buff3.insert(1,aie::load_v<8>( f3_state ) );
  v_buff2.insert(1,aie::load_v<8>( f2_state ) );
  v_buff1.insert(1,aie::load_v<8>( f1_state ) );
  v_buff0.insert(1,aie::load_v<8>( f0_state ) );
  f3_coeffs = aie::load_v<8>(f3_taps);
  f2_coeffs = aie::load_v<8>(f2_taps);
  f1_coeffs = aie::load_v<8>(f1_taps);
  f0_coeffs = aie::load_v<8>(f0_taps);


  auto p_sig_i = aie::begin_vector<8>(sig_i);
  auto p_del_i = aie::begin_vector<8>(del_i);
  auto p_sig_o = aie::begin_vector<8>(sig_o);


  // Loop over some samples:
  for ( unsigned rr=0; rr < BUFFER_SIZE/16; rr++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
  {
    v_buff3.insert(0,*p_sig_i);
    v_buff2.insert(0,*p_sig_i);
    v_buff1.insert(0,*p_sig_i);
    v_buff0.insert(0,*p_sig_i++);
    del = aie::filter_even(aie::vector_cast<int16>(*p_del_i++)); // 8x32bits -> 16x16bits -> 8x16bits
    acc_f3 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_antisym(f3_coeffs,0,v_buff3,9);
    y3 = acc_f3.to_vector<TT_SIG>(DNSHIFT);
    acc_f2 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_sym(f2_coeffs,0,v_buff2,9);
    y2 = acc_f2.to_vector<TT_SIG>(DNSHIFT);
    acc_f1 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_antisym(f1_coeffs,0,v_buff1,9);
    y1 = acc_f1.to_vector<TT_SIG>(DNSHIFT);
    acc_f0 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_sym(f0_coeffs,0,v_buff0,9);
    y0 = acc_f0.to_vector<TT_SIG>(DNSHIFT);
    acc_x = aie::mul(y3,del);
    z2 = aie::add(acc_x.to_vector<TT_SIG>(DNSHIFT),y2);
    acc_y = aie::mul(z2,del);
    z1 = aie::add(acc_y.to_vector<TT_SIG>(DNSHIFT),y1);
    acc_z = aie::mul(z1,del);
    *p_sig_o++ = aie::add(y0,acc_z.to_vector<TT_SIG>(DNSHIFT));

    v_buff3.insert(1,*p_sig_i);
    v_buff2.insert(1,*p_sig_i);
    v_buff1.insert(1,*p_sig_i);
    v_buff0.insert(1,*p_sig_i++);
    del = aie::filter_even(aie::vector_cast<int16>(*p_del_i++)); // 8x32bits -> 16x16bits -> 8x16bits
    acc_f3 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_antisym(f3_coeffs,0,v_buff3,1);
    y3 = acc_f3.to_vector<TT_SIG>(DNSHIFT);
    acc_f2 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_sym(f2_coeffs,0,v_buff2,1);
    y2 = acc_f2.to_vector<TT_SIG>(DNSHIFT);
    acc_f1 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_antisym(f1_coeffs,0,v_buff1,1);
    y1 = acc_f1.to_vector<TT_SIG>(DNSHIFT);
    acc_f0 = aie::sliding_mul_sym_xy_ops<8,8,1,1,int16,cint16>::mul_sym(f0_coeffs,0,v_buff0,1);
    y0 = acc_f0.to_vector<TT_SIG>(DNSHIFT);
    acc_x = aie::mul(y3,del);
    z2 = aie::add(acc_x.to_vector<TT_SIG>(DNSHIFT),y2);
    acc_y = aie::mul(z2,del);
    z1 = aie::add(acc_y.to_vector<TT_SIG>(DNSHIFT),y1);
    acc_z = aie::mul(z1,del);
    *p_sig_o++ = aie::add(y0,acc_z.to_vector<TT_SIG>(DNSHIFT));
  }
  *(v8cint16*)f3_state = v_buff3.extract<8>(1);
  *(v8cint16*)f2_state = v_buff2.extract<8>(1);
  *(v8cint16*)f1_state = v_buff1.extract<8>(1);
  *(v8cint16*)f0_state = v_buff0.extract<8>(1);
}
