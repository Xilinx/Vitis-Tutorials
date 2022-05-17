/* (c) Copyright 2019 Xilinx, Inc. All rights reserved.
 This file contains confidential and proprietary information 
 of Xilinx, Inc. and is protected under U.S. and
 international copyright and other intellectual property 
 laws.
 
 DISCLAIMER
 This disclaimer is not a license and does not grant any 
 rights to the materials distributed herewith. Except as 
 otherwise provided in a valid license issued to you by 
 Xilinx, and to the maximum extent permitted by applicable
 law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
 WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES 
 AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING 
 BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
 INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and 
 (2) Xilinx shall not be liable (whether in contract or tort, 
 including negligence, or under any other theory of 
 liability) for any loss or damage of any kind or nature 
 related to, arising under or in connection with these 
 materials, including for any direct, or any indirect, 
 special, incidental, or consequential loss or damage 
 (including loss of data, profits, goodwill, or any type of 
 loss or damage suffered as a result of any action brought 
 by a third party) even if such damage or loss was 
 reasonably foreseeable or Xilinx had been advised of the 
 possibility of the same.
 
 CRITICAL APPLICATIONS
 Xilinx products are not designed or intended to be fail-
 safe, or for use in any application requiring fail-safe
 performance, such as life-support or safety devices or 
 systems, Class III medical devices, nuclear facilities, 
 applications related to the deployment of airbags, or any 
 other applications that could lead to death, personal 
 injury, or severe property or environmental damage 
 (individually and collectively, "Critical 
 Applications"). Customer assumes the sole risk and 
 liability of any use of Xilinx products in Critical 
 Applications, subject only to applicable laws and 
 regulations governing limitations on product liability.
 
 THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS 
 PART OF THIS FILE AT ALL TIMES.
*/

//////////////////////////////////////////////////////////////////////////////// 
// default_nettype of none prevents implicit wire declaration.
`default_nettype none
`timescale 1 ns / 1 ps
// Top level of the kernel. Do not modify module name, parameters or ports.
module ethernet_krnl_axis_x4 #(
  parameter integer C_S_AXI_CONTROL_ADDR_WIDTH = 12 ,
  parameter integer C_S_AXI_CONTROL_DATA_WIDTH = 32 ,
  parameter integer C_M_AXI_ADDR_WIDTH       = 64 ,
  parameter integer C_M_AXI_DATA_WIDTH       = 64,
  parameter integer C_M_AXI_ID_WIDTH         = 1,
  parameter integer C_XFER_SIZE_WIDTH        = 32  
)
(
  // System Signals
  input  wire                                    ap_clk               ,
  input  wire                                    ap_rst_n             ,
  //  Note: A minimum subset of AXI4 memory mapped signals are declared.  AXI
  // signals omitted from these interfaces are automatically inferred with the
  // optimal values for Xilinx SDx systems.  This allows Xilinx AXI4 Interconnects
  // within the system to be optimized by removing logic for AXI4 protocol
  // features that are not necessary. When adapting AXI4 masters within the RTL
  // kernel that have signals not declared below, it is suitable to add the
  // signals to the declarations below to connect them to the AXI4 Master.
  // 
  // List of ommited signals - effect
  // -------------------------------
  // ID - Transaction ID are used for multithreading and out of order
  // transactions.  This increases complexity. This saves logic and increases Fmax
  // in the system when ommited.
  // SIZE - Default value is log2(data width in bytes). Needed for subsize bursts.
  // This saves logic and increases Fmax in the system when ommited.
  // BURST - Default value (0b01) is incremental.  Wrap and fixed bursts are not
  // recommended. This saves logic and increases Fmax in the system when ommited.
  // LOCK - Not supported in AXI4
  // CACHE - Default value (0b0011) allows modifiable transactions. No benefit to
  // changing this.
  // PROT - Has no effect in SDx systems.
  // QOS - Has no effect in SDx systems.
  // REGION - Has no effect in SDx systems.
  // USER - Has no effect in SDx systems.
  // RESP - Not useful in most SDx systems.
  // 
  // AXI4-Lite slave interface
  input  wire                                    s_axi_control_awvalid,
  output wire                                    s_axi_control_awready,
  input  wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]   s_axi_control_awaddr ,
  input  wire                                    s_axi_control_wvalid ,
  output wire                                    s_axi_control_wready ,
  input  wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0]   s_axi_control_wdata  ,
  input  wire [C_S_AXI_CONTROL_DATA_WIDTH/8-1:0] s_axi_control_wstrb  ,
  input  wire                                    s_axi_control_arvalid,
  output wire                                    s_axi_control_arready,
  input  wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]   s_axi_control_araddr ,
  output wire                                    s_axi_control_rvalid ,
  input  wire                                    s_axi_control_rready ,
  output wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0]   s_axi_control_rdata  ,
  output wire [2-1:0]                            s_axi_control_rresp  ,
  output wire                                    s_axi_control_bvalid ,
  input  wire                                    s_axi_control_bready ,
  output wire [2-1:0]                            s_axi_control_bresp  ,
  
  //
  output wire        rx0_axis_tvalid,
  output wire [63:0] rx0_axis_tdata,
  output wire        rx0_axis_tlast,
  output wire [7:0]  rx0_axis_tkeep,
  output wire        rx0_axis_tuser,
  input  wire        rx0_axis_tready,
  
  output wire        rx1_axis_tvalid,
  output wire [63:0] rx1_axis_tdata,
  output wire        rx1_axis_tlast,
  output wire [7:0]  rx1_axis_tkeep,
  output wire        rx1_axis_tuser,
  input  wire        rx1_axis_tready,
  
  output wire        rx2_axis_tvalid,
  output wire [63:0] rx2_axis_tdata,
  output wire        rx2_axis_tlast,
  output wire [7:0]  rx2_axis_tkeep,
  output wire        rx2_axis_tuser,
  input  wire        rx2_axis_tready,
  
  output wire        rx3_axis_tvalid,
  output wire [63:0] rx3_axis_tdata,
  output wire        rx3_axis_tlast,
  output wire [7:0]  rx3_axis_tkeep,
  output wire        rx3_axis_tuser,
  input  wire        rx3_axis_tready,      
  
  output wire        tx0_axis_tready,       
  input  wire        tx0_axis_tvalid,
  input  wire [63:0] tx0_axis_tdata,
  input  wire        tx0_axis_tlast,
  input  wire [7:0]  tx0_axis_tkeep,
  input  wire        tx0_axis_tuser,
  
  output wire        tx1_axis_tready,       
  input  wire        tx1_axis_tvalid,
  input  wire [63:0] tx1_axis_tdata,
  input  wire        tx1_axis_tlast,
  input  wire [7:0]  tx1_axis_tkeep,
  input  wire        tx1_axis_tuser,
  
  output wire        tx2_axis_tready,       
  input  wire        tx2_axis_tvalid,
  input  wire [63:0] tx2_axis_tdata,
  input  wire        tx2_axis_tlast,
  input  wire [7:0]  tx2_axis_tkeep,
  input  wire        tx2_axis_tuser,
  
  output wire        tx3_axis_tready,       
  input  wire        tx3_axis_tvalid,
  input  wire [63:0] tx3_axis_tdata,
  input  wire        tx3_axis_tlast,
  input  wire [7:0]  tx3_axis_tkeep,
  input  wire        tx3_axis_tuser,      
  //
  input  wire clk_gt_freerun,
  input  wire [3:0] gt_rxp_in,
  input  wire [3:0] gt_rxn_in,
  output wire [3:0] gt_txp_out,
  output wire [3:0] gt_txn_out,
  input  wire gt_refclk_p,
  input  wire gt_refclk_n  
);
localparam C_NUM_LANES = 4;
///////////////////////////////////////////////////////////////////////////////
// Wires and Variables
///////////////////////////////////////////////////////////////////////////////
(* DONT_TOUCH = "yes" *)
logic areset = 1'b0;

logic [C_NUM_LANES-1:0] axi_str_txd_tvalid;
logic [C_NUM_LANES-1:0] axi_str_txd_tready;
logic [C_NUM_LANES-1:0] axi_str_txd_tlast;
logic [C_NUM_LANES-1:0] [7:0] axi_str_txd_tkeep;
logic [C_NUM_LANES-1:0] axi_str_txd_tuser;  // Unused. We never force an error on Tx.
logic [C_NUM_LANES-1:0] [63:0] axi_str_txd_tdata;
logic [C_NUM_LANES-1:0] axi_str_rxd_tvalid;
logic [C_NUM_LANES-1:0] axi_str_rxd_tready;
logic [C_NUM_LANES-1:0] axi_str_rxd_tlast;
logic [C_NUM_LANES-1:0] [7:0] axi_str_rxd_tkeep;
logic [C_NUM_LANES-1:0] [63:0] axi_str_rxd_tdata;
logic [C_NUM_LANES-1:0] axi_str_rxd_tuser;

logic [C_NUM_LANES-1:0][31:0] scalar;
logic [C_NUM_LANES-1:0] stat_rx_block_lock;
logic [C_NUM_LANES-1:0][2:0] gt_loopback;
logic [C_NUM_LANES-1:0] gtwiz_reset_rx_datapath;
logic [C_NUM_LANES-1:0] gtwiz_reset_tx_datapath;
logic [C_NUM_LANES-1:0] ap_stat_rx_block_lock_sync;

