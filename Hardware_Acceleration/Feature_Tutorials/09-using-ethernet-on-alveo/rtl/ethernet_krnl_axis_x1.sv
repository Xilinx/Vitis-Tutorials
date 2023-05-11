/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


////////////////////////////////////////////////////////////////////////////////
// default_nettype of none prevents implicit wire declaration.
`default_nettype none
`timescale 1 ns / 1 ps
// Top level of the kernel. Do not modify module name, parameters or ports.
module ethernet_krnl_axis_x1 #(
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
  output wire rx0_axis_tvalid,
  output wire [63:0] rx0_axis_tdata,
  output wire rx0_axis_tlast,
  output wire [7:0] rx0_axis_tkeep,
  output wire rx0_axis_tuser,
  input  wire rx0_axis_tready,

  output wire tx0_axis_tready,
  input  wire tx0_axis_tvalid,
  input wire [63:0] tx0_axis_tdata,
  input wire tx0_axis_tlast,
  input wire [7:0] tx0_axis_tkeep,
  input wire tx0_axis_tuser,
  //
  input  wire clk_gt_freerun,
  input  wire [3:0] gt_rxp_in,
  input  wire [3:0] gt_rxn_in,
  output wire [3:0] gt_txp_out,
  output wire [3:0] gt_txn_out,
  input  wire gt_refclk_p,
  input  wire gt_refclk_n
);

localparam C_NUM_LANES = 1;

///////////////////////////////////////////////////////////////////////////////
// Wires and Variables
///////////////////////////////////////////////////////////////////////////////
(* DONT_TOUCH = "yes" *)
logic areset = 1'b0;

logic [C_NUM_LANES-1:0] axi_str_txd_tvalid;
logic [C_NUM_LANES-1:0] axi_str_txd_tready;
logic [C_NUM_LANES-1:0] axi_str_txd_tlast;
logic [C_NUM_LANES-1:0] [7:0] axi_str_txd_tkeep;
logic [C_NUM_LANES-1:0] axi_str_txd_tuser; // Unused. We never force an error on Tx.
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
  .rx_block_lock( ap_stat_rx_block_lock_sync)
);

