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

// This module instantiates 32 cascade chains

module DSP58_array #(

parameter NUM_CASCADE_CHAINS = 32,
parameter CASCADE_LEN = 32,
parameter A_DATA_WIDTH = 27,
parameter B_DATA_WIDTH = 24,
parameter OUTPUT_DATA_WIDTH = 58

) (

input                          clk,
input [CASCADE_LEN-1:0]        b_wen [NUM_CASCADE_CHAINS-1:0], 
input [A_DATA_WIDTH-1:0]       a_data [NUM_CASCADE_CHAINS-1:0] [CASCADE_LEN-1:0],
input [B_DATA_WIDTH-1:0]       b_data [NUM_CASCADE_CHAINS-1:0] [CASCADE_LEN-1:0], 
output [OUTPUT_DATA_WIDTH-1:0] y_data_out [NUM_CASCADE_CHAINS-1:0]

);

genvar k;

for (k=0; k<NUM_CASCADE_CHAINS; k=k+1) begin : cascade_chain_loop

DSP58_cascade_chain  DSP58_cascade_chain_inst (

.clk            (clk),
.b_wen          (b_wen[k]),
.a_data         (a_data[k]),
.b_data         (b_data[k]),
.y_data_out     (y_data_out[k])

);

end

endmodule
