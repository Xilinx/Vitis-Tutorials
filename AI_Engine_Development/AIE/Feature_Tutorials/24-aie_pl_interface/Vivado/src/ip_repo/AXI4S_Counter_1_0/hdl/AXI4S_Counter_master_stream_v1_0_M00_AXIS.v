/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

`timescale 1 ns / 1 ps

	module AXI4S_Counter_master_stream_v1_0_M00_AXIS #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line

		// Width of S_AXIS address bus. The slave accepts the read and write addresses of width C_M_AXIS_TDATA_WIDTH.
		parameter integer C_M_AXIS_TDATA_WIDTH	= 64

	)
	(
		// Users to add ports here

		// User ports ends
		// Do not modify the ports beyond this line

		// Global ports
		input wire  M_AXIS_ACLK,
		// 
		input wire  M_AXIS_ARESETN,
		// Master Stream Ports. TVALID indicates that the master is driving a valid transfer, A transfer takes place when both TVALID and TREADY are asserted. 
		output wire  M_AXIS_TVALID,
		// TDATA is the primary payload that is used to provide the data that is passing across the interface from the master.
		output wire [C_M_AXIS_TDATA_WIDTH-1 : 0] M_AXIS_TDATA,
		// TSTRB is the byte qualifier that indicates whether the content of the associated byte of TDATA is processed as a data byte or a position byte.
		output wire [(C_M_AXIS_TDATA_WIDTH/8)-1 : 0] M_AXIS_TSTRB,
		// TLAST indicates the boundary of a packet.
		output wire  M_AXIS_TLAST,
		// TREADY indicates that the slave can accept a transfer in the current cycle.
		input wire  M_AXIS_TREADY
	);
	
	// Signals definitions
	reg    tvalid;
	reg    [C_M_AXIS_TDATA_WIDTH-1:0]tdata;
	wire   tready;
	wire   clk;
	wire   aresetn;
    reg    [C_M_AXIS_TDATA_WIDTH-1:0] counter;
	
	// I/O Connections assignments

	assign M_AXIS_TVALID   = tvalid;
	assign M_AXIS_TDATA	   = tdata;
	assign M_AXIS_TLAST	   = 0;
	assign M_AXIS_TSTRB	   = {(C_M_AXIS_TDATA_WIDTH/8){1'b1}};
	assign clk             = M_AXIS_ACLK;
	assign aresetn         = M_AXIS_ARESETN;
	assign tready          = M_AXIS_TREADY;
	
	

    always @(posedge clk or negedge aresetn) begin
        if (aresetn == 0) begin
            counter <= {C_M_AXIS_TDATA_WIDTH{1'b0}};
            tvalid  <= 1'b0;
        end else begin
            if (tready && tvalid) begin
                counter <= counter + 1;
            end
            tvalid <= 1'b1;  // Always assert tvalid once reset is released
        end
    end

    always @(posedge clk or negedge aresetn) begin
        if (aresetn == 0) begin
            tdata <= {C_M_AXIS_TDATA_WIDTH{1'b0}};
        end else begin
            tdata <= counter;
        end
    end

    endmodule
