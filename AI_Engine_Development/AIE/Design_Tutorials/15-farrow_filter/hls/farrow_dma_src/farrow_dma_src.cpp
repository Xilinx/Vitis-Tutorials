//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "farrow_dma_src.h"
using namespace farrow_dma_src;

// ------------------------------------------------------------
// Load Buffer
// ------------------------------------------------------------

void load_buffer( TT_DATA mem[DEPTH], TT_DATA (&buff)[DEPTH] )
{
  // Assume DDR4 buffer is presented in linear sample order:
  // --> Store words in buff
  //     buff: s0 s1 s2 s3 ....
  TT_ADDR dd = TT_ADDR(0);
 LOAD_BUFF: for (int mm=0; mm < DEPTH; mm++) {
#pragma HLS PIPELINE II=1
    buff[dd] = mem[mm];
    dd = ( dd == TT_ADDR(DEPTH-1) ) ? TT_ADDR(0) : TT_ADDR(dd+1);
  } // mm
}

// ------------------------------------------------------------
// Transmit
// ------------------------------------------------------------

void transmit( TT_DATA (&buff)[DEPTH], TT_STREAM& sig_o, const int& loop_cnt )
{
 REPEAT: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=4
  RUN_DEPTH: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS PIPELINE II=1
      TT_DATA val_128b;
      TT_SAMPLE val[4];
#pragma HLS array_partition variable=val dim=1
      if ( dd < DEPTH ) {
        // Send out the samples:
        ( val[3], val[2], val[1], val[0] ) = buff[dd]; // Samples s3, s2, s1, s0
        val_128b = (val[3],val[2],val[1],val[0]);
      }
      else {
        val_128b = TT_DATA(0);
      }
      sig_o.write( val_128b );
    } // dd
  } // ll
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
farrow_dma_src_wrapper(  farrow_dma_src::TT_DATA mem[farrow_dma_src::DEPTH],
                         int loop_cnt,
                         farrow_dma_src::TT_STREAM& sig_o )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[DEPTH];

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back end transmit from PL BRAM to AIE:
  transmit( buff, sig_o, loop_cnt );
}