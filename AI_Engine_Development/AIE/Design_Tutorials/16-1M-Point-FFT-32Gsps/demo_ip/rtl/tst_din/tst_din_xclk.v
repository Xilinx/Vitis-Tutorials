//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module tst_din_xclk
(
	//--------------------------------
	// register clock domain
	//--------------------------------
	input   clk_reg,
	input    test_en_reg,
	output  [31:0] itecnt_reg,
	//----------------------------------
	// aie clock domain
	//----------------------------------	
	input   clk_aie,
	output  test_en_aie,
	input  [31:0] itecnt_aie
);

//----------------------------------------
// REG -> AIE
//----------------------------------------
xpm_cdc_single #(
 .DEST_SYNC_FF(4), // DECIMAL; range: 2-10
 .INIT_SYNC_FF(1), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
 .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
 .SRC_INPUT_REG(0) // DECIMAL; 0=do not register input, 1=register input
)
CDC0(
 .dest_out(test_en_aie), // 1-bit output: src_in synchronized to the destination clock domain. This output is
 // registered.
 .dest_clk(clk_aie), // 1-bit input: Clock signal for the destination clock domain.
 .src_clk(), // 1-bit input: optional; required when SRC_INPUT_REG = 1
 .src_in(test_en_reg) // 1-bit input: Input signal to be synchronized to dest_clk domain.
);


//----------------------------------------
// AIE -> REG
//----------------------------------------
xpm_cdc_array_single #(
 .DEST_SYNC_FF(4), // DECIMAL; range: 2-10
 .INIT_SYNC_FF(0), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
 .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
 .SRC_INPUT_REG(0), // DECIMAL; 0=do not register input, 1=register input
 .WIDTH(32) // DECIMAL; range: 1-1024
)
CDC1 (
 .dest_out(itecnt_reg), // WIDTH-bit output: src_in synchronized to the destination clock domain. This
 // output is registered.
 .dest_clk(clk_reg), // 1-bit input: Clock signal for the destination clock domain.
 .src_clk(), // 1-bit input: optional; required when SRC_INPUT_REG = 1
 .src_in(itecnt_aie) // WIDTH-bit input: Input single-bit array to be synchronized to destination clock
 // domain. It is assumed that each bit of the array is unrelated to the others. This
 // is reflected in the constraints applied to this macro. To transfer a binary value
 // losslessly across the two clock domains, use the XPM_CDC_GRAY macro instead.
);



endmodule
