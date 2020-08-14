// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
// Date        : Wed May  6 15:18:20 2020
// Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
// Command     : write_verilog -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_runOnfpga_1_0_stub.v
// Design      : pfm_dynamic_runOnfpga_1_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xcu200-fsgd2104-2-e
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "runOnfpga,Vivado 2020.1" *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(stall_start_ext, stall_done_ext, 
  stall_start_str, stall_done_str, stall_start_int, stall_done_int, s_axi_control_AWADDR, 
  s_axi_control_AWVALID, s_axi_control_AWREADY, s_axi_control_WDATA, s_axi_control_WSTRB, 
  s_axi_control_WVALID, s_axi_control_WREADY, s_axi_control_BRESP, s_axi_control_BVALID, 
  s_axi_control_BREADY, s_axi_control_ARADDR, s_axi_control_ARVALID, 
  s_axi_control_ARREADY, s_axi_control_RDATA, s_axi_control_RRESP, s_axi_control_RVALID, 
  s_axi_control_RREADY, ap_clk, ap_rst_n, event_done, interrupt, event_start, 
  m_axi_maxiport0_AWADDR, m_axi_maxiport0_AWLEN, m_axi_maxiport0_AWSIZE, 
  m_axi_maxiport0_AWBURST, m_axi_maxiport0_AWLOCK, m_axi_maxiport0_AWREGION, 
  m_axi_maxiport0_AWCACHE, m_axi_maxiport0_AWPROT, m_axi_maxiport0_AWQOS, 
  m_axi_maxiport0_AWVALID, m_axi_maxiport0_AWREADY, m_axi_maxiport0_WDATA, 
  m_axi_maxiport0_WSTRB, m_axi_maxiport0_WLAST, m_axi_maxiport0_WVALID, 
  m_axi_maxiport0_WREADY, m_axi_maxiport0_BRESP, m_axi_maxiport0_BVALID, 
  m_axi_maxiport0_BREADY, m_axi_maxiport0_ARADDR, m_axi_maxiport0_ARLEN, 
  m_axi_maxiport0_ARSIZE, m_axi_maxiport0_ARBURST, m_axi_maxiport0_ARLOCK, 
  m_axi_maxiport0_ARREGION, m_axi_maxiport0_ARCACHE, m_axi_maxiport0_ARPROT, 
  m_axi_maxiport0_ARQOS, m_axi_maxiport0_ARVALID, m_axi_maxiport0_ARREADY, 
  m_axi_maxiport0_RDATA, m_axi_maxiport0_RRESP, m_axi_maxiport0_RLAST, 
  m_axi_maxiport0_RVALID, m_axi_maxiport0_RREADY, m_axi_maxiport1_AWADDR, 
  m_axi_maxiport1_AWLEN, m_axi_maxiport1_AWSIZE, m_axi_maxiport1_AWBURST, 
  m_axi_maxiport1_AWLOCK, m_axi_maxiport1_AWREGION, m_axi_maxiport1_AWCACHE, 
  m_axi_maxiport1_AWPROT, m_axi_maxiport1_AWQOS, m_axi_maxiport1_AWVALID, 
  m_axi_maxiport1_AWREADY, m_axi_maxiport1_WDATA, m_axi_maxiport1_WSTRB, 
  m_axi_maxiport1_WLAST, m_axi_maxiport1_WVALID, m_axi_maxiport1_WREADY, 
  m_axi_maxiport1_BRESP, m_axi_maxiport1_BVALID, m_axi_maxiport1_BREADY, 
  m_axi_maxiport1_ARADDR, m_axi_maxiport1_ARLEN, m_axi_maxiport1_ARSIZE, 
  m_axi_maxiport1_ARBURST, m_axi_maxiport1_ARLOCK, m_axi_maxiport1_ARREGION, 
  m_axi_maxiport1_ARCACHE, m_axi_maxiport1_ARPROT, m_axi_maxiport1_ARQOS, 
  m_axi_maxiport1_ARVALID, m_axi_maxiport1_ARREADY, m_axi_maxiport1_RDATA, 
  m_axi_maxiport1_RRESP, m_axi_maxiport1_RLAST, m_axi_maxiport1_RVALID, 
  m_axi_maxiport1_RREADY)
