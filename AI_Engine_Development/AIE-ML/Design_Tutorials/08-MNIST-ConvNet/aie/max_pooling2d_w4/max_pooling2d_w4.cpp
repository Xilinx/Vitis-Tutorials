//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "max_pooling2d_w4.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

max_pooling2d_w4::max_pooling2d_w4( void )
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void max_pooling2d_w4::run( input_buffer<bfloat16>& ifm_i, output_buffer<bfloat16>& ofm_o )
{
  // Inputs contain (11,16,64) tensors:
  auto itw = aie::begin_vector<16>(ofm_o);
  auto itrA = aie::begin_restrict_vector<16>(ifm_i);
  auto itrB = aie::begin_restrict_vector<16>(ifm_i);    itrB += 1;
  std::array<aie::vector<bfloat16,32>,8> buff;

  // Loop over output image rows:
  for (unsigned rr=0,off=0; rr < 5; rr++)
    chess_prepare_for_pipelining
  {
    // Loop over output pixels:
    for (unsigned pp=0,idx=off; pp < 8; pp++)
      chess_prepare_for_pipelining
    {
      // Pair of registers contains one pixel for all 64 input layers:
      buff[0].insert(0,*(itrA+idx+ 0));   buff[0].insert(1,*(itrB+idx+ 0)); // Pixel 0, ic=( 0,31), row n
      buff[1].insert(0,*(itrA+idx+ 2));   buff[1].insert(1,*(itrB+idx+ 2)); // Pixel 0, ic=(32,63), row n
      buff[2].insert(0,*(itrA+idx+ 4));   buff[2].insert(1,*(itrB+idx+ 4)); // Pixel 1, ic=( 0,31), row n
      buff[3].insert(0,*(itrA+idx+ 6));   buff[3].insert(1,*(itrB+idx+ 6)); // Pixel 1, ic=(32,63), row n
      buff[4].insert(0,*(itrA+idx+64));   buff[4].insert(1,*(itrB+idx+64)); // Pixel 0, ic=( 0,31), row n+1
      buff[5].insert(0,*(itrA+idx+66));   buff[5].insert(1,*(itrB+idx+66)); // Pixel 0, ic=(32,63), row n+1
      buff[6].insert(0,*(itrA+idx+68));   buff[6].insert(1,*(itrB+idx+68)); // Pixel 1, ic=( 0,31), row n+1
      buff[7].insert(0,*(itrA+idx+70));   buff[7].insert(1,*(itrB+idx+70)); // Pixel 1, ic=(32,63), row n+1
      buff[0] = aie::max(buff[0],buff[2]); // best pixels row n    ic=( 0,31)
      buff[1] = aie::max(buff[1],buff[3]); // best pixels row n    ic=(32,63)
      buff[2] = aie::max(buff[4],buff[6]); // best pixels row n+1  ic=( 0,31)
      buff[3] = aie::max(buff[5],buff[7]); // best pixels row n+1  ic=(32,63)
      buff[4] = aie::max(buff[0],buff[2]); // best pixels          oc=( 0,31)
      buff[5] = aie::max(buff[1],buff[3]); // best pixels          oc=(32,63)
      // Corresponds to 1 output row, 1 output pixel for 64 output layers:
      *itw++ = buff[4].extract<16>(0);
      *itw++ = buff[4].extract<16>(1);
      *itw++ = buff[5].extract<16>(0);
      *itw++ = buff[5].extract<16>(1);
      idx += 8; // Advance by 2 input pixels
    } // pp
    off = off + 128; // Advance by 2 input rows
    // The 'chess_separator_scheduler()' below avoid a back-end compiler scheduling issue that introduces
    // functional errors in the output. Filed CRVO-12023. This temporary workaround may have a
    // throughput impact.
    chess_separator_scheduler();
  } // rr
}


