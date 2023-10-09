//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dma_stream_snk.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace dma_stream_snk;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void dma_stream_snk_wrapper( TT_DATA mem[NSTREAM*DEPTH], int loop_sel, int loop_cnt, int dft_perm, TT_STREAM sig_i[NSTREAM] );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4_g[NSTREAM*DEPTH];
  TT_STREAM sig_i[NSTREAM];

  // Configure the same as 'host.cpp' for top level application:
  int loop_cnt = 4;
  int loop_sel = 1;
  int dft_perm = 1;

  // Load stimulus (in order produced by DFT):
  std::minstd_rand gen;
  for (int ll=0; ll < loop_cnt; ll++) {
    for (int mm=0,ss=0; mm < NSTREAM*DEPTH; mm++) {
      TT_DATA val = TT_DATA( gen() );
      if ( ll == loop_sel ) {
        ddr4_g[mm] = val;
      }
      sig_i[ss].write( val );
      if ( dft_perm == 1 ) {
        if      ( ss == NSTREAM-2 ) ss = 1;
        else if ( ss == NSTREAM-1 ) ss = 0;
        else                        ss = ss + 2;
      }
      else {
        if ( ss == NSTREAM-1 ) ss = 0;
        else                   ss = ss + 1;
      }
    }
  }

  // Run DUT:
  TT_DATA ddr4_o[NSTREAM*DEPTH];
  dma_stream_snk_wrapper( ddr4_o, loop_sel, loop_cnt, dft_perm, sig_i );

  // Check output streams:
  bool flag = 0;
  for (int mm=0; mm < NSTREAM*DEPTH; mm++) {
    TT_DATA act_g = ddr4_g[mm];
    TT_DATA act_o = ddr4_o[mm];
    flag |= (act_o != act_g);
    cout << "mm: " << mm << " act_o: " << act_o << " act_g: " << act_g << endl;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

