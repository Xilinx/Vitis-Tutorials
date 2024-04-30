//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module tst_din_dgen_prbs31
#(parameter r_init = 32'h89abcdef)
(
	input  clk,
	input  srst,
	input             en,
	output reg        vld_o,
	output reg [27:0] dat_o
);

reg [30:0] r;
reg vld_s;

always @(posedge clk) begin

	vld_s <= en;
	vld_o <= vld_s;
	
	dat_o <= r[30:3];

	r[ 0] <= srst? r_init[ 0] : en? r[28]: r[ 0];
	r[ 1] <= srst? r_init[ 1] : en? r[29]: r[ 1];
	r[ 2] <= srst? r_init[ 2] : en? r[30]: r[ 2];
	r[ 3] <= srst? r_init[ 3] : en? r[ 0] ^ r[ 3]: r[ 3];
	r[ 4] <= srst? r_init[ 4] : en? r[ 1] ^ r[ 4]: r[ 4];
	r[ 5] <= srst? r_init[ 5] : en? r[ 2] ^ r[ 5]: r[ 5];
	r[ 6] <= srst? r_init[ 6] : en? r[ 3] ^ r[ 6]: r[ 6];
	r[ 7] <= srst? r_init[ 7] : en? r[ 4] ^ r[ 7]: r[ 7];
	r[ 8] <= srst? r_init[ 8] : en? r[ 5] ^ r[ 8]: r[ 8];
	r[ 9] <= srst? r_init[ 9] : en? r[ 6] ^ r[ 9]: r[ 9];
	r[10] <= srst? r_init[10] : en? r[ 7] ^ r[10]: r[10];
	r[11] <= srst? r_init[11] : en? r[ 8] ^ r[11]: r[11];
	r[12] <= srst? r_init[12] : en? r[ 9] ^ r[12]: r[12];
	r[13] <= srst? r_init[13] : en? r[10] ^ r[13]: r[13];
	r[14] <= srst? r_init[14] : en? r[11] ^ r[14]: r[14];
	r[15] <= srst? r_init[15] : en? r[12] ^ r[15]: r[15];
	r[16] <= srst? r_init[16] : en? r[13] ^ r[16]: r[16];
	r[17] <= srst? r_init[17] : en? r[14] ^ r[17]: r[17];
	r[18] <= srst? r_init[18] : en? r[15] ^ r[18]: r[18];
	r[19] <= srst? r_init[19] : en? r[16] ^ r[19]: r[19];
	r[20] <= srst? r_init[20] : en? r[17] ^ r[20]: r[20];
	r[21] <= srst? r_init[21] : en? r[18] ^ r[21]: r[21];
	r[22] <= srst? r_init[22] : en? r[19] ^ r[22]: r[22];
	r[23] <= srst? r_init[23] : en? r[20] ^ r[23]: r[23];
	r[24] <= srst? r_init[24] : en? r[21] ^ r[24]: r[24];
	r[25] <= srst? r_init[25] : en? r[22] ^ r[25]: r[25];
	r[26] <= srst? r_init[26] : en? r[23] ^ r[26]: r[26];
	r[27] <= srst? r_init[27] : en? r[24] ^ r[27]: r[27];
	r[28] <= srst? r_init[28] : en? r[25] ^ r[28]: r[28];
	r[29] <= srst? r_init[29] : en? r[26] ^ r[29]: r[29];
	r[30] <= srst? r_init[30] : en? r[27] ^ r[30]: r[30];

end


endmodule

