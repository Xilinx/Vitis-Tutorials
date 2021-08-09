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

// Description     : Generic 2-Port Register File
// Memory Name rules :
//   Port - DP dual port, TP  two port, SP single port
//   Type - SR sram compiler (ra1sh, ra2sh);
//          RF register file compiler (rf1sh, rf2sh)
//   Size - depth x bits

module TPSR (
   // Outputs
   QA,
   // Inputs
   CLKA, CLKB, CENA, CENB, AA, AB, DB
   );

parameter       WORD_DEPTH = 8,
                DATA_WIDTH = 8;
localparam      ADDR_WIDTH = nextpow2(WORD_DEPTH);

input                           CLKA;
input                           CLKB;
input                           CENA;
input                           CENB;
input  [ADDR_WIDTH-1:0]         AA;
input  [ADDR_WIDTH-1:0]         AB;

output [DATA_WIDTH-1:0]         QA;
input  [DATA_WIDTH-1:0]         DB;

// Constant function for parameter computation
function integer nextpow2(input integer x);
      integer t;
      begin
	 t=x-1;// To be compatible with matlab
	 for(nextpow2=0; t>0; nextpow2=nextpow2+1)
	   t = t >> 1;
      end
endfunction // nextpow2

  reg    [DATA_WIDTH-1:0]         memory[0:WORD_DEPTH-1];
  reg    [DATA_WIDTH-1:0]         QA;

  // Write
  always @(posedge CLKB) begin
    if (!CENB)
      memory[AB] <= DB;
  end

  // Read
  always @(posedge CLKA) begin
    if (!CENA)
      QA <= memory[AA];
  end

endmodule 

