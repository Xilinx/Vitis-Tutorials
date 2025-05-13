//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "dR_comp.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
dR_comp<REPEAT,NSAMP>::dR_comp(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  pulse = 0;
  count = 0;
}

// ------------------------------------------------------------
// Run
//
// Scheduling: minII = 26  actII = 50  for 32 samples
// Throughput  2745 MB/sec or ~680 Msps
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
inline __attribute__((noinline))
void dR_comp<REPEAT,NSAMP>::run( input_buffer<float>& sig_i, const float (&R0_range)[NPULSE],
                                 output_buffer<float>& domain_o, output_buffer<float>& theta_o )
{
  // Load current R0 range:
  aie::vector<float,8> R0_vec = aie::broadcast<float,8>(R0_range[pulse]);

  // Update state:
  if ( count == REPEAT-1 ) {
    pulse = (pulse == NPULSE-1) ? 0 : pulse+1; // Advance to next pulse
    count = 0;
  }
  else {
    count++;
  }

  auto itr  = aie::begin_restrict_vector<8>(sig_i);
  auto itw0 = aie::begin_restrict_vector<8>(domain_o);
  auto itw1 = aie::begin_restrict_vector<8>(theta_o);
  aie::vector<float,8> dR;
  aie::accum<accfloat,8> acc;     acc.from_vector<float>(aie::broadcast<float,8>(0.5));

  for (unsigned rr=0; rr < NSAMP/8; rr++)
    chess_prepare_for_pipelining
      chess_unroll_loop(4)
  {
    dR = aie::sub(aie::mul(*itr++,aie::broadcast<float,8>(sqrt_expand)),R0_vec);
    *itw0++ = aie::mac(acc,dR,aie::broadcast<float,8>(scale_domain));
    *itw1++ = aie::mul(dR,aie::broadcast<float,8>(scale_theta));
  } // rr

}


