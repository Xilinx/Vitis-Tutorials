//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft_dma_src.h"
using namespace ifft_dma_src;

// ------------------------------------------------------------
// Load Buffer
// ------------------------------------------------------------

void load_buffer( TT_DATA mem[NFFT/2], TT_SAMPLE (&buff)[NSTREAM][NROW][DEPTH] )
{
  // Assume DDR4 buffer presents samples in column-wise order already, store them in stream buffers
  // --> Streams will transmit rows of matrix (256 + 4 samples)
  // --> Assign rows to streams in polyphase order (Rows 0, 1, 2, 3, 4, 5, 6, 7, 8 to streams 0, 1, 2, 3, 4, 0, 1, 2, etc)
  // LOAD_ROW : for (int row=0,rr=0; row < DEPTH; row+=2) { // Have two samples per DDR4 word

 LOAD_SAMP : for (int samp=0,row=0,rr=0,col=0,ss=0,mm=0; samp < DEPTH*DEPTH; samp+=2) {
#pragma HLS pipeline II=1
    TT_SAMPLE val0, val1;
    if ( row >= NFFT_1D || col >= NFFT_1D ) {
      (val1, val0) = TT_DATA(0);
    }
    else {
      (val1,val0) = mem[mm++];
    }
    if ( ss == NSTREAM-1 ) {
      buff[NSTREAM-1][rr  ][col] = val0;
      buff[0        ][rr+1][col] = val1;
    }
    else {
      buff[ss  ][rr][col] = val0;
      buff[ss+1][rr][col] = val1;
    }
    // Advance:
    rr = ( ss >= NSTREAM-2 ) ? rr + 1 : rr;
    ss = ( ss >= NSTREAM-2 ) ? (ss + 2 - NSTREAM) : (ss+2);
    if ( row == DEPTH-2 ) {
      col = col + 1;
      row = 0;
      rr = 0;
    }
    else {
      row += 2;
    }
  } // col
}

// ------------------------------------------------------------
// Transmit
// ------------------------------------------------------------

void transmit( TT_SAMPLE (&buff)[NSTREAM][NROW][DEPTH], TT_STREAM sig_o[NSTREAM], const int& loop_cnt )
{
  // Extra "ll" loop iteration since ifft_transpose() has same latency as a single transpose operation
 REPEAT: for (int ll=0; ll <= loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  RUN_DEPTH: for (int cc=0,rr=0,dd=0; cc <NROW*DEPTH; cc+=2) { // Send two samples per stream word
#pragma HLS PIPELINE II=1
    STREAM1: for (int ss=0; ss < NSTREAM; ss++) {
        TT_SAMPLE val0 = (ll != loop_cnt) ? buff[ss][rr][dd  ] : TT_SAMPLE(0);
        TT_SAMPLE val1 = (ll != loop_cnt) ? buff[ss][rr][dd+1] : TT_SAMPLE(0);
        sig_o[ss].write( (val1,val0) );
        // std::cout << "ss: " << ss << " val0: " << val0 << " val1: " << val1 << std::endl;
      } // ss
      rr = (dd == DEPTH-2) ? (rr+1) : rr;
      dd = (dd == DEPTH-2) ? 0 : dd+2; // Send two samples per stream word
    } // cc
  } // ll
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
ifft_dma_src_wrapper( ifft_dma_src::TT_DATA mem[ifft_dma_src::NFFT/2],
                      int loop_cnt,
                      ifft_dma_src::TT_STREAM sig_o[ifft_dma_src::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=NFFT/2
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_SAMPLE buff[NSTREAM][NROW][DEPTH];
#pragma HLS array_partition variable=buff dim=1
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=uram
#pragma HLS dependence variable=buff type=intra false

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back end transmit from PL BRAM to AIE:
  transmit( buff, sig_o, loop_cnt );
}



