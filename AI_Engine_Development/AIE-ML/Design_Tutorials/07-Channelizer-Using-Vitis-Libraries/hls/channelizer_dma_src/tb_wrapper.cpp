//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "channelizer_dma_src.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace channelizer_dma_src;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  // Create DDR4 contents:
  TT_DATA ddr4[DEPTH*NSTREAM];
  TT_SAMPLE sig_g[NSTREAM][DEPTH*4];

  // Test with incrementing sequence:
  std::ifstream ss_seq;
  ss_seq.open("seq_i.txt",std::ifstream::in);
  if ( ss_seq.is_open() == 0 ) { std::cerr << "Cannot open data/seq_i.txt" << std::endl; exit(1); }
  for (unsigned ss=0; ss < DEPTH*NSTREAM; ss++) {
    int d0, d1, d2, d3;
    ss_seq >> d0;
    ss_seq >> d1;
    ss_seq >> d2;
    ss_seq >> d3;
    ddr4[ss] = ( TT_SAMPLE(d3), TT_SAMPLE(d2), TT_SAMPLE(d1), TT_SAMPLE(d0) );
  }
  ss_seq.close();

  // Compute golden data:
  for (int ss=0; ss < NSTREAM; ss++) {
    std::string fname = "stream"+std::to_string(ss)+"_o.txt";
    std::ifstream ss_i(fname,std::ifstream::in);
    if ( ss_i.is_open() == 0 ) { std::cerr << "Cannot open data/" << fname << std::endl; exit(1); }
      for (int cc=0; cc < DEPTH*4; cc++) {
        int d0;
        ss_i >> d0;
        sig_g[ss][cc] = TT_SAMPLE(d0);
      }
    ss_i.close();
  }

  // Run DUT:
  int loop_cnt = 1;
  TT_STREAM sig_o[NSTREAM];
  channelizer_dma_src_wrapper( ddr4, loop_cnt, sig_o );

  for (int ss=0; ss < NSTREAM; ss++) {
    cout << "Stream" << ss << " Size: " << sig_o[ss].size() << endl;
  }

  // Check output streams:
  bool flag = 0;
  for (int ss=0; ss < NSTREAM; ss++) {
      for (int dd=0; dd < DEPTH*4; dd+=4) {
        TT_SAMPLE act0_o, act1_o, act2_o, act3_o;
        (act3_o,act2_o,act1_o,act0_o) = sig_o[ss].read();
        TT_SAMPLE act0_g = sig_g[ss][dd  ];
        TT_SAMPLE act1_g = sig_g[ss][dd+1];
        TT_SAMPLE act2_g = sig_g[ss][dd+2];
        TT_SAMPLE act3_g = sig_g[ss][dd+3];
        bool this_flag = (act0_o != act0_g) || (act1_o != act1_g) || (act2_o != act2_g) || (act3_o != act3_g);
        flag |= this_flag;
        if ( this_flag == 1 ) {
          cout << "ss: " << ss << " dd: " << dd+0 << " act_o: " << act0_o << " act_g: " << act0_g << endl;
          cout << "ss: " << ss << " dd: " << dd+1 << " act_o: " << act1_o << " act_g: " << act1_g << endl;
          cout << "ss: " << ss << " dd: " << dd+2 << " act_o: " << act2_o << " act_g: " << act2_g << endl;
          cout << "ss: " << ss << " dd: " << dd+3 << " act_o: " << act3_o << " act_g: " << act3_g << endl;
        }
      }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}