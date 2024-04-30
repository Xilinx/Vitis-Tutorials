//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1 ns/1 ps
module wrp_shff_sw_unit
#(parameter BITWIDTH = 64)
(
	input         clk,
	input                 sel_i,
	input  [BITWIDTH-1:0] x0_i,
	input  [BITWIDTH-1:0] x1_i,
	output reg [BITWIDTH-1:0] y_o
);


// one stage register for input
//reg  sel_s;
//reg  [BITWIDTH-1:0] x0_s;
//reg  [BITWIDTH-1:0] x1_s;


//----------------------------------------
// switching core
//----------------------------------------
always @(posedge clk) begin

	// register inputs
	//sel_s <= sel_i;
	//x0_s  <= x0_i;
	//x1_s  <= x1_i;
	
	// output
	//y_o <= sel_s? x1_s : x0_s;
	
	y_o <= sel_i? x1_i : x0_i;

end	

endmodule
