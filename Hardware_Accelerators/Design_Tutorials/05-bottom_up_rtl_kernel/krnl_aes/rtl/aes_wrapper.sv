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


module aes_wrapper #(parameter AES_ENINE_NUM = 4) (
    input                       CLK,        // AXI bus clock
    input                       RESETn,

    // axi control slave connection
    input                       mode,       // AES cipher mode, 0 - decrypt, 1 - encrypt
    input   [1:0]               key_length, // AES key length, 00 - 128bit, 01 - 192bit, 10 - 256bit
    input   [255:0]             key,        // AES KEY. When shorted than 256bit, aligned to MSB
    output  [AES_ENINE_NUM-1:0] status,     // some bit is 1 mean the related AES engine are busy
    input                       keyexp_start,   // multi-cycle keyexp_start signal from axi slave
    output                      keyexp_done,    // one-cycle keyexp_done signal to axi slave

    // axi stream ports
    axis_if.SLAVE               axis_if_slave[AES_ENINE_NUM],
    axis_if.MASTER              axis_if_master[AES_ENINE_NUM]

);

    logic   [AES_ENINE_NUM-1:0]     int_keyexp_done;
    
    assign keyexp_done = &int_keyexp_done;      // actually all int_keyexp_done[] signals are fully synchronized in this case
    
    generate
        genvar i;
        for (i = 0; i < AES_ENINE_NUM; i = i + 1) begin : aes_inst

            // internel AXIS signals
            logic [127:0]   int_s_tdata;
            logic           int_s_tvalid;
            logic           int_s_tready;
            
            logic [127:0]   int_m_tdata;
            logic           int_m_tvalid;
            logic           int_m_tready;            

            // internal misc signals
            logic   int_status;
            logic   keyexp_start_reg;

            // internel aes module control signals
            logic start_cipher;
            logic start_exp;
            logic op_finish;
            logic exp_finish;
            
            logic exp_finish_reg;   // registered exp_finish
            logic exp_finish_rising; // indicating rising edge of exp_finish
            
            logic op_finish_reg;    // registered op_finish
            logic op_finish_rising; // indicating rising edge of op_finish        
            
            logic [127:0]   int_s_tdata_reg;
            
            
            // port signal assignment
            assign int_s_tdata = axis_if_slave[i].tdata;
            assign int_s_tvalid = axis_if_slave[i].tvalid;
            assign axis_if_slave[i].tready = int_s_tready;
            
            assign axis_if_master[i].tdata = int_m_tdata;
            assign axis_if_master[i].tvalid = int_m_tvalid;
            assign int_m_tready = axis_if_master[i].tready;
            
            assign status[i] = int_status;
            assign int_keyexp_done[i] = exp_finish_rising;  // use rising edge of exp_finish as keyexp_done
            
            always @(posedge CLK)
                int_status <= !op_finish;
    
            // exp_finish rising edge detection
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn)
                    exp_finish_reg <= 1'b1;
                else
                    exp_finish_reg <= exp_finish;
            end
            
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn)
                    exp_finish_rising <= 1'b0;
                else if (!exp_finish_reg && exp_finish)
                    exp_finish_rising <= 1'b1;
                else
                    exp_finish_rising <= 1'b0;
            end

            // op_finish rising edge detection, as keyexp_done output
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn)
                    op_finish_reg <= 1'b1;
                else
                    op_finish_reg <= op_finish;
            end
            
            assign op_finish_rising = !op_finish_reg && op_finish;

            // generate axis master tvalid
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn)
                    int_m_tvalid <= 1'b0;
                else if (int_m_tvalid && int_m_tready)
                    int_m_tvalid <= 1'b0;
                else if (op_finish_rising)
                    int_m_tvalid <= 1'b1;
            end

            // generate axis slave tready
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn)
                    int_s_tready <= 1'b1;
                else if (int_s_tvalid && int_s_tready)
                    int_s_tready <= 1'b0;
                else if (int_m_tvalid && int_m_tready)
                    int_s_tready <= 1'b1;
            end
                        
            // generate 1-cycle start_cipher from AXIS tvalid and tready signal            
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn) begin
                    start_cipher <= 1'b0;
                    int_s_tdata_reg <= 128'b0;
                end
                else if (int_s_tvalid && int_s_tready) begin
                    start_cipher <= 1'b1;
                    int_s_tdata_reg <= int_s_tdata;
                end
                else begin
                    start_cipher <= 1'b0;
                end
            end                    
            
            // use keyexp_start to generate 1-cycle start_exp pulse
            always @ (posedge CLK or negedge RESETn) begin
                if (!RESETn) begin
                    keyexp_start_reg <= 1'b0;
                    start_exp    <= 1'b0;
                end else begin
                    keyexp_start_reg <= keyexp_start;
                    if (start_exp) 
                        start_exp <= 1'b0;
                    else if (!keyexp_start_reg && keyexp_start)
                        start_exp <= 1'b1;
                end
            end
                    
            // instantiation of Aes module
            Aes uAes (
                .CLK            (CLK),
                .RESETn         (RESETn),
                .DATA_INPUT     (int_s_tdata_reg),
                .KEY            (key),
                .DATA_OUTPUT    (int_m_tdata),
                .OP_MODE        (mode),
                .NK             (key_length),
                .NR             (4'b0),
                .START_CIPHER   (start_cipher),
                .START_KEYEXP   (start_exp),
                .OP_FINISH      (op_finish),
                .EXP_FINISH     (exp_finish)
            );

        end // aes_inst
    endgenerate

endmodule
