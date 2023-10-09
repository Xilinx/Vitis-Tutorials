//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dma_stream_src.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace dma_stream_src;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void dma_stream_src_wrapper( TT_DATA* mem, int loop_cnt, TT_STREAM sig_o[NSTREAM] );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4[NSTREAM*DEPTH];
  TT_DATA sig_g[NSTREAM][DEPTH];

  TT_DATA act_o;
  TT_DATA act_g;

  // minstd_rand gen_rnd;
  for (int mm=0,ss=0,dd=0; mm < NSTREAM*DEPTH; mm++) {
    ddr4[mm] = TT_DATA(mm);
    sig_g[ss][dd] = TT_DATA(mm);
    if ( ss == NSTREAM-1 ) {
      ss = 0;
      dd = ( dd == DEPTH-1 ) ? 0 : dd + 1;
    }
    else {
      ss = ss + 1;
    }
  }

  // Run DUT:
  int loop_cnt = 1;
  TT_STREAM sig_o[NSTREAM];
  dma_stream_src_wrapper( ddr4, loop_cnt, sig_o );

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

