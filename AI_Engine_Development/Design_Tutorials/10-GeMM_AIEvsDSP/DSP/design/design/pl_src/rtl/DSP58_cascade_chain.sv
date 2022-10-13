// © Copyright 2021 Xilinx, Inc.
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

// In this module 32 DSP58s are cascaded to form one chain
//
module DSP58_cascade_chain #(

parameter A_DATA_WIDTH = 27,
parameter CASCADE_LEN = 32,
parameter B_DATA_WIDTH = 24,
parameter OUTPUT_DATA_WIDTH = 58

) (

input                          clk,
input [CASCADE_LEN-1:0]        b_wen,
input [A_DATA_WIDTH-1:0]       a_data [CASCADE_LEN-1:0],
input [B_DATA_WIDTH-1:0]       b_data [CASCADE_LEN-1:0],
output [OUTPUT_DATA_WIDTH-1:0] y_data_out   // Only the last DSP58 output is driven out

);

wire [OUTPUT_DATA_WIDTH-1:0] y_data_out_int [CASCADE_LEN-1:0];
wire [OUTPUT_DATA_WIDTH-1:0] y_data_in_cascade [CASCADE_LEN-1:0];
wire [OUTPUT_DATA_WIDTH-1:0] y_data_out_cascade [CASCADE_LEN-1:0]; 
wire [8:0] OPMODE [CASCADE_LEN-1:0];   

genvar k;

// Drive output of last DSP58 
assign y_data_out = y_data_out_int [CASCADE_LEN-1]; 

// Drive cascade data out from prev DSP to next DSP
// OPMODE is also driven in this loop, since different OPMODE values need to
// be driven from first DSP and other DSPs

for (k=0; k<32; k=k+1) begin 
  if (k==0) begin
     assign y_data_in_cascade[k] = 58'b0;
     assign OPMODE[k] = 9'h5;
  end
  else begin
     assign y_data_in_cascade[k] = y_data_out_cascade[k-1];
     assign OPMODE[k] = 9'h15;
  end
end

for (k=0; k<32; k=k+1) begin : dsp58_loop

DSP58_top DSP58_top_inst (

  .clk                   (clk),
  .b_wen                 (b_wen[k]),     
  .a_data                (a_data[k]),      
  .b_data                (b_data[k]),      
  .OPMODE                (OPMODE[k]), 
  .y_data_in_cascade     (y_data_in_cascade[k]),  
  .y_data_out_cascade    (y_data_out_cascade[k]), 
  .y_data_out_fabric     (y_data_out_int[k]) 

  );

end

endmodule


