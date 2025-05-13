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

template<unsigned NSAMP>
fmod_floor<NSAMP>::fmod_floor(void)
{
  aie::set_rounding(aie::rounding_mode::floor);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Baseline
//
// Pipelining:     minII=19   actII=20  for 8 samples
// Throughput:     1869 MB/sec or ~450 Msps
// Total cycles:   1329
// ------------------------------------------------------------

template<unsigned NSAMP>
inline __attribute__((noinline))
void fmod_floor<NSAMP>::run( input_buffer<float>& sig_i, output_buffer<float>& sig_o )
{
  auto itr = aie::begin_restrict_vector<8>(sig_i);
  auto itw = aie::begin_restrict_vector<8>(SCRATCH);

  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    *itw++ = aie::sub(*itr++,0.5f);
  }
  itr = aie::begin_restrict_vector<8>(SCRATCH);
  int32* ptw = reinterpret_cast<int32*>(sig_o.data());
  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    aie::store_v(ptw,aie::to_fixed(*itr++,0));
    ptw += 8;
  }
  int32* ptr = reinterpret_cast<int32*>(sig_o.data());
  itw = aie::begin_restrict_vector<8>(SCRATCH);
  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    *itw++ = aie::to_float(aie::load_v<8>(ptr));
    ptr += 8;
  }
  auto iti = aie::begin_restrict_vector<8>(sig_i);
  itr = aie::begin_restrict_vector<8>(SCRATCH);
  itw = aie::begin_restrict_vector<8>(sig_o);
  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    *itw++ = aie::min(aie::sub(*iti++,*itr++),MAXVAL);
  }
}

