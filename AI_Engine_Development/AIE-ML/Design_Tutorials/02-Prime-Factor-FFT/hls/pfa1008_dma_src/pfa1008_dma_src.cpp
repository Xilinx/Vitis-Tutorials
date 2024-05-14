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
  // Push out dummy writes to account for latency involved in PL HLS data path:
  //    pfa1008_permute_i:   NFFT/4+0 cycles
  //    pfa1008_permute_o:   NFFT/4+0 cycles
  //                Total:   NFFT/2+0 cycles
  static constexpr int LATENCY = NFFT/2;
 REPEAT: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  RUN_DEPTH: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS PIPELINE II=1
      sig_o.write( buff[dd] );
    } // dd
  } // ll
 RUN_LATENCY: for (int dd=0; dd < LATENCY; dd++) {
#pragma HLS PIPELINE II=1
    sig_o.write( TT_DATA(0) );
  } // dd
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
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[DEPTH];

  // Front end load from DDR4 to PL BRAM:
  load_buffer( mem, buff );

  // Back end transmit from PL BRAM to AIE:
  transmit( buff, sig_o, loop_cnt );
}



