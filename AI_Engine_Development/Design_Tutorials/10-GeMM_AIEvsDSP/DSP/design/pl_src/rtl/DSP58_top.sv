/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

module DSP58_top #(
  parameter TRUE = 1,
  parameter FALSE = 0,
  parameter REGISTER_Y    = TRUE,  // Enable DSP PREG: 0=bypass PREG; 1=use PREG
  parameter P1   = TRUE,  // Input pipeline depth. 
  parameter P2   = TRUE,  // Internal pipeline depth. 
  parameter P3   = TRUE,  // Another internal pipeline depth. 
  parameter A_DATA_WIDTH  = 27,    // A input data width
  parameter DSP_A_WIDTH = 27,
  parameter SIGN_EXTEND_A = 0, // Sign extend A to DSP A port; only when A_DATA_WIDTH < DSP A port width
  parameter B_DATA_WIDTH  = 24,    // B input data width.
  parameter DSP_B_WIDTH = 24,
  parameter SIGN_EXTEND_B = 0, // Sign extend B to DSP B port; only when B_DATA_WIDTH < DSP B port width
  parameter SIGN_EXTEND_C = 0,  // sign extend C to DSP C port; only when C_DATA_WIDTH < DSP C port width
  parameter Y_DATA_WIDTH  = 58    // DSP output data width
)  (

  input                     clk,

  input                     b_wen,     // clock enable A ping register 
  input  [A_DATA_WIDTH-1:0] a_data,      // A data input
  input  [B_DATA_WIDTH-1:0] b_data,      // B data input
  input  [8:0]              OPMODE, 
  input  [Y_DATA_WIDTH-1:0] y_data_in_cascade,  // Vertical input data for summation
  output [Y_DATA_WIDTH-1:0] y_data_out_cascade, // Vertical sum output, dedicated DSP cascade
  output [Y_DATA_WIDTH-1:0] y_data_out_fabric   // DSP output to fabric = DSP P output

  );

  wire [DSP_A_WIDTH-1:0] dsp_a_data = SIGN_EXTEND_A ? $signed(a_data) : $unsigned(a_data);
  wire [DSP_B_WIDTH-1:0] dsp_b_data = SIGN_EXTEND_B ? $signed(b_data) : $unsigned(b_data);
  wire use_c_data = 1'b0;

  DSP58 #(
    // Feature Control Attributes: Data Path Selection
    .AMULTSEL("A"),
    .A_INPUT("DIRECT"),
    .BMULTSEL("B"),
    .B_INPUT("DIRECT"),
    .PREADDINSEL("A"),
    .USE_MULT("MULTIPLY"),
    .USE_SIMD("ONE58"),
    .USE_WIDEXOR("FALSE"),
    .XORSIMD("XOR24_34_58_116"),
    .DSP_MODE ("INT24"),
    // Register Control Attributes: Pipeline Register Configuration
    .AREG(1),        // AREG is used as ping-pong register thus always enable
    .ACASCREG(1),    // DSP constraint: ACASCREG == AREG
    .BREG(P2),
    .BCASCREG(P2),   // DSP constraint: BCASCREG == BREG
    .CREG(P3),
    .DREG(1),
    .INMODEREG(P1),
    .ADREG(P2),
    .MREG(P3),
    .ALUMODEREG(1),
    .OPMODEREG(P3),
    .CARRYINREG(1),
    .CARRYINSELREG(1),
    .PREG(REGISTER_Y)
  ) dsp58_inst (
    .CLK(clk),
    .ACIN('b0),
    .BCIN('b0),
    .CARRYCASCIN('b0),
    .MULTSIGNIN('b0),
    .CARRYIN('b0),
    .D('b1),            // D unused thus can be tied to 1 to save routing resource. 1 is free tie-off. 0 consume route
    // Cascade outputs
    .P(y_data_out_fabric),
    .PCOUT(y_data_out_cascade),
    // Cascade inputs
    .PCIN(y_data_in_cascade),
    // Control inputs
    .ALUMODE('b0),
    .CARRYINSEL('b0),
    .INMODE(5'b00000),
    //.OPMODE({use_c_data,use_c_data,2'b00,use_y_cascade,1'b0,use_multiplier,1'b0,use_multiplier}),
    .OPMODE(OPMODE),
    // Data inputs
    .A(dsp_a_data),
    .B(dsp_b_data),
    .C(16'b0),
    // Reset/Clock Enable inputs
    .CEA1(1'b0),
    .CEA2(1'b1),
    .CEAD('b1),         // TODO: use use_multiplier to disable; but must be pipelined version
    .CEALUMODE('b0),
    .CEB1(1'b0),
    .CEB2(b_wen),         // TODO: use use_multiplier to disable; but must be pipelined version
    .CEC(use_c_data),
    .CEM('b1),          // TODO: use use_multiplier to disable; but must be pipelined version
    .CECARRYIN(1'b0),
    .CECTRL(1'b1),
    .CED(1'b0),
    .CEINMODE(1'b1),
    .CEP(1'b1),        // TODO: use use_multiplier to disable; but must be pipelined version
    .RSTA(1'b0),
    .RSTALLCARRYIN(1'b0),
    .RSTALUMODE(1'b0),
    .RSTB(1'b0),
    .RSTC(1'b0),
    .RSTCTRL(1'b0),
    .RSTD(1'b0),
    .RSTINMODE(1'b0),
    .RSTM(1'b0),
    .RSTP(1'b0),
    // unused pins
    .ACOUT(),
    .BCOUT(),
    .CARRYCASCOUT(),
    .CARRYOUT(),
    .MULTSIGNOUT(),
    .OVERFLOW(),
    .UNDERFLOW(),
    .PATTERNDETECT(),
    .PATTERNBDETECT(),
    .XOROUT()
  );

endmodule
