// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
// Date        : Thu May  7 16:56:57 2020
// Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
// Command     : write_verilog -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_memory_subsystem_0_stub.v
// Design      : pfm_dynamic_memory_subsystem_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xcu200-fsgd2104-2-e
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "bd_d216,Vivado 2020.1" *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(aclk, aclk1, aclk2, aresetn, S_AXI_CTRL_awaddr, 
  S_AXI_CTRL_awprot, S_AXI_CTRL_awvalid, S_AXI_CTRL_awready, S_AXI_CTRL_wdata, 
  S_AXI_CTRL_wstrb, S_AXI_CTRL_wvalid, S_AXI_CTRL_wready, S_AXI_CTRL_bresp, 
  S_AXI_CTRL_bvalid, S_AXI_CTRL_bready, S_AXI_CTRL_araddr, S_AXI_CTRL_arprot, 
  S_AXI_CTRL_arvalid, S_AXI_CTRL_arready, S_AXI_CTRL_rdata, S_AXI_CTRL_rresp, 
  S_AXI_CTRL_rvalid, S_AXI_CTRL_rready, S00_AXI_awid, S00_AXI_awaddr, S00_AXI_awlen, 
  S00_AXI_awsize, S00_AXI_awburst, S00_AXI_awlock, S00_AXI_awcache, S00_AXI_awprot, 
  S00_AXI_awregion, S00_AXI_awqos, S00_AXI_awvalid, S00_AXI_awready, S00_AXI_wdata, 
  S00_AXI_wstrb, S00_AXI_wlast, S00_AXI_wvalid, S00_AXI_wready, S00_AXI_bid, S00_AXI_bresp, 
  S00_AXI_bvalid, S00_AXI_bready, S00_AXI_arid, S00_AXI_araddr, S00_AXI_arlen, 
  S00_AXI_arsize, S00_AXI_arburst, S00_AXI_arlock, S00_AXI_arcache, S00_AXI_arprot, 
  S00_AXI_arregion, S00_AXI_arqos, S00_AXI_arvalid, S00_AXI_arready, S00_AXI_rid, 
  S00_AXI_rdata, S00_AXI_rresp, S00_AXI_rlast, S00_AXI_rvalid, S00_AXI_rready, 
  S01_AXI_awaddr, S01_AXI_awlen, S01_AXI_awburst, S01_AXI_awlock, S01_AXI_awcache, 
  S01_AXI_awprot, S01_AXI_awregion, S01_AXI_awqos, S01_AXI_awvalid, S01_AXI_awready, 
  S01_AXI_wdata, S01_AXI_wstrb, S01_AXI_wlast, S01_AXI_wvalid, S01_AXI_wready, S01_AXI_bresp, 
  S01_AXI_bvalid, S01_AXI_bready, S01_AXI_araddr, S01_AXI_arlen, S01_AXI_arburst, 
  S01_AXI_arlock, S01_AXI_arcache, S01_AXI_arprot, S01_AXI_arregion, S01_AXI_arqos, 
  S01_AXI_arvalid, S01_AXI_arready, S01_AXI_rdata, S01_AXI_rresp, S01_AXI_rlast, 
  S01_AXI_rvalid, S01_AXI_rready, S02_AXI_awaddr, S02_AXI_awlen, S02_AXI_awburst, 
  S02_AXI_awlock, S02_AXI_awcache, S02_AXI_awprot, S02_AXI_awregion, S02_AXI_awqos, 
  S02_AXI_awvalid, S02_AXI_awready, S02_AXI_wdata, S02_AXI_wstrb, S02_AXI_wlast, 
  S02_AXI_wvalid, S02_AXI_wready, S02_AXI_bresp, S02_AXI_bvalid, S02_AXI_bready, 
  S02_AXI_araddr, S02_AXI_arlen, S02_AXI_arburst, S02_AXI_arlock, S02_AXI_arcache, 
  S02_AXI_arprot, S02_AXI_arregion, S02_AXI_arqos, S02_AXI_arvalid, S02_AXI_arready, 
  S02_AXI_rdata, S02_AXI_rresp, S02_AXI_rlast, S02_AXI_rvalid, S02_AXI_rready, S03_AXI_awid, 
  S03_AXI_awaddr, S03_AXI_awlen, S03_AXI_awsize, S03_AXI_awburst, S03_AXI_awlock, 
  S03_AXI_awcache, S03_AXI_awprot, S03_AXI_awregion, S03_AXI_awqos, S03_AXI_awvalid, 
  S03_AXI_awready, S03_AXI_wdata, S03_AXI_wstrb, S03_AXI_wlast, S03_AXI_wvalid, 
  S03_AXI_wready, S03_AXI_bid, S03_AXI_bresp, S03_AXI_bvalid, S03_AXI_bready, S03_AXI_arid, 
  S03_AXI_araddr, S03_AXI_arlen, S03_AXI_arsize, S03_AXI_arburst, S03_AXI_arlock, 
  S03_AXI_arcache, S03_AXI_arprot, S03_AXI_arregion, S03_AXI_arqos, S03_AXI_arvalid, 
  S03_AXI_arready, S03_AXI_rid, S03_AXI_rdata, S03_AXI_rresp, S03_AXI_rlast, S03_AXI_rvalid, 
  S03_AXI_rready, S04_AXI_awaddr, S04_AXI_awlen, S04_AXI_awlock, S04_AXI_awcache, 
  S04_AXI_awprot, S04_AXI_awregion, S04_AXI_awqos, S04_AXI_awvalid, S04_AXI_awready, 
  S04_AXI_wdata, S04_AXI_wstrb, S04_AXI_wlast, S04_AXI_wvalid, S04_AXI_wready, S04_AXI_bresp, 
  S04_AXI_bvalid, S04_AXI_bready, S04_AXI_araddr, S04_AXI_arlen, S04_AXI_arlock, 
  S04_AXI_arcache, S04_AXI_arprot, S04_AXI_arregion, S04_AXI_arqos, S04_AXI_arvalid, 
  S04_AXI_arready, S04_AXI_rdata, S04_AXI_rresp, S04_AXI_rlast, S04_AXI_rvalid, 
  S04_AXI_rready, S05_AXI_awaddr, S05_AXI_awlen, S05_AXI_awlock, S05_AXI_awcache, 
  S05_AXI_awprot, S05_AXI_awregion, S05_AXI_awqos, S05_AXI_awvalid, S05_AXI_awready, 
  S05_AXI_wdata, S05_AXI_wstrb, S05_AXI_wlast, S05_AXI_wvalid, S05_AXI_wready, S05_AXI_bresp, 
  S05_AXI_bvalid, S05_AXI_bready, S05_AXI_araddr, S05_AXI_arlen, S05_AXI_arlock, 
  S05_AXI_arcache, S05_AXI_arprot, S05_AXI_arregion, S05_AXI_arqos, S05_AXI_arvalid, 
  S05_AXI_arready, S05_AXI_rdata, S05_AXI_rresp, S05_AXI_rlast, S05_AXI_rvalid, 
  S05_AXI_rready, DDR4_MEM00_dq, DDR4_MEM00_dqs_t, DDR4_MEM00_dqs_c, DDR4_MEM00_adr, 
  DDR4_MEM00_ba, DDR4_MEM00_bg, DDR4_MEM00_act_n, DDR4_MEM00_reset_n, DDR4_MEM00_ck_t, 
  DDR4_MEM00_ck_c, DDR4_MEM00_cke, DDR4_MEM00_cs_n, DDR4_MEM00_odt, DDR4_MEM00_par, 
  DDR4_MEM01_dq, DDR4_MEM01_dqs_t, DDR4_MEM01_dqs_c, DDR4_MEM01_adr, DDR4_MEM01_ba, 
  DDR4_MEM01_bg, DDR4_MEM01_act_n, DDR4_MEM01_reset_n, DDR4_MEM01_ck_t, DDR4_MEM01_ck_c, 
  DDR4_MEM01_cke, DDR4_MEM01_cs_n, DDR4_MEM01_odt, DDR4_MEM01_par, DDR4_MEM02_dq, 
  DDR4_MEM02_dqs_t, DDR4_MEM02_dqs_c, DDR4_MEM02_adr, DDR4_MEM02_ba, DDR4_MEM02_bg, 
  DDR4_MEM02_act_n, DDR4_MEM02_reset_n, DDR4_MEM02_ck_t, DDR4_MEM02_ck_c, DDR4_MEM02_cke, 
  DDR4_MEM02_cs_n, DDR4_MEM02_odt, DDR4_MEM02_par, DDR4_MEM00_DIFF_CLK_clk_p, 
  DDR4_MEM00_DIFF_CLK_clk_n, DDR4_MEM01_DIFF_CLK_clk_p, DDR4_MEM01_DIFF_CLK_clk_n, 
  DDR4_MEM02_DIFF_CLK_clk_p, DDR4_MEM02_DIFF_CLK_clk_n, ddr4_mem00_ui_clk, 
  ddr4_mem01_ui_clk, ddr4_mem02_ui_clk, ddr4_mem00_sys_rst, ddr4_mem01_sys_rst, 
  ddr4_mem02_sys_rst, ddr4_mem_calib_complete, ddr4_mem_calib_vec, M00_AXI_awaddr, 
  M00_AXI_awlen, M00_AXI_awsize, M00_AXI_awburst, M00_AXI_awlock, M00_AXI_awcache, 
  M00_AXI_awprot, M00_AXI_awregion, M00_AXI_awqos, M00_AXI_awvalid, M00_AXI_awready, 
  M00_AXI_wdata, M00_AXI_wstrb, M00_AXI_wlast, M00_AXI_wvalid, M00_AXI_wready, M00_AXI_bresp, 
  M00_AXI_bvalid, M00_AXI_bready, M00_AXI_araddr, M00_AXI_arlen, M00_AXI_arsize, 
  M00_AXI_arburst, M00_AXI_arlock, M00_AXI_arcache, M00_AXI_arprot, M00_AXI_arregion, 
  M00_AXI_arqos, M00_AXI_arvalid, M00_AXI_arready, M00_AXI_rdata, M00_AXI_rresp, 
  M00_AXI_rlast, M00_AXI_rvalid, M00_AXI_rready, S01_AXI_arid, S01_AXI_awid, S01_AXI_bid, 
  S01_AXI_rid, S02_AXI_arid, S02_AXI_awid, S02_AXI_bid, S02_AXI_rid)
