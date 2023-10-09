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

extern void pfa1008_dma_src_wrapper( TT_DATA* mem, int loop_cnt, TT_STREAM sig_o[NSTREAM] );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4[NSTREAM*DEPTH];
  TT_DATA sig_g[NSTREAM][DEPTH];

  TT_DATA act_o;
  TT_DATA act_g;

  // Test with incrementing sequence:
  for (int mm=0,dd=0; mm < 4*NSTREAM*DEPTH; mm+=8,dd++) {
    ddr4[2*dd+0] = ( TT_SAMPLE(mm+3), TT_SAMPLE(mm+2), TT_SAMPLE(mm+1), TT_SAMPLE(mm+0) );
    ddr4[2*dd+1] = ( TT_SAMPLE(mm+7), TT_SAMPLE(mm+6), TT_SAMPLE(mm+5), TT_SAMPLE(mm+4) );
    sig_g[0][dd] = ( TT_SAMPLE(mm+6), TT_SAMPLE(mm+4), TT_SAMPLE(mm+2), TT_SAMPLE(mm+0) );
    sig_g[1][dd] = ( TT_SAMPLE(mm+7), TT_SAMPLE(mm+5), TT_SAMPLE(mm+3), TT_SAMPLE(mm+1) );
  }

  // Run DUT:
  int loop_cnt = 1;
  TT_STREAM sig_o[NSTREAM];
  pfa1008_dma_src_wrapper( ddr4, loop_cnt, sig_o );

  for (int ss=0; ss < NSTREAM; ss++) {
    cout << "Stream" << ss << " Size: " << sig_o[ss].size() << endl;
  }

  // Check output streams:
  bool flag = 0;
  for (int qq=0; qq < DEPTH; qq++) {
    for (int pp=0; pp < NSTREAM; pp++) {
      act_o = sig_o[pp].read();
      act_g = sig_g[pp][qq];
      flag |= (act_o != act_g);
      cout << "pp: " << pp << " qq: " << qq << " act_o: " << act_o << " act_g: " << act_g << endl;
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

