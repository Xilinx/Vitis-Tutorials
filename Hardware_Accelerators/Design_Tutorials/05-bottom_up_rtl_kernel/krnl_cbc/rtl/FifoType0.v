//
// Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
  