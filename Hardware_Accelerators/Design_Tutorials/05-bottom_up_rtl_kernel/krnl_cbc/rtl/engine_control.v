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

module engine_control (
    input               aclk,
    input               areset_n,

// AXI read master axis signals mux
    input               axis_slv_rmst_tvalid_in,
    input       [127:0] axis_slv_rmst_tdata_in,
    output reg          axis_slv_rmst_tready_out,

    input               axis_slv_rmst_tready_in_0,
    input               axis_slv_rmst_tready_in_1,
    input               axis_slv_rmst_tready_in_2,
    input               axis_slv_rmst_tready_in_3,

    output              axis_slv_rmst_tvalid_out_0,
    output              axis_slv_rmst_tvalid_out_1,
    output              axis_slv_rmst_tvalid_out_2,
    output              axis_slv_rmst_tvalid_out_3,

    output      [127:0] axis_slv_rmst_tdata_out_0,
    output      [127:0] axis_slv_rmst_tdata_out_1,
    output      [127:0] axis_slv_rmst_tdata_out_2,
    output      [127:0] axis_slv_rmst_tdata_out_3,

// AXI write master axis signals mux
    output  reg         axis_mst_wmst_tvalid_out,
    output  reg [127:0] axis_mst_wmst_tdata_out,
    input               axis_mst_wmst_tready_in,

    input               axis_mst_wmst_tvalid_in_0,
    input       [127:0] axis_mst_wmst_tdata_in_0,
    output              axis_mst_wmst_tready_out_0,

    input               axis_mst_wmst_tvalid_in_1,
    input       [127:0] axis_mst_wmst_tdata_in_1,
    output              axis_mst_wmst_tready_out_1,

    input               axis_mst_wmst_tvalid_in_2,
    input       [127:0] axis_mst_wmst_tdata_in_2,
    output              axis_mst_wmst_tready_out_2,

    input               axis_mst_wmst_tvalid_in_3,
    input       [127:0] axis_mst_wmst_tdata_in_3,
    output              axis_mst_wmst_tready_out_3,

// AXI read master control signals
    output  reg         rmst_req_out,
    input               rmst_done,

// AXI write master control signals
    output  reg         wmst_req_out,
    output  reg [63:0]  wmst_xfer_addr_out,
    output  reg [63:0]  wmst_xfer_size_out,
    input               wmst_done,

    input               wmst_req_in_0,
    input       [63:0]  wmst_xfer_addr_in_0,
    input       [63:0]  wmst_xfer_size_in_0,

    input               wmst_req_in_1,
    input       [63:0]  wmst_xfer_addr_in_1,
    input       [63:0]  wmst_xfer_size_in_1,

    input               wmst_req_in_2,
    input       [63:0]  wmst_xfer_addr_in_2,
    input       [63:0]  wmst_xfer_size_in_2,

    input               wmst_req_in_3,
    input       [63:0]  wmst_xfer_addr_in_3,
    input       [63:0]  wmst_xfer_size_in_3,

// kernel control signals
    input               ap_start,
    input               ap_continue,
    output              ap_ready,
    output reg          ap_done,
    output              ap_idle,

// engine control signals
    output reg          op_start_0,
    output reg          op_start_1,
    output reg          op_start_2,
    output reg          op_start_3
);

    // ap_ctrl_chain input sync: indicate the target engine number if new ap_start received.
    //    used for axi read master mux and ap_ready signal generation                     
    reg [3:0]   engine_input_flag;      
    
    // ap_ctrl_chain output sync
    //    used for axi write master mux and ap_done signal generation
    reg [3:0]   engine_output_flag;     
    
    reg         rmst_busy;      // when set to 1, read master is in busy status
    reg         wmst_busy;      // when set to 1, write master is in busy status

    reg [2:0]   engine_busy_cnt; // counting the number of busy engines

    reg [3:0]   wmst_req_latch;  // latch the write master request from engines

