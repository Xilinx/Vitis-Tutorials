/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// This module generates control signals needed for Moving Matrix B data from
// URAMs to ping/pong BRAMs, and also to write the data from output BRAMs to
// URAMs
// This module will require a start signal from PS app, which indicates that
// PS App is done with writing the matrices to URAMs

module controller #(

parameter COL_URAM_ADDR_WIDTH = 12,
parameter COL_URAM_DATA_WIDTH = 512,
parameter NUM_BRAM = 8,
parameter BRAM_ADDR_WIDTH = 9,
parameter OP_URAM_ADDR_WIDTH = 15,
parameter OP_BRAM_DATA_WIDTH = 512,
parameter OP_URAM_DATA_WIDTH = 512,
parameter NUM_CASCADE_CHAINS = 32,
parameter MAT_DATA_BASE_ADDR = 32'h0,
parameter BRAM_DATA_WIDTH = 64,
parameter M_LARGE=1024,
parameter M=32,
parameter N_LARGE=1024,
parameter N=32


) (

input                                 clk,
input                                 reset,

// Control signals from DUT
input                                 ping_pong_col_bram_sel,
input                                 write_op_bram,
output reg                            ping_done,
input                                 ps_app_done,
output wire                           start,

// Col URAM Port B read interface
// 8 Col URAMs will be read simultaneously, and written to 8 BRAMs
// So only 8 Col URAM enb are generated
output reg [7:0]                      col_uram_enb,   // 1 for group of 8 URAMs 
output reg [COL_URAM_ADDR_WIDTH-1:0]  col_uram_addrb, 
input [COL_URAM_DATA_WIDTH-1:0]       col_uram_doutb,

// Col BRAM Port A write interface
output reg [NUM_BRAM-1:0]             col_bram_ena,
output wire [NUM_BRAM-1:0]            col_bram_wea,
output reg [BRAM_ADDR_WIDTH-1:0]      col_bram_addra,
output reg [COL_URAM_DATA_WIDTH-1:0]  col_bram_dina,

// Output BRAM Read Signals
// 16-bits are read from 32 BRAMs and written to 8 64-bit wide URAMs 
output reg [10:0]                     op_bram_addrb,
output reg [NUM_CASCADE_CHAINS-1:0]   op_bram_enb,
input      [OP_BRAM_DATA_WIDTH-1:0]   op_bram_doutb,

// Output URAM write signals
// 8 Output URAMs will be written simultaneously
// So only 8 output URAM ena are generated
output reg [63:0]                     op_uram_ena,  // 1 for group of 8 URAMs
output reg [63:0]                     op_uram_wea,  // 1 for group of 8 URAMs
output reg [14:0]                     op_uram_addra,
output reg [OP_URAM_DATA_WIDTH-1:0]   op_uram_dina,

output reg                            read_op_uram,
// Statistics registers
output reg [4:0]                             num_mat_done
);
always @(posedge clk) begin
   if (reset) begin
      num_mat_done <= 'b0;
   end
   else if (op_uram_ena[0] & (op_uram_addra[4:0]=='d31)) begin
      num_mat_done <= num_mat_done + 1'b1;
   end
end 

// FSM parameters
localparam MAT_B_INIT_READ     = 1'b0;
localparam MAT_B_PERIODIC_READ = 1'b1;

// Internal Signals
reg state;
reg mat_B_init_rd_req_int, mat_B_init_rd_req_int2, mat_B_init_rd_req;
reg ping_pong_col_bram_sel_d;
reg mat_B_periodic_rd_req;
reg read_counter_en;
reg [9:0] read_counter;
reg [31:0] rd_beat_count;
reg [31:0] rd_beat_count_d;
reg [31:0] rd_beat_count_d1;
reg read_op_bram;
reg [31:0] op_bram_read_count;
reg write_op_bram_d; 
reg wr_req;

reg [BRAM_ADDR_WIDTH-1:0]      col_bram_addra_int;
reg [BRAM_ADDR_WIDTH-1:0]      col_bram_addra_int1;
reg [BRAM_ADDR_WIDTH-1:0]      col_bram_addra_int2;
reg [BRAM_ADDR_WIDTH-1:0]      col_bram_addra_int3;
reg [NUM_BRAM-1:0]             col_bram_ena_int;
reg [NUM_BRAM-1:0]             col_bram_ena_int1;
reg [NUM_BRAM-1:0]             col_bram_ena_int2;
reg [NUM_BRAM-1:0]             col_bram_ena_int3;
// When ping/pong Col BRAMs are initialized, matrix mutliplication 
// can be started. This is indicated by driving the start
// signal to 1
assign start = (state == MAT_B_PERIODIC_READ);

// Read State Machine is coded below

always @(posedge clk) begin
   if (reset) begin
      state <= MAT_B_INIT_READ; 
   end   
   else begin
      case (state) 
	MAT_B_INIT_READ :
	begin
	   //if (rd_beat_count == 4095) 
	   if (rd_beat_count_d1 == 1023) 
	   begin
	      state <= MAT_B_PERIODIC_READ;
	   end
	 end	
         MAT_B_PERIODIC_READ : 
	 begin
	    state <= MAT_B_PERIODIC_READ;
	 end   
	 default:
	    state <= MAT_B_INIT_READ;
      endcase
    end
end

// Generate Read Requests

//case 1 - Martix B Initial Reads

always @(posedge clk) begin
   if (reset) begin
       mat_B_init_rd_req_int  <= 1'b0;
       mat_B_init_rd_req_int2 <= 1'b0;
       mat_B_init_rd_req      <= 1'b0;
   end
   else begin
       mat_B_init_rd_req_int  <= ps_app_done; 
       mat_B_init_rd_req_int2 <= mat_B_init_rd_req_int;
       mat_B_init_rd_req      <= mat_B_init_rd_req_int & ~mat_B_init_rd_req_int2;
   end
end

//Case 2 - Matrix B Periodic Reads
// mat_B_periodic_rd_req is generated whenever ping_pong_col_bram_sel toggles during MAT_B_PERIODIC_READ state
always @(posedge clk) begin
   if (reset) begin
       ping_pong_col_bram_sel_d      <= 1'b0;
       mat_B_periodic_rd_req         <= 1'b0;
   end
   else begin
       ping_pong_col_bram_sel_d  <= ping_pong_col_bram_sel;
   //mat_B_periodic_rd_req     <= ((state == MAT_B_PERIODIC_READ) & (ping_pong_col_bram_sel & ~ping_pong_col_bram_sel_d));
   //mat_B_periodic_rd_req     <= ((state == MAT_B_PERIODIC_READ) & (~ping_pong_col_bram_sel & ping_pong_col_bram_sel_d)); // only 1 to 0 transition                                                                                                                                //of ping_pong_col_bram_sel 
                                                                                                                           //is captured
       mat_B_periodic_rd_req     <= ((state == MAT_B_PERIODIC_READ) & ((~ping_pong_col_bram_sel & ping_pong_col_bram_sel_d)|(ping_pong_col_bram_sel & ~ping_pong_col_bram_sel_d)));
   end
end

// When mat_B_init_rd_req is generated, we need to read 1K addresses from Col
// URAMs, and write to Ping/Pong Col BRAMs. After first 512 are done,
// ping_done output should be generated
// When mat_B_periodic_rd_req is genrated, we need to read 512 addresses from
// Col URAMs
// Col URAM address should always be incremental
// Col BRAM address will rotate from 0 to 511

// read counter is incremented to keep cound of number of data items
// transfered from URAM to BRAM
always @(posedge clk) begin
   if (reset) begin
      read_counter_en <= 1'b0;
   end
   else if (state  == MAT_B_INIT_READ) begin
      read_counter_en <= (mat_B_init_rd_req | (read_counter_en & (read_counter != 1023)));
   end
   else if (state == MAT_B_PERIODIC_READ) begin
      read_counter_en <= (mat_B_periodic_rd_req | (read_counter_en & (read_counter != 511)));
   end
   else begin
      read_counter_en <= 1'b0;
   end
end   

always @(posedge clk) begin
   if (reset) begin
      read_counter <= 10'b0;
   end
   else begin
      read_counter <= read_counter_en ? read_counter + 1'b1 : 10'b0;
   end
end   

always @(posedge clk) begin
    if (reset) begin
       col_uram_enb   <= 8'h01;
       col_uram_addrb <= 'b0;
       //col_bram_ena      <= 8'h01;
       col_bram_ena        <= 8'h00;
       col_bram_ena_int       <= 8'h00;
       col_bram_ena_int1       <= 8'h00;
       col_bram_ena_int2       <= 8'h00;
       col_bram_addra      <= 'b0; 
       col_bram_addra_int  <= 'b0; 
       col_bram_addra_int1 <= 'b0; 
       col_bram_addra_int2 <= 'b0; 
       col_bram_addra_int3 <= 'b0; 
    end
    else if (read_counter_en) begin
       col_uram_addrb      <= col_uram_addrb + 1'b1;
       col_bram_addra_int  <= col_bram_addra_int + 1'b1;
       col_bram_addra_int1 <= col_bram_addra_int;
       col_bram_addra_int2 <= col_bram_addra_int1;
       col_bram_addra_int3 <= col_bram_addra_int2;
       col_bram_addra      <= col_bram_addra_int3;
       col_bram_ena_int    <= 8'hff;
       col_bram_ena_int1   <= col_bram_ena_int;
       col_bram_ena_int2   <= col_bram_ena_int1;
       col_bram_ena        <= col_bram_ena_int2;
       if (col_uram_addrb == 12'd4095) begin
          col_uram_enb <= {col_uram_enb[6:0], col_uram_enb[7]};
       end	  
       //if (col_bram_addra == 9'd511) begin
       //   col_bram_ena <= {col_bram_ena[6:0], col_bram_ena[7]};
       //end	  
   end
   else begin 
       col_bram_ena_int    <= 8'b0;
       col_bram_ena_int1   <= col_bram_ena_int;
       col_bram_ena_int2   <= col_bram_ena_int1;
       col_bram_ena        <= col_bram_ena_int2;
       col_bram_addra_int  <= 'b0;
       col_bram_addra_int1 <= col_bram_addra_int;
       col_bram_addra_int2 <= col_bram_addra_int1;
       col_bram_addra_int3 <= col_bram_addra_int2;
       col_bram_addra      <= col_bram_addra_int3;
   end
end  

assign col_bram_wea = col_bram_ena;

//URAM data is flopped to drive BRAM data

always @(posedge clk) begin
   if (reset) begin
      col_bram_dina <= 'b0;
   end
   else begin
      col_bram_dina <= col_uram_doutb;
   end
end   

// count number of read bursts to determine end of various states
// When 512 bursts are done, MAT_A_READ is completed
// When 16 bursts are done, MAT_B_INIT_READ is completed 
// Looks like DDR AXI module is not driving tlast signal. So instead of
// counting number of bursts, number of beats are counted, and when that
// count reaches 128K, that indicates end of Matrix A


always @(posedge clk) begin
   if (reset) begin
	 rd_beat_count_d  <= 1'b0;
	 rd_beat_count_d1 <= 1'b0;
   end
   else begin
	 rd_beat_count_d  <= rd_beat_count;
	 rd_beat_count_d1 <= rd_beat_count_d;
   end
end

always @(posedge clk) begin
   if (reset) begin
         rd_beat_count <= 9'b0;
	 ping_done <= 1'b0;
   end
   else if (read_counter_en) begin
      if (rd_beat_count == 131071) begin 
 	  rd_beat_count <= 32'b0;
      end
      else begin
          rd_beat_count <= rd_beat_count + 1'b1;
      end	 
      //if (rd_beat_count == 4095) begin
      if (rd_beat_count_d1 == 1023) begin
	 rd_beat_count <= 9'b0;
      end   
      else begin
         //if (rd_beat_count == 2047) begin
         if (rd_beat_count_d1 == 512) begin
	    ping_done <= 1'b1;
	 end
	 else begin
	    ping_done <= 1'b0;
	 end  
	 rd_beat_count <= rd_beat_count + 1'b1;
       end	  
   end   
end   

///////////////////////////////////////////////////////
// Reading Output BRAM and writing to OP URAM
///////////////////////////////////////////////////////

// Falling edge of write_op_bram signal indicates that 1K*16-bit data is
// available in 32 OP BRAMs. This info is used to generate write request

always @(posedge clk) begin
   if (reset) begin
     write_op_bram_d <= 1'b0;
     wr_req          <= 1'b0;
   end
   else begin
      write_op_bram_d <= write_op_bram;
      wr_req          <= write_op_bram_d & ~write_op_bram;
   end
end   
reg read_op_uram_int;
reg read_op_uram_int1;
reg read_op_uram_int2;
reg read_op_uram_int3;
reg read_op_uram_int4;
reg read_op_uram_int5;
reg read_op_uram_int6;
reg read_op_bram_d;
always @(posedge clk) begin
   if (reset) begin
      read_op_bram       <= 1'b0;
      read_op_bram_d     <= 1'b0;
      read_op_uram_int   <= 1'b0;
      read_op_uram_int1  <= 1'b0;
      read_op_uram_int2  <= 1'b0;
      read_op_uram_int3  <= 1'b0;
      read_op_uram_int4  <= 1'b0;
      read_op_uram_int5  <= 1'b0;
      read_op_uram_int6  <= 1'b0;
      op_bram_read_count <= 32'b0;
      read_op_uram       <= 1'b0;
   end
   else begin
      //read_op_bram <= wr_req | (read_op_bram & ~(op_bram_read_count == 1023)); 
      read_op_bram <= wr_req | (read_op_bram & ~(op_bram_read_count == M_LARGE -1'b1)); 
      read_op_bram_d <= read_op_bram | read_op_bram_d;  // Level Signal 
      read_op_uram_int <= read_op_bram_d; 
      read_op_uram_int1 <= read_op_uram_int; 
      read_op_uram_int2 <= read_op_uram_int1; 
      read_op_uram_int3 <= read_op_uram_int2; 
      read_op_uram_int4 <= read_op_uram_int3; 
      read_op_uram_int5 <= read_op_uram_int4; 
      read_op_uram_int6 <= read_op_uram_int5; 
      read_op_uram      <= read_op_uram_int6;
      if (read_op_bram) begin
         op_bram_read_count <= op_bram_read_count + 1'b1;
      end
      else begin
         op_bram_read_count <= 32'b0;
      end
   end
end

always @(posedge clk) begin
   if (reset) begin
     op_bram_addrb <= 11'b0;
     op_bram_enb   <= 32'h0; 
   end
   else if (read_op_bram) begin
        op_bram_enb <= 32'hffffffff; 
	if (op_bram_enb[0]) begin
              op_bram_addrb <= op_bram_addrb + 1'b1;
	end
    end
    else begin
       op_bram_enb <= 32'b0;
       op_bram_addrb <= 'b0;
    end   
end

reg op_bram_enb0_d1;
reg op_bram_enb0_d2;
reg op_bram_enb0_d3;
reg op_bram_enb0_d4;

always @(posedge clk) begin
   if (reset) begin
       op_bram_enb0_d1 <= 1'b0;
       op_bram_enb0_d2 <= 1'b0;
       op_bram_enb0_d3 <= 1'b0;
       op_bram_enb0_d4 <= 1'b0;
   end
   else begin
       op_bram_enb0_d1 <= op_bram_enb[0]; 
       op_bram_enb0_d2 <= op_bram_enb0_d1;
       op_bram_enb0_d3 <= op_bram_enb0_d2;
       op_bram_enb0_d4 <= op_bram_enb0_d3;
   end
end   

// 32 OP BRAMs are read parallelly, each driving 16-bit. These are written to
// 32 OP URAMs each taking 16-bit. OP URAMs are configured as 16K*16, and each
// group of 32 OP URAMs will store half of output Matrix
// 32K OP URAM addr counter is implemented, and MSB of the counter determines
// whether upper or lower half will get written

always @(posedge clk) begin
    if (reset) begin
       op_uram_ena <= 64'b0;
       op_uram_wea <= 64'b0;
       op_uram_addra <= 'b0;
       op_uram_dina <= 'b0;
    end
    else begin
       op_uram_ena <= (op_uram_addra[14] == 1'b0)  ? {32'b0, {32{op_bram_enb0_d3}}} : {{32{op_bram_enb0_d3}}, 32'b0};
       op_uram_wea <= (op_uram_addra[14] == 1'b0)  ? {32'b0, {32{op_bram_enb0_d3}}} : {{32{op_bram_enb0_d3}}, 32'b0};
       if (op_uram_ena) begin
          op_uram_addra <= op_uram_addra + 1'b1;
       end
       op_uram_dina <= op_bram_doutb;
    end
end    

endmodule

