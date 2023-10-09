/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// partial Sum BRAMs -possibe configurations 512*64-bit or 256 *128-bit
// On each PL clk we receive 32*128-bit or 64*64-bit data
// Partial sum BRAMs should be enough to fit first 32 columns of op matrix. Max config 1k*1k. So it has to accomodate 32*1k*64-bit 
// depth required is 512. so we can use 64 BRAMs each 512*64-bit 

module partial_sum_bram #(

parameter OUTPUT_DATA_WIDTH = 64,
parameter ADDR_WIDTH = 9,
parameter BRAM_DATA_WIDTH = 64,
parameter NUM_CASCADE_CHAINS = 32,
parameter NUM_BRAMS = 64 


) (

input clk,
input rst,

// Port A Signals
input [2*NUM_CASCADE_CHAINS-1:0]               bram_ena,
input [2*NUM_CASCADE_CHAINS-1:0]               bram_wea,
input [ADDR_WIDTH-1:0]                       bram_addra [2*NUM_CASCADE_CHAINS-1:0],
input [OUTPUT_DATA_WIDTH-1:0]                bram_dina [2*NUM_CASCADE_CHAINS-1:0],

// Port B Signals
input [ADDR_WIDTH-1:0]                       bram_addrb [2*NUM_CASCADE_CHAINS-1:0],
input [2*NUM_CASCADE_CHAINS-1:0]             bram_enb,
output [OUTPUT_DATA_WIDTH-1:0]               bram_doutb [2*NUM_CASCADE_CHAINS-1:0]

);

// 1K*64 partial Sum BRAM is formed by using 2 512*64 BRAMs, which are decoded using the upper address bit [9]
//wire [2*NUM_CASCADE_CHAINS-1:0] bram_ena0, bram_ena1;
//wire [2*NUM_CASCADE_CHAINS-1:0] bram_wea0, bram_wea1;
//wire [2*NUM_CASCADE_CHAINS-1:0] bram_enb0, bram_enb1;
//wire [OUTPUT_DATA_WIDTH-1:0]  bram_doutb0 [2*NUM_CASCADE_CHAINS-1:0];
//wire [OUTPUT_DATA_WIDTH-1:0]  bram_doutb1 [2*NUM_CASCADE_CHAINS-1:0];

//reg [ADDR_WIDTH-1:0]                       bram_addrb_d  [2*NUM_CASCADE_CHAINS-1:0];
//reg [ADDR_WIDTH-1:0]                       bram_addrb_d1 [2*NUM_CASCADE_CHAINS-1:0];
//reg [ADDR_WIDTH-1:0]                       bram_addrb_d2 [2*NUM_CASCADE_CHAINS-1:0];
genvar j;
//for (j=0; j<2*NUM_CASCADE_CHAINS; j=j+1) begin
//   always @(posedge clk) begin
//      if (rst) begin
//          bram_addrb_d[j] <= 'b0;
//      end
//      else begin
//          bram_addrb_d[j]  <= bram_addrb[j];
//	  bram_addrb_d1[j] <= bram_addrb_d[j];
//          bram_addrb_d2[j] <= bram_addrb_d1[j];
//      end
//   end   
//end



genvar b;

//for (b=0; b<2*NUM_CASCADE_CHAINS; b=b+1) begin
//   assign bram_ena0[b] = bram_ena[b] & ~bram_addra[b][9]; 
//   assign bram_ena1[b] = bram_ena[b] &  bram_addra[b][9]; 
//   assign bram_wea0[b] = bram_wea[b] & ~bram_addra[b][9]; 
//   assign bram_wea1[b] = bram_wea[b] &  bram_addra[b][9]; 
//   assign bram_enb0[b] = bram_enb[b] & ~bram_addrb[b][9]; 
//   assign bram_enb1[b] = bram_enb[b] &  bram_addrb[b][9]; 
//   //assign bram_doutb[b] = bram_addrb[b][9] ? bram_doutb1[b] : bram_doutb0[b];
//   assign bram_doutb[b] = bram_addrb_d2[b][9] ? bram_doutb1[b] : bram_doutb0[b];
//end

// Instantiate 32 BRAMs (Single Port)
// Port A - Write access
// Port B - Read access

for (b=0; b<2*NUM_CASCADE_CHAINS; b=b+1) begin : ps_bram_loop

xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (9), // DECIMAL
       .ADDR_WIDTH_B             (9), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (64), // DECIMAL
       .CASCADE_HEIGHT           (1), // DECIMAL
       .CLOCKING_MODE            ("common_clock"), // String
       .ECC_MODE                 ("no_ecc"), // String
       .MEMORY_INIT_FILE         ("NONE"),
       .MEMORY_INIT_PARAM        ("0"), // String
       .MEMORY_OPTIMIZATION      ("true"), // String
       .MEMORY_PRIMITIVE         ("block"), // String
       .MEMORY_SIZE              (32768), // DECIMAL
       .MESSAGE_CONTROL          (0), // DECIMAL
       .READ_DATA_WIDTH_B        (64), // DECIMAL
       .READ_LATENCY_B           (3), // DECIMAL
       .READ_RESET_VALUE_B       ("0"), // String
       .RST_MODE_A               ("SYNC"), // String
       .RST_MODE_B               ("SYNC"), // String
       .SIM_ASSERT_CHK           (0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
       .USE_EMBEDDED_CONSTRAINT  (0), // DECIMAL
       .USE_MEM_INIT             (1), // DECIMAL
       .WAKEUP_TIME              ("disable_sleep"), // String
       .WRITE_DATA_WIDTH_A       (64), // DECIMAL
       .WRITE_MODE_B             ("no_change") // String
      ) xpm_memory_sdpram_inst0 (
       .dbiterrb                 (),
       .doutb                    (bram_doutb[b]), 
       .sbiterrb                 (),
       .addra                    (bram_addra[b]),    
       .addrb                    (bram_addrb[b]), 
       .clka                     (clk),
       .clkb                     (clk),
       .dina                     (bram_dina[b]),
       .ena                      (bram_ena[b]),     // common to all BRAMs
       .enb                      (bram_enb[b]),     // common to all BRAMs
       .injectdbiterra           (1'b0),
       .injectsbiterra           (1'b0),
       .regceb                   (1'b1),
       .rstb                     (rst),
       .sleep                    (1'b0),
       .wea                      (bram_wea[b])     // common to all BRAMs
      );

end      

endmodule     

