//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns/1ps
module tst_dout_regs
#(parameter
    C_S_AXI_ADDR_WIDTH = 8,
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
	input test_en,
	input [47:0] r00a_smp,
	input [47:0] r00a_err,
	input [47:0] r00a_idl,
	input [15:0] r00a_lat,
	input [47:0] r01a_smp,
	input [47:0] r01a_err,
	input [47:0] r01a_idl,
	input [15:0] r01a_lat,
	input [47:0] r02a_smp,
	input [47:0] r02a_err,
	input [47:0] r02a_idl,
	input [15:0] r02a_lat,
	input [47:0] r03a_smp,
	input [47:0] r03a_err,
	input [47:0] r03a_idl,
	input [15:0] r03a_lat,
	input [47:0] r04a_smp,
	input [47:0] r04a_err,
	input [47:0] r04a_idl,
	input [15:0] r04a_lat,
	input [47:0] r05a_smp,
	input [47:0] r05a_err,
	input [47:0] r05a_idl,
	input [15:0] r05a_lat,
	input [47:0] r06a_smp,
	input [47:0] r06a_err,
	input [47:0] r06a_idl,
	input [15:0] r06a_lat,
	input [47:0] r07a_smp,
	input [47:0] r07a_err,
	input [47:0] r07a_idl,
	input [15:0] r07a_lat,
	input [47:0] r08a_smp,
	input [47:0] r08a_err,
	input [47:0] r08a_idl,
	input [15:0] r08a_lat,
	input [47:0] r09a_smp,
	input [47:0] r09a_err,
	input [47:0] r09a_idl,
	input [15:0] r09a_lat,
	input [47:0] r10a_smp,
	input [47:0] r10a_err,
	input [47:0] r10a_idl,
	input [15:0] r10a_lat,
	input [47:0] r11a_smp,
	input [47:0] r11a_err,
	input [47:0] r11a_idl,
	input [15:0] r11a_lat,
	input [47:0] r12a_smp,
	input [47:0] r12a_err,
	input [47:0] r12a_idl,
	input [15:0] r12a_lat,
	input [47:0] r13a_smp,
	input [47:0] r13a_err,
	input [47:0] r13a_idl,
	input [15:0] r13a_lat,
	input [47:0] r14a_smp,
	input [47:0] r14a_err,
	input [47:0] r14a_idl,
	input [15:0] r14a_lat,
	input [47:0] r15a_smp,
	input [47:0] r15a_err,
	input [47:0] r15a_idl,
	input [15:0] r15a_lat,	
	input [47:0] r16a_smp,
	input [47:0] r16a_err,
	input [47:0] r16a_idl,
	input [15:0] r16a_lat,
	input [47:0] r17a_smp,
	input [47:0] r17a_err,
	input [47:0] r17a_idl,
	input [15:0] r17a_lat,
	input [47:0] r18a_smp,
	input [47:0] r18a_err,
	input [47:0] r18a_idl,
	input [15:0] r18a_lat,
	input [47:0] r19a_smp,
	input [47:0] r19a_err,
	input [47:0] r19a_idl,
	input [15:0] r19a_lat,
	input [47:0] r20a_smp,
	input [47:0] r20a_err,
	input [47:0] r20a_idl,
	input [15:0] r20a_lat,
	input [47:0] r21a_smp,
	input [47:0] r21a_err,
	input [47:0] r21a_idl,
	input [15:0] r21a_lat,
	input [47:0] r22a_smp,
	input [47:0] r22a_err,
	input [47:0] r22a_idl,
	input [15:0] r22a_lat,
	input [47:0] r23a_smp,
	input [47:0] r23a_err,
	input [47:0] r23a_idl,
	input [15:0] r23a_lat,
	input [47:0] r24a_smp,
	input [47:0] r24a_err,
	input [47:0] r24a_idl,
	input [15:0] r24a_lat,
	input [47:0] r25a_smp,
	input [47:0] r25a_err,
	input [47:0] r25a_idl,
	input [15:0] r25a_lat,
	input [47:0] r26a_smp,
	input [47:0] r26a_err,
	input [47:0] r26a_idl,
	input [15:0] r26a_lat,
	input [47:0] r27a_smp,
	input [47:0] r27a_err,
	input [47:0] r27a_idl,
	input [15:0] r27a_lat,
	input [47:0] r28a_smp,
	input [47:0] r28a_err,
	input [47:0] r28a_idl,
	input [15:0] r28a_lat,
	input [47:0] r29a_smp,
	input [47:0] r29a_err,
	input [47:0] r29a_idl,
	input [15:0] r29a_lat,
	input [47:0] r30a_smp,
	input [47:0] r30a_err,
	input [47:0] r30a_idl,
	input [15:0] r30a_lat,
	input [47:0] r31a_smp,
	input [47:0] r31a_err,
	input [47:0] r31a_idl,
	input [15:0] r31a_lat,		
	input [47:0] r00b_smp,
	input [47:0] r00b_err,
	input [47:0] r00b_idl,
	input [15:0] r00b_lat,
	input [47:0] r01b_smp,
	input [47:0] r01b_err,
	input [47:0] r01b_idl,
	input [15:0] r01b_lat,
	input [47:0] r02b_smp,
	input [47:0] r02b_err,
	input [47:0] r02b_idl,
	input [15:0] r02b_lat,
	input [47:0] r03b_smp,
	input [47:0] r03b_err,
	input [47:0] r03b_idl,
	input [15:0] r03b_lat,
	input [47:0] r04b_smp,
	input [47:0] r04b_err,
	input [47:0] r04b_idl,
	input [15:0] r04b_lat,
	input [47:0] r05b_smp,
	input [47:0] r05b_err,
	input [47:0] r05b_idl,
	input [15:0] r05b_lat,
	input [47:0] r06b_smp,
	input [47:0] r06b_err,
	input [47:0] r06b_idl,
	input [15:0] r06b_lat,
	input [47:0] r07b_smp,
	input [47:0] r07b_err,
	input [47:0] r07b_idl,
	input [15:0] r07b_lat,
	input [47:0] r08b_smp,
	input [47:0] r08b_err,
	input [47:0] r08b_idl,
	input [15:0] r08b_lat,
	input [47:0] r09b_smp,
	input [47:0] r09b_err,
	input [47:0] r09b_idl,
	input [15:0] r09b_lat,
	input [47:0] r10b_smp,
	input [47:0] r10b_err,
	input [47:0] r10b_idl,
	input [15:0] r10b_lat,
	input [47:0] r11b_smp,
	input [47:0] r11b_err,
	input [47:0] r11b_idl,
	input [15:0] r11b_lat,
	input [47:0] r12b_smp,
	input [47:0] r12b_err,
	input [47:0] r12b_idl,
	input [15:0] r12b_lat,
	input [47:0] r13b_smp,
	input [47:0] r13b_err,
	input [47:0] r13b_idl,
	input [15:0] r13b_lat,
	input [47:0] r14b_smp,
	input [47:0] r14b_err,
	input [47:0] r14b_idl,
	input [15:0] r14b_lat,
	input [47:0] r15b_smp,
	input [47:0] r15b_err,
	input [47:0] r15b_idl,
	input [15:0] r15b_lat,
	input [47:0] r16b_smp,
	input [47:0] r16b_err,
	input [47:0] r16b_idl,
	input [15:0] r16b_lat,
	input [47:0] r17b_smp,
	input [47:0] r17b_err,
	input [47:0] r17b_idl,
	input [15:0] r17b_lat,
	input [47:0] r18b_smp,
	input [47:0] r18b_err,
	input [47:0] r18b_idl,
	input [15:0] r18b_lat,
	input [47:0] r19b_smp,
	input [47:0] r19b_err,
	input [47:0] r19b_idl,
	input [15:0] r19b_lat,
	input [47:0] r20b_smp,
	input [47:0] r20b_err,
	input [47:0] r20b_idl,
	input [15:0] r20b_lat,
	input [47:0] r21b_smp,
	input [47:0] r21b_err,
	input [47:0] r21b_idl,
	input [15:0] r21b_lat,
	input [47:0] r22b_smp,
	input [47:0] r22b_err,
	input [47:0] r22b_idl,
	input [15:0] r22b_lat,
	input [47:0] r23b_smp,
	input [47:0] r23b_err,
	input [47:0] r23b_idl,
	input [15:0] r23b_lat,
	input [47:0] r24b_smp,
	input [47:0] r24b_err,
	input [47:0] r24b_idl,
	input [15:0] r24b_lat,
	input [47:0] r25b_smp,
	input [47:0] r25b_err,
	input [47:0] r25b_idl,
	input [15:0] r25b_lat,
	input [47:0] r26b_smp,
	input [47:0] r26b_err,
	input [47:0] r26b_idl,
	input [15:0] r26b_lat,
	input [47:0] r27b_smp,
	input [47:0] r27b_err,
	input [47:0] r27b_idl,
	input [15:0] r27b_lat,
	input [47:0] r28b_smp,
	input [47:0] r28b_err,
	input [47:0] r28b_idl,
	input [15:0] r28b_lat,
	input [47:0] r29b_smp,
	input [47:0] r29b_err,
	input [47:0] r29b_idl,
	input [15:0] r29b_lat,
	input [47:0] r30b_smp,
	input [47:0] r30b_err,
	input [47:0] r30b_idl,
	input [15:0] r30b_lat,
	input [47:0] r31b_smp,
	input [47:0] r31b_err,
	input [47:0] r31b_idl,
	input [15:0] r31b_lat,			
	input        r_done
);

