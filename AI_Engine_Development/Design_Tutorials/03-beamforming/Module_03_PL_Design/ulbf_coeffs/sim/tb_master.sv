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

`define tCK_100 10.0
`define tCK_400 2.5
`define ULBF_0PORT_BASE_ADDR  32'h0000_0000
`define ULBF_1PORT_BASE_ADDR  32'h0010_0000
`define ULBF_2PORT_BASE_ADDR  32'h0020_0000
`define ULBF_3PORT_BASE_ADDR  32'h0030_0000
`define ULBF_4PORT_BASE_ADDR  32'h0040_0000
`define ULBF_5PORT_BASE_ADDR  32'h0050_0000
`define ULBF_6PORT_BASE_ADDR  32'h0060_0000
`define ULBF_7PORT_BASE_ADDR  32'h0070_0000
`define CSR_OFFSET 32'h0008_0000
`define AXI4PC_EOS_OFF


import axi_vip_pkg::*;
import design_1_axi_vip_0_0_pkg::*;
import axi4stream_vip_pkg::*;
import design_1_axi4stream_vip_0_0_pkg::*;
import design_1_axi4stream_vip_1_0_pkg::*;
import design_1_axi4stream_vip_2_0_pkg::*;
import design_1_axi4stream_vip_3_0_pkg::*;
import design_1_axi4stream_vip_4_0_pkg::*;
import design_1_axi4stream_vip_5_0_pkg::*;
import design_1_axi4stream_vip_6_0_pkg::*;
import design_1_axi4stream_vip_7_0_pkg::*;

