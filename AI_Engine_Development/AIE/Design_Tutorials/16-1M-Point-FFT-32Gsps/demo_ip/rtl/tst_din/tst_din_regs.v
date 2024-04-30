//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns/1ps
module tst_din_regs
#(parameter
    C_S_AXI_ADDR_WIDTH = 12,
    C_S_AXI_DATA_WIDTH = 32
)(
    input                           ACLK,
    input                           ARESET,
    input  [C_S_AXI_ADDR_WIDTH-1:0] AWADDR,
    input                           AWVALID,
    output                          AWREADY,
    input  [C_S_AXI_DATA_WIDTH-1:0] WDATA,
    input  [C_S_AXI_DATA_WIDTH/8-1:0] WSTRB,
    input                           WVALID,
    output                          WREADY,
    output [1:0]                    BRESP,
    output                          BVALID,
    input                           BREADY,
    input  [C_S_AXI_ADDR_WIDTH-1:0] ARADDR,
    input                           ARVALID,
    output                          ARREADY,
    output [C_S_AXI_DATA_WIDTH-1:0] RDATA,
    output [1:0]                    RRESP,
    output                          RVALID,
    input                           RREADY,
	//-----------------------------------------
	output reg test_en,
	input [31:0] itecnt
);

//-------------------------------------------------
// Test Memory Space
//-------------------------------------------------
// 0x20 : Test Control Registers
//        bit 0  - Test Enable  (Read/Write)
// 0x24 : Iteration Cout (Read Only)
//------------------------Parameter----------------------
localparam ADDR_ONOFF  = 12'h20;
localparam ADDR_NITE   = 12'h24;
//-------------------------------------------------------
localparam WRIDLE       = 2'd0;
localparam WRDATA       = 2'd1;
localparam WRRESP       = 2'd2;
localparam WRRESET      = 2'd3;
localparam RDIDLE       = 2'd0;
localparam RDDATA       = 2'd1;
localparam RDRESET      = 2'd2;
localparam ADDR_BITS    = C_S_AXI_ADDR_WIDTH;
//----------------------------------------------------------

//------------------------Local signal-------------------
reg  [1:0]            wstate = WRRESET;
reg  [ADDR_BITS-1:0]  waddr;
wire [31:0]           wmask;
wire                  aw_hs;
wire                  w_hs;
reg  [1:0]            rstate = RDRESET;
reg  [31:0]           rdata;
wire                  ar_hs;
wire [ADDR_BITS-1:0]  raddr;


//------------------------AXI write fsm------------------
assign AWREADY = (wstate == WRIDLE);
assign WREADY  = (wstate == WRDATA);
assign BRESP   = 2'b00;  // OKAY
assign BVALID  = (wstate == WRRESP);
assign wmask   = { {8{WSTRB[3]}}, {8{WSTRB[2]}}, {8{WSTRB[1]}}, {8{WSTRB[0]}} };
assign aw_hs   = AWVALID & AWREADY;
assign w_hs    = WVALID & WREADY;

assign ARREADY = (rstate == RDIDLE);
assign RDATA   = rdata;
assign RRESP   = 2'b00;  // OKAY
assign RVALID  = (rstate == RDDATA);
assign ar_hs   = ARVALID & ARREADY;
assign raddr   = ARADDR[ADDR_BITS-1:0];

always @(posedge ACLK) begin

	if (aw_hs)  waddr <= AWADDR[ADDR_BITS-1:0];

	// write state machine
	if (ARESET) wstate <= WRRESET;
	else begin
		case (wstate)
			WRIDLE: wstate <= AWVALID? WRDATA : wstate;
			WRDATA: wstate <= WVALID?  WRRESP : wstate;
			WRRESP: wstate <= BREADY?  WRIDLE : wstate;
			default:wstate <= WRIDLE;
		endcase
	end
	
	
	// read state machine
	if (ARESET) rstate <= RDRESET;
	else begin
		case (rstate)
			RDIDLE:  rstate <= ARVALID?           RDDATA : rstate;
			RDDATA:  rstate <= (RREADY & RVALID)? RDIDLE : rstate;
			default: rstate <= RDIDLE;
		endcase
	end
	
end
//---------------------------------------------------------------------------	
	

//-----------------------------
// READ DATA MUX
//-----------------------------
always @(posedge ACLK) begin
    if (ar_hs) begin
        case (raddr)
			ADDR_ONOFF : rdata <= {31'd0, test_en};
			ADDR_NITE  : rdata <= itecnt;
			default    : rdata <= 32'b0;
        endcase
    end
end



//-----------------------------
// Registers
//-----------------------------
always @(posedge ACLK) test_en <= ARESET? 1'b0 : (w_hs  && (waddr == ADDR_ONOFF))? (WDATA[0] & wmask[0])   : test_en;


endmodule
