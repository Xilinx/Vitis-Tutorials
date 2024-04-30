//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module tst_dout_mon_core
#(parameter ROMLATENCY=6, DATABW=32)
(
	input   clk,
	input   srst,
	input   dout_rst,
	input        vld_i,
	input [DATABW-1:0] dat_i,
	output  reg   afifo_re,
	input         afifo_vld,
	input  [DATABW-1:0] afifo_rd,
	output reg       vld_o,
	output reg [6:0]  cnt_o,
	output reg [6:0]  err_o,
	output reg [11:0] idl_o,
	output reg [15:0] lat_o,
	output reg done_o
);

integer j;
localparam input_dly = ROMLATENCY+2;

reg [5:0]  cnt;  // decimate by 64 times
reg [DATABW-1:0] s[0:input_dly];
reg [DATABW-1:0] dut, gold;
reg mismatch;
reg is_err, is_vld, cnt_rst;
reg vld_p0, vld_p1; 
reg [6:0] cnt_s, err_s;

reg is_first;
reg [12:0] icnt;
reg [11:0] idl_s;

always @(posedge clk) mismatch <= |(dut^gold);


//---------------------------------------------
always @(posedge clk) begin

	// fifo read enable
	afifo_re <= (~srst) & vld_i;

	// push the input to the shift reg
	s[input_dly] <= dat_i;
	// shift the registers
	for(j=1; j<=input_dly; j=j+1) begin
		s[j-1] <= s[j];
	end

	// get output
	vld_p0 <= afifo_vld;
	gold   <= afifo_rd;
	dut    <= s[0];
	
	// comparison
	vld_p1 <= vld_p0;

	// compare
	is_err <= vld_p1 & mismatch;
	is_vld <= vld_p1;
	is_first <= (dout_rst|srst)? 1'b1 : vld_p1? 1'b0 : is_first;
	//---------------------------------------------

	// generate a decimated counter signal	
	cnt <= srst? 0 : cnt+1;
	cnt_rst <= &cnt;
	// count idle cycles between every two valids
	icnt <= (is_first|is_vld)? 0: icnt[12]? icnt : (icnt+1);
	
	// is_vld should be aligned with err
	cnt_s <= srst? 0: (cnt_rst&is_vld)?          1 : cnt_rst? 0 : is_vld? (cnt_s+1)          : cnt_s;
	err_s <= srst? 0: (cnt_rst&is_err)?          1 : cnt_rst? 0 : is_err? (err_s+1)          : err_s;
	idl_s <= srst? 0: (cnt_rst&is_vld)?  icnt[11:0]: cnt_rst? 0 : is_vld? (idl_s+icnt[11:0]) : idl_s;
	
	// latch cnt and err for slower clock domain
	vld_o <= srst? 1'b0 : cnt_rst^vld_o;
	cnt_o <= srst? 0: cnt_rst? cnt_s : cnt_o;
	err_o <= srst? 0: cnt_rst? err_s : err_o;
	idl_o <= srst? 0: cnt_rst? idl_s : idl_o;
	//------------------------------------------------------------------------
	// count the length of is_first, which the length from start to 1st output
	//------------------------------------------------------------------------
	lat_o <= (srst|dout_rst)? 0 : is_first? (lat_o+1) :lat_o;

	//-------------------------------
	done_o <= srst? 1'b1 : cnt_rst? icnt[12] : done_o;

end



endmodule
