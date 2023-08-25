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

void capture_streams( TT_DATA (&buff)[NSTREAM][DEPTH], TT_STREAM sig_i[NSTREAM],
                      const int& loop_sel, const int& loop_cnt )
{
  // Streams incoming in polyphase order, need to store into DDR in linear order:
  // --> ss0: s0 s2 s4 s6 s8 s10 s12 s14
  // --> ss1: s1 s3 s5 s7 s9 s11 s13 s15
  //
  // --> buff[0]: s0 s1 s2 s3 s8  s9  s10 s11
  // --> buff[1]: s4 s5 s6 s7 s12 s13 s14 s15
 CAPTURE: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  SAMPLE_IN: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS pipeline II=1
      TT_SAMPLE val0[4], val1[4];
#pragma HLS array_partition variable=val0 dim=1
#pragma HLS array_partition variable=val1 dim=1
      ( val0[3], val0[2], val0[1], val0[0] ) = sig_i[0].read();
      ( val1[3], val1[2], val1[1], val1[0] ) = sig_i[1].read();
      if ( ll == loop_sel ) {
        buff[0][dd] = ( val1[1], val0[1], val1[0], val0[0] );
        buff[1][dd] = ( val1[3], val0[3], val1[2], val0[2] );
      }
    }  //dd
  } // ll
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

void
pfa1008_dma_snk_wrapper( pfa1008_dma_snk::TT_DATA mem[pfa1008_dma_snk::DEPTH*pfa1008_dma_snk::NSTREAM],
                         int loop_sel,
                         int loop_cnt,
                         pfa1008_dma_snk::TT_STREAM sig_i[pfa1008_dma_snk::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_i
#pragma HLS interface s_axilite  port=loop_sel    bundle=control
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[NSTREAM][DEPTH];
#pragma HLS array_partition variable=buff dim=1

  // Front end load from DDR4 to PL BRAM:
  capture_streams( buff, sig_i, loop_sel, loop_cnt );

  // Back end transmit from PL BRAM to AIE:
  read_buffer( mem, buff );
}



