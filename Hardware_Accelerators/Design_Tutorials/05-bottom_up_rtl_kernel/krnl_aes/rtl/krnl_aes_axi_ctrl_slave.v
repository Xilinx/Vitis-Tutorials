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
module krnl_aes_axi_ctrl_slave #(parameter AES_ENINE_NUM = 4) (
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
    // ap_ctrl_hs signals
    output          ap_start,
    input           ap_done,
    // control register signals
    output          mode,
    output  [1:0]   key_len,
    input   [AES_ENINE_NUM-1:0]  status,
    output  [31:0]  key_w7,
    output  [31:0]  key_w6,
    output  [31:0]  key_w5,
    output  [31:0]  key_w4,
    output  [31:0]  key_w3,
    output  [31:0]  key_w2,
    output  [31:0]  key_w1,
    output  [31:0]  key_w0
);

//------------------------Register Address Map-------------------
// 0x000 : CTRL
// 0x010 : MODE
// 0x018 : KEY_LEN
// 0x020 : STATUS
// 0x028 : KEY_W7
// 0x030 : KEY_W6
// 0x038 : KEY_W5
// 0x040 : KEY_W4
// 0x048 : KEY_W3
// 0x050 : KEY_W2
// 0x058 : KEY_W1
// 0x060 : KEY_W0

//------------------------Parameter----------------------
localparam
    // register address map
    ADDR_CTRL       = 12'h000, 
    ADDR_MODE       = 12'h010, 
    ADDR_KEY_LEN    = 12'h018, 
    ADDR_STATUS     = 12'h020, 
    ADDR_KEY_W7     = 12'h028, 
    ADDR_KEY_W6     = 12'h030, 
    ADDR_KEY_W5     = 12'h038, 
    ADDR_KEY_W4     = 12'h040, 
    ADDR_KEY_W3     = 12'h048, 
    ADDR_KEY_W2     = 12'h050, 
    ADDR_KEY_W1     = 12'h058, 
    ADDR_KEY_W0     = 12'h060, 
    
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
    reg             reg_ctrl_ap_done;
    wire            reg_ctrl_ap_ready;      // copy version of ap_done
    reg             reg_ctrl_ap_start;
    reg             reg_ctrl_ap_continue;   // useless in ap_ctrl_hs mode
    wire            ap_ready;
    
    reg  [31:0]     reg_mode;               // bit[0] used
    reg  [31:0]     reg_key_len;            // bit[1:0] used
    wire [31:0]     reg_status;             // bit[AES_ENINE_NUM-1:0] used
    reg  [31:0]     reg_key_w7;
    reg  [31:0]     reg_key_w6;
    reg  [31:0]     reg_key_w5;
    reg  [31:0]     reg_key_w4;
    reg  [31:0]     reg_key_w3;
    reg  [31:0]     reg_key_w2;
    reg  [31:0]     reg_key_w1;
    reg  [31:0]     reg_key_w0;

    assign reg_ctrl_ap_ready = reg_ctrl_ap_done;
    assign ap_ready = ap_done; 
    assign reg_status = status;

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
                ADDR_MODE: begin
                    rdata <= reg_mode;
                end
                // --------------------------------------
                ADDR_KEY_LEN: begin
                    rdata <= reg_key_len;
                end              
                // --------------------------------------
                ADDR_STATUS: begin
                    rdata <= reg_status;
                end
                // --------------------------------------
                ADDR_KEY_W7: begin
                    rdata <= reg_key_w7;
                end
                // --------------------------------------
                ADDR_KEY_W6: begin
                    rdata <= reg_key_w6;
                end
                // --------------------------------------
                ADDR_KEY_W5: begin
                    rdata <= reg_key_w5;
                end
                // --------------------------------------
                ADDR_KEY_W4: begin
                    rdata <= reg_key_w4;
                end
                // --------------------------------------
                ADDR_KEY_W3: begin
                    rdata <= reg_key_w3;
                end
                // --------------------------------------
                ADDR_KEY_W2: begin
                    rdata <= reg_key_w2;
                end
                // --------------------------------------
                ADDR_KEY_W1: begin
                    rdata <= reg_key_w1;
                end
                // --------------------------------------
                ADDR_KEY_W0: begin
                    rdata <= reg_key_w0;
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
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_ctrl_ap_done <= 1'b0;
        else
            if (ap_done)
                reg_ctrl_ap_done <= 1'b1;
            else if (ar_hs && raddr == ADDR_CTRL)
                reg_ctrl_ap_done <= 1'b0; // clear on read
    end
    
    // reg_ctrl_ap_idle
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_ctrl_ap_idle <= 1'b1;
        else if (!reg_ctrl_ap_idle && ap_done)
            reg_ctrl_ap_idle <= 1'b1;
        else if (reg_ctrl_ap_start)
            reg_ctrl_ap_idle <= 1'b0;
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
    
    // reg_key_len
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_len <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_LEN)
                reg_key_len <= (WDATA & wmask) | (reg_key_len & ~wmask);
    end
    
    // reg_key_w7
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w7 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W7)
                reg_key_w7 <= (WDATA & wmask) | (reg_key_w7 & ~wmask);
    end

    // reg_key_w6
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w6 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W6)
                reg_key_w6 <= (WDATA & wmask) | (reg_key_w6 & ~wmask);
    end

    // reg_key_w5
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w5 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W5)
                reg_key_w5 <= (WDATA & wmask) | (reg_key_w5 & ~wmask);
    end

    // reg_key_w4
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w4 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W4)
                reg_key_w4 <= (WDATA & wmask) | (reg_key_w4 & ~wmask);
    end

    // reg_key_w3
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w3 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W3)
                reg_key_w3 <= (WDATA & wmask) | (reg_key_w3 & ~wmask);
    end

    // reg_key_w2
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w2 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W2)
                reg_key_w2 <= (WDATA & wmask) | (reg_key_w2 & ~wmask);
    end

    // reg_key_w1
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w1 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W1)
                reg_key_w1 <= (WDATA & wmask) | (reg_key_w1 & ~wmask);
    end

    // reg_key_w0
    always @(posedge ACLK) begin
        if (!ARESETn)
            reg_key_w0 <= 'h0;
        else
            if (w_hs && waddr == ADDR_KEY_W0)
                reg_key_w0 <= (WDATA & wmask) | (reg_key_w0 & ~wmask);
    end


    

//------------------------Control registers output-----------------
    assign ap_start     = reg_ctrl_ap_start;
    assign mode         = reg_mode[0];
    assign key_len      = reg_key_len[1:0];
    assign key_w7       = reg_key_w7;   
    assign key_w6       = reg_key_w6;  
    assign key_w5       = reg_key_w5;  
    assign key_w4       = reg_key_w4;  
    assign key_w3       = reg_key_w3;  
    assign key_w2       = reg_key_w2;  
    assign key_w1       = reg_key_w1;  
    assign key_w0       = reg_key_w0;  

endmodule
