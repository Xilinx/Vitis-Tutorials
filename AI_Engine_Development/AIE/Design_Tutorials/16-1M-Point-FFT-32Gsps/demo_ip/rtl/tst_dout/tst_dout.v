//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns / 1ps

module tst_dout #(
  parameter integer C_S_AXI_CONTROL_ADDR_WIDTH = 12,
  parameter integer C_S_AXI_CONTROL_DATA_WIDTH = 32
)
(
  // System Signals
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 regclk CLK" *)
  (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF s_axi_control:testen, ASSOCIATED_RESET reg_resetn" *)
  input  wire                                    reg_clk,
  input  wire                                    reg_resetn,
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
  // a sync signal to ifft tester
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 testen TVALID" *)   input        tst_axi_tvld,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 testen TDATA"  *)   input  [7:0] tst_axi_tdat,
 /////////////////////////////////////////////////////////////////////
 /////////  DUT Interfaces
 /////////////////////////////////////////////////////////////////////
 (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 datclk CLK" *)
 (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF d00a:d01a:d02a:d03a:d04a:d05a:d06a:d07a:d08a:d09a:d10a:d11a:d12a:d13a:d14a:d15a:d16a:d17a:d18a:d19a:d20a:d21a:d22a:d23a:d24a:d25a:d26a:d27a:d28a:d29a:d30a:d31a:d00b:d01b:d02b:d03b:d04b:d05b:d06b:d07b:d08b:d09b:d10b:d11b:d12b:d13b:d14b:d15b:d16b:d17b:d18b:d19b:d20b:d21b:d22b:d23b:d24b:d25b:d26b:d27b:d28b:d29b:d30b:d31b"*)
 input  wire        dat_clk,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00a TREADY" *)    output       d00a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00a TVALID" *)    input        d00a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00a TDATA" *)     input [63:0] d00a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01a TREADY" *)    output       d01a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01a TVALID" *)    input        d01a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01a TDATA" *)     input [63:0] d01a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02a TREADY" *)    output       d02a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02a TVALID" *)    input        d02a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02a TDATA" *)     input [63:0] d02a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03a TREADY" *)    output       d03a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03a TVALID" *)    input        d03a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03a TDATA" *)     input [63:0] d03a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04a TREADY" *)    output       d04a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04a TVALID" *)    input        d04a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04a TDATA" *)     input [63:0] d04a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05a TREADY" *)    output       d05a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05a TVALID" *)    input        d05a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05a TDATA" *)     input [63:0] d05a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06a TREADY" *)    output       d06a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06a TVALID" *)    input        d06a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06a TDATA" *)     input [63:0] d06a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07a TREADY" *)    output       d07a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07a TVALID" *)    input        d07a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07a TDATA" *)     input [63:0] d07a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08a TREADY" *)    output       d08a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08a TVALID" *)    input        d08a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08a TDATA" *)     input [63:0] d08a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09a TREADY" *)    output       d09a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09a TVALID" *)    input        d09a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09a TDATA" *)     input [63:0] d09a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10a TREADY" *)    output       d10a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10a TVALID" *)    input        d10a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10a TDATA" *)     input [63:0] d10a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11a TREADY" *)    output       d11a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11a TVALID" *)    input        d11a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11a TDATA" *)     input [63:0] d11a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12a TREADY" *)    output       d12a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12a TVALID" *)    input        d12a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12a TDATA" *)     input [63:0] d12a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13a TREADY" *)    output       d13a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13a TVALID" *)    input        d13a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13a TDATA" *)     input [63:0] d13a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14a TREADY" *)    output       d14a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14a TVALID" *)    input        d14a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14a TDATA" *)     input [63:0] d14a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15a TREADY" *)    output       d15a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15a TVALID" *)    input        d15a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15a TDATA" *)     input [63:0] d15a_axi_tdat, 
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16a TREADY" *)    output       d16a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16a TVALID" *)    input        d16a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16a TDATA" *)     input [63:0] d16a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17a TREADY" *)    output       d17a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17a TVALID" *)    input        d17a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17a TDATA" *)     input [63:0] d17a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18a TREADY" *)    output       d18a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18a TVALID" *)    input        d18a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18a TDATA" *)     input [63:0] d18a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19a TREADY" *)    output       d19a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19a TVALID" *)    input        d19a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19a TDATA" *)     input [63:0] d19a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20a TREADY" *)    output       d20a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20a TVALID" *)    input        d20a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20a TDATA" *)     input [63:0] d20a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21a TREADY" *)    output       d21a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21a TVALID" *)    input        d21a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21a TDATA" *)     input [63:0] d21a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22a TREADY" *)    output       d22a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22a TVALID" *)    input        d22a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22a TDATA" *)     input [63:0] d22a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23a TREADY" *)    output       d23a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23a TVALID" *)    input        d23a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23a TDATA" *)     input [63:0] d23a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24a TREADY" *)    output       d24a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24a TVALID" *)    input        d24a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24a TDATA" *)     input [63:0] d24a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25a TREADY" *)    output       d25a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25a TVALID" *)    input        d25a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25a TDATA" *)     input [63:0] d25a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26a TREADY" *)    output       d26a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26a TVALID" *)    input        d26a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26a TDATA" *)     input [63:0] d26a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27a TREADY" *)    output       d27a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27a TVALID" *)    input        d27a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27a TDATA" *)     input [63:0] d27a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28a TREADY" *)    output       d28a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28a TVALID" *)    input        d28a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28a TDATA" *)     input [63:0] d28a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29a TREADY" *)    output       d29a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29a TVALID" *)    input        d29a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29a TDATA" *)     input [63:0] d29a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30a TREADY" *)    output       d30a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30a TVALID" *)    input        d30a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30a TDATA" *)     input [63:0] d30a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31a TREADY" *)    output       d31a_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31a TVALID" *)    input        d31a_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31a TDATA" *)     input [63:0] d31a_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00b TREADY" *)    output       d00b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00b TVALID" *)    input        d00b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00b TDATA" *)     input [63:0] d00b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01b TREADY" *)    output       d01b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01b TVALID" *)    input        d01b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01b TDATA" *)     input [63:0] d01b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02b TREADY" *)    output       d02b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02b TVALID" *)    input        d02b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02b TDATA" *)     input [63:0] d02b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03b TREADY" *)    output       d03b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03b TVALID" *)    input        d03b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03b TDATA" *)     input [63:0] d03b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04b TREADY" *)    output       d04b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04b TVALID" *)    input        d04b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04b TDATA" *)     input [63:0] d04b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05b TREADY" *)    output       d05b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05b TVALID" *)    input        d05b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05b TDATA" *)     input [63:0] d05b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06b TREADY" *)    output       d06b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06b TVALID" *)    input        d06b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06b TDATA" *)     input [63:0] d06b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07b TREADY" *)    output       d07b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07b TVALID" *)    input        d07b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07b TDATA" *)     input [63:0] d07b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08b TREADY" *)    output       d08b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08b TVALID" *)    input        d08b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08b TDATA" *)     input [63:0] d08b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09b TREADY" *)    output       d09b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09b TVALID" *)    input        d09b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09b TDATA" *)     input [63:0] d09b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10b TREADY" *)    output       d10b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10b TVALID" *)    input        d10b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10b TDATA" *)     input [63:0] d10b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11b TREADY" *)    output       d11b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11b TVALID" *)    input        d11b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11b TDATA" *)     input [63:0] d11b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12b TREADY" *)    output       d12b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12b TVALID" *)    input        d12b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12b TDATA" *)     input [63:0] d12b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13b TREADY" *)    output       d13b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13b TVALID" *)    input        d13b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13b TDATA" *)     input [63:0] d13b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14b TREADY" *)    output       d14b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14b TVALID" *)    input        d14b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14b TDATA" *)     input [63:0] d14b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15b TREADY" *)    output       d15b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15b TVALID" *)    input        d15b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15b TDATA" *)     input [63:0] d15b_axi_tdat, 
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16b TREADY" *)    output       d16b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16b TVALID" *)    input        d16b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16b TDATA" *)     input [63:0] d16b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17b TREADY" *)    output       d17b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17b TVALID" *)    input        d17b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17b TDATA" *)     input [63:0] d17b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18b TREADY" *)    output       d18b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18b TVALID" *)    input        d18b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18b TDATA" *)     input [63:0] d18b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19b TREADY" *)    output       d19b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19b TVALID" *)    input        d19b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19b TDATA" *)     input [63:0] d19b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20b TREADY" *)    output       d20b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20b TVALID" *)    input        d20b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20b TDATA" *)     input [63:0] d20b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21b TREADY" *)    output       d21b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21b TVALID" *)    input        d21b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21b TDATA" *)     input [63:0] d21b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22b TREADY" *)    output       d22b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22b TVALID" *)    input        d22b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22b TDATA" *)     input [63:0] d22b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23b TREADY" *)    output       d23b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23b TVALID" *)    input        d23b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23b TDATA" *)     input [63:0] d23b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24b TREADY" *)    output       d24b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24b TVALID" *)    input        d24b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24b TDATA" *)     input [63:0] d24b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25b TREADY" *)    output       d25b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25b TVALID" *)    input        d25b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25b TDATA" *)     input [63:0] d25b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26b TREADY" *)    output       d26b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26b TVALID" *)    input        d26b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26b TDATA" *)     input [63:0] d26b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27b TREADY" *)    output       d27b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27b TVALID" *)    input        d27b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27b TDATA" *)     input [63:0] d27b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28b TREADY" *)    output       d28b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28b TVALID" *)    input        d28b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28b TDATA" *)     input [63:0] d28b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29b TREADY" *)    output       d29b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29b TVALID" *)    input        d29b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29b TDATA" *)     input [63:0] d29b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30b TREADY" *)    output       d30b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30b TVALID" *)    input        d30b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30b TDATA" *)     input [63:0] d30b_axi_tdat,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31b TREADY" *)    output       d31b_axi_trdy,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31b TVALID" *)    input        d31b_axi_tvld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31b TDATA" *)     input [63:0] d31b_axi_tdat
);


//--------------------------------------------------
// Register Signals
//--------------------------------------------------
wire [47:0] r_smp_reg[0:63];
wire [47:0] r_err_reg[0:63];
wire [47:0] r_idl_reg[0:63];
wire [15:0] r_lat_reg[0:63];


reg test_en_reg;
wire test_en_aie;

wire [63:0] r_vld_aie;
wire [6:0]  r_smp_aie[0:63];
wire [6:0]  r_err_aie[0:63];
wire [11:0] r_idl_aie[0:63];
wire [15:0] r_lat_aie[0:63];


wire        dout_done_reg;
wire [63:0] dout_done_aie;


reg [63:0] d_axi_tvld;
reg [15:0] d_axi_tdat[0:63];



//------------------------------------
// Reset Signals
//------------------------------------
reg  srst, srst_p, srst_xclk;
wire  srst_2;
reg  dout_rst, vld_all, vld_all_d;
reg [7:0] rstcnt;


