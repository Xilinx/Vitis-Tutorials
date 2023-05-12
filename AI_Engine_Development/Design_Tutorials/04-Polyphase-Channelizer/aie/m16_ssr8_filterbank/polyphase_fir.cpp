//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "polyphase_fir.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned PATTERN,unsigned NSAMP>
polyphase_fir<PATTERN,NSAMP>::polyphase_fir( TT_COEFF (&taps0_i)[NUM_POLY*TAPS_PER_PHASE],
                                             TT_COEFF (&taps1_i)[NUM_POLY*TAPS_PER_PHASE] )
: taps0( taps0_i ), taps1( taps1_i ),
  state0(aie::zeros<TT_DATA,TAPS_PER_PHASE>()), state1(aie::zeros<TT_DATA,TAPS_PER_PHASE>())
{
  aie::set_rounding(aie::rounding_mode::positive_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Intrinsic Version
// ------------------------------------------------------------

template<unsigned PATTERN,unsigned NSAMP>
void polyphase_fir<PATTERN,NSAMP>::run_i( input_stream<TT_DATA>* __restrict sig_i,
                                          output_stream<TT_DATA>* __restrict sig_o )
{
  // Declare buffers:
  v16cint16 buff0  = null_v16cint16();
  v16cint16 buff1  = null_v16cint16();
  v16int16  coeff0 = null_v16int16();
  v16int16  coeff1 = null_v16int16();
  v4cacc48  acc0   = null_v4cacc48();
  v4cacc48  acc1   = null_v4cacc48();
  // Initialize state:
  // Pointers to tap weights:
  v16int16* __restrict ptr0 = (v16int16*)taps0;
  v16int16* __restrict ptr1 = (v16int16*)taps1;
  // Pointers to state history:
  v8cint16* __restrict his0 = (v8cint16*)&state0;
  v8cint16* __restrict his1 = (v8cint16*)&state1;

  // Load state history:
  buff0 = upd_w(buff0,0,*his0);
  buff1 = upd_w(buff1,0,*his1);

  // This tile computes two filters (8 samples each) over 16 cycles:
  for ( unsigned ii=0; ii < NSAMP/16; ii++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
  {
    cint16 discard;

    if ( PATTERN == 0 ) {
      REPEATED_D0(buff0,coeff0,ptr0,acc0); // Generate 4 samples for filter A
      REPEATED_D0(buff1,coeff1,ptr1,acc1); // Generate 4 samples for filter B
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);  // Generate 4 samples for filter A
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);  // Generate 4 samples for filter B
    }
    else if ( PATTERN == 1 ) {
      REPEATED_D1(buff0,coeff0,ptr0,acc0);
      REPEATED_D1(buff1,coeff1,ptr1,acc1);
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
    }
    else if ( PATTERN == 2 ) {
      REPEATED_D2(buff0,coeff0,ptr0,acc0);
      REPEATED_D2(buff1,coeff1,ptr1,acc1);
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
    }
    else if ( PATTERN == 3 ) {
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
      HOLDOVER_D0(buff0,coeff0,ptr0,acc0);
      HOLDOVER_D0(buff1,coeff1,ptr1,acc1);
    }
    else if ( PATTERN == 4 ) {
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
      REPEATED_D0(buff0,coeff0,ptr0,acc0);
      REPEATED_D0(buff1,coeff1,ptr1,acc1);
    }
    else if ( PATTERN == 5 ) {
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
      REPEATED_D1(buff0,coeff0,ptr0,acc0);
      REPEATED_D1(buff1,coeff1,ptr1,acc1);
    }
    else if ( PATTERN == 6 ) {
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
      REPEATED_D2(buff0,coeff0,ptr0,acc0);
      REPEATED_D2(buff1,coeff1,ptr1,acc1);
    }
    else if ( PATTERN == 7 ) {
      HOLDOVER_D0(buff0,coeff0,ptr0,acc0);
      HOLDOVER_D0(buff1,coeff1,ptr1,acc1);
      NORMAL_FIR(buff0,coeff0,ptr0,acc0);
      NORMAL_FIR(buff1,coeff1,ptr1,acc1);
    }
    RESET_COEFF;
  }
  // Store state history:
  *his0 = ext_w(buff0,0);
  *his1 = ext_w(buff1,0);

}

