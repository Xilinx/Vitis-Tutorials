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

// --=========================================================================--
// Purpose             : General FIFO controller, type 0
// -----------------------------------------------------------------------------
// Description   
// This controller is used with syn-read SRAM or register file to form a
// FIFO. When data is pushed into FIFO, it appears at the data output port
// at once (don't need POP req).
//          __    __    __    __    __    __
// CLK     _|  |__|  |__|  |__|  |__|  |__|  |_
//         _______       _____       _____       
// POP req        |_____|     |_____|     |_____|
//         _____________ ___________ ___________
// Pop Data____Data 0___|___Data 1__|___Data 2__|    
//
// FIFO depth is 2's power of address bit width   
// Full/empty flags and pushed-data counter output are provided.
// --=========================================================================--

                       
module FifoCtlType0 (
         clk,
         rst_n,
         push_req_n,
         pop_req_n,
         we_n,
         empty,
         full,
         error,
         wr_addr,
         rd_addr,
         data_cnt,
         clear);

parameter addr_bits  = 8;

  input  clk;
  input  rst_n;
  input  push_req_n;
  input  pop_req_n;
  output we_n;
  output empty;
  output full;
  output error;
  output [addr_bits-1 : 0] wr_addr;
  output [addr_bits-1 : 0] rd_addr;
  output [addr_bits   : 0] data_cnt;
  input  clear;             // The FIFO are reset when clear is high.

  wire   clk;
  wire   rst_n;
  wire   push_req_n;
  wire   pop_req_n;
  reg    we_n;
  reg    empty;
  reg    full;
  reg    error;
  wire   clear;
  reg    [addr_bits-1 : 0] wr_addr_reg;
  reg    [addr_bits-1 : 0] rd_addr_reg;
  reg    [addr_bits-1 : 0] rd_addr;
  wire   [addr_bits-1 : 0] wr_addr;

  wire   [addr_bits-1 : 0] wr_addr_incr;
  wire   [addr_bits-1 : 0] rd_addr_incr;

  wire   [addr_bits   : 0] data_cnt;
    
  assign wr_addr_incr = wr_addr_reg + 1'b1; 
  assign rd_addr_incr = rd_addr_reg + 1'b1; 

  always @ (posedge clk or negedge rst_n)
    if (~rst_n)
    begin
      wr_addr_reg <= 0;
      rd_addr_reg <= 0;
      empty       <= 1'b1;
      full        <= 1'b0;
      error       <= 1'b0;
    end
    else
      if (clear)
      begin
        wr_addr_reg <= 0;
        rd_addr_reg <= 0;
        empty       <= 1'b1;
        full        <= 1'b0;
        error       <= 1'b0;
      end
      else
      begin
        if (~push_req_n && ~pop_req_n)
        begin
          wr_addr_reg <= wr_addr_incr;
          rd_addr_reg <= rd_addr_incr;
        end
        else  
          if (~push_req_n)
          begin
            if (~full)
            begin
              empty <= 1'b0;
              wr_addr_reg <= wr_addr_incr;  
              if (wr_addr_incr == rd_addr_reg)
                full <= 1'b1;
            end
            else
              error <= 1'b1;
          end
          else
          begin
            if (~pop_req_n)
              if (~empty)
              begin
                full <= 1'b0;
                rd_addr_reg <= rd_addr_incr;
                if (rd_addr_incr == wr_addr_reg)
                  empty <= 1'b1;
              end
              else
                error <= 1'b1;
          end
      end

  always @ (*)
    if (pop_req_n)
      rd_addr = rd_addr_reg;
    else
      if (~empty || ~push_req_n)
        rd_addr = rd_addr_incr;
      else
        rd_addr = rd_addr_reg;

  assign wr_addr = wr_addr_reg;


  always @ (*)
  if (push_req_n)
    we_n = 1'b1;
  else
    if (~full || ~pop_req_n)
      we_n = 1'b0;
    else
      we_n = 1'b1;

  wire [addr_bits-1:0] addr_diff_wr;

  
  assign addr_diff_wr = wr_addr_reg - rd_addr_reg;
  
  assign data_cnt = {full, addr_diff_wr};
  

endmodule