//----------------------------------------------------------
// Register Map and System Controls
//----------------------------------------------------------
tst_dout_regs #(C_S_AXI_CONTROL_ADDR_WIDTH, C_S_AXI_CONTROL_DATA_WIDTH) REGS(
    reg_clk, srst,
    s_axi_control_awaddr, s_axi_control_awvalid, s_axi_control_awready, 
	s_axi_control_wdata, s_axi_control_wstrb, s_axi_control_wvalid, s_axi_control_wready,
    s_axi_control_bresp,  s_axi_control_bvalid,  s_axi_control_bready,
    s_axi_control_araddr, s_axi_control_arvalid, s_axi_control_arready, 
	s_axi_control_rdata, s_axi_control_rresp, s_axi_control_rvalid, s_axi_control_rready,
	//-------------------------------------------------
	// Status Registers
	//-------------------------------------------------
	test_en_reg,
 	r_smp_reg[0],	    r_err_reg[0],	    r_idl_reg[0],	    r_lat_reg[0],
	r_smp_reg[1],	    r_err_reg[1],	    r_idl_reg[1],	    r_lat_reg[1],
 	r_smp_reg[2],	    r_err_reg[2],	    r_idl_reg[2],	    r_lat_reg[2],
	r_smp_reg[3],	    r_err_reg[3],	    r_idl_reg[3],	    r_lat_reg[3],
 	r_smp_reg[4],	    r_err_reg[4],	    r_idl_reg[4],	    r_lat_reg[4],
	r_smp_reg[5],	    r_err_reg[5],	    r_idl_reg[5],	    r_lat_reg[5],
 	r_smp_reg[6],	    r_err_reg[6],	    r_idl_reg[6],	    r_lat_reg[6],
	r_smp_reg[7],	    r_err_reg[7],	    r_idl_reg[7],	    r_lat_reg[7],
 	r_smp_reg[8+0],	    r_err_reg[8+0],	    r_idl_reg[8+0],	    r_lat_reg[8+0],
	r_smp_reg[8+1],	    r_err_reg[8+1],	    r_idl_reg[8+1],	    r_lat_reg[8+1],
 	r_smp_reg[8+2],	    r_err_reg[8+2],	    r_idl_reg[8+2],	    r_lat_reg[8+2],
	r_smp_reg[8+3],	    r_err_reg[8+3],	    r_idl_reg[8+3],	    r_lat_reg[8+3],
 	r_smp_reg[8+4],	    r_err_reg[8+4],	    r_idl_reg[8+4],	    r_lat_reg[8+4],
	r_smp_reg[8+5],	    r_err_reg[8+5],	    r_idl_reg[8+5],	    r_lat_reg[8+5],
 	r_smp_reg[8+6],	    r_err_reg[8+6],	    r_idl_reg[8+6],	    r_lat_reg[8+6],
	r_smp_reg[8+7],	    r_err_reg[8+7],	    r_idl_reg[8+7],	    r_lat_reg[8+7],	
 	r_smp_reg[8*2+0],	r_err_reg[8*2+0],	r_idl_reg[8*2+0],	r_lat_reg[8*2+0],
	r_smp_reg[8*2+1],	r_err_reg[8*2+1],	r_idl_reg[8*2+1],	r_lat_reg[8*2+1],
 	r_smp_reg[8*2+2],	r_err_reg[8*2+2],	r_idl_reg[8*2+2],	r_lat_reg[8*2+2],
	r_smp_reg[8*2+3],	r_err_reg[8*2+3],	r_idl_reg[8*2+3],	r_lat_reg[8*2+3],
 	r_smp_reg[8*2+4],	r_err_reg[8*2+4],	r_idl_reg[8*2+4],	r_lat_reg[8*2+4],
	r_smp_reg[8*2+5],	r_err_reg[8*2+5],	r_idl_reg[8*2+5],	r_lat_reg[8*2+5],
 	r_smp_reg[8*2+6],	r_err_reg[8*2+6],	r_idl_reg[8*2+6],	r_lat_reg[8*2+6],
	r_smp_reg[8*2+7],	r_err_reg[8*2+7],	r_idl_reg[8*2+7],	r_lat_reg[8*2+7],
 	r_smp_reg[8*3+0],	r_err_reg[8*3+0],	r_idl_reg[8*3+0],	r_lat_reg[8*3+0],
	r_smp_reg[8*3+1],	r_err_reg[8*3+1],	r_idl_reg[8*3+1],	r_lat_reg[8*3+1],
 	r_smp_reg[8*3+2],	r_err_reg[8*3+2],	r_idl_reg[8*3+2],	r_lat_reg[8*3+2],
	r_smp_reg[8*3+3],	r_err_reg[8*3+3],	r_idl_reg[8*3+3],	r_lat_reg[8*3+3],
 	r_smp_reg[8*3+4],	r_err_reg[8*3+4],	r_idl_reg[8*3+4],	r_lat_reg[8*3+4],
	r_smp_reg[8*3+5],	r_err_reg[8*3+5],	r_idl_reg[8*3+5],	r_lat_reg[8*3+5],
 	r_smp_reg[8*3+6],	r_err_reg[8*3+6],	r_idl_reg[8*3+6],	r_lat_reg[8*3+6],
	r_smp_reg[8*3+7],	r_err_reg[8*3+7],	r_idl_reg[8*3+7],	r_lat_reg[8*3+7],
 	r_smp_reg[8*4+0],	r_err_reg[8*4+0],	r_idl_reg[8*4+0],	r_lat_reg[8*4+0],
	r_smp_reg[8*4+1],	r_err_reg[8*4+1],	r_idl_reg[8*4+1],	r_lat_reg[8*4+1],
 	r_smp_reg[8*4+2],	r_err_reg[8*4+2],	r_idl_reg[8*4+2],	r_lat_reg[8*4+2],
	r_smp_reg[8*4+3],	r_err_reg[8*4+3],	r_idl_reg[8*4+3],	r_lat_reg[8*4+3],
 	r_smp_reg[8*4+4],	r_err_reg[8*4+4],	r_idl_reg[8*4+4],	r_lat_reg[8*4+4],
	r_smp_reg[8*4+5],	r_err_reg[8*4+5],	r_idl_reg[8*4+5],	r_lat_reg[8*4+5],
 	r_smp_reg[8*4+6],	r_err_reg[8*4+6],	r_idl_reg[8*4+6],	r_lat_reg[8*4+6],
	r_smp_reg[8*4+7],	r_err_reg[8*4+7],	r_idl_reg[8*4+7],	r_lat_reg[8*4+7],
 	r_smp_reg[8*5+0],	r_err_reg[8*5+0],	r_idl_reg[8*5+0],	r_lat_reg[8*5+0],
	r_smp_reg[8*5+1],	r_err_reg[8*5+1],	r_idl_reg[8*5+1],	r_lat_reg[8*5+1],
 	r_smp_reg[8*5+2],	r_err_reg[8*5+2],	r_idl_reg[8*5+2],	r_lat_reg[8*5+2],
	r_smp_reg[8*5+3],	r_err_reg[8*5+3],	r_idl_reg[8*5+3],	r_lat_reg[8*5+3],
 	r_smp_reg[8*5+4],	r_err_reg[8*5+4],	r_idl_reg[8*5+4],	r_lat_reg[8*5+4],
	r_smp_reg[8*5+5],	r_err_reg[8*5+5],	r_idl_reg[8*5+5],	r_lat_reg[8*5+5],
 	r_smp_reg[8*5+6],	r_err_reg[8*5+6],	r_idl_reg[8*5+6],	r_lat_reg[8*5+6],
	r_smp_reg[8*5+7],	r_err_reg[8*5+7],	r_idl_reg[8*5+7],	r_lat_reg[8*5+7],
 	r_smp_reg[8*6+0],	r_err_reg[8*6+0],	r_idl_reg[8*6+0],	r_lat_reg[8*6+0],
	r_smp_reg[8*6+1],	r_err_reg[8*6+1],	r_idl_reg[8*6+1],	r_lat_reg[8*6+1],
 	r_smp_reg[8*6+2],	r_err_reg[8*6+2],	r_idl_reg[8*6+2],	r_lat_reg[8*6+2],
	r_smp_reg[8*6+3],	r_err_reg[8*6+3],	r_idl_reg[8*6+3],	r_lat_reg[8*6+3],
 	r_smp_reg[8*6+4],	r_err_reg[8*6+4],	r_idl_reg[8*6+4],	r_lat_reg[8*6+4],
	r_smp_reg[8*6+5],	r_err_reg[8*6+5],	r_idl_reg[8*6+5],	r_lat_reg[8*6+5],
 	r_smp_reg[8*6+6],	r_err_reg[8*6+6],	r_idl_reg[8*6+6],	r_lat_reg[8*6+6],
	r_smp_reg[8*6+7],	r_err_reg[8*6+7],	r_idl_reg[8*6+7],	r_lat_reg[8*6+7],
 	r_smp_reg[8*7+0],	r_err_reg[8*7+0],	r_idl_reg[8*7+0],	r_lat_reg[8*7+0],
	r_smp_reg[8*7+1],	r_err_reg[8*7+1],	r_idl_reg[8*7+1],	r_lat_reg[8*7+1],
 	r_smp_reg[8*7+2],	r_err_reg[8*7+2],	r_idl_reg[8*7+2],	r_lat_reg[8*7+2],
	r_smp_reg[8*7+3],	r_err_reg[8*7+3],	r_idl_reg[8*7+3],	r_lat_reg[8*7+3],
 	r_smp_reg[8*7+4],	r_err_reg[8*7+4],	r_idl_reg[8*7+4],	r_lat_reg[8*7+4],
	r_smp_reg[8*7+5],	r_err_reg[8*7+5],	r_idl_reg[8*7+5],	r_lat_reg[8*7+5],
 	r_smp_reg[8*7+6],	r_err_reg[8*7+6],	r_idl_reg[8*7+6],	r_lat_reg[8*7+6],
	r_smp_reg[8*7+7],	r_err_reg[8*7+7],	r_idl_reg[8*7+7],	r_lat_reg[8*7+7],
	dout_done_reg
);


