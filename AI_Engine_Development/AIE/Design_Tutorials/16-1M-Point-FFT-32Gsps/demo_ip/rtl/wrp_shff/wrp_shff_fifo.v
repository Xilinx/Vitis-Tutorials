//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

(* KEEP_HIERARCHY = "SOFT" *)
module wrp_shff_fifo
#(parameter RA_INIT = 4'd0)
(
	input  clk,
	input  srst_i,
	//------------------------ Interface with Shuffle Network
	output reg   rdy_o,
	input        vld_i,
	input [63:0] dat_i,
	//------------------------ Interface with AIE
	input         axi_trdy,
	output        axi_tvld,
	output [63:0] axi_tdat
);

localparam  ram_delay = 2;
localparam uram_delay = 6;


reg srst;
wire ready;

wire fifo_af;
wire fifo_we;
wire [63:0] fifo_wd;

wire buf_af, buf_we, buf_wdone, buf_rdone, buf_empty;
wire [13:0] buf_wa, buf_ra;
wire [63:0] buf_rd, buf_wd;


always @(posedge clk) srst <= srst_i;

//------------------------------------------------
// Combine the ready signals to give backpressure
//------------------------------------------------
always@(posedge clk)  rdy_o <= ready;

//------------------------------------------
// Shuffle Network Output Interleaver
//------------------------------------------
wrp_shff_fifo_in  #(RA_INIT, ram_delay) DIN( clk, srst, ready, vld_i, dat_i, buf_af, buf_we, buf_wdone, buf_wa, buf_wd);
wrp_shff_fifo_out #(        uram_delay) DOUT(clk, srst, buf_empty, buf_rdone, buf_ra, buf_rd, fifo_af, fifo_we, fifo_wd);
wrp_shff_fifo_ctrl CTRL(clk, srst, buf_wdone, buf_rdone, buf_af, buf_empty);

//------------------------------------------
// AIE Sync FIFO
//------------------------------------------
wrp_shff_fifo_axi S(clk, srst,  axi_trdy,axi_tvld,axi_tdat, fifo_af, fifo_we, fifo_wd);


//------------------------------------------
// URAM, 16K x 64Bits
//------------------------------------------
wrp_shff_fifo_uram#(14, 64, uram_delay) RAM(clk, buf_we, buf_wa, buf_wd, buf_ra, buf_rd);


endmodule
