//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_dma_snk.h"
using namespace pfa1008_dma_snk;

// ------------------------------------------------------------
// Stream Capture
// ------------------------------------------------------------

unsigned capture_streams( TT_DATA (&buff)[NSTREAM][DEPTH], TT_STREAM sig_i[NSTREAM],
                          unsigned word_count )
{
  // Streams incoming in polyphase order, need to store into DDR in linear order:
  // --> ss0: s0 s2 s4 s6 s8 s10 s12 s14
  // --> ss1: s1 s3 s5 s7 s9 s11 s13 s15
  //
  // --> buff[0]: s0 s1 s2 s3 s8  s9  s10 s11
  // --> buff[1]: s4 s5 s6 s7 s12 s13 s14 s15
  unsigned   cnt_smp = 0;
  unsigned   cnt_cyc = 0;
  bool     saw_first = 0;
  TT_ADDR         dd = 0;
  unsigned      base = word_count - DEPTH;
  while (cnt_smp < word_count) {
#pragma HLS pipeline II=1
    bool   full = (sig_i[0].empty()==0) && (sig_i[1].empty()==0);
    bool capture = (cnt_smp < base) ? 0 : 1;
    if (full == 1) {
      saw_first = 1;
      TT_SAMPLE val0[4], val1[4];
#pragma HLS array_partition variable=val0 dim=1
#pragma HLS array_partition variable=val1 dim=1
      ( val0[3], val0[2], val0[1], val0[0] ) = sig_i[0].read();
      ( val1[3], val1[2], val1[1], val1[0] ) = sig_i[1].read();
      if (capture == 1) {
        buff[0][dd] = ( val1[1], val0[1], val1[0], val0[0] );
        buff[1][dd] = ( val1[3], val0[3], val1[2], val0[2] );
        dd = dd + 1;
      }
      cnt_smp++;
    }
    if (saw_first == 1)
      cnt_cyc++;
  }
  return(cnt_cyc);
}

// ------------------------------------------------------------
// Read Buffer
// ------------------------------------------------------------

void read_buffer( TT_DATA mem[NSTREAM*DEPTH], TT_DATA (&buff)[NSTREAM][DEPTH] )
{
  ap_uint<1> ss = 0;
  ap_uint<10> dd = 0;
 READ_BUFF: for (int mm=0; mm < DEPTH*NSTREAM; mm++) {
#pragma HLS PIPELINE II=1
    mem[mm] = buff[ss][dd];
    // We have stored alternate words in buff[0] vs buff[1]:
    if ( ss == ap_uint<1>(0) ) {
      ss = 1;
    }
    else {
      ss = 0;
      dd = ( dd == ap_uint<10>(DEPTH-1) ) ? ap_uint<10>(0) : ap_uint<10>(dd + 1);
    }
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

unsigned
pfa1008_dma_snk_wrapper( pfa1008_dma_snk::TT_DATA mem[pfa1008_dma_snk::DEPTH*pfa1008_dma_snk::NSTREAM],
                         unsigned int word_count,
                         pfa1008_dma_snk::TT_STREAM sig_i[pfa1008_dma_snk::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_i
#pragma HLS interface s_axilite  port=word_count  bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[NSTREAM][DEPTH];
#pragma HLS array_partition variable=buff dim=1
  unsigned cycle_count;

  // Front end load from DDR4 to PL BRAM:
  cycle_count = capture_streams( buff, sig_i, word_count );

  // Back end transmit from PL BRAM to AIE:
  read_buffer( mem, buff );

  return(cycle_count);
}



