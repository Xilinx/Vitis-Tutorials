//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "bp_update.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned NSAMP>
bp_update<NSAMP>::bp_update(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned NSAMP>
inline __attribute__((noinline))
void bp_update<NSAMP>::run( input_buffer <cfloat>& image_i,
                            input_buffer <cfloat>& interp_i,
                            input_buffer <cfloat>& phase_i,
                            output_buffer<cfloat>& image_o )
{
  auto ir0 = aie::begin_restrict_vector<4>(image_i);
  auto ir1 = aie::begin_restrict_vector<4>(interp_i);
  auto ir2 = aie::begin_restrict_vector<4>(phase_i);
  auto itw = aie::begin_vector<4>(image_o);
  aie::accum<caccfloat,4> acc_p;
  aie::accum<caccfloat,4> acc_u;

  for (unsigned rr=0; rr < NSAMP/4; rr++)
    chess_prepare_for_pipelining
  {
    acc_p.from_vector(*ir0++);
    acc_u = aie::mac(acc_p,*ir1++,*ir2++);
    *itw++ = acc_u.to_vector<cfloat>();
  } // rr
}


