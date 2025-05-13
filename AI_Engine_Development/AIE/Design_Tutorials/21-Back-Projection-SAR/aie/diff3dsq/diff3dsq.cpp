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

template<unsigned REPEAT,unsigned NSAMP>
diff3dsq<REPEAT,NSAMP>::diff3dsq(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  pulse = 0;
  count = 0;
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
inline __attribute__((noinline))
void diff3dsq<REPEAT,NSAMP>::run( input_buffer<TT_DATA>& sig_i,
                                  const TT_DATA (&coord)[NCOORD], output_buffer<TT_DATA>& sig_o )
{
  auto itr0 = aie::begin_restrict_vector<8>(sig_i); // Input (x,y,z) is interleaved
  auto itr1 = aie::begin_restrict_vector<8>(sig_i);  itr1 += 1;
  auto itr2 = aie::begin_restrict_vector<8>(sig_i);  itr2 += 2;
  auto itw  = aie::begin_vector<8>(sig_o);

  // Load current aperture coordinates:
  aie::vector<TT_DATA,8> AntX = aie::broadcast<TT_DATA,8>(coord[pulse         ]);
  aie::vector<TT_DATA,8> AntY = aie::broadcast<TT_DATA,8>(coord[pulse+  NPULSE]);
  aie::vector<TT_DATA,8> AntZ = aie::broadcast<TT_DATA,8>(coord[pulse+2*NPULSE]);
  std::array<aie::accum<TT_ACC,8>,8> acc;

  // Update state:
  if ( count == REPEAT-1 ) {
    pulse = (pulse == NPULSE-1) ? 0 : pulse+1; // Advance to next pulse
    count = 0;
  }
  else {
    count++;
  }

  for (unsigned rr=0; rr < NSAMP/16; rr++)
    chess_prepare_for_pipelining
  {
    acc[0] = aie::mul_square(       aie::sub(AntX,*itr0));  itr0 += 3;
    acc[1] = aie::mac_square(acc[0],aie::sub(AntY,*itr1));  itr1 += 3;
    acc[2] = aie::mac_square(acc[1],aie::sub(AntZ,*itr2));  itr2 += 3;
    acc[3] = aie::mul(acc[2].to_vector<TT_DATA>(),aie::broadcast<TT_DATA,8>(sqrt_compress));
    *itw++ = acc[3].to_vector<TT_DATA>();

    acc[4] = aie::mul_square(       aie::sub(AntX,*itr0));  itr0 += 3;
    acc[5] = aie::mac_square(acc[4],aie::sub(AntY,*itr1));  itr1 += 3;
    acc[6] = aie::mac_square(acc[5],aie::sub(AntZ,*itr2));  itr2 += 3;
    acc[7] = aie::mul(acc[6].to_vector<TT_DATA>(),aie::broadcast<TT_DATA,8>(sqrt_compress));
    *itw++ = acc[7].to_vector<TT_DATA>();
  } // rr
}


