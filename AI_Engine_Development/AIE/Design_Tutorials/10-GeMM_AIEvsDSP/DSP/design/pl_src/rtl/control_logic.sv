/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// This module generates all the control logic required for -
// 1. Read rows and  columns from BRAMs to DSPs
// 2. Storing row and column read data into flops
// 3. Muxing the data and mux select signal
// 4. Write partial sum to BRAMs 

module control_logic #(

parameter M_LARGE            = 1024,
parameter N_LARGE            = 1024,
parameter M                  = 32,
parameter N                  = 32,
parameter NUM_CASCADE_CHAINS = 32,
parameter CASCADE_LEN        = 32,
parameter OUTPUT_DATA_WIDTH  = 58,
parameter OUTPUT_ADDR_WIDTH  = 10,
parameter ADDR_WIDTH         = 10,
parameter URAM_ADDR_WIDTH    = 12,
parameter URAM_DATA_WIDTH    = 64,
parameter A_DATA_WIDTH       = 16,
parameter B_DATA_WIDTH       = 16
//parameter NUM_COL_URAMS      = 4, //4 sets of 16 URAMs
//parameter NUM_ROW_URAMS      = 4  //4 sets of 16 URAMs

) (

input clk,
input reset,

// ROW URAM read interface
(* DONT_TOUCH = "yes" *) output reg [URAM_ADDR_WIDTH-1:0] row_uram_addrb , //common for all urams
(* DONT_TOUCH = "yes" *) output reg [3:0]                 row_uram_enb   , //common for all urams
                         input      [URAM_DATA_WIDTH-1:0] row_uram_doutb [15:0],

// Col URAM read interface
output     [URAM_ADDR_WIDTH-1:0]                          col_uram_addrb,
output reg [3:0]                                          col_uram_enb  ,
input      [URAM_DATA_WIDTH-1:0]                          col_uram_doutb [15:0],

// A inputs to mux
output reg [A_DATA_WIDTH-1:0]                             mux_A_data0 [CASCADE_LEN-1:0],
output reg [A_DATA_WIDTH-1:0]                             mux_A_data1 [CASCADE_LEN-1:0],

// B inputs to mux
output reg [B_DATA_WIDTH-1:0]                             mux_B_data0 [CASCADE_LEN-1:0],
output reg [B_DATA_WIDTH-1:0]                             mux_B_data1 [CASCADE_LEN-1:0],

//valid signals
output reg                                                mux_A_data_valid ,
output reg                                                mux_B_data_valid ,
input                                                     DSP_op_data_valid,


// Partial Sum Write interface
output reg  [2*NUM_CASCADE_CHAINS-1:0]                    ps_bram_ena,
output reg  [2*NUM_CASCADE_CHAINS-1:0]                    ps_bram_wea,
output reg  [OUTPUT_ADDR_WIDTH-1:0]                       ps_bram_addra [2*NUM_CASCADE_CHAINS-1:0],

//op uram
output reg  [2*NUM_CASCADE_CHAINS-1:0]                    op_uram_ena,
output reg  [2*NUM_CASCADE_CHAINS-1:0]                    op_uram_wea,
output reg  [13:0]                                        op_uram_addra [2*NUM_CASCADE_CHAINS-1:0],

// Partial Sum Read interface to read back partial sum
output reg  [2*NUM_CASCADE_CHAINS-1:0]                    ps_bram_enb,
output reg  [OUTPUT_ADDR_WIDTH-1:0]                       ps_bram_addrb [2*NUM_CASCADE_CHAINS-1:0],

// Control and Status signals

input                                                     start,
output reg                                                add_ps_d2,
output reg                                                add_ps_d2_int1,
output reg                                                write_op_uram,
output reg                                                done,
output reg [31:0]                                         active_clk_count,

// Statistics registers
output reg [4:0]                                          num_mat_done
);


