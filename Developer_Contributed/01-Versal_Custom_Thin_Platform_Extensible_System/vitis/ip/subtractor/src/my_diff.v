//
// Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

`timescale 1 ps / 1 ps

// ONLY TO BE USED FOR STANDALONE RTL DEBUG
// `default_nettype none

(* use_dsp = "yes" *) module my_diff
  # (
    parameter integer SIZEIN  = 16
  )
  (
    input  wire                       resetn,
    input  wire                       clk,
    input  wire signed [SIZEIN-1:0]   a,
    input  wire signed [SIZEIN-1:0]   b,
    output wire signed [SIZEIN-1:0]   res
  );

  localparam [SIZEIN:0]     one_mul = 1;
  reg signed [SIZEIN-1:0]   a_reg;
  reg signed [SIZEIN-1:0]   b_reg;
  reg signed [SIZEIN:0]     diff_reg;
  reg signed [2*SIZEIN-1:0] m_reg;
  reg signed [2*SIZEIN-1:0] p_reg;

  assign res = p_reg[SIZEIN-1:0];

  always @(posedge clk) begin
    if (!resetn) begin
      a_reg     <= 0;
      b_reg     <= 0;
      diff_reg  <= 0;
      m_reg     <= 0;
      p_reg     <= 0;
    end
    else begin
      a_reg     <= a;
      b_reg     <= b;
      diff_reg  <= a_reg - b_reg;
      m_reg     <= diff_reg * one_mul;
      p_reg     <= m_reg;
    end
  end

endmodule