/* synthesis syn_black_box black_box_pad_pin="stall_start_ext,stall_done_ext,stall_start_str,stall_done_str,stall_start_int,stall_done_int,s_axi_control_AWADDR[6:0],s_axi_control_AWVALID,s_axi_control_AWREADY,s_axi_control_WDATA[31:0],s_axi_control_WSTRB[3:0],s_axi_control_WVALID,s_axi_control_WREADY,s_axi_control_BRESP[1:0],s_axi_control_BVALID,s_axi_control_BREADY,s_axi_control_ARADDR[6:0],s_axi_control_ARVALID,s_axi_control_ARREADY,s_axi_control_RDATA[31:0],s_axi_control_RRESP[1:0],s_axi_control_RVALID,s_axi_control_RREADY,ap_clk,ap_rst_n,event_done,interrupt,event_start,m_axi_maxiport0_AWADDR[63:0],m_axi_maxiport0_AWLEN[7:0],m_axi_maxiport0_AWSIZE[2:0],m_axi_maxiport0_AWBURST[1:0],m_axi_maxiport0_AWLOCK[1:0],m_axi_maxiport0_AWREGION[3:0],m_axi_maxiport0_AWCACHE[3:0],m_axi_maxiport0_AWPROT[2:0],m_axi_maxiport0_AWQOS[3:0],m_axi_maxiport0_AWVALID,m_axi_maxiport0_AWREADY,m_axi_maxiport0_WDATA[511:0],m_axi_maxiport0_WSTRB[63:0],m_axi_maxiport0_WLAST,m_axi_maxiport0_WVALID,m_axi_maxiport0_WREADY,m_axi_maxiport0_BRESP[1:0],m_axi_maxiport0_BVALID,m_axi_maxiport0_BREADY,m_axi_maxiport0_ARADDR[63:0],m_axi_maxiport0_ARLEN[7:0],m_axi_maxiport0_ARSIZE[2:0],m_axi_maxiport0_ARBURST[1:0],m_axi_maxiport0_ARLOCK[1:0],m_axi_maxiport0_ARREGION[3:0],m_axi_maxiport0_ARCACHE[3:0],m_axi_maxiport0_ARPROT[2:0],m_axi_maxiport0_ARQOS[3:0],m_axi_maxiport0_ARVALID,m_axi_maxiport0_ARREADY,m_axi_maxiport0_RDATA[511:0],m_axi_maxiport0_RRESP[1:0],m_axi_maxiport0_RLAST,m_axi_maxiport0_RVALID,m_axi_maxiport0_RREADY,m_axi_maxiport1_AWADDR[63:0],m_axi_maxiport1_AWLEN[7:0],m_axi_maxiport1_AWSIZE[2:0],m_axi_maxiport1_AWBURST[1:0],m_axi_maxiport1_AWLOCK[1:0],m_axi_maxiport1_AWREGION[3:0],m_axi_maxiport1_AWCACHE[3:0],m_axi_maxiport1_AWPROT[2:0],m_axi_maxiport1_AWQOS[3:0],m_axi_maxiport1_AWVALID,m_axi_maxiport1_AWREADY,m_axi_maxiport1_WDATA[511:0],m_axi_maxiport1_WSTRB[63:0],m_axi_maxiport1_WLAST,m_axi_maxiport1_WVALID,m_axi_maxiport1_WREADY,m_axi_maxiport1_BRESP[1:0],m_axi_maxiport1_BVALID,m_axi_maxiport1_BREADY,m_axi_maxiport1_ARADDR[63:0],m_axi_maxiport1_ARLEN[7:0],m_axi_maxiport1_ARSIZE[2:0],m_axi_maxiport1_ARBURST[1:0],m_axi_maxiport1_ARLOCK[1:0],m_axi_maxiport1_ARREGION[3:0],m_axi_maxiport1_ARCACHE[3:0],m_axi_maxiport1_ARPROT[2:0],m_axi_maxiport1_ARQOS[3:0],m_axi_maxiport1_ARVALID,m_axi_maxiport1_ARREADY,m_axi_maxiport1_RDATA[511:0],m_axi_maxiport1_RRESP[1:0],m_axi_maxiport1_RLAST,m_axi_maxiport1_RVALID,m_axi_maxiport1_RREADY" */;
  output stall_start_ext;
  output stall_done_ext;
  output stall_start_str;
  output stall_done_str;
  output stall_start_int;
  output stall_done_int;
  input [6:0]s_axi_control_AWADDR;
  input s_axi_control_AWVALID;
  output s_axi_control_AWREADY;
  input [31:0]s_axi_control_WDATA;
  input [3:0]s_axi_control_WSTRB;
  input s_axi_control_WVALID;
  output s_axi_control_WREADY;
  output [1:0]s_axi_control_BRESP;
  output s_axi_control_BVALID;
  input s_axi_control_BREADY;
  input [6:0]s_axi_control_ARADDR;
  input s_axi_control_ARVALID;
  output s_axi_control_ARREADY;
  output [31:0]s_axi_control_RDATA;
  output [1:0]s_axi_control_RRESP;
  output s_axi_control_RVALID;
  input s_axi_control_RREADY;
  input ap_clk;
  input ap_rst_n;
  output event_done;
  output interrupt;
  output event_start;
  output [63:0]m_axi_maxiport0_AWADDR;
  output [7:0]m_axi_maxiport0_AWLEN;
  output [2:0]m_axi_maxiport0_AWSIZE;
  output [1:0]m_axi_maxiport0_AWBURST;
  output [1:0]m_axi_maxiport0_AWLOCK;
  output [3:0]m_axi_maxiport0_AWREGION;
  output [3:0]m_axi_maxiport0_AWCACHE;
  output [2:0]m_axi_maxiport0_AWPROT;
  output [3:0]m_axi_maxiport0_AWQOS;
  output m_axi_maxiport0_AWVALID;
  input m_axi_maxiport0_AWREADY;
  output [511:0]m_axi_maxiport0_WDATA;
  output [63:0]m_axi_maxiport0_WSTRB;
  output m_axi_maxiport0_WLAST;
  output m_axi_maxiport0_WVALID;
  input m_axi_maxiport0_WREADY;
  input [1:0]m_axi_maxiport0_BRESP;
  input m_axi_maxiport0_BVALID;
  output m_axi_maxiport0_BREADY;
  output [63:0]m_axi_maxiport0_ARADDR;
  output [7:0]m_axi_maxiport0_ARLEN;
  output [2:0]m_axi_maxiport0_ARSIZE;
  output [1:0]m_axi_maxiport0_ARBURST;
  output [1:0]m_axi_maxiport0_ARLOCK;
  output [3:0]m_axi_maxiport0_ARREGION;
  output [3:0]m_axi_maxiport0_ARCACHE;
  output [2:0]m_axi_maxiport0_ARPROT;
  output [3:0]m_axi_maxiport0_ARQOS;
  output m_axi_maxiport0_ARVALID;
  input m_axi_maxiport0_ARREADY;
  input [511:0]m_axi_maxiport0_RDATA;
  input [1:0]m_axi_maxiport0_RRESP;
  input m_axi_maxiport0_RLAST;
  input m_axi_maxiport0_RVALID;
  output m_axi_maxiport0_RREADY;
  output [63:0]m_axi_maxiport1_AWADDR;
  output [7:0]m_axi_maxiport1_AWLEN;
  output [2:0]m_axi_maxiport1_AWSIZE;
  output [1:0]m_axi_maxiport1_AWBURST;
  output [1:0]m_axi_maxiport1_AWLOCK;
  output [3:0]m_axi_maxiport1_AWREGION;
  output [3:0]m_axi_maxiport1_AWCACHE;
  output [2:0]m_axi_maxiport1_AWPROT;
  output [3:0]m_axi_maxiport1_AWQOS;
  output m_axi_maxiport1_AWVALID;
  input m_axi_maxiport1_AWREADY;
  output [511:0]m_axi_maxiport1_WDATA;
  output [63:0]m_axi_maxiport1_WSTRB;
  output m_axi_maxiport1_WLAST;
  output m_axi_maxiport1_WVALID;
  input m_axi_maxiport1_WREADY;
  input [1:0]m_axi_maxiport1_BRESP;
  input m_axi_maxiport1_BVALID;
  output m_axi_maxiport1_BREADY;
  output [63:0]m_axi_maxiport1_ARADDR;
  output [7:0]m_axi_maxiport1_ARLEN;
  output [2:0]m_axi_maxiport1_ARSIZE;
  output [1:0]m_axi_maxiport1_ARBURST;
  output [1:0]m_axi_maxiport1_ARLOCK;
  output [3:0]m_axi_maxiport1_ARREGION;
  output [3:0]m_axi_maxiport1_ARCACHE;
  output [2:0]m_axi_maxiport1_ARPROT;
  output [3:0]m_axi_maxiport1_ARQOS;
  output m_axi_maxiport1_ARVALID;
  input m_axi_maxiport1_ARREADY;
  input [511:0]m_axi_maxiport1_RDATA;
  input [1:0]m_axi_maxiport1_RRESP;
  input m_axi_maxiport1_RLAST;
  input m_axi_maxiport1_RVALID;
  output m_axi_maxiport1_RREADY;
endmodule
