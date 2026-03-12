//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "packet_sender.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace packet_sender;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main( void )
{
  static constexpr int NSAMP = 4096; // Aligns with gen_vectors.m
  TT_STREAM_OUT sig_g[NSTREAM_O];
  TT_STREAM     sig_i[NSTREAM_I];

  //
  std::ifstream ss_i0,ss_i1,ss_o0,ss_o1;
  ss_i0.open("sig_i_0.txt",std::ifstream::in);
  ss_i1.open("sig_i_1.txt",std::ifstream::in);
  ss_o0.open("sig_o_0.txt",std::ifstream::in);
  ss_o1.open("sig_o_1.txt",std::ifstream::in);
  if (ss_i0.is_open()==0) { cerr << "Cannot open data/sig_i_0.txt" << endl; return(1); }
  if (ss_i1.is_open()==0) { cerr << "Cannot open data/sig_i_1.txt" << endl; return(1); }
  if (ss_o0.is_open()==0) { cerr << "Cannot open data/sig_o_0.txt" << endl; return(1); }
  if (ss_o1.is_open()==0) { cerr << "Cannot open data/sig_o_1.txt" << endl; return(1); }

  for (unsigned ss=0; ss < NSAMP; ss+=8) {
    TT_SAMPLE s0,s1,s2,s3,s4,s5,s6,s7;
    ss_i0 >> s0;
    ss_i0 >> s1;
    ss_i0 >> s2;
    ss_i0 >> s3;
    ss_i1 >> s4;
    ss_i1 >> s5;
    ss_i1 >> s6;
    ss_i1 >> s7;
    sig_i[0].write( ( s3, s2, s1, s0 ) );
    sig_i[1].write( ( s7, s6, s5, s4 ) );
  }

  for (unsigned ss=0; ss < NSAMP/4/NSTREAM_O + PACKET_NUM; ss++) {
    ap_axiu<NBITS,0,0,0> tmp0;
    ap_axiu<NBITS,0,0,0> tmp1;
    TT_SAMPLE s00, s01, s02, s03;
    TT_SAMPLE s04, s05, s06, s07;
    ap_uint<16> tkeep0;
    ap_uint<16> tkeep1;
    ap_uint<1> tlast0;
    ap_uint<1> tlast1;

    ss_o0 >> s00;
    ss_o0 >> s01;
    ss_o0 >> s02;
    ss_o0 >> s03;
    ss_o0 >> tkeep0;
    ss_o0 >> tlast0;
    tmp0.data=(s03,s02,s01,s00);
    tmp0.keep=tkeep0;
    tmp0.last=tlast0;
    sig_g[0].write( tmp0 );
    
    ss_o1 >> s04;
    ss_o1 >> s05;
    ss_o1 >> s06;
    ss_o1 >> s07;
    ss_o1 >> tkeep1;
    ss_o1 >> tlast1;
    tmp1.data=(s07,s06,s05,s04);
    tmp1.keep=tkeep1;
    tmp1.last=tlast1;
    sig_g[1].write( tmp1 );
  }


  for (unsigned dd=0; dd < NSTREAM_I; dd++)
    cout << "Input Stream " << dd << " Size: " << sig_i[dd].size() << endl;

  for (unsigned dd=0; dd < NSTREAM_O; dd++)
    cout << "Golden Stream " << dd << " Size: " << sig_g[dd].size() << endl;

  // Run DUT:
  TT_STREAM_OUT sig_o[NSTREAM_O];
  for (unsigned ss=0; ss < NSAMP/packet_sender::NSAMPLES; ss ++) {
    packet_sender_wrapper( sig_i, sig_o );
  }

  for (unsigned dd=0; dd < NSTREAM_O; dd++)
    cout << "Output Stream " << dd << " Size: " << sig_o[dd].size() << endl;


  // Compare actual against golden
  bool flag = 0;
  for (unsigned ss=0; ss < NSAMP/4/NSTREAM_O + PACKET_NUM; ss++) {
    ap_axiu<NBITS,0,0,0> tmp0,tmp1;

    // Actual
    tmp0 = sig_o[0].read();
    tmp1 = sig_o[1].read();
    TT_SAMPLE s00, s01, s02, s03;
    TT_SAMPLE s04, s05, s06, s07;
    ap_uint<16> s_tkeep0;
    ap_uint<16> s_tkeep1;
    ap_uint<1> s_tlast0;
    ap_uint<1> s_tlast1;
    (s03,s02,s01,s00) = tmp0.data;
    s_tkeep0= tmp0.keep;
    s_tlast0= tmp0.last;
    (s07,s06,s05,s04) = tmp1.data;
    s_tkeep1= tmp1.keep;
    s_tlast1= tmp1.last;

    // Golden
    tmp0 = sig_g[0].read();
    tmp1 = sig_g[1].read();
    TT_SAMPLE g00, g01, g02, g03;
    TT_SAMPLE g04, g05, g06, g07;
    ap_uint<16> g_tkeep0;
    ap_uint<16> g_tkeep1;
    ap_uint<1> g_tlast0;
    ap_uint<1> g_tlast1;
    (g03,g02,g01,g00) = tmp0.data;
    g_tkeep0= tmp0.keep;
    g_tlast0= tmp0.last;
    (g07,g06,g05,g04) = tmp1.data;
    g_tkeep1= tmp1.keep;
    g_tlast1= tmp1.last;
    
    // Compare
    bool this_flag = (g00 != s00) || (g01 != s01) || (g02 != s02) || (g03 != s03) || (g04 != s04) || (g05 != s05) || (g06 != s06) || (g07 != s07) || (g_tkeep0 != s_tkeep0) || (g_tkeep1 != s_tkeep1) || (g_tlast0 != s_tlast0) || (g_tlast1 != s_tlast1);
    flag |= this_flag;
  }
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}
