//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns / 1ps
module tst_din_ctrl
(
	input   clk,
	input   srst,
	input   en_i,
	output [31:0] nite_o,
	output reg start_o,
	input  done_i
);

localparam stat_wait = 2'b01;
localparam stat_idle = 2'b00;
localparam stat_rd   = 2'b10;
localparam stat_rst  = 2'b11;

reg [1:0] cst;

reg  [15:0] ite_msb, ite_lsb;
reg ite_msb_incr;
reg [3:0]  shft;
reg en_d, en_s, alldone, alldone_d;




// split num iteration into two 16-bit registers
assign nite_o = {ite_msb, ite_lsb};

always @(posedge clk) begin

	start_o <= (cst==stat_rd)? 1'b1 : 1'b0;
	shft    <= (cst==stat_wait)? {1'b0, shft[3:1]} : 4'b1000;
	alldone <= done_i;
	alldone_d <= alldone;

	// count the number of iterations
	en_d <= en_i;
	en_s <= en_d;
	
	ite_lsb <= srst|((~en_s)&en_d)? 0 : start_o? ite_lsb+1 : ite_lsb;
	ite_msb_incr <= (&ite_lsb) & start_o;
	ite_msb <= srst|((~en_s)&en_d)? 0 : ite_msb_incr? ite_msb+1 : ite_msb;
	
end
//-----------------------------------------------


//-----------------------------------------
// state machine
//-----------------------------------------
always@(posedge clk)
begin
  if(srst==1'b1) cst  <= stat_rst;
  else begin  
	   case(cst)
		stat_idle:	cst <= en_s? stat_rd : cst;
		stat_rd:	cst <= stat_wait;
		stat_wait :  cst <= (alldone&(~alldone_d))? stat_idle : cst;
		default:    cst <= stat_idle;
	   endcase
   end
end



endmodule
