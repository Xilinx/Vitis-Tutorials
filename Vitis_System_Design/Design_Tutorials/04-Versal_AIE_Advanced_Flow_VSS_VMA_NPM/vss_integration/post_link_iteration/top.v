// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

// Top-level module instantiating vitis_design_vma_wrapper and axis_sink, connecting AXI Stream interface
`timescale 1 ps / 1 ps

module top (
    // LPDDR4 Clock inputs
    input lpddr4_clk1_clk_n,
    input lpddr4_clk1_clk_p,
    input lpddr4_clk2_clk_n,
    input lpddr4_clk2_clk_p,
    input lpddr4_clk3_clk_n,
    input lpddr4_clk3_clk_p,
    
    // LPDDR4 Channel 0 Trip 1
    output [5:0] ch0_lpddr4_trip1_ca_a,
    output ch0_lpddr4_trip1_ck_c_a,
    output ch0_lpddr4_trip1_ck_t_a,
    output ch0_lpddr4_trip1_cke_a,
    output ch0_lpddr4_trip1_cs_a,
    inout [1:0] ch0_lpddr4_trip1_dmi_a,
    inout [1:0] ch0_lpddr4_trip1_dmi_b,
    inout [15:0] ch0_lpddr4_trip1_dq_a,
    inout [15:0] ch0_lpddr4_trip1_dq_b,
    inout [1:0] ch0_lpddr4_trip1_dqs_c_a,
    inout [1:0] ch0_lpddr4_trip1_dqs_c_b,
    inout [1:0] ch0_lpddr4_trip1_dqs_t_a,
    inout [1:0] ch0_lpddr4_trip1_dqs_t_b,
    output ch0_lpddr4_trip1_reset_n,
    
    // LPDDR4 Channel 0 Trip 2
    output [5:0] ch0_lpddr4_trip2_ca_a,
    output ch0_lpddr4_trip2_ck_c_a,
    output ch0_lpddr4_trip2_ck_t_a,
    output ch0_lpddr4_trip2_cke_a,
    output ch0_lpddr4_trip2_cs_a,
    inout [1:0] ch0_lpddr4_trip2_dmi_a,
    inout [1:0] ch0_lpddr4_trip2_dmi_b,
    inout [15:0] ch0_lpddr4_trip2_dq_a,
    inout [15:0] ch0_lpddr4_trip2_dq_b,
    inout [1:0] ch0_lpddr4_trip2_dqs_c_a,
    inout [1:0] ch0_lpddr4_trip2_dqs_c_b,
    inout [1:0] ch0_lpddr4_trip2_dqs_t_a,
    inout [1:0] ch0_lpddr4_trip2_dqs_t_b,
    output ch0_lpddr4_trip2_reset_n,
    
    // LPDDR4 Channel 0 Trip 3
    output [5:0] ch0_lpddr4_trip3_ca_a,
    output ch0_lpddr4_trip3_ck_c_a,
    output ch0_lpddr4_trip3_ck_t_a,
    output ch0_lpddr4_trip3_cke_a,
    output ch0_lpddr4_trip3_cs_a,
    inout [1:0] ch0_lpddr4_trip3_dmi_a,
    inout [1:0] ch0_lpddr4_trip3_dmi_b,
    inout [15:0] ch0_lpddr4_trip3_dq_a,
    inout [15:0] ch0_lpddr4_trip3_dq_b,
    inout [1:0] ch0_lpddr4_trip3_dqs_c_a,
    inout [1:0] ch0_lpddr4_trip3_dqs_c_b,
    inout [1:0] ch0_lpddr4_trip3_dqs_t_a,
    inout [1:0] ch0_lpddr4_trip3_dqs_t_b,
    output ch0_lpddr4_trip3_reset_n,
    
    // LPDDR4 Channel 1 Trip 1
    output [5:0] ch1_lpddr4_trip1_ca_a,
    output ch1_lpddr4_trip1_ck_c_a,
    output ch1_lpddr4_trip1_ck_t_a,
    output ch1_lpddr4_trip1_cke_a,
    output ch1_lpddr4_trip1_cs_a,
    inout [1:0] ch1_lpddr4_trip1_dmi_a,
    inout [1:0] ch1_lpddr4_trip1_dmi_b,
    inout [15:0] ch1_lpddr4_trip1_dq_a,
    inout [15:0] ch1_lpddr4_trip1_dq_b,
    inout [1:0] ch1_lpddr4_trip1_dqs_c_a,
    inout [1:0] ch1_lpddr4_trip1_dqs_c_b,
    inout [1:0] ch1_lpddr4_trip1_dqs_t_a,
    inout [1:0] ch1_lpddr4_trip1_dqs_t_b,
    output ch1_lpddr4_trip1_reset_n,
    
    // LPDDR4 Channel 1 Trip 2
    output [5:0] ch1_lpddr4_trip2_ca_a,
    output ch1_lpddr4_trip2_ck_c_a,
    output ch1_lpddr4_trip2_ck_t_a,
    output ch1_lpddr4_trip2_cke_a,
    output ch1_lpddr4_trip2_cs_a,
    inout [1:0] ch1_lpddr4_trip2_dmi_a,
    inout [1:0] ch1_lpddr4_trip2_dmi_b,
    inout [15:0] ch1_lpddr4_trip2_dq_a,
    inout [15:0] ch1_lpddr4_trip2_dq_b,
    inout [1:0] ch1_lpddr4_trip2_dqs_c_a,
    inout [1:0] ch1_lpddr4_trip2_dqs_c_b,
    inout [1:0] ch1_lpddr4_trip2_dqs_t_a,
    inout [1:0] ch1_lpddr4_trip2_dqs_t_b,
    output ch1_lpddr4_trip2_reset_n,
    
    // LPDDR4 Channel 1 Trip 3
    output [5:0] ch1_lpddr4_trip3_ca_a,
    output ch1_lpddr4_trip3_ck_c_a,
    output ch1_lpddr4_trip3_ck_t_a,
    output ch1_lpddr4_trip3_cke_a,
    output ch1_lpddr4_trip3_cs_a,
    inout [1:0] ch1_lpddr4_trip3_dmi_a,
    inout [1:0] ch1_lpddr4_trip3_dmi_b,
    inout [15:0] ch1_lpddr4_trip3_dq_a,
    inout [15:0] ch1_lpddr4_trip3_dq_b,
    inout [1:0] ch1_lpddr4_trip3_dqs_c_a,
    inout [1:0] ch1_lpddr4_trip3_dqs_c_b,
    inout [1:0] ch1_lpddr4_trip3_dqs_t_a,
    inout [1:0] ch1_lpddr4_trip3_dqs_t_b,
    output ch1_lpddr4_trip3_reset_n
);

	// AXI Stream signals
	wire [31:0] M_AXIS_0_tdata;
	wire [0:0] M_AXIS_0_tkeep;
	wire [0:0] M_AXIS_0_tlast;
	wire [0:0] M_AXIS_0_tready;
	wire [0:0] M_AXIS_0_tvalid;
	wire aclk_0;
	wire aresetn_0;

	// Instantiate vitis_design_vma_wrapper
	vitis_design_vma_wrapper u_vitis_design_vma_wrapper (
		.M_AXIS_0_tdata(M_AXIS_0_tdata),
		.M_AXIS_0_tkeep(M_AXIS_0_tkeep),
		.M_AXIS_0_tlast(M_AXIS_0_tlast),
		.M_AXIS_0_tready(M_AXIS_0_tready),
		.M_AXIS_0_tvalid(M_AXIS_0_tvalid),
		.aclk_0(aclk_0),
		.aresetn_0(1'b1),
		.pl0_ref_clk(aclk_0),
		
		// LPDDR4 Clock inputs
		.lpddr4_clk1_clk_n(lpddr4_clk1_clk_n),
		.lpddr4_clk1_clk_p(lpddr4_clk1_clk_p),
		.lpddr4_clk2_clk_n(lpddr4_clk2_clk_n),
		.lpddr4_clk2_clk_p(lpddr4_clk2_clk_p),
		.lpddr4_clk3_clk_n(lpddr4_clk3_clk_n),
		.lpddr4_clk3_clk_p(lpddr4_clk3_clk_p),
		
		// LPDDR4 Channel 0 Trip 1
		.ch0_lpddr4_trip1_ca_a(ch0_lpddr4_trip1_ca_a),
		.ch0_lpddr4_trip1_ck_c_a(ch0_lpddr4_trip1_ck_c_a),
		.ch0_lpddr4_trip1_ck_t_a(ch0_lpddr4_trip1_ck_t_a),
		.ch0_lpddr4_trip1_cke_a(ch0_lpddr4_trip1_cke_a),
		.ch0_lpddr4_trip1_cs_a(ch0_lpddr4_trip1_cs_a),
		.ch0_lpddr4_trip1_dmi_a(ch0_lpddr4_trip1_dmi_a),
		.ch0_lpddr4_trip1_dmi_b(ch0_lpddr4_trip1_dmi_b),
		.ch0_lpddr4_trip1_dq_a(ch0_lpddr4_trip1_dq_a),
		.ch0_lpddr4_trip1_dq_b(ch0_lpddr4_trip1_dq_b),
		.ch0_lpddr4_trip1_dqs_c_a(ch0_lpddr4_trip1_dqs_c_a),
		.ch0_lpddr4_trip1_dqs_c_b(ch0_lpddr4_trip1_dqs_c_b),
		.ch0_lpddr4_trip1_dqs_t_a(ch0_lpddr4_trip1_dqs_t_a),
		.ch0_lpddr4_trip1_dqs_t_b(ch0_lpddr4_trip1_dqs_t_b),
		.ch0_lpddr4_trip1_reset_n(ch0_lpddr4_trip1_reset_n),
		
		// LPDDR4 Channel 0 Trip 2
		.ch0_lpddr4_trip2_ca_a(ch0_lpddr4_trip2_ca_a),
		.ch0_lpddr4_trip2_ck_c_a(ch0_lpddr4_trip2_ck_c_a),
		.ch0_lpddr4_trip2_ck_t_a(ch0_lpddr4_trip2_ck_t_a),
		.ch0_lpddr4_trip2_cke_a(ch0_lpddr4_trip2_cke_a),
		.ch0_lpddr4_trip2_cs_a(ch0_lpddr4_trip2_cs_a),
		.ch0_lpddr4_trip2_dmi_a(ch0_lpddr4_trip2_dmi_a),
		.ch0_lpddr4_trip2_dmi_b(ch0_lpddr4_trip2_dmi_b),
		.ch0_lpddr4_trip2_dq_a(ch0_lpddr4_trip2_dq_a),
		.ch0_lpddr4_trip2_dq_b(ch0_lpddr4_trip2_dq_b),
		.ch0_lpddr4_trip2_dqs_c_a(ch0_lpddr4_trip2_dqs_c_a),
		.ch0_lpddr4_trip2_dqs_c_b(ch0_lpddr4_trip2_dqs_c_b),
		.ch0_lpddr4_trip2_dqs_t_a(ch0_lpddr4_trip2_dqs_t_a),
		.ch0_lpddr4_trip2_dqs_t_b(ch0_lpddr4_trip2_dqs_t_b),
		.ch0_lpddr4_trip2_reset_n(ch0_lpddr4_trip2_reset_n),
		
		// LPDDR4 Channel 0 Trip 3
		.ch0_lpddr4_trip3_ca_a(ch0_lpddr4_trip3_ca_a),
		.ch0_lpddr4_trip3_ck_c_a(ch0_lpddr4_trip3_ck_c_a),
		.ch0_lpddr4_trip3_ck_t_a(ch0_lpddr4_trip3_ck_t_a),
		.ch0_lpddr4_trip3_cke_a(ch0_lpddr4_trip3_cke_a),
		.ch0_lpddr4_trip3_cs_a(ch0_lpddr4_trip3_cs_a),
		.ch0_lpddr4_trip3_dmi_a(ch0_lpddr4_trip3_dmi_a),
		.ch0_lpddr4_trip3_dmi_b(ch0_lpddr4_trip3_dmi_b),
		.ch0_lpddr4_trip3_dq_a(ch0_lpddr4_trip3_dq_a),
		.ch0_lpddr4_trip3_dq_b(ch0_lpddr4_trip3_dq_b),
		.ch0_lpddr4_trip3_dqs_c_a(ch0_lpddr4_trip3_dqs_c_a),
		.ch0_lpddr4_trip3_dqs_c_b(ch0_lpddr4_trip3_dqs_c_b),
		.ch0_lpddr4_trip3_dqs_t_a(ch0_lpddr4_trip3_dqs_t_a),
		.ch0_lpddr4_trip3_dqs_t_b(ch0_lpddr4_trip3_dqs_t_b),
		.ch0_lpddr4_trip3_reset_n(ch0_lpddr4_trip3_reset_n),
		
		// LPDDR4 Channel 1 Trip 1
		.ch1_lpddr4_trip1_ca_a(ch1_lpddr4_trip1_ca_a),
		.ch1_lpddr4_trip1_ck_c_a(ch1_lpddr4_trip1_ck_c_a),
		.ch1_lpddr4_trip1_ck_t_a(ch1_lpddr4_trip1_ck_t_a),
		.ch1_lpddr4_trip1_cke_a(ch1_lpddr4_trip1_cke_a),
		.ch1_lpddr4_trip1_cs_a(ch1_lpddr4_trip1_cs_a),
		.ch1_lpddr4_trip1_dmi_a(ch1_lpddr4_trip1_dmi_a),
		.ch1_lpddr4_trip1_dmi_b(ch1_lpddr4_trip1_dmi_b),
		.ch1_lpddr4_trip1_dq_a(ch1_lpddr4_trip1_dq_a),
		.ch1_lpddr4_trip1_dq_b(ch1_lpddr4_trip1_dq_b),
		.ch1_lpddr4_trip1_dqs_c_a(ch1_lpddr4_trip1_dqs_c_a),
		.ch1_lpddr4_trip1_dqs_c_b(ch1_lpddr4_trip1_dqs_c_b),
		.ch1_lpddr4_trip1_dqs_t_a(ch1_lpddr4_trip1_dqs_t_a),
		.ch1_lpddr4_trip1_dqs_t_b(ch1_lpddr4_trip1_dqs_t_b),
		.ch1_lpddr4_trip1_reset_n(ch1_lpddr4_trip1_reset_n),
		
		// LPDDR4 Channel 1 Trip 2
		.ch1_lpddr4_trip2_ca_a(ch1_lpddr4_trip2_ca_a),
		.ch1_lpddr4_trip2_ck_c_a(ch1_lpddr4_trip2_ck_c_a),
		.ch1_lpddr4_trip2_ck_t_a(ch1_lpddr4_trip2_ck_t_a),
		.ch1_lpddr4_trip2_cke_a(ch1_lpddr4_trip2_cke_a),
		.ch1_lpddr4_trip2_cs_a(ch1_lpddr4_trip2_cs_a),
		.ch1_lpddr4_trip2_dmi_a(ch1_lpddr4_trip2_dmi_a),
		.ch1_lpddr4_trip2_dmi_b(ch1_lpddr4_trip2_dmi_b),
		.ch1_lpddr4_trip2_dq_a(ch1_lpddr4_trip2_dq_a),
		.ch1_lpddr4_trip2_dq_b(ch1_lpddr4_trip2_dq_b),
		.ch1_lpddr4_trip2_dqs_c_a(ch1_lpddr4_trip2_dqs_c_a),
		.ch1_lpddr4_trip2_dqs_c_b(ch1_lpddr4_trip2_dqs_c_b),
		.ch1_lpddr4_trip2_dqs_t_a(ch1_lpddr4_trip2_dqs_t_a),
		.ch1_lpddr4_trip2_dqs_t_b(ch1_lpddr4_trip2_dqs_t_b),
		.ch1_lpddr4_trip2_reset_n(ch1_lpddr4_trip2_reset_n),
		
		// LPDDR4 Channel 1 Trip 3
		.ch1_lpddr4_trip3_ca_a(ch1_lpddr4_trip3_ca_a),
		.ch1_lpddr4_trip3_ck_c_a(ch1_lpddr4_trip3_ck_c_a),
		.ch1_lpddr4_trip3_ck_t_a(ch1_lpddr4_trip3_ck_t_a),
		.ch1_lpddr4_trip3_cke_a(ch1_lpddr4_trip3_cke_a),
		.ch1_lpddr4_trip3_cs_a(ch1_lpddr4_trip3_cs_a),
		.ch1_lpddr4_trip3_dmi_a(ch1_lpddr4_trip3_dmi_a),
		.ch1_lpddr4_trip3_dmi_b(ch1_lpddr4_trip3_dmi_b),
		.ch1_lpddr4_trip3_dq_a(ch1_lpddr4_trip3_dq_a),
		.ch1_lpddr4_trip3_dq_b(ch1_lpddr4_trip3_dq_b),
		.ch1_lpddr4_trip3_dqs_c_a(ch1_lpddr4_trip3_dqs_c_a),
		.ch1_lpddr4_trip3_dqs_c_b(ch1_lpddr4_trip3_dqs_c_b),
		.ch1_lpddr4_trip3_dqs_t_a(ch1_lpddr4_trip3_dqs_t_a),
		.ch1_lpddr4_trip3_dqs_t_b(ch1_lpddr4_trip3_dqs_t_b),
		.ch1_lpddr4_trip3_reset_n(ch1_lpddr4_trip3_reset_n)
	);

	// Instantiate axis_sink
	axis_sink u_axis_sink (
		.aclk(aclk_0),
		.aresetn(1'b1),
		.s_axis_tdata(M_AXIS_0_tdata),
		.s_axis_tkeep(M_AXIS_0_tkeep),
		.s_axis_tlast(M_AXIS_0_tlast),
		.s_axis_tvalid(M_AXIS_0_tvalid),
		.s_axis_tready(M_AXIS_0_tready)
		// ... connect other IO as needed ...
	);

endmodule
