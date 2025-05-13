//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "range_gen.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP,unsigned ID>
range_gen<REPEAT,NSAMP,ID>::range_gen(void)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP,unsigned ID>
inline __attribute__((noinline))
void range_gen<REPEAT,NSAMP,ID>::run( output_stream<TT_DATA>* sig_o )
{
  aie::vector<TT_DATA,8> xvec = aie::broadcast<TT_DATA,8>(X_INIT);
  aie::vector<TT_DATA,8> yinit = aie::load_v<8>(Y_INIT);
  aie::vector<TT_DATA,8> yvec = yinit;

  for (unsigned rr=0,ss=0; rr < TOT_SAMP/8; rr++)
    chess_prepare_for_pipelining
  {
    writeincr(sig_o, xvec);
    writeincr(sig_o, yvec);
    writeincr(sig_o, aie::zeros<TT_DATA,8>());
    yvec = aie::sub(yvec,aie::broadcast<TT_DATA,8>(RANGE_STEP)); // Decrement Y
    if ( ss == (NPIXEL/8-1) ) {
      xvec = aie::add(xvec,aie::broadcast<TT_DATA,8>(RANGE_INC)); // Increment X
      yvec = yinit;                                               // Reset Y
      ss = 0;
    }
    else ss++;
  } // rr
}


