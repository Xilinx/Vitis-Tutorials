//
// Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


`timescale 1ns/1ps
module krnl_cbc_axi_ctrl_slave  (
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
    // ap_ctrl_chain signals
    output          ap_start,
    input           ap_done,
    input           ap_idle,
    input           ap_ready,
    output          ap_continue,
    // control register signals
    output          mode,
    output          cbc_mode,
    output  [31:0]  iv_w3,
    output  [31:0]  iv_w2,
    output  [31:0]  iv_w1,
    output  [31:0]  iv_w0,
    output  [63:0]  src_addr,
    output  [63:0]  dest_addr,
    output  [31:0]  words_num

);

//------------------------Register Address Map-------------------
// 0x000 : CTRL
// 0x010 : MODE
// 0x018 : IV_W3
// 0x020 : IV_W2
// 0x028 : IV_W1
// 0x030 : IV_W0
// 0x038 : WORDS_NUM
// 0x040 : SRC_ADDR_0[31:0]
// 0x044 : SRC_ADDR_1[63:32]
// 0x048 : DEST_ADDR_0[31:0]
// 0x04C : DEST_ADDR_1[63:0]
// 0x050 : CBC_MODE

//------------------------Parameter----------------------
localparam
    // register address map
    ADDR_CTRL       = 12'h000, 
    ADDR_MODE       = 12'h010, 
    ADDR_IV_W3      = 12'h018, 
    ADDR_IV_W2      = 12'h020, 
    ADDR_IV_W1      = 12'h028, 
    ADDR_IV_W0      = 12'h030, 
    ADDR_WORDS_NUM  = 12'h038, 
    ADDR_SRC_ADDR_0 = 12'h040, 
    ADDR_SRC_ADDR_1 = 12'h044, 
    ADDR_DEST_ADDR_0= 12'h048, 
    ADDR_DEST_ADDR_1= 12'h04C,
    ADDR_CBC_MODE   = 12'h050,
    
    // registers write state machine
    WRIDLE          = 2'd0,
    WRDATA          = 2'd1,
    WRRESP          = 2'd2,
    WRRESET         = 2'd3,
    
    // registers read state machine
    RDIDLE          = 2'd0,
    RDDATA          = 2'd1,
    RDRESET         = 2'd2;

//------------------------Signal Declaration----------------------
    // axi operation
    reg  [1:0]      wstate;
    reg  [1:0]      wnext;
    reg  [11:0]     waddr;
    wire [31:0]     wmask;
    wire            aw_hs;
    wire            w_hs;
    reg  [1:0]      rstate;
    reg  [1:0]      rnext;
    reg  [31:0]     rdata;
    wire            ar_hs;
    wire [11:0]     raddr;
    
    // control register bit
    reg             reg_ctrl_ap_idle;
    wire            reg_ctrl_ap_done;
    reg             reg_ctrl_ap_ready;
    reg             reg_ctrl_ap_start;
    reg             reg_ctrl_ap_continue;
    
    reg  [31:0]     reg_mode;               // bit[0] used
    reg  [31:0]     reg_iv_w3;
    reg  [31:0]     reg_iv_w2;
    reg  [31:0]     reg_iv_w1;
    reg  [31:0]     reg_iv_w0;
    reg  [63:0]     reg_src_addr;
    reg  [63:0]     reg_dest_addr;
    reg  [31:0]     reg_words_num;
    reg  [31:0]     reg_cbc_mode;


//------------------------AXI protocol control------------------

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
        if (!ARESETn)
            wstate <= WRRESET;
        else
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
        if (aw_hs)
            waddr <= AWADDR;
    end
    
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
                ADDR_CTRL: begin
                    rdata[0] <= reg_ctrl_ap_start; 
                    rdata[1] <= reg_ctrl_ap_done;
                    rdata[2] <= reg_ctrl_ap_idle;
                    rdata[3] <= reg_ctrl_ap_ready;  
                    rdata[4] <= reg_ctrl_ap_continue;
                    rdata[31:5] <= 'h0;
                end
                // --------------------------------------
                ADDR_CBC_MODE: begin
                    rdata <= reg_cbc_mode;
                end
                // --------------------------------------
                ADDR_MODE: begin
                    rdata <= reg_mode;
                end
                // --------------------------------------
                ADDR_IV_W3: begin
                    rdata <= reg_iv_w3;
                end
                // --------------------------------------
                ADDR_IV_W2: begin
                    rdata <= reg_iv_w2;
                end
                // --------------------------------------
                ADDR_IV_W1: begin
                    rdata <= reg_iv_w1;
                end
                // --------------------------------------
                ADDR_IV_W0: begin
                    rdata <= reg_iv_w0;
                end
                // --------------------------------------
                ADDR_SRC_ADDR_0: begin
                    rdata <= reg_src_addr[31:0];
                end
                // --------------------------------------
                ADDR_SRC_ADDR_1: begin
                    rdata <= reg_src_addr[63:32];
                end
                // --------------------------------------
                ADDR_DEST_ADDR_0: begin
                    rdata <= reg_dest_addr[31:0];
                end
                // --------------------------------------
                ADDR_DEST_ADDR_1: begin
                    rdata <= reg_dest_addr[63:32];
                end
                // --------------------------------------
                ADDR_WORDS_NUM: begin
                    rdata <= reg_words_num;
                end
            endcase
        end
    end
    
    
// Control register update operation

    // reg_ctrl_ap_start
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_ctrl_ap_start <= 1'b0;
        else
            if (w_hs && waddr == ADDR_CTRL && WSTRB[0] && WDATA[0])
                reg_ctrl_ap_start <= 1'b1;
            else if (ap_ready)
                reg_ctrl_ap_start <= 1'b0; // clear when ap_ready asserted
    end

    // reg_ctrl_ap_done
    assign reg_ctrl_ap_done = ap_done; // ap_done clear is controlled by CU

    
    // reg_ctrl_ap_idle
    always @(posedge ACLK) begin
        reg_ctrl_ap_idle <= ap_idle;
    end

    // reg_ctrl_ap_ready
    always @(posedge ACLK) begin
        reg_ctrl_ap_ready <= ap_ready;
    end

    // reg_ctrl_ap_continue
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_ctrl_ap_continue <= 1'b0;
        else
            if (w_hs && waddr == ADDR_CTRL && WSTRB[0] && WDATA[4])
                reg_ctrl_ap_continue <= 1'b1;
            else
                reg_ctrl_ap_continue <= 1'b0; // self clear
    end
    
    // reg_mode
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_mode <= 'h0;
        else
            if (w_hs && waddr == ADDR_MODE)
                reg_mode <= (WDATA & wmask) | (reg_mode & ~wmask);
    end

    // reg_cbc_mode
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_cbc_mode <= 'h0;
        else
            if (w_hs && waddr == ADDR_CBC_MODE)
                reg_cbc_mode <= (WDATA & wmask) | (reg_cbc_mode & ~wmask);
    end

    // reg_iv_w3
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_iv_w3 <= 'h0;
        else
            if (w_hs && waddr == ADDR_IV_W3)
                reg_iv_w3 <= (WDATA & wmask) | (reg_iv_w3 & ~wmask);
    end

    // reg_iv_w2
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_iv_w2 <= 'h0;
        else
            if (w_hs && waddr == ADDR_IV_W2)
                reg_iv_w2 <= (WDATA & wmask) | (reg_iv_w2 & ~wmask);
    end

    // reg_iv_w1
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_iv_w1 <= 'h0;
        else
            if (w_hs && waddr == ADDR_IV_W1)
                reg_iv_w1 <= (WDATA & wmask) | (reg_iv_w1 & ~wmask);
    end

    // reg_iv_w0
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_iv_w0 <= 'h0;
        else
            if (w_hs && waddr == ADDR_IV_W0)
                reg_iv_w0 <= (WDATA & wmask) | (reg_iv_w0 & ~wmask);
    end

    // reg_src_addr [31:0]
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_src_addr[31:0] <= 'h0;
        else
            if (w_hs && waddr == ADDR_SRC_ADDR_0)
                reg_src_addr[31:0] <= (WDATA & wmask) | (reg_src_addr[31:0] & ~wmask);
    end

    // reg_src_addr [63:32]
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_src_addr[63:32] <= 'h0;
        else
            if (w_hs && waddr == ADDR_SRC_ADDR_1)
                reg_src_addr[63:32] <= (WDATA & wmask) | (reg_src_addr[63:32] & ~wmask);
    end

    // reg_dest_addr [31:0]
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_dest_addr[31:0] <= 'h0;
        else
            if (w_hs && waddr == ADDR_DEST_ADDR_0)
                reg_dest_addr[31:0] <= (WDATA & wmask) | (reg_dest_addr[31:0] & ~wmask);
    end

    // reg_dest_addr [63:32]
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_dest_addr[63:32] <= 'h0;
        else
            if (w_hs && waddr == ADDR_DEST_ADDR_1)
                reg_dest_addr[63:32] <= (WDATA & wmask) | (reg_dest_addr[63:32] & ~wmask);
    end

    // reg_words_num
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_words_num <= 'h0;
        else
            if (w_hs && waddr == ADDR_WORDS_NUM)
                reg_words_num <= (WDATA & wmask) | (reg_words_num & ~wmask);
    end


//------------------------Control registers output-----------------
    assign ap_start     = reg_ctrl_ap_start;
    assign ap_continue  = reg_ctrl_ap_continue;
    assign mode         = reg_mode[0]; 
    assign cbc_mode     = reg_cbc_mode[0];
    assign iv_w3        = reg_iv_w3;  
    assign iv_w2        = reg_iv_w2;  
    assign iv_w1        = reg_iv_w1;  
    assign iv_w0        = reg_iv_w0;  
    assign src_addr     = reg_src_addr;
    assign dest_addr    = reg_dest_addr;
    assign words_num    = reg_words_num;

endmodule
