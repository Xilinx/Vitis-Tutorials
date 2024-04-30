//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module tst_din_dgen
#(parameter rini = 64'h1234589abcdef)
(
	input  clk,
	input  srst,
	input   start,
	output  reg done,
	input              fifo_af,
	output reg         fifo_we,
	output reg [63:0]  fifo_wd
);


localparam ROMBWA = 16;
localparam romlatency = 1;

//-----------------------------------------------
// one FFT is 2^20 / 2^5 /2 = 2^14 samples
// every iteration is 4 FFTs, so 2^16
//-----------------------------------------------


reg re;
reg  [ROMBWA:0] cnt;

reg  [romlatency:0] shft;
reg  [romlatency:0] done_p;
wire vld_p;
wire [27:0] dat_r;
wire [27:0] dat_i;
wire [63:0] fifo_wd_p;

reg done_p0, done_p1;

reg srst_p;


always @(posedge clk) begin

	// counter
	cnt  <= srst? 0 : start? -65536 : (cnt[ROMBWA]& (~fifo_af))? cnt+1 : cnt;
	
	// read enable signal
	re <= cnt[ROMBWA]&(~fifo_af);
	
	// delay valid to align with memory output
	shft   <= srst? 0  : {re, shft[romlatency:1]};

	done_p0 <= ~cnt[ROMBWA];
	done_p1 <= done_p0;
	done_p <= srst? 0 : {done_p0 & (~done_p1), done_p[romlatency:1]};
	
	fifo_we <= shft[0];
	fifo_wd <= fifo_wd_p;
	
	done <= srst? 1'b1 : start? 1'b0 : done_p? 1'b1 : done;
	
	srst_p <= srst | done_p;
	
end

assign fifo_wd_p = {dat_i[27], 4'd8, dat_i[26:0],  dat_r[27], 4'd8, dat_r[26:0]};

// prbs generator
tst_din_dgen_prbs31 #(rini[31:0] ) PRBS_1(clk,srst_p,re, vld_p, dat_r);
tst_din_dgen_prbs31 #(rini[63:32]) PRBS_2(clk,srst_p,re, vld_p, dat_i);




endmodule