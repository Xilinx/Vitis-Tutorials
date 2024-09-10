/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: AMD
// Engineer: Florent Werbrouck
// 
// Create Date: 07/01/2024 02:47:43 PM
// Design Name: 
// Module Name: top_level
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top_level
   (ch0_lpddr4_c0_ca_a,
    ch0_lpddr4_c0_ca_b,
    ch0_lpddr4_c0_ck_c_a,
    ch0_lpddr4_c0_ck_c_b,
    ch0_lpddr4_c0_ck_t_a,
    ch0_lpddr4_c0_ck_t_b,
    ch0_lpddr4_c0_cke_a,
    ch0_lpddr4_c0_cke_b,
    ch0_lpddr4_c0_cs_a,
    ch0_lpddr4_c0_cs_b,
    ch0_lpddr4_c0_dmi_a,
    ch0_lpddr4_c0_dmi_b,
    ch0_lpddr4_c0_dq_a,
    ch0_lpddr4_c0_dq_b,
    ch0_lpddr4_c0_dqs_c_a,
    ch0_lpddr4_c0_dqs_c_b,
    ch0_lpddr4_c0_dqs_t_a,
    ch0_lpddr4_c0_dqs_t_b,
    ch0_lpddr4_c0_reset_n,
    ch0_lpddr4_c1_ca_a,
    ch0_lpddr4_c1_ca_b,
    ch0_lpddr4_c1_ck_c_a,
    ch0_lpddr4_c1_ck_c_b,
    ch0_lpddr4_c1_ck_t_a,
    ch0_lpddr4_c1_ck_t_b,
    ch0_lpddr4_c1_cke_a,
    ch0_lpddr4_c1_cke_b,
    ch0_lpddr4_c1_cs_a,
    ch0_lpddr4_c1_cs_b,
    ch0_lpddr4_c1_dmi_a,
    ch0_lpddr4_c1_dmi_b,
    ch0_lpddr4_c1_dq_a,
    ch0_lpddr4_c1_dq_b,
    ch0_lpddr4_c1_dqs_c_a,
    ch0_lpddr4_c1_dqs_c_b,
    ch0_lpddr4_c1_dqs_t_a,
    ch0_lpddr4_c1_dqs_t_b,
    ch0_lpddr4_c1_reset_n,
    ch1_lpddr4_c0_ca_a,
    ch1_lpddr4_c0_ca_b,
    ch1_lpddr4_c0_ck_c_a,
    ch1_lpddr4_c0_ck_c_b,
    ch1_lpddr4_c0_ck_t_a,
    ch1_lpddr4_c0_ck_t_b,
    ch1_lpddr4_c0_cke_a,
    ch1_lpddr4_c0_cke_b,
    ch1_lpddr4_c0_cs_a,
    ch1_lpddr4_c0_cs_b,
    ch1_lpddr4_c0_dmi_a,
    ch1_lpddr4_c0_dmi_b,
    ch1_lpddr4_c0_dq_a,
    ch1_lpddr4_c0_dq_b,
    ch1_lpddr4_c0_dqs_c_a,
    ch1_lpddr4_c0_dqs_c_b,
    ch1_lpddr4_c0_dqs_t_a,
    ch1_lpddr4_c0_dqs_t_b,
    ch1_lpddr4_c0_reset_n,
    ch1_lpddr4_c1_ca_a,
    ch1_lpddr4_c1_ca_b,
    ch1_lpddr4_c1_ck_c_a,
    ch1_lpddr4_c1_ck_c_b,
    ch1_lpddr4_c1_ck_t_a,
    ch1_lpddr4_c1_ck_t_b,
    ch1_lpddr4_c1_cke_a,
    ch1_lpddr4_c1_cke_b,
    ch1_lpddr4_c1_cs_a,
    ch1_lpddr4_c1_cs_b,
    ch1_lpddr4_c1_dmi_a,
    ch1_lpddr4_c1_dmi_b,
    ch1_lpddr4_c1_dq_a,
    ch1_lpddr4_c1_dq_b,
    ch1_lpddr4_c1_dqs_c_a,
    ch1_lpddr4_c1_dqs_c_b,
    ch1_lpddr4_c1_dqs_t_a,
    ch1_lpddr4_c1_dqs_t_b,
    ch1_lpddr4_c1_reset_n,
    ddr4_dimm1_act_n,
    ddr4_dimm1_adr,
    ddr4_dimm1_ba,
    ddr4_dimm1_bg,
    ddr4_dimm1_ck_c,
    ddr4_dimm1_ck_t,
    ddr4_dimm1_cke,
    ddr4_dimm1_cs_n,
    ddr4_dimm1_dm_n,
    ddr4_dimm1_dq,
    ddr4_dimm1_dqs_c,
    ddr4_dimm1_dqs_t,
    ddr4_dimm1_odt,
    ddr4_dimm1_reset_n,
    ddr4_dimm1_sma_clk_clk_n,
    ddr4_dimm1_sma_clk_clk_p,
    lpddr4_sma_clk1_clk_n,
    lpddr4_sma_clk1_clk_p,
    lpddr4_sma_clk2_clk_n,
    lpddr4_sma_clk2_clk_p);
  output [5:0]ch0_lpddr4_c0_ca_a;
  output [5:0]ch0_lpddr4_c0_ca_b;
  output ch0_lpddr4_c0_ck_c_a;
  output ch0_lpddr4_c0_ck_c_b;
  output ch0_lpddr4_c0_ck_t_a;
  output ch0_lpddr4_c0_ck_t_b;
  output ch0_lpddr4_c0_cke_a;
  output ch0_lpddr4_c0_cke_b;
  output ch0_lpddr4_c0_cs_a;
  output ch0_lpddr4_c0_cs_b;
  inout [1:0]ch0_lpddr4_c0_dmi_a;
  inout [1:0]ch0_lpddr4_c0_dmi_b;
  inout [15:0]ch0_lpddr4_c0_dq_a;
  inout [15:0]ch0_lpddr4_c0_dq_b;
  inout [1:0]ch0_lpddr4_c0_dqs_c_a;
  inout [1:0]ch0_lpddr4_c0_dqs_c_b;
  inout [1:0]ch0_lpddr4_c0_dqs_t_a;
  inout [1:0]ch0_lpddr4_c0_dqs_t_b;
  output ch0_lpddr4_c0_reset_n;
  output [5:0]ch0_lpddr4_c1_ca_a;
  output [5:0]ch0_lpddr4_c1_ca_b;
  output ch0_lpddr4_c1_ck_c_a;
  output ch0_lpddr4_c1_ck_c_b;
  output ch0_lpddr4_c1_ck_t_a;
  output ch0_lpddr4_c1_ck_t_b;
  output ch0_lpddr4_c1_cke_a;
  output ch0_lpddr4_c1_cke_b;
  output ch0_lpddr4_c1_cs_a;
  output ch0_lpddr4_c1_cs_b;
  inout [1:0]ch0_lpddr4_c1_dmi_a;
  inout [1:0]ch0_lpddr4_c1_dmi_b;
  inout [15:0]ch0_lpddr4_c1_dq_a;
  inout [15:0]ch0_lpddr4_c1_dq_b;
  inout [1:0]ch0_lpddr4_c1_dqs_c_a;
  inout [1:0]ch0_lpddr4_c1_dqs_c_b;
  inout [1:0]ch0_lpddr4_c1_dqs_t_a;
  inout [1:0]ch0_lpddr4_c1_dqs_t_b;
  output ch0_lpddr4_c1_reset_n;
  output [5:0]ch1_lpddr4_c0_ca_a;
  output [5:0]ch1_lpddr4_c0_ca_b;
  output ch1_lpddr4_c0_ck_c_a;
  output ch1_lpddr4_c0_ck_c_b;
  output ch1_lpddr4_c0_ck_t_a;
  output ch1_lpddr4_c0_ck_t_b;
  output ch1_lpddr4_c0_cke_a;
  output ch1_lpddr4_c0_cke_b;
  output ch1_lpddr4_c0_cs_a;
  output ch1_lpddr4_c0_cs_b;
  inout [1:0]ch1_lpddr4_c0_dmi_a;
  inout [1:0]ch1_lpddr4_c0_dmi_b;
  inout [15:0]ch1_lpddr4_c0_dq_a;
  inout [15:0]ch1_lpddr4_c0_dq_b;
  inout [1:0]ch1_lpddr4_c0_dqs_c_a;
  inout [1:0]ch1_lpddr4_c0_dqs_c_b;
  inout [1:0]ch1_lpddr4_c0_dqs_t_a;
  inout [1:0]ch1_lpddr4_c0_dqs_t_b;
  output ch1_lpddr4_c0_reset_n;
  output [5:0]ch1_lpddr4_c1_ca_a;
  output [5:0]ch1_lpddr4_c1_ca_b;
  output ch1_lpddr4_c1_ck_c_a;
  output ch1_lpddr4_c1_ck_c_b;
  output ch1_lpddr4_c1_ck_t_a;
  output ch1_lpddr4_c1_ck_t_b;
  output ch1_lpddr4_c1_cke_a;
  output ch1_lpddr4_c1_cke_b;
  output ch1_lpddr4_c1_cs_a;
  output ch1_lpddr4_c1_cs_b;
  inout [1:0]ch1_lpddr4_c1_dmi_a;
  inout [1:0]ch1_lpddr4_c1_dmi_b;
  inout [15:0]ch1_lpddr4_c1_dq_a;
  inout [15:0]ch1_lpddr4_c1_dq_b;
  inout [1:0]ch1_lpddr4_c1_dqs_c_a;
  inout [1:0]ch1_lpddr4_c1_dqs_c_b;
  inout [1:0]ch1_lpddr4_c1_dqs_t_a;
  inout [1:0]ch1_lpddr4_c1_dqs_t_b;
  output ch1_lpddr4_c1_reset_n;
  output ddr4_dimm1_act_n;
  output [16:0]ddr4_dimm1_adr;
  output [1:0]ddr4_dimm1_ba;
  output [1:0]ddr4_dimm1_bg;
  output ddr4_dimm1_ck_c;
  output ddr4_dimm1_ck_t;
  output ddr4_dimm1_cke;
  output ddr4_dimm1_cs_n;
  inout [7:0]ddr4_dimm1_dm_n;
  inout [63:0]ddr4_dimm1_dq;
  inout [7:0]ddr4_dimm1_dqs_c;
  inout [7:0]ddr4_dimm1_dqs_t;
  output ddr4_dimm1_odt;
  output ddr4_dimm1_reset_n;
  input ddr4_dimm1_sma_clk_clk_n;
  input ddr4_dimm1_sma_clk_clk_p;
  input lpddr4_sma_clk1_clk_n;
  input lpddr4_sma_clk1_clk_p;
  input lpddr4_sma_clk2_clk_n;
  input lpddr4_sma_clk2_clk_p;

  wire [5:0]ch0_lpddr4_c0_ca_a;
  wire [5:0]ch0_lpddr4_c0_ca_b;
  wire ch0_lpddr4_c0_ck_c_a;
  wire ch0_lpddr4_c0_ck_c_b;
  wire ch0_lpddr4_c0_ck_t_a;
  wire ch0_lpddr4_c0_ck_t_b;
  wire ch0_lpddr4_c0_cke_a;
  wire ch0_lpddr4_c0_cke_b;
  wire ch0_lpddr4_c0_cs_a;
  wire ch0_lpddr4_c0_cs_b;
  wire [1:0]ch0_lpddr4_c0_dmi_a;
  wire [1:0]ch0_lpddr4_c0_dmi_b;
  wire [15:0]ch0_lpddr4_c0_dq_a;
  wire [15:0]ch0_lpddr4_c0_dq_b;
  wire [1:0]ch0_lpddr4_c0_dqs_c_a;
  wire [1:0]ch0_lpddr4_c0_dqs_c_b;
  wire [1:0]ch0_lpddr4_c0_dqs_t_a;
  wire [1:0]ch0_lpddr4_c0_dqs_t_b;
  wire ch0_lpddr4_c0_reset_n;
  wire [5:0]ch0_lpddr4_c1_ca_a;
  wire [5:0]ch0_lpddr4_c1_ca_b;
  wire ch0_lpddr4_c1_ck_c_a;
  wire ch0_lpddr4_c1_ck_c_b;
  wire ch0_lpddr4_c1_ck_t_a;
  wire ch0_lpddr4_c1_ck_t_b;
  wire ch0_lpddr4_c1_cke_a;
  wire ch0_lpddr4_c1_cke_b;
  wire ch0_lpddr4_c1_cs_a;
  wire ch0_lpddr4_c1_cs_b;
  wire [1:0]ch0_lpddr4_c1_dmi_a;
  wire [1:0]ch0_lpddr4_c1_dmi_b;
  wire [15:0]ch0_lpddr4_c1_dq_a;
  wire [15:0]ch0_lpddr4_c1_dq_b;
  wire [1:0]ch0_lpddr4_c1_dqs_c_a;
  wire [1:0]ch0_lpddr4_c1_dqs_c_b;
  wire [1:0]ch0_lpddr4_c1_dqs_t_a;
  wire [1:0]ch0_lpddr4_c1_dqs_t_b;
  wire ch0_lpddr4_c1_reset_n;
  wire [5:0]ch1_lpddr4_c0_ca_a;
  wire [5:0]ch1_lpddr4_c0_ca_b;
  wire ch1_lpddr4_c0_ck_c_a;
  wire ch1_lpddr4_c0_ck_c_b;
  wire ch1_lpddr4_c0_ck_t_a;
  wire ch1_lpddr4_c0_ck_t_b;
  wire ch1_lpddr4_c0_cke_a;
  wire ch1_lpddr4_c0_cke_b;
  wire ch1_lpddr4_c0_cs_a;
  wire ch1_lpddr4_c0_cs_b;
  wire [1:0]ch1_lpddr4_c0_dmi_a;
  wire [1:0]ch1_lpddr4_c0_dmi_b;
  wire [15:0]ch1_lpddr4_c0_dq_a;
  wire [15:0]ch1_lpddr4_c0_dq_b;
  wire [1:0]ch1_lpddr4_c0_dqs_c_a;
  wire [1:0]ch1_lpddr4_c0_dqs_c_b;
  wire [1:0]ch1_lpddr4_c0_dqs_t_a;
  wire [1:0]ch1_lpddr4_c0_dqs_t_b;
  wire ch1_lpddr4_c0_reset_n;
  wire [5:0]ch1_lpddr4_c1_ca_a;
  wire [5:0]ch1_lpddr4_c1_ca_b;
  wire ch1_lpddr4_c1_ck_c_a;
  wire ch1_lpddr4_c1_ck_c_b;
  wire ch1_lpddr4_c1_ck_t_a;
  wire ch1_lpddr4_c1_ck_t_b;
  wire ch1_lpddr4_c1_cke_a;
  wire ch1_lpddr4_c1_cke_b;
  wire ch1_lpddr4_c1_cs_a;
  wire ch1_lpddr4_c1_cs_b;
  wire [1:0]ch1_lpddr4_c1_dmi_a;
  wire [1:0]ch1_lpddr4_c1_dmi_b;
  wire [15:0]ch1_lpddr4_c1_dq_a;
  wire [15:0]ch1_lpddr4_c1_dq_b;
  wire [1:0]ch1_lpddr4_c1_dqs_c_a;
  wire [1:0]ch1_lpddr4_c1_dqs_c_b;
  wire [1:0]ch1_lpddr4_c1_dqs_t_a;
  wire [1:0]ch1_lpddr4_c1_dqs_t_b;
  wire ch1_lpddr4_c1_reset_n;
  wire ddr4_dimm1_act_n;
  wire [16:0]ddr4_dimm1_adr;
  wire [1:0]ddr4_dimm1_ba;
  wire [1:0]ddr4_dimm1_bg;
  wire ddr4_dimm1_ck_c;
  wire ddr4_dimm1_ck_t;
  wire ddr4_dimm1_cke;
  wire ddr4_dimm1_cs_n;
  wire [7:0]ddr4_dimm1_dm_n;
  wire [63:0]ddr4_dimm1_dq;
  wire [7:0]ddr4_dimm1_dqs_c;
  wire [7:0]ddr4_dimm1_dqs_t;
  wire ddr4_dimm1_odt;
  wire ddr4_dimm1_reset_n;
  wire ddr4_dimm1_sma_clk_clk_n;
  wire ddr4_dimm1_sma_clk_clk_p;
  wire lpddr4_sma_clk1_clk_n;
  wire lpddr4_sma_clk1_clk_p;
  wire lpddr4_sma_clk2_clk_n;
  wire lpddr4_sma_clk2_clk_p;

  custom_pfm_strmIn_strmOut_bd_wrapper bd_wrapper
       (.M00_AXIS_tdata(s_axis_tdata),
        .M00_AXIS_tlast(s_axis_tlast),
        .M00_AXIS_tready(s_axis_tready),
        .M00_AXIS_tstrb(s_axis_tstrb),
        .M00_AXIS_tvalid(s_axis_tvalid),
        .S00_AXIS_tdata(m_axis_tdata),
        .S00_AXIS_tlast(m_axis_tlast),
        .S00_AXIS_tready(m_axis_tready),
        .S00_AXIS_tstrb(m_axis_tstrb),
        .S00_AXIS_tvalid(m_axis_tvalid),
        .axis_aclk(aclk),
        .axis_aresetn(aresetn),
        .ch0_lpddr4_c0_ca_a(ch0_lpddr4_c0_ca_a),
        .ch0_lpddr4_c0_ca_b(ch0_lpddr4_c0_ca_b),
        .ch0_lpddr4_c0_ck_c_a(ch0_lpddr4_c0_ck_c_a),
        .ch0_lpddr4_c0_ck_c_b(ch0_lpddr4_c0_ck_c_b),
        .ch0_lpddr4_c0_ck_t_a(ch0_lpddr4_c0_ck_t_a),
        .ch0_lpddr4_c0_ck_t_b(ch0_lpddr4_c0_ck_t_b),
        .ch0_lpddr4_c0_cke_a(ch0_lpddr4_c0_cke_a),
        .ch0_lpddr4_c0_cke_b(ch0_lpddr4_c0_cke_b),
        .ch0_lpddr4_c0_cs_a(ch0_lpddr4_c0_cs_a),
        .ch0_lpddr4_c0_cs_b(ch0_lpddr4_c0_cs_b),
        .ch0_lpddr4_c0_dmi_a(ch0_lpddr4_c0_dmi_a),
        .ch0_lpddr4_c0_dmi_b(ch0_lpddr4_c0_dmi_b),
        .ch0_lpddr4_c0_dq_a(ch0_lpddr4_c0_dq_a),
        .ch0_lpddr4_c0_dq_b(ch0_lpddr4_c0_dq_b),
        .ch0_lpddr4_c0_dqs_c_a(ch0_lpddr4_c0_dqs_c_a),
        .ch0_lpddr4_c0_dqs_c_b(ch0_lpddr4_c0_dqs_c_b),
        .ch0_lpddr4_c0_dqs_t_a(ch0_lpddr4_c0_dqs_t_a),
        .ch0_lpddr4_c0_dqs_t_b(ch0_lpddr4_c0_dqs_t_b),
        .ch0_lpddr4_c0_reset_n(ch0_lpddr4_c0_reset_n),
        .ch0_lpddr4_c1_ca_a(ch0_lpddr4_c1_ca_a),
        .ch0_lpddr4_c1_ca_b(ch0_lpddr4_c1_ca_b),
        .ch0_lpddr4_c1_ck_c_a(ch0_lpddr4_c1_ck_c_a),
        .ch0_lpddr4_c1_ck_c_b(ch0_lpddr4_c1_ck_c_b),
        .ch0_lpddr4_c1_ck_t_a(ch0_lpddr4_c1_ck_t_a),
        .ch0_lpddr4_c1_ck_t_b(ch0_lpddr4_c1_ck_t_b),
        .ch0_lpddr4_c1_cke_a(ch0_lpddr4_c1_cke_a),
        .ch0_lpddr4_c1_cke_b(ch0_lpddr4_c1_cke_b),
        .ch0_lpddr4_c1_cs_a(ch0_lpddr4_c1_cs_a),
        .ch0_lpddr4_c1_cs_b(ch0_lpddr4_c1_cs_b),
        .ch0_lpddr4_c1_dmi_a(ch0_lpddr4_c1_dmi_a),
        .ch0_lpddr4_c1_dmi_b(ch0_lpddr4_c1_dmi_b),
        .ch0_lpddr4_c1_dq_a(ch0_lpddr4_c1_dq_a),
        .ch0_lpddr4_c1_dq_b(ch0_lpddr4_c1_dq_b),
        .ch0_lpddr4_c1_dqs_c_a(ch0_lpddr4_c1_dqs_c_a),
        .ch0_lpddr4_c1_dqs_c_b(ch0_lpddr4_c1_dqs_c_b),
        .ch0_lpddr4_c1_dqs_t_a(ch0_lpddr4_c1_dqs_t_a),
        .ch0_lpddr4_c1_dqs_t_b(ch0_lpddr4_c1_dqs_t_b),
        .ch0_lpddr4_c1_reset_n(ch0_lpddr4_c1_reset_n),
        .ch1_lpddr4_c0_ca_a(ch1_lpddr4_c0_ca_a),
        .ch1_lpddr4_c0_ca_b(ch1_lpddr4_c0_ca_b),
        .ch1_lpddr4_c0_ck_c_a(ch1_lpddr4_c0_ck_c_a),
        .ch1_lpddr4_c0_ck_c_b(ch1_lpddr4_c0_ck_c_b),
        .ch1_lpddr4_c0_ck_t_a(ch1_lpddr4_c0_ck_t_a),
        .ch1_lpddr4_c0_ck_t_b(ch1_lpddr4_c0_ck_t_b),
        .ch1_lpddr4_c0_cke_a(ch1_lpddr4_c0_cke_a),
        .ch1_lpddr4_c0_cke_b(ch1_lpddr4_c0_cke_b),
        .ch1_lpddr4_c0_cs_a(ch1_lpddr4_c0_cs_a),
        .ch1_lpddr4_c0_cs_b(ch1_lpddr4_c0_cs_b),
        .ch1_lpddr4_c0_dmi_a(ch1_lpddr4_c0_dmi_a),
        .ch1_lpddr4_c0_dmi_b(ch1_lpddr4_c0_dmi_b),
        .ch1_lpddr4_c0_dq_a(ch1_lpddr4_c0_dq_a),
        .ch1_lpddr4_c0_dq_b(ch1_lpddr4_c0_dq_b),
        .ch1_lpddr4_c0_dqs_c_a(ch1_lpddr4_c0_dqs_c_a),
        .ch1_lpddr4_c0_dqs_c_b(ch1_lpddr4_c0_dqs_c_b),
        .ch1_lpddr4_c0_dqs_t_a(ch1_lpddr4_c0_dqs_t_a),
        .ch1_lpddr4_c0_dqs_t_b(ch1_lpddr4_c0_dqs_t_b),
        .ch1_lpddr4_c0_reset_n(ch1_lpddr4_c0_reset_n),
        .ch1_lpddr4_c1_ca_a(ch1_lpddr4_c1_ca_a),
        .ch1_lpddr4_c1_ca_b(ch1_lpddr4_c1_ca_b),
        .ch1_lpddr4_c1_ck_c_a(ch1_lpddr4_c1_ck_c_a),
        .ch1_lpddr4_c1_ck_c_b(ch1_lpddr4_c1_ck_c_b),
        .ch1_lpddr4_c1_ck_t_a(ch1_lpddr4_c1_ck_t_a),
        .ch1_lpddr4_c1_ck_t_b(ch1_lpddr4_c1_ck_t_b),
        .ch1_lpddr4_c1_cke_a(ch1_lpddr4_c1_cke_a),
        .ch1_lpddr4_c1_cke_b(ch1_lpddr4_c1_cke_b),
        .ch1_lpddr4_c1_cs_a(ch1_lpddr4_c1_cs_a),
        .ch1_lpddr4_c1_cs_b(ch1_lpddr4_c1_cs_b),
        .ch1_lpddr4_c1_dmi_a(ch1_lpddr4_c1_dmi_a),
        .ch1_lpddr4_c1_dmi_b(ch1_lpddr4_c1_dmi_b),
        .ch1_lpddr4_c1_dq_a(ch1_lpddr4_c1_dq_a),
        .ch1_lpddr4_c1_dq_b(ch1_lpddr4_c1_dq_b),
        .ch1_lpddr4_c1_dqs_c_a(ch1_lpddr4_c1_dqs_c_a),
        .ch1_lpddr4_c1_dqs_c_b(ch1_lpddr4_c1_dqs_c_b),
        .ch1_lpddr4_c1_dqs_t_a(ch1_lpddr4_c1_dqs_t_a),
        .ch1_lpddr4_c1_dqs_t_b(ch1_lpddr4_c1_dqs_t_b),
        .ch1_lpddr4_c1_reset_n(ch1_lpddr4_c1_reset_n),
        .ddr4_dimm1_act_n(ddr4_dimm1_act_n),
        .ddr4_dimm1_adr(ddr4_dimm1_adr),
        .ddr4_dimm1_ba(ddr4_dimm1_ba),
        .ddr4_dimm1_bg(ddr4_dimm1_bg),
        .ddr4_dimm1_ck_c(ddr4_dimm1_ck_c),
        .ddr4_dimm1_ck_t(ddr4_dimm1_ck_t),
        .ddr4_dimm1_cke(ddr4_dimm1_cke),
        .ddr4_dimm1_cs_n(ddr4_dimm1_cs_n),
        .ddr4_dimm1_dm_n(ddr4_dimm1_dm_n),
        .ddr4_dimm1_dq(ddr4_dimm1_dq),
        .ddr4_dimm1_dqs_c(ddr4_dimm1_dqs_c),
        .ddr4_dimm1_dqs_t(ddr4_dimm1_dqs_t),
        .ddr4_dimm1_odt(ddr4_dimm1_odt),
        .ddr4_dimm1_reset_n(ddr4_dimm1_reset_n),
        .ddr4_dimm1_sma_clk_clk_n(ddr4_dimm1_sma_clk_clk_n),
        .ddr4_dimm1_sma_clk_clk_p(ddr4_dimm1_sma_clk_clk_p),
        .lpddr4_sma_clk1_clk_n(lpddr4_sma_clk1_clk_n),
        .lpddr4_sma_clk1_clk_p(lpddr4_sma_clk1_clk_p),
        .lpddr4_sma_clk2_clk_n(lpddr4_sma_clk2_clk_n),
        .lpddr4_sma_clk2_clk_p(lpddr4_sma_clk2_clk_p));
    
    wire         aclk;
    wire         aresetn;
    
    // AXI4-Stream slave interface (input)
    wire [63:0]  s_axis_tdata;
    wire         s_axis_tvalid;
    wire        s_axis_tready;
    wire         s_axis_tlast;
    wire [3:0]   s_axis_tstrb;
    
    // AXI4-Stream master interface (output)
    wire [63:0] m_axis_tdata;
    wire        m_axis_tvalid;
    wire         m_axis_tready;
    wire        m_axis_tlast;
    wire [3:0]  m_axis_tstrb;
    
    // Instantiating the AXI4S_counter_0 IP core
    AXI4S_Counter_0 axi4s_counter_0_0 (
        .m00_axis_aclk(aclk),
        .m00_axis_aresetn(aresetn),

        // Slave AXI4-Stream interface
        .m00_axis_tdata(m_axis_tdata),
        .m00_axis_tvalid(m_axis_tvalid),
        .m00_axis_tready(m_axis_tready),
        .m00_axis_tlast(m_axis_tlast),
        .m00_axis_tstrb(m_axis_tstrb)
     );
     
      // Instantiating the AXI4S_counter_0 IP core
    dummy_sink_0 dummy_sink_0_0 (
        .s00_axis_aclk(aclk),
        .s00_axis_aresetn(aresetn),

        // Slave AXI4-Stream interface
        .s00_axis_tdata(s_axis_tdata),
        .s00_axis_tvalid(s_axis_tvalid),
        .s00_axis_tready(s_axis_tready),
        .s00_axis_tlast(s_axis_tlast),
        .s00_axis_tstrb(s_axis_tstrb)
     );
     
    
endmodule
