//
// Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
// Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

`timescale 1 ps / 1 ps

// ONLY TO BE USED FOR STANDALONE RTL DEBUG
// `default_nettype none

module my_sub
  # (
    parameter integer DWIDTH  = 64
  )
  (
    input  wire                 aresetn,
    input  wire                 aclk,
    input  wire                 s00_axi_aclk,
    input  wire                 s00_axi_aresetn,
    // Ports of Axi Slave Bus Interface S00_AXI
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
    input  wire                 s04_axis_tlast,
    output wire [DWIDTH/2-1:0]  subtract_val0,
    output wire [DWIDTH/2-1:0]  subtract_val1,
    output wire [DWIDTH/2-1:0]  subtract_val2,
    output wire [DWIDTH/2-1:0]  subtract_val3
  );

  localparam true_c   = 1'b1;
  localparam false_c  = 1'b0;

  reg  [DWIDTH/4-1:0]   buf0_i;
  reg  [DWIDTH/4-1:0]   buf0_q;
  reg  [DWIDTH/4-1:0]   buf1_i;
  reg  [DWIDTH/4-1:0]   buf1_q;
  reg  [DWIDTH/4-1:0]   buf2_i;
  reg  [DWIDTH/4-1:0]   buf2_q;
  reg  [DWIDTH/4-1:0]   buf3_i;
  reg  [DWIDTH/4-1:0]   buf3_q;
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
  reg                   my_strobe;
  reg                   my_strobe_d;
  reg                   my_strobe_d2;
  wire                  my_strobe_edge;

  // Tie off tready
  assign s00_axis_tready   = true_c;
  assign s01_axis_tready   = true_c;
  assign s02_axis_tready   = true_c;
  assign s03_axis_tready   = true_c;
  assign s04_axis_tready   = true_c;

  assign subtract_val0     = {buf0_i, buf0_q};
  assign subtract_val1     = {buf1_i, buf1_q};
  assign subtract_val2     = {buf2_i, buf2_q};
  assign subtract_val3     = {buf3_i, buf3_q};

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

  always @(posedge s00_axi_aclk)
  begin
    if (!aresetn)
    begin
      my_strobe  <= false_c;
    end
    else
    begin
      my_strobe  <= !my_strobe;
    end
  end

  // Buffer in aclk to detect strobe edges
  always @(posedge aclk)
  begin
    if (!aresetn)
    begin
      my_strobe_d  <= false_c;
      my_strobe_d2 <= false_c;
    end
    else
    begin
      my_strobe_d  <= my_strobe;
      my_strobe_d2 <= my_strobe_d;
    end
  end

  assign my_strobe_edge = my_strobe_d ^ my_strobe_d2;


  // Simple hold circuit
  always @(posedge aclk)
  begin
    if (!aresetn)
    begin
      buf0_i      <= 0;
      buf0_q      <= 0;
      buf1_i      <= 0;
      buf1_q      <= 0;
      buf2_i      <= 0;
      buf2_q      <= 0;
      buf3_i      <= 0;
      buf3_q      <= 0;
    end
    else if (my_strobe_edge) begin
      buf0_i      <= diff0_i;
      buf0_q      <= diff0_q;
      buf1_i      <= diff1_i;
      buf1_q      <= diff1_q;
      buf2_i      <= diff2_i;
      buf2_q      <= diff2_q;
      buf3_i      <= diff3_i;
      buf3_q      <= diff3_q;
    end
  end

endmodule
