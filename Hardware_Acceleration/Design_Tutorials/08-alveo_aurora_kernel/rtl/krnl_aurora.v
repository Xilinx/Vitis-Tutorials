/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

module krnl_aurora (
// Platform axi/axis ports
    input           ap_clk,
    input           ap_rst_n,

// TX/RX AXIS interface
    output          rx_axis_tvalid,
    output [255:0]  rx_axis_tdata,
    input           rx_axis_tready,

    output          tx_axis_tready,
    input           tx_axis_tvalid,
    input  [255:0]  tx_axis_tdata,

  // AXI4-Lite slave interface
    input           s_axi_control_awvalid,
    output          s_axi_control_awready,
    input  [11:0]   s_axi_control_awaddr ,
    input           s_axi_control_wvalid ,
    output          s_axi_control_wready ,
    input  [31:0]   s_axi_control_wdata  ,
    input  [3:0]    s_axi_control_wstrb  ,
    input           s_axi_control_arvalid,
    output          s_axi_control_arready,
    input  [11:0]   s_axi_control_araddr ,
    output          s_axi_control_rvalid ,
    input           s_axi_control_rready ,
    output [31:0]   s_axi_control_rdata  ,
    output [1:0]    s_axi_control_rresp  ,
    output          s_axi_control_bvalid ,
    input           s_axi_control_bready ,
    output [1:0]    s_axi_control_bresp  , 

// GT ref clock
    input           gt_refclk_p,
    input           gt_refclk_n,
    input  [3:0]    gt_rxp_in,
    input  [3:0]    gt_rxn_in,
    output [3:0]    gt_txp_out,
    output [3:0]    gt_txn_out,

// assume 50MHz init clk
    input           init_clk

);

// internal axi/axis signals

wire            user_clk;

wire [255:0]    s_axi_tx_tdata_u;
wire            s_axi_tx_tvalid_u;
wire            s_axi_tx_tready_u;
wire [255:0]    m_axi_rx_tdata_u;
wire            m_axi_rx_tvalid_u;


// aurora status signals
//   user_clk domain
wire [3:0]      line_up_u; 
wire            channel_up_u;
wire            soft_err_u;
wire            hard_err_u;
wire            mmcm_not_locked_out_u;
wire [3:0]      gt_powergood_u;
//   init_clk domain
wire            gt_pll_lock_i;
//   sync status signals
wire [3:0]      line_up_sync;
wire [3:0]      gt_powergood_sync;
wire            channel_up_sync;
wire            soft_err_sync;
wire            hard_err_sync;
wire            mmcm_not_locked_out_sync;
wire            gt_pll_lock_sync;
wire [12:0]     aurora_status;

// internal reset signals
reg             reset_pb_i;
reg             pma_init_i;


// aurora reset generation
reg [26:0]      rst_cnt;
wire            ap_rst_n_i;
wire            ap_rst_n_u;

xpm_cdc_async_rst reset_sync_0 (
    .src_arst   (ap_rst_n),
    .dest_clk   (init_clk),
    .dest_arst  (ap_rst_n_i)
);

xpm_cdc_async_rst reset_sync_1 (
    .src_arst   (ap_rst_n),
    .dest_clk   (user_clk),
    .dest_arst  (ap_rst_n_u)
);

