/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

//  row of 32*32 int16 elements mean data width is 512 bits
//  64 Single-ported URAMs in 4k*64-bit configuration (No ECC) are used for this storage
//  4 sets of 16 URAMs - 2 rows/columns (2* 32 int16 elements) can be fit at one addr location for 16 URAMs 
module row_uram #(

parameter ADDR_WIDTH   = 12,
parameter NUM_URAM = 64 

) (

input                       clk,
input                       rst,

// Port A Signals
input [63:0]                uram_ena,   
input [63:0]                uram_wea,   
input [ADDR_WIDTH-1:0]      uram_addra, 
input [63:0]                uram_dina,  

// Port B Signals
input [ADDR_WIDTH-1:0]       uram_addrb ,
input [3:0]                  uram_enb   ,
output [63:0]                uram_doutb [15:0]

);


localparam [15*8-1:0] mem_init_file000[0:63]= {"row_init_00", "row_init_01", "row_init_02", "row_init_03",
                                               "row_init_04", "row_init_05", "row_init_06", "row_init_07",
                                               "row_init_08", "row_init_09", "row_init_10", "row_init_11",
                                               "row_init_12", "row_init_13", "row_init_14", "row_init_15",
                                               "row_init_16", "row_init_17", "row_init_18", "row_init_19",
                                               "row_init_20", "row_init_21", "row_init_22", "row_init_23",
                                               "row_init_24", "row_init_25", "row_init_26", "row_init_27",
                                               "row_init_28", "row_init_29", "row_init_30", "row_init_31",
                                               "row_init_32", "row_init_33", "row_init_34", "row_init_35",
                                               "row_init_36", "row_init_37", "row_init_38", "row_init_39",
                                               "row_init_40", "row_init_41", "row_init_42", "row_init_43",
                                               "row_init_44", "row_init_45", "row_init_46", "row_init_47",
                                               "row_init_48", "row_init_49", "row_init_50", "row_init_51",
                                               "row_init_52", "row_init_53", "row_init_54", "row_init_55",
                                               "row_init_56", "row_init_57", "row_init_58", "row_init_59",
                                               "row_init_60", "row_init_61", "row_init_62", "row_init_63"
					       };

genvar b;
reg [3:0]uram_enb_d ;
reg [3:0]uram_enb_d1;
reg [3:0]uram_enb_d2;
wire [63:0] uram_doutb_vec[63:0];

assign uram_doutb[15]=  ({64{uram_enb_d2[3]}} & uram_doutb_vec[63]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[47]) | 
                        ({64{uram_enb_d2[1]}} & uram_doutb_vec[31]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[15]) ;

assign uram_doutb[14]=  ({64{uram_enb_d2[3]}} & uram_doutb_vec[62]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[46]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[30]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[14]) ;

assign uram_doutb[13]=  ({64{uram_enb_d2[3]}} & uram_doutb_vec[61]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[45]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[29]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[13]) ;

assign uram_doutb[12]=  ({64{uram_enb_d2[3]}} & uram_doutb_vec[60]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[44]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[28]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[12]) ;

assign uram_doutb[11]=  ({64{uram_enb_d2[3]}} & uram_doutb_vec[59]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[43]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[27]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[11]) ;

assign uram_doutb[10]=  ({64{uram_enb_d2[3]}} & uram_doutb_vec[58]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[42]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[26]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[10]) ;

assign uram_doutb[9] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[57]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[41]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[25]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[9] ) ; 

assign uram_doutb[8] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[56]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[40]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[24]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[8] ) ; 

assign uram_doutb[7] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[55]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[39]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[23]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[7] ) ; 

assign uram_doutb[6] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[54]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[38]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[22]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[6] ) ; 

assign uram_doutb[5] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[53]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[37]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[21]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[5] ) ; 

assign uram_doutb[4] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[52]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[36]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[20]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[4] ) ; 

assign uram_doutb[3] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[51]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[35]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[19]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[3] ) ; 

assign uram_doutb[2] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[50]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[34]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[18]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[2] ) ; 

assign uram_doutb[1] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[49]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[33]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[17]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[1] ) ; 

assign uram_doutb[0] =  ({64{uram_enb_d2[3]}} & uram_doutb_vec[48]) | ({64{uram_enb_d2[2]}} & uram_doutb_vec[32]) | 
		        ({64{uram_enb_d2[1]}} & uram_doutb_vec[16]) | ({64{uram_enb_d2[0]}} & uram_doutb_vec[0] ) ;

always @(posedge clk) begin
   if (rst) begin
      //uram_enb_d  <= '{default:'b0};
      //uram_enb_d1 <= '{default:'b0};
      //uram_enb_d2 <= '{default:'b0};
      uram_enb_d  <= 'b0;
      uram_enb_d1 <= 'b0;
      uram_enb_d2 <= 'b0;
   end
   else begin 
      uram_enb_d   <= uram_enb; 
      uram_enb_d1  <= uram_enb_d; 
      uram_enb_d2  <= uram_enb_d1; 
   end
end 
wire [63:0] uram_enb_vec;

assign uram_enb_vec = { {16{uram_enb[3]}},  {16{uram_enb[2]}}, {16{uram_enb[1]}}, {16{uram_enb[0]}} };		 


// Instantiate 8 URAMs (Single Port)
// Port A - Write access
// Port B - Read access
//

for (b=0; b<NUM_URAM; b=b+1) begin : row_uram_loop

xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (12), // DECIMAL
       .ADDR_WIDTH_B             (12), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (64), // DECIMAL
       .CASCADE_HEIGHT           (0), // DECIMAL
       .CLOCKING_MODE            ("common_clock"), // String
       .ECC_MODE                 ("no_ecc"), // String
       //.MEMORY_INIT_FILE         ({mem_init_file000[b], ".mem"}),
       .MEMORY_INIT_FILE         ("NONE"),
       .MEMORY_INIT_PARAM        ("0"), // String
       .MEMORY_OPTIMIZATION      ("true"), // String
       .MEMORY_PRIMITIVE         ("ultra"), // String
       .MEMORY_SIZE              (262144), // DECIMAL
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
       .WRITE_MODE_B             ("read_first") // String
      ) row_uram_inst (
       .dbiterrb                 (),
       .doutb                    (uram_doutb_vec[b]),
       .sbiterrb                 (),
       .addra                    (uram_addra),    
       .addrb                    (uram_addrb),   // CHECK if correct
       .clka                     (clk),
       .clkb                     (clk),
       .dina                     (uram_dina),
       .ena                      (uram_ena[b]),        // common to all BRAMs    
       .enb                      (uram_enb_vec[b]),  // common to all BRAMs
       .injectdbiterra           (1'b0),
       .injectsbiterra           (1'b0),
       .regceb                   (1'b1),
       .rstb                     (rst),
       .sleep                    (1'b0),
       .wea                      (uram_wea[b])     // common to all BRAMs     
      );
      
end     

endmodule     

