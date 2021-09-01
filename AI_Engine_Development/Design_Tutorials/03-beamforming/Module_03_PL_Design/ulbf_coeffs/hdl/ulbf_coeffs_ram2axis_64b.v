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


module ulbf_coeffs_ram2axis_64b #(
        parameter DATA_WIDTH=64,
        parameter RAM_READ_LATENCY=4,
        parameter MEM_INIT_FILE="ulbf_cin00_hex.mem",
        parameter RAM_DEPTH=4096
        ) (
        
        input wire go,
        input wire m_axis_rst,
        output wire  done,
	output wire [15:0] addrb_wire,
        
        input wire [11:0] block_size,
        input wire [11:0] niter,
        input wire [15:0] rollover_addr,
    
        // fifo
        input wire [DATA_WIDTH-1:0] fifo_dout,
        input wire fifo_data_valid,
        input wire fifo_prog_full,
        output wire fifo_rd_en,
        output wire fifo_wr_en,
        output wire [DATA_WIDTH-1:0] fifo_din,
        
        // axi4s master
        input wire m_axis_clk,  
        input wire m_axis_tready, 
        output reg m_axis_tvalid,
        output reg m_axis_tlast,  
        output reg [DATA_WIDTH-1:0]    m_axis_tdata, 
        output reg [DATA_WIDTH/8 -1:0] m_axis_tkeep,

        output wire [DATA_WIDTH-1:0] douta,
        input wire [DATA_WIDTH-1:0] dina,
        input wire                  bram_clk,
        input wire                  bram_rst,
        input wire                  ena,
        input wire [DATA_WIDTH/8 -1:0] wea,
        input wire [15:0]           addra
    );
            
     wire go_int;            
    (* KEEP = "TRUE" *) reg go_0=0;
    (* KEEP = "TRUE" *) reg go_1=0;
    (* KEEP = "TRUE" *) reg go_2=0;
    (* KEEP = "TRUE" *) reg go_3_0=0;
    (* KEEP = "TRUE" *) reg go_3_1=0;
    (* KEEP = "TRUE" *) reg go_3_2=0;
    always@(posedge m_axis_clk)
    begin
        go_0 <= go;
        go_1 <= go_0;
        go_2 <= go_1;
        go_3_0 <= go_2;
        go_3_1 <= go_2;
        go_3_2 <= go_2;
    end
    assign go_int = go_3_0;

    (* KEEP = "TRUE" *) reg done_int=0;
    (* KEEP = "TRUE" *) reg done_0=0;
    (* KEEP = "TRUE" *) reg done_1=0;
    (* KEEP = "TRUE" *) reg done_2=0;
    (* KEEP = "TRUE" *) reg done_3=0;
    always@(posedge m_axis_clk)
    begin
        done_0 <= done_int;
        done_1 <= done_0;
        done_2 <= done_1;
        done_3 <= done_2;
    end
    assign done = done_3;

    wire m_axis_rst_int;
    (* KEEP = "TRUE" *) reg m_axis_rst_0=0;
    (* KEEP = "TRUE" *) reg m_axis_rst_1=0;
    (* KEEP = "TRUE" *) reg m_axis_rst_2=0;
    (* KEEP = "TRUE" *) reg m_axis_rst_3=0;
    always@(posedge m_axis_clk)
    begin
        m_axis_rst_0 <= m_axis_rst;   //Q: below 3 commands will get optimized away
        m_axis_rst_1 <= m_axis_rst_0;  
        m_axis_rst_2 <= m_axis_rst_1;
        m_axis_rst_3 <= m_axis_rst_2;
    end
    //assign m_axis_rst_int = m_axis_rst_3;
    assign m_axis_rst_int = m_axis_rst;

    //-------------- end of pipeline 


    reg [15:0] rollover_addr_minus1=16'd0;
    reg [11:0] block_size_minus1=12'd0;
    reg [11:0] block_size_minus2=12'd0;
    reg [11:0] niter_minus1=12'd0;
    reg [11:0] niter_ff=12'd0;
    always@(posedge m_axis_clk)
    begin
      rollover_addr_minus1 <= rollover_addr - 16'd1;
      niter_ff <= niter;
      niter_minus1 <= niter - 12'd1;
      block_size_minus1 <= block_size-12'd1;
      block_size_minus2 <= block_size_minus1-12'd1;
    end

  // fifo to axi
    reg [11:0] block_counter=12'd0;
    reg [11:0] iter_counter=12'd0;
    
// Q: why 4 individual compares?
   (* KEEP = "TRUE" *) wire block_counter_cmp0;
   (* KEEP = "TRUE" *) wire block_counter_cmp1;
   (* KEEP = "TRUE" *) wire block_counter_cmp2;
   (* KEEP = "TRUE" *) wire block_counter_cmp3;
   wire block_counter_rst;

   assign block_counter_cmp0 = block_counter[2:0]==block_size_minus1[2:0];
   assign block_counter_cmp1 = block_counter[5:3]==block_size_minus1[5:3];
   assign block_counter_cmp2 = block_counter[8:6]==block_size_minus1[8:6];
   assign block_counter_cmp3 = block_counter[11:9]==block_size_minus1[11:9];

   assign block_counter_rst = &{block_counter_cmp0,block_counter_cmp1,block_counter_cmp2,block_counter_cmp3};
    
    
    assign fifo_rd_en = m_axis_tready && (go_int) &&(~done_int);
    
    //*********************************************
    
    always@(*)
    begin
      if((m_axis_tready) && (go_3_1) && (~done_int))
          begin
              m_axis_tvalid <= fifo_data_valid;
              m_axis_tkeep <= {8{fifo_data_valid}};
              m_axis_tdata <= fifo_dout;
              if (block_counter==block_size_minus1)
                    m_axis_tlast <= 1'b1;
              else
                    m_axis_tlast <= 1'b0;
          end
        else
          begin
              m_axis_tvalid <= 1'b0;
              m_axis_tkeep <= 8'd0;
              m_axis_tdata <= 64'd0;  
              m_axis_tlast <= 1'b0;  
          end
    end
    
    //*********************************************
   
