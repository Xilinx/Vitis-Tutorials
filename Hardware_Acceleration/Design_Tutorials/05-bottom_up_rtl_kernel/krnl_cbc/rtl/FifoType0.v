//
//
//# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
//# SPDX-License-Identifier: X11
//
//

module FifoType0 (

 CLK,
 nRESET,
 PUSH_REQ,
 POP_REQ,
 PUSH_DATA,
 CLEAR,
  
 POP_DATA,
 EMPTY,
 FULL,
 ERROR,
 DATA_CNT

);
  parameter data_width = 32;
  parameter addr_bits = 8;

  input CLK;
  input nRESET;
  input PUSH_REQ;
  input POP_REQ;
  input [data_width-1:0] PUSH_DATA;
  input CLEAR;
  
  output [data_width-1:0] POP_DATA;
  output        EMPTY;
  output        FULL;
  output        ERROR;
  output [addr_bits:0]  DATA_CNT;
  
  wire we_n;
  wire [addr_bits-1:0] wr_addr;
  wire [addr_bits-1:0] rd_addr;
  
  FifoCtlType0 #(.addr_bits(addr_bits)) uFifoCtl (
         .clk           (CLK),
         .rst_n         (nRESET),
         .push_req_n    (!PUSH_REQ),
         .pop_req_n     (!POP_REQ),
         .we_n          (we_n),
         .empty         (EMPTY),
         .full          (FULL),
         .error         (ERROR),
         .wr_addr       (wr_addr),
         .rd_addr       (rd_addr),
         .data_cnt      (DATA_CNT),
         .clear         (CLEAR)
  );

  wire cena;

  //assign cena = (wr_addr == rd_addr) && !we_n;
  assign cena = 1'b0;
  
  TPSR #(.WORD_DEPTH(2**addr_bits), .DATA_WIDTH(data_width)) u_2pRAM (
    .AB     (wr_addr),
    .AA     (rd_addr),
    .DB     (PUSH_DATA),
    .QA     (POP_DATA),
    .CLKB   (CLK),
    .CLKA   (CLK),
    .CENB   (we_n),
    .CENA   (cena)
  ); 


endmodule
  