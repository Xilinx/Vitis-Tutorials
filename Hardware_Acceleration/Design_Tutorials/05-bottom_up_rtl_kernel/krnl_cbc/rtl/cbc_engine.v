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

module cbc_engine (
    input           aclk,
    input           areset_n,
    
// Operation control
    input           mode,                   // 0 - decryption; 1 - encryption
    input           cbc_mode,               // 0 - AES ECB mode; 1 - AES CBC mode
    input   [127:0] IV,                     // initial vector of CBC operation
    input   [9:0]   words_num,              // 128-bit words number to process, maximum 1023
    input           op_start,               // AES/CBC operation start, one-cycle pulse
    input   [63:0]  write_addr,             // axi master write address

// AXI stream slave port, receive data from AXI read master
    input           axis_slv_rmst_tvalid,
    input   [127:0] axis_slv_rmst_tdata,
    output          axis_slv_rmst_tready,

// AXI stream master port, send data to AXI write master
    output          axis_mst_wmst_tvalid,
    input           axis_mst_wmst_tready,
    output  [127:0] axis_mst_wmst_tdata,

// AXI stream slave port, receive data from AES engine    
    input           axis_slv_aes_tvalid,
    input   [127:0] axis_slv_aes_tdata,
    output          axis_slv_aes_tready,

// AXI stream master port, send data to AES engine
    output reg      axis_mst_aes_tvalid,
    input           axis_mst_aes_tready,
    output reg [127:0] axis_mst_aes_tdata,

// global memory write master control    
    output reg      wmst_req,               // constant wmst_req, cleared by wmst_req_clear
    output reg [63:0]  wmst_xfer_addr,
    output  [63:0]  wmst_xfer_size  

);


// words input and output via AES engine counter
    reg     [9:0]   aes_in_cnt;
    reg     [9:0]   aes_out_cnt;

// input fifo signals
    wire            in_fifo_full;
    wire            in_fifo_empty;
    wire            in_fifo_push_req;
    wire    [127:0] in_fifo_push_data;
    wire            in_fifo_pop_req;
    wire    [127:0] in_fifo_pop_data;  
    wire    [10:0]  in_fifo_data_cnt;
    reg     [127:0] in_fifo_pop_data_dly_1;
    reg     [127:0] in_fifo_pop_data_dly_2;  
    reg             in_fifo_data_enough;     // when input fifo received requird words, set this register to 1;

// output fifo signals
    wire            out_fifo_full;
    wire            out_fifo_empty;
    wire            out_fifo_push_req;
    reg     [127:0] out_fifo_push_data;
    wire            out_fifo_pop_req;
    wire    [127:0] out_fifo_pop_data;    
    wire    [10:0]  out_fifo_data_cnt;
    reg     [10:0]  out_fifo_data_cnt_reg;

// input and output word counter 
    reg     [9:0]   in_word_counter;
    reg     [9:0]   out_word_counter;

// control signal registing
    reg             mode_reg;
    reg             cbc_mode_reg;
    reg     [127:0] iv_reg;
    reg     [9:0]   words_num_reg; 

// AES in/out word counting
    always @ (posedge aclk) begin
        if (op_start) begin
            in_word_counter <= 10'b0;
            out_word_counter <= 10'b0;
        end else begin
            if (in_fifo_pop_req)
                in_word_counter <= in_word_counter + 1'b1;
            if (out_fifo_push_req)
                out_word_counter <= out_word_counter + 1'b1;
        end
    end

