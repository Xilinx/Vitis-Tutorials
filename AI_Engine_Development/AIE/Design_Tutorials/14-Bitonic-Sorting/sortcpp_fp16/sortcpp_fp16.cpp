//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins
#include <algorithm>
#include <array>
#include <adf.h>
#include <aie_api/aie.hpp>

#include "sortcpp_fp16.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

sortcpp_fp16::sortcpp_fp16(void )
{
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void sortcpp_fp16::run( input_stream<float>* sig_i, output_stream<float>* sig_o )
{
  std::array<float,SIZE> vec;
  // Input:
  for (unsigned ii=0; ii<SIZE; ii++) chess_prepare_for_pipelining {
    vec[ii] = readincr(sig_i);
  }
  // Sort:
  std::sort(vec.begin(),vec.end());
  // Output:
  for (unsigned ii=0; ii<SIZE; ii++) chess_prepare_for_pipelining {
      writeincr(sig_o,vec[ii]);
  }
}



