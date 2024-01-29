//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft_transpose.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace ifft_transpose;

static constexpr int NTRANSFORM = 2;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  TT_STREAM sig_i[NSTREAM];
  TT_STREAM sig_o[NSTREAM];
  TT_STREAM sig_g[NSTREAM];

  // Load input streams from files:
  for (int ss=0; ss < NSTREAM; ss++) {
    std::string fname = "stream"+std::to_string(ss)+"_i.txt";
    std::ifstream ss_i(fname,std::ifstream::in);
    if ( ss_i.is_open() == 0 ) { std::cerr << "Cannot open data/" << fname << std::endl; exit(1); }
    for (int tt=0; tt < NTRANSFORM; tt++) {
      for (int rr=0; rr < NROW; rr++) {
        for (int cc=0; cc < DEPTH; cc+=2) {
          int d0, d1;
          ss_i >> d0;
          ss_i >> d1;
          sig_i[ss].write( ( TT_SAMPLE(d1), TT_SAMPLE(d0) ) );
        }
      }
    }
    ss_i.close();
  }

  // Write zeros to manage startup latency of 2 cycles:
  int LATENCY = DEPTH*NROW/2;
  for (int ss=0; ss < NSTREAM; ss++) {
    for (int xx=0; xx < LATENCY; xx++) {
      sig_i[ss].write( TT_DATA(0) );
    }
  }

  for (int ss=0; ss < NSTREAM; ss++) {
    cout << "Stream-I" << ss << " Size: " << sig_i[ss].size() << endl;
  }

  // Load output streams from files:
  for (int ss=0; ss < NSTREAM; ss++) {
    std::string fname = "stream"+std::to_string(ss)+"_o.txt";
    std::ifstream ss_o(fname,std::ifstream::in);
    if ( ss_o.is_open() == 0 ) { std::cerr << "Cannot open data/" << fname << std::endl; exit(1); }
    for (int tt=0; tt < NTRANSFORM; tt++) {
      for (int rr=0; rr < NROW; rr++) {
        for (int cc=0; cc < DEPTH; cc+=2) {
          int d0, d1;
          ss_o >> d0;
          ss_o >> d1;
          sig_g[ss].write( ( TT_SAMPLE(d1), TT_SAMPLE(d0) ) );
        }
      }
    }
    ss_o.close();
  }

  for (int ss=0; ss < NSTREAM; ss++) {
    cout << "Stream-G" << ss << " Size: " << sig_g[ss].size() << endl;
  }

  // Run DUT (including extra cycles for latency):
  for (int ss=0; ss < NTRANSFORM*DEPTH*NROW/2+LATENCY; ss++) {
    ifft_transpose_wrapper( sig_i, sig_o );
  }

  for (int ss=0; ss < NSTREAM; ss++) {
    cout << "Stream" << ss << " Size: " << sig_o[ss].size() << endl;
  }

  // Check output streams:
  bool flag = 0;
  for (int xx=0; xx < NTRANSFORM*DEPTH*DEPTH/NSTREAM; xx+=2) {
    for (int ss=0; ss < NSTREAM; ss++) {
      TT_DATA gld_o = sig_g[ss].read();
      TT_DATA act_o = sig_o[ss].read();
      bool this_flag = ( act_o != gld_o );
      flag |= this_flag;
      if ( this_flag == 1 )
        cout << "xx: " << xx << "  gld_o: " << gld_o << "   act_o: " << act_o << endl;
    }
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

