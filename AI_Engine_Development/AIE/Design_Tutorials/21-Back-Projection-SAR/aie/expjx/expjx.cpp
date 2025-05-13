//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "expjx.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template <unsigned NSAMP>
expjx<NSAMP>::expjx(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP>
inline __attribute__((noinline))
void expjx<NSAMP>::run( input_buffer<float>& cos_i, input_buffer<float>& sin_i, output_buffer<cfloat>& exp_o )
{
  auto itr0 = aie::begin_vector<8>(cos_i);
  auto itr1 = aie::begin_vector<8>(sin_i);
  auto itw  = aie::begin_vector<4>(exp_o);
  for (unsigned ii=0; ii < NSAMP/8; ii++)
    chess_prepare_for_pipelining
  {
    auto real = *itr0++;
    auto imag = *itr1++;
    auto result = aie::interleave_zip(real,imag,1);
    *itw++ = (result.first).cast_to<cfloat>();
    *itw++ = (result.second).cast_to<cfloat>();
  } // ii
}


