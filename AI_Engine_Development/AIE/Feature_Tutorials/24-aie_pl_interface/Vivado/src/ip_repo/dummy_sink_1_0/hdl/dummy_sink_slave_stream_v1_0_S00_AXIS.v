/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

`timescale 1 ns / 1 ps

	module dummy_sink_slave_stream_v1_0_S00_AXIS #
	(
		// AXI4Stream sink: Data Width
		parameter integer C_S_AXIS_TDATA_WIDTH	= 64
	)
	(
		// AXI4Stream sink: Clock
		input wire  S_AXIS_ACLK,
		// AXI4Stream sink: Reset
		input wire  S_AXIS_ARESETN,
		// Ready to accept data in
		output wire  S_AXIS_TREADY,
		// Data in
		input wire [C_S_AXIS_TDATA_WIDTH-1 : 0] S_AXIS_TDATA,
		// Byte qualifier
		input wire [(C_S_AXIS_TDATA_WIDTH/8)-1 : 0] S_AXIS_TSTRB,
		// Indicates boundary of last packet
		input wire  S_AXIS_TLAST,
		// Data is in valid
		input wire  S_AXIS_TVALID
	);
	
	reg    tready;
	wire   clk;
	wire   aresetn;
	
	assign clk             = S_AXIS_ACLK;
	assign aresetn         = S_AXIS_ARESETN;
	assign S_AXIS_TREADY   = tready;
	
    // Process the incoming data
    always @(posedge clk or negedge aresetn) begin
        if (!aresetn) begin
            tready  <= 1'b0;
        end else begin
            tready <= 1'b1;
        end
    end

	endmodule
