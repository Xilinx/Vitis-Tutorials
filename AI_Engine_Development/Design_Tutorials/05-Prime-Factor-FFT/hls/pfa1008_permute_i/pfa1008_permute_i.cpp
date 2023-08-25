//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "pfa1008_permute_i.h"
#include "pfa1008_permute_i_luts.h"

using namespace std;
using namespace pfa1008_permute_i;

// ------------------------------------------------------------
// Unpack & store samples
// ------------------------------------------------------------

void pfa1008_unpack( TT_STREAM sig_i[SSR_IO], TT_DATA (&permute_i)[8] )
{
  // Unpack samples from input streams (coming in polyphase order):
  ( permute_i[6], permute_i[4], permute_i[2], permute_i[0] ) = sig_i[0].read();
  ( permute_i[7], permute_i[5], permute_i[3], permute_i[1] ) = sig_i[1].read();
}

void pfa1008_permute( TT_DATA (&permute_i)[8], TT_DATA (&permute_o)[8] )
{
  static bool    ping = 0;
  static TT_ADDR wr_addr = 0;
  static TT_ADDR rd_addr = 0;
#pragma HLS reset variable=ping
#pragma HLS reset variable=wr_addr
#pragma HLS reset variable=rd_addr
  static TT_DATA buff[2][4][4][NFFT/4];
#pragma HLS array_partition variable=buff dim=1
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=bram
#pragma HLS dependence variable=buff type=intra false
  static TT_ADDR    permute_lut[4][NFFT] = { PERM_I_ADDR, PERM_I_ADDR, PERM_I_ADDR, PERM_I_ADDR };
#pragma HLS array_partition variable=permute_lut dim=1
#pragma HLS bind_storage variable=permute_lut type=RAM_T2P impl=bram
  ap_uint<2> offset[8];
  TT_INDEX addr[8];
  TT_DATA data0[8], data1[8], data2[8], data3[8];
#pragma HLS array_partition variable=offset dim=1
#pragma HLS array_partition variable=addr dim=1
#pragma HLS array_partition variable=data0 dim=1
#pragma HLS array_partition variable=data1 dim=1
#pragma HLS array_partition variable=data2 dim=1
#pragma HLS array_partition variable=data3 dim=1

  // ==================== PERMUTE READ ADDRESS ====================

 PERMUTE : for (int ss=0; ss < 8; ss++) {
    TT_ADDR rd_addr_use = permute_lut[ss>>1][rd_addr+ss];
    addr[ss].range(7,0)   = rd_addr_use.range(9,2);
    offset[ss].range(1,0) = rd_addr_use.range(1,0);
    // cout << "rd_addr_use: " << rd_addr_use << "  addr: " << addr[ss] << "  offset: " << offset[ss] << endl;
  }

  // ==================== READ & WRITE BUFFER ====================

  // Store 8 copies to buffer in linear order:
  TT_INDEX wr_addrA; wr_addrA.range(7,0) = (wr_addr  ).range(9,2);
  TT_INDEX wr_addrB; wr_addrB.range(7,0) = (wr_addr+4).range(9,2);
  buff[ping][0][0][wr_addrA] = permute_i[0];
  buff[ping][0][1][wr_addrA] = permute_i[0];
  buff[ping][0][2][wr_addrA] = permute_i[0];
  buff[ping][0][3][wr_addrA] = permute_i[0];

  buff[ping][0][0][wr_addrB] = permute_i[4];
  buff[ping][0][1][wr_addrB] = permute_i[4];
  buff[ping][0][2][wr_addrB] = permute_i[4];
  buff[ping][0][3][wr_addrB] = permute_i[4];

  buff[ping][1][0][wr_addrA] = permute_i[1];
  buff[ping][1][1][wr_addrA] = permute_i[1];
  buff[ping][1][2][wr_addrA] = permute_i[1];
  buff[ping][1][3][wr_addrA] = permute_i[1];

  buff[ping][1][0][wr_addrB] = permute_i[5];
  buff[ping][1][1][wr_addrB] = permute_i[5];
  buff[ping][1][2][wr_addrB] = permute_i[5];
  buff[ping][1][3][wr_addrB] = permute_i[5];

  buff[ping][2][0][wr_addrA] = permute_i[2];
  buff[ping][2][1][wr_addrA] = permute_i[2];
  buff[ping][2][2][wr_addrA] = permute_i[2];
  buff[ping][2][3][wr_addrA] = permute_i[2];

  buff[ping][2][0][wr_addrB] = permute_i[6];
  buff[ping][2][1][wr_addrB] = permute_i[6];
  buff[ping][2][2][wr_addrB] = permute_i[6];
  buff[ping][2][3][wr_addrB] = permute_i[6];

  buff[ping][3][0][wr_addrA] = permute_i[3];
  buff[ping][3][1][wr_addrA] = permute_i[3];
  buff[ping][3][2][wr_addrA] = permute_i[3];
  buff[ping][3][3][wr_addrA] = permute_i[3];

  buff[ping][3][0][wr_addrB] = permute_i[7];
  buff[ping][3][1][wr_addrB] = permute_i[7];
  buff[ping][3][2][wr_addrB] = permute_i[7];
  buff[ping][3][3][wr_addrB] = permute_i[7];

  // Read all possibilities:
  data0[0] = buff[!ping][0][0][addr[0]];
  data0[1] = buff[!ping][0][0][addr[1]];
  data0[2] = buff[!ping][0][1][addr[2]];
  data0[3] = buff[!ping][0][1][addr[3]];
  data0[4] = buff[!ping][0][2][addr[4]];
  data0[5] = buff[!ping][0][2][addr[5]];
  data0[6] = buff[!ping][0][3][addr[6]];
  data0[7] = buff[!ping][0][3][addr[7]];

  data1[0] = buff[!ping][1][0][addr[0]];
  data1[1] = buff[!ping][1][0][addr[1]];
  data1[2] = buff[!ping][1][1][addr[2]];
  data1[3] = buff[!ping][1][1][addr[3]];
  data1[4] = buff[!ping][1][2][addr[4]];
  data1[5] = buff[!ping][1][2][addr[5]];
  data1[6] = buff[!ping][1][3][addr[6]];
  data1[7] = buff[!ping][1][3][addr[7]];

  data2[0] = buff[!ping][2][0][addr[0]];
  data2[1] = buff[!ping][2][0][addr[1]];
  data2[2] = buff[!ping][2][1][addr[2]];
  data2[3] = buff[!ping][2][1][addr[3]];
  data2[4] = buff[!ping][2][2][addr[4]];
  data2[5] = buff[!ping][2][2][addr[5]];
  data2[6] = buff[!ping][2][3][addr[6]];
  data2[7] = buff[!ping][2][3][addr[7]];

  data3[0] = buff[!ping][3][0][addr[0]];
  data3[1] = buff[!ping][3][0][addr[1]];
  data3[2] = buff[!ping][3][1][addr[2]];
  data3[3] = buff[!ping][3][1][addr[3]];
  data3[4] = buff[!ping][3][2][addr[4]];
  data3[5] = buff[!ping][3][2][addr[5]];
  data3[6] = buff[!ping][3][3][addr[6]];
  data3[7] = buff[!ping][3][3][addr[7]];

  // Apply polyphase muxes:
 MUX_ALL : for (int ss=0; ss < 8; ss++) {
    switch ( offset[ss] ) {
    case 0 : permute_o[ss] = data0[ss]; break;
    case 1 : permute_o[ss] = data1[ss]; break;
    case 2 : permute_o[ss] = data2[ss]; break;
    default: permute_o[ss] = data3[ss]; break;
    }
    // cout << permute_o[ss] << " ";
  }
  // cout << endl;

  // ==================== Update State ====================
  bool last_wr = ( wr_addr == TT_ADDR(NFFT-8) );
  bool last_rd = ( rd_addr == TT_ADDR(NFFT-8) );
  ping    = ( last_wr == 1 ) ? !ping      : ping;
  wr_addr = ( last_wr == 1 ) ? TT_ADDR(0) : TT_ADDR(wr_addr+8);
  rd_addr = ( last_rd == 1 ) ? TT_ADDR(0) : TT_ADDR(rd_addr+8);
}

