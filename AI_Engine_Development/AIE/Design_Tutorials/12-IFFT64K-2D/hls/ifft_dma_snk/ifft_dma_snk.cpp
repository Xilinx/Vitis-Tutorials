//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft_dma_snk.h"
using namespace ifft_dma_snk;

// ------------------------------------------------------------
// Stream Capture
// ------------------------------------------------------------

void capture_streams( TT_SAMPLE (&buff)[NSTREAM][DEPTH*DEPTH/NSTREAM], TT_STREAM sig_i[NSTREAM],
                      const int& loop_sel, const int& loop_cnt )
{
  // Incoming samples arriving down columns:
 CAPTURE: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
    for (int cc=0,addr=0; cc < DEPTH/NSTREAM; cc++) {
      for (int rr=0; rr < DEPTH/2; rr++) {
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
      } // rr
    } // cc
  } // ll
}

// ------------------------------------------------------------
// Read Buffer
// ------------------------------------------------------------

void read_buffer( TT_DATA mem[NFFT/2], TT_SAMPLE (&buff)[NSTREAM][DEPTH*DEPTH/NSTREAM] )
{
  // Outgoing samples send along rows:
  for (int rr=0,mm=0; rr < NFFT_1D; rr++) {
    int addr0 = rr;
    int ss0=0;
    for (int cc=0; cc < NFFT_1D; cc+=2) {
#pragma HLS PIPELINE II=1
      int ss1 = (ss0 + 1) % NSTREAM;
      int addr1 = (ss0 == NSTREAM-1) ? addr0 + DEPTH : addr0;
      TT_SAMPLE val0 = buff[ss0][addr0];
      TT_SAMPLE val1 = buff[ss1][addr1];
      mem[mm++] = ( val1, val0 );
      if ( ss0 == NSTREAM-1 || ss1 == NSTREAM-1 ) {
        addr0 = addr0 + DEPTH;
      }
      ss0 = (ss0 + 2) % NSTREAM;
    }
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
ifft_dma_snk_wrapper( ifft_dma_snk::TT_DATA mem[ifft_dma_snk::NFFT/2],
                      int loop_sel,
                      int loop_cnt,
                      ifft_dma_snk::TT_STREAM sig_i[ifft_dma_snk::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=NFFT/2
#pragma HLS interface axis       port=sig_i
#pragma HLS interface s_axilite  port=loop_sel    bundle=control
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_SAMPLE buff[NSTREAM][DEPTH*DEPTH/NSTREAM];
#pragma HLS array_partition variable=buff dim=1
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=uram

  // Front end load from DDR4 to PL BRAM:
  capture_streams( buff, sig_i, loop_sel, loop_cnt );

  // Back end transmit from PL BRAM to AIE:
  read_buffer( mem, buff );
}



