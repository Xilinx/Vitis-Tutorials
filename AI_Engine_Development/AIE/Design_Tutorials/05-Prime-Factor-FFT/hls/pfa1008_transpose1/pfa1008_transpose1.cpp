//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_transpose1.h"

using namespace std;
using namespace pfa1008_transpose1;

// Approach:
// 1) Collect 14 samples each from ss0 and ss1 (4 transforms) and write to buffer in polyphase order
// 2) Read from buffer using strided address @ 8 samples per clock
// 3) Collect into 9 x 4 = 36 samples, alternating 8 transforms across ss0 and ss1

// ------------------------------------------------------------
// Multiplex Buffer Index
// ------------------------------------------------------------

void mux_index( TT_INDEX (&wr_idx)[8], ap_uint<2> (&wr_off)[4], int val,
                TT_INDEX& wr_idx0, TT_INDEX& wr_idx1 )
{
  if      ( wr_off[0] == ap_uint<2>(val) ) { wr_idx0 = wr_idx[0];  wr_idx1 = wr_idx[4]; }
  else if ( wr_off[1] == ap_uint<2>(val) ) { wr_idx0 = wr_idx[1];  wr_idx1 = wr_idx[5]; }
  else if ( wr_off[2] == ap_uint<2>(val) ) { wr_idx0 = wr_idx[2];  wr_idx1 = wr_idx[6]; }
  else                                     { wr_idx0 = wr_idx[3];  wr_idx1 = wr_idx[7]; }
}

// ------------------------------------------------------------
// Multiplex Buffer Write Data
// ------------------------------------------------------------

void mux_wrdata( TT_DATA (&wr_data)[8], ap_uint<2> (&wr_off)[4], int val,
                 TT_DATA& wr_data0, TT_DATA& wr_data1 )
{
  if      ( wr_off[0] == ap_uint<2>(val) ) { wr_data0 = wr_data[0];  wr_data1 = wr_data[4]; }
  else if ( wr_off[1] == ap_uint<2>(val) ) { wr_data0 = wr_data[1];  wr_data1 = wr_data[5]; }
  else if ( wr_off[2] == ap_uint<2>(val) ) { wr_data0 = wr_data[2];  wr_data1 = wr_data[6]; }
  else                                     { wr_data0 = wr_data[3];  wr_data1 = wr_data[7]; }
}

// ------------------------------------------------------------
// Multiplex Buffer Read Data
// ------------------------------------------------------------

void mux_rddata( TT_DATA (&read_data)[8], ap_uint<2> (&rd_off)[4], int val, TT_DATA (&trans_o)[8] )
{
  if      ( rd_off[0] == ap_uint<2>(val) ) { trans_o[0] = read_data[val<<1];  trans_o[4] = read_data[(val<<1)+1]; }
  else if ( rd_off[1] == ap_uint<2>(val) ) { trans_o[1] = read_data[val<<1];  trans_o[5] = read_data[(val<<1)+1]; }
  else if ( rd_off[2] == ap_uint<2>(val) ) { trans_o[2] = read_data[val<<1];  trans_o[6] = read_data[(val<<1)+1]; }
  else                                     { trans_o[3] = read_data[val<<1];  trans_o[7] = read_data[(val<<1)+1]; }
}

// ------------------------------------------------------------
// Unpack & store samples
// ------------------------------------------------------------

