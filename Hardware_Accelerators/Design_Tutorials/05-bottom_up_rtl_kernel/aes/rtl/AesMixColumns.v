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
//  This block implements MixColumns() Transformation.
//  This block operates on column data.
//                     S0_in
//                     S1_in
//                     S2_in
//                     S3_in

`timescale 1ns/1ps

module AesMixColumns (
        S0_in,
        S1_in,
        S2_in,
        S3_in,
        
        S0_out,
        S1_out,
        S2_out,
        S3_out
  );

  input  [7:0] S0_in;
  input  [7:0] S1_in;
  input  [7:0] S2_in;
  input  [7:0] S3_in;
                 
  output [7:0] S0_out;
  output [7:0] S1_out;
  output [7:0] S2_out;
  output [7:0] S3_out;
                 
  wire   [7:0] S0_in;
  wire   [7:0] S1_in;
  wire   [7:0] S2_in;
  wire   [7:0] S3_in;
                 
  wire   [7:0] S0_out;
  wire   [7:0] S1_out;
  wire   [7:0] S2_out;
  wire   [7:0] S3_out;
  

// --=========================================================================--
// Define Coefficient Multiplication Result Function
// --=========================================================================--
  function [7:0] Sx1;
    input [7:0] in;
    Sx1 = in;
  endfunction

  function [7:0] Sx2;
    input [7:0] in;
    if (in [7] == 1'b1)
      Sx2 = {in [6:0], 1'b0} ^ 8'b00011011;
    else
      Sx2 = {in [6:0], 1'b0};
  endfunction

  function [7:0] Sx3;
    input [7:0] in;
    Sx3 = Sx2 (in) ^ Sx1 (in);
  endfunction


// --=========================================================================--
// Matrix Multiplication
// --=========================================================================--
  assign S0_out = (Sx2(S0_in) ^ Sx3(S1_in)) ^ (Sx1(S2_in) ^ Sx1(S3_in));
  assign S1_out = (Sx1(S0_in) ^ Sx2(S1_in)) ^ (Sx3(S2_in) ^ Sx1(S3_in));
  assign S2_out = (Sx1(S0_in) ^ Sx1(S1_in)) ^ (Sx2(S2_in) ^ Sx3(S3_in));
  assign S3_out = (Sx3(S0_in) ^ Sx1(S1_in)) ^ (Sx1(S2_in) ^ Sx2(S3_in));

endmodule