xpm_cdc_single #(
    .DEST_SYNC_FF(4),
    .INIT_SYNC_FF(0),
    .SIM_ASSERT_CHK(0),
    .SRC_INPUT_REG(0)
) ap_rx_block_lock_sync_i (
    .dest_out(ap_stat_rx_block_lock_sync),
    .dest_clk(ap_clk),
    .src_clk(1'b0),
    .src_in(stat_rx_block_lock[0])
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
xxv_ethernet_0 i_ethernet_0 (
  .gt_txp_out(gt_txp_out[0]),                                            // output wire gt_txp_out
  .gt_txn_out(gt_txn_out[0]),                                            // output wire gt_txn_out
  .gt_rxp_in(gt_rxp_in[0]),                                              // input wire gt_rxp_in
  .gt_rxn_in(gt_rxn_in[0]),                                              // input wire gt_rxn_in
  .rx_core_clk_0(rx_core_clk[0]),                                        // input wire rx_core_clk_0
  .txoutclksel_in_0(3'd5),                                              // input wire [2 : 0] txoutclksel_in_0
  .rxoutclksel_in_0(3'd5),                                              // input wire [2 : 0] rxoutclksel_in_0
  .gtwiz_reset_tx_datapath_0(gtwiz_reset_tx_datapath[0]),                                     // input wire gtwiz_reset_tx_datapath_0
  .gtwiz_reset_rx_datapath_0(gtwiz_reset_rx_datapath[0]),                  // input wire gtwiz_reset_rx_datapath_0
  .rxrecclkout_0(),                                                     // output wire rxrecclkout_0
  .sys_reset(areset),                                                   // input wire sys_reset
  .dclk(clk_gt_freerun),                                                // input wire dclk
  .tx_clk_out_0(tx_clk_out[0]),                                          // output wire tx_clk_out_0
  .rx_clk_out_0(),                                                      // output wire rx_clk_out_0
  .gt_refclk_p(gt_refclk_p),                                           // input wire gt_refclk_p
  .gt_refclk_n(gt_refclk_n),                                           // input wire gt_refclk_n
  .gt_refclk_out(),                                                     // output wire gt_refclk_out
  .gtpowergood_out_0(),                                                 // output wire gtpowergood_out_0
  .rx_reset_0(1'b0),                                                             // input wire rx_reset_0
  .user_rx_reset_0(rx_reset[0]),                                               // output wire user_rx_reset_0
  .rx_axis_tvalid_0(axi_str_rxd_tvalid[0]),                                    // output wire rx_axis_tvalid_0
  .rx_axis_tdata_0(axi_str_rxd_tdata[0]),                                      // output wire [63 : 0] rx_axis_tdata_0
  .rx_axis_tlast_0(axi_str_rxd_tlast[0]),                                      // output wire rx_axis_tlast_0
  .rx_axis_tkeep_0(axi_str_rxd_tkeep[0]),                                      // output wire [7 : 0] rx_axis_tkeep_0
  .rx_axis_tuser_0(axi_str_rxd_tuser[0]),                                      // output wire rx_axis_tuser_0
  .ctl_rx_enable_0(1'b1),                                    // input wire ctl_rx_enable_0
  .ctl_rx_check_preamble_0(1'b1),                    // input wire ctl_rx_check_preamble_0
  .ctl_rx_check_sfd_0(1'b1),                              // input wire ctl_rx_check_sfd_0
  .ctl_rx_force_resync_0(1'b0),                        // input wire ctl_rx_force_resync_0
  .ctl_rx_delete_fcs_0(1'b1),                            // input wire ctl_rx_delete_fcs_0
  .ctl_rx_ignore_fcs_0(1'b0),                            // input wire ctl_rx_ignore_fcs_0
  .ctl_rx_max_packet_len_0(15'd9600),                    // input wire [14 : 0] ctl_rx_max_packet_len_0
  .ctl_rx_min_packet_len_0(8'd64),                    // input wire [7 : 0] ctl_rx_min_packet_len_0
  .ctl_rx_process_lfi_0(1'b0),                          // input wire ctl_rx_process_lfi_0
  .ctl_rx_test_pattern_0(1'b0),                        // input wire ctl_rx_test_pattern_0
  .ctl_rx_data_pattern_select_0(1'b0),          // input wire ctl_rx_data_pattern_select_0
  .ctl_rx_test_pattern_enable_0(1'b0),          // input wire ctl_rx_test_pattern_enable_0
  .ctl_rx_custom_preamble_enable_0(1'b0),    // input wire ctl_rx_custom_preamble_enable_0
  .stat_rx_framing_err_0(),                        // output wire stat_rx_framing_err_0
  .stat_rx_framing_err_valid_0(),            // output wire stat_rx_framing_err_valid_0
  .stat_rx_local_fault_0(),                        // output wire stat_rx_local_fault_0
  .stat_rx_block_lock_0(stat_rx_block_lock[0]),                          // output wire stat_rx_block_lock_0
  .stat_rx_valid_ctrl_code_0(),                // output wire stat_rx_valid_ctrl_code_0
  .stat_rx_status_0(),                                  // output wire stat_rx_status_0
  .stat_rx_remote_fault_0(),                      // output wire stat_rx_remote_fault_0
  .stat_rx_bad_fcs_0(),                                // output wire [1 : 0] stat_rx_bad_fcs_0
  .stat_rx_stomped_fcs_0(),                        // output wire [1 : 0] stat_rx_stomped_fcs_0
  .stat_rx_truncated_0(),                            // output wire stat_rx_truncated_0
  .stat_rx_internal_local_fault_0(),      // output wire stat_rx_internal_local_fault_0
  .stat_rx_received_local_fault_0(),      // output wire stat_rx_received_local_fault_0
  .stat_rx_hi_ber_0(),                                  // output wire stat_rx_hi_ber_0
  .stat_rx_got_signal_os_0(),                    // output wire stat_rx_got_signal_os_0
  .stat_rx_test_pattern_mismatch_0(),    // output wire stat_rx_test_pattern_mismatch_0
  .stat_rx_total_bytes_0(),                        // output wire [3 : 0] stat_rx_total_bytes_0
  .stat_rx_total_packets_0(),                    // output wire [1 : 0] stat_rx_total_packets_0
  .stat_rx_total_good_bytes_0(),              // output wire [13 : 0] stat_rx_total_good_bytes_0
  .stat_rx_total_good_packets_0(),          // output wire stat_rx_total_good_packets_0
  .stat_rx_packet_bad_fcs_0(),                  // output wire stat_rx_packet_bad_fcs_0
  .stat_rx_packet_64_bytes_0(),                // output wire stat_rx_packet_64_bytes_0
  .stat_rx_packet_65_127_bytes_0(),        // output wire stat_rx_packet_65_127_bytes_0
  .stat_rx_packet_128_255_bytes_0(),      // output wire stat_rx_packet_128_255_bytes_0
  .stat_rx_packet_256_511_bytes_0(),      // output wire stat_rx_packet_256_511_bytes_0
  .stat_rx_packet_512_1023_bytes_0(),    // output wire stat_rx_packet_512_1023_bytes_0
  .stat_rx_packet_1024_1518_bytes_0(),  // output wire stat_rx_packet_1024_1518_bytes_0
  .stat_rx_packet_1519_1522_bytes_0(),  // output wire stat_rx_packet_1519_1522_bytes_0
  .stat_rx_packet_1523_1548_bytes_0(),  // output wire stat_rx_packet_1523_1548_bytes_0
  .stat_rx_packet_1549_2047_bytes_0(),  // output wire stat_rx_packet_1549_2047_bytes_0
  .stat_rx_packet_2048_4095_bytes_0(),  // output wire stat_rx_packet_2048_4095_bytes_0
  .stat_rx_packet_4096_8191_bytes_0(),  // output wire stat_rx_packet_4096_8191_bytes_0
  .stat_rx_packet_8192_9215_bytes_0(),  // output wire stat_rx_packet_8192_9215_bytes_0
  .stat_rx_packet_small_0(),                      // output wire stat_rx_packet_small_0
  .stat_rx_packet_large_0(),                      // output wire stat_rx_packet_large_0
  .stat_rx_unicast_0(),                                // output wire stat_rx_unicast_0
  .stat_rx_multicast_0(),                            // output wire stat_rx_multicast_0
  .stat_rx_broadcast_0(),                            // output wire stat_rx_broadcast_0
  .stat_rx_oversize_0(),                              // output wire stat_rx_oversize_0
  .stat_rx_toolong_0(),                                // output wire stat_rx_toolong_0
  .stat_rx_undersize_0(),                            // output wire stat_rx_undersize_0
  .stat_rx_fragment_0(),                              // output wire stat_rx_fragment_0
  .stat_rx_vlan_0(),                                      // output wire stat_rx_vlan_0
  .stat_rx_inrangeerr_0(),
  .stat_rx_jabber_0(),                                  // output wire stat_rx_jabber_0
  .stat_rx_bad_code_0(),                              // output wire stat_rx_bad_code_0
  .stat_rx_bad_sfd_0(),                                // output wire stat_rx_bad_sfd_0
  .stat_rx_bad_preamble_0(),                      // output wire stat_rx_bad_preamble_0
  .tx_reset_0(1'b0),                                              // input wire tx_reset_0
  .user_tx_reset_0(tx_reset[0]),                                    // output wire user_tx_reset_0
  .tx_axis_tready_0(axi_str_txd_tready[0]),                                  // output wire tx_axis_tready_0
  .tx_axis_tvalid_0(axi_str_txd_tvalid[0]),                                  // input wire tx_axis_tvalid_0
  .tx_axis_tdata_0(axi_str_txd_tdata[0]),                                    // input wire [63 : 0] tx_axis_tdata_0
  .tx_axis_tlast_0(axi_str_txd_tlast[0]),                                    // input wire tx_axis_tlast_0
  .tx_axis_tkeep_0(axi_str_txd_tkeep[0]),                                    // input wire [7 : 0] tx_axis_tkeep_0
  .tx_axis_tuser_0(1'b0),                                    // input wire tx_axis_tuser_0
  .tx_unfout_0(),                                            // output wire tx_unfout_0
  .tx_preamblein_0(56'd0),                                    // input wire [55 : 0] tx_preamblein_0
  .rx_preambleout_0(),                                  // output wire [55 : 0] rx_preambleout_0
  .stat_tx_local_fault_0(),                        // output wire stat_tx_local_fault_0
  .stat_tx_total_bytes_0(),                        // output wire [3 : 0] stat_tx_total_bytes_0
  .stat_tx_total_packets_0(),                    // output wire stat_tx_total_packets_0
  .stat_tx_total_good_bytes_0(),              // output wire [13 : 0] stat_tx_total_good_bytes_0
  .stat_tx_total_good_packets_0(),          // output wire stat_tx_total_good_packets_0
  .stat_tx_bad_fcs_0(),                                // output wire stat_tx_bad_fcs_0
  .stat_tx_packet_64_bytes_0(),                // output wire stat_tx_packet_64_bytes_0
  .stat_tx_packet_65_127_bytes_0(),        // output wire stat_tx_packet_65_127_bytes_0
  .stat_tx_packet_128_255_bytes_0(),      // output wire stat_tx_packet_128_255_bytes_0
  .stat_tx_packet_256_511_bytes_0(),      // output wire stat_tx_packet_256_511_bytes_0
  .stat_tx_packet_512_1023_bytes_0(),    // output wire stat_tx_packet_512_1023_bytes_0
  .stat_tx_packet_1024_1518_bytes_0(),  // output wire stat_tx_packet_1024_1518_bytes_0
  .stat_tx_packet_1519_1522_bytes_0(),  // output wire stat_tx_packet_1519_1522_bytes_0
  .stat_tx_packet_1523_1548_bytes_0(),  // output wire stat_tx_packet_1523_1548_bytes_0
  .stat_tx_packet_1549_2047_bytes_0(),  // output wire stat_tx_packet_1549_2047_bytes_0
  .stat_tx_packet_2048_4095_bytes_0(),  // output wire stat_tx_packet_2048_4095_bytes_0
  .stat_tx_packet_4096_8191_bytes_0(),  // output wire stat_tx_packet_4096_8191_bytes_0
  .stat_tx_packet_8192_9215_bytes_0(),  // output wire stat_tx_packet_8192_9215_bytes_0
  .stat_tx_packet_small_0(),                      // output wire stat_tx_packet_small_0
  .stat_tx_packet_large_0(),                      // output wire stat_tx_packet_large_0
  .stat_tx_unicast_0(),                                // output wire stat_tx_unicast_0
  .stat_tx_multicast_0(),                            // output wire stat_tx_multicast_0
  .stat_tx_broadcast_0(),                            // output wire stat_tx_broadcast_0
  .stat_tx_vlan_0(),                                      // output wire stat_tx_vlan_0
  .stat_tx_frame_error_0(),                        // output wire stat_tx_frame_error_0
  .ctl_tx_enable_0(1'b1),                                    // input wire ctl_tx_enable_0
  .ctl_tx_send_rfi_0(1'b0),                                // input wire ctl_tx_send_rfi_0
  .ctl_tx_send_lfi_0(1'b0),                                // input wire ctl_tx_send_lfi_0
  .ctl_tx_send_idle_0(1'b0),                              // input wire ctl_tx_send_idle_0
  .ctl_tx_fcs_ins_enable_0(1'b1),                    // input wire ctl_tx_fcs_ins_enable_0
  .ctl_tx_ignore_fcs_0(1'b0),                            // input wire ctl_tx_ignore_fcs_0
  .ctl_tx_test_pattern_0(1'b0),                        // input wire ctl_tx_test_pattern_0
  .ctl_tx_test_pattern_enable_0(1'b0),          // input wire ctl_tx_test_pattern_enable_0
  .ctl_tx_test_pattern_select_0(1'b0),          // input wire ctl_tx_test_pattern_select_0
  .ctl_tx_data_pattern_select_0(1'b0),          // input wire ctl_tx_data_pattern_select_0
  .ctl_tx_test_pattern_seed_a_0(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_a_0
  .ctl_tx_test_pattern_seed_b_0(58'd0),          // input wire [57 : 0] ctl_tx_test_pattern_seed_b_0
  .ctl_tx_ipg_value_0(4'd12),                              // input wire [3 : 0] ctl_tx_ipg_value_0
  .ctl_tx_custom_preamble_enable_0(1'b0),    // input wire ctl_tx_custom_preamble_enable_0
  .gt_loopback_in_0(gt_loopback[0]),                                 // input wire [2 : 0] gt_loopback_in_0
  .qpllreset_in_0(1'b0)
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

endmodule
`default_nettype wire
