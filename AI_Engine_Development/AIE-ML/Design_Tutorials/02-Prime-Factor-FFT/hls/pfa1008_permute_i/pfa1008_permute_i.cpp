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

void pfa1008_unpack( TT_STREAM& sig_i, TT_DATA (&permute_i)[4] )
{
  // Unpack samples from input streams:
  ( permute_i[3], permute_i[2], permute_i[1], permute_i[0] ) = sig_i.read();
}

// ------------------------------------------------------------
// Permute
// ------------------------------------------------------------

void pfa1008_permute( TT_DATA (&permute_i)[4], TT_DATA (&permute_o)[4] )
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
  static TT_ADDR    permute_lut[2][NFFT] = { PERM_I_ADDR, PERM_I_ADDR };
#pragma HLS array_partition variable=permute_lut dim=1
#pragma HLS bind_storage variable=permute_lut type=RAM_T2P impl=bram
  ap_uint<2> offset[4];
  TT_INDEX addr[4];
  TT_DATA data0[4], data1[4], data2[4], data3[4];
#pragma HLS array_partition variable=offset dim=1
#pragma HLS array_partition variable=addr dim=1
#pragma HLS array_partition variable=data0 dim=1
#pragma HLS array_partition variable=data1 dim=1
#pragma HLS array_partition variable=data2 dim=1
#pragma HLS array_partition variable=data3 dim=1

  // ==================== PERMUTE READ ADDRESS ====================

 PERMUTE : for (int ss=0; ss < 4; ss++) {
    TT_ADDR rd_addr_use = permute_lut[ss>>1][rd_addr+ss];
    addr[ss].range(7,0)   = rd_addr_use.range(9,2);
    offset[ss].range(1,0) = rd_addr_use.range(1,0);
    // cout << "rd_addr_use: " << rd_addr_use << "  addr: " << addr[ss] << "  offset: " << offset[ss] << endl;
  }

  // ==================== READ & WRITE BUFFER ====================

  // Store 2 copies to buffer in linear order:
  TT_INDEX wr_addrA; wr_addrA.range(7,0) = (wr_addr  ).range(9,2);
  buff[ping][0][0][wr_addrA] = permute_i[0];
  buff[ping][0][1][wr_addrA] = permute_i[0];
  buff[ping][0][2][wr_addrA] = permute_i[0];
  buff[ping][0][3][wr_addrA] = permute_i[0];

  buff[ping][1][0][wr_addrA] = permute_i[1];
  buff[ping][1][1][wr_addrA] = permute_i[1];
  buff[ping][1][2][wr_addrA] = permute_i[1];
  buff[ping][1][3][wr_addrA] = permute_i[1];

  buff[ping][2][0][wr_addrA] = permute_i[2];
  buff[ping][2][1][wr_addrA] = permute_i[2];
  buff[ping][2][2][wr_addrA] = permute_i[2];
  buff[ping][2][3][wr_addrA] = permute_i[2];

  buff[ping][3][0][wr_addrA] = permute_i[3];
  buff[ping][3][1][wr_addrA] = permute_i[3];
  buff[ping][3][2][wr_addrA] = permute_i[3];
  buff[ping][3][3][wr_addrA] = permute_i[3];

  // Read all possibilities:
  data0[0] = buff[!ping][0][0][addr[0]];
  data0[1] = buff[!ping][0][1][addr[1]];
  data0[2] = buff[!ping][0][2][addr[2]];
  data0[3] = buff[!ping][0][3][addr[3]];

  data1[0] = buff[!ping][1][0][addr[0]];
  data1[1] = buff[!ping][1][1][addr[1]];
  data1[2] = buff[!ping][1][2][addr[2]];
  data1[3] = buff[!ping][1][3][addr[3]];

  data2[0] = buff[!ping][2][0][addr[0]];
  data2[1] = buff[!ping][2][1][addr[1]];
  data2[2] = buff[!ping][2][2][addr[2]];
  data2[3] = buff[!ping][2][3][addr[3]];

  data3[0] = buff[!ping][3][0][addr[0]];
  data3[1] = buff[!ping][3][1][addr[1]];
  data3[2] = buff[!ping][3][2][addr[2]];
  data3[3] = buff[!ping][3][3][addr[3]];

  // Apply polyphase muxes:
 MUX_ALL : for (int ss=0; ss < 4; ss++) {
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
  bool last_wr = ( wr_addr == TT_ADDR(NFFT-4) );
  bool last_rd = ( rd_addr == TT_ADDR(NFFT-4) );
  ping    = ( last_wr == 1 ) ? !ping      : ping;
  wr_addr = ( last_wr == 1 ) ? TT_ADDR(0) : TT_ADDR(wr_addr+4);
  rd_addr = ( last_rd == 1 ) ? TT_ADDR(0) : TT_ADDR(rd_addr+4);
}

// ------------------------------------------------------------
// Write output streams
// ------------------------------------------------------------

void pfa1008_write_streams( TT_DATA (&permI)[4], TT_STREAM& sig_o )
{
  static bool        running = 0;
  static ap_uint<10> startup = 0; // Count off first block of "don't cares" from buffer:
#pragma HLS reset variable=running
#pragma HLS reset variable=startup

  // cout << "startup: " << startup << "  fsm_state: " << fsm_state << "  running: " << running << "  permute_o:  "
  //      << permI[0] << " " << permI[1] << " " << permI[2] << " " << permI[3] << " "
  //      << permI[4] << " " << permI[5] << " " << permI[6] << " " << permI[7] << endl;

  // Collect stream samples:
  TT_AXI4 ss0_data = ( permI[3], permI[2], permI[1], permI[0] );

  // Write streams:
  if ( running == 1 ) {
    sig_o.write( ss0_data );
  }

  // Update startup count:
  if ( startup == ap_uint<10>(NFFT/4-1) ) {
    running = 1;
  }
  else {
    startup = ap_uint<10>(startup + 1);
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void pfa1008_permute_i_wrapper( TT_STREAM& sig_i, TT_STREAM& sig_o )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS pipeline II=1

  static TT_DATA permute_i[4], permute_o[4];
#pragma HLS array_partition variable=permute_i dim=1
#pragma HLS array_partition variable=permute_o dim=1

  // Unpack & store samples:
  pfa1008_unpack( sig_i, permute_i );

  // Permute:
  pfa1008_permute( permute_i, permute_o );

  // Format output streams:
  pfa1008_write_streams( permute_o, sig_o );
}



