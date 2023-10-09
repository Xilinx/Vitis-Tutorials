/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// 32 output BRAMs in 2K*16-bit configuration store the column of final output matrix
// This column should be read out from DUT before next column is written 

module op_bram #(

parameter OUTPUT_DATA_WIDTH = 16,
parameter OUTPUT_ADDR_WIDTH = 11,
parameter NUM_CASCADE_CHAINS = 32,
parameter NUM_BRAMS = 32

) (

input clk,
input rst,

// Port A Signals
input [NUM_CASCADE_CHAINS-1:0]               bram_ena,
input [NUM_CASCADE_CHAINS-1:0]               bram_wea,
input [OUTPUT_ADDR_WIDTH-1:0]                bram_addra [NUM_CASCADE_CHAINS-1:0],
input [OUTPUT_DATA_WIDTH-1:0]                bram_dina [NUM_CASCADE_CHAINS-1:0],

// Port B Signals
input [OUTPUT_ADDR_WIDTH-1:0]                bram_addrb,
input [NUM_CASCADE_CHAINS-1:0]               bram_enb,
output wire [511:0]                          bram_doutb 

);

genvar b;

// Instantiate 32 BRAMs (Single Port)
// Port A - Write access
// Port B - Read access

wire [OUTPUT_DATA_WIDTH-1:0] bram_doutb_vec [NUM_CASCADE_CHAINS-1:0];


for (b=0; b<NUM_CASCADE_CHAINS; b=b+1) begin
  assign bram_doutb [(16*(b+1))-1 : 16*b] = bram_doutb_vec [b]; 
end

for (b=0; b<NUM_CASCADE_CHAINS; b=b+1) begin : op_bram_loop

xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (11), // DECIMAL
       .ADDR_WIDTH_B             (11), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (16), // DECIMAL
       .CASCADE_HEIGHT           (1), // DECIMAL
       .CLOCKING_MODE            ("common_clock"), // String
       .ECC_MODE                 ("no_ecc"), // String
       .MEMORY_INIT_FILE         ("NONE"),
       .MEMORY_INIT_PARAM        ("0"), // String
       .MEMORY_OPTIMIZATION      ("true"), // String
       .MEMORY_PRIMITIVE         ("block"), // String
       .MEMORY_SIZE              (32768), // DECIMAL
       .MESSAGE_CONTROL          (0), // DECIMAL
       .READ_DATA_WIDTH_B        (16), // DECIMAL
       .READ_LATENCY_B           (3), // DECIMAL
       .READ_RESET_VALUE_B       ("0"), // String
       .RST_MODE_A               ("SYNC"), // String
       .RST_MODE_B               ("SYNC"), // String
       .SIM_ASSERT_CHK           (0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
       .USE_EMBEDDED_CONSTRAINT  (0), // DECIMAL
       .USE_MEM_INIT             (1), // DECIMAL
       .WAKEUP_TIME              ("disable_sleep"), // String
       .WRITE_DATA_WIDTH_A       (16), // DECIMAL
       .WRITE_MODE_B             ("no_change") // String
      ) xpm_memory_sdpram_inst (
       .dbiterrb                 (),
       .doutb                    (bram_doutb_vec[b]), 
       .sbiterrb                 (),
       .addra                    (bram_addra[b]),    
       .addrb                    (bram_addrb),   // Common to all BRAMs 
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

