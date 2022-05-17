//  © Copyright 2020 Xilinx, Inc.
//  Licensed under the Apache License, Version 2.0 (the 'License');
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//       http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an 'AS IS' BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


`timescale 1ns / 1ps


module ulbf_slave_cdc (
	input wire bram_clk_a,
	input wire s_axis_clk,
	
	input wire slave_rst_bram,
	input wire rxdone_saxis,
	input wire [11:0] niter_bram,
        input wire [3:0] current_state_saxis,
        input wire [15:0] rxram_counter_saxis,
        input wire [63:0] doutb_saxis,
        input wire [15:0] addrb_bram,
	input wire web_bram,
	input wire enb_bram,

	output wire slave_rst_saxis,
	output wire rxdone_bram,
	output wire [11:0] niter_saxis,
        output wire [3:0] current_state_bram,
        output wire [15:0] rxram_counter_bram,
        output wire [63:0] doutb_bram,
        output wire [15:0] addrb_saxis,
	output wire web_saxis,
	output wire enb_saxis
	
 );
	wire [31:0] doutb_bram0, doutb_bram1;

    xpm_cdc_single #(
       .DEST_SYNC_FF(4),   // DECIMAL; range: 2-10
       .INIT_SYNC_FF(0),   // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
       .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
       .SRC_INPUT_REG(1)   // DECIMAL; 0=do not register input, 1=register input
       )
    xpm_cdc_single_rst (
       .dest_out(slave_rst_saxis), // 1-bit output: src_in synchronized to the destination clock domain. This output is registered.
       .dest_clk(s_axis_clk), // 1-bit input: Clock signal for the destination clock domain.
       .src_clk(bram_clk_a),   // 1-bit input: optional; required when SRC_INPUT_REG = 1
       .src_in(slave_rst_bram)      // 1-bit input: Input signal to be synchronized to dest_clk domain.
    );
   
      
   xpm_cdc_gray #(
      .DEST_SYNC_FF(4),          // DECIMAL; range: 2-10
      .INIT_SYNC_FF(0),          // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      .REG_OUTPUT(1),            // DECIMAL; 0=disable registered output, 1=enable registered output
      .SIM_ASSERT_CHK(0),        // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .SIM_LOSSLESS_GRAY_CHK(0), // DECIMAL; 0=disable lossless check, 1=enable lossless check
      .WIDTH(12)                  // DECIMAL; range: 2-32
   )
   xpm_cdc_gray_max_iter (
      .dest_out_bin(niter_saxis), // WIDTH-bit output: Binary input bus (src_in_bin) synchronized to
                                   // destination clock domain. This output is combinatorial unless REG_OUTPUT
                                   // is set to 1.

      .dest_clk(s_axis_clk),         // 1-bit input: Destination clock.
      .src_clk(bram_clk_a),           // 1-bit input: Source clock.
      .src_in_bin(niter_bram)      // WIDTH-bit input: Binary input bus that will be synchronized to the
                                   // destination clock domain.

   );


xpm_cdc_single #(
       .DEST_SYNC_FF(4),   // DECIMAL; range: 2-10
       .INIT_SYNC_FF(0),   // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
       .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
       .SRC_INPUT_REG(1)   // DECIMAL; 0=do not register input, 1=register input
       )
    xpm_cdc_single_done (
       .dest_out(rxdone_bram), // 1-bit output: src_in synchronized to the destination clock domain. This output is registered.
       .dest_clk(bram_clk_a), // 1-bit input: Clock signal for the destination clock domain.
       .src_clk(s_axis_clk),   // 1-bit input: optional; required when SRC_INPUT_REG = 1
       .src_in(rxdone_saxis)      // 1-bit input: Input signal to be synchronized to dest_clk domain.
    );

   xpm_cdc_gray #(
      .DEST_SYNC_FF(4),          // DECIMAL; range: 2-10
      .INIT_SYNC_FF(0),          // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      .REG_OUTPUT(1),            // DECIMAL; 0=disable registered output, 1=enable registered output
      .SIM_ASSERT_CHK(0),        // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .SIM_LOSSLESS_GRAY_CHK(0), // DECIMAL; 0=disable lossless check, 1=enable lossless check
      .WIDTH(4)                  // DECIMAL; range: 2-32
   )
   xpm_cdc_gray_curr_state (
      .dest_out_bin(current_state_bram), // WIDTH-bit output: Binary input bus (src_in_bin) synchronized to
                                   // destination clock domain. This output is combinatorial unless REG_OUTPUT
                                   // is set to 1.

      .dest_clk(bram_clk_a),         // 1-bit input: Destination clock.
      .src_clk(s_axis_clk),           // 1-bit input: Source clock.
      .src_in_bin(current_state_saxis)      // WIDTH-bit input: Binary input bus that will be synchronized to the
                                   // destination clock domain.

   );
  
   xpm_cdc_gray #(
      .DEST_SYNC_FF(4),          // DECIMAL; range: 2-10
      .INIT_SYNC_FF(0),          // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      .REG_OUTPUT(1),            // DECIMAL; 0=disable registered output, 1=enable registered output
      .SIM_ASSERT_CHK(0),        // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .SIM_LOSSLESS_GRAY_CHK(0), // DECIMAL; 0=disable lossless check, 1=enable lossless check
      .WIDTH(16)                  // DECIMAL; range: 2-32
   )
   xpm_cdc_gray_rxram_counter (
      .dest_out_bin(rxram_counter_bram), // WIDTH-bit output: Binary input bus (src_in_bin) synchronized to
                                   // destination clock domain. This output is combinatorial unless REG_OUTPUT
                                   // is set to 1.

      .dest_clk(bram_clk_a),         // 1-bit input: Destination clock.
      .src_clk(s_axis_clk),           // 1-bit input: Source clock.
      .src_in_bin(rxram_counter_saxis)      // WIDTH-bit input: Binary input bus that will be synchronized to the
                                   // destination clock domain.

   );

//----------------------------
   xpm_cdc_gray #(
      .DEST_SYNC_FF(3),          // DECIMAL; range: 2-10
      .INIT_SYNC_FF(0),          // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      .REG_OUTPUT(1),            // DECIMAL; 0=disable registered output, 1=enable registered output
      .SIM_ASSERT_CHK(0),        // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .SIM_LOSSLESS_GRAY_CHK(0), // DECIMAL; 0=disable lossless check, 1=enable lossless check
      .WIDTH(32)                  // DECIMAL; range: 2-32
   )
   xpm_cdc_gray_doutb0 (
      .dest_out_bin(doutb_bram0), // WIDTH-bit output: Binary input bus (src_in_bin) synchronized to
                                   // destination clock domain. This output is combinatorial unless REG_OUTPUT
                                   // is set to 1.

      .dest_clk(bram_clk_a),         // 1-bit input: Destination clock.
      .src_clk(s_axis_clk),           // 1-bit input: Source clock.
      .src_in_bin(doutb_saxis[31:0])      // WIDTH-bit input: Binary input bus that will be synchronized to the
                                   // destination clock domain.

   );

xpm_cdc_gray #(
      .DEST_SYNC_FF(3),          // DECIMAL; range: 2-10
      .INIT_SYNC_FF(0),          // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      .REG_OUTPUT(1),            // DECIMAL; 0=disable registered output, 1=enable registered output
      .SIM_ASSERT_CHK(0),        // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .SIM_LOSSLESS_GRAY_CHK(0), // DECIMAL; 0=disable lossless check, 1=enable lossless check
      .WIDTH(32)                  // DECIMAL; range: 2-32
   )
   xpm_cdc_gray_doutb1 (
      .dest_out_bin(doutb_bram1), // WIDTH-bit output: Binary input bus (src_in_bin) synchronized to
                                   // destination clock domain. This output is combinatorial unless REG_OUTPUT
                                   // is set to 1.

      .dest_clk(bram_clk_a),         // 1-bit input: Destination clock.
      .src_clk(s_axis_clk),           // 1-bit input: Source clock.
      .src_in_bin(doutb_saxis[63:32])      // WIDTH-bit input: Binary input bus that will be synchronized to the
                                   // destination clock domain.

   );

  assign doutb_bram = {doutb_bram1, doutb_bram0};
  
   xpm_cdc_gray #(
      .DEST_SYNC_FF(3),          // DECIMAL; range: 2-10
      .INIT_SYNC_FF(0),          // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      .REG_OUTPUT(1),            // DECIMAL; 0=disable registered output, 1=enable registered output
      .SIM_ASSERT_CHK(0),        // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .SIM_LOSSLESS_GRAY_CHK(0), // DECIMAL; 0=disable lossless check, 1=enable lossless check
      .WIDTH(16)                  // DECIMAL; range: 2-32
   )
   xpm_cdc_gray_addrb (
      .dest_out_bin(addrb_saxis), // WIDTH-bit output: Binary input bus (src_in_bin) synchronized to
                                   // destination clock domain. This output is combinatorial unless REG_OUTPUT
                                   // is set to 1.

      .dest_clk(s_axis_clk),         // 1-bit input: Destination clock.
      .src_clk(bram_clk_a),           // 1-bit input: Source clock.
      .src_in_bin(addrb_bram)      // WIDTH-bit input: Binary input bus that will be synchronized to the
                                   // destination clock domain.

   );

    xpm_cdc_single #(
       .DEST_SYNC_FF(4),   // DECIMAL; range: 2-10
       .INIT_SYNC_FF(0),   // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
       .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
       .SRC_INPUT_REG(1)   // DECIMAL; 0=do not register input, 1=register input
       )
    xpm_cdc_single_web (
       .dest_out(web_saxis), // 1-bit output: src_in synchronized to the destination clock domain. This output is registered.
       .dest_clk(s_axis_clk), // 1-bit input: Clock signal for the destination clock domain.
       .src_clk(bram_clk_a),   // 1-bit input: optional; required when SRC_INPUT_REG = 1
       .src_in(web_bram)      // 1-bit input: Input signal to be synchronized to dest_clk domain.
    );

    xpm_cdc_single #(
       .DEST_SYNC_FF(3),   // DECIMAL; range: 2-10
       .INIT_SYNC_FF(0),   // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
       .SIM_ASSERT_CHK(0), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
       .SRC_INPUT_REG(1)   // DECIMAL; 0=do not register input, 1=register input
       )
    xpm_cdc_single_enb (
       .dest_out(enb_saxis), // 1-bit output: src_in synchronized to the destination clock domain. This output is registered.
       .dest_clk(s_axis_clk), // 1-bit input: Clock signal for the destination clock domain.
       .src_clk(bram_clk_a),   // 1-bit input: optional; required when SRC_INPUT_REG = 1
       .src_in(enb_bram)      // 1-bit input: Input signal to be synchronized to dest_clk domain.
    );
endmodule


