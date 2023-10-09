/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

module B_data_mux #(

parameter CASCADE_LEN = 32,
parameter N = 32

) (

input             clk,
input             reset,
input [23:0]      colvec_ff_vec       [N-1:0][CASCADE_LEN-1:0],
input [4:0]       DSP58_B_data_muxsel [CASCADE_LEN-1:0],
output reg [23:0] DSP58_B_data_mux    [CASCADE_LEN-1:0]

);

// 31:1 mux is broken down into 2 stages
// 4 8:1 muxes are implemented first and te mux output is flopped
// Then 4:1 mux is implemented
// Final output is flopped in control_logic module, hence not flopped in this module

integer kk;
reg [23:0] DSP58_B_data_mux0  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux1  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux2  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux3  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux0_ff  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux1_ff  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux2_ff  [CASCADE_LEN-1:0];
reg [23:0] DSP58_B_data_mux3_ff  [CASCADE_LEN-1:0];
reg [4:0]  DSP58_B_data_muxsel_ff [CASCADE_LEN-1:0];

always @(*) begin
    for (kk=0; kk<CASCADE_LEN; kk=kk+1) begin
       case (DSP58_B_data_muxsel[kk][2:0]) // Depending upon muxsel value one of the columns is selected 
	  3'd0  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [0][kk];
	  3'd1  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [1][kk];
	  3'd2  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [2][kk];
	  3'd3  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [3][kk];
	  3'd4  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [4][kk];
	  3'd5  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [5][kk];
	  3'd6  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [6][kk];
	  3'd7  : DSP58_B_data_mux0 [kk] = colvec_ff_vec [7][kk];
       endcase
    end
end    

always @(*) begin
    for (kk=0; kk<CASCADE_LEN; kk=kk+1) begin
       case (DSP58_B_data_muxsel[kk][2:0]) // Depending upon muxsel value one of the columns is selected 
	  3'd0  : DSP58_B_data_mux1 [kk] = colvec_ff_vec [8][kk];
	  3'd1  : DSP58_B_data_mux1 [kk] = colvec_ff_vec [9][kk];
	  3'd2 : DSP58_B_data_mux1 [kk] = colvec_ff_vec [10][kk];
	  3'd3 : DSP58_B_data_mux1 [kk] = colvec_ff_vec [11][kk];
	  3'd4 : DSP58_B_data_mux1 [kk] = colvec_ff_vec [12][kk];
	  3'd5 : DSP58_B_data_mux1 [kk] = colvec_ff_vec [13][kk];
	  3'd6 : DSP58_B_data_mux1 [kk] = colvec_ff_vec [14][kk];
	  3'd7 : DSP58_B_data_mux1 [kk] = colvec_ff_vec [15][kk];
       endcase
    end
end    

always @(*) begin
    for (kk=0; kk<CASCADE_LEN; kk=kk+1) begin
       case (DSP58_B_data_muxsel[kk][2:0]) // Depending upon muxsel value one of the columns is selected 
	  3'd0 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [16][kk];
	  3'd1 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [17][kk];
	  3'd2 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [18][kk];
	  3'd3 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [19][kk];
	  3'd4 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [20][kk];
	  3'd5 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [21][kk];
	  3'd6 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [22][kk];
	  3'd7 : DSP58_B_data_mux2 [kk] = colvec_ff_vec [23][kk];
       endcase
    end
end    

always @(*) begin
    for (kk=0; kk<CASCADE_LEN; kk=kk+1) begin
       case (DSP58_B_data_muxsel[kk][2:0]) // Depending upon muxsel value one of the columns is selected 
	  3'd0 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [24][kk];
	  3'd1 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [25][kk];
	  3'd2 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [26][kk];
	  3'd3 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [27][kk];
	  3'd4 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [28][kk];
	  3'd5 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [29][kk];
	  3'd6 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [30][kk];
	  3'd7 : DSP58_B_data_mux3 [kk] = colvec_ff_vec [31][kk];
       endcase
    end
end 

// 8:1 mux outputs are flopped

always @(posedge clk) begin
   if (reset) begin
      DSP58_B_data_mux0_ff   <= '{default:0};
      DSP58_B_data_mux1_ff   <= '{default:0};
      DSP58_B_data_mux2_ff   <= '{default:0};
      DSP58_B_data_mux3_ff   <= '{default:0};
      DSP58_B_data_muxsel_ff <= '{default:0}; 
   end
   else begin
      DSP58_B_data_mux0_ff  <= DSP58_B_data_mux0;
      DSP58_B_data_mux1_ff  <= DSP58_B_data_mux1; 
      DSP58_B_data_mux2_ff  <= DSP58_B_data_mux2; 
      DSP58_B_data_mux3_ff  <= DSP58_B_data_mux3;
      DSP58_B_data_muxsel_ff <= DSP58_B_data_muxsel;
   end
end   

// flopped 8:1 mux outputs are muxed using muxsel_ff [4:3]

always @(*) begin
    for (kk=0; kk<CASCADE_LEN; kk=kk+1) begin
       case (DSP58_B_data_muxsel_ff[kk][4:3]) // Depending upon muxsel value one of the columns is selected 
	  2'd0  : DSP58_B_data_mux [kk] = DSP58_B_data_mux0_ff [kk];
	  2'd1  : DSP58_B_data_mux [kk] = DSP58_B_data_mux1_ff [kk];
	  2'd2  : DSP58_B_data_mux [kk] = DSP58_B_data_mux2_ff [kk];
	  2'd3  : DSP58_B_data_mux [kk] = DSP58_B_data_mux3_ff [kk];
       endcase
    end
end 

endmodule
