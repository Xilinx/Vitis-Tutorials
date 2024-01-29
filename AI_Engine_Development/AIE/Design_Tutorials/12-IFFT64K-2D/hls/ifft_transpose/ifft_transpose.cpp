//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "ifft_transpose.h"

using namespace std;
using namespace ifft_transpose;

// Approach:
// 0) 5 matrix rows incoming (polyphase row order) at two samples per PL cycle
// 1) Read 2 samples each from 5 input streams coming from 2D matrix rows
// 2) Write 10 samples into 5 memory banks, with cyclic rotating bank ID to allow simultaneous R/W via rows or columns
// 3) Write 2 samples to 5 output streams per PL cycle

// ------------------------------------------------------------
// Unpack & store samples
// ------------------------------------------------------------

void ifft_unpack( TT_STREAM sig_i[NSTREAM], TT_SAMPLE (&trans_i)[2*NSTREAM] )
{
 READ: for (int ss=0; ss < NSTREAM; ss++) {
    ( trans_i[(ss<<1)+1], trans_i[(ss<<1)+0] ) = sig_i[ss].read();
    // std::cout << "val0: " << trans_i[(ss<<1)] << " val1: " << trans_i[(ss<<1)+1] << std::endl;
  }
}

// ------------------------------------------------------------
// Transpose Samples:
// ------------------------------------------------------------