//-----------------------------------------------
// Kernel <-> Reg Cross Clock Domain
//-----------------------------------------------
tst_dout_xclk XCK
(
	//--------------------------------
	// register clock domain
	//--------------------------------
	reg_clk, srst_xclk,
	test_en_reg,
 	r_smp_reg[0],	    r_err_reg[0],	    r_idl_reg[0],	    r_lat_reg[0],
	r_smp_reg[1],	    r_err_reg[1],	    r_idl_reg[1],	    r_lat_reg[1],
 	r_smp_reg[2],	    r_err_reg[2],	    r_idl_reg[2],	    r_lat_reg[2],
	r_smp_reg[3],	    r_err_reg[3],	    r_idl_reg[3],	    r_lat_reg[3],
 	r_smp_reg[4],	    r_err_reg[4],	    r_idl_reg[4],	    r_lat_reg[4],
	r_smp_reg[5],	    r_err_reg[5],	    r_idl_reg[5],	    r_lat_reg[5],
 	r_smp_reg[6],	    r_err_reg[6],	    r_idl_reg[6],	    r_lat_reg[6],
	r_smp_reg[7],	    r_err_reg[7],	    r_idl_reg[7],	    r_lat_reg[7],
 	r_smp_reg[8+0],	    r_err_reg[8+0],	    r_idl_reg[8+0],	    r_lat_reg[8+0],
	r_smp_reg[8+1],	    r_err_reg[8+1],	    r_idl_reg[8+1],	    r_lat_reg[8+1],
 	r_smp_reg[8+2],	    r_err_reg[8+2],	    r_idl_reg[8+2],	    r_lat_reg[8+2],
	r_smp_reg[8+3],	    r_err_reg[8+3],	    r_idl_reg[8+3],	    r_lat_reg[8+3],
 	r_smp_reg[8+4],	    r_err_reg[8+4],	    r_idl_reg[8+4],	    r_lat_reg[8+4],
	r_smp_reg[8+5],	    r_err_reg[8+5],	    r_idl_reg[8+5],	    r_lat_reg[8+5],
 	r_smp_reg[8+6],	    r_err_reg[8+6],	    r_idl_reg[8+6],	    r_lat_reg[8+6],
	r_smp_reg[8+7],	    r_err_reg[8+7],	    r_idl_reg[8+7],	    r_lat_reg[8+7],	
 	r_smp_reg[8*2+0],	r_err_reg[8*2+0],	r_idl_reg[8*2+0],	r_lat_reg[8*2+0],
	r_smp_reg[8*2+1],	r_err_reg[8*2+1],	r_idl_reg[8*2+1],	r_lat_reg[8*2+1],
 	r_smp_reg[8*2+2],	r_err_reg[8*2+2],	r_idl_reg[8*2+2],	r_lat_reg[8*2+2],
	r_smp_reg[8*2+3],	r_err_reg[8*2+3],	r_idl_reg[8*2+3],	r_lat_reg[8*2+3],
 	r_smp_reg[8*2+4],	r_err_reg[8*2+4],	r_idl_reg[8*2+4],	r_lat_reg[8*2+4],
	r_smp_reg[8*2+5],	r_err_reg[8*2+5],	r_idl_reg[8*2+5],	r_lat_reg[8*2+5],
 	r_smp_reg[8*2+6],	r_err_reg[8*2+6],	r_idl_reg[8*2+6],	r_lat_reg[8*2+6],
	r_smp_reg[8*2+7],	r_err_reg[8*2+7],	r_idl_reg[8*2+7],	r_lat_reg[8*2+7],
 	r_smp_reg[8*3+0],	r_err_reg[8*3+0],	r_idl_reg[8*3+0],	r_lat_reg[8*3+0],
	r_smp_reg[8*3+1],	r_err_reg[8*3+1],	r_idl_reg[8*3+1],	r_lat_reg[8*3+1],
 	r_smp_reg[8*3+2],	r_err_reg[8*3+2],	r_idl_reg[8*3+2],	r_lat_reg[8*3+2],
	r_smp_reg[8*3+3],	r_err_reg[8*3+3],	r_idl_reg[8*3+3],	r_lat_reg[8*3+3],
 	r_smp_reg[8*3+4],	r_err_reg[8*3+4],	r_idl_reg[8*3+4],	r_lat_reg[8*3+4],
	r_smp_reg[8*3+5],	r_err_reg[8*3+5],	r_idl_reg[8*3+5],	r_lat_reg[8*3+5],
 	r_smp_reg[8*3+6],	r_err_reg[8*3+6],	r_idl_reg[8*3+6],	r_lat_reg[8*3+6],
	r_smp_reg[8*3+7],	r_err_reg[8*3+7],	r_idl_reg[8*3+7],	r_lat_reg[8*3+7],
 	r_smp_reg[8*4+0],
	r_err_reg[8*4+0],
	r_idl_reg[8*4+0],
	r_lat_reg[8*4+0],
	r_smp_reg[8*4+1],	r_err_reg[8*4+1],	r_idl_reg[8*4+1],	r_lat_reg[8*4+1],
 	r_smp_reg[8*4+2],	r_err_reg[8*4+2],	r_idl_reg[8*4+2],	r_lat_reg[8*4+2],
	r_smp_reg[8*4+3],	r_err_reg[8*4+3],	r_idl_reg[8*4+3],	r_lat_reg[8*4+3],
 	r_smp_reg[8*4+4],	r_err_reg[8*4+4],	r_idl_reg[8*4+4],	r_lat_reg[8*4+4],
	r_smp_reg[8*4+5],	r_err_reg[8*4+5],	r_idl_reg[8*4+5],	r_lat_reg[8*4+5],
 	r_smp_reg[8*4+6],	r_err_reg[8*4+6],	r_idl_reg[8*4+6],	r_lat_reg[8*4+6],
	r_smp_reg[8*4+7],	r_err_reg[8*4+7],	r_idl_reg[8*4+7],	r_lat_reg[8*4+7],
 	r_smp_reg[8*5+0],	r_err_reg[8*5+0],	r_idl_reg[8*5+0],	r_lat_reg[8*5+0],
	r_smp_reg[8*5+1],	r_err_reg[8*5+1],	r_idl_reg[8*5+1],	r_lat_reg[8*5+1],
 	r_smp_reg[8*5+2],	r_err_reg[8*5+2],	r_idl_reg[8*5+2],	r_lat_reg[8*5+2],
	r_smp_reg[8*5+3],	r_err_reg[8*5+3],	r_idl_reg[8*5+3],	r_lat_reg[8*5+3],
 	r_smp_reg[8*5+4],	r_err_reg[8*5+4],	r_idl_reg[8*5+4],	r_lat_reg[8*5+4],
	r_smp_reg[8*5+5],	r_err_reg[8*5+5],	r_idl_reg[8*5+5],	r_lat_reg[8*5+5],
 	r_smp_reg[8*5+6],	r_err_reg[8*5+6],	r_idl_reg[8*5+6],	r_lat_reg[8*5+6],
	r_smp_reg[8*5+7],	r_err_reg[8*5+7],	r_idl_reg[8*5+7],	r_lat_reg[8*5+7],
 	r_smp_reg[8*6+0],	r_err_reg[8*6+0],	r_idl_reg[8*6+0],	r_lat_reg[8*6+0],
	r_smp_reg[8*6+1],	r_err_reg[8*6+1],	r_idl_reg[8*6+1],	r_lat_reg[8*6+1],
 	r_smp_reg[8*6+2],	r_err_reg[8*6+2],	r_idl_reg[8*6+2],	r_lat_reg[8*6+2],
	r_smp_reg[8*6+3],	r_err_reg[8*6+3],	r_idl_reg[8*6+3],	r_lat_reg[8*6+3],
 	r_smp_reg[8*6+4],	r_err_reg[8*6+4],	r_idl_reg[8*6+4],	r_lat_reg[8*6+4],
	r_smp_reg[8*6+5],	r_err_reg[8*6+5],	r_idl_reg[8*6+5],	r_lat_reg[8*6+5],
 	r_smp_reg[8*6+6],	r_err_reg[8*6+6],	r_idl_reg[8*6+6],	r_lat_reg[8*6+6],
	r_smp_reg[8*6+7],	r_err_reg[8*6+7],	r_idl_reg[8*6+7],	r_lat_reg[8*6+7],
 	r_smp_reg[8*7+0],	r_err_reg[8*7+0],	r_idl_reg[8*7+0],	r_lat_reg[8*7+0],
	r_smp_reg[8*7+1],	r_err_reg[8*7+1],	r_idl_reg[8*7+1],	r_lat_reg[8*7+1],
 	r_smp_reg[8*7+2],	r_err_reg[8*7+2],	r_idl_reg[8*7+2],	r_lat_reg[8*7+2],
	r_smp_reg[8*7+3],	r_err_reg[8*7+3],	r_idl_reg[8*7+3],	r_lat_reg[8*7+3],
 	r_smp_reg[8*7+4],	r_err_reg[8*7+4],	r_idl_reg[8*7+4],	r_lat_reg[8*7+4],
	r_smp_reg[8*7+5],	r_err_reg[8*7+5],	r_idl_reg[8*7+5],	r_lat_reg[8*7+5],
 	r_smp_reg[8*7+6],	r_err_reg[8*7+6],	r_idl_reg[8*7+6],	r_lat_reg[8*7+6],
	r_smp_reg[8*7+7],	r_err_reg[8*7+7],	r_idl_reg[8*7+7],	r_lat_reg[8*7+7],
	dout_done_reg,
	//----------------------------------
	// aie clock domain
	//----------------------------------	
	dat_clk, test_en_aie,
	r_vld_aie[0], r_smp_aie[0], r_err_aie[0], r_idl_aie[0], r_lat_aie[0],	
	r_vld_aie[1], r_smp_aie[1], r_err_aie[1], r_idl_aie[1], r_lat_aie[1],
	r_vld_aie[2], r_smp_aie[2], r_err_aie[2], r_idl_aie[2], r_lat_aie[2],	
	r_vld_aie[3], r_smp_aie[3], r_err_aie[3], r_idl_aie[3], r_lat_aie[3],
	r_vld_aie[4], r_smp_aie[4], r_err_aie[4], r_idl_aie[4], r_lat_aie[4],	
	r_vld_aie[5], r_smp_aie[5], r_err_aie[5], r_idl_aie[5], r_lat_aie[5],
	r_vld_aie[6], r_smp_aie[6], r_err_aie[6], r_idl_aie[6], r_lat_aie[6],	
	r_vld_aie[7], r_smp_aie[7], r_err_aie[7], r_idl_aie[7], r_lat_aie[7],
	r_vld_aie[8+0], r_smp_aie[8+0], r_err_aie[8+0], r_idl_aie[8+0], r_lat_aie[8+0],	
	r_vld_aie[8+1], r_smp_aie[8+1], r_err_aie[8+1], r_idl_aie[8+1], r_lat_aie[8+1],
	r_vld_aie[8+2], r_smp_aie[8+2], r_err_aie[8+2], r_idl_aie[8+2], r_lat_aie[8+2],	
	r_vld_aie[8+3], r_smp_aie[8+3], r_err_aie[8+3], r_idl_aie[8+3], r_lat_aie[8+3],
	r_vld_aie[8+4], r_smp_aie[8+4], r_err_aie[8+4], r_idl_aie[8+4], r_lat_aie[8+4],	
	r_vld_aie[8+5], r_smp_aie[8+5], r_err_aie[8+5], r_idl_aie[8+5], r_lat_aie[8+5],
	r_vld_aie[8+6], r_smp_aie[8+6], r_err_aie[8+6], r_idl_aie[8+6], r_lat_aie[8+6],	
	r_vld_aie[8+7], r_smp_aie[8+7], r_err_aie[8+7], r_idl_aie[8+7], r_lat_aie[8+7],	
	r_vld_aie[8*2+0], r_smp_aie[8*2+0], r_err_aie[8*2+0], r_idl_aie[8*2+0], r_lat_aie[8*2+0],	
	r_vld_aie[8*2+1], r_smp_aie[8*2+1], r_err_aie[8*2+1], r_idl_aie[8*2+1], r_lat_aie[8*2+1],
	r_vld_aie[8*2+2], r_smp_aie[8*2+2], r_err_aie[8*2+2], r_idl_aie[8*2+2], r_lat_aie[8*2+2],	
	r_vld_aie[8*2+3], r_smp_aie[8*2+3], r_err_aie[8*2+3], r_idl_aie[8*2+3], r_lat_aie[8*2+3],
	r_vld_aie[8*2+4], r_smp_aie[8*2+4], r_err_aie[8*2+4], r_idl_aie[8*2+4], r_lat_aie[8*2+4],	
	r_vld_aie[8*2+5], r_smp_aie[8*2+5], r_err_aie[8*2+5], r_idl_aie[8*2+5], r_lat_aie[8*2+5],
	r_vld_aie[8*2+6], r_smp_aie[8*2+6], r_err_aie[8*2+6], r_idl_aie[8*2+6], r_lat_aie[8*2+6],	
	r_vld_aie[8*2+7], r_smp_aie[8*2+7], r_err_aie[8*2+7], r_idl_aie[8*2+7], r_lat_aie[8*2+7],
	r_vld_aie[8*3+0], r_smp_aie[8*3+0], r_err_aie[8*3+0], r_idl_aie[8*3+0], r_lat_aie[8*3+0],	
	r_vld_aie[8*3+1], r_smp_aie[8*3+1], r_err_aie[8*3+1], r_idl_aie[8*3+1], r_lat_aie[8*3+1],
	r_vld_aie[8*3+2], r_smp_aie[8*3+2], r_err_aie[8*3+2], r_idl_aie[8*3+2], r_lat_aie[8*3+2],	
	r_vld_aie[8*3+3], r_smp_aie[8*3+3], r_err_aie[8*3+3], r_idl_aie[8*3+3], r_lat_aie[8*3+3],
	r_vld_aie[8*3+4], r_smp_aie[8*3+4], r_err_aie[8*3+4], r_idl_aie[8*3+4], r_lat_aie[8*3+4],	
	r_vld_aie[8*3+5], r_smp_aie[8*3+5], r_err_aie[8*3+5], r_idl_aie[8*3+5], r_lat_aie[8*3+5],
	r_vld_aie[8*3+6], r_smp_aie[8*3+6], r_err_aie[8*3+6], r_idl_aie[8*3+6], r_lat_aie[8*3+6],	
	r_vld_aie[8*3+7], r_smp_aie[8*3+7], r_err_aie[8*3+7], r_idl_aie[8*3+7], r_lat_aie[8*3+7],		
	r_vld_aie[8*4+0], r_smp_aie[8*4+0], r_err_aie[8*4+0], r_idl_aie[8*4+0], r_lat_aie[8*4+0],	
	r_vld_aie[8*4+1], r_smp_aie[8*4+1], r_err_aie[8*4+1], r_idl_aie[8*4+1], r_lat_aie[8*4+1],
	r_vld_aie[8*4+2], r_smp_aie[8*4+2], r_err_aie[8*4+2], r_idl_aie[8*4+2], r_lat_aie[8*4+2],	
	r_vld_aie[8*4+3], r_smp_aie[8*4+3], r_err_aie[8*4+3], r_idl_aie[8*4+3], r_lat_aie[8*4+3],
	r_vld_aie[8*4+4], r_smp_aie[8*4+4], r_err_aie[8*4+4], r_idl_aie[8*4+4], r_lat_aie[8*4+4],	
	r_vld_aie[8*4+5], r_smp_aie[8*4+5], r_err_aie[8*4+5], r_idl_aie[8*4+5], r_lat_aie[8*4+5],
	r_vld_aie[8*4+6], r_smp_aie[8*4+6], r_err_aie[8*4+6], r_idl_aie[8*4+6], r_lat_aie[8*4+6],	
	r_vld_aie[8*4+7], r_smp_aie[8*4+7], r_err_aie[8*4+7], r_idl_aie[8*4+7], r_lat_aie[8*4+7],
	r_vld_aie[8*5+0], r_smp_aie[8*5+0], r_err_aie[8*5+0], r_idl_aie[8*5+0], r_lat_aie[8*5+0],	
	r_vld_aie[8*5+1], r_smp_aie[8*5+1], r_err_aie[8*5+1], r_idl_aie[8*5+1], r_lat_aie[8*5+1],
	r_vld_aie[8*5+2], r_smp_aie[8*5+2], r_err_aie[8*5+2], r_idl_aie[8*5+2], r_lat_aie[8*5+2],	
	r_vld_aie[8*5+3], r_smp_aie[8*5+3], r_err_aie[8*5+3], r_idl_aie[8*5+3], r_lat_aie[8*5+3],
	r_vld_aie[8*5+4], r_smp_aie[8*5+4], r_err_aie[8*5+4], r_idl_aie[8*5+4], r_lat_aie[8*5+4],	
	r_vld_aie[8*5+5], r_smp_aie[8*5+5], r_err_aie[8*5+5], r_idl_aie[8*5+5], r_lat_aie[8*5+5],
	r_vld_aie[8*5+6], r_smp_aie[8*5+6], r_err_aie[8*5+6], r_idl_aie[8*5+6], r_lat_aie[8*5+6],	
	r_vld_aie[8*5+7], r_smp_aie[8*5+7], r_err_aie[8*5+7], r_idl_aie[8*5+7], r_lat_aie[8*5+7],	
	r_vld_aie[8*6+0], r_smp_aie[8*6+0], r_err_aie[8*6+0], r_idl_aie[8*6+0], r_lat_aie[8*6+0],	
	r_vld_aie[8*6+1], r_smp_aie[8*6+1], r_err_aie[8*6+1], r_idl_aie[8*6+1], r_lat_aie[8*6+1],
	r_vld_aie[8*6+2], r_smp_aie[8*6+2], r_err_aie[8*6+2], r_idl_aie[8*6+2], r_lat_aie[8*6+2],	
	r_vld_aie[8*6+3], r_smp_aie[8*6+3], r_err_aie[8*6+3], r_idl_aie[8*6+3], r_lat_aie[8*6+3],
	r_vld_aie[8*6+4], r_smp_aie[8*6+4], r_err_aie[8*6+4], r_idl_aie[8*6+4], r_lat_aie[8*6+4],	
	r_vld_aie[8*6+5], r_smp_aie[8*6+5], r_err_aie[8*6+5], r_idl_aie[8*6+5], r_lat_aie[8*6+5],
	r_vld_aie[8*6+6], r_smp_aie[8*6+6], r_err_aie[8*6+6], r_idl_aie[8*6+6], r_lat_aie[8*6+6],	
	r_vld_aie[8*6+7], r_smp_aie[8*6+7], r_err_aie[8*6+7], r_idl_aie[8*6+7], r_lat_aie[8*6+7],
	r_vld_aie[8*7+0], r_smp_aie[8*7+0], r_err_aie[8*7+0], r_idl_aie[8*7+0], r_lat_aie[8*7+0],	
	r_vld_aie[8*7+1], r_smp_aie[8*7+1], r_err_aie[8*7+1], r_idl_aie[8*7+1], r_lat_aie[8*7+1],
	r_vld_aie[8*7+2], r_smp_aie[8*7+2], r_err_aie[8*7+2], r_idl_aie[8*7+2], r_lat_aie[8*7+2],	
	r_vld_aie[8*7+3], r_smp_aie[8*7+3], r_err_aie[8*7+3], r_idl_aie[8*7+3], r_lat_aie[8*7+3],
	r_vld_aie[8*7+4], r_smp_aie[8*7+4], r_err_aie[8*7+4], r_idl_aie[8*7+4], r_lat_aie[8*7+4],	
	r_vld_aie[8*7+5], r_smp_aie[8*7+5], r_err_aie[8*7+5], r_idl_aie[8*7+5], r_lat_aie[8*7+5],
	r_vld_aie[8*7+6], r_smp_aie[8*7+6], r_err_aie[8*7+6], r_idl_aie[8*7+6], r_lat_aie[8*7+6],	
	r_vld_aie[8*7+7], r_smp_aie[8*7+7], r_err_aie[8*7+7], r_idl_aie[8*7+7], r_lat_aie[8*7+7],			
	dout_done_aie
);


