//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module wrp_shff_fifo_ctrl
(
	input  clk,
	input  srst,
	//------------------------ Interface with Shuffle Network
	input buf_wdone,
	input buf_rdone,
	//------------------------ Interface with AIE
	output reg buf_af,
	output reg buf_empty
);


reg [9:0] wcnt;
reg [5:0] rcnt;

reg [10:0] w_tok, r_tok;

reg wcnt_b3, wcnt_b3_d, w_x8, is_w_last;
reg rcnt_b5, rcnt_b5_d, r_x32;


always@(posedge clk)  begin

	wcnt <= srst? 0 : buf_wdone? wcnt+1 : wcnt;
	rcnt <= srst? 0 : buf_rdone? rcnt+1 : rcnt;
	
	wcnt_b3   <= wcnt[3];
	wcnt_b3_d <= wcnt_b3;
	w_x8 <= wcnt_b3 ^ wcnt_b3_d;
	
	rcnt_b5   <= rcnt[5];
	rcnt_b5_d <= rcnt_b5;
	r_x32 <= rcnt_b5 ^ rcnt_b5_d;

	// write token is set to be 1024 at beginning
	// every wdone reduces the write token by one
	// every 32 rdones will increase the write token by 32
	// almost full is asserted when write token is less than 32
	w_tok <= srst? 11'd1024 : (w_x8 & r_x32)? (w_tok+24) : w_x8? (w_tok-8) : r_x32? (w_tok+32) : w_tok;
	
	// almost full is asserted when there are less than 16 data blocks can be written
	buf_af <= ~|w_tok[10:4];

	// read token is set to 0 at beginning
	// when wcnt[9:5] are set, it is going to be the last col/row
	// every wdone addes 32 tokens
	// every read done reduces the token by 1
	is_w_last <= &{wcnt[9:5], buf_wdone};
	r_tok <= srst? 0 : (is_w_last & buf_rdone)? (r_tok+31) :  is_w_last? (r_tok+32) : buf_rdone? (r_tok-1) : r_tok;
	
	buf_empty <= ~|r_tok;
	
end


endmodule
