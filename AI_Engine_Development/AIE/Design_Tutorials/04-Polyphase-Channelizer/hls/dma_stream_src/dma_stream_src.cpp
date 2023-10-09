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
// Wrapper
// ------------------------------------------------------------

void dma_stream_src_wrapper( TT_DATA mem[NSTREAM*DEPTH], int loop_cnt, TT_STREAM sig_o[NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[NSTREAM][DEPTH];
  //#pragma HLS bind_storage variable=buff latency=3 impl=bram type=RAM_2P
#pragma HLS array_partition variable=buff dim=1

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back-end transmit from PL BRAM contents:
  transmit( buff, sig_o, loop_cnt );
}




