// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
// Date        : Thu May  7 16:56:57 2020
// Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_memory_subsystem_0_sim_netlist.v
// Design      : pfm_dynamic_memory_subsystem_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xcu200-fsgd2104-2-e
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* HW_HANDOFF = "pfm_dynamic_memory_subsystem_0.hwdef" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216
   (DDR4_MEM00_DIFF_CLK_clk_n,
    DDR4_MEM00_DIFF_CLK_clk_p,
    DDR4_MEM00_act_n,
    DDR4_MEM00_adr,
    DDR4_MEM00_ba,
    DDR4_MEM00_bg,
    DDR4_MEM00_ck_c,
    DDR4_MEM00_ck_t,
    DDR4_MEM00_cke,
    DDR4_MEM00_cs_n,
    DDR4_MEM00_dq,
    DDR4_MEM00_dqs_c,
    DDR4_MEM00_dqs_t,
    DDR4_MEM00_odt,
    DDR4_MEM00_par,
    DDR4_MEM00_reset_n,
    DDR4_MEM01_DIFF_CLK_clk_n,
    DDR4_MEM01_DIFF_CLK_clk_p,
    DDR4_MEM01_act_n,
    DDR4_MEM01_adr,
    DDR4_MEM01_ba,
    DDR4_MEM01_bg,
    DDR4_MEM01_ck_c,
    DDR4_MEM01_ck_t,
    DDR4_MEM01_cke,
    DDR4_MEM01_cs_n,
    DDR4_MEM01_dq,
    DDR4_MEM01_dqs_c,
    DDR4_MEM01_dqs_t,
    DDR4_MEM01_odt,
    DDR4_MEM01_par,
    DDR4_MEM01_reset_n,
    DDR4_MEM02_DIFF_CLK_clk_n,
    DDR4_MEM02_DIFF_CLK_clk_p,
    DDR4_MEM02_act_n,
    DDR4_MEM02_adr,
    DDR4_MEM02_ba,
    DDR4_MEM02_bg,
    DDR4_MEM02_ck_c,
    DDR4_MEM02_ck_t,
    DDR4_MEM02_cke,
    DDR4_MEM02_cs_n,
    DDR4_MEM02_dq,
    DDR4_MEM02_dqs_c,
    DDR4_MEM02_dqs_t,
    DDR4_MEM02_odt,
    DDR4_MEM02_par,
    DDR4_MEM02_reset_n,
    M00_AXI_araddr,
    M00_AXI_arburst,
    M00_AXI_arcache,
    M00_AXI_arlen,
    M00_AXI_arlock,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arready,
    M00_AXI_arregion,
    M00_AXI_arsize,
    M00_AXI_arvalid,
    M00_AXI_awaddr,
    M00_AXI_awburst,
    M00_AXI_awcache,
    M00_AXI_awlen,
    M00_AXI_awlock,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awready,
    M00_AXI_awregion,
    M00_AXI_awsize,
    M00_AXI_awvalid,
    M00_AXI_bready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_rdata,
    M00_AXI_rlast,
    M00_AXI_rready,
    M00_AXI_rresp,
    M00_AXI_rvalid,
    M00_AXI_wdata,
    M00_AXI_wlast,
    M00_AXI_wready,
    M00_AXI_wstrb,
    M00_AXI_wvalid,
    S00_AXI_araddr,
    S00_AXI_arburst,
    S00_AXI_arcache,
    S00_AXI_arid,
    S00_AXI_arlen,
    S00_AXI_arlock,
    S00_AXI_arprot,
    S00_AXI_arqos,
    S00_AXI_arready,
    S00_AXI_arregion,
    S00_AXI_arsize,
    S00_AXI_arvalid,
    S00_AXI_awaddr,
    S00_AXI_awburst,
    S00_AXI_awcache,
    S00_AXI_awid,
    S00_AXI_awlen,
    S00_AXI_awlock,
    S00_AXI_awprot,
    S00_AXI_awqos,
    S00_AXI_awready,
    S00_AXI_awregion,
    S00_AXI_awsize,
    S00_AXI_awvalid,
    S00_AXI_bid,
    S00_AXI_bready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_rdata,
    S00_AXI_rid,
    S00_AXI_rlast,
    S00_AXI_rready,
    S00_AXI_rresp,
    S00_AXI_rvalid,
    S00_AXI_wdata,
    S00_AXI_wlast,
    S00_AXI_wready,
    S00_AXI_wstrb,
    S00_AXI_wvalid,
    S01_AXI_araddr,
    S01_AXI_arburst,
    S01_AXI_arcache,
    S01_AXI_arid,
    S01_AXI_arlen,
    S01_AXI_arlock,
    S01_AXI_arprot,
    S01_AXI_arqos,
    S01_AXI_arready,
    S01_AXI_arregion,
    S01_AXI_arvalid,
    S01_AXI_awaddr,
    S01_AXI_awburst,
    S01_AXI_awcache,
    S01_AXI_awid,
    S01_AXI_awlen,
    S01_AXI_awlock,
    S01_AXI_awprot,
    S01_AXI_awqos,
    S01_AXI_awready,
    S01_AXI_awregion,
    S01_AXI_awvalid,
    S01_AXI_bid,
    S01_AXI_bready,
    S01_AXI_bresp,
    S01_AXI_bvalid,
    S01_AXI_rdata,
    S01_AXI_rid,
    S01_AXI_rlast,
    S01_AXI_rready,
    S01_AXI_rresp,
    S01_AXI_rvalid,
    S01_AXI_wdata,
    S01_AXI_wlast,
    S01_AXI_wready,
    S01_AXI_wstrb,
    S01_AXI_wvalid,
    S02_AXI_araddr,
    S02_AXI_arburst,
    S02_AXI_arcache,
    S02_AXI_arid,
    S02_AXI_arlen,
    S02_AXI_arlock,
    S02_AXI_arprot,
    S02_AXI_arqos,
    S02_AXI_arready,
    S02_AXI_arregion,
    S02_AXI_arvalid,
    S02_AXI_awaddr,
    S02_AXI_awburst,
    S02_AXI_awcache,
    S02_AXI_awid,
    S02_AXI_awlen,
    S02_AXI_awlock,
    S02_AXI_awprot,
    S02_AXI_awqos,
    S02_AXI_awready,
    S02_AXI_awregion,
    S02_AXI_awvalid,
    S02_AXI_bid,
    S02_AXI_bready,
    S02_AXI_bresp,
    S02_AXI_bvalid,
    S02_AXI_rdata,
    S02_AXI_rid,
    S02_AXI_rlast,
    S02_AXI_rready,
    S02_AXI_rresp,
    S02_AXI_rvalid,
    S02_AXI_wdata,
    S02_AXI_wlast,
    S02_AXI_wready,
    S02_AXI_wstrb,
    S02_AXI_wvalid,
    S03_AXI_araddr,
    S03_AXI_arburst,
    S03_AXI_arcache,
    S03_AXI_arid,
    S03_AXI_arlen,
    S03_AXI_arlock,
    S03_AXI_arprot,
    S03_AXI_arqos,
    S03_AXI_arready,
    S03_AXI_arregion,
    S03_AXI_arsize,
    S03_AXI_arvalid,
    S03_AXI_awaddr,
    S03_AXI_awburst,
    S03_AXI_awcache,
    S03_AXI_awid,
    S03_AXI_awlen,
    S03_AXI_awlock,
    S03_AXI_awprot,
    S03_AXI_awqos,
    S03_AXI_awready,
    S03_AXI_awregion,
    S03_AXI_awsize,
    S03_AXI_awvalid,
    S03_AXI_bid,
    S03_AXI_bready,
    S03_AXI_bresp,
    S03_AXI_bvalid,
    S03_AXI_rdata,
    S03_AXI_rid,
    S03_AXI_rlast,
    S03_AXI_rready,
    S03_AXI_rresp,
    S03_AXI_rvalid,
    S03_AXI_wdata,
    S03_AXI_wlast,
    S03_AXI_wready,
    S03_AXI_wstrb,
    S03_AXI_wvalid,
    S04_AXI_araddr,
    S04_AXI_arcache,
    S04_AXI_arlen,
    S04_AXI_arlock,
    S04_AXI_arprot,
    S04_AXI_arqos,
    S04_AXI_arready,
    S04_AXI_arregion,
    S04_AXI_arvalid,
    S04_AXI_awaddr,
    S04_AXI_awcache,
    S04_AXI_awlen,
    S04_AXI_awlock,
    S04_AXI_awprot,
    S04_AXI_awqos,
    S04_AXI_awready,
    S04_AXI_awregion,
    S04_AXI_awvalid,
    S04_AXI_bready,
    S04_AXI_bresp,
    S04_AXI_bvalid,
    S04_AXI_rdata,
    S04_AXI_rlast,
    S04_AXI_rready,
    S04_AXI_rresp,
    S04_AXI_rvalid,
    S04_AXI_wdata,
    S04_AXI_wlast,
    S04_AXI_wready,
    S04_AXI_wstrb,
    S04_AXI_wvalid,
    S05_AXI_araddr,
    S05_AXI_arcache,
    S05_AXI_arlen,
    S05_AXI_arlock,
    S05_AXI_arprot,
    S05_AXI_arqos,
    S05_AXI_arready,
    S05_AXI_arregion,
    S05_AXI_arvalid,
    S05_AXI_awaddr,
    S05_AXI_awcache,
    S05_AXI_awlen,
    S05_AXI_awlock,
    S05_AXI_awprot,
    S05_AXI_awqos,
    S05_AXI_awready,
    S05_AXI_awregion,
    S05_AXI_awvalid,
    S05_AXI_bready,
    S05_AXI_bresp,
    S05_AXI_bvalid,
    S05_AXI_rdata,
    S05_AXI_rlast,
    S05_AXI_rready,
    S05_AXI_rresp,
    S05_AXI_rvalid,
    S05_AXI_wdata,
    S05_AXI_wlast,
    S05_AXI_wready,
    S05_AXI_wstrb,
    S05_AXI_wvalid,
    S_AXI_CTRL_araddr,
    S_AXI_CTRL_arprot,
    S_AXI_CTRL_arready,
    S_AXI_CTRL_arvalid,
    S_AXI_CTRL_awaddr,
    S_AXI_CTRL_awprot,
    S_AXI_CTRL_awready,
    S_AXI_CTRL_awvalid,
    S_AXI_CTRL_bready,
    S_AXI_CTRL_bresp,
    S_AXI_CTRL_bvalid,
    S_AXI_CTRL_rdata,
    S_AXI_CTRL_rready,
    S_AXI_CTRL_rresp,
    S_AXI_CTRL_rvalid,
    S_AXI_CTRL_wdata,
    S_AXI_CTRL_wready,
    S_AXI_CTRL_wstrb,
    S_AXI_CTRL_wvalid,
    aclk,
    aclk1,
    aclk2,
    aresetn,
    ddr4_mem00_sys_rst,
    ddr4_mem00_ui_clk,
    ddr4_mem01_sys_rst,
    ddr4_mem01_ui_clk,
    ddr4_mem02_sys_rst,
    ddr4_mem02_ui_clk,
    ddr4_mem_calib_complete,
    ddr4_mem_calib_vec);
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM00_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000" *) input DDR4_MEM00_DIFF_CLK_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_P" *) input DDR4_MEM00_DIFF_CLK_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ACT_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM00, AXI_ARBITRATION_SCHEME TDM, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 11, CAS_WRITE_LATENCY 11, CS_ENABLED true, DATA_MASK_ENABLED true, DATA_WIDTH 8, MEMORY_TYPE COMPONENTS, MEM_ADDR_MAP ROW_COLUMN_BANK, SLOT Single, TIMEPERIOD_PS 1250" *) output DDR4_MEM00_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ADR" *) output [16:0]DDR4_MEM00_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BA" *) output [1:0]DDR4_MEM00_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BG" *) output [1:0]DDR4_MEM00_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_C" *) output [0:0]DDR4_MEM00_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_T" *) output [0:0]DDR4_MEM00_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CKE" *) output [0:0]DDR4_MEM00_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CS_N" *) output [0:0]DDR4_MEM00_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQ" *) inout [71:0]DDR4_MEM00_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_C" *) inout [17:0]DDR4_MEM00_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_T" *) inout [17:0]DDR4_MEM00_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ODT" *) output [0:0]DDR4_MEM00_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 PAR" *) output DDR4_MEM00_par;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 RESET_N" *) output DDR4_MEM00_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM01_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000" *) input DDR4_MEM01_DIFF_CLK_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_P" *) input DDR4_MEM01_DIFF_CLK_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ACT_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM01, AXI_ARBITRATION_SCHEME RD_PRI_REG, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 17, CAS_WRITE_LATENCY 12, CS_ENABLED true, CUSTOM_PARTS no_file_loaded, DATA_MASK_ENABLED NONE, DATA_WIDTH 72, MEMORY_PART MTA18ASF2G72PZ-2G3, MEMORY_TYPE RDIMMs, MEM_ADDR_MAP ROW_COLUMN_BANK_INTLV, SLOT Single, TIMEPERIOD_PS 833" *) output DDR4_MEM01_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ADR" *) output [16:0]DDR4_MEM01_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BA" *) output [1:0]DDR4_MEM01_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BG" *) output [1:0]DDR4_MEM01_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_C" *) output [0:0]DDR4_MEM01_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_T" *) output [0:0]DDR4_MEM01_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CKE" *) output [0:0]DDR4_MEM01_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CS_N" *) output [0:0]DDR4_MEM01_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQ" *) inout [71:0]DDR4_MEM01_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_C" *) inout [17:0]DDR4_MEM01_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_T" *) inout [17:0]DDR4_MEM01_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ODT" *) output [0:0]DDR4_MEM01_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 PAR" *) output DDR4_MEM01_par;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 RESET_N" *) output DDR4_MEM01_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM02_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000" *) input DDR4_MEM02_DIFF_CLK_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_P" *) input DDR4_MEM02_DIFF_CLK_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ACT_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM02, AXI_ARBITRATION_SCHEME TDM, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 11, CAS_WRITE_LATENCY 11, CS_ENABLED true, DATA_MASK_ENABLED true, DATA_WIDTH 8, MEMORY_TYPE COMPONENTS, MEM_ADDR_MAP ROW_COLUMN_BANK, SLOT Single, TIMEPERIOD_PS 1250" *) output DDR4_MEM02_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ADR" *) output [16:0]DDR4_MEM02_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BA" *) output [1:0]DDR4_MEM02_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BG" *) output [1:0]DDR4_MEM02_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_C" *) output [0:0]DDR4_MEM02_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_T" *) output [0:0]DDR4_MEM02_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CKE" *) output [0:0]DDR4_MEM02_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CS_N" *) output [0:0]DDR4_MEM02_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQ" *) inout [71:0]DDR4_MEM02_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_C" *) inout [17:0]DDR4_MEM02_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_T" *) inout [17:0]DDR4_MEM02_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ODT" *) output [0:0]DDR4_MEM02_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 PAR" *) output DDR4_MEM02_par;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 RESET_N" *) output DDR4_MEM02_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 1, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) output [38:0]M00_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST" *) output [1:0]M00_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARCACHE" *) output [3:0]M00_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN" *) output [7:0]M00_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARLOCK" *) output [0:0]M00_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARPROT" *) output [2:0]M00_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARQOS" *) output [3:0]M00_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY" *) input M00_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREGION" *) output [3:0]M00_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE" *) output [2:0]M00_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID" *) output M00_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR" *) output [38:0]M00_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST" *) output [1:0]M00_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWCACHE" *) output [3:0]M00_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN" *) output [7:0]M00_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWLOCK" *) output [0:0]M00_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWPROT" *) output [2:0]M00_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWQOS" *) output [3:0]M00_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY" *) input M00_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREGION" *) output [3:0]M00_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE" *) output [2:0]M00_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID" *) output M00_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BREADY" *) output M00_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BRESP" *) input [1:0]M00_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BVALID" *) input M00_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RDATA" *) input [511:0]M00_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RLAST" *) input M00_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RREADY" *) output M00_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RRESP" *) input [1:0]M00_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RVALID" *) input M00_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) output [511:0]M00_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WLAST" *) output M00_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WREADY" *) input M00_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) output [63:0]M00_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WVALID" *) output M00_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S00_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [38:0]S00_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARBURST" *) input [1:0]S00_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARCACHE" *) input [3:0]S00_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARID" *) input [3:0]S00_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARLEN" *) input [7:0]S00_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARLOCK" *) input [0:0]S00_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARPROT" *) input [2:0]S00_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARQOS" *) input [3:0]S00_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARREADY" *) output S00_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARREGION" *) input [3:0]S00_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARSIZE" *) input [2:0]S00_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARVALID" *) input S00_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWADDR" *) input [38:0]S00_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWBURST" *) input [1:0]S00_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWCACHE" *) input [3:0]S00_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWID" *) input [3:0]S00_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWLEN" *) input [7:0]S00_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWLOCK" *) input [0:0]S00_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWPROT" *) input [2:0]S00_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWQOS" *) input [3:0]S00_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWREADY" *) output S00_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWREGION" *) input [3:0]S00_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWSIZE" *) input [2:0]S00_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWVALID" *) input S00_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BID" *) output [3:0]S00_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BREADY" *) input S00_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BRESP" *) output [1:0]S00_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BVALID" *) output S00_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RDATA" *) output [511:0]S00_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RID" *) output [3:0]S00_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RLAST" *) output S00_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RREADY" *) input S00_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RRESP" *) output [1:0]S00_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RVALID" *) output S00_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WDATA" *) input [511:0]S00_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WLAST" *) input S00_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WREADY" *) output S00_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WSTRB" *) input [63:0]S00_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WVALID" *) input S00_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S01_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [38:0]S01_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARBURST" *) input [1:0]S01_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARCACHE" *) input [3:0]S01_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARID" *) input [3:0]S01_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARLEN" *) input [7:0]S01_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARLOCK" *) input [0:0]S01_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARPROT" *) input [2:0]S01_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARQOS" *) input [3:0]S01_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARREADY" *) output S01_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARREGION" *) input [3:0]S01_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARVALID" *) input S01_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWADDR" *) input [38:0]S01_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWBURST" *) input [1:0]S01_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWCACHE" *) input [3:0]S01_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWID" *) input [3:0]S01_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWLEN" *) input [7:0]S01_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWLOCK" *) input [0:0]S01_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWPROT" *) input [2:0]S01_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWQOS" *) input [3:0]S01_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWREADY" *) output S01_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWREGION" *) input [3:0]S01_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWVALID" *) input S01_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BID" *) output [3:0]S01_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BREADY" *) input S01_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BRESP" *) output [1:0]S01_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BVALID" *) output S01_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RDATA" *) output [511:0]S01_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RID" *) output [3:0]S01_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RLAST" *) output S01_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RREADY" *) input S01_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RRESP" *) output [1:0]S01_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RVALID" *) output S01_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WDATA" *) input [511:0]S01_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WLAST" *) input S01_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WREADY" *) output S01_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WSTRB" *) input [63:0]S01_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WVALID" *) input S01_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S02_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [38:0]S02_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARBURST" *) input [1:0]S02_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARCACHE" *) input [3:0]S02_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARID" *) input [3:0]S02_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARLEN" *) input [7:0]S02_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARLOCK" *) input [0:0]S02_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARPROT" *) input [2:0]S02_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARQOS" *) input [3:0]S02_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARREADY" *) output S02_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARREGION" *) input [3:0]S02_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARVALID" *) input S02_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWADDR" *) input [38:0]S02_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWBURST" *) input [1:0]S02_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWCACHE" *) input [3:0]S02_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWID" *) input [3:0]S02_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWLEN" *) input [7:0]S02_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWLOCK" *) input [0:0]S02_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWPROT" *) input [2:0]S02_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWQOS" *) input [3:0]S02_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWREADY" *) output S02_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWREGION" *) input [3:0]S02_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWVALID" *) input S02_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BID" *) output [3:0]S02_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BREADY" *) input S02_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BRESP" *) output [1:0]S02_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BVALID" *) output S02_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RDATA" *) output [511:0]S02_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RID" *) output [3:0]S02_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RLAST" *) output S02_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RREADY" *) input S02_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RRESP" *) output [1:0]S02_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RVALID" *) output S02_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WDATA" *) input [511:0]S02_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WLAST" *) input S02_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WREADY" *) output S02_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WSTRB" *) input [63:0]S02_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WVALID" *) input S02_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S03_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [38:0]S03_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARBURST" *) input [1:0]S03_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARCACHE" *) input [3:0]S03_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARID" *) input [3:0]S03_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARLEN" *) input [7:0]S03_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARLOCK" *) input [0:0]S03_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARPROT" *) input [2:0]S03_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARQOS" *) input [3:0]S03_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARREADY" *) output S03_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARREGION" *) input [3:0]S03_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARSIZE" *) input [2:0]S03_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARVALID" *) input S03_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWADDR" *) input [38:0]S03_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWBURST" *) input [1:0]S03_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWCACHE" *) input [3:0]S03_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWID" *) input [3:0]S03_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWLEN" *) input [7:0]S03_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWLOCK" *) input [0:0]S03_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWPROT" *) input [2:0]S03_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWQOS" *) input [3:0]S03_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWREADY" *) output S03_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWREGION" *) input [3:0]S03_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWSIZE" *) input [2:0]S03_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWVALID" *) input S03_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BID" *) output [3:0]S03_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BREADY" *) input S03_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BRESP" *) output [1:0]S03_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BVALID" *) output S03_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RDATA" *) output [511:0]S03_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RID" *) output [3:0]S03_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RLAST" *) output S03_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RREADY" *) input S03_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RRESP" *) output [1:0]S03_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RVALID" *) output S03_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WDATA" *) input [511:0]S03_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WLAST" *) input S03_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WREADY" *) output S03_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WSTRB" *) input [63:0]S03_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WVALID" *) input S03_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S04_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 0, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [38:0]S04_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARCACHE" *) input [3:0]S04_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARLEN" *) input [7:0]S04_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARLOCK" *) input [0:0]S04_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARPROT" *) input [2:0]S04_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARQOS" *) input [3:0]S04_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARREADY" *) output S04_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARREGION" *) input [3:0]S04_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARVALID" *) input S04_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWADDR" *) input [38:0]S04_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWCACHE" *) input [3:0]S04_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWLEN" *) input [7:0]S04_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWLOCK" *) input [0:0]S04_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWPROT" *) input [2:0]S04_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWQOS" *) input [3:0]S04_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWREADY" *) output S04_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWREGION" *) input [3:0]S04_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWVALID" *) input S04_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI BREADY" *) input S04_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI BRESP" *) output [1:0]S04_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI BVALID" *) output S04_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RDATA" *) output [511:0]S04_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RLAST" *) output S04_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RREADY" *) input S04_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RRESP" *) output [1:0]S04_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RVALID" *) output S04_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WDATA" *) input [511:0]S04_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WLAST" *) input S04_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WREADY" *) output S04_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WSTRB" *) input [63:0]S04_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WVALID" *) input S04_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S05_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 0, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [38:0]S05_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARCACHE" *) input [3:0]S05_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARLEN" *) input [7:0]S05_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARLOCK" *) input [0:0]S05_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARPROT" *) input [2:0]S05_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARQOS" *) input [3:0]S05_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARREADY" *) output S05_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARREGION" *) input [3:0]S05_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARVALID" *) input S05_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWADDR" *) input [38:0]S05_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWCACHE" *) input [3:0]S05_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWLEN" *) input [7:0]S05_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWLOCK" *) input [0:0]S05_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWPROT" *) input [2:0]S05_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWQOS" *) input [3:0]S05_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWREADY" *) output S05_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWREGION" *) input [3:0]S05_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWVALID" *) input S05_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI BREADY" *) input S05_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI BRESP" *) output [1:0]S05_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI BVALID" *) output S05_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RDATA" *) output [511:0]S05_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RLAST" *) output S05_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RREADY" *) input S05_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RRESP" *) output [1:0]S05_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RVALID" *) output S05_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WDATA" *) input [511:0]S05_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WLAST" *) input S05_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WREADY" *) output S05_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WSTRB" *) input [63:0]S05_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WVALID" *) input S05_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S_AXI_CTRL, ADDR_WIDTH 25, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, DATA_WIDTH 32, FREQ_HZ 50925925, HAS_BRESP 1, HAS_BURST 0, HAS_CACHE 0, HAS_LOCK 0, HAS_PROT 1, HAS_QOS 0, HAS_REGION 0, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 1, NUM_READ_OUTSTANDING 2, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 2, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4LITE, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0" *) input [24:0]S_AXI_CTRL_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARPROT" *) input [2:0]S_AXI_CTRL_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARREADY" *) output S_AXI_CTRL_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARVALID" *) input S_AXI_CTRL_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWADDR" *) input [24:0]S_AXI_CTRL_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWPROT" *) input [2:0]S_AXI_CTRL_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWREADY" *) output S_AXI_CTRL_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWVALID" *) input S_AXI_CTRL_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BREADY" *) input S_AXI_CTRL_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BRESP" *) output [1:0]S_AXI_CTRL_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BVALID" *) output S_AXI_CTRL_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RDATA" *) output [31:0]S_AXI_CTRL_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RREADY" *) input S_AXI_CTRL_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RRESP" *) output [1:0]S_AXI_CTRL_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RVALID" *) output S_AXI_CTRL_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WDATA" *) input [31:0]S_AXI_CTRL_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WREADY" *) output S_AXI_CTRL_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WSTRB" *) input [3:0]S_AXI_CTRL_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WVALID" *) input S_AXI_CTRL_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.ACLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.ACLK, ASSOCIATED_BUSIF S00_AXI:S01_AXI:S02_AXI:S03_AXI:S04_AXI:S05_AXI, ASSOCIATED_CLKEN m_sc_aclken, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000" *) input aclk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.ACLK1 CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.ACLK1, ASSOCIATED_BUSIF S_AXI_CTRL, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, FREQ_HZ 50925925, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000" *) input aclk1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.ACLK2 CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.ACLK2, ASSOCIATED_BUSIF M00_AXI, ASSOCIATED_CLKEN m_sc_aclken, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000" *) input aclk2;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.ARESETN RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.ARESETN, INSERT_VIP 0, POLARITY ACTIVE_LOW" *) input aresetn;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.DDR4_MEM00_SYS_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.DDR4_MEM00_SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input ddr4_mem00_sys_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.DDR4_MEM00_UI_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.DDR4_MEM00_UI_CLK, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem00_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000" *) output ddr4_mem00_ui_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.DDR4_MEM01_SYS_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.DDR4_MEM01_SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input ddr4_mem01_sys_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.DDR4_MEM01_UI_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.DDR4_MEM01_UI_CLK, CLK_DOMAIN bd_d216_ddr4_mem01_0_c0_ddr4_ui_clk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.00" *) output ddr4_mem01_ui_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.DDR4_MEM02_SYS_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.DDR4_MEM02_SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input ddr4_mem02_sys_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.DDR4_MEM02_UI_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.DDR4_MEM02_UI_CLK, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem02_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000" *) output ddr4_mem02_ui_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.DDR4_MEM_CALIB_COMPLETE DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.DDR4_MEM_CALIB_COMPLETE, LAYERED_METADATA undef" *) output ddr4_mem_calib_complete;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.DDR4_MEM_CALIB_VEC DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.DDR4_MEM_CALIB_VEC, LAYERED_METADATA undef, PortWidth 3" *) output [2:0]ddr4_mem_calib_vec;

  wire \<const0> ;
  wire DDR4_MEM01_DIFF_CLK_clk_n;
  wire DDR4_MEM01_DIFF_CLK_clk_p;
  wire DDR4_MEM01_act_n;
  wire [16:0]DDR4_MEM01_adr;
  wire [1:0]DDR4_MEM01_ba;
  wire [1:0]DDR4_MEM01_bg;
  wire [0:0]DDR4_MEM01_ck_c;
  wire [0:0]DDR4_MEM01_ck_t;
  wire [0:0]DDR4_MEM01_cke;
  wire [0:0]DDR4_MEM01_cs_n;
  wire [71:0]DDR4_MEM01_dq;
  wire [17:0]DDR4_MEM01_dqs_c;
  wire [17:0]DDR4_MEM01_dqs_t;
  wire [0:0]DDR4_MEM01_odt;
  wire DDR4_MEM01_par;
  wire DDR4_MEM01_reset_n;
  wire [38:0]M00_AXI_araddr;
  wire [1:0]M00_AXI_arburst;
  wire [3:0]M00_AXI_arcache;
  wire [7:0]M00_AXI_arlen;
  wire [0:0]M00_AXI_arlock;
  wire [2:0]M00_AXI_arprot;
  wire [3:0]M00_AXI_arqos;
  wire M00_AXI_arready;
  wire [3:0]M00_AXI_arregion;
  wire [2:0]M00_AXI_arsize;
  wire M00_AXI_arvalid;
  wire [38:0]M00_AXI_awaddr;
  wire [1:0]M00_AXI_awburst;
  wire [3:0]M00_AXI_awcache;
  wire [7:0]M00_AXI_awlen;
  wire [0:0]M00_AXI_awlock;
  wire [2:0]M00_AXI_awprot;
  wire [3:0]M00_AXI_awqos;
  wire M00_AXI_awready;
  wire [3:0]M00_AXI_awregion;
  wire [2:0]M00_AXI_awsize;
  wire M00_AXI_awvalid;
  wire M00_AXI_bready;
  wire [1:0]M00_AXI_bresp;
  wire M00_AXI_bvalid;
  wire [511:0]M00_AXI_rdata;
  wire M00_AXI_rlast;
  wire M00_AXI_rready;
  wire [1:0]M00_AXI_rresp;
  wire M00_AXI_rvalid;
  wire [511:0]M00_AXI_wdata;
  wire M00_AXI_wlast;
  wire M00_AXI_wready;
  wire [63:0]M00_AXI_wstrb;
  wire M00_AXI_wvalid;
  wire [38:0]S01_AXI_araddr;
  wire [1:0]S01_AXI_arburst;
  wire [3:0]S01_AXI_arcache;
  wire [3:0]S01_AXI_arid;
  wire [7:0]S01_AXI_arlen;
  wire [0:0]S01_AXI_arlock;
  wire [2:0]S01_AXI_arprot;
  wire [3:0]S01_AXI_arqos;
  wire S01_AXI_arready;
  wire [3:0]S01_AXI_arregion;
  wire S01_AXI_arvalid;
  wire [38:0]S01_AXI_awaddr;
  wire [1:0]S01_AXI_awburst;
  wire [3:0]S01_AXI_awcache;
  wire [3:0]S01_AXI_awid;
  wire [7:0]S01_AXI_awlen;
  wire [0:0]S01_AXI_awlock;
  wire [2:0]S01_AXI_awprot;
  wire [3:0]S01_AXI_awqos;
  wire S01_AXI_awready;
  wire [3:0]S01_AXI_awregion;
  wire S01_AXI_awvalid;
  wire [3:0]S01_AXI_bid;
  wire S01_AXI_bready;
  wire [1:0]S01_AXI_bresp;
  wire S01_AXI_bvalid;
  wire [511:0]S01_AXI_rdata;
  wire [3:0]S01_AXI_rid;
  wire S01_AXI_rlast;
  wire S01_AXI_rready;
  wire [1:0]S01_AXI_rresp;
  wire S01_AXI_rvalid;
  wire [511:0]S01_AXI_wdata;
  wire S01_AXI_wlast;
  wire S01_AXI_wready;
  wire [63:0]S01_AXI_wstrb;
  wire S01_AXI_wvalid;
  wire [38:0]S02_AXI_araddr;
  wire [1:0]S02_AXI_arburst;
  wire [3:0]S02_AXI_arcache;
  wire [3:0]S02_AXI_arid;
  wire [7:0]S02_AXI_arlen;
  wire [0:0]S02_AXI_arlock;
  wire [2:0]S02_AXI_arprot;
  wire [3:0]S02_AXI_arqos;
  wire S02_AXI_arready;
  wire [3:0]S02_AXI_arregion;
  wire S02_AXI_arvalid;
  wire [38:0]S02_AXI_awaddr;
  wire [1:0]S02_AXI_awburst;
  wire [3:0]S02_AXI_awcache;
  wire [3:0]S02_AXI_awid;
  wire [7:0]S02_AXI_awlen;
  wire [0:0]S02_AXI_awlock;
  wire [2:0]S02_AXI_awprot;
  wire [3:0]S02_AXI_awqos;
  wire S02_AXI_awready;
  wire [3:0]S02_AXI_awregion;
  wire S02_AXI_awvalid;
  wire [3:0]S02_AXI_bid;
  wire S02_AXI_bready;
  wire [1:0]S02_AXI_bresp;
  wire S02_AXI_bvalid;
  wire [511:0]S02_AXI_rdata;
  wire [3:0]S02_AXI_rid;
  wire S02_AXI_rlast;
  wire S02_AXI_rready;
  wire [1:0]S02_AXI_rresp;
  wire S02_AXI_rvalid;
  wire [511:0]S02_AXI_wdata;
  wire S02_AXI_wlast;
  wire S02_AXI_wready;
  wire [63:0]S02_AXI_wstrb;
  wire S02_AXI_wvalid;
  wire [38:0]S04_AXI_araddr;
  wire [3:0]S04_AXI_arcache;
  wire [7:0]S04_AXI_arlen;
  wire [0:0]S04_AXI_arlock;
  wire [2:0]S04_AXI_arprot;
  wire [3:0]S04_AXI_arqos;
  wire S04_AXI_arready;
  wire [3:0]S04_AXI_arregion;
  wire S04_AXI_arvalid;
  wire [38:0]S04_AXI_awaddr;
  wire [3:0]S04_AXI_awcache;
  wire [7:0]S04_AXI_awlen;
  wire [0:0]S04_AXI_awlock;
  wire [2:0]S04_AXI_awprot;
  wire [3:0]S04_AXI_awqos;
  wire S04_AXI_awready;
  wire [3:0]S04_AXI_awregion;
  wire S04_AXI_awvalid;
  wire S04_AXI_bready;
  wire [1:0]S04_AXI_bresp;
  wire S04_AXI_bvalid;
  wire [511:0]S04_AXI_rdata;
  wire S04_AXI_rlast;
  wire S04_AXI_rready;
  wire [1:0]S04_AXI_rresp;
  wire S04_AXI_rvalid;
  wire [511:0]S04_AXI_wdata;
  wire S04_AXI_wlast;
  wire S04_AXI_wready;
  wire [63:0]S04_AXI_wstrb;
  wire S04_AXI_wvalid;
  wire [38:0]S05_AXI_araddr;
  wire [3:0]S05_AXI_arcache;
  wire [7:0]S05_AXI_arlen;
  wire [0:0]S05_AXI_arlock;
  wire [2:0]S05_AXI_arprot;
  wire [3:0]S05_AXI_arqos;
  wire S05_AXI_arready;
  wire [3:0]S05_AXI_arregion;
  wire S05_AXI_arvalid;
  wire [38:0]S05_AXI_awaddr;
  wire [3:0]S05_AXI_awcache;
  wire [7:0]S05_AXI_awlen;
  wire [0:0]S05_AXI_awlock;
  wire [2:0]S05_AXI_awprot;
  wire [3:0]S05_AXI_awqos;
  wire S05_AXI_awready;
  wire [3:0]S05_AXI_awregion;
  wire S05_AXI_awvalid;
  wire S05_AXI_bready;
  wire [1:0]S05_AXI_bresp;
  wire S05_AXI_bvalid;
  wire [511:0]S05_AXI_rdata;
  wire S05_AXI_rlast;
  wire S05_AXI_rready;
  wire [1:0]S05_AXI_rresp;
  wire S05_AXI_rvalid;
  wire [511:0]S05_AXI_wdata;
  wire S05_AXI_wlast;
  wire S05_AXI_wready;
  wire [63:0]S05_AXI_wstrb;
  wire S05_AXI_wvalid;
  wire [24:0]S_AXI_CTRL_araddr;
  wire [2:0]S_AXI_CTRL_arprot;
  wire S_AXI_CTRL_arready;
  wire S_AXI_CTRL_arvalid;
  wire [24:0]S_AXI_CTRL_awaddr;
  wire [2:0]S_AXI_CTRL_awprot;
  wire S_AXI_CTRL_awready;
  wire S_AXI_CTRL_awvalid;
  wire S_AXI_CTRL_bready;
  wire [1:0]S_AXI_CTRL_bresp;
  wire S_AXI_CTRL_bvalid;
  wire [31:0]S_AXI_CTRL_rdata;
  wire S_AXI_CTRL_rready;
  wire [1:0]S_AXI_CTRL_rresp;
  wire S_AXI_CTRL_rvalid;
  wire [31:0]S_AXI_CTRL_wdata;
  wire S_AXI_CTRL_wready;
  wire [3:0]S_AXI_CTRL_wstrb;
  wire S_AXI_CTRL_wvalid;
  wire aclk;
  wire aclk1;
  wire aclk2;
  wire aresetn;
  wire ddr4_mem01_sys_rst;
  wire ddr4_mem01_ui_clk;
  wire ddr4_mem_calib_complete;
  wire [2:0]ddr4_mem_calib_vec;
  wire [33:0]interconnect_DDR4_MEM01_M00_AXI_ARADDR;
  wire [1:0]interconnect_DDR4_MEM01_M00_AXI_ARBURST;
  wire [3:0]interconnect_DDR4_MEM01_M00_AXI_ARCACHE;
  wire [7:0]interconnect_DDR4_MEM01_M00_AXI_ARLEN;
  wire interconnect_DDR4_MEM01_M00_AXI_ARLOCK;
  wire [2:0]interconnect_DDR4_MEM01_M00_AXI_ARPROT;
  wire [3:0]interconnect_DDR4_MEM01_M00_AXI_ARQOS;
  wire interconnect_DDR4_MEM01_M00_AXI_ARREADY;
  wire interconnect_DDR4_MEM01_M00_AXI_ARVALID;
  wire [33:0]interconnect_DDR4_MEM01_M00_AXI_AWADDR;
  wire [1:0]interconnect_DDR4_MEM01_M00_AXI_AWBURST;
  wire [3:0]interconnect_DDR4_MEM01_M00_AXI_AWCACHE;
  wire [7:0]interconnect_DDR4_MEM01_M00_AXI_AWLEN;
  wire interconnect_DDR4_MEM01_M00_AXI_AWLOCK;
  wire [2:0]interconnect_DDR4_MEM01_M00_AXI_AWPROT;
  wire [3:0]interconnect_DDR4_MEM01_M00_AXI_AWQOS;
  wire interconnect_DDR4_MEM01_M00_AXI_AWREADY;
  wire interconnect_DDR4_MEM01_M00_AXI_AWVALID;
  wire interconnect_DDR4_MEM01_M00_AXI_BREADY;
  wire [1:0]interconnect_DDR4_MEM01_M00_AXI_BRESP;
  wire interconnect_DDR4_MEM01_M00_AXI_BVALID;
  wire [511:0]interconnect_DDR4_MEM01_M00_AXI_RDATA;
  wire interconnect_DDR4_MEM01_M00_AXI_RLAST;
  wire interconnect_DDR4_MEM01_M00_AXI_RREADY;
  wire [1:0]interconnect_DDR4_MEM01_M00_AXI_RRESP;
  wire interconnect_DDR4_MEM01_M00_AXI_RVALID;
  wire [511:0]interconnect_DDR4_MEM01_M00_AXI_WDATA;
  wire interconnect_DDR4_MEM01_M00_AXI_WLAST;
  wire interconnect_DDR4_MEM01_M00_AXI_WREADY;
  wire [63:0]interconnect_DDR4_MEM01_M00_AXI_WSTRB;
  wire interconnect_DDR4_MEM01_M00_AXI_WVALID;
  wire psr_aclk2_SLR1_interconnect_aresetn;
  wire psr_aclk_SLR1_interconnect_aresetn;

  assign DDR4_MEM00_act_n = \<const0> ;
  assign DDR4_MEM00_adr[16] = \<const0> ;
  assign DDR4_MEM00_adr[15] = \<const0> ;
  assign DDR4_MEM00_adr[14] = \<const0> ;
  assign DDR4_MEM00_adr[13] = \<const0> ;
  assign DDR4_MEM00_adr[12] = \<const0> ;
  assign DDR4_MEM00_adr[11] = \<const0> ;
  assign DDR4_MEM00_adr[10] = \<const0> ;
  assign DDR4_MEM00_adr[9] = \<const0> ;
  assign DDR4_MEM00_adr[8] = \<const0> ;
  assign DDR4_MEM00_adr[7] = \<const0> ;
  assign DDR4_MEM00_adr[6] = \<const0> ;
  assign DDR4_MEM00_adr[5] = \<const0> ;
  assign DDR4_MEM00_adr[4] = \<const0> ;
  assign DDR4_MEM00_adr[3] = \<const0> ;
  assign DDR4_MEM00_adr[2] = \<const0> ;
  assign DDR4_MEM00_adr[1] = \<const0> ;
  assign DDR4_MEM00_adr[0] = \<const0> ;
  assign DDR4_MEM00_ba[1] = \<const0> ;
  assign DDR4_MEM00_ba[0] = \<const0> ;
  assign DDR4_MEM00_bg[1] = \<const0> ;
  assign DDR4_MEM00_bg[0] = \<const0> ;
  assign DDR4_MEM00_ck_c[0] = \<const0> ;
  assign DDR4_MEM00_ck_t[0] = \<const0> ;
  assign DDR4_MEM00_cke[0] = \<const0> ;
  assign DDR4_MEM00_cs_n[0] = \<const0> ;
  assign DDR4_MEM00_odt[0] = \<const0> ;
  assign DDR4_MEM00_par = \<const0> ;
  assign DDR4_MEM00_reset_n = \<const0> ;
  assign DDR4_MEM02_act_n = \<const0> ;
  assign DDR4_MEM02_adr[16] = \<const0> ;
  assign DDR4_MEM02_adr[15] = \<const0> ;
  assign DDR4_MEM02_adr[14] = \<const0> ;
  assign DDR4_MEM02_adr[13] = \<const0> ;
  assign DDR4_MEM02_adr[12] = \<const0> ;
  assign DDR4_MEM02_adr[11] = \<const0> ;
  assign DDR4_MEM02_adr[10] = \<const0> ;
  assign DDR4_MEM02_adr[9] = \<const0> ;
  assign DDR4_MEM02_adr[8] = \<const0> ;
  assign DDR4_MEM02_adr[7] = \<const0> ;
  assign DDR4_MEM02_adr[6] = \<const0> ;
  assign DDR4_MEM02_adr[5] = \<const0> ;
  assign DDR4_MEM02_adr[4] = \<const0> ;
  assign DDR4_MEM02_adr[3] = \<const0> ;
  assign DDR4_MEM02_adr[2] = \<const0> ;
  assign DDR4_MEM02_adr[1] = \<const0> ;
  assign DDR4_MEM02_adr[0] = \<const0> ;
  assign DDR4_MEM02_ba[1] = \<const0> ;
  assign DDR4_MEM02_ba[0] = \<const0> ;
  assign DDR4_MEM02_bg[1] = \<const0> ;
  assign DDR4_MEM02_bg[0] = \<const0> ;
  assign DDR4_MEM02_ck_c[0] = \<const0> ;
  assign DDR4_MEM02_ck_t[0] = \<const0> ;
  assign DDR4_MEM02_cke[0] = \<const0> ;
  assign DDR4_MEM02_cs_n[0] = \<const0> ;
  assign DDR4_MEM02_odt[0] = \<const0> ;
  assign DDR4_MEM02_par = \<const0> ;
  assign DDR4_MEM02_reset_n = \<const0> ;
  assign S00_AXI_arready = \<const0> ;
  assign S00_AXI_awready = \<const0> ;
  assign S00_AXI_bid[3] = \<const0> ;
  assign S00_AXI_bid[2] = \<const0> ;
  assign S00_AXI_bid[1] = \<const0> ;
  assign S00_AXI_bid[0] = \<const0> ;
  assign S00_AXI_bresp[1] = \<const0> ;
  assign S00_AXI_bresp[0] = \<const0> ;
  assign S00_AXI_bvalid = \<const0> ;
  assign S00_AXI_rdata[511] = \<const0> ;
  assign S00_AXI_rdata[510] = \<const0> ;
  assign S00_AXI_rdata[509] = \<const0> ;
  assign S00_AXI_rdata[508] = \<const0> ;
  assign S00_AXI_rdata[507] = \<const0> ;
  assign S00_AXI_rdata[506] = \<const0> ;
  assign S00_AXI_rdata[505] = \<const0> ;
  assign S00_AXI_rdata[504] = \<const0> ;
  assign S00_AXI_rdata[503] = \<const0> ;
  assign S00_AXI_rdata[502] = \<const0> ;
  assign S00_AXI_rdata[501] = \<const0> ;
  assign S00_AXI_rdata[500] = \<const0> ;
  assign S00_AXI_rdata[499] = \<const0> ;
  assign S00_AXI_rdata[498] = \<const0> ;
  assign S00_AXI_rdata[497] = \<const0> ;
  assign S00_AXI_rdata[496] = \<const0> ;
  assign S00_AXI_rdata[495] = \<const0> ;
  assign S00_AXI_rdata[494] = \<const0> ;
  assign S00_AXI_rdata[493] = \<const0> ;
  assign S00_AXI_rdata[492] = \<const0> ;
  assign S00_AXI_rdata[491] = \<const0> ;
  assign S00_AXI_rdata[490] = \<const0> ;
  assign S00_AXI_rdata[489] = \<const0> ;
  assign S00_AXI_rdata[488] = \<const0> ;
  assign S00_AXI_rdata[487] = \<const0> ;
  assign S00_AXI_rdata[486] = \<const0> ;
  assign S00_AXI_rdata[485] = \<const0> ;
  assign S00_AXI_rdata[484] = \<const0> ;
  assign S00_AXI_rdata[483] = \<const0> ;
  assign S00_AXI_rdata[482] = \<const0> ;
  assign S00_AXI_rdata[481] = \<const0> ;
  assign S00_AXI_rdata[480] = \<const0> ;
  assign S00_AXI_rdata[479] = \<const0> ;
  assign S00_AXI_rdata[478] = \<const0> ;
  assign S00_AXI_rdata[477] = \<const0> ;
  assign S00_AXI_rdata[476] = \<const0> ;
  assign S00_AXI_rdata[475] = \<const0> ;
  assign S00_AXI_rdata[474] = \<const0> ;
  assign S00_AXI_rdata[473] = \<const0> ;
  assign S00_AXI_rdata[472] = \<const0> ;
  assign S00_AXI_rdata[471] = \<const0> ;
  assign S00_AXI_rdata[470] = \<const0> ;
  assign S00_AXI_rdata[469] = \<const0> ;
  assign S00_AXI_rdata[468] = \<const0> ;
  assign S00_AXI_rdata[467] = \<const0> ;
  assign S00_AXI_rdata[466] = \<const0> ;
  assign S00_AXI_rdata[465] = \<const0> ;
  assign S00_AXI_rdata[464] = \<const0> ;
  assign S00_AXI_rdata[463] = \<const0> ;
  assign S00_AXI_rdata[462] = \<const0> ;
  assign S00_AXI_rdata[461] = \<const0> ;
  assign S00_AXI_rdata[460] = \<const0> ;
  assign S00_AXI_rdata[459] = \<const0> ;
  assign S00_AXI_rdata[458] = \<const0> ;
  assign S00_AXI_rdata[457] = \<const0> ;
  assign S00_AXI_rdata[456] = \<const0> ;
  assign S00_AXI_rdata[455] = \<const0> ;
  assign S00_AXI_rdata[454] = \<const0> ;
  assign S00_AXI_rdata[453] = \<const0> ;
  assign S00_AXI_rdata[452] = \<const0> ;
  assign S00_AXI_rdata[451] = \<const0> ;
  assign S00_AXI_rdata[450] = \<const0> ;
  assign S00_AXI_rdata[449] = \<const0> ;
  assign S00_AXI_rdata[448] = \<const0> ;
  assign S00_AXI_rdata[447] = \<const0> ;
  assign S00_AXI_rdata[446] = \<const0> ;
  assign S00_AXI_rdata[445] = \<const0> ;
  assign S00_AXI_rdata[444] = \<const0> ;
  assign S00_AXI_rdata[443] = \<const0> ;
  assign S00_AXI_rdata[442] = \<const0> ;
  assign S00_AXI_rdata[441] = \<const0> ;
  assign S00_AXI_rdata[440] = \<const0> ;
  assign S00_AXI_rdata[439] = \<const0> ;
  assign S00_AXI_rdata[438] = \<const0> ;
  assign S00_AXI_rdata[437] = \<const0> ;
  assign S00_AXI_rdata[436] = \<const0> ;
  assign S00_AXI_rdata[435] = \<const0> ;
  assign S00_AXI_rdata[434] = \<const0> ;
  assign S00_AXI_rdata[433] = \<const0> ;
  assign S00_AXI_rdata[432] = \<const0> ;
  assign S00_AXI_rdata[431] = \<const0> ;
  assign S00_AXI_rdata[430] = \<const0> ;
  assign S00_AXI_rdata[429] = \<const0> ;
  assign S00_AXI_rdata[428] = \<const0> ;
  assign S00_AXI_rdata[427] = \<const0> ;
  assign S00_AXI_rdata[426] = \<const0> ;
  assign S00_AXI_rdata[425] = \<const0> ;
  assign S00_AXI_rdata[424] = \<const0> ;
  assign S00_AXI_rdata[423] = \<const0> ;
  assign S00_AXI_rdata[422] = \<const0> ;
  assign S00_AXI_rdata[421] = \<const0> ;
  assign S00_AXI_rdata[420] = \<const0> ;
  assign S00_AXI_rdata[419] = \<const0> ;
  assign S00_AXI_rdata[418] = \<const0> ;
  assign S00_AXI_rdata[417] = \<const0> ;
  assign S00_AXI_rdata[416] = \<const0> ;
  assign S00_AXI_rdata[415] = \<const0> ;
  assign S00_AXI_rdata[414] = \<const0> ;
  assign S00_AXI_rdata[413] = \<const0> ;
  assign S00_AXI_rdata[412] = \<const0> ;
  assign S00_AXI_rdata[411] = \<const0> ;
  assign S00_AXI_rdata[410] = \<const0> ;
  assign S00_AXI_rdata[409] = \<const0> ;
  assign S00_AXI_rdata[408] = \<const0> ;
  assign S00_AXI_rdata[407] = \<const0> ;
  assign S00_AXI_rdata[406] = \<const0> ;
  assign S00_AXI_rdata[405] = \<const0> ;
  assign S00_AXI_rdata[404] = \<const0> ;
  assign S00_AXI_rdata[403] = \<const0> ;
  assign S00_AXI_rdata[402] = \<const0> ;
  assign S00_AXI_rdata[401] = \<const0> ;
  assign S00_AXI_rdata[400] = \<const0> ;
  assign S00_AXI_rdata[399] = \<const0> ;
  assign S00_AXI_rdata[398] = \<const0> ;
  assign S00_AXI_rdata[397] = \<const0> ;
  assign S00_AXI_rdata[396] = \<const0> ;
  assign S00_AXI_rdata[395] = \<const0> ;
  assign S00_AXI_rdata[394] = \<const0> ;
  assign S00_AXI_rdata[393] = \<const0> ;
  assign S00_AXI_rdata[392] = \<const0> ;
  assign S00_AXI_rdata[391] = \<const0> ;
  assign S00_AXI_rdata[390] = \<const0> ;
  assign S00_AXI_rdata[389] = \<const0> ;
  assign S00_AXI_rdata[388] = \<const0> ;
  assign S00_AXI_rdata[387] = \<const0> ;
  assign S00_AXI_rdata[386] = \<const0> ;
  assign S00_AXI_rdata[385] = \<const0> ;
  assign S00_AXI_rdata[384] = \<const0> ;
  assign S00_AXI_rdata[383] = \<const0> ;
  assign S00_AXI_rdata[382] = \<const0> ;
  assign S00_AXI_rdata[381] = \<const0> ;
  assign S00_AXI_rdata[380] = \<const0> ;
  assign S00_AXI_rdata[379] = \<const0> ;
  assign S00_AXI_rdata[378] = \<const0> ;
  assign S00_AXI_rdata[377] = \<const0> ;
  assign S00_AXI_rdata[376] = \<const0> ;
  assign S00_AXI_rdata[375] = \<const0> ;
  assign S00_AXI_rdata[374] = \<const0> ;
  assign S00_AXI_rdata[373] = \<const0> ;
  assign S00_AXI_rdata[372] = \<const0> ;
  assign S00_AXI_rdata[371] = \<const0> ;
  assign S00_AXI_rdata[370] = \<const0> ;
  assign S00_AXI_rdata[369] = \<const0> ;
  assign S00_AXI_rdata[368] = \<const0> ;
  assign S00_AXI_rdata[367] = \<const0> ;
  assign S00_AXI_rdata[366] = \<const0> ;
  assign S00_AXI_rdata[365] = \<const0> ;
  assign S00_AXI_rdata[364] = \<const0> ;
  assign S00_AXI_rdata[363] = \<const0> ;
  assign S00_AXI_rdata[362] = \<const0> ;
  assign S00_AXI_rdata[361] = \<const0> ;
  assign S00_AXI_rdata[360] = \<const0> ;
  assign S00_AXI_rdata[359] = \<const0> ;
  assign S00_AXI_rdata[358] = \<const0> ;
  assign S00_AXI_rdata[357] = \<const0> ;
  assign S00_AXI_rdata[356] = \<const0> ;
  assign S00_AXI_rdata[355] = \<const0> ;
  assign S00_AXI_rdata[354] = \<const0> ;
  assign S00_AXI_rdata[353] = \<const0> ;
  assign S00_AXI_rdata[352] = \<const0> ;
  assign S00_AXI_rdata[351] = \<const0> ;
  assign S00_AXI_rdata[350] = \<const0> ;
  assign S00_AXI_rdata[349] = \<const0> ;
  assign S00_AXI_rdata[348] = \<const0> ;
  assign S00_AXI_rdata[347] = \<const0> ;
  assign S00_AXI_rdata[346] = \<const0> ;
  assign S00_AXI_rdata[345] = \<const0> ;
  assign S00_AXI_rdata[344] = \<const0> ;
  assign S00_AXI_rdata[343] = \<const0> ;
  assign S00_AXI_rdata[342] = \<const0> ;
  assign S00_AXI_rdata[341] = \<const0> ;
  assign S00_AXI_rdata[340] = \<const0> ;
  assign S00_AXI_rdata[339] = \<const0> ;
  assign S00_AXI_rdata[338] = \<const0> ;
  assign S00_AXI_rdata[337] = \<const0> ;
  assign S00_AXI_rdata[336] = \<const0> ;
  assign S00_AXI_rdata[335] = \<const0> ;
  assign S00_AXI_rdata[334] = \<const0> ;
  assign S00_AXI_rdata[333] = \<const0> ;
  assign S00_AXI_rdata[332] = \<const0> ;
  assign S00_AXI_rdata[331] = \<const0> ;
  assign S00_AXI_rdata[330] = \<const0> ;
  assign S00_AXI_rdata[329] = \<const0> ;
  assign S00_AXI_rdata[328] = \<const0> ;
  assign S00_AXI_rdata[327] = \<const0> ;
  assign S00_AXI_rdata[326] = \<const0> ;
  assign S00_AXI_rdata[325] = \<const0> ;
  assign S00_AXI_rdata[324] = \<const0> ;
  assign S00_AXI_rdata[323] = \<const0> ;
  assign S00_AXI_rdata[322] = \<const0> ;
  assign S00_AXI_rdata[321] = \<const0> ;
  assign S00_AXI_rdata[320] = \<const0> ;
  assign S00_AXI_rdata[319] = \<const0> ;
  assign S00_AXI_rdata[318] = \<const0> ;
  assign S00_AXI_rdata[317] = \<const0> ;
  assign S00_AXI_rdata[316] = \<const0> ;
  assign S00_AXI_rdata[315] = \<const0> ;
  assign S00_AXI_rdata[314] = \<const0> ;
  assign S00_AXI_rdata[313] = \<const0> ;
  assign S00_AXI_rdata[312] = \<const0> ;
  assign S00_AXI_rdata[311] = \<const0> ;
  assign S00_AXI_rdata[310] = \<const0> ;
  assign S00_AXI_rdata[309] = \<const0> ;
  assign S00_AXI_rdata[308] = \<const0> ;
  assign S00_AXI_rdata[307] = \<const0> ;
  assign S00_AXI_rdata[306] = \<const0> ;
  assign S00_AXI_rdata[305] = \<const0> ;
  assign S00_AXI_rdata[304] = \<const0> ;
  assign S00_AXI_rdata[303] = \<const0> ;
  assign S00_AXI_rdata[302] = \<const0> ;
  assign S00_AXI_rdata[301] = \<const0> ;
  assign S00_AXI_rdata[300] = \<const0> ;
  assign S00_AXI_rdata[299] = \<const0> ;
  assign S00_AXI_rdata[298] = \<const0> ;
  assign S00_AXI_rdata[297] = \<const0> ;
  assign S00_AXI_rdata[296] = \<const0> ;
  assign S00_AXI_rdata[295] = \<const0> ;
  assign S00_AXI_rdata[294] = \<const0> ;
  assign S00_AXI_rdata[293] = \<const0> ;
  assign S00_AXI_rdata[292] = \<const0> ;
  assign S00_AXI_rdata[291] = \<const0> ;
  assign S00_AXI_rdata[290] = \<const0> ;
  assign S00_AXI_rdata[289] = \<const0> ;
  assign S00_AXI_rdata[288] = \<const0> ;
  assign S00_AXI_rdata[287] = \<const0> ;
  assign S00_AXI_rdata[286] = \<const0> ;
  assign S00_AXI_rdata[285] = \<const0> ;
  assign S00_AXI_rdata[284] = \<const0> ;
  assign S00_AXI_rdata[283] = \<const0> ;
  assign S00_AXI_rdata[282] = \<const0> ;
  assign S00_AXI_rdata[281] = \<const0> ;
  assign S00_AXI_rdata[280] = \<const0> ;
  assign S00_AXI_rdata[279] = \<const0> ;
  assign S00_AXI_rdata[278] = \<const0> ;
  assign S00_AXI_rdata[277] = \<const0> ;
  assign S00_AXI_rdata[276] = \<const0> ;
  assign S00_AXI_rdata[275] = \<const0> ;
  assign S00_AXI_rdata[274] = \<const0> ;
  assign S00_AXI_rdata[273] = \<const0> ;
  assign S00_AXI_rdata[272] = \<const0> ;
  assign S00_AXI_rdata[271] = \<const0> ;
  assign S00_AXI_rdata[270] = \<const0> ;
  assign S00_AXI_rdata[269] = \<const0> ;
  assign S00_AXI_rdata[268] = \<const0> ;
  assign S00_AXI_rdata[267] = \<const0> ;
  assign S00_AXI_rdata[266] = \<const0> ;
  assign S00_AXI_rdata[265] = \<const0> ;
  assign S00_AXI_rdata[264] = \<const0> ;
  assign S00_AXI_rdata[263] = \<const0> ;
  assign S00_AXI_rdata[262] = \<const0> ;
  assign S00_AXI_rdata[261] = \<const0> ;
  assign S00_AXI_rdata[260] = \<const0> ;
  assign S00_AXI_rdata[259] = \<const0> ;
  assign S00_AXI_rdata[258] = \<const0> ;
  assign S00_AXI_rdata[257] = \<const0> ;
  assign S00_AXI_rdata[256] = \<const0> ;
  assign S00_AXI_rdata[255] = \<const0> ;
  assign S00_AXI_rdata[254] = \<const0> ;
  assign S00_AXI_rdata[253] = \<const0> ;
  assign S00_AXI_rdata[252] = \<const0> ;
  assign S00_AXI_rdata[251] = \<const0> ;
  assign S00_AXI_rdata[250] = \<const0> ;
  assign S00_AXI_rdata[249] = \<const0> ;
  assign S00_AXI_rdata[248] = \<const0> ;
  assign S00_AXI_rdata[247] = \<const0> ;
  assign S00_AXI_rdata[246] = \<const0> ;
  assign S00_AXI_rdata[245] = \<const0> ;
  assign S00_AXI_rdata[244] = \<const0> ;
  assign S00_AXI_rdata[243] = \<const0> ;
  assign S00_AXI_rdata[242] = \<const0> ;
  assign S00_AXI_rdata[241] = \<const0> ;
  assign S00_AXI_rdata[240] = \<const0> ;
  assign S00_AXI_rdata[239] = \<const0> ;
  assign S00_AXI_rdata[238] = \<const0> ;
  assign S00_AXI_rdata[237] = \<const0> ;
  assign S00_AXI_rdata[236] = \<const0> ;
  assign S00_AXI_rdata[235] = \<const0> ;
  assign S00_AXI_rdata[234] = \<const0> ;
  assign S00_AXI_rdata[233] = \<const0> ;
  assign S00_AXI_rdata[232] = \<const0> ;
  assign S00_AXI_rdata[231] = \<const0> ;
  assign S00_AXI_rdata[230] = \<const0> ;
  assign S00_AXI_rdata[229] = \<const0> ;
  assign S00_AXI_rdata[228] = \<const0> ;
  assign S00_AXI_rdata[227] = \<const0> ;
  assign S00_AXI_rdata[226] = \<const0> ;
  assign S00_AXI_rdata[225] = \<const0> ;
  assign S00_AXI_rdata[224] = \<const0> ;
  assign S00_AXI_rdata[223] = \<const0> ;
  assign S00_AXI_rdata[222] = \<const0> ;
  assign S00_AXI_rdata[221] = \<const0> ;
  assign S00_AXI_rdata[220] = \<const0> ;
  assign S00_AXI_rdata[219] = \<const0> ;
  assign S00_AXI_rdata[218] = \<const0> ;
  assign S00_AXI_rdata[217] = \<const0> ;
  assign S00_AXI_rdata[216] = \<const0> ;
  assign S00_AXI_rdata[215] = \<const0> ;
  assign S00_AXI_rdata[214] = \<const0> ;
  assign S00_AXI_rdata[213] = \<const0> ;
  assign S00_AXI_rdata[212] = \<const0> ;
  assign S00_AXI_rdata[211] = \<const0> ;
  assign S00_AXI_rdata[210] = \<const0> ;
  assign S00_AXI_rdata[209] = \<const0> ;
  assign S00_AXI_rdata[208] = \<const0> ;
  assign S00_AXI_rdata[207] = \<const0> ;
  assign S00_AXI_rdata[206] = \<const0> ;
  assign S00_AXI_rdata[205] = \<const0> ;
  assign S00_AXI_rdata[204] = \<const0> ;
  assign S00_AXI_rdata[203] = \<const0> ;
  assign S00_AXI_rdata[202] = \<const0> ;
  assign S00_AXI_rdata[201] = \<const0> ;
  assign S00_AXI_rdata[200] = \<const0> ;
  assign S00_AXI_rdata[199] = \<const0> ;
  assign S00_AXI_rdata[198] = \<const0> ;
  assign S00_AXI_rdata[197] = \<const0> ;
  assign S00_AXI_rdata[196] = \<const0> ;
  assign S00_AXI_rdata[195] = \<const0> ;
  assign S00_AXI_rdata[194] = \<const0> ;
  assign S00_AXI_rdata[193] = \<const0> ;
  assign S00_AXI_rdata[192] = \<const0> ;
  assign S00_AXI_rdata[191] = \<const0> ;
  assign S00_AXI_rdata[190] = \<const0> ;
  assign S00_AXI_rdata[189] = \<const0> ;
  assign S00_AXI_rdata[188] = \<const0> ;
  assign S00_AXI_rdata[187] = \<const0> ;
  assign S00_AXI_rdata[186] = \<const0> ;
  assign S00_AXI_rdata[185] = \<const0> ;
  assign S00_AXI_rdata[184] = \<const0> ;
  assign S00_AXI_rdata[183] = \<const0> ;
  assign S00_AXI_rdata[182] = \<const0> ;
  assign S00_AXI_rdata[181] = \<const0> ;
  assign S00_AXI_rdata[180] = \<const0> ;
  assign S00_AXI_rdata[179] = \<const0> ;
  assign S00_AXI_rdata[178] = \<const0> ;
  assign S00_AXI_rdata[177] = \<const0> ;
  assign S00_AXI_rdata[176] = \<const0> ;
  assign S00_AXI_rdata[175] = \<const0> ;
  assign S00_AXI_rdata[174] = \<const0> ;
  assign S00_AXI_rdata[173] = \<const0> ;
  assign S00_AXI_rdata[172] = \<const0> ;
  assign S00_AXI_rdata[171] = \<const0> ;
  assign S00_AXI_rdata[170] = \<const0> ;
  assign S00_AXI_rdata[169] = \<const0> ;
  assign S00_AXI_rdata[168] = \<const0> ;
  assign S00_AXI_rdata[167] = \<const0> ;
  assign S00_AXI_rdata[166] = \<const0> ;
  assign S00_AXI_rdata[165] = \<const0> ;
  assign S00_AXI_rdata[164] = \<const0> ;
  assign S00_AXI_rdata[163] = \<const0> ;
  assign S00_AXI_rdata[162] = \<const0> ;
  assign S00_AXI_rdata[161] = \<const0> ;
  assign S00_AXI_rdata[160] = \<const0> ;
  assign S00_AXI_rdata[159] = \<const0> ;
  assign S00_AXI_rdata[158] = \<const0> ;
  assign S00_AXI_rdata[157] = \<const0> ;
  assign S00_AXI_rdata[156] = \<const0> ;
  assign S00_AXI_rdata[155] = \<const0> ;
  assign S00_AXI_rdata[154] = \<const0> ;
  assign S00_AXI_rdata[153] = \<const0> ;
  assign S00_AXI_rdata[152] = \<const0> ;
  assign S00_AXI_rdata[151] = \<const0> ;
  assign S00_AXI_rdata[150] = \<const0> ;
  assign S00_AXI_rdata[149] = \<const0> ;
  assign S00_AXI_rdata[148] = \<const0> ;
  assign S00_AXI_rdata[147] = \<const0> ;
  assign S00_AXI_rdata[146] = \<const0> ;
  assign S00_AXI_rdata[145] = \<const0> ;
  assign S00_AXI_rdata[144] = \<const0> ;
  assign S00_AXI_rdata[143] = \<const0> ;
  assign S00_AXI_rdata[142] = \<const0> ;
  assign S00_AXI_rdata[141] = \<const0> ;
  assign S00_AXI_rdata[140] = \<const0> ;
  assign S00_AXI_rdata[139] = \<const0> ;
  assign S00_AXI_rdata[138] = \<const0> ;
  assign S00_AXI_rdata[137] = \<const0> ;
  assign S00_AXI_rdata[136] = \<const0> ;
  assign S00_AXI_rdata[135] = \<const0> ;
  assign S00_AXI_rdata[134] = \<const0> ;
  assign S00_AXI_rdata[133] = \<const0> ;
  assign S00_AXI_rdata[132] = \<const0> ;
  assign S00_AXI_rdata[131] = \<const0> ;
  assign S00_AXI_rdata[130] = \<const0> ;
  assign S00_AXI_rdata[129] = \<const0> ;
  assign S00_AXI_rdata[128] = \<const0> ;
  assign S00_AXI_rdata[127] = \<const0> ;
  assign S00_AXI_rdata[126] = \<const0> ;
  assign S00_AXI_rdata[125] = \<const0> ;
  assign S00_AXI_rdata[124] = \<const0> ;
  assign S00_AXI_rdata[123] = \<const0> ;
  assign S00_AXI_rdata[122] = \<const0> ;
  assign S00_AXI_rdata[121] = \<const0> ;
  assign S00_AXI_rdata[120] = \<const0> ;
  assign S00_AXI_rdata[119] = \<const0> ;
  assign S00_AXI_rdata[118] = \<const0> ;
  assign S00_AXI_rdata[117] = \<const0> ;
  assign S00_AXI_rdata[116] = \<const0> ;
  assign S00_AXI_rdata[115] = \<const0> ;
  assign S00_AXI_rdata[114] = \<const0> ;
  assign S00_AXI_rdata[113] = \<const0> ;
  assign S00_AXI_rdata[112] = \<const0> ;
  assign S00_AXI_rdata[111] = \<const0> ;
  assign S00_AXI_rdata[110] = \<const0> ;
  assign S00_AXI_rdata[109] = \<const0> ;
  assign S00_AXI_rdata[108] = \<const0> ;
  assign S00_AXI_rdata[107] = \<const0> ;
  assign S00_AXI_rdata[106] = \<const0> ;
  assign S00_AXI_rdata[105] = \<const0> ;
  assign S00_AXI_rdata[104] = \<const0> ;
  assign S00_AXI_rdata[103] = \<const0> ;
  assign S00_AXI_rdata[102] = \<const0> ;
  assign S00_AXI_rdata[101] = \<const0> ;
  assign S00_AXI_rdata[100] = \<const0> ;
  assign S00_AXI_rdata[99] = \<const0> ;
  assign S00_AXI_rdata[98] = \<const0> ;
  assign S00_AXI_rdata[97] = \<const0> ;
  assign S00_AXI_rdata[96] = \<const0> ;
  assign S00_AXI_rdata[95] = \<const0> ;
  assign S00_AXI_rdata[94] = \<const0> ;
  assign S00_AXI_rdata[93] = \<const0> ;
  assign S00_AXI_rdata[92] = \<const0> ;
  assign S00_AXI_rdata[91] = \<const0> ;
  assign S00_AXI_rdata[90] = \<const0> ;
  assign S00_AXI_rdata[89] = \<const0> ;
  assign S00_AXI_rdata[88] = \<const0> ;
  assign S00_AXI_rdata[87] = \<const0> ;
  assign S00_AXI_rdata[86] = \<const0> ;
  assign S00_AXI_rdata[85] = \<const0> ;
  assign S00_AXI_rdata[84] = \<const0> ;
  assign S00_AXI_rdata[83] = \<const0> ;
  assign S00_AXI_rdata[82] = \<const0> ;
  assign S00_AXI_rdata[81] = \<const0> ;
  assign S00_AXI_rdata[80] = \<const0> ;
  assign S00_AXI_rdata[79] = \<const0> ;
  assign S00_AXI_rdata[78] = \<const0> ;
  assign S00_AXI_rdata[77] = \<const0> ;
  assign S00_AXI_rdata[76] = \<const0> ;
  assign S00_AXI_rdata[75] = \<const0> ;
  assign S00_AXI_rdata[74] = \<const0> ;
  assign S00_AXI_rdata[73] = \<const0> ;
  assign S00_AXI_rdata[72] = \<const0> ;
  assign S00_AXI_rdata[71] = \<const0> ;
  assign S00_AXI_rdata[70] = \<const0> ;
  assign S00_AXI_rdata[69] = \<const0> ;
  assign S00_AXI_rdata[68] = \<const0> ;
  assign S00_AXI_rdata[67] = \<const0> ;
  assign S00_AXI_rdata[66] = \<const0> ;
  assign S00_AXI_rdata[65] = \<const0> ;
  assign S00_AXI_rdata[64] = \<const0> ;
  assign S00_AXI_rdata[63] = \<const0> ;
  assign S00_AXI_rdata[62] = \<const0> ;
  assign S00_AXI_rdata[61] = \<const0> ;
  assign S00_AXI_rdata[60] = \<const0> ;
  assign S00_AXI_rdata[59] = \<const0> ;
  assign S00_AXI_rdata[58] = \<const0> ;
  assign S00_AXI_rdata[57] = \<const0> ;
  assign S00_AXI_rdata[56] = \<const0> ;
  assign S00_AXI_rdata[55] = \<const0> ;
  assign S00_AXI_rdata[54] = \<const0> ;
  assign S00_AXI_rdata[53] = \<const0> ;
  assign S00_AXI_rdata[52] = \<const0> ;
  assign S00_AXI_rdata[51] = \<const0> ;
  assign S00_AXI_rdata[50] = \<const0> ;
  assign S00_AXI_rdata[49] = \<const0> ;
  assign S00_AXI_rdata[48] = \<const0> ;
  assign S00_AXI_rdata[47] = \<const0> ;
  assign S00_AXI_rdata[46] = \<const0> ;
  assign S00_AXI_rdata[45] = \<const0> ;
  assign S00_AXI_rdata[44] = \<const0> ;
  assign S00_AXI_rdata[43] = \<const0> ;
  assign S00_AXI_rdata[42] = \<const0> ;
  assign S00_AXI_rdata[41] = \<const0> ;
  assign S00_AXI_rdata[40] = \<const0> ;
  assign S00_AXI_rdata[39] = \<const0> ;
  assign S00_AXI_rdata[38] = \<const0> ;
  assign S00_AXI_rdata[37] = \<const0> ;
  assign S00_AXI_rdata[36] = \<const0> ;
  assign S00_AXI_rdata[35] = \<const0> ;
  assign S00_AXI_rdata[34] = \<const0> ;
  assign S00_AXI_rdata[33] = \<const0> ;
  assign S00_AXI_rdata[32] = \<const0> ;
  assign S00_AXI_rdata[31] = \<const0> ;
  assign S00_AXI_rdata[30] = \<const0> ;
  assign S00_AXI_rdata[29] = \<const0> ;
  assign S00_AXI_rdata[28] = \<const0> ;
  assign S00_AXI_rdata[27] = \<const0> ;
  assign S00_AXI_rdata[26] = \<const0> ;
  assign S00_AXI_rdata[25] = \<const0> ;
  assign S00_AXI_rdata[24] = \<const0> ;
  assign S00_AXI_rdata[23] = \<const0> ;
  assign S00_AXI_rdata[22] = \<const0> ;
  assign S00_AXI_rdata[21] = \<const0> ;
  assign S00_AXI_rdata[20] = \<const0> ;
  assign S00_AXI_rdata[19] = \<const0> ;
  assign S00_AXI_rdata[18] = \<const0> ;
  assign S00_AXI_rdata[17] = \<const0> ;
  assign S00_AXI_rdata[16] = \<const0> ;
  assign S00_AXI_rdata[15] = \<const0> ;
  assign S00_AXI_rdata[14] = \<const0> ;
  assign S00_AXI_rdata[13] = \<const0> ;
  assign S00_AXI_rdata[12] = \<const0> ;
  assign S00_AXI_rdata[11] = \<const0> ;
  assign S00_AXI_rdata[10] = \<const0> ;
  assign S00_AXI_rdata[9] = \<const0> ;
  assign S00_AXI_rdata[8] = \<const0> ;
  assign S00_AXI_rdata[7] = \<const0> ;
  assign S00_AXI_rdata[6] = \<const0> ;
  assign S00_AXI_rdata[5] = \<const0> ;
  assign S00_AXI_rdata[4] = \<const0> ;
  assign S00_AXI_rdata[3] = \<const0> ;
  assign S00_AXI_rdata[2] = \<const0> ;
  assign S00_AXI_rdata[1] = \<const0> ;
  assign S00_AXI_rdata[0] = \<const0> ;
  assign S00_AXI_rid[3] = \<const0> ;
  assign S00_AXI_rid[2] = \<const0> ;
  assign S00_AXI_rid[1] = \<const0> ;
  assign S00_AXI_rid[0] = \<const0> ;
  assign S00_AXI_rlast = \<const0> ;
  assign S00_AXI_rresp[1] = \<const0> ;
  assign S00_AXI_rresp[0] = \<const0> ;
  assign S00_AXI_rvalid = \<const0> ;
  assign S00_AXI_wready = \<const0> ;
  assign S03_AXI_arready = \<const0> ;
  assign S03_AXI_awready = \<const0> ;
  assign S03_AXI_bid[3] = \<const0> ;
  assign S03_AXI_bid[2] = \<const0> ;
  assign S03_AXI_bid[1] = \<const0> ;
  assign S03_AXI_bid[0] = \<const0> ;
  assign S03_AXI_bresp[1] = \<const0> ;
  assign S03_AXI_bresp[0] = \<const0> ;
  assign S03_AXI_bvalid = \<const0> ;
  assign S03_AXI_rdata[511] = \<const0> ;
  assign S03_AXI_rdata[510] = \<const0> ;
  assign S03_AXI_rdata[509] = \<const0> ;
  assign S03_AXI_rdata[508] = \<const0> ;
  assign S03_AXI_rdata[507] = \<const0> ;
  assign S03_AXI_rdata[506] = \<const0> ;
  assign S03_AXI_rdata[505] = \<const0> ;
  assign S03_AXI_rdata[504] = \<const0> ;
  assign S03_AXI_rdata[503] = \<const0> ;
  assign S03_AXI_rdata[502] = \<const0> ;
  assign S03_AXI_rdata[501] = \<const0> ;
  assign S03_AXI_rdata[500] = \<const0> ;
  assign S03_AXI_rdata[499] = \<const0> ;
  assign S03_AXI_rdata[498] = \<const0> ;
  assign S03_AXI_rdata[497] = \<const0> ;
  assign S03_AXI_rdata[496] = \<const0> ;
  assign S03_AXI_rdata[495] = \<const0> ;
  assign S03_AXI_rdata[494] = \<const0> ;
  assign S03_AXI_rdata[493] = \<const0> ;
  assign S03_AXI_rdata[492] = \<const0> ;
  assign S03_AXI_rdata[491] = \<const0> ;
  assign S03_AXI_rdata[490] = \<const0> ;
  assign S03_AXI_rdata[489] = \<const0> ;
  assign S03_AXI_rdata[488] = \<const0> ;
  assign S03_AXI_rdata[487] = \<const0> ;
  assign S03_AXI_rdata[486] = \<const0> ;
  assign S03_AXI_rdata[485] = \<const0> ;
  assign S03_AXI_rdata[484] = \<const0> ;
  assign S03_AXI_rdata[483] = \<const0> ;
  assign S03_AXI_rdata[482] = \<const0> ;
  assign S03_AXI_rdata[481] = \<const0> ;
  assign S03_AXI_rdata[480] = \<const0> ;
  assign S03_AXI_rdata[479] = \<const0> ;
  assign S03_AXI_rdata[478] = \<const0> ;
  assign S03_AXI_rdata[477] = \<const0> ;
  assign S03_AXI_rdata[476] = \<const0> ;
  assign S03_AXI_rdata[475] = \<const0> ;
  assign S03_AXI_rdata[474] = \<const0> ;
  assign S03_AXI_rdata[473] = \<const0> ;
  assign S03_AXI_rdata[472] = \<const0> ;
  assign S03_AXI_rdata[471] = \<const0> ;
  assign S03_AXI_rdata[470] = \<const0> ;
  assign S03_AXI_rdata[469] = \<const0> ;
  assign S03_AXI_rdata[468] = \<const0> ;
  assign S03_AXI_rdata[467] = \<const0> ;
  assign S03_AXI_rdata[466] = \<const0> ;
  assign S03_AXI_rdata[465] = \<const0> ;
  assign S03_AXI_rdata[464] = \<const0> ;
  assign S03_AXI_rdata[463] = \<const0> ;
  assign S03_AXI_rdata[462] = \<const0> ;
  assign S03_AXI_rdata[461] = \<const0> ;
  assign S03_AXI_rdata[460] = \<const0> ;
  assign S03_AXI_rdata[459] = \<const0> ;
  assign S03_AXI_rdata[458] = \<const0> ;
  assign S03_AXI_rdata[457] = \<const0> ;
  assign S03_AXI_rdata[456] = \<const0> ;
  assign S03_AXI_rdata[455] = \<const0> ;
  assign S03_AXI_rdata[454] = \<const0> ;
  assign S03_AXI_rdata[453] = \<const0> ;
  assign S03_AXI_rdata[452] = \<const0> ;
  assign S03_AXI_rdata[451] = \<const0> ;
  assign S03_AXI_rdata[450] = \<const0> ;
  assign S03_AXI_rdata[449] = \<const0> ;
  assign S03_AXI_rdata[448] = \<const0> ;
  assign S03_AXI_rdata[447] = \<const0> ;
  assign S03_AXI_rdata[446] = \<const0> ;
  assign S03_AXI_rdata[445] = \<const0> ;
  assign S03_AXI_rdata[444] = \<const0> ;
  assign S03_AXI_rdata[443] = \<const0> ;
  assign S03_AXI_rdata[442] = \<const0> ;
  assign S03_AXI_rdata[441] = \<const0> ;
  assign S03_AXI_rdata[440] = \<const0> ;
  assign S03_AXI_rdata[439] = \<const0> ;
  assign S03_AXI_rdata[438] = \<const0> ;
  assign S03_AXI_rdata[437] = \<const0> ;
  assign S03_AXI_rdata[436] = \<const0> ;
  assign S03_AXI_rdata[435] = \<const0> ;
  assign S03_AXI_rdata[434] = \<const0> ;
  assign S03_AXI_rdata[433] = \<const0> ;
  assign S03_AXI_rdata[432] = \<const0> ;
  assign S03_AXI_rdata[431] = \<const0> ;
  assign S03_AXI_rdata[430] = \<const0> ;
  assign S03_AXI_rdata[429] = \<const0> ;
  assign S03_AXI_rdata[428] = \<const0> ;
  assign S03_AXI_rdata[427] = \<const0> ;
  assign S03_AXI_rdata[426] = \<const0> ;
  assign S03_AXI_rdata[425] = \<const0> ;
  assign S03_AXI_rdata[424] = \<const0> ;
  assign S03_AXI_rdata[423] = \<const0> ;
  assign S03_AXI_rdata[422] = \<const0> ;
  assign S03_AXI_rdata[421] = \<const0> ;
  assign S03_AXI_rdata[420] = \<const0> ;
  assign S03_AXI_rdata[419] = \<const0> ;
  assign S03_AXI_rdata[418] = \<const0> ;
  assign S03_AXI_rdata[417] = \<const0> ;
  assign S03_AXI_rdata[416] = \<const0> ;
  assign S03_AXI_rdata[415] = \<const0> ;
  assign S03_AXI_rdata[414] = \<const0> ;
  assign S03_AXI_rdata[413] = \<const0> ;
  assign S03_AXI_rdata[412] = \<const0> ;
  assign S03_AXI_rdata[411] = \<const0> ;
  assign S03_AXI_rdata[410] = \<const0> ;
  assign S03_AXI_rdata[409] = \<const0> ;
  assign S03_AXI_rdata[408] = \<const0> ;
  assign S03_AXI_rdata[407] = \<const0> ;
  assign S03_AXI_rdata[406] = \<const0> ;
  assign S03_AXI_rdata[405] = \<const0> ;
  assign S03_AXI_rdata[404] = \<const0> ;
  assign S03_AXI_rdata[403] = \<const0> ;
  assign S03_AXI_rdata[402] = \<const0> ;
  assign S03_AXI_rdata[401] = \<const0> ;
  assign S03_AXI_rdata[400] = \<const0> ;
  assign S03_AXI_rdata[399] = \<const0> ;
  assign S03_AXI_rdata[398] = \<const0> ;
  assign S03_AXI_rdata[397] = \<const0> ;
  assign S03_AXI_rdata[396] = \<const0> ;
  assign S03_AXI_rdata[395] = \<const0> ;
  assign S03_AXI_rdata[394] = \<const0> ;
  assign S03_AXI_rdata[393] = \<const0> ;
  assign S03_AXI_rdata[392] = \<const0> ;
  assign S03_AXI_rdata[391] = \<const0> ;
  assign S03_AXI_rdata[390] = \<const0> ;
  assign S03_AXI_rdata[389] = \<const0> ;
  assign S03_AXI_rdata[388] = \<const0> ;
  assign S03_AXI_rdata[387] = \<const0> ;
  assign S03_AXI_rdata[386] = \<const0> ;
  assign S03_AXI_rdata[385] = \<const0> ;
  assign S03_AXI_rdata[384] = \<const0> ;
  assign S03_AXI_rdata[383] = \<const0> ;
  assign S03_AXI_rdata[382] = \<const0> ;
  assign S03_AXI_rdata[381] = \<const0> ;
  assign S03_AXI_rdata[380] = \<const0> ;
  assign S03_AXI_rdata[379] = \<const0> ;
  assign S03_AXI_rdata[378] = \<const0> ;
  assign S03_AXI_rdata[377] = \<const0> ;
  assign S03_AXI_rdata[376] = \<const0> ;
  assign S03_AXI_rdata[375] = \<const0> ;
  assign S03_AXI_rdata[374] = \<const0> ;
  assign S03_AXI_rdata[373] = \<const0> ;
  assign S03_AXI_rdata[372] = \<const0> ;
  assign S03_AXI_rdata[371] = \<const0> ;
  assign S03_AXI_rdata[370] = \<const0> ;
  assign S03_AXI_rdata[369] = \<const0> ;
  assign S03_AXI_rdata[368] = \<const0> ;
  assign S03_AXI_rdata[367] = \<const0> ;
  assign S03_AXI_rdata[366] = \<const0> ;
  assign S03_AXI_rdata[365] = \<const0> ;
  assign S03_AXI_rdata[364] = \<const0> ;
  assign S03_AXI_rdata[363] = \<const0> ;
  assign S03_AXI_rdata[362] = \<const0> ;
  assign S03_AXI_rdata[361] = \<const0> ;
  assign S03_AXI_rdata[360] = \<const0> ;
  assign S03_AXI_rdata[359] = \<const0> ;
  assign S03_AXI_rdata[358] = \<const0> ;
  assign S03_AXI_rdata[357] = \<const0> ;
  assign S03_AXI_rdata[356] = \<const0> ;
  assign S03_AXI_rdata[355] = \<const0> ;
  assign S03_AXI_rdata[354] = \<const0> ;
  assign S03_AXI_rdata[353] = \<const0> ;
  assign S03_AXI_rdata[352] = \<const0> ;
  assign S03_AXI_rdata[351] = \<const0> ;
  assign S03_AXI_rdata[350] = \<const0> ;
  assign S03_AXI_rdata[349] = \<const0> ;
  assign S03_AXI_rdata[348] = \<const0> ;
  assign S03_AXI_rdata[347] = \<const0> ;
  assign S03_AXI_rdata[346] = \<const0> ;
  assign S03_AXI_rdata[345] = \<const0> ;
  assign S03_AXI_rdata[344] = \<const0> ;
  assign S03_AXI_rdata[343] = \<const0> ;
  assign S03_AXI_rdata[342] = \<const0> ;
  assign S03_AXI_rdata[341] = \<const0> ;
  assign S03_AXI_rdata[340] = \<const0> ;
  assign S03_AXI_rdata[339] = \<const0> ;
  assign S03_AXI_rdata[338] = \<const0> ;
  assign S03_AXI_rdata[337] = \<const0> ;
  assign S03_AXI_rdata[336] = \<const0> ;
  assign S03_AXI_rdata[335] = \<const0> ;
  assign S03_AXI_rdata[334] = \<const0> ;
  assign S03_AXI_rdata[333] = \<const0> ;
  assign S03_AXI_rdata[332] = \<const0> ;
  assign S03_AXI_rdata[331] = \<const0> ;
  assign S03_AXI_rdata[330] = \<const0> ;
  assign S03_AXI_rdata[329] = \<const0> ;
  assign S03_AXI_rdata[328] = \<const0> ;
  assign S03_AXI_rdata[327] = \<const0> ;
  assign S03_AXI_rdata[326] = \<const0> ;
  assign S03_AXI_rdata[325] = \<const0> ;
  assign S03_AXI_rdata[324] = \<const0> ;
  assign S03_AXI_rdata[323] = \<const0> ;
  assign S03_AXI_rdata[322] = \<const0> ;
  assign S03_AXI_rdata[321] = \<const0> ;
  assign S03_AXI_rdata[320] = \<const0> ;
  assign S03_AXI_rdata[319] = \<const0> ;
  assign S03_AXI_rdata[318] = \<const0> ;
  assign S03_AXI_rdata[317] = \<const0> ;
  assign S03_AXI_rdata[316] = \<const0> ;
  assign S03_AXI_rdata[315] = \<const0> ;
  assign S03_AXI_rdata[314] = \<const0> ;
  assign S03_AXI_rdata[313] = \<const0> ;
  assign S03_AXI_rdata[312] = \<const0> ;
  assign S03_AXI_rdata[311] = \<const0> ;
  assign S03_AXI_rdata[310] = \<const0> ;
  assign S03_AXI_rdata[309] = \<const0> ;
  assign S03_AXI_rdata[308] = \<const0> ;
  assign S03_AXI_rdata[307] = \<const0> ;
  assign S03_AXI_rdata[306] = \<const0> ;
  assign S03_AXI_rdata[305] = \<const0> ;
  assign S03_AXI_rdata[304] = \<const0> ;
  assign S03_AXI_rdata[303] = \<const0> ;
  assign S03_AXI_rdata[302] = \<const0> ;
  assign S03_AXI_rdata[301] = \<const0> ;
  assign S03_AXI_rdata[300] = \<const0> ;
  assign S03_AXI_rdata[299] = \<const0> ;
  assign S03_AXI_rdata[298] = \<const0> ;
  assign S03_AXI_rdata[297] = \<const0> ;
  assign S03_AXI_rdata[296] = \<const0> ;
  assign S03_AXI_rdata[295] = \<const0> ;
  assign S03_AXI_rdata[294] = \<const0> ;
  assign S03_AXI_rdata[293] = \<const0> ;
  assign S03_AXI_rdata[292] = \<const0> ;
  assign S03_AXI_rdata[291] = \<const0> ;
  assign S03_AXI_rdata[290] = \<const0> ;
  assign S03_AXI_rdata[289] = \<const0> ;
  assign S03_AXI_rdata[288] = \<const0> ;
  assign S03_AXI_rdata[287] = \<const0> ;
  assign S03_AXI_rdata[286] = \<const0> ;
  assign S03_AXI_rdata[285] = \<const0> ;
  assign S03_AXI_rdata[284] = \<const0> ;
  assign S03_AXI_rdata[283] = \<const0> ;
  assign S03_AXI_rdata[282] = \<const0> ;
  assign S03_AXI_rdata[281] = \<const0> ;
  assign S03_AXI_rdata[280] = \<const0> ;
  assign S03_AXI_rdata[279] = \<const0> ;
  assign S03_AXI_rdata[278] = \<const0> ;
  assign S03_AXI_rdata[277] = \<const0> ;
  assign S03_AXI_rdata[276] = \<const0> ;
  assign S03_AXI_rdata[275] = \<const0> ;
  assign S03_AXI_rdata[274] = \<const0> ;
  assign S03_AXI_rdata[273] = \<const0> ;
  assign S03_AXI_rdata[272] = \<const0> ;
  assign S03_AXI_rdata[271] = \<const0> ;
  assign S03_AXI_rdata[270] = \<const0> ;
  assign S03_AXI_rdata[269] = \<const0> ;
  assign S03_AXI_rdata[268] = \<const0> ;
  assign S03_AXI_rdata[267] = \<const0> ;
  assign S03_AXI_rdata[266] = \<const0> ;
  assign S03_AXI_rdata[265] = \<const0> ;
  assign S03_AXI_rdata[264] = \<const0> ;
  assign S03_AXI_rdata[263] = \<const0> ;
  assign S03_AXI_rdata[262] = \<const0> ;
  assign S03_AXI_rdata[261] = \<const0> ;
  assign S03_AXI_rdata[260] = \<const0> ;
  assign S03_AXI_rdata[259] = \<const0> ;
  assign S03_AXI_rdata[258] = \<const0> ;
  assign S03_AXI_rdata[257] = \<const0> ;
  assign S03_AXI_rdata[256] = \<const0> ;
  assign S03_AXI_rdata[255] = \<const0> ;
  assign S03_AXI_rdata[254] = \<const0> ;
  assign S03_AXI_rdata[253] = \<const0> ;
  assign S03_AXI_rdata[252] = \<const0> ;
  assign S03_AXI_rdata[251] = \<const0> ;
  assign S03_AXI_rdata[250] = \<const0> ;
  assign S03_AXI_rdata[249] = \<const0> ;
  assign S03_AXI_rdata[248] = \<const0> ;
  assign S03_AXI_rdata[247] = \<const0> ;
  assign S03_AXI_rdata[246] = \<const0> ;
  assign S03_AXI_rdata[245] = \<const0> ;
  assign S03_AXI_rdata[244] = \<const0> ;
  assign S03_AXI_rdata[243] = \<const0> ;
  assign S03_AXI_rdata[242] = \<const0> ;
  assign S03_AXI_rdata[241] = \<const0> ;
  assign S03_AXI_rdata[240] = \<const0> ;
  assign S03_AXI_rdata[239] = \<const0> ;
  assign S03_AXI_rdata[238] = \<const0> ;
  assign S03_AXI_rdata[237] = \<const0> ;
  assign S03_AXI_rdata[236] = \<const0> ;
  assign S03_AXI_rdata[235] = \<const0> ;
  assign S03_AXI_rdata[234] = \<const0> ;
  assign S03_AXI_rdata[233] = \<const0> ;
  assign S03_AXI_rdata[232] = \<const0> ;
  assign S03_AXI_rdata[231] = \<const0> ;
  assign S03_AXI_rdata[230] = \<const0> ;
  assign S03_AXI_rdata[229] = \<const0> ;
  assign S03_AXI_rdata[228] = \<const0> ;
  assign S03_AXI_rdata[227] = \<const0> ;
  assign S03_AXI_rdata[226] = \<const0> ;
  assign S03_AXI_rdata[225] = \<const0> ;
  assign S03_AXI_rdata[224] = \<const0> ;
  assign S03_AXI_rdata[223] = \<const0> ;
  assign S03_AXI_rdata[222] = \<const0> ;
  assign S03_AXI_rdata[221] = \<const0> ;
  assign S03_AXI_rdata[220] = \<const0> ;
  assign S03_AXI_rdata[219] = \<const0> ;
  assign S03_AXI_rdata[218] = \<const0> ;
  assign S03_AXI_rdata[217] = \<const0> ;
  assign S03_AXI_rdata[216] = \<const0> ;
  assign S03_AXI_rdata[215] = \<const0> ;
  assign S03_AXI_rdata[214] = \<const0> ;
  assign S03_AXI_rdata[213] = \<const0> ;
  assign S03_AXI_rdata[212] = \<const0> ;
  assign S03_AXI_rdata[211] = \<const0> ;
  assign S03_AXI_rdata[210] = \<const0> ;
  assign S03_AXI_rdata[209] = \<const0> ;
  assign S03_AXI_rdata[208] = \<const0> ;
  assign S03_AXI_rdata[207] = \<const0> ;
  assign S03_AXI_rdata[206] = \<const0> ;
  assign S03_AXI_rdata[205] = \<const0> ;
  assign S03_AXI_rdata[204] = \<const0> ;
  assign S03_AXI_rdata[203] = \<const0> ;
  assign S03_AXI_rdata[202] = \<const0> ;
  assign S03_AXI_rdata[201] = \<const0> ;
  assign S03_AXI_rdata[200] = \<const0> ;
  assign S03_AXI_rdata[199] = \<const0> ;
  assign S03_AXI_rdata[198] = \<const0> ;
  assign S03_AXI_rdata[197] = \<const0> ;
  assign S03_AXI_rdata[196] = \<const0> ;
  assign S03_AXI_rdata[195] = \<const0> ;
  assign S03_AXI_rdata[194] = \<const0> ;
  assign S03_AXI_rdata[193] = \<const0> ;
  assign S03_AXI_rdata[192] = \<const0> ;
  assign S03_AXI_rdata[191] = \<const0> ;
  assign S03_AXI_rdata[190] = \<const0> ;
  assign S03_AXI_rdata[189] = \<const0> ;
  assign S03_AXI_rdata[188] = \<const0> ;
  assign S03_AXI_rdata[187] = \<const0> ;
  assign S03_AXI_rdata[186] = \<const0> ;
  assign S03_AXI_rdata[185] = \<const0> ;
  assign S03_AXI_rdata[184] = \<const0> ;
  assign S03_AXI_rdata[183] = \<const0> ;
  assign S03_AXI_rdata[182] = \<const0> ;
  assign S03_AXI_rdata[181] = \<const0> ;
  assign S03_AXI_rdata[180] = \<const0> ;
  assign S03_AXI_rdata[179] = \<const0> ;
  assign S03_AXI_rdata[178] = \<const0> ;
  assign S03_AXI_rdata[177] = \<const0> ;
  assign S03_AXI_rdata[176] = \<const0> ;
  assign S03_AXI_rdata[175] = \<const0> ;
  assign S03_AXI_rdata[174] = \<const0> ;
  assign S03_AXI_rdata[173] = \<const0> ;
  assign S03_AXI_rdata[172] = \<const0> ;
  assign S03_AXI_rdata[171] = \<const0> ;
  assign S03_AXI_rdata[170] = \<const0> ;
  assign S03_AXI_rdata[169] = \<const0> ;
  assign S03_AXI_rdata[168] = \<const0> ;
  assign S03_AXI_rdata[167] = \<const0> ;
  assign S03_AXI_rdata[166] = \<const0> ;
  assign S03_AXI_rdata[165] = \<const0> ;
  assign S03_AXI_rdata[164] = \<const0> ;
  assign S03_AXI_rdata[163] = \<const0> ;
  assign S03_AXI_rdata[162] = \<const0> ;
  assign S03_AXI_rdata[161] = \<const0> ;
  assign S03_AXI_rdata[160] = \<const0> ;
  assign S03_AXI_rdata[159] = \<const0> ;
  assign S03_AXI_rdata[158] = \<const0> ;
  assign S03_AXI_rdata[157] = \<const0> ;
  assign S03_AXI_rdata[156] = \<const0> ;
  assign S03_AXI_rdata[155] = \<const0> ;
  assign S03_AXI_rdata[154] = \<const0> ;
  assign S03_AXI_rdata[153] = \<const0> ;
  assign S03_AXI_rdata[152] = \<const0> ;
  assign S03_AXI_rdata[151] = \<const0> ;
  assign S03_AXI_rdata[150] = \<const0> ;
  assign S03_AXI_rdata[149] = \<const0> ;
  assign S03_AXI_rdata[148] = \<const0> ;
  assign S03_AXI_rdata[147] = \<const0> ;
  assign S03_AXI_rdata[146] = \<const0> ;
  assign S03_AXI_rdata[145] = \<const0> ;
  assign S03_AXI_rdata[144] = \<const0> ;
  assign S03_AXI_rdata[143] = \<const0> ;
  assign S03_AXI_rdata[142] = \<const0> ;
  assign S03_AXI_rdata[141] = \<const0> ;
  assign S03_AXI_rdata[140] = \<const0> ;
  assign S03_AXI_rdata[139] = \<const0> ;
  assign S03_AXI_rdata[138] = \<const0> ;
  assign S03_AXI_rdata[137] = \<const0> ;
  assign S03_AXI_rdata[136] = \<const0> ;
  assign S03_AXI_rdata[135] = \<const0> ;
  assign S03_AXI_rdata[134] = \<const0> ;
  assign S03_AXI_rdata[133] = \<const0> ;
  assign S03_AXI_rdata[132] = \<const0> ;
  assign S03_AXI_rdata[131] = \<const0> ;
  assign S03_AXI_rdata[130] = \<const0> ;
  assign S03_AXI_rdata[129] = \<const0> ;
  assign S03_AXI_rdata[128] = \<const0> ;
  assign S03_AXI_rdata[127] = \<const0> ;
  assign S03_AXI_rdata[126] = \<const0> ;
  assign S03_AXI_rdata[125] = \<const0> ;
  assign S03_AXI_rdata[124] = \<const0> ;
  assign S03_AXI_rdata[123] = \<const0> ;
  assign S03_AXI_rdata[122] = \<const0> ;
  assign S03_AXI_rdata[121] = \<const0> ;
  assign S03_AXI_rdata[120] = \<const0> ;
  assign S03_AXI_rdata[119] = \<const0> ;
  assign S03_AXI_rdata[118] = \<const0> ;
  assign S03_AXI_rdata[117] = \<const0> ;
  assign S03_AXI_rdata[116] = \<const0> ;
  assign S03_AXI_rdata[115] = \<const0> ;
  assign S03_AXI_rdata[114] = \<const0> ;
  assign S03_AXI_rdata[113] = \<const0> ;
  assign S03_AXI_rdata[112] = \<const0> ;
  assign S03_AXI_rdata[111] = \<const0> ;
  assign S03_AXI_rdata[110] = \<const0> ;
  assign S03_AXI_rdata[109] = \<const0> ;
  assign S03_AXI_rdata[108] = \<const0> ;
  assign S03_AXI_rdata[107] = \<const0> ;
  assign S03_AXI_rdata[106] = \<const0> ;
  assign S03_AXI_rdata[105] = \<const0> ;
  assign S03_AXI_rdata[104] = \<const0> ;
  assign S03_AXI_rdata[103] = \<const0> ;
  assign S03_AXI_rdata[102] = \<const0> ;
  assign S03_AXI_rdata[101] = \<const0> ;
  assign S03_AXI_rdata[100] = \<const0> ;
  assign S03_AXI_rdata[99] = \<const0> ;
  assign S03_AXI_rdata[98] = \<const0> ;
  assign S03_AXI_rdata[97] = \<const0> ;
  assign S03_AXI_rdata[96] = \<const0> ;
  assign S03_AXI_rdata[95] = \<const0> ;
  assign S03_AXI_rdata[94] = \<const0> ;
  assign S03_AXI_rdata[93] = \<const0> ;
  assign S03_AXI_rdata[92] = \<const0> ;
  assign S03_AXI_rdata[91] = \<const0> ;
  assign S03_AXI_rdata[90] = \<const0> ;
  assign S03_AXI_rdata[89] = \<const0> ;
  assign S03_AXI_rdata[88] = \<const0> ;
  assign S03_AXI_rdata[87] = \<const0> ;
  assign S03_AXI_rdata[86] = \<const0> ;
  assign S03_AXI_rdata[85] = \<const0> ;
  assign S03_AXI_rdata[84] = \<const0> ;
  assign S03_AXI_rdata[83] = \<const0> ;
  assign S03_AXI_rdata[82] = \<const0> ;
  assign S03_AXI_rdata[81] = \<const0> ;
  assign S03_AXI_rdata[80] = \<const0> ;
  assign S03_AXI_rdata[79] = \<const0> ;
  assign S03_AXI_rdata[78] = \<const0> ;
  assign S03_AXI_rdata[77] = \<const0> ;
  assign S03_AXI_rdata[76] = \<const0> ;
  assign S03_AXI_rdata[75] = \<const0> ;
  assign S03_AXI_rdata[74] = \<const0> ;
  assign S03_AXI_rdata[73] = \<const0> ;
  assign S03_AXI_rdata[72] = \<const0> ;
  assign S03_AXI_rdata[71] = \<const0> ;
  assign S03_AXI_rdata[70] = \<const0> ;
  assign S03_AXI_rdata[69] = \<const0> ;
  assign S03_AXI_rdata[68] = \<const0> ;
  assign S03_AXI_rdata[67] = \<const0> ;
  assign S03_AXI_rdata[66] = \<const0> ;
  assign S03_AXI_rdata[65] = \<const0> ;
  assign S03_AXI_rdata[64] = \<const0> ;
  assign S03_AXI_rdata[63] = \<const0> ;
  assign S03_AXI_rdata[62] = \<const0> ;
  assign S03_AXI_rdata[61] = \<const0> ;
  assign S03_AXI_rdata[60] = \<const0> ;
  assign S03_AXI_rdata[59] = \<const0> ;
  assign S03_AXI_rdata[58] = \<const0> ;
  assign S03_AXI_rdata[57] = \<const0> ;
  assign S03_AXI_rdata[56] = \<const0> ;
  assign S03_AXI_rdata[55] = \<const0> ;
  assign S03_AXI_rdata[54] = \<const0> ;
  assign S03_AXI_rdata[53] = \<const0> ;
  assign S03_AXI_rdata[52] = \<const0> ;
  assign S03_AXI_rdata[51] = \<const0> ;
  assign S03_AXI_rdata[50] = \<const0> ;
  assign S03_AXI_rdata[49] = \<const0> ;
  assign S03_AXI_rdata[48] = \<const0> ;
  assign S03_AXI_rdata[47] = \<const0> ;
  assign S03_AXI_rdata[46] = \<const0> ;
  assign S03_AXI_rdata[45] = \<const0> ;
  assign S03_AXI_rdata[44] = \<const0> ;
  assign S03_AXI_rdata[43] = \<const0> ;
  assign S03_AXI_rdata[42] = \<const0> ;
  assign S03_AXI_rdata[41] = \<const0> ;
  assign S03_AXI_rdata[40] = \<const0> ;
  assign S03_AXI_rdata[39] = \<const0> ;
  assign S03_AXI_rdata[38] = \<const0> ;
  assign S03_AXI_rdata[37] = \<const0> ;
  assign S03_AXI_rdata[36] = \<const0> ;
  assign S03_AXI_rdata[35] = \<const0> ;
  assign S03_AXI_rdata[34] = \<const0> ;
  assign S03_AXI_rdata[33] = \<const0> ;
  assign S03_AXI_rdata[32] = \<const0> ;
  assign S03_AXI_rdata[31] = \<const0> ;
  assign S03_AXI_rdata[30] = \<const0> ;
  assign S03_AXI_rdata[29] = \<const0> ;
  assign S03_AXI_rdata[28] = \<const0> ;
  assign S03_AXI_rdata[27] = \<const0> ;
  assign S03_AXI_rdata[26] = \<const0> ;
  assign S03_AXI_rdata[25] = \<const0> ;
  assign S03_AXI_rdata[24] = \<const0> ;
  assign S03_AXI_rdata[23] = \<const0> ;
  assign S03_AXI_rdata[22] = \<const0> ;
  assign S03_AXI_rdata[21] = \<const0> ;
  assign S03_AXI_rdata[20] = \<const0> ;
  assign S03_AXI_rdata[19] = \<const0> ;
  assign S03_AXI_rdata[18] = \<const0> ;
  assign S03_AXI_rdata[17] = \<const0> ;
  assign S03_AXI_rdata[16] = \<const0> ;
  assign S03_AXI_rdata[15] = \<const0> ;
  assign S03_AXI_rdata[14] = \<const0> ;
  assign S03_AXI_rdata[13] = \<const0> ;
  assign S03_AXI_rdata[12] = \<const0> ;
  assign S03_AXI_rdata[11] = \<const0> ;
  assign S03_AXI_rdata[10] = \<const0> ;
  assign S03_AXI_rdata[9] = \<const0> ;
  assign S03_AXI_rdata[8] = \<const0> ;
  assign S03_AXI_rdata[7] = \<const0> ;
  assign S03_AXI_rdata[6] = \<const0> ;
  assign S03_AXI_rdata[5] = \<const0> ;
  assign S03_AXI_rdata[4] = \<const0> ;
  assign S03_AXI_rdata[3] = \<const0> ;
  assign S03_AXI_rdata[2] = \<const0> ;
  assign S03_AXI_rdata[1] = \<const0> ;
  assign S03_AXI_rdata[0] = \<const0> ;
  assign S03_AXI_rid[3] = \<const0> ;
  assign S03_AXI_rid[2] = \<const0> ;
  assign S03_AXI_rid[1] = \<const0> ;
  assign S03_AXI_rid[0] = \<const0> ;
  assign S03_AXI_rlast = \<const0> ;
  assign S03_AXI_rresp[1] = \<const0> ;
  assign S03_AXI_rresp[0] = \<const0> ;
  assign S03_AXI_rvalid = \<const0> ;
  assign S03_AXI_wready = \<const0> ;
  assign ddr4_mem00_ui_clk = \<const0> ;
  assign ddr4_mem02_ui_clk = \<const0> ;
  GND GND
       (.G(\<const0> ));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_interconnect_imp_6HQKUY interconnect
       (.M00_AXI_araddr(interconnect_DDR4_MEM01_M00_AXI_ARADDR),
        .M00_AXI_arburst(interconnect_DDR4_MEM01_M00_AXI_ARBURST),
        .M00_AXI_arcache(interconnect_DDR4_MEM01_M00_AXI_ARCACHE),
        .M00_AXI_arlen(interconnect_DDR4_MEM01_M00_AXI_ARLEN),
        .M00_AXI_arlock(interconnect_DDR4_MEM01_M00_AXI_ARLOCK),
        .M00_AXI_arprot(interconnect_DDR4_MEM01_M00_AXI_ARPROT),
        .M00_AXI_arqos(interconnect_DDR4_MEM01_M00_AXI_ARQOS),
        .M00_AXI_arready(M00_AXI_arready),
        .M00_AXI_arregion(M00_AXI_arregion),
        .M00_AXI_arsize(M00_AXI_arsize),
        .M00_AXI_arvalid(interconnect_DDR4_MEM01_M00_AXI_ARVALID),
        .M00_AXI_awaddr(interconnect_DDR4_MEM01_M00_AXI_AWADDR),
        .M00_AXI_awburst(interconnect_DDR4_MEM01_M00_AXI_AWBURST),
        .M00_AXI_awcache(interconnect_DDR4_MEM01_M00_AXI_AWCACHE),
        .M00_AXI_awlen(interconnect_DDR4_MEM01_M00_AXI_AWLEN),
        .M00_AXI_awlock(interconnect_DDR4_MEM01_M00_AXI_AWLOCK),
        .M00_AXI_awprot(interconnect_DDR4_MEM01_M00_AXI_AWPROT),
        .M00_AXI_awqos(interconnect_DDR4_MEM01_M00_AXI_AWQOS),
        .M00_AXI_awready(M00_AXI_awready),
        .M00_AXI_awregion(M00_AXI_awregion),
        .M00_AXI_awsize(M00_AXI_awsize),
        .M00_AXI_awvalid(interconnect_DDR4_MEM01_M00_AXI_AWVALID),
        .M00_AXI_bready(interconnect_DDR4_MEM01_M00_AXI_BREADY),
        .M00_AXI_bresp(M00_AXI_bresp),
        .M00_AXI_bvalid(M00_AXI_bvalid),
        .M00_AXI_rdata(M00_AXI_rdata),
        .M00_AXI_rlast(M00_AXI_rlast),
        .M00_AXI_rready(interconnect_DDR4_MEM01_M00_AXI_RREADY),
        .M00_AXI_rresp(M00_AXI_rresp),
        .M00_AXI_rvalid(M00_AXI_rvalid),
        .M00_AXI_wdata(interconnect_DDR4_MEM01_M00_AXI_WDATA),
        .M00_AXI_wlast(interconnect_DDR4_MEM01_M00_AXI_WLAST),
        .M00_AXI_wready(M00_AXI_wready),
        .M00_AXI_wstrb(interconnect_DDR4_MEM01_M00_AXI_WSTRB),
        .M00_AXI_wvalid(interconnect_DDR4_MEM01_M00_AXI_WVALID),
        .S01_AXI_araddr(S01_AXI_araddr),
        .S01_AXI_arburst(S01_AXI_arburst),
        .S01_AXI_arcache(S01_AXI_arcache),
        .S01_AXI_arid(S01_AXI_arid),
        .S01_AXI_arlen(S01_AXI_arlen),
        .S01_AXI_arlock(S01_AXI_arlock),
        .S01_AXI_arprot(S01_AXI_arprot),
        .S01_AXI_arqos(S01_AXI_arqos),
        .S01_AXI_arready(S01_AXI_arready),
        .S01_AXI_arregion(S01_AXI_arregion),
        .S01_AXI_arvalid(S01_AXI_arvalid),
        .S01_AXI_awaddr(S01_AXI_awaddr),
        .S01_AXI_awburst(S01_AXI_awburst),
        .S01_AXI_awcache(S01_AXI_awcache),
        .S01_AXI_awid(S01_AXI_awid),
        .S01_AXI_awlen(S01_AXI_awlen),
        .S01_AXI_awlock(S01_AXI_awlock),
        .S01_AXI_awprot(S01_AXI_awprot),
        .S01_AXI_awqos(S01_AXI_awqos),
        .S01_AXI_awready(S01_AXI_awready),
        .S01_AXI_awregion(S01_AXI_awregion),
        .S01_AXI_awvalid(S01_AXI_awvalid),
        .S01_AXI_bid(S01_AXI_bid),
        .S01_AXI_bready(S01_AXI_bready),
        .S01_AXI_bresp(S01_AXI_bresp),
        .S01_AXI_bvalid(S01_AXI_bvalid),
        .S01_AXI_rdata(S01_AXI_rdata),
        .S01_AXI_rid(S01_AXI_rid),
        .S01_AXI_rlast(S01_AXI_rlast),
        .S01_AXI_rready(S01_AXI_rready),
        .S01_AXI_rresp(S01_AXI_rresp),
        .S01_AXI_rvalid(S01_AXI_rvalid),
        .S01_AXI_wdata(S01_AXI_wdata),
        .S01_AXI_wlast(S01_AXI_wlast),
        .S01_AXI_wready(S01_AXI_wready),
        .S01_AXI_wstrb(S01_AXI_wstrb),
        .S01_AXI_wvalid(S01_AXI_wvalid),
        .S02_AXI_araddr(S02_AXI_araddr),
        .S02_AXI_arburst(S02_AXI_arburst),
        .S02_AXI_arcache(S02_AXI_arcache),
        .S02_AXI_arid(S02_AXI_arid),
        .S02_AXI_arlen(S02_AXI_arlen),
        .S02_AXI_arlock(S02_AXI_arlock),
        .S02_AXI_arprot(S02_AXI_arprot),
        .S02_AXI_arqos(S02_AXI_arqos),
        .S02_AXI_arready(S02_AXI_arready),
        .S02_AXI_arregion(S02_AXI_arregion),
        .S02_AXI_arvalid(S02_AXI_arvalid),
        .S02_AXI_awaddr(S02_AXI_awaddr),
        .S02_AXI_awburst(S02_AXI_awburst),
        .S02_AXI_awcache(S02_AXI_awcache),
        .S02_AXI_awid(S02_AXI_awid),
        .S02_AXI_awlen(S02_AXI_awlen),
        .S02_AXI_awlock(S02_AXI_awlock),
        .S02_AXI_awprot(S02_AXI_awprot),
        .S02_AXI_awqos(S02_AXI_awqos),
        .S02_AXI_awready(S02_AXI_awready),
        .S02_AXI_awregion(S02_AXI_awregion),
        .S02_AXI_awvalid(S02_AXI_awvalid),
        .S02_AXI_bid(S02_AXI_bid),
        .S02_AXI_bready(S02_AXI_bready),
        .S02_AXI_bresp(S02_AXI_bresp),
        .S02_AXI_bvalid(S02_AXI_bvalid),
        .S02_AXI_rdata(S02_AXI_rdata),
        .S02_AXI_rid(S02_AXI_rid),
        .S02_AXI_rlast(S02_AXI_rlast),
        .S02_AXI_rready(S02_AXI_rready),
        .S02_AXI_rresp(S02_AXI_rresp),
        .S02_AXI_rvalid(S02_AXI_rvalid),
        .S02_AXI_wdata(S02_AXI_wdata),
        .S02_AXI_wlast(S02_AXI_wlast),
        .S02_AXI_wready(S02_AXI_wready),
        .S02_AXI_wstrb(S02_AXI_wstrb),
        .S02_AXI_wvalid(S02_AXI_wvalid),
        .S04_AXI_araddr(S04_AXI_araddr),
        .S04_AXI_arcache(S04_AXI_arcache),
        .S04_AXI_arlen(S04_AXI_arlen),
        .S04_AXI_arlock(S04_AXI_arlock),
        .S04_AXI_arprot(S04_AXI_arprot),
        .S04_AXI_arqos(S04_AXI_arqos),
        .S04_AXI_arready(S04_AXI_arready),
        .S04_AXI_arregion(S04_AXI_arregion),
        .S04_AXI_arvalid(S04_AXI_arvalid),
        .S04_AXI_awaddr(S04_AXI_awaddr),
        .S04_AXI_awcache(S04_AXI_awcache),
        .S04_AXI_awlen(S04_AXI_awlen),
        .S04_AXI_awlock(S04_AXI_awlock),
        .S04_AXI_awprot(S04_AXI_awprot),
        .S04_AXI_awqos(S04_AXI_awqos),
        .S04_AXI_awready(S04_AXI_awready),
        .S04_AXI_awregion(S04_AXI_awregion),
        .S04_AXI_awvalid(S04_AXI_awvalid),
        .S04_AXI_bready(S04_AXI_bready),
        .S04_AXI_bresp(S04_AXI_bresp),
        .S04_AXI_bvalid(S04_AXI_bvalid),
        .S04_AXI_rdata(S04_AXI_rdata),
        .S04_AXI_rlast(S04_AXI_rlast),
        .S04_AXI_rready(S04_AXI_rready),
        .S04_AXI_rresp(S04_AXI_rresp),
        .S04_AXI_rvalid(S04_AXI_rvalid),
        .S04_AXI_wdata(S04_AXI_wdata),
        .S04_AXI_wlast(S04_AXI_wlast),
        .S04_AXI_wready(S04_AXI_wready),
        .S04_AXI_wstrb(S04_AXI_wstrb),
        .S04_AXI_wvalid(S04_AXI_wvalid),
        .S05_AXI_araddr(S05_AXI_araddr),
        .S05_AXI_arcache(S05_AXI_arcache),
        .S05_AXI_arlen(S05_AXI_arlen),
        .S05_AXI_arlock(S05_AXI_arlock),
        .S05_AXI_arprot(S05_AXI_arprot),
        .S05_AXI_arqos(S05_AXI_arqos),
        .S05_AXI_arready(S05_AXI_arready),
        .S05_AXI_arregion(S05_AXI_arregion),
        .S05_AXI_arvalid(S05_AXI_arvalid),
        .S05_AXI_awaddr(S05_AXI_awaddr),
        .S05_AXI_awcache(S05_AXI_awcache),
        .S05_AXI_awlen(S05_AXI_awlen),
        .S05_AXI_awlock(S05_AXI_awlock),
        .S05_AXI_awprot(S05_AXI_awprot),
        .S05_AXI_awqos(S05_AXI_awqos),
        .S05_AXI_awready(S05_AXI_awready),
        .S05_AXI_awregion(S05_AXI_awregion),
        .S05_AXI_awvalid(S05_AXI_awvalid),
        .S05_AXI_bready(S05_AXI_bready),
        .S05_AXI_bresp(S05_AXI_bresp),
        .S05_AXI_bvalid(S05_AXI_bvalid),
        .S05_AXI_rdata(S05_AXI_rdata),
        .S05_AXI_rlast(S05_AXI_rlast),
        .S05_AXI_rready(S05_AXI_rready),
        .S05_AXI_rresp(S05_AXI_rresp),
        .S05_AXI_rvalid(S05_AXI_rvalid),
        .S05_AXI_wdata(S05_AXI_wdata),
        .S05_AXI_wlast(S05_AXI_wlast),
        .S05_AXI_wready(S05_AXI_wready),
        .S05_AXI_wstrb(S05_AXI_wstrb),
        .S05_AXI_wvalid(S05_AXI_wvalid),
        .S_AXI_arready(interconnect_DDR4_MEM01_M00_AXI_ARREADY),
        .S_AXI_awready(interconnect_DDR4_MEM01_M00_AXI_AWREADY),
        .S_AXI_bresp(interconnect_DDR4_MEM01_M00_AXI_BRESP),
        .S_AXI_bvalid(interconnect_DDR4_MEM01_M00_AXI_BVALID),
        .S_AXI_rdata(interconnect_DDR4_MEM01_M00_AXI_RDATA),
        .S_AXI_rlast(interconnect_DDR4_MEM01_M00_AXI_RLAST),
        .S_AXI_rresp(interconnect_DDR4_MEM01_M00_AXI_RRESP),
        .S_AXI_rvalid(interconnect_DDR4_MEM01_M00_AXI_RVALID),
        .S_AXI_wready(interconnect_DDR4_MEM01_M00_AXI_WREADY),
        .aclk(aclk),
        .aclk2(aclk2),
        .\bbstub_m_axi_araddr[38] (M00_AXI_araddr),
        .\bbstub_m_axi_arburst[1] (M00_AXI_arburst),
        .\bbstub_m_axi_arcache[3] (M00_AXI_arcache),
        .\bbstub_m_axi_arlen[7] (M00_AXI_arlen),
        .\bbstub_m_axi_arlock[0] (M00_AXI_arlock),
        .\bbstub_m_axi_arprot[2] (M00_AXI_arprot),
        .\bbstub_m_axi_arqos[3] (M00_AXI_arqos),
        .bbstub_m_axi_arvalid(M00_AXI_arvalid),
        .\bbstub_m_axi_awaddr[38] (M00_AXI_awaddr),
        .\bbstub_m_axi_awburst[1] (M00_AXI_awburst),
        .\bbstub_m_axi_awcache[3] (M00_AXI_awcache),
        .\bbstub_m_axi_awlen[7] (M00_AXI_awlen),
        .\bbstub_m_axi_awlock[0] (M00_AXI_awlock),
        .\bbstub_m_axi_awprot[2] (M00_AXI_awprot),
        .\bbstub_m_axi_awqos[3] (M00_AXI_awqos),
        .bbstub_m_axi_awvalid(M00_AXI_awvalid),
        .bbstub_m_axi_bready(M00_AXI_bready),
        .bbstub_m_axi_rready(M00_AXI_rready),
        .\bbstub_m_axi_wdata[511] (M00_AXI_wdata),
        .bbstub_m_axi_wlast(M00_AXI_wlast),
        .\bbstub_m_axi_wstrb[63] (M00_AXI_wstrb),
        .bbstub_m_axi_wvalid(M00_AXI_wvalid),
        .ddr4_mem01_ui_clk(ddr4_mem01_ui_clk),
        .interconnect_aresetn(psr_aclk_SLR1_interconnect_aresetn),
        .interconnect_aresetn1(psr_aclk2_SLR1_interconnect_aresetn));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_memory_imp_1K98CM8 memory
       (.DDR4_MEM01_DIFF_CLK_clk_n(DDR4_MEM01_DIFF_CLK_clk_n),
        .DDR4_MEM01_DIFF_CLK_clk_p(DDR4_MEM01_DIFF_CLK_clk_p),
        .DDR4_MEM01_act_n(DDR4_MEM01_act_n),
        .DDR4_MEM01_adr(DDR4_MEM01_adr),
        .DDR4_MEM01_ba(DDR4_MEM01_ba),
        .DDR4_MEM01_bg(DDR4_MEM01_bg),
        .DDR4_MEM01_ck_c(DDR4_MEM01_ck_c),
        .DDR4_MEM01_ck_t(DDR4_MEM01_ck_t),
        .DDR4_MEM01_cke(DDR4_MEM01_cke),
        .DDR4_MEM01_cs_n(DDR4_MEM01_cs_n),
        .DDR4_MEM01_dq(DDR4_MEM01_dq),
        .DDR4_MEM01_dqs_c(DDR4_MEM01_dqs_c),
        .DDR4_MEM01_dqs_t(DDR4_MEM01_dqs_t),
        .DDR4_MEM01_odt(DDR4_MEM01_odt),
        .DDR4_MEM01_par(DDR4_MEM01_par),
        .DDR4_MEM01_reset_n(DDR4_MEM01_reset_n),
        .S_AXI_CTRL_araddr(S_AXI_CTRL_araddr),
        .S_AXI_CTRL_arprot(S_AXI_CTRL_arprot),
        .S_AXI_CTRL_arready(S_AXI_CTRL_arready),
        .S_AXI_CTRL_arvalid(S_AXI_CTRL_arvalid),
        .S_AXI_CTRL_awaddr(S_AXI_CTRL_awaddr),
        .S_AXI_CTRL_awprot(S_AXI_CTRL_awprot),
        .S_AXI_CTRL_awready(S_AXI_CTRL_awready),
        .S_AXI_CTRL_awvalid(S_AXI_CTRL_awvalid),
        .S_AXI_CTRL_bready(S_AXI_CTRL_bready),
        .S_AXI_CTRL_bresp(S_AXI_CTRL_bresp),
        .S_AXI_CTRL_bvalid(S_AXI_CTRL_bvalid),
        .S_AXI_CTRL_rdata(S_AXI_CTRL_rdata),
        .S_AXI_CTRL_rready(S_AXI_CTRL_rready),
        .S_AXI_CTRL_rresp(S_AXI_CTRL_rresp),
        .S_AXI_CTRL_rvalid(S_AXI_CTRL_rvalid),
        .S_AXI_CTRL_wdata(S_AXI_CTRL_wdata),
        .S_AXI_CTRL_wready(S_AXI_CTRL_wready),
        .S_AXI_CTRL_wstrb(S_AXI_CTRL_wstrb),
        .S_AXI_CTRL_wvalid(S_AXI_CTRL_wvalid),
        .S_AXI_araddr(interconnect_DDR4_MEM01_M00_AXI_ARADDR),
        .S_AXI_arburst(interconnect_DDR4_MEM01_M00_AXI_ARBURST),
        .S_AXI_arcache(interconnect_DDR4_MEM01_M00_AXI_ARCACHE),
        .S_AXI_arlen(interconnect_DDR4_MEM01_M00_AXI_ARLEN),
        .S_AXI_arlock(interconnect_DDR4_MEM01_M00_AXI_ARLOCK),
        .S_AXI_arprot(interconnect_DDR4_MEM01_M00_AXI_ARPROT),
        .S_AXI_arqos(interconnect_DDR4_MEM01_M00_AXI_ARQOS),
        .S_AXI_arready(interconnect_DDR4_MEM01_M00_AXI_ARREADY),
        .S_AXI_arvalid(interconnect_DDR4_MEM01_M00_AXI_ARVALID),
        .S_AXI_awaddr(interconnect_DDR4_MEM01_M00_AXI_AWADDR),
        .S_AXI_awburst(interconnect_DDR4_MEM01_M00_AXI_AWBURST),
        .S_AXI_awcache(interconnect_DDR4_MEM01_M00_AXI_AWCACHE),
        .S_AXI_awlen(interconnect_DDR4_MEM01_M00_AXI_AWLEN),
        .S_AXI_awlock(interconnect_DDR4_MEM01_M00_AXI_AWLOCK),
        .S_AXI_awprot(interconnect_DDR4_MEM01_M00_AXI_AWPROT),
        .S_AXI_awqos(interconnect_DDR4_MEM01_M00_AXI_AWQOS),
        .S_AXI_awready(interconnect_DDR4_MEM01_M00_AXI_AWREADY),
        .S_AXI_awvalid(interconnect_DDR4_MEM01_M00_AXI_AWVALID),
        .S_AXI_bready(interconnect_DDR4_MEM01_M00_AXI_BREADY),
        .S_AXI_bresp(interconnect_DDR4_MEM01_M00_AXI_BRESP),
        .S_AXI_bvalid(interconnect_DDR4_MEM01_M00_AXI_BVALID),
        .S_AXI_rdata(interconnect_DDR4_MEM01_M00_AXI_RDATA),
        .S_AXI_rlast(interconnect_DDR4_MEM01_M00_AXI_RLAST),
        .S_AXI_rready(interconnect_DDR4_MEM01_M00_AXI_RREADY),
        .S_AXI_rresp(interconnect_DDR4_MEM01_M00_AXI_RRESP),
        .S_AXI_rvalid(interconnect_DDR4_MEM01_M00_AXI_RVALID),
        .S_AXI_wdata(interconnect_DDR4_MEM01_M00_AXI_WDATA),
        .S_AXI_wlast(interconnect_DDR4_MEM01_M00_AXI_WLAST),
        .S_AXI_wready(interconnect_DDR4_MEM01_M00_AXI_WREADY),
        .S_AXI_wstrb(interconnect_DDR4_MEM01_M00_AXI_WSTRB),
        .S_AXI_wvalid(interconnect_DDR4_MEM01_M00_AXI_WVALID),
        .aclk1(aclk1),
        .aresetn(aresetn),
        .ddr4_mem01_sys_rst(ddr4_mem01_sys_rst),
        .ddr4_mem01_ui_clk(ddr4_mem01_ui_clk),
        .ddr4_mem_calib_complete(ddr4_mem_calib_complete),
        .ddr4_mem_calib_vec(ddr4_mem_calib_vec));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_reset_imp_1YKOSPE reset
       (.aclk(aclk),
        .aclk2(aclk2),
        .aresetn(aresetn),
        .interconnect_aresetn(psr_aclk_SLR1_interconnect_aresetn),
        .interconnect_aresetn1(psr_aclk2_SLR1_interconnect_aresetn));
endmodule

(* CHECK_LICENSE_TYPE = "bd_d216_calib_concat_0,xlconcat_v2_1_3_xlconcat,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* X_CORE_INFO = "xlconcat_v2_1_3_xlconcat,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0
   (In0,
    dout);
  input [0:0]In0;
  output [0:0]dout;

  wire [0:0]In0;

  assign dout[0] = In0;
endmodule

(* X_CORE_INFO = "util_reduced_logic_v2_0_4_util_reduced_logic,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_reduce_0
   (Op1,
    Res);
  input [0:0]Op1;
  output Res;


endmodule

(* CHECK_LICENSE_TYPE = "bd_d216_calib_vector_concat_0,xlconcat_v2_1_3_xlconcat,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* X_CORE_INFO = "xlconcat_v2_1_3_xlconcat,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0
   (In0,
    In1,
    In2,
    dout);
  input [0:0]In0;
  input [0:0]In1;
  input [0:0]In2;
  output [2:0]dout;

  wire [0:0]In0;
  wire [0:0]In1;
  wire [0:0]In2;

  assign dout[2] = In2;
  assign dout[1] = In1;
  assign dout[0] = In0;
endmodule

(* X_CORE_INFO = "ddr4_v2_2_9,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_0
   (sys_rst,
    c0_sys_clk_p,
    c0_sys_clk_n,
    c0_ddr4_act_n,
    c0_ddr4_adr,
    c0_ddr4_ba,
    c0_ddr4_bg,
    c0_ddr4_cke,
    c0_ddr4_odt,
    c0_ddr4_cs_n,
    c0_ddr4_ck_t,
    c0_ddr4_ck_c,
    c0_ddr4_reset_n,
    c0_ddr4_parity,
    c0_ddr4_dq,
    c0_ddr4_dqs_c,
    c0_ddr4_dqs_t,
    c0_init_calib_complete,
    c0_ddr4_ui_clk,
    c0_ddr4_ui_clk_sync_rst,
    addn_ui_clkout1,
    dbg_clk,
    c0_ddr4_s_axi_ctrl_awvalid,
    c0_ddr4_s_axi_ctrl_awready,
    c0_ddr4_s_axi_ctrl_awaddr,
    c0_ddr4_s_axi_ctrl_wvalid,
    c0_ddr4_s_axi_ctrl_wready,
    c0_ddr4_s_axi_ctrl_wdata,
    c0_ddr4_s_axi_ctrl_bvalid,
    c0_ddr4_s_axi_ctrl_bready,
    c0_ddr4_s_axi_ctrl_bresp,
    c0_ddr4_s_axi_ctrl_arvalid,
    c0_ddr4_s_axi_ctrl_arready,
    c0_ddr4_s_axi_ctrl_araddr,
    c0_ddr4_s_axi_ctrl_rvalid,
    c0_ddr4_s_axi_ctrl_rready,
    c0_ddr4_s_axi_ctrl_rdata,
    c0_ddr4_s_axi_ctrl_rresp,
    c0_ddr4_interrupt,
    c0_ddr4_aresetn,
    c0_ddr4_s_axi_awid,
    c0_ddr4_s_axi_awaddr,
    c0_ddr4_s_axi_awlen,
    c0_ddr4_s_axi_awsize,
    c0_ddr4_s_axi_awburst,
    c0_ddr4_s_axi_awlock,
    c0_ddr4_s_axi_awcache,
    c0_ddr4_s_axi_awprot,
    c0_ddr4_s_axi_awqos,
    c0_ddr4_s_axi_awvalid,
    c0_ddr4_s_axi_awready,
    c0_ddr4_s_axi_wdata,
    c0_ddr4_s_axi_wstrb,
    c0_ddr4_s_axi_wlast,
    c0_ddr4_s_axi_wvalid,
    c0_ddr4_s_axi_wready,
    c0_ddr4_s_axi_bready,
    c0_ddr4_s_axi_bid,
    c0_ddr4_s_axi_bresp,
    c0_ddr4_s_axi_bvalid,
    c0_ddr4_s_axi_arid,
    c0_ddr4_s_axi_araddr,
    c0_ddr4_s_axi_arlen,
    c0_ddr4_s_axi_arsize,
    c0_ddr4_s_axi_arburst,
    c0_ddr4_s_axi_arlock,
    c0_ddr4_s_axi_arcache,
    c0_ddr4_s_axi_arprot,
    c0_ddr4_s_axi_arqos,
    c0_ddr4_s_axi_arvalid,
    c0_ddr4_s_axi_arready,
    c0_ddr4_s_axi_rready,
    c0_ddr4_s_axi_rid,
    c0_ddr4_s_axi_rdata,
    c0_ddr4_s_axi_rresp,
    c0_ddr4_s_axi_rlast,
    c0_ddr4_s_axi_rvalid,
    dbg_bus);
  input sys_rst;
  input c0_sys_clk_p;
  input c0_sys_clk_n;
  output c0_ddr4_act_n;
  output [16:0]c0_ddr4_adr;
  output [1:0]c0_ddr4_ba;
  output [1:0]c0_ddr4_bg;
  output [0:0]c0_ddr4_cke;
  output [0:0]c0_ddr4_odt;
  output [0:0]c0_ddr4_cs_n;
  output [0:0]c0_ddr4_ck_t;
  output [0:0]c0_ddr4_ck_c;
  output c0_ddr4_reset_n;
  output c0_ddr4_parity;
  inout [71:0]c0_ddr4_dq;
  inout [17:0]c0_ddr4_dqs_c;
  inout [17:0]c0_ddr4_dqs_t;
  output c0_init_calib_complete;
  output c0_ddr4_ui_clk;
  output c0_ddr4_ui_clk_sync_rst;
  output addn_ui_clkout1;
  output dbg_clk;
  input c0_ddr4_s_axi_ctrl_awvalid;
  output c0_ddr4_s_axi_ctrl_awready;
  input [31:0]c0_ddr4_s_axi_ctrl_awaddr;
  input c0_ddr4_s_axi_ctrl_wvalid;
  output c0_ddr4_s_axi_ctrl_wready;
  input [31:0]c0_ddr4_s_axi_ctrl_wdata;
  output c0_ddr4_s_axi_ctrl_bvalid;
  input c0_ddr4_s_axi_ctrl_bready;
  output [1:0]c0_ddr4_s_axi_ctrl_bresp;
  input c0_ddr4_s_axi_ctrl_arvalid;
  output c0_ddr4_s_axi_ctrl_arready;
  input [31:0]c0_ddr4_s_axi_ctrl_araddr;
  output c0_ddr4_s_axi_ctrl_rvalid;
  input c0_ddr4_s_axi_ctrl_rready;
  output [31:0]c0_ddr4_s_axi_ctrl_rdata;
  output [1:0]c0_ddr4_s_axi_ctrl_rresp;
  output c0_ddr4_interrupt;
  input c0_ddr4_aresetn;
  input [0:0]c0_ddr4_s_axi_awid;
  input [33:0]c0_ddr4_s_axi_awaddr;
  input [7:0]c0_ddr4_s_axi_awlen;
  input [2:0]c0_ddr4_s_axi_awsize;
  input [1:0]c0_ddr4_s_axi_awburst;
  input [0:0]c0_ddr4_s_axi_awlock;
  input [3:0]c0_ddr4_s_axi_awcache;
  input [2:0]c0_ddr4_s_axi_awprot;
  input [3:0]c0_ddr4_s_axi_awqos;
  input c0_ddr4_s_axi_awvalid;
  output c0_ddr4_s_axi_awready;
  input [511:0]c0_ddr4_s_axi_wdata;
  input [63:0]c0_ddr4_s_axi_wstrb;
  input c0_ddr4_s_axi_wlast;
  input c0_ddr4_s_axi_wvalid;
  output c0_ddr4_s_axi_wready;
  input c0_ddr4_s_axi_bready;
  output [0:0]c0_ddr4_s_axi_bid;
  output [1:0]c0_ddr4_s_axi_bresp;
  output c0_ddr4_s_axi_bvalid;
  input [0:0]c0_ddr4_s_axi_arid;
  input [33:0]c0_ddr4_s_axi_araddr;
  input [7:0]c0_ddr4_s_axi_arlen;
  input [2:0]c0_ddr4_s_axi_arsize;
  input [1:0]c0_ddr4_s_axi_arburst;
  input [0:0]c0_ddr4_s_axi_arlock;
  input [3:0]c0_ddr4_s_axi_arcache;
  input [2:0]c0_ddr4_s_axi_arprot;
  input [3:0]c0_ddr4_s_axi_arqos;
  input c0_ddr4_s_axi_arvalid;
  output c0_ddr4_s_axi_arready;
  input c0_ddr4_s_axi_rready;
  output [0:0]c0_ddr4_s_axi_rid;
  output [511:0]c0_ddr4_s_axi_rdata;
  output [1:0]c0_ddr4_s_axi_rresp;
  output c0_ddr4_s_axi_rlast;
  output c0_ddr4_s_axi_rvalid;
  output [511:0]dbg_bus;


endmodule

(* X_CORE_INFO = "axi_clock_converter_v2_1_20_axi_clock_converter,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_ctrl_cc_0
   (s_axi_aclk,
    s_axi_aresetn,
    s_axi_awaddr,
    s_axi_awprot,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arprot,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_aclk,
    m_axi_aresetn,
    m_axi_awaddr,
    m_axi_awprot,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arprot,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rvalid,
    m_axi_rready);
  input s_axi_aclk;
  input s_axi_aresetn;
  input [31:0]s_axi_awaddr;
  input [2:0]s_axi_awprot;
  input s_axi_awvalid;
  output s_axi_awready;
  input [31:0]s_axi_wdata;
  input [3:0]s_axi_wstrb;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [31:0]s_axi_araddr;
  input [2:0]s_axi_arprot;
  input s_axi_arvalid;
  output s_axi_arready;
  output [31:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rvalid;
  input s_axi_rready;
  input m_axi_aclk;
  input m_axi_aresetn;
  output [31:0]m_axi_awaddr;
  output [2:0]m_axi_awprot;
  output m_axi_awvalid;
  input m_axi_awready;
  output [31:0]m_axi_wdata;
  output [3:0]m_axi_wstrb;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [31:0]m_axi_araddr;
  output [2:0]m_axi_arprot;
  output m_axi_arvalid;
  input m_axi_arready;
  input [31:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "bd_e7f0,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_DDR4_MEM01_0
   (aclk,
    aclk1,
    aresetn,
    S00_AXI_awid,
    S00_AXI_awaddr,
    S00_AXI_awlen,
    S00_AXI_awsize,
    S00_AXI_awburst,
    S00_AXI_awlock,
    S00_AXI_awcache,
    S00_AXI_awprot,
    S00_AXI_awqos,
    S00_AXI_awvalid,
    S00_AXI_awready,
    S00_AXI_wdata,
    S00_AXI_wstrb,
    S00_AXI_wlast,
    S00_AXI_wvalid,
    S00_AXI_wready,
    S00_AXI_bid,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_bready,
    S00_AXI_arid,
    S00_AXI_araddr,
    S00_AXI_arlen,
    S00_AXI_arsize,
    S00_AXI_arburst,
    S00_AXI_arlock,
    S00_AXI_arcache,
    S00_AXI_arprot,
    S00_AXI_arqos,
    S00_AXI_arvalid,
    S00_AXI_arready,
    S00_AXI_rid,
    S00_AXI_rdata,
    S00_AXI_rresp,
    S00_AXI_rlast,
    S00_AXI_rvalid,
    S00_AXI_rready,
    S01_AXI_awid,
    S01_AXI_awaddr,
    S01_AXI_awlen,
    S01_AXI_awsize,
    S01_AXI_awburst,
    S01_AXI_awlock,
    S01_AXI_awcache,
    S01_AXI_awprot,
    S01_AXI_awqos,
    S01_AXI_awuser,
    S01_AXI_awvalid,
    S01_AXI_awready,
    S01_AXI_wdata,
    S01_AXI_wstrb,
    S01_AXI_wlast,
    S01_AXI_wuser,
    S01_AXI_wvalid,
    S01_AXI_wready,
    S01_AXI_bid,
    S01_AXI_bresp,
    S01_AXI_buser,
    S01_AXI_bvalid,
    S01_AXI_bready,
    S01_AXI_arid,
    S01_AXI_araddr,
    S01_AXI_arlen,
    S01_AXI_arsize,
    S01_AXI_arburst,
    S01_AXI_arlock,
    S01_AXI_arcache,
    S01_AXI_arprot,
    S01_AXI_arqos,
    S01_AXI_aruser,
    S01_AXI_arvalid,
    S01_AXI_arready,
    S01_AXI_rid,
    S01_AXI_rdata,
    S01_AXI_rresp,
    S01_AXI_rlast,
    S01_AXI_ruser,
    S01_AXI_rvalid,
    S01_AXI_rready,
    M00_AXI_awaddr,
    M00_AXI_awlen,
    M00_AXI_awsize,
    M00_AXI_awburst,
    M00_AXI_awlock,
    M00_AXI_awcache,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awvalid,
    M00_AXI_awready,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    M00_AXI_wlast,
    M00_AXI_wvalid,
    M00_AXI_wready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_bready,
    M00_AXI_araddr,
    M00_AXI_arlen,
    M00_AXI_arsize,
    M00_AXI_arburst,
    M00_AXI_arlock,
    M00_AXI_arcache,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arvalid,
    M00_AXI_arready,
    M00_AXI_rdata,
    M00_AXI_rresp,
    M00_AXI_rlast,
    M00_AXI_rvalid,
    M00_AXI_rready);
  input aclk;
  input aclk1;
  input aresetn;
  input [3:0]S00_AXI_awid;
  input [38:0]S00_AXI_awaddr;
  input [7:0]S00_AXI_awlen;
  input [2:0]S00_AXI_awsize;
  input [1:0]S00_AXI_awburst;
  input [0:0]S00_AXI_awlock;
  input [3:0]S00_AXI_awcache;
  input [2:0]S00_AXI_awprot;
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
  input [3:0]S00_AXI_arqos;
  input S00_AXI_arvalid;
  output S00_AXI_arready;
  output [3:0]S00_AXI_rid;
  output [511:0]S00_AXI_rdata;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rlast;
  output S00_AXI_rvalid;
  input S00_AXI_rready;
  input [1:0]S01_AXI_awid;
  input [38:0]S01_AXI_awaddr;
  input [7:0]S01_AXI_awlen;
  input [2:0]S01_AXI_awsize;
  input [1:0]S01_AXI_awburst;
  input [0:0]S01_AXI_awlock;
  input [3:0]S01_AXI_awcache;
  input [2:0]S01_AXI_awprot;
  input [3:0]S01_AXI_awqos;
  input [113:0]S01_AXI_awuser;
  input S01_AXI_awvalid;
  output S01_AXI_awready;
  input [511:0]S01_AXI_wdata;
  input [63:0]S01_AXI_wstrb;
  input S01_AXI_wlast;
  input [13:0]S01_AXI_wuser;
  input S01_AXI_wvalid;
  output S01_AXI_wready;
  output [1:0]S01_AXI_bid;
  output [1:0]S01_AXI_bresp;
  output [113:0]S01_AXI_buser;
  output S01_AXI_bvalid;
  input S01_AXI_bready;
  input [1:0]S01_AXI_arid;
  input [38:0]S01_AXI_araddr;
  input [7:0]S01_AXI_arlen;
  input [2:0]S01_AXI_arsize;
  input [1:0]S01_AXI_arburst;
  input [0:0]S01_AXI_arlock;
  input [3:0]S01_AXI_arcache;
  input [2:0]S01_AXI_arprot;
  input [3:0]S01_AXI_arqos;
  input [113:0]S01_AXI_aruser;
  input S01_AXI_arvalid;
  output S01_AXI_arready;
  output [1:0]S01_AXI_rid;
  output [511:0]S01_AXI_rdata;
  output [1:0]S01_AXI_rresp;
  output S01_AXI_rlast;
  output [13:0]S01_AXI_ruser;
  output S01_AXI_rvalid;
  input S01_AXI_rready;
  output [33:0]M00_AXI_awaddr;
  output [7:0]M00_AXI_awlen;
  output [2:0]M00_AXI_awsize;
  output [1:0]M00_AXI_awburst;
  output [0:0]M00_AXI_awlock;
  output [3:0]M00_AXI_awcache;
  output [2:0]M00_AXI_awprot;
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
  output [33:0]M00_AXI_araddr;
  output [7:0]M00_AXI_arlen;
  output [2:0]M00_AXI_arsize;
  output [1:0]M00_AXI_arburst;
  output [0:0]M00_AXI_arlock;
  output [3:0]M00_AXI_arcache;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arqos;
  output M00_AXI_arvalid;
  input M00_AXI_arready;
  input [511:0]M00_AXI_rdata;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rlast;
  input M00_AXI_rvalid;
  output M00_AXI_rready;


endmodule

(* X_CORE_INFO = "bd_2b97,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_M00_AXI_MEM00_0
   (aclk,
    aclk1,
    aresetn,
    S00_AXI_awid,
    S00_AXI_awaddr,
    S00_AXI_awlen,
    S00_AXI_awsize,
    S00_AXI_awburst,
    S00_AXI_awlock,
    S00_AXI_awcache,
    S00_AXI_awprot,
    S00_AXI_awqos,
    S00_AXI_awvalid,
    S00_AXI_awready,
    S00_AXI_wdata,
    S00_AXI_wstrb,
    S00_AXI_wlast,
    S00_AXI_wvalid,
    S00_AXI_wready,
    S00_AXI_bid,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_bready,
    S00_AXI_arid,
    S00_AXI_araddr,
    S00_AXI_arlen,
    S00_AXI_arsize,
    S00_AXI_arburst,
    S00_AXI_arlock,
    S00_AXI_arcache,
    S00_AXI_arprot,
    S00_AXI_arqos,
    S00_AXI_arvalid,
    S00_AXI_arready,
    S00_AXI_rid,
    S00_AXI_rdata,
    S00_AXI_rresp,
    S00_AXI_rlast,
    S00_AXI_rvalid,
    S00_AXI_rready,
    S01_AXI_awaddr,
    S01_AXI_awlen,
    S01_AXI_awsize,
    S01_AXI_awburst,
    S01_AXI_awlock,
    S01_AXI_awcache,
    S01_AXI_awprot,
    S01_AXI_awqos,
    S01_AXI_awvalid,
    S01_AXI_awready,
    S01_AXI_wdata,
    S01_AXI_wstrb,
    S01_AXI_wlast,
    S01_AXI_wvalid,
    S01_AXI_wready,
    S01_AXI_bresp,
    S01_AXI_bvalid,
    S01_AXI_bready,
    S01_AXI_araddr,
    S01_AXI_arlen,
    S01_AXI_arsize,
    S01_AXI_arburst,
    S01_AXI_arlock,
    S01_AXI_arcache,
    S01_AXI_arprot,
    S01_AXI_arqos,
    S01_AXI_arvalid,
    S01_AXI_arready,
    S01_AXI_rdata,
    S01_AXI_rresp,
    S01_AXI_rlast,
    S01_AXI_rvalid,
    S01_AXI_rready,
    S02_AXI_awid,
    S02_AXI_awaddr,
    S02_AXI_awlen,
    S02_AXI_awsize,
    S02_AXI_awburst,
    S02_AXI_awlock,
    S02_AXI_awcache,
    S02_AXI_awprot,
    S02_AXI_awqos,
    S02_AXI_awuser,
    S02_AXI_awvalid,
    S02_AXI_awready,
    S02_AXI_wdata,
    S02_AXI_wstrb,
    S02_AXI_wlast,
    S02_AXI_wuser,
    S02_AXI_wvalid,
    S02_AXI_wready,
    S02_AXI_bid,
    S02_AXI_bresp,
    S02_AXI_buser,
    S02_AXI_bvalid,
    S02_AXI_bready,
    S02_AXI_arid,
    S02_AXI_araddr,
    S02_AXI_arlen,
    S02_AXI_arsize,
    S02_AXI_arburst,
    S02_AXI_arlock,
    S02_AXI_arcache,
    S02_AXI_arprot,
    S02_AXI_arqos,
    S02_AXI_aruser,
    S02_AXI_arvalid,
    S02_AXI_arready,
    S02_AXI_rid,
    S02_AXI_rdata,
    S02_AXI_rresp,
    S02_AXI_rlast,
    S02_AXI_ruser,
    S02_AXI_rvalid,
    S02_AXI_rready,
    M00_AXI_awaddr,
    M00_AXI_awlen,
    M00_AXI_awsize,
    M00_AXI_awburst,
    M00_AXI_awlock,
    M00_AXI_awcache,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awvalid,
    M00_AXI_awready,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    M00_AXI_wlast,
    M00_AXI_wvalid,
    M00_AXI_wready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_bready,
    M00_AXI_araddr,
    M00_AXI_arlen,
    M00_AXI_arsize,
    M00_AXI_arburst,
    M00_AXI_arlock,
    M00_AXI_arcache,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arvalid,
    M00_AXI_arready,
    M00_AXI_rdata,
    M00_AXI_rresp,
    M00_AXI_rlast,
    M00_AXI_rvalid,
    M00_AXI_rready);
  input aclk;
  input aclk1;
  input aresetn;
  input [3:0]S00_AXI_awid;
  input [38:0]S00_AXI_awaddr;
  input [7:0]S00_AXI_awlen;
  input [2:0]S00_AXI_awsize;
  input [1:0]S00_AXI_awburst;
  input [0:0]S00_AXI_awlock;
  input [3:0]S00_AXI_awcache;
  input [2:0]S00_AXI_awprot;
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
  input [2:0]S01_AXI_awsize;
  input [1:0]S01_AXI_awburst;
  input [0:0]S01_AXI_awlock;
  input [3:0]S01_AXI_awcache;
  input [2:0]S01_AXI_awprot;
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
  input [2:0]S01_AXI_arsize;
  input [1:0]S01_AXI_arburst;
  input [0:0]S01_AXI_arlock;
  input [3:0]S01_AXI_arcache;
  input [2:0]S01_AXI_arprot;
  input [3:0]S01_AXI_arqos;
  input S01_AXI_arvalid;
  output S01_AXI_arready;
  output [511:0]S01_AXI_rdata;
  output [1:0]S01_AXI_rresp;
  output S01_AXI_rlast;
  output S01_AXI_rvalid;
  input S01_AXI_rready;
  input [1:0]S02_AXI_awid;
  input [38:0]S02_AXI_awaddr;
  input [7:0]S02_AXI_awlen;
  input [2:0]S02_AXI_awsize;
  input [1:0]S02_AXI_awburst;
  input [0:0]S02_AXI_awlock;
  input [3:0]S02_AXI_awcache;
  input [2:0]S02_AXI_awprot;
  input [3:0]S02_AXI_awqos;
  input [113:0]S02_AXI_awuser;
  input S02_AXI_awvalid;
  output S02_AXI_awready;
  input [511:0]S02_AXI_wdata;
  input [63:0]S02_AXI_wstrb;
  input S02_AXI_wlast;
  input [13:0]S02_AXI_wuser;
  input S02_AXI_wvalid;
  output S02_AXI_wready;
  output [1:0]S02_AXI_bid;
  output [1:0]S02_AXI_bresp;
  output [113:0]S02_AXI_buser;
  output S02_AXI_bvalid;
  input S02_AXI_bready;
  input [1:0]S02_AXI_arid;
  input [38:0]S02_AXI_araddr;
  input [7:0]S02_AXI_arlen;
  input [2:0]S02_AXI_arsize;
  input [1:0]S02_AXI_arburst;
  input [0:0]S02_AXI_arlock;
  input [3:0]S02_AXI_arcache;
  input [2:0]S02_AXI_arprot;
  input [3:0]S02_AXI_arqos;
  input [113:0]S02_AXI_aruser;
  input S02_AXI_arvalid;
  output S02_AXI_arready;
  output [1:0]S02_AXI_rid;
  output [511:0]S02_AXI_rdata;
  output [1:0]S02_AXI_rresp;
  output S02_AXI_rlast;
  output [13:0]S02_AXI_ruser;
  output S02_AXI_rvalid;
  input S02_AXI_rready;
  output [38:0]M00_AXI_awaddr;
  output [7:0]M00_AXI_awlen;
  output [2:0]M00_AXI_awsize;
  output [1:0]M00_AXI_awburst;
  output [0:0]M00_AXI_awlock;
  output [3:0]M00_AXI_awcache;
  output [2:0]M00_AXI_awprot;
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
  output [3:0]M00_AXI_arqos;
  output M00_AXI_arvalid;
  input M00_AXI_arready;
  input [511:0]M00_AXI_rdata;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rlast;
  input M00_AXI_rvalid;
  output M00_AXI_rready;


endmodule

(* X_CORE_INFO = "bd_8bbf,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_S04_AXI_0
   (aclk,
    aresetn,
    S00_AXI_awaddr,
    S00_AXI_awlen,
    S00_AXI_awsize,
    S00_AXI_awburst,
    S00_AXI_awlock,
    S00_AXI_awcache,
    S00_AXI_awprot,
    S00_AXI_awqos,
    S00_AXI_awvalid,
    S00_AXI_awready,
    S00_AXI_wdata,
    S00_AXI_wstrb,
    S00_AXI_wlast,
    S00_AXI_wvalid,
    S00_AXI_wready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_bready,
    S00_AXI_araddr,
    S00_AXI_arlen,
    S00_AXI_arsize,
    S00_AXI_arburst,
    S00_AXI_arlock,
    S00_AXI_arcache,
    S00_AXI_arprot,
    S00_AXI_arqos,
    S00_AXI_arvalid,
    S00_AXI_arready,
    S00_AXI_rdata,
    S00_AXI_rresp,
    S00_AXI_rlast,
    S00_AXI_rvalid,
    S00_AXI_rready,
    M00_AXI_awid,
    M00_AXI_awaddr,
    M00_AXI_awlen,
    M00_AXI_awsize,
    M00_AXI_awburst,
    M00_AXI_awlock,
    M00_AXI_awcache,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awuser,
    M00_AXI_awvalid,
    M00_AXI_awready,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    M00_AXI_wlast,
    M00_AXI_wuser,
    M00_AXI_wvalid,
    M00_AXI_wready,
    M00_AXI_bid,
    M00_AXI_bresp,
    M00_AXI_buser,
    M00_AXI_bvalid,
    M00_AXI_bready,
    M00_AXI_arid,
    M00_AXI_araddr,
    M00_AXI_arlen,
    M00_AXI_arsize,
    M00_AXI_arburst,
    M00_AXI_arlock,
    M00_AXI_arcache,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_aruser,
    M00_AXI_arvalid,
    M00_AXI_arready,
    M00_AXI_rid,
    M00_AXI_rdata,
    M00_AXI_rresp,
    M00_AXI_rlast,
    M00_AXI_ruser,
    M00_AXI_rvalid,
    M00_AXI_rready,
    M01_AXI_awid,
    M01_AXI_awaddr,
    M01_AXI_awlen,
    M01_AXI_awsize,
    M01_AXI_awburst,
    M01_AXI_awlock,
    M01_AXI_awcache,
    M01_AXI_awprot,
    M01_AXI_awqos,
    M01_AXI_awuser,
    M01_AXI_awvalid,
    M01_AXI_awready,
    M01_AXI_wdata,
    M01_AXI_wstrb,
    M01_AXI_wlast,
    M01_AXI_wuser,
    M01_AXI_wvalid,
    M01_AXI_wready,
    M01_AXI_bid,
    M01_AXI_bresp,
    M01_AXI_buser,
    M01_AXI_bvalid,
    M01_AXI_bready,
    M01_AXI_arid,
    M01_AXI_araddr,
    M01_AXI_arlen,
    M01_AXI_arsize,
    M01_AXI_arburst,
    M01_AXI_arlock,
    M01_AXI_arcache,
    M01_AXI_arprot,
    M01_AXI_arqos,
    M01_AXI_aruser,
    M01_AXI_arvalid,
    M01_AXI_arready,
    M01_AXI_rid,
    M01_AXI_rdata,
    M01_AXI_rresp,
    M01_AXI_rlast,
    M01_AXI_ruser,
    M01_AXI_rvalid,
    M01_AXI_rready);
  input aclk;
  input aresetn;
  input [38:0]S00_AXI_awaddr;
  input [7:0]S00_AXI_awlen;
  input [2:0]S00_AXI_awsize;
  input [1:0]S00_AXI_awburst;
  input [0:0]S00_AXI_awlock;
  input [3:0]S00_AXI_awcache;
  input [2:0]S00_AXI_awprot;
  input [3:0]S00_AXI_awqos;
  input S00_AXI_awvalid;
  output S00_AXI_awready;
  input [511:0]S00_AXI_wdata;
  input [63:0]S00_AXI_wstrb;
  input S00_AXI_wlast;
  input S00_AXI_wvalid;
  output S00_AXI_wready;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  input S00_AXI_bready;
  input [38:0]S00_AXI_araddr;
  input [7:0]S00_AXI_arlen;
  input [2:0]S00_AXI_arsize;
  input [1:0]S00_AXI_arburst;
  input [0:0]S00_AXI_arlock;
  input [3:0]S00_AXI_arcache;
  input [2:0]S00_AXI_arprot;
  input [3:0]S00_AXI_arqos;
  input S00_AXI_arvalid;
  output S00_AXI_arready;
  output [511:0]S00_AXI_rdata;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rlast;
  output S00_AXI_rvalid;
  input S00_AXI_rready;
  output [1:0]M00_AXI_awid;
  output [38:0]M00_AXI_awaddr;
  output [7:0]M00_AXI_awlen;
  output [2:0]M00_AXI_awsize;
  output [1:0]M00_AXI_awburst;
  output [0:0]M00_AXI_awlock;
  output [3:0]M00_AXI_awcache;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awqos;
  output [113:0]M00_AXI_awuser;
  output M00_AXI_awvalid;
  input M00_AXI_awready;
  output [511:0]M00_AXI_wdata;
  output [63:0]M00_AXI_wstrb;
  output M00_AXI_wlast;
  output [13:0]M00_AXI_wuser;
  output M00_AXI_wvalid;
  input M00_AXI_wready;
  input [1:0]M00_AXI_bid;
  input [1:0]M00_AXI_bresp;
  input [113:0]M00_AXI_buser;
  input M00_AXI_bvalid;
  output M00_AXI_bready;
  output [1:0]M00_AXI_arid;
  output [38:0]M00_AXI_araddr;
  output [7:0]M00_AXI_arlen;
  output [2:0]M00_AXI_arsize;
  output [1:0]M00_AXI_arburst;
  output [0:0]M00_AXI_arlock;
  output [3:0]M00_AXI_arcache;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arqos;
  output [113:0]M00_AXI_aruser;
  output M00_AXI_arvalid;
  input M00_AXI_arready;
  input [1:0]M00_AXI_rid;
  input [511:0]M00_AXI_rdata;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rlast;
  input [13:0]M00_AXI_ruser;
  input M00_AXI_rvalid;
  output M00_AXI_rready;
  output [1:0]M01_AXI_awid;
  output [38:0]M01_AXI_awaddr;
  output [7:0]M01_AXI_awlen;
  output [2:0]M01_AXI_awsize;
  output [1:0]M01_AXI_awburst;
  output [0:0]M01_AXI_awlock;
  output [3:0]M01_AXI_awcache;
  output [2:0]M01_AXI_awprot;
  output [3:0]M01_AXI_awqos;
  output [113:0]M01_AXI_awuser;
  output M01_AXI_awvalid;
  input M01_AXI_awready;
  output [511:0]M01_AXI_wdata;
  output [63:0]M01_AXI_wstrb;
  output M01_AXI_wlast;
  output [13:0]M01_AXI_wuser;
  output M01_AXI_wvalid;
  input M01_AXI_wready;
  input [1:0]M01_AXI_bid;
  input [1:0]M01_AXI_bresp;
  input [113:0]M01_AXI_buser;
  input M01_AXI_bvalid;
  output M01_AXI_bready;
  output [1:0]M01_AXI_arid;
  output [38:0]M01_AXI_araddr;
  output [7:0]M01_AXI_arlen;
  output [2:0]M01_AXI_arsize;
  output [1:0]M01_AXI_arburst;
  output [0:0]M01_AXI_arlock;
  output [3:0]M01_AXI_arcache;
  output [2:0]M01_AXI_arprot;
  output [3:0]M01_AXI_arqos;
  output [113:0]M01_AXI_aruser;
  output M01_AXI_arvalid;
  input M01_AXI_arready;
  input [1:0]M01_AXI_rid;
  input [511:0]M01_AXI_rdata;
  input [1:0]M01_AXI_rresp;
  input M01_AXI_rlast;
  input [13:0]M01_AXI_ruser;
  input M01_AXI_rvalid;
  output M01_AXI_rready;


endmodule

(* X_CORE_INFO = "axi_mmu_v2_1_19_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_ddrmem_ctrl_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awprot,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arprot,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awprot,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arprot,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [24:0]s_axi_awaddr;
  input [2:0]s_axi_awprot;
  input s_axi_awvalid;
  output s_axi_awready;
  input [31:0]s_axi_wdata;
  input [3:0]s_axi_wstrb;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [24:0]s_axi_araddr;
  input [2:0]s_axi_arprot;
  input s_axi_arvalid;
  output s_axi_arready;
  output [31:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rvalid;
  input s_axi_rready;
  output [31:0]m_axi_awaddr;
  output [2:0]m_axi_awprot;
  output m_axi_awvalid;
  input m_axi_awready;
  output [31:0]m_axi_wdata;
  output [3:0]m_axi_wstrb;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [31:0]m_axi_araddr;
  output [2:0]m_axi_arprot;
  output m_axi_arvalid;
  input m_axi_arready;
  input [31:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk2_SLR1_0
   (slowest_sync_clk,
    ext_reset_in,
    aux_reset_in,
    mb_debug_sys_rst,
    dcm_locked,
    mb_reset,
    bus_struct_reset,
    peripheral_reset,
    interconnect_aresetn,
    peripheral_aresetn);
  input slowest_sync_clk;
  input ext_reset_in;
  input aux_reset_in;
  input mb_debug_sys_rst;
  input dcm_locked;
  output mb_reset;
  output [0:0]bus_struct_reset;
  output [0:0]peripheral_reset;
  output [0:0]interconnect_aresetn;
  output [0:0]peripheral_aresetn;


endmodule

(* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk_SLR1_0
   (slowest_sync_clk,
    ext_reset_in,
    aux_reset_in,
    mb_debug_sys_rst,
    dcm_locked,
    mb_reset,
    bus_struct_reset,
    peripheral_reset,
    interconnect_aresetn,
    peripheral_aresetn);
  input slowest_sync_clk;
  input ext_reset_in;
  input aux_reset_in;
  input mb_debug_sys_rst;
  input dcm_locked;
  output mb_reset;
  output [0:0]bus_struct_reset;
  output [0:0]peripheral_reset;
  output [0:0]interconnect_aresetn;
  output [0:0]peripheral_aresetn;


endmodule

(* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ctrl_interconnect_0
   (slowest_sync_clk,
    ext_reset_in,
    aux_reset_in,
    mb_debug_sys_rst,
    dcm_locked,
    mb_reset,
    bus_struct_reset,
    peripheral_reset,
    interconnect_aresetn,
    peripheral_aresetn);
  input slowest_sync_clk;
  input ext_reset_in;
  input aux_reset_in;
  input mb_debug_sys_rst;
  input dcm_locked;
  output mb_reset;
  output [0:0]bus_struct_reset;
  output [0:0]peripheral_reset;
  output [0:0]interconnect_aresetn;
  output [0:0]peripheral_aresetn;


endmodule

(* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ddr4_mem01_0
   (slowest_sync_clk,
    ext_reset_in,
    aux_reset_in,
    mb_debug_sys_rst,
    dcm_locked,
    mb_reset,
    bus_struct_reset,
    peripheral_reset,
    interconnect_aresetn,
    peripheral_aresetn);
  input slowest_sync_clk;
  input ext_reset_in;
  input aux_reset_in;
  input mb_debug_sys_rst;
  input dcm_locked;
  output mb_reset;
  output [0:0]bus_struct_reset;
  output [0:0]peripheral_reset;
  output [0:0]interconnect_aresetn;
  output [0:0]peripheral_aresetn;


endmodule

(* X_CORE_INFO = "axi_register_slice_v2_1_21_axi_register_slice,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_rs_M00_AXI_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awsize,
    s_axi_awburst,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awregion,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arsize,
    s_axi_arburst,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arregion,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awsize,
    m_axi_awburst,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awregion,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arsize,
    m_axi_arburst,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arregion,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [38:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [2:0]s_axi_awsize;
  input [1:0]s_axi_awburst;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awregion;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [38:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [2:0]s_axi_arsize;
  input [1:0]s_axi_arburst;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arregion;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [38:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [2:0]m_axi_awsize;
  output [1:0]m_axi_awburst;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awregion;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [38:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [2:0]m_axi_arsize;
  output [1:0]m_axi_arburst;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arregion;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_DDR4_MEM01_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awburst,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arburst,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awburst,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arburst,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [33:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [1:0]s_axi_awburst;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [33:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [1:0]s_axi_arburst;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [33:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [1:0]m_axi_awburst;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [33:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [1:0]m_axi_arburst;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_M00_AXI_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awburst,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arburst,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awburst,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arburst,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [38:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [1:0]s_axi_awburst;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [38:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [1:0]s_axi_arburst;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [38:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [1:0]m_axi_awburst;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [38:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [1:0]m_axi_arburst;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S01_AXI_0
   (aclk,
    aresetn,
    s_axi_awid,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awburst,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awregion,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bid,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_arid,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arburst,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arregion,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rid,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awid,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awburst,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awregion,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bid,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_arid,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arburst,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arregion,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rid,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [3:0]s_axi_awid;
  input [38:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [1:0]s_axi_awburst;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awregion;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [3:0]s_axi_bid;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [3:0]s_axi_arid;
  input [38:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [1:0]s_axi_arburst;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arregion;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [3:0]s_axi_rid;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [3:0]m_axi_awid;
  output [38:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [1:0]m_axi_awburst;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awregion;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [3:0]m_axi_bid;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [3:0]m_axi_arid;
  output [38:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [1:0]m_axi_arburst;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arregion;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [3:0]m_axi_rid;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S02_AXI_0
   (aclk,
    aresetn,
    s_axi_awid,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awburst,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awregion,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bid,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_arid,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arburst,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arregion,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rid,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awid,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awburst,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awregion,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bid,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_arid,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arburst,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arregion,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rid,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [3:0]s_axi_awid;
  input [38:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [1:0]s_axi_awburst;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awregion;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [3:0]s_axi_bid;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [3:0]s_axi_arid;
  input [38:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [1:0]s_axi_arburst;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arregion;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [3:0]s_axi_rid;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [3:0]m_axi_awid;
  output [38:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [1:0]m_axi_awburst;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awregion;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [3:0]m_axi_bid;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [3:0]m_axi_arid;
  output [38:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [1:0]m_axi_arburst;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arregion;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [3:0]m_axi_rid;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S04_AXI_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awregion,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arregion,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awregion,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arregion,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [38:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awregion;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [38:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arregion;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [38:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awregion;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [38:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arregion;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S05_AXI_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awregion,
    s_axi_awqos,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arregion,
    s_axi_arqos,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awregion,
    m_axi_awqos,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arregion,
    m_axi_arqos,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [38:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awregion;
  input [3:0]s_axi_awqos;
  input s_axi_awvalid;
  output s_axi_awready;
  input [511:0]s_axi_wdata;
  input [63:0]s_axi_wstrb;
  input s_axi_wlast;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [38:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arregion;
  input [3:0]s_axi_arqos;
  input s_axi_arvalid;
  output s_axi_arready;
  output [511:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rlast;
  output s_axi_rvalid;
  input s_axi_rready;
  output [38:0]m_axi_awaddr;
  output [7:0]m_axi_awlen;
  output [0:0]m_axi_awlock;
  output [3:0]m_axi_awcache;
  output [2:0]m_axi_awprot;
  output [3:0]m_axi_awregion;
  output [3:0]m_axi_awqos;
  output m_axi_awvalid;
  input m_axi_awready;
  output [511:0]m_axi_wdata;
  output [63:0]m_axi_wstrb;
  output m_axi_wlast;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [38:0]m_axi_araddr;
  output [7:0]m_axi_arlen;
  output [0:0]m_axi_arlock;
  output [3:0]m_axi_arcache;
  output [2:0]m_axi_arprot;
  output [3:0]m_axi_arregion;
  output [3:0]m_axi_arqos;
  output m_axi_arvalid;
  input m_axi_arready;
  input [511:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rlast;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ctrl_DDR4_MEM01_0
   (aclk,
    aresetn,
    s_axi_awaddr,
    s_axi_awprot,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bresp,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_araddr,
    s_axi_arprot,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awaddr,
    m_axi_awprot,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bresp,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_araddr,
    m_axi_arprot,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [31:0]s_axi_awaddr;
  input [2:0]s_axi_awprot;
  input s_axi_awvalid;
  output s_axi_awready;
  input [31:0]s_axi_wdata;
  input [3:0]s_axi_wstrb;
  input s_axi_wvalid;
  output s_axi_wready;
  output [1:0]s_axi_bresp;
  output s_axi_bvalid;
  input s_axi_bready;
  input [31:0]s_axi_araddr;
  input [2:0]s_axi_arprot;
  input s_axi_arvalid;
  output s_axi_arready;
  output [31:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output s_axi_rvalid;
  input s_axi_rready;
  output [31:0]m_axi_awaddr;
  output [2:0]m_axi_awprot;
  output m_axi_awvalid;
  input m_axi_awready;
  output [31:0]m_axi_wdata;
  output [3:0]m_axi_wstrb;
  output m_axi_wvalid;
  input m_axi_wready;
  input [1:0]m_axi_bresp;
  input m_axi_bvalid;
  output m_axi_bready;
  output [31:0]m_axi_araddr;
  output [2:0]m_axi_arprot;
  output m_axi_arvalid;
  input m_axi_arready;
  input [31:0]m_axi_rdata;
  input [1:0]m_axi_rresp;
  input m_axi_rvalid;
  output m_axi_rready;


endmodule

(* X_CORE_INFO = "clk_vip_v1_0_2_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_clk_DDR4_MEM01_0
   (clk_in,
    clk_out);
  input clk_in;
  output clk_out;


endmodule

(* X_CORE_INFO = "rst_vip_v1_0_4_top,Vivado 2020.1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_rst_DDR4_MEM01_0
   (rst_in,
    rst_out);
  input rst_in;
  output rst_out;


endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_interconnect_imp_6HQKUY
   (M00_AXI_awaddr,
    M00_AXI_awlen,
    M00_AXI_awburst,
    M00_AXI_awlock,
    M00_AXI_awcache,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awvalid,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    M00_AXI_wlast,
    M00_AXI_wvalid,
    M00_AXI_bready,
    M00_AXI_araddr,
    M00_AXI_arlen,
    M00_AXI_arburst,
    M00_AXI_arlock,
    M00_AXI_arcache,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arvalid,
    M00_AXI_rready,
    \bbstub_m_axi_awaddr[38] ,
    \bbstub_m_axi_awlen[7] ,
    M00_AXI_awsize,
    \bbstub_m_axi_awburst[1] ,
    \bbstub_m_axi_awlock[0] ,
    \bbstub_m_axi_awcache[3] ,
    \bbstub_m_axi_awprot[2] ,
    M00_AXI_awregion,
    \bbstub_m_axi_awqos[3] ,
    bbstub_m_axi_awvalid,
    \bbstub_m_axi_wdata[511] ,
    \bbstub_m_axi_wstrb[63] ,
    bbstub_m_axi_wlast,
    bbstub_m_axi_wvalid,
    bbstub_m_axi_bready,
    \bbstub_m_axi_araddr[38] ,
    \bbstub_m_axi_arlen[7] ,
    M00_AXI_arsize,
    \bbstub_m_axi_arburst[1] ,
    \bbstub_m_axi_arlock[0] ,
    \bbstub_m_axi_arcache[3] ,
    \bbstub_m_axi_arprot[2] ,
    M00_AXI_arregion,
    \bbstub_m_axi_arqos[3] ,
    bbstub_m_axi_arvalid,
    bbstub_m_axi_rready,
    S01_AXI_awready,
    S01_AXI_wready,
    S01_AXI_bid,
    S01_AXI_bresp,
    S01_AXI_bvalid,
    S01_AXI_arready,
    S01_AXI_rid,
    S01_AXI_rdata,
    S01_AXI_rresp,
    S01_AXI_rlast,
    S01_AXI_rvalid,
    S02_AXI_awready,
    S02_AXI_wready,
    S02_AXI_bid,
    S02_AXI_bresp,
    S02_AXI_bvalid,
    S02_AXI_arready,
    S02_AXI_rid,
    S02_AXI_rdata,
    S02_AXI_rresp,
    S02_AXI_rlast,
    S02_AXI_rvalid,
    S04_AXI_awready,
    S04_AXI_wready,
    S04_AXI_bresp,
    S04_AXI_bvalid,
    S04_AXI_arready,
    S04_AXI_rdata,
    S04_AXI_rresp,
    S04_AXI_rlast,
    S04_AXI_rvalid,
    S05_AXI_awready,
    S05_AXI_wready,
    S05_AXI_bresp,
    S05_AXI_bvalid,
    S05_AXI_arready,
    S05_AXI_rdata,
    S05_AXI_rresp,
    S05_AXI_rlast,
    S05_AXI_rvalid,
    ddr4_mem01_ui_clk,
    aclk,
    interconnect_aresetn,
    S_AXI_awready,
    S_AXI_wready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_arready,
    S_AXI_rdata,
    S_AXI_rresp,
    S_AXI_rlast,
    S_AXI_rvalid,
    aclk2,
    interconnect_aresetn1,
    M00_AXI_awready,
    M00_AXI_wready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_arready,
    M00_AXI_rdata,
    M00_AXI_rresp,
    M00_AXI_rlast,
    M00_AXI_rvalid,
    S01_AXI_awid,
    S01_AXI_awaddr,
    S01_AXI_awlen,
    S01_AXI_awburst,
    S01_AXI_awlock,
    S01_AXI_awcache,
    S01_AXI_awprot,
    S01_AXI_awregion,
    S01_AXI_awqos,
    S01_AXI_awvalid,
    S01_AXI_wdata,
    S01_AXI_wstrb,
    S01_AXI_wlast,
    S01_AXI_wvalid,
    S01_AXI_bready,
    S01_AXI_arid,
    S01_AXI_araddr,
    S01_AXI_arlen,
    S01_AXI_arburst,
    S01_AXI_arlock,
    S01_AXI_arcache,
    S01_AXI_arprot,
    S01_AXI_arregion,
    S01_AXI_arqos,
    S01_AXI_arvalid,
    S01_AXI_rready,
    S02_AXI_awid,
    S02_AXI_awaddr,
    S02_AXI_awlen,
    S02_AXI_awburst,
    S02_AXI_awlock,
    S02_AXI_awcache,
    S02_AXI_awprot,
    S02_AXI_awregion,
    S02_AXI_awqos,
    S02_AXI_awvalid,
    S02_AXI_wdata,
    S02_AXI_wstrb,
    S02_AXI_wlast,
    S02_AXI_wvalid,
    S02_AXI_bready,
    S02_AXI_arid,
    S02_AXI_araddr,
    S02_AXI_arlen,
    S02_AXI_arburst,
    S02_AXI_arlock,
    S02_AXI_arcache,
    S02_AXI_arprot,
    S02_AXI_arregion,
    S02_AXI_arqos,
    S02_AXI_arvalid,
    S02_AXI_rready,
    S04_AXI_awaddr,
    S04_AXI_awlen,
    S04_AXI_awlock,
    S04_AXI_awcache,
    S04_AXI_awprot,
    S04_AXI_awregion,
    S04_AXI_awqos,
    S04_AXI_awvalid,
    S04_AXI_wdata,
    S04_AXI_wstrb,
    S04_AXI_wlast,
    S04_AXI_wvalid,
    S04_AXI_bready,
    S04_AXI_araddr,
    S04_AXI_arlen,
    S04_AXI_arlock,
    S04_AXI_arcache,
    S04_AXI_arprot,
    S04_AXI_arregion,
    S04_AXI_arqos,
    S04_AXI_arvalid,
    S04_AXI_rready,
    S05_AXI_awaddr,
    S05_AXI_awlen,
    S05_AXI_awlock,
    S05_AXI_awcache,
    S05_AXI_awprot,
    S05_AXI_awregion,
    S05_AXI_awqos,
    S05_AXI_awvalid,
    S05_AXI_wdata,
    S05_AXI_wstrb,
    S05_AXI_wlast,
    S05_AXI_wvalid,
    S05_AXI_bready,
    S05_AXI_araddr,
    S05_AXI_arlen,
    S05_AXI_arlock,
    S05_AXI_arcache,
    S05_AXI_arprot,
    S05_AXI_arregion,
    S05_AXI_arqos,
    S05_AXI_arvalid,
    S05_AXI_rready);
  output [33:0]M00_AXI_awaddr;
  output [7:0]M00_AXI_awlen;
  output [1:0]M00_AXI_awburst;
  output [0:0]M00_AXI_awlock;
  output [3:0]M00_AXI_awcache;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awqos;
  output M00_AXI_awvalid;
  output [511:0]M00_AXI_wdata;
  output [63:0]M00_AXI_wstrb;
  output M00_AXI_wlast;
  output M00_AXI_wvalid;
  output M00_AXI_bready;
  output [33:0]M00_AXI_araddr;
  output [7:0]M00_AXI_arlen;
  output [1:0]M00_AXI_arburst;
  output [0:0]M00_AXI_arlock;
  output [3:0]M00_AXI_arcache;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arqos;
  output M00_AXI_arvalid;
  output M00_AXI_rready;
  output [38:0]\bbstub_m_axi_awaddr[38] ;
  output [7:0]\bbstub_m_axi_awlen[7] ;
  output [2:0]M00_AXI_awsize;
  output [1:0]\bbstub_m_axi_awburst[1] ;
  output [0:0]\bbstub_m_axi_awlock[0] ;
  output [3:0]\bbstub_m_axi_awcache[3] ;
  output [2:0]\bbstub_m_axi_awprot[2] ;
  output [3:0]M00_AXI_awregion;
  output [3:0]\bbstub_m_axi_awqos[3] ;
  output bbstub_m_axi_awvalid;
  output [511:0]\bbstub_m_axi_wdata[511] ;
  output [63:0]\bbstub_m_axi_wstrb[63] ;
  output bbstub_m_axi_wlast;
  output bbstub_m_axi_wvalid;
  output bbstub_m_axi_bready;
  output [38:0]\bbstub_m_axi_araddr[38] ;
  output [7:0]\bbstub_m_axi_arlen[7] ;
  output [2:0]M00_AXI_arsize;
  output [1:0]\bbstub_m_axi_arburst[1] ;
  output [0:0]\bbstub_m_axi_arlock[0] ;
  output [3:0]\bbstub_m_axi_arcache[3] ;
  output [2:0]\bbstub_m_axi_arprot[2] ;
  output [3:0]M00_AXI_arregion;
  output [3:0]\bbstub_m_axi_arqos[3] ;
  output bbstub_m_axi_arvalid;
  output bbstub_m_axi_rready;
  output S01_AXI_awready;
  output S01_AXI_wready;
  output [3:0]S01_AXI_bid;
  output [1:0]S01_AXI_bresp;
  output S01_AXI_bvalid;
  output S01_AXI_arready;
  output [3:0]S01_AXI_rid;
  output [511:0]S01_AXI_rdata;
  output [1:0]S01_AXI_rresp;
  output S01_AXI_rlast;
  output S01_AXI_rvalid;
  output S02_AXI_awready;
  output S02_AXI_wready;
  output [3:0]S02_AXI_bid;
  output [1:0]S02_AXI_bresp;
  output S02_AXI_bvalid;
  output S02_AXI_arready;
  output [3:0]S02_AXI_rid;
  output [511:0]S02_AXI_rdata;
  output [1:0]S02_AXI_rresp;
  output S02_AXI_rlast;
  output S02_AXI_rvalid;
  output S04_AXI_awready;
  output S04_AXI_wready;
  output [1:0]S04_AXI_bresp;
  output S04_AXI_bvalid;
  output S04_AXI_arready;
  output [511:0]S04_AXI_rdata;
  output [1:0]S04_AXI_rresp;
  output S04_AXI_rlast;
  output S04_AXI_rvalid;
  output S05_AXI_awready;
  output S05_AXI_wready;
  output [1:0]S05_AXI_bresp;
  output S05_AXI_bvalid;
  output S05_AXI_arready;
  output [511:0]S05_AXI_rdata;
  output [1:0]S05_AXI_rresp;
  output S05_AXI_rlast;
  output S05_AXI_rvalid;
  input ddr4_mem01_ui_clk;
  input aclk;
  input [0:0]interconnect_aresetn;
  input S_AXI_awready;
  input S_AXI_wready;
  input [1:0]S_AXI_bresp;
  input S_AXI_bvalid;
  input S_AXI_arready;
  input [511:0]S_AXI_rdata;
  input [1:0]S_AXI_rresp;
  input S_AXI_rlast;
  input S_AXI_rvalid;
  input aclk2;
  input [0:0]interconnect_aresetn1;
  input M00_AXI_awready;
  input M00_AXI_wready;
  input [1:0]M00_AXI_bresp;
  input M00_AXI_bvalid;
  input M00_AXI_arready;
  input [511:0]M00_AXI_rdata;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rlast;
  input M00_AXI_rvalid;
  input [3:0]S01_AXI_awid;
  input [38:0]S01_AXI_awaddr;
  input [7:0]S01_AXI_awlen;
  input [1:0]S01_AXI_awburst;
  input [0:0]S01_AXI_awlock;
  input [3:0]S01_AXI_awcache;
  input [2:0]S01_AXI_awprot;
  input [3:0]S01_AXI_awregion;
  input [3:0]S01_AXI_awqos;
  input S01_AXI_awvalid;
  input [511:0]S01_AXI_wdata;
  input [63:0]S01_AXI_wstrb;
  input S01_AXI_wlast;
  input S01_AXI_wvalid;
  input S01_AXI_bready;
  input [3:0]S01_AXI_arid;
  input [38:0]S01_AXI_araddr;
  input [7:0]S01_AXI_arlen;
  input [1:0]S01_AXI_arburst;
  input [0:0]S01_AXI_arlock;
  input [3:0]S01_AXI_arcache;
  input [2:0]S01_AXI_arprot;
  input [3:0]S01_AXI_arregion;
  input [3:0]S01_AXI_arqos;
  input S01_AXI_arvalid;
  input S01_AXI_rready;
  input [3:0]S02_AXI_awid;
  input [38:0]S02_AXI_awaddr;
  input [7:0]S02_AXI_awlen;
  input [1:0]S02_AXI_awburst;
  input [0:0]S02_AXI_awlock;
  input [3:0]S02_AXI_awcache;
  input [2:0]S02_AXI_awprot;
  input [3:0]S02_AXI_awregion;
  input [3:0]S02_AXI_awqos;
  input S02_AXI_awvalid;
  input [511:0]S02_AXI_wdata;
  input [63:0]S02_AXI_wstrb;
  input S02_AXI_wlast;
  input S02_AXI_wvalid;
  input S02_AXI_bready;
  input [3:0]S02_AXI_arid;
  input [38:0]S02_AXI_araddr;
  input [7:0]S02_AXI_arlen;
  input [1:0]S02_AXI_arburst;
  input [0:0]S02_AXI_arlock;
  input [3:0]S02_AXI_arcache;
  input [2:0]S02_AXI_arprot;
  input [3:0]S02_AXI_arregion;
  input [3:0]S02_AXI_arqos;
  input S02_AXI_arvalid;
  input S02_AXI_rready;
  input [38:0]S04_AXI_awaddr;
  input [7:0]S04_AXI_awlen;
  input [0:0]S04_AXI_awlock;
  input [3:0]S04_AXI_awcache;
  input [2:0]S04_AXI_awprot;
  input [3:0]S04_AXI_awregion;
  input [3:0]S04_AXI_awqos;
  input S04_AXI_awvalid;
  input [511:0]S04_AXI_wdata;
  input [63:0]S04_AXI_wstrb;
  input S04_AXI_wlast;
  input S04_AXI_wvalid;
  input S04_AXI_bready;
  input [38:0]S04_AXI_araddr;
  input [7:0]S04_AXI_arlen;
  input [0:0]S04_AXI_arlock;
  input [3:0]S04_AXI_arcache;
  input [2:0]S04_AXI_arprot;
  input [3:0]S04_AXI_arregion;
  input [3:0]S04_AXI_arqos;
  input S04_AXI_arvalid;
  input S04_AXI_rready;
  input [38:0]S05_AXI_awaddr;
  input [7:0]S05_AXI_awlen;
  input [0:0]S05_AXI_awlock;
  input [3:0]S05_AXI_awcache;
  input [2:0]S05_AXI_awprot;
  input [3:0]S05_AXI_awregion;
  input [3:0]S05_AXI_awqos;
  input S05_AXI_awvalid;
  input [511:0]S05_AXI_wdata;
  input [63:0]S05_AXI_wstrb;
  input S05_AXI_wlast;
  input S05_AXI_wvalid;
  input S05_AXI_bready;
  input [38:0]S05_AXI_araddr;
  input [7:0]S05_AXI_arlen;
  input [0:0]S05_AXI_arlock;
  input [3:0]S05_AXI_arcache;
  input [2:0]S05_AXI_arprot;
  input [3:0]S05_AXI_arregion;
  input [3:0]S05_AXI_arqos;
  input S05_AXI_arvalid;
  input S05_AXI_rready;

  wire [33:0]M00_AXI_araddr;
  wire [1:0]M00_AXI_arburst;
  wire [3:0]M00_AXI_arcache;
  wire [7:0]M00_AXI_arlen;
  wire [0:0]M00_AXI_arlock;
  wire [2:0]M00_AXI_arprot;
  wire [3:0]M00_AXI_arqos;
  wire M00_AXI_arready;
  wire [3:0]M00_AXI_arregion;
  wire [2:0]M00_AXI_arsize;
  wire M00_AXI_arvalid;
  wire [33:0]M00_AXI_awaddr;
  wire [1:0]M00_AXI_awburst;
  wire [3:0]M00_AXI_awcache;
  wire [7:0]M00_AXI_awlen;
  wire [0:0]M00_AXI_awlock;
  wire [2:0]M00_AXI_awprot;
  wire [3:0]M00_AXI_awqos;
  wire M00_AXI_awready;
  wire [3:0]M00_AXI_awregion;
  wire [2:0]M00_AXI_awsize;
  wire M00_AXI_awvalid;
  wire M00_AXI_bready;
  wire [1:0]M00_AXI_bresp;
  wire M00_AXI_bvalid;
  wire [511:0]M00_AXI_rdata;
  wire M00_AXI_rlast;
  wire M00_AXI_rready;
  wire [1:0]M00_AXI_rresp;
  wire M00_AXI_rvalid;
  wire [511:0]M00_AXI_wdata;
  wire M00_AXI_wlast;
  wire M00_AXI_wready;
  wire [63:0]M00_AXI_wstrb;
  wire M00_AXI_wvalid;
  wire [38:0]S01_AXI_araddr;
  wire [1:0]S01_AXI_arburst;
  wire [3:0]S01_AXI_arcache;
  wire [3:0]S01_AXI_arid;
  wire [7:0]S01_AXI_arlen;
  wire [0:0]S01_AXI_arlock;
  wire [2:0]S01_AXI_arprot;
  wire [3:0]S01_AXI_arqos;
  wire S01_AXI_arready;
  wire [3:0]S01_AXI_arregion;
  wire S01_AXI_arvalid;
  wire [38:0]S01_AXI_awaddr;
  wire [1:0]S01_AXI_awburst;
  wire [3:0]S01_AXI_awcache;
  wire [3:0]S01_AXI_awid;
  wire [7:0]S01_AXI_awlen;
  wire [0:0]S01_AXI_awlock;
  wire [2:0]S01_AXI_awprot;
  wire [3:0]S01_AXI_awqos;
  wire S01_AXI_awready;
  wire [3:0]S01_AXI_awregion;
  wire S01_AXI_awvalid;
  wire [3:0]S01_AXI_bid;
  wire S01_AXI_bready;
  wire [1:0]S01_AXI_bresp;
  wire S01_AXI_bvalid;
  wire [511:0]S01_AXI_rdata;
  wire [3:0]S01_AXI_rid;
  wire S01_AXI_rlast;
  wire S01_AXI_rready;
  wire [1:0]S01_AXI_rresp;
  wire S01_AXI_rvalid;
  wire [511:0]S01_AXI_wdata;
  wire S01_AXI_wlast;
  wire S01_AXI_wready;
  wire [63:0]S01_AXI_wstrb;
  wire S01_AXI_wvalid;
  wire [38:0]S02_AXI_araddr;
  wire [1:0]S02_AXI_arburst;
  wire [3:0]S02_AXI_arcache;
  wire [3:0]S02_AXI_arid;
  wire [7:0]S02_AXI_arlen;
  wire [0:0]S02_AXI_arlock;
  wire [2:0]S02_AXI_arprot;
  wire [3:0]S02_AXI_arqos;
  wire S02_AXI_arready;
  wire [3:0]S02_AXI_arregion;
  wire S02_AXI_arvalid;
  wire [38:0]S02_AXI_awaddr;
  wire [1:0]S02_AXI_awburst;
  wire [3:0]S02_AXI_awcache;
  wire [3:0]S02_AXI_awid;
  wire [7:0]S02_AXI_awlen;
  wire [0:0]S02_AXI_awlock;
  wire [2:0]S02_AXI_awprot;
  wire [3:0]S02_AXI_awqos;
  wire S02_AXI_awready;
  wire [3:0]S02_AXI_awregion;
  wire S02_AXI_awvalid;
  wire [3:0]S02_AXI_bid;
  wire S02_AXI_bready;
  wire [1:0]S02_AXI_bresp;
  wire S02_AXI_bvalid;
  wire [511:0]S02_AXI_rdata;
  wire [3:0]S02_AXI_rid;
  wire S02_AXI_rlast;
  wire S02_AXI_rready;
  wire [1:0]S02_AXI_rresp;
  wire S02_AXI_rvalid;
  wire [511:0]S02_AXI_wdata;
  wire S02_AXI_wlast;
  wire S02_AXI_wready;
  wire [63:0]S02_AXI_wstrb;
  wire S02_AXI_wvalid;
  wire [38:0]S04_AXI_araddr;
  wire [3:0]S04_AXI_arcache;
  wire [7:0]S04_AXI_arlen;
  wire [0:0]S04_AXI_arlock;
  wire [2:0]S04_AXI_arprot;
  wire [3:0]S04_AXI_arqos;
  wire S04_AXI_arready;
  wire [3:0]S04_AXI_arregion;
  wire S04_AXI_arvalid;
  wire [38:0]S04_AXI_awaddr;
  wire [3:0]S04_AXI_awcache;
  wire [7:0]S04_AXI_awlen;
  wire [0:0]S04_AXI_awlock;
  wire [2:0]S04_AXI_awprot;
  wire [3:0]S04_AXI_awqos;
  wire S04_AXI_awready;
  wire [3:0]S04_AXI_awregion;
  wire S04_AXI_awvalid;
  wire S04_AXI_bready;
  wire [1:0]S04_AXI_bresp;
  wire S04_AXI_bvalid;
  wire [511:0]S04_AXI_rdata;
  wire S04_AXI_rlast;
  wire S04_AXI_rready;
  wire [1:0]S04_AXI_rresp;
  wire S04_AXI_rvalid;
  wire [511:0]S04_AXI_wdata;
  wire S04_AXI_wlast;
  wire S04_AXI_wready;
  wire [63:0]S04_AXI_wstrb;
  wire S04_AXI_wvalid;
  wire [38:0]S05_AXI_araddr;
  wire [3:0]S05_AXI_arcache;
  wire [7:0]S05_AXI_arlen;
  wire [0:0]S05_AXI_arlock;
  wire [2:0]S05_AXI_arprot;
  wire [3:0]S05_AXI_arqos;
  wire S05_AXI_arready;
  wire [3:0]S05_AXI_arregion;
  wire S05_AXI_arvalid;
  wire [38:0]S05_AXI_awaddr;
  wire [3:0]S05_AXI_awcache;
  wire [7:0]S05_AXI_awlen;
  wire [0:0]S05_AXI_awlock;
  wire [2:0]S05_AXI_awprot;
  wire [3:0]S05_AXI_awqos;
  wire S05_AXI_awready;
  wire [3:0]S05_AXI_awregion;
  wire S05_AXI_awvalid;
  wire S05_AXI_bready;
  wire [1:0]S05_AXI_bresp;
  wire S05_AXI_bvalid;
  wire [511:0]S05_AXI_rdata;
  wire S05_AXI_rlast;
  wire S05_AXI_rready;
  wire [1:0]S05_AXI_rresp;
  wire S05_AXI_rvalid;
  wire [511:0]S05_AXI_wdata;
  wire S05_AXI_wlast;
  wire S05_AXI_wready;
  wire [63:0]S05_AXI_wstrb;
  wire S05_AXI_wvalid;
  wire S_AXI_arready;
  wire S_AXI_awready;
  wire [1:0]S_AXI_bresp;
  wire S_AXI_bvalid;
  wire [511:0]S_AXI_rdata;
  wire S_AXI_rlast;
  wire [1:0]S_AXI_rresp;
  wire S_AXI_rvalid;
  wire S_AXI_wready;
  wire aclk;
  wire aclk2;
  wire [38:0]\bbstub_m_axi_araddr[38] ;
  wire [1:0]\bbstub_m_axi_arburst[1] ;
  wire [3:0]\bbstub_m_axi_arcache[3] ;
  wire [7:0]\bbstub_m_axi_arlen[7] ;
  wire [0:0]\bbstub_m_axi_arlock[0] ;
  wire [2:0]\bbstub_m_axi_arprot[2] ;
  wire [3:0]\bbstub_m_axi_arqos[3] ;
  wire bbstub_m_axi_arvalid;
  wire [38:0]\bbstub_m_axi_awaddr[38] ;
  wire [1:0]\bbstub_m_axi_awburst[1] ;
  wire [3:0]\bbstub_m_axi_awcache[3] ;
  wire [7:0]\bbstub_m_axi_awlen[7] ;
  wire [0:0]\bbstub_m_axi_awlock[0] ;
  wire [2:0]\bbstub_m_axi_awprot[2] ;
  wire [3:0]\bbstub_m_axi_awqos[3] ;
  wire bbstub_m_axi_awvalid;
  wire bbstub_m_axi_bready;
  wire bbstub_m_axi_rready;
  wire [511:0]\bbstub_m_axi_wdata[511] ;
  wire bbstub_m_axi_wlast;
  wire [63:0]\bbstub_m_axi_wstrb[63] ;
  wire bbstub_m_axi_wvalid;
  wire ddr4_mem01_ui_clk;
  wire [38:0]interconnect_M00_AXI_MEM00_M00_AXI_ARADDR;
  wire [1:0]interconnect_M00_AXI_MEM00_M00_AXI_ARBURST;
  wire [3:0]interconnect_M00_AXI_MEM00_M00_AXI_ARCACHE;
  wire [7:0]interconnect_M00_AXI_MEM00_M00_AXI_ARLEN;
  wire interconnect_M00_AXI_MEM00_M00_AXI_ARLOCK;
  wire [2:0]interconnect_M00_AXI_MEM00_M00_AXI_ARPROT;
  wire [3:0]interconnect_M00_AXI_MEM00_M00_AXI_ARQOS;
  wire interconnect_M00_AXI_MEM00_M00_AXI_ARREADY;
  wire interconnect_M00_AXI_MEM00_M00_AXI_ARVALID;
  wire [38:0]interconnect_M00_AXI_MEM00_M00_AXI_AWADDR;
  wire [1:0]interconnect_M00_AXI_MEM00_M00_AXI_AWBURST;
  wire [3:0]interconnect_M00_AXI_MEM00_M00_AXI_AWCACHE;
  wire [7:0]interconnect_M00_AXI_MEM00_M00_AXI_AWLEN;
  wire interconnect_M00_AXI_MEM00_M00_AXI_AWLOCK;
  wire [2:0]interconnect_M00_AXI_MEM00_M00_AXI_AWPROT;
  wire [3:0]interconnect_M00_AXI_MEM00_M00_AXI_AWQOS;
  wire interconnect_M00_AXI_MEM00_M00_AXI_AWREADY;
  wire interconnect_M00_AXI_MEM00_M00_AXI_AWVALID;
  wire interconnect_M00_AXI_MEM00_M00_AXI_BREADY;
  wire [1:0]interconnect_M00_AXI_MEM00_M00_AXI_BRESP;
  wire interconnect_M00_AXI_MEM00_M00_AXI_BVALID;
  wire [511:0]interconnect_M00_AXI_MEM00_M00_AXI_RDATA;
  wire interconnect_M00_AXI_MEM00_M00_AXI_RLAST;
  wire interconnect_M00_AXI_MEM00_M00_AXI_RREADY;
  wire [1:0]interconnect_M00_AXI_MEM00_M00_AXI_RRESP;
  wire interconnect_M00_AXI_MEM00_M00_AXI_RVALID;
  wire [511:0]interconnect_M00_AXI_MEM00_M00_AXI_WDATA;
  wire interconnect_M00_AXI_MEM00_M00_AXI_WLAST;
  wire interconnect_M00_AXI_MEM00_M00_AXI_WREADY;
  wire [63:0]interconnect_M00_AXI_MEM00_M00_AXI_WSTRB;
  wire interconnect_M00_AXI_MEM00_M00_AXI_WVALID;
  wire [38:0]interconnect_S04_AXI_M00_AXI_ARADDR;
  wire [1:0]interconnect_S04_AXI_M00_AXI_ARBURST;
  wire [3:0]interconnect_S04_AXI_M00_AXI_ARCACHE;
  wire [1:0]interconnect_S04_AXI_M00_AXI_ARID;
  wire [7:0]interconnect_S04_AXI_M00_AXI_ARLEN;
  wire interconnect_S04_AXI_M00_AXI_ARLOCK;
  wire [2:0]interconnect_S04_AXI_M00_AXI_ARPROT;
  wire [3:0]interconnect_S04_AXI_M00_AXI_ARQOS;
  wire interconnect_S04_AXI_M00_AXI_ARREADY;
  wire [2:0]interconnect_S04_AXI_M00_AXI_ARSIZE;
  wire [113:0]interconnect_S04_AXI_M00_AXI_ARUSER;
  wire interconnect_S04_AXI_M00_AXI_ARVALID;
  wire [38:0]interconnect_S04_AXI_M00_AXI_AWADDR;
  wire [1:0]interconnect_S04_AXI_M00_AXI_AWBURST;
  wire [3:0]interconnect_S04_AXI_M00_AXI_AWCACHE;
  wire [1:0]interconnect_S04_AXI_M00_AXI_AWID;
  wire [7:0]interconnect_S04_AXI_M00_AXI_AWLEN;
  wire interconnect_S04_AXI_M00_AXI_AWLOCK;
  wire [2:0]interconnect_S04_AXI_M00_AXI_AWPROT;
  wire [3:0]interconnect_S04_AXI_M00_AXI_AWQOS;
  wire interconnect_S04_AXI_M00_AXI_AWREADY;
  wire [2:0]interconnect_S04_AXI_M00_AXI_AWSIZE;
  wire [113:0]interconnect_S04_AXI_M00_AXI_AWUSER;
  wire interconnect_S04_AXI_M00_AXI_AWVALID;
  wire [1:0]interconnect_S04_AXI_M00_AXI_BID;
  wire interconnect_S04_AXI_M00_AXI_BREADY;
  wire [1:0]interconnect_S04_AXI_M00_AXI_BRESP;
  wire [113:0]interconnect_S04_AXI_M00_AXI_BUSER;
  wire interconnect_S04_AXI_M00_AXI_BVALID;
  wire [511:0]interconnect_S04_AXI_M00_AXI_RDATA;
  wire [1:0]interconnect_S04_AXI_M00_AXI_RID;
  wire interconnect_S04_AXI_M00_AXI_RLAST;
  wire interconnect_S04_AXI_M00_AXI_RREADY;
  wire [1:0]interconnect_S04_AXI_M00_AXI_RRESP;
  wire [13:0]interconnect_S04_AXI_M00_AXI_RUSER;
  wire interconnect_S04_AXI_M00_AXI_RVALID;
  wire [511:0]interconnect_S04_AXI_M00_AXI_WDATA;
  wire interconnect_S04_AXI_M00_AXI_WLAST;
  wire interconnect_S04_AXI_M00_AXI_WREADY;
  wire [63:0]interconnect_S04_AXI_M00_AXI_WSTRB;
  wire [13:0]interconnect_S04_AXI_M00_AXI_WUSER;
  wire interconnect_S04_AXI_M00_AXI_WVALID;
  wire [38:0]interconnect_S04_AXI_M01_AXI_ARADDR;
  wire [1:0]interconnect_S04_AXI_M01_AXI_ARBURST;
  wire [3:0]interconnect_S04_AXI_M01_AXI_ARCACHE;
  wire [1:0]interconnect_S04_AXI_M01_AXI_ARID;
  wire [7:0]interconnect_S04_AXI_M01_AXI_ARLEN;
  wire interconnect_S04_AXI_M01_AXI_ARLOCK;
  wire [2:0]interconnect_S04_AXI_M01_AXI_ARPROT;
  wire [3:0]interconnect_S04_AXI_M01_AXI_ARQOS;
  wire interconnect_S04_AXI_M01_AXI_ARREADY;
  wire [2:0]interconnect_S04_AXI_M01_AXI_ARSIZE;
  wire [113:0]interconnect_S04_AXI_M01_AXI_ARUSER;
  wire interconnect_S04_AXI_M01_AXI_ARVALID;
  wire [38:0]interconnect_S04_AXI_M01_AXI_AWADDR;
  wire [1:0]interconnect_S04_AXI_M01_AXI_AWBURST;
  wire [3:0]interconnect_S04_AXI_M01_AXI_AWCACHE;
  wire [1:0]interconnect_S04_AXI_M01_AXI_AWID;
  wire [7:0]interconnect_S04_AXI_M01_AXI_AWLEN;
  wire interconnect_S04_AXI_M01_AXI_AWLOCK;
  wire [2:0]interconnect_S04_AXI_M01_AXI_AWPROT;
  wire [3:0]interconnect_S04_AXI_M01_AXI_AWQOS;
  wire interconnect_S04_AXI_M01_AXI_AWREADY;
  wire [2:0]interconnect_S04_AXI_M01_AXI_AWSIZE;
  wire [113:0]interconnect_S04_AXI_M01_AXI_AWUSER;
  wire interconnect_S04_AXI_M01_AXI_AWVALID;
  wire [1:0]interconnect_S04_AXI_M01_AXI_BID;
  wire interconnect_S04_AXI_M01_AXI_BREADY;
  wire [1:0]interconnect_S04_AXI_M01_AXI_BRESP;
  wire [113:0]interconnect_S04_AXI_M01_AXI_BUSER;
  wire interconnect_S04_AXI_M01_AXI_BVALID;
  wire [511:0]interconnect_S04_AXI_M01_AXI_RDATA;
  wire [1:0]interconnect_S04_AXI_M01_AXI_RID;
  wire interconnect_S04_AXI_M01_AXI_RLAST;
  wire interconnect_S04_AXI_M01_AXI_RREADY;
  wire [1:0]interconnect_S04_AXI_M01_AXI_RRESP;
  wire [13:0]interconnect_S04_AXI_M01_AXI_RUSER;
  wire interconnect_S04_AXI_M01_AXI_RVALID;
  wire [511:0]interconnect_S04_AXI_M01_AXI_WDATA;
  wire interconnect_S04_AXI_M01_AXI_WLAST;
  wire interconnect_S04_AXI_M01_AXI_WREADY;
  wire [63:0]interconnect_S04_AXI_M01_AXI_WSTRB;
  wire [13:0]interconnect_S04_AXI_M01_AXI_WUSER;
  wire interconnect_S04_AXI_M01_AXI_WVALID;
  wire [0:0]interconnect_aresetn;
  wire [0:0]interconnect_aresetn1;
  wire [38:0]vip_M00_AXI_M_AXI_ARADDR;
  wire [1:0]vip_M00_AXI_M_AXI_ARBURST;
  wire [3:0]vip_M00_AXI_M_AXI_ARCACHE;
  wire [7:0]vip_M00_AXI_M_AXI_ARLEN;
  wire vip_M00_AXI_M_AXI_ARLOCK;
  wire [2:0]vip_M00_AXI_M_AXI_ARPROT;
  wire [3:0]vip_M00_AXI_M_AXI_ARQOS;
  wire vip_M00_AXI_M_AXI_ARREADY;
  wire vip_M00_AXI_M_AXI_ARVALID;
  wire [38:0]vip_M00_AXI_M_AXI_AWADDR;
  wire [1:0]vip_M00_AXI_M_AXI_AWBURST;
  wire [3:0]vip_M00_AXI_M_AXI_AWCACHE;
  wire [7:0]vip_M00_AXI_M_AXI_AWLEN;
  wire vip_M00_AXI_M_AXI_AWLOCK;
  wire [2:0]vip_M00_AXI_M_AXI_AWPROT;
  wire [3:0]vip_M00_AXI_M_AXI_AWQOS;
  wire vip_M00_AXI_M_AXI_AWREADY;
  wire vip_M00_AXI_M_AXI_AWVALID;
  wire vip_M00_AXI_M_AXI_BREADY;
  wire [1:0]vip_M00_AXI_M_AXI_BRESP;
  wire vip_M00_AXI_M_AXI_BVALID;
  wire [511:0]vip_M00_AXI_M_AXI_RDATA;
  wire vip_M00_AXI_M_AXI_RLAST;
  wire vip_M00_AXI_M_AXI_RREADY;
  wire [1:0]vip_M00_AXI_M_AXI_RRESP;
  wire vip_M00_AXI_M_AXI_RVALID;
  wire [511:0]vip_M00_AXI_M_AXI_WDATA;
  wire vip_M00_AXI_M_AXI_WLAST;
  wire vip_M00_AXI_M_AXI_WREADY;
  wire [63:0]vip_M00_AXI_M_AXI_WSTRB;
  wire vip_M00_AXI_M_AXI_WVALID;
  wire [38:0]vip_S01_AXI_M_AXI_ARADDR;
  wire [1:0]vip_S01_AXI_M_AXI_ARBURST;
  wire [3:0]vip_S01_AXI_M_AXI_ARCACHE;
  wire [3:0]vip_S01_AXI_M_AXI_ARID;
  wire [7:0]vip_S01_AXI_M_AXI_ARLEN;
  wire vip_S01_AXI_M_AXI_ARLOCK;
  wire [2:0]vip_S01_AXI_M_AXI_ARPROT;
  wire [3:0]vip_S01_AXI_M_AXI_ARQOS;
  wire vip_S01_AXI_M_AXI_ARREADY;
  wire vip_S01_AXI_M_AXI_ARVALID;
  wire [38:0]vip_S01_AXI_M_AXI_AWADDR;
  wire [1:0]vip_S01_AXI_M_AXI_AWBURST;
  wire [3:0]vip_S01_AXI_M_AXI_AWCACHE;
  wire [3:0]vip_S01_AXI_M_AXI_AWID;
  wire [7:0]vip_S01_AXI_M_AXI_AWLEN;
  wire vip_S01_AXI_M_AXI_AWLOCK;
  wire [2:0]vip_S01_AXI_M_AXI_AWPROT;
  wire [3:0]vip_S01_AXI_M_AXI_AWQOS;
  wire vip_S01_AXI_M_AXI_AWREADY;
  wire vip_S01_AXI_M_AXI_AWVALID;
  wire [3:0]vip_S01_AXI_M_AXI_BID;
  wire vip_S01_AXI_M_AXI_BREADY;
  wire [1:0]vip_S01_AXI_M_AXI_BRESP;
  wire vip_S01_AXI_M_AXI_BVALID;
  wire [511:0]vip_S01_AXI_M_AXI_RDATA;
  wire [3:0]vip_S01_AXI_M_AXI_RID;
  wire vip_S01_AXI_M_AXI_RLAST;
  wire vip_S01_AXI_M_AXI_RREADY;
  wire [1:0]vip_S01_AXI_M_AXI_RRESP;
  wire vip_S01_AXI_M_AXI_RVALID;
  wire [511:0]vip_S01_AXI_M_AXI_WDATA;
  wire vip_S01_AXI_M_AXI_WLAST;
  wire vip_S01_AXI_M_AXI_WREADY;
  wire [63:0]vip_S01_AXI_M_AXI_WSTRB;
  wire vip_S01_AXI_M_AXI_WVALID;
  wire [38:0]vip_S02_AXI_M_AXI_ARADDR;
  wire [1:0]vip_S02_AXI_M_AXI_ARBURST;
  wire [3:0]vip_S02_AXI_M_AXI_ARCACHE;
  wire [3:0]vip_S02_AXI_M_AXI_ARID;
  wire [7:0]vip_S02_AXI_M_AXI_ARLEN;
  wire vip_S02_AXI_M_AXI_ARLOCK;
  wire [2:0]vip_S02_AXI_M_AXI_ARPROT;
  wire [3:0]vip_S02_AXI_M_AXI_ARQOS;
  wire vip_S02_AXI_M_AXI_ARREADY;
  wire vip_S02_AXI_M_AXI_ARVALID;
  wire [38:0]vip_S02_AXI_M_AXI_AWADDR;
  wire [1:0]vip_S02_AXI_M_AXI_AWBURST;
  wire [3:0]vip_S02_AXI_M_AXI_AWCACHE;
  wire [3:0]vip_S02_AXI_M_AXI_AWID;
  wire [7:0]vip_S02_AXI_M_AXI_AWLEN;
  wire vip_S02_AXI_M_AXI_AWLOCK;
  wire [2:0]vip_S02_AXI_M_AXI_AWPROT;
  wire [3:0]vip_S02_AXI_M_AXI_AWQOS;
  wire vip_S02_AXI_M_AXI_AWREADY;
  wire vip_S02_AXI_M_AXI_AWVALID;
  wire [3:0]vip_S02_AXI_M_AXI_BID;
  wire vip_S02_AXI_M_AXI_BREADY;
  wire [1:0]vip_S02_AXI_M_AXI_BRESP;
  wire vip_S02_AXI_M_AXI_BVALID;
  wire [511:0]vip_S02_AXI_M_AXI_RDATA;
  wire [3:0]vip_S02_AXI_M_AXI_RID;
  wire vip_S02_AXI_M_AXI_RLAST;
  wire vip_S02_AXI_M_AXI_RREADY;
  wire [1:0]vip_S02_AXI_M_AXI_RRESP;
  wire vip_S02_AXI_M_AXI_RVALID;
  wire [511:0]vip_S02_AXI_M_AXI_WDATA;
  wire vip_S02_AXI_M_AXI_WLAST;
  wire vip_S02_AXI_M_AXI_WREADY;
  wire [63:0]vip_S02_AXI_M_AXI_WSTRB;
  wire vip_S02_AXI_M_AXI_WVALID;
  wire [38:0]vip_S04_AXI_M_AXI_ARADDR;
  wire [3:0]vip_S04_AXI_M_AXI_ARCACHE;
  wire [7:0]vip_S04_AXI_M_AXI_ARLEN;
  wire vip_S04_AXI_M_AXI_ARLOCK;
  wire [2:0]vip_S04_AXI_M_AXI_ARPROT;
  wire [3:0]vip_S04_AXI_M_AXI_ARQOS;
  wire vip_S04_AXI_M_AXI_ARREADY;
  wire vip_S04_AXI_M_AXI_ARVALID;
  wire [38:0]vip_S04_AXI_M_AXI_AWADDR;
  wire [3:0]vip_S04_AXI_M_AXI_AWCACHE;
  wire [7:0]vip_S04_AXI_M_AXI_AWLEN;
  wire vip_S04_AXI_M_AXI_AWLOCK;
  wire [2:0]vip_S04_AXI_M_AXI_AWPROT;
  wire [3:0]vip_S04_AXI_M_AXI_AWQOS;
  wire vip_S04_AXI_M_AXI_AWREADY;
  wire vip_S04_AXI_M_AXI_AWVALID;
  wire vip_S04_AXI_M_AXI_BREADY;
  wire [1:0]vip_S04_AXI_M_AXI_BRESP;
  wire vip_S04_AXI_M_AXI_BVALID;
  wire [511:0]vip_S04_AXI_M_AXI_RDATA;
  wire vip_S04_AXI_M_AXI_RLAST;
  wire vip_S04_AXI_M_AXI_RREADY;
  wire [1:0]vip_S04_AXI_M_AXI_RRESP;
  wire vip_S04_AXI_M_AXI_RVALID;
  wire [511:0]vip_S04_AXI_M_AXI_WDATA;
  wire vip_S04_AXI_M_AXI_WLAST;
  wire vip_S04_AXI_M_AXI_WREADY;
  wire [63:0]vip_S04_AXI_M_AXI_WSTRB;
  wire vip_S04_AXI_M_AXI_WVALID;
  wire [38:0]vip_S05_AXI_M_AXI_ARADDR;
  wire [3:0]vip_S05_AXI_M_AXI_ARCACHE;
  wire [7:0]vip_S05_AXI_M_AXI_ARLEN;
  wire vip_S05_AXI_M_AXI_ARLOCK;
  wire [2:0]vip_S05_AXI_M_AXI_ARPROT;
  wire [3:0]vip_S05_AXI_M_AXI_ARQOS;
  wire vip_S05_AXI_M_AXI_ARREADY;
  wire vip_S05_AXI_M_AXI_ARVALID;
  wire [38:0]vip_S05_AXI_M_AXI_AWADDR;
  wire [3:0]vip_S05_AXI_M_AXI_AWCACHE;
  wire [7:0]vip_S05_AXI_M_AXI_AWLEN;
  wire vip_S05_AXI_M_AXI_AWLOCK;
  wire [2:0]vip_S05_AXI_M_AXI_AWPROT;
  wire [3:0]vip_S05_AXI_M_AXI_AWQOS;
  wire vip_S05_AXI_M_AXI_AWREADY;
  wire vip_S05_AXI_M_AXI_AWVALID;
  wire vip_S05_AXI_M_AXI_BREADY;
  wire [1:0]vip_S05_AXI_M_AXI_BRESP;
  wire vip_S05_AXI_M_AXI_BVALID;
  wire [511:0]vip_S05_AXI_M_AXI_RDATA;
  wire vip_S05_AXI_M_AXI_RLAST;
  wire vip_S05_AXI_M_AXI_RREADY;
  wire [1:0]vip_S05_AXI_M_AXI_RRESP;
  wire vip_S05_AXI_M_AXI_RVALID;
  wire [511:0]vip_S05_AXI_M_AXI_WDATA;
  wire vip_S05_AXI_M_AXI_WLAST;
  wire vip_S05_AXI_M_AXI_WREADY;
  wire [63:0]vip_S05_AXI_M_AXI_WSTRB;
  wire vip_S05_AXI_M_AXI_WVALID;
  wire [2:0]NLW_interconnect_ddr4_mem01_M00_AXI_arsize_UNCONNECTED;
  wire [2:0]NLW_interconnect_ddr4_mem01_M00_AXI_awsize_UNCONNECTED;
  wire [2:0]NLW_interconnect_m00_axi_mem00_M00_AXI_arsize_UNCONNECTED;
  wire [2:0]NLW_interconnect_m00_axi_mem00_M00_AXI_awsize_UNCONNECTED;
  wire [3:0]NLW_vip_s01_axi_m_axi_arregion_UNCONNECTED;
  wire [3:0]NLW_vip_s01_axi_m_axi_awregion_UNCONNECTED;
  wire [3:0]NLW_vip_s02_axi_m_axi_arregion_UNCONNECTED;
  wire [3:0]NLW_vip_s02_axi_m_axi_awregion_UNCONNECTED;
  wire [3:0]NLW_vip_s04_axi_m_axi_arregion_UNCONNECTED;
  wire [3:0]NLW_vip_s04_axi_m_axi_awregion_UNCONNECTED;
  wire [3:0]NLW_vip_s05_axi_m_axi_arregion_UNCONNECTED;
  wire [3:0]NLW_vip_s05_axi_m_axi_awregion_UNCONNECTED;

  (* X_CORE_INFO = "bd_e7f0,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_DDR4_MEM01_0 interconnect_ddr4_mem01
       (.M00_AXI_araddr(M00_AXI_araddr),
        .M00_AXI_arburst(M00_AXI_arburst),
        .M00_AXI_arcache(M00_AXI_arcache),
        .M00_AXI_arlen(M00_AXI_arlen),
        .M00_AXI_arlock(M00_AXI_arlock),
        .M00_AXI_arprot(M00_AXI_arprot),
        .M00_AXI_arqos(M00_AXI_arqos),
        .M00_AXI_arready(S_AXI_arready),
        .M00_AXI_arsize(NLW_interconnect_ddr4_mem01_M00_AXI_arsize_UNCONNECTED[2:0]),
        .M00_AXI_arvalid(M00_AXI_arvalid),
        .M00_AXI_awaddr(M00_AXI_awaddr),
        .M00_AXI_awburst(M00_AXI_awburst),
        .M00_AXI_awcache(M00_AXI_awcache),
        .M00_AXI_awlen(M00_AXI_awlen),
        .M00_AXI_awlock(M00_AXI_awlock),
        .M00_AXI_awprot(M00_AXI_awprot),
        .M00_AXI_awqos(M00_AXI_awqos),
        .M00_AXI_awready(S_AXI_awready),
        .M00_AXI_awsize(NLW_interconnect_ddr4_mem01_M00_AXI_awsize_UNCONNECTED[2:0]),
        .M00_AXI_awvalid(M00_AXI_awvalid),
        .M00_AXI_bready(M00_AXI_bready),
        .M00_AXI_bresp(S_AXI_bresp),
        .M00_AXI_bvalid(S_AXI_bvalid),
        .M00_AXI_rdata(S_AXI_rdata),
        .M00_AXI_rlast(S_AXI_rlast),
        .M00_AXI_rready(M00_AXI_rready),
        .M00_AXI_rresp(S_AXI_rresp),
        .M00_AXI_rvalid(S_AXI_rvalid),
        .M00_AXI_wdata(M00_AXI_wdata),
        .M00_AXI_wlast(M00_AXI_wlast),
        .M00_AXI_wready(S_AXI_wready),
        .M00_AXI_wstrb(M00_AXI_wstrb),
        .M00_AXI_wvalid(M00_AXI_wvalid),
        .S00_AXI_araddr(vip_S02_AXI_M_AXI_ARADDR),
        .S00_AXI_arburst(vip_S02_AXI_M_AXI_ARBURST),
        .S00_AXI_arcache(vip_S02_AXI_M_AXI_ARCACHE),
        .S00_AXI_arid(vip_S02_AXI_M_AXI_ARID),
        .S00_AXI_arlen(vip_S02_AXI_M_AXI_ARLEN),
        .S00_AXI_arlock(vip_S02_AXI_M_AXI_ARLOCK),
        .S00_AXI_arprot(vip_S02_AXI_M_AXI_ARPROT),
        .S00_AXI_arqos(vip_S02_AXI_M_AXI_ARQOS),
        .S00_AXI_arready(vip_S02_AXI_M_AXI_ARREADY),
        .S00_AXI_arsize({1'b1,1'b1,1'b0}),
        .S00_AXI_arvalid(vip_S02_AXI_M_AXI_ARVALID),
        .S00_AXI_awaddr(vip_S02_AXI_M_AXI_AWADDR),
        .S00_AXI_awburst(vip_S02_AXI_M_AXI_AWBURST),
        .S00_AXI_awcache(vip_S02_AXI_M_AXI_AWCACHE),
        .S00_AXI_awid(vip_S02_AXI_M_AXI_AWID),
        .S00_AXI_awlen(vip_S02_AXI_M_AXI_AWLEN),
        .S00_AXI_awlock(vip_S02_AXI_M_AXI_AWLOCK),
        .S00_AXI_awprot(vip_S02_AXI_M_AXI_AWPROT),
        .S00_AXI_awqos(vip_S02_AXI_M_AXI_AWQOS),
        .S00_AXI_awready(vip_S02_AXI_M_AXI_AWREADY),
        .S00_AXI_awsize({1'b1,1'b1,1'b0}),
        .S00_AXI_awvalid(vip_S02_AXI_M_AXI_AWVALID),
        .S00_AXI_bid(vip_S02_AXI_M_AXI_BID),
        .S00_AXI_bready(vip_S02_AXI_M_AXI_BREADY),
        .S00_AXI_bresp(vip_S02_AXI_M_AXI_BRESP),
        .S00_AXI_bvalid(vip_S02_AXI_M_AXI_BVALID),
        .S00_AXI_rdata(vip_S02_AXI_M_AXI_RDATA),
        .S00_AXI_rid(vip_S02_AXI_M_AXI_RID),
        .S00_AXI_rlast(vip_S02_AXI_M_AXI_RLAST),
        .S00_AXI_rready(vip_S02_AXI_M_AXI_RREADY),
        .S00_AXI_rresp(vip_S02_AXI_M_AXI_RRESP),
        .S00_AXI_rvalid(vip_S02_AXI_M_AXI_RVALID),
        .S00_AXI_wdata(vip_S02_AXI_M_AXI_WDATA),
        .S00_AXI_wlast(vip_S02_AXI_M_AXI_WLAST),
        .S00_AXI_wready(vip_S02_AXI_M_AXI_WREADY),
        .S00_AXI_wstrb(vip_S02_AXI_M_AXI_WSTRB),
        .S00_AXI_wvalid(vip_S02_AXI_M_AXI_WVALID),
        .S01_AXI_araddr(interconnect_S04_AXI_M00_AXI_ARADDR),
        .S01_AXI_arburst(interconnect_S04_AXI_M00_AXI_ARBURST),
        .S01_AXI_arcache(interconnect_S04_AXI_M00_AXI_ARCACHE),
        .S01_AXI_arid(interconnect_S04_AXI_M00_AXI_ARID),
        .S01_AXI_arlen(interconnect_S04_AXI_M00_AXI_ARLEN),
        .S01_AXI_arlock(interconnect_S04_AXI_M00_AXI_ARLOCK),
        .S01_AXI_arprot(interconnect_S04_AXI_M00_AXI_ARPROT),
        .S01_AXI_arqos(interconnect_S04_AXI_M00_AXI_ARQOS),
        .S01_AXI_arready(interconnect_S04_AXI_M00_AXI_ARREADY),
        .S01_AXI_arsize(interconnect_S04_AXI_M00_AXI_ARSIZE),
        .S01_AXI_aruser(interconnect_S04_AXI_M00_AXI_ARUSER),
        .S01_AXI_arvalid(interconnect_S04_AXI_M00_AXI_ARVALID),
        .S01_AXI_awaddr(interconnect_S04_AXI_M00_AXI_AWADDR),
        .S01_AXI_awburst(interconnect_S04_AXI_M00_AXI_AWBURST),
        .S01_AXI_awcache(interconnect_S04_AXI_M00_AXI_AWCACHE),
        .S01_AXI_awid(interconnect_S04_AXI_M00_AXI_AWID),
        .S01_AXI_awlen(interconnect_S04_AXI_M00_AXI_AWLEN),
        .S01_AXI_awlock(interconnect_S04_AXI_M00_AXI_AWLOCK),
        .S01_AXI_awprot(interconnect_S04_AXI_M00_AXI_AWPROT),
        .S01_AXI_awqos(interconnect_S04_AXI_M00_AXI_AWQOS),
        .S01_AXI_awready(interconnect_S04_AXI_M00_AXI_AWREADY),
        .S01_AXI_awsize(interconnect_S04_AXI_M00_AXI_AWSIZE),
        .S01_AXI_awuser(interconnect_S04_AXI_M00_AXI_AWUSER),
        .S01_AXI_awvalid(interconnect_S04_AXI_M00_AXI_AWVALID),
        .S01_AXI_bid(interconnect_S04_AXI_M00_AXI_BID),
        .S01_AXI_bready(interconnect_S04_AXI_M00_AXI_BREADY),
        .S01_AXI_bresp(interconnect_S04_AXI_M00_AXI_BRESP),
        .S01_AXI_buser(interconnect_S04_AXI_M00_AXI_BUSER),
        .S01_AXI_bvalid(interconnect_S04_AXI_M00_AXI_BVALID),
        .S01_AXI_rdata(interconnect_S04_AXI_M00_AXI_RDATA),
        .S01_AXI_rid(interconnect_S04_AXI_M00_AXI_RID),
        .S01_AXI_rlast(interconnect_S04_AXI_M00_AXI_RLAST),
        .S01_AXI_rready(interconnect_S04_AXI_M00_AXI_RREADY),
        .S01_AXI_rresp(interconnect_S04_AXI_M00_AXI_RRESP),
        .S01_AXI_ruser(interconnect_S04_AXI_M00_AXI_RUSER),
        .S01_AXI_rvalid(interconnect_S04_AXI_M00_AXI_RVALID),
        .S01_AXI_wdata(interconnect_S04_AXI_M00_AXI_WDATA),
        .S01_AXI_wlast(interconnect_S04_AXI_M00_AXI_WLAST),
        .S01_AXI_wready(interconnect_S04_AXI_M00_AXI_WREADY),
        .S01_AXI_wstrb(interconnect_S04_AXI_M00_AXI_WSTRB),
        .S01_AXI_wuser(interconnect_S04_AXI_M00_AXI_WUSER),
        .S01_AXI_wvalid(interconnect_S04_AXI_M00_AXI_WVALID),
        .aclk(ddr4_mem01_ui_clk),
        .aclk1(aclk),
        .aresetn(interconnect_aresetn));
  (* X_CORE_INFO = "bd_2b97,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_M00_AXI_MEM00_0 interconnect_m00_axi_mem00
       (.M00_AXI_araddr(interconnect_M00_AXI_MEM00_M00_AXI_ARADDR),
        .M00_AXI_arburst(interconnect_M00_AXI_MEM00_M00_AXI_ARBURST),
        .M00_AXI_arcache(interconnect_M00_AXI_MEM00_M00_AXI_ARCACHE),
        .M00_AXI_arlen(interconnect_M00_AXI_MEM00_M00_AXI_ARLEN),
        .M00_AXI_arlock(interconnect_M00_AXI_MEM00_M00_AXI_ARLOCK),
        .M00_AXI_arprot(interconnect_M00_AXI_MEM00_M00_AXI_ARPROT),
        .M00_AXI_arqos(interconnect_M00_AXI_MEM00_M00_AXI_ARQOS),
        .M00_AXI_arready(interconnect_M00_AXI_MEM00_M00_AXI_ARREADY),
        .M00_AXI_arsize(NLW_interconnect_m00_axi_mem00_M00_AXI_arsize_UNCONNECTED[2:0]),
        .M00_AXI_arvalid(interconnect_M00_AXI_MEM00_M00_AXI_ARVALID),
        .M00_AXI_awaddr(interconnect_M00_AXI_MEM00_M00_AXI_AWADDR),
        .M00_AXI_awburst(interconnect_M00_AXI_MEM00_M00_AXI_AWBURST),
        .M00_AXI_awcache(interconnect_M00_AXI_MEM00_M00_AXI_AWCACHE),
        .M00_AXI_awlen(interconnect_M00_AXI_MEM00_M00_AXI_AWLEN),
        .M00_AXI_awlock(interconnect_M00_AXI_MEM00_M00_AXI_AWLOCK),
        .M00_AXI_awprot(interconnect_M00_AXI_MEM00_M00_AXI_AWPROT),
        .M00_AXI_awqos(interconnect_M00_AXI_MEM00_M00_AXI_AWQOS),
        .M00_AXI_awready(interconnect_M00_AXI_MEM00_M00_AXI_AWREADY),
        .M00_AXI_awsize(NLW_interconnect_m00_axi_mem00_M00_AXI_awsize_UNCONNECTED[2:0]),
        .M00_AXI_awvalid(interconnect_M00_AXI_MEM00_M00_AXI_AWVALID),
        .M00_AXI_bready(interconnect_M00_AXI_MEM00_M00_AXI_BREADY),
        .M00_AXI_bresp(interconnect_M00_AXI_MEM00_M00_AXI_BRESP),
        .M00_AXI_bvalid(interconnect_M00_AXI_MEM00_M00_AXI_BVALID),
        .M00_AXI_rdata(interconnect_M00_AXI_MEM00_M00_AXI_RDATA),
        .M00_AXI_rlast(interconnect_M00_AXI_MEM00_M00_AXI_RLAST),
        .M00_AXI_rready(interconnect_M00_AXI_MEM00_M00_AXI_RREADY),
        .M00_AXI_rresp(interconnect_M00_AXI_MEM00_M00_AXI_RRESP),
        .M00_AXI_rvalid(interconnect_M00_AXI_MEM00_M00_AXI_RVALID),
        .M00_AXI_wdata(interconnect_M00_AXI_MEM00_M00_AXI_WDATA),
        .M00_AXI_wlast(interconnect_M00_AXI_MEM00_M00_AXI_WLAST),
        .M00_AXI_wready(interconnect_M00_AXI_MEM00_M00_AXI_WREADY),
        .M00_AXI_wstrb(interconnect_M00_AXI_MEM00_M00_AXI_WSTRB),
        .M00_AXI_wvalid(interconnect_M00_AXI_MEM00_M00_AXI_WVALID),
        .S00_AXI_araddr(vip_S01_AXI_M_AXI_ARADDR),
        .S00_AXI_arburst(vip_S01_AXI_M_AXI_ARBURST),
        .S00_AXI_arcache(vip_S01_AXI_M_AXI_ARCACHE),
        .S00_AXI_arid(vip_S01_AXI_M_AXI_ARID),
        .S00_AXI_arlen(vip_S01_AXI_M_AXI_ARLEN),
        .S00_AXI_arlock(vip_S01_AXI_M_AXI_ARLOCK),
        .S00_AXI_arprot(vip_S01_AXI_M_AXI_ARPROT),
        .S00_AXI_arqos(vip_S01_AXI_M_AXI_ARQOS),
        .S00_AXI_arready(vip_S01_AXI_M_AXI_ARREADY),
        .S00_AXI_arsize({1'b1,1'b1,1'b0}),
        .S00_AXI_arvalid(vip_S01_AXI_M_AXI_ARVALID),
        .S00_AXI_awaddr(vip_S01_AXI_M_AXI_AWADDR),
        .S00_AXI_awburst(vip_S01_AXI_M_AXI_AWBURST),
        .S00_AXI_awcache(vip_S01_AXI_M_AXI_AWCACHE),
        .S00_AXI_awid(vip_S01_AXI_M_AXI_AWID),
        .S00_AXI_awlen(vip_S01_AXI_M_AXI_AWLEN),
        .S00_AXI_awlock(vip_S01_AXI_M_AXI_AWLOCK),
        .S00_AXI_awprot(vip_S01_AXI_M_AXI_AWPROT),
        .S00_AXI_awqos(vip_S01_AXI_M_AXI_AWQOS),
        .S00_AXI_awready(vip_S01_AXI_M_AXI_AWREADY),
        .S00_AXI_awsize({1'b1,1'b1,1'b0}),
        .S00_AXI_awvalid(vip_S01_AXI_M_AXI_AWVALID),
        .S00_AXI_bid(vip_S01_AXI_M_AXI_BID),
        .S00_AXI_bready(vip_S01_AXI_M_AXI_BREADY),
        .S00_AXI_bresp(vip_S01_AXI_M_AXI_BRESP),
        .S00_AXI_bvalid(vip_S01_AXI_M_AXI_BVALID),
        .S00_AXI_rdata(vip_S01_AXI_M_AXI_RDATA),
        .S00_AXI_rid(vip_S01_AXI_M_AXI_RID),
        .S00_AXI_rlast(vip_S01_AXI_M_AXI_RLAST),
        .S00_AXI_rready(vip_S01_AXI_M_AXI_RREADY),
        .S00_AXI_rresp(vip_S01_AXI_M_AXI_RRESP),
        .S00_AXI_rvalid(vip_S01_AXI_M_AXI_RVALID),
        .S00_AXI_wdata(vip_S01_AXI_M_AXI_WDATA),
        .S00_AXI_wlast(vip_S01_AXI_M_AXI_WLAST),
        .S00_AXI_wready(vip_S01_AXI_M_AXI_WREADY),
        .S00_AXI_wstrb(vip_S01_AXI_M_AXI_WSTRB),
        .S00_AXI_wvalid(vip_S01_AXI_M_AXI_WVALID),
        .S01_AXI_araddr(vip_S05_AXI_M_AXI_ARADDR),
        .S01_AXI_arburst({1'b0,1'b1}),
        .S01_AXI_arcache(vip_S05_AXI_M_AXI_ARCACHE),
        .S01_AXI_arlen(vip_S05_AXI_M_AXI_ARLEN),
        .S01_AXI_arlock(vip_S05_AXI_M_AXI_ARLOCK),
        .S01_AXI_arprot(vip_S05_AXI_M_AXI_ARPROT),
        .S01_AXI_arqos(vip_S05_AXI_M_AXI_ARQOS),
        .S01_AXI_arready(vip_S05_AXI_M_AXI_ARREADY),
        .S01_AXI_arsize({1'b1,1'b1,1'b0}),
        .S01_AXI_arvalid(vip_S05_AXI_M_AXI_ARVALID),
        .S01_AXI_awaddr(vip_S05_AXI_M_AXI_AWADDR),
        .S01_AXI_awburst({1'b0,1'b1}),
        .S01_AXI_awcache(vip_S05_AXI_M_AXI_AWCACHE),
        .S01_AXI_awlen(vip_S05_AXI_M_AXI_AWLEN),
        .S01_AXI_awlock(vip_S05_AXI_M_AXI_AWLOCK),
        .S01_AXI_awprot(vip_S05_AXI_M_AXI_AWPROT),
        .S01_AXI_awqos(vip_S05_AXI_M_AXI_AWQOS),
        .S01_AXI_awready(vip_S05_AXI_M_AXI_AWREADY),
        .S01_AXI_awsize({1'b1,1'b1,1'b0}),
        .S01_AXI_awvalid(vip_S05_AXI_M_AXI_AWVALID),
        .S01_AXI_bready(vip_S05_AXI_M_AXI_BREADY),
        .S01_AXI_bresp(vip_S05_AXI_M_AXI_BRESP),
        .S01_AXI_bvalid(vip_S05_AXI_M_AXI_BVALID),
        .S01_AXI_rdata(vip_S05_AXI_M_AXI_RDATA),
        .S01_AXI_rlast(vip_S05_AXI_M_AXI_RLAST),
        .S01_AXI_rready(vip_S05_AXI_M_AXI_RREADY),
        .S01_AXI_rresp(vip_S05_AXI_M_AXI_RRESP),
        .S01_AXI_rvalid(vip_S05_AXI_M_AXI_RVALID),
        .S01_AXI_wdata(vip_S05_AXI_M_AXI_WDATA),
        .S01_AXI_wlast(vip_S05_AXI_M_AXI_WLAST),
        .S01_AXI_wready(vip_S05_AXI_M_AXI_WREADY),
        .S01_AXI_wstrb(vip_S05_AXI_M_AXI_WSTRB),
        .S01_AXI_wvalid(vip_S05_AXI_M_AXI_WVALID),
        .S02_AXI_araddr(interconnect_S04_AXI_M01_AXI_ARADDR),
        .S02_AXI_arburst(interconnect_S04_AXI_M01_AXI_ARBURST),
        .S02_AXI_arcache(interconnect_S04_AXI_M01_AXI_ARCACHE),
        .S02_AXI_arid(interconnect_S04_AXI_M01_AXI_ARID),
        .S02_AXI_arlen(interconnect_S04_AXI_M01_AXI_ARLEN),
        .S02_AXI_arlock(interconnect_S04_AXI_M01_AXI_ARLOCK),
        .S02_AXI_arprot(interconnect_S04_AXI_M01_AXI_ARPROT),
        .S02_AXI_arqos(interconnect_S04_AXI_M01_AXI_ARQOS),
        .S02_AXI_arready(interconnect_S04_AXI_M01_AXI_ARREADY),
        .S02_AXI_arsize(interconnect_S04_AXI_M01_AXI_ARSIZE),
        .S02_AXI_aruser(interconnect_S04_AXI_M01_AXI_ARUSER),
        .S02_AXI_arvalid(interconnect_S04_AXI_M01_AXI_ARVALID),
        .S02_AXI_awaddr(interconnect_S04_AXI_M01_AXI_AWADDR),
        .S02_AXI_awburst(interconnect_S04_AXI_M01_AXI_AWBURST),
        .S02_AXI_awcache(interconnect_S04_AXI_M01_AXI_AWCACHE),
        .S02_AXI_awid(interconnect_S04_AXI_M01_AXI_AWID),
        .S02_AXI_awlen(interconnect_S04_AXI_M01_AXI_AWLEN),
        .S02_AXI_awlock(interconnect_S04_AXI_M01_AXI_AWLOCK),
        .S02_AXI_awprot(interconnect_S04_AXI_M01_AXI_AWPROT),
        .S02_AXI_awqos(interconnect_S04_AXI_M01_AXI_AWQOS),
        .S02_AXI_awready(interconnect_S04_AXI_M01_AXI_AWREADY),
        .S02_AXI_awsize(interconnect_S04_AXI_M01_AXI_AWSIZE),
        .S02_AXI_awuser(interconnect_S04_AXI_M01_AXI_AWUSER),
        .S02_AXI_awvalid(interconnect_S04_AXI_M01_AXI_AWVALID),
        .S02_AXI_bid(interconnect_S04_AXI_M01_AXI_BID),
        .S02_AXI_bready(interconnect_S04_AXI_M01_AXI_BREADY),
        .S02_AXI_bresp(interconnect_S04_AXI_M01_AXI_BRESP),
        .S02_AXI_buser(interconnect_S04_AXI_M01_AXI_BUSER),
        .S02_AXI_bvalid(interconnect_S04_AXI_M01_AXI_BVALID),
        .S02_AXI_rdata(interconnect_S04_AXI_M01_AXI_RDATA),
        .S02_AXI_rid(interconnect_S04_AXI_M01_AXI_RID),
        .S02_AXI_rlast(interconnect_S04_AXI_M01_AXI_RLAST),
        .S02_AXI_rready(interconnect_S04_AXI_M01_AXI_RREADY),
        .S02_AXI_rresp(interconnect_S04_AXI_M01_AXI_RRESP),
        .S02_AXI_ruser(interconnect_S04_AXI_M01_AXI_RUSER),
        .S02_AXI_rvalid(interconnect_S04_AXI_M01_AXI_RVALID),
        .S02_AXI_wdata(interconnect_S04_AXI_M01_AXI_WDATA),
        .S02_AXI_wlast(interconnect_S04_AXI_M01_AXI_WLAST),
        .S02_AXI_wready(interconnect_S04_AXI_M01_AXI_WREADY),
        .S02_AXI_wstrb(interconnect_S04_AXI_M01_AXI_WSTRB),
        .S02_AXI_wuser(interconnect_S04_AXI_M01_AXI_WUSER),
        .S02_AXI_wvalid(interconnect_S04_AXI_M01_AXI_WVALID),
        .aclk(aclk2),
        .aclk1(aclk),
        .aresetn(interconnect_aresetn1));
  (* X_CORE_INFO = "bd_8bbf,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_S04_AXI_0 interconnect_s04_axi
       (.M00_AXI_araddr(interconnect_S04_AXI_M00_AXI_ARADDR),
        .M00_AXI_arburst(interconnect_S04_AXI_M00_AXI_ARBURST),
        .M00_AXI_arcache(interconnect_S04_AXI_M00_AXI_ARCACHE),
        .M00_AXI_arid(interconnect_S04_AXI_M00_AXI_ARID),
        .M00_AXI_arlen(interconnect_S04_AXI_M00_AXI_ARLEN),
        .M00_AXI_arlock(interconnect_S04_AXI_M00_AXI_ARLOCK),
        .M00_AXI_arprot(interconnect_S04_AXI_M00_AXI_ARPROT),
        .M00_AXI_arqos(interconnect_S04_AXI_M00_AXI_ARQOS),
        .M00_AXI_arready(interconnect_S04_AXI_M00_AXI_ARREADY),
        .M00_AXI_arsize(interconnect_S04_AXI_M00_AXI_ARSIZE),
        .M00_AXI_aruser(interconnect_S04_AXI_M00_AXI_ARUSER),
        .M00_AXI_arvalid(interconnect_S04_AXI_M00_AXI_ARVALID),
        .M00_AXI_awaddr(interconnect_S04_AXI_M00_AXI_AWADDR),
        .M00_AXI_awburst(interconnect_S04_AXI_M00_AXI_AWBURST),
        .M00_AXI_awcache(interconnect_S04_AXI_M00_AXI_AWCACHE),
        .M00_AXI_awid(interconnect_S04_AXI_M00_AXI_AWID),
        .M00_AXI_awlen(interconnect_S04_AXI_M00_AXI_AWLEN),
        .M00_AXI_awlock(interconnect_S04_AXI_M00_AXI_AWLOCK),
        .M00_AXI_awprot(interconnect_S04_AXI_M00_AXI_AWPROT),
        .M00_AXI_awqos(interconnect_S04_AXI_M00_AXI_AWQOS),
        .M00_AXI_awready(interconnect_S04_AXI_M00_AXI_AWREADY),
        .M00_AXI_awsize(interconnect_S04_AXI_M00_AXI_AWSIZE),
        .M00_AXI_awuser(interconnect_S04_AXI_M00_AXI_AWUSER),
        .M00_AXI_awvalid(interconnect_S04_AXI_M00_AXI_AWVALID),
        .M00_AXI_bid(interconnect_S04_AXI_M00_AXI_BID),
        .M00_AXI_bready(interconnect_S04_AXI_M00_AXI_BREADY),
        .M00_AXI_bresp(interconnect_S04_AXI_M00_AXI_BRESP),
        .M00_AXI_buser(interconnect_S04_AXI_M00_AXI_BUSER),
        .M00_AXI_bvalid(interconnect_S04_AXI_M00_AXI_BVALID),
        .M00_AXI_rdata(interconnect_S04_AXI_M00_AXI_RDATA),
        .M00_AXI_rid(interconnect_S04_AXI_M00_AXI_RID),
        .M00_AXI_rlast(interconnect_S04_AXI_M00_AXI_RLAST),
        .M00_AXI_rready(interconnect_S04_AXI_M00_AXI_RREADY),
        .M00_AXI_rresp(interconnect_S04_AXI_M00_AXI_RRESP),
        .M00_AXI_ruser(interconnect_S04_AXI_M00_AXI_RUSER),
        .M00_AXI_rvalid(interconnect_S04_AXI_M00_AXI_RVALID),
        .M00_AXI_wdata(interconnect_S04_AXI_M00_AXI_WDATA),
        .M00_AXI_wlast(interconnect_S04_AXI_M00_AXI_WLAST),
        .M00_AXI_wready(interconnect_S04_AXI_M00_AXI_WREADY),
        .M00_AXI_wstrb(interconnect_S04_AXI_M00_AXI_WSTRB),
        .M00_AXI_wuser(interconnect_S04_AXI_M00_AXI_WUSER),
        .M00_AXI_wvalid(interconnect_S04_AXI_M00_AXI_WVALID),
        .M01_AXI_araddr(interconnect_S04_AXI_M01_AXI_ARADDR),
        .M01_AXI_arburst(interconnect_S04_AXI_M01_AXI_ARBURST),
        .M01_AXI_arcache(interconnect_S04_AXI_M01_AXI_ARCACHE),
        .M01_AXI_arid(interconnect_S04_AXI_M01_AXI_ARID),
        .M01_AXI_arlen(interconnect_S04_AXI_M01_AXI_ARLEN),
        .M01_AXI_arlock(interconnect_S04_AXI_M01_AXI_ARLOCK),
        .M01_AXI_arprot(interconnect_S04_AXI_M01_AXI_ARPROT),
        .M01_AXI_arqos(interconnect_S04_AXI_M01_AXI_ARQOS),
        .M01_AXI_arready(interconnect_S04_AXI_M01_AXI_ARREADY),
        .M01_AXI_arsize(interconnect_S04_AXI_M01_AXI_ARSIZE),
        .M01_AXI_aruser(interconnect_S04_AXI_M01_AXI_ARUSER),
        .M01_AXI_arvalid(interconnect_S04_AXI_M01_AXI_ARVALID),
        .M01_AXI_awaddr(interconnect_S04_AXI_M01_AXI_AWADDR),
        .M01_AXI_awburst(interconnect_S04_AXI_M01_AXI_AWBURST),
        .M01_AXI_awcache(interconnect_S04_AXI_M01_AXI_AWCACHE),
        .M01_AXI_awid(interconnect_S04_AXI_M01_AXI_AWID),
        .M01_AXI_awlen(interconnect_S04_AXI_M01_AXI_AWLEN),
        .M01_AXI_awlock(interconnect_S04_AXI_M01_AXI_AWLOCK),
        .M01_AXI_awprot(interconnect_S04_AXI_M01_AXI_AWPROT),
        .M01_AXI_awqos(interconnect_S04_AXI_M01_AXI_AWQOS),
        .M01_AXI_awready(interconnect_S04_AXI_M01_AXI_AWREADY),
        .M01_AXI_awsize(interconnect_S04_AXI_M01_AXI_AWSIZE),
        .M01_AXI_awuser(interconnect_S04_AXI_M01_AXI_AWUSER),
        .M01_AXI_awvalid(interconnect_S04_AXI_M01_AXI_AWVALID),
        .M01_AXI_bid(interconnect_S04_AXI_M01_AXI_BID),
        .M01_AXI_bready(interconnect_S04_AXI_M01_AXI_BREADY),
        .M01_AXI_bresp(interconnect_S04_AXI_M01_AXI_BRESP),
        .M01_AXI_buser(interconnect_S04_AXI_M01_AXI_BUSER),
        .M01_AXI_bvalid(interconnect_S04_AXI_M01_AXI_BVALID),
        .M01_AXI_rdata(interconnect_S04_AXI_M01_AXI_RDATA),
        .M01_AXI_rid(interconnect_S04_AXI_M01_AXI_RID),
        .M01_AXI_rlast(interconnect_S04_AXI_M01_AXI_RLAST),
        .M01_AXI_rready(interconnect_S04_AXI_M01_AXI_RREADY),
        .M01_AXI_rresp(interconnect_S04_AXI_M01_AXI_RRESP),
        .M01_AXI_ruser(interconnect_S04_AXI_M01_AXI_RUSER),
        .M01_AXI_rvalid(interconnect_S04_AXI_M01_AXI_RVALID),
        .M01_AXI_wdata(interconnect_S04_AXI_M01_AXI_WDATA),
        .M01_AXI_wlast(interconnect_S04_AXI_M01_AXI_WLAST),
        .M01_AXI_wready(interconnect_S04_AXI_M01_AXI_WREADY),
        .M01_AXI_wstrb(interconnect_S04_AXI_M01_AXI_WSTRB),
        .M01_AXI_wuser(interconnect_S04_AXI_M01_AXI_WUSER),
        .M01_AXI_wvalid(interconnect_S04_AXI_M01_AXI_WVALID),
        .S00_AXI_araddr(vip_S04_AXI_M_AXI_ARADDR),
        .S00_AXI_arburst({1'b0,1'b1}),
        .S00_AXI_arcache(vip_S04_AXI_M_AXI_ARCACHE),
        .S00_AXI_arlen(vip_S04_AXI_M_AXI_ARLEN),
        .S00_AXI_arlock(vip_S04_AXI_M_AXI_ARLOCK),
        .S00_AXI_arprot(vip_S04_AXI_M_AXI_ARPROT),
        .S00_AXI_arqos(vip_S04_AXI_M_AXI_ARQOS),
        .S00_AXI_arready(vip_S04_AXI_M_AXI_ARREADY),
        .S00_AXI_arsize({1'b1,1'b1,1'b0}),
        .S00_AXI_arvalid(vip_S04_AXI_M_AXI_ARVALID),
        .S00_AXI_awaddr(vip_S04_AXI_M_AXI_AWADDR),
        .S00_AXI_awburst({1'b0,1'b1}),
        .S00_AXI_awcache(vip_S04_AXI_M_AXI_AWCACHE),
        .S00_AXI_awlen(vip_S04_AXI_M_AXI_AWLEN),
        .S00_AXI_awlock(vip_S04_AXI_M_AXI_AWLOCK),
        .S00_AXI_awprot(vip_S04_AXI_M_AXI_AWPROT),
        .S00_AXI_awqos(vip_S04_AXI_M_AXI_AWQOS),
        .S00_AXI_awready(vip_S04_AXI_M_AXI_AWREADY),
        .S00_AXI_awsize({1'b1,1'b1,1'b0}),
        .S00_AXI_awvalid(vip_S04_AXI_M_AXI_AWVALID),
        .S00_AXI_bready(vip_S04_AXI_M_AXI_BREADY),
        .S00_AXI_bresp(vip_S04_AXI_M_AXI_BRESP),
        .S00_AXI_bvalid(vip_S04_AXI_M_AXI_BVALID),
        .S00_AXI_rdata(vip_S04_AXI_M_AXI_RDATA),
        .S00_AXI_rlast(vip_S04_AXI_M_AXI_RLAST),
        .S00_AXI_rready(vip_S04_AXI_M_AXI_RREADY),
        .S00_AXI_rresp(vip_S04_AXI_M_AXI_RRESP),
        .S00_AXI_rvalid(vip_S04_AXI_M_AXI_RVALID),
        .S00_AXI_wdata(vip_S04_AXI_M_AXI_WDATA),
        .S00_AXI_wlast(vip_S04_AXI_M_AXI_WLAST),
        .S00_AXI_wready(vip_S04_AXI_M_AXI_WREADY),
        .S00_AXI_wstrb(vip_S04_AXI_M_AXI_WSTRB),
        .S00_AXI_wvalid(vip_S04_AXI_M_AXI_WVALID),
        .aclk(aclk),
        .aresetn(interconnect_aresetn));
  (* X_CORE_INFO = "axi_register_slice_v2_1_21_axi_register_slice,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_rs_M00_AXI_0 rs_m00_axi
       (.aclk(aclk2),
        .aresetn(interconnect_aresetn1),
        .m_axi_araddr(\bbstub_m_axi_araddr[38] ),
        .m_axi_arburst(\bbstub_m_axi_arburst[1] ),
        .m_axi_arcache(\bbstub_m_axi_arcache[3] ),
        .m_axi_arlen(\bbstub_m_axi_arlen[7] ),
        .m_axi_arlock(\bbstub_m_axi_arlock[0] ),
        .m_axi_arprot(\bbstub_m_axi_arprot[2] ),
        .m_axi_arqos(\bbstub_m_axi_arqos[3] ),
        .m_axi_arready(M00_AXI_arready),
        .m_axi_arregion(M00_AXI_arregion),
        .m_axi_arsize(M00_AXI_arsize),
        .m_axi_arvalid(bbstub_m_axi_arvalid),
        .m_axi_awaddr(\bbstub_m_axi_awaddr[38] ),
        .m_axi_awburst(\bbstub_m_axi_awburst[1] ),
        .m_axi_awcache(\bbstub_m_axi_awcache[3] ),
        .m_axi_awlen(\bbstub_m_axi_awlen[7] ),
        .m_axi_awlock(\bbstub_m_axi_awlock[0] ),
        .m_axi_awprot(\bbstub_m_axi_awprot[2] ),
        .m_axi_awqos(\bbstub_m_axi_awqos[3] ),
        .m_axi_awready(M00_AXI_awready),
        .m_axi_awregion(M00_AXI_awregion),
        .m_axi_awsize(M00_AXI_awsize),
        .m_axi_awvalid(bbstub_m_axi_awvalid),
        .m_axi_bready(bbstub_m_axi_bready),
        .m_axi_bresp(M00_AXI_bresp),
        .m_axi_bvalid(M00_AXI_bvalid),
        .m_axi_rdata(M00_AXI_rdata),
        .m_axi_rlast(M00_AXI_rlast),
        .m_axi_rready(bbstub_m_axi_rready),
        .m_axi_rresp(M00_AXI_rresp),
        .m_axi_rvalid(M00_AXI_rvalid),
        .m_axi_wdata(\bbstub_m_axi_wdata[511] ),
        .m_axi_wlast(bbstub_m_axi_wlast),
        .m_axi_wready(M00_AXI_wready),
        .m_axi_wstrb(\bbstub_m_axi_wstrb[63] ),
        .m_axi_wvalid(bbstub_m_axi_wvalid),
        .s_axi_araddr(vip_M00_AXI_M_AXI_ARADDR),
        .s_axi_arburst(vip_M00_AXI_M_AXI_ARBURST),
        .s_axi_arcache(vip_M00_AXI_M_AXI_ARCACHE),
        .s_axi_arlen(vip_M00_AXI_M_AXI_ARLEN),
        .s_axi_arlock(vip_M00_AXI_M_AXI_ARLOCK),
        .s_axi_arprot(vip_M00_AXI_M_AXI_ARPROT),
        .s_axi_arqos(vip_M00_AXI_M_AXI_ARQOS),
        .s_axi_arready(vip_M00_AXI_M_AXI_ARREADY),
        .s_axi_arregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arsize({1'b1,1'b1,1'b0}),
        .s_axi_arvalid(vip_M00_AXI_M_AXI_ARVALID),
        .s_axi_awaddr(vip_M00_AXI_M_AXI_AWADDR),
        .s_axi_awburst(vip_M00_AXI_M_AXI_AWBURST),
        .s_axi_awcache(vip_M00_AXI_M_AXI_AWCACHE),
        .s_axi_awlen(vip_M00_AXI_M_AXI_AWLEN),
        .s_axi_awlock(vip_M00_AXI_M_AXI_AWLOCK),
        .s_axi_awprot(vip_M00_AXI_M_AXI_AWPROT),
        .s_axi_awqos(vip_M00_AXI_M_AXI_AWQOS),
        .s_axi_awready(vip_M00_AXI_M_AXI_AWREADY),
        .s_axi_awregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awsize({1'b1,1'b1,1'b0}),
        .s_axi_awvalid(vip_M00_AXI_M_AXI_AWVALID),
        .s_axi_bready(vip_M00_AXI_M_AXI_BREADY),
        .s_axi_bresp(vip_M00_AXI_M_AXI_BRESP),
        .s_axi_bvalid(vip_M00_AXI_M_AXI_BVALID),
        .s_axi_rdata(vip_M00_AXI_M_AXI_RDATA),
        .s_axi_rlast(vip_M00_AXI_M_AXI_RLAST),
        .s_axi_rready(vip_M00_AXI_M_AXI_RREADY),
        .s_axi_rresp(vip_M00_AXI_M_AXI_RRESP),
        .s_axi_rvalid(vip_M00_AXI_M_AXI_RVALID),
        .s_axi_wdata(vip_M00_AXI_M_AXI_WDATA),
        .s_axi_wlast(vip_M00_AXI_M_AXI_WLAST),
        .s_axi_wready(vip_M00_AXI_M_AXI_WREADY),
        .s_axi_wstrb(vip_M00_AXI_M_AXI_WSTRB),
        .s_axi_wvalid(vip_M00_AXI_M_AXI_WVALID));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_M00_AXI_0 vip_m00_axi
       (.aclk(aclk2),
        .aresetn(interconnect_aresetn1),
        .m_axi_araddr(vip_M00_AXI_M_AXI_ARADDR),
        .m_axi_arburst(vip_M00_AXI_M_AXI_ARBURST),
        .m_axi_arcache(vip_M00_AXI_M_AXI_ARCACHE),
        .m_axi_arlen(vip_M00_AXI_M_AXI_ARLEN),
        .m_axi_arlock(vip_M00_AXI_M_AXI_ARLOCK),
        .m_axi_arprot(vip_M00_AXI_M_AXI_ARPROT),
        .m_axi_arqos(vip_M00_AXI_M_AXI_ARQOS),
        .m_axi_arready(vip_M00_AXI_M_AXI_ARREADY),
        .m_axi_arvalid(vip_M00_AXI_M_AXI_ARVALID),
        .m_axi_awaddr(vip_M00_AXI_M_AXI_AWADDR),
        .m_axi_awburst(vip_M00_AXI_M_AXI_AWBURST),
        .m_axi_awcache(vip_M00_AXI_M_AXI_AWCACHE),
        .m_axi_awlen(vip_M00_AXI_M_AXI_AWLEN),
        .m_axi_awlock(vip_M00_AXI_M_AXI_AWLOCK),
        .m_axi_awprot(vip_M00_AXI_M_AXI_AWPROT),
        .m_axi_awqos(vip_M00_AXI_M_AXI_AWQOS),
        .m_axi_awready(vip_M00_AXI_M_AXI_AWREADY),
        .m_axi_awvalid(vip_M00_AXI_M_AXI_AWVALID),
        .m_axi_bready(vip_M00_AXI_M_AXI_BREADY),
        .m_axi_bresp(vip_M00_AXI_M_AXI_BRESP),
        .m_axi_bvalid(vip_M00_AXI_M_AXI_BVALID),
        .m_axi_rdata(vip_M00_AXI_M_AXI_RDATA),
        .m_axi_rlast(vip_M00_AXI_M_AXI_RLAST),
        .m_axi_rready(vip_M00_AXI_M_AXI_RREADY),
        .m_axi_rresp(vip_M00_AXI_M_AXI_RRESP),
        .m_axi_rvalid(vip_M00_AXI_M_AXI_RVALID),
        .m_axi_wdata(vip_M00_AXI_M_AXI_WDATA),
        .m_axi_wlast(vip_M00_AXI_M_AXI_WLAST),
        .m_axi_wready(vip_M00_AXI_M_AXI_WREADY),
        .m_axi_wstrb(vip_M00_AXI_M_AXI_WSTRB),
        .m_axi_wvalid(vip_M00_AXI_M_AXI_WVALID),
        .s_axi_araddr(interconnect_M00_AXI_MEM00_M00_AXI_ARADDR),
        .s_axi_arburst(interconnect_M00_AXI_MEM00_M00_AXI_ARBURST),
        .s_axi_arcache(interconnect_M00_AXI_MEM00_M00_AXI_ARCACHE),
        .s_axi_arlen(interconnect_M00_AXI_MEM00_M00_AXI_ARLEN),
        .s_axi_arlock(interconnect_M00_AXI_MEM00_M00_AXI_ARLOCK),
        .s_axi_arprot(interconnect_M00_AXI_MEM00_M00_AXI_ARPROT),
        .s_axi_arqos(interconnect_M00_AXI_MEM00_M00_AXI_ARQOS),
        .s_axi_arready(interconnect_M00_AXI_MEM00_M00_AXI_ARREADY),
        .s_axi_arvalid(interconnect_M00_AXI_MEM00_M00_AXI_ARVALID),
        .s_axi_awaddr(interconnect_M00_AXI_MEM00_M00_AXI_AWADDR),
        .s_axi_awburst(interconnect_M00_AXI_MEM00_M00_AXI_AWBURST),
        .s_axi_awcache(interconnect_M00_AXI_MEM00_M00_AXI_AWCACHE),
        .s_axi_awlen(interconnect_M00_AXI_MEM00_M00_AXI_AWLEN),
        .s_axi_awlock(interconnect_M00_AXI_MEM00_M00_AXI_AWLOCK),
        .s_axi_awprot(interconnect_M00_AXI_MEM00_M00_AXI_AWPROT),
        .s_axi_awqos(interconnect_M00_AXI_MEM00_M00_AXI_AWQOS),
        .s_axi_awready(interconnect_M00_AXI_MEM00_M00_AXI_AWREADY),
        .s_axi_awvalid(interconnect_M00_AXI_MEM00_M00_AXI_AWVALID),
        .s_axi_bready(interconnect_M00_AXI_MEM00_M00_AXI_BREADY),
        .s_axi_bresp(interconnect_M00_AXI_MEM00_M00_AXI_BRESP),
        .s_axi_bvalid(interconnect_M00_AXI_MEM00_M00_AXI_BVALID),
        .s_axi_rdata(interconnect_M00_AXI_MEM00_M00_AXI_RDATA),
        .s_axi_rlast(interconnect_M00_AXI_MEM00_M00_AXI_RLAST),
        .s_axi_rready(interconnect_M00_AXI_MEM00_M00_AXI_RREADY),
        .s_axi_rresp(interconnect_M00_AXI_MEM00_M00_AXI_RRESP),
        .s_axi_rvalid(interconnect_M00_AXI_MEM00_M00_AXI_RVALID),
        .s_axi_wdata(interconnect_M00_AXI_MEM00_M00_AXI_WDATA),
        .s_axi_wlast(interconnect_M00_AXI_MEM00_M00_AXI_WLAST),
        .s_axi_wready(interconnect_M00_AXI_MEM00_M00_AXI_WREADY),
        .s_axi_wstrb(interconnect_M00_AXI_MEM00_M00_AXI_WSTRB),
        .s_axi_wvalid(interconnect_M00_AXI_MEM00_M00_AXI_WVALID));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S01_AXI_0 vip_s01_axi
       (.aclk(aclk),
        .aresetn(interconnect_aresetn),
        .m_axi_araddr(vip_S01_AXI_M_AXI_ARADDR),
        .m_axi_arburst(vip_S01_AXI_M_AXI_ARBURST),
        .m_axi_arcache(vip_S01_AXI_M_AXI_ARCACHE),
        .m_axi_arid(vip_S01_AXI_M_AXI_ARID),
        .m_axi_arlen(vip_S01_AXI_M_AXI_ARLEN),
        .m_axi_arlock(vip_S01_AXI_M_AXI_ARLOCK),
        .m_axi_arprot(vip_S01_AXI_M_AXI_ARPROT),
        .m_axi_arqos(vip_S01_AXI_M_AXI_ARQOS),
        .m_axi_arready(vip_S01_AXI_M_AXI_ARREADY),
        .m_axi_arregion(NLW_vip_s01_axi_m_axi_arregion_UNCONNECTED[3:0]),
        .m_axi_arvalid(vip_S01_AXI_M_AXI_ARVALID),
        .m_axi_awaddr(vip_S01_AXI_M_AXI_AWADDR),
        .m_axi_awburst(vip_S01_AXI_M_AXI_AWBURST),
        .m_axi_awcache(vip_S01_AXI_M_AXI_AWCACHE),
        .m_axi_awid(vip_S01_AXI_M_AXI_AWID),
        .m_axi_awlen(vip_S01_AXI_M_AXI_AWLEN),
        .m_axi_awlock(vip_S01_AXI_M_AXI_AWLOCK),
        .m_axi_awprot(vip_S01_AXI_M_AXI_AWPROT),
        .m_axi_awqos(vip_S01_AXI_M_AXI_AWQOS),
        .m_axi_awready(vip_S01_AXI_M_AXI_AWREADY),
        .m_axi_awregion(NLW_vip_s01_axi_m_axi_awregion_UNCONNECTED[3:0]),
        .m_axi_awvalid(vip_S01_AXI_M_AXI_AWVALID),
        .m_axi_bid(vip_S01_AXI_M_AXI_BID),
        .m_axi_bready(vip_S01_AXI_M_AXI_BREADY),
        .m_axi_bresp(vip_S01_AXI_M_AXI_BRESP),
        .m_axi_bvalid(vip_S01_AXI_M_AXI_BVALID),
        .m_axi_rdata(vip_S01_AXI_M_AXI_RDATA),
        .m_axi_rid(vip_S01_AXI_M_AXI_RID),
        .m_axi_rlast(vip_S01_AXI_M_AXI_RLAST),
        .m_axi_rready(vip_S01_AXI_M_AXI_RREADY),
        .m_axi_rresp(vip_S01_AXI_M_AXI_RRESP),
        .m_axi_rvalid(vip_S01_AXI_M_AXI_RVALID),
        .m_axi_wdata(vip_S01_AXI_M_AXI_WDATA),
        .m_axi_wlast(vip_S01_AXI_M_AXI_WLAST),
        .m_axi_wready(vip_S01_AXI_M_AXI_WREADY),
        .m_axi_wstrb(vip_S01_AXI_M_AXI_WSTRB),
        .m_axi_wvalid(vip_S01_AXI_M_AXI_WVALID),
        .s_axi_araddr(S01_AXI_araddr),
        .s_axi_arburst(S01_AXI_arburst),
        .s_axi_arcache(S01_AXI_arcache),
        .s_axi_arid(S01_AXI_arid),
        .s_axi_arlen(S01_AXI_arlen),
        .s_axi_arlock(S01_AXI_arlock),
        .s_axi_arprot(S01_AXI_arprot),
        .s_axi_arqos(S01_AXI_arqos),
        .s_axi_arready(S01_AXI_arready),
        .s_axi_arregion(S01_AXI_arregion),
        .s_axi_arvalid(S01_AXI_arvalid),
        .s_axi_awaddr(S01_AXI_awaddr),
        .s_axi_awburst(S01_AXI_awburst),
        .s_axi_awcache(S01_AXI_awcache),
        .s_axi_awid(S01_AXI_awid),
        .s_axi_awlen(S01_AXI_awlen),
        .s_axi_awlock(S01_AXI_awlock),
        .s_axi_awprot(S01_AXI_awprot),
        .s_axi_awqos(S01_AXI_awqos),
        .s_axi_awready(S01_AXI_awready),
        .s_axi_awregion(S01_AXI_awregion),
        .s_axi_awvalid(S01_AXI_awvalid),
        .s_axi_bid(S01_AXI_bid),
        .s_axi_bready(S01_AXI_bready),
        .s_axi_bresp(S01_AXI_bresp),
        .s_axi_bvalid(S01_AXI_bvalid),
        .s_axi_rdata(S01_AXI_rdata),
        .s_axi_rid(S01_AXI_rid),
        .s_axi_rlast(S01_AXI_rlast),
        .s_axi_rready(S01_AXI_rready),
        .s_axi_rresp(S01_AXI_rresp),
        .s_axi_rvalid(S01_AXI_rvalid),
        .s_axi_wdata(S01_AXI_wdata),
        .s_axi_wlast(S01_AXI_wlast),
        .s_axi_wready(S01_AXI_wready),
        .s_axi_wstrb(S01_AXI_wstrb),
        .s_axi_wvalid(S01_AXI_wvalid));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S02_AXI_0 vip_s02_axi
       (.aclk(aclk),
        .aresetn(interconnect_aresetn),
        .m_axi_araddr(vip_S02_AXI_M_AXI_ARADDR),
        .m_axi_arburst(vip_S02_AXI_M_AXI_ARBURST),
        .m_axi_arcache(vip_S02_AXI_M_AXI_ARCACHE),
        .m_axi_arid(vip_S02_AXI_M_AXI_ARID),
        .m_axi_arlen(vip_S02_AXI_M_AXI_ARLEN),
        .m_axi_arlock(vip_S02_AXI_M_AXI_ARLOCK),
        .m_axi_arprot(vip_S02_AXI_M_AXI_ARPROT),
        .m_axi_arqos(vip_S02_AXI_M_AXI_ARQOS),
        .m_axi_arready(vip_S02_AXI_M_AXI_ARREADY),
        .m_axi_arregion(NLW_vip_s02_axi_m_axi_arregion_UNCONNECTED[3:0]),
        .m_axi_arvalid(vip_S02_AXI_M_AXI_ARVALID),
        .m_axi_awaddr(vip_S02_AXI_M_AXI_AWADDR),
        .m_axi_awburst(vip_S02_AXI_M_AXI_AWBURST),
        .m_axi_awcache(vip_S02_AXI_M_AXI_AWCACHE),
        .m_axi_awid(vip_S02_AXI_M_AXI_AWID),
        .m_axi_awlen(vip_S02_AXI_M_AXI_AWLEN),
        .m_axi_awlock(vip_S02_AXI_M_AXI_AWLOCK),
        .m_axi_awprot(vip_S02_AXI_M_AXI_AWPROT),
        .m_axi_awqos(vip_S02_AXI_M_AXI_AWQOS),
        .m_axi_awready(vip_S02_AXI_M_AXI_AWREADY),
        .m_axi_awregion(NLW_vip_s02_axi_m_axi_awregion_UNCONNECTED[3:0]),
        .m_axi_awvalid(vip_S02_AXI_M_AXI_AWVALID),
        .m_axi_bid(vip_S02_AXI_M_AXI_BID),
        .m_axi_bready(vip_S02_AXI_M_AXI_BREADY),
        .m_axi_bresp(vip_S02_AXI_M_AXI_BRESP),
        .m_axi_bvalid(vip_S02_AXI_M_AXI_BVALID),
        .m_axi_rdata(vip_S02_AXI_M_AXI_RDATA),
        .m_axi_rid(vip_S02_AXI_M_AXI_RID),
        .m_axi_rlast(vip_S02_AXI_M_AXI_RLAST),
        .m_axi_rready(vip_S02_AXI_M_AXI_RREADY),
        .m_axi_rresp(vip_S02_AXI_M_AXI_RRESP),
        .m_axi_rvalid(vip_S02_AXI_M_AXI_RVALID),
        .m_axi_wdata(vip_S02_AXI_M_AXI_WDATA),
        .m_axi_wlast(vip_S02_AXI_M_AXI_WLAST),
        .m_axi_wready(vip_S02_AXI_M_AXI_WREADY),
        .m_axi_wstrb(vip_S02_AXI_M_AXI_WSTRB),
        .m_axi_wvalid(vip_S02_AXI_M_AXI_WVALID),
        .s_axi_araddr(S02_AXI_araddr),
        .s_axi_arburst(S02_AXI_arburst),
        .s_axi_arcache(S02_AXI_arcache),
        .s_axi_arid(S02_AXI_arid),
        .s_axi_arlen(S02_AXI_arlen),
        .s_axi_arlock(S02_AXI_arlock),
        .s_axi_arprot(S02_AXI_arprot),
        .s_axi_arqos(S02_AXI_arqos),
        .s_axi_arready(S02_AXI_arready),
        .s_axi_arregion(S02_AXI_arregion),
        .s_axi_arvalid(S02_AXI_arvalid),
        .s_axi_awaddr(S02_AXI_awaddr),
        .s_axi_awburst(S02_AXI_awburst),
        .s_axi_awcache(S02_AXI_awcache),
        .s_axi_awid(S02_AXI_awid),
        .s_axi_awlen(S02_AXI_awlen),
        .s_axi_awlock(S02_AXI_awlock),
        .s_axi_awprot(S02_AXI_awprot),
        .s_axi_awqos(S02_AXI_awqos),
        .s_axi_awready(S02_AXI_awready),
        .s_axi_awregion(S02_AXI_awregion),
        .s_axi_awvalid(S02_AXI_awvalid),
        .s_axi_bid(S02_AXI_bid),
        .s_axi_bready(S02_AXI_bready),
        .s_axi_bresp(S02_AXI_bresp),
        .s_axi_bvalid(S02_AXI_bvalid),
        .s_axi_rdata(S02_AXI_rdata),
        .s_axi_rid(S02_AXI_rid),
        .s_axi_rlast(S02_AXI_rlast),
        .s_axi_rready(S02_AXI_rready),
        .s_axi_rresp(S02_AXI_rresp),
        .s_axi_rvalid(S02_AXI_rvalid),
        .s_axi_wdata(S02_AXI_wdata),
        .s_axi_wlast(S02_AXI_wlast),
        .s_axi_wready(S02_AXI_wready),
        .s_axi_wstrb(S02_AXI_wstrb),
        .s_axi_wvalid(S02_AXI_wvalid));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S04_AXI_0 vip_s04_axi
       (.aclk(aclk),
        .aresetn(interconnect_aresetn),
        .m_axi_araddr(vip_S04_AXI_M_AXI_ARADDR),
        .m_axi_arcache(vip_S04_AXI_M_AXI_ARCACHE),
        .m_axi_arlen(vip_S04_AXI_M_AXI_ARLEN),
        .m_axi_arlock(vip_S04_AXI_M_AXI_ARLOCK),
        .m_axi_arprot(vip_S04_AXI_M_AXI_ARPROT),
        .m_axi_arqos(vip_S04_AXI_M_AXI_ARQOS),
        .m_axi_arready(vip_S04_AXI_M_AXI_ARREADY),
        .m_axi_arregion(NLW_vip_s04_axi_m_axi_arregion_UNCONNECTED[3:0]),
        .m_axi_arvalid(vip_S04_AXI_M_AXI_ARVALID),
        .m_axi_awaddr(vip_S04_AXI_M_AXI_AWADDR),
        .m_axi_awcache(vip_S04_AXI_M_AXI_AWCACHE),
        .m_axi_awlen(vip_S04_AXI_M_AXI_AWLEN),
        .m_axi_awlock(vip_S04_AXI_M_AXI_AWLOCK),
        .m_axi_awprot(vip_S04_AXI_M_AXI_AWPROT),
        .m_axi_awqos(vip_S04_AXI_M_AXI_AWQOS),
        .m_axi_awready(vip_S04_AXI_M_AXI_AWREADY),
        .m_axi_awregion(NLW_vip_s04_axi_m_axi_awregion_UNCONNECTED[3:0]),
        .m_axi_awvalid(vip_S04_AXI_M_AXI_AWVALID),
        .m_axi_bready(vip_S04_AXI_M_AXI_BREADY),
        .m_axi_bresp(vip_S04_AXI_M_AXI_BRESP),
        .m_axi_bvalid(vip_S04_AXI_M_AXI_BVALID),
        .m_axi_rdata(vip_S04_AXI_M_AXI_RDATA),
        .m_axi_rlast(vip_S04_AXI_M_AXI_RLAST),
        .m_axi_rready(vip_S04_AXI_M_AXI_RREADY),
        .m_axi_rresp(vip_S04_AXI_M_AXI_RRESP),
        .m_axi_rvalid(vip_S04_AXI_M_AXI_RVALID),
        .m_axi_wdata(vip_S04_AXI_M_AXI_WDATA),
        .m_axi_wlast(vip_S04_AXI_M_AXI_WLAST),
        .m_axi_wready(vip_S04_AXI_M_AXI_WREADY),
        .m_axi_wstrb(vip_S04_AXI_M_AXI_WSTRB),
        .m_axi_wvalid(vip_S04_AXI_M_AXI_WVALID),
        .s_axi_araddr(S04_AXI_araddr),
        .s_axi_arcache(S04_AXI_arcache),
        .s_axi_arlen(S04_AXI_arlen),
        .s_axi_arlock(S04_AXI_arlock),
        .s_axi_arprot(S04_AXI_arprot),
        .s_axi_arqos(S04_AXI_arqos),
        .s_axi_arready(S04_AXI_arready),
        .s_axi_arregion(S04_AXI_arregion),
        .s_axi_arvalid(S04_AXI_arvalid),
        .s_axi_awaddr(S04_AXI_awaddr),
        .s_axi_awcache(S04_AXI_awcache),
        .s_axi_awlen(S04_AXI_awlen),
        .s_axi_awlock(S04_AXI_awlock),
        .s_axi_awprot(S04_AXI_awprot),
        .s_axi_awqos(S04_AXI_awqos),
        .s_axi_awready(S04_AXI_awready),
        .s_axi_awregion(S04_AXI_awregion),
        .s_axi_awvalid(S04_AXI_awvalid),
        .s_axi_bready(S04_AXI_bready),
        .s_axi_bresp(S04_AXI_bresp),
        .s_axi_bvalid(S04_AXI_bvalid),
        .s_axi_rdata(S04_AXI_rdata),
        .s_axi_rlast(S04_AXI_rlast),
        .s_axi_rready(S04_AXI_rready),
        .s_axi_rresp(S04_AXI_rresp),
        .s_axi_rvalid(S04_AXI_rvalid),
        .s_axi_wdata(S04_AXI_wdata),
        .s_axi_wlast(S04_AXI_wlast),
        .s_axi_wready(S04_AXI_wready),
        .s_axi_wstrb(S04_AXI_wstrb),
        .s_axi_wvalid(S04_AXI_wvalid));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S05_AXI_0 vip_s05_axi
       (.aclk(aclk),
        .aresetn(interconnect_aresetn),
        .m_axi_araddr(vip_S05_AXI_M_AXI_ARADDR),
        .m_axi_arcache(vip_S05_AXI_M_AXI_ARCACHE),
        .m_axi_arlen(vip_S05_AXI_M_AXI_ARLEN),
        .m_axi_arlock(vip_S05_AXI_M_AXI_ARLOCK),
        .m_axi_arprot(vip_S05_AXI_M_AXI_ARPROT),
        .m_axi_arqos(vip_S05_AXI_M_AXI_ARQOS),
        .m_axi_arready(vip_S05_AXI_M_AXI_ARREADY),
        .m_axi_arregion(NLW_vip_s05_axi_m_axi_arregion_UNCONNECTED[3:0]),
        .m_axi_arvalid(vip_S05_AXI_M_AXI_ARVALID),
        .m_axi_awaddr(vip_S05_AXI_M_AXI_AWADDR),
        .m_axi_awcache(vip_S05_AXI_M_AXI_AWCACHE),
        .m_axi_awlen(vip_S05_AXI_M_AXI_AWLEN),
        .m_axi_awlock(vip_S05_AXI_M_AXI_AWLOCK),
        .m_axi_awprot(vip_S05_AXI_M_AXI_AWPROT),
        .m_axi_awqos(vip_S05_AXI_M_AXI_AWQOS),
        .m_axi_awready(vip_S05_AXI_M_AXI_AWREADY),
        .m_axi_awregion(NLW_vip_s05_axi_m_axi_awregion_UNCONNECTED[3:0]),
        .m_axi_awvalid(vip_S05_AXI_M_AXI_AWVALID),
        .m_axi_bready(vip_S05_AXI_M_AXI_BREADY),
        .m_axi_bresp(vip_S05_AXI_M_AXI_BRESP),
        .m_axi_bvalid(vip_S05_AXI_M_AXI_BVALID),
        .m_axi_rdata(vip_S05_AXI_M_AXI_RDATA),
        .m_axi_rlast(vip_S05_AXI_M_AXI_RLAST),
        .m_axi_rready(vip_S05_AXI_M_AXI_RREADY),
        .m_axi_rresp(vip_S05_AXI_M_AXI_RRESP),
        .m_axi_rvalid(vip_S05_AXI_M_AXI_RVALID),
        .m_axi_wdata(vip_S05_AXI_M_AXI_WDATA),
        .m_axi_wlast(vip_S05_AXI_M_AXI_WLAST),
        .m_axi_wready(vip_S05_AXI_M_AXI_WREADY),
        .m_axi_wstrb(vip_S05_AXI_M_AXI_WSTRB),
        .m_axi_wvalid(vip_S05_AXI_M_AXI_WVALID),
        .s_axi_araddr(S05_AXI_araddr),
        .s_axi_arcache(S05_AXI_arcache),
        .s_axi_arlen(S05_AXI_arlen),
        .s_axi_arlock(S05_AXI_arlock),
        .s_axi_arprot(S05_AXI_arprot),
        .s_axi_arqos(S05_AXI_arqos),
        .s_axi_arready(S05_AXI_arready),
        .s_axi_arregion(S05_AXI_arregion),
        .s_axi_arvalid(S05_AXI_arvalid),
        .s_axi_awaddr(S05_AXI_awaddr),
        .s_axi_awcache(S05_AXI_awcache),
        .s_axi_awlen(S05_AXI_awlen),
        .s_axi_awlock(S05_AXI_awlock),
        .s_axi_awprot(S05_AXI_awprot),
        .s_axi_awqos(S05_AXI_awqos),
        .s_axi_awready(S05_AXI_awready),
        .s_axi_awregion(S05_AXI_awregion),
        .s_axi_awvalid(S05_AXI_awvalid),
        .s_axi_bready(S05_AXI_bready),
        .s_axi_bresp(S05_AXI_bresp),
        .s_axi_bvalid(S05_AXI_bvalid),
        .s_axi_rdata(S05_AXI_rdata),
        .s_axi_rlast(S05_AXI_rlast),
        .s_axi_rready(S05_AXI_rready),
        .s_axi_rresp(S05_AXI_rresp),
        .s_axi_rvalid(S05_AXI_rvalid),
        .s_axi_wdata(S05_AXI_wdata),
        .s_axi_wlast(S05_AXI_wlast),
        .s_axi_wready(S05_AXI_wready),
        .s_axi_wstrb(S05_AXI_wstrb),
        .s_axi_wvalid(S05_AXI_wvalid));
endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_memory_imp_1K98CM8
   (ddr4_mem_calib_vec,
    ddr4_mem_calib_complete,
    DDR4_MEM01_act_n,
    DDR4_MEM01_adr,
    DDR4_MEM01_ba,
    DDR4_MEM01_bg,
    DDR4_MEM01_cke,
    DDR4_MEM01_odt,
    DDR4_MEM01_cs_n,
    DDR4_MEM01_ck_t,
    DDR4_MEM01_ck_c,
    DDR4_MEM01_reset_n,
    DDR4_MEM01_par,
    ddr4_mem01_ui_clk,
    S_AXI_CTRL_awready,
    S_AXI_CTRL_wready,
    S_AXI_CTRL_bresp,
    S_AXI_CTRL_bvalid,
    S_AXI_CTRL_arready,
    S_AXI_CTRL_rdata,
    S_AXI_CTRL_rresp,
    S_AXI_CTRL_rvalid,
    S_AXI_awready,
    S_AXI_wready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_arready,
    S_AXI_rdata,
    S_AXI_rresp,
    S_AXI_rlast,
    S_AXI_rvalid,
    DDR4_MEM01_dq,
    DDR4_MEM01_dqs_c,
    DDR4_MEM01_dqs_t,
    ddr4_mem01_sys_rst,
    DDR4_MEM01_DIFF_CLK_clk_p,
    DDR4_MEM01_DIFF_CLK_clk_n,
    aclk1,
    S_AXI_CTRL_awaddr,
    S_AXI_CTRL_awprot,
    S_AXI_CTRL_awvalid,
    S_AXI_CTRL_wdata,
    S_AXI_CTRL_wstrb,
    S_AXI_CTRL_wvalid,
    S_AXI_CTRL_bready,
    S_AXI_CTRL_araddr,
    S_AXI_CTRL_arprot,
    S_AXI_CTRL_arvalid,
    S_AXI_CTRL_rready,
    aresetn,
    S_AXI_awaddr,
    S_AXI_awlen,
    S_AXI_awburst,
    S_AXI_awlock,
    S_AXI_awcache,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awvalid,
    S_AXI_wdata,
    S_AXI_wstrb,
    S_AXI_wlast,
    S_AXI_wvalid,
    S_AXI_bready,
    S_AXI_araddr,
    S_AXI_arlen,
    S_AXI_arburst,
    S_AXI_arlock,
    S_AXI_arcache,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arvalid,
    S_AXI_rready);
  output [2:0]ddr4_mem_calib_vec;
  output ddr4_mem_calib_complete;
  output DDR4_MEM01_act_n;
  output [16:0]DDR4_MEM01_adr;
  output [1:0]DDR4_MEM01_ba;
  output [1:0]DDR4_MEM01_bg;
  output [0:0]DDR4_MEM01_cke;
  output [0:0]DDR4_MEM01_odt;
  output [0:0]DDR4_MEM01_cs_n;
  output [0:0]DDR4_MEM01_ck_t;
  output [0:0]DDR4_MEM01_ck_c;
  output DDR4_MEM01_reset_n;
  output DDR4_MEM01_par;
  output ddr4_mem01_ui_clk;
  output S_AXI_CTRL_awready;
  output S_AXI_CTRL_wready;
  output [1:0]S_AXI_CTRL_bresp;
  output S_AXI_CTRL_bvalid;
  output S_AXI_CTRL_arready;
  output [31:0]S_AXI_CTRL_rdata;
  output [1:0]S_AXI_CTRL_rresp;
  output S_AXI_CTRL_rvalid;
  output S_AXI_awready;
  output S_AXI_wready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output S_AXI_arready;
  output [511:0]S_AXI_rdata;
  output [1:0]S_AXI_rresp;
  output S_AXI_rlast;
  output S_AXI_rvalid;
  inout [71:0]DDR4_MEM01_dq;
  inout [17:0]DDR4_MEM01_dqs_c;
  inout [17:0]DDR4_MEM01_dqs_t;
  input ddr4_mem01_sys_rst;
  input DDR4_MEM01_DIFF_CLK_clk_p;
  input DDR4_MEM01_DIFF_CLK_clk_n;
  input aclk1;
  input [24:0]S_AXI_CTRL_awaddr;
  input [2:0]S_AXI_CTRL_awprot;
  input S_AXI_CTRL_awvalid;
  input [31:0]S_AXI_CTRL_wdata;
  input [3:0]S_AXI_CTRL_wstrb;
  input S_AXI_CTRL_wvalid;
  input S_AXI_CTRL_bready;
  input [24:0]S_AXI_CTRL_araddr;
  input [2:0]S_AXI_CTRL_arprot;
  input S_AXI_CTRL_arvalid;
  input S_AXI_CTRL_rready;
  input aresetn;
  input [33:0]S_AXI_awaddr;
  input [7:0]S_AXI_awlen;
  input [1:0]S_AXI_awburst;
  input [0:0]S_AXI_awlock;
  input [3:0]S_AXI_awcache;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  input S_AXI_awvalid;
  input [511:0]S_AXI_wdata;
  input [63:0]S_AXI_wstrb;
  input S_AXI_wlast;
  input S_AXI_wvalid;
  input S_AXI_bready;
  input [33:0]S_AXI_araddr;
  input [7:0]S_AXI_arlen;
  input [1:0]S_AXI_arburst;
  input [0:0]S_AXI_arlock;
  input [3:0]S_AXI_arcache;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  input S_AXI_arvalid;
  input S_AXI_rready;

  wire DDR4_MEM01_DIFF_CLK_clk_n;
  wire DDR4_MEM01_DIFF_CLK_clk_p;
  wire DDR4_MEM01_act_n;
  wire [16:0]DDR4_MEM01_adr;
  wire [1:0]DDR4_MEM01_ba;
  wire [1:0]DDR4_MEM01_bg;
  wire [0:0]DDR4_MEM01_ck_c;
  wire [0:0]DDR4_MEM01_ck_t;
  wire [0:0]DDR4_MEM01_cke;
  wire [0:0]DDR4_MEM01_cs_n;
  wire [71:0]DDR4_MEM01_dq;
  wire [17:0]DDR4_MEM01_dqs_c;
  wire [17:0]DDR4_MEM01_dqs_t;
  wire [0:0]DDR4_MEM01_odt;
  wire DDR4_MEM01_par;
  wire DDR4_MEM01_reset_n;
  wire [24:0]S_AXI_CTRL_araddr;
  wire [2:0]S_AXI_CTRL_arprot;
  wire S_AXI_CTRL_arready;
  wire S_AXI_CTRL_arvalid;
  wire [24:0]S_AXI_CTRL_awaddr;
  wire [2:0]S_AXI_CTRL_awprot;
  wire S_AXI_CTRL_awready;
  wire S_AXI_CTRL_awvalid;
  wire S_AXI_CTRL_bready;
  wire [1:0]S_AXI_CTRL_bresp;
  wire S_AXI_CTRL_bvalid;
  wire [31:0]S_AXI_CTRL_rdata;
  wire S_AXI_CTRL_rready;
  wire [1:0]S_AXI_CTRL_rresp;
  wire S_AXI_CTRL_rvalid;
  wire [31:0]S_AXI_CTRL_wdata;
  wire S_AXI_CTRL_wready;
  wire [3:0]S_AXI_CTRL_wstrb;
  wire S_AXI_CTRL_wvalid;
  wire [33:0]S_AXI_araddr;
  wire [1:0]S_AXI_arburst;
  wire [3:0]S_AXI_arcache;
  wire [7:0]S_AXI_arlen;
  wire [0:0]S_AXI_arlock;
  wire [2:0]S_AXI_arprot;
  wire [3:0]S_AXI_arqos;
  wire S_AXI_arready;
  wire S_AXI_arvalid;
  wire [33:0]S_AXI_awaddr;
  wire [1:0]S_AXI_awburst;
  wire [3:0]S_AXI_awcache;
  wire [7:0]S_AXI_awlen;
  wire [0:0]S_AXI_awlock;
  wire [2:0]S_AXI_awprot;
  wire [3:0]S_AXI_awqos;
  wire S_AXI_awready;
  wire S_AXI_awvalid;
  wire S_AXI_bready;
  wire [1:0]S_AXI_bresp;
  wire S_AXI_bvalid;
  wire [511:0]S_AXI_rdata;
  wire S_AXI_rlast;
  wire S_AXI_rready;
  wire [1:0]S_AXI_rresp;
  wire S_AXI_rvalid;
  wire [511:0]S_AXI_wdata;
  wire S_AXI_wlast;
  wire S_AXI_wready;
  wire [63:0]S_AXI_wstrb;
  wire S_AXI_wvalid;
  wire aclk1;
  wire aresetn;
  wire calib_concat_dout;
  wire ddr4_mem01_c0_ddr4_ui_clk;
  wire ddr4_mem01_c0_ddr4_ui_clk_sync_rst;
  wire ddr4_mem01_c0_init_calib_complete;
  wire [31:0]ddr4_mem01_ctrl_cc_M_AXI_ARADDR;
  wire ddr4_mem01_ctrl_cc_M_AXI_ARREADY;
  wire ddr4_mem01_ctrl_cc_M_AXI_ARVALID;
  wire [31:0]ddr4_mem01_ctrl_cc_M_AXI_AWADDR;
  wire ddr4_mem01_ctrl_cc_M_AXI_AWREADY;
  wire ddr4_mem01_ctrl_cc_M_AXI_AWVALID;
  wire ddr4_mem01_ctrl_cc_M_AXI_BREADY;
  wire [1:0]ddr4_mem01_ctrl_cc_M_AXI_BRESP;
  wire ddr4_mem01_ctrl_cc_M_AXI_BVALID;
  wire [31:0]ddr4_mem01_ctrl_cc_M_AXI_RDATA;
  wire ddr4_mem01_ctrl_cc_M_AXI_RREADY;
  wire [1:0]ddr4_mem01_ctrl_cc_M_AXI_RRESP;
  wire ddr4_mem01_ctrl_cc_M_AXI_RVALID;
  wire [31:0]ddr4_mem01_ctrl_cc_M_AXI_WDATA;
  wire ddr4_mem01_ctrl_cc_M_AXI_WREADY;
  wire ddr4_mem01_ctrl_cc_M_AXI_WVALID;
  wire ddr4_mem01_sys_rst;
  wire ddr4_mem01_ui_clk;
  wire ddr4_mem_calib_complete;
  wire [2:0]ddr4_mem_calib_vec;
  wire [31:0]interconnect_ddrmem_ctrl_M_AXI_ARADDR;
  wire [2:0]interconnect_ddrmem_ctrl_M_AXI_ARPROT;
  wire interconnect_ddrmem_ctrl_M_AXI_ARREADY;
  wire interconnect_ddrmem_ctrl_M_AXI_ARVALID;
  wire [31:0]interconnect_ddrmem_ctrl_M_AXI_AWADDR;
  wire [2:0]interconnect_ddrmem_ctrl_M_AXI_AWPROT;
  wire interconnect_ddrmem_ctrl_M_AXI_AWREADY;
  wire interconnect_ddrmem_ctrl_M_AXI_AWVALID;
  wire interconnect_ddrmem_ctrl_M_AXI_BREADY;
  wire [1:0]interconnect_ddrmem_ctrl_M_AXI_BRESP;
  wire interconnect_ddrmem_ctrl_M_AXI_BVALID;
  wire [31:0]interconnect_ddrmem_ctrl_M_AXI_RDATA;
  wire interconnect_ddrmem_ctrl_M_AXI_RREADY;
  wire [1:0]interconnect_ddrmem_ctrl_M_AXI_RRESP;
  wire interconnect_ddrmem_ctrl_M_AXI_RVALID;
  wire [31:0]interconnect_ddrmem_ctrl_M_AXI_WDATA;
  wire interconnect_ddrmem_ctrl_M_AXI_WREADY;
  wire [3:0]interconnect_ddrmem_ctrl_M_AXI_WSTRB;
  wire interconnect_ddrmem_ctrl_M_AXI_WVALID;
  wire psr_ctrl_interconnect_interconnect_aresetn;
  wire psr_ddr4_mem01_interconnect_aresetn;
  wire [33:0]vip_DDR4_MEM01_M_AXI_ARADDR;
  wire [1:0]vip_DDR4_MEM01_M_AXI_ARBURST;
  wire [3:0]vip_DDR4_MEM01_M_AXI_ARCACHE;
  wire [7:0]vip_DDR4_MEM01_M_AXI_ARLEN;
  wire vip_DDR4_MEM01_M_AXI_ARLOCK;
  wire [2:0]vip_DDR4_MEM01_M_AXI_ARPROT;
  wire [3:0]vip_DDR4_MEM01_M_AXI_ARQOS;
  wire vip_DDR4_MEM01_M_AXI_ARREADY;
  wire vip_DDR4_MEM01_M_AXI_ARVALID;
  wire [33:0]vip_DDR4_MEM01_M_AXI_AWADDR;
  wire [1:0]vip_DDR4_MEM01_M_AXI_AWBURST;
  wire [3:0]vip_DDR4_MEM01_M_AXI_AWCACHE;
  wire [7:0]vip_DDR4_MEM01_M_AXI_AWLEN;
  wire vip_DDR4_MEM01_M_AXI_AWLOCK;
  wire [2:0]vip_DDR4_MEM01_M_AXI_AWPROT;
  wire [3:0]vip_DDR4_MEM01_M_AXI_AWQOS;
  wire vip_DDR4_MEM01_M_AXI_AWREADY;
  wire vip_DDR4_MEM01_M_AXI_AWVALID;
  wire vip_DDR4_MEM01_M_AXI_BREADY;
  wire [1:0]vip_DDR4_MEM01_M_AXI_BRESP;
  wire vip_DDR4_MEM01_M_AXI_BVALID;
  wire [511:0]vip_DDR4_MEM01_M_AXI_RDATA;
  wire vip_DDR4_MEM01_M_AXI_RLAST;
  wire vip_DDR4_MEM01_M_AXI_RREADY;
  wire [1:0]vip_DDR4_MEM01_M_AXI_RRESP;
  wire vip_DDR4_MEM01_M_AXI_RVALID;
  wire [511:0]vip_DDR4_MEM01_M_AXI_WDATA;
  wire vip_DDR4_MEM01_M_AXI_WLAST;
  wire vip_DDR4_MEM01_M_AXI_WREADY;
  wire [63:0]vip_DDR4_MEM01_M_AXI_WSTRB;
  wire vip_DDR4_MEM01_M_AXI_WVALID;
  wire [31:0]vip_ctrl_DDR4_MEM01_M_AXI_ARADDR;
  wire [2:0]vip_ctrl_DDR4_MEM01_M_AXI_ARPROT;
  wire vip_ctrl_DDR4_MEM01_M_AXI_ARREADY;
  wire vip_ctrl_DDR4_MEM01_M_AXI_ARVALID;
  wire [31:0]vip_ctrl_DDR4_MEM01_M_AXI_AWADDR;
  wire [2:0]vip_ctrl_DDR4_MEM01_M_AXI_AWPROT;
  wire vip_ctrl_DDR4_MEM01_M_AXI_AWREADY;
  wire vip_ctrl_DDR4_MEM01_M_AXI_AWVALID;
  wire vip_ctrl_DDR4_MEM01_M_AXI_BREADY;
  wire [1:0]vip_ctrl_DDR4_MEM01_M_AXI_BRESP;
  wire vip_ctrl_DDR4_MEM01_M_AXI_BVALID;
  wire [31:0]vip_ctrl_DDR4_MEM01_M_AXI_RDATA;
  wire vip_ctrl_DDR4_MEM01_M_AXI_RREADY;
  wire [1:0]vip_ctrl_DDR4_MEM01_M_AXI_RRESP;
  wire vip_ctrl_DDR4_MEM01_M_AXI_RVALID;
  wire [31:0]vip_ctrl_DDR4_MEM01_M_AXI_WDATA;
  wire vip_ctrl_DDR4_MEM01_M_AXI_WREADY;
  wire [3:0]vip_ctrl_DDR4_MEM01_M_AXI_WSTRB;
  wire vip_ctrl_DDR4_MEM01_M_AXI_WVALID;
  wire vip_ui_rst_DDR4_MEM01_rst_out;
  wire NLW_ddr4_mem01_addn_ui_clkout1_UNCONNECTED;
  wire NLW_ddr4_mem01_c0_ddr4_interrupt_UNCONNECTED;
  wire NLW_ddr4_mem01_dbg_clk_UNCONNECTED;
  wire [0:0]NLW_ddr4_mem01_c0_ddr4_s_axi_bid_UNCONNECTED;
  wire [0:0]NLW_ddr4_mem01_c0_ddr4_s_axi_rid_UNCONNECTED;
  wire [511:0]NLW_ddr4_mem01_dbg_bus_UNCONNECTED;
  wire [2:0]NLW_ddr4_mem01_ctrl_cc_m_axi_arprot_UNCONNECTED;
  wire [2:0]NLW_ddr4_mem01_ctrl_cc_m_axi_awprot_UNCONNECTED;
  wire [3:0]NLW_ddr4_mem01_ctrl_cc_m_axi_wstrb_UNCONNECTED;
  wire NLW_psr_ctrl_interconnect_mb_reset_UNCONNECTED;
  wire [0:0]NLW_psr_ctrl_interconnect_bus_struct_reset_UNCONNECTED;
  wire [0:0]NLW_psr_ctrl_interconnect_peripheral_aresetn_UNCONNECTED;
  wire [0:0]NLW_psr_ctrl_interconnect_peripheral_reset_UNCONNECTED;
  wire NLW_psr_ddr4_mem01_mb_reset_UNCONNECTED;
  wire [0:0]NLW_psr_ddr4_mem01_bus_struct_reset_UNCONNECTED;
  wire [0:0]NLW_psr_ddr4_mem01_peripheral_aresetn_UNCONNECTED;
  wire [0:0]NLW_psr_ddr4_mem01_peripheral_reset_UNCONNECTED;

  (* CHECK_LICENSE_TYPE = "bd_d216_calib_concat_0,xlconcat_v2_1_3_xlconcat,{}" *) 
  (* DowngradeIPIdentifiedWarnings = "yes" *) 
  (* X_CORE_INFO = "xlconcat_v2_1_3_xlconcat,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0 calib_concat
       (.In0(ddr4_mem01_c0_init_calib_complete),
        .dout(calib_concat_dout));
  (* X_CORE_INFO = "util_reduced_logic_v2_0_4_util_reduced_logic,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_reduce_0 calib_reduce
       (.Op1(calib_concat_dout),
        .Res(ddr4_mem_calib_complete));
  (* CHECK_LICENSE_TYPE = "bd_d216_calib_vector_concat_0,xlconcat_v2_1_3_xlconcat,{}" *) 
  (* DowngradeIPIdentifiedWarnings = "yes" *) 
  (* X_CORE_INFO = "xlconcat_v2_1_3_xlconcat,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0 calib_vector_concat
       (.In0(1'b0),
        .In1(ddr4_mem01_c0_init_calib_complete),
        .In2(1'b0),
        .dout(ddr4_mem_calib_vec));
  (* X_CORE_INFO = "ddr4_v2_2_9,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_0 ddr4_mem01
       (.addn_ui_clkout1(NLW_ddr4_mem01_addn_ui_clkout1_UNCONNECTED),
        .c0_ddr4_act_n(DDR4_MEM01_act_n),
        .c0_ddr4_adr(DDR4_MEM01_adr),
        .c0_ddr4_aresetn(psr_ddr4_mem01_interconnect_aresetn),
        .c0_ddr4_ba(DDR4_MEM01_ba),
        .c0_ddr4_bg(DDR4_MEM01_bg),
        .c0_ddr4_ck_c(DDR4_MEM01_ck_c),
        .c0_ddr4_ck_t(DDR4_MEM01_ck_t),
        .c0_ddr4_cke(DDR4_MEM01_cke),
        .c0_ddr4_cs_n(DDR4_MEM01_cs_n),
        .c0_ddr4_dq(DDR4_MEM01_dq),
        .c0_ddr4_dqs_c(DDR4_MEM01_dqs_c),
        .c0_ddr4_dqs_t(DDR4_MEM01_dqs_t),
        .c0_ddr4_interrupt(NLW_ddr4_mem01_c0_ddr4_interrupt_UNCONNECTED),
        .c0_ddr4_odt(DDR4_MEM01_odt),
        .c0_ddr4_parity(DDR4_MEM01_par),
        .c0_ddr4_reset_n(DDR4_MEM01_reset_n),
        .c0_ddr4_s_axi_araddr(vip_DDR4_MEM01_M_AXI_ARADDR),
        .c0_ddr4_s_axi_arburst(vip_DDR4_MEM01_M_AXI_ARBURST),
        .c0_ddr4_s_axi_arcache(vip_DDR4_MEM01_M_AXI_ARCACHE),
        .c0_ddr4_s_axi_arid(1'b0),
        .c0_ddr4_s_axi_arlen(vip_DDR4_MEM01_M_AXI_ARLEN),
        .c0_ddr4_s_axi_arlock(vip_DDR4_MEM01_M_AXI_ARLOCK),
        .c0_ddr4_s_axi_arprot(vip_DDR4_MEM01_M_AXI_ARPROT),
        .c0_ddr4_s_axi_arqos(vip_DDR4_MEM01_M_AXI_ARQOS),
        .c0_ddr4_s_axi_arready(vip_DDR4_MEM01_M_AXI_ARREADY),
        .c0_ddr4_s_axi_arsize({1'b1,1'b1,1'b0}),
        .c0_ddr4_s_axi_arvalid(vip_DDR4_MEM01_M_AXI_ARVALID),
        .c0_ddr4_s_axi_awaddr(vip_DDR4_MEM01_M_AXI_AWADDR),
        .c0_ddr4_s_axi_awburst(vip_DDR4_MEM01_M_AXI_AWBURST),
        .c0_ddr4_s_axi_awcache(vip_DDR4_MEM01_M_AXI_AWCACHE),
        .c0_ddr4_s_axi_awid(1'b0),
        .c0_ddr4_s_axi_awlen(vip_DDR4_MEM01_M_AXI_AWLEN),
        .c0_ddr4_s_axi_awlock(vip_DDR4_MEM01_M_AXI_AWLOCK),
        .c0_ddr4_s_axi_awprot(vip_DDR4_MEM01_M_AXI_AWPROT),
        .c0_ddr4_s_axi_awqos(vip_DDR4_MEM01_M_AXI_AWQOS),
        .c0_ddr4_s_axi_awready(vip_DDR4_MEM01_M_AXI_AWREADY),
        .c0_ddr4_s_axi_awsize({1'b1,1'b1,1'b0}),
        .c0_ddr4_s_axi_awvalid(vip_DDR4_MEM01_M_AXI_AWVALID),
        .c0_ddr4_s_axi_bid(NLW_ddr4_mem01_c0_ddr4_s_axi_bid_UNCONNECTED[0]),
        .c0_ddr4_s_axi_bready(vip_DDR4_MEM01_M_AXI_BREADY),
        .c0_ddr4_s_axi_bresp(vip_DDR4_MEM01_M_AXI_BRESP),
        .c0_ddr4_s_axi_bvalid(vip_DDR4_MEM01_M_AXI_BVALID),
        .c0_ddr4_s_axi_ctrl_araddr(ddr4_mem01_ctrl_cc_M_AXI_ARADDR),
        .c0_ddr4_s_axi_ctrl_arready(ddr4_mem01_ctrl_cc_M_AXI_ARREADY),
        .c0_ddr4_s_axi_ctrl_arvalid(ddr4_mem01_ctrl_cc_M_AXI_ARVALID),
        .c0_ddr4_s_axi_ctrl_awaddr(ddr4_mem01_ctrl_cc_M_AXI_AWADDR),
        .c0_ddr4_s_axi_ctrl_awready(ddr4_mem01_ctrl_cc_M_AXI_AWREADY),
        .c0_ddr4_s_axi_ctrl_awvalid(ddr4_mem01_ctrl_cc_M_AXI_AWVALID),
        .c0_ddr4_s_axi_ctrl_bready(ddr4_mem01_ctrl_cc_M_AXI_BREADY),
        .c0_ddr4_s_axi_ctrl_bresp(ddr4_mem01_ctrl_cc_M_AXI_BRESP),
        .c0_ddr4_s_axi_ctrl_bvalid(ddr4_mem01_ctrl_cc_M_AXI_BVALID),
        .c0_ddr4_s_axi_ctrl_rdata(ddr4_mem01_ctrl_cc_M_AXI_RDATA),
        .c0_ddr4_s_axi_ctrl_rready(ddr4_mem01_ctrl_cc_M_AXI_RREADY),
        .c0_ddr4_s_axi_ctrl_rresp(ddr4_mem01_ctrl_cc_M_AXI_RRESP),
        .c0_ddr4_s_axi_ctrl_rvalid(ddr4_mem01_ctrl_cc_M_AXI_RVALID),
        .c0_ddr4_s_axi_ctrl_wdata(ddr4_mem01_ctrl_cc_M_AXI_WDATA),
        .c0_ddr4_s_axi_ctrl_wready(ddr4_mem01_ctrl_cc_M_AXI_WREADY),
        .c0_ddr4_s_axi_ctrl_wvalid(ddr4_mem01_ctrl_cc_M_AXI_WVALID),
        .c0_ddr4_s_axi_rdata(vip_DDR4_MEM01_M_AXI_RDATA),
        .c0_ddr4_s_axi_rid(NLW_ddr4_mem01_c0_ddr4_s_axi_rid_UNCONNECTED[0]),
        .c0_ddr4_s_axi_rlast(vip_DDR4_MEM01_M_AXI_RLAST),
        .c0_ddr4_s_axi_rready(vip_DDR4_MEM01_M_AXI_RREADY),
        .c0_ddr4_s_axi_rresp(vip_DDR4_MEM01_M_AXI_RRESP),
        .c0_ddr4_s_axi_rvalid(vip_DDR4_MEM01_M_AXI_RVALID),
        .c0_ddr4_s_axi_wdata(vip_DDR4_MEM01_M_AXI_WDATA),
        .c0_ddr4_s_axi_wlast(vip_DDR4_MEM01_M_AXI_WLAST),
        .c0_ddr4_s_axi_wready(vip_DDR4_MEM01_M_AXI_WREADY),
        .c0_ddr4_s_axi_wstrb(vip_DDR4_MEM01_M_AXI_WSTRB),
        .c0_ddr4_s_axi_wvalid(vip_DDR4_MEM01_M_AXI_WVALID),
        .c0_ddr4_ui_clk(ddr4_mem01_c0_ddr4_ui_clk),
        .c0_ddr4_ui_clk_sync_rst(ddr4_mem01_c0_ddr4_ui_clk_sync_rst),
        .c0_init_calib_complete(ddr4_mem01_c0_init_calib_complete),
        .c0_sys_clk_n(DDR4_MEM01_DIFF_CLK_clk_n),
        .c0_sys_clk_p(DDR4_MEM01_DIFF_CLK_clk_p),
        .dbg_bus(NLW_ddr4_mem01_dbg_bus_UNCONNECTED[511:0]),
        .dbg_clk(NLW_ddr4_mem01_dbg_clk_UNCONNECTED),
        .sys_rst(ddr4_mem01_sys_rst));
  (* X_CORE_INFO = "axi_clock_converter_v2_1_20_axi_clock_converter,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_ctrl_cc_0 ddr4_mem01_ctrl_cc
       (.m_axi_aclk(ddr4_mem01_ui_clk),
        .m_axi_araddr(ddr4_mem01_ctrl_cc_M_AXI_ARADDR),
        .m_axi_aresetn(psr_ddr4_mem01_interconnect_aresetn),
        .m_axi_arprot(NLW_ddr4_mem01_ctrl_cc_m_axi_arprot_UNCONNECTED[2:0]),
        .m_axi_arready(ddr4_mem01_ctrl_cc_M_AXI_ARREADY),
        .m_axi_arvalid(ddr4_mem01_ctrl_cc_M_AXI_ARVALID),
        .m_axi_awaddr(ddr4_mem01_ctrl_cc_M_AXI_AWADDR),
        .m_axi_awprot(NLW_ddr4_mem01_ctrl_cc_m_axi_awprot_UNCONNECTED[2:0]),
        .m_axi_awready(ddr4_mem01_ctrl_cc_M_AXI_AWREADY),
        .m_axi_awvalid(ddr4_mem01_ctrl_cc_M_AXI_AWVALID),
        .m_axi_bready(ddr4_mem01_ctrl_cc_M_AXI_BREADY),
        .m_axi_bresp(ddr4_mem01_ctrl_cc_M_AXI_BRESP),
        .m_axi_bvalid(ddr4_mem01_ctrl_cc_M_AXI_BVALID),
        .m_axi_rdata(ddr4_mem01_ctrl_cc_M_AXI_RDATA),
        .m_axi_rready(ddr4_mem01_ctrl_cc_M_AXI_RREADY),
        .m_axi_rresp(ddr4_mem01_ctrl_cc_M_AXI_RRESP),
        .m_axi_rvalid(ddr4_mem01_ctrl_cc_M_AXI_RVALID),
        .m_axi_wdata(ddr4_mem01_ctrl_cc_M_AXI_WDATA),
        .m_axi_wready(ddr4_mem01_ctrl_cc_M_AXI_WREADY),
        .m_axi_wstrb(NLW_ddr4_mem01_ctrl_cc_m_axi_wstrb_UNCONNECTED[3:0]),
        .m_axi_wvalid(ddr4_mem01_ctrl_cc_M_AXI_WVALID),
        .s_axi_aclk(aclk1),
        .s_axi_araddr(vip_ctrl_DDR4_MEM01_M_AXI_ARADDR),
        .s_axi_aresetn(psr_ctrl_interconnect_interconnect_aresetn),
        .s_axi_arprot(vip_ctrl_DDR4_MEM01_M_AXI_ARPROT),
        .s_axi_arready(vip_ctrl_DDR4_MEM01_M_AXI_ARREADY),
        .s_axi_arvalid(vip_ctrl_DDR4_MEM01_M_AXI_ARVALID),
        .s_axi_awaddr(vip_ctrl_DDR4_MEM01_M_AXI_AWADDR),
        .s_axi_awprot(vip_ctrl_DDR4_MEM01_M_AXI_AWPROT),
        .s_axi_awready(vip_ctrl_DDR4_MEM01_M_AXI_AWREADY),
        .s_axi_awvalid(vip_ctrl_DDR4_MEM01_M_AXI_AWVALID),
        .s_axi_bready(vip_ctrl_DDR4_MEM01_M_AXI_BREADY),
        .s_axi_bresp(vip_ctrl_DDR4_MEM01_M_AXI_BRESP),
        .s_axi_bvalid(vip_ctrl_DDR4_MEM01_M_AXI_BVALID),
        .s_axi_rdata(vip_ctrl_DDR4_MEM01_M_AXI_RDATA),
        .s_axi_rready(vip_ctrl_DDR4_MEM01_M_AXI_RREADY),
        .s_axi_rresp(vip_ctrl_DDR4_MEM01_M_AXI_RRESP),
        .s_axi_rvalid(vip_ctrl_DDR4_MEM01_M_AXI_RVALID),
        .s_axi_wdata(vip_ctrl_DDR4_MEM01_M_AXI_WDATA),
        .s_axi_wready(vip_ctrl_DDR4_MEM01_M_AXI_WREADY),
        .s_axi_wstrb(vip_ctrl_DDR4_MEM01_M_AXI_WSTRB),
        .s_axi_wvalid(vip_ctrl_DDR4_MEM01_M_AXI_WVALID));
  (* X_CORE_INFO = "axi_mmu_v2_1_19_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_ddrmem_ctrl_0 interconnect_ddrmem_ctrl
       (.aclk(aclk1),
        .aresetn(psr_ctrl_interconnect_interconnect_aresetn),
        .m_axi_araddr(interconnect_ddrmem_ctrl_M_AXI_ARADDR),
        .m_axi_arprot(interconnect_ddrmem_ctrl_M_AXI_ARPROT),
        .m_axi_arready(interconnect_ddrmem_ctrl_M_AXI_ARREADY),
        .m_axi_arvalid(interconnect_ddrmem_ctrl_M_AXI_ARVALID),
        .m_axi_awaddr(interconnect_ddrmem_ctrl_M_AXI_AWADDR),
        .m_axi_awprot(interconnect_ddrmem_ctrl_M_AXI_AWPROT),
        .m_axi_awready(interconnect_ddrmem_ctrl_M_AXI_AWREADY),
        .m_axi_awvalid(interconnect_ddrmem_ctrl_M_AXI_AWVALID),
        .m_axi_bready(interconnect_ddrmem_ctrl_M_AXI_BREADY),
        .m_axi_bresp(interconnect_ddrmem_ctrl_M_AXI_BRESP),
        .m_axi_bvalid(interconnect_ddrmem_ctrl_M_AXI_BVALID),
        .m_axi_rdata(interconnect_ddrmem_ctrl_M_AXI_RDATA),
        .m_axi_rready(interconnect_ddrmem_ctrl_M_AXI_RREADY),
        .m_axi_rresp(interconnect_ddrmem_ctrl_M_AXI_RRESP),
        .m_axi_rvalid(interconnect_ddrmem_ctrl_M_AXI_RVALID),
        .m_axi_wdata(interconnect_ddrmem_ctrl_M_AXI_WDATA),
        .m_axi_wready(interconnect_ddrmem_ctrl_M_AXI_WREADY),
        .m_axi_wstrb(interconnect_ddrmem_ctrl_M_AXI_WSTRB),
        .m_axi_wvalid(interconnect_ddrmem_ctrl_M_AXI_WVALID),
        .s_axi_araddr(S_AXI_CTRL_araddr),
        .s_axi_arprot(S_AXI_CTRL_arprot),
        .s_axi_arready(S_AXI_CTRL_arready),
        .s_axi_arvalid(S_AXI_CTRL_arvalid),
        .s_axi_awaddr(S_AXI_CTRL_awaddr),
        .s_axi_awprot(S_AXI_CTRL_awprot),
        .s_axi_awready(S_AXI_CTRL_awready),
        .s_axi_awvalid(S_AXI_CTRL_awvalid),
        .s_axi_bready(S_AXI_CTRL_bready),
        .s_axi_bresp(S_AXI_CTRL_bresp),
        .s_axi_bvalid(S_AXI_CTRL_bvalid),
        .s_axi_rdata(S_AXI_CTRL_rdata),
        .s_axi_rready(S_AXI_CTRL_rready),
        .s_axi_rresp(S_AXI_CTRL_rresp),
        .s_axi_rvalid(S_AXI_CTRL_rvalid),
        .s_axi_wdata(S_AXI_CTRL_wdata),
        .s_axi_wready(S_AXI_CTRL_wready),
        .s_axi_wstrb(S_AXI_CTRL_wstrb),
        .s_axi_wvalid(S_AXI_CTRL_wvalid));
  (* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ctrl_interconnect_0 psr_ctrl_interconnect
       (.aux_reset_in(1'b1),
        .bus_struct_reset(NLW_psr_ctrl_interconnect_bus_struct_reset_UNCONNECTED[0]),
        .dcm_locked(1'b1),
        .ext_reset_in(aresetn),
        .interconnect_aresetn(psr_ctrl_interconnect_interconnect_aresetn),
        .mb_debug_sys_rst(1'b0),
        .mb_reset(NLW_psr_ctrl_interconnect_mb_reset_UNCONNECTED),
        .peripheral_aresetn(NLW_psr_ctrl_interconnect_peripheral_aresetn_UNCONNECTED[0]),
        .peripheral_reset(NLW_psr_ctrl_interconnect_peripheral_reset_UNCONNECTED[0]),
        .slowest_sync_clk(aclk1));
  (* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ddr4_mem01_0 psr_ddr4_mem01
       (.aux_reset_in(1'b1),
        .bus_struct_reset(NLW_psr_ddr4_mem01_bus_struct_reset_UNCONNECTED[0]),
        .dcm_locked(1'b1),
        .ext_reset_in(vip_ui_rst_DDR4_MEM01_rst_out),
        .interconnect_aresetn(psr_ddr4_mem01_interconnect_aresetn),
        .mb_debug_sys_rst(1'b0),
        .mb_reset(NLW_psr_ddr4_mem01_mb_reset_UNCONNECTED),
        .peripheral_aresetn(NLW_psr_ddr4_mem01_peripheral_aresetn_UNCONNECTED[0]),
        .peripheral_reset(NLW_psr_ddr4_mem01_peripheral_reset_UNCONNECTED[0]),
        .slowest_sync_clk(ddr4_mem01_ui_clk));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_DDR4_MEM01_0 vip_DDR4_MEM01
       (.aclk(ddr4_mem01_ui_clk),
        .aresetn(psr_ddr4_mem01_interconnect_aresetn),
        .m_axi_araddr(vip_DDR4_MEM01_M_AXI_ARADDR),
        .m_axi_arburst(vip_DDR4_MEM01_M_AXI_ARBURST),
        .m_axi_arcache(vip_DDR4_MEM01_M_AXI_ARCACHE),
        .m_axi_arlen(vip_DDR4_MEM01_M_AXI_ARLEN),
        .m_axi_arlock(vip_DDR4_MEM01_M_AXI_ARLOCK),
        .m_axi_arprot(vip_DDR4_MEM01_M_AXI_ARPROT),
        .m_axi_arqos(vip_DDR4_MEM01_M_AXI_ARQOS),
        .m_axi_arready(vip_DDR4_MEM01_M_AXI_ARREADY),
        .m_axi_arvalid(vip_DDR4_MEM01_M_AXI_ARVALID),
        .m_axi_awaddr(vip_DDR4_MEM01_M_AXI_AWADDR),
        .m_axi_awburst(vip_DDR4_MEM01_M_AXI_AWBURST),
        .m_axi_awcache(vip_DDR4_MEM01_M_AXI_AWCACHE),
        .m_axi_awlen(vip_DDR4_MEM01_M_AXI_AWLEN),
        .m_axi_awlock(vip_DDR4_MEM01_M_AXI_AWLOCK),
        .m_axi_awprot(vip_DDR4_MEM01_M_AXI_AWPROT),
        .m_axi_awqos(vip_DDR4_MEM01_M_AXI_AWQOS),
        .m_axi_awready(vip_DDR4_MEM01_M_AXI_AWREADY),
        .m_axi_awvalid(vip_DDR4_MEM01_M_AXI_AWVALID),
        .m_axi_bready(vip_DDR4_MEM01_M_AXI_BREADY),
        .m_axi_bresp(vip_DDR4_MEM01_M_AXI_BRESP),
        .m_axi_bvalid(vip_DDR4_MEM01_M_AXI_BVALID),
        .m_axi_rdata(vip_DDR4_MEM01_M_AXI_RDATA),
        .m_axi_rlast(vip_DDR4_MEM01_M_AXI_RLAST),
        .m_axi_rready(vip_DDR4_MEM01_M_AXI_RREADY),
        .m_axi_rresp(vip_DDR4_MEM01_M_AXI_RRESP),
        .m_axi_rvalid(vip_DDR4_MEM01_M_AXI_RVALID),
        .m_axi_wdata(vip_DDR4_MEM01_M_AXI_WDATA),
        .m_axi_wlast(vip_DDR4_MEM01_M_AXI_WLAST),
        .m_axi_wready(vip_DDR4_MEM01_M_AXI_WREADY),
        .m_axi_wstrb(vip_DDR4_MEM01_M_AXI_WSTRB),
        .m_axi_wvalid(vip_DDR4_MEM01_M_AXI_WVALID),
        .s_axi_araddr(S_AXI_araddr),
        .s_axi_arburst(S_AXI_arburst),
        .s_axi_arcache(S_AXI_arcache),
        .s_axi_arlen(S_AXI_arlen),
        .s_axi_arlock(S_AXI_arlock),
        .s_axi_arprot(S_AXI_arprot),
        .s_axi_arqos(S_AXI_arqos),
        .s_axi_arready(S_AXI_arready),
        .s_axi_arvalid(S_AXI_arvalid),
        .s_axi_awaddr(S_AXI_awaddr),
        .s_axi_awburst(S_AXI_awburst),
        .s_axi_awcache(S_AXI_awcache),
        .s_axi_awlen(S_AXI_awlen),
        .s_axi_awlock(S_AXI_awlock),
        .s_axi_awprot(S_AXI_awprot),
        .s_axi_awqos(S_AXI_awqos),
        .s_axi_awready(S_AXI_awready),
        .s_axi_awvalid(S_AXI_awvalid),
        .s_axi_bready(S_AXI_bready),
        .s_axi_bresp(S_AXI_bresp),
        .s_axi_bvalid(S_AXI_bvalid),
        .s_axi_rdata(S_AXI_rdata),
        .s_axi_rlast(S_AXI_rlast),
        .s_axi_rready(S_AXI_rready),
        .s_axi_rresp(S_AXI_rresp),
        .s_axi_rvalid(S_AXI_rvalid),
        .s_axi_wdata(S_AXI_wdata),
        .s_axi_wlast(S_AXI_wlast),
        .s_axi_wready(S_AXI_wready),
        .s_axi_wstrb(S_AXI_wstrb),
        .s_axi_wvalid(S_AXI_wvalid));
  (* X_CORE_INFO = "axi_vip_v1_1_7_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ctrl_DDR4_MEM01_0 vip_ctrl_DDR4_MEM01
       (.aclk(aclk1),
        .aresetn(psr_ctrl_interconnect_interconnect_aresetn),
        .m_axi_araddr(vip_ctrl_DDR4_MEM01_M_AXI_ARADDR),
        .m_axi_arprot(vip_ctrl_DDR4_MEM01_M_AXI_ARPROT),
        .m_axi_arready(vip_ctrl_DDR4_MEM01_M_AXI_ARREADY),
        .m_axi_arvalid(vip_ctrl_DDR4_MEM01_M_AXI_ARVALID),
        .m_axi_awaddr(vip_ctrl_DDR4_MEM01_M_AXI_AWADDR),
        .m_axi_awprot(vip_ctrl_DDR4_MEM01_M_AXI_AWPROT),
        .m_axi_awready(vip_ctrl_DDR4_MEM01_M_AXI_AWREADY),
        .m_axi_awvalid(vip_ctrl_DDR4_MEM01_M_AXI_AWVALID),
        .m_axi_bready(vip_ctrl_DDR4_MEM01_M_AXI_BREADY),
        .m_axi_bresp(vip_ctrl_DDR4_MEM01_M_AXI_BRESP),
        .m_axi_bvalid(vip_ctrl_DDR4_MEM01_M_AXI_BVALID),
        .m_axi_rdata(vip_ctrl_DDR4_MEM01_M_AXI_RDATA),
        .m_axi_rready(vip_ctrl_DDR4_MEM01_M_AXI_RREADY),
        .m_axi_rresp(vip_ctrl_DDR4_MEM01_M_AXI_RRESP),
        .m_axi_rvalid(vip_ctrl_DDR4_MEM01_M_AXI_RVALID),
        .m_axi_wdata(vip_ctrl_DDR4_MEM01_M_AXI_WDATA),
        .m_axi_wready(vip_ctrl_DDR4_MEM01_M_AXI_WREADY),
        .m_axi_wstrb(vip_ctrl_DDR4_MEM01_M_AXI_WSTRB),
        .m_axi_wvalid(vip_ctrl_DDR4_MEM01_M_AXI_WVALID),
        .s_axi_araddr(interconnect_ddrmem_ctrl_M_AXI_ARADDR),
        .s_axi_arprot(interconnect_ddrmem_ctrl_M_AXI_ARPROT),
        .s_axi_arready(interconnect_ddrmem_ctrl_M_AXI_ARREADY),
        .s_axi_arvalid(interconnect_ddrmem_ctrl_M_AXI_ARVALID),
        .s_axi_awaddr(interconnect_ddrmem_ctrl_M_AXI_AWADDR),
        .s_axi_awprot(interconnect_ddrmem_ctrl_M_AXI_AWPROT),
        .s_axi_awready(interconnect_ddrmem_ctrl_M_AXI_AWREADY),
        .s_axi_awvalid(interconnect_ddrmem_ctrl_M_AXI_AWVALID),
        .s_axi_bready(interconnect_ddrmem_ctrl_M_AXI_BREADY),
        .s_axi_bresp(interconnect_ddrmem_ctrl_M_AXI_BRESP),
        .s_axi_bvalid(interconnect_ddrmem_ctrl_M_AXI_BVALID),
        .s_axi_rdata(interconnect_ddrmem_ctrl_M_AXI_RDATA),
        .s_axi_rready(interconnect_ddrmem_ctrl_M_AXI_RREADY),
        .s_axi_rresp(interconnect_ddrmem_ctrl_M_AXI_RRESP),
        .s_axi_rvalid(interconnect_ddrmem_ctrl_M_AXI_RVALID),
        .s_axi_wdata(interconnect_ddrmem_ctrl_M_AXI_WDATA),
        .s_axi_wready(interconnect_ddrmem_ctrl_M_AXI_WREADY),
        .s_axi_wstrb(interconnect_ddrmem_ctrl_M_AXI_WSTRB),
        .s_axi_wvalid(interconnect_ddrmem_ctrl_M_AXI_WVALID));
  (* X_CORE_INFO = "clk_vip_v1_0_2_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_clk_DDR4_MEM01_0 vip_ui_clk_DDR4_MEM01
       (.clk_in(ddr4_mem01_c0_ddr4_ui_clk),
        .clk_out(ddr4_mem01_ui_clk));
  (* X_CORE_INFO = "rst_vip_v1_0_4_top,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_rst_DDR4_MEM01_0 vip_ui_rst_DDR4_MEM01
       (.rst_in(ddr4_mem01_c0_ddr4_ui_clk_sync_rst),
        .rst_out(vip_ui_rst_DDR4_MEM01_rst_out));
endmodule

(* CHECK_LICENSE_TYPE = "pfm_dynamic_memory_subsystem_0,bd_d216,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* X_CORE_INFO = "bd_d216,Vivado 2020.1" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (aclk,
    aclk1,
    aclk2,
    aresetn,
    S_AXI_CTRL_awaddr,
    S_AXI_CTRL_awprot,
    S_AXI_CTRL_awvalid,
    S_AXI_CTRL_awready,
    S_AXI_CTRL_wdata,
    S_AXI_CTRL_wstrb,
    S_AXI_CTRL_wvalid,
    S_AXI_CTRL_wready,
    S_AXI_CTRL_bresp,
    S_AXI_CTRL_bvalid,
    S_AXI_CTRL_bready,
    S_AXI_CTRL_araddr,
    S_AXI_CTRL_arprot,
    S_AXI_CTRL_arvalid,
    S_AXI_CTRL_arready,
    S_AXI_CTRL_rdata,
    S_AXI_CTRL_rresp,
    S_AXI_CTRL_rvalid,
    S_AXI_CTRL_rready,
    S00_AXI_awid,
    S00_AXI_awaddr,
    S00_AXI_awlen,
    S00_AXI_awsize,
    S00_AXI_awburst,
    S00_AXI_awlock,
    S00_AXI_awcache,
    S00_AXI_awprot,
    S00_AXI_awregion,
    S00_AXI_awqos,
    S00_AXI_awvalid,
    S00_AXI_awready,
    S00_AXI_wdata,
    S00_AXI_wstrb,
    S00_AXI_wlast,
    S00_AXI_wvalid,
    S00_AXI_wready,
    S00_AXI_bid,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_bready,
    S00_AXI_arid,
    S00_AXI_araddr,
    S00_AXI_arlen,
    S00_AXI_arsize,
    S00_AXI_arburst,
    S00_AXI_arlock,
    S00_AXI_arcache,
    S00_AXI_arprot,
    S00_AXI_arregion,
    S00_AXI_arqos,
    S00_AXI_arvalid,
    S00_AXI_arready,
    S00_AXI_rid,
    S00_AXI_rdata,
    S00_AXI_rresp,
    S00_AXI_rlast,
    S00_AXI_rvalid,
    S00_AXI_rready,
    S01_AXI_awaddr,
    S01_AXI_awlen,
    S01_AXI_awburst,
    S01_AXI_awlock,
    S01_AXI_awcache,
    S01_AXI_awprot,
    S01_AXI_awregion,
    S01_AXI_awqos,
    S01_AXI_awvalid,
    S01_AXI_awready,
    S01_AXI_wdata,
    S01_AXI_wstrb,
    S01_AXI_wlast,
    S01_AXI_wvalid,
    S01_AXI_wready,
    S01_AXI_bresp,
    S01_AXI_bvalid,
    S01_AXI_bready,
    S01_AXI_araddr,
    S01_AXI_arlen,
    S01_AXI_arburst,
    S01_AXI_arlock,
    S01_AXI_arcache,
    S01_AXI_arprot,
    S01_AXI_arregion,
    S01_AXI_arqos,
    S01_AXI_arvalid,
    S01_AXI_arready,
    S01_AXI_rdata,
    S01_AXI_rresp,
    S01_AXI_rlast,
    S01_AXI_rvalid,
    S01_AXI_rready,
    S02_AXI_awaddr,
    S02_AXI_awlen,
    S02_AXI_awburst,
    S02_AXI_awlock,
    S02_AXI_awcache,
    S02_AXI_awprot,
    S02_AXI_awregion,
    S02_AXI_awqos,
    S02_AXI_awvalid,
    S02_AXI_awready,
    S02_AXI_wdata,
    S02_AXI_wstrb,
    S02_AXI_wlast,
    S02_AXI_wvalid,
    S02_AXI_wready,
    S02_AXI_bresp,
    S02_AXI_bvalid,
    S02_AXI_bready,
    S02_AXI_araddr,
    S02_AXI_arlen,
    S02_AXI_arburst,
    S02_AXI_arlock,
    S02_AXI_arcache,
    S02_AXI_arprot,
    S02_AXI_arregion,
    S02_AXI_arqos,
    S02_AXI_arvalid,
    S02_AXI_arready,
    S02_AXI_rdata,
    S02_AXI_rresp,
    S02_AXI_rlast,
    S02_AXI_rvalid,
    S02_AXI_rready,
    S03_AXI_awid,
    S03_AXI_awaddr,
    S03_AXI_awlen,
    S03_AXI_awsize,
    S03_AXI_awburst,
    S03_AXI_awlock,
    S03_AXI_awcache,
    S03_AXI_awprot,
    S03_AXI_awregion,
    S03_AXI_awqos,
    S03_AXI_awvalid,
    S03_AXI_awready,
    S03_AXI_wdata,
    S03_AXI_wstrb,
    S03_AXI_wlast,
    S03_AXI_wvalid,
    S03_AXI_wready,
    S03_AXI_bid,
    S03_AXI_bresp,
    S03_AXI_bvalid,
    S03_AXI_bready,
    S03_AXI_arid,
    S03_AXI_araddr,
    S03_AXI_arlen,
    S03_AXI_arsize,
    S03_AXI_arburst,
    S03_AXI_arlock,
    S03_AXI_arcache,
    S03_AXI_arprot,
    S03_AXI_arregion,
    S03_AXI_arqos,
    S03_AXI_arvalid,
    S03_AXI_arready,
    S03_AXI_rid,
    S03_AXI_rdata,
    S03_AXI_rresp,
    S03_AXI_rlast,
    S03_AXI_rvalid,
    S03_AXI_rready,
    S04_AXI_awaddr,
    S04_AXI_awlen,
    S04_AXI_awlock,
    S04_AXI_awcache,
    S04_AXI_awprot,
    S04_AXI_awregion,
    S04_AXI_awqos,
    S04_AXI_awvalid,
    S04_AXI_awready,
    S04_AXI_wdata,
    S04_AXI_wstrb,
    S04_AXI_wlast,
    S04_AXI_wvalid,
    S04_AXI_wready,
    S04_AXI_bresp,
    S04_AXI_bvalid,
    S04_AXI_bready,
    S04_AXI_araddr,
    S04_AXI_arlen,
    S04_AXI_arlock,
    S04_AXI_arcache,
    S04_AXI_arprot,
    S04_AXI_arregion,
    S04_AXI_arqos,
    S04_AXI_arvalid,
    S04_AXI_arready,
    S04_AXI_rdata,
    S04_AXI_rresp,
    S04_AXI_rlast,
    S04_AXI_rvalid,
    S04_AXI_rready,
    S05_AXI_awaddr,
    S05_AXI_awlen,
    S05_AXI_awlock,
    S05_AXI_awcache,
    S05_AXI_awprot,
    S05_AXI_awregion,
    S05_AXI_awqos,
    S05_AXI_awvalid,
    S05_AXI_awready,
    S05_AXI_wdata,
    S05_AXI_wstrb,
    S05_AXI_wlast,
    S05_AXI_wvalid,
    S05_AXI_wready,
    S05_AXI_bresp,
    S05_AXI_bvalid,
    S05_AXI_bready,
    S05_AXI_araddr,
    S05_AXI_arlen,
    S05_AXI_arlock,
    S05_AXI_arcache,
    S05_AXI_arprot,
    S05_AXI_arregion,
    S05_AXI_arqos,
    S05_AXI_arvalid,
    S05_AXI_arready,
    S05_AXI_rdata,
    S05_AXI_rresp,
    S05_AXI_rlast,
    S05_AXI_rvalid,
    S05_AXI_rready,
    DDR4_MEM00_dq,
    DDR4_MEM00_dqs_t,
    DDR4_MEM00_dqs_c,
    DDR4_MEM00_adr,
    DDR4_MEM00_ba,
    DDR4_MEM00_bg,
    DDR4_MEM00_act_n,
    DDR4_MEM00_reset_n,
    DDR4_MEM00_ck_t,
    DDR4_MEM00_ck_c,
    DDR4_MEM00_cke,
    DDR4_MEM00_cs_n,
    DDR4_MEM00_odt,
    DDR4_MEM00_par,
    DDR4_MEM01_dq,
    DDR4_MEM01_dqs_t,
    DDR4_MEM01_dqs_c,
    DDR4_MEM01_adr,
    DDR4_MEM01_ba,
    DDR4_MEM01_bg,
    DDR4_MEM01_act_n,
    DDR4_MEM01_reset_n,
    DDR4_MEM01_ck_t,
    DDR4_MEM01_ck_c,
    DDR4_MEM01_cke,
    DDR4_MEM01_cs_n,
    DDR4_MEM01_odt,
    DDR4_MEM01_par,
    DDR4_MEM02_dq,
    DDR4_MEM02_dqs_t,
    DDR4_MEM02_dqs_c,
    DDR4_MEM02_adr,
    DDR4_MEM02_ba,
    DDR4_MEM02_bg,
    DDR4_MEM02_act_n,
    DDR4_MEM02_reset_n,
    DDR4_MEM02_ck_t,
    DDR4_MEM02_ck_c,
    DDR4_MEM02_cke,
    DDR4_MEM02_cs_n,
    DDR4_MEM02_odt,
    DDR4_MEM02_par,
    DDR4_MEM00_DIFF_CLK_clk_p,
    DDR4_MEM00_DIFF_CLK_clk_n,
    DDR4_MEM01_DIFF_CLK_clk_p,
    DDR4_MEM01_DIFF_CLK_clk_n,
    DDR4_MEM02_DIFF_CLK_clk_p,
    DDR4_MEM02_DIFF_CLK_clk_n,
    ddr4_mem00_ui_clk,
    ddr4_mem01_ui_clk,
    ddr4_mem02_ui_clk,
    ddr4_mem00_sys_rst,
    ddr4_mem01_sys_rst,
    ddr4_mem02_sys_rst,
    ddr4_mem_calib_complete,
    ddr4_mem_calib_vec,
    M00_AXI_awaddr,
    M00_AXI_awlen,
    M00_AXI_awsize,
    M00_AXI_awburst,
    M00_AXI_awlock,
    M00_AXI_awcache,
    M00_AXI_awprot,
    M00_AXI_awregion,
    M00_AXI_awqos,
    M00_AXI_awvalid,
    M00_AXI_awready,
    M00_AXI_wdata,
    M00_AXI_wstrb,
    M00_AXI_wlast,
    M00_AXI_wvalid,
    M00_AXI_wready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_bready,
    M00_AXI_araddr,
    M00_AXI_arlen,
    M00_AXI_arsize,
    M00_AXI_arburst,
    M00_AXI_arlock,
    M00_AXI_arcache,
    M00_AXI_arprot,
    M00_AXI_arregion,
    M00_AXI_arqos,
    M00_AXI_arvalid,
    M00_AXI_arready,
    M00_AXI_rdata,
    M00_AXI_rresp,
    M00_AXI_rlast,
    M00_AXI_rvalid,
    M00_AXI_rready,
    S01_AXI_arid,
    S01_AXI_awid,
    S01_AXI_bid,
    S01_AXI_rid,
    S02_AXI_arid,
    S02_AXI_awid,
    S02_AXI_bid,
    S02_AXI_rid);
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.aclk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.aclk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, ASSOCIATED_BUSIF S00_AXI:S01_AXI:S02_AXI:S03_AXI:S04_AXI:S05_AXI, INSERT_VIP 0, ASSOCIATED_CLKEN m_sc_aclken" *) input aclk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.aclk1 CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.aclk1, FREQ_HZ 50925925, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, ASSOCIATED_BUSIF S_AXI_CTRL, INSERT_VIP 0" *) input aclk1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.aclk2 CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.aclk2, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, ASSOCIATED_BUSIF M00_AXI, INSERT_VIP 0, ASSOCIATED_CLKEN m_sc_aclken" *) input aclk2;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.aresetn RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.aresetn, POLARITY ACTIVE_LOW, INSERT_VIP 0" *) input aresetn;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWADDR" *) input [24:0]S_AXI_CTRL_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWPROT" *) input [2:0]S_AXI_CTRL_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWVALID" *) input S_AXI_CTRL_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWREADY" *) output S_AXI_CTRL_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WDATA" *) input [31:0]S_AXI_CTRL_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WSTRB" *) input [3:0]S_AXI_CTRL_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WVALID" *) input S_AXI_CTRL_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WREADY" *) output S_AXI_CTRL_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BRESP" *) output [1:0]S_AXI_CTRL_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BVALID" *) output S_AXI_CTRL_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BREADY" *) input S_AXI_CTRL_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARADDR" *) input [24:0]S_AXI_CTRL_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARPROT" *) input [2:0]S_AXI_CTRL_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARVALID" *) input S_AXI_CTRL_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARREADY" *) output S_AXI_CTRL_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RDATA" *) output [31:0]S_AXI_CTRL_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RRESP" *) output [1:0]S_AXI_CTRL_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RVALID" *) output S_AXI_CTRL_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S_AXI_CTRL, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input S_AXI_CTRL_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWID" *) input [3:0]S00_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWADDR" *) input [38:0]S00_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWLEN" *) input [7:0]S00_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWSIZE" *) input [2:0]S00_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWBURST" *) input [1:0]S00_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWLOCK" *) input [0:0]S00_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWCACHE" *) input [3:0]S00_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWPROT" *) input [2:0]S00_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWREGION" *) input [3:0]S00_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWQOS" *) input [3:0]S00_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWVALID" *) input S00_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWREADY" *) output S00_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WDATA" *) input [511:0]S00_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WSTRB" *) input [63:0]S00_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WLAST" *) input S00_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WVALID" *) input S00_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WREADY" *) output S00_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BID" *) output [3:0]S00_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BRESP" *) output [1:0]S00_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BVALID" *) output S00_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BREADY" *) input S00_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARID" *) input [3:0]S00_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARADDR" *) input [38:0]S00_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARLEN" *) input [7:0]S00_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARSIZE" *) input [2:0]S00_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARBURST" *) input [1:0]S00_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARLOCK" *) input [0:0]S00_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARCACHE" *) input [3:0]S00_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARPROT" *) input [2:0]S00_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARREGION" *) input [3:0]S00_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARQOS" *) input [3:0]S00_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARVALID" *) input S00_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARREADY" *) output S00_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RID" *) output [3:0]S00_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RDATA" *) output [511:0]S00_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RRESP" *) output [1:0]S00_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RLAST" *) output S00_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RVALID" *) output S00_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S00_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input S00_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWADDR" *) input [38:0]S01_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWLEN" *) input [7:0]S01_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWBURST" *) input [1:0]S01_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWLOCK" *) input [0:0]S01_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWCACHE" *) input [3:0]S01_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWPROT" *) input [2:0]S01_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWREGION" *) input [3:0]S01_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWQOS" *) input [3:0]S01_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWVALID" *) input S01_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWREADY" *) output S01_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WDATA" *) input [511:0]S01_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WSTRB" *) input [63:0]S01_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WLAST" *) input S01_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WVALID" *) input S01_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI WREADY" *) output S01_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BRESP" *) output [1:0]S01_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BVALID" *) output S01_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BREADY" *) input S01_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARADDR" *) input [38:0]S01_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARLEN" *) input [7:0]S01_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARBURST" *) input [1:0]S01_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARLOCK" *) input [0:0]S01_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARCACHE" *) input [3:0]S01_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARPROT" *) input [2:0]S01_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARREGION" *) input [3:0]S01_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARQOS" *) input [3:0]S01_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARVALID" *) input S01_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARREADY" *) output S01_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RDATA" *) output [511:0]S01_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RRESP" *) output [1:0]S01_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RLAST" *) output S01_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RVALID" *) output S01_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RREADY" *) input S01_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWADDR" *) input [38:0]S02_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWLEN" *) input [7:0]S02_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWBURST" *) input [1:0]S02_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWLOCK" *) input [0:0]S02_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWCACHE" *) input [3:0]S02_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWPROT" *) input [2:0]S02_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWREGION" *) input [3:0]S02_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWQOS" *) input [3:0]S02_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWVALID" *) input S02_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWREADY" *) output S02_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WDATA" *) input [511:0]S02_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WSTRB" *) input [63:0]S02_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WLAST" *) input S02_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WVALID" *) input S02_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI WREADY" *) output S02_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BRESP" *) output [1:0]S02_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BVALID" *) output S02_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BREADY" *) input S02_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARADDR" *) input [38:0]S02_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARLEN" *) input [7:0]S02_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARBURST" *) input [1:0]S02_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARLOCK" *) input [0:0]S02_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARCACHE" *) input [3:0]S02_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARPROT" *) input [2:0]S02_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARREGION" *) input [3:0]S02_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARQOS" *) input [3:0]S02_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARVALID" *) input S02_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARREADY" *) output S02_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RDATA" *) output [511:0]S02_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RRESP" *) output [1:0]S02_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RLAST" *) output S02_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RVALID" *) output S02_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RREADY" *) input S02_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWID" *) input [3:0]S03_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWADDR" *) input [38:0]S03_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWLEN" *) input [7:0]S03_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWSIZE" *) input [2:0]S03_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWBURST" *) input [1:0]S03_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWLOCK" *) input [0:0]S03_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWCACHE" *) input [3:0]S03_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWPROT" *) input [2:0]S03_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWREGION" *) input [3:0]S03_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWQOS" *) input [3:0]S03_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWVALID" *) input S03_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI AWREADY" *) output S03_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WDATA" *) input [511:0]S03_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WSTRB" *) input [63:0]S03_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WLAST" *) input S03_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WVALID" *) input S03_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI WREADY" *) output S03_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BID" *) output [3:0]S03_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BRESP" *) output [1:0]S03_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BVALID" *) output S03_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI BREADY" *) input S03_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARID" *) input [3:0]S03_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARADDR" *) input [38:0]S03_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARLEN" *) input [7:0]S03_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARSIZE" *) input [2:0]S03_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARBURST" *) input [1:0]S03_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARLOCK" *) input [0:0]S03_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARCACHE" *) input [3:0]S03_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARPROT" *) input [2:0]S03_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARREGION" *) input [3:0]S03_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARQOS" *) input [3:0]S03_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARVALID" *) input S03_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI ARREADY" *) output S03_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RID" *) output [3:0]S03_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RDATA" *) output [511:0]S03_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RRESP" *) output [1:0]S03_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RLAST" *) output S03_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RVALID" *) output S03_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S03_AXI RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S03_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input S03_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWADDR" *) input [38:0]S04_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWLEN" *) input [7:0]S04_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWLOCK" *) input [0:0]S04_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWCACHE" *) input [3:0]S04_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWPROT" *) input [2:0]S04_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWREGION" *) input [3:0]S04_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWQOS" *) input [3:0]S04_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWVALID" *) input S04_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI AWREADY" *) output S04_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WDATA" *) input [511:0]S04_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WSTRB" *) input [63:0]S04_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WLAST" *) input S04_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WVALID" *) input S04_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI WREADY" *) output S04_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI BRESP" *) output [1:0]S04_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI BVALID" *) output S04_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI BREADY" *) input S04_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARADDR" *) input [38:0]S04_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARLEN" *) input [7:0]S04_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARLOCK" *) input [0:0]S04_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARCACHE" *) input [3:0]S04_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARPROT" *) input [2:0]S04_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARREGION" *) input [3:0]S04_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARQOS" *) input [3:0]S04_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARVALID" *) input S04_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI ARREADY" *) output S04_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RDATA" *) output [511:0]S04_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RRESP" *) output [1:0]S04_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RLAST" *) output S04_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RVALID" *) output S04_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S04_AXI RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S04_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 0, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input S04_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWADDR" *) input [38:0]S05_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWLEN" *) input [7:0]S05_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWLOCK" *) input [0:0]S05_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWCACHE" *) input [3:0]S05_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWPROT" *) input [2:0]S05_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWREGION" *) input [3:0]S05_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWQOS" *) input [3:0]S05_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWVALID" *) input S05_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI AWREADY" *) output S05_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WDATA" *) input [511:0]S05_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WSTRB" *) input [63:0]S05_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WLAST" *) input S05_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WVALID" *) input S05_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI WREADY" *) output S05_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI BRESP" *) output [1:0]S05_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI BVALID" *) output S05_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI BREADY" *) input S05_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARADDR" *) input [38:0]S05_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARLEN" *) input [7:0]S05_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARLOCK" *) input [0:0]S05_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARCACHE" *) input [3:0]S05_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARPROT" *) input [2:0]S05_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARREGION" *) input [3:0]S05_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARQOS" *) input [3:0]S05_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARVALID" *) input S05_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI ARREADY" *) output S05_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RDATA" *) output [511:0]S05_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RRESP" *) output [1:0]S05_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RLAST" *) output S05_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RVALID" *) output S05_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S05_AXI RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S05_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 0, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input S05_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQ" *) inout [71:0]DDR4_MEM00_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_T" *) inout [17:0]DDR4_MEM00_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_C" *) inout [17:0]DDR4_MEM00_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ADR" *) output [16:0]DDR4_MEM00_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BA" *) output [1:0]DDR4_MEM00_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BG" *) output [1:0]DDR4_MEM00_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ACT_N" *) output DDR4_MEM00_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 RESET_N" *) output DDR4_MEM00_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_T" *) output [0:0]DDR4_MEM00_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_C" *) output [0:0]DDR4_MEM00_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CKE" *) output [0:0]DDR4_MEM00_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CS_N" *) output [0:0]DDR4_MEM00_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ODT" *) output [0:0]DDR4_MEM00_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 PAR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM00, CAN_DEBUG false, TIMEPERIOD_PS 1250, MEMORY_TYPE COMPONENTS, DATA_WIDTH 8, CS_ENABLED true, DATA_MASK_ENABLED true, SLOT Single, MEM_ADDR_MAP ROW_COLUMN_BANK, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME TDM, CAS_LATENCY 11, CAS_WRITE_LATENCY 11" *) output DDR4_MEM00_par;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQ" *) inout [71:0]DDR4_MEM01_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_T" *) inout [17:0]DDR4_MEM01_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_C" *) inout [17:0]DDR4_MEM01_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ADR" *) output [16:0]DDR4_MEM01_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BA" *) output [1:0]DDR4_MEM01_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BG" *) output [1:0]DDR4_MEM01_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ACT_N" *) output DDR4_MEM01_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 RESET_N" *) output DDR4_MEM01_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_T" *) output [0:0]DDR4_MEM01_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_C" *) output [0:0]DDR4_MEM01_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CKE" *) output [0:0]DDR4_MEM01_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CS_N" *) output [0:0]DDR4_MEM01_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ODT" *) output [0:0]DDR4_MEM01_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 PAR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM01, CAN_DEBUG false, TIMEPERIOD_PS 833, MEMORY_TYPE RDIMMs, MEMORY_PART MTA18ASF2G72PZ-2G3, DATA_WIDTH 72, CS_ENABLED true, DATA_MASK_ENABLED NONE, SLOT Single, CUSTOM_PARTS no_file_loaded, MEM_ADDR_MAP ROW_COLUMN_BANK_INTLV, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME RD_PRI_REG, CAS_LATENCY 17, CAS_WRITE_LATENCY 12" *) output DDR4_MEM01_par;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQ" *) inout [71:0]DDR4_MEM02_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_T" *) inout [17:0]DDR4_MEM02_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_C" *) inout [17:0]DDR4_MEM02_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ADR" *) output [16:0]DDR4_MEM02_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BA" *) output [1:0]DDR4_MEM02_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BG" *) output [1:0]DDR4_MEM02_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ACT_N" *) output DDR4_MEM02_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 RESET_N" *) output DDR4_MEM02_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_T" *) output [0:0]DDR4_MEM02_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_C" *) output [0:0]DDR4_MEM02_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CKE" *) output [0:0]DDR4_MEM02_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CS_N" *) output [0:0]DDR4_MEM02_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ODT" *) output [0:0]DDR4_MEM02_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 PAR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM02, CAN_DEBUG false, TIMEPERIOD_PS 1250, MEMORY_TYPE COMPONENTS, DATA_WIDTH 8, CS_ENABLED true, DATA_MASK_ENABLED true, SLOT Single, MEM_ADDR_MAP ROW_COLUMN_BANK, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME TDM, CAS_LATENCY 11, CAS_WRITE_LATENCY 11" *) output DDR4_MEM02_par;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_P" *) input DDR4_MEM00_DIFF_CLK_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM00_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000" *) input DDR4_MEM00_DIFF_CLK_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_P" *) input DDR4_MEM01_DIFF_CLK_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM01_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000" *) input DDR4_MEM01_DIFF_CLK_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_P" *) input DDR4_MEM02_DIFF_CLK_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DDR4_MEM02_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000" *) input DDR4_MEM02_DIFF_CLK_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.ddr4_mem00_ui_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.ddr4_mem00_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem00_ui_clk, INSERT_VIP 0" *) output ddr4_mem00_ui_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.ddr4_mem01_ui_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.ddr4_mem01_ui_clk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, PHASE 0.00, CLK_DOMAIN bd_d216_ddr4_mem01_0_c0_ddr4_ui_clk, INSERT_VIP 0" *) output ddr4_mem01_ui_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.ddr4_mem02_ui_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.ddr4_mem02_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem02_ui_clk, INSERT_VIP 0" *) output ddr4_mem02_ui_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.ddr4_mem00_sys_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.ddr4_mem00_sys_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ddr4_mem00_sys_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.ddr4_mem01_sys_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.ddr4_mem01_sys_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ddr4_mem01_sys_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.ddr4_mem02_sys_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.ddr4_mem02_sys_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ddr4_mem02_sys_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ddr4_mem_calib_complete DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ddr4_mem_calib_complete, LAYERED_METADATA undef" *) output ddr4_mem_calib_complete;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ddr4_mem_calib_vec DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ddr4_mem_calib_vec, LAYERED_METADATA undef, PortWidth 3" *) output [2:0]ddr4_mem_calib_vec;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR" *) output [38:0]M00_AXI_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN" *) output [7:0]M00_AXI_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE" *) output [2:0]M00_AXI_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST" *) output [1:0]M00_AXI_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWLOCK" *) output [0:0]M00_AXI_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWCACHE" *) output [3:0]M00_AXI_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWPROT" *) output [2:0]M00_AXI_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREGION" *) output [3:0]M00_AXI_awregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWQOS" *) output [3:0]M00_AXI_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID" *) output M00_AXI_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY" *) input M00_AXI_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA" *) output [511:0]M00_AXI_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB" *) output [63:0]M00_AXI_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WLAST" *) output M00_AXI_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WVALID" *) output M00_AXI_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WREADY" *) input M00_AXI_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BRESP" *) input [1:0]M00_AXI_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BVALID" *) input M00_AXI_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BREADY" *) output M00_AXI_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR" *) output [38:0]M00_AXI_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN" *) output [7:0]M00_AXI_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE" *) output [2:0]M00_AXI_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST" *) output [1:0]M00_AXI_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARLOCK" *) output [0:0]M00_AXI_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARCACHE" *) output [3:0]M00_AXI_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARPROT" *) output [2:0]M00_AXI_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREGION" *) output [3:0]M00_AXI_arregion;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARQOS" *) output [3:0]M00_AXI_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID" *) output M00_AXI_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY" *) input M00_AXI_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RDATA" *) input [511:0]M00_AXI_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RRESP" *) input [1:0]M00_AXI_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RLAST" *) input M00_AXI_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RVALID" *) input M00_AXI_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 0, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 1, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) output M00_AXI_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI ARID" *) input [3:0]S01_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI AWID" *) input [3:0]S01_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI BID" *) output [3:0]S01_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S01_AXI RID" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S01_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) output [3:0]S01_AXI_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI ARID" *) input [3:0]S02_AXI_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI AWID" *) input [3:0]S02_AXI_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI BID" *) output [3:0]S02_AXI_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S02_AXI RID" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S02_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) output [3:0]S02_AXI_rid;

  wire DDR4_MEM00_DIFF_CLK_clk_n;
  wire DDR4_MEM00_DIFF_CLK_clk_p;
  wire DDR4_MEM00_act_n;
  wire [16:0]DDR4_MEM00_adr;
  wire [1:0]DDR4_MEM00_ba;
  wire [1:0]DDR4_MEM00_bg;
  wire [0:0]DDR4_MEM00_ck_c;
  wire [0:0]DDR4_MEM00_ck_t;
  wire [0:0]DDR4_MEM00_cke;
  wire [0:0]DDR4_MEM00_cs_n;
  wire [71:0]DDR4_MEM00_dq;
  wire [17:0]DDR4_MEM00_dqs_c;
  wire [17:0]DDR4_MEM00_dqs_t;
  wire [0:0]DDR4_MEM00_odt;
  wire DDR4_MEM00_par;
  wire DDR4_MEM00_reset_n;
  wire DDR4_MEM01_DIFF_CLK_clk_n;
  wire DDR4_MEM01_DIFF_CLK_clk_p;
  wire DDR4_MEM01_act_n;
  wire [16:0]DDR4_MEM01_adr;
  wire [1:0]DDR4_MEM01_ba;
  wire [1:0]DDR4_MEM01_bg;
  wire [0:0]DDR4_MEM01_ck_c;
  wire [0:0]DDR4_MEM01_ck_t;
  wire [0:0]DDR4_MEM01_cke;
  wire [0:0]DDR4_MEM01_cs_n;
  wire [71:0]DDR4_MEM01_dq;
  wire [17:0]DDR4_MEM01_dqs_c;
  wire [17:0]DDR4_MEM01_dqs_t;
  wire [0:0]DDR4_MEM01_odt;
  wire DDR4_MEM01_par;
  wire DDR4_MEM01_reset_n;
  wire DDR4_MEM02_DIFF_CLK_clk_n;
  wire DDR4_MEM02_DIFF_CLK_clk_p;
  wire DDR4_MEM02_act_n;
  wire [16:0]DDR4_MEM02_adr;
  wire [1:0]DDR4_MEM02_ba;
  wire [1:0]DDR4_MEM02_bg;
  wire [0:0]DDR4_MEM02_ck_c;
  wire [0:0]DDR4_MEM02_ck_t;
  wire [0:0]DDR4_MEM02_cke;
  wire [0:0]DDR4_MEM02_cs_n;
  wire [71:0]DDR4_MEM02_dq;
  wire [17:0]DDR4_MEM02_dqs_c;
  wire [17:0]DDR4_MEM02_dqs_t;
  wire [0:0]DDR4_MEM02_odt;
  wire DDR4_MEM02_par;
  wire DDR4_MEM02_reset_n;
  wire [38:0]M00_AXI_araddr;
  wire [1:0]M00_AXI_arburst;
  wire [3:0]M00_AXI_arcache;
  wire [7:0]M00_AXI_arlen;
  wire [0:0]M00_AXI_arlock;
  wire [2:0]M00_AXI_arprot;
  wire [3:0]M00_AXI_arqos;
  wire M00_AXI_arready;
  wire [3:0]M00_AXI_arregion;
  wire [2:0]M00_AXI_arsize;
  wire M00_AXI_arvalid;
  wire [38:0]M00_AXI_awaddr;
  wire [1:0]M00_AXI_awburst;
  wire [3:0]M00_AXI_awcache;
  wire [7:0]M00_AXI_awlen;
  wire [0:0]M00_AXI_awlock;
  wire [2:0]M00_AXI_awprot;
  wire [3:0]M00_AXI_awqos;
  wire M00_AXI_awready;
  wire [3:0]M00_AXI_awregion;
  wire [2:0]M00_AXI_awsize;
  wire M00_AXI_awvalid;
  wire M00_AXI_bready;
  wire [1:0]M00_AXI_bresp;
  wire M00_AXI_bvalid;
  wire [511:0]M00_AXI_rdata;
  wire M00_AXI_rlast;
  wire M00_AXI_rready;
  wire [1:0]M00_AXI_rresp;
  wire M00_AXI_rvalid;
  wire [511:0]M00_AXI_wdata;
  wire M00_AXI_wlast;
  wire M00_AXI_wready;
  wire [63:0]M00_AXI_wstrb;
  wire M00_AXI_wvalid;
  wire [38:0]S00_AXI_araddr;
  wire [1:0]S00_AXI_arburst;
  wire [3:0]S00_AXI_arcache;
  wire [3:0]S00_AXI_arid;
  wire [7:0]S00_AXI_arlen;
  wire [0:0]S00_AXI_arlock;
  wire [2:0]S00_AXI_arprot;
  wire [3:0]S00_AXI_arqos;
  wire S00_AXI_arready;
  wire [3:0]S00_AXI_arregion;
  wire [2:0]S00_AXI_arsize;
  wire S00_AXI_arvalid;
  wire [38:0]S00_AXI_awaddr;
  wire [1:0]S00_AXI_awburst;
  wire [3:0]S00_AXI_awcache;
  wire [3:0]S00_AXI_awid;
  wire [7:0]S00_AXI_awlen;
  wire [0:0]S00_AXI_awlock;
  wire [2:0]S00_AXI_awprot;
  wire [3:0]S00_AXI_awqos;
  wire S00_AXI_awready;
  wire [3:0]S00_AXI_awregion;
  wire [2:0]S00_AXI_awsize;
  wire S00_AXI_awvalid;
  wire [3:0]S00_AXI_bid;
  wire S00_AXI_bready;
  wire [1:0]S00_AXI_bresp;
  wire S00_AXI_bvalid;
  wire [511:0]S00_AXI_rdata;
  wire [3:0]S00_AXI_rid;
  wire S00_AXI_rlast;
  wire S00_AXI_rready;
  wire [1:0]S00_AXI_rresp;
  wire S00_AXI_rvalid;
  wire [511:0]S00_AXI_wdata;
  wire S00_AXI_wlast;
  wire S00_AXI_wready;
  wire [63:0]S00_AXI_wstrb;
  wire S00_AXI_wvalid;
  wire [38:0]S01_AXI_araddr;
  wire [1:0]S01_AXI_arburst;
  wire [3:0]S01_AXI_arcache;
  wire [3:0]S01_AXI_arid;
  wire [7:0]S01_AXI_arlen;
  wire [0:0]S01_AXI_arlock;
  wire [2:0]S01_AXI_arprot;
  wire [3:0]S01_AXI_arqos;
  wire S01_AXI_arready;
  wire [3:0]S01_AXI_arregion;
  wire S01_AXI_arvalid;
  wire [38:0]S01_AXI_awaddr;
  wire [1:0]S01_AXI_awburst;
  wire [3:0]S01_AXI_awcache;
  wire [3:0]S01_AXI_awid;
  wire [7:0]S01_AXI_awlen;
  wire [0:0]S01_AXI_awlock;
  wire [2:0]S01_AXI_awprot;
  wire [3:0]S01_AXI_awqos;
  wire S01_AXI_awready;
  wire [3:0]S01_AXI_awregion;
  wire S01_AXI_awvalid;
  wire [3:0]S01_AXI_bid;
  wire S01_AXI_bready;
  wire [1:0]S01_AXI_bresp;
  wire S01_AXI_bvalid;
  wire [511:0]S01_AXI_rdata;
  wire [3:0]S01_AXI_rid;
  wire S01_AXI_rlast;
  wire S01_AXI_rready;
  wire [1:0]S01_AXI_rresp;
  wire S01_AXI_rvalid;
  wire [511:0]S01_AXI_wdata;
  wire S01_AXI_wlast;
  wire S01_AXI_wready;
  wire [63:0]S01_AXI_wstrb;
  wire S01_AXI_wvalid;
  wire [38:0]S02_AXI_araddr;
  wire [1:0]S02_AXI_arburst;
  wire [3:0]S02_AXI_arcache;
  wire [3:0]S02_AXI_arid;
  wire [7:0]S02_AXI_arlen;
  wire [0:0]S02_AXI_arlock;
  wire [2:0]S02_AXI_arprot;
  wire [3:0]S02_AXI_arqos;
  wire S02_AXI_arready;
  wire [3:0]S02_AXI_arregion;
  wire S02_AXI_arvalid;
  wire [38:0]S02_AXI_awaddr;
  wire [1:0]S02_AXI_awburst;
  wire [3:0]S02_AXI_awcache;
  wire [3:0]S02_AXI_awid;
  wire [7:0]S02_AXI_awlen;
  wire [0:0]S02_AXI_awlock;
  wire [2:0]S02_AXI_awprot;
  wire [3:0]S02_AXI_awqos;
  wire S02_AXI_awready;
  wire [3:0]S02_AXI_awregion;
  wire S02_AXI_awvalid;
  wire [3:0]S02_AXI_bid;
  wire S02_AXI_bready;
  wire [1:0]S02_AXI_bresp;
  wire S02_AXI_bvalid;
  wire [511:0]S02_AXI_rdata;
  wire [3:0]S02_AXI_rid;
  wire S02_AXI_rlast;
  wire S02_AXI_rready;
  wire [1:0]S02_AXI_rresp;
  wire S02_AXI_rvalid;
  wire [511:0]S02_AXI_wdata;
  wire S02_AXI_wlast;
  wire S02_AXI_wready;
  wire [63:0]S02_AXI_wstrb;
  wire S02_AXI_wvalid;
  wire [38:0]S03_AXI_araddr;
  wire [1:0]S03_AXI_arburst;
  wire [3:0]S03_AXI_arcache;
  wire [3:0]S03_AXI_arid;
  wire [7:0]S03_AXI_arlen;
  wire [0:0]S03_AXI_arlock;
  wire [2:0]S03_AXI_arprot;
  wire [3:0]S03_AXI_arqos;
  wire S03_AXI_arready;
  wire [3:0]S03_AXI_arregion;
  wire [2:0]S03_AXI_arsize;
  wire S03_AXI_arvalid;
  wire [38:0]S03_AXI_awaddr;
  wire [1:0]S03_AXI_awburst;
  wire [3:0]S03_AXI_awcache;
  wire [3:0]S03_AXI_awid;
  wire [7:0]S03_AXI_awlen;
  wire [0:0]S03_AXI_awlock;
  wire [2:0]S03_AXI_awprot;
  wire [3:0]S03_AXI_awqos;
  wire S03_AXI_awready;
  wire [3:0]S03_AXI_awregion;
  wire [2:0]S03_AXI_awsize;
  wire S03_AXI_awvalid;
  wire [3:0]S03_AXI_bid;
  wire S03_AXI_bready;
  wire [1:0]S03_AXI_bresp;
  wire S03_AXI_bvalid;
  wire [511:0]S03_AXI_rdata;
  wire [3:0]S03_AXI_rid;
  wire S03_AXI_rlast;
  wire S03_AXI_rready;
  wire [1:0]S03_AXI_rresp;
  wire S03_AXI_rvalid;
  wire [511:0]S03_AXI_wdata;
  wire S03_AXI_wlast;
  wire S03_AXI_wready;
  wire [63:0]S03_AXI_wstrb;
  wire S03_AXI_wvalid;
  wire [38:0]S04_AXI_araddr;
  wire [3:0]S04_AXI_arcache;
  wire [7:0]S04_AXI_arlen;
  wire [0:0]S04_AXI_arlock;
  wire [2:0]S04_AXI_arprot;
  wire [3:0]S04_AXI_arqos;
  wire S04_AXI_arready;
  wire [3:0]S04_AXI_arregion;
  wire S04_AXI_arvalid;
  wire [38:0]S04_AXI_awaddr;
  wire [3:0]S04_AXI_awcache;
  wire [7:0]S04_AXI_awlen;
  wire [0:0]S04_AXI_awlock;
  wire [2:0]S04_AXI_awprot;
  wire [3:0]S04_AXI_awqos;
  wire S04_AXI_awready;
  wire [3:0]S04_AXI_awregion;
  wire S04_AXI_awvalid;
  wire S04_AXI_bready;
  wire [1:0]S04_AXI_bresp;
  wire S04_AXI_bvalid;
  wire [511:0]S04_AXI_rdata;
  wire S04_AXI_rlast;
  wire S04_AXI_rready;
  wire [1:0]S04_AXI_rresp;
  wire S04_AXI_rvalid;
  wire [511:0]S04_AXI_wdata;
  wire S04_AXI_wlast;
  wire S04_AXI_wready;
  wire [63:0]S04_AXI_wstrb;
  wire S04_AXI_wvalid;
  wire [38:0]S05_AXI_araddr;
  wire [3:0]S05_AXI_arcache;
  wire [7:0]S05_AXI_arlen;
  wire [0:0]S05_AXI_arlock;
  wire [2:0]S05_AXI_arprot;
  wire [3:0]S05_AXI_arqos;
  wire S05_AXI_arready;
  wire [3:0]S05_AXI_arregion;
  wire S05_AXI_arvalid;
  wire [38:0]S05_AXI_awaddr;
  wire [3:0]S05_AXI_awcache;
  wire [7:0]S05_AXI_awlen;
  wire [0:0]S05_AXI_awlock;
  wire [2:0]S05_AXI_awprot;
  wire [3:0]S05_AXI_awqos;
  wire S05_AXI_awready;
  wire [3:0]S05_AXI_awregion;
  wire S05_AXI_awvalid;
  wire S05_AXI_bready;
  wire [1:0]S05_AXI_bresp;
  wire S05_AXI_bvalid;
  wire [511:0]S05_AXI_rdata;
  wire S05_AXI_rlast;
  wire S05_AXI_rready;
  wire [1:0]S05_AXI_rresp;
  wire S05_AXI_rvalid;
  wire [511:0]S05_AXI_wdata;
  wire S05_AXI_wlast;
  wire S05_AXI_wready;
  wire [63:0]S05_AXI_wstrb;
  wire S05_AXI_wvalid;
  wire [24:0]S_AXI_CTRL_araddr;
  wire [2:0]S_AXI_CTRL_arprot;
  wire S_AXI_CTRL_arready;
  wire S_AXI_CTRL_arvalid;
  wire [24:0]S_AXI_CTRL_awaddr;
  wire [2:0]S_AXI_CTRL_awprot;
  wire S_AXI_CTRL_awready;
  wire S_AXI_CTRL_awvalid;
  wire S_AXI_CTRL_bready;
  wire [1:0]S_AXI_CTRL_bresp;
  wire S_AXI_CTRL_bvalid;
  wire [31:0]S_AXI_CTRL_rdata;
  wire S_AXI_CTRL_rready;
  wire [1:0]S_AXI_CTRL_rresp;
  wire S_AXI_CTRL_rvalid;
  wire [31:0]S_AXI_CTRL_wdata;
  wire S_AXI_CTRL_wready;
  wire [3:0]S_AXI_CTRL_wstrb;
  wire S_AXI_CTRL_wvalid;
  wire aclk;
  wire aclk1;
  wire aclk2;
  wire aresetn;
  wire ddr4_mem00_sys_rst;
  wire ddr4_mem00_ui_clk;
  wire ddr4_mem01_sys_rst;
  wire ddr4_mem01_ui_clk;
  wire ddr4_mem02_sys_rst;
  wire ddr4_mem02_ui_clk;
  wire ddr4_mem_calib_complete;
  wire [2:0]ddr4_mem_calib_vec;

  (* HW_HANDOFF = "pfm_dynamic_memory_subsystem_0.hwdef" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216 inst
       (.DDR4_MEM00_DIFF_CLK_clk_n(DDR4_MEM00_DIFF_CLK_clk_n),
        .DDR4_MEM00_DIFF_CLK_clk_p(DDR4_MEM00_DIFF_CLK_clk_p),
        .DDR4_MEM00_act_n(DDR4_MEM00_act_n),
        .DDR4_MEM00_adr(DDR4_MEM00_adr),
        .DDR4_MEM00_ba(DDR4_MEM00_ba),
        .DDR4_MEM00_bg(DDR4_MEM00_bg),
        .DDR4_MEM00_ck_c(DDR4_MEM00_ck_c),
        .DDR4_MEM00_ck_t(DDR4_MEM00_ck_t),
        .DDR4_MEM00_cke(DDR4_MEM00_cke),
        .DDR4_MEM00_cs_n(DDR4_MEM00_cs_n),
        .DDR4_MEM00_dq(DDR4_MEM00_dq),
        .DDR4_MEM00_dqs_c(DDR4_MEM00_dqs_c),
        .DDR4_MEM00_dqs_t(DDR4_MEM00_dqs_t),
        .DDR4_MEM00_odt(DDR4_MEM00_odt),
        .DDR4_MEM00_par(DDR4_MEM00_par),
        .DDR4_MEM00_reset_n(DDR4_MEM00_reset_n),
        .DDR4_MEM01_DIFF_CLK_clk_n(DDR4_MEM01_DIFF_CLK_clk_n),
        .DDR4_MEM01_DIFF_CLK_clk_p(DDR4_MEM01_DIFF_CLK_clk_p),
        .DDR4_MEM01_act_n(DDR4_MEM01_act_n),
        .DDR4_MEM01_adr(DDR4_MEM01_adr),
        .DDR4_MEM01_ba(DDR4_MEM01_ba),
        .DDR4_MEM01_bg(DDR4_MEM01_bg),
        .DDR4_MEM01_ck_c(DDR4_MEM01_ck_c),
        .DDR4_MEM01_ck_t(DDR4_MEM01_ck_t),
        .DDR4_MEM01_cke(DDR4_MEM01_cke),
        .DDR4_MEM01_cs_n(DDR4_MEM01_cs_n),
        .DDR4_MEM01_dq(DDR4_MEM01_dq),
        .DDR4_MEM01_dqs_c(DDR4_MEM01_dqs_c),
        .DDR4_MEM01_dqs_t(DDR4_MEM01_dqs_t),
        .DDR4_MEM01_odt(DDR4_MEM01_odt),
        .DDR4_MEM01_par(DDR4_MEM01_par),
        .DDR4_MEM01_reset_n(DDR4_MEM01_reset_n),
        .DDR4_MEM02_DIFF_CLK_clk_n(DDR4_MEM02_DIFF_CLK_clk_n),
        .DDR4_MEM02_DIFF_CLK_clk_p(DDR4_MEM02_DIFF_CLK_clk_p),
        .DDR4_MEM02_act_n(DDR4_MEM02_act_n),
        .DDR4_MEM02_adr(DDR4_MEM02_adr),
        .DDR4_MEM02_ba(DDR4_MEM02_ba),
        .DDR4_MEM02_bg(DDR4_MEM02_bg),
        .DDR4_MEM02_ck_c(DDR4_MEM02_ck_c),
        .DDR4_MEM02_ck_t(DDR4_MEM02_ck_t),
        .DDR4_MEM02_cke(DDR4_MEM02_cke),
        .DDR4_MEM02_cs_n(DDR4_MEM02_cs_n),
        .DDR4_MEM02_dq(DDR4_MEM02_dq),
        .DDR4_MEM02_dqs_c(DDR4_MEM02_dqs_c),
        .DDR4_MEM02_dqs_t(DDR4_MEM02_dqs_t),
        .DDR4_MEM02_odt(DDR4_MEM02_odt),
        .DDR4_MEM02_par(DDR4_MEM02_par),
        .DDR4_MEM02_reset_n(DDR4_MEM02_reset_n),
        .M00_AXI_araddr(M00_AXI_araddr),
        .M00_AXI_arburst(M00_AXI_arburst),
        .M00_AXI_arcache(M00_AXI_arcache),
        .M00_AXI_arlen(M00_AXI_arlen),
        .M00_AXI_arlock(M00_AXI_arlock),
        .M00_AXI_arprot(M00_AXI_arprot),
        .M00_AXI_arqos(M00_AXI_arqos),
        .M00_AXI_arready(M00_AXI_arready),
        .M00_AXI_arregion(M00_AXI_arregion),
        .M00_AXI_arsize(M00_AXI_arsize),
        .M00_AXI_arvalid(M00_AXI_arvalid),
        .M00_AXI_awaddr(M00_AXI_awaddr),
        .M00_AXI_awburst(M00_AXI_awburst),
        .M00_AXI_awcache(M00_AXI_awcache),
        .M00_AXI_awlen(M00_AXI_awlen),
        .M00_AXI_awlock(M00_AXI_awlock),
        .M00_AXI_awprot(M00_AXI_awprot),
        .M00_AXI_awqos(M00_AXI_awqos),
        .M00_AXI_awready(M00_AXI_awready),
        .M00_AXI_awregion(M00_AXI_awregion),
        .M00_AXI_awsize(M00_AXI_awsize),
        .M00_AXI_awvalid(M00_AXI_awvalid),
        .M00_AXI_bready(M00_AXI_bready),
        .M00_AXI_bresp(M00_AXI_bresp),
        .M00_AXI_bvalid(M00_AXI_bvalid),
        .M00_AXI_rdata(M00_AXI_rdata),
        .M00_AXI_rlast(M00_AXI_rlast),
        .M00_AXI_rready(M00_AXI_rready),
        .M00_AXI_rresp(M00_AXI_rresp),
        .M00_AXI_rvalid(M00_AXI_rvalid),
        .M00_AXI_wdata(M00_AXI_wdata),
        .M00_AXI_wlast(M00_AXI_wlast),
        .M00_AXI_wready(M00_AXI_wready),
        .M00_AXI_wstrb(M00_AXI_wstrb),
        .M00_AXI_wvalid(M00_AXI_wvalid),
        .S00_AXI_araddr(S00_AXI_araddr),
        .S00_AXI_arburst(S00_AXI_arburst),
        .S00_AXI_arcache(S00_AXI_arcache),
        .S00_AXI_arid(S00_AXI_arid),
        .S00_AXI_arlen(S00_AXI_arlen),
        .S00_AXI_arlock(S00_AXI_arlock),
        .S00_AXI_arprot(S00_AXI_arprot),
        .S00_AXI_arqos(S00_AXI_arqos),
        .S00_AXI_arready(S00_AXI_arready),
        .S00_AXI_arregion(S00_AXI_arregion),
        .S00_AXI_arsize(S00_AXI_arsize),
        .S00_AXI_arvalid(S00_AXI_arvalid),
        .S00_AXI_awaddr(S00_AXI_awaddr),
        .S00_AXI_awburst(S00_AXI_awburst),
        .S00_AXI_awcache(S00_AXI_awcache),
        .S00_AXI_awid(S00_AXI_awid),
        .S00_AXI_awlen(S00_AXI_awlen),
        .S00_AXI_awlock(S00_AXI_awlock),
        .S00_AXI_awprot(S00_AXI_awprot),
        .S00_AXI_awqos(S00_AXI_awqos),
        .S00_AXI_awready(S00_AXI_awready),
        .S00_AXI_awregion(S00_AXI_awregion),
        .S00_AXI_awsize(S00_AXI_awsize),
        .S00_AXI_awvalid(S00_AXI_awvalid),
        .S00_AXI_bid(S00_AXI_bid),
        .S00_AXI_bready(S00_AXI_bready),
        .S00_AXI_bresp(S00_AXI_bresp),
        .S00_AXI_bvalid(S00_AXI_bvalid),
        .S00_AXI_rdata(S00_AXI_rdata),
        .S00_AXI_rid(S00_AXI_rid),
        .S00_AXI_rlast(S00_AXI_rlast),
        .S00_AXI_rready(S00_AXI_rready),
        .S00_AXI_rresp(S00_AXI_rresp),
        .S00_AXI_rvalid(S00_AXI_rvalid),
        .S00_AXI_wdata(S00_AXI_wdata),
        .S00_AXI_wlast(S00_AXI_wlast),
        .S00_AXI_wready(S00_AXI_wready),
        .S00_AXI_wstrb(S00_AXI_wstrb),
        .S00_AXI_wvalid(S00_AXI_wvalid),
        .S01_AXI_araddr(S01_AXI_araddr),
        .S01_AXI_arburst(S01_AXI_arburst),
        .S01_AXI_arcache(S01_AXI_arcache),
        .S01_AXI_arid(S01_AXI_arid),
        .S01_AXI_arlen(S01_AXI_arlen),
        .S01_AXI_arlock(S01_AXI_arlock),
        .S01_AXI_arprot(S01_AXI_arprot),
        .S01_AXI_arqos(S01_AXI_arqos),
        .S01_AXI_arready(S01_AXI_arready),
        .S01_AXI_arregion(S01_AXI_arregion),
        .S01_AXI_arvalid(S01_AXI_arvalid),
        .S01_AXI_awaddr(S01_AXI_awaddr),
        .S01_AXI_awburst(S01_AXI_awburst),
        .S01_AXI_awcache(S01_AXI_awcache),
        .S01_AXI_awid(S01_AXI_awid),
        .S01_AXI_awlen(S01_AXI_awlen),
        .S01_AXI_awlock(S01_AXI_awlock),
        .S01_AXI_awprot(S01_AXI_awprot),
        .S01_AXI_awqos(S01_AXI_awqos),
        .S01_AXI_awready(S01_AXI_awready),
        .S01_AXI_awregion(S01_AXI_awregion),
        .S01_AXI_awvalid(S01_AXI_awvalid),
        .S01_AXI_bid(S01_AXI_bid),
        .S01_AXI_bready(S01_AXI_bready),
        .S01_AXI_bresp(S01_AXI_bresp),
        .S01_AXI_bvalid(S01_AXI_bvalid),
        .S01_AXI_rdata(S01_AXI_rdata),
        .S01_AXI_rid(S01_AXI_rid),
        .S01_AXI_rlast(S01_AXI_rlast),
        .S01_AXI_rready(S01_AXI_rready),
        .S01_AXI_rresp(S01_AXI_rresp),
        .S01_AXI_rvalid(S01_AXI_rvalid),
        .S01_AXI_wdata(S01_AXI_wdata),
        .S01_AXI_wlast(S01_AXI_wlast),
        .S01_AXI_wready(S01_AXI_wready),
        .S01_AXI_wstrb(S01_AXI_wstrb),
        .S01_AXI_wvalid(S01_AXI_wvalid),
        .S02_AXI_araddr(S02_AXI_araddr),
        .S02_AXI_arburst(S02_AXI_arburst),
        .S02_AXI_arcache(S02_AXI_arcache),
        .S02_AXI_arid(S02_AXI_arid),
        .S02_AXI_arlen(S02_AXI_arlen),
        .S02_AXI_arlock(S02_AXI_arlock),
        .S02_AXI_arprot(S02_AXI_arprot),
        .S02_AXI_arqos(S02_AXI_arqos),
        .S02_AXI_arready(S02_AXI_arready),
        .S02_AXI_arregion(S02_AXI_arregion),
        .S02_AXI_arvalid(S02_AXI_arvalid),
        .S02_AXI_awaddr(S02_AXI_awaddr),
        .S02_AXI_awburst(S02_AXI_awburst),
        .S02_AXI_awcache(S02_AXI_awcache),
        .S02_AXI_awid(S02_AXI_awid),
        .S02_AXI_awlen(S02_AXI_awlen),
        .S02_AXI_awlock(S02_AXI_awlock),
        .S02_AXI_awprot(S02_AXI_awprot),
        .S02_AXI_awqos(S02_AXI_awqos),
        .S02_AXI_awready(S02_AXI_awready),
        .S02_AXI_awregion(S02_AXI_awregion),
        .S02_AXI_awvalid(S02_AXI_awvalid),
        .S02_AXI_bid(S02_AXI_bid),
        .S02_AXI_bready(S02_AXI_bready),
        .S02_AXI_bresp(S02_AXI_bresp),
        .S02_AXI_bvalid(S02_AXI_bvalid),
        .S02_AXI_rdata(S02_AXI_rdata),
        .S02_AXI_rid(S02_AXI_rid),
        .S02_AXI_rlast(S02_AXI_rlast),
        .S02_AXI_rready(S02_AXI_rready),
        .S02_AXI_rresp(S02_AXI_rresp),
        .S02_AXI_rvalid(S02_AXI_rvalid),
        .S02_AXI_wdata(S02_AXI_wdata),
        .S02_AXI_wlast(S02_AXI_wlast),
        .S02_AXI_wready(S02_AXI_wready),
        .S02_AXI_wstrb(S02_AXI_wstrb),
        .S02_AXI_wvalid(S02_AXI_wvalid),
        .S03_AXI_araddr(S03_AXI_araddr),
        .S03_AXI_arburst(S03_AXI_arburst),
        .S03_AXI_arcache(S03_AXI_arcache),
        .S03_AXI_arid(S03_AXI_arid),
        .S03_AXI_arlen(S03_AXI_arlen),
        .S03_AXI_arlock(S03_AXI_arlock),
        .S03_AXI_arprot(S03_AXI_arprot),
        .S03_AXI_arqos(S03_AXI_arqos),
        .S03_AXI_arready(S03_AXI_arready),
        .S03_AXI_arregion(S03_AXI_arregion),
        .S03_AXI_arsize(S03_AXI_arsize),
        .S03_AXI_arvalid(S03_AXI_arvalid),
        .S03_AXI_awaddr(S03_AXI_awaddr),
        .S03_AXI_awburst(S03_AXI_awburst),
        .S03_AXI_awcache(S03_AXI_awcache),
        .S03_AXI_awid(S03_AXI_awid),
        .S03_AXI_awlen(S03_AXI_awlen),
        .S03_AXI_awlock(S03_AXI_awlock),
        .S03_AXI_awprot(S03_AXI_awprot),
        .S03_AXI_awqos(S03_AXI_awqos),
        .S03_AXI_awready(S03_AXI_awready),
        .S03_AXI_awregion(S03_AXI_awregion),
        .S03_AXI_awsize(S03_AXI_awsize),
        .S03_AXI_awvalid(S03_AXI_awvalid),
        .S03_AXI_bid(S03_AXI_bid),
        .S03_AXI_bready(S03_AXI_bready),
        .S03_AXI_bresp(S03_AXI_bresp),
        .S03_AXI_bvalid(S03_AXI_bvalid),
        .S03_AXI_rdata(S03_AXI_rdata),
        .S03_AXI_rid(S03_AXI_rid),
        .S03_AXI_rlast(S03_AXI_rlast),
        .S03_AXI_rready(S03_AXI_rready),
        .S03_AXI_rresp(S03_AXI_rresp),
        .S03_AXI_rvalid(S03_AXI_rvalid),
        .S03_AXI_wdata(S03_AXI_wdata),
        .S03_AXI_wlast(S03_AXI_wlast),
        .S03_AXI_wready(S03_AXI_wready),
        .S03_AXI_wstrb(S03_AXI_wstrb),
        .S03_AXI_wvalid(S03_AXI_wvalid),
        .S04_AXI_araddr(S04_AXI_araddr),
        .S04_AXI_arcache(S04_AXI_arcache),
        .S04_AXI_arlen(S04_AXI_arlen),
        .S04_AXI_arlock(S04_AXI_arlock),
        .S04_AXI_arprot(S04_AXI_arprot),
        .S04_AXI_arqos(S04_AXI_arqos),
        .S04_AXI_arready(S04_AXI_arready),
        .S04_AXI_arregion(S04_AXI_arregion),
        .S04_AXI_arvalid(S04_AXI_arvalid),
        .S04_AXI_awaddr(S04_AXI_awaddr),
        .S04_AXI_awcache(S04_AXI_awcache),
        .S04_AXI_awlen(S04_AXI_awlen),
        .S04_AXI_awlock(S04_AXI_awlock),
        .S04_AXI_awprot(S04_AXI_awprot),
        .S04_AXI_awqos(S04_AXI_awqos),
        .S04_AXI_awready(S04_AXI_awready),
        .S04_AXI_awregion(S04_AXI_awregion),
        .S04_AXI_awvalid(S04_AXI_awvalid),
        .S04_AXI_bready(S04_AXI_bready),
        .S04_AXI_bresp(S04_AXI_bresp),
        .S04_AXI_bvalid(S04_AXI_bvalid),
        .S04_AXI_rdata(S04_AXI_rdata),
        .S04_AXI_rlast(S04_AXI_rlast),
        .S04_AXI_rready(S04_AXI_rready),
        .S04_AXI_rresp(S04_AXI_rresp),
        .S04_AXI_rvalid(S04_AXI_rvalid),
        .S04_AXI_wdata(S04_AXI_wdata),
        .S04_AXI_wlast(S04_AXI_wlast),
        .S04_AXI_wready(S04_AXI_wready),
        .S04_AXI_wstrb(S04_AXI_wstrb),
        .S04_AXI_wvalid(S04_AXI_wvalid),
        .S05_AXI_araddr(S05_AXI_araddr),
        .S05_AXI_arcache(S05_AXI_arcache),
        .S05_AXI_arlen(S05_AXI_arlen),
        .S05_AXI_arlock(S05_AXI_arlock),
        .S05_AXI_arprot(S05_AXI_arprot),
        .S05_AXI_arqos(S05_AXI_arqos),
        .S05_AXI_arready(S05_AXI_arready),
        .S05_AXI_arregion(S05_AXI_arregion),
        .S05_AXI_arvalid(S05_AXI_arvalid),
        .S05_AXI_awaddr(S05_AXI_awaddr),
        .S05_AXI_awcache(S05_AXI_awcache),
        .S05_AXI_awlen(S05_AXI_awlen),
        .S05_AXI_awlock(S05_AXI_awlock),
        .S05_AXI_awprot(S05_AXI_awprot),
        .S05_AXI_awqos(S05_AXI_awqos),
        .S05_AXI_awready(S05_AXI_awready),
        .S05_AXI_awregion(S05_AXI_awregion),
        .S05_AXI_awvalid(S05_AXI_awvalid),
        .S05_AXI_bready(S05_AXI_bready),
        .S05_AXI_bresp(S05_AXI_bresp),
        .S05_AXI_bvalid(S05_AXI_bvalid),
        .S05_AXI_rdata(S05_AXI_rdata),
        .S05_AXI_rlast(S05_AXI_rlast),
        .S05_AXI_rready(S05_AXI_rready),
        .S05_AXI_rresp(S05_AXI_rresp),
        .S05_AXI_rvalid(S05_AXI_rvalid),
        .S05_AXI_wdata(S05_AXI_wdata),
        .S05_AXI_wlast(S05_AXI_wlast),
        .S05_AXI_wready(S05_AXI_wready),
        .S05_AXI_wstrb(S05_AXI_wstrb),
        .S05_AXI_wvalid(S05_AXI_wvalid),
        .S_AXI_CTRL_araddr(S_AXI_CTRL_araddr),
        .S_AXI_CTRL_arprot(S_AXI_CTRL_arprot),
        .S_AXI_CTRL_arready(S_AXI_CTRL_arready),
        .S_AXI_CTRL_arvalid(S_AXI_CTRL_arvalid),
        .S_AXI_CTRL_awaddr(S_AXI_CTRL_awaddr),
        .S_AXI_CTRL_awprot(S_AXI_CTRL_awprot),
        .S_AXI_CTRL_awready(S_AXI_CTRL_awready),
        .S_AXI_CTRL_awvalid(S_AXI_CTRL_awvalid),
        .S_AXI_CTRL_bready(S_AXI_CTRL_bready),
        .S_AXI_CTRL_bresp(S_AXI_CTRL_bresp),
        .S_AXI_CTRL_bvalid(S_AXI_CTRL_bvalid),
        .S_AXI_CTRL_rdata(S_AXI_CTRL_rdata),
        .S_AXI_CTRL_rready(S_AXI_CTRL_rready),
        .S_AXI_CTRL_rresp(S_AXI_CTRL_rresp),
        .S_AXI_CTRL_rvalid(S_AXI_CTRL_rvalid),
        .S_AXI_CTRL_wdata(S_AXI_CTRL_wdata),
        .S_AXI_CTRL_wready(S_AXI_CTRL_wready),
        .S_AXI_CTRL_wstrb(S_AXI_CTRL_wstrb),
        .S_AXI_CTRL_wvalid(S_AXI_CTRL_wvalid),
        .aclk(aclk),
        .aclk1(aclk1),
        .aclk2(aclk2),
        .aresetn(aresetn),
        .ddr4_mem00_sys_rst(ddr4_mem00_sys_rst),
        .ddr4_mem00_ui_clk(ddr4_mem00_ui_clk),
        .ddr4_mem01_sys_rst(ddr4_mem01_sys_rst),
        .ddr4_mem01_ui_clk(ddr4_mem01_ui_clk),
        .ddr4_mem02_sys_rst(ddr4_mem02_sys_rst),
        .ddr4_mem02_ui_clk(ddr4_mem02_ui_clk),
        .ddr4_mem_calib_complete(ddr4_mem_calib_complete),
        .ddr4_mem_calib_vec(ddr4_mem_calib_vec));
endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_reset_imp_1YKOSPE
   (interconnect_aresetn1,
    interconnect_aresetn,
    aclk2,
    aresetn,
    aclk);
  output [0:0]interconnect_aresetn1;
  output [0:0]interconnect_aresetn;
  input aclk2;
  input aresetn;
  input aclk;

  wire aclk;
  wire aclk2;
  wire aresetn;
  wire [0:0]interconnect_aresetn;
  wire [0:0]interconnect_aresetn1;
  wire NLW_psr_aclk2_SLR1_mb_reset_UNCONNECTED;
  wire [0:0]NLW_psr_aclk2_SLR1_bus_struct_reset_UNCONNECTED;
  wire [0:0]NLW_psr_aclk2_SLR1_peripheral_aresetn_UNCONNECTED;
  wire [0:0]NLW_psr_aclk2_SLR1_peripheral_reset_UNCONNECTED;
  wire NLW_psr_aclk_SLR1_mb_reset_UNCONNECTED;
  wire [0:0]NLW_psr_aclk_SLR1_bus_struct_reset_UNCONNECTED;
  wire [0:0]NLW_psr_aclk_SLR1_peripheral_aresetn_UNCONNECTED;
  wire [0:0]NLW_psr_aclk_SLR1_peripheral_reset_UNCONNECTED;

  (* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk2_SLR1_0 psr_aclk2_SLR1
       (.aux_reset_in(1'b1),
        .bus_struct_reset(NLW_psr_aclk2_SLR1_bus_struct_reset_UNCONNECTED[0]),
        .dcm_locked(1'b1),
        .ext_reset_in(aresetn),
        .interconnect_aresetn(interconnect_aresetn1),
        .mb_debug_sys_rst(1'b0),
        .mb_reset(NLW_psr_aclk2_SLR1_mb_reset_UNCONNECTED),
        .peripheral_aresetn(NLW_psr_aclk2_SLR1_peripheral_aresetn_UNCONNECTED[0]),
        .peripheral_reset(NLW_psr_aclk2_SLR1_peripheral_reset_UNCONNECTED[0]),
        .slowest_sync_clk(aclk2));
  (* X_CORE_INFO = "proc_sys_reset,Vivado 2020.1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk_SLR1_0 psr_aclk_SLR1
       (.aux_reset_in(1'b1),
        .bus_struct_reset(NLW_psr_aclk_SLR1_bus_struct_reset_UNCONNECTED[0]),
        .dcm_locked(1'b1),
        .ext_reset_in(aresetn),
        .interconnect_aresetn(interconnect_aresetn),
        .mb_debug_sys_rst(1'b0),
        .mb_reset(NLW_psr_aclk_SLR1_mb_reset_UNCONNECTED),
        .peripheral_aresetn(NLW_psr_aclk_SLR1_peripheral_aresetn_UNCONNECTED[0]),
        .peripheral_reset(NLW_psr_aclk_SLR1_peripheral_reset_UNCONNECTED[0]),
        .slowest_sync_clk(aclk));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;
    parameter GRES_WIDTH = 10000;
    parameter GRES_START = 10000;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    wire GRESTORE;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;
    reg GRESTORE_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;
    assign (strong1, weak0) GRESTORE = GRESTORE_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

    initial begin 
	GRESTORE_int = 1'b0;
	#(GRES_START);
	GRESTORE_int = 1'b1;
	#(GRES_WIDTH);
	GRESTORE_int = 1'b0;
    end

endmodule
`endif
