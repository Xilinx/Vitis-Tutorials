//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "channelizer_dma_src.h"
using namespace channelizer_dma_src;

// ------------------------------------------------------------
// Load Buffer
// ------------------------------------------------------------

void load_buffer( TT_DATA mem[DEPTH*NSTREAM], TT_SAMPLE (&buff)[NSTREAM][DEPTH*4] )
{
  // Assume DDR4 buffer is presented in linear sample order:
  // --> Assign samples to streams in polyphase order (samples 0, 1, 2, 3, 4, 5, 6, 7, 8 to streams 0, 1, 2, 3, 4, 5, 6, 7, 8 etc)
  // Have two samples per DDR4 word
 LOAD_SAMP : for (int mm=0,dd=0,ss=0; mm < DEPTH*NSTREAM; mm++) {
#pragma HLS pipeline II=1
    TT_SAMPLE val0, val1, val2, val3;
    (val3,val2,val1,val0) = mem[mm];
    buff[ss  ][dd  ] = val0;
    buff[ss+1][dd  ] = val1;
    buff[ss  ][dd+1] = val2;
    buff[ss+1][dd+1] = val3;
    dd+=2;
  } // mm
}

// ------------------------------------------------------------
// Transmit
// ------------------------------------------------------------

void transmit( TT_SAMPLE (&buff)[NSTREAM][DEPTH*4], TT_STREAM sig_o[NSTREAM], const int& loop_cnt )
{
    int dd=0;
 REPEAT: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  RUN_DEPTH: for (int cc=0,dd=0; cc <DEPTH; cc++) { // Send four samples per stream word
#pragma HLS PIPELINE II=1
    STREAM1: for (int ss=0; ss < NSTREAM; ss++) {
        TT_SAMPLE val0 = buff[ss][dd  ];
        TT_SAMPLE val1 = buff[ss][dd+1];
        TT_SAMPLE val2 = buff[ss][dd+2];
        TT_SAMPLE val3 = buff[ss][dd+3];
        sig_o[ss].write( (val3,val2,val1,val0) );
      } // ss
      dd+=4;                                        // Send four samples per stream word
    } // cc
  } // ll
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
channelizer_dma_src_wrapper( channelizer_dma_src::TT_DATA mem[channelizer_dma_src::DEPTH*channelizer_dma_src::NSTREAM],
                             int loop_cnt,
                             channelizer_dma_src::TT_STREAM sig_o[channelizer_dma_src::NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_SAMPLE buff[NSTREAM][DEPTH*4];              // Four samples per DDR4 word
#pragma HLS array_partition variable=buff dim=1
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=uram
#pragma HLS dependence variable=buff type=intra false

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back end transmit from PL BRAM to AIE:
  transmit( buff, sig_o, loop_cnt );
}
