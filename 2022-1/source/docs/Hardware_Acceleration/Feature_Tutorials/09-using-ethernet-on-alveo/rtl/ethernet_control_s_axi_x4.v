// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1ns/1ps
module ethernet_control_s_axi
#(parameter
    C_S_AXI_ADDR_WIDTH = 5,
    C_S_AXI_DATA_WIDTH = 32
)(
    input  wire                          ACLK,
    input  wire                          ARESET,
    input  wire                          ACLK_EN,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] AWADDR,
    input  wire                          AWVALID,
    output wire                          AWREADY,
    input  wire [C_S_AXI_DATA_WIDTH-1:0] WDATA,
    input  wire [C_S_AXI_DATA_WIDTH/8-1:0] WSTRB,
    input  wire                          WVALID,
    output wire                          WREADY,
    output wire [1:0]                    BRESP,
    output wire                          BVALID,
    input  wire                          BREADY,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] ARADDR,
    input  wire                          ARVALID,
    output wire                          ARREADY,
    output wire [C_S_AXI_DATA_WIDTH-1:0] RDATA,
    output wire [1:0]                    RRESP,
    output wire                          RVALID,
    input  wire                          RREADY,
    output wire [31:0]                   scalar00,
    output wire [31:0]                   scalar01,
    output wire [31:0]                   scalar02,
    output wire [31:0]                   scalar03,
    input  wire [3:0]                    rx_block_lock
);
//------------------------Address Info-------------------
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Data signal of scalar00
//        bit 31~0 - scalar00[31:0] (Read/Write)
// 0x14 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

//------------------------Parameter----------------------
localparam
    ADDR_CFG0            = 'h10,
    ADDR_STATUS0         = 'h14,
    ADDR_CFG1            = 'h20,
    ADDR_STATUS1         = 'h24,    
    ADDR_CFG2            = 'h30,
    ADDR_STATUS2         = 'h34,        
    ADDR_CFG3            = 'h40,
    ADDR_STATUS3         = 'h44,        
    WRIDLE               = 2'd0,
    WRDATA               = 2'd1,
    WRRESP               = 2'd2,
    WRRESET              = 2'd3,
    RDIDLE               = 2'd0,
    RDDATA               = 2'd1,
    RDRESET              = 2'd2,
    ADDR_BITS         = 12;

//------------------------Local signal-------------------
    reg  [1:0]                    wstate = WRRESET;
    reg  [1:0]                    wnext;
    reg  [ADDR_BITS-1:0]          waddr;
    wire [31:0]                   wmask;
    wire                          aw_hs;
    wire                          w_hs;
    reg  [1:0]                    rstate = RDRESET;
    reg  [1:0]                    rnext;
    reg  [31:0]                   rdata;
    wire                          ar_hs;
    wire [ADDR_BITS-1:0]          raddr;
    // internal registers
    reg  [31:0]                   int_scalar00 = 'b0;
    reg  [31:0]                   int_scalar01 = 'b0;
    reg  [31:0]                   int_scalar02 = 'b0;
    reg  [31:0]                   int_scalar03 = 'b0;

//------------------------Instantiation------------------

//------------------------AXI write fsm------------------
assign AWREADY = (wstate == WRIDLE);
assign WREADY  = (wstate == WRDATA);
assign BRESP   = 2'b00;  // OKAY
assign BVALID  = (wstate == WRRESP);
assign wmask   = { {8{WSTRB[3]}}, {8{WSTRB[2]}}, {8{WSTRB[1]}}, {8{WSTRB[0]}} };
assign aw_hs   = AWVALID & AWREADY;
assign w_hs    = WVALID & WREADY;

// wstate
always @(posedge ACLK) begin
    if (ARESET)
        wstate <= WRRESET;
    else if (ACLK_EN)
        wstate <= wnext;
end

// wnext
always @(*) begin
    case (wstate)
        WRIDLE:
            if (AWVALID)
                wnext = WRDATA;
            else
                wnext = WRIDLE;
        WRDATA:
            if (WVALID)
                wnext = WRRESP;
            else
                wnext = WRDATA;
        WRRESP:
            if (BREADY)
                wnext = WRIDLE;
            else
                wnext = WRRESP;
        default:
            wnext = WRIDLE;
    endcase
end

// waddr
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (aw_hs)
            waddr <= AWADDR[ADDR_BITS-1:0];
    end
end

//------------------------AXI read fsm-------------------
assign ARREADY = (rstate == RDIDLE);
assign RDATA   = rdata;
assign RRESP   = 2'b00;  // OKAY
assign RVALID  = (rstate == RDDATA);
assign ar_hs   = ARVALID & ARREADY;
assign raddr   = ARADDR[ADDR_BITS-1:0];

// rstate
always @(posedge ACLK) begin
    if (ARESET)
        rstate <= RDRESET;
    else if (ACLK_EN)
        rstate <= rnext;
end

// rnext
always @(*) begin
    case (rstate)
        RDIDLE:
            if (ARVALID)
                rnext = RDDATA;
            else
                rnext = RDIDLE;
        RDDATA:
            if (RREADY & RVALID)
                rnext = RDIDLE;
            else
                rnext = RDDATA;
        default:
            rnext = RDIDLE;
    endcase
end

// rdata
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (ar_hs) begin
            rdata <= 1'b0;
            case (raddr)
            	ADDR_CFG0: begin
            		rdata <= int_scalar00[31:0];
            	end
                ADDR_STATUS0: begin
                    rdata <= {31'd0, rx_block_lock[0]};
                end
            	ADDR_CFG1: begin
            		rdata <= int_scalar01[31:0];
            	end
                ADDR_STATUS1: begin
                    rdata <= {31'd0, rx_block_lock[1]};
                end            
            	ADDR_CFG2: begin
            		rdata <= int_scalar02[31:0];
            	end
                ADDR_STATUS2: begin
                    rdata <= {31'd0, rx_block_lock[2]};
                end                    
            	ADDR_CFG3: begin
            		rdata <= int_scalar03[31:0];
            	end
                ADDR_STATUS3: begin
                    rdata <= {31'd0, rx_block_lock[3]};
                end                
            endcase
        end
    end
end


//------------------------Register logic-----------------
assign scalar00 = int_scalar00;
assign scalar01 = int_scalar01;
assign scalar02 = int_scalar02;
assign scalar03 = int_scalar03;
// int_scalar00[31:0]
always @(posedge ACLK) begin
    if (ARESET) begin
        int_scalar00[31:0] <= 0;
        int_scalar01[31:0] <= 0;
        int_scalar02[31:0] <= 0;
        int_scalar03[31:0] <= 0;
    end        
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_CFG0)
            int_scalar00[31:0] <= (WDATA[31:0] & wmask) | (int_scalar00[31:0] & ~wmask);
        else if (w_hs && waddr == ADDR_CFG1)
            int_scalar01[31:0] <= (WDATA[31:0] & wmask) | (int_scalar01[31:0] & ~wmask);
        else if (w_hs && waddr == ADDR_CFG2)
            int_scalar02[31:0] <= (WDATA[31:0] & wmask) | (int_scalar02[31:0] & ~wmask);
        else if (w_hs && waddr == ADDR_CFG3)
            int_scalar03[31:0] <= (WDATA[31:0] & wmask) | (int_scalar03[31:0] & ~wmask);
    end
end


//------------------------Memory logic-------------------

endmodule
