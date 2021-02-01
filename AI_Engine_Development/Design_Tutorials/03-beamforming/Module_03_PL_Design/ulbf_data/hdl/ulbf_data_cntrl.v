//  © Copyright 2020 Xilinx, Inc.
//  Licensed under the Apache License, Version 2.0 (the 'License');
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//       http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an 'AS IS' BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


`timescale 1ns / 1ps


module ulbf_data_cntrl (
        input wire  [19:0]  BRAM_PORTA_addr,
        input wire          BRAM_PORTA_clk,
        input wire  [31:0]  BRAM_PORTA_din,
        output wire [31:0]  BRAM_PORTA_dout,
        input wire          BRAM_PORTA_en,
        input wire          BRAM_PORTA_rst,
        input wire          BRAM_PORTA_we,

        input wire [31:0] csr_rddata,

        input wire [63:0]   douta,
        output reg [63:0]  dina,
        output wire         ena,
        output wire [7:0]   wea,
        output wire [15:0]  addra
    );

    reg [7:0] wea_pre;
    reg [31:0] rddata  = 32'd0;

    wire        is_csr, is_write, is_read;
    
    assign addra = (is_csr) ? 16'b0 : ((BRAM_PORTA_addr) >> 3) & 16'hffff;
    assign wea   = (is_csr) ? 8'h0  : ((is_write) ? wea_pre : 4'h0);
    assign ena   = (is_csr) ? 1'b0  : (is_write || is_read );
    
    always@(*)
    begin
      case (BRAM_PORTA_addr[2])
        'b0:  begin
                dina = {32'b0, BRAM_PORTA_din };
                wea_pre  = 8'h0f;
                rddata = douta[31:0];
            end
        'b1:  begin
                dina = {BRAM_PORTA_din, 32'b0 };
                wea_pre  = 8'hf0;
                rddata = douta[63:32];
            end
        default:  begin
                dina = { 32'b0, 32'b0 };
                wea_pre  = 8'h00;
                rddata = 32'b0;
            end
        endcase
    end

    assign is_csr      = BRAM_PORTA_addr[19];
    assign is_write    = BRAM_PORTA_en && BRAM_PORTA_we;
    assign is_read     = BRAM_PORTA_en && (~BRAM_PORTA_we); 
   
    assign BRAM_PORTA_dout  = (is_csr) ? csr_rddata : rddata;

endmodule
