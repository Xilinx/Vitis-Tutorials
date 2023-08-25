//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_permute_o.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace pfa1008_permute_o;

static constexpr int NTRANSFORM = 8;

// ------------------------------------------------------------
// Read & format input from files
// ------------------------------------------------------------

TT_AXI4 read_from_files( ifstream& ss )
{
  // Read four "cint16" values from two input streams
  int d_re, d_im;
  ap_int<16> val_re[4], val_im[4];
  ss >> d_re >> d_im;   val_re[0] = ap_int<16>(d_re);   val_im[0] = ap_int<16>(d_im);
  ss >> d_re >> d_im;   val_re[1] = ap_int<16>(d_re);   val_im[1] = ap_int<16>(d_im);
  ss >> d_re >> d_im;   val_re[2] = ap_int<16>(d_re);   val_im[2] = ap_int<16>(d_im);
  ss >> d_re >> d_im;   val_re[3] = ap_int<16>(d_re);   val_im[3] = ap_int<16>(d_im);
  TT_AXI4 val = ( val_im[3].range(15,0), val_re[3].range(15,0),
                  val_im[2].range(15,0), val_re[2].range(15,0),
                  val_im[1].range(15,0), val_re[1].range(15,0),
                  val_im[0].range(15,0), val_re[0].range(15,0) );
  return( val );
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void pfa1008_permute_o_wrapper( TT_STREAM sig_i[SSR_IO], TT_STREAM sig_o[SSR_IO] );

int main( void )
{
  TT_STREAM sig_i[SSR_IO];
  TT_STREAM sig_o[SSR_IO];

  // Load data from files:
  std::ifstream ss0_i, ss1_i;
  ss0_i.open("ss0_i.txt",std::ifstream::in);
  ss1_i.open("ss1_i.txt",std::ifstream::in);
  if ( ss0_i.is_open() == 0 ) cerr << "Cannot open file data/ss0_i.txt" << endl;
  if ( ss1_i.is_open() == 0 ) cerr << "Cannot open file data/ss1_i.txt" << endl;

  // Grab contents from input files:
  for (int xx=0; xx < NTRANSFORM*NFFT/8; xx++) {
    sig_i[0].write( read_from_files( ss0_i ) );
    sig_i[1].write( read_from_files( ss1_i ) );
  }
  // Write zeros to manage startup latency of NFFT/8 cycles:
  int LATENCY = NFFT/8;
  for (int xx=0; xx < LATENCY; xx++) {
    sig_i[0].write( TT_AXI4(0) );
    sig_i[1].write( TT_AXI4(0) );
  }
  ss0_i.close();
  ss1_i.close();
  cout << "*** Parsed DDR4 Data ***" << endl;

  // Run DUT (including extra cycles for latency):
  for (int ss=0; ss < NTRANSFORM*NFFT/8+LATENCY; ss++) {
    pfa1008_permute_o_wrapper( sig_i, sig_o );
  }

  cout << "Done pfa1008_permute_o_wrapper()" << endl;

  for (int ss=0; ss < SSR_IO; ss++) {
    cout << "Stream" << ss << " Size: " << sig_o[ss].size() << endl;
  }

  // Check output streams:
  std::ifstream ss0_o, ss1_o;
  ss0_o.open("ss0_o.txt",std::ifstream::in);
  ss1_o.open("ss1_o.txt",std::ifstream::in);
  if ( ss0_o.is_open() == 0 ) cerr << "Cannot open file data/ss0_o.txt" << endl;
  if ( ss1_o.is_open() == 0 ) cerr << "Cannot open file data/ss1_o.txt" << endl;
  bool flag = 0;
  for (int xx=0; xx < NTRANSFORM*NFFT/8; xx++) {
    TT_AXI4 gld0_o = read_from_files( ss0_o );
    TT_AXI4 gld1_o = read_from_files( ss1_o );
    TT_AXI4 act0_o = sig_o[0].read();
    TT_AXI4 act1_o = sig_o[1].read();
    bool this_flag0 = ( act0_o != gld0_o );
    bool this_flag1 = ( act1_o != gld1_o );
    flag |= this_flag0 || this_flag1;
    if ( this_flag0 == 1 )
      cout << "xx: " << xx << "  gld0_o: " << gld0_o << "   act0_o: " << act0_o << endl;
    if ( this_flag1 == 1 )
      cout << "xx: " << xx << "  gld1_o: " << gld1_o << "   act1_o: " << act1_o << endl;
  }
  ss0_o.close();
  ss1_o.close();

  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

