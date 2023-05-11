/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


//////////////////////////////////////////////////////////////////////////////// 
// default_nettype of none prevents implicit wire declaration.
`default_nettype none
`timescale 1 ns / 1 ps
// Top level of the kernel. Do not modify module name, parameters or ports.
module data_fifo_krnl (
  // System Signals
  input  wire        ap_clk               ,
  input  wire        ap_rst_n             ,

  input  wire        rx_axis_tvalid,
  input  wire [63:0] rx_axis_tdata,
  input  wire        rx_axis_tlast,
  input  wire [7:0]  rx_axis_tkeep,
  input  wire        rx_axis_tuser,
  output wire        rx_axis_tready,
  
  input  wire        tx_axis_tready,
  output wire        tx_axis_tvalid,
  output wire [63:0] tx_axis_tdata,
  output wire        tx_axis_tlast,
  output wire [7:0]  tx_axis_tkeep,
  output wire        tx_axis_tuser
);

///////////////////////////////////////////////////////////////////////////////
// Wires and Variables
///////////////////////////////////////////////////////////////////////////////

logic areset = 1'b0;

logic axi_str_txd_tvalid;
logic axi_str_txd_tready;
logic axi_str_txd_tlast;
logic [7:0] axi_str_txd_tkeep;
logic axi_str_txd_tuser;
logic [63:0] axi_str_txd_tdata;
logic axi_str_rxd_tvalid;
logic axi_str_rxd_tready;
logic axi_str_rxd_tlast;
logic [7:0] axi_str_rxd_tkeep;
logic [63:0] axi_str_rxd_tdata;
logic axi_str_rxd_tuser;

logic i_m_axis_tvalid;
logic i_m_axis_tready;
logic [63:0] i_m_axis_tdata;
logic [7:0] i_m_axis_tkeep;
logic i_m_axis_tlast;
logic i_m_axis_tuser;

logic tx_reset_0;
logic rx_reset_0;

wire rx_core_clk_0;
wire tx_clk_out_0;

assign rx_core_clk_0 = tx_clk_out_0;


// Register and invert reset signal.
always @(posedge ap_clk) begin
  areset <= ~ap_rst_n;
end

axis_data_fifo_0 tx_fifo_0 (
  .s_axis_aresetn(~areset),
  .s_axis_aclk(ap_clk),  
  .s_axis_tvalid(i_m_axis_tvalid),
  .s_axis_tready(i_m_axis_tready),
  .s_axis_tdata(i_m_axis_tdata),
  .s_axis_tkeep(i_m_axis_tkeep),
  .s_axis_tlast(i_m_axis_tlast),
  .s_axis_tuser(i_m_axis_tuser),
  .m_axis_aclk(ap_clk),
  .m_axis_tvalid(tx_axis_tvalid),
  .m_axis_tready(tx_axis_tready),
  .m_axis_tdata(tx_axis_tdata),
  .m_axis_tkeep(tx_axis_tkeep),
  .m_axis_tlast(tx_axis_tlast),
  .m_axis_tuser(tx_axis_tuser)
);

axis_data_fifo_0 rx_fifo_0 (
  .s_axis_aresetn(~areset),
  .s_axis_aclk(ap_clk),  
  .s_axis_tvalid(rx_axis_tvalid),
  .s_axis_tready(rx_axis_tready),
  .s_axis_tdata(rx_axis_tdata),
  .s_axis_tkeep(rx_axis_tkeep),
  .s_axis_tlast(rx_axis_tlast),
  .s_axis_tuser(rx_axis_tuser),
  .m_axis_aclk(ap_clk),
  .m_axis_tvalid(i_m_axis_tvalid),
  .m_axis_tready(i_m_axis_tready),
  .m_axis_tdata(i_m_axis_tdata),
  .m_axis_tkeep(i_m_axis_tkeep),
  .m_axis_tlast(i_m_axis_tlast),
  .m_axis_tuser(i_m_axis_tuser)
);

endmodule

