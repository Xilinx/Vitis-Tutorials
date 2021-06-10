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
//  This block implements key expansion block for AES. For the purpose of
//  flexibility, all together 64 word storage space are set, so 16 output key
//  can be provided (up to 16 iterations can be supported).

`timescale 1ns/1ps

module AesKeyExpansion (
// Global Signals
        CLK,
        RESETn,
// On-Chip Memory Interface
        KEY_Q,
        KEY_WEN,
        KEY_A,
        KEY_D,
// Key Expansion Control
        NK,            // Key Length (WORD): 00, 01, 10 corresponds to 4, 6, 8
        KEY,           // Input Key (128 bits, 192 bits, or 256 bits)
                       // The key is aligned to MSB, and the lowest words is unused
                       //    if key length is less than 8 words
// Behavior Control
        START,         // When START is high, key expantion is executed. START can 
                       // keep high for at least two cycle to avoid glitch on FINISH.
        FINISH         // FINISH = 1 indicates the end of the processing.
  );

  input         CLK;
  input         RESETn;
  
  input  [31:0] KEY_Q;
  output        KEY_WEN;
  output [ 5:0] KEY_A;
  output [31:0] KEY_D;
  
  input  [ 1:0] NK;
  input [255:0] KEY;
  
  input         START;
  output        FINISH;
  
  wire          CLK;
  wire          RESETn;
  
  wire   [31:0] KEY_Q;
  wire          KEY_WEN;
  reg    [ 5:0] KEY_A;
  reg    [31:0] KEY_D;
  
  wire   [ 1:0] NK;
  wire  [255:0] KEY;
  
  wire          START;
  wire          FINISH;
  

// --=========================================================================--
// Define Rcon() function
// --=========================================================================-- 
  function [7:0] Rcon;
    input [5:0] i;
    input [1:0] NK;
    if (NK == 2'b00)        // Nk = 4
      case (i)
        6'd4   : Rcon = 8'd1;
        6'd8   : Rcon = 8'd2;
        6'd12  : Rcon = 8'd4;
        6'd16  : Rcon = 8'd8;
        6'd20  : Rcon = 8'd16;
        6'd24  : Rcon = 8'd32;
        6'd28  : Rcon = 8'd64;
        6'd32  : Rcon = 8'd128;
        6'd36  : Rcon = 8'd27;
        6'd40  : Rcon = 8'd54;
        6'd44  : Rcon = 8'd108;
        6'd48  : Rcon = 8'd216;
        6'd52  : Rcon = 8'd171;
        6'd56  : Rcon = 8'd77;
        6'd60  : Rcon = 8'd154;
        default: Rcon = 8'd0;
      endcase
    else
      if (NK == 2'b01)      // Nk = 6
        case (i)
          6'd6   : Rcon = 8'd1;
          6'd12  : Rcon = 8'd2;
          6'd18  : Rcon = 8'd4;
          6'd24  : Rcon = 8'd8;
          6'd30  : Rcon = 8'd16;
          6'd36  : Rcon = 8'd32;
          6'd42  : Rcon = 8'd64;
          6'd48  : Rcon = 8'd128;
          6'd54  : Rcon = 8'd27;
          6'd60  : Rcon = 8'd54;
          default: Rcon = 8'd0;
        endcase
      else
        case (i)            // NK = 8
          6'd8   : Rcon = 8'd1;
          6'd16  : Rcon = 8'd2;
          6'd24  : Rcon = 8'd4;
          6'd32  : Rcon = 8'd8;
          6'd40  : Rcon = 8'd16;
          6'd48  : Rcon = 8'd32;
          6'd56  : Rcon = 8'd64;
          default: Rcon = 8'd0;
        endcase
  endfunction


// --=========================================================================--
// Mod_Nk_0 Function
//   This function is used to determind whether the value of counter mod Nk
// equals 0 (it will return 1, otherwise 0)
// --=========================================================================-- 
  function Mod_Nk_0;
    input [5:0] i;
    input [1:0] NK;
    case (NK)
      2'b00 :      // Nk = 4
        case (i)
          6'd4   : Mod_Nk_0 = 1'b1;
          6'd8   : Mod_Nk_0 = 1'b1;
          6'd12  : Mod_Nk_0 = 1'b1;
          6'd16  : Mod_Nk_0 = 1'b1;
          6'd20  : Mod_Nk_0 = 1'b1;
          6'd24  : Mod_Nk_0 = 1'b1;
          6'd28  : Mod_Nk_0 = 1'b1;
          6'd32  : Mod_Nk_0 = 1'b1;
          6'd36  : Mod_Nk_0 = 1'b1;
          6'd40  : Mod_Nk_0 = 1'b1;
          6'd44  : Mod_Nk_0 = 1'b1;
          6'd48  : Mod_Nk_0 = 1'b1;
          6'd52  : Mod_Nk_0 = 1'b1;
          6'd56  : Mod_Nk_0 = 1'b1;
          6'd60  : Mod_Nk_0 = 1'b1;
          default: Mod_Nk_0 = 1'b0;
        endcase
      2'b01 :      // Nk = 6
        case (i)
          6'd6   : Mod_Nk_0 = 1'b1;
          6'd12  : Mod_Nk_0 = 1'b1;
          6'd18  : Mod_Nk_0 = 1'b1;
          6'd24  : Mod_Nk_0 = 1'b1;
          6'd30  : Mod_Nk_0 = 1'b1;
          6'd36  : Mod_Nk_0 = 1'b1;
          6'd42  : Mod_Nk_0 = 1'b1;
          6'd48  : Mod_Nk_0 = 1'b1;
          6'd54  : Mod_Nk_0 = 1'b1;
          6'd60  : Mod_Nk_0 = 1'b1;
          default: Mod_Nk_0 = 1'b0;
        endcase
      default :    // Nk = 8
        case (i)
          6'd8   : Mod_Nk_0 = 1'b1;
          6'd16  : Mod_Nk_0 = 1'b1;
          6'd24  : Mod_Nk_0 = 1'b1;
          6'd32  : Mod_Nk_0 = 1'b1;
          6'd40  : Mod_Nk_0 = 1'b1;
          6'd48  : Mod_Nk_0 = 1'b1;
          6'd56  : Mod_Nk_0 = 1'b1;
          default: Mod_Nk_0 = 1'b0;
        endcase
    endcase
  endfunction
          

// --=========================================================================--
// Mod_Nk_4 Function
//   This function is used to determind whether the value of counter mod Nk (8)
// equals 4 (it will return 1, otherwise 0)
// --=========================================================================-- 
  function Mod_Nk_4;
    input [5:0] i;
    case (i)
      6'd12  : Mod_Nk_4 = 1'b1;
      6'd20  : Mod_Nk_4 = 1'b1;
      6'd28  : Mod_Nk_4 = 1'b1;
      6'd36  : Mod_Nk_4 = 1'b1;
      6'd44  : Mod_Nk_4 = 1'b1;
      6'd52  : Mod_Nk_4 = 1'b1;
      6'd60  : Mod_Nk_4 = 1'b1;
      default: Mod_Nk_4 = 1'b0;
    endcase
  endfunction


// --=========================================================================--
// Counter Control
// --=========================================================================--
  reg    [ 6:0] counter;   // Key/address counter and main state variable.
                           // The LSB indicates the two state in key calculation
                           //  (read and write memory).
  reg    [ 6:0] counter_next;

  always @ (counter or START or NK)
  begin
    if (counter[6:1] == 6'd0)
    begin
      if (START == 1'b1)
        counter_next = 7'd2;       // starting
      else
        counter_next = 7'd0;
    end
    else
    begin
      case (NK)
        2'b00  :   // Nk = 4
          if (counter[6:1] < 6'd3)
            counter_next = counter + 2'd2;
          else
            counter_next = counter + 1'd1;
      //--------
        2'b01  :   // Nk = 6
          if (counter[6:1] < 6'd5)
            counter_next = counter + 2'd2;
          else
            counter_next = counter + 1'd1;
      //--------
        default:   // Nk = 8
          if (counter[6:1] < 6'd7)
            counter_next = counter + 2'd2;
          else
            counter_next = counter + 1'd1;
      endcase
    end
  end
      
  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
      counter <= 7'd0;
    else
      counter <= counter_next; 


// --=========================================================================--
// FINISH, SRAM Address , and Read / Write Control (FINISH can also be used as CEN)
// --=========================================================================--

  assign FINISH  = (START == 1'b0) && ((counter == 7'd0) || (counter == 7'd127));
  assign KEY_WEN = FINISH | counter[0];

  always @ (counter or NK)
    if (counter [0] == 1'b0) // Write phase
      KEY_A = counter [6:1];
    else                     // Read phase
      KEY_A = counter [6:1] - ((NK == 2'b00) ? 2'd3 : ((NK == 2'b01) ? 3'd5 : 3'd7));


// --=========================================================================--
// Datapath / SRAM Data Control
// --=========================================================================--
  reg  [31:0] w_temp;       // Corresponds to the 'temp' in AES pseudo code ( W[i] )
  reg  [31:0] w_temp_next;
  reg  [31:0] temp;         // Temporary variable
  wire [31:0] SB_in, SB_out;

// W[i] array initialization control
  always @ (counter or KEY or w_temp_next or NK)     // Initialization or datapath
    case (counter [6:1])
      6'd0  : KEY_D = KEY [255 : 224];
      6'd1  : KEY_D = KEY [223 : 192];
      6'd2  : KEY_D = KEY [191 : 160];
      6'd3  : KEY_D = KEY [159 : 128];
      6'd4  : 
        if (NK != 2'b00)
          KEY_D = KEY [127 : 96];
        else
          KEY_D = w_temp_next;
      6'd5  :
        if (NK != 2'b00)
          KEY_D = KEY [95 : 64];
        else
          KEY_D = w_temp_next;
      6'd6  :
        if (NK[1] == 1'b1)
          KEY_D = KEY [63 : 32];
        else
          KEY_D = w_temp_next;
      6'd7  :
        if (NK[1] == 1'b1)
          KEY_D = KEY [31 : 0];
        else
          KEY_D = w_temp_next;
      default :  KEY_D = w_temp_next;
    endcase

// 'temp' (pesudo code) exceptional case operation control
  always @ (SB_out or counter or NK or w_temp)
    if (Mod_Nk_0 (counter[6:1], NK))        // (i mod Nk = 0)
      temp = {SB_out [31:24] ^ Rcon (counter[6:1], NK), SB_out [23:0]};
    else
      if (NK[1] && Mod_Nk_4(counter[6:1]))  // (Nk > 6 and i mod Nk = 4)
        temp = SB_out;
      else
        temp = w_temp;

// w[i-1] (temp) initialization control          
  always @ (NK or counter or KEY or temp or KEY_Q)
    case ({NK, counter[6:1]})
      {2'b00, 6'd3} : w_temp_next = KEY [159:128];
      {2'b01, 6'd5} : w_temp_next = KEY [ 95: 64];
      {2'b10, 6'd7},
      {2'b11, 6'd7} : w_temp_next = KEY [ 31:  0];
      default       : w_temp_next = KEY_Q ^ temp;
    endcase

  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
      w_temp <= 32'd0;
    else
      if (counter[0] == 1'b0)          // Assignment on '0' sub-state (LSB of counter)
        w_temp <= w_temp_next;
    
// i mod Nk == 0 or ~= 0  (RotWord or not)
  assign SB_in = Mod_Nk_0 (counter[6:1], NK) ? {w_temp[23:0], w_temp[31:24]} : w_temp;

// --=========================================================================--
// Instantiation of SubBytes() Block
// --=========================================================================-- 
  AesSubBytes nAesSubBytes (
        .S0_in    (SB_in [31:24]),
        .S1_in    (SB_in [23:16]),
        .S2_in    (SB_in [15: 8]),
        .S3_in    (SB_in [ 7: 0]),
        
        .S0_out   (SB_out [31:24]),
        .S1_out   (SB_out [23:16]),
        .S2_out   (SB_out [15: 8]),
        .S3_out   (SB_out [ 7: 0])
  );



endmodule

