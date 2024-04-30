//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns / 1ps
module wrp_shff
(
	(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 datclk CLK" *)
	(* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF in00:in01:in02:in03:in04:in05:in06:in07:in10:in11:in12:in13:in14:in15:in16:in17:out00:out01:out02:out03:out04:out05:out06:out07:out10:out11:out12:out13:out14:out15:out16:out17, ASSOCIATED_RESET dat_resetn"*)
	input           dat_clk,
	input           dat_resetn,
	/////////////////////////////////////////////////////////////////////
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in00 TREADY" *)   output         in00_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in00 TVALID" *)    input         in00_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in00 TDATA" *)     input [63:0]  in00_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in01 TREADY" *)   output         in01_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in01 TVALID" *)    input         in01_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in01 TDATA" *)     input [63:0]  in01_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in02 TREADY" *)   output         in02_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in02 TVALID" *)    input         in02_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in02 TDATA" *)     input [63:0]  in02_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in03 TREADY" *)   output         in03_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in03 TVALID" *)    input         in03_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in03 TDATA" *)     input [63:0]  in03_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in04 TREADY" *)   output         in04_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in04 TVALID" *)    input         in04_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in04 TDATA" *)     input [63:0]  in04_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in05 TREADY" *)   output         in05_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in05 TVALID" *)    input         in05_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in05 TDATA" *)     input [63:0]  in05_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in06 TREADY" *)   output         in06_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in06 TVALID" *)    input         in06_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in06 TDATA" *)     input [63:0]  in06_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in07 TREADY" *)   output         in07_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in07 TVALID" *)    input         in07_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in07 TDATA" *)     input [63:0]  in07_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in10 TREADY" *)   output         in10_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in10 TVALID" *)    input         in10_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in10 TDATA" *)     input [63:0]  in10_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in11 TREADY" *)   output         in11_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in11 TVALID" *)    input         in11_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in11 TDATA" *)     input [63:0]  in11_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in12 TREADY" *)   output         in12_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in12 TVALID" *)    input         in12_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in12 TDATA" *)     input [63:0]  in12_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in13 TREADY" *)   output         in13_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in13 TVALID" *)    input         in13_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in13 TDATA" *)     input [63:0]  in13_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in14 TREADY" *)   output         in14_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in14 TVALID" *)    input         in14_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in14 TDATA" *)     input [63:0]  in14_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in15 TREADY" *)   output         in15_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in15 TVALID" *)    input         in15_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in15 TDATA" *)     input [63:0]  in15_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in16 TREADY" *)   output         in16_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in16 TVALID" *)    input         in16_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in16 TDATA" *)     input [63:0]  in16_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in17 TREADY" *)   output         in17_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in17 TVALID" *)    input         in17_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 in17 TDATA" *)     input [63:0]  in17_axi_tdat,	
	/////////////////////////////////////////////////////////////////////
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out00  TREADY" *)    input        out00_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out00  TVALID" *)   output        out00_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out00  TDATA" *)    output [63:0] out00_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out01  TREADY" *)    input        out01_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out01  TVALID" *)   output        out01_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out01  TDATA" *)    output [63:0] out01_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out02  TREADY" *)    input        out02_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out02  TVALID" *)   output        out02_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out02  TDATA" *)    output [63:0] out02_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out03  TREADY" *)    input        out03_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out03  TVALID" *)   output        out03_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out03  TDATA" *)    output [63:0] out03_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out04  TREADY" *)    input        out04_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out04  TVALID" *)   output        out04_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out04  TDATA" *)    output [63:0] out04_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out05  TREADY" *)    input        out05_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out05  TVALID" *)   output        out05_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out05  TDATA" *)    output [63:0] out05_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out06  TREADY" *)    input        out06_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out06  TVALID" *)   output        out06_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out06  TDATA" *)    output [63:0] out06_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out07  TREADY" *)    input        out07_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out07  TVALID" *)   output        out07_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out07  TDATA" *)    output [63:0] out07_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out10  TREADY" *)    input        out10_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out10  TVALID" *)   output        out10_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out10  TDATA" *)    output [63:0] out10_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out11  TREADY" *)    input        out11_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out11  TVALID" *)   output        out11_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out11  TDATA" *)    output [63:0] out11_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out12  TREADY" *)    input        out12_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out12  TVALID" *)   output        out12_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out12  TDATA" *)    output [63:0] out12_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out13  TREADY" *)    input        out13_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out13  TVALID" *)   output        out13_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out13  TDATA" *)    output [63:0] out13_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out14  TREADY" *)    input        out14_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out14  TVALID" *)   output        out14_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out14  TDATA" *)    output [63:0] out14_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out15  TREADY" *)    input        out15_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out15  TVALID" *)   output        out15_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out15  TDATA" *)    output [63:0] out15_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out16  TREADY" *)    input        out16_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out16  TVALID" *)   output        out16_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out16  TDATA" *)    output [63:0] out16_axi_tdat,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out17  TREADY" *)    input        out17_axi_trdy,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out17  TVALID" *)   output        out17_axi_tvld,
	(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 out17  TDATA" *)    output [63:0] out17_axi_tdat	
);

