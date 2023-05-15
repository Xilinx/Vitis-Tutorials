//
//# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
//# SPDX-License-Identifier: X11
//

// default_nettype of none prevents implicit wire declaration.
`default_nettype none
`timescale 1 ps / 1 ps
import axi_vip_pkg::*;
import slv_fontread_axi_m_vip_pkg::*;
import axi4stream_vip_pkg::*;
import dataout_axis_m_vip_pkg::*;
import control_rtc_gen_vip_pkg::*;

module rtc_gen_tb ();
parameter integer LP_MAX_LENGTH = 8192;
parameter integer LP_MAX_TRANSFER_LENGTH = 16384 / 4;
parameter integer C_S_AXI_CONTROL_ADDR_WIDTH = 12;
parameter integer C_S_AXI_CONTROL_DATA_WIDTH = 32;
parameter integer C_FONTREAD_AXI_M_ADDR_WIDTH = 64;
parameter integer C_FONTREAD_AXI_M_DATA_WIDTH = 32;
parameter integer C_DATAOUT_AXIS_M_TDATA_WIDTH = 64;

parameter integer FONT_WIDTH = 160;
parameter integer FONT_HEIGHT = 240;
parameter integer FONT_LIB_SIZE = FONT_HEIGHT * FONT_WIDTH * 4 * 11 / 32;

// Control Register
parameter KRNL_CTRL_REG_ADDR     = 32'h00000000;
parameter CTRL_START_MASK        = 32'h00000001;
parameter CTRL_DONE_MASK         = 32'h00000002;
parameter CTRL_IDLE_MASK         = 32'h00000004;
parameter CTRL_READY_MASK        = 32'h00000008;
parameter CTRL_CONTINUE_MASK     = 32'h00000010; // Only ap_ctrl_chain
parameter CTRL_AUTO_RESTART_MASK = 32'h00000080; // Not used

// Global Interrupt Enable Register
parameter KRNL_GIE_REG_ADDR      = 32'h00000004;
parameter GIE_GIE_MASK           = 32'h00000001;
// IP Interrupt Enable Register
parameter KRNL_IER_REG_ADDR      = 32'h00000008;
parameter IER_DONE_MASK          = 32'h00000001;
parameter IER_READY_MASK         = 32'h00000002;
// IP Interrupt Status Register
parameter KRNL_ISR_REG_ADDR      = 32'h0000000c;
parameter ISR_DONE_MASK          = 32'h00000001;
parameter ISR_READY_MASK         = 32'h00000002;

parameter integer LP_CLK_PERIOD_PS = 4000; // 250 MHz

//System Signals
logic ap_clk = 0;

initial begin: AP_CLK
  forever begin
    ap_clk = #(LP_CLK_PERIOD_PS/2) ~ap_clk;
  end
end
 
//System Signals
logic ap_rst_n = 0;
logic initial_reset  =0;

task automatic ap_rst_n_sequence(input integer unsigned width = 20);
  @(posedge ap_clk);
  #1ps;
  ap_rst_n = 0;
  repeat (width) @(posedge ap_clk);
  #1ps;
  ap_rst_n = 1;
endtask

initial begin: AP_RST
  ap_rst_n_sequence(50);
  initial_reset =1;
end
//AXI4 master interface fontread_axi_m
wire [1-1:0] fontread_axi_m_awvalid;
wire [1-1:0] fontread_axi_m_awready;
wire [C_FONTREAD_AXI_M_ADDR_WIDTH-1:0] fontread_axi_m_awaddr;
wire [8-1:0] fontread_axi_m_awlen;
wire [1-1:0] fontread_axi_m_wvalid;
wire [1-1:0] fontread_axi_m_wready;
wire [C_FONTREAD_AXI_M_DATA_WIDTH-1:0] fontread_axi_m_wdata;
wire [C_FONTREAD_AXI_M_DATA_WIDTH/8-1:0] fontread_axi_m_wstrb;
wire [1-1:0] fontread_axi_m_wlast;
wire [1-1:0] fontread_axi_m_bvalid;
wire [1-1:0] fontread_axi_m_bready;
wire [1-1:0] fontread_axi_m_arvalid;
wire [1-1:0] fontread_axi_m_arready;
wire [C_FONTREAD_AXI_M_ADDR_WIDTH-1:0] fontread_axi_m_araddr;
wire [8-1:0] fontread_axi_m_arlen;
wire [1-1:0] fontread_axi_m_rvalid;
wire [1-1:0] fontread_axi_m_rready;
wire [C_FONTREAD_AXI_M_DATA_WIDTH-1:0] fontread_axi_m_rdata;
wire [1-1:0] fontread_axi_m_rlast;
// AXI4-Stream master (write_only) interface dataout_axis_m
wire [1-1:0] dataout_axis_m_tvalid;
wire [1-1:0] dataout_axis_m_tready;
wire [C_DATAOUT_AXIS_M_TDATA_WIDTH-1:0] dataout_axis_m_tdata;
wire [C_DATAOUT_AXIS_M_TDATA_WIDTH/8-1:0] dataout_axis_m_tkeep;
wire [1-1:0] dataout_axis_m_tlast;
//AXI4LITE control signals
wire [1-1:0] s_axi_control_awvalid;
wire [1-1:0] s_axi_control_awready;
wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0] s_axi_control_awaddr;
wire [1-1:0] s_axi_control_wvalid;
wire [1-1:0] s_axi_control_wready;
wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0] s_axi_control_wdata;
wire [C_S_AXI_CONTROL_DATA_WIDTH/8-1:0] s_axi_control_wstrb;
wire [1-1:0] s_axi_control_arvalid;
wire [1-1:0] s_axi_control_arready;
wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0] s_axi_control_araddr;
wire [1-1:0] s_axi_control_rvalid;
wire [1-1:0] s_axi_control_rready;
wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0] s_axi_control_rdata;
wire [2-1:0] s_axi_control_rresp;
wire [1-1:0] s_axi_control_bvalid;
wire [1-1:0] s_axi_control_bready;
wire [2-1:0] s_axi_control_bresp;
wire interrupt;

