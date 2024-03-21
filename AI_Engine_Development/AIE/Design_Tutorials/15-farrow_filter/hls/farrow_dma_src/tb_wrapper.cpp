//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "farrow_dma_src.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace farrow_dma_src;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void farrow_dma_src_wrapper( TT_DATA* mem, int loop_cnt, TT_STREAM& sig_o );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4[DEPTH];
  TT_DATA sig_g[DEPTH];

  TT_DATA act_sig_o;
  TT_DATA act_sig_g;

  // Test with incrementing sequence:
  for (int mm=0,dd=0; mm < 4*DEPTH; mm+=4,dd++) {
    ddr4[dd] =  ( TT_SAMPLE(mm+3), TT_SAMPLE(mm+2), TT_SAMPLE(mm+1), TT_SAMPLE(mm+0) );
    sig_g[dd] = ( TT_SAMPLE(mm+3), TT_SAMPLE(mm+2), TT_SAMPLE(mm+1), TT_SAMPLE(mm+0) );
  }

  // Run DUT:
  int loop_cnt = 1;
  TT_STREAM sig_o;
  farrow_dma_src_wrapper( ddr4, loop_cnt, sig_o );

  cout << "Stream Size: " << sig_o.size() << endl;

  // Check output streams:
  bool flag = 0;
  for (int qq=0; qq < DEPTH; qq++) {
    act_sig_o = sig_o.read();
    act_sig_g = sig_g[qq];
    flag |= (act_sig_o != act_sig_g);
    cout << " act_sig_o: " << act_sig_o << " act_sig_g: " << act_sig_g << endl;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}