// ------------------------------------------------------------
// Write output streams
// ------------------------------------------------------------

void pfa1008_write_streams( TT_DATA (&permI)[8], TT_STREAM sig_o[SSR_IO] )
{
  static bool        running = 0;
  static ap_uint<3>  fsm_state = 0;
  static ap_uint<10> startup = 0; // Count off first block of "don't cares" from buffer plus latency below:
  static TT_DATA     save0[8], save1[8];
#pragma HLS reset variable=running
#pragma HLS reset variable=fsm_state
#pragma HLS array_partition variable=save0 dim=1
#pragma HLS array_partition variable=save1 dim=1

  // cout << "startup: " << startup << "  fsm_state: " << fsm_state << "  running: " << running << "  permute_o:  "
  //      << permI[0] << " " << permI[1] << " " << permI[2] << " " << permI[3] << " "
  //      << permI[4] << " " << permI[5] << " " << permI[6] << " " << permI[7] << endl;

  // Collect stream samples:
  // --> this introduces 8 samples of latency (single PL cycle)
  TT_AXI4 ss0_data;
  switch ( fsm_state ) {
  case 0:  ss0_data = ( save0[0], save1[7], save1[6], save1[5] ); break;
  case 1:  ss0_data = ( save0[3], save0[2], save0[1], save0[0] ); break;
  case 2:  ss0_data = ( save0[6], save1[6], save1[5], save1[4] ); break;
  case 3:  ss0_data = ( save0[2], save0[1], save0[0], save1[7] ); break;
  case 4:  ss0_data = ( save0[5], save0[4], save1[4], save1[3] ); break;
  case 5:  ss0_data = ( save0[1], save0[0], save1[7], save1[6] ); break;
  default: ss0_data = ( save0[4], save0[3], save0[2], save1[2] ); break;
  }
  TT_AXI4 ss1_data;
  switch ( fsm_state ) {
  case 0:  ss1_data = ( save0[7], save0[6], save0[5], save0[4] ); break;
  case 1:  ss1_data = ( permI[2], permI[1], permI[0], save0[7] ); break;
  case 2:  ss1_data = ( permI[5], save0[5], save0[4], save0[3] ); break;
  case 3:  ss1_data = ( permI[1], permI[0], save0[7], save0[6] ); break;
  case 4:  ss1_data = ( permI[4], permI[3], save0[3], save0[2] ); break;
  case 5:  ss1_data = ( permI[0], save0[7], save0[6], save0[5] ); break;
  default: ss1_data = ( permI[3], permI[2], permI[1], save0[1] ); break;
  }

  // Write streams:
  // --> Don't write the first sample (it's due to latency) and we want to
  //     keep the FFT boundaries aligned!
  if ( running == 1 ) {
    sig_o[0].write( ss0_data );
    sig_o[1].write( ss1_data );
  }

  // Update startup count:
  if ( startup == ap_uint<10>(NFFT/8) ) {
    running = 1;
  }
  else {
    startup = ap_uint<10>(startup + 1);
  }

  if ( running == 1 ) {
    fsm_state = ( fsm_state == ap_uint<3>(6) ) ? ap_uint<3>(0) : ap_uint<3>(fsm_state+1);
  }

  // Update state:
 STATE : for (int ss=0; ss < 8; ss++) {
    save1[ss] = save0[ss];
    save0[ss] = permI[ss];
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void pfa1008_permute_i_wrapper( TT_STREAM sig_i[SSR_IO], TT_STREAM sig_o[SSR_IO] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS pipeline II=1

  static TT_DATA permute_i[8], permute_o[8];
#pragma HLS array_partition variable=permute_i dim=1
#pragma HLS array_partition variable=permute_o dim=1

  // Unpack & store samples:
  pfa1008_unpack( sig_i, permute_i );

  // Permute:
  pfa1008_permute( permute_i, permute_o );

  // Format output streams:
  pfa1008_write_streams( permute_o, sig_o );
}