// DUT instantiation
rtc_gen #(
  .C_S_AXI_CONTROL_ADDR_WIDTH   ( C_S_AXI_CONTROL_ADDR_WIDTH   ),
  .C_S_AXI_CONTROL_DATA_WIDTH   ( C_S_AXI_CONTROL_DATA_WIDTH   ),
  .C_FONTREAD_AXI_M_ADDR_WIDTH  ( C_FONTREAD_AXI_M_ADDR_WIDTH  ),
  .C_FONTREAD_AXI_M_DATA_WIDTH  ( C_FONTREAD_AXI_M_DATA_WIDTH  ),
  .C_DATAOUT_AXIS_M_TDATA_WIDTH ( C_DATAOUT_AXIS_M_TDATA_WIDTH )
)
inst_dut (
  .ap_clk                 ( ap_clk                 ),
  .ap_rst_n               ( ap_rst_n               ),
  .fontread_axi_m_awvalid ( fontread_axi_m_awvalid ),
  .fontread_axi_m_awready ( fontread_axi_m_awready ),
  .fontread_axi_m_awaddr  ( fontread_axi_m_awaddr  ),
  .fontread_axi_m_awlen   ( fontread_axi_m_awlen   ),
  .fontread_axi_m_wvalid  ( fontread_axi_m_wvalid  ),
  .fontread_axi_m_wready  ( fontread_axi_m_wready  ),
  .fontread_axi_m_wdata   ( fontread_axi_m_wdata   ),
  .fontread_axi_m_wstrb   ( fontread_axi_m_wstrb   ),
  .fontread_axi_m_wlast   ( fontread_axi_m_wlast   ),
  .fontread_axi_m_bvalid  ( fontread_axi_m_bvalid  ),
  .fontread_axi_m_bready  ( fontread_axi_m_bready  ),
  .fontread_axi_m_arvalid ( fontread_axi_m_arvalid ),
  .fontread_axi_m_arready ( fontread_axi_m_arready ),
  .fontread_axi_m_araddr  ( fontread_axi_m_araddr  ),
  .fontread_axi_m_arlen   ( fontread_axi_m_arlen   ),
  .fontread_axi_m_rvalid  ( fontread_axi_m_rvalid  ),
  .fontread_axi_m_rready  ( fontread_axi_m_rready  ),
  .fontread_axi_m_rdata   ( fontread_axi_m_rdata   ),
  .fontread_axi_m_rlast   ( fontread_axi_m_rlast   ),
  .dataout_axis_m_tvalid  ( dataout_axis_m_tvalid  ),
  .dataout_axis_m_tready  ( dataout_axis_m_tready  ),
  .dataout_axis_m_tdata   ( dataout_axis_m_tdata   ),
  .dataout_axis_m_tkeep   ( dataout_axis_m_tkeep   ),
  .dataout_axis_m_tlast   ( dataout_axis_m_tlast   ),
  .s_axi_control_awvalid  ( s_axi_control_awvalid  ),
  .s_axi_control_awready  ( s_axi_control_awready  ),
  .s_axi_control_awaddr   ( s_axi_control_awaddr   ),
  .s_axi_control_wvalid   ( s_axi_control_wvalid   ),
  .s_axi_control_wready   ( s_axi_control_wready   ),
  .s_axi_control_wdata    ( s_axi_control_wdata    ),
  .s_axi_control_wstrb    ( s_axi_control_wstrb    ),
  .s_axi_control_arvalid  ( s_axi_control_arvalid  ),
  .s_axi_control_arready  ( s_axi_control_arready  ),
  .s_axi_control_araddr   ( s_axi_control_araddr   ),
  .s_axi_control_rvalid   ( s_axi_control_rvalid   ),
  .s_axi_control_rready   ( s_axi_control_rready   ),
  .s_axi_control_rdata    ( s_axi_control_rdata    ),
  .s_axi_control_rresp    ( s_axi_control_rresp    ),
  .s_axi_control_bvalid   ( s_axi_control_bvalid   ),
  .s_axi_control_bready   ( s_axi_control_bready   ),
  .s_axi_control_bresp    ( s_axi_control_bresp    ),
  .interrupt              ( interrupt              )
);

