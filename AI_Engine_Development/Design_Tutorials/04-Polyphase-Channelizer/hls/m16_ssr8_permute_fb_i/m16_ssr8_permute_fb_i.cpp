//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "m16_ssr8_permute_fb_i.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

m16_ssr8_permute_fb_i::m16_ssr8_permute_fb_i( void )
{
}

// ------------------------------------------------------------
// Unpack input samples
// ------------------------------------------------------------

void m16_ssr8_permute_fb_i::unpack_samples( TT_DATA (&data_i_0)[M], TT_DATA (&data_i_1)[M], TT_STREAM sig_i[SSR_I] )
{
  static TT_DATA prev_data[2] = {0,0};
#pragma HLS reset variable=prev_data
#pragma HLS INLINE
 READ: for (unsigned ii=0; ii < SSR_I; ii++) {
    TT_AXI4 val_i = sig_i[ii].read();
    ( data_i_1[ii+SSR_I], data_i_1[ii], data_i_0[ii+SSR_I], data_i_0[ii] ) = val_i;
  }
  data_i_0[M-2] = prev_data[0];
  data_i_0[M-1] = prev_data[1];
  data_i_1[M-2] = data_i_0[0];
  data_i_1[M-1] = data_i_0[1];
  // Update state:
  prev_data[0] = data_i_1[0];
  prev_data[1] = data_i_1[1];
}

// ------------------------------------------------------------
// Apply Permutation:
// ------------------------------------------------------------

void m16_ssr8_permute_fb_i::apply_permutation( TT_DATA (&data_i)[M], TT_DATA (&data_o)[M], ap_uint<3> state )
{
  static ap_uint<4> permute[NSTATE][M] = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15},
                                           { 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 0, 1},
                                           { 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 0, 1, 2, 3},
                                           { 6, 7, 8, 9,10,11,12,13,14,15, 0, 1, 2, 3, 4, 5},
                                           { 8, 9,10,11,12,13,14,15, 0, 1, 2, 3, 4, 5, 6, 7},
                                           {10,11,12,13,14,15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                           {12,13,14,15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11},
                                           {14,15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13} };

#pragma HLS INLINE
 PERMUTE: for (unsigned mm=0; mm < M; mm++) {
    unsigned pp = permute[state][mm];
    data_o[pp] = data_i[mm];
  } // mm
}

// ------------------------------------------------------------
// Align Latency
// ------------------------------------------------------------

void m16_ssr8_permute_fb_i::align_latency( TT_DATA (&data_i_0)[M], TT_DATA (&data_i_1)[M],
                                           TT_DATA (&data_o_0)[M], TT_DATA (&data_o_1)[M] )
{
  // AIE filterbank kernel is aligned to 8-cycle period (due to tap assignments).
  // So we introduce 6 cycles additional latency to compliment the 2 cycles added during stream formating
  static TT_DATA buff0[M][LATENCY] = { TT_DATA(0) };
  static TT_DATA buff1[M][LATENCY] = { TT_DATA(0) };
  static ap_uint<2> cnt = 0;
#pragma HLS reset variable=cnt;
#pragma HLS array_partition variable=buff0 dim=0
#pragma HLS array_partition variable=buff1 dim=0
 DELAY: for (unsigned mm=0; mm < M; mm++) {
    data_o_0[mm] = buff0[mm][cnt];
    data_o_1[mm] = buff1[mm][cnt];
    buff0[mm][cnt] = data_i_0[mm];
    buff1[mm][cnt] = data_i_1[mm];
  }
  cnt = ( cnt == ap_uint<2>(LATENCY-1) ) ? ap_uint<2>(0) : ap_uint<2>(cnt + 1);
}

// ------------------------------------------------------------
// Format Streams
// ------------------------------------------------------------

