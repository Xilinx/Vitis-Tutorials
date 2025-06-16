//
// Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

`timescale 100 ps / 1 ps

module tb_top();

  // Connections used in testbench

  logic   [63:0]    AI_ENGINE_0_DM_OUT0_tdata;
  logic             AI_ENGINE_0_DM_OUT0_tkeep;
  logic             AI_ENGINE_0_DM_OUT0_tlast;
  logic             AI_ENGINE_0_DM_OUT0_tready;
  logic             AI_ENGINE_0_DM_OUT0_tvalid;
  logic   [63:0]    AI_ENGINE_0_DM_OUT1_tdata;
  logic             AI_ENGINE_0_DM_OUT1_tkeep;
  logic             AI_ENGINE_0_DM_OUT1_tlast;
  logic             AI_ENGINE_0_DM_OUT1_tready;
  logic             AI_ENGINE_0_DM_OUT1_tvalid;
  logic   [63:0]    AI_ENGINE_0_DM_OUT2_tdata;
  logic             AI_ENGINE_0_DM_OUT2_tkeep;
  logic             AI_ENGINE_0_DM_OUT2_tlast;
  logic             AI_ENGINE_0_DM_OUT2_tready;
  logic             AI_ENGINE_0_DM_OUT2_tvalid;
  logic   [63:0]    AI_ENGINE_0_DM_OUT3_tdata;
  logic             AI_ENGINE_0_DM_OUT3_tkeep;
  logic             AI_ENGINE_0_DM_OUT3_tlast;
  logic             AI_ENGINE_0_DM_OUT3_tready;
  logic             AI_ENGINE_0_DM_OUT3_tvalid;
  logic   [31:0]    AXIS1TO2_0_IN0_tdata;
  logic             AXIS1TO2_0_IN0_tready;
  logic             AXIS1TO2_0_IN0_tvalid;
  logic   [63:0]    COUNTER_0_M00_AXIS_tdata;
  logic             COUNTER_0_M00_AXIS_tlast;
  logic             COUNTER_0_M00_AXIS_tready;
  logic             COUNTER_0_M00_AXIS_tvalid;
//  logic   [63:0]    COUNTER_0_M03_AXIS_tdata;
//  logic             COUNTER_0_M03_AXIS_tlast;
//  logic             COUNTER_0_M03_AXIS_tready;
//  logic             COUNTER_0_M03_AXIS_tvalid;
//  logic   [63:0]    COUNTER_0_M04_AXIS_tdata;
//  logic             COUNTER_0_M04_AXIS_tlast;
//  logic             COUNTER_0_M04_AXIS_tready;
//  logic             COUNTER_0_M04_AXIS_tvalid;
  logic   [31:0]    VADD_S_0_OUT0_tdata;
  logic             VADD_S_0_OUT0_tready;
  logic             VADD_S_0_OUT0_tvalid;
  logic             aclk_x1;
  logic             aclk_x2;
  logic             aresetn;


  // Helper parameters
  localparam true_c   = 1'b1;
  localparam false_c  = 1'b0;
  //localparam clk_period_c = 16; // 1600 ps -> 625 MHz clock
  localparam clk_period_c = 20; // 2000 ps -> 500 MHz clock

  // Instances used in testbench
  vss_top_wrapper DUT(
    .AI_ENGINE_0_DM_OUT0_tdata    (AI_ENGINE_0_DM_OUT0_tdata),
    .AI_ENGINE_0_DM_OUT0_tkeep    (AI_ENGINE_0_DM_OUT0_tkeep),
    .AI_ENGINE_0_DM_OUT0_tlast    (AI_ENGINE_0_DM_OUT0_tlast),
    .AI_ENGINE_0_DM_OUT0_tready   (AI_ENGINE_0_DM_OUT0_tready),
    .AI_ENGINE_0_DM_OUT0_tvalid   (AI_ENGINE_0_DM_OUT0_tvalid),
    .AI_ENGINE_0_DM_OUT1_tdata    (AI_ENGINE_0_DM_OUT1_tdata),
    .AI_ENGINE_0_DM_OUT1_tkeep    (AI_ENGINE_0_DM_OUT1_tkeep),
    .AI_ENGINE_0_DM_OUT1_tlast    (AI_ENGINE_0_DM_OUT1_tlast),
    .AI_ENGINE_0_DM_OUT1_tready   (AI_ENGINE_0_DM_OUT1_tready),
    .AI_ENGINE_0_DM_OUT1_tvalid   (AI_ENGINE_0_DM_OUT1_tvalid),
    .AI_ENGINE_0_DM_OUT2_tdata    (AI_ENGINE_0_DM_OUT2_tdata),
    .AI_ENGINE_0_DM_OUT2_tkeep    (AI_ENGINE_0_DM_OUT2_tkeep),
    .AI_ENGINE_0_DM_OUT2_tlast    (AI_ENGINE_0_DM_OUT2_tlast),
    .AI_ENGINE_0_DM_OUT2_tready   (AI_ENGINE_0_DM_OUT2_tready),
    .AI_ENGINE_0_DM_OUT2_tvalid   (AI_ENGINE_0_DM_OUT2_tvalid),
    .AI_ENGINE_0_DM_OUT3_tdata    (AI_ENGINE_0_DM_OUT3_tdata),
    .AI_ENGINE_0_DM_OUT3_tkeep    (AI_ENGINE_0_DM_OUT3_tkeep),
    .AI_ENGINE_0_DM_OUT3_tlast    (AI_ENGINE_0_DM_OUT3_tlast),
    .AI_ENGINE_0_DM_OUT3_tready   (AI_ENGINE_0_DM_OUT3_tready),
    .AI_ENGINE_0_DM_OUT3_tvalid   (AI_ENGINE_0_DM_OUT3_tvalid),
    .AXIS1TO2_0_IN0_tdata         (AXIS1TO2_0_IN0_tdata),
    .AXIS1TO2_0_IN0_tready        (AXIS1TO2_0_IN0_tready),
    .AXIS1TO2_0_IN0_tvalid        (AXIS1TO2_0_IN0_tvalid),
    .COUNTER_0_M00_AXIS_tdata     (COUNTER_0_M00_AXIS_tdata),
    .COUNTER_0_M00_AXIS_tlast     (COUNTER_0_M00_AXIS_tlast),
    .COUNTER_0_M00_AXIS_tready    (COUNTER_0_M00_AXIS_tready),
    .COUNTER_0_M00_AXIS_tvalid    (COUNTER_0_M00_AXIS_tvalid),
//    .COUNTER_0_M03_AXIS_tdata     (COUNTER_0_M03_AXIS_tdata),
//    .COUNTER_0_M03_AXIS_tlast     (COUNTER_0_M03_AXIS_tlast),
//    .COUNTER_0_M03_AXIS_tready    (COUNTER_0_M03_AXIS_tready),
//    .COUNTER_0_M03_AXIS_tvalid    (COUNTER_0_M03_AXIS_tvalid),
//    .COUNTER_0_M04_AXIS_tdata     (COUNTER_0_M04_AXIS_tdata),
//    .COUNTER_0_M04_AXIS_tlast     (COUNTER_0_M04_AXIS_tlast),
//    .COUNTER_0_M04_AXIS_tready    (COUNTER_0_M04_AXIS_tready),
//    .COUNTER_0_M04_AXIS_tvalid    (COUNTER_0_M04_AXIS_tvalid),
    .VADD_S_0_OUT0_tdata          (VADD_S_0_OUT0_tdata),
    .VADD_S_0_OUT0_tready         (VADD_S_0_OUT0_tready),
    .VADD_S_0_OUT0_tvalid         (VADD_S_0_OUT0_tvalid),
    .ai_engine_0_dm_out0_aclk     (aclk_x2),
    .ai_engine_0_dm_out0_aresetn  (aresetn),
    .ai_engine_0_dm_out1_aclk     (aclk_x2),
    .ai_engine_0_dm_out1_aresetn  (aresetn),
    .ai_engine_0_dm_out2_aclk     (aclk_x2),
    .ai_engine_0_dm_out2_aresetn  (aresetn),
    .ai_engine_0_dm_out3_aclk     (aclk_x2),
    .ai_engine_0_dm_out3_aresetn  (aresetn),
    .pl_250mhz_aclk               (aclk_x1),
    .pl_250mhz_aresetn            (aresetn),
    .pl_500mhz_aclk               (aclk_x2),
    .pl_500mhz_aresetn            (aresetn)
  );

  // Simple AXIS port driver
  driver_axis #(
    .IMAG_STEP(1),
    .DWIDTH(32),
    .FSIZE(256),
    .INIT_DLY(20)
  ) driver_i0 (
    .aresetn(aresetn),
    .aclk(aclk_x1),
    .m00_axis_tready(AXIS1TO2_0_IN0_tready),
    .m00_axis_tdata(AXIS1TO2_0_IN0_tdata),
    .m00_axis_tvalid(AXIS1TO2_0_IN0_tvalid),
    .m00_axis_tlast()
  );


  // Simple AXIS port monitor
  monitor_axis #(
    .DWIDTH(32)
  ) monitor_i0 (
    .aresetn(aresetn),
    .aclk(aclk_x1),
    .s00_axis_tready(VADD_S_0_OUT0_tready),
    .s00_axis_tdata(VADD_S_0_OUT0_tdata),
    .s00_axis_tvalid(VADD_S_0_OUT0_tvalid),
    .s00_axis_tlast(false_c)
  );


  // Testbench

  // Force tready to enable remaining AXIS masters to send data
  initial begin
    AI_ENGINE_0_DM_OUT0_tready  = true_c;
    AI_ENGINE_0_DM_OUT1_tready  = true_c;
    AI_ENGINE_0_DM_OUT2_tready  = true_c;
    AI_ENGINE_0_DM_OUT3_tready  = true_c;
    COUNTER_0_M00_AXIS_tready   = true_c;
//    COUNTER_0_M03_AXIS_tready   = true_c;
//    COUNTER_0_M04_AXIS_tready   = true_c;
  end

  // Clocks and reset
  initial begin
    aclk_x1 <= true_c;
    aclk_x2 <= true_c;
    aresetn = false_c;
    aresetn = # 800 true_c; // Hold PL reset until AIE is booted
    fork
      forever #(2*clk_period_c/2) aclk_x1 = ~aclk_x1;
      forever #(1*clk_period_c/2) aclk_x2 = ~aclk_x2;
    join
  end


endmodule
