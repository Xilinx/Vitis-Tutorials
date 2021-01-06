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
`define SLAVE_BASE_ADDR  32'hA400_0000
`define SLAVE_CSR_OFFSET 32'h0008_0000

import axi_vip_pkg::*;
import design_1_axi_vip_0_0_pkg::*;
import axi4stream_vip_pkg::*;
import design_1_axi4stream_vip_0_0_pkg::*;

module tb_slave();
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
        .s_axis_clk(clk400)
        );
        
    design_1_axi_vip_0_0_mst_t             master_agent;
    design_1_axi4stream_vip_0_0_mst_t      master_agent_stream;
    bit [31:0] addr, data;
    xil_axi_resp_t 	resp;
    xil_axi4stream_uint   master_agent_stream_verbosity = 0;
        
    reg [3:0] niter=4;
    reg [15:0] block_size=32;    
    integer i=0;
    bit error=0;
    
    initial
    begin
        #(`tCK_100*200);
        master_agent = new("master vip agent",design_1_wrapper_i.design_1_i.axi_vip_0.inst.IF);
        master_agent.start_master();
        #(`tCK_100*10);
        addr = `SLAVE_BASE_ADDR + `SLAVE_CSR_OFFSET+ 32'h0;
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'hbeee_0001, resp);
        addr = `SLAVE_BASE_ADDR + `SLAVE_CSR_OFFSET+ 32'hC;
        data = 32'd4;
        master_agent.AXI4LITE_WRITE_BURST(addr,0,data,resp);
        master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
        verify_axi_read(data, 32'd4, resp);
        #(`tCK_100*50);
        
        master_agent_stream = new("master vip agent",design_1_wrapper_i.design_1_i.axi4stream_vip_0.inst.IF);
        master_agent_stream.vif_proxy.set_dummy_drive_type(XIL_AXI4STREAM_VIF_DRIVE_NONE);
        master_agent_stream.set_agent_tag("Master VIP");
        master_agent_stream.set_verbosity(master_agent_stream_verbosity);
        master_agent_stream.start_master();
        
        fork
            for (integer i=0; i<niter; i=i+1) begin
                send_a_packet(block_size);
            end
            wait_for_rxdone(error);
        join
        if (error) begin
            $display("[FAIL] Slave is not DONE. Check niter, block_size, etc. ");
            end
        else begin
            check_slave_ram(niter, block_size, error);
        end
        
	$display("********************************");
        if (error)
            $display("***** SIMULATION FAILED *********");
        else
            $display("***** SIMULATION PASSED *********");
        $display("********************************");
        $finish;
    end


    task wait_for_rxdone;
        output bit error ;
        reg [31:0] timeout_counter;
        begin
            error = 0;
            timeout_counter = 0;
            addr = `SLAVE_BASE_ADDR + `SLAVE_CSR_OFFSET + 32'h20;
            while(data!=1)
            begin
                if (timeout_counter==100) begin
                    error = 1'b1;
                    break;
                end
                else begin
                    master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
                    #(`tCK_100*10);
                end
                timeout_counter = timeout_counter + 1;
                $display("[wait_ror_rxdone] %d : %d", timeout_counter, data);
            end
        end
    endtask     :wait_for_rxdone    
    
        
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
           
           
    task check_slave_ram;
        input bit[3:0] niter;
        input bit[15:0] block_size;
        output bit error;
        begin
            addr = `SLAVE_BASE_ADDR;
            for (integer ii=0; ii<niter; ii=ii+1) begin
                for (integer j=0; j<block_size; j=j+1) begin
                    master_agent.AXI4LITE_READ_BURST(addr,0,data,resp);
                    if (data!=j)
                        error = 1;
                    $display("[check_slave_ram] Addr: 0x%x Read: 0x%x  Expected: 0x%x ", addr, data, j);    
                    addr = addr + 4;
                end
            end
        end
    endtask     :check_slave_ram    
    
    
    /****************************************************************************************************************
     This task is taken from VIP Example 
     Task send_a_packet shows how to send a packet with a certain number of words and the first 32-bit word is 0,
     the second 32-bit word is 1, and the last 32-bit word is num_words. The final beat has Tlast is 1
    
     This task is just being used in AXI4STREAM VIP when it is being configured to have TLAST and TDATA
     WIDTH >0
    ***************************************************************************************************************/
    task automatic send_a_packet(xil_axi4stream_uint num_words);
      xil_axi4stream_uint                     total_transfer;
      bit[8*8-1:0]              data_beat;
      bit[8-1:0]            keep_beat;
      bit[31:0]                               payload; 
      bit [7:0]                               byte_mem[xil_axi4stream_ulong];
      axi4stream_transaction                  wr_transaction; 
      axi4stream_transaction                  wr_transactionc;    
    
      if((num_words*4)%(8 ) !=0) begin
        $display("Warning, stream VIP TDATA_WIDTH is %d byte, the number of words you wants to pass in a whole packet is %d, the last transfer will be filled with 0",8,num_words );
      end
      
      for(xil_axi4stream_uint word_cnt=0; word_cnt<num_words; word_cnt++) begin
        payload = word_cnt;
        for(xil_axi4stream_uint byte_cnt=0; byte_cnt<4; byte_cnt++) begin
          byte_mem[word_cnt*4+byte_cnt] = payload[byte_cnt*8+:8];
        end  
      end  
     
      if( (num_words*4)%(8 ) !=0) begin
        total_transfer = (num_words*4)/(8 ) +1;
      end else begin
        total_transfer = (num_words*4)/(8 ) ;
      end
    
      for(xil_axi4stream_uint i=0; i<total_transfer; i++) begin
        for(xil_axi4stream_uint byte_cnt=0; byte_cnt<(8); byte_cnt++) begin
          data_beat[byte_cnt*8+:8] = byte_mem[byte_cnt+i*(8)];
          keep_beat[byte_cnt]=1'b1;
        end  
        wr_transaction = master_agent_stream.driver.create_transaction("Master VIP write transaction");
        wr_transactionc = master_agent_stream.driver.create_transaction("Master VIP write transactionc");
        wr_transaction.set_driver_return_item_policy(XIL_AXI4STREAM_AT_ACCEPT_RETURN );
        SEND_PACKET_FAILURE: assert(wr_transaction.randomize());
        wr_transaction.set_data_beat(data_beat);
        wr_transaction.set_keep_beat(keep_beat);
        wr_transaction.set_last(0);
        if(i == total_transfer-1) begin
          wr_transaction.set_last(1);  
        end     
        master_agent_stream.driver.send(wr_transaction);
        master_agent_stream.driver.seq_item_port.get_next_rsp(wr_transactionc);
      end
    endtask  :send_a_packet
    
    
      
endmodule

