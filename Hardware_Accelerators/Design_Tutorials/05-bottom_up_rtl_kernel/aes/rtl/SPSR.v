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
   CLK, CEN, WEN, A, D
   );

parameter       WORD_DEPTH = 8,
                DATA_WIDTH = 8;
localparam      ADDR_WIDTH = $clog2(WORD_DEPTH);

input                           CLK;
input                           CEN;
input                           WEN;
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
    if ((!CEN) && (!WEN))
      memory[A] <= D;
  end

  // Read
  always @(posedge CLK) begin
    if ((!CEN) && WEN)
      Q <= memory[A];
  end


endmodule 

