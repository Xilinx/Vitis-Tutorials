//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "dma_stream_snk.h"

using namespace dma_stream_snk;

// ------------------------------------------------------------
// Stream Capture
// ------------------------------------------------------------

void capture_streams( TT_DATA (&buff)[NSTREAM][DEPTH], TT_STREAM sig_i[NSTREAM],
                      const int& loop_sel, const int& loop_cnt )
{
 CAPTURE: for (int ll=0; ll < loop_cnt; ll++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=8
  SAMPLE_IN: for (int dd=0; dd < DEPTH; dd++) {
#pragma HLS pipeline II=1
    STREAM_IN: for (int ss=0; ss < NSTREAM; ss++) {
        TT_DATA val = sig_i[ss].read();
        if ( ll == loop_sel ) {
          buff[ss][dd] = val;
        }
      } // ss
    }  //dd
  } // ll
}

// ------------------------------------------------------------
// Read Buffer
// ------------------------------------------------------------

void read_buffer( TT_DATA mem[NSTREAM*DEPTH], TT_DATA (&buff)[NSTREAM][DEPTH], int dft_perm )
{
  ap_uint<3> ss = 0;
  ap_uint<9> dd = 0;
 READ_BUFF: for (int mm=0; mm < DEPTH*NSTREAM; mm++) {
#pragma HLS PIPELINE II=1
    mem[mm] = buff[ss][dd];
    if ( dft_perm == 0 ) {
      if ( ss == ap_uint<3>(NSTREAM-1) ) {
        ss = 0;
        dd = ( dd == ap_uint<9>(DEPTH-1) ) ? ap_uint<9>(0) : ap_uint<9>(dd + 1);
      }
      else {
        ss = ss + 1;
      }
    }
    else  {
      // Permutation model (for DFT output):
      // Data from DFT comes out 4 samples at a time on even streams first, followed by odd streams second
      // as we go across the matrix columns of the DFT. Reading in this order back to LPDDR restores the proper
      // sample ordering:
      if ( ss == ap_uint<3>(NSTREAM-2) ) {    // Last even stream
        ss = 1;
      }
      else if ( ss == ap_uint<3>(NSTREAM-1) ) { // Last odd stream
        ss = 0;
        dd = ( dd == ap_uint<9>(DEPTH-1) ) ? ap_uint<9>(0) : ap_uint<9>(dd + 1);
      }
      else {
        ss = ss + 2;
      }
    }
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void dma_stream_snk_wrapper( TT_DATA mem[NSTREAM*DEPTH], int loop_sel, int loop_cnt, int dft_perm, TT_STREAM sig_i[NSTREAM] )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH*NSTREAM
#pragma HLS interface axis       port=sig_i
#pragma HLS interface s_axilite  port=loop_sel    bundle=control
#pragma HLS interface s_axilite  port=loop_cnt    bundle=control
#pragma HLS interface s_axilite  port=dft_perm    bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control
#pragma HLS DATAFLOW

  // Internal buffer:
  TT_DATA buff[NSTREAM][DEPTH];
  //#pragma HLS bind_storage variable=buff latency=3 impl=bram type=RAM_2P
#pragma HLS array_partition variable=buff dim=1

  // Capture channelizer output streams to PL BRAM:
  capture_streams( buff, sig_i, loop_sel, loop_cnt );

  // Front end load from DDR4 to PL BRAM:
  read_buffer( mem, buff, dft_perm );
}