void ifft_do_transpose( TT_SAMPLE (&trans_i)[2*NSTREAM], TT_SAMPLE (&trans_o)[2*NSTREAM] )
{
  static bool ping = 0;
  static TT_SAMPLE buff[2][NSTREAM][DEPTH*DEPTH/NSTREAM];
#pragma HLS array_partition variable=buff dim=1 type=complete
#pragma HLS array_partition variable=buff dim=2 type=complete
#pragma HLS bind_storage variable=buff type=RAM_T2P impl=uram
#pragma HLS dependence variable=buff type=intra false

  static int wr_bank_addr = 0;
  static int phase = 0;
#pragma HLS reset variable=wr_bank_addr
#pragma HLS reset variable=phase

  static int rd_bank_addr = 0;
  static int rd_grp = 0;
  static int rd_col = 0;
  static int rd_row = 0;
#pragma HLS reset variable=rd_bank_addr
#pragma HLS reset variable=rd_grp
#pragma HLS reset variable=rd_col
#pragma HLS reset variable=rd_row

  // Write data mux:
  TT_SAMPLE wr_data0[NSTREAM], wr_data1[NSTREAM];
  switch (phase) {
  case 0:
    wr_data0[0] = trans_i[0];   wr_data1[4] = trans_i[1];
    wr_data0[1] = trans_i[2];   wr_data1[0] = trans_i[3];
    wr_data0[2] = trans_i[4];   wr_data1[1] = trans_i[5];
    wr_data0[3] = trans_i[6];   wr_data1[2] = trans_i[7];
    wr_data0[4] = trans_i[8];   wr_data1[3] = trans_i[9];
    break;
  case 1:
    wr_data0[3] = trans_i[0];   wr_data1[2] = trans_i[1];
    wr_data0[4] = trans_i[2];   wr_data1[3] = trans_i[3];
    wr_data0[0] = trans_i[4];   wr_data1[4] = trans_i[5];
    wr_data0[1] = trans_i[6];   wr_data1[0] = trans_i[7];
    wr_data0[2] = trans_i[8];   wr_data1[1] = trans_i[9];
    break;
  case 2:
    wr_data0[1] = trans_i[0];   wr_data1[0] = trans_i[1];
    wr_data0[2] = trans_i[2];   wr_data1[1] = trans_i[3];
    wr_data0[3] = trans_i[4];   wr_data1[2] = trans_i[5];
    wr_data0[4] = trans_i[6];   wr_data1[3] = trans_i[7];
    wr_data0[0] = trans_i[8];   wr_data1[4] = trans_i[9];
    break;
  case 3:
    wr_data0[4] = trans_i[0];   wr_data1[3] = trans_i[1];
    wr_data0[0] = trans_i[2];   wr_data1[4] = trans_i[3];
    wr_data0[1] = trans_i[4];   wr_data1[0] = trans_i[5];
    wr_data0[2] = trans_i[6];   wr_data1[1] = trans_i[7];
    wr_data0[3] = trans_i[8];   wr_data1[2] = trans_i[9];
    break;
  default:
    wr_data0[2] = trans_i[0];   wr_data1[1] = trans_i[1];
    wr_data0[3] = trans_i[2];   wr_data1[2] = trans_i[3];
    wr_data0[4] = trans_i[4];   wr_data1[3] = trans_i[5];
    wr_data0[0] = trans_i[6];   wr_data1[4] = trans_i[7];
    wr_data0[1] = trans_i[8];   wr_data1[0] = trans_i[9];
    break;
  }

  // Read address mux:
  int offset = ( rd_grp == (NSTREAM-1) ) ? DEPTH : 0;
  int rd_addr0[NSTREAM], rd_addr1[NSTREAM];
  switch (phase) {
  case 0:
    rd_addr0[0] = rd_bank_addr + 0;   rd_addr1[0] = rd_bank_addr + 1 + offset;
    rd_addr0[1] = rd_bank_addr + 4;   rd_addr1[1] = rd_bank_addr + 0 + offset;
    rd_addr0[2] = rd_bank_addr + 3;   rd_addr1[2] = rd_bank_addr + 4 + offset;
    rd_addr0[3] = rd_bank_addr + 2;   rd_addr1[3] = rd_bank_addr + 3 + offset;
    rd_addr0[4] = rd_bank_addr + 1;   rd_addr1[4] = rd_bank_addr + 2 + offset;
    break;
  case 1:
    rd_addr0[0] = rd_bank_addr + 2;   rd_addr1[0] = rd_bank_addr + 3 + offset;
    rd_addr0[1] = rd_bank_addr + 1;   rd_addr1[1] = rd_bank_addr + 2 + offset;
    rd_addr0[2] = rd_bank_addr + 0;   rd_addr1[2] = rd_bank_addr + 1 + offset;
    rd_addr0[3] = rd_bank_addr + 4;   rd_addr1[3] = rd_bank_addr + 0 + offset;
    rd_addr0[4] = rd_bank_addr + 3;   rd_addr1[4] = rd_bank_addr + 4 + offset;
    break;
  case 2:
    rd_addr0[0] = rd_bank_addr + 4;   rd_addr1[0] = rd_bank_addr + 0 + offset;
    rd_addr0[1] = rd_bank_addr + 3;   rd_addr1[1] = rd_bank_addr + 4 + offset;
    rd_addr0[2] = rd_bank_addr + 2;   rd_addr1[2] = rd_bank_addr + 3 + offset;
    rd_addr0[3] = rd_bank_addr + 1;   rd_addr1[3] = rd_bank_addr + 2 + offset;
    rd_addr0[4] = rd_bank_addr + 0;   rd_addr1[4] = rd_bank_addr + 1 + offset;
    break;
  case 3:
    rd_addr0[0] = rd_bank_addr + 1;   rd_addr1[0] = rd_bank_addr + 2 + offset;
    rd_addr0[1] = rd_bank_addr + 0;   rd_addr1[1] = rd_bank_addr + 1 + offset;
    rd_addr0[2] = rd_bank_addr + 4;   rd_addr1[2] = rd_bank_addr + 0 + offset;
    rd_addr0[3] = rd_bank_addr + 3;   rd_addr1[3] = rd_bank_addr + 4 + offset;
    rd_addr0[4] = rd_bank_addr + 2;   rd_addr1[4] = rd_bank_addr + 3 + offset;
    break;
  default:
    rd_addr0[0] = rd_bank_addr + 3;   rd_addr1[0] = rd_bank_addr + 4 + offset;
    rd_addr0[1] = rd_bank_addr + 2;   rd_addr1[1] = rd_bank_addr + 3 + offset;
    rd_addr0[2] = rd_bank_addr + 1;   rd_addr1[2] = rd_bank_addr + 2 + offset;
    rd_addr0[3] = rd_bank_addr + 0;   rd_addr1[3] = rd_bank_addr + 1 + offset;
    rd_addr0[4] = rd_bank_addr + 4;   rd_addr1[4] = rd_bank_addr + 0 + offset;
    break;
  }

  // Read and Write:
  TT_SAMPLE rd_data0[NSTREAM], rd_data1[NSTREAM];
  int wr_addr0 = wr_bank_addr;
  int wr_addr1 = wr_bank_addr + 1;
  for (int ss=0; ss < NSTREAM; ss++) {
    buff[ping][ss][wr_addr0] = wr_data0[ss];
    buff[ping][ss][wr_addr1] = wr_data1[ss];
    rd_data0[ss] = buff[!ping][ss][rd_addr0[ss]];
    rd_data1[ss] = buff[!ping][ss][rd_addr1[ss]];
  }

  // Read Data Output Mux:
  switch (phase) {
  case 0:
    trans_o[0] = rd_data0[0];   trans_o[1] = rd_data1[1];
    trans_o[2] = rd_data0[4];   trans_o[3] = rd_data1[0];
    trans_o[4] = rd_data0[3];   trans_o[5] = rd_data1[4];
    trans_o[6] = rd_data0[2];   trans_o[7] = rd_data1[3];
    trans_o[8] = rd_data0[1];   trans_o[9] = rd_data1[2];
    break;
  case 1:
    trans_o[0] = rd_data0[2];   trans_o[1] = rd_data1[3];
    trans_o[2] = rd_data0[1];   trans_o[3] = rd_data1[2];
    trans_o[4] = rd_data0[0];   trans_o[5] = rd_data1[1];
    trans_o[6] = rd_data0[4];   trans_o[7] = rd_data1[0];
    trans_o[8] = rd_data0[3];   trans_o[9] = rd_data1[4];
    break;
  case 2:
    trans_o[0] = rd_data0[4];   trans_o[1] = rd_data1[0];
    trans_o[2] = rd_data0[3];   trans_o[3] = rd_data1[4];
    trans_o[4] = rd_data0[2];   trans_o[5] = rd_data1[3];
    trans_o[6] = rd_data0[1];   trans_o[7] = rd_data1[2];
    trans_o[8] = rd_data0[0];   trans_o[9] = rd_data1[1];
    break;
  case 3:
    trans_o[0] = rd_data0[1];   trans_o[1] = rd_data1[2];
    trans_o[2] = rd_data0[0];   trans_o[3] = rd_data1[1];
    trans_o[4] = rd_data0[4];   trans_o[5] = rd_data1[0];
    trans_o[6] = rd_data0[3];   trans_o[7] = rd_data1[4];
    trans_o[8] = rd_data0[2];   trans_o[9] = rd_data1[3];
    break;
  default:
    trans_o[0] = rd_data0[3];   trans_o[1] = rd_data1[4];
    trans_o[2] = rd_data0[2];   trans_o[3] = rd_data1[3];
    trans_o[4] = rd_data0[1];   trans_o[5] = rd_data1[2];
    trans_o[6] = rd_data0[0];   trans_o[7] = rd_data1[1];
    trans_o[8] = rd_data0[4];   trans_o[9] = rd_data1[0];
    break;
  }

  // Update ping:
  bool last_wr = (wr_bank_addr == DEPTH*NROW-2);
  if ( last_wr == 1 ) {
    ping = !ping;
    // std::cout << "END OF PING PONG FRAME" << std::endl;
  }

  // Update write address:
  wr_bank_addr = (last_wr == 1) ? 0 : wr_bank_addr + 2;
  phase = ( phase == NSTREAM-1 ) ? 0 : phase + 1;

  // Update read address:
  int last_row = (rd_row == (DEPTH-2));
  if      ( last_wr  == 1 ) rd_col = 0;
  else if ( last_row == 1 ) rd_col = rd_col + 1;
  if ( last_row == 1 ) {
    rd_bank_addr = rd_col * NSTREAM;
  }
  else if ( rd_grp == (2*NSTREAM-2) || rd_grp == (NSTREAM-1) ) {
    rd_bank_addr = rd_bank_addr + DEPTH;
  }
  rd_row = ( last_row == 1 ) ? 0 : rd_row + 2;
  rd_grp = (rd_grp == (2*NSTREAM-2)) ? 0 : rd_grp + 2;
}

