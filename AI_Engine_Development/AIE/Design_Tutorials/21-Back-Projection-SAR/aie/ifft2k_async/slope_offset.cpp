//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "slope_offset.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

slope_offset::slope_offset(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void slope_offset::run( input_buffer<cfloat>& sig_i,
                        output_async_buffer<float>& real_o, output_async_buffer<float>& imag_o )
{
  real_o.acquire();
  imag_o.acquire();
  auto itr0 = aie::begin_restrict_vector<4>(sig_i);
  auto itr1 = aie::begin_restrict_vector<4>(sig_i); itr1 += 1;
  auto itw0 = aie::begin_vector<8>(real_o);
  auto itw1 = aie::begin_vector<8>(imag_o);

  aie::vector<cfloat,16> buff_i;
  aie::vector<float,8>   xvec = aie::load_v<8>(x_init);

  for (unsigned rr=0; rr < NSAMP/8-1; rr++)
    chess_prepare_for_pipelining
  {
    buff_i.insert(0,*(itr0+0));
    buff_i.insert(1,*(itr1+0));
    buff_i.insert(2,*(itr0+2));
    buff_i.insert(3,*(itr1+2));
    auto data0_r = aie::real(buff_i.extract<8>(0));
    auto data0_i = aie::imag(buff_i.extract<8>(0));
    auto tmp = aie::shuffle_down(buff_i,1);
    auto data1_r = aie::real(tmp.extract<8>(0));
    auto data1_i = aie::imag(tmp.extract<8>(0));
    auto slope_r  = (aie::mul(aie::sub(data1_r,data0_r),scale_x)).to_vector<float>();
    auto slope_i  = (aie::mul(aie::sub(data1_i,data0_i),scale_x)).to_vector<float>();
    auto offset_r = aie::sub(data0_r,(aie::mul(slope_r,xvec)).to_vector<float>());
    auto offset_i = aie::sub(data0_i,(aie::mul(slope_i,xvec)).to_vector<float>());
    auto result_r = aie::interleave_zip(slope_r,offset_r,1);
    auto result_i = aie::interleave_zip(slope_i,offset_i,1);
    *itw0++ = result_r.first;
    *itw0++ = result_r.second;
    *itw1++ = result_i.first;
    *itw1++ = result_i.second;
    xvec = aie::add(xvec,step_x);
    itr0 += 2;
    itr1 += 2;
  } // rr
  // Last iteration -- special case, last bin has zero valued
  buff_i.insert(0,*(itr0+0));
  buff_i.insert(1,*(itr1+0));
  buff_i.insert(2,aie::zeros<cfloat,4>());
  buff_i.insert(3,aie::zeros<cfloat,4>());
  auto data0_r = aie::real(buff_i.extract<8>(0));
  auto data0_i = aie::imag(buff_i.extract<8>(0));
  auto tmp = aie::shuffle_down(buff_i,1);
  auto data1_r = aie::real(tmp.extract<8>(0));
  auto data1_i = aie::imag(tmp.extract<8>(0));
  auto slope_r  = (aie::mul(aie::sub(data1_r,data0_r),scale_x)).to_vector<float>();
  auto slope_i  = (aie::mul(aie::sub(data1_i,data0_i),scale_x)).to_vector<float>();
  slope_r.set(0.0,7);
  slope_i.set(0.0,7);
  auto offset_r = aie::sub(data0_r,(aie::mul(slope_r,xvec)).to_vector<float>());
  auto offset_i = aie::sub(data0_i,(aie::mul(slope_i,xvec)).to_vector<float>());
  offset_r.set(0.0,7);
  offset_i.set(0.0,7);
  auto result_r = aie::interleave_zip(slope_r,offset_r,1);
  auto result_i = aie::interleave_zip(slope_i,offset_i,1);
  *itw0++ = result_r.first;
  *itw0++ = result_r.second;
  *itw1++ = result_i.first;
  *itw1++ = result_i.second;

  real_o.release();
  imag_o.release();
}