/* synthesis syn_black_box black_box_pad_pin="aclk,aclk1,aclk2,aresetn,S_AXI_CTRL_awaddr[24:0],S_AXI_CTRL_awprot[2:0],S_AXI_CTRL_awvalid,S_AXI_CTRL_awready,S_AXI_CTRL_wdata[31:0],S_AXI_CTRL_wstrb[3:0],S_AXI_CTRL_wvalid,S_AXI_CTRL_wready,S_AXI_CTRL_bresp[1:0],S_AXI_CTRL_bvalid,S_AXI_CTRL_bready,S_AXI_CTRL_araddr[24:0],S_AXI_CTRL_arprot[2:0],S_AXI_CTRL_arvalid,S_AXI_CTRL_arready,S_AXI_CTRL_rdata[31:0],S_AXI_CTRL_rresp[1:0],S_AXI_CTRL_rvalid,S_AXI_CTRL_rready,S00_AXI_awid[3:0],S00_AXI_awaddr[38:0],S00_AXI_awlen[7:0],S00_AXI_awsize[2:0],S00_AXI_awburst[1:0],S00_AXI_awlock[0:0],S00_AXI_awcache[3:0],S00_AXI_awprot[2:0],S00_AXI_awregion[3:0],S00_AXI_awqos[3:0],S00_AXI_awvalid,S00_AXI_awready,S00_AXI_wdata[511:0],S00_AXI_wstrb[63:0],S00_AXI_wlast,S00_AXI_wvalid,S00_AXI_wready,S00_AXI_bid[3:0],S00_AXI_bresp[1:0],S00_AXI_bvalid,S00_AXI_bready,S00_AXI_arid[3:0],S00_AXI_araddr[38:0],S00_AXI_arlen[7:0],S00_AXI_arsize[2:0],S00_AXI_arburst[1:0],S00_AXI_arlock[0:0],S00_AXI_arcache[3:0],S00_AXI_arprot[2:0],S00_AXI_arregion[3:0],S00_AXI_arqos[3:0],S00_AXI_arvalid,S00_AXI_arready,S00_AXI_rid[3:0],S00_AXI_rdata[511:0],S00_AXI_rresp[1:0],S00_AXI_rlast,S00_AXI_rvalid,S00_AXI_rready,S01_AXI_awaddr[38:0],S01_AXI_awlen[7:0],S01_AXI_awburst[1:0],S01_AXI_awlock[0:0],S01_AXI_awcache[3:0],S01_AXI_awprot[2:0],S01_AXI_awregion[3:0],S01_AXI_awqos[3:0],S01_AXI_awvalid,S01_AXI_awready,S01_AXI_wdata[511:0],S01_AXI_wstrb[63:0],S01_AXI_wlast,S01_AXI_wvalid,S01_AXI_wready,S01_AXI_bresp[1:0],S01_AXI_bvalid,S01_AXI_bready,S01_AXI_araddr[38:0],S01_AXI_arlen[7:0],S01_AXI_arburst[1:0],S01_AXI_arlock[0:0],S01_AXI_arcache[3:0],S01_AXI_arprot[2:0],S01_AXI_arregion[3:0],S01_AXI_arqos[3:0],S01_AXI_arvalid,S01_AXI_arready,S01_AXI_rdata[511:0],S01_AXI_rresp[1:0],S01_AXI_rlast,S01_AXI_rvalid,S01_AXI_rready,S02_AXI_awaddr[38:0],S02_AXI_awlen[7:0],S02_AXI_awburst[1:0],S02_AXI_awlock[0:0],S02_AXI_awcache[3:0],S02_AXI_awprot[2:0],S02_AXI_awregion[3:0],S02_AXI_awqos[3:0],S02_AXI_awvalid,S02_AXI_awready,S02_AXI_wdata[511:0],S02_AXI_wstrb[63:0],S02_AXI_wlast,S02_AXI_wvalid,S02_AXI_wready,S02_AXI_bresp[1:0],S02_AXI_bvalid,S02_AXI_bready,S02_AXI_araddr[38:0],S02_AXI_arlen[7:0],S02_AXI_arburst[1:0],S02_AXI_arlock[0:0],S02_AXI_arcache[3:0],S02_AXI_arprot[2:0],S02_AXI_arregion[3:0],S02_AXI_arqos[3:0],S02_AXI_arvalid,S02_AXI_arready,S02_AXI_rdata[511:0],S02_AXI_rresp[1:0],S02_AXI_rlast,S02_AXI_rvalid,S02_AXI_rready,S03_AXI_awid[3:0],S03_AXI_awaddr[38:0],S03_AXI_awlen[7:0],S03_AXI_awsize[2:0],S03_AXI_awburst[1:0],S03_AXI_awlock[0:0],S03_AXI_awcache[3:0],S03_AXI_awprot[2:0],S03_AXI_awregion[3:0],S03_AXI_awqos[3:0],S03_AXI_awvalid,S03_AXI_awready,S03_AXI_wdata[511:0],S03_AXI_wstrb[63:0],S03_AXI_wlast,S03_AXI_wvalid,S03_AXI_wready,S03_AXI_bid[3:0],S03_AXI_bresp[1:0],S03_AXI_bvalid,S03_AXI_bready,S03_AXI_arid[3:0],S03_AXI_araddr[38:0],S03_AXI_arlen[7:0],S03_AXI_arsize[2:0],S03_AXI_arburst[1:0],S03_AXI_arlock[0:0],S03_AXI_arcache[3:0],S03_AXI_arprot[2:0],S03_AXI_arregion[3:0],S03_AXI_arqos[3:0],S03_AXI_arvalid,S03_AXI_arready,S03_AXI_rid[3:0],S03_AXI_rdata[511:0],S03_AXI_rresp[1:0],S03_AXI_rlast,S03_AXI_rvalid,S03_AXI_rready,S04_AXI_awaddr[38:0],S04_AXI_awlen[7:0],S04_AXI_awlock[0:0],S04_AXI_awcache[3:0],S04_AXI_awprot[2:0],S04_AXI_awregion[3:0],S04_AXI_awqos[3:0],S04_AXI_awvalid,S04_AXI_awready,S04_AXI_wdata[511:0],S04_AXI_wstrb[63:0],S04_AXI_wlast,S04_AXI_wvalid,S04_AXI_wready,S04_AXI_bresp[1:0],S04_AXI_bvalid,S04_AXI_bready,S04_AXI_araddr[38:0],S04_AXI_arlen[7:0],S04_AXI_arlock[0:0],S04_AXI_arcache[3:0],S04_AXI_arprot[2:0],S04_AXI_arregion[3:0],S04_AXI_arqos[3:0],S04_AXI_arvalid,S04_AXI_arready,S04_AXI_rdata[511:0],S04_AXI_rresp[1:0],S04_AXI_rlast,S04_AXI_rvalid,S04_AXI_rready,S05_AXI_awaddr[38:0],S05_AXI_awlen[7:0],S05_AXI_awlock[0:0],S05_AXI_awcache[3:0],S05_AXI_awprot[2:0],S05_AXI_awregion[3:0],S05_AXI_awqos[3:0],S05_AXI_awvalid,S05_AXI_awready,S05_AXI_wdata[511:0],S05_AXI_wstrb[63:0],S05_AXI_wlast,S05_AXI_wvalid,S05_AXI_wready,S05_AXI_bresp[1:0],S05_AXI_bvalid,S05_AXI_bready,S05_AXI_araddr[38:0],S05_AXI_arlen[7:0],S05_AXI_arlock[0:0],S05_AXI_arcache[3:0],S05_AXI_arprot[2:0],S05_AXI_arregion[3:0],S05_AXI_arqos[3:0],S05_AXI_arvalid,S05_AXI_arready,S05_AXI_rdata[511:0],S05_AXI_rresp[1:0],S05_AXI_rlast,S05_AXI_rvalid,S05_AXI_rready,DDR4_MEM00_dq[71:0],DDR4_MEM00_dqs_t[17:0],DDR4_MEM00_dqs_c[17:0],DDR4_MEM00_adr[16:0],DDR4_MEM00_ba[1:0],DDR4_MEM00_bg[1:0],DDR4_MEM00_act_n,DDR4_MEM00_reset_n,DDR4_MEM00_ck_t[0:0],DDR4_MEM00_ck_c[0:0],DDR4_MEM00_cke[0:0],DDR4_MEM00_cs_n[0:0],DDR4_MEM00_odt[0:0],DDR4_MEM00_par,DDR4_MEM01_dq[71:0],DDR4_MEM01_dqs_t[17:0],DDR4_MEM01_dqs_c[17:0],DDR4_MEM01_adr[16:0],DDR4_MEM01_ba[1:0],DDR4_MEM01_bg[1:0],DDR4_MEM01_act_n,DDR4_MEM01_reset_n,DDR4_MEM01_ck_t[0:0],DDR4_MEM01_ck_c[0:0],DDR4_MEM01_cke[0:0],DDR4_MEM01_cs_n[0:0],DDR4_MEM01_odt[0:0],DDR4_MEM01_par,DDR4_MEM02_dq[71:0],DDR4_MEM02_dqs_t[17:0],DDR4_MEM02_dqs_c[17:0],DDR4_MEM02_adr[16:0],DDR4_MEM02_ba[1:0],DDR4_MEM02_bg[1:0],DDR4_MEM02_act_n,DDR4_MEM02_reset_n,DDR4_MEM02_ck_t[0:0],DDR4_MEM02_ck_c[0:0],DDR4_MEM02_cke[0:0],DDR4_MEM02_cs_n[0:0],DDR4_MEM02_odt[0:0],DDR4_MEM02_par,DDR4_MEM00_DIFF_CLK_clk_p,DDR4_MEM00_DIFF_CLK_clk_n,DDR4_MEM01_DIFF_CLK_clk_p,DDR4_MEM01_DIFF_CLK_clk_n,DDR4_MEM02_DIFF_CLK_clk_p,DDR4_MEM02_DIFF_CLK_clk_n,ddr4_mem00_ui_clk,ddr4_mem01_ui_clk,ddr4_mem02_ui_clk,ddr4_mem00_sys_rst,ddr4_mem01_sys_rst,ddr4_mem02_sys_rst,ddr4_mem_calib_complete,ddr4_mem_calib_vec[2:0],M00_AXI_awaddr[38:0],M00_AXI_awlen[7:0],M00_AXI_awsize[2:0],M00_AXI_awburst[1:0],M00_AXI_awlock[0:0],M00_AXI_awcache[3:0],M00_AXI_awprot[2:0],M00_AXI_awregion[3:0],M00_AXI_awqos[3:0],M00_AXI_awvalid,M00_AXI_awready,M00_AXI_wdata[511:0],M00_AXI_wstrb[63:0],M00_AXI_wlast,M00_AXI_wvalid,M00_AXI_wready,M00_AXI_bresp[1:0],M00_AXI_bvalid,M00_AXI_bready,M00_AXI_araddr[38:0],M00_AXI_arlen[7:0],M00_AXI_arsize[2:0],M00_AXI_arburst[1:0],M00_AXI_arlock[0:0],M00_AXI_arcache[3:0],M00_AXI_arprot[2:0],M00_AXI_arregion[3:0],M00_AXI_arqos[3:0],M00_AXI_arvalid,M00_AXI_arready,M00_AXI_rdata[511:0],M00_AXI_rresp[1:0],M00_AXI_rlast,M00_AXI_rvalid,M00_AXI_rready,S01_AXI_arid[3:0],S01_AXI_awid[3:0],S01_AXI_bid[3:0],S01_AXI_rid[3:0],S02_AXI_arid[3:0],S02_AXI_awid[3:0],S02_AXI_bid[3:0],S02_AXI_rid[3:0]" */;
  input aclk;
  input aclk1;
  input aclk2;
  input aresetn;
  input [24:0]S_AXI_CTRL_awaddr;
  input [2:0]S_AXI_CTRL_awprot;
  input S_AXI_CTRL_awvalid;
  output S_AXI_CTRL_awready;
  input [31:0]S_AXI_CTRL_wdata;
  input [3:0]S_AXI_CTRL_wstrb;
  input S_AXI_CTRL_wvalid;
  output S_AXI_CTRL_wready;
  output [1:0]S_AXI_CTRL_bresp;
  output S_AXI_CTRL_bvalid;
  input S_AXI_CTRL_bready;
  input [24:0]S_AXI_CTRL_araddr;
  input [2:0]S_AXI_CTRL_arprot;
  input S_AXI_CTRL_arvalid;
  output S_AXI_CTRL_arready;
  output [31:0]S_AXI_CTRL_rdata;
  output [1:0]S_AXI_CTRL_rresp;
  output S_AXI_CTRL_rvalid;
  input S_AXI_CTRL_rready;
  input [3:0]S00_AXI_awid;
  input [38:0]S00_AXI_awaddr;
  input [7:0]S00_AXI_awlen;
  input [2:0]S00_AXI_awsize;
  input [1:0]S00_AXI_awburst;
  input [0:0]S00_AXI_awlock;
  input [3:0]S00_AXI_awcache;
  input [2:0]S00_AXI_awprot;
  input [3:0]S00_AXI_awregion;
  input [3:0]S00_AXI_awqos;
  input S00_AXI_awvalid;
  output S00_AXI_awready;
  input [511:0]S00_AXI_wdata;
  input [63:0]S00_AXI_wstrb;
  input S00_AXI_wlast;
  input S00_AXI_wvalid;
  output S00_AXI_wready;
  output [3:0]S00_AXI_bid;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  input S00_AXI_bready;
  input [3:0]S00_AXI_arid;
  input [38:0]S00_AXI_araddr;
  input [7:0]S00_AXI_arlen;
  input [2:0]S00_AXI_arsize;
  input [1:0]S00_AXI_arburst;
  input [0:0]S00_AXI_arlock;
  input [3:0]S00_AXI_arcache;
  input [2:0]S00_AXI_arprot;
  input [3:0]S00_AXI_arregion;
  input [3:0]S00_AXI_arqos;
  input S00_AXI_arvalid;
  output S00_AXI_arready;
  output [3:0]S00_AXI_rid;
  output [511:0]S00_AXI_rdata;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rlast;
  output S00_AXI_rvalid;
  input S00_AXI_rready;
  input [38:0]S01_AXI_awaddr;
  input [7:0]S01_AXI_awlen;
  input [1:0]S01_AXI_awburst;
  input [0:0]S01_AXI_awlock;
  input [3:0]S01_AXI_awcache;
  input [2:0]S01_AXI_awprot;
  input [3:0]S01_AXI_awregion;
  input [3:0]S01_AXI_awqos;
  input S01_AXI_awvalid;
  output S01_AXI_awready;
  input [511:0]S01_AXI_wdata;
  input [63:0]S01_AXI_wstrb;
  input S01_AXI_wlast;
  input S01_AXI_wvalid;
  output S01_AXI_wready;
  output [1:0]S01_AXI_bresp;
  output S01_AXI_bvalid;
  input S01_AXI_bready;
  input [38:0]S01_AXI_araddr;
  input [7:0]S01_AXI_arlen;
  input [1:0]S01_AXI_arburst;
  input [0:0]S01_AXI_arlock;
  input [3:0]S01_AXI_arcache;
  input [2:0]S01_AXI_arprot;
  input [3:0]S01_AXI_arregion;
  input [3:0]S01_AXI_arqos;
  input S01_AXI_arvalid;
  output S01_AXI_arready;
  output [511:0]S01_AXI_rdata;
  output [1:0]S01_AXI_rresp;
  output S01_AXI_rlast;
  output S01_AXI_rvalid;
  input S01_AXI_rready;
  input [38:0]S02_AXI_awaddr;
  input [7:0]S02_AXI_awlen;
  input [1:0]S02_AXI_awburst;
  input [0:0]S02_AXI_awlock;
  input [3:0]S02_AXI_awcache;
  input [2:0]S02_AXI_awprot;
  input [3:0]S02_AXI_awregion;
  input [3:0]S02_AXI_awqos;
  input S02_AXI_awvalid;
  output S02_AXI_awready;
  input [511:0]S02_AXI_wdata;
  input [63:0]S02_AXI_wstrb;
  input S02_AXI_wlast;
  input S02_AXI_wvalid;
  output S02_AXI_wready;
  output [1:0]S02_AXI_bresp;
  output S02_AXI_bvalid;
  input S02_AXI_bready;
  input [38:0]S02_AXI_araddr;
  input [7:0]S02_AXI_arlen;
  input [1:0]S02_AXI_arburst;
  input [0:0]S02_AXI_arlock;
  input [3:0]S02_AXI_arcache;
  input [2:0]S02_AXI_arprot;
  input [3:0]S02_AXI_arregion;
  input [3:0]S02_AXI_arqos;
  input S02_AXI_arvalid;
  output S02_AXI_arready;
  output [511:0]S02_AXI_rdata;
  output [1:0]S02_AXI_rresp;
  output S02_AXI_rlast;
  output S02_AXI_rvalid;
  input S02_AXI_rready;
  input [3:0]S03_AXI_awid;
  input [38:0]S03_AXI_awaddr;
  input [7:0]S03_AXI_awlen;
  input [2:0]S03_AXI_awsize;
  input [1:0]S03_AXI_awburst;
  input [0:0]S03_AXI_awlock;
  input [3:0]S03_AXI_awcache;
  input [2:0]S03_AXI_awprot;
  input [3:0]S03_AXI_awregion;
  input [3:0]S03_AXI_awqos;
  input S03_AXI_awvalid;
  output S03_AXI_awready;
  input [511:0]S03_AXI_wdata;
  input [63:0]S03_AXI_wstrb;
  input S03_AXI_wlast;
  input S03_AXI_wvalid;
  output S03_AXI_wready;
  output [3:0]S03_AXI_bid;
  output [1:0]S03_AXI_bresp;
  output S03_AXI_bvalid;
  input S03_AXI_bready;
  input [3:0]S03_AXI_arid;
  input [38:0]S03_AXI_araddr;
  input [7:0]S03_AXI_arlen;
  input [2:0]S03_AXI_arsize;
  input [1:0]S03_AXI_arburst;
  input [0:0]S03_AXI_arlock;
  input [3:0]S03_AXI_arcache;
  input [2:0]S03_AXI_arprot;
  input [3:0]S03_AXI_arregion;
  input [3:0]S03_AXI_arqos;
  input S03_AXI_arvalid;
  output S03_AXI_arready;
  output [3:0]S03_AXI_rid;
  output [511:0]S03_AXI_rdata;
  output [1:0]S03_AXI_rresp;
  output S03_AXI_rlast;
  output S03_AXI_rvalid;
  input S03_AXI_rready;
  input [38:0]S04_AXI_awaddr;
  input [7:0]S04_AXI_awlen;
  input [0:0]S04_AXI_awlock;
  input [3:0]S04_AXI_awcache;
  input [2:0]S04_AXI_awprot;
  input [3:0]S04_AXI_awregion;
  input [3:0]S04_AXI_awqos;
  input S04_AXI_awvalid;
  output S04_AXI_awready;
  input [511:0]S04_AXI_wdata;
  input [63:0]S04_AXI_wstrb;
  input S04_AXI_wlast;
  input S04_AXI_wvalid;
  output S04_AXI_wready;
  output [1:0]S04_AXI_bresp;
  output S04_AXI_bvalid;
  input S04_AXI_bready;
  input [38:0]S04_AXI_araddr;
  input [7:0]S04_AXI_arlen;
  input [0:0]S04_AXI_arlock;
  input [3:0]S04_AXI_arcache;
  input [2:0]S04_AXI_arprot;
  input [3:0]S04_AXI_arregion;
  input [3:0]S04_AXI_arqos;
  input S04_AXI_arvalid;
  output S04_AXI_arready;
  output [511:0]S04_AXI_rdata;
  output [1:0]S04_AXI_rresp;
  output S04_AXI_rlast;
  output S04_AXI_rvalid;
  input S04_AXI_rready;
  input [38:0]S05_AXI_awaddr;
  input [7:0]S05_AXI_awlen;
  input [0:0]S05_AXI_awlock;
  input [3:0]S05_AXI_awcache;
  input [2:0]S05_AXI_awprot;
  input [3:0]S05_AXI_awregion;
  input [3:0]S05_AXI_awqos;
  input S05_AXI_awvalid;
  output S05_AXI_awready;
  input [511:0]S05_AXI_wdata;
  input [63:0]S05_AXI_wstrb;
  input S05_AXI_wlast;
  input S05_AXI_wvalid;
  output S05_AXI_wready;
  output [1:0]S05_AXI_bresp;
  output S05_AXI_bvalid;
  input S05_AXI_bready;
  input [38:0]S05_AXI_araddr;
  input [7:0]S05_AXI_arlen;
  input [0:0]S05_AXI_arlock;
  input [3:0]S05_AXI_arcache;
  input [2:0]S05_AXI_arprot;
  input [3:0]S05_AXI_arregion;
  input [3:0]S05_AXI_arqos;
  input S05_AXI_arvalid;
  output S05_AXI_arready;
  output [511:0]S05_AXI_rdata;
  output [1:0]S05_AXI_rresp;
  output S05_AXI_rlast;
  output S05_AXI_rvalid;
  input S05_AXI_rready;
  inout [71:0]DDR4_MEM00_dq;
  inout [17:0]DDR4_MEM00_dqs_t;
  inout [17:0]DDR4_MEM00_dqs_c;
  output [16:0]DDR4_MEM00_adr;
  output [1:0]DDR4_MEM00_ba;
  output [1:0]DDR4_MEM00_bg;
  output DDR4_MEM00_act_n;
  output DDR4_MEM00_reset_n;
  output [0:0]DDR4_MEM00_ck_t;
  output [0:0]DDR4_MEM00_ck_c;
  output [0:0]DDR4_MEM00_cke;
  output [0:0]DDR4_MEM00_cs_n;
  output [0:0]DDR4_MEM00_odt;
  output DDR4_MEM00_par;
  inout [71:0]DDR4_MEM01_dq;
  inout [17:0]DDR4_MEM01_dqs_t;
  inout [17:0]DDR4_MEM01_dqs_c;
  output [16:0]DDR4_MEM01_adr;
  output [1:0]DDR4_MEM01_ba;
  output [1:0]DDR4_MEM01_bg;
  output DDR4_MEM01_act_n;
  output DDR4_MEM01_reset_n;
  output [0:0]DDR4_MEM01_ck_t;
  output [0:0]DDR4_MEM01_ck_c;
  output [0:0]DDR4_MEM01_cke;
  output [0:0]DDR4_MEM01_cs_n;
  output [0:0]DDR4_MEM01_odt;
  output DDR4_MEM01_par;
  inout [71:0]DDR4_MEM02_dq;
  inout [17:0]DDR4_MEM02_dqs_t;
  inout [17:0]DDR4_MEM02_dqs_c;
  output [16:0]DDR4_MEM02_adr;
  output [1:0]DDR4_MEM02_ba;
  output [1:0]DDR4_MEM02_bg;
  output DDR4_MEM02_act_n;
  output DDR4_MEM02_reset_n;
  output [0:0]DDR4_MEM02_ck_t;
  output [0:0]DDR4_MEM02_ck_c;
  output [0:0]DDR4_MEM02_cke;
  output [0:0]DDR4_MEM02_cs_n;
  output [0:0]DDR4_MEM02_odt;
  output DDR4_MEM02_par;
  input DDR4_MEM00_DIFF_CLK_clk_p;
  input DDR4_MEM00_DIFF_CLK_clk_n;
  input DDR4_MEM01_DIFF_CLK_clk_p;
  input DDR4_MEM01_DIFF_CLK_clk_n;
  input DDR4_MEM02_DIFF_CLK_clk_p;
  input DDR4_MEM02_DIFF_CLK_clk_n;
  output ddr4_mem00_ui_clk;
  output ddr4_mem01_ui_clk;
  output ddr4_mem02_ui_clk;
  input ddr4_mem00_sys_rst;
  input ddr4_mem01_sys_rst;
  input ddr4_mem02_sys_rst;
  output ddr4_mem_calib_complete;
  output [2:0]ddr4_mem_calib_vec;
  output [38:0]M00_AXI_awaddr;
  output [7:0]M00_AXI_awlen;
  output [2:0]M00_AXI_awsize;
  output [1:0]M00_AXI_awburst;
  output [0:0]M00_AXI_awlock;
  output [3:0]M00_AXI_awcache;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awregion;
  output [3:0]M00_AXI_awqos;
  output M00_AXI_awvalid;
  input M00_AXI_awready;
  output [511:0]M00_AXI_wdata;
  output [63:0]M00_AXI_wstrb;
  output M00_AXI_wlast;
  output M00_AXI_wvalid;
  input M00_AXI_wready;
  input [1:0]M00_AXI_bresp;
  input M00_AXI_bvalid;
  output M00_AXI_bready;
  output [38:0]M00_AXI_araddr;
  output [7:0]M00_AXI_arlen;
  output [2:0]M00_AXI_arsize;
  output [1:0]M00_AXI_arburst;
  output [0:0]M00_AXI_arlock;
  output [3:0]M00_AXI_arcache;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arregion;
  output [3:0]M00_AXI_arqos;
  output M00_AXI_arvalid;
  input M00_AXI_arready;
  input [511:0]M00_AXI_rdata;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rlast;
  input M00_AXI_rvalid;
  output M00_AXI_rready;
  input [3:0]S01_AXI_arid;
  input [3:0]S01_AXI_awid;
  output [3:0]S01_AXI_bid;
  output [3:0]S01_AXI_rid;
  input [3:0]S02_AXI_arid;
  input [3:0]S02_AXI_awid;
  output [3:0]S02_AXI_bid;
  output [3:0]S02_AXI_rid;
endmodule
