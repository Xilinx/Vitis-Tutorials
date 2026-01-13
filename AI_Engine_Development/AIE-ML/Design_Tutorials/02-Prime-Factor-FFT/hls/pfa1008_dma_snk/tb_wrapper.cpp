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

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4_g[DEPTH];
  TT_STREAM sig_i;

  // Configure the same as 'host.cpp' for top level application:
  unsigned word_count = 8*1008;

  // Load stimulus (in order produced by DFT):
  std::minstd_rand gen;
  for (unsigned ww=0,mm=0; ww < word_count; ww++) {
    TT_DATA data = TT_DATA( gen() );
    if (ww >= (word_count-DEPTH)) {
      ddr4_g[mm++] = data;
    }
    sig_i.write( data );
  }

  // Run DUT:
  TT_DATA ddr4_o[DEPTH];
  unsigned cycle_count = pfa1008_dma_snk_wrapper( ddr4_o, word_count, sig_i );
  std::cout << "cycle_count: " << cycle_count << std::endl;

  // Check output streams:
  bool flag = 0;
  for (int mm=0; mm < DEPTH; mm++) {
    TT_DATA act_g = ddr4_g[mm];
    TT_DATA act_o = ddr4_o[mm];
    bool this_flag = (act_o != act_g);
    flag |= this_flag;
    if (this_flag == 1)
      cout << "mm: " << mm << " act_o: " << act_o << " act_g: " << act_g << endl;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

