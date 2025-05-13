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

bp_update::bp_update(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void bp_update::run( input_buffer <cfloat>& pixel_i,
                     input_buffer <cfloat>& dist_i,
                     input_buffer <cfloat>& phcorr_i,
                     output_buffer<cfloat>& pixel_o )
{
  auto ir0 = aie::begin_restrict_vector<4>(pixel_i);
  auto ir1 = aie::begin_restrict_vector<4>(dist_i);
  auto ir2 = aie::begin_restrict_vector<4>(phcorr_i);
  auto itw = aie::begin_vector<4>(pixel_o);
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