void m16_ssr8_permute_fb_i::format_streams( TT_DATA (&data_o_0)[M], TT_DATA (&data_o_1)[M], TT_AXI4 streams_o[SSR_O] )
{
  static bool odd_cycle = 0;
  static TT_AXI4 bot_streams[SSR_O] = { TT_AXI4(0) };
  static TT_DATA data_p_0[M] = { TT_DATA(0) };
  static TT_DATA data_p_1[M] = { TT_DATA(0) };
#pragma HLS reset           variable=odd_cycle
#pragma HLS array_partition variable=bot_streams
#pragma HLS array_partition variable=data_p_0
#pragma HLS array_partition variable=data_p_1
#pragma HLS INLINE

  // Format all output streams on even cycles:
  TT_AXI4 top_streams[SSR_O];
#pragma HLS array_partition variable=top_streams
  if ( odd_cycle == 0 ) {
  FORMAT: for (int ss=0; ss < SSR_O; ss++) {
      top_streams[ss] = ( data_o_1[2*ss  ], data_o_0[2*ss  ], data_p_1[2*ss  ], data_p_0[2*ss  ] );
      bot_streams[ss] = ( data_o_1[2*ss+1], data_o_0[2*ss+1], data_p_1[2*ss+1], data_p_0[2*ss+1] );
    }
  }

  // Send streams:
 COPY_STREAM: for (int ss=0; ss < SSR_O; ss++) {
    streams_o[ss] = ( odd_cycle == 0 ) ? top_streams[ss] : bot_streams[ss];
  }

  // Update 'data' state with current inputs on odd cycles:
  if ( odd_cycle == 1 ) {
  COPY: for (int mm=0; mm < M; mm++) {
      data_p_0[mm] = data_o_0[mm];
      data_p_1[mm] = data_o_1[mm];
    }
  }

  // Update cycle:
  odd_cycle = 1 - odd_cycle;
}

// ------------------------------------------------------------
// Write Output Streams
// ------------------------------------------------------------

void m16_ssr8_permute_fb_i::write_streams( TT_AXI4 streams_o[SSR_O], TT_STREAM sig_o[SSR_O] )
{
#pragma HLS INLINE
 WRITE: for (unsigned ii=0; ii < SSR_O; ii++) {
    sig_o[ii].write( streams_o[ii] );
  }
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void m16_ssr8_permute_fb_i::run( TT_STREAM sig_i[SSR_I],
                                 TT_STREAM sig_o[SSR_O] )
{
#pragma HLS inline

  // Need to declare as 'static' in Vitis HLS to allow resets to be applied:
  static ap_uint<3> fsm_state = 0;
#pragma HLS reset variable=fsm_state

  // Unpack the new input samples and combine with state history:
  TT_DATA data_i_0[M], data_i_1[M];
  TT_DATA data_d_0[M], data_d_1[M];
  TT_AXI4 streams_o[SSR_O];
#pragma HLS array_partition variable=data_i_0
#pragma HLS array_partition variable=data_i_1
#pragma HLS array_partition variable=data_d_0
#pragma HLS array_partition variable=data_d_1
#pragma HLS array_partition variable=streams_o

  unpack_samples( data_i_0, data_i_1, sig_i );

  // Apply output permutation:
  TT_DATA data_o_0[M];
  TT_DATA data_o_1[M];
#pragma HLS array_partition variable=data_o_0
#pragma HLS array_partition variable=data_o_1
  apply_permutation( data_i_0, data_o_0, fsm_state ); fsm_state++;
  apply_permutation( data_i_1, data_o_1, fsm_state ); fsm_state++;

  // Align latency to Polyphase FIR:
  align_latency( data_o_0, data_o_1, data_d_0, data_d_1 );

  // Format streams:
  format_streams( data_d_0, data_d_1, streams_o );

  // Write outputs to streams:
  write_streams( streams_o, sig_o );
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

using TT_DUT = m16_ssr8_permute_fb_i;

void m16_ssr8_permute_fb_i_wrapper( TT_DUT::TT_STREAM sig_i[TT_DUT::SSR_I],
                                    TT_DUT::TT_STREAM sig_o[TT_DUT::SSR_O] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS pipeline II=1
  static m16_ssr8_permute_fb_i dut;
  dut.run( sig_i, sig_o );
}



