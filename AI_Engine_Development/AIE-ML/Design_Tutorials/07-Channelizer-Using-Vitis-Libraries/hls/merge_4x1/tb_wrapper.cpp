//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "merge_4x1.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace merge_4x1;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  static constexpr int NSAMP = 512; // Must be a multiple of 8
  TT_STREAM sig_g;
  TT_STREAM sig_i[NSTREAM_I];

  // Test with incrementing sequence:
  for (unsigned ss=0; ss < NSAMP; ss+=2*NSTREAM_I) {
    for (unsigned dd=0; dd < NSTREAM_I; dd++) {
      sig_g.write( ( TT_SAMPLE(ss+2*dd+1), TT_SAMPLE(ss+2*dd) ) );
      sig_i[dd].write( ( TT_SAMPLE(ss+dd+NSTREAM_I), TT_SAMPLE(ss+dd) ) );
    }
  }
  cout << "Gold Stream Size: " << sig_g.size() << endl;
  for (unsigned dd=0; dd < NSTREAM_I; dd++)
    cout << "Input Stream " << dd << " Size: " << sig_i[dd].size() << endl;

  // Run DUT:
  TT_STREAM sig_o;
  for (unsigned ss=0; ss < NSAMP; ss += 2*NSTREAM_I) {
    merge_4x1_wrapper( sig_i, sig_o );
  }

  cout << "Output Stream Size: " << sig_o.size() << endl;
  for (unsigned dd=0; dd < NSTREAM_I; dd++)
    cout << "Input Stream " << dd << " Size: " << sig_i[dd].size() << endl;

  // Check output stream:
  bool flag = 0;
  for (unsigned ss=0; ss < NSAMP; ss+=2) {
    TT_DATA act_o = sig_o.read();
    TT_DATA gld_o = sig_g.read();
    bool this_flag = (act_o != gld_o);
    flag |= this_flag;
    if ( this_flag == 1 ) {
      cout << "ss: " << ss << " act_o: " << act_o << " gld_o: " << gld_o << endl;
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}
