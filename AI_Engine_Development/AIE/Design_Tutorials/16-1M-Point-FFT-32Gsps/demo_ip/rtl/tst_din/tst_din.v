//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

`timescale 1ns / 1ps

module tst_din #(
  parameter integer C_S_AXI_CONTROL_ADDR_WIDTH = 12,
  parameter integer C_S_AXI_CONTROL_DATA_WIDTH = 32
)
(
  // System Signals
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 regclk CLK" *)
  (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF endout:s_axi_control, ASSOCIATED_RESET reg_resetn" *)
  input                                      reg_clk,
  input                                      reg_resetn,
  // AXI4-Lite slave interface
  input                                     s_axi_control_awvalid,
  output                                    s_axi_control_awready,
  input  [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]   s_axi_control_awaddr ,
  input                                     s_axi_control_wvalid ,
  output                                    s_axi_control_wready ,
  input  [C_S_AXI_CONTROL_DATA_WIDTH-1:0]   s_axi_control_wdata  ,
  input  [C_S_AXI_CONTROL_DATA_WIDTH/8-1:0] s_axi_control_wstrb  ,
  input                                     s_axi_control_arvalid,
  output                                    s_axi_control_arready,
  input  [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]   s_axi_control_araddr ,
  output                                    s_axi_control_rvalid ,
  input                                     s_axi_control_rready ,
  output [C_S_AXI_CONTROL_DATA_WIDTH-1:0]   s_axi_control_rdata  ,
  output [2-1:0]                            s_axi_control_rresp  ,
  output                                    s_axi_control_bvalid ,
  input                                     s_axi_control_bready ,
  output [2-1:0]                            s_axi_control_bresp  ,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 endout TVALID" *)   output  reg  douten_vld,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 endout TDATA" *)    output [7:0] douten_dat, 
 /////////////////////////////////////////////////////////////////////
 /////////  DUT Interfaces
 /////////////////////////////////////////////////////////////////////
 (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 datclk CLK" *)
 (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF d00a:d01a:d02a:d03a:d04a:d05a:d06a:d07a:d08a:d09a:d10a:d11a:d12a:d13a:d14a:d15a:d16a:d17a:d18a:d19a:d20a:d21a:d22a:d23a:d24a:d25a:d26a:d27a:d28a:d29a:d30a:d31a:d00b:d01b:d02b:d03b:d04b:d05b:d06b:d07b:d08b:d09b:d10b:d11b:d12b:d13b:d14b:d15b:d16b:d17b:d18b:d19b:d20b:d21b:d22b:d23b:d24b:d25b:d26b:d27b:d28b:d29b:d30b:d31b" *)
 input         dat_clk,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00a  TREADY" *)    input        d00a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00a  TVALID" *)   output        d00a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00a  TDATA" *)    output [63:0] d00a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01a  TREADY" *)    input        d01a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01a  TVALID" *)   output        d01a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01a  TDATA" *)    output [63:0] d01a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02a  TREADY" *)    input        d02a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02a  TVALID" *)   output        d02a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02a  TDATA" *)    output [63:0] d02a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03a  TREADY" *)    input        d03a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03a  TVALID" *)   output        d03a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03a  TDATA" *)    output [63:0] d03a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04a  TREADY" *)    input        d04a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04a  TVALID" *)   output        d04a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04a  TDATA" *)    output [63:0] d04a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05a  TREADY" *)    input        d05a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05a  TVALID" *)   output        d05a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05a  TDATA" *)    output [63:0] d05a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06a  TREADY" *)    input        d06a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06a  TVALID" *)   output        d06a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06a  TDATA" *)    output [63:0] d06a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07a  TREADY" *)    input        d07a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07a  TVALID" *)   output        d07a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07a  TDATA" *)    output [63:0] d07a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08a  TREADY" *)    input        d08a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08a  TVALID" *)   output        d08a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08a  TDATA" *)    output [63:0] d08a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09a  TREADY" *)    input        d09a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09a  TVALID" *)   output        d09a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09a  TDATA" *)    output [63:0] d09a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10a  TREADY" *)    input        d10a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10a  TVALID" *)   output        d10a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10a  TDATA" *)    output [63:0] d10a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11a  TREADY" *)    input        d11a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11a  TVALID" *)   output        d11a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11a  TDATA" *)    output [63:0] d11a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12a  TREADY" *)    input        d12a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12a  TVALID" *)   output        d12a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12a  TDATA" *)    output [63:0] d12a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13a  TREADY" *)    input        d13a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13a  TVALID" *)   output        d13a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13a  TDATA" *)    output [63:0] d13a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14a  TREADY" *)    input        d14a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14a  TVALID" *)   output        d14a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14a  TDATA" *)    output [63:0] d14a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15a  TREADY" *)    input        d15a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15a  TVALID" *)   output        d15a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15a  TDATA" *)    output [63:0] d15a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16a  TREADY" *)    input        d16a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16a  TVALID" *)   output        d16a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16a  TDATA" *)    output [63:0] d16a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17a  TREADY" *)    input        d17a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17a  TVALID" *)   output        d17a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17a  TDATA" *)    output [63:0] d17a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18a  TREADY" *)    input        d18a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18a  TVALID" *)   output        d18a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18a  TDATA" *)    output [63:0] d18a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19a  TREADY" *)    input        d19a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19a  TVALID" *)   output        d19a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19a  TDATA" *)    output [63:0] d19a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20a  TREADY" *)    input        d20a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20a  TVALID" *)   output        d20a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20a  TDATA" *)    output [63:0] d20a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21a  TREADY" *)    input        d21a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21a  TVALID" *)   output        d21a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21a  TDATA" *)    output [63:0] d21a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22a  TREADY" *)    input        d22a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22a  TVALID" *)   output        d22a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22a  TDATA" *)    output [63:0] d22a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23a  TREADY" *)    input        d23a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23a  TVALID" *)   output        d23a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23a  TDATA" *)    output [63:0] d23a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24a  TREADY" *)    input        d24a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24a  TVALID" *)   output        d24a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24a  TDATA" *)    output [63:0] d24a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25a  TREADY" *)    input        d25a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25a  TVALID" *)   output        d25a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25a  TDATA" *)    output [63:0] d25a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26a  TREADY" *)    input        d26a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26a  TVALID" *)   output        d26a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26a  TDATA" *)    output [63:0] d26a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27a  TREADY" *)    input        d27a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27a  TVALID" *)   output        d27a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27a  TDATA" *)    output [63:0] d27a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28a  TREADY" *)    input        d28a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28a  TVALID" *)   output        d28a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28a  TDATA" *)    output [63:0] d28a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29a  TREADY" *)    input        d29a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29a  TVALID" *)   output        d29a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29a  TDATA" *)    output [63:0] d29a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30a  TREADY" *)    input        d30a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30a  TVALID" *)   output        d30a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30a  TDATA" *)    output [63:0] d30a_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31a  TREADY" *)    input        d31a_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31a  TVALID" *)   output        d31a_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31a  TDATA" *)    output [63:0] d31a_wd, 
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00b  TREADY" *)    input        d00b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00b  TVALID" *)   output        d00b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d00b  TDATA" *)    output [63:0] d00b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01b  TREADY" *)    input        d01b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01b  TVALID" *)   output        d01b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d01b  TDATA" *)    output [63:0] d01b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02b  TREADY" *)    input        d02b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02b  TVALID" *)   output        d02b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d02b  TDATA" *)    output [63:0] d02b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03b  TREADY" *)    input        d03b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03b  TVALID" *)   output        d03b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d03b  TDATA" *)    output [63:0] d03b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04b  TREADY" *)    input        d04b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04b  TVALID" *)   output        d04b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d04b  TDATA" *)    output [63:0] d04b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05b  TREADY" *)    input        d05b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05b  TVALID" *)   output        d05b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d05b  TDATA" *)    output [63:0] d05b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06b  TREADY" *)    input        d06b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06b  TVALID" *)   output        d06b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d06b  TDATA" *)    output [63:0] d06b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07b  TREADY" *)    input        d07b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07b  TVALID" *)   output        d07b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d07b  TDATA" *)    output [63:0] d07b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08b  TREADY" *)    input        d08b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08b  TVALID" *)   output        d08b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d08b  TDATA" *)    output [63:0] d08b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09b  TREADY" *)    input        d09b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09b  TVALID" *)   output        d09b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d09b  TDATA" *)    output [63:0] d09b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10b  TREADY" *)    input        d10b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10b  TVALID" *)   output        d10b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d10b  TDATA" *)    output [63:0] d10b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11b  TREADY" *)    input        d11b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11b  TVALID" *)   output        d11b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d11b  TDATA" *)    output [63:0] d11b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12b  TREADY" *)    input        d12b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12b  TVALID" *)   output        d12b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d12b  TDATA" *)    output [63:0] d12b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13b  TREADY" *)    input        d13b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13b  TVALID" *)   output        d13b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d13b  TDATA" *)    output [63:0] d13b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14b  TREADY" *)    input        d14b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14b  TVALID" *)   output        d14b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d14b  TDATA" *)    output [63:0] d14b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15b  TREADY" *)    input        d15b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15b  TVALID" *)   output        d15b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d15b  TDATA" *)    output [63:0] d15b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16b  TREADY" *)    input        d16b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16b  TVALID" *)   output        d16b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d16b  TDATA" *)    output [63:0] d16b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17b  TREADY" *)    input        d17b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17b  TVALID" *)   output        d17b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d17b  TDATA" *)    output [63:0] d17b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18b  TREADY" *)    input        d18b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18b  TVALID" *)   output        d18b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d18b  TDATA" *)    output [63:0] d18b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19b  TREADY" *)    input        d19b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19b  TVALID" *)   output        d19b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d19b  TDATA" *)    output [63:0] d19b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20b  TREADY" *)    input        d20b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20b  TVALID" *)   output        d20b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d20b  TDATA" *)    output [63:0] d20b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21b  TREADY" *)    input        d21b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21b  TVALID" *)   output        d21b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d21b  TDATA" *)    output [63:0] d21b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22b  TREADY" *)    input        d22b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22b  TVALID" *)   output        d22b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d22b  TDATA" *)    output [63:0] d22b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23b  TREADY" *)    input        d23b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23b  TVALID" *)   output        d23b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d23b  TDATA" *)    output [63:0] d23b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24b  TREADY" *)    input        d24b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24b  TVALID" *)   output        d24b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d24b  TDATA" *)    output [63:0] d24b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25b  TREADY" *)    input        d25b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25b  TVALID" *)   output        d25b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d25b  TDATA" *)    output [63:0] d25b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26b  TREADY" *)    input        d26b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26b  TVALID" *)   output        d26b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d26b  TDATA" *)    output [63:0] d26b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27b  TREADY" *)    input        d27b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27b  TVALID" *)   output        d27b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d27b  TDATA" *)    output [63:0] d27b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28b  TREADY" *)    input        d28b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28b  TVALID" *)   output        d28b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d28b  TDATA" *)    output [63:0] d28b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29b  TREADY" *)    input        d29b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29b  TVALID" *)   output        d29b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d29b  TDATA" *)    output [63:0] d29b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30b  TREADY" *)    input        d30b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30b  TVALID" *)   output        d30b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d30b  TDATA" *)    output [63:0] d30b_wd,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31b  TREADY" *)    input        d31b_af,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31b  TVALID" *)   output        d31b_we,
 (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 d31b  TDATA" *)    output [63:0] d31b_wd
);


//--------------------------------------------------
// Register Signals
//--------------------------------------------------
wire test_en_reg;
wire test_en_aie;

wire [31:0] itecnt_aie;
wire [31:0] itecnt_reg;

wire test_start_aie;
wire [63:0] test_done;

//------------------------------------
// Reset Signals
//------------------------------------
reg  srst, srst_p, srst_xclk;
wire srst_2;
reg [7:0] rstcnt;


//----------------------------------------------------------
// Register Map and System Controls
//----------------------------------------------------------
tst_din_regs #(C_S_AXI_CONTROL_ADDR_WIDTH, C_S_AXI_CONTROL_DATA_WIDTH) REGS(
    reg_clk, srst,
    s_axi_control_awaddr, s_axi_control_awvalid, s_axi_control_awready, 
	s_axi_control_wdata, s_axi_control_wstrb, s_axi_control_wvalid, s_axi_control_wready,
    s_axi_control_bresp,  s_axi_control_bvalid,  s_axi_control_bready,
    s_axi_control_araddr, s_axi_control_arvalid, s_axi_control_arready, 
	s_axi_control_rdata, s_axi_control_rresp, s_axi_control_rvalid, s_axi_control_rready,
	//-------------------------------------------------
	// Standard Control Interface
	//-------------------------------------------------
    test_en_reg, itecnt_reg
);


//-----------------------------------------------
// Kernel <-> Reg Cross Clock Domain
//-----------------------------------------------
tst_din_xclk XCK
(
	//--------------------------------
	// register clock domain
	//--------------------------------
	reg_clk, test_en_reg, itecnt_reg,
	//----------------------------------
	// data clock domain
	//----------------------------------	
	dat_clk, test_en_aie, itecnt_aie
);


//-----------------------------------------------
// Stimulus Driver
//-----------------------------------------------
tst_din_ctrl CTRL0(dat_clk, srst_2, test_en_aie, itecnt_aie, test_start_aie, &test_done);
                                                                                                         
tst_din_dgen #(64'h56789f0156789f00) D00a(dat_clk,srst_2,test_start_aie, test_done[ 0],  d00a_af,d00a_we,d00a_wd);
tst_din_dgen #(64'h56789f0556789f04) D01a(dat_clk,srst_2,test_start_aie, test_done[ 1],  d01a_af,d01a_we,d01a_wd);
tst_din_dgen #(64'h56789f0956789f08) D02a(dat_clk,srst_2,test_start_aie, test_done[ 2],  d02a_af,d02a_we,d02a_wd);
tst_din_dgen #(64'h56789f0d56789f0c) D03a(dat_clk,srst_2,test_start_aie, test_done[ 3],  d03a_af,d03a_we,d03a_wd);
tst_din_dgen #(64'h56789f1156789f10) D04a(dat_clk,srst_2,test_start_aie, test_done[ 4],  d04a_af,d04a_we,d04a_wd);
tst_din_dgen #(64'h56789f1556789f14) D05a(dat_clk,srst_2,test_start_aie, test_done[ 5],  d05a_af,d05a_we,d05a_wd);
tst_din_dgen #(64'h56789f1956789f18) D06a(dat_clk,srst_2,test_start_aie, test_done[ 6],  d06a_af,d06a_we,d06a_wd);
tst_din_dgen #(64'h56789f1d56789f1c) D07a(dat_clk,srst_2,test_start_aie, test_done[ 7],  d07a_af,d07a_we,d07a_wd);
tst_din_dgen #(64'h56789f2156789f20) D08a(dat_clk,srst_2,test_start_aie, test_done[ 8],  d08a_af,d08a_we,d08a_wd);
tst_din_dgen #(64'h56789f2556789f24) D09a(dat_clk,srst_2,test_start_aie, test_done[ 9],  d09a_af,d09a_we,d09a_wd);
tst_din_dgen #(64'h56789f2956789f28) D10a(dat_clk,srst_2,test_start_aie, test_done[10],  d10a_af,d10a_we,d10a_wd);
tst_din_dgen #(64'h56789f2d56789f2c) D11a(dat_clk,srst_2,test_start_aie, test_done[11],  d11a_af,d11a_we,d11a_wd);
tst_din_dgen #(64'h56789f3156789f30) D12a(dat_clk,srst_2,test_start_aie, test_done[12],  d12a_af,d12a_we,d12a_wd);
tst_din_dgen #(64'h56789f3556789f34) D13a(dat_clk,srst_2,test_start_aie, test_done[13],  d13a_af,d13a_we,d13a_wd);
tst_din_dgen #(64'h56789f3956789f38) D14a(dat_clk,srst_2,test_start_aie, test_done[14],  d14a_af,d14a_we,d14a_wd);
tst_din_dgen #(64'h56789f3d56789f3c) D15a(dat_clk,srst_2,test_start_aie, test_done[15],  d15a_af,d15a_we,d15a_wd);
tst_din_dgen #(64'h56789f4156789f40) D16a(dat_clk,srst_2,test_start_aie, test_done[16],  d16a_af,d16a_we,d16a_wd);                                                                                      
tst_din_dgen #(64'h56789f4556789f44) D17a(dat_clk,srst_2,test_start_aie, test_done[17],  d17a_af,d17a_we,d17a_wd);
tst_din_dgen #(64'h56789f4956789f48) D18a(dat_clk,srst_2,test_start_aie, test_done[18],  d18a_af,d18a_we,d18a_wd);
tst_din_dgen #(64'h56789f4d56789f4c) D19a(dat_clk,srst_2,test_start_aie, test_done[19],  d19a_af,d19a_we,d19a_wd);
tst_din_dgen #(64'h56789f5156789f50) D20a(dat_clk,srst_2,test_start_aie, test_done[20],  d20a_af,d20a_we,d20a_wd);
tst_din_dgen #(64'h56789f5556789f54) D21a(dat_clk,srst_2,test_start_aie, test_done[21],  d21a_af,d21a_we,d21a_wd);
tst_din_dgen #(64'h56789f5956789f58) D22a(dat_clk,srst_2,test_start_aie, test_done[22],  d22a_af,d22a_we,d22a_wd);
tst_din_dgen #(64'h56789f5d56789f5c) D23a(dat_clk,srst_2,test_start_aie, test_done[23],  d23a_af,d23a_we,d23a_wd);
tst_din_dgen #(64'h56789f6156789f60) D24a(dat_clk,srst_2,test_start_aie, test_done[24],  d24a_af,d24a_we,d24a_wd);
tst_din_dgen #(64'h56789f6556789f64) D25a(dat_clk,srst_2,test_start_aie, test_done[25],  d25a_af,d25a_we,d25a_wd);
tst_din_dgen #(64'h56789f6956789f68) D26a(dat_clk,srst_2,test_start_aie, test_done[26],  d26a_af,d26a_we,d26a_wd);
tst_din_dgen #(64'h56789f6d56789f6c) D27a(dat_clk,srst_2,test_start_aie, test_done[27],  d27a_af,d27a_we,d27a_wd);
tst_din_dgen #(64'h56789f7156789f70) D28a(dat_clk,srst_2,test_start_aie, test_done[28],  d28a_af,d28a_we,d28a_wd);
tst_din_dgen #(64'h56789f7556789f74) D29a(dat_clk,srst_2,test_start_aie, test_done[29],  d29a_af,d29a_we,d29a_wd);
tst_din_dgen #(64'h56789f7956789f78) D30a(dat_clk,srst_2,test_start_aie, test_done[30],  d30a_af,d30a_we,d30a_wd);
tst_din_dgen #(64'h56789f7d56789f7c) D31a(dat_clk,srst_2,test_start_aie, test_done[31],  d31a_af,d31a_we,d31a_wd);

tst_din_dgen #(64'h56789f0356789f02) D00b(dat_clk,srst_2,test_start_aie, test_done[32+ 0],  d00b_af,d00b_we,d00b_wd);
tst_din_dgen #(64'h56789f0756789f06) D01b(dat_clk,srst_2,test_start_aie, test_done[32+ 1],  d01b_af,d01b_we,d01b_wd);
tst_din_dgen #(64'h56789f0b56789f0a) D02b(dat_clk,srst_2,test_start_aie, test_done[32+ 2],  d02b_af,d02b_we,d02b_wd);
tst_din_dgen #(64'h56789f0f56789f0e) D03b(dat_clk,srst_2,test_start_aie, test_done[32+ 3],  d03b_af,d03b_we,d03b_wd);
tst_din_dgen #(64'h56789f1356789f12) D04b(dat_clk,srst_2,test_start_aie, test_done[32+ 4],  d04b_af,d04b_we,d04b_wd);
tst_din_dgen #(64'h56789f1756789f16) D05b(dat_clk,srst_2,test_start_aie, test_done[32+ 5],  d05b_af,d05b_we,d05b_wd);
tst_din_dgen #(64'h56789f1b56789f1a) D06b(dat_clk,srst_2,test_start_aie, test_done[32+ 6],  d06b_af,d06b_we,d06b_wd);
tst_din_dgen #(64'h56789f1f56789f1e) D07b(dat_clk,srst_2,test_start_aie, test_done[32+ 7],  d07b_af,d07b_we,d07b_wd);
tst_din_dgen #(64'h56789f2356789f22) D08b(dat_clk,srst_2,test_start_aie, test_done[32+ 8],  d08b_af,d08b_we,d08b_wd);
tst_din_dgen #(64'h56789f2756789f26) D09b(dat_clk,srst_2,test_start_aie, test_done[32+ 9],  d09b_af,d09b_we,d09b_wd);
tst_din_dgen #(64'h56789f2b56789f2a) D10b(dat_clk,srst_2,test_start_aie, test_done[32+10],  d10b_af,d10b_we,d10b_wd);
tst_din_dgen #(64'h56789f2f56789f2e) D11b(dat_clk,srst_2,test_start_aie, test_done[32+11],  d11b_af,d11b_we,d11b_wd);
tst_din_dgen #(64'h56789f3356789f32) D12b(dat_clk,srst_2,test_start_aie, test_done[32+12],  d12b_af,d12b_we,d12b_wd);
tst_din_dgen #(64'h56789f3756789f36) D13b(dat_clk,srst_2,test_start_aie, test_done[32+13],  d13b_af,d13b_we,d13b_wd);
tst_din_dgen #(64'h56789f3b56789f3a) D14b(dat_clk,srst_2,test_start_aie, test_done[32+14],  d14b_af,d14b_we,d14b_wd);
tst_din_dgen #(64'h56789f3f56789f3e) D15b(dat_clk,srst_2,test_start_aie, test_done[32+15],  d15b_af,d15b_we,d15b_wd);
tst_din_dgen #(64'h56789f4356789f42) D16b(dat_clk,srst_2,test_start_aie, test_done[32+16],  d16b_af,d16b_we,d16b_wd);                                                                                      
tst_din_dgen #(64'h56789f4756789f46) D17b(dat_clk,srst_2,test_start_aie, test_done[32+17],  d17b_af,d17b_we,d17b_wd);
tst_din_dgen #(64'h56789f4b56789f4a) D18b(dat_clk,srst_2,test_start_aie, test_done[32+18],  d18b_af,d18b_we,d18b_wd);
tst_din_dgen #(64'h56789f4f56789f4e) D19b(dat_clk,srst_2,test_start_aie, test_done[32+19],  d19b_af,d19b_we,d19b_wd);
tst_din_dgen #(64'h56789f5356789f52) D20b(dat_clk,srst_2,test_start_aie, test_done[32+20],  d20b_af,d20b_we,d20b_wd);
tst_din_dgen #(64'h56789f5756789f56) D21b(dat_clk,srst_2,test_start_aie, test_done[32+21],  d21b_af,d21b_we,d21b_wd);
tst_din_dgen #(64'h56789f5b56789f5a) D22b(dat_clk,srst_2,test_start_aie, test_done[32+22],  d22b_af,d22b_we,d22b_wd);
tst_din_dgen #(64'h56789f5f56789f5e) D23b(dat_clk,srst_2,test_start_aie, test_done[32+23],  d23b_af,d23b_we,d23b_wd);
tst_din_dgen #(64'h56789f6356789f62) D24b(dat_clk,srst_2,test_start_aie, test_done[32+24],  d24b_af,d24b_we,d24b_wd);
tst_din_dgen #(64'h56789f6756789f66) D25b(dat_clk,srst_2,test_start_aie, test_done[32+25],  d25b_af,d25b_we,d25b_wd);
tst_din_dgen #(64'h56789f6b56789f6a) D26b(dat_clk,srst_2,test_start_aie, test_done[32+26],  d26b_af,d26b_we,d26b_wd);
tst_din_dgen #(64'h56789f6f56789f6e) D27b(dat_clk,srst_2,test_start_aie, test_done[32+27],  d27b_af,d27b_we,d27b_wd);
tst_din_dgen #(64'h56789f7356789f72) D28b(dat_clk,srst_2,test_start_aie, test_done[32+28],  d28b_af,d28b_we,d28b_wd);
tst_din_dgen #(64'h56789f7756789f76) D29b(dat_clk,srst_2,test_start_aie, test_done[32+29],  d29b_af,d29b_we,d29b_wd);
tst_din_dgen #(64'h56789f7b56789f7a) D30b(dat_clk,srst_2,test_start_aie, test_done[32+30],  d30b_af,d30b_we,d30b_wd);
tst_din_dgen #(64'h56789f7f56789f7e) D31b(dat_clk,srst_2,test_start_aie, test_done[32+31],  d31b_af,d31b_we,d31b_wd);


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

	douten_vld <= test_en_reg;
	
end
//---------------------------------------------
assign douten_dat = 0;



endmodule
