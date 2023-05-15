//
//# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
//# SPDX-License-Identifier: X11
//

// Description     : Generic Single-Port SRAM
// Memory Name rules :
//   Port - DP dual port, TP  two port, SP single port
//   Type - SR sram compiler (ra1sh, ra2sh);
//          RF register file compiler (rf1sh, rf2sh)
//   Size - depth x bits

module SPSR (
   // Outputs
   Q,
   // Inputs
   CLK, CE, WE, A, D
   );

parameter       WORD_DEPTH = 8,
                DATA_WIDTH = 8;
localparam      ADDR_WIDTH = $clog2(WORD_DEPTH);

input                           CLK;
input                           CE;
input                           WE;
input  [ADDR_WIDTH-1:0]         A;
input  [DATA_WIDTH-1:0]         D;

output [DATA_WIDTH-1:0]         Q;

// -===================================================-
//  RAM FPGA implementation
//  Following code can be synthesized by FPGA compiler
// -===================================================-
  reg    [DATA_WIDTH-1:0]         memory[0:WORD_DEPTH-1];
  reg    [DATA_WIDTH-1:0]         Q;

  // Write
  always @(posedge CLK) begin
    if (CE && WE)
      memory[A] <= D;
  end

  // Read
  always @(posedge CLK) begin
    if (CE && !WE)
      Q <= memory[A];
  end


endmodule 

