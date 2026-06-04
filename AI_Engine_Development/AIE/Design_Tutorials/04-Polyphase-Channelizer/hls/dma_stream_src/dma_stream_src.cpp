//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dma_stream_src.h"

using namespace dma_stream_src;

// ------------------------------------------------------------
// Load Buffer
// ------------------------------------------------------------

void load_buffer( TT_DATA mem[NSTREAM*DEPTH], TT_DATA (&buff)[NSTREAM][DEPTH] )
{
// Load samples in linear order from DDR4, store into separate stream buffers:
  ap_uint<3> ss = 0;
  ap_uint<9> dd = 0;
 LOAD_BUFF: for (int mm=0; mm < DEPTH*NSTREAM; mm++) {
#pragma HLS PIPELINE II=1
    buff[ss][dd] = mem[mm];
    if ( ss == ap_uint<3>(NSTREAM-1) ) {
      ss = 0;
      dd = ( dd == ap_uint<9>(DEPTH-1) ) ? ap_uint<9>(0) : ap_uint<9>(dd + 1);
    }
    else {
      ss = ss + 1;
    }
  }
}

// ------------------------------------------------------------
// Transmit
// ------------------------------------------------------------

void transmit( TT_DATA (&buff)[NSTREAM][DEPTH], TT_STREAM sig_o[NSTREAM], const int& loop_cnt )
{
 REPEAT: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  RUN_DEPTH: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS PIPELINE II=1
    RUN_STREAM: for (int ss=0; ss < NSTREAM; ss++) {
        sig_o[ss].write( buff[ss][dd] );
      } // ss
    } // dd
  }
}

// ------------------------------------------------------------
// Zero Pad
// ------------------------------------------------------------

void zero_pad( TT_STREAM sig_o[NSTREAM] )
{
  // Pad output streams with zeros to cover ~200 ns latency at 312.5 MHz (64 cycles x 3.2 ns = 204.8 ns).
  // This allows the DMA_SRC to complete without backpressure stall from DMA_SNK.
  static constexpr int PAD_DEPTH = 64;
 ZERO_PAD: for (int dd=0; dd < PAD_DEPTH; dd++) {
#pragma HLS PIPELINE II=1
   ZERO_STREAM: for (int ss=0; ss < NSTREAM; ss++) {
      sig_o[ss].write( TT_DATA(0) );
    }
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void dma_stream_src_wrapper( TT_DATA mem[NSTREAM*DEPTH], int loop_cnt, TT_STREAM sig_o[NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control

  // Internal buffer:
  TT_DATA buff[NSTREAM][DEPTH];
  #pragma HLS bind_storage variable=buff latency=3 impl=bram type=RAM_2P latency=3

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back-end transmit from PL BRAM contents:
  transmit( buff, sig_o, loop_cnt );

  // Zero-pad for latency of downstream graph/kernels to DMA_SNK (~200 ns):
  zero_pad( sig_o );
}




