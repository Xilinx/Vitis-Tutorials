//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "max_pooling2d_w2.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

max_pooling2d_w2::max_pooling2d_w2( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void max_pooling2d_w2::run( input_buffer<bfloat16>& ifm_i, output_buffer<bfloat16>& ofm_o )
{
  // Inputs contain (26,32,16) tensors:
  auto itw = aie::begin_vector<16>(ofm_o);
  auto itrA = aie::begin_restrict_vector<16>(ifm_i);
  auto itrB = aie::begin_restrict_vector<16>(ifm_i);
  std::array<aie::vector<bfloat16,32>,8> buff;

  // Outer loop over output image rows 0 through 13:
  //    Inner loop over output pixels 0 through 16, four at a time:
  // Collapse into a single loop (13*4 = 52)
  for (unsigned rr=0,off=0,idx=0; rr < 52; rr++)
    chess_prepare_for_pipelining
  {
    // Pair of registers contains one pixel for all 64 input layers:
    buff[0].insert(0,*(itrA+idx+ 0));   buff[0].insert(1,*(itrB+idx+ 2));  // Pixel 0 & 2 row n   for ic=(0,15)
    buff[1].insert(0,*(itrA+idx+ 1));   buff[1].insert(1,*(itrB+idx+ 3));  // Pixel 1 & 3 row n   for ic=(0,15)
    buff[2].insert(0,*(itrA+idx+32));   buff[2].insert(1,*(itrB+idx+34));  // Pixel 0 & 2 row n+1 for ic=(0,15)
    buff[3].insert(0,*(itrA+idx+33));   buff[3].insert(1,*(itrB+idx+35));  // Pixel 1 & 3 row n+1 for ic=(0,15)
    buff[4].insert(0,*(itrA+idx+ 4));   buff[4].insert(1,*(itrB+idx+ 6));  // Pixel 4 & 6 row n   for ic=(0,15)
    buff[5].insert(0,*(itrA+idx+ 5));   buff[5].insert(1,*(itrB+idx+ 7));  // Pixel 5 & 7 row n   for ic=(0,15)
    buff[6].insert(0,*(itrA+idx+36));   buff[6].insert(1,*(itrB+idx+38));  // Pixel 4 & 6 row n+1 for ic=(0,15)
    buff[7].insert(0,*(itrA+idx+37));   buff[7].insert(1,*(itrB+idx+39));  // Pixel 5 & 7 row n+1 for ic=(0,15)
    buff[0] = aie::max(buff[0],buff[1]);
    buff[2] = aie::max(buff[2],buff[3]);
    buff[0] = aie::max(buff[0],buff[2]);
    buff[4] = aie::max(buff[4],buff[5]);
    buff[6] = aie::max(buff[6],buff[7]);
    buff[4] = aie::max(buff[4],buff[6]);
    // Corresponds to 1 output row, 2 output pixels for 16 output layers:
    *itw++ = buff[0].extract<16>(0);
    *itw++ = buff[0].extract<16>(1);
    *itw++ = buff[4].extract<16>(0);
    *itw++ = buff[4].extract<16>(1);
    idx += 8; // Advance by 4 input pixels
    if (rr % 4 == 3) {
      off = off + 64;           // Advance by 2 input rows
      idx = off;                // Reset index
    }
    // The 'chess_separator_scheduler()' below avoid a back-end compiler scheduling issue that introduces
    // functional errors in the output. Filed CRVO-12023. This temporary workaround may have a
    // throughput impact.
    chess_separator_scheduler();
  } // rr
}


