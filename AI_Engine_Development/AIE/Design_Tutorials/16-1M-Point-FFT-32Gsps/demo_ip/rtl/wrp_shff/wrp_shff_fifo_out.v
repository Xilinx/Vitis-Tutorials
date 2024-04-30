//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module wrp_shff_fifo_out
#(parameter uram_delay=2)
(
	input         clk,
	input         srst,
	//--------------------------- input from shuffle network
	input         buf_empty,
	output  reg   buf_rdone,
	output  reg   [13:0] buf_ra,
	input         [63:0] buf_rd,
	//--------------------------- interface Sync FIFO
	input       fifo_af,
	output  reg fifo_we,
	output  reg [63:0] fifo_wd
);


localparam ramlatency = uram_delay;

// every read/write session is 4x64 bits
// 64-depth memory is divided into 16 blocks
reg [9:0] ra_msb;
reg [3:0] ra_lsb;
reg [ramlatency+1:0] re_shft;


// every read/write session is 16 words
// use msb to indicate reading status
reg [4:0] rcnt;
reg [10:0] ra_cnt;

// flag to indicate whether any data in the buffer
reg rd_start, rd_start_d1, rd_done;

reg buf_not_empty, fifo_not_full;

reg [1:0] cst;


localparam stat_rst  = 2'b11;
localparam stat_idle = 2'b00;
localparam stat_rd   = 2'b10;
localparam stat_rcnt = 2'b01;



//------------------------------------------
// Read State Machine
//------------------------------------------
always@(posedge clk)
begin
  if(srst==1'b1) cst  <= stat_rst;
  else begin  
	   case(cst)
	    stat_idle:  cst <=  (buf_not_empty&fifo_not_full)?  stat_rd : cst;
		stat_rd:	cst <=  stat_rcnt;
		stat_rcnt:	cst <=  rd_done? stat_idle : cst;
		default:    cst <=  stat_idle;
	   endcase
	end
end


//----------------------------------------
// memory read interface
//----------------------------------------
always @(posedge clk) begin
	
	// generate a read counter. use it for lsb of read address
	rcnt <= srst? 0 : rd_start? 16 : rcnt[4]? rcnt+1 : rcnt;
	
	// latch read address
	ra_cnt <= srst? 0 : rd_done? (ra_cnt+1) : ra_cnt;
	ra_msb <= srst? 0 : ~rd_start_d1? ra_msb : ra_cnt[10]? ra_cnt[9:0] : {ra_cnt[4:0], ra_cnt[9:5]};
	ra_lsb <= rcnt[3:0];
	
	// generate read address
	buf_ra <= {ra_msb, ra_lsb};

	// for valid signal generation
	re_shft    <= srst? 0 : {rcnt[4], re_shft[ramlatency+1:1]};

	// generate a done signal
	rd_start <= (cst==stat_rd)? 1'b1 : 1'b0;
	rd_start_d1 <= rd_start;
	
	rd_done  <= (rcnt[3:0]==11)? 1'b1 : 1'b0;
	
	buf_rdone <= rd_start;
	
	// output
	fifo_we      <= re_shft[0];
	fifo_wd      <= buf_rd;
	
	// register fifo almost full signal
	buf_not_empty <= ~buf_empty;
	fifo_not_full <= ~fifo_af;
	
end


		

endmodule