//------------------------------------
// Reset Signals
//------------------------------------
reg  srst, srst_p, srst_xclk;
reg [7:0] rstcnt;

wire  start_x, vld_y;
wire [15:0] rdy;

wire [63:0] x[0:15], y[0:15];


//--------------------------------------------------
// AIE Input Interface
//--------------------------------------------------
wrp_shff_din DIN
(
	dat_clk, srst,
	//------------------------ Interface with AIE
	in00_axi_trdy,	in00_axi_tvld,	in00_axi_tdat,
	in01_axi_trdy,	in01_axi_tvld,	in01_axi_tdat,	
	in02_axi_trdy,	in02_axi_tvld,	in02_axi_tdat,	
	in03_axi_trdy,	in03_axi_tvld,	in03_axi_tdat,	
	in04_axi_trdy,	in04_axi_tvld,	in04_axi_tdat,	
	in05_axi_trdy,	in05_axi_tvld,	in05_axi_tdat,	
	in06_axi_trdy,	in06_axi_tvld,	in06_axi_tdat,	
	in07_axi_trdy,	in07_axi_tvld,	in07_axi_tdat,
	//------------------------
	in10_axi_trdy,	in10_axi_tvld,	in10_axi_tdat,
	in11_axi_trdy,	in11_axi_tvld,	in11_axi_tdat,	
	in12_axi_trdy,	in12_axi_tvld,	in12_axi_tdat,	
	in13_axi_trdy,	in13_axi_tvld,	in13_axi_tdat,	
	in14_axi_trdy,	in14_axi_tvld,	in14_axi_tdat,	
	in15_axi_trdy,	in15_axi_tvld,	in15_axi_tdat,
	in16_axi_trdy,	in16_axi_tvld,	in16_axi_tdat,	
	in17_axi_trdy,	in17_axi_tvld,	in17_axi_tdat,
	//------------------------
	rdy, start_x,
	x[0],	x[1],	x[2],	x[3],
	x[4],	x[5],	x[6],	x[7],
	x[8],	x[9],	x[10],	x[11],
	x[12],	x[13],	x[14],	x[15]
);


//--------------------------------------------------
// Shuffle Network
//--------------------------------------------------
wrp_shff_sw SW
(
	dat_clk, srst, start_x,
	x[0],	x[1],	x[2],	x[3],
	x[4],	x[5],	x[6],	x[7],
	x[8],	x[9],	x[10],	x[11],
	x[12],	x[13],	x[14],	x[15],
	//
	vld_y,
	y[0],	y[1],	y[2],	y[3],
	y[4],	y[5],	y[6],	y[7],
	y[8],	y[9],	y[10],	y[11],
	y[12],	y[13],	y[14],	y[15]
);



//--------------------------------------------------
// URAM Buffer and Output
//--------------------------------------------------
wrp_shff_fifo #(4'd0) F00( dat_clk, srst, rdy[0],   vld_y, y[0],    out00_axi_trdy, out00_axi_tvld, out00_axi_tdat);
wrp_shff_fifo #(4'd1) F01( dat_clk, srst, rdy[1],   vld_y, y[1],    out01_axi_trdy, out01_axi_tvld, out01_axi_tdat);
wrp_shff_fifo #(4'd2) F02( dat_clk, srst, rdy[2],   vld_y, y[2],    out02_axi_trdy, out02_axi_tvld, out02_axi_tdat);
wrp_shff_fifo #(4'd3) F03( dat_clk, srst, rdy[3],   vld_y, y[3],    out03_axi_trdy, out03_axi_tvld, out03_axi_tdat);
wrp_shff_fifo #(4'd4) F04( dat_clk, srst, rdy[4],   vld_y, y[4],    out04_axi_trdy, out04_axi_tvld, out04_axi_tdat);
wrp_shff_fifo #(4'd5) F05( dat_clk, srst, rdy[5],   vld_y, y[5],    out05_axi_trdy, out05_axi_tvld, out05_axi_tdat);
wrp_shff_fifo #(4'd6) F06( dat_clk, srst, rdy[6],   vld_y, y[6],    out06_axi_trdy, out06_axi_tvld, out06_axi_tdat);
wrp_shff_fifo #(4'd7) F07( dat_clk, srst, rdy[7],   vld_y, y[7],    out07_axi_trdy, out07_axi_tvld, out07_axi_tdat);
                     
