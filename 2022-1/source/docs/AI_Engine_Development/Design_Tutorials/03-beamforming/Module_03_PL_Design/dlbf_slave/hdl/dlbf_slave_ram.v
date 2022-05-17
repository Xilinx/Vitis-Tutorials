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


module dlbf_slave_ram #(
    parameter TDATA_WIDTH = 64,
    parameter TKEEP_WIDTH = 8,
    parameter URAM_DEPTH = 8192,
    parameter URAM_READ_LATENCY = 4
    ) (
    // axi4s slave
    input wire                   s_axis_tvalid,
    input wire                   s_axis_tlast,
    input wire [TDATA_WIDTH-1:0] s_axis_tdata,
    output wire                  s_axis_tready,
    input wire [TKEEP_WIDTH-1:0] s_axis_tkeep,
    input                        s_axis_clk,

    input wire BRAM_PORTA_rst,
    input wire BRAM_PORTA_clk,
    
    
    input wire slave_rst,
    output wire rxdone,
    input wire [11:0] niter,
    output wire [3:0] current_state_wire,
    output wire [15:0] rxram_counter_wire,

    input wire [7:0] web,
    input wire enb,
    input wire [15:0] addrb,
    input wire [63:0] dinb,
    output wire [63:0] doutb
    
    );
    
    
    wire [15:0] addra;
    wire [63:0] douta, dina;
    wire [7:0]  wea;
    wire        ena;
    
    
    reg [11:0] iter_counter = 12'b0;

   // ---------------------------------------------
    wire [15:0] addrb_int;
    (* KEEP = "TRUE" *) reg [15:0] addrb_0=0;
    (* KEEP = "TRUE" *) reg [15:0] addrb_1=0;
    (* KEEP = "TRUE" *) reg [15:0] addrb_2=0;
    (* KEEP = "TRUE" *) reg [15:0] addrb_3=0;
    always@(posedge s_axis_clk)
    begin
        addrb_0 <= addrb;
        addrb_1 <= addrb_0;
        addrb_2 <= addrb_1;
        addrb_3 <= addrb_2;
    end
    assign addrb_int = addrb_3;

    wire [7:0] web_int;
    (* KEEP = "TRUE" *) reg [7:0] web_0=0;
    (* KEEP = "TRUE" *) reg [7:0] web_1=0;
    (* KEEP = "TRUE" *) reg [7:0] web_2=0;
    (* KEEP = "TRUE" *) reg [7:0] web_3=0;
    always@(posedge s_axis_clk)
    begin
        web_0 <= web;
        web_1 <= web_0;
        web_2 <= web_1;
        web_3 <= web_2;
    end
    assign web_int = web_3;

    wire enb_int;
    (* KEEP = "TRUE" *) reg enb_0=0;
    (* KEEP = "TRUE" *) reg enb_1=0;
    (* KEEP = "TRUE" *) reg enb_2=0;
    (* KEEP = "TRUE" *) reg enb_3=0;
    always@(posedge s_axis_clk)
    begin
        enb_0 <= enb;
        enb_1 <= enb_0;
        enb_2 <= enb_1;
        enb_3 <= enb_2;
    end
    assign enb_int = enb_3;   // --------------------------------------------- 
   
   // rx go
   // control machine for dma of axi4s master into rxram
    assign s_axis_tready = 1'b1;
    
    reg         rxram_we=1'b0;
    reg [15:0]  rxram_counter=16'd0;
    reg [3:0]   rxpayload_id=4'b0;
    reg [73:0] rxram_data=74'b0;
    reg [73:0] rxram_data0=74'b0;
    reg [73:0] rxram_data1=74'b0;
    
    
    // MAIN FSM states - one hot
    //parameter S0  = 4'b0001;
    //parameter S1  = 4'b0010;
    //reg [3:0] current_state=S0;
    reg [3:0] current_state=4'h1;
    
    assign rxram_counter_wire = rxram_counter;
    assign current_state_wire = current_state;
   
   
    always@(posedge s_axis_clk)
    begin
        rxram_data0	<= {s_axis_tlast, s_axis_tvalid, s_axis_tkeep, s_axis_tdata};
	rxram_data1	<= rxram_data0;
	rxram_data	<= rxram_data1;
   end

    wire s_axis_tlast_ff, s_axis_tvalid_ff;
    assign s_axis_tlast_ff  = rxram_data1[73];
    assign s_axis_tvalid_ff = rxram_data1[72];

    // logic to take data from rx axis and load it into bram
    always@(posedge s_axis_clk)
    begin
        if (slave_rst)
            begin
              rxram_counter <= 16'd0;
              current_state <= 4'h1;
	      iter_counter <= 12'd0;	
	      rxpayload_id <= 4'h0;
	      rxram_we <= 1'b0	;
            end
        else
        begin    
            case(current_state)
              4'h1: begin // idle - wait for tvalid
                if (s_axis_tvalid_ff)  begin
                  if (s_axis_tlast_ff) begin    // tvalid and tlast in same cycle
                    rxpayload_id <= 4'ha;
                    current_state <= 4'h1;
		    iter_counter <= iter_counter + 12'b1;
                    end  
                  else begin
                    rxpayload_id <= 4'hb;
                    current_state <= 4'h2;
                    end
                  rxram_counter <= rxram_counter + 'b1;
                  rxram_we <= 1'b1;
                end
                else begin
                    current_state <= 4'h1;
                    rxram_we <= 1'b0;
                    end
                end
              
              4'h2: begin // wait for tlast after tvalid
                if (s_axis_tvalid_ff) begin
                    if (s_axis_tlast_ff) begin    
                      rxpayload_id <= 4'he;
                      current_state <= 4'h1;
		      iter_counter <= iter_counter + 12'b1;
                      end
                    else begin
                      rxpayload_id <= 4'h0;
                      current_state <= 4'h2;
                      end
                    rxram_counter <= rxram_counter + 'b1;  
                    rxram_we <= 1'b1;  
                    end
                 else begin
                    rxram_we <= 1'b0;
                    current_state <= 4'h2;
                    end
                end        
              
              default: begin
                  current_state <= 4'h1;
                  rxram_we <= 1'b0;
                  end    
            endcase 
        end         
     end
    
    reg 	rxram_we_ff0 = 1'b0;
    reg 	rxram_we_ff1 = 1'b0;
    reg [15:0]  rxram_counter_ff0 = 16'b0;
    reg [15:0]  rxram_counter_ff1 = 16'b0;
    reg [15:0]  rxram_counter_ff2 = 16'b0;
    reg [63:0]  rxram_data_ff0 = 64'b0;
    reg [63:0]  rxram_data_ff1 = 64'b0;

    
    wire iter_counter_done;

   (* KEEP = "TRUE" *) wire niter_cmp0;
   (* KEEP = "TRUE" *) wire niter_cmp1;
   (* KEEP = "TRUE" *) wire niter_cmp2;
   (* KEEP = "TRUE" *) wire niter_cmp3;
   
   assign niter_cmp0 = niter[2:0]==iter_counter[2:0];
   assign niter_cmp1 = niter[5:3]==iter_counter[5:3];
   assign niter_cmp2 = niter[8:6]==iter_counter[8:6];
   assign niter_cmp3 = niter[11:9]==iter_counter[11:9];
   
   assign iter_counter_done = &{niter_cmp0, niter_cmp1, niter_cmp2, niter_cmp3}; 
   
   //assign rxdone = (niter==12'd0) ? 1'b0 : ( (iter_counter==niter) ? 1'b1 : 1'b0 ) ;
   assign rxdone = (niter==12'd0) ? 1'b0 : ( (iter_counter_done) ? 1'b1 : 1'b0 ) ;
   
    always@(posedge s_axis_clk)
    begin
        rxram_we_ff0 <= rxram_we;
        rxram_we_ff1 <= rxram_we_ff0;
 	
	rxram_data_ff0 <= rxram_data;
	rxram_data_ff1 <= rxram_data_ff0;

	rxram_counter_ff0 <= rxram_counter;
	rxram_counter_ff1 <= rxram_counter_ff0;
	rxram_counter_ff2 <= rxram_counter_ff1;
    end

    assign addra      = rxram_counter_ff2;
    assign wea        = {8{rxram_we_ff1}};
    assign ena        = rxram_we_ff1;
    assign dina       = rxram_data_ff1[63:0];
    
   
   // rst porb - rd only--------------------------------------------
   wire rsta;
   assign rsta = slave_rst;
   wire rstb;
   //assign rstb = BRAM_PORTA_rst;
   assign rstb = slave_rst;
   //---------------------------------------------------------------
    // choice 2 of 2: xpm memory
              
              
   // xpm_memory_tdpram: True Dual Port RAM
   // Xilinx Parameterized Macro, version 2019.2

    parameter ADDR_WIDTH  = 16;  // Address Width
    parameter BYTE_WRITE_WIDTH = 8;   
    parameter DATA_WIDTH  = TDATA_WIDTH;  // Data Width, (Byte * NUM_COL) 
    parameter READ_LATENCY  = URAM_READ_LATENCY;    // Number of pipeline Registers
    parameter MEMORY_SIZE = TDATA_WIDTH*URAM_DEPTH; // 64x4096

   wire dbiterra, dbiterrb;
   wire sbiterra, sbiterrb;
   xpm_memory_tdpram #(
      // Port A
      .ADDR_WIDTH_A(ADDR_WIDTH),               // DECIMAL
      .BYTE_WRITE_WIDTH_A(BYTE_WRITE_WIDTH),        // DECIMAL
      .READ_DATA_WIDTH_A(DATA_WIDTH),         // DECIMAL
      .READ_LATENCY_A(READ_LATENCY),             // DECIMAL
      .READ_RESET_VALUE_A("0"),       // String
      .RST_MODE_A("SYNC"),            // String
      .WRITE_DATA_WIDTH_A(DATA_WIDTH),        // DECIMAL
      .WRITE_MODE_A("no_change"),     // String
      
      // Port B
      .ADDR_WIDTH_B(ADDR_WIDTH),               // DECIMAL
      .BYTE_WRITE_WIDTH_B(BYTE_WRITE_WIDTH),        // DECIMAL
      .READ_DATA_WIDTH_B(DATA_WIDTH),         // DECIMAL
      .READ_LATENCY_B(READ_LATENCY),             // DECIMAL
      .READ_RESET_VALUE_B("0"),       // String
      .RST_MODE_B("SYNC"),            // String
      .WRITE_DATA_WIDTH_B(DATA_WIDTH),        // DECIMAL
      .WRITE_MODE_B("no_change"),      // String
      
      // Common
      .AUTO_SLEEP_TIME(0),            // DECIMAL
      .CASCADE_HEIGHT(1),             // DECIMAL
      .CLOCKING_MODE("common_clock"), // String
      .ECC_MODE("no_ecc"),            // String
      .MEMORY_INIT_FILE("none"),      // String
      .MEMORY_INIT_PARAM("0"),        // String
      .MEMORY_OPTIMIZATION("false"),   // String
      .MEMORY_PRIMITIVE("ultra"),      // String
      .MEMORY_SIZE(MEMORY_SIZE),             // DECIMAL 
      .MESSAGE_CONTROL(0),            // DECIMAL
      .SIM_ASSERT_CHK(0),             // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .USE_EMBEDDED_CONSTRAINT(0),    // DECIMAL
      .USE_MEM_INIT(1),               // DECIMAL
      .WAKEUP_TIME("disable_sleep")  // String
      
   )
   xpm_memory_tdpram_inst (
      // Port A - write/rd
      .dbiterra(dbiterra),             // 1-bit output: Status signal to indicate double bit error occurrence
                                       // on the data output of port A.
      .douta(douta),                   // READ_DATA_WIDTH_A-bit output: Data output for port A read operations.
      .sbiterra(sbiterra),             // 1-bit output: Status signal to indicate single bit error occurrence
                                       // on the data output of port A.
      .addra(addra),                   // ADDR_WIDTH_A-bit input: Address for port A write and read operations.
      .clka(s_axis_clk),                     // 1-bit input: Clock signal for port A. Also clocks port B when
                                       // parameter CLOCKING_MODE is "common_clock".
      .dina(dina),                     // WRITE_DATA_WIDTH_A-bit input: Data input for port A write operations.
      .ena(ena),                       // 1-bit input: Memory enable signal for port A. Must be high on clock
                                       // cycles when read or write operations are initiated. Pipelined
                                       // internally.
      .injectdbiterra(1'b0), // 1-bit input: Controls double bit error injection on input data when
                                       // ECC enabled (Error injection capability is not available in
                                       // "decode_only" mode).
      .injectsbiterra(1'b0), // 1-bit input: Controls single bit error injection on input data when
                                       // ECC enabled (Error injection capability is not available in
                                       // "decode_only" mode).
      .regcea(1'b1),                 // 1-bit input: Clock Enable for the last register stage on the output
                                       // data path.
      .rsta(rsta),                     // 1-bit input: Reset signal for the final port A output register stage.
                                       // Synchronously resets output port douta to the value specified by
                                       // parameter READ_RESET_VALUE_A.
      .wea(wea),                       // WRITE_DATA_WIDTH_A-bit input: Write enable vector for port A input
                                       // data port dina. 1 bit wide when word-wide writes are used. In
                                       // byte-wide write configurations, each bit controls the writing one
                                       // byte of dina to address addra. For example, to synchronously write
                                       // only bits [15-8] of dina when WRITE_DATA_WIDTH_A is 32, wea would be
                                       // 4'b0010.


      
      // Port B - rd only
      .dbiterrb(dbiterrb),             // 1-bit output: Status signal to indicate double bit error occurrence
                                       // on the data output of port A.
      .doutb(doutb),                   // READ_DATA_WIDTH_B-bit output: Data output for port B read operations.
      .sbiterrb(sbiterrb),             // 1-bit output: Status signal to indicate single bit error occurrence
                                       // on the data output of port B.
      .addrb(addrb_int),                   // ADDR_WIDTH_B-bit input: Address for port B write and read operations.
      //.clkb(BRAM_PORTA_clk),           // 1-bit input: Clock signal for port B when parameter CLOCKING_MODE is
                                       // "independent_clock". Unused when parameter CLOCKING_MODE is
                                       // "common_clock".
      .dinb(dinb),                     // WRITE_DATA_WIDTH_B-bit input: Data input for port B write operations.
      .enb(enb_int),                       // 1-bit input: Memory enable signal for port B. Must be high on clock
                                       // cycles when read or write operations are initiated. Pipelined
                                       // internally.
      .injectdbiterrb(1'b0), // 1-bit input: Controls double bit error injection on input data when
                                       // ECC enabled (Error injection capability is not available in
                                       // "decode_only" mode).
      .injectsbiterrb(1'b0), // 1-bit input: Controls single bit error injection on input data when
                                       // ECC enabled (Error injection capability is not available in
                                       // "decode_only" mode).
      .regceb(1'b1),                 // 1-bit input: Clock Enable for the last register stage on the output
                                       // data path.
      .rstb(rstb),                     // 1-bit input: Reset signal for the final port B output register stage.
                                       // Synchronously resets output port doutb to the value specified by
                                       // parameter READ_RESET_VALUE_B.
      .web(web_int),                        // WRITE_DATA_WIDTH_B-bit input: Write enable vector for port B input
                                       // data port dinb. 1 bit wide when word-wide writes are used. In
                                       // byte-wide write configurations, each bit controls the writing one
                                       // byte of dinb to address addrb. For example, to synchronously write
                                       // only bits [15-8] of dinb when WRITE_DATA_WIDTH_B is 32, web would be
                                       // 4'b0010.
      .sleep(1'b0)                   // 1-bit input: sleep signal to enable the dynamic power saving feature.
      
   );

   // End of xpm_memory_tdpram_inst instantiation  
   
   
endmodule

