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

`timescale 1ns/1ps

`define NK          2'b10
`define NR          4'b0
`define CYCLE_TIME  2.5

`ifndef PARA_OVERRIDE
  `define KEY         0
  `define WORD_NUM    100
`endif

module tb_aes;

  reg clk, reset_n;

  reg   [127:0] data_in;
  wire  [127:0] data_out;
  reg           start_cipher;
  reg           start_keyexp;
  wire          op_finish;
  wire          exp_finish;
  reg           op_mode;

  Aes DUT (
// Global Signals
    .CLK            (clk),
    .RESETn         (reset_n),
// Operation Data
    .DATA_INPUT     (data_in),      // Input Data
    .KEY            (256'h`KEY),          // Cipher key
    .DATA_OUTPUT    (data_out),     // Output Data
    .OP_MODE        (op_mode),      // Operation Mode, 1 means enciphering, 0 means deciphering
// Parameter Control
    .NK             (`NK),           // Key Length (WORD): 00, 01, 10 corresponds to 4, 6, 8
    .NR             (`NR),           // If NR is 0x0, default round number is used, otherwise the
                                    // value of NR is used as round number.
// Behavior Control
    .START_CIPHER   (start_cipher), // When START is high, cipher operation is executed (en or de).
    .START_KEYEXP   (start_keyexp), // When START is high, key expantion operation is executed.
    .OP_FINISH      (op_finish),    // OP_FINISH = 1 indicates the end of operation (cipher)
    .EXP_FINISH     (exp_finish)    // EXP_FINISH = 0 means key expansion is being executed
);


  always #(`CYCLE_TIME/2)
    clk = !clk;


  initial begin : general_init
    reset_n = 1'b0;
    clk = 1'b0;
    start_cipher = 0;
    start_keyexp = 0;
    #10 @(posedge clk) reset_n = 1'b1;    
  end
  
  initial begin : main_test_routine
    integer     temp;
    integer     plain_file;
    integer     cipher_file;      
    integer     i;
    integer     mismatch;
    integer     start_time;
    integer     finish_time;
    integer     enc_fail;
    integer     dec_fail;

    reg [127:0] plain_data[0:`WORD_NUM-1];
    reg [127:0] cipher_data[0:`WORD_NUM-1];
    reg [127:0] data_ref;

    enc_fail = 0;
    dec_fail = 0;

    plain_file = $fopen("plain.dat", "rb");
    cipher_file = $fopen("cipher.dat", "rb");
    
    temp = $fread(plain_data, plain_file);
    temp = $fread(cipher_data, cipher_file);
    
    $fclose(plain_file);
    $fclose(cipher_file);
    
    #1000
    
    // Key Expansion Operation
    @(posedge clk) start_keyexp = 1;
    @(posedge clk) start_keyexp = 0;
    @(posedge exp_finish); @(posedge clk);

    op_mode = 1;
    mismatch = 0;
    $display("-------------------------------------------------------------------------");
    $display("------------------------ AES-256 Encryption Test ------------------------");
    $display("-------------------------------------------------------------------------");
    $display("Running Frequency = %6d MHz", 1000/`CYCLE_TIME);
    $display("      Data number = %6d Bytes", `WORD_NUM*16);
    $display("              Key = %h", 'h`KEY);   
    // Encrypt WORK_NUM input data
    $display("-- Jobs Start");
    start_time = $stime();
    for (i = 0; i < `WORD_NUM; i = i + 1) begin
      data_in = plain_data[i];
      data_ref = cipher_data[i];
      @(posedge clk) start_cipher = 1;
      @(posedge clk) start_cipher = 0;
      @(posedge op_finish);
      @(posedge clk);
      if (data_ref == data_out)
          $display("--- WORD %8d MATCH, CIPHER = %h", i, data_out);
      else
          $display("--- WORD %8d MSMATCH, EXP = %h, ACT = %h", i, data_ref, data_out);
      if (data_ref !== data_out) begin
        mismatch = mismatch + 1;
      end
    end
    finish_time = $stime();
    $display("-- Jobs Finish");
    $display("Data check errors : %6d", mismatch);
    if (mismatch == 0) begin
      $display("Verification SUCCESS!");
      $display("Throughput: %6d MB/S", `WORD_NUM * 16 * 1000 / (finish_time - start_time));
    end else begin
      $display("Verification FAILED!");
      enc_fail = 1;
    end
    $display("\n");
    $fclose(plain_file);
    $fclose(cipher_file);

    #1000

    op_mode = 0;
    mismatch = 0; 
    $display("-------------------------------------------------------------------------");
    $display("------------------------ AES-256 Decryption Test ------------------------");
    $display("-------------------------------------------------------------------------");
    $display("Running Frequency = %6d MHz", 1000/`CYCLE_TIME);
    $display("      Data number = %6d Bytes", `WORD_NUM*16);
    $display("              Key = %h", 'h`KEY);   
    // Encrypt WORK_NUM input data
    $display("-- Jobs Start");
    start_time = $stime();
    for (i = 0; i < `WORD_NUM; i = i + 1) begin
      data_in = cipher_data[i];
      data_ref = plain_data[i];
      start_cipher = 1;
      @(posedge clk) start_cipher = 1;
      @(posedge clk) start_cipher = 0;
      @(posedge op_finish);
      @(posedge clk);
      if (data_ref == data_out)
          $display("--- WORD %8d MATCH, PLAIN = %h", i, data_out);
      else
          $display("--- WORD %8d MSMATCH, EXP = %h, ACT = %h", i, data_ref, data_out);
      if (data_ref !== data_out) begin
        mismatch = mismatch + 1;
      end
    end
    finish_time = $stime();
    $display("-- Jobs Finish");
    $display("Data check errors : %6d", mismatch);
    if (mismatch == 0) begin
      $display("Verification SUCCESS!");
      $display("Throughput: %6d MB/S", `WORD_NUM * 16 * 1000 / (finish_time - start_time));
    end else begin
      $display("Verification FAILED!");
      dec_fail = 1;
    end
    $display("-------------------------------------------------------------------------\n");
    if (enc_fail == 1)
      $display("Encryption test FAILURE");
    else
      $display("Encryption test PASS");
    if (dec_fail == 1)
      $display("Decryption test FAILURE");
    else
      $display("Decryption test PASS");
    $display("\n");
    #1000
    $finish();
  end

    
// Waveform dump
`ifdef DUMP_WAVEFORM
  initial begin
    $dumpfile("tb_aes.vcd");
    $dumpvars(0,tb_aes);
  end
`endif

endmodule