// ------------------------------------------------------------
// Write output streams
// ------------------------------------------------------------

void ifft_write_streams( TT_SAMPLE (&trans_o)[2*NSTREAM], TT_STREAM sig_o[NSTREAM] )
{
  static bool        running = 0;
  static ap_uint<13> startup = 0;
#pragma HLS reset variable=running
#pragma HLS reset variable=startup


 WRITE: for (int ss=0; ss < NSTREAM; ss++) {
    if ( running == 1 ) {
      sig_o[ss].write( ( trans_o[(ss<<1)+1], trans_o[(ss<<1)+0] ) );
    }
  }
  if ( startup == ap_uint<13>(DEPTH*NROW/2-1) ) {
    running = 1;
  }
  else {
    startup = ap_uint<13>(startup + 1);
  }
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void ifft_transpose_wrapper( TT_STREAM sig_i[NSTREAM], TT_STREAM sig_o[NSTREAM] )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS pipeline II=1

  TT_SAMPLE transpose_i[2*NSTREAM], transpose_o[2*NSTREAM];
#pragma HLS array_partition variable=transpose_i dim=1
#pragma HLS array_partition variable=transpose_o dim=1

  // Unpack samples:
  ifft_unpack( sig_i, transpose_i );

  // Transpose samples:
  ifft_do_transpose( transpose_i, transpose_o );

  // Format output streams:
  ifft_write_streams( transpose_o, sig_o );
}