// --------------------------------------------------------------------------------------
//   AP_CTRL_CHAIN input sync
// --------------------------------------------------------------------------------------

    // when a read master request is finished, round robin the engine input flag
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            engine_input_flag <= 4'b0001;
        end else begin
            if (rmst_done)
                engine_input_flag <= {engine_input_flag[2:0], engine_input_flag[3]};
        end
    end

    // rmst axis signal mux
    always @ (*) begin
        axis_slv_rmst_tready_out = 1'b0;
        case (engine_input_flag)
            4'b0001 : axis_slv_rmst_tready_out = axis_slv_rmst_tready_in_0; 
            4'b0010 : axis_slv_rmst_tready_out = axis_slv_rmst_tready_in_1;
            4'b0100 : axis_slv_rmst_tready_out = axis_slv_rmst_tready_in_2;
            4'b1000 : axis_slv_rmst_tready_out = axis_slv_rmst_tready_in_3;
        endcase
    end

    assign axis_slv_rmst_tvalid_out_0 = (engine_input_flag == 4'b0001) ? axis_slv_rmst_tvalid_in : 1'b0;
    assign axis_slv_rmst_tvalid_out_1 = (engine_input_flag == 4'b0010) ? axis_slv_rmst_tvalid_in : 1'b0;
    assign axis_slv_rmst_tvalid_out_2 = (engine_input_flag == 4'b0100) ? axis_slv_rmst_tvalid_in : 1'b0;
    assign axis_slv_rmst_tvalid_out_3 = (engine_input_flag == 4'b1000) ? axis_slv_rmst_tvalid_in : 1'b0;

    assign axis_slv_rmst_tdata_out_0 = (engine_input_flag == 4'b0001) ? axis_slv_rmst_tdata_in : 128'b0;
    assign axis_slv_rmst_tdata_out_1 = (engine_input_flag == 4'b0010) ? axis_slv_rmst_tdata_in : 128'b0;
    assign axis_slv_rmst_tdata_out_2 = (engine_input_flag == 4'b0100) ? axis_slv_rmst_tdata_in : 128'b0;
    assign axis_slv_rmst_tdata_out_3 = (engine_input_flag == 4'b1000) ? axis_slv_rmst_tdata_in : 128'b0;

    // engine busy counting
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n)
            engine_busy_cnt <= 3'b0;
        else if (ap_start && ap_ready && !wmst_done)
            engine_busy_cnt <= engine_busy_cnt + 1'b1;
        else if (!(ap_start && ap_ready) && wmst_done)
            engine_busy_cnt <= engine_busy_cnt - 1'b1;
    end

    // read master status update
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            rmst_busy <= 1'b0;
        end else begin
            if (ap_start && ap_ready) 
                rmst_busy <= 1'b1;
            else if (rmst_done)
                rmst_busy <= 1'b0;
        end
    end

    // read master request generation
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            rmst_req_out <= 1'b0;
        end else begin
            if (ap_start && ap_ready) 
                rmst_req_out <= 1'b1;
            else if (rmst_req_out)
                rmst_req_out <= 1'b0;
        end
    end


    // if there are at least one engines in idle, and read master is idle, set ap_ready to high
    assign ap_ready = (engine_busy_cnt < 3'd4) && !rmst_busy;    
    assign ap_idle = (engine_busy_cnt == 3'd0);

    // generate op_start pulse to cbc_engine
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n)
            op_start_0 <= 1'b0;
        else if (op_start_0)
            op_start_0 <= 1'b0;
        else if (engine_input_flag[0] && ap_start && ap_ready)
            op_start_0 <= 1'b1;
    end

    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n)
            op_start_1 <= 1'b0;
        else if (op_start_1)
            op_start_1 <= 1'b0;
        else if (engine_input_flag[1] && ap_start && ap_ready)
            op_start_1 <= 1'b1;
    end

    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n)
            op_start_2 <= 1'b0;
        else if (op_start_2)
            op_start_2 <= 1'b0;
        else if (engine_input_flag[2] && ap_start && ap_ready)
            op_start_2 <= 1'b1;
    end

    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n)
            op_start_3 <= 1'b0;
        else if (op_start_3)
            op_start_3 <= 1'b0;
        else if (engine_input_flag[3] && ap_start && ap_ready)
            op_start_3 <= 1'b1;
    end



// --------------------------------------------------------------------------------------
//   AP_CTRL_CHAIN output sync
// --------------------------------------------------------------------------------------
    // write master status update
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            wmst_busy <= 1'b0;
        end else begin
            if (wmst_req_out) 
                wmst_busy <= 1'b1;
            else if (wmst_done)
                wmst_busy <= 1'b0;
        end
    end
    
    // when write master is idle and ap_continue asserted, round robin the engine output flag
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            engine_output_flag <= 4'b0001;
        end else begin
            if (!wmst_busy && ap_continue)
                engine_output_flag <= {engine_output_flag[2:0], engine_output_flag[3]};
        end
    end
    
    // write master request generation
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            wmst_req_latch <= 4'b0;
        end else begin
            if (wmst_req_in_3)
                wmst_req_latch[3] <= 1'b1;
            else if ((engine_output_flag == 4'b1000) && wmst_req_out)
                wmst_req_latch[3] <= 1'b0;

            if (wmst_req_in_2)
                wmst_req_latch[2] <= 1'b1;
            else if ((engine_output_flag == 4'b0100) && wmst_req_out)
                wmst_req_latch[2] <= 1'b0;

            if (wmst_req_in_1)
                wmst_req_latch[1] <= 1'b1;
            else if ((engine_output_flag == 4'b0010) && wmst_req_out)
                wmst_req_latch[1] <= 1'b0;

            if (wmst_req_in_0)
                wmst_req_latch[0] <= 1'b1;
            else if ((engine_output_flag == 4'b0001) && wmst_req_out)
                wmst_req_latch[0] <= 1'b0;
        end     
    end

    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            wmst_req_out <= 1'b0;
        end else begin
            if (wmst_req_out)
                wmst_req_out <= 1'b0;
            else
                wmst_req_out <= |(engine_output_flag & wmst_req_latch);
        end
    end

    // wmst axis signal mux
    always @ (*) begin
        axis_mst_wmst_tvalid_out = 1'b0;
        axis_mst_wmst_tdata_out = 128'h0;
        case (engine_output_flag)
            4'b0001 : begin
                axis_mst_wmst_tvalid_out = axis_mst_wmst_tvalid_in_0; 
                axis_mst_wmst_tdata_out  = axis_mst_wmst_tdata_in_0;    
            end 
            4'b0010 : begin
                axis_mst_wmst_tvalid_out = axis_mst_wmst_tvalid_in_1; 
                axis_mst_wmst_tdata_out  = axis_mst_wmst_tdata_in_1;    
            end 
            4'b0100 : begin
                axis_mst_wmst_tvalid_out = axis_mst_wmst_tvalid_in_2; 
                axis_mst_wmst_tdata_out  = axis_mst_wmst_tdata_in_2;    
            end 
            4'b1000 : begin
                axis_mst_wmst_tvalid_out = axis_mst_wmst_tvalid_in_3; 
                axis_mst_wmst_tdata_out  = axis_mst_wmst_tdata_in_3;    
            end 
        endcase
    end

    assign axis_mst_wmst_tready_out_0 = (engine_output_flag == 4'b0001) ? axis_mst_wmst_tready_in : 1'b0;
    assign axis_mst_wmst_tready_out_1 = (engine_output_flag == 4'b0010) ? axis_mst_wmst_tready_in : 1'b0;
    assign axis_mst_wmst_tready_out_2 = (engine_output_flag == 4'b0100) ? axis_mst_wmst_tready_in : 1'b0;
    assign axis_mst_wmst_tready_out_3 = (engine_output_flag == 4'b1000) ? axis_mst_wmst_tready_in : 1'b0;

    // wmst control signals mux
    always @ (*) begin
        wmst_xfer_addr_out = 64'h0;
        wmst_xfer_size_out = 64'h0;
        case (engine_output_flag)
            4'b0001 : begin
                wmst_xfer_addr_out = wmst_xfer_addr_in_0;
                wmst_xfer_size_out = wmst_xfer_size_in_0;
            end
            4'b0010 : begin
                wmst_xfer_addr_out = wmst_xfer_addr_in_1;
                wmst_xfer_size_out = wmst_xfer_size_in_1;
            end
            4'b0100 : begin
                wmst_xfer_addr_out = wmst_xfer_addr_in_2;
                wmst_xfer_size_out = wmst_xfer_size_in_2;
            end
            4'b1000 : begin
                wmst_xfer_addr_out = wmst_xfer_addr_in_3;
                wmst_xfer_size_out = wmst_xfer_size_in_3;
            end
        endcase
    end


    // ap_done generation
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n)
            ap_done <= 1'b0;
        else if (ap_done && ap_continue)    // ap_done clear when ap_continue asserted
            ap_done <= 1'b0;
        else if (wmst_done)                 // when any CBC engine finish axi master write, assert ap_done
            ap_done <= 1'b1;
    end


endmodule
