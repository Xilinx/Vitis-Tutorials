//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "merge_8x4.h"
using namespace merge_8x4;

// ------------------------------------------------------------
// Consumer
// ------------------------------------------------------------

void consumer( TT_STREAM sig_i[NSTREAM_I], hls::stream_of_blocks<TT_BLOCK>& ss0, hls::stream_of_blocks<TT_BLOCK>& ss1, hls::stream_of_blocks<TT_BLOCK>& ss2, hls::stream_of_blocks<TT_BLOCK>& ss3 )
{
#pragma HLS pipeline II=2
  hls::write_lock<TT_BLOCK> WL0(ss0);
  WL0[ 0] = sig_i[ 0].read();
  WL0[ 1] = sig_i[ 1].read();
  hls::write_lock<TT_BLOCK> WL1(ss1);
  WL1[ 0] = sig_i[ 2].read();
  WL1[ 1] = sig_i[ 3].read();
  hls::write_lock<TT_BLOCK> WL2(ss2);
  WL2[ 0] = sig_i[ 4].read();
  WL2[ 1] = sig_i[ 5].read();
  hls::write_lock<TT_BLOCK> WL3(ss3);
  WL3[ 0] = sig_i[ 6].read();
  WL3[ 1] = sig_i[ 7].read();
}

// ------------------------------------------------------------
// Producer
// ------------------------------------------------------------

void producer( hls::stream_of_blocks<TT_BLOCK>& ss0, hls::stream_of_blocks<TT_BLOCK>& ss1, hls::stream_of_blocks<TT_BLOCK>& ss2, hls::stream_of_blocks<TT_BLOCK>& ss3, TT_STREAM sig_o[NSTREAM_O] )
{
#pragma HLS pipeline II=2
  // We produce 16 samples over 8 cycles as 128-bit pairs @ 625 Msps (two cint32 @ 312.5 MHz)
  // Inputs arrive in polyphase order:
  hls::read_lock<TT_BLOCK> RL0(ss0);
  hls::read_lock<TT_BLOCK> RL1(ss1);
  hls::read_lock<TT_BLOCK> RL2(ss2);
  hls::read_lock<TT_BLOCK> RL3(ss3);
  TT_SAMPLE v00, v08;    (v08,v00) = RL0[ 0];
  TT_SAMPLE v01, v09;    (v09,v01) = RL0[ 1];
  TT_SAMPLE v02, v10;    (v10,v02) = RL1[ 0];
  TT_SAMPLE v03, v11;    (v11,v03) = RL1[ 1];
  TT_SAMPLE v04, v12;    (v12,v04) = RL2[ 0];
  TT_SAMPLE v05, v13;    (v13,v05) = RL2[ 1];
  TT_SAMPLE v06, v14;    (v14,v06) = RL3[ 0];
  TT_SAMPLE v07, v15;    (v15,v07) = RL3[ 1];

  sig_o[0].write( (v04,v00) );
  sig_o[1].write( (v05,v01) );
  sig_o[2].write( (v06,v02) );
  sig_o[3].write( (v07,v03) );
  
  sig_o[0].write( (v12,v08) );
  sig_o[1].write( (v13,v09) );
  sig_o[2].write( (v14,v10) );
  sig_o[3].write( (v15,v11) );
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void merge_8x4_wrapper( TT_STREAM sig_i[NSTREAM_I], TT_STREAM sig_o[NSTREAM_O] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS interface axis       port=sig_i
#pragma HLS interface axis       port=sig_o

  hls::stream_of_blocks<TT_BLOCK> ss0;
  hls::stream_of_blocks<TT_BLOCK> ss1;
  hls::stream_of_blocks<TT_BLOCK> ss2;
  hls::stream_of_blocks<TT_BLOCK> ss3;
#pragma HLS bind_storage variable=ss0 type=ram_t2p impl=bram
#pragma HLS bind_storage variable=ss1 type=ram_t2p impl=bram
#pragma HLS bind_storage variable=ss2 type=ram_t2p impl=bram
#pragma HLS bind_storage variable=ss3 type=ram_t2p impl=bram

#pragma HLS dataflow

  consumer( sig_i, ss0, ss1, ss2, ss3 );
  producer( ss0, ss1, ss2, ss3, sig_o );
}



