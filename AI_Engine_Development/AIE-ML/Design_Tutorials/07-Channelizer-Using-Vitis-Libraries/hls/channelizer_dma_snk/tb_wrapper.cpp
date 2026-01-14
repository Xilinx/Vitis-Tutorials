//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "channelizer_dma_snk.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace channelizer_dma_snk;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4_g[NSTREAM*DEPTH];
  TT_STREAM sig_i[NSTREAM];

  // Configure the same as 'host.cpp' for top level application:
  int loop_cnt = 8;
  int loop_sel = 1;

  // Load stimulus
  std::minstd_rand gen;
  for (int ll=0; ll < loop_cnt; ll++) {
    for (int mm=0,ss=0; mm < NSTREAM*DEPTH; mm+=4) {
      TT_DATA data0 = ( TT_SAMPLE(gen()),TT_SAMPLE(gen()) );
      TT_DATA data1 = ( TT_SAMPLE(gen()),TT_SAMPLE(gen()) );
      TT_DATA data2 = ( TT_SAMPLE(gen()),TT_SAMPLE(gen()) );
      TT_DATA data3 = ( TT_SAMPLE(gen()),TT_SAMPLE(gen()) );
      TT_SAMPLE val0[2], val1[2],val2[2], val3[2];
      ( val0[1], val0[0] ) = data0;
      ( val1[1], val1[0] ) = data1;
      ( val2[1], val2[0] ) = data2;
      ( val3[1], val3[0] ) = data3;
      if ( ll == loop_sel ) {
        ddr4_g[mm  ] = ( val1[0], val0[0] );
        ddr4_g[mm+1] = ( val3[0], val2[0] );
        ddr4_g[mm+2] = ( val1[1], val0[1] );
        ddr4_g[mm+3] = ( val3[1], val2[1] );
      }
      sig_i[0].write( data0 );
      sig_i[1].write( data1 );
      sig_i[2].write( data2 );
      sig_i[3].write( data3 );
    }
  }

  // Run DUT:
  TT_DATA ddr4_o[NSTREAM*DEPTH];
  channelizer_dma_snk_wrapper( ddr4_o, loop_sel, loop_cnt, sig_i );

  // Check output streams:
  bool flag = 0;
  for (int mm=0; mm < NSTREAM*DEPTH; mm++) {
    TT_DATA act_g = ddr4_g[mm];
    TT_DATA act_o = ddr4_o[mm];
    flag |= (act_o != act_g);
    if ( flag == 1 ) {
      cout << "mm: " << mm << " act_o: " << act_o << " act_g: " << act_g << endl;
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}
