//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>
#include <cmath>

#include "fmod_floor.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

fmod_floor::fmod_floor(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Baseline
//
// Pipelining:     minII=72   actII=107  for 32 samples
// Throughput:     1504 MB/sec or ~360 Msps
// Total cycles:   1738
// ------------------------------------------------------------

void fmod_floor::run( input_buffer<float>& sig_i, output_buffer<float>& sig_o )
{
  auto itr0 = aie::begin_restrict_vector<8>(sig_i);
  auto itr1 = aie::begin_restrict_vector<8>(sig_i);  itr1 += 1;
  auto itr2 = aie::begin_restrict_vector<8>(sig_i);  itr2 += 2;
  auto itr3 = aie::begin_restrict_vector<8>(sig_i);  itr3 += 3;
  auto itw = aie::begin_restrict_vector<8>(sig_o);
  std::array<aie::vector<float,8>,4> vec;
  for (unsigned rr=0; rr < NSAMP/32; rr++)
    chess_prepare_for_pipelining
  {
    vec[0] = *itr0;  itr0 += 4;
    vec[1] = *itr1;  itr1 += 4;
    vec[2] = *itr2;  itr2 += 4;
    vec[3] = *itr3;  itr3 += 4;
    vec[0] = aie::sub(vec[0],aie::to_float(aie::to_fixed(aie::sub(vec[0],0.5f),0)));
    vec[1] = aie::sub(vec[1],aie::to_float(aie::to_fixed(aie::sub(vec[1],0.5f),0)));
    vec[2] = aie::sub(vec[2],aie::to_float(aie::to_fixed(aie::sub(vec[2],0.5f),0)));
    vec[3] = aie::sub(vec[3],aie::to_float(aie::to_fixed(aie::sub(vec[3],0.5f),0)));
    *itw++ = vec[0];
    *itw++ = vec[1];
    *itw++ = vec[2];
    *itw++ = vec[3];
  } // rr
}