// Master Control instantiation
control_rtc_gen_vip inst_control_rtc_gen_vip (
  .aclk          ( ap_clk                ),
  .aresetn       ( ap_rst_n              ),
  .m_axi_awvalid ( s_axi_control_awvalid ),
  .m_axi_awready ( s_axi_control_awready ),
  .m_axi_awaddr  ( s_axi_control_awaddr  ),
  .m_axi_wvalid  ( s_axi_control_wvalid  ),
  .m_axi_wready  ( s_axi_control_wready  ),
  .m_axi_wdata   ( s_axi_control_wdata   ),
  .m_axi_wstrb   ( s_axi_control_wstrb   ),
  .m_axi_arvalid ( s_axi_control_arvalid ),
  .m_axi_arready ( s_axi_control_arready ),
  .m_axi_araddr  ( s_axi_control_araddr  ),
  .m_axi_rvalid  ( s_axi_control_rvalid  ),
  .m_axi_rready  ( s_axi_control_rready  ),
  .m_axi_rdata   ( s_axi_control_rdata   ),
  .m_axi_rresp   ( s_axi_control_rresp   ),
  .m_axi_bvalid  ( s_axi_control_bvalid  ),
  .m_axi_bready  ( s_axi_control_bready  ),
  .m_axi_bresp   ( s_axi_control_bresp   )
);

control_rtc_gen_vip_mst_t  ctrl;

// Slave MM VIP instantiation
slv_fontread_axi_m_vip inst_slv_fontread_axi_m_vip (
  .aclk          ( ap_clk                 ),
  .aresetn       ( ap_rst_n               ),
  .s_axi_awvalid ( fontread_axi_m_awvalid ),
  .s_axi_awready ( fontread_axi_m_awready ),
  .s_axi_awaddr  ( fontread_axi_m_awaddr  ),
  .s_axi_awlen   ( fontread_axi_m_awlen   ),
  .s_axi_wvalid  ( fontread_axi_m_wvalid  ),
  .s_axi_wready  ( fontread_axi_m_wready  ),
  .s_axi_wdata   ( fontread_axi_m_wdata   ),
  .s_axi_wstrb   ( fontread_axi_m_wstrb   ),
  .s_axi_wlast   ( fontread_axi_m_wlast   ),
  .s_axi_bvalid  ( fontread_axi_m_bvalid  ),
  .s_axi_bready  ( fontread_axi_m_bready  ),
  .s_axi_arvalid ( fontread_axi_m_arvalid ),
  .s_axi_arready ( fontread_axi_m_arready ),
  .s_axi_araddr  ( fontread_axi_m_araddr  ),
  .s_axi_arlen   ( fontread_axi_m_arlen   ),
  .s_axi_rvalid  ( fontread_axi_m_rvalid  ),
  .s_axi_rready  ( fontread_axi_m_rready  ),
  .s_axi_rdata   ( fontread_axi_m_rdata   ),
  .s_axi_rlast   ( fontread_axi_m_rlast   )
);


slv_fontread_axi_m_vip_slv_mem_t   fontread_axi_m;
slv_fontread_axi_m_vip_slv_t   fontread_axi_m_slv;
// AXI4-Stream master (write_only) interface dataout_axis_m

// AXI4-Stream VIP instantiation
dataout_axis_m_vip inst_dataout_axis_m_vip (
  .aclk          ( ap_clk                ),
  .aresetn       ( ap_rst_n              ),
  .s_axis_tvalid ( dataout_axis_m_tvalid ),
  .s_axis_tready ( dataout_axis_m_tready ),
  .s_axis_tdata  ( dataout_axis_m_tdata  ),
  .s_axis_tkeep  ( dataout_axis_m_tkeep  ),
  .s_axis_tlast  ( dataout_axis_m_tlast  )
);


dataout_axis_m_vip_slv_t   dataout_axis_m_slv;

parameter NUM_AXIS_MST = 0;
parameter NUM_AXIS_SLV = 1;

bit               error_found = 0;

///////////////////////////////////////////////////////////////////////////
// Pointer for interface : fontread_axi_m
bit [63:0] read_addr_ptr = 64'h0;

axi4stream_monitor_transaction            slv_monitor_tran_q[1][$];
string                                    slv_name_q[$];

axi4stream_monitor_transaction            dataout_axis_m_slv_monitor_transaction;


task automatic system_reset_sequence(input integer unsigned width = 20);
  $display("%t : Starting System Reset Sequence", $time);
  fork
    ap_rst_n_sequence(25);
    
  join

  // Clear slave queue
  for(xil_axi4stream_uint slv=0; slv< NUM_AXIS_SLV; slv++) begin
    slv_monitor_tran_q[slv] = {};
  end

endtask


/////////////////////////////////////////////////////////////////////////////////////////////////
// Generate a random 32bit number
function bit [31:0] get_random_4bytes();
  bit [31:0] rptr;
  ptr_random_failed: assert(std::randomize(rptr));
  return(rptr);
endfunction

