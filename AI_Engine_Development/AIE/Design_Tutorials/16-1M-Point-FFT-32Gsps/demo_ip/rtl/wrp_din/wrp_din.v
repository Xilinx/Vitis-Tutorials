//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns / 1ps
module wrp_din
(
	/////////////////////////////////////////////////////////////////////
	/////////  Data Plane Clock Domain 300MHz
	/////////////////////////////////////////////////////////////////////
	(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 datclk CLK" *)
	(* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF i0a:i1a:i2a:i3a:i4a:i5a:i6a:i7a:i0b:i1b:i2b:i3b:i4b:i5b:i6b:i7b:o0a:o1a:o2a:o3a:o4a:o5a:o6a:o7a:o0b:o1b:o2b:o3b:o4b:o5b:o6b:o7b, ASSOCIATED_RESET dat_resetn"*)
	input           dat_clk,
	input           dat_resetn,
	/////////////////////////////////////////////////////////////////////
	// Customer Logic -> Wrapper
	////////////////////////////////////////////////////////////////////
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i0a TREADY" *)   output        i0a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i0a TVALID" *)    input        i0a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i0a TDATA" *)     input [63:0] i0a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i1a TREADY" *)   output        i1a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i1a TVALID" *)    input        i1a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i1a TDATA" *)     input [63:0] i1a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i2a TREADY" *)   output        i2a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i2a TVALID" *)    input        i2a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i2a TDATA" *)     input [63:0] i2a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i3a TREADY" *)   output        i3a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i3a TVALID" *)    input        i3a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i3a TDATA" *)     input [63:0] i3a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i4a TREADY" *)   output        i4a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i4a TVALID" *)    input        i4a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i4a TDATA" *)     input [63:0] i4a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i5a TREADY" *)   output        i5a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i5a TVALID" *)    input        i5a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i5a TDATA" *)     input [63:0] i5a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i6a TREADY" *)   output        i6a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i6a TVALID" *)    input        i6a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i6a TDATA" *)     input [63:0] i6a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i7a TREADY" *)   output        i7a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i7a TVALID" *)    input        i7a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i7a TDATA" *)     input [63:0] i7a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i0b TREADY" *)   output        i0b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i0b TVALID" *)    input        i0b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i0b TDATA" *)     input [63:0] i0b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i1b TREADY" *)   output        i1b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i1b TVALID" *)    input        i1b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i1b TDATA" *)     input [63:0] i1b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i2b TREADY" *)   output        i2b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i2b TVALID" *)    input        i2b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i2b TDATA" *)     input [63:0] i2b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i3b TREADY" *)   output        i3b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i3b TVALID" *)    input        i3b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i3b TDATA" *)     input [63:0] i3b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i4b TREADY" *)   output        i4b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i4b TVALID" *)    input        i4b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i4b TDATA" *)     input [63:0] i4b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i5b TREADY" *)   output        i5b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i5b TVALID" *)    input        i5b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i5b TDATA" *)     input [63:0] i5b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i6b TREADY" *)   output        i6b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i6b TVALID" *)    input        i6b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i6b TDATA" *)     input [63:0] i6b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i7b TREADY" *)   output        i7b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i7b TVALID" *)    input        i7b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 i7b TDATA" *)     input [63:0] i7b_axi_tdat,
	/////////////////////////////////////////////////////////////////////
	// Wrapper -> AIE
	////////////////////////////////////////////////////////////////////
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o0a TREADY" *)    input        o0a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o0a TVALID" *)   output        o0a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o0a TDATA" *)    output [63:0] o0a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o1a TREADY" *)    input        o1a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o1a TVALID" *)   output        o1a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o1a TDATA" *)    output [63:0] o1a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o2a TREADY" *)    input        o2a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o2a TVALID" *)   output        o2a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o2a TDATA" *)    output [63:0] o2a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o3a TREADY" *)    input        o3a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o3a TVALID" *)   output        o3a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o3a TDATA" *)    output [63:0] o3a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o4a TREADY" *)    input        o4a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o4a TVALID" *)   output        o4a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o4a TDATA" *)    output [63:0] o4a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o5a TREADY" *)    input        o5a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o5a TVALID" *)   output        o5a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o5a TDATA" *)    output [63:0] o5a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o6a TREADY" *)    input        o6a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o6a TVALID" *)   output        o6a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o6a TDATA" *)    output [63:0] o6a_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o7a TREADY" *)    input        o7a_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o7a TVALID" *)   output        o7a_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o7a TDATA" *)    output [63:0] o7a_axi_tdat,	
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o0b TREADY" *)    input        o0b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o0b TVALID" *)   output        o0b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o0b TDATA" *)    output [63:0] o0b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o1b TREADY" *)    input        o1b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o1b TVALID" *)   output        o1b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o1b TDATA" *)    output [63:0] o1b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o2b TREADY" *)    input        o2b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o2b TVALID" *)   output        o2b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o2b TDATA" *)    output [63:0] o2b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o3b TREADY" *)    input        o3b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o3b TVALID" *)   output        o3b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o3b TDATA" *)    output [63:0] o3b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o4b TREADY" *)    input        o4b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o4b TVALID" *)   output        o4b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o4b TDATA" *)    output [63:0] o4b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o5b TREADY" *)    input        o5b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o5b TVALID" *)   output        o5b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o5b TDATA" *)    output [63:0] o5b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o6b TREADY" *)    input        o6b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o6b TVALID" *)   output        o6b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o6b TDATA" *)    output [63:0] o6b_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o7b TREADY" *)    input        o7b_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o7b TVALID" *)   output        o7b_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 o7b TDATA" *)    output [63:0] o7b_axi_tdat
);