void pfa1008_unpack( TT_STREAM sig_i[SSR_IO], TT_DATA (&trans)[8] )
{
  static ap_uint<3>  fsm_state = 0;
  static bool        running = 0;
  static TT_DATA     save0[8], save1[8];
#pragma HLS reset variable=fsm_state
#pragma HLS array_partition variable=save0 dim=1
#pragma HLS array_partition variable=save1 dim=1

  // ==================== UNPACK ====================

  // Unpack samples from input streams (incoming alternating transforms on ss0 then ss1):
  TT_DATA inss0[4], inss1[4];
#pragma HLS array_partition variable=inss0 dim=1
#pragma HLS array_partition variable=inss1 dim=1
  ( inss0[3], inss0[2], inss0[1], inss0[0] ) = sig_i[0].read();
  ( inss1[3], inss1[2], inss1[1], inss1[0] ) = sig_i[1].read();
  // cout << "stream inputs: "
  //      << inss1[3] << " " << inss1[2] << " " << inss1[1] << " " << inss1[0] << " "
  //      << inss0[3] << " " << inss0[2] << " " << inss0[1] << " " << inss0[0] << endl;

  // ==================== REPACKAGE ====================
  // Introduces a latency of 8 samples (one PL cycle)
  switch ( fsm_state ) {
  case 0:
    trans[7] = save0[7];
    trans[6] = save0[6];
    trans[5] = save0[5];
    trans[4] = save0[4];
    trans[3] = save1[7];
    trans[2] = save1[6];
    trans[1] = save1[5];
    trans[0] = save0[3]; break;
  case 1:
    trans[7] = save0[4];
    trans[6] = inss0[2];
    trans[5] = inss0[1];
    trans[4] = inss0[0];
    trans[3] = save0[3];
    trans[2] = save0[2];
    trans[1] = save0[1];
    trans[0] = save0[0]; break;
  case 2:
    trans[7] = inss0[0];
    trans[6] = save0[3];
    trans[5] = save0[6];
    trans[4] = save0[5];
    trans[3] = save0[4];
    trans[2] = save1[7];
    trans[1] = save1[6];
    trans[0] = save1[5]; break;
  case 3:
    trans[7] = save0[5];
    trans[6] = save0[4];
    trans[5] = save1[7];
    trans[4] = inss0[1];
    trans[3] = inss0[0];
    trans[2] = save0[3];
    trans[1] = save0[2];
    trans[0] = save0[1]; break;
  case 4:
    trans[7] = inss0[1];
    trans[6] = inss0[0];
    trans[5] = save0[3];
    trans[4] = save0[2];
    trans[3] = save0[5];
    trans[2] = save0[4];
    trans[1] = save1[7];
    trans[0] = save1[6]; break;
  case 5:
    trans[7] = save0[6];
    trans[6] = save0[5];
    trans[5] = save0[4];
    trans[4] = save1[7];
    trans[3] = save1[6];
    trans[2] = inss0[0];
    trans[1] = save0[3];
    trans[0] = save0[2]; break;
  case 6:
    trans[7] = inss0[2];
    trans[6] = inss0[1];
    trans[5] = inss0[0];
    trans[4] = save0[3];
    trans[3] = save0[2];
    trans[2] = save0[1];
    trans[1] = save0[4];
    trans[0] = save1[7]; break;
  }
  // cout << "input order: "
  //      << trans[0] << " " << trans[1] << " " << trans[2] << " " << trans[3] << " "
  //      << trans[4] << " " << trans[5] << " " << trans[6] << " " << trans[7] << endl;

  //==================== UPDATE FSM ====================
  // Update 'save1'
 UPDATE1: for (int ss=0; ss < 8; ss++) {
    save1[ss] = save0[ss];
    save0[ss] = ( ss < 4 ) ? inss0[ss] : inss1[ss-4];
  }
  // Update 'fsm_state'
  fsm_state = ( fsm_state == ap_uint<3>(6) ) ? ap_uint<3>(0) : ap_uint<3>(fsm_state+1);
}

// ------------------------------------------------------------
// Transpose Samples:
// ------------------------------------------------------------

