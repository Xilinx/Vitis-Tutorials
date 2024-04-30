//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1 ns/1 ps
module wrp_shff_sw
#(parameter BITWIDTH = 64)
(
	input         clk,
	input         srst,
	input         start_i,
	input  [BITWIDTH-1:0] x00_i,
	input  [BITWIDTH-1:0] x01_i,
	input  [BITWIDTH-1:0] x02_i,
	input  [BITWIDTH-1:0] x03_i,
	input  [BITWIDTH-1:0] x04_i,
	input  [BITWIDTH-1:0] x05_i,
	input  [BITWIDTH-1:0] x06_i,
	input  [BITWIDTH-1:0] x07_i,
	input  [BITWIDTH-1:0] x10_i,
	input  [BITWIDTH-1:0] x11_i,
	input  [BITWIDTH-1:0] x12_i,
	input  [BITWIDTH-1:0] x13_i,
	input  [BITWIDTH-1:0] x14_i,
	input  [BITWIDTH-1:0] x15_i,
	input  [BITWIDTH-1:0] x16_i,
	input  [BITWIDTH-1:0] x17_i,
	//
	output reg    vld_o,
	output reg [BITWIDTH-1:0] y00_o,
	output reg [BITWIDTH-1:0] y01_o,
	output reg [BITWIDTH-1:0] y02_o,
	output reg [BITWIDTH-1:0] y03_o,
	output reg [BITWIDTH-1:0] y04_o,
	output reg [BITWIDTH-1:0] y05_o,
	output reg [BITWIDTH-1:0] y06_o,
	output reg [BITWIDTH-1:0] y07_o,	
	output reg [BITWIDTH-1:0] y10_o,
	output reg [BITWIDTH-1:0] y11_o,
	output reg [BITWIDTH-1:0] y12_o,
	output reg [BITWIDTH-1:0] y13_o,
	output reg [BITWIDTH-1:0] y14_o,
	output reg [BITWIDTH-1:0] y15_o,
	output reg [BITWIDTH-1:0] y16_o,
	output reg [BITWIDTH-1:0] y17_o
);


// one stage register for input
reg  [4:0] cnt;
reg  sel_1, sel_2, sel_3, sel_2d, sel_3d, sel_3dd;
reg [BITWIDTH-1:0]  x[0:15];
wire [BITWIDTH-1:0] a[0:15], b[0:15], c[0:15], d[0:15];

reg [3:0] vld_s;


//----------------------------------------------------
// switching controller, every 8 cycles is one period
//----------------------------------------------------
always @(posedge clk) begin

	cnt <= srst? 0 : start_i? 16 : cnt[4]? cnt+1 : 0;
	
	x[ 0] <= x00_i;	x[ 1] <= x01_i;	x[2]  <= x02_i;	x[ 3] <= x03_i;
	x[ 4] <= x04_i;	x[ 5] <= x05_i;	x[6]  <= x06_i;	x[ 7] <= x07_i;
	x[ 8] <= x10_i;	x[ 9] <= x11_i;	x[10] <= x12_i;	x[11] <= x13_i;
	x[12] <= x14_i;	x[13] <= x15_i;	x[14] <= x16_i;	x[15] <= x17_i;
	
	sel_1  <= cnt[2];
	sel_2  <= cnt[1];  sel_2d <= sel_2;
	sel_3  <= cnt[0];  sel_3d <= sel_3; sel_3dd <= sel_3d;

	vld_s <= srst? 0 : {cnt[4], vld_s[3:1]};
	vld_o <= vld_s[0];
	
	y00_o <= d[0];	y01_o <= d[1];	y02_o <= d[2];	y03_o <= d[3];
	y04_o <= d[4];	y05_o <= d[5];	y06_o <= d[6];	y07_o <= d[7];
	y10_o <= d[8];	y11_o <= d[9];	y12_o <= d[10];	y13_o <= d[11];
	y14_o <= d[12];	y15_o <= d[13];	y16_o <= d[14];	y17_o <= d[15];

end