//-----------------------------------------------
// Output Monitor
//-----------------------------------------------
assign d00a_axi_trdy = 1'b1;
assign d01a_axi_trdy = 1'b1;
assign d02a_axi_trdy = 1'b1;
assign d03a_axi_trdy = 1'b1;
assign d04a_axi_trdy = 1'b1;
assign d05a_axi_trdy = 1'b1;
assign d06a_axi_trdy = 1'b1;
assign d07a_axi_trdy = 1'b1;
assign d08a_axi_trdy = 1'b1;
assign d09a_axi_trdy = 1'b1;
assign d10a_axi_trdy = 1'b1;
assign d11a_axi_trdy = 1'b1;
assign d12a_axi_trdy = 1'b1;
assign d13a_axi_trdy = 1'b1;
assign d14a_axi_trdy = 1'b1;
assign d15a_axi_trdy = 1'b1;
assign d16a_axi_trdy = 1'b1;
assign d17a_axi_trdy = 1'b1;
assign d18a_axi_trdy = 1'b1;
assign d19a_axi_trdy = 1'b1;
assign d20a_axi_trdy = 1'b1;
assign d21a_axi_trdy = 1'b1;
assign d22a_axi_trdy = 1'b1;
assign d23a_axi_trdy = 1'b1;
assign d24a_axi_trdy = 1'b1;
assign d25a_axi_trdy = 1'b1;
assign d26a_axi_trdy = 1'b1;
assign d27a_axi_trdy = 1'b1;
assign d28a_axi_trdy = 1'b1;
assign d29a_axi_trdy = 1'b1;
assign d30a_axi_trdy = 1'b1;
assign d31a_axi_trdy = 1'b1;
assign d00b_axi_trdy = 1'b1;
assign d01b_axi_trdy = 1'b1;
assign d02b_axi_trdy = 1'b1;
assign d03b_axi_trdy = 1'b1;
assign d04b_axi_trdy = 1'b1;
assign d05b_axi_trdy = 1'b1;
assign d06b_axi_trdy = 1'b1;
assign d07b_axi_trdy = 1'b1;
assign d08b_axi_trdy = 1'b1;
assign d09b_axi_trdy = 1'b1;
assign d10b_axi_trdy = 1'b1;
assign d11b_axi_trdy = 1'b1;
assign d12b_axi_trdy = 1'b1;
assign d13b_axi_trdy = 1'b1;
assign d14b_axi_trdy = 1'b1;
assign d15b_axi_trdy = 1'b1;
assign d16b_axi_trdy = 1'b1;
assign d17b_axi_trdy = 1'b1;
assign d18b_axi_trdy = 1'b1;
assign d19b_axi_trdy = 1'b1;
assign d20b_axi_trdy = 1'b1;
assign d21b_axi_trdy = 1'b1;
assign d22b_axi_trdy = 1'b1;
assign d23b_axi_trdy = 1'b1;
assign d24b_axi_trdy = 1'b1;
assign d25b_axi_trdy = 1'b1;
assign d26b_axi_trdy = 1'b1;
assign d27b_axi_trdy = 1'b1;
assign d28b_axi_trdy = 1'b1;
assign d29b_axi_trdy = 1'b1;
assign d30b_axi_trdy = 1'b1;
assign d31b_axi_trdy = 1'b1;