parameter MSB         = $clog2(M_LARGE/M);
parameter M_LARGE_MSB = $clog2(M_LARGE);
integer i, jj, kk, ll;
genvar j,k,l;

// Internal signals

reg                               Start_done;
reg [3:0]                         col_uram_enb_d;
reg [3:0]                         col_uram_enb_d1;
reg [3:0]                         col_uram_enb_d2;
reg [3:0]                         col_uram_enb_d3;
reg [3:0]                         row_uram_enb_d;
reg [3:0]                         row_uram_enb_d1;
reg [3:0]                         row_uram_enb_d2;

reg [2*NUM_CASCADE_CHAINS-1:0]    partial_sum_done; 
reg [2*NUM_CASCADE_CHAINS-1:0]    partial_sum_done_d1; 
reg [2*NUM_CASCADE_CHAINS-1:0]    partial_sum_done_d2; 
reg [4:0]                         muxsel_counter [CASCADE_LEN-1:0];
reg [4:0]                         col_submatrix_counter;
reg [4:0]                         row_submatrix_counter;
reg [4:0]                         row_submatrix_counter_d;
//reg [4:0]                         row_submatrix_counter_int;
reg [4:0]                         col_submatrix_iter_counter;
reg [4:0]                         col_submatrix_iter_counter_d;
reg [3:0]                         row_counter;
reg [URAM_ADDR_WIDTH +1:0]        addr_counter;
reg [URAM_ADDR_WIDTH +1:0]        addr_counter_prev;
reg [URAM_ADDR_WIDTH -3:0]        row_uram_addrb_msb ; //URAM_ADDR_WIDTH-1-4+2
reg [3:0]                         row_uram_addrb_lsb ;

/******************************************************************************/
// Once Row URAM and Col URAMs are populated start input will be driven to 1
// Matrix Multiplication operation starts after that
/******************************************************************************/

always @(posedge clk) begin
  if (reset) begin
     Start_done  <= 1'b0;
  end
  else begin
     Start_done  <= start | Start_done;
  end
end  

/************************************************************************************/
//  Read Columns of Matrix A and drive them clock-by-clock to Port A of DSP58 
/************************************************************************************/
wire [15:0]row_uram_doutb_int[2*NUM_CASCADE_CHAINS-1:0];
wire [15:0]col_uram_doutb_int[2*NUM_CASCADE_CHAINS-1:0];
genvar ii;

for (ii =0; ii < 16; ii= ii+1) begin
  for (j =0; j< 4 ; j =j+1 ) begin
    assign row_uram_doutb_int[4*ii +j] = row_uram_doutb[ii][16*(j+1)-1:16*j];
    assign col_uram_doutb_int[4*ii +j] = col_uram_doutb[ii][16*(j+1)-1:16*j];
  end
end  

for (ii =0; ii < NUM_CASCADE_CHAINS; ii= ii+1) begin
  always @(posedge clk) begin
    if (reset) begin
       mux_A_data0[ii]   <= 'b0;
       mux_A_data1[ii]   <= 'b0;
       mux_B_data0[ii]   <= 'b0;
       mux_B_data1[ii]   <= 'b0;
    end
    else begin
       mux_A_data0[ii]   <= row_uram_doutb_int[ii];
       mux_A_data1[ii]   <= row_uram_doutb_int[ii+NUM_CASCADE_CHAINS];
       mux_B_data0[ii]   <= col_uram_doutb_int[ii];
       mux_B_data1[ii]   <= col_uram_doutb_int[ii+NUM_CASCADE_CHAINS];
    end
  end  
end


reg delay_start_B;
reg delay_stop_B;

