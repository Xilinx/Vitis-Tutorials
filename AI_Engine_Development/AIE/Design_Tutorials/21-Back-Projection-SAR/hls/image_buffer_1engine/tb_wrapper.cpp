//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "image_buffer_1engine.h"
#include <iostream>

using namespace std;
using namespace image_buffer_1engine;

// Declare these here or the stack bombs out:
TT_DATA ddr4_gld[DEPTH] = {0};
TT_DATA ddr4_act[DEPTH] = {0};

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  static constexpr unsigned NPULSE_USE = 2;
  // Test with incrementing sequence:
  TT_STREAM aie_to_buff;

  TT_DATA prev = TT_DATA(0);
  for (unsigned pp=0; pp < NPULSE_USE; pp++) {
    TT_DATA val_i = prev + TT_DATA(1);
    for (unsigned dd=0; dd < DEPTH; dd++) {
      TT_DATA val_u = val_i + TT_DATA(dd);
      aie_to_buff.write(val_u);
      if (pp == NPULSE_USE-1) ddr4_gld[dd] = val_u;
    }
  }
  cout << "Stream 'aie_to_buff' size before kernel: "<< aie_to_buff.size() << endl;

  // Run DUT:
  TT_STREAM buff_to_aie;
  image_buffer_1engine_wrapper( ddr4_act, NPULSE_USE, buff_to_aie, aie_to_buff );
  cout << "Stream 'aie_to_buff' size after kernel: "<< aie_to_buff.size() << endl;
  cout << "Stream 'buff_to_aie' size after kernel: "<< buff_to_aie.size() << endl;

  // Check DDR contents:
  bool flag = 0;
  for (unsigned dd=0; dd < DEPTH; dd++) {
    TT_DATA act_o = ddr4_act[dd];
    TT_DATA gld_o = ddr4_gld[dd];
    bool this_flag = ( act_o != gld_o );
    if ( this_flag ==1 && dd == 0 )
      cout << "gld_o: " << gld_o << "  act_o: " << act_o << endl;
    flag |= this_flag;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

