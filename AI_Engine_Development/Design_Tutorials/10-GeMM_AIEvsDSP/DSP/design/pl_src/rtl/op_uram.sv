/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// Output URAM stores entire output matrix 

module op_uram #(

parameter URAM_ADDR_WIDTH = 14,
parameter NUM_URAM = 64,
parameter NUM_CASCADE_CHAINS = 32

) (

input                          clk,
input                          rst,

// Port A Signals
input [63:0]                   uram_ena,   
input [63:0]                   uram_wea,
input [13:0]                   uram_addra[2*NUM_CASCADE_CHAINS-1:0],
input [15:0]                   uram_dina[2*NUM_CASCADE_CHAINS-1:0],

// Port B Signals
input [63:0]                   uram_enb,    
input [63:0]                   uram_doutb_valid,    
input [13:0]                   uram_addrb,
output [15:0]                  uram_doutb 

);

genvar b;

wire [15:0] uram_dina_vec [63:0];

//assign uram_dina_vec[0]  = uram_dina[15:0];
//assign uram_dina_vec[1]  = uram_dina[31:16];
//assign uram_dina_vec[2]  = uram_dina[47:32];
//assign uram_dina_vec[3]  = uram_dina[63:48];
//assign uram_dina_vec[4]  = uram_dina[79:64];
//assign uram_dina_vec[5]  = uram_dina[95:80];
//assign uram_dina_vec[6]  = uram_dina[111:96];
//assign uram_dina_vec[7]  = uram_dina[127:112];
//assign uram_dina_vec[8]  = uram_dina[143:128];
//assign uram_dina_vec[9]  = uram_dina[159:144];
//assign uram_dina_vec[10] = uram_dina[175:160];
//assign uram_dina_vec[11] = uram_dina[191:176];
//assign uram_dina_vec[12] = uram_dina[207:192];
//assign uram_dina_vec[13] = uram_dina[223:208];
//assign uram_dina_vec[14] = uram_dina[239:224];
//assign uram_dina_vec[15] = uram_dina[255:240];
//assign uram_dina_vec[16] = uram_dina[271:256];
//assign uram_dina_vec[17] = uram_dina[287:272];
//assign uram_dina_vec[18] = uram_dina[303:288];
//assign uram_dina_vec[19] = uram_dina[319:304];
//assign uram_dina_vec[20] = uram_dina[335:320];
//assign uram_dina_vec[21] = uram_dina[351:336];
//assign uram_dina_vec[22] = uram_dina[367:352];
//assign uram_dina_vec[23] = uram_dina[383:368];
//assign uram_dina_vec[24] = uram_dina[399:384];
//assign uram_dina_vec[25] = uram_dina[415:400];
//assign uram_dina_vec[26] = uram_dina[431:416];
//assign uram_dina_vec[27] = uram_dina[447:432];
//assign uram_dina_vec[28] = uram_dina[463:448];
//assign uram_dina_vec[29] = uram_dina[479:464];
//assign uram_dina_vec[30] = uram_dina[495:480];
//assign uram_dina_vec[31] = uram_dina[511:496];
//assign uram_dina_vec[32] = uram_dina[15:0];
//assign uram_dina_vec[33] = uram_dina[31:16];
//assign uram_dina_vec[34] = uram_dina[47:32];
//assign uram_dina_vec[35] = uram_dina[63:48];
//assign uram_dina_vec[36] = uram_dina[79:64];
//assign uram_dina_vec[37] = uram_dina[95:80];
//assign uram_dina_vec[38] = uram_dina[111:96];
//assign uram_dina_vec[39] = uram_dina[127:112];
//assign uram_dina_vec[40] = uram_dina[143:128];
//assign uram_dina_vec[41] = uram_dina[159:144];
//assign uram_dina_vec[42] = uram_dina[175:160];
//assign uram_dina_vec[43] = uram_dina[191:176];
//assign uram_dina_vec[44] = uram_dina[207:192];
//assign uram_dina_vec[45] = uram_dina[223:208];
//assign uram_dina_vec[46] = uram_dina[239:224];
//assign uram_dina_vec[47] = uram_dina[255:240];
//assign uram_dina_vec[48] = uram_dina[271:256];
//assign uram_dina_vec[49] = uram_dina[287:272];
//assign uram_dina_vec[50] = uram_dina[303:288];
//assign uram_dina_vec[51] = uram_dina[319:304];
//assign uram_dina_vec[52] = uram_dina[335:320];
//assign uram_dina_vec[53] = uram_dina[351:336];
//assign uram_dina_vec[54] = uram_dina[367:352];
//assign uram_dina_vec[55] = uram_dina[383:368];
//assign uram_dina_vec[56] = uram_dina[399:384];
//assign uram_dina_vec[57] = uram_dina[415:400];
//assign uram_dina_vec[58] = uram_dina[431:416];
//assign uram_dina_vec[59] = uram_dina[447:432];
//assign uram_dina_vec[60] = uram_dina[463:448];
//assign uram_dina_vec[61] = uram_dina[479:464];
//assign uram_dina_vec[62] = uram_dina[495:480];
//assign uram_dina_vec[63] = uram_dina[511:496];

