//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "merge_8x4.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace merge_8x4;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  static constexpr int NSAMP = 512; // Must be a multiple of 16
  TT_STREAM sig_g[NSTREAM_O];
  TT_STREAM sig_i[NSTREAM_I];

  // Load stream stimulus:
  for (int ss=0; ss < NSTREAM_I; ss++) {
    std::ifstream ss_i;
    ss_i.open("stream" + std::to_string(ss) + "_i.txt",std::ifstream::in);
    for (int dd=0; dd < NSAMP/NSTREAM_I; dd+=2) {
      int d0,d1;
      ss_i >> d0;
      ss_i >> d1;
      sig_i[ss].write( ( TT_SAMPLE(d1), TT_SAMPLE(d0) ) );
    }
    ss_i.close();
  }

  // Load golden outputs:
  for (int ss=0; ss < NSTREAM_O; ss++) {
    std::ifstream ss_i;
    ss_i.open("stream" + std::to_string(ss) + "_o.txt",std::ifstream::in);
    for (int dd=0; dd < NSAMP/NSTREAM_O; dd+=2) {
      int d0,d1;
      ss_i >> d0;
      ss_i >> d1;
      sig_g[ss].write( ( TT_SAMPLE(d1), TT_SAMPLE(d0) ) );
    }
    ss_i.close();
  }

  for (unsigned dd=0; dd < NSTREAM_I; dd++)
    cout << "Input Stream " << dd << " Size: " << sig_i[dd].size() << endl;
  for (unsigned dd=0; dd < NSTREAM_O; dd++)
    cout << "Gold Stream Size: " << dd << " Size: " << sig_g[dd].size() << endl;

  // Run DUT:
  TT_STREAM sig_o[NSTREAM_O];
  for (unsigned ss=0; ss < NSAMP; ss += 2*NSTREAM_I) {
    merge_8x4_wrapper( sig_i, sig_o );
  }

  for (unsigned dd=0; dd < NSTREAM_O; dd++)
    cout << "Output Stream " << dd << " Size: " << sig_o[dd].size() << endl;

  // Check output stream:
  bool flag = 0;
  for (unsigned dd=0; dd < NSTREAM_O; dd++) {
    for (unsigned ss=0; ss < NSAMP/NSTREAM_O; ss+=2) {
      TT_DATA act_o = sig_o[dd].read();
      TT_DATA gld_o = sig_g[dd].read();
      bool this_flag = (act_o != gld_o);
      flag |= this_flag;
      if ( this_flag == 1 ) {
        cout << "dd: " << dd << " ss: " << ss << " act_o: " << act_o << " gld_o: " << gld_o << endl;
      }
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}