logic [C_NUM_LANES-1:0] tx_reset;
logic [C_NUM_LANES-1:0] rx_reset;
logic [C_NUM_LANES-1:0] rx_core_clk;
logic [C_NUM_LANES-1:0] tx_clk_out;

logic [C_NUM_LANES-1:0] fifo_resetn = '1;
logic sys_reset_comb;
logic sys_reset = 0;

// AXI4-Lite slave interface
ethernet_control_s_axi #(
  .C_S_AXI_ADDR_WIDTH ( C_S_AXI_CONTROL_ADDR_WIDTH ),
  .C_S_AXI_DATA_WIDTH ( C_S_AXI_CONTROL_DATA_WIDTH )
)
inst_control_s_axi (
  .ACLK     ( ap_clk                ),
  .ARESET   ( 1'b0                  ),
  .ACLK_EN  ( 1'b1                  ),
  .AWVALID  ( s_axi_control_awvalid ),
  .AWREADY  ( s_axi_control_awready ),
  .AWADDR   ( s_axi_control_awaddr  ),
  .WVALID   ( s_axi_control_wvalid  ),
  .WREADY   ( s_axi_control_wready  ),
  .WDATA    ( s_axi_control_wdata   ),
  .WSTRB    ( s_axi_control_wstrb   ),
  .ARVALID  ( s_axi_control_arvalid ),
  .ARREADY  ( s_axi_control_arready ),
  .ARADDR   ( s_axi_control_araddr  ),
  .RVALID   ( s_axi_control_rvalid  ),
  .RREADY   ( s_axi_control_rready  ),
  .RDATA    ( s_axi_control_rdata   ),
  .RRESP    ( s_axi_control_rresp   ),
  .BVALID   ( s_axi_control_bvalid  ),
  .BREADY   ( s_axi_control_bready  ),
  .BRESP    ( s_axi_control_bresp   ),
  .scalar00 ( scalar[0]             ),
  .scalar01 ( scalar[1]             ),
  .scalar02 ( scalar[2]             ),
  .scalar03 ( scalar[3]             ),
  .rx_block_lock( ap_stat_rx_block_lock_sync)
);

xpm_cdc_array_single #(
    .DEST_SYNC_FF(4),
    .INIT_SYNC_FF(0),
    .SIM_ASSERT_CHK(0),
    .SRC_INPUT_REG(0),
    .WIDTH(C_NUM_LANES)
) ap_rx_block_lock_sync_i (
    .dest_out(ap_stat_rx_block_lock_sync),
    .dest_clk(ap_clk),
    .src_clk(1'b0),
    .src_in(stat_rx_block_lock)
);

always_comb begin
    sys_reset_comb = 0;
    for (int ii = 0; ii < C_NUM_LANES; ii++) begin
        gt_loopback[ii] = scalar[ii][0+:3];
        gtwiz_reset_rx_datapath[ii] = scalar[ii][4];
        gtwiz_reset_tx_datapath[ii] = scalar[ii][5];
        sys_reset_comb |= scalar[ii][12];
    end
end

always_ff @(posedge ap_clk) begin
  sys_reset <= sys_reset_comb;
  for (int ii = 0; ii < C_NUM_LANES; ii++) begin
      areset <= ~ap_rst_n || sys_reset;
      fifo_resetn[ii] <= !(areset || scalar[ii][8]);
  end 
end

always_comb begin
    for (int ii = 0; ii < C_NUM_LANES; ii++) begin
        rx_core_clk[ii] = tx_clk_out[ii];
    end
end

//Ethernet Subsystem instance
xxv_ethernet_x4_0 i_ethernet_0 (
  .gt_txp_out(gt_txp_out),                                              // output wire [3 : 0] gt_txp_out
  .gt_txn_out(gt_txn_out),                                              // output wire [3 : 0] gt_txn_out
  .gt_rxp_in(gt_rxp_in),                                                // input wire [3 : 0] gt_rxp_in
  .gt_rxn_in(gt_rxn_in),                                                // input wire [3 : 0] gt_rxn_in
  .rx_core_clk_0(rx_core_clk[0]),                                        // input wire rx_core_clk_0
  .rx_core_clk_1(rx_core_clk[1]),                                        // input wire rx_core_clk_1
  .rx_core_clk_2(rx_core_clk[2]),                                        // input wire rx_core_clk_2
  .rx_core_clk_3(rx_core_clk[3]),                                        // input wire rx_core_clk_3
  .txoutclksel_in_0(3'd5),                                  // input wire [2 : 0] txoutclksel_in_0
  .rxoutclksel_in_0(3'd5),                                  // input wire [2 : 0] rxoutclksel_in_0
  .txoutclksel_in_1(3'd5),                                  // input wire [2 : 0] txoutclksel_in_1
  .rxoutclksel_in_1(3'd5),                                  // input wire [2 : 0] rxoutclksel_in_1
  .txoutclksel_in_2(3'd5),                                  // input wire [2 : 0] txoutclksel_in_2
  .rxoutclksel_in_2(3'd5),                                  // input wire [2 : 0] rxoutclksel_in_2
  .txoutclksel_in_3(3'd5),                                  // input wire [2 : 0] txoutclksel_in_3
  .rxoutclksel_in_3(3'd5),                                  // input wire [2 : 0] rxoutclksel_in_3
  .gtwiz_reset_tx_datapath_0(gtwiz_reset_tx_datapath[0]),                // input wire gtwiz_reset_tx_datapath_0
  .gtwiz_reset_tx_datapath_1(gtwiz_reset_tx_datapath[1]),                // input wire gtwiz_reset_tx_datapath_1
  .gtwiz_reset_tx_datapath_2(gtwiz_reset_tx_datapath[2]),                // input wire gtwiz_reset_tx_datapath_2
  .gtwiz_reset_tx_datapath_3(gtwiz_reset_tx_datapath[3]),                // input wire gtwiz_reset_tx_datapath_3
  .gtwiz_reset_rx_datapath_0(gtwiz_reset_rx_datapath[0]),                // input wire gtwiz_reset_rx_datapath_0
  .gtwiz_reset_rx_datapath_1(gtwiz_reset_rx_datapath[1]),                // input wire gtwiz_reset_rx_datapath_1
  .gtwiz_reset_rx_datapath_2(gtwiz_reset_rx_datapath[2]),                // input wire gtwiz_reset_rx_datapath_2
  .gtwiz_reset_rx_datapath_3(gtwiz_reset_rx_datapath[3]),                // input wire gtwiz_reset_rx_datapath_3
  .rxrecclkout_0(),                                        // output wire rxrecclkout_0
  .rxrecclkout_1(),                                        // output wire rxrecclkout_1
  .rxrecclkout_2(),                                        // output wire rxrecclkout_2
  .rxrecclkout_3(),                                        // output wire rxrecclkout_3
  .sys_reset(areset),                                                // input wire sys_reset
  .dclk(clk_gt_freerun),                                                          // input wire dclk
  .tx_clk_out_0(tx_clk_out[0]),                                          // output wire tx_clk_out_0
  .tx_clk_out_1(tx_clk_out[1]),                                          // output wire tx_clk_out_1
  .tx_clk_out_2(tx_clk_out[2]),                                          // output wire tx_clk_out_2
  .tx_clk_out_3(tx_clk_out[3]),                                          // output wire tx_clk_out_3
  .rx_clk_out_0(),                                          // output wire rx_clk_out_0
  .rx_clk_out_1(),                                          // output wire rx_clk_out_1
  .rx_clk_out_2(),                                          // output wire rx_clk_out_2
  .rx_clk_out_3(),                                          // output wire rx_clk_out_3
  .gt_refclk_p(gt_refclk_p),                                            // input wire gt_refclk_p
  .gt_refclk_n(gt_refclk_n),                                            // input wire gt_refclk_n
  .gt_refclk_out(),                                        // output wire gt_refclk_out
  .gtpowergood_out_0(),                                // output wire gtpowergood_out_0
  .gtpowergood_out_1(),                                // output wire gtpowergood_out_1
  .gtpowergood_out_2(),                                // output wire gtpowergood_out_2
  .gtpowergood_out_3(),                                // output wire gtpowergood_out_3
  .rx_reset_0(1'b0),                                              // input wire rx_reset_0
  .rx_reset_1(1'b0),                                              // input wire rx_reset_1
  .rx_reset_2(1'b0),                                              // input wire rx_reset_2
  .rx_reset_3(1'b0),                                              // input wire rx_reset_3
  .user_rx_reset_0(rx_reset[0]),                                    // output wire user_rx_reset_0
  .user_rx_reset_1(rx_reset[1]),                                    // output wire user_rx_reset_1
  .user_rx_reset_2(rx_reset[2]),                                    // output wire user_rx_reset_2
  .user_rx_reset_3(rx_reset[3]),                                    // output wire user_rx_reset_3
  .rx_axis_tvalid_0(axi_str_rxd_tvalid[0]),                                  // output wire rx_axis_tvalid_0
  .rx_axis_tvalid_1(axi_str_rxd_tvalid[1]),                                  // output wire rx_axis_tvalid_1
  .rx_axis_tvalid_2(axi_str_rxd_tvalid[2]),                                  // output wire rx_axis_tvalid_2
  .rx_axis_tvalid_3(axi_str_rxd_tvalid[3]),                                  // output wire rx_axis_tvalid_3
  .rx_axis_tdata_0(axi_str_rxd_tdata[0]),                                    // output wire [63 : 0] rx_axis_tdata_0
  .rx_axis_tdata_1(axi_str_rxd_tdata[1]),                                    // output wire [63 : 0] rx_axis_tdata_1
  .rx_axis_tdata_2(axi_str_rxd_tdata[2]),                                    // output wire [63 : 0] rx_axis_tdata_2
  .rx_axis_tdata_3(axi_str_rxd_tdata[3]),                                    // output wire [63 : 0] rx_axis_tdata_3
  .rx_axis_tlast_0(axi_str_rxd_tlast[0]),                                    // output wire rx_axis_tlast_0
  .rx_axis_tlast_1(axi_str_rxd_tlast[1]),                                    // output wire rx_axis_tlast_1
  .rx_axis_tlast_2(axi_str_rxd_tlast[2]),                                    // output wire rx_axis_tlast_2
  .rx_axis_tlast_3(axi_str_rxd_tlast[3]),                                    // output wire rx_axis_tlast_3
  .rx_axis_tkeep_0(axi_str_rxd_tkeep[0]),                                    // output wire [7 : 0] rx_axis_tkeep_0
  .rx_axis_tkeep_1(axi_str_rxd_tkeep[1]),                                    // output wire [7 : 0] rx_axis_tkeep_1
  .rx_axis_tkeep_2(axi_str_rxd_tkeep[2]),                                    // output wire [7 : 0] rx_axis_tkeep_2
  .rx_axis_tkeep_3(axi_str_rxd_tkeep[3]),                                    // output wire [7 : 0] rx_axis_tkeep_3
  .rx_axis_tuser_0(axi_str_rxd_tuser[0]),                                    // output wire rx_axis_tuser_0
  .rx_axis_tuser_1(axi_str_rxd_tuser[1]),                                    // output wire rx_axis_tuser_1
  .rx_axis_tuser_2(axi_str_rxd_tuser[2]),                                    // output wire rx_axis_tuser_2
  .rx_axis_tuser_3(axi_str_rxd_tuser[3]),                                    // output wire rx_axis_tuser_3
  .ctl_rx_enable_0(1'b1),                                    // input wire ctl_rx_enable_0
  .ctl_rx_enable_1(1'b1),                                    // input wire ctl_rx_enable_1
  .ctl_rx_enable_2(1'b1),                                    // input wire ctl_rx_enable_2
  .ctl_rx_enable_3(1'b1),                                    // input wire ctl_rx_enable_3
  .ctl_rx_check_preamble_0(1'b1),                    // input wire ctl_rx_check_preamble_0
  .ctl_rx_check_preamble_1(1'b1),                    // input wire ctl_rx_check_preamble_1
  .ctl_rx_check_preamble_2(1'b1),                    // input wire ctl_rx_check_preamble_2
  .ctl_rx_check_preamble_3(1'b1),                    // input wire ctl_rx_check_preamble_3
  .ctl_rx_check_sfd_0(1'b1),                              // input wire ctl_rx_check_sfd_0
  .ctl_rx_check_sfd_1(1'b1),                              // input wire ctl_rx_check_sfd_1
  .ctl_rx_check_sfd_2(1'b1),                              // input wire ctl_rx_check_sfd_2
  .ctl_rx_check_sfd_3(1'b1),                              // input wire ctl_rx_check_sfd_3
  .ctl_rx_force_resync_0(1'b0),                        // input wire ctl_rx_force_resync_0
  .ctl_rx_force_resync_1(1'b0),                        // input wire ctl_rx_force_resync_1
  .ctl_rx_force_resync_2(1'b0),                        // input wire ctl_rx_force_resync_2
  .ctl_rx_force_resync_3(1'b0),                        // input wire ctl_rx_force_resync_3
  .ctl_rx_delete_fcs_0(1'b1),                            // input wire ctl_rx_delete_fcs_0
  .ctl_rx_delete_fcs_1(1'b1),                            // input wire ctl_rx_delete_fcs_1
  .ctl_rx_delete_fcs_2(1'b1),                            // input wire ctl_rx_delete_fcs_2
  .ctl_rx_delete_fcs_3(1'b1),                            // input wire ctl_rx_delete_fcs_3
  .ctl_rx_ignore_fcs_0(1'b0),                            // input wire ctl_rx_ignore_fcs_0
  .ctl_rx_ignore_fcs_1(1'b0),                            // input wire ctl_rx_ignore_fcs_1
  .ctl_rx_ignore_fcs_2(1'b0),                            // input wire ctl_rx_ignore_fcs_2
  .ctl_rx_ignore_fcs_3(1'b0),                            // input wire ctl_rx_ignore_fcs_3
  .ctl_rx_max_packet_len_0(15'd9600),                    // input wire [14 : 0] ctl_rx_max_packet_len_0
  .ctl_rx_max_packet_len_1(15'd9600),                    // input wire [14 : 0] ctl_rx_max_packet_len_1
  .ctl_rx_max_packet_len_2(15'd9600),                    // input wire [14 : 0] ctl_rx_max_packet_len_2
  .ctl_rx_max_packet_len_3(15'd9600),                    // input wire [14 : 0] ctl_rx_max_packet_len_3
  .ctl_rx_min_packet_len_0(8'd64),                    // input wire [7 : 0] ctl_rx_min_packet_len_0
  .ctl_rx_min_packet_len_1(8'd64),                    // input wire [7 : 0] ctl_rx_min_packet_len_1
  .ctl_rx_min_packet_len_2(8'd64),                    // input wire [7 : 0] ctl_rx_min_packet_len_2
  .ctl_rx_min_packet_len_3(8'd64),                    // input wire [7 : 0] ctl_rx_min_packet_len_3
  .ctl_rx_process_lfi_0(1'b0),                          // input wire ctl_rx_process_lfi_0
  .ctl_rx_process_lfi_1(1'b0),                          // input wire ctl_rx_process_lfi_1
  .ctl_rx_process_lfi_2(1'b0),                          // input wire ctl_rx_process_lfi_2
  .ctl_rx_process_lfi_3(1'b0),                          // input wire ctl_rx_process_lfi_3
  .ctl_rx_test_pattern_0(1'b0),                        // input wire ctl_rx_test_pattern_0
  .ctl_rx_test_pattern_1(1'b0),                        // input wire ctl_rx_test_pattern_1
  .ctl_rx_test_pattern_2(1'b0),                        // input wire ctl_rx_test_pattern_2
  .ctl_rx_test_pattern_3(1'b0),                        // input wire ctl_rx_test_pattern_3
  .ctl_rx_data_pattern_select_0(1'b0),          // input wire ctl_rx_data_pattern_select_0
  .ctl_rx_data_pattern_select_1(1'b0),          // input wire ctl_rx_data_pattern_select_1
  .ctl_rx_data_pattern_select_2(1'b0),          // input wire ctl_rx_data_pattern_select_2
  .ctl_rx_data_pattern_select_3(1'b0),          // input wire ctl_rx_data_pattern_select_3
  .ctl_rx_test_pattern_enable_0(1'b0),          // input wire ctl_rx_test_pattern_enable_0
  .ctl_rx_test_pattern_enable_1(1'b0),          // input wire ctl_rx_test_pattern_enable_1
  .ctl_rx_test_pattern_enable_2(1'b0),          // input wire ctl_rx_test_pattern_enable_2
  .ctl_rx_test_pattern_enable_3(1'b0),          // input wire ctl_rx_test_pattern_enable_3
  .ctl_rx_custom_preamble_enable_0(1'b0),    // input wire ctl_rx_custom_preamble_enable_0
  .ctl_rx_custom_preamble_enable_1(1'b0),    // input wire ctl_rx_custom_preamble_enable_1
  .ctl_rx_custom_preamble_enable_2(1'b0),    // input wire ctl_rx_custom_preamble_enable_2
  .ctl_rx_custom_preamble_enable_3(1'b0),    // input wire ctl_rx_custom_preamble_enable_3
  .stat_rx_framing_err_0(),                        // output wire stat_rx_framing_err_0
  .stat_rx_framing_err_1(),                        // output wire stat_rx_framing_err_1
  .stat_rx_framing_err_2(),                        // output wire stat_rx_framing_err_2
  .stat_rx_framing_err_3(),                        // output wire stat_rx_framing_err_3
  .stat_rx_framing_err_valid_0(),            // output wire stat_rx_framing_err_valid_0
  .stat_rx_framing_err_valid_1(),            // output wire stat_rx_framing_err_valid_1
  .stat_rx_framing_err_valid_2(),            // output wire stat_rx_framing_err_valid_2
  .stat_rx_framing_err_valid_3(),            // output wire stat_rx_framing_err_valid_3
  .stat_rx_local_fault_0(),                        // output wire stat_rx_local_fault_0
  .stat_rx_local_fault_1(),                        // output wire stat_rx_local_fault_1
  .stat_rx_local_fault_2(),                        // output wire stat_rx_local_fault_2
  .stat_rx_local_fault_3(),                        // output wire stat_rx_local_fault_3
  .stat_rx_block_lock_0(stat_rx_block_lock[0]),                          // output wire stat_rx_block_lock_0
  .stat_rx_block_lock_1(stat_rx_block_lock[1]),                          // output wire stat_rx_block_lock_1
  .stat_rx_block_lock_2(stat_rx_block_lock[2]),                          // output wire stat_rx_block_lock_2
  .stat_rx_block_lock_3(stat_rx_block_lock[3]),                          // output wire stat_rx_block_lock_3
  .stat_rx_valid_ctrl_code_0(),                // output wire stat_rx_valid_ctrl_code_0
  .stat_rx_valid_ctrl_code_1(),                // output wire stat_rx_valid_ctrl_code_1
  .stat_rx_valid_ctrl_code_2(),                // output wire stat_rx_valid_ctrl_code_2
  .stat_rx_valid_ctrl_code_3(),                // output wire stat_rx_valid_ctrl_code_3
  .stat_rx_status_0(),                                  // output wire stat_rx_status_0
  .stat_rx_status_1(),                                  // output wire stat_rx_status_1
  .stat_rx_status_2(),                                  // output wire stat_rx_status_2
  .stat_rx_status_3(),                                  // output wire stat_rx_status_3
  .stat_rx_remote_fault_0(),                      // output wire stat_rx_remote_fault_0
  .stat_rx_remote_fault_1(),                      // output wire stat_rx_remote_fault_1
  .stat_rx_remote_fault_2(),                      // output wire stat_rx_remote_fault_2
  .stat_rx_remote_fault_3(),                      // output wire stat_rx_remote_fault_3
  .stat_rx_bad_fcs_0(),                                // output wire [1 : 0] stat_rx_bad_fcs_0
  .stat_rx_bad_fcs_1(),                                // output wire [1 : 0] stat_rx_bad_fcs_1
  .stat_rx_bad_fcs_2(),                                // output wire [1 : 0] stat_rx_bad_fcs_2
  .stat_rx_bad_fcs_3(),                                // output wire [1 : 0] stat_rx_bad_fcs_3
  .stat_rx_stomped_fcs_0(),                        // output wire [1 : 0] stat_rx_stomped_fcs_0
  .stat_rx_stomped_fcs_1(),                        // output wire [1 : 0] stat_rx_stomped_fcs_1
  .stat_rx_stomped_fcs_2(),                        // output wire [1 : 0] stat_rx_stomped_fcs_2
  .stat_rx_stomped_fcs_3(),                        // output wire [1 : 0] stat_rx_stomped_fcs_3
  .stat_rx_truncated_0(),                            // output wire stat_rx_truncated_0
  .stat_rx_truncated_1(),                            // output wire stat_rx_truncated_1
  .stat_rx_truncated_2(),                            // output wire stat_rx_truncated_2
  .stat_rx_truncated_3(),                            // output wire stat_rx_truncated_3
  .stat_rx_internal_local_fault_0(),      // output wire stat_rx_internal_local_fault_0
  .stat_rx_internal_local_fault_1(),      // output wire stat_rx_internal_local_fault_1
  .stat_rx_internal_local_fault_2(),      // output wire stat_rx_internal_local_fault_2
  .stat_rx_internal_local_fault_3(),      // output wire stat_rx_internal_local_fault_3
  .stat_rx_received_local_fault_0(),      // output wire stat_rx_received_local_fault_0
  .stat_rx_received_local_fault_1(),      // output wire stat_rx_received_local_fault_1
  .stat_rx_received_local_fault_2(),      // output wire stat_rx_received_local_fault_2
  .stat_rx_received_local_fault_3(),      // output wire stat_rx_received_local_fault_3
  .stat_rx_hi_ber_0(),                                  // output wire stat_rx_hi_ber_0
  .stat_rx_hi_ber_1(),                                  // output wire stat_rx_hi_ber_1
  .stat_rx_hi_ber_2(),                                  // output wire stat_rx_hi_ber_2
  .stat_rx_hi_ber_3(),                                  // output wire stat_rx_hi_ber_3
  .stat_rx_got_signal_os_0(),                    // output wire stat_rx_got_signal_os_0
  .stat_rx_got_signal_os_1(),                    // output wire stat_rx_got_signal_os_1
  .stat_rx_got_signal_os_2(),                    // output wire stat_rx_got_signal_os_2
  .stat_rx_got_signal_os_3(),                    // output wire stat_rx_got_signal_os_3
  .stat_rx_test_pattern_mismatch_0(),    // output wire stat_rx_test_pattern_mismatch_0
  .stat_rx_test_pattern_mismatch_1(),    // output wire stat_rx_test_pattern_mismatch_1
  .stat_rx_test_pattern_mismatch_2(),    // output wire stat_rx_test_pattern_mismatch_2
  .stat_rx_test_pattern_mismatch_3(),    // output wire stat_rx_test_pattern_mismatch_3
  .stat_rx_total_bytes_0(),                        // output wire [3 : 0] stat_rx_total_bytes_0
  .stat_rx_total_bytes_1(),                        // output wire [3 : 0] stat_rx_total_bytes_1
  .stat_rx_total_bytes_2(),                        // output wire [3 : 0] stat_rx_total_bytes_2
  .stat_rx_total_bytes_3(),                        // output wire [3 : 0] stat_rx_total_bytes_3
  .stat_rx_total_packets_0(),                    // output wire [1 : 0] stat_rx_total_packets_0
  .stat_rx_total_packets_1(),                    // output wire [1 : 0] stat_rx_total_packets_1
  .stat_rx_total_packets_2(),                    // output wire [1 : 0] stat_rx_total_packets_2
  .stat_rx_total_packets_3(),                    // output wire [1 : 0] stat_rx_total_packets_3
  .stat_rx_total_good_bytes_0(),              // output wire [13 : 0] stat_rx_total_good_bytes_0
  .stat_rx_total_good_bytes_1(),              // output wire [13 : 0] stat_rx_total_good_bytes_1
  .stat_rx_total_good_bytes_2(),              // output wire [13 : 0] stat_rx_total_good_bytes_2
  .stat_rx_total_good_bytes_3(),              // output wire [13 : 0] stat_rx_total_good_bytes_3
  .stat_rx_total_good_packets_0(),          // output wire stat_rx_total_good_packets_0
  .stat_rx_total_good_packets_1(),          // output wire stat_rx_total_good_packets_1
  .stat_rx_total_good_packets_2(),          // output wire stat_rx_total_good_packets_2
  .stat_rx_total_good_packets_3(),          // output wire stat_rx_total_good_packets_3
  .stat_rx_packet_bad_fcs_0(),                  // output wire stat_rx_packet_bad_fcs_0
  .stat_rx_packet_bad_fcs_1(),                  // output wire stat_rx_packet_bad_fcs_1
  .stat_rx_packet_bad_fcs_2(),                  // output wire stat_rx_packet_bad_fcs_2
  .stat_rx_packet_bad_fcs_3(),                  // output wire stat_rx_packet_bad_fcs_3
  .stat_rx_packet_64_bytes_0(),                // output wire stat_rx_packet_64_bytes_0
  .stat_rx_packet_64_bytes_1(),                // output wire stat_rx_packet_64_bytes_1
  .stat_rx_packet_64_bytes_2(),                // output wire stat_rx_packet_64_bytes_2
  .stat_rx_packet_64_bytes_3(),                // output wire stat_rx_packet_64_bytes_3
  .stat_rx_packet_65_127_bytes_0(),        // output wire stat_rx_packet_65_127_bytes_0
  .stat_rx_packet_65_127_bytes_1(),        // output wire stat_rx_packet_65_127_bytes_1
  .stat_rx_packet_65_127_bytes_2(),        // output wire stat_rx_packet_65_127_bytes_2
  .stat_rx_packet_65_127_bytes_3(),        // output wire stat_rx_packet_65_127_bytes_3
  .stat_rx_packet_128_255_bytes_0(),      // output wire stat_rx_packet_128_255_bytes_0
  .stat_rx_packet_128_255_bytes_1(),      // output wire stat_rx_packet_128_255_bytes_1
  .stat_rx_packet_128_255_bytes_2(),      // output wire stat_rx_packet_128_255_bytes_2
  .stat_rx_packet_128_255_bytes_3(),      // output wire stat_rx_packet_128_255_bytes_3
  .stat_rx_packet_256_511_bytes_0(),      // output wire stat_rx_packet_256_511_bytes_0
  .stat_rx_packet_256_511_bytes_1(),      // output wire stat_rx_packet_256_511_bytes_1
  .stat_rx_packet_256_511_bytes_2(),      // output wire stat_rx_packet_256_511_bytes_2
  .stat_rx_packet_256_511_bytes_3(),      // output wire stat_rx_packet_256_511_bytes_3
  .stat_rx_packet_512_1023_bytes_0(),    // output wire stat_rx_packet_512_1023_bytes_0
  .stat_rx_packet_512_1023_bytes_1(),    // output wire stat_rx_packet_512_1023_bytes_1
  .stat_rx_packet_512_1023_bytes_2(),    // output wire stat_rx_packet_512_1023_bytes_2
  .stat_rx_packet_512_1023_bytes_3(),    // output wire stat_rx_packet_512_1023_bytes_3
  .stat_rx_packet_1024_1518_bytes_0(),  // output wire stat_rx_packet_1024_1518_bytes_0
  .stat_rx_packet_1024_1518_bytes_1(),  // output wire stat_rx_packet_1024_1518_bytes_1
  .stat_rx_packet_1024_1518_bytes_2(),  // output wire stat_rx_packet_1024_1518_bytes_2
  .stat_rx_packet_1024_1518_bytes_3(),  // output wire stat_rx_packet_1024_1518_bytes_3
  .stat_rx_packet_1519_1522_bytes_0(),  // output wire stat_rx_packet_1519_1522_bytes_0
  .stat_rx_packet_1519_1522_bytes_1(),  // output wire stat_rx_packet_1519_1522_bytes_1
  .stat_rx_packet_1519_1522_bytes_2(),  // output wire stat_rx_packet_1519_1522_bytes_2
  .stat_rx_packet_1519_1522_bytes_3(),  // output wire stat_rx_packet_1519_1522_bytes_3
  .stat_rx_packet_1523_1548_bytes_0(),  // output wire stat_rx_packet_1523_1548_bytes_0
  .stat_rx_packet_1523_1548_bytes_1(),  // output wire stat_rx_packet_1523_1548_bytes_1
  .stat_rx_packet_1523_1548_bytes_2(),  // output wire stat_rx_packet_1523_1548_bytes_2
  .stat_rx_packet_1523_1548_bytes_3(),  // output wire stat_rx_packet_1523_1548_bytes_3
  .stat_rx_packet_1549_2047_bytes_0(),  // output wire stat_rx_packet_1549_2047_bytes_0
  .stat_rx_packet_1549_2047_bytes_1(),  // output wire stat_rx_packet_1549_2047_bytes_1
  .stat_rx_packet_1549_2047_bytes_2(),  // output wire stat_rx_packet_1549_2047_bytes_2
  .stat_rx_packet_1549_2047_bytes_3(),  // output wire stat_rx_packet_1549_2047_bytes_3
  .stat_rx_packet_2048_4095_bytes_0(),  // output wire stat_rx_packet_2048_4095_bytes_0
  .stat_rx_packet_2048_4095_bytes_1(),  // output wire stat_rx_packet_2048_4095_bytes_1
  .stat_rx_packet_2048_4095_bytes_2(),  // output wire stat_rx_packet_2048_4095_bytes_2
  .stat_rx_packet_2048_4095_bytes_3(),  // output wire stat_rx_packet_2048_4095_bytes_3
  .stat_rx_packet_4096_8191_bytes_0(),  // output wire stat_rx_packet_4096_8191_bytes_0
  .stat_rx_packet_4096_8191_bytes_1(),  // output wire stat_rx_packet_4096_8191_bytes_1
  .stat_rx_packet_4096_8191_bytes_2(),  // output wire stat_rx_packet_4096_8191_bytes_2
  .stat_rx_packet_4096_8191_bytes_3(),  // output wire stat_rx_packet_4096_8191_bytes_3
  .stat_rx_packet_8192_9215_bytes_0(),  // output wire stat_rx_packet_8192_9215_bytes_0
  .stat_rx_packet_8192_9215_bytes_1(),  // output wire stat_rx_packet_8192_9215_bytes_1
  .stat_rx_packet_8192_9215_bytes_2(),  // output wire stat_rx_packet_8192_9215_bytes_2
  .stat_rx_packet_8192_9215_bytes_3(),  // output wire stat_rx_packet_8192_9215_bytes_3
  .stat_rx_packet_small_0(),                      // output wire stat_rx_packet_small_0
  .stat_rx_packet_small_1(),                      // output wire stat_rx_packet_small_1
  .stat_rx_packet_small_2(),                      // output wire stat_rx_packet_small_2
  .stat_rx_packet_small_3(),                      // output wire stat_rx_packet_small_3
  .stat_rx_packet_large_0(),                      // output wire stat_rx_packet_large_0
  .stat_rx_packet_large_1(),                      // output wire stat_rx_packet_large_1
  .stat_rx_packet_large_2(),                      // output wire stat_rx_packet_large_2
  .stat_rx_packet_large_3(),                      // output wire stat_rx_packet_large_3
  .stat_rx_unicast_0(),                                // output wire stat_rx_unicast_0
  .stat_rx_unicast_1(),                                // output wire stat_rx_unicast_1
  .stat_rx_unicast_2(),                                // output wire stat_rx_unicast_2
  .stat_rx_unicast_3(),                                // output wire stat_rx_unicast_3
  .stat_rx_multicast_0(),                            // output wire stat_rx_multicast_0
  .stat_rx_multicast_1(),                            // output wire stat_rx_multicast_1
  .stat_rx_multicast_2(),                            // output wire stat_rx_multicast_2
  .stat_rx_multicast_3(),                            // output wire stat_rx_multicast_3
  .stat_rx_broadcast_0(),                            // output wire stat_rx_broadcast_0
  .stat_rx_broadcast_1(),                            // output wire stat_rx_broadcast_1
  .stat_rx_broadcast_2(),                            // output wire stat_rx_broadcast_2
  .stat_rx_broadcast_3(),                            // output wire stat_rx_broadcast_3
  .stat_rx_oversize_0(),                              // output wire stat_rx_oversize_0
  .stat_rx_oversize_1(),                              // output wire stat_rx_oversize_1
  .stat_rx_oversize_2(),                              // output wire stat_rx_oversize_2
  .stat_rx_oversize_3(),                              // output wire stat_rx_oversize_3
  .stat_rx_toolong_0(),                                // output wire stat_rx_toolong_0
  .stat_rx_toolong_1(),                                // output wire stat_rx_toolong_1
  .stat_rx_toolong_2(),                                // output wire stat_rx_toolong_2
  .stat_rx_toolong_3(),                                // output wire stat_rx_toolong_3
  .stat_rx_undersize_0(),                            // output wire stat_rx_undersize_0
  .stat_rx_undersize_1(),                            // output wire stat_rx_undersize_1
  .stat_rx_undersize_2(),                            // output wire stat_rx_undersize_2
  .stat_rx_undersize_3(),                            // output wire stat_rx_undersize_3
  .stat_rx_fragment_0(),                              // output wire stat_rx_fragment_0
  .stat_rx_fragment_1(),                              // output wire stat_rx_fragment_1
  .stat_rx_fragment_2(),                              // output wire stat_rx_fragment_2
  .stat_rx_fragment_3(),                              // output wire stat_rx_fragment_3
  .stat_rx_vlan_0(),                                      // output wire stat_rx_vlan_0
  .stat_rx_vlan_1(),                                      // output wire stat_rx_vlan_1
  .stat_rx_vlan_2(),                                      // output wire stat_rx_vlan_2
  .stat_rx_vlan_3(),                                      // output wire stat_rx_vlan_3
  .stat_rx_inrangeerr_0(),
  .stat_rx_inrangeerr_1(),
  .stat_rx_inrangeerr_2(),
  .stat_rx_inrangeerr_3(),
  .stat_rx_jabber_0(),                                  // output wire stat_rx_jabber_0
  .stat_rx_jabber_1(),                                  // output wire stat_rx_jabber_1
  .stat_rx_jabber_2(),                                  // output wire stat_rx_jabber_2
  .stat_rx_jabber_3(),                                  // output wire stat_rx_jabber_3
  .stat_rx_bad_code_0(),                              // output wire stat_rx_bad_code_0
  .stat_rx_bad_code_1(),                              // output wire stat_rx_bad_code_1
  .stat_rx_bad_code_2(),                              // output wire stat_rx_bad_code_2
  .stat_rx_bad_code_3(),                              // output wire stat_rx_bad_code_3
  .stat_rx_bad_sfd_0(),                                // output wire stat_rx_bad_sfd_0
  .stat_rx_bad_sfd_1(),                                // output wire stat_rx_bad_sfd_1
  .stat_rx_bad_sfd_2(),                                // output wire stat_rx_bad_sfd_2
  .stat_rx_bad_sfd_3(),                                // output wire stat_rx_bad_sfd_3
  .stat_rx_bad_preamble_0(),                      // output wire stat_rx_bad_preamble_0
  .stat_rx_bad_preamble_1(),                      // output wire stat_rx_bad_preamble_1
  .stat_rx_bad_preamble_2(),                      // output wire stat_rx_bad_preamble_2
  .stat_rx_bad_preamble_3(),                      // output wire stat_rx_bad_preamble_3
  .tx_reset_0(1'b0),                                              // input wire tx_reset_0
  .tx_reset_1(1'b0),                                              // input wire tx_reset_1
  .tx_reset_2(1'b0),                                              // input wire tx_reset_2
  .tx_reset_3(1'b0),                                              // input wire tx_reset_3
  .user_tx_reset_0(tx_reset[0]),                                    // output wire user_tx_reset_0
  .user_tx_reset_1(tx_reset[1]),                                    // output wire user_tx_reset_1
  .user_tx_reset_2(tx_reset[2]),                                    // output wire user_tx_reset_2
  .user_tx_reset_3(tx_reset[3]),                                    // output wire user_tx_reset_3
  .tx_axis_tready_0(axi_str_txd_tready[0]),                                  // output wire tx_axis_tready_0
  .tx_axis_tready_1(axi_str_txd_tready[1]),                                  // output wire tx_axis_tready_1
  .tx_axis_tready_2(axi_str_txd_tready[2]),                                  // output wire tx_axis_tready_2
  .tx_axis_tready_3(axi_str_txd_tready[3]),                                  // output wire tx_axis_tready_3
  .tx_axis_tvalid_0(axi_str_txd_tvalid[0]),                                  // input wire tx_axis_tvalid_0
  .tx_axis_tvalid_1(axi_str_txd_tvalid[1]),                                  // input wire tx_axis_tvalid_1
  .tx_axis_tvalid_2(axi_str_txd_tvalid[2]),                                  // input wire tx_axis_tvalid_2
  .tx_axis_tvalid_3(axi_str_txd_tvalid[3]),                                  // input wire tx_axis_tvalid_3
  .tx_axis_tdata_0(axi_str_txd_tdata[0]),                                    // input wire [63 : 0] tx_axis_tdata_0
  .tx_axis_tdata_1(axi_str_txd_tdata[1]),                                    // input wire [63 : 0] tx_axis_tdata_1
  .tx_axis_tdata_2(axi_str_txd_tdata[2]),                                    // input wire [63 : 0] tx_axis_tdata_2
  .tx_axis_tdata_3(axi_str_txd_tdata[3]),                                    // input wire [63 : 0] tx_axis_tdata_3
  .tx_axis_tlast_0(axi_str_txd_tlast[0]),                                    // input wire tx_axis_tlast_0
  .tx_axis_tlast_1(axi_str_txd_tlast[1]),                                    // input wire tx_axis_tlast_1
  .tx_axis_tlast_2(axi_str_txd_tlast[2]),                                    // input wire tx_axis_tlast_2
  .tx_axis_tlast_3(axi_str_txd_tlast[3]),                                    // input wire tx_axis_tlast_3
  .tx_axis_tkeep_0(axi_str_txd_tkeep[0]),                                    // input wire [7 : 0] tx_axis_tkeep_0
  .tx_axis_tkeep_1(axi_str_txd_tkeep[1]),                                    // input wire [7 : 0] tx_axis_tkeep_1
  .tx_axis_tkeep_2(axi_str_txd_tkeep[2]),                                    // input wire [7 : 0] tx_axis_tkeep_2
  .tx_axis_tkeep_3(axi_str_txd_tkeep[3]),                                    // input wire [7 : 0] tx_axis_tkeep_3
  .tx_axis_tuser_0(1'b0),                                    // input wire tx_axis_tuser_0
  .tx_axis_tuser_1(1'b0),                                    // input wire tx_axis_tuser_1
  .tx_axis_tuser_2(1'b0),                                    // input wire tx_axis_tuser_2
  .tx_axis_tuser_3(1'b0),                                    // input wire tx_axis_tuser_3
  .tx_unfout_0(),                                            // output wire tx_unfout_0
  .tx_unfout_1(),                                            // output wire tx_unfout_1
  .tx_unfout_2(),                                            // output wire tx_unfout_2
  .tx_unfout_3(),                                            // output wire tx_unfout_3
  .tx_preamblein_0(56'd0),                                    // input wire [55 : 0] tx_preamblein_0
  .rx_preambleout_0(),                                  // output wire [55 : 0] rx_preambleout_0
  .tx_preamblein_1(56'd0),                                    // input wire [55 : 0] tx_preamblein_1
  .rx_preambleout_1(),                                  // output wire [55 : 0] rx_preambleout_1
  .tx_preamblein_2(56'd0),                                    // input wire [55 : 0] tx_preamblein_2
  .rx_preambleout_2(),                                  // output wire [55 : 0] rx_preambleout_2
  .tx_preamblein_3(56'd0),                                    // input wire [55 : 0] tx_preamblein_3
  .rx_preambleout_3(),                                  // output wire [55 : 0] rx_preambleout_3
  .stat_tx_local_fault_0(),                        // output wire stat_tx_local_fault_0
  .stat_tx_local_fault_1(),                        // output wire stat_tx_local_fault_1
  .stat_tx_local_fault_2(),                        // output wire stat_tx_local_fault_2
  .stat_tx_local_fault_3(),                        // output wire stat_tx_local_fault_3
  .stat_tx_total_bytes_0(),                        // output wire [3 : 0] stat_tx_total_bytes_0
  .stat_tx_total_bytes_1(),                        // output wire [3 : 0] stat_tx_total_bytes_1
  .stat_tx_total_bytes_2(),                        // output wire [3 : 0] stat_tx_total_bytes_2
  .stat_tx_total_bytes_3(),                        // output wire [3 : 0] stat_tx_total_bytes_3
  .stat_tx_total_packets_0(),                    // output wire stat_tx_total_packets_0
  .stat_tx_total_packets_1(),                    // output wire stat_tx_total_packets_1
  .stat_tx_total_packets_2(),                    // output wire stat_tx_total_packets_2
  .stat_tx_total_packets_3(),                    // output wire stat_tx_total_packets_3
  .stat_tx_total_good_bytes_0(),              // output wire [13 : 0] stat_tx_total_good_bytes_0
  .stat_tx_total_good_bytes_1(),              // output wire [13 : 0] stat_tx_total_good_bytes_1
  .stat_tx_total_good_bytes_2(),              // output wire [13 : 0] stat_tx_total_good_bytes_2
  .stat_tx_total_good_bytes_3(),              // output wire [13 : 0] stat_tx_total_good_bytes_3
  .stat_tx_total_good_packets_0(),          // output wire stat_tx_total_good_packets_0
  .stat_tx_total_good_packets_1(),          // output wire stat_tx_total_good_packets_1
  .stat_tx_total_good_packets_2(),          // output wire stat_tx_total_good_packets_2
  .stat_tx_total_good_packets_3(),          // output wire stat_tx_total_good_packets_3
  .stat_tx_bad_fcs_0(),                                // output wire stat_tx_bad_fcs_0
  .stat_tx_bad_fcs_1(),                                // output wire stat_tx_bad_fcs_1
  .stat_tx_bad_fcs_2(),                                // output wire stat_tx_bad_fcs_2
  .stat_tx_bad_fcs_3(),                                // output wire stat_tx_bad_fcs_3
  .stat_tx_packet_64_bytes_0(),                // output wire stat_tx_packet_64_bytes_0
  .stat_tx_packet_64_bytes_1(),                // output wire stat_tx_packet_64_bytes_1
  .stat_tx_packet_64_bytes_2(),                // output wire stat_tx_packet_64_bytes_2
  .stat_tx_packet_64_bytes_3(),                // output wire stat_tx_packet_64_bytes_3
  .stat_tx_packet_65_127_bytes_0(),        // output wire stat_tx_packet_65_127_bytes_0
  .stat_tx_packet_65_127_bytes_1(),        // output wire stat_tx_packet_65_127_bytes_1
  .stat_tx_packet_65_127_bytes_2(),        // output wire stat_tx_packet_65_127_bytes_2
  .stat_tx_packet_65_127_bytes_3(),        // output wire stat_tx_packet_65_127_bytes_3
  .stat_tx_packet_128_255_bytes_0(),      // output wire stat_tx_packet_128_255_bytes_0
  .stat_tx_packet_128_255_bytes_1(),      // output wire stat_tx_packet_128_255_bytes_1
  .stat_tx_packet_128_255_bytes_2(),      // output wire stat_tx_packet_128_255_bytes_2
  .stat_tx_packet_128_255_bytes_3(),      // output wire stat_tx_packet_128_255_bytes_3
  .stat_tx_packet_256_511_bytes_0(),      // output wire stat_tx_packet_256_511_bytes_0
  .stat_tx_packet_256_511_bytes_1(),      // output wire stat_tx_packet_256_511_bytes_1
  .stat_tx_packet_256_511_bytes_2(),      // output wire stat_tx_packet_256_511_bytes_2
  .stat_tx_packet_256_511_bytes_3(),      // output wire stat_tx_packet_256_511_bytes_3
  .stat_tx_packet_512_1023_bytes_0(),    // output wire stat_tx_packet_512_1023_bytes_0
  .stat_tx_packet_512_1023_bytes_1(),    // output wire stat_tx_packet_512_1023_bytes_1
  .stat_tx_packet_512_1023_bytes_2(),    // output wire stat_tx_packet_512_1023_bytes_2
  .stat_tx_packet_512_1023_bytes_3(),    // output wire stat_tx_packet_512_1023_bytes_3
  .stat_tx_packet_1024_1518_bytes_0(),  // output wire stat_tx_packet_1024_1518_bytes_0
  .stat_tx_packet_1024_1518_bytes_1(),  // output wire stat_tx_packet_1024_1518_bytes_1
  .stat_tx_packet_1024_1518_bytes_2(),  // output wire stat_tx_packet_1024_1518_bytes_2
  .stat_tx_packet_1024_1518_bytes_3(),  // output wire stat_tx_packet_1024_1518_bytes_3
  .stat_tx_packet_1519_1522_bytes_0(),  // output wire stat_tx_packet_1519_1522_bytes_0
  .stat_tx_packet_1519_1522_bytes_1(),  // output wire stat_tx_packet_1519_1522_bytes_1
  .stat_tx_packet_1519_1522_bytes_2(),  // output wire stat_tx_packet_1519_1522_bytes_2
  .stat_tx_packet_1519_1522_bytes_3(),  // output wire stat_tx_packet_1519_1522_bytes_3
  .stat_tx_packet_1523_1548_bytes_0(),  // output wire stat_tx_packet_1523_1548_bytes_0
  .stat_tx_packet_1523_1548_bytes_1(),  // output wire stat_tx_packet_1523_1548_bytes_1
  .stat_tx_packet_1523_1548_bytes_2(),  // output wire stat_tx_packet_1523_1548_bytes_2
  .stat_tx_packet_1523_1548_bytes_3(),  // output wire stat_tx_packet_1523_1548_bytes_3
  .stat_tx_packet_1549_2047_bytes_0(),  // output wire stat_tx_packet_1549_2047_bytes_0
  .stat_tx_packet_1549_2047_bytes_1(),  // output wire stat_tx_packet_1549_2047_bytes_1
  .stat_tx_packet_1549_2047_bytes_2(),  // output wire stat_tx_packet_1549_2047_bytes_2
  .stat_tx_packet_1549_2047_bytes_3(),  // output wire stat_tx_packet_1549_2047_bytes_3
  .stat_tx_packet_2048_4095_bytes_0(),  // output wire stat_tx_packet_2048_4095_bytes_0
  .stat_tx_packet_2048_4095_bytes_1(),  // output wire stat_tx_packet_2048_4095_bytes_1
  .stat_tx_packet_2048_4095_bytes_2(),  // output wire stat_tx_packet_2048_4095_bytes_2
  .stat_tx_packet_2048_4095_bytes_3(),  // output wire stat_tx_packet_2048_4095_bytes_3
  .stat_tx_packet_4096_8191_bytes_0(),  // output wire stat_tx_packet_4096_8191_bytes_0
  .stat_tx_packet_4096_8191_bytes_1(),  // output wire stat_tx_packet_4096_8191_bytes_1
  .stat_tx_packet_4096_8191_bytes_2(),  // output wire stat_tx_packet_4096_8191_bytes_2
  .stat_tx_packet_4096_8191_bytes_3(),  // output wire stat_tx_packet_4096_8191_bytes_3
  .stat_tx_packet_8192_9215_bytes_0(),  // output wire stat_tx_packet_8192_9215_bytes_0
  .stat_tx_packet_8192_9215_bytes_1(),  // output wire stat_tx_packet_8192_9215_bytes_1
  .stat_tx_packet_8192_9215_bytes_2(),  // output wire stat_tx_packet_8192_9215_bytes_2
  .stat_tx_packet_8192_9215_bytes_3(),  // output wire stat_tx_packet_8192_9215_bytes_3
  .stat_tx_packet_small_0(),                      // output wire stat_tx_packet_small_0
  .stat_tx_packet_small_1(),                      // output wire stat_tx_packet_small_1
  .stat_tx_packet_small_2(),                      // output wire stat_tx_packet_small_2
  .stat_tx_packet_small_3(),                      // output wire stat_tx_packet_small_3
  .stat_tx_packet_large_0(),                      // output wire stat_tx_packet_large_0
  .stat_tx_packet_large_1(),                      // output wire stat_tx_packet_large_1
  .stat_tx_packet_large_2(),                      // output wire stat_tx_packet_large_2
  .stat_tx_packet_large_3(),                      // output wire stat_tx_packet_large_3
  .stat_tx_unicast_0(),                                // output wire stat_tx_unicast_0
  .stat_tx_unicast_1(),                                // output wire stat_tx_unicast_1
  .stat_tx_unicast_2(),                                // output wire stat_tx_unicast_2
  .stat_tx_unicast_3(),                                // output wire stat_tx_unicast_3
  .stat_tx_multicast_0(),                            // output wire stat_tx_multicast_0
  .stat_tx_multicast_1(),                            // output wire stat_tx_multicast_1
  .stat_tx_multicast_2(),                            // output wire stat_tx_multicast_2
  .stat_tx_multicast_3(),                            // output wire stat_tx_multicast_3
  .stat_tx_broadcast_0(),                            // output wire stat_tx_broadcast_0
  .stat_tx_broadcast_1(),                            // output wire stat_tx_broadcast_1
  .stat_tx_broadcast_2(),                            // output wire stat_tx_broadcast_2
  .stat_tx_broadcast_3(),                            // output wire stat_tx_broadcast_3
  .stat_tx_vlan_0(),                                      // output wire stat_tx_vlan_0
  .stat_tx_vlan_1(),                                      // output wire stat_tx_vlan_1
  .stat_tx_vlan_2(),                                      // output wire stat_tx_vlan_2
  .stat_tx_vlan_3(),                                      // output wire stat_tx_vlan_3
  .stat_tx_frame_error_0(),                        // output wire stat_tx_frame_error_0
  .stat_tx_frame_error_1(),                        // output wire stat_tx_frame_error_1
  .stat_tx_frame_error_2(),                        // output wire stat_tx_frame_error_2
  .stat_tx_frame_error_3(),                        // output wire stat_tx_frame_error_3
  .ctl_tx_enable_0(1'b1),                                    // input wire ctl_tx_enable_0
  .ctl_tx_enable_1(1'b1),                                    // input wire ctl_tx_enable_1
  .ctl_tx_enable_2(1'b1),                                    // input wire ctl_tx_enable_2
  .ctl_tx_enable_3(1'b1),                                    // input wire ctl_tx_enable_3
  .ctl_tx_send_rfi_0(1'b0),                                // input wire ctl_tx_send_rfi_0
  .ctl_tx_send_rfi_1(1'b0),                                // input wire ctl_tx_send_rfi_1
  .ctl_tx_send_rfi_2(1'b0),                                // input wire ctl_tx_send_rfi_2
  .ctl_tx_send_rfi_3(1'b0),                                // input wire ctl_tx_send_rfi_3
  .ctl_tx_send_lfi_0(1'b0),                                // input wire ctl_tx_send_lfi_0
  .ctl_tx_send_lfi_1(1'b0),                                // input wire ctl_tx_send_lfi_1
  .ctl_tx_send_lfi_2(1'b0),                                // input wire ctl_tx_send_lfi_2
  .ctl_tx_send_lfi_3(1'b0),                                // input wire ctl_tx_send_lfi_3
  .ctl_tx_send_idle_0(1'b0),                              // input wire ctl_tx_send_idle_0
  .ctl_tx_send_idle_1(1'b0),                              // input wire ctl_tx_send_idle_1
  .ctl_tx_send_idle_2(1'b0),                              // input wire ctl_tx_send_idle_2
  .ctl_tx_send_idle_3(1'b0),                              // input wire ctl_tx_send_idle_3
  .ctl_tx_fcs_ins_enable_0(1'b1),                    // input wire ctl_tx_fcs_ins_enable_0
  .ctl_tx_fcs_ins_enable_1(1'b1),                    // input wire ctl_tx_fcs_ins_enable_1
  .ctl_tx_fcs_ins_enable_2(1'b1),                    // input wire ctl_tx_fcs_ins_enable_2
  .ctl_tx_fcs_ins_enable_3(1'b1),                    // input wire ctl_tx_fcs_ins_enable_3
  .ctl_tx_ignore_fcs_0(1'b0),                            // input wire ctl_tx_ignore_fcs_0
  .ctl_tx_ignore_fcs_1(1'b0),                            // input wire ctl_tx_ignore_fcs_1
  .ctl_tx_ignore_fcs_2(1'b0),                            // input wire ctl_tx_ignore_fcs_2
  .ctl_tx_ignore_fcs_3(1'b0),                            // input wire ctl_tx_ignore_fcs_3
  .ctl_tx_test_pattern_0(1'b0),                        // input wire ctl_tx_test_pattern_0
  .ctl_tx_test_pattern_1(1'b0),                        // input wire ctl_tx_test_pattern_1
  .ctl_tx_test_pattern_2(1'b0),                        // input wire ctl_tx_test_pattern_2
  .ctl_tx_test_pattern_3(1'b0),                        // input wire ctl_tx_test_pattern_3
  .ctl_tx_test_pattern_enable_0(1'b0),          // input wire ctl_tx_test_pattern_enable_0
  .ctl_tx_test_pattern_enable_1(1'b0),          // input wire ctl_tx_test_pattern_enable_1
  .ctl_tx_test_pattern_enable_2(1'b0),          // input wire ctl_tx_test_pattern_enable_2
  .ctl_tx_test_pattern_enable_3(1'b0),          // input wire ctl_tx_test_pattern_enable_3
  .ctl_tx_test_pattern_select_0(1'b0),          // input wire ctl_tx_test_pattern_select_0
  .ctl_tx_test_pattern_select_1(1'b0),          // input wire ctl_tx_test_pattern_select_1
  .ctl_tx_test_pattern_select_2(1'b0),          // input wire ctl_tx_test_pattern_select_2
  .ctl_tx_test_pattern_select_3(1'b0),          // input wire ctl_tx_test_pattern_select_3
  .ctl_tx_data_pattern_select_0(1'b0),          // input wire ctl_tx_data_pattern_select_0
  .ctl_tx_data_pattern_select_1(1'b0),          // input wire ctl_tx_data_pattern_select_1
  .ctl_tx_data_pattern_select_2(1'b0),          // input wire ctl_tx_data_pattern_select_2
  .ctl_tx_data_pattern_select_3(1'b0),          // input wire ctl_tx_data_pattern_select_3
  .ctl_tx_test_pattern_seed_a_0(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_a_0
  .ctl_tx_test_pattern_seed_a_1(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_a_1
  .ctl_tx_test_pattern_seed_a_2(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_a_2
  .ctl_tx_test_pattern_seed_a_3(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_a_3
  .ctl_tx_test_pattern_seed_b_0(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_b_0
  .ctl_tx_test_pattern_seed_b_1(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_b_1
  .ctl_tx_test_pattern_seed_b_2(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_b_2
  .ctl_tx_test_pattern_seed_b_3(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_b_3
  .ctl_tx_ipg_value_0(4'd12),                              // input wire [3 : 0] ctl_tx_ipg_value_0
  .ctl_tx_ipg_value_1(4'd12),                              // input wire [3 : 0] ctl_tx_ipg_value_1
  .ctl_tx_ipg_value_2(4'd12),                              // input wire [3 : 0] ctl_tx_ipg_value_2
  .ctl_tx_ipg_value_3(4'd12),                              // input wire [3 : 0] ctl_tx_ipg_value_3
  .ctl_tx_custom_preamble_enable_0(1'b0),    // input wire ctl_tx_custom_preamble_enable_0
  .ctl_tx_custom_preamble_enable_1(1'b0),    // input wire ctl_tx_custom_preamble_enable_1
  .ctl_tx_custom_preamble_enable_2(1'b0),    // input wire ctl_tx_custom_preamble_enable_2
  .ctl_tx_custom_preamble_enable_3(1'b0),    // input wire ctl_tx_custom_preamble_enable_3
  .gt_loopback_in_0(gt_loopback[0]),                                  // input wire [2 : 0] gt_loopback_in_0
  .gt_loopback_in_1(gt_loopback[1]),                                  // input wire [2 : 0] gt_loopback_in_1
  .gt_loopback_in_2(gt_loopback[2]),                                  // input wire [2 : 0] gt_loopback_in_2
  .gt_loopback_in_3(gt_loopback[3])                                   // input wire [2 : 0] gt_loopback_in_3
);

axis_data_fifo_0 tx0_fifo (
  .s_axis_aresetn(fifo_resetn[0]),
  .s_axis_aclk(ap_clk),  
  .s_axis_tvalid(tx0_axis_tvalid),
  .s_axis_tready(tx0_axis_tready),
  .s_axis_tdata(tx0_axis_tdata),
  .s_axis_tkeep(tx0_axis_tkeep),
  .s_axis_tlast(tx0_axis_tlast),
  .s_axis_tuser(tx0_axis_tuser),
  .m_axis_aclk(tx_clk_out[0]),
  .m_axis_tvalid(axi_str_txd_tvalid[0]),
  .m_axis_tready(axi_str_txd_tready[0]),
  .m_axis_tdata(axi_str_txd_tdata[0]),
  .m_axis_tkeep(axi_str_txd_tkeep[0]),
  .m_axis_tlast(axi_str_txd_tlast[0]),
  .m_axis_tuser()
);

axis_data_fifo_0 rx0_fifo (
  .s_axis_aresetn(fifo_resetn[0]),
  .s_axis_aclk(rx_core_clk[0]),  
  .s_axis_tvalid(axi_str_rxd_tvalid[0]),
  .s_axis_tready(),
  .s_axis_tdata(axi_str_rxd_tdata[0]),
  .s_axis_tkeep(axi_str_rxd_tkeep[0]),
  .s_axis_tlast(axi_str_rxd_tlast[0]),
  .s_axis_tuser(axi_str_rxd_tuser[0]),
  .m_axis_aclk(ap_clk),
  .m_axis_tvalid(rx0_axis_tvalid),
  .m_axis_tready(rx0_axis_tready),
  .m_axis_tdata(rx0_axis_tdata),
  .m_axis_tkeep(rx0_axis_tkeep),
  .m_axis_tlast(rx0_axis_tlast),
  .m_axis_tuser(rx0_axis_tuser)
);

axis_data_fifo_0 tx1_fifo (
  .s_axis_aresetn(fifo_resetn[1]),
  .s_axis_aclk(ap_clk),  
  .s_axis_tvalid(tx1_axis_tvalid),
  .s_axis_tready(tx1_axis_tready),
  .s_axis_tdata(tx1_axis_tdata),
  .s_axis_tkeep(tx1_axis_tkeep),
  .s_axis_tlast(tx1_axis_tlast),
  .s_axis_tuser(tx1_axis_tuser),
  .m_axis_aclk(tx_clk_out[1]),
  .m_axis_tvalid(axi_str_txd_tvalid[1]),
  .m_axis_tready(axi_str_txd_tready[1]),
  .m_axis_tdata(axi_str_txd_tdata[1]),
  .m_axis_tkeep(axi_str_txd_tkeep[1]),
  .m_axis_tlast(axi_str_txd_tlast[1]),
  .m_axis_tuser()
);

axis_data_fifo_0 rx1_fifo (
  .s_axis_aresetn(fifo_resetn[1]),
  .s_axis_aclk(rx_core_clk[1]),  
  .s_axis_tvalid(axi_str_rxd_tvalid[1]),
  .s_axis_tready(),
  .s_axis_tdata(axi_str_rxd_tdata[1]),
  .s_axis_tkeep(axi_str_rxd_tkeep[1]),
  .s_axis_tlast(axi_str_rxd_tlast[1]),
  .s_axis_tuser(axi_str_rxd_tuser[1]),
  .m_axis_aclk(ap_clk),
  .m_axis_tvalid(rx1_axis_tvalid),
  .m_axis_tready(rx1_axis_tready),
  .m_axis_tdata(rx1_axis_tdata),
  .m_axis_tkeep(rx1_axis_tkeep),
  .m_axis_tlast(rx1_axis_tlast),
  .m_axis_tuser(rx1_axis_tuser)
);


axis_data_fifo_0 tx2_fifo (
  .s_axis_aresetn(fifo_resetn[2]),
  .s_axis_aclk(ap_clk),  
  .s_axis_tvalid(tx2_axis_tvalid),
  .s_axis_tready(tx2_axis_tready),
  .s_axis_tdata(tx2_axis_tdata),
  .s_axis_tkeep(tx2_axis_tkeep),
  .s_axis_tlast(tx2_axis_tlast),
  .s_axis_tuser(tx2_axis_tuser),
  .m_axis_aclk(tx_clk_out[2]),
  .m_axis_tvalid(axi_str_txd_tvalid[2]),
  .m_axis_tready(axi_str_txd_tready[2]),
  .m_axis_tdata(axi_str_txd_tdata[2]),
  .m_axis_tkeep(axi_str_txd_tkeep[2]),
  .m_axis_tlast(axi_str_txd_tlast[2]),
  .m_axis_tuser()
);

axis_data_fifo_0 rx2_fifo (
  .s_axis_aresetn(fifo_resetn[2]),
  .s_axis_aclk(rx_core_clk[2]),  
  .s_axis_tvalid(axi_str_rxd_tvalid[2]),
  .s_axis_tready(),
  .s_axis_tdata(axi_str_rxd_tdata[2]),
  .s_axis_tkeep(axi_str_rxd_tkeep[2]),
  .s_axis_tlast(axi_str_rxd_tlast[2]),
  .s_axis_tuser(axi_str_rxd_tuser[2]),
  .m_axis_aclk(ap_clk),
  .m_axis_tvalid(rx2_axis_tvalid),
  .m_axis_tready(rx2_axis_tready),
  .m_axis_tdata(rx2_axis_tdata),
  .m_axis_tkeep(rx2_axis_tkeep),
  .m_axis_tlast(rx2_axis_tlast),
  .m_axis_tuser(rx2_axis_tuser)
);


axis_data_fifo_0 tx3_fifo (
  .s_axis_aresetn(fifo_resetn[3]),
  .s_axis_aclk(ap_clk),  
  .s_axis_tvalid(tx3_axis_tvalid),
  .s_axis_tready(tx3_axis_tready),
  .s_axis_tdata(tx3_axis_tdata),
  .s_axis_tkeep(tx3_axis_tkeep),
  .s_axis_tlast(tx3_axis_tlast),
  .s_axis_tuser(tx3_axis_tuser),
  .m_axis_aclk(tx_clk_out[3]),
  .m_axis_tvalid(axi_str_txd_tvalid[3]),
  .m_axis_tready(axi_str_txd_tready[3]),
  .m_axis_tdata(axi_str_txd_tdata[3]),
  .m_axis_tkeep(axi_str_txd_tkeep[3]),
  .m_axis_tlast(axi_str_txd_tlast[3]),
  .m_axis_tuser()
);

axis_data_fifo_0 rx3_fifo (
  .s_axis_aresetn(fifo_resetn[3]),
  .s_axis_aclk(rx_core_clk[3]),  
  .s_axis_tvalid(axi_str_rxd_tvalid[3]),
  .s_axis_tready(),
  .s_axis_tdata(axi_str_rxd_tdata[3]),
  .s_axis_tkeep(axi_str_rxd_tkeep[3]),
  .s_axis_tlast(axi_str_rxd_tlast[3]),
  .s_axis_tuser(axi_str_rxd_tuser[3]),
  .m_axis_aclk(ap_clk),
  .m_axis_tvalid(rx3_axis_tvalid),
  .m_axis_tready(rx3_axis_tready),
  .m_axis_tdata(rx3_axis_tdata),
  .m_axis_tkeep(rx3_axis_tkeep),
  .m_axis_tlast(rx3_axis_tlast),
  .m_axis_tuser(rx3_axis_tuser)
);

endmodule
`default_nettype wire