//-------------------------------------------------
// Test Memory Space
//-------------------------------------------------
// 0x20 : Test Control Registers
//        bit 0  - Test Enable  (Read/Write)
//-------------------------------------------------
// 0x28 : Antenna Group Selector
// 0x2C : MSB of Total Number of Samples
// 0x30 : MSB of Total Number of Errors
// 0x34 : LSB of Num of Samples + Errors
// 0x38 : MSB of Number of Idle Samples
// 0x3C : LSB of Num of Idles + Measured Latency
//-------------------------------------------------

//------------------------Parameter----------------------
localparam ADDR_ONOFF  = 6'h20;
localparam ADDR_ASEL   = 6'h28;
localparam ADDR_RES0   = 6'h2C;
localparam ADDR_RES1   = 6'h30;
localparam ADDR_RES2   = 6'h34;
localparam ADDR_RES3   = 6'h38;
localparam ADDR_RES4   = 6'h3C;
//-------------------------------------------------------
localparam WRIDLE       = 2'd0;
localparam WRDATA       = 2'd1;
localparam WRRESP       = 2'd2;
localparam WRRESET      = 2'd3;
localparam RDIDLE       = 2'd0;
localparam RDDATA       = 2'd1;
localparam RDRESET      = 2'd2;
localparam ADDR_BITS    = 6;
//----------------------------------------------------------

