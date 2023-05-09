/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

`timescale 1ns/1ps

module krnl_aurora_control_s_axi (
    input           ACLK,
    input           ARESETn,
    // AXI signals
    input   [11:0]  AWADDR,
    input           AWVALID,
    output          AWREADY,
    input   [31:0]  WDATA,
    input   [3:0]   WSTRB,
    input           WVALID,
    output          WREADY,
    output  [1:0]   BRESP,
    output          BVALID,
    input           BREADY,
    input   [11:0]  ARADDR,
    input           ARVALID,
    output          ARREADY,
    output  [31:0]  RDATA,
    output  [1:0]   RRESP,
    output          RVALID,
    input           RREADY,
    // control register signals
    input   [12:0]  aurora_status
);

//------------------------Address Info-------------------
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Aurora status

//------------------------Parameter----------------------
localparam
    ADDR_STATUS     = 12'h010, 
    
    // registers read state machine
    RDIDLE          = 2'd0,
    RDDATA          = 2'd1,
    RDRESET         = 2'd2;

//------------------------Signal Declaration----------------------
    // axi operation
    reg  [1:0]      rstate;
    reg  [1:0]      rnext;
    reg  [31:0]     rdata;
    wire            ar_hs;
    wire [11:0]     raddr;
    

//------------------------AXI protocol control------------------    
    //------------------------AXI read fsm-------------------
    assign ARREADY = (rstate == RDIDLE);
    assign RDATA   = rdata;
    assign RRESP   = 2'b00;  // OKAY
    assign RVALID  = (rstate == RDDATA);
    assign ar_hs   = ARVALID & ARREADY;
    assign raddr   = ARADDR;
    
    // rstate
    always @(posedge ACLK) begin
        if (!ARESETn)
            rstate <= RDRESET;
        else
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
        if (ar_hs) begin
            case (raddr)
                ADDR_STATUS: begin
                    rdata <= aurora_status;
                end
            endcase
        end
    end

    // axi write channel tie-off
    assign  AWREADY = 1'b1;
    assign  WREADY  = 1'b1;
    assign  BRESP   = 2'b00;
    assign  BVALID  = 1'b0;

endmodule
