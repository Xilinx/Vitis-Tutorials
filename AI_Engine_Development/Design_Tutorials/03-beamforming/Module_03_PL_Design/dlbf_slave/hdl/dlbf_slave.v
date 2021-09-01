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


module dlbf_slave #(
        parameter TDATA_WIDTH=64,
        parameter RAM_DEPTH=1536,
        parameter TKEEP_WIDTH=TDATA_WIDTH/8
        ) (
        input wire [19:0]S_AXI_araddr,
        input wire [2:0]S_AXI_arprot,
        output wire S_AXI_arready,
        input wire S_AXI_arvalid,
        input wire [19:0]S_AXI_awaddr,
        input wire [2:0]S_AXI_awprot,
        output wire S_AXI_awready,
        input wire S_AXI_awvalid,
        input wire S_AXI_bready,
        output wire [1:0]S_AXI_bresp,
        output wire S_AXI_bvalid,
        output wire [31:0]S_AXI_rdata,
        input wire S_AXI_rready,
        output wire [1:0]S_AXI_rresp,
        output wire S_AXI_rvalid,
        input wire [31:0]S_AXI_wdata,
        output wire S_AXI_wready,
        input wire [3:0]S_AXI_wstrb,
        input wire S_AXI_wvalid,
        input wire s_axi_aclk,
        input wire s_axi_aresetn,
        
        // axi4s slave
    input s_axis_clk,
    (* X_INTERFACE_PARAMETER = "CLK_DOMAIN clk_domain_saxis,LAYERED_METADATA  undef, PHASE 0,FREQ_HZ 400000000,HAS_TLAST 1,HAS_TKEEP 1,HAS_TSTRB 0,HAS_TREADY 1,TUSER_WIDTH 0,TID_WIDTH 0,TDEST_WIDTH 0" *)
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TVALID" *)   input wire s_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TLAST" *)    input wire s_axis_tlast,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TDATA" *)    input wire [TDATA_WIDTH-1:0] s_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TREADY" *)   output wire s_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 S_AXIS TKEEP" *)    input wire [TKEEP_WIDTH-1:0] s_axis_tkeep
    
    
    );
            
            
    wire [19:0] bram_addr_a;
    wire        bram_clk_a;
    wire        bram_en_a;
    wire [31:0] bram_rddata_a;
    wire        bram_rst_a;
    wire [3:0]  bram_we_a;
    wire [31:0] bram_wrdata_a;
    
    
    design_dlbf_slave_wrapper design_dlbf_slave_wrapper_i (
        .S_AXI_araddr(S_AXI_araddr),
        .S_AXI_arprot(S_AXI_arprot),
        .S_AXI_arready(S_AXI_arready),
        .S_AXI_arvalid(S_AXI_arvalid),
        .S_AXI_awaddr(S_AXI_awaddr),
        .S_AXI_awprot(S_AXI_awprot),
        .S_AXI_awready(S_AXI_awready),
        .S_AXI_awvalid(S_AXI_awvalid),
        .S_AXI_bready(S_AXI_bready),
        .S_AXI_bresp(S_AXI_bresp),
        .S_AXI_bvalid(S_AXI_bvalid),
        .S_AXI_rdata(S_AXI_rdata),
        .S_AXI_rready(S_AXI_rready),
        .S_AXI_rresp(S_AXI_rresp),
        .S_AXI_rvalid(S_AXI_rvalid),
        .S_AXI_wdata(S_AXI_wdata),
        .S_AXI_wready(S_AXI_wready),
        .S_AXI_wstrb(S_AXI_wstrb),
        .S_AXI_wvalid(S_AXI_wvalid),
        .s_axi_aclk(s_axi_aclk),
        .s_axi_aresetn(s_axi_aresetn),
        
        .bram_addr_a(bram_addr_a),
        .bram_clk_a(bram_clk_a),
        .bram_en_a(bram_en_a),
        .bram_rddata_a(bram_rddata_a),
        .bram_rst_a(bram_rst_a),
        .bram_we_a(bram_we_a),
        .bram_wrdata_a(bram_wrdata_a)
        
        );
    
    wire [7:0]  web_bram;
    wire        web_pulse, web_saxis;
    wire        enb_bram, enb_saxis, enb_pulse;
    wire [63:0] dinb, doutb;
    wire        slave_rst_bram, slave_rst_saxis;
    wire        rxdone_bram, rxdone_saxis;
    wire [11:0] niter_bram, niter_saxis;
    wire [3:0]  current_state_saxis, current_state_bram;
    wire [15:0] rxram_counter_saxis, rxram_counter_bram;
    wire [63:0] doutb_saxis, doutb_bram;
    wire [15:0] addrb_bram, addrb_saxis;


 

    dlbf_slave_cdc dlbf_slave_cdc_i (
	.bram_clk_a(bram_clk_a),
	.s_axis_clk(s_axis_clk),

	.slave_rst_bram(slave_rst_bram),
	.rxdone_saxis(rxdone_saxis),
	.niter_bram(niter_bram),
	.current_state_saxis(current_state_saxis),
	.rxram_counter_saxis(rxram_counter_saxis),
	.addrb_bram(addrb_bram),
	.doutb_saxis(doutb_saxis),
	.web_bram(web_bram[0]),
	.enb_bram(enb_bram),
	

	.slave_rst_saxis(slave_rst_saxis),
	.rxdone_bram(rxdone_bram),
	.niter_saxis(niter_saxis),
	.current_state_bram(current_state_bram),
	.rxram_counter_bram(rxram_counter_bram),
	.addrb_saxis(addrb_saxis),
	.doutb_bram(doutb_bram),
	.web_saxis(web_saxis),
	.enb_saxis(enb_saxis)
     );


    dlbf_slave_cntrl dlbf_slave_cntrl_i(
	.BRAM_PORTA_addr(bram_addr_a),
	.BRAM_PORTA_clk(bram_clk_a),
	.BRAM_PORTA_din(bram_wrdata_a),
	.BRAM_PORTA_dout(bram_rddata_a),
	.BRAM_PORTA_en(bram_en_a),
	.BRAM_PORTA_rst(bram_rst_a),
	.BRAM_PORTA_we(bram_we_a),

	.slave_rst(slave_rst_bram),
	.niter(niter_bram),
	.rxdone(rxdone_bram),
	.rxram_counter(rxram_counter_bram),

	.web(web_bram),
	.enb(enb_bram),
	.addrb(addrb_bram),
	.dinb(dinb), 
	.doutb(doutb_bram)
        );

    edge_detector_dlbf web_pulse_i (
	.clk(s_axis_clk), 
	.rst(slave_rst_saxis), 
	.signal(web_saxis), 
	.edge_pos(web_pulse)
	);



   dlbf_slave_ram  #(
    .TDATA_WIDTH(64),
    .TKEEP_WIDTH(8),
    .URAM_DEPTH(RAM_DEPTH),
    .URAM_READ_LATENCY(4)
    ) dlbf_slave_ram_i (
    .BRAM_PORTA_rst(bram_rst_a),
    .BRAM_PORTA_clk(bram_clk_a),
    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tlast(s_axis_tlast),
    .s_axis_tdata(s_axis_tdata),
    .s_axis_tready(s_axis_tready),
    .s_axis_tkeep(s_axis_tkeep),
    .s_axis_clk(s_axis_clk),

    .slave_rst(slave_rst_saxis),
    .rxdone(rxdone_saxis),
    .niter(niter_saxis),
    .current_state_wire(current_state_saxis),
    .rxram_counter_wire(rxram_counter_saxis),

         .web({8{web_pulse}}),
         .enb(enb_saxis),
         .addrb(addrb_saxis),
         //.dinb(dinb),
         .dinb(64'd0),
         .doutb(doutb_saxis)
    
    );

    

endmodule

module edge_detector_dlbf(input wire clk, input wire rst, input wire signal, output wire  edge_pos);
	reg signal_ff;
	always@(posedge clk)
		signal_ff <= (rst) ? 1'b0 : signal;
	assign edge_pos = (signal && (~signal_ff) );	
endmodule

