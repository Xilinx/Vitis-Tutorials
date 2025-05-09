//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "conv2d_w1.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

conv2d_w1::conv2d_w1(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  fsm_state = 0;
}

// ------------------------------------------------------------
// Grab Weights
// ------------------------------------------------------------

void conv2d_w1::grab_wts( input_async_buffer<bfloat16>& wts_i )
{
  if (fsm_state == 0) {
    wts_i.acquire();
    fsm_state = 1;
  }
}

// ------------------------------------------------------------
// Pass Through (Debug Mem Tile Zero Pad)
// ------------------------------------------------------------

void conv2d_w1::passthru( input_buffer <bfloat16>& ifm_i,
                          output_buffer<bfloat16>& ofm_o )
{
  auto itr = aie::begin_vector<16>(ifm_i);
  auto itw = aie::begin_vector<16>(ofm_o);
  for (unsigned cc=0; cc < 28; cc++) {
    for (unsigned rr=0; rr < 32/16; rr++) {
      *itw++ = *itr++;
    }
  }
}

// ------------------------------------------------------------
// Filter 3x3
// ------------------------------------------------------------

void conv2d_w1::filter_3x3( input_buffer<bfloat16>&       ifm_i,
                            input_async_buffer<bfloat16>& wts_i,
                            output_buffer<bfloat16>&      ofm_o )
{
  // Input shape is (28,32), output shape is (26,32,16)
  std::array<aie::vector<bfloat16,32>,3> buffd;
  std::array<aie::vector<bfloat16,32>,3> buffw;
  std::array<aie::accum<accfloat,16>,3>  acc;
  aie::accum<accfloat,16>                bias;

  auto itrA = aie::begin_restrict_vector<16>(ifm_i);
  auto itrB = aie::begin_restrict_vector<16>(ifm_i);
  auto itw = aie::begin_vector<16>(ofm_o);
  auto itapA = aie::begin_restrict_vector<16>(wts_i);
  auto itapB = aie::begin_restrict_vector<16>(wts_i);

  // Must have upper 16 lanes zero:
  buffw[0] = aie::zeros<bfloat16,32>();
  buffw[1] = aie::zeros<bfloat16,32>();
  buffw[2] = aie::zeros<bfloat16,32>();

  bias.from_vector(aie::load_v<16>(wts_i.data()+144));

  // Loop over output image rows:
  for (unsigned rr=0; rr < 26; rr++)
    chess_prepare_for_pipelining
  {
    // Read in next 3 input image rows:
    buffd[0].insert(0,*(itrA+0));     buffd[0].insert(1,*(itrB+1));
    buffd[1].insert(0,*(itrA+2));     buffd[1].insert(1,*(itrB+3));
    buffd[2].insert(0,*(itrA+4));     buffd[2].insert(1,*(itrB+5));

    // Loop over output image columns taking two at a time:
    unsigned c = 0;
    for (unsigned cc=0; cc < 26; cc++)
      chess_prepare_for_pipelining
    {
      // 1st iteration w0, w1, w2
      buffw[0].insert(0,*(itapA+0));
      buffw[1].insert(0,*(itapB+1));
      buffw[2].insert(0,*(itapA+2));
      acc[0] = mul_elem_16_2(aie::broadcast<bfloat16,32>(buffd[0].get(c  )),buffw[0]);
      acc[1] = mul_elem_16_2(aie::broadcast<bfloat16,32>(buffd[0].get(c+1)),buffw[1]);
      acc[2] = mul_elem_16_2(aie::broadcast<bfloat16,32>(buffd[0].get(c+2)),buffw[2]);
      // 2nd iteration w3, w4, w5
      buffw[0].insert(0,*(itapA+3));
      buffw[1].insert(0,*(itapB+4));
      buffw[2].insert(0,*(itapA+5));
      acc[0] = mac_elem_16_2(aie::broadcast<bfloat16,32>(buffd[1].get(c  )),buffw[0],acc[0]);
      acc[1] = mac_elem_16_2(aie::broadcast<bfloat16,32>(buffd[1].get(c+1)),buffw[1],acc[1]);
      acc[2] = mac_elem_16_2(aie::broadcast<bfloat16,32>(buffd[1].get(c+2)),buffw[2],acc[2]);
      // 3rd iteration w6, w7, w8
      buffw[0].insert(0,*(itapA+6));
      buffw[1].insert(0,*(itapB+7));
      buffw[2].insert(0,*(itapA+8));
      acc[0] = mac_elem_16_2(aie::broadcast<bfloat16,32>(buffd[2].get(c  )),buffw[0],acc[0]);
      acc[1] = mac_elem_16_2(aie::broadcast<bfloat16,32>(buffd[2].get(c+1)),buffw[1],acc[1]);
      acc[2] = mac_elem_16_2(aie::broadcast<bfloat16,32>(buffd[2].get(c+2)),buffw[2],acc[2]);
      c = chess_copy(c+2)-1;

      // Add bias and RELU:
      acc[0] = aie::add(aie::add(acc[0],acc[2]),aie::add(acc[1],bias));
      *itw++ = aie::max(acc[0].to_vector<bfloat16>(),bfloat16(0));
    } // cc
    // Add zero-padding for pixels from 26 to 32:
    *itw++ = aie::zeros<bfloat16,16>();
    *itw++ = aie::zeros<bfloat16,16>();
    *itw++ = aie::zeros<bfloat16,16>();
    *itw++ = aie::zeros<bfloat16,16>();
    *itw++ = aie::zeros<bfloat16,16>();
    *itw++ = aie::zeros<bfloat16,16>();
    itrA += 2;  // Advance by one row
    itrB += 2;  // Advance by one row
  } // rr
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void conv2d_w1::run( input_buffer      <bfloat16>& ifm_i,
                     input_async_buffer<bfloat16>& wts_i,
                     output_buffer     <bfloat16>& ofm_o )
{
  grab_wts( wts_i );
  filter_3x3( ifm_i, wts_i, ofm_o );
}