wire [15:0] uram_doutb_vec [63:0];

reg [63:0] uram_enb_d;
reg [63:0] uram_enb_d2;
reg [63:0] uram_enb_d3;

always @(posedge clk) begin
  if (rst) begin
       uram_enb_d  <= 64'b0;
       uram_enb_d2 <= 64'b0;
       uram_enb_d3 <= 64'b0;
  end
  else begin
       uram_enb_d  <= uram_enb;
       uram_enb_d2 <= uram_enb_d;
       uram_enb_d3 <= uram_enb_d2;
  end
end  

assign uram_doutb =   {16{uram_doutb_valid[0]}} & uram_doutb_vec[0] 
                    | {16{uram_doutb_valid[1]}} & uram_doutb_vec[1]
                    | {16{uram_doutb_valid[2]}} & uram_doutb_vec[2]
                    | {16{uram_doutb_valid[3]}} & uram_doutb_vec[3]
                    | {16{uram_doutb_valid[4]}} & uram_doutb_vec[4]
                    | {16{uram_doutb_valid[5]}} & uram_doutb_vec[5]
                    | {16{uram_doutb_valid[6]}} & uram_doutb_vec[6]
                    | {16{uram_doutb_valid[7]}} & uram_doutb_vec[7]
                    | {16{uram_doutb_valid[8]}} & uram_doutb_vec[8]
                    | {16{uram_doutb_valid[9]}} & uram_doutb_vec[9]
                    | {16{uram_doutb_valid[10]}} & uram_doutb_vec[10]
                    | {16{uram_doutb_valid[11]}} & uram_doutb_vec[11]
                    | {16{uram_doutb_valid[12]}} & uram_doutb_vec[12]
                    | {16{uram_doutb_valid[13]}} & uram_doutb_vec[13]
                    | {16{uram_doutb_valid[14]}} & uram_doutb_vec[14]
                    | {16{uram_doutb_valid[15]}} & uram_doutb_vec[15]
                    | {16{uram_doutb_valid[16]}} & uram_doutb_vec[16]
                    | {16{uram_doutb_valid[17]}} & uram_doutb_vec[17]
                    | {16{uram_doutb_valid[18]}} & uram_doutb_vec[18]
                    | {16{uram_doutb_valid[19]}} & uram_doutb_vec[19]
                    | {16{uram_doutb_valid[20]}} & uram_doutb_vec[20]
                    | {16{uram_doutb_valid[21]}} & uram_doutb_vec[21]
                    | {16{uram_doutb_valid[22]}} & uram_doutb_vec[22]
                    | {16{uram_doutb_valid[23]}} & uram_doutb_vec[23]
                    | {16{uram_doutb_valid[24]}} & uram_doutb_vec[24]
                    | {16{uram_doutb_valid[25]}} & uram_doutb_vec[25]
                    | {16{uram_doutb_valid[26]}} & uram_doutb_vec[26]
                    | {16{uram_doutb_valid[27]}} & uram_doutb_vec[27]
                    | {16{uram_doutb_valid[28]}} & uram_doutb_vec[28]
                    | {16{uram_doutb_valid[29]}} & uram_doutb_vec[29]
                    | {16{uram_doutb_valid[30]}} & uram_doutb_vec[30]
                    | {16{uram_doutb_valid[31]}} & uram_doutb_vec[31]
                    | {16{uram_doutb_valid[32]}} & uram_doutb_vec[32]
                    | {16{uram_doutb_valid[33]}} & uram_doutb_vec[33]
                    | {16{uram_doutb_valid[34]}} & uram_doutb_vec[34]
                    | {16{uram_doutb_valid[35]}} & uram_doutb_vec[35]
                    | {16{uram_doutb_valid[36]}} & uram_doutb_vec[36]
                    | {16{uram_doutb_valid[37]}} & uram_doutb_vec[37]
                    | {16{uram_doutb_valid[38]}} & uram_doutb_vec[38]
                    | {16{uram_doutb_valid[39]}} & uram_doutb_vec[39]
                    | {16{uram_doutb_valid[40]}} & uram_doutb_vec[40]
                    | {16{uram_doutb_valid[41]}} & uram_doutb_vec[41]
                    | {16{uram_doutb_valid[42]}} & uram_doutb_vec[42]
                    | {16{uram_doutb_valid[43]}} & uram_doutb_vec[43]
                    | {16{uram_doutb_valid[44]}} & uram_doutb_vec[44]
                    | {16{uram_doutb_valid[45]}} & uram_doutb_vec[45]
                    | {16{uram_doutb_valid[46]}} & uram_doutb_vec[46]
                    | {16{uram_doutb_valid[47]}} & uram_doutb_vec[47]
                    | {16{uram_doutb_valid[48]}} & uram_doutb_vec[48]
                    | {16{uram_doutb_valid[49]}} & uram_doutb_vec[49]
                    | {16{uram_doutb_valid[50]}} & uram_doutb_vec[50]
                    | {16{uram_doutb_valid[51]}} & uram_doutb_vec[51]
                    | {16{uram_doutb_valid[52]}} & uram_doutb_vec[52]
                    | {16{uram_doutb_valid[53]}} & uram_doutb_vec[53]
                    | {16{uram_doutb_valid[54]}} & uram_doutb_vec[54]
                    | {16{uram_doutb_valid[55]}} & uram_doutb_vec[55]
                    | {16{uram_doutb_valid[56]}} & uram_doutb_vec[56]
                    | {16{uram_doutb_valid[57]}} & uram_doutb_vec[57]
                    | {16{uram_doutb_valid[58]}} & uram_doutb_vec[58]
                    | {16{uram_doutb_valid[59]}} & uram_doutb_vec[59]
                    | {16{uram_doutb_valid[60]}} & uram_doutb_vec[60]
                    | {16{uram_doutb_valid[61]}} & uram_doutb_vec[61]
                    | {16{uram_doutb_valid[62]}} & uram_doutb_vec[62]
                    | {16{uram_doutb_valid[63]}} & uram_doutb_vec[63];
