//
// Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

`timescale 1 ns / 1 ps

// Behavioral module to monitor AXIS transactions
// Currently only sampling the transactions to dummy register
// `default_nettype none

module monitor_axis
  # (
    parameter integer DWIDTH    = 32
  )
  (
    input  wire                               aclk,
    input  wire                               aresetn,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis TREADY" *)
    output reg                                s00_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis  TDATA" *)
    input  wire [DWIDTH-1:0]                  s00_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis  TVALID" *)
    input  wire                               s00_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 s00_axis  TLAST" *)
    input  wire                               s00_axis_tlast
  );

  localparam true_c   = 1'b1;
  localparam false_c  = 1'b0;

  reg  [DWIDTH-1:0]   monitor_tdata;
  reg                 monitor_tvalid;
  reg                 monitor_tlast;

  initial begin
    s00_axis_tready = false_c;
    monitor_tdata   = 0;
    monitor_tvalid  = 0;
    monitor_tlast   = 0;
  end

  always @(posedge aclk)
  begin
    if (!aresetn) begin
      s00_axis_tready <= false_c;
      monitor_tdata   <= 0;
      monitor_tvalid  <= 0;
      monitor_tlast   <= 0;
    end
    else begin
      s00_axis_tready <=true_c;
      monitor_tdata   <= s00_axis_tdata;
      monitor_tvalid  <= s00_axis_tvalid;
      monitor_tlast   <= s00_axis_tlast;
    end
  end


endmodule


