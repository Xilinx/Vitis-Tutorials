/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// DSP_data_controller has a mux that works on DSP clk - mux select works on DSP clk
// 2:1 mux. It takes two inputs (two rows/columns of submatrix data) and one output goes to FIXGEMM
// DSP_data_controller also takes outputs from FIXGEMM on 2 consecutive DSP cycles and gives one op on PL clk
module DSP_data_controller #(

parameter NUM_CASCADE_CHAINS = 32

) (
input clk, //DSP clk
input rst,


//mux inputs, outputs
input      [15:0]A_data0    [NUM_CASCADE_CHAINS-1:0],
input      [15:0]A_data1    [NUM_CASCADE_CHAINS-1:0],
input      [15:0]B_data0    [NUM_CASCADE_CHAINS-1:0],
input      [15:0]B_data1    [NUM_CASCADE_CHAINS-1:0],
output reg [15:0]DSP_A_data [NUM_CASCADE_CHAINS-1:0],
output reg [15:0]DSP_B_data [NUM_CASCADE_CHAINS-1:0],

input            mux_A_data_valid,
input            mux_B_data_valid,
output reg       DSP_A_data_valid,
output reg       DSP_B_data_valid,

input            DSP_op_data_valid,

input      [39:0]dsp_op          [NUM_CASCADE_CHAINS-1:0],
output reg [39:0]ps_bram_dina    [2*NUM_CASCADE_CHAINS-1:0]
);

reg         mux_sel;
reg         even_odd;
reg [39:0]  ps_bram_dina_int     [NUM_CASCADE_CHAINS-1:0];
reg [39:0]  ps_bram_dina_int_d   [NUM_CASCADE_CHAINS-1:0];
reg [39:0]  ps_bram_dina_res     [2*NUM_CASCADE_CHAINS-1:0];
reg         DSP_op_data_valid_done;
reg         DSP_op_data_valid_done_int;
reg         mux_A_data_valid_done_int;
reg         mux_A_data_valid_done;
reg         mux_B_data_valid_done_int;
reg         mux_B_data_valid_done;
reg         DSP_A_data_valid_int;
reg         DSP_B_data_valid_int;
reg         DSP_A_data_valid_int1;
reg         DSP_B_data_valid_int1;
reg         DSP_A_data_valid_int2;
reg         DSP_B_data_valid_int2;
reg         DSP_A_data_valid_int3;
reg         DSP_B_data_valid_int3;