//Q: Same question as before.    
   (* KEEP = "TRUE" *) wire niter_cmp0;
   (* KEEP = "TRUE" *) wire niter_cmp1;
   (* KEEP = "TRUE" *) wire niter_cmp2;
   (* KEEP = "TRUE" *) wire niter_cmp3;
   wire iter_counter_rst;
   
   assign niter_cmp0 = niter_minus1[2:0]==iter_counter[2:0];
   assign niter_cmp1 = niter_minus1[5:3]==iter_counter[5:3];
   assign niter_cmp2 = niter_minus1[8:6]==iter_counter[8:6];
   assign niter_cmp3 = niter_minus1[11:9]==iter_counter[11:9];
   
   assign iter_counter_rst = &{niter_cmp0, niter_cmp1, niter_cmp2, niter_cmp3}; 
 
    always@(posedge m_axis_clk)
    begin
      if (m_axis_rst_int)
          block_counter <= 12'd0;
      else
        if(m_axis_tvalid)
            begin
              //if (block_counter==block_size_minus1)
              if (block_counter_rst)
                    block_counter <= 12'd0;
              else
                    block_counter <= block_counter + 1;
            end
    end
    
    always@(posedge m_axis_clk)
    begin
      if (m_axis_rst_int)
          iter_counter <= 12'd0;
      else
       if (m_axis_tlast)   
          iter_counter <= iter_counter + 12'b1; 
    end    

    always@(posedge m_axis_clk)
    begin
      if (m_axis_rst_int)
          done_int <= 1'b0;
      else
          if ( (~done_int) && (niter_ff!=12'h000) )
             //done_int <= ((iter_counter==niter_minus1) && (block_counter==block_size_minus1));
             done_int <= ((iter_counter_rst) && (block_counter_rst));
    end
    
   // uram to fifo
   reg [RAM_READ_LATENCY-1:0] fifo_wr_en_ff;
   
   always@(posedge m_axis_clk)
   begin
       if (m_axis_rst_int) 
           fifo_wr_en_ff <= 'd0;
       else 
           if (go_int)
             fifo_wr_en_ff <= {fifo_wr_en_ff[RAM_READ_LATENCY-2:0], (~fifo_prog_full)};
   end
   assign  fifo_wr_en = fifo_wr_en_ff[RAM_READ_LATENCY-1];
   
   reg [15:0] addrb=16'd0;
   
   (* KEEP = "TRUE" *) wire addrb_cmp0;
   (* KEEP = "TRUE" *) wire addrb_cmp1;
   (* KEEP = "TRUE" *) wire addrb_cmp2;
   (* KEEP = "TRUE" *) wire addrb_cmp3;
   (* KEEP = "TRUE" *) wire addrb_cmp4;
   (* KEEP = "TRUE" *) wire addrb_cmp5;
   wire addrb_rst;

   assign addrb_cmp0 = addrb[2:0]==rollover_addr_minus1[2:0];
   assign addrb_cmp1 = addrb[5:3]==rollover_addr_minus1[5:3];
   assign addrb_cmp2 = addrb[8:6]==rollover_addr_minus1[8:6];
   assign addrb_cmp3 = addrb[11:9]==rollover_addr_minus1[11:9];
   assign addrb_cmp4 = addrb[14:12]==rollover_addr_minus1[14:12];
   assign addrb_cmp5 = addrb[15:15]==rollover_addr_minus1[15:15];

   assign addrb_rst = &{addrb_cmp0,addrb_cmp1,addrb_cmp2,addrb_cmp3,addrb_cmp4,addrb_cmp5}; 

   always@(posedge m_axis_clk)
   begin
     if (m_axis_rst_int)
       addrb <= 16'd0;
     else
       if((~fifo_prog_full) && (go_3_2))
         //if (addrb==rollover_addr_minus1)
         if (addrb_rst)
           addrb <= 0;
         else
           addrb <= addrb + 1;
   end
   
	 assign addrb_wire = addrb;

    //uram
    ulbf_coeffs_xpm_ram  #(
        .RAM_DEPTH(RAM_DEPTH),
        .DATA_WIDTH(DATA_WIDTH),
        .RAM_READ_LATENCY(RAM_READ_LATENCY),
        .MEM_INIT_FILE(MEM_INIT_FILE)
    ) ram_master_64b_i (
        .douta(douta),
        .dina(dina),
        .ena(ena),
        .wea(wea),
        .addra(addra),
        .bram_rst(bram_rst),
        .bram_clk(bram_clk),

        .doutb(fifo_din),
        .m_axis_rst(m_axis_rst_int),
        .m_axis_clk(m_axis_clk),
        .enb(~fifo_prog_full),
        .addrb(addrb)
      );
    
        
    
endmodule
