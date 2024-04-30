//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

module tst_dout_mon_rom
#(parameter romReadLatency=3, golden_rom_file_name = "gold0.mem", DATA_BW=64, ADDR_BW=14, ROMLEN=2352)
(
	input   clk,
	input   srst,
	input   fifo_re,
	output  reg fifo_rvld,
	output  [DATA_BW-1:0] fifo_rdat
);

localparam bw_ra=ADDR_BW;
localparam romdly=romReadLatency;
localparam ra_last = (ROMLEN-2);

reg [bw_ra-1:0] ra;
reg [bw_ra-1:0] ra_d1;
reg [bw_ra-1:0] ra_d2;
reg [romdly:0]  shft;

reg islast;

// dealing with header AXI input
always @(posedge clk) begin

	ra <= srst? 0 : ~fifo_re? ra : (ra+1);
	ra_d1 <= ra;
	ra_d2 <= ra_d1;
	
	// delay re for some cycles
	shft <= srst? 0 : {fifo_re, shft[romdly:1]};
	
	// control ram we
	fifo_rvld <= shft[0];
	
	// indicate end of buffer
	islast <= srst? 1'b0 : ~fifo_re? islast : (ra==ra_last)?  1'b1 : 1'b0;
	
end

//-----------------------------------------------------------
// URAM ROM
//-----------------------------------------------------------
// xpm_memory_sprom: Single Port ROM
// Xilinx Parameterized Macro, version 2019.2
xpm_memory_sprom #(
.ADDR_WIDTH_A(bw_ra), // DECIMAL
.AUTO_SLEEP_TIME(0), // DECIMAL
.CASCADE_HEIGHT(0), // DECIMAL
.ECC_MODE("no_ecc"), // String
.MEMORY_INIT_FILE(golden_rom_file_name), // String
.MEMORY_INIT_PARAM("0"), // String
.MEMORY_OPTIMIZATION("false"), // String
.MEMORY_PRIMITIVE("ultra"), // String
.MEMORY_SIZE(DATA_BW* (2**bw_ra) ),  //
.MESSAGE_CONTROL(0), // DECIMAL
.READ_DATA_WIDTH_A(DATA_BW), // DECIMAL
.READ_LATENCY_A(romReadLatency), // DECIMAL
.READ_RESET_VALUE_A("0"), // String
.RST_MODE_A("SYNC"), // String
.SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
.USE_MEM_INIT(1), // DECIMAL
.WAKEUP_TIME("disable_sleep") // String
)
xpm_memory_sprom_inst (
.dbiterra(), // 1-bit output: Leave open.
.douta(fifo_rdat), // READ_DATA_WIDTH_A-bit output: Data output for port A read operations.
.sbiterra(), // 1-bit output: Leave open.
.addra(ra_d2), // ADDR_WIDTH_A-bit input: Address for port A read operations.
.clka(clk), // 1-bit input: Clock signal for port A.
.ena(1'b1), // 1-bit input: Memory enable signal for port A. Must be high on clock
// cycles when read operations are initiated. Pipelined internally.
.injectdbiterra(1'b0), // 1-bit input: Do not change from the provided value.
.injectsbiterra(1'b0), // 1-bit input: Do not change from the provided value.
.regcea(1'b1), // 1-bit input: Do not change from the provided value.
.rsta(1'b0), // 1-bit input: Reset signal for the final port A output register stage.
// Synchronously resets output port douta to the value specified by
// parameter READ_RESET_VALUE_A.
.sleep() // 1-bit input: sleep signal to enable the dynamic power saving feature.
);
// End of xpm_memory_sprom_inst instantiation

	
endmodule
