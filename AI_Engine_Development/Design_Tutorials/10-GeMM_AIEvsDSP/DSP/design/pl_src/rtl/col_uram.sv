/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// Col URAM stores entire matrix B in transposed format 

//  64 Single-ported URAMs in 4k*64-bit configuration (No ECC) are used for this storage
//  4 sets of 16 URAMs - 2 rows/columns (2* 32 int16 elements) can be fit at one addr location for 16 URAMs
module col_uram #(

parameter URAM_DATA_WIDTH = 1024,
parameter URAM_ADDR_WIDTH = 12,
parameter NUM_URAM = 64

) (

input                       clk,
input                       rst,

// Port A Signals
input [NUM_URAM-1:0]           uram_ena,   
input [NUM_URAM-1:0]           uram_wea,
input [URAM_ADDR_WIDTH-1:0]    uram_addra,
input [63:0]                   uram_dina,

// Port B Signals
input [3:0]                    uram_enb,    // 16 URAMs are read simultaneously
input [URAM_ADDR_WIDTH-1:0]    uram_addrb,
output [63:0]                  uram_doutb[15:0] 

);

genvar b;
reg [3:0]uram_enb_d ;
reg [3:0]uram_enb_d1;
reg [3:0]uram_enb_d2;
// Col URAMs are initialized from mem files 
localparam [10*8-1:0] mem_init_file00[0:63] = {"col_init00", "col_init01", "col_init02", "col_init03",
                                               "col_init04", "col_init05", "col_init06", "col_init07",
					       "col_init08", "col_init09", "col_init10", "col_init11",
                                               "col_init12", "col_init13", "col_init14", "col_init15",
					       "col_init16", "col_init17", "col_init18", "col_init19",
					       "col_init20", "col_init21", "col_init22", "col_init23",
					       "col_init24", "col_init25", "col_init26", "col_init27",
					       "col_init28", "col_init29", "col_init30", "col_init31",
					       "col_init32", "col_init33", "col_init34", "col_init35",
					       "col_init36", "col_init37", "col_init38", "col_init39",
					       "col_init40", "col_init41", "col_init42", "col_init43",
					       "col_init44", "col_init45", "col_init46", "col_init47",
					       "col_init48", "col_init49", "col_init50", "col_init51",
					       "col_init52", "col_init53", "col_init54", "col_init55",
					       "col_init56", "col_init57", "col_init58", "col_init59",
					       "col_init60", "col_init61", "col_init62", "col_init63" };

// Since 8 URAMs are read simultaneously, one uram_enb will drive 8 URAMs
wire [63:0] uram_enb_vec;

assign uram_enb_vec = { {16{uram_enb[3]}},  {16{uram_enb[2]}}, {16{uram_enb[1]}}, {16{uram_enb[0]}} };		 

wire [63:0] uram_doutb_vec[63:0];

//wire [1023:0] uram_doutb0;
//wire [1023:0] uram_doutb1;
//wire [1023:0] uram_doutb2;
//wire [1023:0] uram_doutb3;

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

//assign uram_doutb1={ };
//assign uram_doutb2={ };
//assign uram_doutb3={ };
//
//assign uram_doutb =  {1024{uram_enb_d2[0]}} & uram_doutb0 
//                   | {1024{uram_enb_d2[1]}} & uram_doutb1
//                   | {1024{uram_enb_d2[2]}} & uram_doutb2
//                   | {1024{uram_enb_d2[3]}} & uram_doutb3;

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

// Instantiate 64 URAMs (Dual Port)
// Port A - Write access
// Port B - Read access
//

for (b=0; b<NUM_URAM; b=b+1) begin : col_uram_loop
//4K*64-bit 
xpm_memory_sdpram #(
       .ADDR_WIDTH_A             (12), // DECIMAL
       .ADDR_WIDTH_B             (12), // DECIMAL
       .AUTO_SLEEP_TIME          (0), // DECIMAL
       .BYTE_WRITE_WIDTH_A       (64), // DECIMAL
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
      ) col_uram_inst (
       .dbiterrb                 (),
       .doutb                    (uram_doutb_vec[b]),
       .sbiterrb                 (),
       .addra                    (uram_addra),    // All written to same address simultaneously
       .addrb                    (uram_addrb),
       .clka                     (clk),
       .clkb                     (clk),
       .dina                     (uram_dina),
       .ena                      (uram_ena[b]),     
       .enb                      (uram_enb_vec[b]),     // common to all BRAMs
       .injectdbiterra           (1'b0),
       .injectsbiterra           (1'b0),
       .regceb                   (1'b1),
       .rstb                     (rst),
       .sleep                    (1'b0),
       .wea                      (uram_wea[b])     
      );

end

 endmodule     

