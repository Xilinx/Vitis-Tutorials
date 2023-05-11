//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

`timescale 1 ps / 1 ps

// ONLY TO BE USED FOR STANDALONE RTL DEBUG
// `default_nettype none

module counter
  # (
    parameter integer DWIDTH  = 64
  )
  (
    input  wire                               aclk,
    input  wire                               aresetn,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis TREADY" *)
    input  wire                               m00_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis  TDATA" *)
    output wire [DWIDTH-1:0]                  m00_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis  TVALID" *)
    output wire                               m00_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis  TLAST" *)
    output wire                               m00_axis_tlast,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m01_axis TREADY" *)
    input  wire                               m01_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m01_axis  TDATA" *)
    output wire [DWIDTH-1:0]                  m01_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m01_axis  TVALID" *)
    output wire                               m01_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m01_axis  TLAST" *)
    output wire                               m01_axis_tlast,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m02_axis TREADY" *)
    input  wire                               m02_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m02_axis  TDATA" *)
    output wire [DWIDTH-1:0]                  m02_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m02_axis  TVALID" *)
    output wire                               m02_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m02_axis  TLAST" *)
    output wire                               m02_axis_tlast,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m03_axis TREADY" *)
    input  wire                               m03_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m03_axis  TDATA" *)
    output wire [DWIDTH-1:0]                  m03_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m03_axis  TVALID" *)
    output wire                               m03_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m03_axis  TLAST" *)
    output wire                               m03_axis_tlast,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m04_axis TREADY" *)
    input  wire                               m04_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m04_axis  TDATA" *)
    output wire [DWIDTH-1:0]                  m04_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m04_axis  TVALID" *)
    output wire                               m04_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m04_axis  TLAST" *)
    output wire                               m04_axis_tlast
  );

  localparam true_c   = 1'b1;
  localparam false_c  = 1'b0;

  reg  [DWIDTH/4-2:0] cnt_val;
  reg                 tvalid;
  reg                 tlast;
  reg                 cnt_err;
  reg                 first_time;
  wire                enable;

  // removed as Ip has no AXI4 lite/irq //assign counter_error = cnt_err;

  // Produce counter values for two cint16 to match AXIS 64 bit to AIE
  // Values are to be interpreted as cint16
  assign m00_axis_tdata = {16'b0, cnt_val, 1'b1, 16'b0, cnt_val, 1'b0};
  assign m01_axis_tdata = {16'b0, cnt_val, 1'b1, 16'b0, cnt_val, 1'b0};
  assign m02_axis_tdata = {16'b0, cnt_val, 1'b1, 16'b0, cnt_val, 1'b0};
  assign m03_axis_tdata = {16'b0, cnt_val, 1'b1, 16'b0, cnt_val, 1'b0};
  assign m04_axis_tdata = {16'b0, cnt_val, 1'b1, 16'b0, cnt_val, 1'b0};
  assign m00_axis_tvalid = tvalid;
  assign m00_axis_tlast  = tlast;
  assign m01_axis_tvalid = tvalid;
  assign m01_axis_tlast  = tlast;
  assign m02_axis_tvalid = tvalid;
  assign m02_axis_tlast  = tlast;
  assign m03_axis_tvalid = tvalid;
  assign m03_axis_tlast  = tlast;
  assign m04_axis_tvalid = tvalid;
  assign m04_axis_tlast  = tlast;
  assign enable = tvalid;// Freerunning // && m00_axis_tready;

  always @(posedge aclk)
  begin
    if (!aresetn)
    begin
      cnt_val     <= 0;
      tvalid      <= false_c;
      tlast       <= true_c;
      cnt_err     <= false_c;
      first_time  <= false_c;
    end
    else if (!first_time) begin
      first_time  <= true_c;
      tvalid      <= true_c;
    end
    else // if (enable) // No backpressure support
    begin
      cnt_val     <= cnt_val + 1;
      tvalid      <= true_c;
      tlast       <= true_c;
      cnt_err     <= !m00_axis_tready || !m01_axis_tready || !m02_axis_tready || !m03_axis_tready || !m04_axis_tready;
    end
  end

endmodule
