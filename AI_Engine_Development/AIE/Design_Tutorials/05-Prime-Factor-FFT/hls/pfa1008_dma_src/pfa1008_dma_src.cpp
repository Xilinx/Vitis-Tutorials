//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_dma_src.h"
using namespace pfa1008_dma_src;

// ------------------------------------------------------------
// Load Buffer
// ------------------------------------------------------------

void load_buffer( TT_DATA mem[NSTREAM*DEPTH], TT_DATA (&buff)[NSTREAM][DEPTH] )
{
  // Assume DDR4 buffer is presented in linear sample order:
  // --> Store alternate words in buff[0] vs buff[1]
  //     buff[0]: s0 s1 s2 s3 s8  s9  s10 s11 s16 s17 s18 s19
  //     buff[1]: s4 s5 s6 s7 s12 s13 s14 s15 s20 s21 s22 s23
  bool ss = 0;
  TT_ADDR dd = TT_ADDR(0);
 LOAD_BUFF: for (int mm=0; mm < DEPTH*NSTREAM; mm++) {
#pragma HLS PIPELINE II=1
    buff[ss][dd] = mem[mm];
    if ( ss == 1 ) {
      ss = 0;
      dd = ( dd == TT_ADDR(DEPTH-1) ) ? TT_ADDR(0) : TT_ADDR(dd+1);
    }
    else {
      ss = 1;
    }
  } // mm
}

// ------------------------------------------------------------
// Transmit
// ------------------------------------------------------------

void transmit( TT_DATA (&buff)[NSTREAM][DEPTH], TT_STREAM sig_o[NSTREAM], const int& loop_cnt )
{
 REPEAT: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=16
  RUN_DEPTH: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS PIPELINE II=1
      TT_DATA ss0_val, ss1_val;
      TT_SAMPLE val0[4], val1[4];
#pragma HLS array_partition variable=val0 dim=1
#pragma HLS array_partition variable=val1 dim=1
      if ( dd < DEPTH ) {
        // Send out the samples in polyphase order, even on ss0, odd on ss1:
        ( val0[3], val0[2], val0[1], val0[0] ) = buff[0][dd]; // Samples d3, d2, d1, d0
        ( val1[3], val1[2], val1[1], val1[0] ) = buff[1][dd]; // Samples d7, d6, d5, d4
        ss0_val = (val1[2],val1[0],val0[2],val0[0]); // Even samples (d6, d4, d2, d0)
        ss1_val = (val1[3],val1[1],val0[3],val0[1]); // Odd  samples (d7, d5, d3, d1)
      }
      else {
        ss0_val = TT_DATA(0);
        ss1_val = TT_DATA(0);
      }
      sig_o[0].write( ss0_val );
      sig_o[1].write( ss1_val );
    } // dd
  } // ll
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void zero_pad( TT_STREAM sig_o[NSTREAM] )
{
  // This latency equals approx 4.6 us to account for data path latency between DMA_SRC and DMA_SNK:
  static constexpr int LATENCY = 1438;

  PAD_ZERO: for (int ii=0; ii < LATENCY; ii++) {
#pragma HLS PIPELINE II=1
    sig_o[0].write( TT_DATA(0) );
    sig_o[1].write( TT_DATA(0) );
  }
}

void
pfa1008_dma_src_wrapper( pfa1008_dma_src::TT_DATA mem[pfa1008_dma_src::DEPTH*pfa1008_dma_src::NSTREAM],
                         int loop_cnt,
                         pfa1008_dma_src::TT_STREAM sig_o[pfa1008_dma_src::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control

  // Internal buffer:
  TT_DATA buff[NSTREAM][DEPTH];
  #pragma HLS bind_storage variable=buff latency=3 impl=bram type=RAM_2P

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back end transmit from PL BRAM to AIE:
  transmit( buff, sig_o, loop_cnt );

  // Zero pad to account for latency of data path:
  zero_pad( sig_o );
}



