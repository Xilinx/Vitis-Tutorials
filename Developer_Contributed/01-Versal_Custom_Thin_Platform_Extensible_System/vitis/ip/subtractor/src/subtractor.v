//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

`timescale 1 ps / 1 ps

// ONLY TO BE USED FOR STANDALONE RTL DEBUG
// `default_nettype none

module subtractor
  # (
    parameter integer DWIDTH  = 64,
    parameter integer C_S00_AXI_DATA_WIDTH  = 32,
    parameter integer C_S00_AXI_ADDR_WIDTH  = 4
  )
  (
    input  wire                 aresetn,
    input  wire                 aclk,
    // Ports of Axi Slave Bus Interface S00_AXI
    //input wire  s00_axi_aclk,
    //input wire  s00_axi_aresetn,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB AWADDR" *)
    input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB AWPROT" *)
    input wire [2 : 0] s00_axi_awprot,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB AWVALID" *)
    input wire  s00_axi_awvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB AWREADY" *)
    output wire  s00_axi_awready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB WDATA" *)
    input wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB WSTRB" *)
    input wire [(C_S00_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB WVALID" *)
    input wire  s00_axi_wvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB WREADY" *)
    output wire  s00_axi_wready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB BRESP" *)
    output wire [1 : 0] s00_axi_bresp,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB BVALID" *)
    output wire  s00_axi_bvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB BREADY" *)
    input wire  s00_axi_bready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB ARADDR" *)
    input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB ARPROT" *)
    input wire [2 : 0] s00_axi_arprot,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB ARVALID" *)
    input wire  s00_axi_arvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB ARREADY" *)
    output wire  s00_axi_arready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB RDATA" *)
    output wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB RRESP" *)
    output wire [1 : 0] s00_axi_rresp,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB RVALID" *)
    output wire  s00_axi_rvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI_SUB RREADY" *)
    input wire  s00_axi_rready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis TREADY" *)
    output wire                 s00_axis_tready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis  TDATA" *)
    input  wire [DWIDTH-1:0]    s00_axis_tdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis  TVALID" *)
    input  wire                 s00_axis_tvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis  TLAST" *)
    input  wire                 s00_axis_tlast,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s01_axis TREADY" *)
    output wire                 s01_axis_tready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s01_axis  TDATA" *)
    input  wire [DWIDTH-1:0]    s01_axis_tdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s01_axis  TVALID" *)
    input  wire                 s01_axis_tvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s01_axis  TLAST" *)
    input  wire                 s01_axis_tlast,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s02_axis TREADY" *)
    output wire                 s02_axis_tready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s02_axis  TDATA" *)
    input  wire [DWIDTH-1:0]    s02_axis_tdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s02_axis  TVALID" *)
    input  wire                 s02_axis_tvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s02_axis  TLAST" *)
    input  wire                 s02_axis_tlast,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s03_axis TREADY" *)
    output wire                 s03_axis_tready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s03_axis  TDATA" *)
    input  wire [DWIDTH-1:0]    s03_axis_tdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s03_axis  TVALID" *)
    input  wire                 s03_axis_tvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s03_axis  TLAST" *)
    input  wire                 s03_axis_tlast,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s04_axis TREADY" *)
    output wire                 s04_axis_tready,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s04_axis  TDATA" *)
    input  wire [DWIDTH-1:0]    s04_axis_tdata,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s04_axis  TVALID" *)
    input  wire                 s04_axis_tvalid,
    //(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s04_axis  TLAST" *)
    input  wire                 s04_axis_tlast
  );

  localparam true_c   = 1'b1;
  localparam false_c  = 1'b0;

  reg  [DWIDTH/4-1:0]   a_buf_i;
  reg  [DWIDTH/4-1:0]   a_buf_q;
  reg  [DWIDTH/4-1:0]   b0_buf_i;
  reg  [DWIDTH/4-1:0]   b0_buf_q;
  reg  [DWIDTH/4-1:0]   b1_buf_i;
  reg  [DWIDTH/4-1:0]   b1_buf_q;
  reg  [DWIDTH/4-1:0]   b2_buf_i;
  reg  [DWIDTH/4-1:0]   b2_buf_q;
  reg  [DWIDTH/4-1:0]   b3_buf_i;
  reg  [DWIDTH/4-1:0]   b3_buf_q;
  wire [DWIDTH/4-1:0]   diff0_i;
  wire [DWIDTH/4-1:0]   diff0_q;
  wire [DWIDTH/4-1:0]   diff1_i;
  wire [DWIDTH/4-1:0]   diff1_q;
  wire [DWIDTH/4-1:0]   diff2_i;
  wire [DWIDTH/4-1:0]   diff2_q;
  wire [DWIDTH/4-1:0]   diff3_i;
  wire [DWIDTH/4-1:0]   diff3_q;
  reg                   tvalid;
  reg                   tlast;

  // Tie off tready
  assign s00_axis_tready   = true_c;
  assign s01_axis_tready   = true_c;
  assign s02_axis_tready   = true_c;
  assign s03_axis_tready   = true_c;
  assign s04_axis_tready   = true_c;

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_i0 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .b          (s01_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .res        (diff0_i)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_q0 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/4-1:0]),
    .b          (s01_axis_tdata[DWIDTH/4-1:0]),
    .res        (diff0_q)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_i1 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .b          (s02_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .res        (diff1_i)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_q1 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/4-1:0]),
    .b          (s02_axis_tdata[DWIDTH/4-1:0]),
    .res        (diff1_q)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_i2 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .b          (s03_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .res        (diff2_i)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_q2 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/4-1:0]),
    .b          (s03_axis_tdata[DWIDTH/4-1:0]),
    .res        (diff2_q)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_i3 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .b          (s04_axis_tdata[DWIDTH/2-1:DWIDTH/4]),
    .res        (diff3_i)
  );

  my_diff # (
    .SIZEIN(DWIDTH/4)
  ) diff_q3 (
    .resetn     (aresetn),
    .clk        (aclk),
    .a          (s00_axis_tdata[DWIDTH/4-1:0]),
    .b          (s04_axis_tdata[DWIDTH/4-1:0]),
    .res        (diff3_q)
  );
  // Instantiation of Axi Bus Interface S00_AXI
  subtractor_S00_AXI # (
    .C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
    .C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
  ) subtractor_S00_AXI_inst (
    .subtract_val0({diff0_i, diff0_q}),
    .subtract_val1({diff1_i, diff1_q}),
    .subtract_val2({diff2_i, diff2_q}),
    .subtract_val3({diff3_i, diff3_q}),
    .S_AXI_ACLK(aclk),
    .S_AXI_ARESETN(aresetn),
    .S_AXI_AWADDR(s00_axi_awaddr),
    .S_AXI_AWPROT(s00_axi_awprot),
    .S_AXI_AWVALID(s00_axi_awvalid),
    .S_AXI_AWREADY(s00_axi_awready),
    .S_AXI_WDATA(s00_axi_wdata),
    .S_AXI_WSTRB(s00_axi_wstrb),
    .S_AXI_WVALID(s00_axi_wvalid),
    .S_AXI_WREADY(s00_axi_wready),
    .S_AXI_BRESP(s00_axi_bresp),
    .S_AXI_BVALID(s00_axi_bvalid),
    .S_AXI_BREADY(s00_axi_bready),
    .S_AXI_ARADDR(s00_axi_araddr),
    .S_AXI_ARPROT(s00_axi_arprot),
    .S_AXI_ARVALID(s00_axi_arvalid),
    .S_AXI_ARREADY(s00_axi_arready),
    .S_AXI_RDATA(s00_axi_rdata),
    .S_AXI_RRESP(s00_axi_rresp),
    .S_AXI_RVALID(s00_axi_rvalid),
    .S_AXI_RREADY(s00_axi_rready)
  );

endmodule
