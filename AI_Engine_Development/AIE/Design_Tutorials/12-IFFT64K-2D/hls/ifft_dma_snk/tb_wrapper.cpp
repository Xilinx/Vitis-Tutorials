//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft_dma_snk.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace ifft_dma_snk;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4_g[NFFT/2];
  TT_STREAM sig_i[NSTREAM];

  // Configure the same as 'host.cpp' for top level application:
  int loop_cnt = 8;
  int loop_sel = 1;

  // Load stream stimulus:
  for (int ll=0; ll < loop_cnt; ll++) {
    for (int ss=0; ss < NSTREAM; ss++) {
      std::ifstream ss_i;
      ss_i.open("stream" + std::to_string(ss) + "_i.txt",std::ifstream::in);
      for (int dd=0; dd < DEPTH*DEPTH/NSTREAM/2; dd++) {
        int d0, d1;
        ss_i >> d0;
        ss_i >> d1;
        sig_i[ss].write( ( TT_SAMPLE(d1), TT_SAMPLE(d0) ) );
      }
      ss_i.close();
    }
  }

  // Load golden outputs:
  std::ifstream ss_i;
  ss_i.open("seq_o.txt",std::ifstream::in);
  for (int dd=0; dd < NFFT/2; dd++) {
    int d0, d1;
    ss_i >> d0;
    ss_i >> d1;
    ddr4_g[dd] = ( TT_SAMPLE(d1), TT_SAMPLE(d0) );
  }
  ss_i.close();

  for (int ss=0; ss < NSTREAM; ss++) {
    cout << "Stream" << ss << " Size: " << sig_i[ss].size() << endl;
  }

  // Run DUT:
  TT_DATA ddr4_o[NFFT/2];
  ifft_dma_snk_wrapper( ddr4_o, loop_sel, loop_cnt, sig_i );

  // Check output streams:
  bool flag = 0;
  for (int ss=0; ss < NFFT/2; ss++) {
    TT_DATA act_o = ddr4_o[ss];
    TT_DATA act_g = ddr4_g[ss];
    bool this_flag = (act_o != act_g);
    flag |= this_flag;
    if ( this_flag ) {
      cout << "ss: " << " act_o: " << act_o << " act_g: " << act_g << endl;
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

