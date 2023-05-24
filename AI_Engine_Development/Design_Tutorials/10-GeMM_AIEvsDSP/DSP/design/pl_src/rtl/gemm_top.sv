/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// This is the top level module for GeMM DSP58 implementation
// This module instantaites below modules -
// DSP_data_controller
// row_uram
// col_uram
// op_uram
// partial_sum_bram
// control_logic
// Row and Col BRAM write interfaces are brought out as IOs of the design as of now
// For simulation, we can initialize BRAMs and not exercise BRAM write interface

module gemm_top (

input         clk,
input         dsp_clk,
input         reset,

// Row URAM Write interface 
input [63:0] row_uram_ena,
input [63:0] row_uram_wea,
input [11:0]  row_uram_addra,
input [63:0]  row_uram_dina,

// Col URAM write itnerface
input [63:0]   col_uram_ena,
input [63:0]   col_uram_wea,
input [11:0]   col_uram_addra,
input [63:0]   col_uram_dina,

// OP URAM read interface
input [13:0]   op_uram_addrb,
input [63:0]   op_uram_enb,
input [63:0]   op_uram_doutb_valid,
output[15:0]   op_uram_doutb,

input             start,
output            done,
output wire       write_op_uram,
output reg [31:0] active_clk_count,

// Statistics registers
output reg [4:0]                                     num_mat_done


);

parameter NUM_CASCADE_CHAINS = 32;
parameter CASCADE_LEN = 32;
parameter A_DATA_WIDTH = 16;
parameter B_DATA_WIDTH = 16;
//parameter A_DATA_WIDTH = 27;
//parameter B_DATA_WIDTH = 24;
parameter OUTPUT_DATA_WIDTH = 64;
parameter DSP_OUTPUT_DATA_WIDTH = 40;
parameter OUTPUT_ADDR_WIDTH = 9;
parameter URAM_DATA_WIDTH = 64;
parameter M = 32;
parameter N = 32;
parameter M_LARGE = 1024;
parameter N_LARGE = 1024;
//parameter NUM_ROW_BRAMS = 4;
//parameter NUM_COL_BRAMS = 4;
parameter ADDR_WIDTH = 9;
parameter URAM_ADDR_WIDTH = 12;

// FIXGEMM generics 

//parameter N=32;
//parameter DDR=FALSE; //boolean
parameter FAMILY=3;         // use 1 for 7-series, 2 for US/US+ and 3 for Versal
parameter TRIANGULAR_DELAYS=1; // set to 0 to remove A,B,C and O triangular delays
parameter OX=-32;
parameter OY=-32;

// Internal signals

// Row BRAM Read interface driven from control logic
//wire [URAM_ADDR_WIDTH-1:0] row_uram_addrb [15:0];
wire [URAM_ADDR_WIDTH-1:0]        row_uram_addrb ;
wire [3:0]                        row_uram_enb;
wire [URAM_DATA_WIDTH-1:0]        row_uram_doutb    [15:0];

// Col URAM Read interface driven from control logic
wire [URAM_ADDR_WIDTH-1:0]        col_uram_addrb;
wire [3:0]                        col_uram_enb;
wire [URAM_DATA_WIDTH-1:0]        col_uram_doutb    [15:0];

// Partial Sum BRAM Write interface driven from control logic
wire [2*NUM_CASCADE_CHAINS-1 :0]  ps_bram_ena;
wire [2*NUM_CASCADE_CHAINS-1 :0]  ps_bram_wea;
wire [OUTPUT_ADDR_WIDTH-1    :0]  ps_bram_addra     [2*NUM_CASCADE_CHAINS-1:0];
reg  [URAM_DATA_WIDTH-1      :0]  ps_bram_dina      [2*NUM_CASCADE_CHAINS-1:0];
reg  [URAM_DATA_WIDTH-1      :0]  op_uram_dina_int  [2*NUM_CASCADE_CHAINS-1:0];
reg  [URAM_DATA_WIDTH-1      :0]  op_uram_dina_int_d[2*NUM_CASCADE_CHAINS-1:0];
reg  [2*NUM_CASCADE_CHAINS-1 :0]  ps_bram_ena_d;
reg  [2*NUM_CASCADE_CHAINS-1 :0]  ps_bram_wea_d;
reg  [OUTPUT_ADDR_WIDTH-1    :0]  ps_bram_addra_d   [2*NUM_CASCADE_CHAINS-1:0];
reg  [URAM_DATA_WIDTH-1      :0]  ps_bram_dina_d    [2*NUM_CASCADE_CHAINS-1:0];
wire [DSP_OUTPUT_DATA_WIDTH-1:0]  ps_bram_dina_int1  [2*NUM_CASCADE_CHAINS-1:0];
reg [DSP_OUTPUT_DATA_WIDTH-1:0]  ps_bram_dina_int2  [2*NUM_CASCADE_CHAINS-1:0];
reg [DSP_OUTPUT_DATA_WIDTH-1:0]  ps_bram_dina_int  [2*NUM_CASCADE_CHAINS-1:0];

