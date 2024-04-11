//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "sincos_test.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

sincos_test::sincos_test( void )
{
  aie::set_rounding(aie::rounding_mode::conv_even);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void sincos_test::run( output_stream<TT_PHASE>* restrict phase_o,
                       output_stream<TT_TWID>* restrict twid_o )
{
  for (unsigned ii=0; ii < (1<<NUM_BITS); ii++) {
    unsigned phase = (ii << SHIFT);
    writeincr(phase_o,ii);
    writeincr(twid_o,aie::sincos_complex(phase));
  }
}


