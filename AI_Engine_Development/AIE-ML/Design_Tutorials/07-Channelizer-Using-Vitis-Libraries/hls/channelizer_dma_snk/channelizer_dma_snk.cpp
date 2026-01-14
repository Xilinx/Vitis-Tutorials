//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "channelizer_dma_snk.h"
using namespace channelizer_dma_snk;

// ------------------------------------------------------------
// Stream Capture
// ------------------------------------------------------------

void capture_streams( TT_SAMPLE (&buff)[NSTREAM][2*DEPTH], TT_STREAM sig_i[NSTREAM],
                      const int& loop_sel, const int& loop_cnt )
{
  // Incoming samples
 CAPTURE: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
    for (int cc=0,addr=0; cc < DEPTH; cc++) {
#pragma HLS pipeline II=1
      for (int ss=0; ss < NSTREAM; ss++) {
        TT_SAMPLE val1, val0;
        ( val1, val0 ) = sig_i[ss].read();
        if ( ll == loop_sel ) {
          buff[ss][addr+0] = val0;
          buff[ss][addr+1] = val1;
        }
      } // ss
      addr = addr + 2;
    } // cc
  } // ll
}

// ------------------------------------------------------------
// Read Buffer
// ------------------------------------------------------------

void read_buffer( TT_DATA mem[DEPTH], TT_SAMPLE (&buff)[NSTREAM][2*DEPTH] )
{
  // Outgoing samples
  for (int rr=0,mm=0; rr < 2*DEPTH; rr+=2) {
    int addr0 = rr;
    int ss0=0;
    for (int cc=0; cc < NSTREAM; cc++) {
#pragma HLS PIPELINE II=1
      TT_SAMPLE val0 = buff[ss0  ][addr0];
      TT_SAMPLE val1 = buff[ss0+1][addr0];
      mem[mm++] = ( val1, val0 );
      ss0+=2;
      if ( ss0 == NSTREAM ) {
        addr0++;
        ss0=0;
      }
    }
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
channelizer_dma_snk_wrapper( channelizer_dma_snk::TT_DATA mem[channelizer_dma_snk::DEPTH*channelizer_dma_snk::NSTREAM],
                             int loop_sel,
                             int loop_cnt,
                             channelizer_dma_snk::TT_STREAM sig_i[channelizer_dma_snk::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_i
#pragma HLS interface s_axilite  port=loop_sel    bundle=control
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_SAMPLE buff[NSTREAM][2*DEPTH];                   // Two samples per DDR4 word
#pragma HLS array_partition variable=buff dim=1
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=uram

  // Front end load from DDR4 to PL BRAM:
  capture_streams( buff, sig_i, loop_sel, loop_cnt );

  // Back end transmit from PL BRAM to AIE:
  read_buffer( mem, buff );
}