// writes to output BRAM
//reg  [NUM_CASCADE_CHAINS-1:0]   op_uram_ena;
//reg  [NUM_CASCADE_CHAINS-1:0]   op_uram_wea;
//wire  [URAM_ADDR_WIDTH-1:0]     op_uram_addra [2*NUM_CASCADE_CHAINS-1:0];
wire  [2*NUM_CASCADE_CHAINS-1:0]  op_uram_ena;
wire  [2*NUM_CASCADE_CHAINS-1:0]  op_uram_wea;
wire  [13:0]                      op_uram_addra     [2*NUM_CASCADE_CHAINS-1:0];
reg   [15:0]                      op_uram_dina      [2*NUM_CASCADE_CHAINS-1:0];

wire [A_DATA_WIDTH-1:0]           mux_A_data0       [CASCADE_LEN-1:0];
wire [A_DATA_WIDTH-1:0]           mux_A_data1       [CASCADE_LEN-1:0];
wire [B_DATA_WIDTH-1:0]           mux_B_data0       [CASCADE_LEN-1:0];
wire [B_DATA_WIDTH-1:0]           mux_B_data1       [CASCADE_LEN-1:0];

wire                              mux_A_data_valid;
wire                              mux_B_data_valid;
wire                              DSP_A_data_valid;
wire                              DSP_B_data_valid;
wire                              DSP_op_data_valid;

wire [A_DATA_WIDTH-1:0]           DSP_A_data        [CASCADE_LEN-1:0];
wire [B_DATA_WIDTH-1:0]           DSP_B_data        [CASCADE_LEN-1:0];
wire [39:0]                       DSP_op_data       [CASCADE_LEN-1:0];


wire [63:0]                       ps_bram_doutb_vec [2*NUM_CASCADE_CHAINS-1:0];
wire [31:0]                       op_uram_doutb_vec [2*NUM_CASCADE_CHAINS-1:0];


wire  [2*NUM_CASCADE_CHAINS-1:0]  ps_bram_enb;
wire  [OUTPUT_ADDR_WIDTH-1:0]     ps_bram_addrb     [2*NUM_CASCADE_CHAINS-1:0];

wire                              add_ps_d2;
wire                              add_ps_d2_int1;
wire [OUTPUT_DATA_WIDTH-1:0]      partial_sum_c     [2*NUM_CASCADE_CHAINS-1:0];
reg  [OUTPUT_DATA_WIDTH-1:0]      partial_sum       [2*NUM_CASCADE_CHAINS-1:0];
//reg [OUTPUT_DATA_WIDTH-1:0]     ps_bram_dina_int_ff  [NUM_CASCADE_CHAINS-1:0];

wire ping_pong_col_bram_sel_d3;

genvar k;

////////////////////////////////////////////////////////////////
// To fix timing violations, ps_bram_dina is flopped to match
// timing wrt other control signals
////////////////////////////////////////////////////////////////