//-----------------------------------------
// Stage 0
//-----------------------------------------
wrp_shff_sw_unit #(BITWIDTH) S00(clk, cnt[3], x[0],   x[8+0], a[0] );
wrp_shff_sw_unit #(BITWIDTH) S01(clk, cnt[3], x[1],   x[8+1], a[1] );
wrp_shff_sw_unit #(BITWIDTH) S02(clk, cnt[3], x[2],   x[8+2], a[2] );
wrp_shff_sw_unit #(BITWIDTH) S03(clk, cnt[3], x[3],   x[8+3], a[3] );
wrp_shff_sw_unit #(BITWIDTH) S04(clk, cnt[3], x[4],   x[8+4], a[4] );
wrp_shff_sw_unit #(BITWIDTH) S05(clk, cnt[3], x[5],   x[8+5], a[5] );
wrp_shff_sw_unit #(BITWIDTH) S06(clk, cnt[3], x[6],   x[8+6], a[6] );
wrp_shff_sw_unit #(BITWIDTH) S07(clk, cnt[3], x[7],   x[8+7], a[7] );
wrp_shff_sw_unit #(BITWIDTH) S08(clk, cnt[3], x[0+8], x[0],   a[8] );
wrp_shff_sw_unit #(BITWIDTH) S09(clk, cnt[3], x[1+8], x[1],   a[9] );
wrp_shff_sw_unit #(BITWIDTH) S0a(clk, cnt[3], x[2+8], x[2],   a[10]);
wrp_shff_sw_unit #(BITWIDTH) S0b(clk, cnt[3], x[3+8], x[3],   a[11]);
wrp_shff_sw_unit #(BITWIDTH) S0c(clk, cnt[3], x[4+8], x[4],   a[12]);
wrp_shff_sw_unit #(BITWIDTH) S0d(clk, cnt[3], x[5+8], x[5],   a[13]);
wrp_shff_sw_unit #(BITWIDTH) S0e(clk, cnt[3], x[6+8], x[6],   a[14]);
wrp_shff_sw_unit #(BITWIDTH) S0f(clk, cnt[3], x[7+8], x[7],   a[15]);

//-----------------------------------------
// Stage 1
//-----------------------------------------
wrp_shff_sw_unit #(BITWIDTH) S10(clk, sel_1, a[0],   a[12], b[0] );
wrp_shff_sw_unit #(BITWIDTH) S11(clk, sel_1, a[1],   a[13], b[1] );
wrp_shff_sw_unit #(BITWIDTH) S12(clk, sel_1, a[2],   a[14], b[2] );
wrp_shff_sw_unit #(BITWIDTH) S13(clk, sel_1, a[3],   a[15], b[3] );
wrp_shff_sw_unit #(BITWIDTH) S14(clk, sel_1, a[4],   a[0 ], b[4] );
wrp_shff_sw_unit #(BITWIDTH) S15(clk, sel_1, a[5],   a[1 ], b[5] );
wrp_shff_sw_unit #(BITWIDTH) S16(clk, sel_1, a[6],   a[2 ], b[6] );
wrp_shff_sw_unit #(BITWIDTH) S17(clk, sel_1, a[7],   a[3 ], b[7] );
wrp_shff_sw_unit #(BITWIDTH) S18(clk, sel_1, a[0+8], a[4 ], b[8] );
wrp_shff_sw_unit #(BITWIDTH) S19(clk, sel_1, a[1+8], a[5 ], b[9] );
wrp_shff_sw_unit #(BITWIDTH) S1a(clk, sel_1, a[2+8], a[6 ], b[10]);
wrp_shff_sw_unit #(BITWIDTH) S1b(clk, sel_1, a[3+8], a[7 ], b[11]);
wrp_shff_sw_unit #(BITWIDTH) S1c(clk, sel_1, a[4+8], a[8 ], b[12]);
wrp_shff_sw_unit #(BITWIDTH) S1d(clk, sel_1, a[5+8], a[9 ], b[13]);
wrp_shff_sw_unit #(BITWIDTH) S1e(clk, sel_1, a[6+8], a[10], b[14]);
wrp_shff_sw_unit #(BITWIDTH) S1f(clk, sel_1, a[7+8], a[11], b[15]);