/////////////////////////////////////////////////////////////////////////////////////////////////
// Generate a random 64bit 4k aligned address pointer.
function bit [63:0] get_random_ptr();
  bit [63:0] rptr;
  ptr_random_failed: assert(std::randomize(rptr));
  rptr[31:0] &= ~(32'h00000fff);
  return(rptr);
endfunction

/////////////////////////////////////////////////////////////////////////////////////////////////
// Control interface non-blocking write
// The task will return when the transaction has been accepted by the driver. It will be some
// amount of time before it will appear on the interface.
task automatic write_register (input bit [31:0] addr_in, input bit [31:0] data);
  axi_transaction   wr_xfer;
  wr_xfer = ctrl.wr_driver.create_transaction("wr_xfer");
  assert(wr_xfer.randomize() with {addr == addr_in;});
  wr_xfer.set_data_beat(0, data);
  ctrl.wr_driver.send(wr_xfer);
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// Control interface blocking write
// The task will return when the BRESP has been returned from the kernel.
task automatic blocking_write_register (input bit [31:0] addr_in, input bit [31:0] data);
  axi_transaction   wr_xfer;
  axi_transaction   wr_rsp;
  wr_xfer = ctrl.wr_driver.create_transaction("wr_xfer");
  wr_xfer.set_driver_return_item_policy(XIL_AXI_PAYLOAD_RETURN);
  assert(wr_xfer.randomize() with {addr == addr_in;});
  wr_xfer.set_data_beat(0, data);
  ctrl.wr_driver.send(wr_xfer);
  ctrl.wr_driver.wait_rsp(wr_rsp);
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// Control interface blocking read
// The task will return when the BRESP has been returned from the kernel.
task automatic read_register (input bit [31:0] addr, output bit [31:0] rddata);
  axi_transaction   rd_xfer;
  axi_transaction   rd_rsp;
  bit [31:0] rd_value;
  rd_xfer = ctrl.rd_driver.create_transaction("rd_xfer");
  rd_xfer.set_addr(addr);
  rd_xfer.set_driver_return_item_policy(XIL_AXI_PAYLOAD_RETURN);
  ctrl.rd_driver.send(rd_xfer);
  ctrl.rd_driver.wait_rsp(rd_rsp);
  rd_value = rd_rsp.get_data_beat(0);
  rddata = rd_value;
endtask



/////////////////////////////////////////////////////////////////////////////////////////////////
// Poll the Control interface status register.
// This will poll until the DONE flag in the status register is asserted.
task automatic poll_done_register ();
  bit [31:0] rd_value;
  do begin
    read_register(KRNL_CTRL_REG_ADDR, rd_value);
  end while ((rd_value & CTRL_DONE_MASK) == 0);
endtask

// This will poll until the IDLE flag in the status register is asserted.
task automatic poll_idle_register ();
  bit [31:0] rd_value;
  do begin
    read_register(KRNL_CTRL_REG_ADDR, rd_value);
  end while ((rd_value & CTRL_IDLE_MASK) == 0);
endtask


/////////////////////////////////////////////////////////////////////////////////////////////////
// Start the control VIP, SLAVE memory models and AXI4-Stream.
task automatic start_vips();
  $display("///////////////////////////////////////////////////////////////////////////");
  $display("Control Master: ctrl");
  ctrl = new("ctrl", rtc_gen_tb.inst_control_rtc_gen_vip.inst.IF);
  ctrl.start_master();

  $display("///////////////////////////////////////////////////////////////////////////");
  $display("Starting Memory slave: fontread_axi_m");
  fontread_axi_m = new("fontread_axi_m", rtc_gen_tb.inst_slv_fontread_axi_m_vip.inst.IF);
  fontread_axi_m.start_slave();

  $display("///////////////////////////////////////////////////////////////////////////");
  $display("Starting AXI4-Stream slv: dataout_axis_m");
  dataout_axis_m_slv = new("dataout_axis_m", rtc_gen_tb.inst_dataout_axis_m_vip.inst.IF);
  dataout_axis_m_slv.start_slave();

  slv_name_q.push_back("dataout_axis_m");

endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// For each of the connected slave interfaces, set the Slave to not de-assert WREADY at any time.
// This will show the fastest outbound bandwidth from the WRITE channel.
task automatic slv_no_backpressure_wready();
  axi_ready_gen     rgen;
  $display("%t - Applying slv_no_backpressure_wready", $time);

  rgen = new("fontread_axi_m_no_backpressure_wready");
  rgen.set_ready_policy(XIL_AXI_READY_GEN_NO_BACKPRESSURE);
  fontread_axi_m.wr_driver.set_wready_gen(rgen);

endtask

task automatic slv_no_backpressure_tready();
  $display("%t - Applying slv_no_backpressure_tready", $time);
  fork

  begin
    axi4stream_ready_gen rgen_dataout_axis_m_slv;
    rgen_dataout_axis_m_slv = new("dataout_axis_m_slv_no_backpressure_ready");
    rgen_dataout_axis_m_slv.set_ready_policy(XIL_AXI4STREAM_READY_GEN_NO_BACKPRESSURE);
    dataout_axis_m_slv.driver.send_tready(rgen_dataout_axis_m_slv);
  end

join_none
endtask


/////////////////////////////////////////////////////////////////////////////////////////////////
// For each of the connected slave interfaces, apply a WREADY policy to introduce backpressure.
// Based on the simulation seed the order/shape of the WREADY per-channel will be different.
task automatic slv_random_backpressure_wready();
  axi_ready_gen     rgen;
  $display("%t - Applying slv_random_backpressure_wready", $time);

  rgen = new("fontread_axi_m_random_backpressure_wready");
  rgen.set_ready_policy(XIL_AXI_READY_GEN_RANDOM);
  rgen.set_low_time_range(0,12);
  rgen.set_high_time_range(1,12);
  rgen.set_event_count_range(3,5);
  fontread_axi_m.wr_driver.set_wready_gen(rgen);

endtask


task automatic slv_random_backpressure_tready();
  $display("%t - Applying slv_random_backpressure_tready", $time);
  fork

  begin
    axi4stream_ready_gen rgen_dataout_axis_m_slv;
    rgen_dataout_axis_m_slv = new("dataout_axis_m_slv_random_backpressure_wready");
    rgen_dataout_axis_m_slv.set_ready_policy(XIL_AXI4STREAM_READY_GEN_RANDOM);
    rgen_dataout_axis_m_slv.set_low_time_range(0,12);
    rgen_dataout_axis_m_slv.set_high_time_range(1,12);
    rgen_dataout_axis_m_slv.set_event_count_range(3,5);
    dataout_axis_m_slv.driver.send_tready(rgen_dataout_axis_m_slv);
  end

join_none
endtask


/////////////////////////////////////////////////////////////////////////////////////////////////
// For each of the connected slave interfaces, force the memory model to not insert any inter-beat
// gaps on the READ channel.
task automatic slv_no_delay_rvalid();
  $display("%t - Applying slv_no_delay_rvalid", $time);

  fontread_axi_m.mem_model.set_inter_beat_gap_delay_policy(XIL_AXI_MEMORY_DELAY_FIXED);
  fontread_axi_m.mem_model.set_inter_beat_gap(0);

endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// For each of the connected slave interfaces, Allow the memory model to insert any inter-beat
// gaps on the READ channel.
task automatic slv_random_delay_rvalid();
  $display("%t - Applying slv_random_delay_rvalid", $time);

  fontread_axi_m.mem_model.set_inter_beat_gap_delay_policy(XIL_AXI_MEMORY_DELAY_RANDOM);
  fontread_axi_m.mem_model.set_inter_beat_gap_range(0,10);

endtask

task automatic start_stream_tready();
bit choose_pressure_type = 0;
RAND_TREADY_PRESSURE_FAILED: assert(std::randomize(choose_pressure_type));
case(choose_pressure_type)
0: slv_no_backpressure_tready();
1: slv_random_backpressure_tready();
endcase
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// Check to ensure, following reset the value of the register is 0.
// Check that only the width of the register bits can be written.
task automatic check_register_value(input bit [31:0] addr_in, input integer unsigned register_width, output bit error_found);
  bit [31:0] rddata;
  bit [31:0] mask_data;
  error_found = 0;
  if (register_width < 32) begin
    mask_data = (1 << register_width) - 1;
  end else begin
    mask_data = 32'hffffffff;
  end
  read_register(addr_in, rddata);
  if (rddata != 32'h0) begin
    $error("Initial value mismatch: A:0x%0x : Expected 0x%x -> Got 0x%x", addr_in, 0, rddata);
    error_found = 1;
  end
  blocking_write_register(addr_in, 32'hffffffff);
  read_register(addr_in, rddata);
  if (rddata != mask_data) begin
    $error("Initial value mismatch: A:0x%0x : Expected 0x%x -> Got 0x%x", addr_in, mask_data, rddata);
    error_found = 1;
  end
endtask


/////////////////////////////////////////////////////////////////////////////////////////////////
// For each of the scalar registers, check:
// * reset value
// * correct number bits set on a write
task automatic check_scalar_registers(output bit error_found);
  bit tmp_error_found = 0;
  error_found = 0;
  $display("%t : Checking post reset values of scalar registers", $time);

  ///////////////////////////////////////////////////////////////////////////
  //Check ID 0: work_mode (0x010)
  check_register_value(32'h010, 32, tmp_error_found);
  error_found |= tmp_error_found;

  ///////////////////////////////////////////////////////////////////////////
  //Check ID 1: cs_count (0x018)
  check_register_value(32'h018, 32, tmp_error_found);
  error_found |= tmp_error_found;

  ///////////////////////////////////////////////////////////////////////////
  //Check ID 2: time_format (0x020)
  check_register_value(32'h020, 32, tmp_error_found);
  error_found |= tmp_error_found;

  ///////////////////////////////////////////////////////////////////////////
  //Check ID 3: time_set_value (0x028)
  check_register_value(32'h028, 32, tmp_error_found);
  error_found |= tmp_error_found;

  ///////////////////////////////////////////////////////////////////////////
  //Check ID 4: time_set_en (0x030)
  check_register_value(32'h030, 32, tmp_error_found);
  error_found |= tmp_error_found;

endtask


task automatic set_kernel_arguments_load();
  $display("%t : Setting Kernel Arguments for Font Load Mode", $time);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 0: work_mode (0x010) 
  write_register(32'h010, 32'h0);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 1: cs_count (0x018)
  write_register(32'h018, 32'hff);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 2: time_format (0x020)
  write_register(32'h020, 32'h1);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 3: time_set_val (0x028)
  write_register(32'h028, 32'h00_00_00_00);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 4: time_set_en (0x030)
  write_register(32'h030, 32'h1);

endtask


task automatic set_kernel_arguments_gen();
  $display("%t : Setting Kernel Arguments for Image Generation Mode", $time);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 0: work_mode (0x010) 
  write_register(32'h010, 32'h1);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 1: cs_count (0x018)
  write_register(32'h018, 32'hff);
  
  ///////////////////////////////////////////////////////////////////////////
  //Write ID 2: time_format (0x020)
  write_register(32'h020, 32'h1);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 3: time_set_val (0x028)
  write_register(32'h028, 32'h00_00_00_00);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 4: time_set_en (0x030)
  write_register(32'h030, 32'h0);

endtask


task automatic check_pointer_registers(output bit error_found);
  bit tmp_error_found = 0;
  ///////////////////////////////////////////////////////////////////////////
  //Check the reset states of the pointer registers.
  $display("%t : Checking post reset values of pointer registers", $time);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 6: read_addr (0x040)
  check_register_value(32'h040, 32, tmp_error_found);
  error_found |= tmp_error_found;

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 6: read_addr (0x044)
  check_register_value(32'h044, 32, tmp_error_found);
  error_found |= tmp_error_found;

endtask

task automatic set_memory_pointers();
  ///////////////////////////////////////////////////////////////////////////
  //Randomly generate memory pointers.
  read_addr_ptr = get_random_ptr();

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 6: read_addr (0x040) -> Randomized 4k aligned address (Global memory, lower 32 bits)
  write_register(32'h040, read_addr_ptr[31:0]);

  ///////////////////////////////////////////////////////////////////////////
  //Write ID 6: read_addr (0x044) -> Randomized 4k aligned address (Global memory, upper 32 bits)
  write_register(32'h044, read_addr_ptr[63:32]);

endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// Backdoor fill the font buffer memory.
function void fontread_axi_m_fill_font_buffer(
  input bit [63:0] ptr
);
  reg [31:0] mem [0:FONT_LIB_SIZE-1];
  $readmemh("font_sim_data.txt", mem);
  for (longint unsigned index = 0; index < FONT_LIB_SIZE; index++) begin
    fontread_axi_m.mem_model.backdoor_memory_write_4byte(ptr + (index * 4), mem[index]);
  end
endfunction

bit choose_pressure_type = 0;
bit axis_choose_pressure_type = 0;
bit [1-1:0] axis_tlast_received;

/////////////////////////////////////////////////////////////////////////////////////////////////
// Set up the kernel for operation and set the kernel START bit.
// The task will poll the DONE bit and check the results when complete.
task automatic multiple_iteration(input integer unsigned num_iterations, output bit error_found);
  error_found = 0;

  $display("Starting: multiple_iteration");
  for (integer unsigned iter = 0; iter < num_iterations; iter++) begin

    axis_tlast_received = 1'h0;
    $display("Starting iteration: %d / %d", iter+1, num_iterations);
    RAND_WREADY_PRESSURE_FAILED: assert(std::randomize(choose_pressure_type));
    case(choose_pressure_type)
      0: slv_no_backpressure_wready();
      1: slv_random_backpressure_wready();
    endcase
    RAND_RVALID_PRESSURE_FAILED: assert(std::randomize(choose_pressure_type));
    case(choose_pressure_type)
      0: slv_no_delay_rvalid();
      1: slv_random_delay_rvalid();
    endcase

    $display("Start font library loading");

    set_kernel_arguments_load();
    set_memory_pointers();
    fontread_axi_m_fill_font_buffer(read_addr_ptr);
    // Check that Kernel is IDLE before starting.
    poll_idle_register();
    ///////////////////////////////////////////////////////////////////////////
    //Start transfers
    blocking_write_register(KRNL_CTRL_REG_ADDR, CTRL_START_MASK);

    ctrl.wait_drivers_idle();
///////////////////////////////////////////////////////////////////////////
    //Wait for interrupt being asserted or poll done register
    //@(posedge interrupt);
    poll_done_register();

    $display("Start image output");

    set_kernel_arguments_gen();
    poll_idle_register();
    blocking_write_register(KRNL_CTRL_REG_ADDR, CTRL_START_MASK);
    fork
      start_stream_tready();
      
    join_none    
    
    ctrl.wait_drivers_idle();
    poll_done_register();

    //error_found |= check_axis_result();

    $display("Finished iteration: %d / %d", iter+1, num_iterations);
  end
 endtask

function automatic void data_comparison(bit[31:0] slv_data, bit[31:0] expected_data, ref xil_axi4stream_uint err_cnt, ref bit error_found);
  if (slv_data != expected_data ) begin
    $error("Expected data beat(%h) does not match received slave monitor data beat(%h).", expected_data, slv_data);
    err_cnt++;
    error_found |=1;
  end
endfunction

function automatic xil_axi4stream_uint get_total_bytes(ref axi4stream_monitor_transaction mon[$]);
 xil_axi4stream_uint total_bytes;
 xil_axi4stream_uint size;
 xil_axi4stream_uint dw;
 xil_axi4stream_strb_beat keep;
 size = mon.size();
 keep = mon[size-1].get_keep_beat();
 dw = mon[0].get_data_width()/8;
 total_bytes = (size-1)*dw;
 for (xil_axi4stream_uint i = 0; i < dw; i++) begin
   if (keep[i] == 1'b1 || dw == 1) begin
     total_bytes++;
   end
   else begin
     break;
   end
 end
 return total_bytes;
endfunction


function automatic void check_monitor_result(ref axi4stream_monitor_transaction slv_mon[$], ref axi4stream_monitor_transaction mst_mon[$], ref xil_axi4stream_uint err_cnt, ref bit error_found);
  axi4stream_monitor_transaction  mst_tran;
  axi4stream_monitor_transaction  slv_tran;
  xil_axi4stream_data_beat        mst_data;
  xil_axi4stream_data_beat        slv_data;
  bit[31:0]                       mst_word;
  bit[31:0]                       slv_word;
  xil_axi4stream_uint             slv_trans_size;
  xil_axi4stream_uint             mst_trans_size;
  xil_axi4stream_uint             slv_data_width;
  xil_axi4stream_uint             mst_data_width;

  slv_data_width = slv_mon[0].get_data_width();
  mst_data_width = mst_mon[0].get_data_width();
  if(get_total_bytes(slv_mon) != get_total_bytes(mst_mon)) begin
    $error("Number of bytes in slv_mon (%d) does not equal mst_mon(%d)",get_total_bytes(slv_mon),get_total_bytes(mst_mon));
    error_found = 1;
    err_cnt     += 1;
    return;
  end
  mst_trans_size = mst_mon.size();
  slv_trans_size = slv_mon.size();
  if (mst_data_width >= slv_data_width) begin
    for(xil_axi4stream_uint mst_trans =0; mst_trans< mst_trans_size; mst_trans++) begin
      xil_axi4stream_strb_beat mst_keep ;
      bit slv_last;
      bit mst_last;
      mst_tran = mst_mon.pop_front();
      mst_last = mst_tran.get_last();
      mst_data = mst_tran.get_data_beat();
      if (mst_last == 1'b1) begin
        mst_keep = mst_tran.get_keep_beat();
      end

      for (xil_axi4stream_uint slv_trans = 0; slv_trans < mst_data_width / slv_data_width; slv_trans++) begin
        xil_axi4stream_strb_beat slv_keep ;
        slv_tran = slv_mon.pop_front();
        slv_data = slv_tran.get_data_beat();
        slv_last = slv_tran.get_last();
         if (slv_last == 1'b1) begin
          if (mst_last == 1'b0) begin
            $error("slv_mon received tlast, but mst_mon still has transfers");
          end

          slv_keep = slv_tran.get_keep_beat();

        end
         for(xil_axi4stream_uint word_cnt=0; word_cnt <(slv_data_width/32); word_cnt++) begin
          if (~slv_last || slv_keep[word_cnt*(slv_data_width/32/8)+:32/8] == '1) begin
            for(xil_axi4stream_uint byte_cnt=0; byte_cnt <4; byte_cnt++) begin
              slv_word[(byte_cnt*8)+:8] = slv_data[(word_cnt*32+byte_cnt*8)+:8];
              mst_word[(byte_cnt*8)+:8] = mst_data[(slv_trans*slv_data_width)+(word_cnt*32+byte_cnt*8)+:8];
            end
            data_comparison(slv_word,mst_word+1,err_cnt,error_found);
            if (err_cnt > 5) begin
              $display("Too many errors found. Exiting check...");
              return;
            end
          end
        end
        if (slv_last) begin
          break;
        end
      end
    end
  end
  else begin
    for(xil_axi4stream_uint slv_trans =0; slv_trans< slv_trans_size; slv_trans++) begin
      xil_axi4stream_strb_beat slv_keep ;
      bit mst_last;
      bit slv_last;
      slv_tran = slv_mon.pop_front();
      slv_last = slv_tran.get_last();
      slv_data = slv_tran.get_data_beat();
      if (slv_last == 1'b1) begin
        slv_keep = slv_tran.get_keep_beat();
      end

      for (xil_axi4stream_uint mst_trans = 0; mst_trans < slv_data_width / mst_data_width; mst_trans++) begin
        xil_axi4stream_strb_beat mst_keep ;
        mst_tran = mst_mon.pop_front();
        mst_data = mst_tran.get_data_beat();
        mst_last = mst_tran.get_last();
         if (mst_last == 1'b1) begin
          if (slv_last == 1'b0) begin
            $error("mst_mon received tlast, but slv_mon still has tranfers");
          end

          mst_keep = mst_tran.get_keep_beat();

        end
         for(xil_axi4stream_uint word_cnt=0; word_cnt <(mst_data_width/32); word_cnt++) begin
          if (~mst_last || mst_keep[word_cnt*(mst_data_width/32/8)+:32/8] == '1) begin
            for(xil_axi4stream_uint byte_cnt=0; byte_cnt <4; byte_cnt++) begin
              mst_word[(byte_cnt*8)+:8] = mst_data[(word_cnt*32+byte_cnt*8)+:8];
              slv_word[(byte_cnt*8)+:8] = slv_data[(mst_trans*mst_data_width)+(word_cnt*32+byte_cnt*8)+:8];
            end
          //   $info("loop slv_trans.mst_trans.word_cnt.slv_data_index %d.%d.%d", slv_trans, mst_trans, word_cnt, (mst_trans*mst_data_width)+(word_cnt*32));
            data_comparison(slv_word, mst_word+1,err_cnt,error_found);
            if (err_cnt > 5) begin
              $display("Too many errors found. Exiting check...");
              return;
            end
          end
        end
        if (mst_last) begin
          break;
        end
      end
    end
  end
endfunction

function automatic void check_autogen_monitor_result(ref axi4stream_monitor_transaction slv_mon[$], ref xil_axi4stream_uint err_cnt, ref bit error_found);
  axi4stream_monitor_transaction  slv_tran;
  xil_axi4stream_data_beat        slv_data;
  xil_axi4stream_ulong            shift_bit;
  bit[31:0]                       slv_word;
  bit [31:0]                      auto_gen_word =0;
  bit [31:0]                      final_word;
   while(slv_mon.size() >0) begin
    slv_tran = slv_mon.pop_front();
    slv_data = slv_tran.get_data_beat();
    if (slv_tran.get_data_width() <32) begin
      slv_word = slv_data;
      auto_gen_word +=1;
      shift_bit = (1<< slv_tran.get_data_width()) -1;
      final_word = auto_gen_word & shift_bit;
      data_comparison(slv_word,final_word, err_cnt, error_found);
      if (err_cnt > 5) begin
       $display("Too many errors found. Exiting check...");
       return;
      end
    end else begin
      for(xil_axi4stream_uint word_cnt=0; word_cnt <(slv_tran.get_data_width()/32); word_cnt++) begin
        slv_word = slv_data[(word_cnt*32)+:32];
        auto_gen_word +=1;
        data_comparison(slv_word,auto_gen_word, err_cnt, error_found);
        if (err_cnt > 5) begin
          $display("Too many errors found. Exiting check...");
          return;
        end
      end
    end
  end
endfunction

function automatic bit check_axis_result();
  //compare slave side monitor transaction with master side monitor transaction if the data width match
  string      mst_s="";
  string      slv_s="";

  xil_axi4stream_uint increment_cnt =0 ;
  xil_axi4stream_uint comparison_cnt =0 ;
  xil_axi4stream_uint slv_monitor_data_width_q[];
  xil_axi4stream_uint axis_error_cnt =0;
  bit axis_error_found =0;
  xil_axi4stream_uint unique_slv_data_width_q[$];
  xil_axi4stream_uint unique_cnt;
  slv_monitor_data_width_q = new[1];

  for(xil_axi4stream_uint slv=0; slv< NUM_AXIS_SLV; slv++) begin
    slv_s = slv_name_q.pop_front();
    slv_monitor_data_width_q[slv] = slv_monitor_tran_q[slv][0].get_data_width();
    comparison_cnt = 0;

// $info("Check slv_monitor_data_width_q[%d] =%h  data is autogenerated ", slv, slv_monitor_data_width_q[slv]);
        check_autogen_monitor_result(slv_monitor_tran_q[slv],axis_error_cnt,axis_error_found);
  end

  return(axis_error_found);
endfunction

initial begin
  #200001;
  fork

  forever begin
    dataout_axis_m_slv.monitor.item_collected_port.get(dataout_axis_m_slv_monitor_transaction);
    if (dataout_axis_m_slv_monitor_transaction.get_last())
      axis_tlast_received[0] = 1'b1;
    slv_monitor_tran_q[0].push_back(dataout_axis_m_slv_monitor_transaction);
  end

join_none
end

/////////////////////////////////////////////////////////////////////////////////////////////////
//Instantiate AXI4 LITE VIP
initial begin : STIMULUS
  #200000;
  start_vips();
  //check_scalar_registers(error_found);
  //if (error_found == 1) begin
  //  $display( "Test Failed!");
  //  $finish();
  //end

  //check_pointer_registers(error_found);
  //if (error_found == 1) begin
  //  $display( "Test Failed!");
  //  $finish();
  //end

  multiple_iteration(1, error_found);
//  if (error_found == 1) begin
//    $display( "Test Failed!");
//    $finish();
//  end

//  multiple_iteration(5, error_found);

  if (error_found == 1) begin
    $display( "Test Failed!");
    $finish();
  end else begin
    $display( "Test completed successfully");
  end
  $finish;
end


initial begin
  $dumpfile("rtc_gen.vcd");
  $dumpvars(0, rtc_gen_tb);
end



endmodule
`default_nettype wire