module tb_master();
    // reset and control signals of dut
    reg         clk100 = 1'b0;
    reg         clk400 = 1'b0;
    reg         resetn_100MHz = 1'b0;
    reg         resetn_400MHz = 1'b0;
    
    always
        #(`tCK_100/2) clk100 <= ~clk100;
    always
        #(`tCK_400/2) clk400 <= ~clk400;
            
    initial 
    begin
        #(`tCK_100*50); 
        resetn_100MHz = 1'b1;
        #(`tCK_100*50);
        resetn_400MHz = 1'b1;
    end

    design_1_wrapper design_1_wrapper_i(
        .resetn_100MHz(resetn_100MHz),
        .resetn_400MHz(resetn_400MHz),
        .s_axi_aclk(clk100),
        .m_axis_clk(clk400)
        );
        
    design_1_axi_vip_0_0_mst_t             master_agent;
    design_1_axi4stream_vip_0_0_slv_t      slave_agent_stream0;
    design_1_axi4stream_vip_1_0_slv_t      slave_agent_stream1;
    design_1_axi4stream_vip_2_0_slv_t      slave_agent_stream2;
    design_1_axi4stream_vip_3_0_slv_t      slave_agent_stream3;
    design_1_axi4stream_vip_4_0_slv_t      slave_agent_stream4;
    design_1_axi4stream_vip_5_0_slv_t      slave_agent_stream5;
    design_1_axi4stream_vip_6_0_slv_t      slave_agent_stream6;
    design_1_axi4stream_vip_7_0_slv_t      slave_agent_stream7;
    bit [31:0] addr;
    bit [31:0] data;
    

    xil_axi_resp_t 	resp;
    xil_axi4stream_uint                            slave_agent_stream0_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream1_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream2_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream3_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream4_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream5_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream6_verbosity = 0;
    xil_axi4stream_uint                            slave_agent_stream7_verbosity = 0;
    // Monitor transaction for slave VIP
    axi4stream_monitor_transaction                 slv_monitor_transaction;
    // Monitor transaction queue for slave VIP
    axi4stream_monitor_transaction                 slave_moniter_transaction_queue[$];
    // Size of slave_moniter_transaction_queue
    xil_axi4stream_uint                            slave_moniter_transaction_queue_size =0;
    // Scoreboard transaction from slave monitor transaction queue
    axi4stream_monitor_transaction                 slv_scb_transaction;


    reg [3:0] niter=4;
    reg [15:0] block_size=256;  
    reg [31:0] pkt_header_0=32'h0606_00e0;  
    reg [31:0] pkt_header_1=32'ha562_0e00;   
    integer i=0;
    bit error=0;
    
    initial
    begin
        slave_agent_stream0 = new("slave0 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_0.inst.IF);
        slave_agent_stream0.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream0.set_agent_tag("Slave0 VIP");
        slave_agent_stream0.set_verbosity(slave_agent_stream0_verbosity);
        slave_agent_stream0.start_slave();
        slave_agent_stream0.start_monitor();
        
        slave_agent_stream1 = new("slave1 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_1.inst.IF);
        slave_agent_stream1.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream1.set_agent_tag("Slave1 VIP");
        slave_agent_stream1.set_verbosity(slave_agent_stream1_verbosity);
        slave_agent_stream1.start_slave();
        slave_agent_stream1.start_monitor();

        slave_agent_stream2 = new("slave2 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_2.inst.IF);
        slave_agent_stream2.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream2.set_agent_tag("Slave2 VIP");
        slave_agent_stream2.set_verbosity(slave_agent_stream2_verbosity);
        slave_agent_stream2.start_slave();
        slave_agent_stream2.start_monitor();

        slave_agent_stream3 = new("slave3 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_3.inst.IF);
        slave_agent_stream3.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream3.set_agent_tag("Slave3 VIP");
        slave_agent_stream3.set_verbosity(slave_agent_stream3_verbosity);
        slave_agent_stream3.start_slave();
        slave_agent_stream3.start_monitor();

        slave_agent_stream4 = new("slave4 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_4.inst.IF);
        slave_agent_stream4.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream4.set_agent_tag("Slave4 VIP");
        slave_agent_stream4.set_verbosity(slave_agent_stream4_verbosity);
        slave_agent_stream4.start_slave();
        slave_agent_stream4.start_monitor();
        
        slave_agent_stream5 = new("slave5 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_5.inst.IF);
        slave_agent_stream5.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream5.set_agent_tag("Slave5 VIP");
        slave_agent_stream5.set_verbosity(slave_agent_stream5_verbosity);
        slave_agent_stream5.start_slave();
        slave_agent_stream5.start_monitor();

        slave_agent_stream6 = new("slave6 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_6.inst.IF);
        slave_agent_stream6.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream6.set_agent_tag("Slave6 VIP");
        slave_agent_stream6.set_verbosity(slave_agent_stream6_verbosity);
        slave_agent_stream6.start_slave();
        slave_agent_stream6.start_monitor();

        slave_agent_stream7 = new("slave7 vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_7.inst.IF);
        slave_agent_stream7.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        slave_agent_stream7.set_agent_tag("Slave7 VIP");
        slave_agent_stream7.set_verbosity(slave_agent_stream7_verbosity);
        slave_agent_stream7.start_slave();
        slave_agent_stream7.start_monitor();

        #(`tCK_100*200);
        master_agent = new("master vip agent",design_1_wrapper_i.design_1_i.axi_vip_0.inst.IF);
        master_agent.start_master();
        #(`tCK_100*10);
	
	// Reset all 4 ports 
        data = 32'h1;
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        
        data = 32'h1;
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        
        data = 32'h1;
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);

        data = 32'h1;
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);

        data = 32'h1;
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        
        data = 32'h1;
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        
        data = 32'h1;
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);

        data = 32'h1;
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h0;
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        
        // Check IDs
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'h0123_4567, resp);
        
        // set niter
        data = niter;
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);
        data = niter;
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'hC;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, niter, resp);

        
        // set blocksize
        data = block_size;
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);
        data = block_size;
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'h8;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size, resp);

    // set rollover addr
        data = 32'd1024;
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);
        data = 32'd1024;
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'h10;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, block_size*niter, resp);

	#(`tCK_100*500);
        
        //load_ram(niter, block_size);
        //check_ram(niter, block_size, error);

	// start
        data = 32'h10;
        addr = `ULBF_0PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_1PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_2PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_3PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_4PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_5PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_6PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        data = 32'h10;
        addr = `ULBF_7PORT_BASE_ADDR + `CSR_OFFSET+ 32'h4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);

        //load_ram(niter, block_size);
        //check_ram(niter, block_size, error);
        
            
        #(`tCK_400*500);
        slv_gen_tready(niter, block_size);
        #(`tCK_400*1000);
        
        $display("********************************");
        if (error)
            $display("***** SIMULATION FAILED *********");
        else
            $display("***** SIMULATION PASSED *********");
        $display("********************************");
        //slave_agent_stream0.stop_slave();
        //slave_agent_stream0.stop_monitor();
        //slave_agent_stream1.stop_slave();
        //slave_agent_stream1.stop_monitor();
        
        $finish;
    end

    // 3 port
    //always@(posedge clk400)
    //    if (design_1_wrapper_i.design_1_i.axi4stream_vip_0_S_AXIS_TVALID)
    //        $display("[%t] 3PORT m_axis_tdata: 0x%x",$time, design_1_wrapper_i.design_1_i.axi4stream_vip_0_S_AXIS_TDATA);
        
    // 0 port      
    always@(posedge clk400)
        if (design_1_wrapper_i.design_1_i.axi4stream_vip_0_S_AXIS_TVALID)
            $display("[%t] 0PORT m_axis_tdata: 0x%x",$time, design_1_wrapper_i.design_1_i.axi4stream_vip_0_S_AXIS_TDATA);
        
    // 4 port      
    always@(posedge clk400)
        if (design_1_wrapper_i.design_1_i.axi4stream_vip_4_S_AXIS_TVALID)
            $display("[%t] 4PORT m_axis_tdata: 0x%x",$time, design_1_wrapper_i.design_1_i.axi4stream_vip_4_S_AXIS_TDATA);
        
    task verify_axi_read;
        input bit [31:0] readVal;
        input bit [31:0] expectedVal;
        input bit readResp;
        /*
        XIL_AXI_RESP_OKAY    - 2'b00     Normal Access
        XIL_AXI_RESP_EXOKAY  - 2'b01     Exclusive Access
        XIL_AXI_RESP_SLVERR  - 2'b10     Slave Error
        XIL_AXI_RESP_DECERR  - 2'b11     Decode Error
        */
        begin
            if (readResp!=0)
                $display("[FAIL] Read Response FAILED : %s\n", readResp);
            else
                if (readVal==expectedVal)
                    $display("[PASS] Read: 0x%x  Expected: 0x%x",readVal, expectedVal);
                else
                    $display("[FAIL] Read: 0x%x  Expected: 0x%x",readVal, expectedVal);
        end
    endtask :verify_axi_read
           

    task load_ram;
        input bit[3:0] niter;
        input bit[15:0] block_size;
        begin
            addr = `ULBF_0PORT_BASE_ADDR;
            for (integer ii=0; ii<niter; ii=ii+1) begin
                for (integer j=0; j<block_size; j=j+1) begin
                    data = j;
                    master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
                    $display("[load_ram] Addr: 0x%x Written: 0x%x", addr, data); 
                    addr = addr + 4;
                end
            end
        end
    endtask :load_ram
      

    task check_ram;
        input bit[3:0] niter;
        input bit[15:0] block_size;
        output bit error;
        begin
            addr = `ULBF_0PORT_BASE_ADDR;
            for (integer ii=0; ii<niter; ii=ii+1) begin
                for (integer j=0; j<block_size; j=j+1) begin
                    master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
                    $display("[check_ram] Addr: 0x%x Read: 0x%x  Expected: 0x%x", addr, data, j);  
                    if (data!=j)
                        error = 1;
                    addr = addr + 4;
                end
            end
        end
    endtask     :check_ram   


    /*****************************************************************************************************************
    * Task slv_gen_tready shows how slave VIP agent generates one customerized tready signal. 
    * Declare axi4stream_ready_gen  ready_gen
    * Call create_ready from agent's driver to create a new class of axi4stream_ready_gen 
    * Set the poicy of ready generation in this example, it select XIL_AXI4STREAM_READY_GEN_OSC 
    * Set low time 
    * Set high time
    * Agent's driver send_tready out
    * Ready generation policy are listed below:
    *   XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE     - Ready stays asserted and will not change. The driver
    *                                              will still check for policy changes.
    *   XIL_AXI4STREAM_READY_GEN_SINGLE             - Ready stays low for low_time,goes high and stay high till one 
    *                                         ready/valid handshake occurs, it then goes to low repeats this pattern. 
    *   XIL_AXI4STREAM_READY_GEN_EVENTS             - Ready stays low for low_time,goes high and stay high till one
    *                                          a certain amount of ready/valid handshake occurs, it then goes to 
    *                                          low and repeats this pattern.  
    *   XIL_AXI4STREAM_READY_GEN_OSC                - Ready stays low for low_time and then goes to high and stays 
    *                                          high for high_time, it then goes to low and repeat the same pattern
    *   XIL_AXI4STREAM_READY_GEN_RANDOM             - Ready generates randomly 
    *   XIL_AXI4STREAM_READY_GEN_AFTER_VALID_SINGLE - Ready stays low, once valid goes high, ready stays low for
    *                                          low_time, then it goes high and stay high till one ready/valid handshake 
    *                                          occurs. it then goes low and repeate the same pattern.
    *   XIL_AXI4STREAM_READY_GEN_AFTER_VALID_EVENTS - Ready stays low, once valid goes high, ready stays low for low_time,
    *                                          then it goes high and stay high till some amount of ready/valid handshake
    *                                          event occurs. it then goes low and repeate the same pattern.
    *   XIL_AXI4STREAM_READY_GEN_AFTER_VALID_OSC    - Ready stays low, once valid goes high, ready stays low for low_time, 
    *                                          then it goes high and stay high for high_time. it then goes low 
    *                                          and repeate the same pattern.
    *****************************************************************************************************************/
    task slv_gen_tready();
        input [3:0] niter;
        input [15:0] block_size;
        axi4stream_ready_gen  ready_gen0, ready_gen1, ready_gen2, ready_gen3, ready_gen4, ready_gen5, ready_gen6, ready_gen7;
        begin
            //axi4stream_ready_gen  ready_gen;
            ready_gen0 = slave_agent_stream0.driver.create_ready("ready_gen");
            ready_gen1 = slave_agent_stream1.driver.create_ready("ready_gen");
            ready_gen2 = slave_agent_stream2.driver.create_ready("ready_gen");
            ready_gen3 = slave_agent_stream3.driver.create_ready("ready_gen");
            ready_gen4 = slave_agent_stream4.driver.create_ready("ready_gen");
            ready_gen5 = slave_agent_stream5.driver.create_ready("ready_gen");
            ready_gen6 = slave_agent_stream6.driver.create_ready("ready_gen");
            ready_gen7 = slave_agent_stream7.driver.create_ready("ready_gen");
            //ready_gen.set_ready_policy(XIL_AXI4STREAM_READY_GEN_RANDOM);
            ready_gen0.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen0.set_low_time_range(1, 10);
            ready_gen0.set_high_time_range(1, 10);
            ready_gen0.set_event_count_range(1, 100);
            ready_gen1.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen1.set_low_time_range(1, 10);
            ready_gen1.set_high_time_range(1, 10);
            ready_gen1.set_event_count_range(1, 100);
            ready_gen2.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen2.set_low_time_range(1, 10);
            ready_gen2.set_high_time_range(1, 10);
            ready_gen2.set_event_count_range(1, 100);
            ready_gen3.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen3.set_low_time_range(1, 10);
            ready_gen3.set_high_time_range(1, 10);
            ready_gen3.set_event_count_range(1, 100);
            ready_gen4.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen4.set_low_time_range(1, 10);
            ready_gen4.set_high_time_range(1, 10);
            ready_gen4.set_event_count_range(1, 100);
            ready_gen5.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen5.set_low_time_range(1, 10);
            ready_gen5.set_high_time_range(1, 10);
            ready_gen5.set_event_count_range(1, 100);
            ready_gen6.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen6.set_low_time_range(1, 10);
            ready_gen6.set_high_time_range(1, 10);
            ready_gen6.set_event_count_range(1, 100);
            ready_gen7.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
            ready_gen7.set_low_time_range(1, 10);
            ready_gen7.set_high_time_range(1, 10);
            ready_gen7.set_event_count_range(1, 100);
            slave_agent_stream0.driver.send_tready(ready_gen0);
            slave_agent_stream1.driver.send_tready(ready_gen1);
            slave_agent_stream2.driver.send_tready(ready_gen2);
            slave_agent_stream3.driver.send_tready(ready_gen3);
            slave_agent_stream4.driver.send_tready(ready_gen4);
            slave_agent_stream5.driver.send_tready(ready_gen5);
            slave_agent_stream6.driver.send_tready(ready_gen6);
            slave_agent_stream7.driver.send_tready(ready_gen7);
        end
    endtask :slv_gen_tready



    /***************************************************************************************************
    * Get monitor transaction from slave VIP monitor analysis port
    * Put the transactin into slave monitor transaction queue 
    ***************************************************************************************************/
    
    initial begin
        xil_axi4stream_data_byte tdata;
        forever begin
          slave_agent_stream0.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream1.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream2.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream3.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream4.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream5.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream6.monitor.item_collected_port.get(slv_monitor_transaction);
          slave_agent_stream7.monitor.item_collected_port.get(slv_monitor_transaction);
          //slv_monitor_transaction.get_data(tdata);
          //$display("[SLAVE_MONITOR] Data:0x%x", tdata ); 
          //slave_moniter_transaction_queue.push_back(slv_monitor_transaction);
          //slave_moniter_transaction_queue_size++;
        end
    end
    

endmodule