always @(posedge dat_clk) begin

	d_axi_tvld[    0]<=d00a_axi_tvld;		d_axi_tdat[    0] <= d00a_axi_tdat[63:48] ^ d00a_axi_tdat[47:32] ^ d00a_axi_tdat[31:16] ^ d00a_axi_tdat[15:0];
	d_axi_tvld[    1]<=d01a_axi_tvld;		d_axi_tdat[    1] <= d01a_axi_tdat[63:48] ^ d01a_axi_tdat[47:32] ^ d01a_axi_tdat[31:16] ^ d01a_axi_tdat[15:0];
	d_axi_tvld[    2]<=d02a_axi_tvld;		d_axi_tdat[    2] <= d02a_axi_tdat[63:48] ^ d02a_axi_tdat[47:32] ^ d02a_axi_tdat[31:16] ^ d02a_axi_tdat[15:0];
	d_axi_tvld[    3]<=d03a_axi_tvld;		d_axi_tdat[    3] <= d03a_axi_tdat[63:48] ^ d03a_axi_tdat[47:32] ^ d03a_axi_tdat[31:16] ^ d03a_axi_tdat[15:0];
	d_axi_tvld[    4]<=d04a_axi_tvld;		d_axi_tdat[    4] <= d04a_axi_tdat[63:48] ^ d04a_axi_tdat[47:32] ^ d04a_axi_tdat[31:16] ^ d04a_axi_tdat[15:0];
	d_axi_tvld[    5]<=d05a_axi_tvld;		d_axi_tdat[    5] <= d05a_axi_tdat[63:48] ^ d05a_axi_tdat[47:32] ^ d05a_axi_tdat[31:16] ^ d05a_axi_tdat[15:0];
	d_axi_tvld[    6]<=d06a_axi_tvld;		d_axi_tdat[    6] <= d06a_axi_tdat[63:48] ^ d06a_axi_tdat[47:32] ^ d06a_axi_tdat[31:16] ^ d06a_axi_tdat[15:0];
	d_axi_tvld[    7]<=d07a_axi_tvld;		d_axi_tdat[    7] <= d07a_axi_tdat[63:48] ^ d07a_axi_tdat[47:32] ^ d07a_axi_tdat[31:16] ^ d07a_axi_tdat[15:0];
	d_axi_tvld[8  +0]<=d08a_axi_tvld;		d_axi_tdat[8  +0] <= d08a_axi_tdat[63:48] ^ d08a_axi_tdat[47:32] ^ d08a_axi_tdat[31:16] ^ d08a_axi_tdat[15:0];
	d_axi_tvld[8  +1]<=d09a_axi_tvld;		d_axi_tdat[8  +1] <= d09a_axi_tdat[63:48] ^ d09a_axi_tdat[47:32] ^ d09a_axi_tdat[31:16] ^ d09a_axi_tdat[15:0];
	d_axi_tvld[8  +2]<=d10a_axi_tvld;		d_axi_tdat[8  +2] <= d10a_axi_tdat[63:48] ^ d10a_axi_tdat[47:32] ^ d10a_axi_tdat[31:16] ^ d10a_axi_tdat[15:0];
	d_axi_tvld[8  +3]<=d11a_axi_tvld;		d_axi_tdat[8  +3] <= d11a_axi_tdat[63:48] ^ d11a_axi_tdat[47:32] ^ d11a_axi_tdat[31:16] ^ d11a_axi_tdat[15:0];
	d_axi_tvld[8  +4]<=d12a_axi_tvld;		d_axi_tdat[8  +4] <= d12a_axi_tdat[63:48] ^ d12a_axi_tdat[47:32] ^ d12a_axi_tdat[31:16] ^ d12a_axi_tdat[15:0];
	d_axi_tvld[8  +5]<=d13a_axi_tvld;		d_axi_tdat[8  +5] <= d13a_axi_tdat[63:48] ^ d13a_axi_tdat[47:32] ^ d13a_axi_tdat[31:16] ^ d13a_axi_tdat[15:0];
	d_axi_tvld[8  +6]<=d14a_axi_tvld;		d_axi_tdat[8  +6] <= d14a_axi_tdat[63:48] ^ d14a_axi_tdat[47:32] ^ d14a_axi_tdat[31:16] ^ d14a_axi_tdat[15:0];
	d_axi_tvld[8  +7]<=d15a_axi_tvld;		d_axi_tdat[8  +7] <= d15a_axi_tdat[63:48] ^ d15a_axi_tdat[47:32] ^ d15a_axi_tdat[31:16] ^ d15a_axi_tdat[15:0];
	d_axi_tvld[8*2+0]<=d16a_axi_tvld;		d_axi_tdat[8*2+0] <= d16a_axi_tdat[63:48] ^ d16a_axi_tdat[47:32] ^ d16a_axi_tdat[31:16] ^ d16a_axi_tdat[15:0];
	d_axi_tvld[8*2+1]<=d17a_axi_tvld;		d_axi_tdat[8*2+1] <= d17a_axi_tdat[63:48] ^ d17a_axi_tdat[47:32] ^ d17a_axi_tdat[31:16] ^ d17a_axi_tdat[15:0];
	d_axi_tvld[8*2+2]<=d18a_axi_tvld;		d_axi_tdat[8*2+2] <= d18a_axi_tdat[63:48] ^ d18a_axi_tdat[47:32] ^ d18a_axi_tdat[31:16] ^ d18a_axi_tdat[15:0];
	d_axi_tvld[8*2+3]<=d19a_axi_tvld;		d_axi_tdat[8*2+3] <= d19a_axi_tdat[63:48] ^ d19a_axi_tdat[47:32] ^ d19a_axi_tdat[31:16] ^ d19a_axi_tdat[15:0];
	d_axi_tvld[8*2+4]<=d20a_axi_tvld;		d_axi_tdat[8*2+4] <= d20a_axi_tdat[63:48] ^ d20a_axi_tdat[47:32] ^ d20a_axi_tdat[31:16] ^ d20a_axi_tdat[15:0];
	d_axi_tvld[8*2+5]<=d21a_axi_tvld;		d_axi_tdat[8*2+5] <= d21a_axi_tdat[63:48] ^ d21a_axi_tdat[47:32] ^ d21a_axi_tdat[31:16] ^ d21a_axi_tdat[15:0];
	d_axi_tvld[8*2+6]<=d22a_axi_tvld;		d_axi_tdat[8*2+6] <= d22a_axi_tdat[63:48] ^ d22a_axi_tdat[47:32] ^ d22a_axi_tdat[31:16] ^ d22a_axi_tdat[15:0];
	d_axi_tvld[8*2+7]<=d23a_axi_tvld;		d_axi_tdat[8*2+7] <= d23a_axi_tdat[63:48] ^ d23a_axi_tdat[47:32] ^ d23a_axi_tdat[31:16] ^ d23a_axi_tdat[15:0];
	d_axi_tvld[8*3+0]<=d24a_axi_tvld;		d_axi_tdat[8*3+0] <= d24a_axi_tdat[63:48] ^ d24a_axi_tdat[47:32] ^ d24a_axi_tdat[31:16] ^ d24a_axi_tdat[15:0];
	d_axi_tvld[8*3+1]<=d25a_axi_tvld;		d_axi_tdat[8*3+1] <= d25a_axi_tdat[63:48] ^ d25a_axi_tdat[47:32] ^ d25a_axi_tdat[31:16] ^ d25a_axi_tdat[15:0];
	d_axi_tvld[8*3+2]<=d26a_axi_tvld;		d_axi_tdat[8*3+2] <= d26a_axi_tdat[63:48] ^ d26a_axi_tdat[47:32] ^ d26a_axi_tdat[31:16] ^ d26a_axi_tdat[15:0];
	d_axi_tvld[8*3+3]<=d27a_axi_tvld;		d_axi_tdat[8*3+3] <= d27a_axi_tdat[63:48] ^ d27a_axi_tdat[47:32] ^ d27a_axi_tdat[31:16] ^ d27a_axi_tdat[15:0];
	d_axi_tvld[8*3+4]<=d28a_axi_tvld;		d_axi_tdat[8*3+4] <= d28a_axi_tdat[63:48] ^ d28a_axi_tdat[47:32] ^ d28a_axi_tdat[31:16] ^ d28a_axi_tdat[15:0];
	d_axi_tvld[8*3+5]<=d29a_axi_tvld;		d_axi_tdat[8*3+5] <= d29a_axi_tdat[63:48] ^ d29a_axi_tdat[47:32] ^ d29a_axi_tdat[31:16] ^ d29a_axi_tdat[15:0];
	d_axi_tvld[8*3+6]<=d30a_axi_tvld;		d_axi_tdat[8*3+6] <= d30a_axi_tdat[63:48] ^ d30a_axi_tdat[47:32] ^ d30a_axi_tdat[31:16] ^ d30a_axi_tdat[15:0];
	d_axi_tvld[8*3+7]<=d31a_axi_tvld;		d_axi_tdat[8*3+7] <= d31a_axi_tdat[63:48] ^ d31a_axi_tdat[47:32] ^ d31a_axi_tdat[31:16] ^ d31a_axi_tdat[15:0];
	d_axi_tvld[8*4+0]<=d00b_axi_tvld;		d_axi_tdat[8*4+0] <= d00b_axi_tdat[63:48] ^ d00b_axi_tdat[47:32] ^ d00b_axi_tdat[31:16] ^ d00b_axi_tdat[15:0];
	d_axi_tvld[8*4+1]<=d01b_axi_tvld;		d_axi_tdat[8*4+1] <= d01b_axi_tdat[63:48] ^ d01b_axi_tdat[47:32] ^ d01b_axi_tdat[31:16] ^ d01b_axi_tdat[15:0];
	d_axi_tvld[8*4+2]<=d02b_axi_tvld;		d_axi_tdat[8*4+2] <= d02b_axi_tdat[63:48] ^ d02b_axi_tdat[47:32] ^ d02b_axi_tdat[31:16] ^ d02b_axi_tdat[15:0];
	d_axi_tvld[8*4+3]<=d03b_axi_tvld;		d_axi_tdat[8*4+3] <= d03b_axi_tdat[63:48] ^ d03b_axi_tdat[47:32] ^ d03b_axi_tdat[31:16] ^ d03b_axi_tdat[15:0];
	d_axi_tvld[8*4+4]<=d04b_axi_tvld;		d_axi_tdat[8*4+4] <= d04b_axi_tdat[63:48] ^ d04b_axi_tdat[47:32] ^ d04b_axi_tdat[31:16] ^ d04b_axi_tdat[15:0];
	d_axi_tvld[8*4+5]<=d05b_axi_tvld;		d_axi_tdat[8*4+5] <= d05b_axi_tdat[63:48] ^ d05b_axi_tdat[47:32] ^ d05b_axi_tdat[31:16] ^ d05b_axi_tdat[15:0];
	d_axi_tvld[8*4+6]<=d06b_axi_tvld;		d_axi_tdat[8*4+6] <= d06b_axi_tdat[63:48] ^ d06b_axi_tdat[47:32] ^ d06b_axi_tdat[31:16] ^ d06b_axi_tdat[15:0];
	d_axi_tvld[8*4+7]<=d07b_axi_tvld;		d_axi_tdat[8*4+7] <= d07b_axi_tdat[63:48] ^ d07b_axi_tdat[47:32] ^ d07b_axi_tdat[31:16] ^ d07b_axi_tdat[15:0];
	d_axi_tvld[8*5+0]<=d08b_axi_tvld;		d_axi_tdat[8*5+0] <= d08b_axi_tdat[63:48] ^ d08b_axi_tdat[47:32] ^ d08b_axi_tdat[31:16] ^ d08b_axi_tdat[15:0];
	d_axi_tvld[8*5+1]<=d09b_axi_tvld;		d_axi_tdat[8*5+1] <= d09b_axi_tdat[63:48] ^ d09b_axi_tdat[47:32] ^ d09b_axi_tdat[31:16] ^ d09b_axi_tdat[15:0];
	d_axi_tvld[8*5+2]<=d10b_axi_tvld;		d_axi_tdat[8*5+2] <= d10b_axi_tdat[63:48] ^ d10b_axi_tdat[47:32] ^ d10b_axi_tdat[31:16] ^ d10b_axi_tdat[15:0];
	d_axi_tvld[8*5+3]<=d11b_axi_tvld;		d_axi_tdat[8*5+3] <= d11b_axi_tdat[63:48] ^ d11b_axi_tdat[47:32] ^ d11b_axi_tdat[31:16] ^ d11b_axi_tdat[15:0];
	d_axi_tvld[8*5+4]<=d12b_axi_tvld;		d_axi_tdat[8*5+4] <= d12b_axi_tdat[63:48] ^ d12b_axi_tdat[47:32] ^ d12b_axi_tdat[31:16] ^ d12b_axi_tdat[15:0];
	d_axi_tvld[8*5+5]<=d13b_axi_tvld;		d_axi_tdat[8*5+5] <= d13b_axi_tdat[63:48] ^ d13b_axi_tdat[47:32] ^ d13b_axi_tdat[31:16] ^ d13b_axi_tdat[15:0];
	d_axi_tvld[8*5+6]<=d14b_axi_tvld;		d_axi_tdat[8*5+6] <= d14b_axi_tdat[63:48] ^ d14b_axi_tdat[47:32] ^ d14b_axi_tdat[31:16] ^ d14b_axi_tdat[15:0];
	d_axi_tvld[8*5+7]<=d15b_axi_tvld;		d_axi_tdat[8*5+7] <= d15b_axi_tdat[63:48] ^ d15b_axi_tdat[47:32] ^ d15b_axi_tdat[31:16] ^ d15b_axi_tdat[15:0];
	d_axi_tvld[8*6+0]<=d16b_axi_tvld;		d_axi_tdat[8*6+0] <= d16b_axi_tdat[63:48] ^ d16b_axi_tdat[47:32] ^ d16b_axi_tdat[31:16] ^ d16b_axi_tdat[15:0];
	d_axi_tvld[8*6+1]<=d17b_axi_tvld;		d_axi_tdat[8*6+1] <= d17b_axi_tdat[63:48] ^ d17b_axi_tdat[47:32] ^ d17b_axi_tdat[31:16] ^ d17b_axi_tdat[15:0];
	d_axi_tvld[8*6+2]<=d18b_axi_tvld;		d_axi_tdat[8*6+2] <= d18b_axi_tdat[63:48] ^ d18b_axi_tdat[47:32] ^ d18b_axi_tdat[31:16] ^ d18b_axi_tdat[15:0];
	d_axi_tvld[8*6+3]<=d19b_axi_tvld;		d_axi_tdat[8*6+3] <= d19b_axi_tdat[63:48] ^ d19b_axi_tdat[47:32] ^ d19b_axi_tdat[31:16] ^ d19b_axi_tdat[15:0];
	d_axi_tvld[8*6+4]<=d20b_axi_tvld;		d_axi_tdat[8*6+4] <= d20b_axi_tdat[63:48] ^ d20b_axi_tdat[47:32] ^ d20b_axi_tdat[31:16] ^ d20b_axi_tdat[15:0];
	d_axi_tvld[8*6+5]<=d21b_axi_tvld;		d_axi_tdat[8*6+5] <= d21b_axi_tdat[63:48] ^ d21b_axi_tdat[47:32] ^ d21b_axi_tdat[31:16] ^ d21b_axi_tdat[15:0];
	d_axi_tvld[8*6+6]<=d22b_axi_tvld;		d_axi_tdat[8*6+6] <= d22b_axi_tdat[63:48] ^ d22b_axi_tdat[47:32] ^ d22b_axi_tdat[31:16] ^ d22b_axi_tdat[15:0];
	d_axi_tvld[8*6+7]<=d23b_axi_tvld;		d_axi_tdat[8*6+7] <= d23b_axi_tdat[63:48] ^ d23b_axi_tdat[47:32] ^ d23b_axi_tdat[31:16] ^ d23b_axi_tdat[15:0];
	d_axi_tvld[8*7+0]<=d24b_axi_tvld;		d_axi_tdat[8*7+0] <= d24b_axi_tdat[63:48] ^ d24b_axi_tdat[47:32] ^ d24b_axi_tdat[31:16] ^ d24b_axi_tdat[15:0];
	d_axi_tvld[8*7+1]<=d25b_axi_tvld;		d_axi_tdat[8*7+1] <= d25b_axi_tdat[63:48] ^ d25b_axi_tdat[47:32] ^ d25b_axi_tdat[31:16] ^ d25b_axi_tdat[15:0];
	d_axi_tvld[8*7+2]<=d26b_axi_tvld;		d_axi_tdat[8*7+2] <= d26b_axi_tdat[63:48] ^ d26b_axi_tdat[47:32] ^ d26b_axi_tdat[31:16] ^ d26b_axi_tdat[15:0];
	d_axi_tvld[8*7+3]<=d27b_axi_tvld;		d_axi_tdat[8*7+3] <= d27b_axi_tdat[63:48] ^ d27b_axi_tdat[47:32] ^ d27b_axi_tdat[31:16] ^ d27b_axi_tdat[15:0];
	d_axi_tvld[8*7+4]<=d28b_axi_tvld;		d_axi_tdat[8*7+4] <= d28b_axi_tdat[63:48] ^ d28b_axi_tdat[47:32] ^ d28b_axi_tdat[31:16] ^ d28b_axi_tdat[15:0];
	d_axi_tvld[8*7+5]<=d29b_axi_tvld;		d_axi_tdat[8*7+5] <= d29b_axi_tdat[63:48] ^ d29b_axi_tdat[47:32] ^ d29b_axi_tdat[31:16] ^ d29b_axi_tdat[15:0];
	d_axi_tvld[8*7+6]<=d30b_axi_tvld;		d_axi_tdat[8*7+6] <= d30b_axi_tdat[63:48] ^ d30b_axi_tdat[47:32] ^ d30b_axi_tdat[31:16] ^ d30b_axi_tdat[15:0];
	d_axi_tvld[8*7+7]<=d31b_axi_tvld;		d_axi_tdat[8*7+7] <= d31b_axi_tdat[63:48] ^ d31b_axi_tdat[47:32] ^ d31b_axi_tdat[31:16] ^ d31b_axi_tdat[15:0];

