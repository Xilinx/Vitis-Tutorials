/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

`timescale 1 ns / 1 ps

module my_top();
  
  reg axis_aclk;
  reg axis_aresetn;

  initial begin
    axis_aclk = 1'b0;
    axis_aresetn = 1'b0;
    #10 axis_aresetn = 1'b1;
  end
  
  always #1 axis_aclk <= ~axis_aclk;
 
  wire [31:0] m_axis_tdata;
  wire [0:0]  m_axis_tlast;
  wire [0:0]  m_axis_tready;
  wire [0:0]  m_axis_tvalid;
  wire [3:0]  m_axis_tkeep;
  wire [3:0]  m_axis_tstrb;

  wire [31:0] s_axis_tdata;
  wire [0:0]  s_axis_tlast;
  wire [0:0]  s_axis_tready;
  wire [0:0]  s_axis_tvalid;
  wire [3:0]  s_axis_tkeep;
  wire [3:0]  s_axis_tstrb;

  polar_clip polar_clip_0
       (.ap_clk(axis_aclk),
        .ap_rst_n(axis_aresetn),
        .in_sample_TDATA(s_axis_tdata),
        .in_sample_TVALID(s_axis_tvalid),
        .in_sample_TREADY(s_axis_tready),
        .in_sample_TKEEP(s_axis_tkeep),
        .in_sample_TSTRB(s_axis_tstrb),
        .in_sample_TLAST(s_axis_tlast),
        .out_sample_TDATA(m_axis_tdata),
        .out_sample_TVALID(m_axis_tvalid),
        .out_sample_TREADY(m_axis_tready),
        .out_sample_TKEEP(m_axis_tkeep),
        .out_sample_TSTRB(m_axis_tstrb),
        .out_sample_TLAST(m_axis_tlast)
       );

  aie_wrapper aiestub(
    .axis_aclk(axis_aclk),
    .axis_aresetn(axis_aresetn),
    //0 for (pl2me)
    .in_interpolator_m_axis_tvalid(),
    .in_interpolator_m_axis_tready(),
    .in_interpolator_m_axis_tdata(),
    .in_interpolator_m_axis_tstrb(),
    .in_interpolator_m_axis_tkeep(),
    .in_interpolator_m_axis_tlast(),
  //in_classifier for (pl2me)
    .in_classifier_m_axis_tvalid(m_axis_tvalid),
    .in_classifier_m_axis_tready(m_axis_tready),
    .in_classifier_m_axis_tdata(m_axis_tdata),
    .in_classifier_m_axis_tstrb(m_axis_tstrb),
    .in_classifier_m_axis_tkeep(m_axis_tkeep),
    .in_classifier_m_axis_tlast(1'b1),
  //out_interpolator for (me2pl)
    .out_interpolator_s_axis_tvalid(s_axis_tvalid),
    .out_interpolator_s_axis_tready(s_axis_tready),
    .out_interpolator_s_axis_tdata(s_axis_tdata),
    .out_interpolator_s_axis_tstrb(s_axis_tstrb),
    .out_interpolator_s_axis_tkeep(s_axis_tkeep),
    .out_interpolator_s_axis_tlast(s_axis_tlast),
  //3 for (me2pl)
    .out_classifier_s_axis_tvalid(),
    .out_classifier_s_axis_tready(),
    .out_classifier_s_axis_tdata(),
    .out_classifier_s_axis_tstrb(),
    .out_classifier_s_axis_tkeep(),
    .out_classifier_s_axis_tlast()
  );

endmodule
