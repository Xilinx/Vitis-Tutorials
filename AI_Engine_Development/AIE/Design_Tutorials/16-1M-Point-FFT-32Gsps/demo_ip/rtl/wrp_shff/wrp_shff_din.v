//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

(* KEEP_HIERARCHY = "SOFT" *)
module wrp_shff_din
(
	input  clk,
	input  srst_i,
	//------------------------ Interface with AIE
	output        axi00_trdy,
	input         axi00_tvld,
	input  [63:0] axi00_tdat,
	output        axi01_trdy,
	input         axi01_tvld,
	input  [63:0] axi01_tdat,
	output        axi02_trdy,
	input         axi02_tvld,
	input  [63:0] axi02_tdat,
	output        axi03_trdy,
	input         axi03_tvld,
	input  [63:0] axi03_tdat,
	output        axi04_trdy,
	input         axi04_tvld,
	input  [63:0] axi04_tdat,
	output        axi05_trdy,
	input         axi05_tvld,
	input  [63:0] axi05_tdat,
	output        axi06_trdy,
	input         axi06_tvld,
	input  [63:0] axi06_tdat,
	output        axi07_trdy,
	input         axi07_tvld,
	input  [63:0] axi07_tdat,
	output        axi10_trdy,
	input         axi10_tvld,
	input  [63:0] axi10_tdat,
	output        axi11_trdy,
	input         axi11_tvld,
	input  [63:0] axi11_tdat,
	output        axi12_trdy,
	input         axi12_tvld,
	input  [63:0] axi12_tdat,
	output        axi13_trdy,
	input         axi13_tvld,
	input  [63:0] axi13_tdat,
	output        axi14_trdy,
	input         axi14_tvld,
	input  [63:0] axi14_tdat,
	output        axi15_trdy,
	input         axi15_tvld,
	input  [63:0] axi15_tdat,
	output        axi16_trdy,
	input         axi16_tvld,
	input  [63:0] axi16_tdat,
	output        axi17_trdy,
	input         axi17_tvld,
	input  [63:0] axi17_tdat,
	//------------------------ Interface with Shuffle Network
	input  [15:0] rdy_i,
	output reg start_o,
	output [63:0] y00_o,
	output [63:0] y01_o,
	output [63:0] y02_o,
	output [63:0] y03_o,
	output [63:0] y04_o,
	output [63:0] y05_o,
	output [63:0] y06_o,
	output [63:0] y07_o,
	output [63:0] y10_o,
	output [63:0] y11_o,
	output [63:0] y12_o,
	output [63:0] y13_o,
	output [63:0] y14_o,
	output [63:0] y15_o,
	output [63:0] y16_o,
	output [63:0] y17_o	
);


reg  start;
reg  rdy_d;
wire rdy_s;
wire [15:0] ready;

reg srst;
reg [1:0] cst;

localparam stat_wait = 2'b01;
localparam stat_rd   = 2'b10;
localparam stat_rst  = 2'b00;

localparam ram_delay = 2;

reg [ram_delay+1:0] start_s;
reg [3:0] rcnt;

always @(posedge clk) srst <= srst_i;


//------------------------------------------
// Buffer for AXI0 and AXI1
//------------------------------------------
wrp_shff_din_buff #(4'd0 , ram_delay) B00(clk, srst, axi00_trdy,axi00_tvld,axi00_tdat, ready[0],   start, y00_o);
wrp_shff_din_buff #(4'd1 , ram_delay) B01(clk, srst, axi01_trdy,axi01_tvld,axi01_tdat, ready[1],   start, y01_o);
wrp_shff_din_buff #(4'd2 , ram_delay) B02(clk, srst, axi02_trdy,axi02_tvld,axi02_tdat, ready[2],   start, y02_o);
wrp_shff_din_buff #(4'd3 , ram_delay) B03(clk, srst, axi03_trdy,axi03_tvld,axi03_tdat, ready[3],   start, y03_o);
wrp_shff_din_buff #(4'd4 , ram_delay) B04(clk, srst, axi04_trdy,axi04_tvld,axi04_tdat, ready[4],   start, y04_o);
wrp_shff_din_buff #(4'd5 , ram_delay) B05(clk, srst, axi05_trdy,axi05_tvld,axi05_tdat, ready[5],   start, y05_o);
wrp_shff_din_buff #(4'd6 , ram_delay) B06(clk, srst, axi06_trdy,axi06_tvld,axi06_tdat, ready[6],   start, y06_o);
wrp_shff_din_buff #(4'd7 , ram_delay) B07(clk, srst, axi07_trdy,axi07_tvld,axi07_tdat, ready[7],   start, y07_o);
wrp_shff_din_buff #(4'd8 , ram_delay) B10(clk, srst, axi10_trdy,axi10_tvld,axi10_tdat, ready[8+0], start, y10_o);
wrp_shff_din_buff #(4'd9 , ram_delay) B11(clk, srst, axi11_trdy,axi11_tvld,axi11_tdat, ready[8+1], start, y11_o);
wrp_shff_din_buff #(4'd10, ram_delay) B12(clk, srst, axi12_trdy,axi12_tvld,axi12_tdat, ready[8+2], start, y12_o);
wrp_shff_din_buff #(4'd11, ram_delay) B13(clk, srst, axi13_trdy,axi13_tvld,axi13_tdat, ready[8+3], start, y13_o);
wrp_shff_din_buff #(4'd12, ram_delay) B14(clk, srst, axi14_trdy,axi14_tvld,axi14_tdat, ready[8+4], start, y14_o);
wrp_shff_din_buff #(4'd13, ram_delay) B15(clk, srst, axi15_trdy,axi15_tvld,axi15_tdat, ready[8+5], start, y15_o);
wrp_shff_din_buff #(4'd14, ram_delay) B16(clk, srst, axi16_trdy,axi16_tvld,axi16_tdat, ready[8+6], start, y16_o);
wrp_shff_din_buff #(4'd15, ram_delay) B17(clk, srst, axi17_trdy,axi17_tvld,axi17_tdat, ready[8+7], start, y17_o);




//------------------------------------------
// Contrl State Machine
//------------------------------------------
always@(posedge clk)
begin
  if(srst==1'b1) cst  <= stat_rst;
  else begin  
	   case(cst)
		stat_wait:	cst <= rdy_s?   stat_rd : cst;
		stat_rd  :  cst <= (rcnt[3]&rcnt[2])? stat_wait : cst;
		default:    cst <= stat_wait;
	   endcase
	end
end


//------------------------------------------
// Contrl signal generation
//------------------------------------------
assign rdy_s = &{rdy_d, ready};

always@(posedge clk)
begin

	start <= (cst==stat_wait)? rdy_s: 1'b0;
	rcnt  <= (cst==stat_rd)? rcnt+1 : 0;
	
	start_s <= srst? 0 : {start, start_s[ram_delay+1:1]};
	start_o <= start_s[0];
	
	rdy_d <= &rdy_i;
end



endmodule

