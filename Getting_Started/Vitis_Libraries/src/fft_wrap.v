/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

`timescale 1ns / 1ps

module fft_wrap (
  output          inData_0_ce,
  output          inData_1_ce,
  output          inData_2_ce,
  output          inData_3_ce,

  input   [31:0]  inData_0,
  input   [31:0]  inData_1,
  input   [31:0]  inData_2,
  input   [31:0]  inData_3,

  output          outData_0_we,
  output          outData_1_we,
  output          outData_2_we,
  output          outData_3_we,  

  output  [41:0]  outData_0,
  output  [41:0]  outData_1,
  output  [41:0]  outData_2,
  output  [41:0]  outData_3,
  
  input           clk,
  input           rst,
  input           start,
  output          done,
  output          idle,
  output          ready

);

 fft_top_0 fft_core (
        .ap_clk               (clk),
        .ap_rst               (rst),
        .ap_start             (start),
        .ap_done              (done),
        .ap_idle              (idle),
        .ap_ready             (ready),
        
        .p_inData_0_dout      (inData_0),
        .p_inData_0_empty_n   (1'b1),
        .p_inData_0_read      (inData_0_ce),
        .p_inData_1_dout      (inData_1),
        .p_inData_1_empty_n   (1'b1),
        .p_inData_1_read      (inData_1_ce),
        .p_inData_2_dout      (inData_2),
        .p_inData_2_empty_n   (1'b1),
        .p_inData_2_read      (inData_2_ce),
        .p_inData_3_dout      (inData_3),
        .p_inData_3_empty_n   (1'b1),
        .p_inData_3_read      (inData_3_ce),
        .p_outData_0_din      (outData_0),
        .p_outData_0_full_n   (1'b1),
        .p_outData_0_write    (outData_0_we),
        .p_outData_1_din      (outData_1),
        .p_outData_1_full_n   (1'b1),
        .p_outData_1_write    (outData_1_we),
        .p_outData_2_din      (outData_2),
        .p_outData_2_full_n   (1'b1),
        .p_outData_2_write    (outData_2_we),
        .p_outData_3_din      (outData_3),
        .p_outData_3_full_n   (1'b1),
        .p_outData_3_write    (outData_3_we)
);    
    
endmodule
