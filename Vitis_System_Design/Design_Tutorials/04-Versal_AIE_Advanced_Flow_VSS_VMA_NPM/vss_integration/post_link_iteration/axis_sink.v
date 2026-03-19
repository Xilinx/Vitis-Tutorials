// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

// AXI Stream Sink
// This module acts as a consumer for AXI Stream data from the AI Engine.
// It captures the data for debugging with an integrated ILA core.
module axis_sink (
	input wire [31:0] s_axis_tdata,
	input wire [3:0] s_axis_tkeep,
	input wire s_axis_tlast,
	output wire s_axis_tready,
	input wire s_axis_tvalid,
	input wire aclk,
	input wire aresetn
);

	// Always ready to receive data
	assign s_axis_tready = 1'b1;

	// Dummy registers to hold input data, with keep attribute to prevent optimization
	(* keep = "true" *) reg [31:0] tdata_reg;
	(* keep = "true" *) reg [3:0] tkeep_reg;
	(* keep = "true" *) reg tlast_reg;
	(* keep = "true" *) reg tvalid_reg;

	always @(posedge aclk) begin
		if (!aresetn) begin
			tdata_reg <= 32'b0;
			tkeep_reg <= 4'b0;
			tlast_reg <= 1'b0;
			tvalid_reg <= 1'b0;
		end else if (s_axis_tvalid) begin
			tdata_reg <= s_axis_tdata;
			tkeep_reg <= s_axis_tkeep;
			tlast_reg <= s_axis_tlast;
			tvalid_reg <= s_axis_tvalid;
		end
	end

axis_ila_0 axis_ila_0 (
  .clk(aclk),                                // input wire clk
  .SLOT_0_AXIS_tdata(tdata_reg),    // input wire [31 : 0] SLOT_0_AXIS_tdata
  .SLOT_0_AXIS_tvalid(tvalid_reg),  // input wire SLOT_0_AXIS_tvalid
  .SLOT_0_AXIS_tready(s_axis_tready),  // input wire SLOT_0_AXIS_tready
  .SLOT_0_AXIS_tkeep(tkeep_reg),    // input wire [3 : 0] SLOT_0_AXIS_tkeep
  .SLOT_0_AXIS_tlast(tlast_reg),    // input wire SLOT_0_AXIS_tlast
  .resetn(aresetn)                          // input wire resetn
);


endmodule