end


tst_dout_mon #("fft_gold_00a.mem", 8, 16, 65536) DOUT00a(dat_clk, srst_2, dout_rst, d_axi_tvld[    0], d_axi_tdat[    0][7:0]^d_axi_tdat[    0][15:8], r_vld_aie[    0], r_smp_aie[    0], r_err_aie[    0], r_idl_aie[    0], r_lat_aie[    0], dout_done_aie[    0]);
tst_dout_mon #("fft_gold_01a.mem", 8, 16, 65536) DOUT01a(dat_clk, srst_2, dout_rst, d_axi_tvld[    1], d_axi_tdat[    1][7:0]^d_axi_tdat[    1][15:8], r_vld_aie[    1], r_smp_aie[    1], r_err_aie[    1], r_idl_aie[    1], r_lat_aie[    1], dout_done_aie[    1]);
tst_dout_mon #("fft_gold_02a.mem", 8, 16, 65536) DOUT02a(dat_clk, srst_2, dout_rst, d_axi_tvld[    2], d_axi_tdat[    2][7:0]^d_axi_tdat[    2][15:8], r_vld_aie[    2], r_smp_aie[    2], r_err_aie[    2], r_idl_aie[    2], r_lat_aie[    2], dout_done_aie[    2]);
tst_dout_mon #("fft_gold_03a.mem", 8, 16, 65536) DOUT03a(dat_clk, srst_2, dout_rst, d_axi_tvld[    3], d_axi_tdat[    3][7:0]^d_axi_tdat[    3][15:8], r_vld_aie[    3], r_smp_aie[    3], r_err_aie[    3], r_idl_aie[    3], r_lat_aie[    3], dout_done_aie[    3]);
tst_dout_mon #("fft_gold_04a.mem", 8, 16, 65536) DOUT04a(dat_clk, srst_2, dout_rst, d_axi_tvld[    4], d_axi_tdat[    4][7:0]^d_axi_tdat[    4][15:8], r_vld_aie[    4], r_smp_aie[    4], r_err_aie[    4], r_idl_aie[    4], r_lat_aie[    4], dout_done_aie[    4]);
tst_dout_mon #("fft_gold_05a.mem", 8, 16, 65536) DOUT05a(dat_clk, srst_2, dout_rst, d_axi_tvld[    5], d_axi_tdat[    5][7:0]^d_axi_tdat[    5][15:8], r_vld_aie[    5], r_smp_aie[    5], r_err_aie[    5], r_idl_aie[    5], r_lat_aie[    5], dout_done_aie[    5]);
tst_dout_mon #("fft_gold_06a.mem", 8, 16, 65536) DOUT06a(dat_clk, srst_2, dout_rst, d_axi_tvld[    6], d_axi_tdat[    6][7:0]^d_axi_tdat[    6][15:8], r_vld_aie[    6], r_smp_aie[    6], r_err_aie[    6], r_idl_aie[    6], r_lat_aie[    6], dout_done_aie[    6]);
tst_dout_mon #("fft_gold_07a.mem", 8, 16, 65536) DOUT07a(dat_clk, srst_2, dout_rst, d_axi_tvld[    7], d_axi_tdat[    7][7:0]^d_axi_tdat[    7][15:8], r_vld_aie[    7], r_smp_aie[    7], r_err_aie[    7], r_idl_aie[    7], r_lat_aie[    7], dout_done_aie[    7]);
tst_dout_mon #("fft_gold_08a.mem", 8, 16, 65536) DOUT08a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +0], d_axi_tdat[8  +0][7:0]^d_axi_tdat[8  +0][15:8], r_vld_aie[8  +0], r_smp_aie[8  +0], r_err_aie[8  +0], r_idl_aie[8  +0], r_lat_aie[8  +0], dout_done_aie[8  +0]);
tst_dout_mon #("fft_gold_09a.mem", 8, 16, 65536) DOUT09a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +1], d_axi_tdat[8  +1][7:0]^d_axi_tdat[8  +1][15:8], r_vld_aie[8  +1], r_smp_aie[8  +1], r_err_aie[8  +1], r_idl_aie[8  +1], r_lat_aie[8  +1], dout_done_aie[8  +1]);
tst_dout_mon #("fft_gold_10a.mem", 8, 16, 65536) DOUT10a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +2], d_axi_tdat[8  +2][7:0]^d_axi_tdat[8  +2][15:8], r_vld_aie[8  +2], r_smp_aie[8  +2], r_err_aie[8  +2], r_idl_aie[8  +2], r_lat_aie[8  +2], dout_done_aie[8  +2]);
tst_dout_mon #("fft_gold_11a.mem", 8, 16, 65536) DOUT11a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +3], d_axi_tdat[8  +3][7:0]^d_axi_tdat[8  +3][15:8], r_vld_aie[8  +3], r_smp_aie[8  +3], r_err_aie[8  +3], r_idl_aie[8  +3], r_lat_aie[8  +3], dout_done_aie[8  +3]);
tst_dout_mon #("fft_gold_12a.mem", 8, 16, 65536) DOUT12a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +4], d_axi_tdat[8  +4][7:0]^d_axi_tdat[8  +4][15:8], r_vld_aie[8  +4], r_smp_aie[8  +4], r_err_aie[8  +4], r_idl_aie[8  +4], r_lat_aie[8  +4], dout_done_aie[8  +4]);
tst_dout_mon #("fft_gold_13a.mem", 8, 16, 65536) DOUT13a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +5], d_axi_tdat[8  +5][7:0]^d_axi_tdat[8  +5][15:8], r_vld_aie[8  +5], r_smp_aie[8  +5], r_err_aie[8  +5], r_idl_aie[8  +5], r_lat_aie[8  +5], dout_done_aie[8  +5]);
tst_dout_mon #("fft_gold_14a.mem", 8, 16, 65536) DOUT14a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +6], d_axi_tdat[8  +6][7:0]^d_axi_tdat[8  +6][15:8], r_vld_aie[8  +6], r_smp_aie[8  +6], r_err_aie[8  +6], r_idl_aie[8  +6], r_lat_aie[8  +6], dout_done_aie[8  +6]);
tst_dout_mon #("fft_gold_15a.mem", 8, 16, 65536) DOUT15a(dat_clk, srst_2, dout_rst, d_axi_tvld[8  +7], d_axi_tdat[8  +7][7:0]^d_axi_tdat[8  +7][15:8], r_vld_aie[8  +7], r_smp_aie[8  +7], r_err_aie[8  +7], r_idl_aie[8  +7], r_lat_aie[8  +7], dout_done_aie[8  +7]);
tst_dout_mon #("fft_gold_16a.mem", 8, 16, 65536) DOUT16a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+0], d_axi_tdat[8*2+0][7:0]^d_axi_tdat[8*2+0][15:8], r_vld_aie[8*2+0], r_smp_aie[8*2+0], r_err_aie[8*2+0], r_idl_aie[8*2+0], r_lat_aie[8*2+0], dout_done_aie[8*2+0]);
tst_dout_mon #("fft_gold_17a.mem", 8, 16, 65536) DOUT17a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+1], d_axi_tdat[8*2+1][7:0]^d_axi_tdat[8*2+1][15:8], r_vld_aie[8*2+1], r_smp_aie[8*2+1], r_err_aie[8*2+1], r_idl_aie[8*2+1], r_lat_aie[8*2+1], dout_done_aie[8*2+1]);
tst_dout_mon #("fft_gold_18a.mem", 8, 16, 65536) DOUT18a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+2], d_axi_tdat[8*2+2][7:0]^d_axi_tdat[8*2+2][15:8], r_vld_aie[8*2+2], r_smp_aie[8*2+2], r_err_aie[8*2+2], r_idl_aie[8*2+2], r_lat_aie[8*2+2], dout_done_aie[8*2+2]);
tst_dout_mon #("fft_gold_19a.mem", 8, 16, 65536) DOUT19a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+3], d_axi_tdat[8*2+3][7:0]^d_axi_tdat[8*2+3][15:8], r_vld_aie[8*2+3], r_smp_aie[8*2+3], r_err_aie[8*2+3], r_idl_aie[8*2+3], r_lat_aie[8*2+3], dout_done_aie[8*2+3]);
tst_dout_mon #("fft_gold_20a.mem", 8, 16, 65536) DOUT20a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+4], d_axi_tdat[8*2+4][7:0]^d_axi_tdat[8*2+4][15:8], r_vld_aie[8*2+4], r_smp_aie[8*2+4], r_err_aie[8*2+4], r_idl_aie[8*2+4], r_lat_aie[8*2+4], dout_done_aie[8*2+4]);
tst_dout_mon #("fft_gold_21a.mem", 8, 16, 65536) DOUT21a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+5], d_axi_tdat[8*2+5][7:0]^d_axi_tdat[8*2+5][15:8], r_vld_aie[8*2+5], r_smp_aie[8*2+5], r_err_aie[8*2+5], r_idl_aie[8*2+5], r_lat_aie[8*2+5], dout_done_aie[8*2+5]);
tst_dout_mon #("fft_gold_22a.mem", 8, 16, 65536) DOUT22a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+6], d_axi_tdat[8*2+6][7:0]^d_axi_tdat[8*2+6][15:8], r_vld_aie[8*2+6], r_smp_aie[8*2+6], r_err_aie[8*2+6], r_idl_aie[8*2+6], r_lat_aie[8*2+6], dout_done_aie[8*2+6]);
tst_dout_mon #("fft_gold_23a.mem", 8, 16, 65536) DOUT23a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*2+7], d_axi_tdat[8*2+7][7:0]^d_axi_tdat[8*2+7][15:8], r_vld_aie[8*2+7], r_smp_aie[8*2+7], r_err_aie[8*2+7], r_idl_aie[8*2+7], r_lat_aie[8*2+7], dout_done_aie[8*2+7]);
tst_dout_mon #("fft_gold_24a.mem", 8, 16, 65536) DOUT24a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+0], d_axi_tdat[8*3+0][7:0]^d_axi_tdat[8*3+0][15:8], r_vld_aie[8*3+0], r_smp_aie[8*3+0], r_err_aie[8*3+0], r_idl_aie[8*3+0], r_lat_aie[8*3+0], dout_done_aie[8*3+0]);
tst_dout_mon #("fft_gold_25a.mem", 8, 16, 65536) DOUT25a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+1], d_axi_tdat[8*3+1][7:0]^d_axi_tdat[8*3+1][15:8], r_vld_aie[8*3+1], r_smp_aie[8*3+1], r_err_aie[8*3+1], r_idl_aie[8*3+1], r_lat_aie[8*3+1], dout_done_aie[8*3+1]);
tst_dout_mon #("fft_gold_26a.mem", 8, 16, 65536) DOUT26a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+2], d_axi_tdat[8*3+2][7:0]^d_axi_tdat[8*3+2][15:8], r_vld_aie[8*3+2], r_smp_aie[8*3+2], r_err_aie[8*3+2], r_idl_aie[8*3+2], r_lat_aie[8*3+2], dout_done_aie[8*3+2]);
tst_dout_mon #("fft_gold_27a.mem", 8, 16, 65536) DOUT27a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+3], d_axi_tdat[8*3+3][7:0]^d_axi_tdat[8*3+3][15:8], r_vld_aie[8*3+3], r_smp_aie[8*3+3], r_err_aie[8*3+3], r_idl_aie[8*3+3], r_lat_aie[8*3+3], dout_done_aie[8*3+3]);
tst_dout_mon #("fft_gold_28a.mem", 8, 16, 65536) DOUT28a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+4], d_axi_tdat[8*3+4][7:0]^d_axi_tdat[8*3+4][15:8], r_vld_aie[8*3+4], r_smp_aie[8*3+4], r_err_aie[8*3+4], r_idl_aie[8*3+4], r_lat_aie[8*3+4], dout_done_aie[8*3+4]);
tst_dout_mon #("fft_gold_29a.mem", 8, 16, 65536) DOUT29a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+5], d_axi_tdat[8*3+5][7:0]^d_axi_tdat[8*3+5][15:8], r_vld_aie[8*3+5], r_smp_aie[8*3+5], r_err_aie[8*3+5], r_idl_aie[8*3+5], r_lat_aie[8*3+5], dout_done_aie[8*3+5]);
tst_dout_mon #("fft_gold_30a.mem", 8, 16, 65536) DOUT30a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+6], d_axi_tdat[8*3+6][7:0]^d_axi_tdat[8*3+6][15:8], r_vld_aie[8*3+6], r_smp_aie[8*3+6], r_err_aie[8*3+6], r_idl_aie[8*3+6], r_lat_aie[8*3+6], dout_done_aie[8*3+6]);
tst_dout_mon #("fft_gold_31a.mem", 8, 16, 65536) DOUT31a(dat_clk, srst_2, dout_rst, d_axi_tvld[8*3+7], d_axi_tdat[8*3+7][7:0]^d_axi_tdat[8*3+7][15:8], r_vld_aie[8*3+7], r_smp_aie[8*3+7], r_err_aie[8*3+7], r_idl_aie[8*3+7], r_lat_aie[8*3+7], dout_done_aie[8*3+7]);
tst_dout_mon #("fft_gold_00b.mem", 8, 16, 65536) DOUT00b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+0], d_axi_tdat[8*4+0][7:0]^d_axi_tdat[8*4+0][15:8], r_vld_aie[8*4+0], r_smp_aie[8*4+0], r_err_aie[8*4+0], r_idl_aie[8*4+0], r_lat_aie[8*4+0], dout_done_aie[8*4+0]);
tst_dout_mon #("fft_gold_01b.mem", 8, 16, 65536) DOUT01b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+1], d_axi_tdat[8*4+1][7:0]^d_axi_tdat[8*4+1][15:8], r_vld_aie[8*4+1], r_smp_aie[8*4+1], r_err_aie[8*4+1], r_idl_aie[8*4+1], r_lat_aie[8*4+1], dout_done_aie[8*4+1]);
tst_dout_mon #("fft_gold_02b.mem", 8, 16, 65536) DOUT02b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+2], d_axi_tdat[8*4+2][7:0]^d_axi_tdat[8*4+2][15:8], r_vld_aie[8*4+2], r_smp_aie[8*4+2], r_err_aie[8*4+2], r_idl_aie[8*4+2], r_lat_aie[8*4+2], dout_done_aie[8*4+2]);
tst_dout_mon #("fft_gold_03b.mem", 8, 16, 65536) DOUT03b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+3], d_axi_tdat[8*4+3][7:0]^d_axi_tdat[8*4+3][15:8], r_vld_aie[8*4+3], r_smp_aie[8*4+3], r_err_aie[8*4+3], r_idl_aie[8*4+3], r_lat_aie[8*4+3], dout_done_aie[8*4+3]);
tst_dout_mon #("fft_gold_04b.mem", 8, 16, 65536) DOUT04b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+4], d_axi_tdat[8*4+4][7:0]^d_axi_tdat[8*4+4][15:8], r_vld_aie[8*4+4], r_smp_aie[8*4+4], r_err_aie[8*4+4], r_idl_aie[8*4+4], r_lat_aie[8*4+4], dout_done_aie[8*4+4]);
tst_dout_mon #("fft_gold_05b.mem", 8, 16, 65536) DOUT05b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+5], d_axi_tdat[8*4+5][7:0]^d_axi_tdat[8*4+5][15:8], r_vld_aie[8*4+5], r_smp_aie[8*4+5], r_err_aie[8*4+5], r_idl_aie[8*4+5], r_lat_aie[8*4+5], dout_done_aie[8*4+5]);
tst_dout_mon #("fft_gold_06b.mem", 8, 16, 65536) DOUT06b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+6], d_axi_tdat[8*4+6][7:0]^d_axi_tdat[8*4+6][15:8], r_vld_aie[8*4+6], r_smp_aie[8*4+6], r_err_aie[8*4+6], r_idl_aie[8*4+6], r_lat_aie[8*4+6], dout_done_aie[8*4+6]);
tst_dout_mon #("fft_gold_07b.mem", 8, 16, 65536) DOUT07b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*4+7], d_axi_tdat[8*4+7][7:0]^d_axi_tdat[8*4+7][15:8], r_vld_aie[8*4+7], r_smp_aie[8*4+7], r_err_aie[8*4+7], r_idl_aie[8*4+7], r_lat_aie[8*4+7], dout_done_aie[8*4+7]);
tst_dout_mon #("fft_gold_08b.mem", 8, 16, 65536) DOUT08b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+0], d_axi_tdat[8*5+0][7:0]^d_axi_tdat[8*5+0][15:8], r_vld_aie[8*5+0], r_smp_aie[8*5+0], r_err_aie[8*5+0], r_idl_aie[8*5+0], r_lat_aie[8*5+0], dout_done_aie[8*5+0]);
tst_dout_mon #("fft_gold_09b.mem", 8, 16, 65536) DOUT09b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+1], d_axi_tdat[8*5+1][7:0]^d_axi_tdat[8*5+1][15:8], r_vld_aie[8*5+1], r_smp_aie[8*5+1], r_err_aie[8*5+1], r_idl_aie[8*5+1], r_lat_aie[8*5+1], dout_done_aie[8*5+1]);
tst_dout_mon #("fft_gold_10b.mem", 8, 16, 65536) DOUT10b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+2], d_axi_tdat[8*5+2][7:0]^d_axi_tdat[8*5+2][15:8], r_vld_aie[8*5+2], r_smp_aie[8*5+2], r_err_aie[8*5+2], r_idl_aie[8*5+2], r_lat_aie[8*5+2], dout_done_aie[8*5+2]);
tst_dout_mon #("fft_gold_11b.mem", 8, 16, 65536) DOUT11b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+3], d_axi_tdat[8*5+3][7:0]^d_axi_tdat[8*5+3][15:8], r_vld_aie[8*5+3], r_smp_aie[8*5+3], r_err_aie[8*5+3], r_idl_aie[8*5+3], r_lat_aie[8*5+3], dout_done_aie[8*5+3]);
tst_dout_mon #("fft_gold_12b.mem", 8, 16, 65536) DOUT12b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+4], d_axi_tdat[8*5+4][7:0]^d_axi_tdat[8*5+4][15:8], r_vld_aie[8*5+4], r_smp_aie[8*5+4], r_err_aie[8*5+4], r_idl_aie[8*5+4], r_lat_aie[8*5+4], dout_done_aie[8*5+4]);
tst_dout_mon #("fft_gold_13b.mem", 8, 16, 65536) DOUT13b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+5], d_axi_tdat[8*5+5][7:0]^d_axi_tdat[8*5+5][15:8], r_vld_aie[8*5+5], r_smp_aie[8*5+5], r_err_aie[8*5+5], r_idl_aie[8*5+5], r_lat_aie[8*5+5], dout_done_aie[8*5+5]);
tst_dout_mon #("fft_gold_14b.mem", 8, 16, 65536) DOUT14b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+6], d_axi_tdat[8*5+6][7:0]^d_axi_tdat[8*5+6][15:8], r_vld_aie[8*5+6], r_smp_aie[8*5+6], r_err_aie[8*5+6], r_idl_aie[8*5+6], r_lat_aie[8*5+6], dout_done_aie[8*5+6]);
tst_dout_mon #("fft_gold_15b.mem", 8, 16, 65536) DOUT15b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*5+7], d_axi_tdat[8*5+7][7:0]^d_axi_tdat[8*5+7][15:8], r_vld_aie[8*5+7], r_smp_aie[8*5+7], r_err_aie[8*5+7], r_idl_aie[8*5+7], r_lat_aie[8*5+7], dout_done_aie[8*5+7]);
tst_dout_mon #("fft_gold_16b.mem", 8, 16, 65536) DOUT16b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+0], d_axi_tdat[8*6+0][7:0]^d_axi_tdat[8*6+0][15:8], r_vld_aie[8*6+0], r_smp_aie[8*6+0], r_err_aie[8*6+0], r_idl_aie[8*6+0], r_lat_aie[8*6+0], dout_done_aie[8*6+0]);
tst_dout_mon #("fft_gold_17b.mem", 8, 16, 65536) DOUT17b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+1], d_axi_tdat[8*6+1][7:0]^d_axi_tdat[8*6+1][15:8], r_vld_aie[8*6+1], r_smp_aie[8*6+1], r_err_aie[8*6+1], r_idl_aie[8*6+1], r_lat_aie[8*6+1], dout_done_aie[8*6+1]);
tst_dout_mon #("fft_gold_18b.mem", 8, 16, 65536) DOUT18b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+2], d_axi_tdat[8*6+2][7:0]^d_axi_tdat[8*6+2][15:8], r_vld_aie[8*6+2], r_smp_aie[8*6+2], r_err_aie[8*6+2], r_idl_aie[8*6+2], r_lat_aie[8*6+2], dout_done_aie[8*6+2]);
tst_dout_mon #("fft_gold_19b.mem", 8, 16, 65536) DOUT19b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+3], d_axi_tdat[8*6+3][7:0]^d_axi_tdat[8*6+3][15:8], r_vld_aie[8*6+3], r_smp_aie[8*6+3], r_err_aie[8*6+3], r_idl_aie[8*6+3], r_lat_aie[8*6+3], dout_done_aie[8*6+3]);
tst_dout_mon #("fft_gold_20b.mem", 8, 16, 65536) DOUT20b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+4], d_axi_tdat[8*6+4][7:0]^d_axi_tdat[8*6+4][15:8], r_vld_aie[8*6+4], r_smp_aie[8*6+4], r_err_aie[8*6+4], r_idl_aie[8*6+4], r_lat_aie[8*6+4], dout_done_aie[8*6+4]);
tst_dout_mon #("fft_gold_21b.mem", 8, 16, 65536) DOUT21b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+5], d_axi_tdat[8*6+5][7:0]^d_axi_tdat[8*6+5][15:8], r_vld_aie[8*6+5], r_smp_aie[8*6+5], r_err_aie[8*6+5], r_idl_aie[8*6+5], r_lat_aie[8*6+5], dout_done_aie[8*6+5]);
tst_dout_mon #("fft_gold_22b.mem", 8, 16, 65536) DOUT22b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+6], d_axi_tdat[8*6+6][7:0]^d_axi_tdat[8*6+6][15:8], r_vld_aie[8*6+6], r_smp_aie[8*6+6], r_err_aie[8*6+6], r_idl_aie[8*6+6], r_lat_aie[8*6+6], dout_done_aie[8*6+6]);
tst_dout_mon #("fft_gold_23b.mem", 8, 16, 65536) DOUT23b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*6+7], d_axi_tdat[8*6+7][7:0]^d_axi_tdat[8*6+7][15:8], r_vld_aie[8*6+7], r_smp_aie[8*6+7], r_err_aie[8*6+7], r_idl_aie[8*6+7], r_lat_aie[8*6+7], dout_done_aie[8*6+7]);
tst_dout_mon #("fft_gold_24b.mem", 8, 16, 65536) DOUT24b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+0], d_axi_tdat[8*7+0][7:0]^d_axi_tdat[8*7+0][15:8], r_vld_aie[8*7+0], r_smp_aie[8*7+0], r_err_aie[8*7+0], r_idl_aie[8*7+0], r_lat_aie[8*7+0], dout_done_aie[8*7+0]);
tst_dout_mon #("fft_gold_25b.mem", 8, 16, 65536) DOUT25b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+1], d_axi_tdat[8*7+1][7:0]^d_axi_tdat[8*7+1][15:8], r_vld_aie[8*7+1], r_smp_aie[8*7+1], r_err_aie[8*7+1], r_idl_aie[8*7+1], r_lat_aie[8*7+1], dout_done_aie[8*7+1]);
tst_dout_mon #("fft_gold_26b.mem", 8, 16, 65536) DOUT26b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+2], d_axi_tdat[8*7+2][7:0]^d_axi_tdat[8*7+2][15:8], r_vld_aie[8*7+2], r_smp_aie[8*7+2], r_err_aie[8*7+2], r_idl_aie[8*7+2], r_lat_aie[8*7+2], dout_done_aie[8*7+2]);
tst_dout_mon #("fft_gold_27b.mem", 8, 16, 65536) DOUT27b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+3], d_axi_tdat[8*7+3][7:0]^d_axi_tdat[8*7+3][15:8], r_vld_aie[8*7+3], r_smp_aie[8*7+3], r_err_aie[8*7+3], r_idl_aie[8*7+3], r_lat_aie[8*7+3], dout_done_aie[8*7+3]);
tst_dout_mon #("fft_gold_28b.mem", 8, 16, 65536) DOUT28b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+4], d_axi_tdat[8*7+4][7:0]^d_axi_tdat[8*7+4][15:8], r_vld_aie[8*7+4], r_smp_aie[8*7+4], r_err_aie[8*7+4], r_idl_aie[8*7+4], r_lat_aie[8*7+4], dout_done_aie[8*7+4]);
tst_dout_mon #("fft_gold_29b.mem", 8, 16, 65536) DOUT29b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+5], d_axi_tdat[8*7+5][7:0]^d_axi_tdat[8*7+5][15:8], r_vld_aie[8*7+5], r_smp_aie[8*7+5], r_err_aie[8*7+5], r_idl_aie[8*7+5], r_lat_aie[8*7+5], dout_done_aie[8*7+5]);
tst_dout_mon #("fft_gold_30b.mem", 8, 16, 65536) DOUT30b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+6], d_axi_tdat[8*7+6][7:0]^d_axi_tdat[8*7+6][15:8], r_vld_aie[8*7+6], r_smp_aie[8*7+6], r_err_aie[8*7+6], r_idl_aie[8*7+6], r_lat_aie[8*7+6], dout_done_aie[8*7+6]);
tst_dout_mon #("fft_gold_31b.mem", 8, 16, 65536) DOUT31b(dat_clk, srst_2, dout_rst, d_axi_tvld[8*7+7], d_axi_tdat[8*7+7][7:0]^d_axi_tdat[8*7+7][15:8], r_vld_aie[8*7+7], r_smp_aie[8*7+7], r_err_aie[8*7+7], r_idl_aie[8*7+7], r_lat_aie[8*7+7], dout_done_aie[8*7+7]);




