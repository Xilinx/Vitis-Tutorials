/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// ==============================================================
`timescale 1 ns / 1 ps

module polar_clip_mac_muladd_16s_16s_32s_32_4_1_DSP48_1(
    input clk,
    input rst,
    input ce,
    input  [16 - 1:0] in0,
    input  [16 - 1:0] in1,
    input  [32 - 1:0] in2,
    output [32 - 1:0]  dout);

wire signed [27 - 1:0]     a;
wire signed [24 - 1:0]     b;
wire signed [58 - 1:0]     c;
wire signed [51 - 1:0]     m;
wire signed [58 - 1:0]     p;
reg  signed [51 - 1:0]     m_reg;
reg  signed [27 - 1:0]     a_reg;
reg  signed [24 - 1:0]     b_reg;
reg  signed [58 - 1:0]     p_reg;

assign a  = $signed(in0);
assign b  = $signed(in1);
assign c  = $signed(in2);

assign m  = a_reg * b_reg;
assign p  = m_reg + c;

always @(posedge clk) begin
    if (ce) begin
        m_reg  <= m;
        a_reg  <= a;
        b_reg  <= b;
        p_reg  <= p;
    end
end

assign dout = p_reg;

endmodule
`timescale 1 ns / 1 ps
module polar_clip_mac_muladd_16s_16s_32s_32_4_1(
    clk,
    reset,
    ce,
    din0,
    din1,
    din2,
    dout);

parameter ID = 32'd1;
parameter NUM_STAGE = 32'd1;
parameter din0_WIDTH = 32'd1;
parameter din1_WIDTH = 32'd1;
parameter din2_WIDTH = 32'd1;
parameter dout_WIDTH = 32'd1;
input clk;
input reset;
input ce;
input[din0_WIDTH - 1:0] din0;
input[din1_WIDTH - 1:0] din1;
input[din2_WIDTH - 1:0] din2;
output[dout_WIDTH - 1:0] dout;



polar_clip_mac_muladd_16s_16s_32s_32_4_1_DSP48_1 polar_clip_mac_muladd_16s_16s_32s_32_4_1_DSP48_1_U(
    .clk( clk ),
    .rst( reset ),
    .ce( ce ),
    .in0( din0 ),
    .in1( din1 ),
    .in2( din2 ),
    .dout( dout ));

endmodule

