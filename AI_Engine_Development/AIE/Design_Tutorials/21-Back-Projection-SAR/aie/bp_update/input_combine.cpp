//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "input_combine.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP>
input_combine<NSAMP>::input_combine(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP>
void input_combine<NSAMP>::merge( input_buffer <float>& real_i,
                                  input_buffer <float>& imag_i,
                                  output_buffer<cfloat>& complex_o )
{
  auto ir0 = aie::begin_restrict_vector<8>(real_i);
  auto ir1 = aie::begin_restrict_vector<8>(imag_i);
  auto itw = aie::begin_vector<4>(complex_o);

  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    auto vec = aie::interleave_zip(*ir0++,*ir1++,1);
    *itw++ = (vec.first.extract<8>(0)).cast_to<cfloat>();
    *itw++ = (vec.second.extract<8>(0)).cast_to<cfloat>();
  } // rr
}