// use rst_cnt to generate asserted reset_pb longer than 1 second.
// Assume 100MHz init_clk is used, so 27bit counter is needed. 
// Apparently it works as well for 50MHz init_clk
always @ (posedge init_clk or negedge ap_rst_n_i) begin
    if (!ap_rst_n_i) begin
        reset_pb_i <= 1'b1;
        pma_init_i <= 1'b1;
        rst_cnt    <= 27'b0;
    end else begin
        if (rst_cnt != 27'h7ff_ffff) begin
            rst_cnt <= rst_cnt + 1'b1;
        end
        if (rst_cnt == 27'h7ff_ff00) begin
            pma_init_i <= 1'b0;
        end
        if (rst_cnt == 27'h7ff_ffff) begin
            reset_pb_i <= 1'b0;
        end
    end
end


// aurora status sync
xpm_cdc_array_single #(.WIDTH(4)) status_sync_0 (
    .src_in     (line_up_u),
    .src_clk    (user_clk),
    .dest_clk   (ap_clk),
    .dest_out   (line_up_sync)
);

xpm_cdc_array_single #(.WIDTH(4)) status_sync_1 (
    .src_in     (gt_powergood_u),
    .src_clk    (user_clk),
    .dest_clk   (ap_clk),
    .dest_out   (gt_powergood_sync)
);

xpm_cdc_single  status_sync_2 (
    .src_in     (channel_up_u),
    .src_clk    (user_clk),
    .dest_clk   (ap_clk),
    .dest_out   (channel_up_sync)
);

xpm_cdc_single  status_sync_3 (
    .src_in     (soft_err_u),
    .src_clk    (user_clk),
    .dest_clk   (ap_clk),
    .dest_out   (soft_err_sync)
);

xpm_cdc_single  status_sync_4 (
    .src_in     (hard_err_u),
    .src_clk    (user_clk),
    .dest_clk   (ap_clk),
    .dest_out   (hard_err_sync)
);

xpm_cdc_single  status_sync_5 (
    .src_in     (mmcm_not_locked_out_u),
    .src_clk    (user_clk),
    .dest_clk   (ap_clk),
    .dest_out   (mmcm_not_locked_out_sync)
);

xpm_cdc_single  status_sync_6 (
    .src_in     (gt_pll_lock_i),
    .src_clk    (init_clk),
    .dest_clk   (ap_clk),
    .dest_out   (gt_pll_lock_sync)
);

// Aurora Status assignment :
//  [12]    channel_up
//  [11]    soft_err
//  [10]    hard_err
//  [9]     mmcm_not_locked_out
//  [8]     gt_pll_lock
//  [7:4]   line_up
//  [3:0]   gt_powergood

assign aurora_status = {channel_up_sync,
                        soft_err_sync,
                        hard_err_sync,
                        mmcm_not_locked_out_sync,
                        gt_pll_lock_sync,
                        line_up_sync,
                        gt_powergood_sync
                        };


aurora_64b66b_0 aurora_64b66b_0_inst (
  .rxp                          (gt_rxp_in),                // input wire [0 : 3] rxp
  .rxn                          (gt_rxn_in),                // input wire [0 : 3] rxn
  .reset_pb                     (reset_pb_i),               // input wire reset_pb
  .power_down                   (1'b0),                     // input wire power_down
  .pma_init                     (pma_init_i),               // input wire pma_init
  .loopback                     (3'b0),                     // input wire [2 : 0] loopback
  .txp                          (gt_txp_out),               // output wire [0 : 3] txp
  .txn                          (gt_txn_out),               // output wire [0 : 3] txn
  .hard_err                     (hard_err_u),               // output wire hard_err
  .soft_err                     (soft_err_u),               // output wire soft_err
  .channel_up                   (channel_up_u),             // output wire channel_up
  .lane_up                      (line_up_u),                // output wire [0 : 3] lane_up
  .tx_out_clk                   (),                         // output wire tx_out_clk
  .gt_pll_lock                  (gt_pll_lock_i),            // output wire gt_pll_lock
  .s_axi_tx_tdata               (s_axi_tx_tdata_u),         // input wire [0 : 255] s_axi_tx_tdata
  .s_axi_tx_tvalid              (s_axi_tx_tvalid_u),        // input wire s_axi_tx_tvalid
  .s_axi_tx_tready              (s_axi_tx_tready_u),        // output wire s_axi_tx_tready
  .m_axi_rx_tdata               (m_axi_rx_tdata_u),         // output wire [0 : 255] m_axi_rx_tdata
  .m_axi_rx_tvalid              (m_axi_rx_tvalid_u),        // output wire m_axi_rx_tvalid
  .mmcm_not_locked_out          (mmcm_not_locked_out_u),    // output wire mmcm_not_locked_out
  .gt0_drpaddr                  (10'b0),                    // input wire [9 : 0] gt0_drpaddr
  .gt1_drpaddr                  (10'b0),                    // input wire [9 : 0] gt1_drpaddr
  .gt2_drpaddr                  (10'b0),                    // input wire [9 : 0] gt2_drpaddr
  .gt3_drpaddr                  (10'b0),                    // input wire [9 : 0] gt3_drpaddr
  .gt0_drpdi                    (16'b0),                    // input wire [15 : 0] gt0_drpdi
  .gt1_drpdi                    (16'b0),                    // input wire [15 : 0] gt1_drpdi
  .gt2_drpdi                    (16'b0),                    // input wire [15 : 0] gt2_drpdi
  .gt3_drpdi                    (16'b0),                    // input wire [15 : 0] gt3_drpdi
  .gt0_drprdy                   (),                         // output wire gt0_drprdy
  .gt1_drprdy                   (),                         // output wire gt1_drprdy
  .gt2_drprdy                   (),                         // output wire gt2_drprdy
  .gt3_drprdy                   (),                         // output wire gt3_drprdy
  .gt0_drpwe                    (1'b0),                     // input wire gt0_drpwe
  .gt1_drpwe                    (1'b0),                     // input wire gt1_drpwe
  .gt2_drpwe                    (1'b0),                     // input wire gt2_drpwe
  .gt3_drpwe                    (1'b0),                     // input wire gt3_drpwe
  .gt0_drpen                    (1'b0),                     // input wire gt0_drpen
  .gt1_drpen                    (1'b0),                     // input wire gt1_drpen
  .gt2_drpen                    (1'b0),                     // input wire gt2_drpen
  .gt3_drpen                    (1'b0),                     // input wire gt3_drpen
  .gt0_drpdo                    (),                         // output wire [15 : 0] gt0_drpdo
  .gt1_drpdo                    (),                         // output wire [15 : 0] gt1_drpdo
  .gt2_drpdo                    (),                         // output wire [15 : 0] gt2_drpdo
  .gt3_drpdo                    (),                         // output wire [15 : 0] gt3_drpdo
  .init_clk                     (init_clk),                 // input wire init_clk
  .link_reset_out               (),                         // output wire link_reset_out
  .gt_refclk1_p                 (gt_refclk_p),              // input wire gt_refclk1_p
  .gt_refclk1_n                 (gt_refclk_n),              // input wire gt_refclk1_n
  .user_clk_out                 (user_clk),                 // output wire user_clk_out
  .sync_clk_out                 (),                         // output wire sync_clk_out
  .gt_qpllclk_quad1_out         (),                         // output wire gt_qpllclk_quad1_out
  .gt_qpllrefclk_quad1_out      (),                         // output wire gt_qpllrefclk_quad1_out
  .gt_qpllrefclklost_quad1_out  (),                         // output wire gt_qpllrefclklost_quad1_out
  .gt_qplllock_quad1_out        (),                         // output wire gt_qplllock_quad1_out
  .gt_rxcdrovrden_in            (1'b0),                     // input wire gt_rxcdrovrden_in
  .sys_reset_out                (),                         // output wire sys_reset_out
  .gt_reset_out                 (),                         // output wire gt_reset_out
  .gt_refclk1_out               (),                         // output wire gt_refclk1_out
  .gt_powergood                 (gt_powergood_u)            // output wire [3 : 0] gt_powergood
);

axis_data_fifo_0 axis_data_fifo_0_rx (
  .s_axis_aresetn   (ap_rst_n_u),           // input wire s_axis_aresetn
  .s_axis_aclk      (user_clk),             // input wire s_axis_aclk
  .s_axis_tvalid    (m_axi_rx_tvalid_u),    // input wire s_axis_tvalid
  .s_axis_tready    (),                     // output wire s_axis_tready
  .s_axis_tdata     (m_axi_rx_tdata_u),     // input wire [255 : 0] s_axis_tdata
  .m_axis_aclk      (ap_clk),               // input wire m_axis_aclk
  .m_axis_tvalid    (rx_axis_tvalid),       // output wire m_axis_tvalid
  .m_axis_tready    (rx_axis_tready),       // input wire m_axis_tready
  .m_axis_tdata     (rx_axis_tdata)         // output wire [255 : 0] m_axis_tdata
);

axis_data_fifo_0 axis_data_fifo_0_tx (
  .s_axis_aresetn   (ap_rst_n),             // input wire s_axis_aresetn
  .s_axis_aclk      (ap_clk),               // input wire s_axis_aclk
  .s_axis_tvalid    (tx_axis_tvalid),       // input wire s_axis_tvalid
  .s_axis_tready    (tx_axis_tready),       // output wire s_axis_tready
  .s_axis_tdata     (tx_axis_tdata),        // input wire [255 : 0] s_axis_tdata
  .m_axis_aclk      (user_clk),             // input wire m_axis_aclk
  .m_axis_tvalid    (s_axi_tx_tvalid_u),    // output wire m_axis_tvalid
  .m_axis_tready    (s_axi_tx_tready_u),    // input wire m_axis_tready
  .m_axis_tdata     (s_axi_tx_tdata_u)      // output wire [255 : 0] m_axis_tdata
);

krnl_aurora_control_s_axi axi_control_slave (
  .ACLK             (ap_clk),
  .ARESETn          (ap_rst_n),
  .AWADDR           (s_axi_control_awaddr),
  .AWVALID          (s_axi_control_awvalid),
  .AWREADY          (s_axi_control_awready),
  .WDATA            (s_axi_control_wdata),
  .WSTRB            (s_axi_control_wstrb),
  .WVALID           (s_axi_control_wvalid),
  .WREADY           (s_axi_control_wready),
  .BRESP            (s_axi_control_bresp),
  .BVALID           (s_axi_control_bvalid),
  .BREADY           (s_axi_control_bready),
  .ARADDR           (s_axi_control_araddr),
  .ARVALID          (s_axi_control_arvalid),
  .ARREADY          (s_axi_control_arready),
  .RDATA            (s_axi_control_rdata),
  .RRESP            (s_axi_control_rresp),
  .RVALID           (s_axi_control_rvalid),
  .RREADY           (s_axi_control_rready),
  .aurora_status    (aurora_status)
);


endmodule