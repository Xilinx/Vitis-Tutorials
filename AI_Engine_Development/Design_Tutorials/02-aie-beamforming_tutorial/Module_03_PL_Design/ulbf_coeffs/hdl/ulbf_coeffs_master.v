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


module ulbf_coeffs_master #(
        parameter TDATA_WIDTH=64,
        parameter TKEEP_WIDTH=TDATA_WIDTH/8,
	parameter MEM_INIT_FILE="ulbf_cin00_hex.mem",
	parameter RAM_DEPTH = 2048
        ) (

        input wire [19:0] bram_addr_a,
        input wire        bram_clk_a,
        input wire        bram_en_a,
        output wire [31:0] bram_rddata_a,
        input wire        bram_rst_a,
        input wire [3:0]  bram_we_a,
        input wire [31:0] bram_wrdata_a,	
        
        // axi4s master
        input wire m_axis_clk,  
        output wire                    m00_axis_tvalid,
        output wire                    m00_axis_tlast,  
        input  wire                    m00_axis_tready, 
        output wire [TDATA_WIDTH-1:0]  m00_axis_tdata, 
        output wire [TKEEP_WIDTH-1:0]  m00_axis_tkeep,
        
        input wire [31:0] csr_rddata,
        input wire go_maxis,
        input wire m_axis_rst_maxis,
        input wire [11:0] niter_maxis,
        input wire [11:0] block_size_maxis,
        input wire [15:0] rollover_addr_maxis,

        output wire m0_done_maxis,
	    output wire [15:0] addrb_wire_maxis

    );
            
            
    
    wire        fifo_data_valid;
    wire [63:0] fifo_din;
    wire [63:0] fifo_dout;
    wire        fifo_prog_full;
    wire        fifo_rd_en;
    wire        fifo_wr_en;
    

    wire [63:0] douta;
    wire [63:0] dina;
    wire [15:0] addra;
    wire        ena;
    wire [TDATA_WIDTH/8 -1:0]  wea;


     ulbf_coeffs_xpm_sync_fifo #(
		 .FIFO_DEPTH(32),
		 .FIFO_WIDTH(64),
		 .MEMORY_TYPE("distributed")
		)
		xpm_sync_fifo_i_0 (
		 .rst(m_axis_rst_maxis),
		 .wr_clk(m_axis_clk),
		 .wr_en(fifo_wr_en),
		 .din(fifo_din),
		 .rd_en(fifo_rd_en),
		 .data_valid(fifo_data_valid),
		 .dout(fifo_dout),
		 .prog_full(fifo_prog_full)
		);
		

    ulbf_coeffs_cntrl   plio_master_64b_cntrl_i (
            .BRAM_PORTA_addr(bram_addr_a),
            .BRAM_PORTA_clk(bram_clk_a),
            .BRAM_PORTA_din(bram_wrdata_a),
            .BRAM_PORTA_dout(bram_rddata_a),
            .BRAM_PORTA_en(bram_en_a),
            .BRAM_PORTA_rst(bram_rst_a),
            .BRAM_PORTA_we(bram_we_a),

            .csr_rddata(csr_rddata),

            .douta(douta),
            .dina(dina),
            .ena(ena),
            .wea(wea),
            .addra(addra)
    );    	

    ulbf_coeffs_ram2axis_64b  #(
        .RAM_DEPTH(RAM_DEPTH),
        .DATA_WIDTH(64),
        .RAM_READ_LATENCY(4),
        .MEM_INIT_FILE(MEM_INIT_FILE)
    ) ram2axis_64b_i (
            .go(go_maxis),
            .m_axis_rst(m_axis_rst_maxis),
            .done(m0_done_maxis),
            .addrb_wire(addrb_wire_maxis),

            .block_size(block_size_maxis),
            .niter(niter_maxis),
            .rollover_addr(rollover_addr_maxis),

            .m_axis_clk(m_axis_clk),  
            .m_axis_tready(m00_axis_tready),
            .m_axis_tvalid(m00_axis_tvalid),
            .m_axis_tlast(m00_axis_tlast),
            .m_axis_tdata(m00_axis_tdata),
            .m_axis_tkeep(m00_axis_tkeep),

            .fifo_data_valid(fifo_data_valid),
            .fifo_din(fifo_din),
            .fifo_dout(fifo_dout),
            .fifo_prog_full(fifo_prog_full),
            .fifo_rd_en(fifo_rd_en),
            .fifo_wr_en(fifo_wr_en),

            .douta(douta),
            .dina(dina),
            .bram_clk(bram_clk_a),
            .bram_rst(bram_rst_a),
            .ena(ena),
            .wea(wea),
            .addra(addra) 
    );
    
 
        
    
endmodule