//------------------------------------
// Reset Signals
//------------------------------------
reg  srst, srst_p;
reg [7:0] rstcnt;

//---------------------------------------------------
// Customer Logic -> WRAPPER -> AIE
//---------------------------------------------------
wrp_din_sfifo #(64, 32) FIF0a(dat_clk, srst, o0a_axi_trdy, o0a_axi_tvld, o0a_axi_tdat, i0a_axi_trdy, i0a_axi_tvld, i0a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF1a(dat_clk, srst, o1a_axi_trdy, o1a_axi_tvld, o1a_axi_tdat, i1a_axi_trdy, i1a_axi_tvld, i1a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF2a(dat_clk, srst, o2a_axi_trdy, o2a_axi_tvld, o2a_axi_tdat, i2a_axi_trdy, i2a_axi_tvld, i2a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF3a(dat_clk, srst, o3a_axi_trdy, o3a_axi_tvld, o3a_axi_tdat, i3a_axi_trdy, i3a_axi_tvld, i3a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF4a(dat_clk, srst, o4a_axi_trdy, o4a_axi_tvld, o4a_axi_tdat, i4a_axi_trdy, i4a_axi_tvld, i4a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF5a(dat_clk, srst, o5a_axi_trdy, o5a_axi_tvld, o5a_axi_tdat, i5a_axi_trdy, i5a_axi_tvld, i5a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF6a(dat_clk, srst, o6a_axi_trdy, o6a_axi_tvld, o6a_axi_tdat, i6a_axi_trdy, i6a_axi_tvld, i6a_axi_tdat);
wrp_din_sfifo #(64, 32) FIF7a(dat_clk, srst, o7a_axi_trdy, o7a_axi_tvld, o7a_axi_tdat, i7a_axi_trdy, i7a_axi_tvld, i7a_axi_tdat);

wrp_din_sfifo #(64, 32) FIF0b(dat_clk, srst, o0b_axi_trdy, o0b_axi_tvld, o0b_axi_tdat, i0b_axi_trdy, i0b_axi_tvld, i0b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF1b(dat_clk, srst, o1b_axi_trdy, o1b_axi_tvld, o1b_axi_tdat, i1b_axi_trdy, i1b_axi_tvld, i1b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF2b(dat_clk, srst, o2b_axi_trdy, o2b_axi_tvld, o2b_axi_tdat, i2b_axi_trdy, i2b_axi_tvld, i2b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF3b(dat_clk, srst, o3b_axi_trdy, o3b_axi_tvld, o3b_axi_tdat, i3b_axi_trdy, i3b_axi_tvld, i3b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF4b(dat_clk, srst, o4b_axi_trdy, o4b_axi_tvld, o4b_axi_tdat, i4b_axi_trdy, i4b_axi_tvld, i4b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF5b(dat_clk, srst, o5b_axi_trdy, o5b_axi_tvld, o5b_axi_tdat, i5b_axi_trdy, i5b_axi_tvld, i5b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF6b(dat_clk, srst, o6b_axi_trdy, o6b_axi_tvld, o6b_axi_tdat, i6b_axi_trdy, i6b_axi_tvld, i6b_axi_tdat);
wrp_din_sfifo #(64, 32) FIF7b(dat_clk, srst, o7b_axi_trdy, o7b_axi_tvld, o7b_axi_tdat, i7b_axi_trdy, i7b_axi_tvld, i7b_axi_tdat);

//-----------------------------------------
// Generate Sync Reset Signal
//-----------------------------------------
always@(posedge dat_clk)
begin
	rstcnt    <= ~dat_resetn? {1'b1, 7'd0} : srst_p? rstcnt+1 : rstcnt;
	srst_p    <= rstcnt[7];
	srst      <= srst_p;
end




endmodule