void pfa1008_transpose( TT_DATA (&trans_i)[8], TT_DATA (&trans_o)[8] )
{
  // There is a startup latency of 1 cycle here from unpacking:
  static bool    running = 0;
  static bool    ping = 0;
#pragma HLS reset variable=ping
#pragma HLS reset variable=running

  static TT_ADDR wr_addr = 0;
#pragma HLS reset variable=wr_addr

  static TT_ADDR rd_addr = 0;
  static TT_ADDR rd_start = 1;
#pragma HLS reset variable=rd_addr
#pragma HLS reset variable=rd_start

  static TT_DATA buff[2][4][NFFT/4];
#pragma HLS array_partition variable=buff dim=1
#pragma HLS array_partition variable=buff dim=2
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=bram
#pragma HLS dependence variable=buff type=intra false

  // ==================== READ & WRITE BUFFER ====================

  TT_INDEX wr_idx[8], rd_idx[8];
  TT_INDEX wr_idx0, wr_idx1, wr_idx2, wr_idx3, wr_idx4, wr_idx5, wr_idx6, wr_idx7;
  TT_INDEX rd_idx0, rd_idx1, rd_idx2, rd_idx3, rd_idx4, rd_idx5, rd_idx6, rd_idx7;
  ap_uint<2> wr_off[4], rd_off[4];
  TT_DATA read_data[8], wr_data0, wr_data1, wr_data2, wr_data3, wr_data4, wr_data5, wr_data6, wr_data7;
  TT_DATA rd_data0, rd_data1, rd_data2, rd_data3, rd_data4, rd_data5, rd_data7;
#pragma HLS array_partition variable=wr_idx dim=1
#pragma HLS array_partition variable=rd_idx dim=1
#pragma HLS array_partition variable=wr_off dim=1
#pragma HLS array_partition variable=rd_off dim=1
#pragma HLS array_partition variable=read_data dim=1

 READ_WRITE : for (int ss=0,offW=0,offR=0; ss < 8; ss++,offW++,offR+=7) {
    TT_ADDR wr_addr_use = TT_ADDR(wr_addr + offW);
    TT_ADDR rd_addr_use = TT_ADDR(rd_addr + offR);
    wr_idx[ss].range(7,0) = wr_addr_use.range(9,2);
    rd_idx[ss].range(7,0) = rd_addr_use.range(9,2);
    if ( ss < 4 ) {
      wr_off[ss].range(1,0) = wr_addr_use.range(1,0);
      rd_off[ss].range(1,0) = rd_addr_use.range(1,0);
    }
  }

    // Buffer Write Muxing:
  mux_index( wr_idx, wr_off, 0, wr_idx0, wr_idx1 );
  mux_index( wr_idx, wr_off, 1, wr_idx2, wr_idx3 );
  mux_index( wr_idx, wr_off, 2, wr_idx4, wr_idx5 );
  mux_index( wr_idx, wr_off, 3, wr_idx6, wr_idx7 );

  mux_wrdata( trans_i, wr_off, 0, wr_data0, wr_data1 );
  mux_wrdata( trans_i, wr_off, 1, wr_data2, wr_data3 );
  mux_wrdata( trans_i, wr_off, 2, wr_data4, wr_data5 );
  mux_wrdata( trans_i, wr_off, 3, wr_data6, wr_data7 );

  // Buffer Write:
  buff[ping][0][wr_idx0] = wr_data0;
  buff[ping][0][wr_idx1] = wr_data1;
  buff[ping][1][wr_idx2] = wr_data2;
  buff[ping][1][wr_idx3] = wr_data3;
  buff[ping][2][wr_idx4] = wr_data4;
  buff[ping][2][wr_idx5] = wr_data5;
  buff[ping][3][wr_idx6] = wr_data6;
  buff[ping][3][wr_idx7] = wr_data7;

  // Buffer Read Muxing:
  mux_index( rd_idx, rd_off, 0, rd_idx0, rd_idx1 );
  mux_index( rd_idx, rd_off, 1, rd_idx2, rd_idx3 );
  mux_index( rd_idx, rd_off, 2, rd_idx4, rd_idx5 );
  mux_index( rd_idx, rd_off, 3, rd_idx6, rd_idx7 );

  // Buffer Read:
  read_data[0] = buff[!ping][0][rd_idx0];
  read_data[1] = buff[!ping][0][rd_idx1];
  read_data[2] = buff[!ping][1][rd_idx2];
  read_data[3] = buff[!ping][1][rd_idx3];
  read_data[4] = buff[!ping][2][rd_idx4];
  read_data[5] = buff[!ping][2][rd_idx5];
  read_data[6] = buff[!ping][3][rd_idx6];
  read_data[7] = buff[!ping][3][rd_idx7];

  // Buffer Read Data Muxing:
  mux_rddata( read_data, rd_off, 0, trans_o );
  mux_rddata( read_data, rd_off, 1, trans_o );
  mux_rddata( read_data, rd_off, 2, trans_o );
  mux_rddata( read_data, rd_off, 3, trans_o );

  // cout << "trans_o: "
  //      << trans_o[0] << " " << trans_o[1] << " " << trans_o[2] << " " << trans_o[3] << " "
  //      << trans_o[4] << " " << trans_o[5] << " " << trans_o[6] << " " << trans_o[7] << endl;

  // ==================== Update State ====================
  if ( running == 1 ) {
    // Update read address:
    bool last_rd = ( rd_addr >= TT_ADDR(NFFT-8*7) );
    if ( last_rd == 1 ) {
      if ( rd_start == TT_ADDR(7) ) {
        rd_start = 1;
        rd_addr = 0;
      }
      else {
        rd_addr = rd_start;
        rd_start++;
      }
    }
    else {
      rd_addr += TT_ADDR(8*7);  // 8 hops of Stride-7
    }

    // Update write address:
    bool last_wr = ( wr_addr == TT_ADDR(NFFT-8) );
    ping = ( last_wr == 1 ) ? !ping : ping;
    wr_addr = ( last_wr == 1 ) ? TT_ADDR(0) : TT_ADDR(wr_addr+8);
  } // running
  running = 1;
}

