//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "split_1x16.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace split_1x16;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  static constexpr int NSAMP = 1024; // Must be a multiple of 64
  TT_STREAM sig_i;
  TT_STREAM sig_g[NSTREAM_O];

  // Test with incrementing sequence:
  for (unsigned ss=0; ss < NSAMP; ss+=2*NSTREAM_O) {
    for (unsigned dd=0; dd < NSTREAM_O; dd++) {
      sig_i.write( ( TT_SAMPLE(ss+4*dd+3), TT_SAMPLE(ss+4*dd+2), TT_SAMPLE(ss+4*dd+1), TT_SAMPLE(ss+4*dd) ) );
      sig_g[dd].write( ( TT_SAMPLE(ss+dd+3*NSTREAM_O), TT_SAMPLE(ss+dd+2*NSTREAM_O), TT_SAMPLE(ss+dd+NSTREAM_O), TT_SAMPLE(ss+dd) ) );
    }
  }
  cout << "Input Stream Size: " << sig_i.size() << endl;
  for (unsigned dd=0; dd < NSTREAM_O; dd++)
    cout << "Gold Stream " << dd << " Size: " << sig_g[dd].size() << endl;

  // Run DUT:
  TT_STREAM sig_o[NSTREAM_O];
  for (unsigned ss=0; ss < NSAMP; ss += 2*NSTREAM_O) {
    split_1x16_wrapper( sig_i, sig_o );
  }

  cout << "Input Stream Size: " << sig_i.size() << endl;
  for (unsigned dd=0; dd < NSTREAM_O; dd++)
    cout << "Output Stream " << dd << " Size: " << sig_o[dd].size() << endl;

  // Check output streams:
  bool flag = 0;
  for (unsigned ss=0; ss < NSAMP; ss+=2*NSTREAM_O) {
    for (unsigned dd=0; dd < NSTREAM_O; dd++) {
      TT_DATA act_o = sig_o[dd].read();
      TT_DATA gld_o = sig_g[dd].read();
      bool this_flag = (act_o != gld_o);
      flag |= this_flag;
      if ( this_flag == 1 ) {
        cout << "ss: " << ss << " dd: " << dd << " act_o: " << act_o << " gld_o: " << gld_o << endl;
      }
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}
