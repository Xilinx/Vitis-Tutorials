//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_dma_snk.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace pfa1008_dma_snk;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void pfa1008_dma_snk_wrapper( TT_DATA mem[NSTREAM*DEPTH], int loop_sel, int loop_cnt,
                                     TT_STREAM sig_i[NSTREAM] );

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4_g[NSTREAM*DEPTH];
  TT_STREAM sig_i[NSTREAM];

  // Configure the same as 'host.cpp' for top level application:
  unsigned word_count = 8*1008;

  // Load stimulus (in order produced by DFT):
  std::mt19937 engine(std::random_device{}());
  std::independent_bits_engine<std::mt19937, 32, unsigned> bit_engine(engine);
  bit_engine.seed(1);
  TT_SAMPLE val0[4];
  TT_SAMPLE val1[4];
  for (unsigned ww=0,mm=0; ww < word_count; ww++) {
    TT_DATA data0;
    TT_DATA data1;
    for (unsigned ii=0; ii < 4; ii++) {
      val0[ii] = TT_SAMPLE(bit_engine());
      val1[ii] = TT_SAMPLE(bit_engine());
    }
    data0 = ( val0[3], val0[2], val0[1], val0[0] );
    data1 = ( val1[3], val1[2], val1[1], val1[0] );
    if (ww >= (word_count-DEPTH)) {
      ddr4_g[mm++] = ( val1[1], val0[1], val1[0], val0[0] );
      ddr4_g[mm++] = ( val1[3], val0[3], val1[2], val0[2] );
    }
    sig_i[0].write( data0 );
    sig_i[1].write( data1 );
  }

  // Run DUT:
  TT_DATA ddr4_o[NSTREAM*DEPTH];
  unsigned cycle_count = pfa1008_dma_snk_wrapper( ddr4_o, word_count, sig_i );
  std::cout << "cycle_count: " << cycle_count << std::endl;

  // Check output streams:
  bool flag = 0;
  for (int mm=0; mm < NSTREAM*DEPTH; mm++) {
    TT_DATA act_g = ddr4_g[mm];
    TT_DATA act_o = ddr4_o[mm];
    bool flag_this = (act_o != act_g);
    flag |= flag_this;
    if (flag_this)
      cout << "mm: " << mm << " act_o: " << act_o << " act_g: " << act_g << endl;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

