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

// This block instantiates AesKeyExpansion, AesEncipher, AesDecipher, AesControl
// and a on-chip SRAM (AesKey_64x32) to form the full AES function module. 

`timescale 1ns/1ps

module Aes (
// Global Signals
  input          CLK,
  input          RESETn,
// Operation Data
  input  [127:0] DATA_INPUT,    // Input Data
  input  [255:0] KEY,           // Cipher key
  output [127:0] DATA_OUTPUT,   // Output Data
  input          OP_MODE,       // Operation Mode, 1 means enciphering, 0 means deciphering
// Parameter Control
  input  [1:0]   NK,            // Key Length (WORD): 00, 01, 10 corresponds to 4, 6, 8
  input  [3:0]   NR,            // If NR is 0x0, default round number is used, otherwise the
                                // value of NR is used as round number.
// Behavior Control
  input          START_CIPHER,  // When START is high, cipher operation is executed (en or de).
  input          START_KEYEXP,  // When START is high, key expantion operation is executed.
  output         OP_FINISH,     // OP_FINISH = 1 indicates the end of operation (cipher)
  output         EXP_FINISH     // EXP_FINISH = 0 means key expansion is being executed
);

  
// On-SRAM Signals
  wire [31:0] key_q;
  wire [31:0] key_d;
  wire [ 5:0] key_a;
  wire        key_wen;    

// Interconnection Signals

  wire        finish_enc;
  wire        finish_dec;

  wire        start_enc;
  wire        start_dec;
  wire        start_exp;
  
  wire [5:0]  key_a_exp;
  wire [5:0]  key_a_enc;
  wire [5:0]  key_a_dec;

  wire [3:0]  nr_cipher;
  
  wire [127:0] data_cipher;
  wire [127:0] data_plain;

// Instantiation of Key Expansion Module
  AesKeyExpansion uAesKeyExpansion (
        .CLK        (CLK),
        .RESETn     (RESETn),
        .KEY_Q      (key_q),
        .KEY_WEN    (key_wen),
        .KEY_A      (key_a_exp),
        .KEY_D      (key_d),
        .NK         (NK),
        .KEY        (KEY),
        .START      (start_exp),
        .FINISH     (EXP_FINISH)
    );


// Instantiation of Forware Cipher Module
  AesEncipher uAesEncipher (
        .CLK            (CLK),
        .RESETn         (RESETn),
        .DATA_PLAIN     (DATA_INPUT),
        .DATA_CIPHER    (data_cipher),
        .NR             (nr_cipher),
        .START          (start_enc),
        .FINISH         (finish_enc),
        .KEY_Q          (key_q),
        .KEY_A          (key_a_enc)
  );


// Instantiation of Inverse Cipher Module
  AesDecipher uAesDecipher (
        .CLK            (CLK),
        .RESETn         (RESETn),
        .DATA_CIPHER    (DATA_INPUT),
        .DATA_PLAIN     (data_plain),  // TO BE MODIFIED
        .NR             (nr_cipher),
        .START          (start_dec),
        .FINISH         (finish_dec),
        .KEY_Q          (key_q),
        .KEY_A          (key_a_dec)
  );


// Instantiation of Top Level Control Module
  AesControl uAesControl (
        .CLK            (CLK),
        .RESETn         (RESETn),
        
        .KEY_A_EXP      (key_a_exp),
        .KEY_A_ENC      (key_a_enc),
        .KEY_A_DEC      (key_a_dec),
        .KEY_A          (key_a),
        
        .NK             (NK),
        .NR_IN          (NR),
        .NR_OUT         (nr_cipher),
        
        .START_KEYEXP   (START_KEYEXP),
        .START_EXP      (start_exp),
        .START_CIPHER   (START_CIPHER),
        .START_ENC      (start_enc),
        .START_DEC      (start_dec),
        .MODE           (OP_MODE),
        
        .FINISH_EXP     (EXP_FINISH),
        .FINISH_ENC     (finish_enc),
        .FINISH_DEC     (finish_dec),
        .FINISH         (OP_FINISH),
        
        .DATA_CIPHER    (data_cipher),
        .DATA_PLAIN     (data_plain),
        .DATA_OUTPUT    (DATA_OUTPUT)
  );


// Instantiation of Key Schedule Memory
  wire          Rf64x32_CEN;
  wire  [31:0]  Rf64x32_Q;
  wire          Rf64x32_WEN;
  wire  [5:0]   Rf64x32_A;
  wire  [31:0]  Rf64x32_D;

  wire cen;
  assign cen = OP_FINISH & EXP_FINISH;

  SPSR #(.WORD_DEPTH(64), .DATA_WIDTH(32))
  uAesKey_64x32 (
        .CEN        (cen),
        .Q          (key_q),
        .CLK        (CLK),
        .WEN        (key_wen),
        .A          (key_a),
        .D          (key_d)
  );

endmodule
