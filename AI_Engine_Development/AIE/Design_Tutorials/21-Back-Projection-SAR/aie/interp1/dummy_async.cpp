//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dummy_async.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

dummy_async::dummy_async(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void dummy_async::run( input_buffer<float>& sig_i, output_async_buffer<float>& sig_o )
{
  sig_o.acquire();
  auto itr = aie::begin_vector<4>(sig_i);
  auto itw = aie::begin_vector<4>(sig_o);
  for (unsigned ii=0; ii < NSAMP/4; ii++)
    chess_prepare_for_pipelining
  {
    *itw++ = *itr++;
  }
  sig_o.release();
}


