//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module tst_dout_xclk_results
(
	input   clk,
	input   test_en,
	input         vld_i, // 500MHz decimated by 64
	input   [6:0] cnt_i,	
	input   [6:0] err_i,
	input  [11:0] idl_i,
	input  [15:0] lat_i,
	input         done_i,
	output reg [47:0] smp_cnt_o,
	output reg [47:0] err_cnt_o,
	output reg [47:0] idl_cnt_o,
	output reg [15:0] lat_cnt_o,
	output done_o
);


wire vld_s3;
reg  vld_s4, vld_s;
wire [6:0]  cnt_s1, err_s1; //7+7+12+16=42 bits
wire [11:0] idl_s1;
wire [15:0] lat_s1; 

reg [15:0] lat_s;

reg [24:0] err_ss, cnt_ss, idl_ss;
reg [23:0] err_s,  cnt_s,  idl_s, cnt_msb, err_msb, idl_msb;

reg test_en_d0, test_en_d1;
wire reg_clr;

reg [2:0] reg_clr_shf;


assign reg_clr = reg_clr_shf[0];

always @(posedge clk) begin
	test_en_d0 <= test_en;
	test_en_d1 <= test_en_d0;
	reg_clr_shf <= (test_en_d0 & (~test_en_d1))? 3'b111 : {1'b0, reg_clr_shf[2:1]};
end


xpm_cdc_single #(
 .DEST_SYNC_FF(4), // DECIMAL; range: 2-10
 .INIT_SYNC_FF(1), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
 .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
 .SRC_INPUT_REG(0) // DECIMAL; 0=do not register input, 1=register input
)
CDC0(
 .dest_out(vld_s3), // 1-bit output: src_in synchronized to the destination clock domain. This output is
 // registered.
 .dest_clk(clk), // 1-bit input: Clock signal for the destination clock domain.
 .src_clk(), // 1-bit input: optional; required when SRC_INPUT_REG = 1
 .src_in(vld_i) // 1-bit input: Input signal to be synchronized to dest_clk domain.
);

xpm_cdc_single #(
 .DEST_SYNC_FF(4), // DECIMAL; range: 2-10
 .INIT_SYNC_FF(1), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
 .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
 .SRC_INPUT_REG(0) // DECIMAL; 0=do not register input, 1=register input
)
CDC1(
 .dest_out(done_o), // 1-bit output: src_in synchronized to the destination clock domain. This output is
 // registered.
 .dest_clk(clk), // 1-bit input: Clock signal for the destination clock domain.
 .src_clk(), // 1-bit input: optional; required when SRC_INPUT_REG = 1
 .src_in(done_i) // 1-bit input: Input signal to be synchronized to dest_clk domain.
);

xpm_cdc_array_single #(
 .DEST_SYNC_FF(2), // DECIMAL; range: 2-10
 .INIT_SYNC_FF(0), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
 .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
 .SRC_INPUT_REG(0), // DECIMAL; 0=do not register input, 1=register input
 .WIDTH(42) // DECIMAL; range: 1-1024
)
CDC2 (
 .dest_out({cnt_s1,err_s1,idl_s1,lat_s1}), // WIDTH-bit output: src_in synchronized to the destination clock domain. This
 // output is registered.
 .dest_clk(clk), // 1-bit input: Clock signal for the destination clock domain.
 .src_clk(), // 1-bit input: optional; required when SRC_INPUT_REG = 1
 .src_in({cnt_i,err_i,idl_i,lat_i}) // WIDTH-bit input: Input single-bit array to be synchronized to destination clock
 // domain. It is assumed that each bit of the array is unrelated to the others. This
 // is reflected in the constraints applied to this macro. To transfer a binary value
 // losslessly across the two clock domains, use the XPM_CDC_GRAY macro instead.
);



//----------------------------------------
// count the errors and accumulate
//----------------------------------------
always @(posedge clk) begin

	// edge detection
	vld_s4 <= vld_s3;
	vld_s <= vld_s4^vld_s3;
	
	cnt_ss <= {1'b0, cnt_s} + {18'd0, cnt_s1};
	err_ss <= {1'b0, err_s} + {18'd0, err_s1};
	idl_ss <= {1'b0, idl_s} + {18'd0, idl_s1};
	
	// accumulate
	cnt_s <= reg_clr? 0 : vld_s? cnt_ss[23:0] : cnt_s;
	err_s <= reg_clr? 0 : vld_s? err_ss[23:0] : err_s;
	idl_s <= reg_clr? 0 : vld_s? idl_ss[23:0] : idl_s;
	lat_s <= reg_clr? 0 : vld_s? lat_s1 : lat_s;
	
	cnt_msb <= reg_clr? 0 : cnt_msb + {23'd0, vld_s&cnt_ss[24]};
	err_msb <= reg_clr? 0 : err_msb + {23'd0, vld_s&err_ss[24]};
	idl_msb <= reg_clr? 0 : idl_msb + {23'd0, vld_s&idl_ss[24]};
	
	// output
	smp_cnt_o <= {cnt_msb, cnt_s};
	err_cnt_o <= {err_msb, err_s};
	idl_cnt_o <= {idl_msb, idl_s};
	lat_cnt_o <= lat_s;

end


		

endmodule