for (k=0; k<2*NUM_CASCADE_CHAINS; k=k+1) begin
   always @(posedge clk) begin
     if (reset) begin
       partial_sum[k]        <= 'b0;
       ps_bram_dina[k]       <= 'b0;
       op_uram_dina_int[k]   <= 'b0;
       op_uram_dina_int_d[k] <= 'b0;
       ps_bram_dina_int2[k]  <= 'b0;
       ps_bram_dina_int[k]   <= 'b0;
     end
     else begin
       ps_bram_dina_int2[k]  <= ps_bram_dina_int1[k];
       ps_bram_dina_int[k]   <= ps_bram_dina_int2[k];
       partial_sum[k]        <= partial_sum_c[k];
       ps_bram_dina [k]      <= add_ps_d2 & add_ps_d2_int1 ?  partial_sum[k] : {24'b0, ps_bram_dina_int[k]} ;
       op_uram_dina_int [k]  <= add_ps_d2 & ~(M_LARGE=='d32 & N_LARGE=='d32) ?  partial_sum[k] : {24'b0, ps_bram_dina_int[k]} ;
       op_uram_dina_int_d[k] <= op_uram_dina_int[k];
     end
   end
end   

for (k=0; k<2*NUM_CASCADE_CHAINS;k=k+1) begin
   assign partial_sum_c [k]  = {24'b0, ps_bram_dina_int[k]} + ps_bram_doutb_vec[k];
end   

////////////////////////////////////////////////////////////////
// To fix timing violations, op_uram signals are flopped 
////////////////////////////////////////////////////////////////

for (k=0; k<2*NUM_CASCADE_CHAINS; k=k+1) begin
   always @(posedge clk) begin
     if (reset) begin
       //op_uram_ena[k]   <= 1'b0;
       //op_uram_wea[k]   <= 1'b0; 
       //op_uram_addra[k] <= 'b0;
       op_uram_dina[k]  <= 'b0;
       
       ps_bram_ena_d [k] <= 'b0;
       ps_bram_wea_d [k] <= 'b0;
       ps_bram_addra_d[k]<= 'b0;
       ps_bram_dina_d[k] <= 'b0;
     end
     else begin
       //ps_bram_ena_d [k] <= ps_bram_ena[k];
       //ps_bram_wea_d [k] <= ps_bram_wea[k];
       //ps_bram_addra_d[k] <= ps_bram_addra[k];
       ps_bram_dina_d[k]<= ps_bram_dina[k];
       
       //op_uram_ena[k]   <= write_op_uram ? ps_bram_ena_d[k]   : 'b0;
       //op_uram_wea[k]   <= write_op_uram ? ps_bram_wea_d[k]   : 'b0;
       //op_uram_addra[k] <= write_op_uram ? {1'b0, ps_bram_addra[k]} : 'b0;
       op_uram_dina[k]  <= write_op_uram ? op_uram_dina_int[k][15:0]  : 'b0;  // Add Quantization
     end
  end   
end   


/************************************************************************/
//   Instantiations of child modules
/************************************************************************/

// Instantiate Row BRAM

row_uram #(

.ADDR_WIDTH      (URAM_ADDR_WIDTH)

) row_uram_inst (
           .clk            (clk),
           .rst            (reset),

           // Port A Signals
           .uram_ena       (row_uram_ena),
           .uram_wea       (row_uram_wea),
           .uram_addra     (row_uram_addra),
           .uram_dina      (row_uram_dina),

           // Port B Signals
           .uram_addrb     (row_uram_addrb),
           .uram_enb       (row_uram_enb),
           .uram_doutb     (row_uram_doutb)

);


control_logic #(

.M_LARGE            (M_LARGE),
.N_LARGE            (N_LARGE), 
.M                  (M),
.N                  (N), 
.NUM_CASCADE_CHAINS (NUM_CASCADE_CHAINS), 
.CASCADE_LEN        (CASCADE_LEN),
.OUTPUT_DATA_WIDTH  (OUTPUT_DATA_WIDTH), 
.ADDR_WIDTH         (ADDR_WIDTH),
.URAM_ADDR_WIDTH    (URAM_ADDR_WIDTH),
.URAM_DATA_WIDTH    (URAM_DATA_WIDTH),
.OUTPUT_ADDR_WIDTH  (OUTPUT_ADDR_WIDTH)
) control_logic_inst (

            .clk            (clk),
            .reset          (reset),

            // ROW URAM read interface
            .row_uram_addrb     (row_uram_addrb),
            .row_uram_enb       (row_uram_enb),
            .row_uram_doutb     (row_uram_doutb), 

            // Col BRAM read interface
            .col_uram_addrb     (col_uram_addrb),
            .col_uram_enb       (col_uram_enb),
            .col_uram_doutb     (col_uram_doutb), 
            
	    // mux inputs
	    .mux_A_data0        (mux_A_data0),
            .mux_A_data1        (mux_A_data1),            
            .mux_B_data0        (mux_B_data0),            
            .mux_B_data1        (mux_B_data1),            
	    .mux_A_data_valid   (mux_A_data_valid),
	    .mux_B_data_valid   (mux_B_data_valid),
	    .DSP_op_data_valid  (DSP_op_data_valid),
	    
	    // Partial Sum BRAM Write interface
            .ps_bram_ena         (ps_bram_ena),
            .ps_bram_wea         (ps_bram_wea),
            .ps_bram_addra       (ps_bram_addra),

            .op_uram_ena         (op_uram_ena),
            .op_uram_wea         (op_uram_wea),
            .op_uram_addra       (op_uram_addra),
            // Partial Sum Read interface to read back partial sum
            .ps_bram_enb         (ps_bram_enb),
            .ps_bram_addrb       (ps_bram_addrb), 

            // Control and Status signals
            .add_ps_d2              (add_ps_d2),
            .add_ps_d2_int1         (add_ps_d2_int1),
	    .write_op_uram          (write_op_uram),
	    .done                   (done),
	    .start                  (start),
	    .active_clk_count       (active_clk_count),

            // Statistics registers
            .num_mat_done            (num_mat_done)


);


partial_sum_bram #(

.OUTPUT_DATA_WIDTH   (64), 
.ADDR_WIDTH          (OUTPUT_ADDR_WIDTH), 
.BRAM_DATA_WIDTH     (URAM_DATA_WIDTH), 
.NUM_CASCADE_CHAINS  (NUM_CASCADE_CHAINS) 


) partial_sum_bram_inst (

           .clk           (clk),
           .rst           (reset),

            // Port A Signals
           .bram_ena      (ps_bram_ena),
           .bram_wea      (ps_bram_wea),
           .bram_addra    (ps_bram_addra),
           .bram_dina     (ps_bram_dina),

            // Port B Signals
           .bram_addrb    (ps_bram_addrb),
           .bram_enb      (ps_bram_enb),
           //.bram_doutb    (ps_bram_doutb)
           .bram_doutb    (ps_bram_doutb_vec)

);