/*assign uram_doutb =   {16{uram_enb_d3[0]}} & uram_doutb_vec[0] 
                    | {16{uram_enb_d3[1]}} & uram_doutb_vec[1]
                    | {16{uram_enb_d3[2]}} & uram_doutb_vec[2]
                    | {16{uram_enb_d3[3]}} & uram_doutb_vec[3]
                    | {16{uram_enb_d3[4]}} & uram_doutb_vec[4]
                    | {16{uram_enb_d3[5]}} & uram_doutb_vec[5]
                    | {16{uram_enb_d3[6]}} & uram_doutb_vec[6]
                    | {16{uram_enb_d3[7]}} & uram_doutb_vec[7]
                    | {16{uram_enb_d3[8]}} & uram_doutb_vec[8]
                    | {16{uram_enb_d3[9]}} & uram_doutb_vec[9]
                    | {16{uram_enb_d3[10]}} & uram_doutb_vec[10]
                    | {16{uram_enb_d3[11]}} & uram_doutb_vec[11]
                    | {16{uram_enb_d3[12]}} & uram_doutb_vec[12]
                    | {16{uram_enb_d3[13]}} & uram_doutb_vec[13]
                    | {16{uram_enb_d3[14]}} & uram_doutb_vec[14]
                    | {16{uram_enb_d3[15]}} & uram_doutb_vec[15]
                    | {16{uram_enb_d3[16]}} & uram_doutb_vec[16]
                    | {16{uram_enb_d3[17]}} & uram_doutb_vec[17]
                    | {16{uram_enb_d3[18]}} & uram_doutb_vec[18]
                    | {16{uram_enb_d3[19]}} & uram_doutb_vec[19]
                    | {16{uram_enb_d3[20]}} & uram_doutb_vec[20]
                    | {16{uram_enb_d3[21]}} & uram_doutb_vec[21]
                    | {16{uram_enb_d3[22]}} & uram_doutb_vec[22]
                    | {16{uram_enb_d3[23]}} & uram_doutb_vec[23]
                    | {16{uram_enb_d3[24]}} & uram_doutb_vec[24]
                    | {16{uram_enb_d3[25]}} & uram_doutb_vec[25]
                    | {16{uram_enb_d3[26]}} & uram_doutb_vec[26]
                    | {16{uram_enb_d3[27]}} & uram_doutb_vec[27]
                    | {16{uram_enb_d3[28]}} & uram_doutb_vec[28]
                    | {16{uram_enb_d3[29]}} & uram_doutb_vec[29]
                    | {16{uram_enb_d3[30]}} & uram_doutb_vec[30]
                    | {16{uram_enb_d3[31]}} & uram_doutb_vec[31]
                    | {16{uram_enb_d3[32]}} & uram_doutb_vec[32]
                    | {16{uram_enb_d3[33]}} & uram_doutb_vec[33]
                    | {16{uram_enb_d3[34]}} & uram_doutb_vec[34]
                    | {16{uram_enb_d3[35]}} & uram_doutb_vec[35]
                    | {16{uram_enb_d3[36]}} & uram_doutb_vec[36]
                    | {16{uram_enb_d3[37]}} & uram_doutb_vec[37]
                    | {16{uram_enb_d3[38]}} & uram_doutb_vec[38]
                    | {16{uram_enb_d3[39]}} & uram_doutb_vec[39]
                    | {16{uram_enb_d3[40]}} & uram_doutb_vec[40]
                    | {16{uram_enb_d3[41]}} & uram_doutb_vec[41]
                    | {16{uram_enb_d3[42]}} & uram_doutb_vec[42]
                    | {16{uram_enb_d3[43]}} & uram_doutb_vec[43]
                    | {16{uram_enb_d3[44]}} & uram_doutb_vec[44]
                    | {16{uram_enb_d3[45]}} & uram_doutb_vec[45]
                    | {16{uram_enb_d3[46]}} & uram_doutb_vec[46]
                    | {16{uram_enb_d3[47]}} & uram_doutb_vec[47]
                    | {16{uram_enb_d3[48]}} & uram_doutb_vec[48]
                    | {16{uram_enb_d3[49]}} & uram_doutb_vec[49]
                    | {16{uram_enb_d3[50]}} & uram_doutb_vec[50]
                    | {16{uram_enb_d3[51]}} & uram_doutb_vec[51]
                    | {16{uram_enb_d3[52]}} & uram_doutb_vec[52]
                    | {16{uram_enb_d3[53]}} & uram_doutb_vec[53]
                    | {16{uram_enb_d3[54]}} & uram_doutb_vec[54]
                    | {16{uram_enb_d3[55]}} & uram_doutb_vec[55]
                    | {16{uram_enb_d3[56]}} & uram_doutb_vec[56]
                    | {16{uram_enb_d3[57]}} & uram_doutb_vec[57]
                    | {16{uram_enb_d3[58]}} & uram_doutb_vec[58]
                    | {16{uram_enb_d3[59]}} & uram_doutb_vec[59]
                    | {16{uram_enb_d3[60]}} & uram_doutb_vec[60]
                    | {16{uram_enb_d3[61]}} & uram_doutb_vec[61]
                    | {16{uram_enb_d3[62]}} & uram_doutb_vec[62]
                    | {16{uram_enb_d3[63]}} & uram_doutb_vec[63];*/