//-----------------------------------------
// Stage 2
//-----------------------------------------
wrp_shff_sw_unit #(BITWIDTH) S20(clk, sel_2d, b[0],   b[14], c[0 ]);
wrp_shff_sw_unit #(BITWIDTH) S21(clk, sel_2d, b[1],   b[15], c[1 ]);
wrp_shff_sw_unit #(BITWIDTH) S22(clk, sel_2d, b[2],   b[0 ], c[2 ]);
wrp_shff_sw_unit #(BITWIDTH) S23(clk, sel_2d, b[3],   b[1 ], c[3 ]);
wrp_shff_sw_unit #(BITWIDTH) S24(clk, sel_2d, b[4],   b[2 ], c[4 ]);
wrp_shff_sw_unit #(BITWIDTH) S25(clk, sel_2d, b[5],   b[3 ], c[5 ]);
wrp_shff_sw_unit #(BITWIDTH) S26(clk, sel_2d, b[6],   b[4 ], c[6 ]);
wrp_shff_sw_unit #(BITWIDTH) S27(clk, sel_2d, b[7],   b[5 ], c[7 ]);
wrp_shff_sw_unit #(BITWIDTH) S28(clk, sel_2d, b[0+8], b[6 ], c[8 ]);
wrp_shff_sw_unit #(BITWIDTH) S29(clk, sel_2d, b[1+8], b[7 ], c[9 ]);
wrp_shff_sw_unit #(BITWIDTH) S2a(clk, sel_2d, b[2+8], b[8 ], c[10]);
wrp_shff_sw_unit #(BITWIDTH) S2b(clk, sel_2d, b[3+8], b[9 ], c[11]);
wrp_shff_sw_unit #(BITWIDTH) S2c(clk, sel_2d, b[4+8], b[10], c[12]);
wrp_shff_sw_unit #(BITWIDTH) S2d(clk, sel_2d, b[5+8], b[11], c[13]);
wrp_shff_sw_unit #(BITWIDTH) S2e(clk, sel_2d, b[6+8], b[12], c[14]);
wrp_shff_sw_unit #(BITWIDTH) S2f(clk, sel_2d, b[7+8], b[13], c[15]);


//-----------------------------------------
// Stage 3
//-----------------------------------------
wrp_shff_sw_unit #(BITWIDTH) S30(clk, sel_3dd, c[0],   c[15], d[0] );
wrp_shff_sw_unit #(BITWIDTH) S31(clk, sel_3dd, c[1],   c[0],  d[1] );
wrp_shff_sw_unit #(BITWIDTH) S32(clk, sel_3dd, c[2],   c[1],  d[2] );
wrp_shff_sw_unit #(BITWIDTH) S33(clk, sel_3dd, c[3],   c[2],  d[3] );
wrp_shff_sw_unit #(BITWIDTH) S34(clk, sel_3dd, c[4],   c[3],  d[4] );
wrp_shff_sw_unit #(BITWIDTH) S35(clk, sel_3dd, c[5],   c[4],  d[5] );
wrp_shff_sw_unit #(BITWIDTH) S36(clk, sel_3dd, c[6],   c[5],  d[6] );
wrp_shff_sw_unit #(BITWIDTH) S37(clk, sel_3dd, c[7],   c[6],  d[7] );
wrp_shff_sw_unit #(BITWIDTH) S38(clk, sel_3dd, c[0+8], c[7],  d[8] );
wrp_shff_sw_unit #(BITWIDTH) S39(clk, sel_3dd, c[1+8], c[8],  d[9] );
wrp_shff_sw_unit #(BITWIDTH) S3a(clk, sel_3dd, c[2+8], c[9],  d[10]);
wrp_shff_sw_unit #(BITWIDTH) S3b(clk, sel_3dd, c[3+8], c[10], d[11]);
wrp_shff_sw_unit #(BITWIDTH) S3c(clk, sel_3dd, c[4+8], c[11], d[12]);
wrp_shff_sw_unit #(BITWIDTH) S3d(clk, sel_3dd, c[5+8], c[12], d[13]);
wrp_shff_sw_unit #(BITWIDTH) S3e(clk, sel_3dd, c[6+8], c[13], d[14]);
wrp_shff_sw_unit #(BITWIDTH) S3f(clk, sel_3dd, c[7+8], c[14], d[15]);

//-----------------------------------------



endmodule
