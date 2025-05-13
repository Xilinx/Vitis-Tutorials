//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "diff3dsq.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

diff3dsq::diff3dsq( float (&coord_i)[3*NPULSE] ) : coord(coord_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  pulse = 0;
  count = 0;
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void diff3dsq::run( input_buffer<TT_DATA>& sig_i, output_buffer<TT_DATA>& sig_o )
{
  auto itr0 = aie::begin_restrict_vector<8>(sig_i); // Input (x,y,z) is interleaved
  auto itr1 = aie::begin_restrict_vector<8>(sig_i);  itr1 += 1;
  auto itr2 = aie::begin_restrict_vector<8>(sig_i);  itr2 += 2;
  auto itw  = aie::begin_vector<8>(sig_o);

  // Load current aperture coordinates:
  aie::vector<TT_DATA,8> AntX = aie::broadcast<TT_DATA,8>(coord[pulse         ]);
  aie::vector<TT_DATA,8> AntY = aie::broadcast<TT_DATA,8>(coord[pulse+  NPULSE]);
  aie::vector<TT_DATA,8> AntZ = aie::broadcast<TT_DATA,8>(coord[pulse+2*NPULSE]);
  std::array<aie::accum<TT_ACC,8>,4> acc;

  // Update state:
  if ( count == NSAMP-1 ) {
    pulse = (pulse == NPULSE-1) ? 0 : pulse+1; // Advance to next pulse
    count = 0;
  }
  else {
    count++;
  }

  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
      chess_unroll_loop(4)
  {
    acc[0] = aie::mul_square(       aie::sub(AntX,*itr0));  itr0 += 3;
    acc[1] = aie::mac_square(acc[0],aie::sub(AntY,*itr1));  itr1 += 3;
    acc[2] = aie::mac_square(acc[1],aie::sub(AntZ,*itr2));  itr2 += 3;
    *itw++ = acc[2].to_vector<TT_DATA>();
  } // rr
}