reg [15:0]  A_data0_d [NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data0_d [NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data0_d1[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data0_d1[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data0_d2[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data0_d2[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data0_d3[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data0_d3[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data1_d [NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data1_d [NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data1_d1[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data1_d1[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data1_d2[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data1_d2[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  A_data1_d3[NUM_CASCADE_CHAINS-1:0];
reg [15:0]  B_data1_d3[NUM_CASCADE_CHAINS-1:0];
reg rst_d;

always @(posedge clk) begin
  if (rst) begin
    rst_d <= 1'b1;
  end  
  else begin 
    rst_d <= rst;
  end  
end  
always @(posedge clk) begin
  if (rst) begin
  //if (rst_d) begin
    DSP_A_data_valid  <= 'b0;
    DSP_B_data_valid  <= 'b0;
    DSP_A_data_valid_int  <= 'b0;
    DSP_B_data_valid_int  <= 'b0;
    DSP_A_data_valid_int1  <= 'b0;
    DSP_B_data_valid_int1  <= 'b0;
    DSP_A_data_valid_int2  <= 'b0;
    DSP_B_data_valid_int2  <= 'b0;
    DSP_A_data_valid_int3  <= 'b0;
    DSP_B_data_valid_int3  <= 'b0;

    A_data0_d          <= '{default:0};
    B_data0_d          <= '{default:0};
    A_data0_d1         <= '{default:0};
    B_data0_d1         <= '{default:0};
    A_data0_d2         <= '{default:0};
    B_data0_d2         <= '{default:0};
    A_data0_d3         <= '{default:0};
    B_data0_d3         <= '{default:0};
    
    A_data1_d          <= '{default:0};
    B_data1_d          <= '{default:0};
    A_data1_d1         <= '{default:0};
    B_data1_d1         <= '{default:0};
    A_data1_d2         <= '{default:0};
    B_data1_d2         <= '{default:0};
    A_data1_d3         <= '{default:0};
    B_data1_d3         <= '{default:0};
  end  
  else begin 
    //DSP_A_data_valid  <= mux_A_data_valid & mux_A_data_valid_done;
    //DSP_B_data_valid  <= mux_B_data_valid & mux_B_data_valid_done;
    DSP_A_data_valid_int  <= mux_A_data_valid; 
    DSP_B_data_valid_int  <= mux_B_data_valid; 
    DSP_A_data_valid_int1 <= DSP_A_data_valid_int; 
    DSP_B_data_valid_int1 <= DSP_B_data_valid_int; 
    DSP_A_data_valid_int2 <= DSP_A_data_valid_int1; 
    DSP_B_data_valid_int2 <= DSP_B_data_valid_int1; 
    DSP_A_data_valid_int3 <= DSP_A_data_valid_int2; 
    DSP_B_data_valid_int3 <= DSP_B_data_valid_int2; 
    DSP_A_data_valid      <= DSP_A_data_valid_int3; 
    DSP_B_data_valid      <= DSP_B_data_valid_int3; 

    A_data0_d         <= A_data0;
    B_data0_d         <= B_data0;
    A_data0_d1        <= A_data0_d;
    B_data0_d1        <= B_data0_d;
    A_data0_d2        <= A_data0_d1;
    B_data0_d2        <= B_data0_d1;
    A_data0_d3        <= A_data0_d2;
    B_data0_d3        <= B_data0_d2;

    A_data1_d         <= A_data1;
    B_data1_d         <= B_data1;
    A_data1_d1        <= A_data1_d;
    B_data1_d1        <= B_data1_d;
    A_data1_d2        <= A_data1_d1;
    B_data1_d2        <= B_data1_d1;
    A_data1_d3        <= A_data1_d2;
    B_data1_d3        <= B_data1_d2;
  end  
end
always @(posedge clk) begin
  if (rst) begin
  //if (rst_d) begin
    mux_sel <= 'b0;
  end
  else if (mux_A_data_valid_done | mux_B_data_valid_done) begin 
    mux_sel <= ~mux_sel;
  end  
end

//assign DSP_data = mux_sel ? mat_data0 : mat_data1;

always @(posedge clk) begin
  if (rst) begin
  //if (rst_d) begin
    DSP_A_data <= '{default: 'b0}; 
    DSP_B_data <= '{default: 'b0}; 
  end  
  else begin 
    //DSP_A_data <= mux_sel ?  A_data1_d1 : A_data0_d1 ;
    //DSP_B_data <= mux_sel ?  B_data1_d1 : B_data0_d1 ;
    DSP_A_data <= mux_sel ?  A_data1_d3 : A_data0_d3 ;
    DSP_B_data <= mux_sel ?  B_data1_d3 : B_data0_d3 ;
  end  
end


always @(posedge clk) begin
  if (rst) begin
  //if (rst_d) begin
     DSP_op_data_valid_done      <= 1'b0;
     DSP_op_data_valid_done_int  <= 1'b0;
     mux_A_data_valid_done       <= 1'b0;
     mux_B_data_valid_done       <= 1'b0;
     mux_A_data_valid_done_int   <= 1'b0;
     mux_B_data_valid_done_int   <= 1'b0;
  end
  else begin
     DSP_op_data_valid_done  <= DSP_op_data_valid | DSP_op_data_valid_done;
     //DSP_op_data_valid_done  <=  DSP_op_data_valid_done_int;
     //if (mux_A_data_valid ) begin 
     if (DSP_A_data_valid_int1) begin 
       mux_A_data_valid_done_int  <= 1'b1; 
       mux_A_data_valid_done      <= mux_A_data_valid_done_int; 
     end 
     //if (mux_B_data_valid ) begin 
     if (DSP_B_data_valid_int1) begin 
       mux_B_data_valid_done_int  <= 1'b1; 
       mux_B_data_valid_done      <= mux_B_data_valid_done_int; 
     end 
  end
end 

always @(posedge clk) begin
  if (rst) begin
  //if (rst_d) begin
    even_odd <= 'b0;
  end  
  else begin 
    even_odd <= (~even_odd) & (DSP_op_data_valid_done);
  end  
end

always @(posedge clk) begin
  if (rst) begin
  //if (rst_d) begin
    ps_bram_dina       <= '{default: 'b0};
  end  
  else begin 
    ps_bram_dina       <= ps_bram_dina_res; 
  end  
end

genvar i;

for (i=0; i < NUM_CASCADE_CHAINS; i= i+1) begin
  always @(posedge clk) begin
    if (rst) begin
    //if (rst_d) begin
      ps_bram_dina_res[i]<= 'b0;
      ps_bram_dina_res[i +NUM_CASCADE_CHAINS]<= 'b0;
    end
    else begin 
      if(even_odd) begin
        ps_bram_dina_res[i]                      <= ps_bram_dina_int_d[i];
	ps_bram_dina_res[i + NUM_CASCADE_CHAINS] <= ps_bram_dina_int[i];
      end
    end  
  end  
end
for (i=0; i < NUM_CASCADE_CHAINS; i= i+1) begin
  always @(posedge clk) begin
    if (rst) begin
    //if (rst_d) begin
      ps_bram_dina_int[i]   <= 'b0;
      ps_bram_dina_int_d[i] <= 'b0;
    end
    else begin 
      ps_bram_dina_int[i]    <= dsp_op[i];
      ps_bram_dina_int_d[i]  <= ps_bram_dina_int[i];
      //if(mux_sel) begin
      //  ps_bram_dina[i] <= dsp_op[i]; 
      //end
      //else begin
      //  ps_bram_dina[i+ NUM_CASCADE_CHAINS] <= dsp_op[i]; 
      //end
    end  
  end  
end

endmodule
    