// Instantiate Col URAMs
col_uram #(

.URAM_DATA_WIDTH (512),
.URAM_ADDR_WIDTH (12),
.NUM_URAM        (64)

) col_uram_inst (

               .clk         (clk),
               .rst         (reset),

               // Port A Signals
               .uram_ena    (col_uram_ena),   
               .uram_wea    (col_uram_wea),
               .uram_addra  (col_uram_addra),
               .uram_dina   (col_uram_dina),

               // Port B Signals
               .uram_enb    (col_uram_enb),    // 8 URAMs are read simultaneously
               .uram_addrb  (col_uram_addrb),
               .uram_doutb  (col_uram_doutb) 

);

// Instantiate OP URAMS
op_uram #(

.URAM_ADDR_WIDTH (14),
.NUM_URAM        (64)

) op_uram_inst (

               .clk         (clk),
               .rst         (reset),

               // Port A Signals
               .uram_ena    (op_uram_ena),   
               .uram_wea    (op_uram_wea),
               .uram_addra  (op_uram_addra),
               .uram_dina   (op_uram_dina),

               // Port B Signals
               .uram_enb    (op_uram_enb),    // 8 URAMs are read simultaneously
               .uram_addrb  (op_uram_addrb),
               .uram_doutb  (op_uram_doutb), 
               .uram_doutb_valid  (op_uram_doutb_valid) 

);
 
/******************************************************************************/
// DSP_data_controller 
/******************************************************************************/

DSP_data_controller #(

.NUM_CASCADE_CHAINS(NUM_CASCADE_CHAINS)

) DSP_data_controller_inst(
                           .clk                (dsp_clk), //DSP clk
                           .rst                (reset),
                           
                           //mux inputs, outputs
                           .A_data0            (mux_A_data0), 
                           .A_data1            (mux_A_data1),
                           .B_data0            (mux_B_data0),
                           .B_data1            (mux_B_data1),
                          
			   .mux_A_data_valid   (mux_A_data_valid),
                           .mux_B_data_valid   (mux_B_data_valid),
                           .DSP_A_data_valid   (DSP_A_data_valid),
                           .DSP_B_data_valid   (DSP_B_data_valid),
			   .DSP_op_data_valid  (DSP_op_data_valid), 

                           .DSP_A_data         (DSP_A_data), 
                           .DSP_B_data         (DSP_B_data), 
                           
			   //dsp op interface 
                           .dsp_op             (DSP_op_data),       
                           .ps_bram_dina       (ps_bram_dina_int1) 
);

wire [32*16-1:0]DSP_A_data_vec;
wire [32*16-1:0]DSP_B_data_vec;
wire [32*40-1:0]DSP_op_data_vec;
for (k=0; k <CASCADE_LEN; k=k+1 ) begin
  assign DSP_A_data_vec[A_DATA_WIDTH*(k+1)-1:A_DATA_WIDTH*k] = DSP_A_data[k];
  assign DSP_B_data_vec[B_DATA_WIDTH*(k+1)-1:B_DATA_WIDTH*k] = DSP_B_data[k];
end  
for (k=0; k <NUM_CASCADE_CHAINS; k=k+1 ) begin
  assign DSP_op_data[k] = DSP_op_data_vec[40*(k+1)-1:40*k];
end 

FIXGEMM_WRAPPER #(
.N(N),
.FAMILY(FAMILY),         // use 1 for 7-series, 2 for US/US+ and 3 for Versal
.TRIANGULAR_DELAYS(TRIANGULAR_DELAYS), // set to 0 to remove A,B,C and O triangular delays
.OX(OX),
.OY(OY)
) fixgemm_wrapper_inst(
               .CLK   (dsp_clk), //DSP clk
               .SA     (DSP_A_data_vec),
               .SB     (DSP_B_data_vec),
               .SO     (DSP_op_data_vec),
               .SC     ({40*32{1'b0}}),
               .SVAI   (DSP_A_data_valid), 
               .SVBCI  (DSP_B_data_valid), 
               .SVO    (DSP_op_data_valid) 
);


endmodule
