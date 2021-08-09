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

// This block finishes decipher opeartion of AES.

`timescale 1ns/1ps

module AesDecipher (
// Global Signals
        CLK,
        RESETn,
// Operation Data
        DATA_CIPHER,   // Input Data
        DATA_PLAIN,    // Output Data
// Parameter Control
        NR,            // value of NR is used as round number.
// Behavior Control
        START,         // When START is high, encipher operation starts. START can
                       // keep high for at least 2 cycles to avoid glitch on FINISH.
        FINISH,        // FINISH = 1 indicates the end of operation (key or cipher)
// On-Chip Memory Interface
        KEY_Q,
        KEY_A
  );

  input          CLK;
  input          RESETn;
  
  input  [127:0] DATA_CIPHER;
  output [127:0] DATA_PLAIN;

  input  [3:0]   NR;
  
  input          START;
  output         FINISH;

  input  [31:0]  KEY_Q;
  output [ 5:0]  KEY_A;
  
  wire           CLK;
  wire           RESETn;
  
  wire   [127:0] DATA_PLAIN;
  wire   [127:0] DATA_CIPHER;
  
  wire   [3:0]   NR;
  
  wire           START;
  wire           FINISH;
  
  wire   [31:0]  KEY_Q;
  wire   [ 5:0]  KEY_A;
  

// --=========================================================================--
// Round Counter Control, SRAM Control
// --=========================================================================--
  reg       START_delay; 
  reg [5:0] counter;

// Delay START for one cycle to pre-enable SRAM for read opeartion
  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
      START_delay <= 1'b0;
    else
      START_delay <= START;

// Round and Sub-State Counting  
  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
      counter <= 6'd0;
    else
      if (counter != 6'd0)
      begin
        if ((counter[5:2] == NR) && (counter[1:0] == 2'b11))
          counter <= 6'd0;
        else
          counter <= counter + 1'b1;
      end
      else
      begin
        if (START_delay)
           counter <= 6'd1;
      end

// Working staus indicator: when counter is running or START is detected, FINISH
// signal will be pulled down. FINISH can also be used as CEN
  assign FINISH = ~(START | START_delay | (counter != 6'd0));

// The address are the inverse order of counter value
//  assign KEY_A   = (START & ~START_delay) ? 6'd0 : (counter + 1'b1);
  
  assign KEY_A   = (START & ~START_delay) ? {NR, 2'b11} : ({NR, 2'b10} - counter);


// --=========================================================================--
// Datapath Control
// --=========================================================================--
// Four colomn state registers compose the complete state variable:
//    |  | |  | |  | |  |  31  -  S00
//    |S0| |S1| |S2| |S3|
//    |  | |  | |  | |  |
//    |  | |  | |  | |  |   0  -  S30

  wire [ 31:0] sub_bytes;    // Result of SubBytes()
  wire [127:0] shift_row;    // Result of ShiftRows()
  wire [127:0] mix_colum;    // Result of MixColumns()  
  wire [127:0] add_rdkey;    // Result of AddRoundKey()

  reg  [ 31:0] mux_init;     // MUX output of SubBytes() result or input plain text
  wire [127:0] mux_asgn;     // MUX for round 0 exception
  wire [127:0] mux_mixc;     // MUX for round Nr exception
  
  reg [31:0] S0;
  reg [31:0] S1;
  reg [31:0] S2;
  reg [31:0] S3;
  
  reg [31:0] W1;
  reg [31:0] W2;
  reg [31:0] W3;
  
// State assignment MUX (DATA_PLAIN / SubBytes)
  always @ (sub_bytes or counter or DATA_CIPHER)
    if (counter [5:2] != 4'd0)
      mux_init = sub_bytes;
    else
      case (counter [1:0])
        2'b00: mux_init = DATA_CIPHER [127:96];
        2'b01: mux_init = DATA_CIPHER [ 95:64];
        2'b10: mux_init = DATA_CIPHER [ 63:32];
        2'b11: mux_init = DATA_CIPHER [ 31: 0];
      endcase


// State Assignment Control
  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
    begin
      S0 <= 32'b0;
      S1 <= 32'b0;
      S2 <= 32'b0;
      S3 <= 32'b0;
    end
    else
    begin
      if (counter [1:0] == 2'd3)
      begin
        S0 <= mux_mixc [127:96];
        S1 <= mux_mixc [ 95:64];
        S2 <= mux_mixc [ 63:32];
        S3 <= mux_mixc [ 31: 0];
      end
      else
      begin
        if (~FINISH)
        begin         // Conditional Update: only when counter are running
          S0 <= S1;
          S1 <= S2;
          S2 <= S3;
          S3 <= mux_init;
        end
      end
    end
        
      
// Key Array (w[i]) Assignment Control
  always @ (posedge CLK or negedge RESETn)
    if (~RESETn)
    begin
      W1 <= 32'b0;
      W2 <= 32'b0;
      W3 <= 32'b0;
    end
    else
    begin
      if (~FINISH)
      begin
        W1 <= KEY_Q;
        W2 <= W1;
        W3 <= W2;
      end
    end


// Instantiation of AesInvSubBytes()
  AesInvSubBytes uAesInvSubBytes (
        .S0_in      (S0 [31:24]),
        .S1_in      (S0 [23:16]),
        .S2_in      (S0 [15: 8]),
        .S3_in      (S0 [ 7: 0]),
        // ---------
        .S0_out     (sub_bytes [31:24]),
        .S1_out     (sub_bytes [23:16]),
        .S2_out     (sub_bytes [15: 8]),
        .S3_out     (sub_bytes [ 7: 0])
  );


// Instantiation of AesInvShiftRows()
  AesInvShiftRows uAesInvShiftRows (
        .S0_in      (S1),
        .S1_in      (S2),
        .S2_in      (S3),
        .S3_in      (mux_init),
        // ---------
        .S0_out     (shift_row [127:96]),
        .S1_out     (shift_row [ 95:64]),
        .S2_out     (shift_row [ 63:32]),
        .S3_out     (shift_row [ 31: 0])
  );

// MUX
  assign mux_asgn   = (counter [5:2] == 4'd0) ? {S1, S2, S3, mux_init} : shift_row [127:0];
                                               
// AddRoundKey ()
  assign add_rdkey = {KEY_Q, W1, W2, W3} ^ mux_asgn;

// Instantiation of AesInvMixColumns()
  AesInvMixColumns uAesInvMixColumns_0 (
        .S0_in      (add_rdkey [127:120]),
        .S1_in      (add_rdkey [119:112]),
        .S2_in      (add_rdkey [111:104]),
        .S3_in      (add_rdkey [103: 96]),
        // ---------
        .S0_out     (mix_colum [127:120]),
        .S1_out     (mix_colum [119:112]),
        .S2_out     (mix_colum [111:104]),
        .S3_out     (mix_colum [103: 96]));        
  AesInvMixColumns uAesInvMixColumns_1 (
        .S0_in      (add_rdkey [ 95: 88]),
        .S1_in      (add_rdkey [ 87: 80]),
        .S2_in      (add_rdkey [ 79: 72]),
        .S3_in      (add_rdkey [ 71: 64]),
        // ---------
        .S0_out     (mix_colum [ 95: 88]),
        .S1_out     (mix_colum [ 87: 80]),
        .S2_out     (mix_colum [ 79: 72]),
        .S3_out     (mix_colum [ 71: 64]));
  AesInvMixColumns uAesInvMixColumns_2 (
        .S0_in      (add_rdkey [ 63: 56]),
        .S1_in      (add_rdkey [ 55: 48]),
        .S2_in      (add_rdkey [ 47: 40]),
        .S3_in      (add_rdkey [ 39: 32]),
        // ---------
        .S0_out     (mix_colum [ 63: 56]),
        .S1_out     (mix_colum [ 55: 48]),
        .S2_out     (mix_colum [ 47: 40]),
        .S3_out     (mix_colum [ 39: 32]));
  AesInvMixColumns uAesInvMixColumns_3 (
        .S0_in      (add_rdkey [ 31: 24]),
        .S1_in      (add_rdkey [ 23: 16]),
        .S2_in      (add_rdkey [ 15:  8]),
        .S3_in      (add_rdkey [  7:  0]),
        // ---------
        .S0_out     (mix_colum [ 31: 24]),
        .S1_out     (mix_colum [ 23: 16]),
        .S2_out     (mix_colum [ 15:  8]),
        .S3_out     (mix_colum [  7:  0]));

// MUX
  assign mux_mixc [127:96] = ((counter [5:2] == NR) || (counter [5:2] == 4'd0)) ?
                               add_rdkey [127:96] : mix_colum [127:96];
  assign mux_mixc [ 95:64] = ((counter [5:2] == NR) || (counter [5:2] == 4'd0)) ?
                               add_rdkey [ 95:64] : mix_colum [ 95:64];
  assign mux_mixc [ 63:32] = ((counter [5:2] == NR) || (counter [5:2] == 4'd0)) ?
                               add_rdkey [ 63:32] : mix_colum [ 63:32];
  assign mux_mixc [ 31: 0] = ((counter [5:2] == NR) || (counter [5:2] == 4'd0)) ?
                               add_rdkey [ 31: 0] : mix_colum [ 31: 0];
  
// Output Assignment
  assign DATA_PLAIN = {S0, S1, S2, S3};

endmodule