reg [5:0]  asel;
reg [47:0] r_smp;
reg [47:0] r_err;
reg [47:0] r_idl;
reg [15:0] r_lat;


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
			ADDR_ONOFF : rdata <= {15'd0,r_done, 15'd0, test_en};
			ADDR_ASEL  : rdata <= {29'd0, asel};
			ADDR_RES0  : rdata <= r_smp[47:16];
			ADDR_RES1  : rdata <= r_err[47:16];
			ADDR_RES2  : rdata <= {r_err[15:0], r_smp[15:0]};
			ADDR_RES3  : rdata <= r_idl[47:16];
			ADDR_RES4  : rdata <= {r_idl[15:0], r_lat};
			default    : rdata <= 32'b0;
        endcase
    end
end



//-----------------------------
// Registers
//-----------------------------
always @(posedge ACLK) begin


	//test_en    <= ARESET? 1'b0 : (w_hs  && waddr == ADDR_ONOFF)? WDATA[0] & wmask[0]    : test_en; 
    asel       <= ARESET? 6'd0 : (w_hs  && waddr == ADDR_ASEL)? WDATA[5:0] & wmask[5:0] : asel; 
	
	r_smp <=  (asel==6'h0 )? r00a_smp
			: (asel==6'h1 )? r01a_smp
			: (asel==6'h2 )? r02a_smp
			: (asel==6'h3 )? r03a_smp
			: (asel==6'h4 )? r04a_smp
			: (asel==6'h5 )? r05a_smp
			: (asel==6'h6 )? r06a_smp
			: (asel==6'h7 )? r07a_smp
			: (asel==6'h8 )? r08a_smp
			: (asel==6'h9 )? r09a_smp
			: (asel==6'ha )? r10a_smp
			: (asel==6'hb )? r11a_smp
			: (asel==6'hc )? r12a_smp
			: (asel==6'hd )? r13a_smp
			: (asel==6'he )? r14a_smp
			: (asel==6'hf )? r15a_smp
			: (asel==6'h10)? r16a_smp			
			: (asel==6'h11)? r17a_smp			
			: (asel==6'h12)? r18a_smp			
			: (asel==6'h13)? r19a_smp			
			: (asel==6'h14)? r20a_smp			
			: (asel==6'h15)? r21a_smp			
			: (asel==6'h16)? r22a_smp			
			: (asel==6'h17)? r23a_smp			
			: (asel==6'h18)? r24a_smp			
			: (asel==6'h19)? r25a_smp			
			: (asel==6'h1a)? r26a_smp			
			: (asel==6'h1b)? r27a_smp			
			: (asel==6'h1c)? r28a_smp			
			: (asel==6'h1d)? r29a_smp			
			: (asel==6'h1e)? r30a_smp			
			: (asel==6'h1f)? r31a_smp	
			: (asel==6'h20)? r00b_smp
			: (asel==6'h21)? r01b_smp
			: (asel==6'h22)? r02b_smp
			: (asel==6'h23)? r03b_smp
			: (asel==6'h24)? r04b_smp
			: (asel==6'h25)? r05b_smp
			: (asel==6'h26)? r06b_smp
			: (asel==6'h27)? r07b_smp
			: (asel==6'h28)? r08b_smp
			: (asel==6'h29)? r09b_smp
			: (asel==6'h2a)? r10b_smp
			: (asel==6'h2b)? r11b_smp
			: (asel==6'h2c)? r12b_smp
			: (asel==6'h2d)? r13b_smp
			: (asel==6'h2e)? r14b_smp
			: (asel==6'h2f)? r15b_smp
			: (asel==6'h30)? r16b_smp			
			: (asel==6'h31)? r17b_smp			
			: (asel==6'h32)? r18b_smp			
			: (asel==6'h33)? r19b_smp			
			: (asel==6'h34)? r20b_smp			
			: (asel==6'h35)? r21b_smp			
			: (asel==6'h36)? r22b_smp			
			: (asel==6'h37)? r23b_smp			
			: (asel==6'h38)? r24b_smp			
			: (asel==6'h39)? r25b_smp			
			: (asel==6'h3a)? r26b_smp			
			: (asel==6'h3b)? r27b_smp			
			: (asel==6'h3c)? r28b_smp			
			: (asel==6'h3d)? r29b_smp			
			: (asel==6'h3e)? r30b_smp			
			: (asel==6'h3f)? r31b_smp		
			: -1;
			
			
		
	r_err <=  (asel==6'h0 )? r00a_err
			: (asel==6'h1 )? r01a_err
			: (asel==6'h2 )? r02a_err
			: (asel==6'h3 )? r03a_err
			: (asel==6'h4 )? r04a_err
			: (asel==6'h5 )? r05a_err
			: (asel==6'h6 )? r06a_err
			: (asel==6'h7 )? r07a_err
			: (asel==6'h8 )? r08a_err
            : (asel==6'h9 )? r09a_err
            : (asel==6'ha )? r10a_err
            : (asel==6'hb )? r11a_err
            : (asel==6'hc )? r12a_err
            : (asel==6'hd )? r13a_err
            : (asel==6'he )? r14a_err
            : (asel==6'hf )? r15a_err
            : (asel==6'h10)? r16a_err
            : (asel==6'h11)? r17a_err
            : (asel==6'h12)? r18a_err
            : (asel==6'h13)? r19a_err
            : (asel==6'h14)? r20a_err
            : (asel==6'h15)? r21a_err
            : (asel==6'h16)? r22a_err
            : (asel==6'h17)? r23a_err
            : (asel==6'h18)? r24a_err
            : (asel==6'h19)? r25a_err
            : (asel==6'h1a)? r26a_err
            : (asel==6'h1b)? r27a_err
            : (asel==6'h1c)? r28a_err
            : (asel==6'h1d)? r29a_err
            : (asel==6'h1e)? r30a_err
            : (asel==6'h1f)? r31a_err
            : (asel==6'h20)? r00b_err
            : (asel==6'h21)? r01b_err
            : (asel==6'h22)? r02b_err
            : (asel==6'h23)? r03b_err
            : (asel==6'h24)? r04b_err
            : (asel==6'h25)? r05b_err
            : (asel==6'h26)? r06b_err
            : (asel==6'h27)? r07b_err
            : (asel==6'h28)? r08b_err
            : (asel==6'h29)? r09b_err
            : (asel==6'h2a)? r10b_err
            : (asel==6'h2b)? r11b_err
            : (asel==6'h2c)? r12b_err
            : (asel==6'h2d)? r13b_err
            : (asel==6'h2e)? r14b_err
            : (asel==6'h2f)? r15b_err
            : (asel==6'h30)? r16b_err
            : (asel==6'h31)? r17b_err
            : (asel==6'h32)? r18b_err
            : (asel==6'h33)? r19b_err
            : (asel==6'h34)? r20b_err
            : (asel==6'h35)? r21b_err
            : (asel==6'h36)? r22b_err
            : (asel==6'h37)? r23b_err
            : (asel==6'h38)? r24b_err
            : (asel==6'h39)? r25b_err
            : (asel==6'h3a)? r26b_err
            : (asel==6'h3b)? r27b_err
            : (asel==6'h3c)? r28b_err
            : (asel==6'h3d)? r29b_err
            : (asel==6'h3e)? r30b_err
            : (asel==6'h3f)? r31b_err
            : -1;


	r_idl <=  (asel==6'h0 )? r00a_idl
			: (asel==6'h1 )? r01a_idl
			: (asel==6'h2 )? r02a_idl
			: (asel==6'h3 )? r03a_idl
			: (asel==6'h4 )? r04a_idl
			: (asel==6'h5 )? r05a_idl
			: (asel==6'h6 )? r06a_idl
			: (asel==6'h7 )? r07a_idl
			: (asel==6'h8 )? r08a_idl
            : (asel==6'h9 )? r09a_idl
            : (asel==6'ha )? r10a_idl
            : (asel==6'hb )? r11a_idl
            : (asel==6'hc )? r12a_idl
            : (asel==6'hd )? r13a_idl
            : (asel==6'he )? r14a_idl
            : (asel==6'hf )? r15a_idl
            : (asel==6'h10)? r16a_idl
            : (asel==6'h11)? r17a_idl
            : (asel==6'h12)? r18a_idl
            : (asel==6'h13)? r19a_idl
            : (asel==6'h14)? r20a_idl
            : (asel==6'h15)? r21a_idl
            : (asel==6'h16)? r22a_idl
            : (asel==6'h17)? r23a_idl
            : (asel==6'h18)? r24a_idl
            : (asel==6'h19)? r25a_idl
            : (asel==6'h1a)? r26a_idl
            : (asel==6'h1b)? r27a_idl
            : (asel==6'h1c)? r28a_idl
            : (asel==6'h1d)? r29a_idl
            : (asel==6'h1e)? r30a_idl
            : (asel==6'h1f)? r31a_idl
            : (asel==6'h20)? r00b_idl
            : (asel==6'h21)? r01b_idl
            : (asel==6'h22)? r02b_idl
            : (asel==6'h23)? r03b_idl
            : (asel==6'h24)? r04b_idl
            : (asel==6'h25)? r05b_idl
            : (asel==6'h26)? r06b_idl
            : (asel==6'h27)? r07b_idl
            : (asel==6'h28)? r08b_idl
            : (asel==6'h29)? r09b_idl
            : (asel==6'h2a)? r10b_idl
            : (asel==6'h2b)? r11b_idl
            : (asel==6'h2c)? r12b_idl
            : (asel==6'h2d)? r13b_idl
            : (asel==6'h2e)? r14b_idl
            : (asel==6'h2f)? r15b_idl
            : (asel==6'h30)? r16b_idl
            : (asel==6'h31)? r17b_idl
            : (asel==6'h32)? r18b_idl
            : (asel==6'h33)? r19b_idl
            : (asel==6'h34)? r20b_idl
            : (asel==6'h35)? r21b_idl
            : (asel==6'h36)? r22b_idl
            : (asel==6'h37)? r23b_idl
            : (asel==6'h38)? r24b_idl
            : (asel==6'h39)? r25b_idl
            : (asel==6'h3a)? r26b_idl
            : (asel==6'h3b)? r27b_idl
            : (asel==6'h3c)? r28b_idl
            : (asel==6'h3d)? r29b_idl
            : (asel==6'h3e)? r30b_idl
            : (asel==6'h3f)? r31b_idl
            : -1;


	r_lat <=  (asel==6'h0 )? r00a_lat
			: (asel==6'h1 )? r01a_lat
			: (asel==6'h2 )? r02a_lat
			: (asel==6'h3 )? r03a_lat
			: (asel==6'h4 )? r04a_lat
			: (asel==6'h5 )? r05a_lat
			: (asel==6'h6 )? r06a_lat
			: (asel==6'h7 )? r07a_lat
			: (asel==6'h8 )? r08a_lat
            : (asel==6'h9 )? r09a_lat
            : (asel==6'ha )? r10a_lat
            : (asel==6'hb )? r11a_lat
            : (asel==6'hc )? r12a_lat
            : (asel==6'hd )? r13a_lat
            : (asel==6'he )? r14a_lat
            : (asel==6'hf )? r15a_lat
            : (asel==6'h10)? r16a_lat
            : (asel==6'h11)? r17a_lat
            : (asel==6'h12)? r18a_lat
            : (asel==6'h13)? r19a_lat
            : (asel==6'h14)? r20a_lat
            : (asel==6'h15)? r21a_lat
            : (asel==6'h16)? r22a_lat
            : (asel==6'h17)? r23a_lat
            : (asel==6'h18)? r24a_lat
            : (asel==6'h19)? r25a_lat
            : (asel==6'h1a)? r26a_lat
            : (asel==6'h1b)? r27a_lat
            : (asel==6'h1c)? r28a_lat
            : (asel==6'h1d)? r29a_lat
            : (asel==6'h1e)? r30a_lat
            : (asel==6'h1f)? r31a_lat
            : (asel==6'h20)? r00b_lat
            : (asel==6'h21)? r01b_lat
            : (asel==6'h22)? r02b_lat
            : (asel==6'h23)? r03b_lat
            : (asel==6'h24)? r04b_lat
            : (asel==6'h25)? r05b_lat
            : (asel==6'h26)? r06b_lat
            : (asel==6'h27)? r07b_lat
            : (asel==6'h28)? r08b_lat
            : (asel==6'h29)? r09b_lat
            : (asel==6'h2a)? r10b_lat
            : (asel==6'h2b)? r11b_lat
            : (asel==6'h2c)? r12b_lat
            : (asel==6'h2d)? r13b_lat
            : (asel==6'h2e)? r14b_lat
            : (asel==6'h2f)? r15b_lat
            : (asel==6'h30)? r16b_lat
            : (asel==6'h31)? r17b_lat
            : (asel==6'h32)? r18b_lat
            : (asel==6'h33)? r19b_lat
            : (asel==6'h34)? r20b_lat
            : (asel==6'h35)? r21b_lat
            : (asel==6'h36)? r22b_lat
            : (asel==6'h37)? r23b_lat
            : (asel==6'h38)? r24b_lat
            : (asel==6'h39)? r25b_lat
            : (asel==6'h3a)? r26b_lat
            : (asel==6'h3b)? r27b_lat
            : (asel==6'h3c)? r28b_lat
            : (asel==6'h3d)? r29b_lat
            : (asel==6'h3e)? r30b_lat
            : (asel==6'h3f)? r31b_lat
            : -1;

end

endmodule
