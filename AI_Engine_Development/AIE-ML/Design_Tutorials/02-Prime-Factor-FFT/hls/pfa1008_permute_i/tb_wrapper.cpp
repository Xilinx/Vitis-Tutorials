//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_permute_i.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace pfa1008_permute_i;

static constexpr int NTRANSFORM = 8;

// ------------------------------------------------------------
// Read & format input from files
// ------------------------------------------------------------

TT_AXI4 read_from_files( ifstream& ss )
{
  // Read four "cint16" values:
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

extern void pfa1008_permute_i_wrapper( TT_STREAM& sig_i, TT_STREAM& sig_o );

int main( void )
{
  TT_STREAM sig_i;
  TT_STREAM sig_o;

  // Load data from files:
  std::ifstream ss0_i;
  ss0_i.open("ss0_i.txt",std::ifstream::in);
  if ( ss0_i.is_open() == 0 ) cerr << "Cannot open file data/ss0_i.txt" << endl;

  // Grab contents from input files:
  for (int xx=0; xx < NTRANSFORM*NFFT/4; xx++) { // Four cint16 samples per 128-bit PLIO
    sig_i.write( read_from_files( ss0_i ) );
  }
  // Write zeros to manage startup latency:
  // --> DMA SRC will write this in the final design
  int LATENCY = NFFT/4;
  for (int xx=0; xx < LATENCY; xx++) {
    sig_i.write( TT_AXI4(0) );
  }
  ss0_i.close();
  cout << "*** Parsed DDR4 Data ***" << endl;

  // Run DUT (including extra cycles for latency):
  for (int ss=0; ss < NTRANSFORM*NFFT/4+LATENCY; ss++) {
    pfa1008_permute_i_wrapper( sig_i, sig_o );
  }

  cout << "Done pfa1008_permute_i_wrapper()" << endl;
  cout << "Stream Size: " << sig_o.size() << endl;


  // Check output streams:
  std::ifstream ss0_o;
  ss0_o.open("ss0_o.txt",std::ifstream::in);
  if ( ss0_o.is_open() == 0 ) cerr << "Cannot open file data/ss0_o.txt" << endl;
  bool flag = 0;
  for (int xx=0; xx < NTRANSFORM*NFFT/4; xx++) {
    TT_AXI4 gld0_o = read_from_files( ss0_o );
    TT_AXI4 act0_o = sig_o.read();
    bool this_flag0 = ( act0_o != gld0_o );
    flag |= this_flag0;
    if ( this_flag0 == 1 )
      cout << "xx: " << xx << "  gld0_o: " << gld0_o << "   act0_o: " << act0_o << endl;
  }
  ss0_o.close();

  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

