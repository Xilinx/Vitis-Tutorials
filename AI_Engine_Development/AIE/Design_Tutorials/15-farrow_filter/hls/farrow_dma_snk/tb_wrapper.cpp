//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "farrow_dma_snk.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace farrow_dma_snk;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void farrow_dma_snk_wrapper( TT_DATA mem[DEPTH], int loop_sel, int loop_cnt,
                                     TT_STREAM& sig_i );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4_g[DEPTH];
  TT_STREAM sig_i;

  // Configure the same as 'host.cpp' for top level application:
  int loop_cnt = 4;
  int loop_sel = 1;

  // Load stimulus:
  std::minstd_rand gen;
  for (int ll=0; ll < loop_cnt; ll++) {
    for (int mm=0,ss=0; mm < DEPTH; mm++) {
      TT_DATA data = TT_DATA( gen() );
      TT_SAMPLE val[4];
      ( val[3], val[2], val[1], val[0] ) = data;
      if ( ll == loop_sel ) {
        ddr4_g[mm] = ( val[3], val[2], val[1], val[0] );
      }
      sig_i.write( data );
    }
  }

  // Run DUT:
  TT_DATA ddr4_o[DEPTH];
  farrow_dma_snk_wrapper( ddr4_o, loop_sel, loop_cnt, sig_i );

  // Check output streams:
  bool flag = 0;
  for (int mm=0; mm < DEPTH; mm++) {
    TT_DATA act_g = ddr4_g[mm];
    TT_DATA act_o = ddr4_o[mm];
    flag |= (act_o != act_g);
    cout << "mm: " << mm << " act_o: " << act_o << " act_g: " << act_g << endl;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}
