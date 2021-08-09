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

// Assign correct control signals to respective sub-blocks

`timescale 1ns/1ps

  module  AesControl (
// Global Signals
        CLK,
        RESETn,
// SRAM Address signal
        KEY_A_EXP,
        KEY_A_ENC,
        KEY_A_DEC,
        KEY_A,
// Nr translation
        NK,
        NR_IN,
        NR_OUT,
// START strobe
        START_KEYEXP,
        START_EXP,
        START_CIPHER,
        START_ENC,
        START_DEC,
        MODE,
// Operation finish indicator
        FINISH_EXP,          // Key Expansion finishes
        FINISH_ENC,          // Encipher finishes
        FINISH_DEC,          // Decipher finishes
        FINISH,
// Output MUX
        DATA_CIPHER,
        DATA_PLAIN,
        DATA_OUTPUT
  );

  input          CLK;
  input          RESETn;

  input   [5:0]  KEY_A_EXP;
  input   [5:0]  KEY_A_ENC;
  input   [5:0]  KEY_A_DEC;
  output  [5:0]  KEY_A;

  input   [1:0]  NK;
  input   [3:0]  NR_IN;
  output  [3:0]  NR_OUT;

  input          START_KEYEXP;
  output         START_EXP;
  input          START_CIPHER;
  output         START_ENC;
  output         START_DEC;
  input          MODE;

  input          FINISH_EXP;
  input          FINISH_ENC;
  input          FINISH_DEC;
  output         FINISH;

  input  [127:0] DATA_CIPHER;
  input  [127:0] DATA_PLAIN;
  output [127:0] DATA_OUTPUT;
  
  wire           CLK;
  wire           RESETn;

  wire    [5:0]  KEY_A_EXP;
  wire    [5:0]  KEY_A_ENC;
  wire    [5:0]  KEY_A_DEC;
  wire    [5:0]  KEY_A;

  wire    [1:0]  NK;
  wire    [3:0]  NR_IN;
  reg     [3:0]  NR_OUT;

  wire           START_KEYEXP;
  wire           START_EXP; 
  wire           START_CIPHER;
  wire           START_ENC;
  wire           START_DEC;
  wire           MODE;

  wire           FINISH_EXP;
  wire           FINISH_ENC;
  wire           FINISH_DEC;
  wire           FINISH;

  wire   [127:0] DATA_CIPHER;
  wire   [127:0] DATA_PLAIN;
  wire   [127:0] DATA_OUTPUT;


  wire           START_or;      // ORed START strobe
  reg    [1:0]   start_src;     // Remember the source of START strobe.
                                // 01 - key expansion; 10 - encipher; 11 - decipher
  reg    [1:0]   start_cnt;     // 2-bit counter to generator 3-cycle START strobe
  wire           starting;      // ORed start_cnt


// KEY_A MUX  
  assign KEY_A = (~FINISH_EXP) ? KEY_A_EXP : ((~FINISH_ENC) ? KEY_A_ENC : KEY_A_DEC);


// NR_OUT Determination
  always @ (NR_IN or NK)
    if (NR_IN == 4'd0)
      case (NK)
        2'b00  : NR_OUT = 4'd10;
        2'b01  : NR_OUT = 4'd12;
        default: NR_OUT = 4'd14;
      endcase
    else
      NR_OUT = NR_IN;

  
// START Strobe Generation
  assign START_or = START_KEYEXP | START_CIPHER;    

  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
      start_src <= 2'b0;
    else
      if (START_KEYEXP)         // key expansion
        start_src <= 2'b01;
      else
        if (START_CIPHER)
          if (MODE)             // encipher (MODE = 1)
            start_src <= 2'b10;
          else                  // decipher
            start_src <= 2'b11;
            
  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
      start_cnt <= 2'b0;
    else
      if ((START_or & FINISH) || starting)
        start_cnt <= start_cnt + 1'b1;

  assign starting  = | start_cnt;
  assign START_EXP = starting & (start_src == 2'b01);
  assign START_ENC = starting & (start_src == 2'b10);
  assign START_DEC = starting & (start_src == 2'b11);


// FINISH ANDing
  assign FINISH = FINISH_ENC & FINISH_DEC;


// DATA_OUTPUT MUX
  assign DATA_OUTPUT = (start_src == 2'b10) ? DATA_CIPHER : DATA_PLAIN;
  
  
endmodule