//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_dma_src.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace pfa1008_dma_src;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void pfa1008_dma_src_wrapper( TT_DATA* mem, int loop_cnt, TT_STREAM& sig_o );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4[DEPTH];
  TT_DATA sig_g[DEPTH];

  TT_DATA act_o;
  TT_DATA act_g;

  // Test with incrementing sequence:
  for (int mm=0,dd=0; mm < DEPTH; mm++,dd+=4) {
   ddr4[mm]  = ( TT_SAMPLE(dd+3), TT_SAMPLE(dd+2), TT_SAMPLE(dd+1), TT_SAMPLE(dd+0) );
   sig_g[mm] = ( TT_SAMPLE(dd+3), TT_SAMPLE(dd+2), TT_SAMPLE(dd+1), TT_SAMPLE(dd+0) );
  }

  // Run DUT:
  int loop_cnt = 1;
  TT_STREAM sig_o;
  pfa1008_dma_src_wrapper( ddr4, loop_cnt, sig_o );

  // Check output streams:
  bool flag = 0;
  for (int qq=0; qq < DEPTH; qq++) {
    act_o = sig_o.read();
    act_g = sig_g[qq];
    flag |= (act_o != act_g);
    cout << " qq: " << qq << " act_o: " << act_o << " act_g: " << act_g << endl;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