// Instantiate 64 URAMs (Dual Port)
// Port A - Write access
// Port B - Read access
//

for (b=0; b<NUM_URAM; b=b+1) begin : op_uram_loop
//4K*64-bit 
xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (14), // DECIMAL
       .ADDR_WIDTH_B             (14), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (16), // DECIMAL
       .CASCADE_HEIGHT           (1), // DECIMAL
       .CLOCKING_MODE            ("common_clock"), // String
       .ECC_MODE                 ("no_ecc"), // String
       //.MEMORY_INIT_FILE         ({mem_init_file00[b], ".mem"}),
       .MEMORY_INIT_FILE         ("NONE"),
       .MEMORY_INIT_PARAM        ("0"), // String
       .MEMORY_OPTIMIZATION      ("true"), // String
       .MEMORY_PRIMITIVE         ("ultra"), // String
       .MEMORY_SIZE              (262144), // DECIMAL
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
       .WRITE_MODE_B             ("read_first") // String
      ) op_uram_inst (
       .dbiterrb                 (),
       .doutb                    (uram_doutb_vec[b]),
       .sbiterrb                 (),
       .addra                    (uram_addra[b]),   
       .addrb                    (uram_addrb),
       .clka                     (clk),
       .clkb                     (clk),
       .dina                     (uram_dina[b]),
       .ena                      (uram_ena[b]),     
       .enb                      (uram_enb[b]),     
       .injectdbiterra           (1'b0),
       .injectsbiterra           (1'b0),
       .regceb                   (1'b1),
       .rstb                     (rst),
       .sleep                    (1'b0),
       .wea                      (uram_wea[b])     
      );

end


 endmodule     

