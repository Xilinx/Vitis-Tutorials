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

module ulbf_data_xpm_ram #(
    parameter RAM_DEPTH = 4096,
    parameter DATA_WIDTH = 64,
    parameter RAM_READ_LATENCY = 4,
    parameter MEM_INIT_FILE = "ulbf_din0_hex.mem"
    ) (
    
    // port-a  cips to ram (read/write)
    output wire [DATA_WIDTH-1:0] douta,
    input wire [DATA_WIDTH-1:0] dina,
    input wire                   ena,
    input wire [DATA_WIDTH/8-1:0] wea,
    input wire [15:0]            addra,
    input wire                   bram_rst,
    input wire                   bram_clk,

    // port-b  stream to ram (read only)
    output wire [DATA_WIDTH-1:0] doutb,
    input wire                   enb,
    input wire [15:0]            addrb,
    input wire                   m_axis_rst,
    input wire                   m_axis_clk
    );
    
    wire sbiterra, dbiterra;
    wire sbiterrb, dbiterrb;
    
    // ---------------------------------------------------------------------------------------------   
    // choice 2 of 2: xpm memory
    // xpm_memory_tdpram: True Dual Port RAM
    // Xilinx Parameterized Macro, version 2019.2
    
    parameter ADDR_WIDTH  = 15;  // Address Width
    parameter BYTE_WRITE_WIDTH = 8;   // Number of columns
    //parameter `DATA_WIDTH  = `DATA_WIDTH;  // Data Width, (Byte * NUM_COL) 
    parameter READ_LATENCY  = RAM_READ_LATENCY;    // Number of pipeline Registers
    parameter MEMORY_SIZE = DATA_WIDTH*RAM_DEPTH; 
    
    
   xpm_memory_tdpram #(
      // Port A
      .ADDR_WIDTH_A(ADDR_WIDTH),                    // DECIMAL
      .BYTE_WRITE_WIDTH_A(BYTE_WRITE_WIDTH),        // DECIMAL
      .READ_DATA_WIDTH_A(DATA_WIDTH),               // DECIMAL
      .READ_LATENCY_A(READ_LATENCY),                // DECIMAL
      .READ_RESET_VALUE_A("0"),                     // String
      .RST_MODE_A("SYNC"),                          // String
      .WRITE_DATA_WIDTH_A(DATA_WIDTH),              // DECIMAL
      .WRITE_MODE_A("no_change"),                   // String
      
      // Port B
      .ADDR_WIDTH_B(ADDR_WIDTH),                    // DECIMAL
      .BYTE_WRITE_WIDTH_B(BYTE_WRITE_WIDTH),        // DECIMAL
      .READ_DATA_WIDTH_B(DATA_WIDTH),               // DECIMAL
      .READ_LATENCY_B(READ_LATENCY),                // DECIMAL
      .READ_RESET_VALUE_B("0"),                     // String
      .RST_MODE_B("SYNC"),                          // String
      .WRITE_DATA_WIDTH_B(DATA_WIDTH),              // DECIMAL
      .WRITE_MODE_B("no_change"),                   // String
      
      // Common
      .AUTO_SLEEP_TIME(0),                          // DECIMAL
      .CASCADE_HEIGHT(1),                           // DECIMAL
      .CLOCKING_MODE("independent_clock"),          // String
      .ECC_MODE("no_ecc"),                          // String
      .MEMORY_INIT_FILE(MEM_INIT_FILE),             // String
      .MEMORY_INIT_PARAM("0"),                      // String
      .MEMORY_OPTIMIZATION("false"),                // String
      .MEMORY_PRIMITIVE("block"),                   // String
      .MEMORY_SIZE(MEMORY_SIZE),                    // DECIMAL 
      .MESSAGE_CONTROL(0),                          // DECIMAL
      .SIM_ASSERT_CHK(0),                           // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .USE_EMBEDDED_CONSTRAINT(0),                  // DECIMAL
      .USE_MEM_INIT(1),                             // DECIMAL
      .WAKEUP_TIME("disable_sleep")                 // String
      
   )
   xpm_memory_tdpram_inst (
      // Port A - cips
      .dbiterra(dbiterra),             // 1-bit output: Status signal to indicate double bit error occurrence
                                       // on the data output of port A.
      .douta(douta),                   // READ_`DATA_WIDTH_A-bit output: Data output for port A read operations.
      .sbiterra(sbiterra),             // 1-bit output: Status signal to indicate single bit error occurrence
                                       // on the data output of port A.
      .addra(addra),                   // ADDR_WIDTH_A-bit input: Address for port A write and read operations.
      .clka(bram_clk),                     // 1-bit input: Clock signal for port A. Also clocks port B when
                                       // parameter CLOCKING_MODE is "common_clock".
      .dina(dina),                     // WRITE_`DATA_WIDTH_A-bit input: Data input for port A write operations.
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
      .rsta(bram_rst),                     // 1-bit input: Reset signal for the final port A output register stage.
                                       // Synchronously resets output port douta to the value specified by
                                       // parameter READ_RESET_VALUE_A.
      .wea(wea),                       // WRITE_`DATA_WIDTH_A-bit input: Write enable vector for port A input
                                       // data port dina. 1 bit wide when word-wide writes are used. In
                                       // byte-wide write configurations, each bit controls the writing one
                                       // byte of dina to address addra. For example, to synchronously write
                                       // only bits [15-8] of dina when WRITE_`DATA_WIDTH_A is 32, wea would be
                                       // 4'b0010.


      
      // Port B - stream
      .dbiterrb(dbiterrb),             // 1-bit output: Status signal to indicate double bit error occurrence
                                       // on the data output of port A.
      .doutb(doutb),                   // READ_`DATA_WIDTH_B-bit output: Data output for port B read operations.
      .sbiterrb(sbiterrb),             // 1-bit output: Status signal to indicate single bit error occurrence
                                       // on the data output of port B.
      .addrb(addrb),                   // ADDR_WIDTH_B-bit input: Address for port B write and read operations.
      .clkb(m_axis_clk),                     // 1-bit input: Clock signal for port B when parameter CLOCKING_MODE is
                                       // "independent_clock". Unused when parameter CLOCKING_MODE is
                                       // "common_clock".
      .dinb(64'd0),                     // WRITE_`DATA_WIDTH_B-bit input: Data input for port B write operations.
      .enb(enb),                       // 1-bit input: Memory enable signal for port B. Must be high on clock
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
      .rstb(m_axis_rst),                     // 1-bit input: Reset signal for the final port B output register stage.
                                       // Synchronously resets output port doutb to the value specified by
                                       // parameter READ_RESET_VALUE_B.
      .web(8'd0),                        // WRITE_`DATA_WIDTH_B-bit input: Write enable vector for port B input
                                       // data port dinb. 1 bit wide when word-wide writes are used. In
                                       // byte-wide write configurations, each bit controls the writing one
                                       // byte of dinb to address addrb. For example, to synchronously write
                                       // only bits [15-8] of dinb when WRITE_`DATA_WIDTH_B is 32, web would be
                                       // 4'b0010.

      .sleep(1'b0)                   // 1-bit input: sleep signal to enable the dynamic power saving feature.
      
   );

   // End of xpm_memory_tdpram_inst instantiation                                         
  
endmodule


