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

void load_buffer( TT_DATA mem[DEPTH], TT_DATA (&buff)[DEPTH] )
{
  // Assume DDR4 buffer is presented in linear sample order
 LOAD_BUFF: for (int mm=0; mm < DEPTH; mm++) {
#pragma HLS PIPELINE II=1
    buff[mm] = mem[mm];
  } // mm
}

// ------------------------------------------------------------
// Transmit
// ------------------------------------------------------------

void transmit( TT_DATA (&buff)[DEPTH], TT_STREAM& sig_o, const int& loop_cnt )
{
  static constexpr int LATENCY = NFFT/2;
 REPEAT: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  RUN_DEPTH: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS PIPELINE II=1
      sig_o.write( buff[dd] );
    } // dd
  } // ll
}

// ------------------------------------------------------------
// Zero Pad
// ------------------------------------------------------------

void zero_pad( TT_STREAM& sig_o )
{
  // Add 22.9 us of latency at 312.5 MHz = 7156 cycles:
  static constexpr int NUM_ZERO_PAD = 7156;
 ZERO_PAD: for (int zz=0; zz < NUM_ZERO_PAD; zz++) {
#pragma HLS PIPELINE II=1
    sig_o.write( TT_DATA(0) );
  } // zz
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void
pfa1008_dma_src_wrapper( pfa1008_dma_src::TT_DATA mem[pfa1008_dma_src::DEPTH],
                         int loop_cnt,
                         pfa1008_dma_src::TT_STREAM& sig_o )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH
#pragma HLS interface axis       port=sig_o
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control

  // Internal buffer:
  TT_DATA buff[DEPTH];
#pragma HLS bind_storage variable=buff latency=3

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back end transmit from PL BRAM to AIE:
  transmit( buff, sig_o, loop_cnt );

  // Zero-pad to account for latency from DMA_SRC to DMA_SNK:
  zero_pad( sig_o );
}



