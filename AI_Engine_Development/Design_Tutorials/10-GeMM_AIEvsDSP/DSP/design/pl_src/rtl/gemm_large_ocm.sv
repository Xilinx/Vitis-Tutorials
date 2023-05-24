/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// This is the top level kernel which gets instantiated in the platform
// This module instantitates gemm_top, ps_slave, controller, Matrix B URAMs and output
// matrix URAMs 

module gemm_large_ocm #(

parameter NUM_CASCADE_CHAINS = 32,
parameter CASCADE_LEN = 32,
parameter A_DATA_WIDTH = 16,
parameter B_DATA_WIDTH = 16,
parameter OUTPUT_DATA_WIDTH = 58,
parameter URAM_DATA_WIDTH = 64,
parameter M = 32,
parameter N = 32,
parameter M_LARGE = 1024,
parameter N_LARGE = 1024,
parameter NUM_ROW_BRAMS = 8,
parameter NUM_COL_BRAMS = 8,
parameter ADDR_WIDTH = 9,
parameter URAM_ADDR_WIDTH = 12,
parameter BRAM_ADDR_WIDTH = 9,
parameter MAT_DATA_BASE_ADDR = 32'h200000,  // 2M
parameter S_AXI_ADDR_WIDTH = 16


) (

   //input                       clk,
   //input                       dsp_clk,
   input                       clk,
   input                       S_AXI_ACLK,
   input                       S_AXI_ARESETN,

   input                       S_AXI_AWVALID,
   output wire                 S_AXI_AWREADY,
   input [S_AXI_ADDR_WIDTH-1:0]S_AXI_AWADDR,
   input [2:0]                 S_AXI_AWPROT,

   // AXI4 W Channel
   input                       S_AXI_WVALID,
   output wire                 S_AXI_WREADY,
   input [31:0]                S_AXI_WDATA,
   input [3:0]                 S_AXI_WSTRB,


   // AXI4 AR Channel
   input                       S_AXI_ARVALID,
   output wire                 S_AXI_ARREADY,
   input [S_AXI_ADDR_WIDTH-1:0]S_AXI_ARADDR,
   input [2:0]                 S_AXI_ARPROT,


   // AXI4 R Channel
   output wire                 S_AXI_RVALID,
   input                       S_AXI_RREADY,
   output wire[31:0]           S_AXI_RDATA,
   output wire[1:0]            S_AXI_RRESP,

   // AXI4 B Channel
   output wire                 S_AXI_BVALID,
   input                       S_AXI_BREADY,
   output wire [1:0]           S_AXI_BRESP
);

wire reset;
assign reset = ~S_AXI_ARESETN;

genvar k;

// Internal wires

wire soft_ored_reset;

// Row URAM Write Interface
wire [63:0]              row_uram_ena;
wire [63:0]              row_uram_wea;
wire [11:0]              row_uram_addra;
wire [63:0]              row_uram_dina;

// Col URAM Write Interface
wire [63:0]              col_uram_ena;
wire [63:0]              col_uram_wea;
wire [11:0]              col_uram_addra;
wire [63:0]              col_uram_dina;

//OP URAM Read Interface
wire [63:0]              op_uram_enb;
wire [13:0]              op_uram_addrb;
wire [15:0]              op_uram_doutb;

// OP URAM write itnerface
wire [63:0]               op_uram_ena;   
wire [63:0]               op_uram_wea;
wire [14:0]              op_uram_addra;
wire [511:0]             op_uram_dina;

wire                     start;
wire                     write_op_uram;
wire                     dut_done;

wire                     read_op_uram;
wire [31:0]              active_clk_count;
wire [4:0]               num_mat_done;


wire [63:0]              row_uram_ena_o;
wire [63:0]              row_uram_wea_o;
wire [63:0]              col_uram_ena_o;
wire [63:0]              col_uram_wea_o;
wire [63:0]              op_uram_enb_o;
wire [31:0]              active_clk_count_i;
wire [4:0]               num_mat_done_i;

wire [63:0]              op_uram_doutb_valid_ps;
wire [63:0]              op_uram_doutb_valid;
wire                     op_uram_doutb_ack;
wire 			 uram_ena_ack;
// Instantiate PS Slave

