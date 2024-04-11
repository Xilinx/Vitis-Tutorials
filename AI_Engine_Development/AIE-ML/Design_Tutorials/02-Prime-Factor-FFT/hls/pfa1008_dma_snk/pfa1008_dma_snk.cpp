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

void capture_streams( TT_DATA (&buff)[DEPTH], TT_STREAM& sig_i,
                      const int& loop_sel, const int& loop_cnt )
{
 CAPTURE: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  SAMPLE_IN: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS pipeline II=1
      TT_DATA val = sig_i.read();
      if ( ll == loop_sel ) {
        buff[dd] = val;
      }
    }  //dd
  } // ll
}

// ------------------------------------------------------------
// Read Buffer
// ------------------------------------------------------------

void read_buffer( TT_DATA mem[DEPTH], TT_DATA (&buff)[DEPTH] )
{
 READ_BUFF: for (int mm=0; mm < DEPTH; mm++) {
#pragma HLS PIPELINE II=1
    mem[mm] = buff[mm];
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
pfa1008_dma_snk_wrapper( pfa1008_dma_snk::TT_DATA mem[pfa1008_dma_snk::DEPTH],
                         int loop_sel,
                         int loop_cnt,
                         pfa1008_dma_snk::TT_STREAM& sig_i )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH
#pragma HLS interface axis       port=sig_i
#pragma HLS interface s_axilite  port=loop_sel    bundle=control
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[DEPTH];

  // Front end load from DDR4 to PL BRAM:
  capture_streams( buff, sig_i, loop_sel, loop_cnt );

  // Back end transmit from PL BRAM to AIE:
  read_buffer( mem, buff );
}



