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

unsigned capture_streams( TT_DATA (&buff)[DEPTH], TT_STREAM& sig_i, unsigned int word_count )
{
  unsigned   cnt_smp = 0;
  unsigned   cnt_cyc = 0;
  bool     saw_first = 0;
  TT_ADDR         dd = 0;
  unsigned      base = word_count - DEPTH;
  while (cnt_smp < word_count) {
#pragma HLS pipeline II=1
    bool   empty = sig_i.empty();
    bool capture = (cnt_smp < base) ? 0 : 1;
    if (empty == 0) {
      saw_first = 1;
      TT_DATA val = sig_i.read();
      if (capture == 1) {
        buff[dd] = val;
        dd = dd + 1;
      }
      cnt_smp++;
    }
    if (saw_first == 1)
      cnt_cyc++;
  }
  return(cnt_cyc);
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

unsigned
pfa1008_dma_snk_wrapper( pfa1008_dma_snk::TT_DATA mem[pfa1008_dma_snk::DEPTH],
                         unsigned int word_count,
                         pfa1008_dma_snk::TT_STREAM& sig_i )
{
#pragma HLS interface mode=axis       port=sig_i
#pragma HLS interface mode=m_axi      port=mem         offset=slave   bundle=gmem1   depth=DEPTH
#pragma HLS interface mode=s_axilite  port=mem                        bundle=control
#pragma HLS interface m_axi           port=mem         num_write_outstanding=1  max_write_burst_length=1
#pragma HLS interface m_axi           port=mem         num_read_outstanding=1  max_read_burst_length=1
#pragma HLS interface mode=s_axilite  port=word_count                 bundle=control
#pragma HLS interface mode=s_axilite  port=return                     bundle=control

  // Internal buffer:
  TT_DATA buff[DEPTH];
#pragma HLS bind_storage variable=buff latency=3
  unsigned cycle_count;

  // Front end load from DDR4 to PL BRAM:
  cycle_count = capture_streams( buff, sig_i, word_count );

  // Back end transmit from PL BRAM to AIE:
  read_buffer( mem, buff );

  return(cycle_count);
}



