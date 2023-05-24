/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// Col BRAM stores matrix B in transposed format (So that one column is written/read at one address)
// In DSP58 design, 32 columns of int16 are read at a time, making data width  512-bits
// 8 Single-ported BRAMs in 512*64-bit configuration (No ECC) are used for this storage
// Number of columns of the matrix equals the number of locations in the BRAM
// to store one matrix
// We have 2 sets (ping/pong) of these BRAMs
// While DUT is reading one set, other set needs to be written from DDR (TB in current design) 

module col_bram #(

parameter BRAM_DATA_WIDTH = 64,
parameter NUM_BRAM = 8,
parameter ADDR_WIDTH = 9

) (

input                       clk,
input                       rst,

// Port A Signals
input [NUM_BRAM-1:0]          bram_ena,
input  [NUM_BRAM-1:0]         bram_wea,
input [ADDR_WIDTH-1:0]        bram_addra,
input [511:0]                 bram_dina,

// Port B Signals
input [ADDR_WIDTH-1:0]       bram_addrb,
input                        bram_enb,
output [BRAM_DATA_WIDTH-1:0] bram_doutb [NUM_BRAM-1:0],

input                        ping_pong_sel,   
input                        ping_pong_sel_d3   
);

genvar b;

/*  Col BRAMs are now written from TB, and not preloaded
localparam [10*8-1:0] mem_init_file00_ping[0:7] = {"col_init00_ping", "col_init01_ping", "col_init02_ping", "col_init03_ping",
                                                   "col_init04_ping", "col_init05_ping", "col_init06_ping", "col_init07_ping"};

localparam [10*8-1:0] mem_init_file00_pong[0:7] = {"col_init00_pong", "col_init01_pong", "col_init02_pong", "col_init03_pong",
                                                   "col_init04_pong", "col_init05_pong", "col_init06_pong", "col_init07_pong"};
*/

wire [BRAM_DATA_WIDTH-1:0] bram_dina_vec [NUM_BRAM-1:0];

assign bram_dina_vec[0] = bram_dina[63:0];
assign bram_dina_vec[1] = bram_dina[127:64];
assign bram_dina_vec[2] = bram_dina[191:128];
assign bram_dina_vec[3] = bram_dina[255:192];
assign bram_dina_vec[4] = bram_dina[319:256];
assign bram_dina_vec[5] = bram_dina[383:320];
assign bram_dina_vec[6] = bram_dina[447:384];
assign bram_dina_vec[7] = bram_dina[511:448];

// Decode ping/pong enables for DUT reads
wire bram_enb_ping;
wire bram_enb_pong;

assign bram_enb_ping = bram_enb &  ping_pong_sel;//1 reads from ping, 0 reads from pong - for port B
assign bram_enb_pong = bram_enb & ~ping_pong_sel; 

// Decode ping/pong enables for external writes
wire bram_ena_ping [NUM_BRAM-1:0];
wire bram_ena_pong [NUM_BRAM-1:0];
wire bram_wea_ping [NUM_BRAM-1:0];
wire bram_wea_pong [NUM_BRAM-1:0];

wire [BRAM_DATA_WIDTH-1:0] bram_doutb_ping [NUM_BRAM-1:0];
wire [BRAM_DATA_WIDTH-1:0] bram_doutb_pong [NUM_BRAM-1:0];


for (b=0; b<NUM_BRAM; b=b+1) begin
   assign bram_ena_ping[b] = bram_ena[b] &  ~ping_pong_sel; //1 fills pong, 0 fills ping for port A
   assign bram_ena_pong[b] = bram_ena[b] &   ping_pong_sel;
   assign bram_wea_ping[b] = bram_wea[b] &  ~ping_pong_sel;
   assign bram_wea_pong[b] = bram_wea[b] &   ping_pong_sel;
 end

// Instantiate 8 BRAMs (Single Port)
// Port A - Write access
// Port B - Read access
//

for (b=0; b<NUM_BRAM; b=b+1) begin : col_bram_loop
//64-bit wide 512 depth PING
xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (9), // DECIMAL
       .ADDR_WIDTH_B             (9), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (64), // DECIMAL
       .CASCADE_HEIGHT           (1), // DECIMAL
       .CLOCKING_MODE            ("common_clock"), // String
       .ECC_MODE                 ("no_ecc"), // String
       //.MEMORY_INIT_FILE         ({mem_init_file00_ping[b], ".mem"}),
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
      ) col_bram_ping_inst (
       .dbiterrb                 (),
       .doutb                    (bram_doutb_ping[b]),
       .sbiterrb                 (),
       .addra                    (bram_addra),    // All written to same address simultaneously
       .addrb                    (bram_addrb),
       .clka                     (clk),
       .clkb                     (clk),
       .dina                     (bram_dina_vec[b]),
       .ena                      (bram_ena_ping[b]),     
       .enb                      (bram_enb_ping),     // common to all BRAMs
       .injectdbiterra           (1'b0),
       .injectsbiterra           (1'b0),
       .regceb                   (1'b1),
       .rstb                     (rst),
       .sleep                    (1'b0),
       .wea                      (bram_wea_ping[b])     
      );

//64-bit wide 512 depth PONG
xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (9), // DECIMAL
       .ADDR_WIDTH_B             (9), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (64), // DECIMAL
       .CASCADE_HEIGHT           (1), // DECIMAL
       .CLOCKING_MODE            ("common_clock"), // String
       .ECC_MODE                 ("no_ecc"), // String
       //.MEMORY_INIT_FILE         ({mem_init_file00_pong[b], ".mem"}),
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
      ) col_bram_pong_inst (
       .dbiterrb                 (),
       .doutb                    (bram_doutb_pong[b]),
       .sbiterrb                 (),
       .addra                    (bram_addra),    // All written to same address simultaneously
       .addrb                    (bram_addrb),
       .clka                     (clk),
       .clkb                     (clk),
       .dina                     (bram_dina_vec[b]),
       .ena                      (bram_ena_pong[b]),     
       .enb                      (bram_enb_pong),     // common to all BRAMs
       .injectdbiterra           (1'b0),
       .injectsbiterra           (1'b0),
       .regceb                   (1'b1),
       .rstb                     (rst),
       .sleep                    (1'b0),
       .wea                      (bram_wea_pong[b])     
      );
      
 end     

 // Mux doutb from ping/pong buffers
 for (b=0; b<NUM_BRAM; b=b+1) begin
    assign bram_doutb[b] = ping_pong_sel_d3 ? bram_doutb_ping[b] : bram_doutb_pong[b];
 end   

 endmodule     

