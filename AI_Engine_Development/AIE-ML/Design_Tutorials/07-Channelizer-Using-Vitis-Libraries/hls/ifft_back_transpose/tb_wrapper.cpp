//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "ifft_back_transpose.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  using TT_STREAM = backTransposeCls<POINT_SIZE, SSR, VSS_MODE>::TT_STREAM;
  using TT_SAMPLE = backTransposeCls<POINT_SIZE, SSR, VSS_MODE>::TT_SAMPLE;

  // Create DDR4 contents:
  TT_STREAM sig_i[SSR];
  TT_SAMPLE sig_g[SSR][POINT_SIZE/SSR*2];

  // Load stream stimulus:
  for (int ss=0; ss < SSR; ss++) {
    std::ifstream ss_i;
    ss_i.open("stream" + std::to_string(ss) + "_i.txt",std::ifstream::in);
    for (int dd=0; dd < POINT_SIZE/SSR; dd+=2) {
      int d0,d1;
      ss_i >> d0;
      ss_i >> d1;
      sig_i[ss].write( ( TT_SAMPLE(d1), TT_SAMPLE(d0) ) );
    }
    ss_i.close();
  }

  // Load golden outputs:
  for (int ss=0; ss < SSR; ss++) {
    std::ifstream ss_i;
    ss_i.open("stream" + std::to_string(ss) + "_o.txt",std::ifstream::in);
    for (int dd=0; dd < POINT_SIZE*2/SSR; dd++) {
      int d0;
      ss_i >> d0;
      sig_g[ss][dd] = TT_SAMPLE(d0);
    }
    ss_i.close();
  }

  for (int ss=0; ss < SSR; ss++) {
    cout << "Stream sig_i" << ss << " Size: " << sig_i[ss].size() << endl;
  }

  // Run DUT:
  TT_STREAM sig_o[SSR];
  ifft_back_transpose_wrapper( sig_i, sig_o );

  for (int ss=0; ss < SSR; ss++) {
    cout << "Stream sig_o" << ss << " Size: " << sig_o[ss].size() << endl;
  }

  // Check output streams:
  bool flag = 0;
  for (int ss=0; ss < SSR; ss++) {
    for (int dd=0; dd < POINT_SIZE/SSR; dd+=2) {
      TT_SAMPLE act0_o, act1_o;
      (act1_o,act0_o) = sig_o[ss].read();
      TT_SAMPLE act0_g, act1_g;
      act0_g = sig_g[ss][dd];
      act1_g = sig_g[ss][dd+1];
      bool this_flag = (act0_o != act0_g) || (act1_o != act1_g);
      flag |= this_flag;
      if ( this_flag ) {
        cout << "ss: " << ss << " dd: " << dd << " act0_o: " << act0_o << " act0_g: " << act0_g << endl;
        cout << "ss: " << ss << " dd: " << dd+1 << " act1_o: " << act1_o << " act1_g: " << act1_g << endl;
      }
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

