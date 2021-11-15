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


module ulbf_slave_cntrl (
    input wire  [19:0] BRAM_PORTA_addr,
    input wire         BRAM_PORTA_clk,
    input wire  [31:0] BRAM_PORTA_din,
    output wire [31:0] BRAM_PORTA_dout,
    input wire         BRAM_PORTA_en,
    input wire         BRAM_PORTA_rst,
    input wire         BRAM_PORTA_we,
    
    output wire slave_rst,
    output wire [11:0] niter,
    input wire rxdone,
    input wire [15:0] rxram_counter,

    output reg [7:0]   web,
    output reg         enb,
    output reg [15:0]  addrb,
    output reg  [63:0]  dinb,
    input  wire [63:0]  doutb
    
    );
    
    reg [7:0] web_pre;
        
    reg [31:0] rddata = 32'd0;
    wire is_csr, is_write, is_read;
    
    reg [31:0] csr_rddata = 32'd0;
    wire [7:0] csr_addr;
    reg [31:0] ctrl0 = 32'd0;  // reset
    reg [31:0] ctrl1 = 32'd0; 
    reg [31:0] ctrl2 = 32'd10;  // niter
    wire [31:0] status0;
    wire [31:0] status1;    
    
    //assign web   = (is_csr) ? 8'b0 : (is_write) ? web_pre : 8'b0;
    //assign addrb = ((BRAM_PORTA_addr) >> 3) & 16'hffff; 
    //assign enb   = (is_csr)? 1'b0 : (is_write || is_read ); 
    
    always@(posedge BRAM_PORTA_clk)
    begin
	if (BRAM_PORTA_rst)
		addrb <= 16'd0;
	else
		if (BRAM_PORTA_en)
			addrb <= ((BRAM_PORTA_addr) >> 3) & 16'hffff ; 	
    end

    always@(posedge BRAM_PORTA_clk)
    begin
	if (BRAM_PORTA_rst)
		web <= 8'd0;
	else
		if (BRAM_PORTA_en)
			web <= (is_csr) ? 8'b0 : (is_write) ? web_pre : 8'b0 ; 	
    end

    always@(posedge BRAM_PORTA_clk)
    begin
	if (BRAM_PORTA_rst)
		enb <= 1'd0;
	else
		if (BRAM_PORTA_en)
			enb <= (is_csr)? 1'b0 : (is_write || is_read ) ; 		
    end

    always@(*)
    begin
      case (BRAM_PORTA_addr[2])
        'b0:  begin
                dinb = {32'b0, BRAM_PORTA_din };
                web_pre  = 8'h0f;
                rddata = doutb[31:0];
            end
        'b1:  begin
                dinb = {BRAM_PORTA_din, 32'b0 };
                web_pre  = 8'hf0;
                rddata = doutb[63:32];
            end
        default:  begin
                dinb = { 32'b0, 32'b0 };
                web_pre  = 8'h00;
                rddata = 32'b0;
            end
        endcase
    end

    //------------------------------------------------------------------
    // control register mapping:
    // 0x0000_0000 : rd - ID 32'hBEEE_0001
    // 0x0000_0004 : wr - bit 0 - reset
    // 0x0000_0008 : -  - unused
    // 0x0000_000c : wr - [15:0] niter
    // 0x0000_0010 : rd - [19:16] - current_state
    //               rd - [15:0]  - rxram_counter
    // 0x0000_0020 : rd - bit 0 - rxdone
    //------------------------------------------------------------------
    
    assign is_csr      = BRAM_PORTA_addr[19];
    assign is_write    = BRAM_PORTA_en && BRAM_PORTA_we;
    assign is_read     = BRAM_PORTA_en && (~BRAM_PORTA_we);
        
    assign csr_addr = BRAM_PORTA_addr[7:0];
    
    always@(posedge BRAM_PORTA_clk)
    begin
      if (BRAM_PORTA_rst) begin
          ctrl0 <= 32'd0;
          ctrl1 <= 32'd0;
          ctrl2 <= 32'd10; // niter
      end
      else
      begin
          if (is_write && is_csr)
            begin
              case (csr_addr)
                'h4: begin
                    ctrl0 <= BRAM_PORTA_din;
                end
                'h8: begin
                    ctrl1 <= BRAM_PORTA_din;
                end
                'hC: begin
                    ctrl2 <= BRAM_PORTA_din;
                end
                default: begin
                    ctrl0 <= ctrl0;
                    ctrl1 <= ctrl1;
                    ctrl2 <= ctrl2;
                end
              endcase
          end
      end
    end 
    
    
    always@(*)
    begin
      case (csr_addr)
        'h0:  csr_rddata = 32'hbeee_0001;
        'h4:  csr_rddata = ctrl0;
        'h8:  csr_rddata = ctrl1;
        'hC:  csr_rddata = ctrl2;
        'h20: csr_rddata = status0;
	'h24: csr_rddata = status1;
       default: begin
          csr_rddata = 32'd0;
        end
      endcase
    end

    assign status0         = {31'd0, rxdone};
    assign status1         = {16'd0, rxram_counter};
    assign BRAM_PORTA_dout = (is_csr) ? csr_rddata : rddata;
    assign slave_rst       = ctrl0[0];
    assign niter           = ctrl2[11:0];
endmodule