ps_slave #(.C_S_AXI_ADDR_WIDTH(S_AXI_ADDR_WIDTH)) 
           ps_slave_inst (

             .S_AXI_ACLK        (S_AXI_ACLK),
             .S_AXI_ARESETN     (S_AXI_ARESETN),//active low
	     .soft_reset	(soft_reset_o),

   // AXI4 AW Channel
             .S_AXI_AWVALID     (S_AXI_AWVALID),
             .S_AXI_AWREADY     (S_AXI_AWREADY),
             .S_AXI_AWADDR      (S_AXI_AWADDR),
             .S_AXI_AWPROT      (S_AXI_AWPROT),

             // AXI4 W Channel
             .S_AXI_WVALID      (S_AXI_WVALID),
             .S_AXI_WREADY      (S_AXI_WREADY),
             .S_AXI_WDATA       (S_AXI_WDATA),
             .S_AXI_WSTRB       (S_AXI_WSTRB),

             // AXI4 AR Channel
             .S_AXI_ARVALID     (S_AXI_ARVALID),
             .S_AXI_ARREADY     (S_AXI_ARREADY),
             .S_AXI_ARADDR      (S_AXI_ARADDR),
             .S_AXI_ARPROT      (S_AXI_ARPROT),

             // AXI4 R Channel
             .S_AXI_RVALID      (S_AXI_RVALID),
             .S_AXI_RREADY      (S_AXI_RREADY),
             .S_AXI_RDATA       (S_AXI_RDATA),
             .S_AXI_RRESP       (S_AXI_RRESP),

             // AXI4 B Channel
             .S_AXI_BVALID      (S_AXI_BVALID),
             .S_AXI_BREADY      (S_AXI_BREADY),
             .S_AXI_BRESP       (S_AXI_BRESP),

             // Row URAM Write interface 
             .row_uram_ena   (row_uram_ena_o),
             .row_uram_wea   (row_uram_wea_o),
             .row_uram_addra (row_uram_addra),
             .row_uram_dina  (row_uram_dina),

             // Col URAM Write Interface
             .col_uram_ena   (col_uram_ena_o),
             .col_uram_wea   (col_uram_wea_o),
             .col_uram_addra (col_uram_addra),
             .col_uram_dina  (col_uram_dina),
             .uram_ena_ack   (uram_ena_ack),  

             //OP URAM Read Interface
             .op_uram_enb    (op_uram_enb_o),
             .op_uram_addrb  (op_uram_addrb),
             .op_uram_doutb  (op_uram_doutb),
             .op_uram_doutb_valid      (|op_uram_doutb_valid_ps),
             .op_uram_doutb_ack        (op_uram_doutb_ack),

             .ps_app_done    (ps_app_done_o),
             .dut_done       (dut_done_i),//| read_op_uram),

             .num_mat_done   (num_mat_done_i),
             .active_clk_count (active_clk_count_i),
	     .mem_init_done  (1'b0)   // Make this 0 when URAMs are loaded from AXI bus 
	     //.mem_init_done  (1'b1)   // Make this 0 when URAMs are loaded from AXI bus 

);

// Instantiate gemm_top

gemm_top #(

       .NUM_CASCADE_CHAINS  (NUM_CASCADE_CHAINS), 
       .CASCADE_LEN         (CASCADE_LEN), 
       .A_DATA_WIDTH        (A_DATA_WIDTH), 
       .B_DATA_WIDTH        (B_DATA_WIDTH), 
       .OUTPUT_DATA_WIDTH   (OUTPUT_DATA_WIDTH), 
       .URAM_DATA_WIDTH     (URAM_DATA_WIDTH), 
       .M                   (M), 
       .N                   (N), 
       .M_LARGE             (M_LARGE), 
       .N_LARGE             (N_LARGE), 
       //.NUM_ROW_BRAMS       (NUM_ROW_BRAMS), 
       //.NUM_COL_BRAMS       (NUM_COL_BRAMS), 
       .ADDR_WIDTH          (ADDR_WIDTH), 
       .URAM_ADDR_WIDTH     (URAM_ADDR_WIDTH) 

) gemm_top_inst (

                  .clk                      (clkh),
                  .dsp_clk                  (dsp_clk),
                  //.reset                    (reset | soft_reset),
                  .reset                    (soft_ored_reset), //active high

                  // Row URAM Write interface 
                  .row_uram_ena             (row_uram_ena),
                  .row_uram_wea             (row_uram_wea),
                  .row_uram_addra           (row_uram_addra),
                  .row_uram_dina            (row_uram_dina),

                  // Col URAM write itnerface
                  .col_uram_ena             (col_uram_ena),
                  .col_uram_wea             (col_uram_wea),
                  .col_uram_addra           (col_uram_addra),
                  .col_uram_dina            (col_uram_dina),

                  // OP URAM read interface
                  .op_uram_addrb            (op_uram_addrb),
                  .op_uram_enb              (op_uram_enb),
                  .op_uram_doutb            (op_uram_doutb),
                  .op_uram_doutb_valid      (op_uram_doutb_valid),

                  .write_op_uram            (write_op_uram),
                  .start                    (ps_app_done),
                  .done                     (dut_done),
	          .active_clk_count         (active_clk_count),
                  
                  // Statistics registers
		  .num_mat_done   (num_mat_done)
);

synchronizer synchronizer_inst(
                  .clk                    (clkh),
                  .S_AXI_ACLK             (S_AXI_ACLK),
                  .reset                  (reset), //active high // which reset

                  .soft_reset_i           (soft_reset_o),  
                  .row_uram_ena_i         (row_uram_ena_o), 
                  .row_uram_wea_i         (row_uram_wea_o), 
                  .col_uram_ena_i         (col_uram_ena_o), 
                  .col_uram_wea_i         (col_uram_wea_o), 
                  .op_uram_enb_i          (op_uram_enb_o), 
                  .ps_app_done_i          (ps_app_done_o), 
                  
                  .dut_done_o             (dut_done_i),
                  .num_mat_done_o         (num_mat_done_i),
                  .active_clk_count_o     (active_clk_count_i),
                  
                  .soft_ored_reset_o      (soft_ored_reset),   
                  .row_uram_ena_o         (row_uram_ena),
                  .row_uram_wea_o         (row_uram_wea),
                  .col_uram_ena_o         (col_uram_ena),
                  .col_uram_wea_o         (col_uram_wea),
                  .op_uram_enb_o          (op_uram_enb),  
                  .uram_ena_ack_o         (uram_ena_ack),  

                  .op_uram_doutb_valid_ps (op_uram_doutb_valid_ps),  
                  .op_uram_doutb_valid    (op_uram_doutb_valid),  
                  .op_uram_doutb_ack_i    (op_uram_doutb_ack),
                  .ps_app_done_o          (ps_app_done),  
                  
                  .dut_done_i             (dut_done),
                  .num_mat_done_i         (num_mat_done),
                  .active_clk_count_i     (active_clk_count)

);

MBUFGCE_DIV mbufgce_div_inst(
   .I(clk),
   .CE(1'b1),
   .CLR(1'b0),
   .CLRB_LEAF(1'b1),

   .O1(dsp_clk),
   .O2(clkh)
);

endmodule 