//-----------------------------------------
// Generate Sync Reset for AIE clock domain
//-----------------------------------------
xpm_cdc_async_rst #(
 .DEST_SYNC_FF(4), // DECIMAL; range: 2-10
 .INIT_SYNC_FF(0), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
 .RST_ACTIVE_HIGH(1) // DECIMAL; 0=active low reset, 1=active high reset
)
xpm_cdc_async_rst_inst (
 .dest_arst(srst_2), // 1-bit output: src_arst asynchronous reset signal synchronized to destination
 // clock domain. This output is registered. NOTE: Signal asserts asynchronously
 // but deasserts synchronously to dest_clk. Width of the reset signal is at least
 // (DEST_SYNC_FF*dest_clk) period.
 .dest_clk(dat_clk), // 1-bit input: Destination clock.
 .src_arst(srst_xclk) // 1-bit input: Source asynchronous reset signal.
);
//------------------------------------------------------


//---------------------------------------------
// Manipulate reset in REG clk domain
//---------------------------------------------
always@(posedge reg_clk)
begin
	srst_p <= ~reg_resetn;
	srst   <= srst_p;
	rstcnt    <= srst? {1'b1, 7'd0} : srst_xclk? rstcnt+1 : rstcnt;
	srst_xclk <= srst? 1'b1 : rstcnt[7];	
end
//---------------------------------------------


//---------------------------------------------
// Generate a pulse to reset monitor counters
//---------------------------------------------
always@(posedge dat_clk)
begin

	vld_all <= test_en_aie;
	vld_all_d <= vld_all;
	dout_rst <=(~vld_all_d) & vld_all;
	
end
//---------------------------------------------

always@(posedge reg_clk) test_en_reg <= tst_axi_tvld;  



endmodule