// ------------------------------------------------------------
// Write output streams
// ------------------------------------------------------------

void pfa1008_write_streams( TT_DATA (&trans)[8], TT_STREAM sig_o[SSR_IO] )
{
  // Need to write 9-point transforms alternately to ss0 and ss1:
  static bool        running = 0;
  static ap_uint<4>  fsm_state = 0;
  static ap_uint<10> startup = 0; // Count off first block of "don't cares" from buffer plus latency below:
  static TT_DATA     save0[8], save1[8];
#pragma HLS reset variable=running
#pragma HLS reset variable=fsm_state
#pragma HLS reset variable=startup
#pragma HLS array_partition variable=save0 dim=1
#pragma HLS array_partition variable=save1 dim=1

  // cout << "startup: " << startup << " fsm: " << fsm_state << " running: " << running << " data:  "
  //      << trans[0] << " " << trans[1] << " " << trans[2] << " " << trans[3] << " "
  //      << trans[4] << " " << trans[5] << " " << trans[6] << " " << trans[7] << endl;

  // Collect stream samples:
  // --> Latency of 8 samples (1 PL cycle)
  TT_AXI4 ss0_data;
  switch ( fsm_state ) {
  case 0:  ss0_data = ( save1[6], save1[5], save1[4], save1[3] ); break;
  case 1:  ss0_data = ( save0[3], save0[2], save0[1], save0[0] ); break;
  case 2:  ss0_data = ( save1[7], save1[6], save1[5], save1[4] ); break;
  case 3:  ss0_data = ( save0[4], save0[3], save0[2], save1[0] ); break;
  case 4:  ss0_data = ( save0[0], save1[7], save1[6], save1[5] ); break;
  case 5:  ss0_data = ( save0[5], save0[4], save1[2], save1[1] ); break;
  case 6:  ss0_data = ( save0[1], save0[0], save1[7], save1[6] ); break;
  case 7:  ss0_data = ( save0[6], save1[4], save1[3], save1[2] ); break;
  case 8:  ss0_data = ( save0[2], save0[1], save0[0], save1[7] ); break;
  }
  TT_AXI4 ss1_data;
  switch ( fsm_state ) {
  case 0:  ss1_data = ( save0[7], save0[6], save0[5], save0[4] ); break;
  case 1:  ss1_data = ( trans[4], trans[3], trans[2], trans[1] ); break;
  case 2:  ss1_data = ( trans[0], save0[7], save0[6], save0[5] ); break;
  case 3:  ss1_data = ( trans[5], trans[4], trans[3], save0[1] ); break;
  case 4:  ss1_data = ( trans[1], trans[0], save0[7], save0[6] ); break;
  case 5:  ss1_data = ( trans[6], trans[5], save0[3], save0[2] ); break;
  case 6:  ss1_data = ( trans[2], trans[1], trans[0], save0[7] ); break;
  case 7:  ss1_data = ( trans[7], save0[5], save0[4], save0[3] ); break;
  case 8:  ss1_data = ( trans[3], trans[2], trans[1], trans[0] ); break;
  }

  // Write streams:
  if ( running == 1 ) {
    sig_o[0].write( ss0_data );
    sig_o[1].write( ss1_data );
  }

  if ( startup == ap_uint<10>(NFFT/8+1) ) {
    running = 1;
  }
  else {
    startup = ap_uint<10>(startup + 1);
  }

  if ( running == 1 ) {
    fsm_state = ( fsm_state == ap_uint<4>(8) ) ? ap_uint<4>(0) : ap_uint<4>(fsm_state+1);
  }

  // Update state:
 STATE : for (int ss=0; ss < 8; ss++) {
    save1[ss] = save0[ss];
    save0[ss] = trans[ss];
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void pfa1008_transpose1_wrapper( TT_STREAM sig_i[SSR_IO], TT_STREAM sig_o[SSR_IO] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS pipeline II=1

  TT_DATA transpose_i[8], transpose_o[8];
#pragma HLS array_partition variable=transpose_i dim=1
#pragma HLS array_partition variable=transpose_o dim=1

  // Unpack samples:
  pfa1008_unpack( sig_i, transpose_i );

  // Transpose samples:
  pfa1008_transpose( transpose_i, transpose_o );

  // Format output streams:
  pfa1008_write_streams( transpose_o, sig_o );
}



