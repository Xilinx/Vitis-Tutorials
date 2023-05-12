/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


module fft_tb;
    

   reg   clk;
   reg   rst;
   reg   start;
   wire  done;
   wire  idle;
   wire  ready;

   wire         inData_0_ce;
   wire         inData_1_ce;
   wire         inData_2_ce;
   wire         inData_3_ce;

   wire [31:0]  inData_0;
   wire [31:0]  inData_1;
   wire [31:0]  inData_2;
   wire [31:0]  inData_3;

   wire         outData_0_we;
   wire         outData_1_we;
   wire         outData_2_we;
   wire         outData_3_we;

   wire [41:0]  outData_0;
   wire [41:0]  outData_1;
   wire [41:0]  outData_2;
   wire [41:0]  outData_3;

   reg  [31:0]  data_in    [0:15];
   reg  [41:0]  data_out   [0:15];
   reg  [41:0]  data_ref   [0:15];

   reg  [7:0]   inAddr_0;
   reg  [7:0]   inAddr_1;
   reg  [7:0]   inAddr_2;
   reg  [7:0]   inAddr_3;

   reg  [7:0]   outAddr_0;
   reg  [7:0]   outAddr_1;
   reg  [7:0]   outAddr_2;
   reg  [7:0]   outAddr_3;

   integer i, error;

  fft_wrap DUT (
  .inData_0_ce    (inData_0_ce),
  .inData_1_ce    (inData_1_ce),
  .inData_2_ce    (inData_2_ce),
  .inData_3_ce    (inData_3_ce),
  .inData_0       (inData_0),
  .inData_1       (inData_1),
  .inData_2       (inData_2),
  .inData_3       (inData_3),
  .outData_0_we   (outData_0_we),
  .outData_1_we   (outData_1_we),
  .outData_2_we   (outData_2_we),
  .outData_3_we   (outData_3_we),  
  .outData_0      (outData_0),
  .outData_1      (outData_1),
  .outData_2      (outData_2),
  .outData_3      (outData_3),
  
  .clk            (clk),
  .rst            (rst),
  .start          (start),
  .done           (done),
  .idle           (idle),
  .ready          (ready)

);
    
   // Clock gen
   initial begin
      clk = 1'b0;
      forever #2.5 clk = !clk;
   end
 
   // reset logic
   initial begin
      rst = 1'b1;
      #100 rst = 1'b0;
   end    
   
   // start logic
   initial begin
      start = 1'b0;
      wait (rst == 1'b0);
      #100 @(posedge clk) start = 1'b1;
      wait (done == 1'b1);
      @(posedge clk) start = 1'b0;
   end
   
   // data input stream logic
   assign inData_0 = data_in[inAddr_0];
   assign inData_1 = data_in[inAddr_1];
   assign inData_2 = data_in[inAddr_2];
   assign inData_3 = data_in[inAddr_3];
   
   initial begin
      $readmemh("datain.txt", data_in);
      $readmemh("dataref.txt", data_ref);
      inAddr_0 = 0;
      inAddr_1 = 4;
      inAddr_2 = 8;
      inAddr_3 = 12;
      forever @ (posedge clk) fork
         if (inData_0_ce) inAddr_0 = inAddr_0 + 1;
         if (inData_1_ce) inAddr_1 = inAddr_1 + 1;
         if (inData_2_ce) inAddr_2 = inAddr_2 + 1;
         if (inData_3_ce) inAddr_3 = inAddr_3 + 1;
      join
   end

   // data output stream logic
   initial begin
      outAddr_0 = 0;
      outAddr_1 = 4;
      outAddr_2 = 8;
      outAddr_3 = 12;
      forever @ (posedge clk) fork
         if (outData_0_we) begin
            data_out [outAddr_0] <= outData_0;
            outAddr_0 <= outAddr_0 + 1;
         end

         if (outData_1_we) begin
            data_out [outAddr_1] <= outData_1;
            outAddr_1 <= outAddr_1 + 1;
         end

         if (outData_2_we) begin
            data_out [outAddr_2] <= outData_2;
            outAddr_2 <= outAddr_2 + 1;
         end

         if (outData_3_we) begin
            data_out [outAddr_3] <= outData_3;
            outAddr_3 <= outAddr_3 + 1;
         end

      join
   end

   // data validation
   initial begin
      error = 0;
      #100
      @ (negedge done);
      #100 
      for (i = 0; i < 16; i = i + 1) begin
         if (data_out[i] !== data_ref[i]) begin
             error = error + 1;
         end
      end
      if (error != 0) begin
         $display("Result verification FAILED!");
      end else begin
         $display("Result verification SUCCEED!");
      end
      
      #300 $display("Simulation finished.");
      $finish;

   end

      
endmodule