wrp_shff_fifo #(4'd8) F10( dat_clk, srst, rdy[0+8], vld_y, y[0+8],  out10_axi_trdy, out10_axi_tvld, out10_axi_tdat);
wrp_shff_fifo #(4'd9) F11( dat_clk, srst, rdy[1+8], vld_y, y[1+8],  out11_axi_trdy, out11_axi_tvld, out11_axi_tdat);
wrp_shff_fifo #(4'd10)F12(dat_clk, srst, rdy[2+8], vld_y, y[2+8],  out12_axi_trdy, out12_axi_tvld, out12_axi_tdat);
wrp_shff_fifo #(4'd11)F13(dat_clk, srst, rdy[3+8], vld_y, y[3+8],  out13_axi_trdy, out13_axi_tvld, out13_axi_tdat);
wrp_shff_fifo #(4'd12)F14(dat_clk, srst, rdy[4+8], vld_y, y[4+8],  out14_axi_trdy, out14_axi_tvld, out14_axi_tdat);
wrp_shff_fifo #(4'd13)F15(dat_clk, srst, rdy[5+8], vld_y, y[5+8],  out15_axi_trdy, out15_axi_tvld, out15_axi_tdat);
wrp_shff_fifo #(4'd14)F16(dat_clk, srst, rdy[6+8], vld_y, y[6+8],  out16_axi_trdy, out16_axi_tvld, out16_axi_tdat);
wrp_shff_fifo #(4'd15)F17(dat_clk, srst, rdy[7+8], vld_y, y[7+8],  out17_axi_trdy, out17_axi_tvld, out17_axi_tdat);


//-------------------------------------------------------------
// Extend the external reset signal to make sure a clear reset
//-------------------------------------------------------------
always@(posedge dat_clk)
begin
	rstcnt    <= (~dat_resetn)? 8'h80 : srst_xclk? rstcnt+1 : rstcnt;
	srst_xclk <= rstcnt[7];
	srst_p <= srst_xclk;
	srst   <= srst_p; 
end
//---------------------------------------------




//
////------------------------------------------
//// Debug
////------------------------------------------
//initial begin
//
//  f[0] = $fopen("/home/mattr/NEC/GITHUB/NEC_64TRx_UL/data/rtldump_ulbf_din0a.txt","w");
//  cnt[0]=0;
//
//  while (cnt[0]<10000) begin
//	@(posedge dat_clk)
//	if(out0a_axi_trdy&out0a_axi_tvld) begin
//		$fwrite(f[0],"%d, %d, %d, %d\n",out0a_axi_tdat[15:0],out0a_axi_tdat[31:16],out0a_axi_tdat[47:32],out0a_axi_tdat[63:48]);
//		cnt[0] = cnt[0]+1;
//	end
//  end
//
//  $fclose(f[0]);  
//end
//
//
//initial begin
//
//  f[1] = $fopen("/home/mattr/NEC/GITHUB/NEC_64TRx_UL/data/rtldump_ulbf_din0b.txt","w");
//  cnt[1]=0;
//
//  while (cnt[1]<10000) begin
//	@(posedge dat_clk)
//	if(out0b_axi_trdy&out0b_axi_tvld) begin
//		$fwrite(f[1],"%d, %d, %d, %d\n",out0b_axi_tdat[15:0],out0b_axi_tdat[31:16],out0b_axi_tdat[47:32],out0b_axi_tdat[63:48]);
//		cnt[1] = cnt[1]+1;
//	end
//  end
//
//  $fclose(f[1]);  
//end
//
//


endmodule
