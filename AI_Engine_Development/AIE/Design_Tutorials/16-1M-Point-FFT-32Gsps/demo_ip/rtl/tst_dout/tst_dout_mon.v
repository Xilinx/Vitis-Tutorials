//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

(* KEEP_HIERARCHY = "SOFT" *)
module tst_dout_mon
#(parameter golden_rom_file_name = "demo_gold0.mem", DATABW=64, ADDR_BW=15, ROMLEN=32768)
(
	input  clk,
	input  srst_i,
	input  dout_rst_i,
	input        vld_i,
	input [DATABW-1:0] dat_i,
	//----
	output   vld_o,
	output  [6:0]  smp_cnt_o,
	output  [6:0]  err_cnt_o,
	output  [11:0] idl_cnt_o,
	output  [15:0] lat_cnt_o,
	output  done_o
);

// parameter determined by URAM setting
localparam romlatency=5;
reg srst;
wire afifo_rvld;
wire [DATABW-1:0] afifo_rd;
wire afifo_re;


always@(posedge clk) srst <= srst_i;

//-------------------------------------------------------------
// instantiate Core, ROM, and cross-clock domain register
//-------------------------------------------------------------
tst_dout_mon_core #(romlatency, DATABW)
C(clk, srst, dout_rst_i,
	vld_i, dat_i, 
	afifo_re, afifo_rvld, afifo_rd,
	vld_o, smp_cnt_o, err_cnt_o, idl_cnt_o, lat_cnt_o, done_o);
//--------------------------------------------
tst_dout_mon_rom  #(romlatency, golden_rom_file_name, DATABW, ADDR_BW, ROMLEN)
R(clk, srst|dout_rst_i, afifo_re, afifo_rvld, afifo_rd);
//--------------------------------------------



endmodule