always @(posedge clk) begin
  if (reset) begin
    delay_start_B <= 'b0;
    delay_stop_B <= 'b0;
  end
  else begin
    if (row_counter == 'd15) begin
      delay_start_B <= 'b1;
      if (done) begin
        delay_stop_B <= 'b1;
      end
    end
  end
end  

always @(posedge clk) begin
  if (reset) begin
    mux_A_data_valid <= 'b0;
    mux_B_data_valid <= 'b0;
  end
  else begin
    mux_A_data_valid <= |row_uram_enb_d2;
    mux_B_data_valid <= |col_uram_enb_d2;
    //mux_A_data_valid <= |row_uram_enb_d2;
    //mux_B_data_valid <= |col_uram_enb_d2;
  end
end  
reg [URAM_ADDR_WIDTH +1 :0]col_uram_addrb_int; // URAM_ADDR_WIDTH-1+2 
always @(posedge clk) begin
  if (reset) begin
     col_uram_addrb_int <= 'b0;
     col_uram_enb   <= 'b0;
     col_uram_enb_d <= 1'b0;
     col_uram_enb_d1 <= 1'b0;
     col_uram_enb_d2 <= 1'b0;
     col_uram_enb_d3 <= 1'b0;
  end
  else begin
     //if (!col_uram_enb) begin 
     //  col_uram_enb[0] <= delay_start_B  & !delay_stop_B; // B is started after A
     //end 
     col_uram_enb_d  <= col_uram_enb;
     col_uram_enb_d1  <= col_uram_enb_d;
     col_uram_enb_d2  <= col_uram_enb_d1;
     col_uram_enb_d3  <= col_uram_enb_d2;
     if (col_uram_enb) begin
       if (col_uram_addrb_int == (16*M_LARGE*M_LARGE/(M*M))-1'b1) begin
         col_uram_addrb_int <= 'b0; 
       end
       else begin
         col_uram_addrb_int <= col_uram_addrb_int + 1'b1;
       end
     end
     if (delay_stop_B) begin
       col_uram_enb <= 'b0;
     end
     else if (col_uram_addrb_int[URAM_ADDR_WIDTH-1:0] == 12'd4095 & col_uram_addrb_int[URAM_ADDR_WIDTH-1:0] != 16*M_LARGE*N_LARGE/(M*N)-1'b1) begin
       col_uram_enb <= {col_uram_enb[2:0],col_uram_enb[3]};
       //if (col_uram_addrb_int == 16*M_LARGE/M -1'b1) begin 
       //  col_uram_enb <= col_uram_enb; 
       //end
       //else begin
       //  col_uram_enb <= {col_uram_enb[2:0],col_uram_enb[3]};
       //end
     end  
     else if (!col_uram_enb & delay_start_B) begin
       col_uram_enb <= {3'b0,delay_start_B};
     end
  end
end  
assign col_uram_addrb = col_uram_addrb_int [URAM_ADDR_WIDTH-1 :0];

// Row URAM Read starts when Start pulse is received
// Number of locations read = Number of Columns in A matrix = N

wire [URAM_ADDR_WIDTH+1:0] row_uram_addrb_int; //URAM_ADDR_WIDTH -1 +2
always @(posedge clk) begin
  if (reset) begin
     row_uram_addrb_lsb <= 4'b0; // 16 locations in 16 URAMs- so size is 4
     row_uram_enb       <= 'b0;
     row_uram_enb_d     <= 'b0;
     row_uram_enb_d1    <= 'b0;
     row_uram_enb_d2    <= 'b0;
  end
  else begin
     //if(!row_uram_enb) begin 
     //  row_uram_enb    <= Start_done & !done; 
     //end
     row_uram_enb_d     <= row_uram_enb;
     row_uram_enb_d1    <= row_uram_enb_d;
     row_uram_enb_d2    <= row_uram_enb_d1;
     if (row_uram_enb) begin
        row_uram_addrb_lsb <= row_uram_addrb_lsb + 1'b1;
     end
     if (done) begin
       row_uram_enb <= 'b0;
     end
     //else if (row_uram_addrb_int[URAM_ADDR_WIDTH-1:0] == 12'd4095  & (row_submatrix_counter == M_LARGE/M -1'b1) & 
     //          (col_submatrix_counter == (M_LARGE/M - 1'b1)) & (row_counter == N/2 -1'b1)) begin 
     else if (row_uram_addrb_int[URAM_ADDR_WIDTH-1:0] == 12'd4095 & (col_submatrix_iter_counter != col_submatrix_iter_counter_d)) begin 
       row_uram_enb <= {row_uram_enb[2:0],row_uram_enb[3]};
     end  
     else if (!row_uram_enb & Start_done) begin
       row_uram_enb <= {3'b0,Start_done};
     end
  end
end  

// one submatrix consumes 16 locations in 16 urams, so one row of submatrices consumes 16*(M_LARGE/M) locations
// after one row of column submatrices(like B00,B01,B02,..,B0,31) are done, we move to next row submatrix 

always @(posedge clk) begin
  if (reset) begin
     row_uram_addrb_msb <= 9'b0;
  end
  else begin
     if ((addr_counter_prev[MSB+3:0] == (16*(M_LARGE/M)-1'b1)) & (row_uram_addrb_lsb == 4'd15))begin
        row_uram_addrb_msb <= row_uram_addrb_msb + 1'b1;
     end
  end
end  
assign row_uram_addrb_int = {row_uram_addrb_msb,      row_uram_addrb_lsb};
assign row_uram_addrb     = {row_uram_addrb_msb[6:0], row_uram_addrb_lsb};

//col submatrix changes faster. row submatrix stays for 1 iteration in iteration of 32 op cloumns

// when row_counter reaches 31 it implies one submatrix is done.
//since col submatrix changes faster when row_counter is 31, next col_submatrix should be read

always @(posedge clk) begin
   if (reset) begin
       row_counter <= 4'b0;
       addr_counter <= 'b0;
       addr_counter_prev <= 'b0;
   end
   else if (Start_done) begin
       row_counter <= row_counter + 1'b1;
       addr_counter <= addr_counter + 1'b1;
       addr_counter_prev <= addr_counter; 
   end
end

reg [9:0] num_sub_mat_done;

// when col_submatrix_counter reaches 31 or M_LARGE/M, it means one iteration of partial sum op is done 

always @(posedge clk) begin
   if (reset) begin
      col_submatrix_counter <= 5'b0;
      num_sub_mat_done      <= 'b0;
   end
   else if (row_counter == M/2-1'b1) begin // M/2 bcoz we drive 2 rows/columns on one PL clk 
      if (col_submatrix_counter == M_LARGE/M -1'b1) begin
         col_submatrix_counter <= 1'b0; 
      end
      else begin
         col_submatrix_counter <= col_submatrix_counter + 1'b1;
      end
      num_sub_mat_done      <= num_sub_mat_done + 1'b1;
   end
end

// when col_submatrix_counter is 31 or M_LARGE/M, we move to next partial sum iteration i.e., we move to next row_submatrix 

always @(posedge clk) begin
   if (reset) begin
      row_submatrix_counter <= 10'b0;
      //row_submatrix_counter_int <= 10'b0;
   end
   else if ((col_submatrix_counter == (M_LARGE/M - 1'b1)) & (row_counter == N/2 -1'b1)) begin
      if (row_submatrix_counter == (N_LARGE/N - 1'b1)) begin
         row_submatrix_counter <= 5'b0;
      end
      else begin
         row_submatrix_counter <= row_submatrix_counter + 1'b1;
      end	 
      //row_submatrix_counter_int <= row_submatrix_counter_int + 1'b1;
   end
end
always @(posedge clk) begin
   if (reset) begin
      row_submatrix_counter_d <= 5'b0;
      col_submatrix_iter_counter_d <=5'b0;
   end
   else begin
      row_submatrix_counter_d <= row_submatrix_counter;
      col_submatrix_iter_counter_d <=col_submatrix_iter_counter;
   end
end

//when row_submatrix_counter reaches 31 or M_LARGE/M one iteration of output matrix is done i.e., first 32 columns of op matrix are done.
//whe col_submatrix_iter_counter reaches 31 or M_LARGE/M total op matrix is done
always @(posedge clk) begin
   if (reset) begin
      col_submatrix_iter_counter <= 5'b0;
   end
   else if ((col_submatrix_counter == N_LARGE/N - 1'b1) & (row_submatrix_counter == M_LARGE/M -1'b1) & (row_counter == N/2 -1'b1))begin
      col_submatrix_iter_counter <= col_submatrix_iter_counter +1'b1;
   end
end



// Delay by 1 clock to account for Read latency = 2

always @(posedge clk) begin
  if (reset) begin
     col_uram_enb_d   <= 'b0;
  end
  else begin
     col_uram_enb_d     <= col_uram_enb;
  end
end  

reg active_clk_count_en;
reg start_d, start_p;
reg done_d, done_p;

reg done_int;

always @(posedge clk) begin
   if (reset) begin
      done_int <= 1'b0;
   end
   else if ((col_submatrix_iter_counter==M_LARGE/M -1'b1) & (col_submatrix_counter == N_LARGE/N -1'b1) & (row_submatrix_counter == M_LARGE/M -1'b1) & (row_counter == M/2 -1'b1) & ~done) begin
      done_int <= 1'b1;
   end
end

always @(posedge clk) begin
   if (reset) begin
      done <= 1'b0;
   end
   else begin
      done <= done |((M_LARGE=='d32 & N_LARGE=='d32) ? (num_sub_mat_done=='d2) : done_int);
   end
end

always @(posedge clk) begin
   if (reset) begin
      active_clk_count_en <= 1'b0;
      start_d <= 1'b0;
      start_p <= 1'b0;
      done_d <= 1'b0;
      done_p <= 1'b0;
   end
   else begin
      start_d <= start;
      start_p <= start & !start_d;
      done_d <= done;
      done_p <= done & !done_d;
      active_clk_count_en <= start_p | (active_clk_count_en & !done_p); 
   end
end

always @(posedge clk) begin
   if (reset) begin
      active_clk_count <= 32'b0;
   end
   else if (active_clk_count_en) begin
      active_clk_count <= active_clk_count + 1'b1;
   end
end


/*****************************************************************/
//storing result to partial sum and Output BRAMs 
/*****************************************************************/

reg [4:0]iter;
wire op_uram_write_en;
//assign op_uram_write_en = (row_submatrix_counter == (N_LARGE/N - 1));
assign op_uram_write_en = (iter==(M_LARGE/M -1'b1)) | (M_LARGE==32 & N_LARGE==32); 
always @(posedge clk) begin
   if (reset) begin
     iter <= 'b0;
   end
   else begin
     if(op_uram_addra[0][M_LARGE_MSB-2:0] == M_LARGE/2 -2'd2)begin
       iter <= 'b0; 
     end
     else if(ps_bram_addra[0] == M_LARGE/2 -2'd2) begin
       iter <= iter +1'b1;
     end
   end
end   

// Due to the timing differences, when write_op_uram gets generated, PS BRAM of last but one iteration is still going on.
// So we will need to delay Output BRAM writes
always @(posedge clk) begin
   if (reset) begin
        write_op_uram <= 1'b0;
   end
   else begin
        //write_op_uram <= ((op_uram_write_en & (ps_bram_addra[0] == 10'd1023)) |
	//                      (write_op_uram & ~(ps_bram_addra[0] == 10'd1023)));
        write_op_uram <= ((op_uram_write_en & (ps_bram_addra[0] == M_LARGE/2-1'b1)) | 
	                      (write_op_uram & ~(op_uram_addra[0][M_LARGE_MSB-2:0] == M_LARGE/2-2'd2)));

   end
end   

// For the same timing differences, add_ps signal should also be generated in a delayed manner
// Further timing of add_ps should be such that old data from same address should get added to
// new data and should get written to the same address
// To take care of read latency, add_ps is generated two clocks earlier than PS BRAM completion
wire add_ps_en; 
reg add_ps;
reg add_ps_d1;
reg add_ps_d2_d;

assign add_ps_en = (col_submatrix_iter_counter != 5'b0) ;
reg add_ps_d2_int;
//reg add_ps_d2_int1;
reg add_ps_d2_int2;
always @(posedge clk) begin
   if (reset) begin
        add_ps    <= 1'b0;
        add_ps_d1 <= 1'b0;
        add_ps_d2 <= 1'b0;
        add_ps_d2_int <= 1'b0;
        add_ps_d2_int1 <= 1'b0;
        add_ps_d2_int2 <= 1'b0;
   end
   else begin
        //add_ps    <= add_ps_en & ((ps_bram_addra[0] == 10'd1021) | add_ps);
	if (write_op_uram & (op_uram_addra[0][M_LARGE_MSB-2:0] == M_LARGE/2-'d7) ) begin
	    add_ps <= 1'b0;
	end    
	else if (ps_bram_addra[0]== M_LARGE/2-'d6 & ~add_ps_d1) begin
	    add_ps <= 1'b1;
	end    

	add_ps_d1      <= add_ps;
	add_ps_d2_int  <= add_ps_d1;
	add_ps_d2_int1 <= add_ps_d2_int;
	add_ps_d2_int2 <= add_ps_d2_int1;
	add_ps_d2      <= add_ps_d2_int2;
	add_ps_d2_d    <= add_ps_d2;
   end
end   


// Whenever add_ps is 1, we need to read PS BRAM data 

always @(posedge clk) begin
   if (reset) begin
      ps_bram_enb   <= '{default:0};
      ps_bram_addrb <= '{default:0};
   end
   else begin
      for (jj=0; jj<2*NUM_CASCADE_CHAINS; jj=jj+1) begin
         if (add_ps) begin
            ps_bram_enb[jj] <= 1'b1;
	 end
         else begin
            ps_bram_enb[jj] <= 1'b0;
	 end
	 if (ps_bram_enb[jj]) begin
	   if (ps_bram_addrb[jj]==M_LARGE/2-1'b1) begin
              ps_bram_addrb[jj] <= 'b0;
	   end
	   else begin
              ps_bram_addrb[jj] <= ps_bram_addrb[jj] + 1'b1;
	   end   
	 end   
	 else begin
	   ps_bram_addrb[jj]<='b0; 
	 end
      end	 
   end
end


always @(posedge clk) begin
  for (jj=0; jj<2*NUM_CASCADE_CHAINS; jj=jj+1) begin
     if (reset) begin
         partial_sum_done[jj]    <= 1'b0;
         partial_sum_done_d1[jj] <= 1'b0;
         partial_sum_done_d2[jj] <= 1'b0;
     end
     else begin
        //#1 partial_sum_done[jj]    <=  DSP_op_data_valid; 
        partial_sum_done[jj]    <= DSP_op_data_valid; 
	partial_sum_done_d1[jj] <= partial_sum_done[jj];
	partial_sum_done_d2[jj] <= partial_sum_done_d1[jj];
     end
  end
end  

// Logic to write cascade chain outputs is retained as is
reg  [2*NUM_CASCADE_CHAINS-1:0]  ps_bram_ena_int;
reg  [2*NUM_CASCADE_CHAINS-1:0]  ps_bram_ena_int_d;
reg  [2*NUM_CASCADE_CHAINS-1:0]  ps_bram_ena_int_d1;
reg  [OUTPUT_ADDR_WIDTH-1:0]   ps_bram_addra_int   [2*NUM_CASCADE_CHAINS-1:0];
reg  [OUTPUT_ADDR_WIDTH-1:0]   ps_bram_addra_int_d [2*NUM_CASCADE_CHAINS-1:0];
reg  [OUTPUT_ADDR_WIDTH-1:0]   op_uram_addra_int   [2*NUM_CASCADE_CHAINS-1:0];

always @(posedge clk) begin
   for (jj=0; jj<2*NUM_CASCADE_CHAINS; jj=jj+1) begin
     if (reset) begin
         ps_bram_ena_int [jj]  <= 1'b0;
         ps_bram_ena_int_d [jj]<= 1'b0;
     end
     else begin
         ps_bram_ena_int [jj]   <= partial_sum_done_d2[jj];
         ps_bram_ena_int_d [jj] <= ps_bram_ena_int[jj];
         ps_bram_ena_int_d1 [jj] <= ps_bram_ena_int_d[jj];
     end	 
  end
end

//////////////////////////////////////////////////////////////////////////////////////////
// For timing violation fixes, making ps_bram_addra, ps_bram_ena and ps_bram_wea sequential
///////////////////////////////////////////////////////////////////////////////////////////

always @(posedge clk) begin
   if (reset) begin
      ps_bram_ena   <= 'b0;
      ps_bram_wea   <= 'b0;
      ps_bram_addra <= '{default:0}; 
   end
   else begin
      ps_bram_ena   <= (write_op_uram & add_ps_d2_int1) ? 'b0  : (add_ps_d2 & add_ps_d2_int1 ? ps_bram_ena_int_d  : ps_bram_ena_int );
      ps_bram_wea   <= (write_op_uram & add_ps_d2_int1) ? 'b0  : (add_ps_d2 & add_ps_d2_int1 ? ps_bram_ena_int_d  : ps_bram_ena_int );
      for (jj=0; jj<2*NUM_CASCADE_CHAINS; jj=jj+1) begin
        ps_bram_addra[jj] <= (write_op_uram & add_ps_d2_int) ? 'b0 : (add_ps_d2 & add_ps_d2_int1 ? ps_bram_addra_int_d[jj]: ps_bram_addra_int[jj]);
      end
      
   end
end   

always @(posedge clk) begin
  if (reset) begin
     ps_bram_addra_int   <= '{default:0};
     ps_bram_addra_int_d <= '{default:0};
  end
  else begin
     for (kk=0; kk<2*NUM_CASCADE_CHAINS; kk=kk+1) begin
        if (ps_bram_ena_int[kk] & ~(write_op_uram & add_ps_d2_int1)) begin
	   if (ps_bram_addra_int[kk]==M_LARGE/2-1'b1) begin
              ps_bram_addra_int[kk]   <= 1'b0; 
	   end
	   else begin
              ps_bram_addra_int[kk]   <= ps_bram_addra_int[kk] + 1'b1;
	   end
        end
	else begin
           ps_bram_addra_int[kk]   <= 1'b0; 
	end
        ps_bram_addra_int_d[kk] <= ps_bram_addra_int[kk]; 
     end   
  end
end

always @(posedge clk) begin
  if (reset) begin
      op_uram_ena   <= 'b0;
      op_uram_wea   <= 'b0;
      op_uram_addra <= '{default:0}; 
  end
  else begin
     op_uram_ena   <= (write_op_uram & (add_ps_d2_d | (M_LARGE == 'd32 & N_LARGE == 'd32))) ? ps_bram_ena_int_d1 : 'b0;
     op_uram_wea   <= (write_op_uram & (add_ps_d2_d | (M_LARGE == 'd32 & N_LARGE == 'd32))) ? ps_bram_ena_int_d1 : 'b0;
     for (kk=0; kk<2*NUM_CASCADE_CHAINS; kk=kk+1) begin
        if (op_uram_ena[kk]) begin
           op_uram_addra[kk]   <= op_uram_addra[kk] + 1'b1;
        end
     end   
  end
end

always @(posedge clk) begin
   if (reset) begin
      num_mat_done <= 'b0;
   end
   else if (op_uram_ena[0] & (op_uram_addra[0][3:0]=='d15)) begin
      num_mat_done <= num_mat_done + 1'b1;
   end
end 


endmodule
