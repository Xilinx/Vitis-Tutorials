//
// Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

`timescale 1 ns / 1 ps

// Behavioral module to drive AXIS impulse response after INIT_DLY cycles
// `default_nettype none

module driver_axis
  # (
    parameter bit     IMAG_STEP = 0,
    parameter integer DWIDTH    = 32,
    parameter integer FSIZE     = 64,
    parameter integer INIT_DLY  = 0
  )
  (
    input  wire                               aclk,
    input  wire                               aresetn,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis TREADY" *)
    input  wire                               m00_axis_tready,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis  TDATA" *)
    output reg  [DWIDTH-1:0]                  m00_axis_tdata,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis  TVALID" *)
    output reg                                m00_axis_tvalid,
    (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 m00_axis  TLAST" *)
    output reg                                m00_axis_tlast
  );

  localparam true_c   = 1'b1;
  localparam false_c  = 1'b0;

  reg  [31:0]           cnt_val;

  // Produce an impulse response to cint16 represented by AXIS 32 bit value to AIE
  // Values are to be interpreted as cint16
  initial begin
    cnt_val         = 0;
    m00_axis_tdata  = 0;
    m00_axis_tvalid = false_c;
    m00_axis_tlast  = false_c;
  end

  always @(posedge aclk)
  begin
    if (!aresetn) begin
      cnt_val         <= 0;
      m00_axis_tlast  <= false_c;
      m00_axis_tvalid <= false_c;
    end
    else if (m00_axis_tready) begin
      if (cnt_val < FSIZE) begin
        cnt_val <= cnt_val + 1;
        m00_axis_tlast <= false_c;
        m00_axis_tvalid <= true_c;
      end
      else begin
        cnt_val <= 0;
        m00_axis_tlast <= true_c;
        m00_axis_tvalid <= true_c;
      end
    end
  end

  // Create step response
  always @(posedge aclk)
  begin
    if (cnt_val == INIT_DLY) begin
      // Create a step response with 0x4000 magnitude on either real or
      // imaginary decomposition of the 32 bit word acting as cint16.
      m00_axis_tdata <= (IMAG_STEP) ? ( 1 << (DWIDTH/2 - 2)) : ( 1 << (DWIDTH - 2));
    end
    else begin
      m00_axis_tdata <= 0;
    end
  end

endmodule