// instantiation of input FIFO, depth 1024, width 128
    FifoType0 #(.data_width (128), .addr_bits (10)) input_fifo (
        .CLK        (aclk),
        .nRESET     (areset_n),
        .PUSH_REQ   (in_fifo_push_req),
        .POP_REQ    (in_fifo_pop_req),
        .PUSH_DATA  (in_fifo_push_data),
        .CLEAR      (),
  
        .POP_DATA   (in_fifo_pop_data),
        .EMPTY      (in_fifo_empty),
        .FULL       (in_fifo_full),
        .ERROR      (),
        .DATA_CNT   (in_fifo_data_cnt)
    );

    assign in_fifo_push_req = axis_slv_rmst_tvalid & axis_slv_rmst_tready;
    assign in_fifo_push_data = axis_slv_rmst_tdata;
    assign in_fifo_pop_req = axis_mst_aes_tready & axis_mst_aes_tvalid;
    
    always @(posedge aclk) begin
        if (op_start)
            in_fifo_data_enough <= 1'b0;
        else if (in_fifo_data_cnt >= {1'b0, words_num_reg})
            in_fifo_data_enough <= 1'b1;
    end

// instantiation of output FIFO, depth 1024, width 128
    FifoType0 #(.data_width (128), .addr_bits (10)) output_fifo (
        .CLK        (aclk),
        .nRESET     (areset_n),
        .PUSH_REQ   (out_fifo_push_req),
        .POP_REQ    (out_fifo_pop_req),
        .PUSH_DATA  (out_fifo_push_data),
        .CLEAR      (),
  
        .POP_DATA   (out_fifo_pop_data),
        .EMPTY      (out_fifo_empty),
        .FULL       (out_fifo_full),
        .ERROR      (),
        .DATA_CNT   (out_fifo_data_cnt)
    );

    assign out_fifo_pop_req = axis_mst_wmst_tvalid & axis_mst_wmst_tready;
    assign out_fifo_push_req = axis_slv_aes_tvalid && axis_slv_aes_tready;

    // registering last to-be-process input data for CBC decryption operation
    always @ (posedge aclk) begin
        if (in_fifo_pop_req) begin
            in_fifo_pop_data_dly_1 <= in_fifo_pop_data;
            in_fifo_pop_data_dly_2 <= in_fifo_pop_data_dly_1;
        end
    end

// data post-processing for AES engine output (for CBC decryption mode)
    always @ (*) begin
        if (!cbc_mode_reg || mode) begin    // AES ECB mode encryption/decryption, or CBC mode encrpytion
            out_fifo_push_data = axis_slv_aes_tdata;
        end else begin                      // AES CBC mode decryption
            if (aes_out_cnt == 10'h0) begin     // first word
                out_fifo_push_data = axis_slv_aes_tdata ^ iv_reg;
            end else begin                      // other word
                out_fifo_push_data = axis_slv_aes_tdata ^ in_fifo_pop_data_dly_2;
            end
        end
    end

// data pre-processing for AES engine input , mode and cbc_mode are used for judgement
    always @ (*) begin
        if (!cbc_mode_reg) begin            // AES ECB mode encryption/decryption
            axis_mst_aes_tvalid =  in_fifo_data_enough && !in_fifo_empty && (in_word_counter < words_num_reg);
            axis_mst_aes_tdata  =  in_fifo_pop_data;
        end else if (!mode) begin           // AES CBC mode decryption
            if (aes_in_cnt == 10'h0) begin      // first word
                axis_mst_aes_tvalid = in_fifo_data_enough && !in_fifo_empty && (in_word_counter < words_num_reg);
                axis_mst_aes_tdata  =  in_fifo_pop_data;
            end else begin                      // other word
                axis_mst_aes_tvalid = in_fifo_data_enough && !in_fifo_empty && (in_word_counter < words_num_reg) && axis_slv_aes_tvalid && axis_slv_aes_tready;
                axis_mst_aes_tdata  =  in_fifo_pop_data;
            end
        end else begin                      // AES CBC mode encryption
            if (aes_in_cnt == 10'h0) begin      // first word
                axis_mst_aes_tvalid = in_fifo_data_enough && !in_fifo_empty && (in_word_counter < words_num_reg);
                axis_mst_aes_tdata  = in_fifo_pop_data ^ iv_reg;
            end else begin                      // other word
                axis_mst_aes_tvalid = in_fifo_data_enough && !in_fifo_empty && (in_word_counter < words_num_reg) && axis_slv_aes_tvalid && axis_slv_aes_tready;
                axis_mst_aes_tdata  = in_fifo_pop_data ^ axis_slv_aes_tdata;
            end
        end
    end

// axis signals for AES output assignment
    assign axis_slv_aes_tready = (!out_fifo_full) && axis_mst_aes_tready;

// axis signals for read master assignment
    assign axis_slv_rmst_tready = !in_fifo_full;

// axis signals for write master assignment
    assign axis_mst_wmst_tvalid = !out_fifo_empty;
    assign axis_mst_wmst_tdata = out_fifo_pop_data;

// input word counter
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            aes_in_cnt <= 'h0;
        end else begin
            if (in_fifo_pop_req) begin
                if (aes_in_cnt == (words_num_reg - 1)) begin
                    aes_in_cnt <= 'h0;
                end else begin
                    aes_in_cnt <= aes_in_cnt + 1'b1;
                end
            end
        end
    end

// output word counter
    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            aes_out_cnt <= 'h0;
        end else begin
            if (out_fifo_push_req) begin
                if (aes_out_cnt == (words_num_reg - 1)) begin
                    aes_out_cnt <= 'h0;
                end else begin
                    aes_out_cnt <= aes_out_cnt + 1'b1;
                end
            end
        end
    end

// control signals registering
    always @ (posedge aclk) begin
        if (op_start) begin
            iv_reg          <= IV;
            mode_reg        <= mode;
            cbc_mode_reg    <= cbc_mode;
            words_num_reg   <= words_num;
            wmst_xfer_addr  <= write_addr;
        end
    end

    assign wmst_xfer_size = words_num_reg * 16;

// AXI master control
    always @ (posedge aclk) begin
        out_fifo_data_cnt_reg <= out_fifo_data_cnt;
    end

    always @ (posedge aclk or negedge areset_n) begin
        if (!areset_n) begin
            wmst_req <= 1'b0;
        end else begin
            // generate one-cycle write master request if FIFO is filled with all output data
            if (wmst_req) begin
                wmst_req <= 1'b0;
            end else if ((out_fifo_data_cnt == {1'b0, words_num_reg}) && (out_fifo_data_cnt_reg != {1'b0, words_num_reg})) begin
                wmst_req <= 1'b1;
            end 
        end
    end

endmodule
