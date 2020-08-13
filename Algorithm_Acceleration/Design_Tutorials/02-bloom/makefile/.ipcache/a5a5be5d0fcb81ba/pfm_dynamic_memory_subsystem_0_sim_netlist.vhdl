-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
-- Date        : Thu May  7 16:56:58 2020
-- Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
-- Command     : write_vhdl -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_memory_subsystem_0_sim_netlist.vhdl
-- Design      : pfm_dynamic_memory_subsystem_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xcu200-fsgd2104-2-e
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0 is
  port (
    In0 : in STD_LOGIC_VECTOR ( 0 to 0 );
    dout : out STD_LOGIC_VECTOR ( 0 to 0 )
  );
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0 : entity is "bd_d216_calib_concat_0,xlconcat_v2_1_3_xlconcat,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0 : entity is "yes";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0 : entity is "xlconcat_v2_1_3_xlconcat,Vivado 2020.1";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0 is
  signal \^in0\ : STD_LOGIC_VECTOR ( 0 to 0 );
begin
  \^in0\(0) <= In0(0);
  dout(0) <= \^in0\(0);
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0 is
  port (
    In0 : in STD_LOGIC_VECTOR ( 0 to 0 );
    In1 : in STD_LOGIC_VECTOR ( 0 to 0 );
    In2 : in STD_LOGIC_VECTOR ( 0 to 0 );
    dout : out STD_LOGIC_VECTOR ( 2 downto 0 )
  );
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0 : entity is "bd_d216_calib_vector_concat_0,xlconcat_v2_1_3_xlconcat,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0 : entity is "yes";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0 : entity is "xlconcat_v2_1_3_xlconcat,Vivado 2020.1";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0 is
  signal \^in0\ : STD_LOGIC_VECTOR ( 0 to 0 );
  signal \^in1\ : STD_LOGIC_VECTOR ( 0 to 0 );
  signal \^in2\ : STD_LOGIC_VECTOR ( 0 to 0 );
begin
  \^in0\(0) <= In0(0);
  \^in1\(0) <= In1(0);
  \^in2\(0) <= In2(0);
  dout(2) <= \^in2\(0);
  dout(1) <= \^in1\(0);
  dout(0) <= \^in0\(0);
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_interconnect_imp_6HQKUY is
  port (
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 33 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wvalid : out STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC;
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 33 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC;
    \bbstub_m_axi_awaddr[38]\ : out STD_LOGIC_VECTOR ( 38 downto 0 );
    \bbstub_m_axi_awlen[7]\ : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    \bbstub_m_axi_awburst[1]\ : out STD_LOGIC_VECTOR ( 1 downto 0 );
    \bbstub_m_axi_awlock[0]\ : out STD_LOGIC_VECTOR ( 0 to 0 );
    \bbstub_m_axi_awcache[3]\ : out STD_LOGIC_VECTOR ( 3 downto 0 );
    \bbstub_m_axi_awprot[2]\ : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    \bbstub_m_axi_awqos[3]\ : out STD_LOGIC_VECTOR ( 3 downto 0 );
    bbstub_m_axi_awvalid : out STD_LOGIC;
    \bbstub_m_axi_wdata[511]\ : out STD_LOGIC_VECTOR ( 511 downto 0 );
    \bbstub_m_axi_wstrb[63]\ : out STD_LOGIC_VECTOR ( 63 downto 0 );
    bbstub_m_axi_wlast : out STD_LOGIC;
    bbstub_m_axi_wvalid : out STD_LOGIC;
    bbstub_m_axi_bready : out STD_LOGIC;
    \bbstub_m_axi_araddr[38]\ : out STD_LOGIC_VECTOR ( 38 downto 0 );
    \bbstub_m_axi_arlen[7]\ : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    \bbstub_m_axi_arburst[1]\ : out STD_LOGIC_VECTOR ( 1 downto 0 );
    \bbstub_m_axi_arlock[0]\ : out STD_LOGIC_VECTOR ( 0 to 0 );
    \bbstub_m_axi_arcache[3]\ : out STD_LOGIC_VECTOR ( 3 downto 0 );
    \bbstub_m_axi_arprot[2]\ : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    \bbstub_m_axi_arqos[3]\ : out STD_LOGIC_VECTOR ( 3 downto 0 );
    bbstub_m_axi_arvalid : out STD_LOGIC;
    bbstub_m_axi_rready : out STD_LOGIC;
    S01_AXI_awready : out STD_LOGIC;
    S01_AXI_wready : out STD_LOGIC;
    S01_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_bvalid : out STD_LOGIC;
    S01_AXI_arready : out STD_LOGIC;
    S01_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_rlast : out STD_LOGIC;
    S01_AXI_rvalid : out STD_LOGIC;
    S02_AXI_awready : out STD_LOGIC;
    S02_AXI_wready : out STD_LOGIC;
    S02_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_bvalid : out STD_LOGIC;
    S02_AXI_arready : out STD_LOGIC;
    S02_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_rlast : out STD_LOGIC;
    S02_AXI_rvalid : out STD_LOGIC;
    S04_AXI_awready : out STD_LOGIC;
    S04_AXI_wready : out STD_LOGIC;
    S04_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S04_AXI_bvalid : out STD_LOGIC;
    S04_AXI_arready : out STD_LOGIC;
    S04_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S04_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S04_AXI_rlast : out STD_LOGIC;
    S04_AXI_rvalid : out STD_LOGIC;
    S05_AXI_awready : out STD_LOGIC;
    S05_AXI_wready : out STD_LOGIC;
    S05_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S05_AXI_bvalid : out STD_LOGIC;
    S05_AXI_arready : out STD_LOGIC;
    S05_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S05_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S05_AXI_rlast : out STD_LOGIC;
    S05_AXI_rvalid : out STD_LOGIC;
    ddr4_mem01_ui_clk : in STD_LOGIC;
    aclk : in STD_LOGIC;
    interconnect_aresetn : in STD_LOGIC_VECTOR ( 0 to 0 );
    S_AXI_awready : in STD_LOGIC;
    S_AXI_wready : in STD_LOGIC;
    S_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_bvalid : in STD_LOGIC;
    S_AXI_arready : in STD_LOGIC;
    S_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_rlast : in STD_LOGIC;
    S_AXI_rvalid : in STD_LOGIC;
    aclk2 : in STD_LOGIC;
    interconnect_aresetn1 : in STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_rvalid : in STD_LOGIC;
    S01_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awvalid : in STD_LOGIC;
    S01_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S01_AXI_wlast : in STD_LOGIC;
    S01_AXI_wvalid : in STD_LOGIC;
    S01_AXI_bready : in STD_LOGIC;
    S01_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arvalid : in STD_LOGIC;
    S01_AXI_rready : in STD_LOGIC;
    S02_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awvalid : in STD_LOGIC;
    S02_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S02_AXI_wlast : in STD_LOGIC;
    S02_AXI_wvalid : in STD_LOGIC;
    S02_AXI_bready : in STD_LOGIC;
    S02_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arvalid : in STD_LOGIC;
    S02_AXI_rready : in STD_LOGIC;
    S04_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S04_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S04_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S04_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S04_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awvalid : in STD_LOGIC;
    S04_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S04_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S04_AXI_wlast : in STD_LOGIC;
    S04_AXI_wvalid : in STD_LOGIC;
    S04_AXI_bready : in STD_LOGIC;
    S04_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S04_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S04_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S04_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S04_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arvalid : in STD_LOGIC;
    S04_AXI_rready : in STD_LOGIC;
    S05_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S05_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S05_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S05_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S05_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awvalid : in STD_LOGIC;
    S05_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S05_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S05_AXI_wlast : in STD_LOGIC;
    S05_AXI_wvalid : in STD_LOGIC;
    S05_AXI_bready : in STD_LOGIC;
    S05_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S05_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S05_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S05_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S05_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arvalid : in STD_LOGIC;
    S05_AXI_rready : in STD_LOGIC
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_interconnect_imp_6HQKUY;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_interconnect_imp_6HQKUY is
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_DDR4_MEM01_0 is
  port (
    aclk : in STD_LOGIC;
    aclk1 : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    S00_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awvalid : in STD_LOGIC;
    S00_AXI_awready : out STD_LOGIC;
    S00_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S00_AXI_wlast : in STD_LOGIC;
    S00_AXI_wvalid : in STD_LOGIC;
    S00_AXI_wready : out STD_LOGIC;
    S00_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_bvalid : out STD_LOGIC;
    S00_AXI_bready : in STD_LOGIC;
    S00_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arvalid : in STD_LOGIC;
    S00_AXI_arready : out STD_LOGIC;
    S00_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_rlast : out STD_LOGIC;
    S00_AXI_rvalid : out STD_LOGIC;
    S00_AXI_rready : in STD_LOGIC;
    S01_AXI_awid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awuser : in STD_LOGIC_VECTOR ( 113 downto 0 );
    S01_AXI_awvalid : in STD_LOGIC;
    S01_AXI_awready : out STD_LOGIC;
    S01_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S01_AXI_wlast : in STD_LOGIC;
    S01_AXI_wuser : in STD_LOGIC_VECTOR ( 13 downto 0 );
    S01_AXI_wvalid : in STD_LOGIC;
    S01_AXI_wready : out STD_LOGIC;
    S01_AXI_bid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_buser : out STD_LOGIC_VECTOR ( 113 downto 0 );
    S01_AXI_bvalid : out STD_LOGIC;
    S01_AXI_bready : in STD_LOGIC;
    S01_AXI_arid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_aruser : in STD_LOGIC_VECTOR ( 113 downto 0 );
    S01_AXI_arvalid : in STD_LOGIC;
    S01_AXI_arready : out STD_LOGIC;
    S01_AXI_rid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_rlast : out STD_LOGIC;
    S01_AXI_ruser : out STD_LOGIC_VECTOR ( 13 downto 0 );
    S01_AXI_rvalid : out STD_LOGIC;
    S01_AXI_rready : in STD_LOGIC;
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 33 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wvalid : out STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC;
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 33 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_rvalid : in STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_DDR4_MEM01_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_M00_AXI_MEM00_0 is
  port (
    aclk : in STD_LOGIC;
    aclk1 : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    S00_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awvalid : in STD_LOGIC;
    S00_AXI_awready : out STD_LOGIC;
    S00_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S00_AXI_wlast : in STD_LOGIC;
    S00_AXI_wvalid : in STD_LOGIC;
    S00_AXI_wready : out STD_LOGIC;
    S00_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_bvalid : out STD_LOGIC;
    S00_AXI_bready : in STD_LOGIC;
    S00_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arvalid : in STD_LOGIC;
    S00_AXI_arready : out STD_LOGIC;
    S00_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_rlast : out STD_LOGIC;
    S00_AXI_rvalid : out STD_LOGIC;
    S00_AXI_rready : in STD_LOGIC;
    S01_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awvalid : in STD_LOGIC;
    S01_AXI_awready : out STD_LOGIC;
    S01_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S01_AXI_wlast : in STD_LOGIC;
    S01_AXI_wvalid : in STD_LOGIC;
    S01_AXI_wready : out STD_LOGIC;
    S01_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_bvalid : out STD_LOGIC;
    S01_AXI_bready : in STD_LOGIC;
    S01_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arvalid : in STD_LOGIC;
    S01_AXI_arready : out STD_LOGIC;
    S01_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_rlast : out STD_LOGIC;
    S01_AXI_rvalid : out STD_LOGIC;
    S01_AXI_rready : in STD_LOGIC;
    S02_AXI_awid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awuser : in STD_LOGIC_VECTOR ( 113 downto 0 );
    S02_AXI_awvalid : in STD_LOGIC;
    S02_AXI_awready : out STD_LOGIC;
    S02_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S02_AXI_wlast : in STD_LOGIC;
    S02_AXI_wuser : in STD_LOGIC_VECTOR ( 13 downto 0 );
    S02_AXI_wvalid : in STD_LOGIC;
    S02_AXI_wready : out STD_LOGIC;
    S02_AXI_bid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_buser : out STD_LOGIC_VECTOR ( 113 downto 0 );
    S02_AXI_bvalid : out STD_LOGIC;
    S02_AXI_bready : in STD_LOGIC;
    S02_AXI_arid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_aruser : in STD_LOGIC_VECTOR ( 113 downto 0 );
    S02_AXI_arvalid : in STD_LOGIC;
    S02_AXI_arready : out STD_LOGIC;
    S02_AXI_rid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_rlast : out STD_LOGIC;
    S02_AXI_ruser : out STD_LOGIC_VECTOR ( 13 downto 0 );
    S02_AXI_rvalid : out STD_LOGIC;
    S02_AXI_rready : in STD_LOGIC;
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wvalid : out STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC;
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_rvalid : in STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_M00_AXI_MEM00_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_S04_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    S00_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awvalid : in STD_LOGIC;
    S00_AXI_awready : out STD_LOGIC;
    S00_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S00_AXI_wlast : in STD_LOGIC;
    S00_AXI_wvalid : in STD_LOGIC;
    S00_AXI_wready : out STD_LOGIC;
    S00_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_bvalid : out STD_LOGIC;
    S00_AXI_bready : in STD_LOGIC;
    S00_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arvalid : in STD_LOGIC;
    S00_AXI_arready : out STD_LOGIC;
    S00_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_rlast : out STD_LOGIC;
    S00_AXI_rvalid : out STD_LOGIC;
    S00_AXI_rready : in STD_LOGIC;
    M00_AXI_awid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awuser : out STD_LOGIC_VECTOR ( 113 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wuser : out STD_LOGIC_VECTOR ( 13 downto 0 );
    M00_AXI_wvalid : out STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_buser : in STD_LOGIC_VECTOR ( 113 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC;
    M00_AXI_arid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_aruser : out STD_LOGIC_VECTOR ( 113 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_rid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_ruser : in STD_LOGIC_VECTOR ( 13 downto 0 );
    M00_AXI_rvalid : in STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC;
    M01_AXI_awid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M01_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M01_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M01_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M01_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M01_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M01_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M01_AXI_awuser : out STD_LOGIC_VECTOR ( 113 downto 0 );
    M01_AXI_awvalid : out STD_LOGIC;
    M01_AXI_awready : in STD_LOGIC;
    M01_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M01_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M01_AXI_wlast : out STD_LOGIC;
    M01_AXI_wuser : out STD_LOGIC_VECTOR ( 13 downto 0 );
    M01_AXI_wvalid : out STD_LOGIC;
    M01_AXI_wready : in STD_LOGIC;
    M01_AXI_bid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_buser : in STD_LOGIC_VECTOR ( 113 downto 0 );
    M01_AXI_bvalid : in STD_LOGIC;
    M01_AXI_bready : out STD_LOGIC;
    M01_AXI_arid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M01_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M01_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M01_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M01_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M01_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M01_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M01_AXI_aruser : out STD_LOGIC_VECTOR ( 113 downto 0 );
    M01_AXI_arvalid : out STD_LOGIC;
    M01_AXI_arready : in STD_LOGIC;
    M01_AXI_rid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M01_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M01_AXI_rlast : in STD_LOGIC;
    M01_AXI_ruser : in STD_LOGIC_VECTOR ( 13 downto 0 );
    M01_AXI_rvalid : in STD_LOGIC;
    M01_AXI_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_S04_AXI_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_rs_M00_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_rs_M00_AXI_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_M00_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_M00_AXI_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S01_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_arid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S01_AXI_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S02_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_arid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S02_AXI_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S04_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S04_AXI_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S05_AXI_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S05_AXI_0;
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARLOCK : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARREADY : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_ARVALID : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWLOCK : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWREADY : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_AWVALID : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_BREADY : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_BVALID : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_RLAST : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_RREADY : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_RVALID : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_WLAST : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_WREADY : STD_LOGIC;
  signal interconnect_M00_AXI_MEM00_M00_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal interconnect_M00_AXI_MEM00_M00_AXI_WVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARLOCK : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_ARSIZE : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARUSER : STD_LOGIC_VECTOR ( 113 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_ARVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWLOCK : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_AWSIZE : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWUSER : STD_LOGIC_VECTOR ( 113 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_AWVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_BID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_BREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_BUSER : STD_LOGIC_VECTOR ( 113 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_BVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_RID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_RLAST : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_RREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_RUSER : STD_LOGIC_VECTOR ( 13 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_RVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_WLAST : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_WREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M00_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_WUSER : STD_LOGIC_VECTOR ( 13 downto 0 );
  signal interconnect_S04_AXI_M00_AXI_WVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARLOCK : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_ARSIZE : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARUSER : STD_LOGIC_VECTOR ( 113 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_ARVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWLOCK : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_AWSIZE : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWUSER : STD_LOGIC_VECTOR ( 113 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_AWVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_BID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_BREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_BUSER : STD_LOGIC_VECTOR ( 113 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_BVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_RID : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_RLAST : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_RREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_RUSER : STD_LOGIC_VECTOR ( 13 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_RVALID : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_WLAST : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_WREADY : STD_LOGIC;
  signal interconnect_S04_AXI_M01_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_WUSER : STD_LOGIC_VECTOR ( 13 downto 0 );
  signal interconnect_S04_AXI_M01_AXI_WVALID : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_M00_AXI_M_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_M00_AXI_M_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_M00_AXI_M_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_M00_AXI_M_AXI_ARLOCK : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_M00_AXI_M_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_M00_AXI_M_AXI_ARREADY : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_ARVALID : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_M00_AXI_M_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_M00_AXI_M_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_M00_AXI_M_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_M00_AXI_M_AXI_AWLOCK : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_M00_AXI_M_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_M00_AXI_M_AXI_AWREADY : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_AWVALID : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_BREADY : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_M00_AXI_M_AXI_BVALID : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_M00_AXI_M_AXI_RLAST : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_RREADY : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_M00_AXI_M_AXI_RVALID : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_M00_AXI_M_AXI_WLAST : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_WREADY : STD_LOGIC;
  signal vip_M00_AXI_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal vip_M00_AXI_M_AXI_WVALID : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S01_AXI_M_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S01_AXI_M_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_ARID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S01_AXI_M_AXI_ARLOCK : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S01_AXI_M_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_ARREADY : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_ARVALID : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S01_AXI_M_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S01_AXI_M_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_AWID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S01_AXI_M_AXI_AWLOCK : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S01_AXI_M_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_AWREADY : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_AWVALID : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_BID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_BREADY : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S01_AXI_M_AXI_BVALID : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S01_AXI_M_AXI_RID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S01_AXI_M_AXI_RLAST : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_RREADY : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S01_AXI_M_AXI_RVALID : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S01_AXI_M_AXI_WLAST : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_WREADY : STD_LOGIC;
  signal vip_S01_AXI_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal vip_S01_AXI_M_AXI_WVALID : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S02_AXI_M_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S02_AXI_M_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_ARID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S02_AXI_M_AXI_ARLOCK : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S02_AXI_M_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_ARREADY : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_ARVALID : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S02_AXI_M_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S02_AXI_M_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_AWID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S02_AXI_M_AXI_AWLOCK : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S02_AXI_M_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_AWREADY : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_AWVALID : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_BID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_BREADY : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S02_AXI_M_AXI_BVALID : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S02_AXI_M_AXI_RID : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S02_AXI_M_AXI_RLAST : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_RREADY : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S02_AXI_M_AXI_RVALID : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S02_AXI_M_AXI_WLAST : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_WREADY : STD_LOGIC;
  signal vip_S02_AXI_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal vip_S02_AXI_M_AXI_WVALID : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S04_AXI_M_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S04_AXI_M_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S04_AXI_M_AXI_ARLOCK : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S04_AXI_M_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S04_AXI_M_AXI_ARREADY : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_ARVALID : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S04_AXI_M_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S04_AXI_M_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S04_AXI_M_AXI_AWLOCK : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S04_AXI_M_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S04_AXI_M_AXI_AWREADY : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_AWVALID : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_BREADY : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S04_AXI_M_AXI_BVALID : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S04_AXI_M_AXI_RLAST : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_RREADY : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S04_AXI_M_AXI_RVALID : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S04_AXI_M_AXI_WLAST : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_WREADY : STD_LOGIC;
  signal vip_S04_AXI_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal vip_S04_AXI_M_AXI_WVALID : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S05_AXI_M_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S05_AXI_M_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S05_AXI_M_AXI_ARLOCK : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S05_AXI_M_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S05_AXI_M_AXI_ARREADY : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_ARVALID : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 38 downto 0 );
  signal vip_S05_AXI_M_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S05_AXI_M_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_S05_AXI_M_AXI_AWLOCK : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_S05_AXI_M_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_S05_AXI_M_AXI_AWREADY : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_AWVALID : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_BREADY : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S05_AXI_M_AXI_BVALID : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S05_AXI_M_AXI_RLAST : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_RREADY : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_S05_AXI_M_AXI_RVALID : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_S05_AXI_M_AXI_WLAST : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_WREADY : STD_LOGIC;
  signal vip_S05_AXI_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal vip_S05_AXI_M_AXI_WVALID : STD_LOGIC;
  signal NLW_interconnect_ddr4_mem01_M00_AXI_arsize_UNCONNECTED : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal NLW_interconnect_ddr4_mem01_M00_AXI_awsize_UNCONNECTED : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal NLW_interconnect_m00_axi_mem00_M00_AXI_arsize_UNCONNECTED : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal NLW_interconnect_m00_axi_mem00_M00_AXI_awsize_UNCONNECTED : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal NLW_vip_s01_axi_m_axi_arregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s01_axi_m_axi_awregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s02_axi_m_axi_arregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s02_axi_m_axi_awregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s04_axi_m_axi_arregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s04_axi_m_axi_awregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s05_axi_m_axi_arregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_vip_s05_axi_m_axi_awregion_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of interconnect_ddr4_mem01 : label is "bd_e7f0,Vivado 2020.1";
  attribute X_CORE_INFO of interconnect_m00_axi_mem00 : label is "bd_2b97,Vivado 2020.1";
  attribute X_CORE_INFO of interconnect_s04_axi : label is "bd_8bbf,Vivado 2020.1";
  attribute X_CORE_INFO of rs_m00_axi : label is "axi_register_slice_v2_1_21_axi_register_slice,Vivado 2020.1";
  attribute X_CORE_INFO of vip_m00_axi : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_s01_axi : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_s02_axi : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_s04_axi : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_s05_axi : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
begin
interconnect_ddr4_mem01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_DDR4_MEM01_0
     port map (
      M00_AXI_araddr(33 downto 0) => M00_AXI_araddr(33 downto 0),
      M00_AXI_arburst(1 downto 0) => M00_AXI_arburst(1 downto 0),
      M00_AXI_arcache(3 downto 0) => M00_AXI_arcache(3 downto 0),
      M00_AXI_arlen(7 downto 0) => M00_AXI_arlen(7 downto 0),
      M00_AXI_arlock(0) => M00_AXI_arlock(0),
      M00_AXI_arprot(2 downto 0) => M00_AXI_arprot(2 downto 0),
      M00_AXI_arqos(3 downto 0) => M00_AXI_arqos(3 downto 0),
      M00_AXI_arready => S_AXI_arready,
      M00_AXI_arsize(2 downto 0) => NLW_interconnect_ddr4_mem01_M00_AXI_arsize_UNCONNECTED(2 downto 0),
      M00_AXI_arvalid => M00_AXI_arvalid,
      M00_AXI_awaddr(33 downto 0) => M00_AXI_awaddr(33 downto 0),
      M00_AXI_awburst(1 downto 0) => M00_AXI_awburst(1 downto 0),
      M00_AXI_awcache(3 downto 0) => M00_AXI_awcache(3 downto 0),
      M00_AXI_awlen(7 downto 0) => M00_AXI_awlen(7 downto 0),
      M00_AXI_awlock(0) => M00_AXI_awlock(0),
      M00_AXI_awprot(2 downto 0) => M00_AXI_awprot(2 downto 0),
      M00_AXI_awqos(3 downto 0) => M00_AXI_awqos(3 downto 0),
      M00_AXI_awready => S_AXI_awready,
      M00_AXI_awsize(2 downto 0) => NLW_interconnect_ddr4_mem01_M00_AXI_awsize_UNCONNECTED(2 downto 0),
      M00_AXI_awvalid => M00_AXI_awvalid,
      M00_AXI_bready => M00_AXI_bready,
      M00_AXI_bresp(1 downto 0) => S_AXI_bresp(1 downto 0),
      M00_AXI_bvalid => S_AXI_bvalid,
      M00_AXI_rdata(511 downto 0) => S_AXI_rdata(511 downto 0),
      M00_AXI_rlast => S_AXI_rlast,
      M00_AXI_rready => M00_AXI_rready,
      M00_AXI_rresp(1 downto 0) => S_AXI_rresp(1 downto 0),
      M00_AXI_rvalid => S_AXI_rvalid,
      M00_AXI_wdata(511 downto 0) => M00_AXI_wdata(511 downto 0),
      M00_AXI_wlast => M00_AXI_wlast,
      M00_AXI_wready => S_AXI_wready,
      M00_AXI_wstrb(63 downto 0) => M00_AXI_wstrb(63 downto 0),
      M00_AXI_wvalid => M00_AXI_wvalid,
      S00_AXI_araddr(38 downto 0) => vip_S02_AXI_M_AXI_ARADDR(38 downto 0),
      S00_AXI_arburst(1 downto 0) => vip_S02_AXI_M_AXI_ARBURST(1 downto 0),
      S00_AXI_arcache(3 downto 0) => vip_S02_AXI_M_AXI_ARCACHE(3 downto 0),
      S00_AXI_arid(3 downto 0) => vip_S02_AXI_M_AXI_ARID(3 downto 0),
      S00_AXI_arlen(7 downto 0) => vip_S02_AXI_M_AXI_ARLEN(7 downto 0),
      S00_AXI_arlock(0) => vip_S02_AXI_M_AXI_ARLOCK,
      S00_AXI_arprot(2 downto 0) => vip_S02_AXI_M_AXI_ARPROT(2 downto 0),
      S00_AXI_arqos(3 downto 0) => vip_S02_AXI_M_AXI_ARQOS(3 downto 0),
      S00_AXI_arready => vip_S02_AXI_M_AXI_ARREADY,
      S00_AXI_arsize(2 downto 0) => B"110",
      S00_AXI_arvalid => vip_S02_AXI_M_AXI_ARVALID,
      S00_AXI_awaddr(38 downto 0) => vip_S02_AXI_M_AXI_AWADDR(38 downto 0),
      S00_AXI_awburst(1 downto 0) => vip_S02_AXI_M_AXI_AWBURST(1 downto 0),
      S00_AXI_awcache(3 downto 0) => vip_S02_AXI_M_AXI_AWCACHE(3 downto 0),
      S00_AXI_awid(3 downto 0) => vip_S02_AXI_M_AXI_AWID(3 downto 0),
      S00_AXI_awlen(7 downto 0) => vip_S02_AXI_M_AXI_AWLEN(7 downto 0),
      S00_AXI_awlock(0) => vip_S02_AXI_M_AXI_AWLOCK,
      S00_AXI_awprot(2 downto 0) => vip_S02_AXI_M_AXI_AWPROT(2 downto 0),
      S00_AXI_awqos(3 downto 0) => vip_S02_AXI_M_AXI_AWQOS(3 downto 0),
      S00_AXI_awready => vip_S02_AXI_M_AXI_AWREADY,
      S00_AXI_awsize(2 downto 0) => B"110",
      S00_AXI_awvalid => vip_S02_AXI_M_AXI_AWVALID,
      S00_AXI_bid(3 downto 0) => vip_S02_AXI_M_AXI_BID(3 downto 0),
      S00_AXI_bready => vip_S02_AXI_M_AXI_BREADY,
      S00_AXI_bresp(1 downto 0) => vip_S02_AXI_M_AXI_BRESP(1 downto 0),
      S00_AXI_bvalid => vip_S02_AXI_M_AXI_BVALID,
      S00_AXI_rdata(511 downto 0) => vip_S02_AXI_M_AXI_RDATA(511 downto 0),
      S00_AXI_rid(3 downto 0) => vip_S02_AXI_M_AXI_RID(3 downto 0),
      S00_AXI_rlast => vip_S02_AXI_M_AXI_RLAST,
      S00_AXI_rready => vip_S02_AXI_M_AXI_RREADY,
      S00_AXI_rresp(1 downto 0) => vip_S02_AXI_M_AXI_RRESP(1 downto 0),
      S00_AXI_rvalid => vip_S02_AXI_M_AXI_RVALID,
      S00_AXI_wdata(511 downto 0) => vip_S02_AXI_M_AXI_WDATA(511 downto 0),
      S00_AXI_wlast => vip_S02_AXI_M_AXI_WLAST,
      S00_AXI_wready => vip_S02_AXI_M_AXI_WREADY,
      S00_AXI_wstrb(63 downto 0) => vip_S02_AXI_M_AXI_WSTRB(63 downto 0),
      S00_AXI_wvalid => vip_S02_AXI_M_AXI_WVALID,
      S01_AXI_araddr(38 downto 0) => interconnect_S04_AXI_M00_AXI_ARADDR(38 downto 0),
      S01_AXI_arburst(1 downto 0) => interconnect_S04_AXI_M00_AXI_ARBURST(1 downto 0),
      S01_AXI_arcache(3 downto 0) => interconnect_S04_AXI_M00_AXI_ARCACHE(3 downto 0),
      S01_AXI_arid(1 downto 0) => interconnect_S04_AXI_M00_AXI_ARID(1 downto 0),
      S01_AXI_arlen(7 downto 0) => interconnect_S04_AXI_M00_AXI_ARLEN(7 downto 0),
      S01_AXI_arlock(0) => interconnect_S04_AXI_M00_AXI_ARLOCK,
      S01_AXI_arprot(2 downto 0) => interconnect_S04_AXI_M00_AXI_ARPROT(2 downto 0),
      S01_AXI_arqos(3 downto 0) => interconnect_S04_AXI_M00_AXI_ARQOS(3 downto 0),
      S01_AXI_arready => interconnect_S04_AXI_M00_AXI_ARREADY,
      S01_AXI_arsize(2 downto 0) => interconnect_S04_AXI_M00_AXI_ARSIZE(2 downto 0),
      S01_AXI_aruser(113 downto 0) => interconnect_S04_AXI_M00_AXI_ARUSER(113 downto 0),
      S01_AXI_arvalid => interconnect_S04_AXI_M00_AXI_ARVALID,
      S01_AXI_awaddr(38 downto 0) => interconnect_S04_AXI_M00_AXI_AWADDR(38 downto 0),
      S01_AXI_awburst(1 downto 0) => interconnect_S04_AXI_M00_AXI_AWBURST(1 downto 0),
      S01_AXI_awcache(3 downto 0) => interconnect_S04_AXI_M00_AXI_AWCACHE(3 downto 0),
      S01_AXI_awid(1 downto 0) => interconnect_S04_AXI_M00_AXI_AWID(1 downto 0),
      S01_AXI_awlen(7 downto 0) => interconnect_S04_AXI_M00_AXI_AWLEN(7 downto 0),
      S01_AXI_awlock(0) => interconnect_S04_AXI_M00_AXI_AWLOCK,
      S01_AXI_awprot(2 downto 0) => interconnect_S04_AXI_M00_AXI_AWPROT(2 downto 0),
      S01_AXI_awqos(3 downto 0) => interconnect_S04_AXI_M00_AXI_AWQOS(3 downto 0),
      S01_AXI_awready => interconnect_S04_AXI_M00_AXI_AWREADY,
      S01_AXI_awsize(2 downto 0) => interconnect_S04_AXI_M00_AXI_AWSIZE(2 downto 0),
      S01_AXI_awuser(113 downto 0) => interconnect_S04_AXI_M00_AXI_AWUSER(113 downto 0),
      S01_AXI_awvalid => interconnect_S04_AXI_M00_AXI_AWVALID,
      S01_AXI_bid(1 downto 0) => interconnect_S04_AXI_M00_AXI_BID(1 downto 0),
      S01_AXI_bready => interconnect_S04_AXI_M00_AXI_BREADY,
      S01_AXI_bresp(1 downto 0) => interconnect_S04_AXI_M00_AXI_BRESP(1 downto 0),
      S01_AXI_buser(113 downto 0) => interconnect_S04_AXI_M00_AXI_BUSER(113 downto 0),
      S01_AXI_bvalid => interconnect_S04_AXI_M00_AXI_BVALID,
      S01_AXI_rdata(511 downto 0) => interconnect_S04_AXI_M00_AXI_RDATA(511 downto 0),
      S01_AXI_rid(1 downto 0) => interconnect_S04_AXI_M00_AXI_RID(1 downto 0),
      S01_AXI_rlast => interconnect_S04_AXI_M00_AXI_RLAST,
      S01_AXI_rready => interconnect_S04_AXI_M00_AXI_RREADY,
      S01_AXI_rresp(1 downto 0) => interconnect_S04_AXI_M00_AXI_RRESP(1 downto 0),
      S01_AXI_ruser(13 downto 0) => interconnect_S04_AXI_M00_AXI_RUSER(13 downto 0),
      S01_AXI_rvalid => interconnect_S04_AXI_M00_AXI_RVALID,
      S01_AXI_wdata(511 downto 0) => interconnect_S04_AXI_M00_AXI_WDATA(511 downto 0),
      S01_AXI_wlast => interconnect_S04_AXI_M00_AXI_WLAST,
      S01_AXI_wready => interconnect_S04_AXI_M00_AXI_WREADY,
      S01_AXI_wstrb(63 downto 0) => interconnect_S04_AXI_M00_AXI_WSTRB(63 downto 0),
      S01_AXI_wuser(13 downto 0) => interconnect_S04_AXI_M00_AXI_WUSER(13 downto 0),
      S01_AXI_wvalid => interconnect_S04_AXI_M00_AXI_WVALID,
      aclk => ddr4_mem01_ui_clk,
      aclk1 => aclk,
      aresetn => interconnect_aresetn(0)
    );
interconnect_m00_axi_mem00: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_M00_AXI_MEM00_0
     port map (
      M00_AXI_araddr(38 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARADDR(38 downto 0),
      M00_AXI_arburst(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARBURST(1 downto 0),
      M00_AXI_arcache(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARCACHE(3 downto 0),
      M00_AXI_arlen(7 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARLEN(7 downto 0),
      M00_AXI_arlock(0) => interconnect_M00_AXI_MEM00_M00_AXI_ARLOCK,
      M00_AXI_arprot(2 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARPROT(2 downto 0),
      M00_AXI_arqos(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARQOS(3 downto 0),
      M00_AXI_arready => interconnect_M00_AXI_MEM00_M00_AXI_ARREADY,
      M00_AXI_arsize(2 downto 0) => NLW_interconnect_m00_axi_mem00_M00_AXI_arsize_UNCONNECTED(2 downto 0),
      M00_AXI_arvalid => interconnect_M00_AXI_MEM00_M00_AXI_ARVALID,
      M00_AXI_awaddr(38 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWADDR(38 downto 0),
      M00_AXI_awburst(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWBURST(1 downto 0),
      M00_AXI_awcache(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWCACHE(3 downto 0),
      M00_AXI_awlen(7 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWLEN(7 downto 0),
      M00_AXI_awlock(0) => interconnect_M00_AXI_MEM00_M00_AXI_AWLOCK,
      M00_AXI_awprot(2 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWPROT(2 downto 0),
      M00_AXI_awqos(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWQOS(3 downto 0),
      M00_AXI_awready => interconnect_M00_AXI_MEM00_M00_AXI_AWREADY,
      M00_AXI_awsize(2 downto 0) => NLW_interconnect_m00_axi_mem00_M00_AXI_awsize_UNCONNECTED(2 downto 0),
      M00_AXI_awvalid => interconnect_M00_AXI_MEM00_M00_AXI_AWVALID,
      M00_AXI_bready => interconnect_M00_AXI_MEM00_M00_AXI_BREADY,
      M00_AXI_bresp(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_BRESP(1 downto 0),
      M00_AXI_bvalid => interconnect_M00_AXI_MEM00_M00_AXI_BVALID,
      M00_AXI_rdata(511 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_RDATA(511 downto 0),
      M00_AXI_rlast => interconnect_M00_AXI_MEM00_M00_AXI_RLAST,
      M00_AXI_rready => interconnect_M00_AXI_MEM00_M00_AXI_RREADY,
      M00_AXI_rresp(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_RRESP(1 downto 0),
      M00_AXI_rvalid => interconnect_M00_AXI_MEM00_M00_AXI_RVALID,
      M00_AXI_wdata(511 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_WDATA(511 downto 0),
      M00_AXI_wlast => interconnect_M00_AXI_MEM00_M00_AXI_WLAST,
      M00_AXI_wready => interconnect_M00_AXI_MEM00_M00_AXI_WREADY,
      M00_AXI_wstrb(63 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_WSTRB(63 downto 0),
      M00_AXI_wvalid => interconnect_M00_AXI_MEM00_M00_AXI_WVALID,
      S00_AXI_araddr(38 downto 0) => vip_S01_AXI_M_AXI_ARADDR(38 downto 0),
      S00_AXI_arburst(1 downto 0) => vip_S01_AXI_M_AXI_ARBURST(1 downto 0),
      S00_AXI_arcache(3 downto 0) => vip_S01_AXI_M_AXI_ARCACHE(3 downto 0),
      S00_AXI_arid(3 downto 0) => vip_S01_AXI_M_AXI_ARID(3 downto 0),
      S00_AXI_arlen(7 downto 0) => vip_S01_AXI_M_AXI_ARLEN(7 downto 0),
      S00_AXI_arlock(0) => vip_S01_AXI_M_AXI_ARLOCK,
      S00_AXI_arprot(2 downto 0) => vip_S01_AXI_M_AXI_ARPROT(2 downto 0),
      S00_AXI_arqos(3 downto 0) => vip_S01_AXI_M_AXI_ARQOS(3 downto 0),
      S00_AXI_arready => vip_S01_AXI_M_AXI_ARREADY,
      S00_AXI_arsize(2 downto 0) => B"110",
      S00_AXI_arvalid => vip_S01_AXI_M_AXI_ARVALID,
      S00_AXI_awaddr(38 downto 0) => vip_S01_AXI_M_AXI_AWADDR(38 downto 0),
      S00_AXI_awburst(1 downto 0) => vip_S01_AXI_M_AXI_AWBURST(1 downto 0),
      S00_AXI_awcache(3 downto 0) => vip_S01_AXI_M_AXI_AWCACHE(3 downto 0),
      S00_AXI_awid(3 downto 0) => vip_S01_AXI_M_AXI_AWID(3 downto 0),
      S00_AXI_awlen(7 downto 0) => vip_S01_AXI_M_AXI_AWLEN(7 downto 0),
      S00_AXI_awlock(0) => vip_S01_AXI_M_AXI_AWLOCK,
      S00_AXI_awprot(2 downto 0) => vip_S01_AXI_M_AXI_AWPROT(2 downto 0),
      S00_AXI_awqos(3 downto 0) => vip_S01_AXI_M_AXI_AWQOS(3 downto 0),
      S00_AXI_awready => vip_S01_AXI_M_AXI_AWREADY,
      S00_AXI_awsize(2 downto 0) => B"110",
      S00_AXI_awvalid => vip_S01_AXI_M_AXI_AWVALID,
      S00_AXI_bid(3 downto 0) => vip_S01_AXI_M_AXI_BID(3 downto 0),
      S00_AXI_bready => vip_S01_AXI_M_AXI_BREADY,
      S00_AXI_bresp(1 downto 0) => vip_S01_AXI_M_AXI_BRESP(1 downto 0),
      S00_AXI_bvalid => vip_S01_AXI_M_AXI_BVALID,
      S00_AXI_rdata(511 downto 0) => vip_S01_AXI_M_AXI_RDATA(511 downto 0),
      S00_AXI_rid(3 downto 0) => vip_S01_AXI_M_AXI_RID(3 downto 0),
      S00_AXI_rlast => vip_S01_AXI_M_AXI_RLAST,
      S00_AXI_rready => vip_S01_AXI_M_AXI_RREADY,
      S00_AXI_rresp(1 downto 0) => vip_S01_AXI_M_AXI_RRESP(1 downto 0),
      S00_AXI_rvalid => vip_S01_AXI_M_AXI_RVALID,
      S00_AXI_wdata(511 downto 0) => vip_S01_AXI_M_AXI_WDATA(511 downto 0),
      S00_AXI_wlast => vip_S01_AXI_M_AXI_WLAST,
      S00_AXI_wready => vip_S01_AXI_M_AXI_WREADY,
      S00_AXI_wstrb(63 downto 0) => vip_S01_AXI_M_AXI_WSTRB(63 downto 0),
      S00_AXI_wvalid => vip_S01_AXI_M_AXI_WVALID,
      S01_AXI_araddr(38 downto 0) => vip_S05_AXI_M_AXI_ARADDR(38 downto 0),
      S01_AXI_arburst(1 downto 0) => B"01",
      S01_AXI_arcache(3 downto 0) => vip_S05_AXI_M_AXI_ARCACHE(3 downto 0),
      S01_AXI_arlen(7 downto 0) => vip_S05_AXI_M_AXI_ARLEN(7 downto 0),
      S01_AXI_arlock(0) => vip_S05_AXI_M_AXI_ARLOCK,
      S01_AXI_arprot(2 downto 0) => vip_S05_AXI_M_AXI_ARPROT(2 downto 0),
      S01_AXI_arqos(3 downto 0) => vip_S05_AXI_M_AXI_ARQOS(3 downto 0),
      S01_AXI_arready => vip_S05_AXI_M_AXI_ARREADY,
      S01_AXI_arsize(2 downto 0) => B"110",
      S01_AXI_arvalid => vip_S05_AXI_M_AXI_ARVALID,
      S01_AXI_awaddr(38 downto 0) => vip_S05_AXI_M_AXI_AWADDR(38 downto 0),
      S01_AXI_awburst(1 downto 0) => B"01",
      S01_AXI_awcache(3 downto 0) => vip_S05_AXI_M_AXI_AWCACHE(3 downto 0),
      S01_AXI_awlen(7 downto 0) => vip_S05_AXI_M_AXI_AWLEN(7 downto 0),
      S01_AXI_awlock(0) => vip_S05_AXI_M_AXI_AWLOCK,
      S01_AXI_awprot(2 downto 0) => vip_S05_AXI_M_AXI_AWPROT(2 downto 0),
      S01_AXI_awqos(3 downto 0) => vip_S05_AXI_M_AXI_AWQOS(3 downto 0),
      S01_AXI_awready => vip_S05_AXI_M_AXI_AWREADY,
      S01_AXI_awsize(2 downto 0) => B"110",
      S01_AXI_awvalid => vip_S05_AXI_M_AXI_AWVALID,
      S01_AXI_bready => vip_S05_AXI_M_AXI_BREADY,
      S01_AXI_bresp(1 downto 0) => vip_S05_AXI_M_AXI_BRESP(1 downto 0),
      S01_AXI_bvalid => vip_S05_AXI_M_AXI_BVALID,
      S01_AXI_rdata(511 downto 0) => vip_S05_AXI_M_AXI_RDATA(511 downto 0),
      S01_AXI_rlast => vip_S05_AXI_M_AXI_RLAST,
      S01_AXI_rready => vip_S05_AXI_M_AXI_RREADY,
      S01_AXI_rresp(1 downto 0) => vip_S05_AXI_M_AXI_RRESP(1 downto 0),
      S01_AXI_rvalid => vip_S05_AXI_M_AXI_RVALID,
      S01_AXI_wdata(511 downto 0) => vip_S05_AXI_M_AXI_WDATA(511 downto 0),
      S01_AXI_wlast => vip_S05_AXI_M_AXI_WLAST,
      S01_AXI_wready => vip_S05_AXI_M_AXI_WREADY,
      S01_AXI_wstrb(63 downto 0) => vip_S05_AXI_M_AXI_WSTRB(63 downto 0),
      S01_AXI_wvalid => vip_S05_AXI_M_AXI_WVALID,
      S02_AXI_araddr(38 downto 0) => interconnect_S04_AXI_M01_AXI_ARADDR(38 downto 0),
      S02_AXI_arburst(1 downto 0) => interconnect_S04_AXI_M01_AXI_ARBURST(1 downto 0),
      S02_AXI_arcache(3 downto 0) => interconnect_S04_AXI_M01_AXI_ARCACHE(3 downto 0),
      S02_AXI_arid(1 downto 0) => interconnect_S04_AXI_M01_AXI_ARID(1 downto 0),
      S02_AXI_arlen(7 downto 0) => interconnect_S04_AXI_M01_AXI_ARLEN(7 downto 0),
      S02_AXI_arlock(0) => interconnect_S04_AXI_M01_AXI_ARLOCK,
      S02_AXI_arprot(2 downto 0) => interconnect_S04_AXI_M01_AXI_ARPROT(2 downto 0),
      S02_AXI_arqos(3 downto 0) => interconnect_S04_AXI_M01_AXI_ARQOS(3 downto 0),
      S02_AXI_arready => interconnect_S04_AXI_M01_AXI_ARREADY,
      S02_AXI_arsize(2 downto 0) => interconnect_S04_AXI_M01_AXI_ARSIZE(2 downto 0),
      S02_AXI_aruser(113 downto 0) => interconnect_S04_AXI_M01_AXI_ARUSER(113 downto 0),
      S02_AXI_arvalid => interconnect_S04_AXI_M01_AXI_ARVALID,
      S02_AXI_awaddr(38 downto 0) => interconnect_S04_AXI_M01_AXI_AWADDR(38 downto 0),
      S02_AXI_awburst(1 downto 0) => interconnect_S04_AXI_M01_AXI_AWBURST(1 downto 0),
      S02_AXI_awcache(3 downto 0) => interconnect_S04_AXI_M01_AXI_AWCACHE(3 downto 0),
      S02_AXI_awid(1 downto 0) => interconnect_S04_AXI_M01_AXI_AWID(1 downto 0),
      S02_AXI_awlen(7 downto 0) => interconnect_S04_AXI_M01_AXI_AWLEN(7 downto 0),
      S02_AXI_awlock(0) => interconnect_S04_AXI_M01_AXI_AWLOCK,
      S02_AXI_awprot(2 downto 0) => interconnect_S04_AXI_M01_AXI_AWPROT(2 downto 0),
      S02_AXI_awqos(3 downto 0) => interconnect_S04_AXI_M01_AXI_AWQOS(3 downto 0),
      S02_AXI_awready => interconnect_S04_AXI_M01_AXI_AWREADY,
      S02_AXI_awsize(2 downto 0) => interconnect_S04_AXI_M01_AXI_AWSIZE(2 downto 0),
      S02_AXI_awuser(113 downto 0) => interconnect_S04_AXI_M01_AXI_AWUSER(113 downto 0),
      S02_AXI_awvalid => interconnect_S04_AXI_M01_AXI_AWVALID,
      S02_AXI_bid(1 downto 0) => interconnect_S04_AXI_M01_AXI_BID(1 downto 0),
      S02_AXI_bready => interconnect_S04_AXI_M01_AXI_BREADY,
      S02_AXI_bresp(1 downto 0) => interconnect_S04_AXI_M01_AXI_BRESP(1 downto 0),
      S02_AXI_buser(113 downto 0) => interconnect_S04_AXI_M01_AXI_BUSER(113 downto 0),
      S02_AXI_bvalid => interconnect_S04_AXI_M01_AXI_BVALID,
      S02_AXI_rdata(511 downto 0) => interconnect_S04_AXI_M01_AXI_RDATA(511 downto 0),
      S02_AXI_rid(1 downto 0) => interconnect_S04_AXI_M01_AXI_RID(1 downto 0),
      S02_AXI_rlast => interconnect_S04_AXI_M01_AXI_RLAST,
      S02_AXI_rready => interconnect_S04_AXI_M01_AXI_RREADY,
      S02_AXI_rresp(1 downto 0) => interconnect_S04_AXI_M01_AXI_RRESP(1 downto 0),
      S02_AXI_ruser(13 downto 0) => interconnect_S04_AXI_M01_AXI_RUSER(13 downto 0),
      S02_AXI_rvalid => interconnect_S04_AXI_M01_AXI_RVALID,
      S02_AXI_wdata(511 downto 0) => interconnect_S04_AXI_M01_AXI_WDATA(511 downto 0),
      S02_AXI_wlast => interconnect_S04_AXI_M01_AXI_WLAST,
      S02_AXI_wready => interconnect_S04_AXI_M01_AXI_WREADY,
      S02_AXI_wstrb(63 downto 0) => interconnect_S04_AXI_M01_AXI_WSTRB(63 downto 0),
      S02_AXI_wuser(13 downto 0) => interconnect_S04_AXI_M01_AXI_WUSER(13 downto 0),
      S02_AXI_wvalid => interconnect_S04_AXI_M01_AXI_WVALID,
      aclk => aclk2,
      aclk1 => aclk,
      aresetn => interconnect_aresetn1(0)
    );
interconnect_s04_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_S04_AXI_0
     port map (
      M00_AXI_araddr(38 downto 0) => interconnect_S04_AXI_M00_AXI_ARADDR(38 downto 0),
      M00_AXI_arburst(1 downto 0) => interconnect_S04_AXI_M00_AXI_ARBURST(1 downto 0),
      M00_AXI_arcache(3 downto 0) => interconnect_S04_AXI_M00_AXI_ARCACHE(3 downto 0),
      M00_AXI_arid(1 downto 0) => interconnect_S04_AXI_M00_AXI_ARID(1 downto 0),
      M00_AXI_arlen(7 downto 0) => interconnect_S04_AXI_M00_AXI_ARLEN(7 downto 0),
      M00_AXI_arlock(0) => interconnect_S04_AXI_M00_AXI_ARLOCK,
      M00_AXI_arprot(2 downto 0) => interconnect_S04_AXI_M00_AXI_ARPROT(2 downto 0),
      M00_AXI_arqos(3 downto 0) => interconnect_S04_AXI_M00_AXI_ARQOS(3 downto 0),
      M00_AXI_arready => interconnect_S04_AXI_M00_AXI_ARREADY,
      M00_AXI_arsize(2 downto 0) => interconnect_S04_AXI_M00_AXI_ARSIZE(2 downto 0),
      M00_AXI_aruser(113 downto 0) => interconnect_S04_AXI_M00_AXI_ARUSER(113 downto 0),
      M00_AXI_arvalid => interconnect_S04_AXI_M00_AXI_ARVALID,
      M00_AXI_awaddr(38 downto 0) => interconnect_S04_AXI_M00_AXI_AWADDR(38 downto 0),
      M00_AXI_awburst(1 downto 0) => interconnect_S04_AXI_M00_AXI_AWBURST(1 downto 0),
      M00_AXI_awcache(3 downto 0) => interconnect_S04_AXI_M00_AXI_AWCACHE(3 downto 0),
      M00_AXI_awid(1 downto 0) => interconnect_S04_AXI_M00_AXI_AWID(1 downto 0),
      M00_AXI_awlen(7 downto 0) => interconnect_S04_AXI_M00_AXI_AWLEN(7 downto 0),
      M00_AXI_awlock(0) => interconnect_S04_AXI_M00_AXI_AWLOCK,
      M00_AXI_awprot(2 downto 0) => interconnect_S04_AXI_M00_AXI_AWPROT(2 downto 0),
      M00_AXI_awqos(3 downto 0) => interconnect_S04_AXI_M00_AXI_AWQOS(3 downto 0),
      M00_AXI_awready => interconnect_S04_AXI_M00_AXI_AWREADY,
      M00_AXI_awsize(2 downto 0) => interconnect_S04_AXI_M00_AXI_AWSIZE(2 downto 0),
      M00_AXI_awuser(113 downto 0) => interconnect_S04_AXI_M00_AXI_AWUSER(113 downto 0),
      M00_AXI_awvalid => interconnect_S04_AXI_M00_AXI_AWVALID,
      M00_AXI_bid(1 downto 0) => interconnect_S04_AXI_M00_AXI_BID(1 downto 0),
      M00_AXI_bready => interconnect_S04_AXI_M00_AXI_BREADY,
      M00_AXI_bresp(1 downto 0) => interconnect_S04_AXI_M00_AXI_BRESP(1 downto 0),
      M00_AXI_buser(113 downto 0) => interconnect_S04_AXI_M00_AXI_BUSER(113 downto 0),
      M00_AXI_bvalid => interconnect_S04_AXI_M00_AXI_BVALID,
      M00_AXI_rdata(511 downto 0) => interconnect_S04_AXI_M00_AXI_RDATA(511 downto 0),
      M00_AXI_rid(1 downto 0) => interconnect_S04_AXI_M00_AXI_RID(1 downto 0),
      M00_AXI_rlast => interconnect_S04_AXI_M00_AXI_RLAST,
      M00_AXI_rready => interconnect_S04_AXI_M00_AXI_RREADY,
      M00_AXI_rresp(1 downto 0) => interconnect_S04_AXI_M00_AXI_RRESP(1 downto 0),
      M00_AXI_ruser(13 downto 0) => interconnect_S04_AXI_M00_AXI_RUSER(13 downto 0),
      M00_AXI_rvalid => interconnect_S04_AXI_M00_AXI_RVALID,
      M00_AXI_wdata(511 downto 0) => interconnect_S04_AXI_M00_AXI_WDATA(511 downto 0),
      M00_AXI_wlast => interconnect_S04_AXI_M00_AXI_WLAST,
      M00_AXI_wready => interconnect_S04_AXI_M00_AXI_WREADY,
      M00_AXI_wstrb(63 downto 0) => interconnect_S04_AXI_M00_AXI_WSTRB(63 downto 0),
      M00_AXI_wuser(13 downto 0) => interconnect_S04_AXI_M00_AXI_WUSER(13 downto 0),
      M00_AXI_wvalid => interconnect_S04_AXI_M00_AXI_WVALID,
      M01_AXI_araddr(38 downto 0) => interconnect_S04_AXI_M01_AXI_ARADDR(38 downto 0),
      M01_AXI_arburst(1 downto 0) => interconnect_S04_AXI_M01_AXI_ARBURST(1 downto 0),
      M01_AXI_arcache(3 downto 0) => interconnect_S04_AXI_M01_AXI_ARCACHE(3 downto 0),
      M01_AXI_arid(1 downto 0) => interconnect_S04_AXI_M01_AXI_ARID(1 downto 0),
      M01_AXI_arlen(7 downto 0) => interconnect_S04_AXI_M01_AXI_ARLEN(7 downto 0),
      M01_AXI_arlock(0) => interconnect_S04_AXI_M01_AXI_ARLOCK,
      M01_AXI_arprot(2 downto 0) => interconnect_S04_AXI_M01_AXI_ARPROT(2 downto 0),
      M01_AXI_arqos(3 downto 0) => interconnect_S04_AXI_M01_AXI_ARQOS(3 downto 0),
      M01_AXI_arready => interconnect_S04_AXI_M01_AXI_ARREADY,
      M01_AXI_arsize(2 downto 0) => interconnect_S04_AXI_M01_AXI_ARSIZE(2 downto 0),
      M01_AXI_aruser(113 downto 0) => interconnect_S04_AXI_M01_AXI_ARUSER(113 downto 0),
      M01_AXI_arvalid => interconnect_S04_AXI_M01_AXI_ARVALID,
      M01_AXI_awaddr(38 downto 0) => interconnect_S04_AXI_M01_AXI_AWADDR(38 downto 0),
      M01_AXI_awburst(1 downto 0) => interconnect_S04_AXI_M01_AXI_AWBURST(1 downto 0),
      M01_AXI_awcache(3 downto 0) => interconnect_S04_AXI_M01_AXI_AWCACHE(3 downto 0),
      M01_AXI_awid(1 downto 0) => interconnect_S04_AXI_M01_AXI_AWID(1 downto 0),
      M01_AXI_awlen(7 downto 0) => interconnect_S04_AXI_M01_AXI_AWLEN(7 downto 0),
      M01_AXI_awlock(0) => interconnect_S04_AXI_M01_AXI_AWLOCK,
      M01_AXI_awprot(2 downto 0) => interconnect_S04_AXI_M01_AXI_AWPROT(2 downto 0),
      M01_AXI_awqos(3 downto 0) => interconnect_S04_AXI_M01_AXI_AWQOS(3 downto 0),
      M01_AXI_awready => interconnect_S04_AXI_M01_AXI_AWREADY,
      M01_AXI_awsize(2 downto 0) => interconnect_S04_AXI_M01_AXI_AWSIZE(2 downto 0),
      M01_AXI_awuser(113 downto 0) => interconnect_S04_AXI_M01_AXI_AWUSER(113 downto 0),
      M01_AXI_awvalid => interconnect_S04_AXI_M01_AXI_AWVALID,
      M01_AXI_bid(1 downto 0) => interconnect_S04_AXI_M01_AXI_BID(1 downto 0),
      M01_AXI_bready => interconnect_S04_AXI_M01_AXI_BREADY,
      M01_AXI_bresp(1 downto 0) => interconnect_S04_AXI_M01_AXI_BRESP(1 downto 0),
      M01_AXI_buser(113 downto 0) => interconnect_S04_AXI_M01_AXI_BUSER(113 downto 0),
      M01_AXI_bvalid => interconnect_S04_AXI_M01_AXI_BVALID,
      M01_AXI_rdata(511 downto 0) => interconnect_S04_AXI_M01_AXI_RDATA(511 downto 0),
      M01_AXI_rid(1 downto 0) => interconnect_S04_AXI_M01_AXI_RID(1 downto 0),
      M01_AXI_rlast => interconnect_S04_AXI_M01_AXI_RLAST,
      M01_AXI_rready => interconnect_S04_AXI_M01_AXI_RREADY,
      M01_AXI_rresp(1 downto 0) => interconnect_S04_AXI_M01_AXI_RRESP(1 downto 0),
      M01_AXI_ruser(13 downto 0) => interconnect_S04_AXI_M01_AXI_RUSER(13 downto 0),
      M01_AXI_rvalid => interconnect_S04_AXI_M01_AXI_RVALID,
      M01_AXI_wdata(511 downto 0) => interconnect_S04_AXI_M01_AXI_WDATA(511 downto 0),
      M01_AXI_wlast => interconnect_S04_AXI_M01_AXI_WLAST,
      M01_AXI_wready => interconnect_S04_AXI_M01_AXI_WREADY,
      M01_AXI_wstrb(63 downto 0) => interconnect_S04_AXI_M01_AXI_WSTRB(63 downto 0),
      M01_AXI_wuser(13 downto 0) => interconnect_S04_AXI_M01_AXI_WUSER(13 downto 0),
      M01_AXI_wvalid => interconnect_S04_AXI_M01_AXI_WVALID,
      S00_AXI_araddr(38 downto 0) => vip_S04_AXI_M_AXI_ARADDR(38 downto 0),
      S00_AXI_arburst(1 downto 0) => B"01",
      S00_AXI_arcache(3 downto 0) => vip_S04_AXI_M_AXI_ARCACHE(3 downto 0),
      S00_AXI_arlen(7 downto 0) => vip_S04_AXI_M_AXI_ARLEN(7 downto 0),
      S00_AXI_arlock(0) => vip_S04_AXI_M_AXI_ARLOCK,
      S00_AXI_arprot(2 downto 0) => vip_S04_AXI_M_AXI_ARPROT(2 downto 0),
      S00_AXI_arqos(3 downto 0) => vip_S04_AXI_M_AXI_ARQOS(3 downto 0),
      S00_AXI_arready => vip_S04_AXI_M_AXI_ARREADY,
      S00_AXI_arsize(2 downto 0) => B"110",
      S00_AXI_arvalid => vip_S04_AXI_M_AXI_ARVALID,
      S00_AXI_awaddr(38 downto 0) => vip_S04_AXI_M_AXI_AWADDR(38 downto 0),
      S00_AXI_awburst(1 downto 0) => B"01",
      S00_AXI_awcache(3 downto 0) => vip_S04_AXI_M_AXI_AWCACHE(3 downto 0),
      S00_AXI_awlen(7 downto 0) => vip_S04_AXI_M_AXI_AWLEN(7 downto 0),
      S00_AXI_awlock(0) => vip_S04_AXI_M_AXI_AWLOCK,
      S00_AXI_awprot(2 downto 0) => vip_S04_AXI_M_AXI_AWPROT(2 downto 0),
      S00_AXI_awqos(3 downto 0) => vip_S04_AXI_M_AXI_AWQOS(3 downto 0),
      S00_AXI_awready => vip_S04_AXI_M_AXI_AWREADY,
      S00_AXI_awsize(2 downto 0) => B"110",
      S00_AXI_awvalid => vip_S04_AXI_M_AXI_AWVALID,
      S00_AXI_bready => vip_S04_AXI_M_AXI_BREADY,
      S00_AXI_bresp(1 downto 0) => vip_S04_AXI_M_AXI_BRESP(1 downto 0),
      S00_AXI_bvalid => vip_S04_AXI_M_AXI_BVALID,
      S00_AXI_rdata(511 downto 0) => vip_S04_AXI_M_AXI_RDATA(511 downto 0),
      S00_AXI_rlast => vip_S04_AXI_M_AXI_RLAST,
      S00_AXI_rready => vip_S04_AXI_M_AXI_RREADY,
      S00_AXI_rresp(1 downto 0) => vip_S04_AXI_M_AXI_RRESP(1 downto 0),
      S00_AXI_rvalid => vip_S04_AXI_M_AXI_RVALID,
      S00_AXI_wdata(511 downto 0) => vip_S04_AXI_M_AXI_WDATA(511 downto 0),
      S00_AXI_wlast => vip_S04_AXI_M_AXI_WLAST,
      S00_AXI_wready => vip_S04_AXI_M_AXI_WREADY,
      S00_AXI_wstrb(63 downto 0) => vip_S04_AXI_M_AXI_WSTRB(63 downto 0),
      S00_AXI_wvalid => vip_S04_AXI_M_AXI_WVALID,
      aclk => aclk,
      aresetn => interconnect_aresetn(0)
    );
rs_m00_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_rs_M00_AXI_0
     port map (
      aclk => aclk2,
      aresetn => interconnect_aresetn1(0),
      m_axi_araddr(38 downto 0) => \bbstub_m_axi_araddr[38]\(38 downto 0),
      m_axi_arburst(1 downto 0) => \bbstub_m_axi_arburst[1]\(1 downto 0),
      m_axi_arcache(3 downto 0) => \bbstub_m_axi_arcache[3]\(3 downto 0),
      m_axi_arlen(7 downto 0) => \bbstub_m_axi_arlen[7]\(7 downto 0),
      m_axi_arlock(0) => \bbstub_m_axi_arlock[0]\(0),
      m_axi_arprot(2 downto 0) => \bbstub_m_axi_arprot[2]\(2 downto 0),
      m_axi_arqos(3 downto 0) => \bbstub_m_axi_arqos[3]\(3 downto 0),
      m_axi_arready => M00_AXI_arready,
      m_axi_arregion(3 downto 0) => M00_AXI_arregion(3 downto 0),
      m_axi_arsize(2 downto 0) => M00_AXI_arsize(2 downto 0),
      m_axi_arvalid => bbstub_m_axi_arvalid,
      m_axi_awaddr(38 downto 0) => \bbstub_m_axi_awaddr[38]\(38 downto 0),
      m_axi_awburst(1 downto 0) => \bbstub_m_axi_awburst[1]\(1 downto 0),
      m_axi_awcache(3 downto 0) => \bbstub_m_axi_awcache[3]\(3 downto 0),
      m_axi_awlen(7 downto 0) => \bbstub_m_axi_awlen[7]\(7 downto 0),
      m_axi_awlock(0) => \bbstub_m_axi_awlock[0]\(0),
      m_axi_awprot(2 downto 0) => \bbstub_m_axi_awprot[2]\(2 downto 0),
      m_axi_awqos(3 downto 0) => \bbstub_m_axi_awqos[3]\(3 downto 0),
      m_axi_awready => M00_AXI_awready,
      m_axi_awregion(3 downto 0) => M00_AXI_awregion(3 downto 0),
      m_axi_awsize(2 downto 0) => M00_AXI_awsize(2 downto 0),
      m_axi_awvalid => bbstub_m_axi_awvalid,
      m_axi_bready => bbstub_m_axi_bready,
      m_axi_bresp(1 downto 0) => M00_AXI_bresp(1 downto 0),
      m_axi_bvalid => M00_AXI_bvalid,
      m_axi_rdata(511 downto 0) => M00_AXI_rdata(511 downto 0),
      m_axi_rlast => M00_AXI_rlast,
      m_axi_rready => bbstub_m_axi_rready,
      m_axi_rresp(1 downto 0) => M00_AXI_rresp(1 downto 0),
      m_axi_rvalid => M00_AXI_rvalid,
      m_axi_wdata(511 downto 0) => \bbstub_m_axi_wdata[511]\(511 downto 0),
      m_axi_wlast => bbstub_m_axi_wlast,
      m_axi_wready => M00_AXI_wready,
      m_axi_wstrb(63 downto 0) => \bbstub_m_axi_wstrb[63]\(63 downto 0),
      m_axi_wvalid => bbstub_m_axi_wvalid,
      s_axi_araddr(38 downto 0) => vip_M00_AXI_M_AXI_ARADDR(38 downto 0),
      s_axi_arburst(1 downto 0) => vip_M00_AXI_M_AXI_ARBURST(1 downto 0),
      s_axi_arcache(3 downto 0) => vip_M00_AXI_M_AXI_ARCACHE(3 downto 0),
      s_axi_arlen(7 downto 0) => vip_M00_AXI_M_AXI_ARLEN(7 downto 0),
      s_axi_arlock(0) => vip_M00_AXI_M_AXI_ARLOCK,
      s_axi_arprot(2 downto 0) => vip_M00_AXI_M_AXI_ARPROT(2 downto 0),
      s_axi_arqos(3 downto 0) => vip_M00_AXI_M_AXI_ARQOS(3 downto 0),
      s_axi_arready => vip_M00_AXI_M_AXI_ARREADY,
      s_axi_arregion(3 downto 0) => B"0000",
      s_axi_arsize(2 downto 0) => B"110",
      s_axi_arvalid => vip_M00_AXI_M_AXI_ARVALID,
      s_axi_awaddr(38 downto 0) => vip_M00_AXI_M_AXI_AWADDR(38 downto 0),
      s_axi_awburst(1 downto 0) => vip_M00_AXI_M_AXI_AWBURST(1 downto 0),
      s_axi_awcache(3 downto 0) => vip_M00_AXI_M_AXI_AWCACHE(3 downto 0),
      s_axi_awlen(7 downto 0) => vip_M00_AXI_M_AXI_AWLEN(7 downto 0),
      s_axi_awlock(0) => vip_M00_AXI_M_AXI_AWLOCK,
      s_axi_awprot(2 downto 0) => vip_M00_AXI_M_AXI_AWPROT(2 downto 0),
      s_axi_awqos(3 downto 0) => vip_M00_AXI_M_AXI_AWQOS(3 downto 0),
      s_axi_awready => vip_M00_AXI_M_AXI_AWREADY,
      s_axi_awregion(3 downto 0) => B"0000",
      s_axi_awsize(2 downto 0) => B"110",
      s_axi_awvalid => vip_M00_AXI_M_AXI_AWVALID,
      s_axi_bready => vip_M00_AXI_M_AXI_BREADY,
      s_axi_bresp(1 downto 0) => vip_M00_AXI_M_AXI_BRESP(1 downto 0),
      s_axi_bvalid => vip_M00_AXI_M_AXI_BVALID,
      s_axi_rdata(511 downto 0) => vip_M00_AXI_M_AXI_RDATA(511 downto 0),
      s_axi_rlast => vip_M00_AXI_M_AXI_RLAST,
      s_axi_rready => vip_M00_AXI_M_AXI_RREADY,
      s_axi_rresp(1 downto 0) => vip_M00_AXI_M_AXI_RRESP(1 downto 0),
      s_axi_rvalid => vip_M00_AXI_M_AXI_RVALID,
      s_axi_wdata(511 downto 0) => vip_M00_AXI_M_AXI_WDATA(511 downto 0),
      s_axi_wlast => vip_M00_AXI_M_AXI_WLAST,
      s_axi_wready => vip_M00_AXI_M_AXI_WREADY,
      s_axi_wstrb(63 downto 0) => vip_M00_AXI_M_AXI_WSTRB(63 downto 0),
      s_axi_wvalid => vip_M00_AXI_M_AXI_WVALID
    );
vip_m00_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_M00_AXI_0
     port map (
      aclk => aclk2,
      aresetn => interconnect_aresetn1(0),
      m_axi_araddr(38 downto 0) => vip_M00_AXI_M_AXI_ARADDR(38 downto 0),
      m_axi_arburst(1 downto 0) => vip_M00_AXI_M_AXI_ARBURST(1 downto 0),
      m_axi_arcache(3 downto 0) => vip_M00_AXI_M_AXI_ARCACHE(3 downto 0),
      m_axi_arlen(7 downto 0) => vip_M00_AXI_M_AXI_ARLEN(7 downto 0),
      m_axi_arlock(0) => vip_M00_AXI_M_AXI_ARLOCK,
      m_axi_arprot(2 downto 0) => vip_M00_AXI_M_AXI_ARPROT(2 downto 0),
      m_axi_arqos(3 downto 0) => vip_M00_AXI_M_AXI_ARQOS(3 downto 0),
      m_axi_arready => vip_M00_AXI_M_AXI_ARREADY,
      m_axi_arvalid => vip_M00_AXI_M_AXI_ARVALID,
      m_axi_awaddr(38 downto 0) => vip_M00_AXI_M_AXI_AWADDR(38 downto 0),
      m_axi_awburst(1 downto 0) => vip_M00_AXI_M_AXI_AWBURST(1 downto 0),
      m_axi_awcache(3 downto 0) => vip_M00_AXI_M_AXI_AWCACHE(3 downto 0),
      m_axi_awlen(7 downto 0) => vip_M00_AXI_M_AXI_AWLEN(7 downto 0),
      m_axi_awlock(0) => vip_M00_AXI_M_AXI_AWLOCK,
      m_axi_awprot(2 downto 0) => vip_M00_AXI_M_AXI_AWPROT(2 downto 0),
      m_axi_awqos(3 downto 0) => vip_M00_AXI_M_AXI_AWQOS(3 downto 0),
      m_axi_awready => vip_M00_AXI_M_AXI_AWREADY,
      m_axi_awvalid => vip_M00_AXI_M_AXI_AWVALID,
      m_axi_bready => vip_M00_AXI_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_M00_AXI_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_M00_AXI_M_AXI_BVALID,
      m_axi_rdata(511 downto 0) => vip_M00_AXI_M_AXI_RDATA(511 downto 0),
      m_axi_rlast => vip_M00_AXI_M_AXI_RLAST,
      m_axi_rready => vip_M00_AXI_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_M00_AXI_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_M00_AXI_M_AXI_RVALID,
      m_axi_wdata(511 downto 0) => vip_M00_AXI_M_AXI_WDATA(511 downto 0),
      m_axi_wlast => vip_M00_AXI_M_AXI_WLAST,
      m_axi_wready => vip_M00_AXI_M_AXI_WREADY,
      m_axi_wstrb(63 downto 0) => vip_M00_AXI_M_AXI_WSTRB(63 downto 0),
      m_axi_wvalid => vip_M00_AXI_M_AXI_WVALID,
      s_axi_araddr(38 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARADDR(38 downto 0),
      s_axi_arburst(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARBURST(1 downto 0),
      s_axi_arcache(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARCACHE(3 downto 0),
      s_axi_arlen(7 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARLEN(7 downto 0),
      s_axi_arlock(0) => interconnect_M00_AXI_MEM00_M00_AXI_ARLOCK,
      s_axi_arprot(2 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARPROT(2 downto 0),
      s_axi_arqos(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_ARQOS(3 downto 0),
      s_axi_arready => interconnect_M00_AXI_MEM00_M00_AXI_ARREADY,
      s_axi_arvalid => interconnect_M00_AXI_MEM00_M00_AXI_ARVALID,
      s_axi_awaddr(38 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWADDR(38 downto 0),
      s_axi_awburst(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWBURST(1 downto 0),
      s_axi_awcache(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWCACHE(3 downto 0),
      s_axi_awlen(7 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWLEN(7 downto 0),
      s_axi_awlock(0) => interconnect_M00_AXI_MEM00_M00_AXI_AWLOCK,
      s_axi_awprot(2 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWPROT(2 downto 0),
      s_axi_awqos(3 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_AWQOS(3 downto 0),
      s_axi_awready => interconnect_M00_AXI_MEM00_M00_AXI_AWREADY,
      s_axi_awvalid => interconnect_M00_AXI_MEM00_M00_AXI_AWVALID,
      s_axi_bready => interconnect_M00_AXI_MEM00_M00_AXI_BREADY,
      s_axi_bresp(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_BRESP(1 downto 0),
      s_axi_bvalid => interconnect_M00_AXI_MEM00_M00_AXI_BVALID,
      s_axi_rdata(511 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_RDATA(511 downto 0),
      s_axi_rlast => interconnect_M00_AXI_MEM00_M00_AXI_RLAST,
      s_axi_rready => interconnect_M00_AXI_MEM00_M00_AXI_RREADY,
      s_axi_rresp(1 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_RRESP(1 downto 0),
      s_axi_rvalid => interconnect_M00_AXI_MEM00_M00_AXI_RVALID,
      s_axi_wdata(511 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_WDATA(511 downto 0),
      s_axi_wlast => interconnect_M00_AXI_MEM00_M00_AXI_WLAST,
      s_axi_wready => interconnect_M00_AXI_MEM00_M00_AXI_WREADY,
      s_axi_wstrb(63 downto 0) => interconnect_M00_AXI_MEM00_M00_AXI_WSTRB(63 downto 0),
      s_axi_wvalid => interconnect_M00_AXI_MEM00_M00_AXI_WVALID
    );
vip_s01_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S01_AXI_0
     port map (
      aclk => aclk,
      aresetn => interconnect_aresetn(0),
      m_axi_araddr(38 downto 0) => vip_S01_AXI_M_AXI_ARADDR(38 downto 0),
      m_axi_arburst(1 downto 0) => vip_S01_AXI_M_AXI_ARBURST(1 downto 0),
      m_axi_arcache(3 downto 0) => vip_S01_AXI_M_AXI_ARCACHE(3 downto 0),
      m_axi_arid(3 downto 0) => vip_S01_AXI_M_AXI_ARID(3 downto 0),
      m_axi_arlen(7 downto 0) => vip_S01_AXI_M_AXI_ARLEN(7 downto 0),
      m_axi_arlock(0) => vip_S01_AXI_M_AXI_ARLOCK,
      m_axi_arprot(2 downto 0) => vip_S01_AXI_M_AXI_ARPROT(2 downto 0),
      m_axi_arqos(3 downto 0) => vip_S01_AXI_M_AXI_ARQOS(3 downto 0),
      m_axi_arready => vip_S01_AXI_M_AXI_ARREADY,
      m_axi_arregion(3 downto 0) => NLW_vip_s01_axi_m_axi_arregion_UNCONNECTED(3 downto 0),
      m_axi_arvalid => vip_S01_AXI_M_AXI_ARVALID,
      m_axi_awaddr(38 downto 0) => vip_S01_AXI_M_AXI_AWADDR(38 downto 0),
      m_axi_awburst(1 downto 0) => vip_S01_AXI_M_AXI_AWBURST(1 downto 0),
      m_axi_awcache(3 downto 0) => vip_S01_AXI_M_AXI_AWCACHE(3 downto 0),
      m_axi_awid(3 downto 0) => vip_S01_AXI_M_AXI_AWID(3 downto 0),
      m_axi_awlen(7 downto 0) => vip_S01_AXI_M_AXI_AWLEN(7 downto 0),
      m_axi_awlock(0) => vip_S01_AXI_M_AXI_AWLOCK,
      m_axi_awprot(2 downto 0) => vip_S01_AXI_M_AXI_AWPROT(2 downto 0),
      m_axi_awqos(3 downto 0) => vip_S01_AXI_M_AXI_AWQOS(3 downto 0),
      m_axi_awready => vip_S01_AXI_M_AXI_AWREADY,
      m_axi_awregion(3 downto 0) => NLW_vip_s01_axi_m_axi_awregion_UNCONNECTED(3 downto 0),
      m_axi_awvalid => vip_S01_AXI_M_AXI_AWVALID,
      m_axi_bid(3 downto 0) => vip_S01_AXI_M_AXI_BID(3 downto 0),
      m_axi_bready => vip_S01_AXI_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_S01_AXI_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_S01_AXI_M_AXI_BVALID,
      m_axi_rdata(511 downto 0) => vip_S01_AXI_M_AXI_RDATA(511 downto 0),
      m_axi_rid(3 downto 0) => vip_S01_AXI_M_AXI_RID(3 downto 0),
      m_axi_rlast => vip_S01_AXI_M_AXI_RLAST,
      m_axi_rready => vip_S01_AXI_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_S01_AXI_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_S01_AXI_M_AXI_RVALID,
      m_axi_wdata(511 downto 0) => vip_S01_AXI_M_AXI_WDATA(511 downto 0),
      m_axi_wlast => vip_S01_AXI_M_AXI_WLAST,
      m_axi_wready => vip_S01_AXI_M_AXI_WREADY,
      m_axi_wstrb(63 downto 0) => vip_S01_AXI_M_AXI_WSTRB(63 downto 0),
      m_axi_wvalid => vip_S01_AXI_M_AXI_WVALID,
      s_axi_araddr(38 downto 0) => S01_AXI_araddr(38 downto 0),
      s_axi_arburst(1 downto 0) => S01_AXI_arburst(1 downto 0),
      s_axi_arcache(3 downto 0) => S01_AXI_arcache(3 downto 0),
      s_axi_arid(3 downto 0) => S01_AXI_arid(3 downto 0),
      s_axi_arlen(7 downto 0) => S01_AXI_arlen(7 downto 0),
      s_axi_arlock(0) => S01_AXI_arlock(0),
      s_axi_arprot(2 downto 0) => S01_AXI_arprot(2 downto 0),
      s_axi_arqos(3 downto 0) => S01_AXI_arqos(3 downto 0),
      s_axi_arready => S01_AXI_arready,
      s_axi_arregion(3 downto 0) => S01_AXI_arregion(3 downto 0),
      s_axi_arvalid => S01_AXI_arvalid,
      s_axi_awaddr(38 downto 0) => S01_AXI_awaddr(38 downto 0),
      s_axi_awburst(1 downto 0) => S01_AXI_awburst(1 downto 0),
      s_axi_awcache(3 downto 0) => S01_AXI_awcache(3 downto 0),
      s_axi_awid(3 downto 0) => S01_AXI_awid(3 downto 0),
      s_axi_awlen(7 downto 0) => S01_AXI_awlen(7 downto 0),
      s_axi_awlock(0) => S01_AXI_awlock(0),
      s_axi_awprot(2 downto 0) => S01_AXI_awprot(2 downto 0),
      s_axi_awqos(3 downto 0) => S01_AXI_awqos(3 downto 0),
      s_axi_awready => S01_AXI_awready,
      s_axi_awregion(3 downto 0) => S01_AXI_awregion(3 downto 0),
      s_axi_awvalid => S01_AXI_awvalid,
      s_axi_bid(3 downto 0) => S01_AXI_bid(3 downto 0),
      s_axi_bready => S01_AXI_bready,
      s_axi_bresp(1 downto 0) => S01_AXI_bresp(1 downto 0),
      s_axi_bvalid => S01_AXI_bvalid,
      s_axi_rdata(511 downto 0) => S01_AXI_rdata(511 downto 0),
      s_axi_rid(3 downto 0) => S01_AXI_rid(3 downto 0),
      s_axi_rlast => S01_AXI_rlast,
      s_axi_rready => S01_AXI_rready,
      s_axi_rresp(1 downto 0) => S01_AXI_rresp(1 downto 0),
      s_axi_rvalid => S01_AXI_rvalid,
      s_axi_wdata(511 downto 0) => S01_AXI_wdata(511 downto 0),
      s_axi_wlast => S01_AXI_wlast,
      s_axi_wready => S01_AXI_wready,
      s_axi_wstrb(63 downto 0) => S01_AXI_wstrb(63 downto 0),
      s_axi_wvalid => S01_AXI_wvalid
    );
vip_s02_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S02_AXI_0
     port map (
      aclk => aclk,
      aresetn => interconnect_aresetn(0),
      m_axi_araddr(38 downto 0) => vip_S02_AXI_M_AXI_ARADDR(38 downto 0),
      m_axi_arburst(1 downto 0) => vip_S02_AXI_M_AXI_ARBURST(1 downto 0),
      m_axi_arcache(3 downto 0) => vip_S02_AXI_M_AXI_ARCACHE(3 downto 0),
      m_axi_arid(3 downto 0) => vip_S02_AXI_M_AXI_ARID(3 downto 0),
      m_axi_arlen(7 downto 0) => vip_S02_AXI_M_AXI_ARLEN(7 downto 0),
      m_axi_arlock(0) => vip_S02_AXI_M_AXI_ARLOCK,
      m_axi_arprot(2 downto 0) => vip_S02_AXI_M_AXI_ARPROT(2 downto 0),
      m_axi_arqos(3 downto 0) => vip_S02_AXI_M_AXI_ARQOS(3 downto 0),
      m_axi_arready => vip_S02_AXI_M_AXI_ARREADY,
      m_axi_arregion(3 downto 0) => NLW_vip_s02_axi_m_axi_arregion_UNCONNECTED(3 downto 0),
      m_axi_arvalid => vip_S02_AXI_M_AXI_ARVALID,
      m_axi_awaddr(38 downto 0) => vip_S02_AXI_M_AXI_AWADDR(38 downto 0),
      m_axi_awburst(1 downto 0) => vip_S02_AXI_M_AXI_AWBURST(1 downto 0),
      m_axi_awcache(3 downto 0) => vip_S02_AXI_M_AXI_AWCACHE(3 downto 0),
      m_axi_awid(3 downto 0) => vip_S02_AXI_M_AXI_AWID(3 downto 0),
      m_axi_awlen(7 downto 0) => vip_S02_AXI_M_AXI_AWLEN(7 downto 0),
      m_axi_awlock(0) => vip_S02_AXI_M_AXI_AWLOCK,
      m_axi_awprot(2 downto 0) => vip_S02_AXI_M_AXI_AWPROT(2 downto 0),
      m_axi_awqos(3 downto 0) => vip_S02_AXI_M_AXI_AWQOS(3 downto 0),
      m_axi_awready => vip_S02_AXI_M_AXI_AWREADY,
      m_axi_awregion(3 downto 0) => NLW_vip_s02_axi_m_axi_awregion_UNCONNECTED(3 downto 0),
      m_axi_awvalid => vip_S02_AXI_M_AXI_AWVALID,
      m_axi_bid(3 downto 0) => vip_S02_AXI_M_AXI_BID(3 downto 0),
      m_axi_bready => vip_S02_AXI_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_S02_AXI_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_S02_AXI_M_AXI_BVALID,
      m_axi_rdata(511 downto 0) => vip_S02_AXI_M_AXI_RDATA(511 downto 0),
      m_axi_rid(3 downto 0) => vip_S02_AXI_M_AXI_RID(3 downto 0),
      m_axi_rlast => vip_S02_AXI_M_AXI_RLAST,
      m_axi_rready => vip_S02_AXI_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_S02_AXI_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_S02_AXI_M_AXI_RVALID,
      m_axi_wdata(511 downto 0) => vip_S02_AXI_M_AXI_WDATA(511 downto 0),
      m_axi_wlast => vip_S02_AXI_M_AXI_WLAST,
      m_axi_wready => vip_S02_AXI_M_AXI_WREADY,
      m_axi_wstrb(63 downto 0) => vip_S02_AXI_M_AXI_WSTRB(63 downto 0),
      m_axi_wvalid => vip_S02_AXI_M_AXI_WVALID,
      s_axi_araddr(38 downto 0) => S02_AXI_araddr(38 downto 0),
      s_axi_arburst(1 downto 0) => S02_AXI_arburst(1 downto 0),
      s_axi_arcache(3 downto 0) => S02_AXI_arcache(3 downto 0),
      s_axi_arid(3 downto 0) => S02_AXI_arid(3 downto 0),
      s_axi_arlen(7 downto 0) => S02_AXI_arlen(7 downto 0),
      s_axi_arlock(0) => S02_AXI_arlock(0),
      s_axi_arprot(2 downto 0) => S02_AXI_arprot(2 downto 0),
      s_axi_arqos(3 downto 0) => S02_AXI_arqos(3 downto 0),
      s_axi_arready => S02_AXI_arready,
      s_axi_arregion(3 downto 0) => S02_AXI_arregion(3 downto 0),
      s_axi_arvalid => S02_AXI_arvalid,
      s_axi_awaddr(38 downto 0) => S02_AXI_awaddr(38 downto 0),
      s_axi_awburst(1 downto 0) => S02_AXI_awburst(1 downto 0),
      s_axi_awcache(3 downto 0) => S02_AXI_awcache(3 downto 0),
      s_axi_awid(3 downto 0) => S02_AXI_awid(3 downto 0),
      s_axi_awlen(7 downto 0) => S02_AXI_awlen(7 downto 0),
      s_axi_awlock(0) => S02_AXI_awlock(0),
      s_axi_awprot(2 downto 0) => S02_AXI_awprot(2 downto 0),
      s_axi_awqos(3 downto 0) => S02_AXI_awqos(3 downto 0),
      s_axi_awready => S02_AXI_awready,
      s_axi_awregion(3 downto 0) => S02_AXI_awregion(3 downto 0),
      s_axi_awvalid => S02_AXI_awvalid,
      s_axi_bid(3 downto 0) => S02_AXI_bid(3 downto 0),
      s_axi_bready => S02_AXI_bready,
      s_axi_bresp(1 downto 0) => S02_AXI_bresp(1 downto 0),
      s_axi_bvalid => S02_AXI_bvalid,
      s_axi_rdata(511 downto 0) => S02_AXI_rdata(511 downto 0),
      s_axi_rid(3 downto 0) => S02_AXI_rid(3 downto 0),
      s_axi_rlast => S02_AXI_rlast,
      s_axi_rready => S02_AXI_rready,
      s_axi_rresp(1 downto 0) => S02_AXI_rresp(1 downto 0),
      s_axi_rvalid => S02_AXI_rvalid,
      s_axi_wdata(511 downto 0) => S02_AXI_wdata(511 downto 0),
      s_axi_wlast => S02_AXI_wlast,
      s_axi_wready => S02_AXI_wready,
      s_axi_wstrb(63 downto 0) => S02_AXI_wstrb(63 downto 0),
      s_axi_wvalid => S02_AXI_wvalid
    );
vip_s04_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S04_AXI_0
     port map (
      aclk => aclk,
      aresetn => interconnect_aresetn(0),
      m_axi_araddr(38 downto 0) => vip_S04_AXI_M_AXI_ARADDR(38 downto 0),
      m_axi_arcache(3 downto 0) => vip_S04_AXI_M_AXI_ARCACHE(3 downto 0),
      m_axi_arlen(7 downto 0) => vip_S04_AXI_M_AXI_ARLEN(7 downto 0),
      m_axi_arlock(0) => vip_S04_AXI_M_AXI_ARLOCK,
      m_axi_arprot(2 downto 0) => vip_S04_AXI_M_AXI_ARPROT(2 downto 0),
      m_axi_arqos(3 downto 0) => vip_S04_AXI_M_AXI_ARQOS(3 downto 0),
      m_axi_arready => vip_S04_AXI_M_AXI_ARREADY,
      m_axi_arregion(3 downto 0) => NLW_vip_s04_axi_m_axi_arregion_UNCONNECTED(3 downto 0),
      m_axi_arvalid => vip_S04_AXI_M_AXI_ARVALID,
      m_axi_awaddr(38 downto 0) => vip_S04_AXI_M_AXI_AWADDR(38 downto 0),
      m_axi_awcache(3 downto 0) => vip_S04_AXI_M_AXI_AWCACHE(3 downto 0),
      m_axi_awlen(7 downto 0) => vip_S04_AXI_M_AXI_AWLEN(7 downto 0),
      m_axi_awlock(0) => vip_S04_AXI_M_AXI_AWLOCK,
      m_axi_awprot(2 downto 0) => vip_S04_AXI_M_AXI_AWPROT(2 downto 0),
      m_axi_awqos(3 downto 0) => vip_S04_AXI_M_AXI_AWQOS(3 downto 0),
      m_axi_awready => vip_S04_AXI_M_AXI_AWREADY,
      m_axi_awregion(3 downto 0) => NLW_vip_s04_axi_m_axi_awregion_UNCONNECTED(3 downto 0),
      m_axi_awvalid => vip_S04_AXI_M_AXI_AWVALID,
      m_axi_bready => vip_S04_AXI_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_S04_AXI_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_S04_AXI_M_AXI_BVALID,
      m_axi_rdata(511 downto 0) => vip_S04_AXI_M_AXI_RDATA(511 downto 0),
      m_axi_rlast => vip_S04_AXI_M_AXI_RLAST,
      m_axi_rready => vip_S04_AXI_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_S04_AXI_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_S04_AXI_M_AXI_RVALID,
      m_axi_wdata(511 downto 0) => vip_S04_AXI_M_AXI_WDATA(511 downto 0),
      m_axi_wlast => vip_S04_AXI_M_AXI_WLAST,
      m_axi_wready => vip_S04_AXI_M_AXI_WREADY,
      m_axi_wstrb(63 downto 0) => vip_S04_AXI_M_AXI_WSTRB(63 downto 0),
      m_axi_wvalid => vip_S04_AXI_M_AXI_WVALID,
      s_axi_araddr(38 downto 0) => S04_AXI_araddr(38 downto 0),
      s_axi_arcache(3 downto 0) => S04_AXI_arcache(3 downto 0),
      s_axi_arlen(7 downto 0) => S04_AXI_arlen(7 downto 0),
      s_axi_arlock(0) => S04_AXI_arlock(0),
      s_axi_arprot(2 downto 0) => S04_AXI_arprot(2 downto 0),
      s_axi_arqos(3 downto 0) => S04_AXI_arqos(3 downto 0),
      s_axi_arready => S04_AXI_arready,
      s_axi_arregion(3 downto 0) => S04_AXI_arregion(3 downto 0),
      s_axi_arvalid => S04_AXI_arvalid,
      s_axi_awaddr(38 downto 0) => S04_AXI_awaddr(38 downto 0),
      s_axi_awcache(3 downto 0) => S04_AXI_awcache(3 downto 0),
      s_axi_awlen(7 downto 0) => S04_AXI_awlen(7 downto 0),
      s_axi_awlock(0) => S04_AXI_awlock(0),
      s_axi_awprot(2 downto 0) => S04_AXI_awprot(2 downto 0),
      s_axi_awqos(3 downto 0) => S04_AXI_awqos(3 downto 0),
      s_axi_awready => S04_AXI_awready,
      s_axi_awregion(3 downto 0) => S04_AXI_awregion(3 downto 0),
      s_axi_awvalid => S04_AXI_awvalid,
      s_axi_bready => S04_AXI_bready,
      s_axi_bresp(1 downto 0) => S04_AXI_bresp(1 downto 0),
      s_axi_bvalid => S04_AXI_bvalid,
      s_axi_rdata(511 downto 0) => S04_AXI_rdata(511 downto 0),
      s_axi_rlast => S04_AXI_rlast,
      s_axi_rready => S04_AXI_rready,
      s_axi_rresp(1 downto 0) => S04_AXI_rresp(1 downto 0),
      s_axi_rvalid => S04_AXI_rvalid,
      s_axi_wdata(511 downto 0) => S04_AXI_wdata(511 downto 0),
      s_axi_wlast => S04_AXI_wlast,
      s_axi_wready => S04_AXI_wready,
      s_axi_wstrb(63 downto 0) => S04_AXI_wstrb(63 downto 0),
      s_axi_wvalid => S04_AXI_wvalid
    );
vip_s05_axi: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_S05_AXI_0
     port map (
      aclk => aclk,
      aresetn => interconnect_aresetn(0),
      m_axi_araddr(38 downto 0) => vip_S05_AXI_M_AXI_ARADDR(38 downto 0),
      m_axi_arcache(3 downto 0) => vip_S05_AXI_M_AXI_ARCACHE(3 downto 0),
      m_axi_arlen(7 downto 0) => vip_S05_AXI_M_AXI_ARLEN(7 downto 0),
      m_axi_arlock(0) => vip_S05_AXI_M_AXI_ARLOCK,
      m_axi_arprot(2 downto 0) => vip_S05_AXI_M_AXI_ARPROT(2 downto 0),
      m_axi_arqos(3 downto 0) => vip_S05_AXI_M_AXI_ARQOS(3 downto 0),
      m_axi_arready => vip_S05_AXI_M_AXI_ARREADY,
      m_axi_arregion(3 downto 0) => NLW_vip_s05_axi_m_axi_arregion_UNCONNECTED(3 downto 0),
      m_axi_arvalid => vip_S05_AXI_M_AXI_ARVALID,
      m_axi_awaddr(38 downto 0) => vip_S05_AXI_M_AXI_AWADDR(38 downto 0),
      m_axi_awcache(3 downto 0) => vip_S05_AXI_M_AXI_AWCACHE(3 downto 0),
      m_axi_awlen(7 downto 0) => vip_S05_AXI_M_AXI_AWLEN(7 downto 0),
      m_axi_awlock(0) => vip_S05_AXI_M_AXI_AWLOCK,
      m_axi_awprot(2 downto 0) => vip_S05_AXI_M_AXI_AWPROT(2 downto 0),
      m_axi_awqos(3 downto 0) => vip_S05_AXI_M_AXI_AWQOS(3 downto 0),
      m_axi_awready => vip_S05_AXI_M_AXI_AWREADY,
      m_axi_awregion(3 downto 0) => NLW_vip_s05_axi_m_axi_awregion_UNCONNECTED(3 downto 0),
      m_axi_awvalid => vip_S05_AXI_M_AXI_AWVALID,
      m_axi_bready => vip_S05_AXI_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_S05_AXI_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_S05_AXI_M_AXI_BVALID,
      m_axi_rdata(511 downto 0) => vip_S05_AXI_M_AXI_RDATA(511 downto 0),
      m_axi_rlast => vip_S05_AXI_M_AXI_RLAST,
      m_axi_rready => vip_S05_AXI_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_S05_AXI_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_S05_AXI_M_AXI_RVALID,
      m_axi_wdata(511 downto 0) => vip_S05_AXI_M_AXI_WDATA(511 downto 0),
      m_axi_wlast => vip_S05_AXI_M_AXI_WLAST,
      m_axi_wready => vip_S05_AXI_M_AXI_WREADY,
      m_axi_wstrb(63 downto 0) => vip_S05_AXI_M_AXI_WSTRB(63 downto 0),
      m_axi_wvalid => vip_S05_AXI_M_AXI_WVALID,
      s_axi_araddr(38 downto 0) => S05_AXI_araddr(38 downto 0),
      s_axi_arcache(3 downto 0) => S05_AXI_arcache(3 downto 0),
      s_axi_arlen(7 downto 0) => S05_AXI_arlen(7 downto 0),
      s_axi_arlock(0) => S05_AXI_arlock(0),
      s_axi_arprot(2 downto 0) => S05_AXI_arprot(2 downto 0),
      s_axi_arqos(3 downto 0) => S05_AXI_arqos(3 downto 0),
      s_axi_arready => S05_AXI_arready,
      s_axi_arregion(3 downto 0) => S05_AXI_arregion(3 downto 0),
      s_axi_arvalid => S05_AXI_arvalid,
      s_axi_awaddr(38 downto 0) => S05_AXI_awaddr(38 downto 0),
      s_axi_awcache(3 downto 0) => S05_AXI_awcache(3 downto 0),
      s_axi_awlen(7 downto 0) => S05_AXI_awlen(7 downto 0),
      s_axi_awlock(0) => S05_AXI_awlock(0),
      s_axi_awprot(2 downto 0) => S05_AXI_awprot(2 downto 0),
      s_axi_awqos(3 downto 0) => S05_AXI_awqos(3 downto 0),
      s_axi_awready => S05_AXI_awready,
      s_axi_awregion(3 downto 0) => S05_AXI_awregion(3 downto 0),
      s_axi_awvalid => S05_AXI_awvalid,
      s_axi_bready => S05_AXI_bready,
      s_axi_bresp(1 downto 0) => S05_AXI_bresp(1 downto 0),
      s_axi_bvalid => S05_AXI_bvalid,
      s_axi_rdata(511 downto 0) => S05_AXI_rdata(511 downto 0),
      s_axi_rlast => S05_AXI_rlast,
      s_axi_rready => S05_AXI_rready,
      s_axi_rresp(1 downto 0) => S05_AXI_rresp(1 downto 0),
      s_axi_rvalid => S05_AXI_rvalid,
      s_axi_wdata(511 downto 0) => S05_AXI_wdata(511 downto 0),
      s_axi_wlast => S05_AXI_wlast,
      s_axi_wready => S05_AXI_wready,
      s_axi_wstrb(63 downto 0) => S05_AXI_wstrb(63 downto 0),
      s_axi_wvalid => S05_AXI_wvalid
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_memory_imp_1K98CM8 is
  port (
    ddr4_mem_calib_vec : out STD_LOGIC_VECTOR ( 2 downto 0 );
    ddr4_mem_calib_complete : out STD_LOGIC;
    DDR4_MEM01_act_n : out STD_LOGIC;
    DDR4_MEM01_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM01_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM01_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM01_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_reset_n : out STD_LOGIC;
    DDR4_MEM01_par : out STD_LOGIC;
    ddr4_mem01_ui_clk : out STD_LOGIC;
    S_AXI_CTRL_awready : out STD_LOGIC;
    S_AXI_CTRL_wready : out STD_LOGIC;
    S_AXI_CTRL_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_CTRL_bvalid : out STD_LOGIC;
    S_AXI_CTRL_arready : out STD_LOGIC;
    S_AXI_CTRL_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    S_AXI_CTRL_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_CTRL_rvalid : out STD_LOGIC;
    S_AXI_awready : out STD_LOGIC;
    S_AXI_wready : out STD_LOGIC;
    S_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_bvalid : out STD_LOGIC;
    S_AXI_arready : out STD_LOGIC;
    S_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_rlast : out STD_LOGIC;
    S_AXI_rvalid : out STD_LOGIC;
    DDR4_MEM01_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM01_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM01_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    ddr4_mem01_sys_rst : in STD_LOGIC;
    DDR4_MEM01_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM01_DIFF_CLK_clk_n : in STD_LOGIC;
    aclk1 : in STD_LOGIC;
    S_AXI_CTRL_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    S_AXI_CTRL_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_CTRL_awvalid : in STD_LOGIC;
    S_AXI_CTRL_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    S_AXI_CTRL_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_CTRL_wvalid : in STD_LOGIC;
    S_AXI_CTRL_bready : in STD_LOGIC;
    S_AXI_CTRL_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    S_AXI_CTRL_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_CTRL_arvalid : in STD_LOGIC;
    S_AXI_CTRL_rready : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    S_AXI_awaddr : in STD_LOGIC_VECTOR ( 33 downto 0 );
    S_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_awvalid : in STD_LOGIC;
    S_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S_AXI_wlast : in STD_LOGIC;
    S_AXI_wvalid : in STD_LOGIC;
    S_AXI_bready : in STD_LOGIC;
    S_AXI_araddr : in STD_LOGIC_VECTOR ( 33 downto 0 );
    S_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_arvalid : in STD_LOGIC;
    S_AXI_rready : in STD_LOGIC
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_memory_imp_1K98CM8;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_memory_imp_1K98CM8 is
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_reduce_0 is
  port (
    Op1 : in STD_LOGIC_VECTOR ( 0 to 0 );
    Res : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_reduce_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_0 is
  port (
    sys_rst : in STD_LOGIC;
    c0_sys_clk_p : in STD_LOGIC;
    c0_sys_clk_n : in STD_LOGIC;
    c0_ddr4_act_n : out STD_LOGIC;
    c0_ddr4_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    c0_ddr4_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_reset_n : out STD_LOGIC;
    c0_ddr4_parity : out STD_LOGIC;
    c0_ddr4_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    c0_ddr4_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    c0_ddr4_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    c0_init_calib_complete : out STD_LOGIC;
    c0_ddr4_ui_clk : out STD_LOGIC;
    c0_ddr4_ui_clk_sync_rst : out STD_LOGIC;
    addn_ui_clkout1 : out STD_LOGIC;
    dbg_clk : out STD_LOGIC;
    c0_ddr4_s_axi_ctrl_awvalid : in STD_LOGIC;
    c0_ddr4_s_axi_ctrl_awready : out STD_LOGIC;
    c0_ddr4_s_axi_ctrl_awaddr : in STD_LOGIC_VECTOR ( 31 downto 0 );
    c0_ddr4_s_axi_ctrl_wvalid : in STD_LOGIC;
    c0_ddr4_s_axi_ctrl_wready : out STD_LOGIC;
    c0_ddr4_s_axi_ctrl_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    c0_ddr4_s_axi_ctrl_bvalid : out STD_LOGIC;
    c0_ddr4_s_axi_ctrl_bready : in STD_LOGIC;
    c0_ddr4_s_axi_ctrl_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_s_axi_ctrl_arvalid : in STD_LOGIC;
    c0_ddr4_s_axi_ctrl_arready : out STD_LOGIC;
    c0_ddr4_s_axi_ctrl_araddr : in STD_LOGIC_VECTOR ( 31 downto 0 );
    c0_ddr4_s_axi_ctrl_rvalid : out STD_LOGIC;
    c0_ddr4_s_axi_ctrl_rready : in STD_LOGIC;
    c0_ddr4_s_axi_ctrl_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    c0_ddr4_s_axi_ctrl_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_interrupt : out STD_LOGIC;
    c0_ddr4_aresetn : in STD_LOGIC;
    c0_ddr4_s_axi_awid : in STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_s_axi_awaddr : in STD_LOGIC_VECTOR ( 33 downto 0 );
    c0_ddr4_s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    c0_ddr4_s_axi_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    c0_ddr4_s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    c0_ddr4_s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    c0_ddr4_s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    c0_ddr4_s_axi_awvalid : in STD_LOGIC;
    c0_ddr4_s_axi_awready : out STD_LOGIC;
    c0_ddr4_s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    c0_ddr4_s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    c0_ddr4_s_axi_wlast : in STD_LOGIC;
    c0_ddr4_s_axi_wvalid : in STD_LOGIC;
    c0_ddr4_s_axi_wready : out STD_LOGIC;
    c0_ddr4_s_axi_bready : in STD_LOGIC;
    c0_ddr4_s_axi_bid : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_s_axi_bvalid : out STD_LOGIC;
    c0_ddr4_s_axi_arid : in STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_s_axi_araddr : in STD_LOGIC_VECTOR ( 33 downto 0 );
    c0_ddr4_s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    c0_ddr4_s_axi_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    c0_ddr4_s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    c0_ddr4_s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    c0_ddr4_s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    c0_ddr4_s_axi_arvalid : in STD_LOGIC;
    c0_ddr4_s_axi_arready : out STD_LOGIC;
    c0_ddr4_s_axi_rready : in STD_LOGIC;
    c0_ddr4_s_axi_rid : out STD_LOGIC_VECTOR ( 0 to 0 );
    c0_ddr4_s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    c0_ddr4_s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    c0_ddr4_s_axi_rlast : out STD_LOGIC;
    c0_ddr4_s_axi_rvalid : out STD_LOGIC;
    dbg_bus : out STD_LOGIC_VECTOR ( 511 downto 0 )
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_ctrl_cc_0 is
  port (
    s_axi_aclk : in STD_LOGIC;
    s_axi_aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_aclk : in STD_LOGIC;
    m_axi_aresetn : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_ctrl_cc_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_ddrmem_ctrl_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_ddrmem_ctrl_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ctrl_interconnect_0 is
  port (
    slowest_sync_clk : in STD_LOGIC;
    ext_reset_in : in STD_LOGIC;
    aux_reset_in : in STD_LOGIC;
    mb_debug_sys_rst : in STD_LOGIC;
    dcm_locked : in STD_LOGIC;
    mb_reset : out STD_LOGIC;
    bus_struct_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 )
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ctrl_interconnect_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ddr4_mem01_0 is
  port (
    slowest_sync_clk : in STD_LOGIC;
    ext_reset_in : in STD_LOGIC;
    aux_reset_in : in STD_LOGIC;
    mb_debug_sys_rst : in STD_LOGIC;
    dcm_locked : in STD_LOGIC;
    mb_reset : out STD_LOGIC;
    bus_struct_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 )
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ddr4_mem01_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_DDR4_MEM01_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 33 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_wlast : in STD_LOGIC;
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 33 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC;
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 33 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wlast : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 33 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rlast : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_DDR4_MEM01_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ctrl_DDR4_MEM01_0 is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awvalid : in STD_LOGIC;
    s_axi_awready : out STD_LOGIC;
    s_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_wvalid : in STD_LOGIC;
    s_axi_wready : out STD_LOGIC;
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC;
    s_axi_bready : in STD_LOGIC;
    s_axi_araddr : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arvalid : in STD_LOGIC;
    s_axi_arready : out STD_LOGIC;
    s_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rvalid : out STD_LOGIC;
    s_axi_rready : in STD_LOGIC;
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_awvalid : out STD_LOGIC;
    m_axi_awready : in STD_LOGIC;
    m_axi_wdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_wvalid : out STD_LOGIC;
    m_axi_wready : in STD_LOGIC;
    m_axi_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC;
    m_axi_bready : out STD_LOGIC;
    m_axi_araddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_arvalid : out STD_LOGIC;
    m_axi_arready : in STD_LOGIC;
    m_axi_rdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rvalid : in STD_LOGIC;
    m_axi_rready : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ctrl_DDR4_MEM01_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_clk_DDR4_MEM01_0 is
  port (
    clk_in : in STD_LOGIC;
    clk_out : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_clk_DDR4_MEM01_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_rst_DDR4_MEM01_0 is
  port (
    rst_in : in STD_LOGIC;
    rst_out : out STD_LOGIC
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_rst_DDR4_MEM01_0;
  signal calib_concat_dout : STD_LOGIC;
  signal ddr4_mem01_c0_ddr4_ui_clk : STD_LOGIC;
  signal ddr4_mem01_c0_ddr4_ui_clk_sync_rst : STD_LOGIC;
  signal ddr4_mem01_c0_init_calib_complete : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal ddr4_mem01_ctrl_cc_M_AXI_ARREADY : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_ARVALID : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal ddr4_mem01_ctrl_cc_M_AXI_AWREADY : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_AWVALID : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_BREADY : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal ddr4_mem01_ctrl_cc_M_AXI_BVALID : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_RDATA : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal ddr4_mem01_ctrl_cc_M_AXI_RREADY : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal ddr4_mem01_ctrl_cc_M_AXI_RVALID : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_WDATA : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal ddr4_mem01_ctrl_cc_M_AXI_WREADY : STD_LOGIC;
  signal ddr4_mem01_ctrl_cc_M_AXI_WVALID : STD_LOGIC;
  signal \^ddr4_mem01_ui_clk\ : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_ARREADY : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_ARVALID : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_AWREADY : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_AWVALID : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_BREADY : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_BVALID : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_RDATA : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_RREADY : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_RVALID : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_WDATA : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_WREADY : STD_LOGIC;
  signal interconnect_ddrmem_ctrl_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_ddrmem_ctrl_M_AXI_WVALID : STD_LOGIC;
  signal psr_ctrl_interconnect_interconnect_aresetn : STD_LOGIC;
  signal psr_ddr4_mem01_interconnect_aresetn : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 33 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_ARLOCK : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_ARREADY : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_ARVALID : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 33 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_AWLOCK : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_AWREADY : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_AWVALID : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_BREADY : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_BVALID : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_RLAST : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_RREADY : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_RVALID : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_WLAST : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_WREADY : STD_LOGIC;
  signal vip_DDR4_MEM01_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal vip_DDR4_MEM01_M_AXI_WVALID : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_ARADDR : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_ARREADY : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_ARVALID : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_AWADDR : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_AWREADY : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_AWVALID : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_BREADY : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_BVALID : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_RDATA : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_RREADY : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_RVALID : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_WDATA : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_WREADY : STD_LOGIC;
  signal vip_ctrl_DDR4_MEM01_M_AXI_WSTRB : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal vip_ctrl_DDR4_MEM01_M_AXI_WVALID : STD_LOGIC;
  signal vip_ui_rst_DDR4_MEM01_rst_out : STD_LOGIC;
  signal NLW_ddr4_mem01_addn_ui_clkout1_UNCONNECTED : STD_LOGIC;
  signal NLW_ddr4_mem01_c0_ddr4_interrupt_UNCONNECTED : STD_LOGIC;
  signal NLW_ddr4_mem01_dbg_clk_UNCONNECTED : STD_LOGIC;
  signal NLW_ddr4_mem01_c0_ddr4_s_axi_bid_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_ddr4_mem01_c0_ddr4_s_axi_rid_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_ddr4_mem01_dbg_bus_UNCONNECTED : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal NLW_ddr4_mem01_ctrl_cc_m_axi_arprot_UNCONNECTED : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal NLW_ddr4_mem01_ctrl_cc_m_axi_awprot_UNCONNECTED : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal NLW_ddr4_mem01_ctrl_cc_m_axi_wstrb_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_psr_ctrl_interconnect_mb_reset_UNCONNECTED : STD_LOGIC;
  signal NLW_psr_ctrl_interconnect_bus_struct_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_ctrl_interconnect_peripheral_aresetn_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_ctrl_interconnect_peripheral_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_ddr4_mem01_mb_reset_UNCONNECTED : STD_LOGIC;
  signal NLW_psr_ddr4_mem01_bus_struct_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_ddr4_mem01_peripheral_aresetn_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_ddr4_mem01_peripheral_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of calib_concat : label is "bd_d216_calib_concat_0,xlconcat_v2_1_3_xlconcat,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of calib_concat : label is "yes";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of calib_concat : label is "xlconcat_v2_1_3_xlconcat,Vivado 2020.1";
  attribute X_CORE_INFO of calib_reduce : label is "util_reduced_logic_v2_0_4_util_reduced_logic,Vivado 2020.1";
  attribute CHECK_LICENSE_TYPE of calib_vector_concat : label is "bd_d216_calib_vector_concat_0,xlconcat_v2_1_3_xlconcat,{}";
  attribute DowngradeIPIdentifiedWarnings of calib_vector_concat : label is "yes";
  attribute X_CORE_INFO of calib_vector_concat : label is "xlconcat_v2_1_3_xlconcat,Vivado 2020.1";
  attribute X_CORE_INFO of ddr4_mem01 : label is "ddr4_v2_2_9,Vivado 2020.1";
  attribute X_CORE_INFO of ddr4_mem01_ctrl_cc : label is "axi_clock_converter_v2_1_20_axi_clock_converter,Vivado 2020.1";
  attribute X_CORE_INFO of interconnect_ddrmem_ctrl : label is "axi_mmu_v2_1_19_top,Vivado 2020.1";
  attribute X_CORE_INFO of psr_ctrl_interconnect : label is "proc_sys_reset,Vivado 2020.1";
  attribute X_CORE_INFO of psr_ddr4_mem01 : label is "proc_sys_reset,Vivado 2020.1";
  attribute X_CORE_INFO of vip_DDR4_MEM01 : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_ctrl_DDR4_MEM01 : label is "axi_vip_v1_1_7_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_ui_clk_DDR4_MEM01 : label is "clk_vip_v1_0_2_top,Vivado 2020.1";
  attribute X_CORE_INFO of vip_ui_rst_DDR4_MEM01 : label is "rst_vip_v1_0_4_top,Vivado 2020.1";
begin
  ddr4_mem01_ui_clk <= \^ddr4_mem01_ui_clk\;
calib_concat: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_concat_0
     port map (
      In0(0) => ddr4_mem01_c0_init_calib_complete,
      dout(0) => calib_concat_dout
    );
calib_reduce: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_reduce_0
     port map (
      Op1(0) => calib_concat_dout,
      Res => ddr4_mem_calib_complete
    );
calib_vector_concat: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_calib_vector_concat_0
     port map (
      In0(0) => '0',
      In1(0) => ddr4_mem01_c0_init_calib_complete,
      In2(0) => '0',
      dout(2 downto 0) => ddr4_mem_calib_vec(2 downto 0)
    );
ddr4_mem01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_0
     port map (
      addn_ui_clkout1 => NLW_ddr4_mem01_addn_ui_clkout1_UNCONNECTED,
      c0_ddr4_act_n => DDR4_MEM01_act_n,
      c0_ddr4_adr(16 downto 0) => DDR4_MEM01_adr(16 downto 0),
      c0_ddr4_aresetn => psr_ddr4_mem01_interconnect_aresetn,
      c0_ddr4_ba(1 downto 0) => DDR4_MEM01_ba(1 downto 0),
      c0_ddr4_bg(1 downto 0) => DDR4_MEM01_bg(1 downto 0),
      c0_ddr4_ck_c(0) => DDR4_MEM01_ck_c(0),
      c0_ddr4_ck_t(0) => DDR4_MEM01_ck_t(0),
      c0_ddr4_cke(0) => DDR4_MEM01_cke(0),
      c0_ddr4_cs_n(0) => DDR4_MEM01_cs_n(0),
      c0_ddr4_dq(71 downto 0) => DDR4_MEM01_dq(71 downto 0),
      c0_ddr4_dqs_c(17 downto 0) => DDR4_MEM01_dqs_c(17 downto 0),
      c0_ddr4_dqs_t(17 downto 0) => DDR4_MEM01_dqs_t(17 downto 0),
      c0_ddr4_interrupt => NLW_ddr4_mem01_c0_ddr4_interrupt_UNCONNECTED,
      c0_ddr4_odt(0) => DDR4_MEM01_odt(0),
      c0_ddr4_parity => DDR4_MEM01_par,
      c0_ddr4_reset_n => DDR4_MEM01_reset_n,
      c0_ddr4_s_axi_araddr(33 downto 0) => vip_DDR4_MEM01_M_AXI_ARADDR(33 downto 0),
      c0_ddr4_s_axi_arburst(1 downto 0) => vip_DDR4_MEM01_M_AXI_ARBURST(1 downto 0),
      c0_ddr4_s_axi_arcache(3 downto 0) => vip_DDR4_MEM01_M_AXI_ARCACHE(3 downto 0),
      c0_ddr4_s_axi_arid(0) => '0',
      c0_ddr4_s_axi_arlen(7 downto 0) => vip_DDR4_MEM01_M_AXI_ARLEN(7 downto 0),
      c0_ddr4_s_axi_arlock(0) => vip_DDR4_MEM01_M_AXI_ARLOCK,
      c0_ddr4_s_axi_arprot(2 downto 0) => vip_DDR4_MEM01_M_AXI_ARPROT(2 downto 0),
      c0_ddr4_s_axi_arqos(3 downto 0) => vip_DDR4_MEM01_M_AXI_ARQOS(3 downto 0),
      c0_ddr4_s_axi_arready => vip_DDR4_MEM01_M_AXI_ARREADY,
      c0_ddr4_s_axi_arsize(2 downto 0) => B"110",
      c0_ddr4_s_axi_arvalid => vip_DDR4_MEM01_M_AXI_ARVALID,
      c0_ddr4_s_axi_awaddr(33 downto 0) => vip_DDR4_MEM01_M_AXI_AWADDR(33 downto 0),
      c0_ddr4_s_axi_awburst(1 downto 0) => vip_DDR4_MEM01_M_AXI_AWBURST(1 downto 0),
      c0_ddr4_s_axi_awcache(3 downto 0) => vip_DDR4_MEM01_M_AXI_AWCACHE(3 downto 0),
      c0_ddr4_s_axi_awid(0) => '0',
      c0_ddr4_s_axi_awlen(7 downto 0) => vip_DDR4_MEM01_M_AXI_AWLEN(7 downto 0),
      c0_ddr4_s_axi_awlock(0) => vip_DDR4_MEM01_M_AXI_AWLOCK,
      c0_ddr4_s_axi_awprot(2 downto 0) => vip_DDR4_MEM01_M_AXI_AWPROT(2 downto 0),
      c0_ddr4_s_axi_awqos(3 downto 0) => vip_DDR4_MEM01_M_AXI_AWQOS(3 downto 0),
      c0_ddr4_s_axi_awready => vip_DDR4_MEM01_M_AXI_AWREADY,
      c0_ddr4_s_axi_awsize(2 downto 0) => B"110",
      c0_ddr4_s_axi_awvalid => vip_DDR4_MEM01_M_AXI_AWVALID,
      c0_ddr4_s_axi_bid(0) => NLW_ddr4_mem01_c0_ddr4_s_axi_bid_UNCONNECTED(0),
      c0_ddr4_s_axi_bready => vip_DDR4_MEM01_M_AXI_BREADY,
      c0_ddr4_s_axi_bresp(1 downto 0) => vip_DDR4_MEM01_M_AXI_BRESP(1 downto 0),
      c0_ddr4_s_axi_bvalid => vip_DDR4_MEM01_M_AXI_BVALID,
      c0_ddr4_s_axi_ctrl_araddr(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_ARADDR(31 downto 0),
      c0_ddr4_s_axi_ctrl_arready => ddr4_mem01_ctrl_cc_M_AXI_ARREADY,
      c0_ddr4_s_axi_ctrl_arvalid => ddr4_mem01_ctrl_cc_M_AXI_ARVALID,
      c0_ddr4_s_axi_ctrl_awaddr(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_AWADDR(31 downto 0),
      c0_ddr4_s_axi_ctrl_awready => ddr4_mem01_ctrl_cc_M_AXI_AWREADY,
      c0_ddr4_s_axi_ctrl_awvalid => ddr4_mem01_ctrl_cc_M_AXI_AWVALID,
      c0_ddr4_s_axi_ctrl_bready => ddr4_mem01_ctrl_cc_M_AXI_BREADY,
      c0_ddr4_s_axi_ctrl_bresp(1 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_BRESP(1 downto 0),
      c0_ddr4_s_axi_ctrl_bvalid => ddr4_mem01_ctrl_cc_M_AXI_BVALID,
      c0_ddr4_s_axi_ctrl_rdata(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_RDATA(31 downto 0),
      c0_ddr4_s_axi_ctrl_rready => ddr4_mem01_ctrl_cc_M_AXI_RREADY,
      c0_ddr4_s_axi_ctrl_rresp(1 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_RRESP(1 downto 0),
      c0_ddr4_s_axi_ctrl_rvalid => ddr4_mem01_ctrl_cc_M_AXI_RVALID,
      c0_ddr4_s_axi_ctrl_wdata(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_WDATA(31 downto 0),
      c0_ddr4_s_axi_ctrl_wready => ddr4_mem01_ctrl_cc_M_AXI_WREADY,
      c0_ddr4_s_axi_ctrl_wvalid => ddr4_mem01_ctrl_cc_M_AXI_WVALID,
      c0_ddr4_s_axi_rdata(511 downto 0) => vip_DDR4_MEM01_M_AXI_RDATA(511 downto 0),
      c0_ddr4_s_axi_rid(0) => NLW_ddr4_mem01_c0_ddr4_s_axi_rid_UNCONNECTED(0),
      c0_ddr4_s_axi_rlast => vip_DDR4_MEM01_M_AXI_RLAST,
      c0_ddr4_s_axi_rready => vip_DDR4_MEM01_M_AXI_RREADY,
      c0_ddr4_s_axi_rresp(1 downto 0) => vip_DDR4_MEM01_M_AXI_RRESP(1 downto 0),
      c0_ddr4_s_axi_rvalid => vip_DDR4_MEM01_M_AXI_RVALID,
      c0_ddr4_s_axi_wdata(511 downto 0) => vip_DDR4_MEM01_M_AXI_WDATA(511 downto 0),
      c0_ddr4_s_axi_wlast => vip_DDR4_MEM01_M_AXI_WLAST,
      c0_ddr4_s_axi_wready => vip_DDR4_MEM01_M_AXI_WREADY,
      c0_ddr4_s_axi_wstrb(63 downto 0) => vip_DDR4_MEM01_M_AXI_WSTRB(63 downto 0),
      c0_ddr4_s_axi_wvalid => vip_DDR4_MEM01_M_AXI_WVALID,
      c0_ddr4_ui_clk => ddr4_mem01_c0_ddr4_ui_clk,
      c0_ddr4_ui_clk_sync_rst => ddr4_mem01_c0_ddr4_ui_clk_sync_rst,
      c0_init_calib_complete => ddr4_mem01_c0_init_calib_complete,
      c0_sys_clk_n => DDR4_MEM01_DIFF_CLK_clk_n,
      c0_sys_clk_p => DDR4_MEM01_DIFF_CLK_clk_p,
      dbg_bus(511 downto 0) => NLW_ddr4_mem01_dbg_bus_UNCONNECTED(511 downto 0),
      dbg_clk => NLW_ddr4_mem01_dbg_clk_UNCONNECTED,
      sys_rst => ddr4_mem01_sys_rst
    );
ddr4_mem01_ctrl_cc: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_ddr4_mem01_ctrl_cc_0
     port map (
      m_axi_aclk => \^ddr4_mem01_ui_clk\,
      m_axi_araddr(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_ARADDR(31 downto 0),
      m_axi_aresetn => psr_ddr4_mem01_interconnect_aresetn,
      m_axi_arprot(2 downto 0) => NLW_ddr4_mem01_ctrl_cc_m_axi_arprot_UNCONNECTED(2 downto 0),
      m_axi_arready => ddr4_mem01_ctrl_cc_M_AXI_ARREADY,
      m_axi_arvalid => ddr4_mem01_ctrl_cc_M_AXI_ARVALID,
      m_axi_awaddr(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_AWADDR(31 downto 0),
      m_axi_awprot(2 downto 0) => NLW_ddr4_mem01_ctrl_cc_m_axi_awprot_UNCONNECTED(2 downto 0),
      m_axi_awready => ddr4_mem01_ctrl_cc_M_AXI_AWREADY,
      m_axi_awvalid => ddr4_mem01_ctrl_cc_M_AXI_AWVALID,
      m_axi_bready => ddr4_mem01_ctrl_cc_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => ddr4_mem01_ctrl_cc_M_AXI_BVALID,
      m_axi_rdata(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_RDATA(31 downto 0),
      m_axi_rready => ddr4_mem01_ctrl_cc_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => ddr4_mem01_ctrl_cc_M_AXI_RVALID,
      m_axi_wdata(31 downto 0) => ddr4_mem01_ctrl_cc_M_AXI_WDATA(31 downto 0),
      m_axi_wready => ddr4_mem01_ctrl_cc_M_AXI_WREADY,
      m_axi_wstrb(3 downto 0) => NLW_ddr4_mem01_ctrl_cc_m_axi_wstrb_UNCONNECTED(3 downto 0),
      m_axi_wvalid => ddr4_mem01_ctrl_cc_M_AXI_WVALID,
      s_axi_aclk => aclk1,
      s_axi_araddr(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_ARADDR(31 downto 0),
      s_axi_aresetn => psr_ctrl_interconnect_interconnect_aresetn,
      s_axi_arprot(2 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_ARPROT(2 downto 0),
      s_axi_arready => vip_ctrl_DDR4_MEM01_M_AXI_ARREADY,
      s_axi_arvalid => vip_ctrl_DDR4_MEM01_M_AXI_ARVALID,
      s_axi_awaddr(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_AWADDR(31 downto 0),
      s_axi_awprot(2 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_AWPROT(2 downto 0),
      s_axi_awready => vip_ctrl_DDR4_MEM01_M_AXI_AWREADY,
      s_axi_awvalid => vip_ctrl_DDR4_MEM01_M_AXI_AWVALID,
      s_axi_bready => vip_ctrl_DDR4_MEM01_M_AXI_BREADY,
      s_axi_bresp(1 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_BRESP(1 downto 0),
      s_axi_bvalid => vip_ctrl_DDR4_MEM01_M_AXI_BVALID,
      s_axi_rdata(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_RDATA(31 downto 0),
      s_axi_rready => vip_ctrl_DDR4_MEM01_M_AXI_RREADY,
      s_axi_rresp(1 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_RRESP(1 downto 0),
      s_axi_rvalid => vip_ctrl_DDR4_MEM01_M_AXI_RVALID,
      s_axi_wdata(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_WDATA(31 downto 0),
      s_axi_wready => vip_ctrl_DDR4_MEM01_M_AXI_WREADY,
      s_axi_wstrb(3 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_WSTRB(3 downto 0),
      s_axi_wvalid => vip_ctrl_DDR4_MEM01_M_AXI_WVALID
    );
interconnect_ddrmem_ctrl: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_interconnect_ddrmem_ctrl_0
     port map (
      aclk => aclk1,
      aresetn => psr_ctrl_interconnect_interconnect_aresetn,
      m_axi_araddr(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_ARADDR(31 downto 0),
      m_axi_arprot(2 downto 0) => interconnect_ddrmem_ctrl_M_AXI_ARPROT(2 downto 0),
      m_axi_arready => interconnect_ddrmem_ctrl_M_AXI_ARREADY,
      m_axi_arvalid => interconnect_ddrmem_ctrl_M_AXI_ARVALID,
      m_axi_awaddr(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_AWADDR(31 downto 0),
      m_axi_awprot(2 downto 0) => interconnect_ddrmem_ctrl_M_AXI_AWPROT(2 downto 0),
      m_axi_awready => interconnect_ddrmem_ctrl_M_AXI_AWREADY,
      m_axi_awvalid => interconnect_ddrmem_ctrl_M_AXI_AWVALID,
      m_axi_bready => interconnect_ddrmem_ctrl_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => interconnect_ddrmem_ctrl_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => interconnect_ddrmem_ctrl_M_AXI_BVALID,
      m_axi_rdata(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_RDATA(31 downto 0),
      m_axi_rready => interconnect_ddrmem_ctrl_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => interconnect_ddrmem_ctrl_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => interconnect_ddrmem_ctrl_M_AXI_RVALID,
      m_axi_wdata(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_WDATA(31 downto 0),
      m_axi_wready => interconnect_ddrmem_ctrl_M_AXI_WREADY,
      m_axi_wstrb(3 downto 0) => interconnect_ddrmem_ctrl_M_AXI_WSTRB(3 downto 0),
      m_axi_wvalid => interconnect_ddrmem_ctrl_M_AXI_WVALID,
      s_axi_araddr(24 downto 0) => S_AXI_CTRL_araddr(24 downto 0),
      s_axi_arprot(2 downto 0) => S_AXI_CTRL_arprot(2 downto 0),
      s_axi_arready => S_AXI_CTRL_arready,
      s_axi_arvalid => S_AXI_CTRL_arvalid,
      s_axi_awaddr(24 downto 0) => S_AXI_CTRL_awaddr(24 downto 0),
      s_axi_awprot(2 downto 0) => S_AXI_CTRL_awprot(2 downto 0),
      s_axi_awready => S_AXI_CTRL_awready,
      s_axi_awvalid => S_AXI_CTRL_awvalid,
      s_axi_bready => S_AXI_CTRL_bready,
      s_axi_bresp(1 downto 0) => S_AXI_CTRL_bresp(1 downto 0),
      s_axi_bvalid => S_AXI_CTRL_bvalid,
      s_axi_rdata(31 downto 0) => S_AXI_CTRL_rdata(31 downto 0),
      s_axi_rready => S_AXI_CTRL_rready,
      s_axi_rresp(1 downto 0) => S_AXI_CTRL_rresp(1 downto 0),
      s_axi_rvalid => S_AXI_CTRL_rvalid,
      s_axi_wdata(31 downto 0) => S_AXI_CTRL_wdata(31 downto 0),
      s_axi_wready => S_AXI_CTRL_wready,
      s_axi_wstrb(3 downto 0) => S_AXI_CTRL_wstrb(3 downto 0),
      s_axi_wvalid => S_AXI_CTRL_wvalid
    );
psr_ctrl_interconnect: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ctrl_interconnect_0
     port map (
      aux_reset_in => '1',
      bus_struct_reset(0) => NLW_psr_ctrl_interconnect_bus_struct_reset_UNCONNECTED(0),
      dcm_locked => '1',
      ext_reset_in => aresetn,
      interconnect_aresetn(0) => psr_ctrl_interconnect_interconnect_aresetn,
      mb_debug_sys_rst => '0',
      mb_reset => NLW_psr_ctrl_interconnect_mb_reset_UNCONNECTED,
      peripheral_aresetn(0) => NLW_psr_ctrl_interconnect_peripheral_aresetn_UNCONNECTED(0),
      peripheral_reset(0) => NLW_psr_ctrl_interconnect_peripheral_reset_UNCONNECTED(0),
      slowest_sync_clk => aclk1
    );
psr_ddr4_mem01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_ddr4_mem01_0
     port map (
      aux_reset_in => '1',
      bus_struct_reset(0) => NLW_psr_ddr4_mem01_bus_struct_reset_UNCONNECTED(0),
      dcm_locked => '1',
      ext_reset_in => vip_ui_rst_DDR4_MEM01_rst_out,
      interconnect_aresetn(0) => psr_ddr4_mem01_interconnect_aresetn,
      mb_debug_sys_rst => '0',
      mb_reset => NLW_psr_ddr4_mem01_mb_reset_UNCONNECTED,
      peripheral_aresetn(0) => NLW_psr_ddr4_mem01_peripheral_aresetn_UNCONNECTED(0),
      peripheral_reset(0) => NLW_psr_ddr4_mem01_peripheral_reset_UNCONNECTED(0),
      slowest_sync_clk => \^ddr4_mem01_ui_clk\
    );
vip_DDR4_MEM01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_DDR4_MEM01_0
     port map (
      aclk => \^ddr4_mem01_ui_clk\,
      aresetn => psr_ddr4_mem01_interconnect_aresetn,
      m_axi_araddr(33 downto 0) => vip_DDR4_MEM01_M_AXI_ARADDR(33 downto 0),
      m_axi_arburst(1 downto 0) => vip_DDR4_MEM01_M_AXI_ARBURST(1 downto 0),
      m_axi_arcache(3 downto 0) => vip_DDR4_MEM01_M_AXI_ARCACHE(3 downto 0),
      m_axi_arlen(7 downto 0) => vip_DDR4_MEM01_M_AXI_ARLEN(7 downto 0),
      m_axi_arlock(0) => vip_DDR4_MEM01_M_AXI_ARLOCK,
      m_axi_arprot(2 downto 0) => vip_DDR4_MEM01_M_AXI_ARPROT(2 downto 0),
      m_axi_arqos(3 downto 0) => vip_DDR4_MEM01_M_AXI_ARQOS(3 downto 0),
      m_axi_arready => vip_DDR4_MEM01_M_AXI_ARREADY,
      m_axi_arvalid => vip_DDR4_MEM01_M_AXI_ARVALID,
      m_axi_awaddr(33 downto 0) => vip_DDR4_MEM01_M_AXI_AWADDR(33 downto 0),
      m_axi_awburst(1 downto 0) => vip_DDR4_MEM01_M_AXI_AWBURST(1 downto 0),
      m_axi_awcache(3 downto 0) => vip_DDR4_MEM01_M_AXI_AWCACHE(3 downto 0),
      m_axi_awlen(7 downto 0) => vip_DDR4_MEM01_M_AXI_AWLEN(7 downto 0),
      m_axi_awlock(0) => vip_DDR4_MEM01_M_AXI_AWLOCK,
      m_axi_awprot(2 downto 0) => vip_DDR4_MEM01_M_AXI_AWPROT(2 downto 0),
      m_axi_awqos(3 downto 0) => vip_DDR4_MEM01_M_AXI_AWQOS(3 downto 0),
      m_axi_awready => vip_DDR4_MEM01_M_AXI_AWREADY,
      m_axi_awvalid => vip_DDR4_MEM01_M_AXI_AWVALID,
      m_axi_bready => vip_DDR4_MEM01_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_DDR4_MEM01_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_DDR4_MEM01_M_AXI_BVALID,
      m_axi_rdata(511 downto 0) => vip_DDR4_MEM01_M_AXI_RDATA(511 downto 0),
      m_axi_rlast => vip_DDR4_MEM01_M_AXI_RLAST,
      m_axi_rready => vip_DDR4_MEM01_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_DDR4_MEM01_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_DDR4_MEM01_M_AXI_RVALID,
      m_axi_wdata(511 downto 0) => vip_DDR4_MEM01_M_AXI_WDATA(511 downto 0),
      m_axi_wlast => vip_DDR4_MEM01_M_AXI_WLAST,
      m_axi_wready => vip_DDR4_MEM01_M_AXI_WREADY,
      m_axi_wstrb(63 downto 0) => vip_DDR4_MEM01_M_AXI_WSTRB(63 downto 0),
      m_axi_wvalid => vip_DDR4_MEM01_M_AXI_WVALID,
      s_axi_araddr(33 downto 0) => S_AXI_araddr(33 downto 0),
      s_axi_arburst(1 downto 0) => S_AXI_arburst(1 downto 0),
      s_axi_arcache(3 downto 0) => S_AXI_arcache(3 downto 0),
      s_axi_arlen(7 downto 0) => S_AXI_arlen(7 downto 0),
      s_axi_arlock(0) => S_AXI_arlock(0),
      s_axi_arprot(2 downto 0) => S_AXI_arprot(2 downto 0),
      s_axi_arqos(3 downto 0) => S_AXI_arqos(3 downto 0),
      s_axi_arready => S_AXI_arready,
      s_axi_arvalid => S_AXI_arvalid,
      s_axi_awaddr(33 downto 0) => S_AXI_awaddr(33 downto 0),
      s_axi_awburst(1 downto 0) => S_AXI_awburst(1 downto 0),
      s_axi_awcache(3 downto 0) => S_AXI_awcache(3 downto 0),
      s_axi_awlen(7 downto 0) => S_AXI_awlen(7 downto 0),
      s_axi_awlock(0) => S_AXI_awlock(0),
      s_axi_awprot(2 downto 0) => S_AXI_awprot(2 downto 0),
      s_axi_awqos(3 downto 0) => S_AXI_awqos(3 downto 0),
      s_axi_awready => S_AXI_awready,
      s_axi_awvalid => S_AXI_awvalid,
      s_axi_bready => S_AXI_bready,
      s_axi_bresp(1 downto 0) => S_AXI_bresp(1 downto 0),
      s_axi_bvalid => S_AXI_bvalid,
      s_axi_rdata(511 downto 0) => S_AXI_rdata(511 downto 0),
      s_axi_rlast => S_AXI_rlast,
      s_axi_rready => S_AXI_rready,
      s_axi_rresp(1 downto 0) => S_AXI_rresp(1 downto 0),
      s_axi_rvalid => S_AXI_rvalid,
      s_axi_wdata(511 downto 0) => S_AXI_wdata(511 downto 0),
      s_axi_wlast => S_AXI_wlast,
      s_axi_wready => S_AXI_wready,
      s_axi_wstrb(63 downto 0) => S_AXI_wstrb(63 downto 0),
      s_axi_wvalid => S_AXI_wvalid
    );
vip_ctrl_DDR4_MEM01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ctrl_DDR4_MEM01_0
     port map (
      aclk => aclk1,
      aresetn => psr_ctrl_interconnect_interconnect_aresetn,
      m_axi_araddr(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_ARADDR(31 downto 0),
      m_axi_arprot(2 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_ARPROT(2 downto 0),
      m_axi_arready => vip_ctrl_DDR4_MEM01_M_AXI_ARREADY,
      m_axi_arvalid => vip_ctrl_DDR4_MEM01_M_AXI_ARVALID,
      m_axi_awaddr(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_AWADDR(31 downto 0),
      m_axi_awprot(2 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_AWPROT(2 downto 0),
      m_axi_awready => vip_ctrl_DDR4_MEM01_M_AXI_AWREADY,
      m_axi_awvalid => vip_ctrl_DDR4_MEM01_M_AXI_AWVALID,
      m_axi_bready => vip_ctrl_DDR4_MEM01_M_AXI_BREADY,
      m_axi_bresp(1 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_BRESP(1 downto 0),
      m_axi_bvalid => vip_ctrl_DDR4_MEM01_M_AXI_BVALID,
      m_axi_rdata(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_RDATA(31 downto 0),
      m_axi_rready => vip_ctrl_DDR4_MEM01_M_AXI_RREADY,
      m_axi_rresp(1 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_RRESP(1 downto 0),
      m_axi_rvalid => vip_ctrl_DDR4_MEM01_M_AXI_RVALID,
      m_axi_wdata(31 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_WDATA(31 downto 0),
      m_axi_wready => vip_ctrl_DDR4_MEM01_M_AXI_WREADY,
      m_axi_wstrb(3 downto 0) => vip_ctrl_DDR4_MEM01_M_AXI_WSTRB(3 downto 0),
      m_axi_wvalid => vip_ctrl_DDR4_MEM01_M_AXI_WVALID,
      s_axi_araddr(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_ARADDR(31 downto 0),
      s_axi_arprot(2 downto 0) => interconnect_ddrmem_ctrl_M_AXI_ARPROT(2 downto 0),
      s_axi_arready => interconnect_ddrmem_ctrl_M_AXI_ARREADY,
      s_axi_arvalid => interconnect_ddrmem_ctrl_M_AXI_ARVALID,
      s_axi_awaddr(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_AWADDR(31 downto 0),
      s_axi_awprot(2 downto 0) => interconnect_ddrmem_ctrl_M_AXI_AWPROT(2 downto 0),
      s_axi_awready => interconnect_ddrmem_ctrl_M_AXI_AWREADY,
      s_axi_awvalid => interconnect_ddrmem_ctrl_M_AXI_AWVALID,
      s_axi_bready => interconnect_ddrmem_ctrl_M_AXI_BREADY,
      s_axi_bresp(1 downto 0) => interconnect_ddrmem_ctrl_M_AXI_BRESP(1 downto 0),
      s_axi_bvalid => interconnect_ddrmem_ctrl_M_AXI_BVALID,
      s_axi_rdata(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_RDATA(31 downto 0),
      s_axi_rready => interconnect_ddrmem_ctrl_M_AXI_RREADY,
      s_axi_rresp(1 downto 0) => interconnect_ddrmem_ctrl_M_AXI_RRESP(1 downto 0),
      s_axi_rvalid => interconnect_ddrmem_ctrl_M_AXI_RVALID,
      s_axi_wdata(31 downto 0) => interconnect_ddrmem_ctrl_M_AXI_WDATA(31 downto 0),
      s_axi_wready => interconnect_ddrmem_ctrl_M_AXI_WREADY,
      s_axi_wstrb(3 downto 0) => interconnect_ddrmem_ctrl_M_AXI_WSTRB(3 downto 0),
      s_axi_wvalid => interconnect_ddrmem_ctrl_M_AXI_WVALID
    );
vip_ui_clk_DDR4_MEM01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_clk_DDR4_MEM01_0
     port map (
      clk_in => ddr4_mem01_c0_ddr4_ui_clk,
      clk_out => \^ddr4_mem01_ui_clk\
    );
vip_ui_rst_DDR4_MEM01: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_vip_ui_rst_DDR4_MEM01_0
     port map (
      rst_in => ddr4_mem01_c0_ddr4_ui_clk_sync_rst,
      rst_out => vip_ui_rst_DDR4_MEM01_rst_out
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_reset_imp_1YKOSPE is
  port (
    interconnect_aresetn1 : out STD_LOGIC_VECTOR ( 0 to 0 );
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    aclk2 : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    aclk : in STD_LOGIC
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_reset_imp_1YKOSPE;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_reset_imp_1YKOSPE is
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk2_SLR1_0 is
  port (
    slowest_sync_clk : in STD_LOGIC;
    ext_reset_in : in STD_LOGIC;
    aux_reset_in : in STD_LOGIC;
    mb_debug_sys_rst : in STD_LOGIC;
    dcm_locked : in STD_LOGIC;
    mb_reset : out STD_LOGIC;
    bus_struct_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 )
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk2_SLR1_0;
  component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk_SLR1_0 is
  port (
    slowest_sync_clk : in STD_LOGIC;
    ext_reset_in : in STD_LOGIC;
    aux_reset_in : in STD_LOGIC;
    mb_debug_sys_rst : in STD_LOGIC;
    dcm_locked : in STD_LOGIC;
    mb_reset : out STD_LOGIC;
    bus_struct_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_reset : out STD_LOGIC_VECTOR ( 0 to 0 );
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripheral_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 )
  );
  end component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk_SLR1_0;
  signal NLW_psr_aclk2_SLR1_mb_reset_UNCONNECTED : STD_LOGIC;
  signal NLW_psr_aclk2_SLR1_bus_struct_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_aclk2_SLR1_peripheral_aresetn_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_aclk2_SLR1_peripheral_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_aclk_SLR1_mb_reset_UNCONNECTED : STD_LOGIC;
  signal NLW_psr_aclk_SLR1_bus_struct_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_aclk_SLR1_peripheral_aresetn_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_psr_aclk_SLR1_peripheral_reset_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of psr_aclk2_SLR1 : label is "proc_sys_reset,Vivado 2020.1";
  attribute X_CORE_INFO of psr_aclk_SLR1 : label is "proc_sys_reset,Vivado 2020.1";
begin
psr_aclk2_SLR1: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk2_SLR1_0
     port map (
      aux_reset_in => '1',
      bus_struct_reset(0) => NLW_psr_aclk2_SLR1_bus_struct_reset_UNCONNECTED(0),
      dcm_locked => '1',
      ext_reset_in => aresetn,
      interconnect_aresetn(0) => interconnect_aresetn1(0),
      mb_debug_sys_rst => '0',
      mb_reset => NLW_psr_aclk2_SLR1_mb_reset_UNCONNECTED,
      peripheral_aresetn(0) => NLW_psr_aclk2_SLR1_peripheral_aresetn_UNCONNECTED(0),
      peripheral_reset(0) => NLW_psr_aclk2_SLR1_peripheral_reset_UNCONNECTED(0),
      slowest_sync_clk => aclk2
    );
psr_aclk_SLR1: component decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216_psr_aclk_SLR1_0
     port map (
      aux_reset_in => '1',
      bus_struct_reset(0) => NLW_psr_aclk_SLR1_bus_struct_reset_UNCONNECTED(0),
      dcm_locked => '1',
      ext_reset_in => aresetn,
      interconnect_aresetn(0) => interconnect_aresetn(0),
      mb_debug_sys_rst => '0',
      mb_reset => NLW_psr_aclk_SLR1_mb_reset_UNCONNECTED,
      peripheral_aresetn(0) => NLW_psr_aclk_SLR1_peripheral_aresetn_UNCONNECTED(0),
      peripheral_reset(0) => NLW_psr_aclk_SLR1_peripheral_reset_UNCONNECTED(0),
      slowest_sync_clk => aclk
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216 is
  port (
    DDR4_MEM00_DIFF_CLK_clk_n : in STD_LOGIC;
    DDR4_MEM00_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM00_act_n : out STD_LOGIC;
    DDR4_MEM00_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM00_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM00_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM00_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM00_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM00_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM00_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_par : out STD_LOGIC;
    DDR4_MEM00_reset_n : out STD_LOGIC;
    DDR4_MEM01_DIFF_CLK_clk_n : in STD_LOGIC;
    DDR4_MEM01_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM01_act_n : out STD_LOGIC;
    DDR4_MEM01_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM01_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM01_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM01_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM01_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM01_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM01_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_par : out STD_LOGIC;
    DDR4_MEM01_reset_n : out STD_LOGIC;
    DDR4_MEM02_DIFF_CLK_clk_n : in STD_LOGIC;
    DDR4_MEM02_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM02_act_n : out STD_LOGIC;
    DDR4_MEM02_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM02_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM02_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM02_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM02_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM02_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM02_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_par : out STD_LOGIC;
    DDR4_MEM02_reset_n : out STD_LOGIC;
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC;
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC;
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rvalid : in STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M00_AXI_wvalid : out STD_LOGIC;
    S00_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arready : out STD_LOGIC;
    S00_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arvalid : in STD_LOGIC;
    S00_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awready : out STD_LOGIC;
    S00_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awvalid : in STD_LOGIC;
    S00_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_bready : in STD_LOGIC;
    S00_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_bvalid : out STD_LOGIC;
    S00_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_rlast : out STD_LOGIC;
    S00_AXI_rready : in STD_LOGIC;
    S00_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_rvalid : out STD_LOGIC;
    S00_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_wlast : in STD_LOGIC;
    S00_AXI_wready : out STD_LOGIC;
    S00_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S00_AXI_wvalid : in STD_LOGIC;
    S01_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arready : out STD_LOGIC;
    S01_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arvalid : in STD_LOGIC;
    S01_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awready : out STD_LOGIC;
    S01_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awvalid : in STD_LOGIC;
    S01_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_bready : in STD_LOGIC;
    S01_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_bvalid : out STD_LOGIC;
    S01_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_rlast : out STD_LOGIC;
    S01_AXI_rready : in STD_LOGIC;
    S01_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_rvalid : out STD_LOGIC;
    S01_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_wlast : in STD_LOGIC;
    S01_AXI_wready : out STD_LOGIC;
    S01_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S01_AXI_wvalid : in STD_LOGIC;
    S02_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arready : out STD_LOGIC;
    S02_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arvalid : in STD_LOGIC;
    S02_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awready : out STD_LOGIC;
    S02_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awvalid : in STD_LOGIC;
    S02_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_bready : in STD_LOGIC;
    S02_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_bvalid : out STD_LOGIC;
    S02_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_rlast : out STD_LOGIC;
    S02_AXI_rready : in STD_LOGIC;
    S02_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_rvalid : out STD_LOGIC;
    S02_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_wlast : in STD_LOGIC;
    S02_AXI_wready : out STD_LOGIC;
    S02_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S02_AXI_wvalid : in STD_LOGIC;
    S03_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S03_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S03_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S03_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arready : out STD_LOGIC;
    S03_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_arvalid : in STD_LOGIC;
    S03_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S03_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S03_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S03_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awready : out STD_LOGIC;
    S03_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_awvalid : in STD_LOGIC;
    S03_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_bready : in STD_LOGIC;
    S03_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_bvalid : out STD_LOGIC;
    S03_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S03_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_rlast : out STD_LOGIC;
    S03_AXI_rready : in STD_LOGIC;
    S03_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_rvalid : out STD_LOGIC;
    S03_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S03_AXI_wlast : in STD_LOGIC;
    S03_AXI_wready : out STD_LOGIC;
    S03_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S03_AXI_wvalid : in STD_LOGIC;
    S04_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S04_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S04_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S04_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S04_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arready : out STD_LOGIC;
    S04_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arvalid : in STD_LOGIC;
    S04_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S04_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S04_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S04_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S04_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awready : out STD_LOGIC;
    S04_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awvalid : in STD_LOGIC;
    S04_AXI_bready : in STD_LOGIC;
    S04_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S04_AXI_bvalid : out STD_LOGIC;
    S04_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S04_AXI_rlast : out STD_LOGIC;
    S04_AXI_rready : in STD_LOGIC;
    S04_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S04_AXI_rvalid : out STD_LOGIC;
    S04_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S04_AXI_wlast : in STD_LOGIC;
    S04_AXI_wready : out STD_LOGIC;
    S04_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S04_AXI_wvalid : in STD_LOGIC;
    S05_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S05_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S05_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S05_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S05_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arready : out STD_LOGIC;
    S05_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arvalid : in STD_LOGIC;
    S05_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S05_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S05_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S05_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S05_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awready : out STD_LOGIC;
    S05_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awvalid : in STD_LOGIC;
    S05_AXI_bready : in STD_LOGIC;
    S05_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S05_AXI_bvalid : out STD_LOGIC;
    S05_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S05_AXI_rlast : out STD_LOGIC;
    S05_AXI_rready : in STD_LOGIC;
    S05_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S05_AXI_rvalid : out STD_LOGIC;
    S05_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S05_AXI_wlast : in STD_LOGIC;
    S05_AXI_wready : out STD_LOGIC;
    S05_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S05_AXI_wvalid : in STD_LOGIC;
    S_AXI_CTRL_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    S_AXI_CTRL_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_CTRL_arready : out STD_LOGIC;
    S_AXI_CTRL_arvalid : in STD_LOGIC;
    S_AXI_CTRL_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    S_AXI_CTRL_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_CTRL_awready : out STD_LOGIC;
    S_AXI_CTRL_awvalid : in STD_LOGIC;
    S_AXI_CTRL_bready : in STD_LOGIC;
    S_AXI_CTRL_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_CTRL_bvalid : out STD_LOGIC;
    S_AXI_CTRL_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    S_AXI_CTRL_rready : in STD_LOGIC;
    S_AXI_CTRL_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_CTRL_rvalid : out STD_LOGIC;
    S_AXI_CTRL_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    S_AXI_CTRL_wready : out STD_LOGIC;
    S_AXI_CTRL_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_CTRL_wvalid : in STD_LOGIC;
    aclk : in STD_LOGIC;
    aclk1 : in STD_LOGIC;
    aclk2 : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    ddr4_mem00_sys_rst : in STD_LOGIC;
    ddr4_mem00_ui_clk : out STD_LOGIC;
    ddr4_mem01_sys_rst : in STD_LOGIC;
    ddr4_mem01_ui_clk : out STD_LOGIC;
    ddr4_mem02_sys_rst : in STD_LOGIC;
    ddr4_mem02_ui_clk : out STD_LOGIC;
    ddr4_mem_calib_complete : out STD_LOGIC;
    ddr4_mem_calib_vec : out STD_LOGIC_VECTOR ( 2 downto 0 )
  );
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216 : entity is "pfm_dynamic_memory_subsystem_0.hwdef";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216 is
  signal \<const0>\ : STD_LOGIC;
  signal \^ddr4_mem01_ui_clk\ : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_ARADDR : STD_LOGIC_VECTOR ( 33 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_ARBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_ARCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_ARLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_ARLOCK : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_ARPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_ARQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_ARREADY : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_ARVALID : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_AWADDR : STD_LOGIC_VECTOR ( 33 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_AWBURST : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_AWCACHE : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_AWLEN : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_AWLOCK : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_AWPROT : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_AWQOS : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_AWREADY : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_AWVALID : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_BREADY : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_BRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_BVALID : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_RDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_RLAST : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_RREADY : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_RRESP : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_RVALID : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_WDATA : STD_LOGIC_VECTOR ( 511 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_WLAST : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_WREADY : STD_LOGIC;
  signal interconnect_DDR4_MEM01_M00_AXI_WSTRB : STD_LOGIC_VECTOR ( 63 downto 0 );
  signal interconnect_DDR4_MEM01_M00_AXI_WVALID : STD_LOGIC;
  signal psr_aclk2_SLR1_interconnect_aresetn : STD_LOGIC;
  signal psr_aclk_SLR1_interconnect_aresetn : STD_LOGIC;
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of DDR4_MEM00_DIFF_CLK_clk_n : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_N";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of DDR4_MEM00_DIFF_CLK_clk_n : signal is "XIL_INTERFACENAME DDR4_MEM00_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000";
  attribute X_INTERFACE_INFO of DDR4_MEM00_DIFF_CLK_clk_p : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_P";
  attribute X_INTERFACE_INFO of DDR4_MEM00_act_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ACT_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM00_act_n : signal is "XIL_INTERFACENAME DDR4_MEM00, AXI_ARBITRATION_SCHEME TDM, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 11, CAS_WRITE_LATENCY 11, CS_ENABLED true, DATA_MASK_ENABLED true, DATA_WIDTH 8, MEMORY_TYPE COMPONENTS, MEM_ADDR_MAP ROW_COLUMN_BANK, SLOT Single, TIMEPERIOD_PS 1250";
  attribute X_INTERFACE_INFO of DDR4_MEM00_par : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 PAR";
  attribute X_INTERFACE_INFO of DDR4_MEM00_reset_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 RESET_N";
  attribute X_INTERFACE_INFO of DDR4_MEM01_DIFF_CLK_clk_n : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM01_DIFF_CLK_clk_n : signal is "XIL_INTERFACENAME DDR4_MEM01_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000";
  attribute X_INTERFACE_INFO of DDR4_MEM01_DIFF_CLK_clk_p : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_P";
  attribute X_INTERFACE_INFO of DDR4_MEM01_act_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ACT_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM01_act_n : signal is "XIL_INTERFACENAME DDR4_MEM01, AXI_ARBITRATION_SCHEME RD_PRI_REG, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 17, CAS_WRITE_LATENCY 12, CS_ENABLED true, CUSTOM_PARTS no_file_loaded, DATA_MASK_ENABLED NONE, DATA_WIDTH 72, MEMORY_PART MTA18ASF2G72PZ-2G3, MEMORY_TYPE RDIMMs, MEM_ADDR_MAP ROW_COLUMN_BANK_INTLV, SLOT Single, TIMEPERIOD_PS 833";
  attribute X_INTERFACE_INFO of DDR4_MEM01_par : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 PAR";
  attribute X_INTERFACE_INFO of DDR4_MEM01_reset_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 RESET_N";
  attribute X_INTERFACE_INFO of DDR4_MEM02_DIFF_CLK_clk_n : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM02_DIFF_CLK_clk_n : signal is "XIL_INTERFACENAME DDR4_MEM02_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000";
  attribute X_INTERFACE_INFO of DDR4_MEM02_DIFF_CLK_clk_p : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_P";
  attribute X_INTERFACE_INFO of DDR4_MEM02_act_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ACT_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM02_act_n : signal is "XIL_INTERFACENAME DDR4_MEM02, AXI_ARBITRATION_SCHEME TDM, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 11, CAS_WRITE_LATENCY 11, CS_ENABLED true, DATA_MASK_ENABLED true, DATA_WIDTH 8, MEMORY_TYPE COMPONENTS, MEM_ADDR_MAP ROW_COLUMN_BANK, SLOT Single, TIMEPERIOD_PS 1250";
  attribute X_INTERFACE_INFO of DDR4_MEM02_par : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 PAR";
  attribute X_INTERFACE_INFO of DDR4_MEM02_reset_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 RESET_N";
  attribute X_INTERFACE_INFO of M00_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY";
  attribute X_INTERFACE_INFO of M00_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID";
  attribute X_INTERFACE_INFO of M00_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY";
  attribute X_INTERFACE_INFO of M00_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID";
  attribute X_INTERFACE_INFO of M00_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY";
  attribute X_INTERFACE_INFO of M00_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BVALID";
  attribute X_INTERFACE_INFO of M00_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RLAST";
  attribute X_INTERFACE_INFO of M00_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY";
  attribute X_INTERFACE_INFO of M00_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RVALID";
  attribute X_INTERFACE_INFO of M00_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WLAST";
  attribute X_INTERFACE_INFO of M00_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WREADY";
  attribute X_INTERFACE_INFO of M00_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID";
  attribute X_INTERFACE_INFO of S00_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARREADY";
  attribute X_INTERFACE_INFO of S00_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARVALID";
  attribute X_INTERFACE_INFO of S00_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWREADY";
  attribute X_INTERFACE_INFO of S00_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWVALID";
  attribute X_INTERFACE_INFO of S00_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BREADY";
  attribute X_INTERFACE_INFO of S00_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BVALID";
  attribute X_INTERFACE_INFO of S00_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RLAST";
  attribute X_INTERFACE_INFO of S00_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RREADY";
  attribute X_INTERFACE_INFO of S00_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RVALID";
  attribute X_INTERFACE_INFO of S00_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WLAST";
  attribute X_INTERFACE_INFO of S00_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WREADY";
  attribute X_INTERFACE_INFO of S00_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WVALID";
  attribute X_INTERFACE_INFO of S01_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARREADY";
  attribute X_INTERFACE_INFO of S01_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARVALID";
  attribute X_INTERFACE_INFO of S01_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWREADY";
  attribute X_INTERFACE_INFO of S01_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWVALID";
  attribute X_INTERFACE_INFO of S01_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BREADY";
  attribute X_INTERFACE_INFO of S01_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BVALID";
  attribute X_INTERFACE_INFO of S01_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RLAST";
  attribute X_INTERFACE_INFO of S01_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RREADY";
  attribute X_INTERFACE_INFO of S01_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RVALID";
  attribute X_INTERFACE_INFO of S01_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WLAST";
  attribute X_INTERFACE_INFO of S01_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WREADY";
  attribute X_INTERFACE_INFO of S01_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WVALID";
  attribute X_INTERFACE_INFO of S02_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARREADY";
  attribute X_INTERFACE_INFO of S02_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARVALID";
  attribute X_INTERFACE_INFO of S02_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWREADY";
  attribute X_INTERFACE_INFO of S02_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWVALID";
  attribute X_INTERFACE_INFO of S02_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BREADY";
  attribute X_INTERFACE_INFO of S02_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BVALID";
  attribute X_INTERFACE_INFO of S02_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RLAST";
  attribute X_INTERFACE_INFO of S02_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RREADY";
  attribute X_INTERFACE_INFO of S02_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RVALID";
  attribute X_INTERFACE_INFO of S02_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WLAST";
  attribute X_INTERFACE_INFO of S02_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WREADY";
  attribute X_INTERFACE_INFO of S02_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WVALID";
  attribute X_INTERFACE_INFO of S03_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARREADY";
  attribute X_INTERFACE_INFO of S03_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARVALID";
  attribute X_INTERFACE_INFO of S03_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWREADY";
  attribute X_INTERFACE_INFO of S03_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWVALID";
  attribute X_INTERFACE_INFO of S03_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BREADY";
  attribute X_INTERFACE_INFO of S03_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BVALID";
  attribute X_INTERFACE_INFO of S03_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RLAST";
  attribute X_INTERFACE_INFO of S03_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RREADY";
  attribute X_INTERFACE_INFO of S03_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RVALID";
  attribute X_INTERFACE_INFO of S03_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WLAST";
  attribute X_INTERFACE_INFO of S03_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WREADY";
  attribute X_INTERFACE_INFO of S03_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WVALID";
  attribute X_INTERFACE_INFO of S04_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARREADY";
  attribute X_INTERFACE_INFO of S04_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARVALID";
  attribute X_INTERFACE_INFO of S04_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWREADY";
  attribute X_INTERFACE_INFO of S04_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWVALID";
  attribute X_INTERFACE_INFO of S04_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI BREADY";
  attribute X_INTERFACE_INFO of S04_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI BVALID";
  attribute X_INTERFACE_INFO of S04_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RLAST";
  attribute X_INTERFACE_INFO of S04_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RREADY";
  attribute X_INTERFACE_INFO of S04_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RVALID";
  attribute X_INTERFACE_INFO of S04_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WLAST";
  attribute X_INTERFACE_INFO of S04_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WREADY";
  attribute X_INTERFACE_INFO of S04_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WVALID";
  attribute X_INTERFACE_INFO of S05_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARREADY";
  attribute X_INTERFACE_INFO of S05_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARVALID";
  attribute X_INTERFACE_INFO of S05_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWREADY";
  attribute X_INTERFACE_INFO of S05_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWVALID";
  attribute X_INTERFACE_INFO of S05_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI BREADY";
  attribute X_INTERFACE_INFO of S05_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI BVALID";
  attribute X_INTERFACE_INFO of S05_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RLAST";
  attribute X_INTERFACE_INFO of S05_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RREADY";
  attribute X_INTERFACE_INFO of S05_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RVALID";
  attribute X_INTERFACE_INFO of S05_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WLAST";
  attribute X_INTERFACE_INFO of S05_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WREADY";
  attribute X_INTERFACE_INFO of S05_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_arready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_arvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_bready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_bvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WVALID";
  attribute X_INTERFACE_INFO of aclk : signal is "xilinx.com:signal:clock:1.0 CLK.ACLK CLK";
  attribute X_INTERFACE_PARAMETER of aclk : signal is "XIL_INTERFACENAME CLK.ACLK, ASSOCIATED_BUSIF S00_AXI:S01_AXI:S02_AXI:S03_AXI:S04_AXI:S05_AXI, ASSOCIATED_CLKEN m_sc_aclken, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000";
  attribute X_INTERFACE_INFO of aclk1 : signal is "xilinx.com:signal:clock:1.0 CLK.ACLK1 CLK";
  attribute X_INTERFACE_PARAMETER of aclk1 : signal is "XIL_INTERFACENAME CLK.ACLK1, ASSOCIATED_BUSIF S_AXI_CTRL, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, FREQ_HZ 50925925, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000";
  attribute X_INTERFACE_INFO of aclk2 : signal is "xilinx.com:signal:clock:1.0 CLK.ACLK2 CLK";
  attribute X_INTERFACE_PARAMETER of aclk2 : signal is "XIL_INTERFACENAME CLK.ACLK2, ASSOCIATED_BUSIF M00_AXI, ASSOCIATED_CLKEN m_sc_aclken, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000";
  attribute X_INTERFACE_INFO of aresetn : signal is "xilinx.com:signal:reset:1.0 RST.ARESETN RST";
  attribute X_INTERFACE_PARAMETER of aresetn : signal is "XIL_INTERFACENAME RST.ARESETN, INSERT_VIP 0, POLARITY ACTIVE_LOW";
  attribute X_INTERFACE_INFO of ddr4_mem00_sys_rst : signal is "xilinx.com:signal:reset:1.0 RST.DDR4_MEM00_SYS_RST RST";
  attribute X_INTERFACE_PARAMETER of ddr4_mem00_sys_rst : signal is "XIL_INTERFACENAME RST.DDR4_MEM00_SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH";
  attribute X_INTERFACE_INFO of ddr4_mem00_ui_clk : signal is "xilinx.com:signal:clock:1.0 CLK.DDR4_MEM00_UI_CLK CLK";
  attribute X_INTERFACE_PARAMETER of ddr4_mem00_ui_clk : signal is "XIL_INTERFACENAME CLK.DDR4_MEM00_UI_CLK, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem00_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000";
  attribute X_INTERFACE_INFO of ddr4_mem01_sys_rst : signal is "xilinx.com:signal:reset:1.0 RST.DDR4_MEM01_SYS_RST RST";
  attribute X_INTERFACE_PARAMETER of ddr4_mem01_sys_rst : signal is "XIL_INTERFACENAME RST.DDR4_MEM01_SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH";
  attribute X_INTERFACE_INFO of ddr4_mem01_ui_clk : signal is "xilinx.com:signal:clock:1.0 CLK.DDR4_MEM01_UI_CLK CLK";
  attribute X_INTERFACE_PARAMETER of ddr4_mem01_ui_clk : signal is "XIL_INTERFACENAME CLK.DDR4_MEM01_UI_CLK, CLK_DOMAIN bd_d216_ddr4_mem01_0_c0_ddr4_ui_clk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.00";
  attribute X_INTERFACE_INFO of ddr4_mem02_sys_rst : signal is "xilinx.com:signal:reset:1.0 RST.DDR4_MEM02_SYS_RST RST";
  attribute X_INTERFACE_PARAMETER of ddr4_mem02_sys_rst : signal is "XIL_INTERFACENAME RST.DDR4_MEM02_SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH";
  attribute X_INTERFACE_INFO of ddr4_mem02_ui_clk : signal is "xilinx.com:signal:clock:1.0 CLK.DDR4_MEM02_UI_CLK CLK";
  attribute X_INTERFACE_PARAMETER of ddr4_mem02_ui_clk : signal is "XIL_INTERFACENAME CLK.DDR4_MEM02_UI_CLK, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem02_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.000";
  attribute X_INTERFACE_INFO of ddr4_mem_calib_complete : signal is "xilinx.com:signal:data:1.0 DATA.DDR4_MEM_CALIB_COMPLETE DATA";
  attribute X_INTERFACE_PARAMETER of ddr4_mem_calib_complete : signal is "XIL_INTERFACENAME DATA.DDR4_MEM_CALIB_COMPLETE, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of DDR4_MEM00_adr : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ADR";
  attribute X_INTERFACE_INFO of DDR4_MEM00_ba : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BA";
  attribute X_INTERFACE_INFO of DDR4_MEM00_bg : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BG";
  attribute X_INTERFACE_INFO of DDR4_MEM00_ck_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_C";
  attribute X_INTERFACE_INFO of DDR4_MEM00_ck_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_T";
  attribute X_INTERFACE_INFO of DDR4_MEM00_cke : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CKE";
  attribute X_INTERFACE_INFO of DDR4_MEM00_cs_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CS_N";
  attribute X_INTERFACE_INFO of DDR4_MEM00_dq : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQ";
  attribute X_INTERFACE_INFO of DDR4_MEM00_dqs_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_C";
  attribute X_INTERFACE_INFO of DDR4_MEM00_dqs_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_T";
  attribute X_INTERFACE_INFO of DDR4_MEM00_odt : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ODT";
  attribute X_INTERFACE_INFO of DDR4_MEM01_adr : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ADR";
  attribute X_INTERFACE_INFO of DDR4_MEM01_ba : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BA";
  attribute X_INTERFACE_INFO of DDR4_MEM01_bg : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BG";
  attribute X_INTERFACE_INFO of DDR4_MEM01_ck_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_C";
  attribute X_INTERFACE_INFO of DDR4_MEM01_ck_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_T";
  attribute X_INTERFACE_INFO of DDR4_MEM01_cke : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CKE";
  attribute X_INTERFACE_INFO of DDR4_MEM01_cs_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CS_N";
  attribute X_INTERFACE_INFO of DDR4_MEM01_dq : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQ";
  attribute X_INTERFACE_INFO of DDR4_MEM01_dqs_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_C";
  attribute X_INTERFACE_INFO of DDR4_MEM01_dqs_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_T";
  attribute X_INTERFACE_INFO of DDR4_MEM01_odt : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ODT";
  attribute X_INTERFACE_INFO of DDR4_MEM02_adr : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ADR";
  attribute X_INTERFACE_INFO of DDR4_MEM02_ba : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BA";
  attribute X_INTERFACE_INFO of DDR4_MEM02_bg : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BG";
  attribute X_INTERFACE_INFO of DDR4_MEM02_ck_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_C";
  attribute X_INTERFACE_INFO of DDR4_MEM02_ck_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_T";
  attribute X_INTERFACE_INFO of DDR4_MEM02_cke : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CKE";
  attribute X_INTERFACE_INFO of DDR4_MEM02_cs_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CS_N";
  attribute X_INTERFACE_INFO of DDR4_MEM02_dq : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQ";
  attribute X_INTERFACE_INFO of DDR4_MEM02_dqs_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_C";
  attribute X_INTERFACE_INFO of DDR4_MEM02_dqs_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_T";
  attribute X_INTERFACE_INFO of DDR4_MEM02_odt : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ODT";
  attribute X_INTERFACE_INFO of M00_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of M00_AXI_araddr : signal is "XIL_INTERFACENAME M00_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 1, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of M00_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST";
  attribute X_INTERFACE_INFO of M00_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARCACHE";
  attribute X_INTERFACE_INFO of M00_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN";
  attribute X_INTERFACE_INFO of M00_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARLOCK";
  attribute X_INTERFACE_INFO of M00_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARPROT";
  attribute X_INTERFACE_INFO of M00_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARQOS";
  attribute X_INTERFACE_INFO of M00_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREGION";
  attribute X_INTERFACE_INFO of M00_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE";
  attribute X_INTERFACE_INFO of M00_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR";
  attribute X_INTERFACE_INFO of M00_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST";
  attribute X_INTERFACE_INFO of M00_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWCACHE";
  attribute X_INTERFACE_INFO of M00_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN";
  attribute X_INTERFACE_INFO of M00_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWLOCK";
  attribute X_INTERFACE_INFO of M00_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWPROT";
  attribute X_INTERFACE_INFO of M00_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWQOS";
  attribute X_INTERFACE_INFO of M00_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREGION";
  attribute X_INTERFACE_INFO of M00_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE";
  attribute X_INTERFACE_INFO of M00_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BRESP";
  attribute X_INTERFACE_INFO of M00_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RDATA";
  attribute X_INTERFACE_INFO of M00_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RRESP";
  attribute X_INTERFACE_INFO of M00_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of M00_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of S00_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of S00_AXI_araddr : signal is "XIL_INTERFACENAME S00_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S00_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARBURST";
  attribute X_INTERFACE_INFO of S00_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S00_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARID";
  attribute X_INTERFACE_INFO of S00_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARLEN";
  attribute X_INTERFACE_INFO of S00_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S00_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARPROT";
  attribute X_INTERFACE_INFO of S00_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARQOS";
  attribute X_INTERFACE_INFO of S00_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARREGION";
  attribute X_INTERFACE_INFO of S00_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARSIZE";
  attribute X_INTERFACE_INFO of S00_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWADDR";
  attribute X_INTERFACE_INFO of S00_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWBURST";
  attribute X_INTERFACE_INFO of S00_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S00_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWID";
  attribute X_INTERFACE_INFO of S00_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWLEN";
  attribute X_INTERFACE_INFO of S00_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S00_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWPROT";
  attribute X_INTERFACE_INFO of S00_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWQOS";
  attribute X_INTERFACE_INFO of S00_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWREGION";
  attribute X_INTERFACE_INFO of S00_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWSIZE";
  attribute X_INTERFACE_INFO of S00_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BID";
  attribute X_INTERFACE_INFO of S00_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BRESP";
  attribute X_INTERFACE_INFO of S00_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RDATA";
  attribute X_INTERFACE_INFO of S00_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RID";
  attribute X_INTERFACE_INFO of S00_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RRESP";
  attribute X_INTERFACE_INFO of S00_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WDATA";
  attribute X_INTERFACE_INFO of S00_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WSTRB";
  attribute X_INTERFACE_INFO of S01_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of S01_AXI_araddr : signal is "XIL_INTERFACENAME S01_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S01_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARBURST";
  attribute X_INTERFACE_INFO of S01_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S01_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARID";
  attribute X_INTERFACE_INFO of S01_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARLEN";
  attribute X_INTERFACE_INFO of S01_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S01_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARPROT";
  attribute X_INTERFACE_INFO of S01_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARQOS";
  attribute X_INTERFACE_INFO of S01_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARREGION";
  attribute X_INTERFACE_INFO of S01_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWADDR";
  attribute X_INTERFACE_INFO of S01_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWBURST";
  attribute X_INTERFACE_INFO of S01_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S01_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWID";
  attribute X_INTERFACE_INFO of S01_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWLEN";
  attribute X_INTERFACE_INFO of S01_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S01_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWPROT";
  attribute X_INTERFACE_INFO of S01_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWQOS";
  attribute X_INTERFACE_INFO of S01_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWREGION";
  attribute X_INTERFACE_INFO of S01_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BID";
  attribute X_INTERFACE_INFO of S01_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BRESP";
  attribute X_INTERFACE_INFO of S01_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RDATA";
  attribute X_INTERFACE_INFO of S01_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RID";
  attribute X_INTERFACE_INFO of S01_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RRESP";
  attribute X_INTERFACE_INFO of S01_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WDATA";
  attribute X_INTERFACE_INFO of S01_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WSTRB";
  attribute X_INTERFACE_INFO of S02_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of S02_AXI_araddr : signal is "XIL_INTERFACENAME S02_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S02_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARBURST";
  attribute X_INTERFACE_INFO of S02_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S02_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARID";
  attribute X_INTERFACE_INFO of S02_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARLEN";
  attribute X_INTERFACE_INFO of S02_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S02_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARPROT";
  attribute X_INTERFACE_INFO of S02_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARQOS";
  attribute X_INTERFACE_INFO of S02_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARREGION";
  attribute X_INTERFACE_INFO of S02_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWADDR";
  attribute X_INTERFACE_INFO of S02_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWBURST";
  attribute X_INTERFACE_INFO of S02_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S02_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWID";
  attribute X_INTERFACE_INFO of S02_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWLEN";
  attribute X_INTERFACE_INFO of S02_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S02_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWPROT";
  attribute X_INTERFACE_INFO of S02_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWQOS";
  attribute X_INTERFACE_INFO of S02_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWREGION";
  attribute X_INTERFACE_INFO of S02_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BID";
  attribute X_INTERFACE_INFO of S02_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BRESP";
  attribute X_INTERFACE_INFO of S02_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RDATA";
  attribute X_INTERFACE_INFO of S02_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RID";
  attribute X_INTERFACE_INFO of S02_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RRESP";
  attribute X_INTERFACE_INFO of S02_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WDATA";
  attribute X_INTERFACE_INFO of S02_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WSTRB";
  attribute X_INTERFACE_INFO of S03_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of S03_AXI_araddr : signal is "XIL_INTERFACENAME S03_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 4, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 2, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 2, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S03_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARBURST";
  attribute X_INTERFACE_INFO of S03_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S03_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARID";
  attribute X_INTERFACE_INFO of S03_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARLEN";
  attribute X_INTERFACE_INFO of S03_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S03_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARPROT";
  attribute X_INTERFACE_INFO of S03_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARQOS";
  attribute X_INTERFACE_INFO of S03_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARREGION";
  attribute X_INTERFACE_INFO of S03_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARSIZE";
  attribute X_INTERFACE_INFO of S03_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWADDR";
  attribute X_INTERFACE_INFO of S03_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWBURST";
  attribute X_INTERFACE_INFO of S03_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S03_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWID";
  attribute X_INTERFACE_INFO of S03_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWLEN";
  attribute X_INTERFACE_INFO of S03_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S03_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWPROT";
  attribute X_INTERFACE_INFO of S03_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWQOS";
  attribute X_INTERFACE_INFO of S03_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWREGION";
  attribute X_INTERFACE_INFO of S03_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWSIZE";
  attribute X_INTERFACE_INFO of S03_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BID";
  attribute X_INTERFACE_INFO of S03_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BRESP";
  attribute X_INTERFACE_INFO of S03_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RDATA";
  attribute X_INTERFACE_INFO of S03_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RID";
  attribute X_INTERFACE_INFO of S03_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RRESP";
  attribute X_INTERFACE_INFO of S03_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WDATA";
  attribute X_INTERFACE_INFO of S03_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WSTRB";
  attribute X_INTERFACE_INFO of S04_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of S04_AXI_araddr : signal is "XIL_INTERFACENAME S04_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 0, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S04_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S04_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARLEN";
  attribute X_INTERFACE_INFO of S04_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S04_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARPROT";
  attribute X_INTERFACE_INFO of S04_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARQOS";
  attribute X_INTERFACE_INFO of S04_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARREGION";
  attribute X_INTERFACE_INFO of S04_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWADDR";
  attribute X_INTERFACE_INFO of S04_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S04_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWLEN";
  attribute X_INTERFACE_INFO of S04_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S04_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWPROT";
  attribute X_INTERFACE_INFO of S04_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWQOS";
  attribute X_INTERFACE_INFO of S04_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWREGION";
  attribute X_INTERFACE_INFO of S04_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S04_AXI BRESP";
  attribute X_INTERFACE_INFO of S04_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RDATA";
  attribute X_INTERFACE_INFO of S04_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RRESP";
  attribute X_INTERFACE_INFO of S04_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WDATA";
  attribute X_INTERFACE_INFO of S04_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WSTRB";
  attribute X_INTERFACE_INFO of S05_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of S05_AXI_araddr : signal is "XIL_INTERFACENAME S05_AXI, ADDR_WIDTH 39, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, DATA_WIDTH 512, FREQ_HZ 300000000, HAS_BRESP 1, HAS_BURST 0, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 1, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, NUM_READ_OUTSTANDING 16, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 16, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S05_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S05_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARLEN";
  attribute X_INTERFACE_INFO of S05_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S05_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARPROT";
  attribute X_INTERFACE_INFO of S05_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARQOS";
  attribute X_INTERFACE_INFO of S05_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARREGION";
  attribute X_INTERFACE_INFO of S05_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWADDR";
  attribute X_INTERFACE_INFO of S05_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S05_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWLEN";
  attribute X_INTERFACE_INFO of S05_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S05_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWPROT";
  attribute X_INTERFACE_INFO of S05_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWQOS";
  attribute X_INTERFACE_INFO of S05_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWREGION";
  attribute X_INTERFACE_INFO of S05_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S05_AXI BRESP";
  attribute X_INTERFACE_INFO of S05_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RDATA";
  attribute X_INTERFACE_INFO of S05_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RRESP";
  attribute X_INTERFACE_INFO of S05_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WDATA";
  attribute X_INTERFACE_INFO of S05_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WSTRB";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_araddr : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARADDR";
  attribute X_INTERFACE_PARAMETER of S_AXI_CTRL_araddr : signal is "XIL_INTERFACENAME S_AXI_CTRL, ADDR_WIDTH 25, ARUSER_WIDTH 0, AWUSER_WIDTH 0, BUSER_WIDTH 0, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, DATA_WIDTH 32, FREQ_HZ 50925925, HAS_BRESP 1, HAS_BURST 0, HAS_CACHE 0, HAS_LOCK 0, HAS_PROT 1, HAS_QOS 0, HAS_REGION 0, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 0, INSERT_VIP 0, MAX_BURST_LENGTH 1, NUM_READ_OUTSTANDING 2, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 2, NUM_WRITE_THREADS 1, PHASE 0.000, PROTOCOL AXI4LITE, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 0, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 0";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_arprot : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARPROT";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awaddr : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWADDR";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awprot : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWPROT";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_bresp : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BRESP";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rdata : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RDATA";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rresp : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RRESP";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wdata : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WDATA";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wstrb : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WSTRB";
  attribute X_INTERFACE_INFO of ddr4_mem_calib_vec : signal is "xilinx.com:signal:data:1.0 DATA.DDR4_MEM_CALIB_VEC DATA";
  attribute X_INTERFACE_PARAMETER of ddr4_mem_calib_vec : signal is "XIL_INTERFACENAME DATA.DDR4_MEM_CALIB_VEC, LAYERED_METADATA undef, PortWidth 3";
begin
  DDR4_MEM00_act_n <= \<const0>\;
  DDR4_MEM00_adr(16) <= \<const0>\;
  DDR4_MEM00_adr(15) <= \<const0>\;
  DDR4_MEM00_adr(14) <= \<const0>\;
  DDR4_MEM00_adr(13) <= \<const0>\;
  DDR4_MEM00_adr(12) <= \<const0>\;
  DDR4_MEM00_adr(11) <= \<const0>\;
  DDR4_MEM00_adr(10) <= \<const0>\;
  DDR4_MEM00_adr(9) <= \<const0>\;
  DDR4_MEM00_adr(8) <= \<const0>\;
  DDR4_MEM00_adr(7) <= \<const0>\;
  DDR4_MEM00_adr(6) <= \<const0>\;
  DDR4_MEM00_adr(5) <= \<const0>\;
  DDR4_MEM00_adr(4) <= \<const0>\;
  DDR4_MEM00_adr(3) <= \<const0>\;
  DDR4_MEM00_adr(2) <= \<const0>\;
  DDR4_MEM00_adr(1) <= \<const0>\;
  DDR4_MEM00_adr(0) <= \<const0>\;
  DDR4_MEM00_ba(1) <= \<const0>\;
  DDR4_MEM00_ba(0) <= \<const0>\;
  DDR4_MEM00_bg(1) <= \<const0>\;
  DDR4_MEM00_bg(0) <= \<const0>\;
  DDR4_MEM00_ck_c(0) <= \<const0>\;
  DDR4_MEM00_ck_t(0) <= \<const0>\;
  DDR4_MEM00_cke(0) <= \<const0>\;
  DDR4_MEM00_cs_n(0) <= \<const0>\;
  DDR4_MEM00_odt(0) <= \<const0>\;
  DDR4_MEM00_par <= \<const0>\;
  DDR4_MEM00_reset_n <= \<const0>\;
  DDR4_MEM02_act_n <= \<const0>\;
  DDR4_MEM02_adr(16) <= \<const0>\;
  DDR4_MEM02_adr(15) <= \<const0>\;
  DDR4_MEM02_adr(14) <= \<const0>\;
  DDR4_MEM02_adr(13) <= \<const0>\;
  DDR4_MEM02_adr(12) <= \<const0>\;
  DDR4_MEM02_adr(11) <= \<const0>\;
  DDR4_MEM02_adr(10) <= \<const0>\;
  DDR4_MEM02_adr(9) <= \<const0>\;
  DDR4_MEM02_adr(8) <= \<const0>\;
  DDR4_MEM02_adr(7) <= \<const0>\;
  DDR4_MEM02_adr(6) <= \<const0>\;
  DDR4_MEM02_adr(5) <= \<const0>\;
  DDR4_MEM02_adr(4) <= \<const0>\;
  DDR4_MEM02_adr(3) <= \<const0>\;
  DDR4_MEM02_adr(2) <= \<const0>\;
  DDR4_MEM02_adr(1) <= \<const0>\;
  DDR4_MEM02_adr(0) <= \<const0>\;
  DDR4_MEM02_ba(1) <= \<const0>\;
  DDR4_MEM02_ba(0) <= \<const0>\;
  DDR4_MEM02_bg(1) <= \<const0>\;
  DDR4_MEM02_bg(0) <= \<const0>\;
  DDR4_MEM02_ck_c(0) <= \<const0>\;
  DDR4_MEM02_ck_t(0) <= \<const0>\;
  DDR4_MEM02_cke(0) <= \<const0>\;
  DDR4_MEM02_cs_n(0) <= \<const0>\;
  DDR4_MEM02_odt(0) <= \<const0>\;
  DDR4_MEM02_par <= \<const0>\;
  DDR4_MEM02_reset_n <= \<const0>\;
  S00_AXI_arready <= \<const0>\;
  S00_AXI_awready <= \<const0>\;
  S00_AXI_bid(3) <= \<const0>\;
  S00_AXI_bid(2) <= \<const0>\;
  S00_AXI_bid(1) <= \<const0>\;
  S00_AXI_bid(0) <= \<const0>\;
  S00_AXI_bresp(1) <= \<const0>\;
  S00_AXI_bresp(0) <= \<const0>\;
  S00_AXI_bvalid <= \<const0>\;
  S00_AXI_rdata(511) <= \<const0>\;
  S00_AXI_rdata(510) <= \<const0>\;
  S00_AXI_rdata(509) <= \<const0>\;
  S00_AXI_rdata(508) <= \<const0>\;
  S00_AXI_rdata(507) <= \<const0>\;
  S00_AXI_rdata(506) <= \<const0>\;
  S00_AXI_rdata(505) <= \<const0>\;
  S00_AXI_rdata(504) <= \<const0>\;
  S00_AXI_rdata(503) <= \<const0>\;
  S00_AXI_rdata(502) <= \<const0>\;
  S00_AXI_rdata(501) <= \<const0>\;
  S00_AXI_rdata(500) <= \<const0>\;
  S00_AXI_rdata(499) <= \<const0>\;
  S00_AXI_rdata(498) <= \<const0>\;
  S00_AXI_rdata(497) <= \<const0>\;
  S00_AXI_rdata(496) <= \<const0>\;
  S00_AXI_rdata(495) <= \<const0>\;
  S00_AXI_rdata(494) <= \<const0>\;
  S00_AXI_rdata(493) <= \<const0>\;
  S00_AXI_rdata(492) <= \<const0>\;
  S00_AXI_rdata(491) <= \<const0>\;
  S00_AXI_rdata(490) <= \<const0>\;
  S00_AXI_rdata(489) <= \<const0>\;
  S00_AXI_rdata(488) <= \<const0>\;
  S00_AXI_rdata(487) <= \<const0>\;
  S00_AXI_rdata(486) <= \<const0>\;
  S00_AXI_rdata(485) <= \<const0>\;
  S00_AXI_rdata(484) <= \<const0>\;
  S00_AXI_rdata(483) <= \<const0>\;
  S00_AXI_rdata(482) <= \<const0>\;
  S00_AXI_rdata(481) <= \<const0>\;
  S00_AXI_rdata(480) <= \<const0>\;
  S00_AXI_rdata(479) <= \<const0>\;
  S00_AXI_rdata(478) <= \<const0>\;
  S00_AXI_rdata(477) <= \<const0>\;
  S00_AXI_rdata(476) <= \<const0>\;
  S00_AXI_rdata(475) <= \<const0>\;
  S00_AXI_rdata(474) <= \<const0>\;
  S00_AXI_rdata(473) <= \<const0>\;
  S00_AXI_rdata(472) <= \<const0>\;
  S00_AXI_rdata(471) <= \<const0>\;
  S00_AXI_rdata(470) <= \<const0>\;
  S00_AXI_rdata(469) <= \<const0>\;
  S00_AXI_rdata(468) <= \<const0>\;
  S00_AXI_rdata(467) <= \<const0>\;
  S00_AXI_rdata(466) <= \<const0>\;
  S00_AXI_rdata(465) <= \<const0>\;
  S00_AXI_rdata(464) <= \<const0>\;
  S00_AXI_rdata(463) <= \<const0>\;
  S00_AXI_rdata(462) <= \<const0>\;
  S00_AXI_rdata(461) <= \<const0>\;
  S00_AXI_rdata(460) <= \<const0>\;
  S00_AXI_rdata(459) <= \<const0>\;
  S00_AXI_rdata(458) <= \<const0>\;
  S00_AXI_rdata(457) <= \<const0>\;
  S00_AXI_rdata(456) <= \<const0>\;
  S00_AXI_rdata(455) <= \<const0>\;
  S00_AXI_rdata(454) <= \<const0>\;
  S00_AXI_rdata(453) <= \<const0>\;
  S00_AXI_rdata(452) <= \<const0>\;
  S00_AXI_rdata(451) <= \<const0>\;
  S00_AXI_rdata(450) <= \<const0>\;
  S00_AXI_rdata(449) <= \<const0>\;
  S00_AXI_rdata(448) <= \<const0>\;
  S00_AXI_rdata(447) <= \<const0>\;
  S00_AXI_rdata(446) <= \<const0>\;
  S00_AXI_rdata(445) <= \<const0>\;
  S00_AXI_rdata(444) <= \<const0>\;
  S00_AXI_rdata(443) <= \<const0>\;
  S00_AXI_rdata(442) <= \<const0>\;
  S00_AXI_rdata(441) <= \<const0>\;
  S00_AXI_rdata(440) <= \<const0>\;
  S00_AXI_rdata(439) <= \<const0>\;
  S00_AXI_rdata(438) <= \<const0>\;
  S00_AXI_rdata(437) <= \<const0>\;
  S00_AXI_rdata(436) <= \<const0>\;
  S00_AXI_rdata(435) <= \<const0>\;
  S00_AXI_rdata(434) <= \<const0>\;
  S00_AXI_rdata(433) <= \<const0>\;
  S00_AXI_rdata(432) <= \<const0>\;
  S00_AXI_rdata(431) <= \<const0>\;
  S00_AXI_rdata(430) <= \<const0>\;
  S00_AXI_rdata(429) <= \<const0>\;
  S00_AXI_rdata(428) <= \<const0>\;
  S00_AXI_rdata(427) <= \<const0>\;
  S00_AXI_rdata(426) <= \<const0>\;
  S00_AXI_rdata(425) <= \<const0>\;
  S00_AXI_rdata(424) <= \<const0>\;
  S00_AXI_rdata(423) <= \<const0>\;
  S00_AXI_rdata(422) <= \<const0>\;
  S00_AXI_rdata(421) <= \<const0>\;
  S00_AXI_rdata(420) <= \<const0>\;
  S00_AXI_rdata(419) <= \<const0>\;
  S00_AXI_rdata(418) <= \<const0>\;
  S00_AXI_rdata(417) <= \<const0>\;
  S00_AXI_rdata(416) <= \<const0>\;
  S00_AXI_rdata(415) <= \<const0>\;
  S00_AXI_rdata(414) <= \<const0>\;
  S00_AXI_rdata(413) <= \<const0>\;
  S00_AXI_rdata(412) <= \<const0>\;
  S00_AXI_rdata(411) <= \<const0>\;
  S00_AXI_rdata(410) <= \<const0>\;
  S00_AXI_rdata(409) <= \<const0>\;
  S00_AXI_rdata(408) <= \<const0>\;
  S00_AXI_rdata(407) <= \<const0>\;
  S00_AXI_rdata(406) <= \<const0>\;
  S00_AXI_rdata(405) <= \<const0>\;
  S00_AXI_rdata(404) <= \<const0>\;
  S00_AXI_rdata(403) <= \<const0>\;
  S00_AXI_rdata(402) <= \<const0>\;
  S00_AXI_rdata(401) <= \<const0>\;
  S00_AXI_rdata(400) <= \<const0>\;
  S00_AXI_rdata(399) <= \<const0>\;
  S00_AXI_rdata(398) <= \<const0>\;
  S00_AXI_rdata(397) <= \<const0>\;
  S00_AXI_rdata(396) <= \<const0>\;
  S00_AXI_rdata(395) <= \<const0>\;
  S00_AXI_rdata(394) <= \<const0>\;
  S00_AXI_rdata(393) <= \<const0>\;
  S00_AXI_rdata(392) <= \<const0>\;
  S00_AXI_rdata(391) <= \<const0>\;
  S00_AXI_rdata(390) <= \<const0>\;
  S00_AXI_rdata(389) <= \<const0>\;
  S00_AXI_rdata(388) <= \<const0>\;
  S00_AXI_rdata(387) <= \<const0>\;
  S00_AXI_rdata(386) <= \<const0>\;
  S00_AXI_rdata(385) <= \<const0>\;
  S00_AXI_rdata(384) <= \<const0>\;
  S00_AXI_rdata(383) <= \<const0>\;
  S00_AXI_rdata(382) <= \<const0>\;
  S00_AXI_rdata(381) <= \<const0>\;
  S00_AXI_rdata(380) <= \<const0>\;
  S00_AXI_rdata(379) <= \<const0>\;
  S00_AXI_rdata(378) <= \<const0>\;
  S00_AXI_rdata(377) <= \<const0>\;
  S00_AXI_rdata(376) <= \<const0>\;
  S00_AXI_rdata(375) <= \<const0>\;
  S00_AXI_rdata(374) <= \<const0>\;
  S00_AXI_rdata(373) <= \<const0>\;
  S00_AXI_rdata(372) <= \<const0>\;
  S00_AXI_rdata(371) <= \<const0>\;
  S00_AXI_rdata(370) <= \<const0>\;
  S00_AXI_rdata(369) <= \<const0>\;
  S00_AXI_rdata(368) <= \<const0>\;
  S00_AXI_rdata(367) <= \<const0>\;
  S00_AXI_rdata(366) <= \<const0>\;
  S00_AXI_rdata(365) <= \<const0>\;
  S00_AXI_rdata(364) <= \<const0>\;
  S00_AXI_rdata(363) <= \<const0>\;
  S00_AXI_rdata(362) <= \<const0>\;
  S00_AXI_rdata(361) <= \<const0>\;
  S00_AXI_rdata(360) <= \<const0>\;
  S00_AXI_rdata(359) <= \<const0>\;
  S00_AXI_rdata(358) <= \<const0>\;
  S00_AXI_rdata(357) <= \<const0>\;
  S00_AXI_rdata(356) <= \<const0>\;
  S00_AXI_rdata(355) <= \<const0>\;
  S00_AXI_rdata(354) <= \<const0>\;
  S00_AXI_rdata(353) <= \<const0>\;
  S00_AXI_rdata(352) <= \<const0>\;
  S00_AXI_rdata(351) <= \<const0>\;
  S00_AXI_rdata(350) <= \<const0>\;
  S00_AXI_rdata(349) <= \<const0>\;
  S00_AXI_rdata(348) <= \<const0>\;
  S00_AXI_rdata(347) <= \<const0>\;
  S00_AXI_rdata(346) <= \<const0>\;
  S00_AXI_rdata(345) <= \<const0>\;
  S00_AXI_rdata(344) <= \<const0>\;
  S00_AXI_rdata(343) <= \<const0>\;
  S00_AXI_rdata(342) <= \<const0>\;
  S00_AXI_rdata(341) <= \<const0>\;
  S00_AXI_rdata(340) <= \<const0>\;
  S00_AXI_rdata(339) <= \<const0>\;
  S00_AXI_rdata(338) <= \<const0>\;
  S00_AXI_rdata(337) <= \<const0>\;
  S00_AXI_rdata(336) <= \<const0>\;
  S00_AXI_rdata(335) <= \<const0>\;
  S00_AXI_rdata(334) <= \<const0>\;
  S00_AXI_rdata(333) <= \<const0>\;
  S00_AXI_rdata(332) <= \<const0>\;
  S00_AXI_rdata(331) <= \<const0>\;
  S00_AXI_rdata(330) <= \<const0>\;
  S00_AXI_rdata(329) <= \<const0>\;
  S00_AXI_rdata(328) <= \<const0>\;
  S00_AXI_rdata(327) <= \<const0>\;
  S00_AXI_rdata(326) <= \<const0>\;
  S00_AXI_rdata(325) <= \<const0>\;
  S00_AXI_rdata(324) <= \<const0>\;
  S00_AXI_rdata(323) <= \<const0>\;
  S00_AXI_rdata(322) <= \<const0>\;
  S00_AXI_rdata(321) <= \<const0>\;
  S00_AXI_rdata(320) <= \<const0>\;
  S00_AXI_rdata(319) <= \<const0>\;
  S00_AXI_rdata(318) <= \<const0>\;
  S00_AXI_rdata(317) <= \<const0>\;
  S00_AXI_rdata(316) <= \<const0>\;
  S00_AXI_rdata(315) <= \<const0>\;
  S00_AXI_rdata(314) <= \<const0>\;
  S00_AXI_rdata(313) <= \<const0>\;
  S00_AXI_rdata(312) <= \<const0>\;
  S00_AXI_rdata(311) <= \<const0>\;
  S00_AXI_rdata(310) <= \<const0>\;
  S00_AXI_rdata(309) <= \<const0>\;
  S00_AXI_rdata(308) <= \<const0>\;
  S00_AXI_rdata(307) <= \<const0>\;
  S00_AXI_rdata(306) <= \<const0>\;
  S00_AXI_rdata(305) <= \<const0>\;
  S00_AXI_rdata(304) <= \<const0>\;
  S00_AXI_rdata(303) <= \<const0>\;
  S00_AXI_rdata(302) <= \<const0>\;
  S00_AXI_rdata(301) <= \<const0>\;
  S00_AXI_rdata(300) <= \<const0>\;
  S00_AXI_rdata(299) <= \<const0>\;
  S00_AXI_rdata(298) <= \<const0>\;
  S00_AXI_rdata(297) <= \<const0>\;
  S00_AXI_rdata(296) <= \<const0>\;
  S00_AXI_rdata(295) <= \<const0>\;
  S00_AXI_rdata(294) <= \<const0>\;
  S00_AXI_rdata(293) <= \<const0>\;
  S00_AXI_rdata(292) <= \<const0>\;
  S00_AXI_rdata(291) <= \<const0>\;
  S00_AXI_rdata(290) <= \<const0>\;
  S00_AXI_rdata(289) <= \<const0>\;
  S00_AXI_rdata(288) <= \<const0>\;
  S00_AXI_rdata(287) <= \<const0>\;
  S00_AXI_rdata(286) <= \<const0>\;
  S00_AXI_rdata(285) <= \<const0>\;
  S00_AXI_rdata(284) <= \<const0>\;
  S00_AXI_rdata(283) <= \<const0>\;
  S00_AXI_rdata(282) <= \<const0>\;
  S00_AXI_rdata(281) <= \<const0>\;
  S00_AXI_rdata(280) <= \<const0>\;
  S00_AXI_rdata(279) <= \<const0>\;
  S00_AXI_rdata(278) <= \<const0>\;
  S00_AXI_rdata(277) <= \<const0>\;
  S00_AXI_rdata(276) <= \<const0>\;
  S00_AXI_rdata(275) <= \<const0>\;
  S00_AXI_rdata(274) <= \<const0>\;
  S00_AXI_rdata(273) <= \<const0>\;
  S00_AXI_rdata(272) <= \<const0>\;
  S00_AXI_rdata(271) <= \<const0>\;
  S00_AXI_rdata(270) <= \<const0>\;
  S00_AXI_rdata(269) <= \<const0>\;
  S00_AXI_rdata(268) <= \<const0>\;
  S00_AXI_rdata(267) <= \<const0>\;
  S00_AXI_rdata(266) <= \<const0>\;
  S00_AXI_rdata(265) <= \<const0>\;
  S00_AXI_rdata(264) <= \<const0>\;
  S00_AXI_rdata(263) <= \<const0>\;
  S00_AXI_rdata(262) <= \<const0>\;
  S00_AXI_rdata(261) <= \<const0>\;
  S00_AXI_rdata(260) <= \<const0>\;
  S00_AXI_rdata(259) <= \<const0>\;
  S00_AXI_rdata(258) <= \<const0>\;
  S00_AXI_rdata(257) <= \<const0>\;
  S00_AXI_rdata(256) <= \<const0>\;
  S00_AXI_rdata(255) <= \<const0>\;
  S00_AXI_rdata(254) <= \<const0>\;
  S00_AXI_rdata(253) <= \<const0>\;
  S00_AXI_rdata(252) <= \<const0>\;
  S00_AXI_rdata(251) <= \<const0>\;
  S00_AXI_rdata(250) <= \<const0>\;
  S00_AXI_rdata(249) <= \<const0>\;
  S00_AXI_rdata(248) <= \<const0>\;
  S00_AXI_rdata(247) <= \<const0>\;
  S00_AXI_rdata(246) <= \<const0>\;
  S00_AXI_rdata(245) <= \<const0>\;
  S00_AXI_rdata(244) <= \<const0>\;
  S00_AXI_rdata(243) <= \<const0>\;
  S00_AXI_rdata(242) <= \<const0>\;
  S00_AXI_rdata(241) <= \<const0>\;
  S00_AXI_rdata(240) <= \<const0>\;
  S00_AXI_rdata(239) <= \<const0>\;
  S00_AXI_rdata(238) <= \<const0>\;
  S00_AXI_rdata(237) <= \<const0>\;
  S00_AXI_rdata(236) <= \<const0>\;
  S00_AXI_rdata(235) <= \<const0>\;
  S00_AXI_rdata(234) <= \<const0>\;
  S00_AXI_rdata(233) <= \<const0>\;
  S00_AXI_rdata(232) <= \<const0>\;
  S00_AXI_rdata(231) <= \<const0>\;
  S00_AXI_rdata(230) <= \<const0>\;
  S00_AXI_rdata(229) <= \<const0>\;
  S00_AXI_rdata(228) <= \<const0>\;
  S00_AXI_rdata(227) <= \<const0>\;
  S00_AXI_rdata(226) <= \<const0>\;
  S00_AXI_rdata(225) <= \<const0>\;
  S00_AXI_rdata(224) <= \<const0>\;
  S00_AXI_rdata(223) <= \<const0>\;
  S00_AXI_rdata(222) <= \<const0>\;
  S00_AXI_rdata(221) <= \<const0>\;
  S00_AXI_rdata(220) <= \<const0>\;
  S00_AXI_rdata(219) <= \<const0>\;
  S00_AXI_rdata(218) <= \<const0>\;
  S00_AXI_rdata(217) <= \<const0>\;
  S00_AXI_rdata(216) <= \<const0>\;
  S00_AXI_rdata(215) <= \<const0>\;
  S00_AXI_rdata(214) <= \<const0>\;
  S00_AXI_rdata(213) <= \<const0>\;
  S00_AXI_rdata(212) <= \<const0>\;
  S00_AXI_rdata(211) <= \<const0>\;
  S00_AXI_rdata(210) <= \<const0>\;
  S00_AXI_rdata(209) <= \<const0>\;
  S00_AXI_rdata(208) <= \<const0>\;
  S00_AXI_rdata(207) <= \<const0>\;
  S00_AXI_rdata(206) <= \<const0>\;
  S00_AXI_rdata(205) <= \<const0>\;
  S00_AXI_rdata(204) <= \<const0>\;
  S00_AXI_rdata(203) <= \<const0>\;
  S00_AXI_rdata(202) <= \<const0>\;
  S00_AXI_rdata(201) <= \<const0>\;
  S00_AXI_rdata(200) <= \<const0>\;
  S00_AXI_rdata(199) <= \<const0>\;
  S00_AXI_rdata(198) <= \<const0>\;
  S00_AXI_rdata(197) <= \<const0>\;
  S00_AXI_rdata(196) <= \<const0>\;
  S00_AXI_rdata(195) <= \<const0>\;
  S00_AXI_rdata(194) <= \<const0>\;
  S00_AXI_rdata(193) <= \<const0>\;
  S00_AXI_rdata(192) <= \<const0>\;
  S00_AXI_rdata(191) <= \<const0>\;
  S00_AXI_rdata(190) <= \<const0>\;
  S00_AXI_rdata(189) <= \<const0>\;
  S00_AXI_rdata(188) <= \<const0>\;
  S00_AXI_rdata(187) <= \<const0>\;
  S00_AXI_rdata(186) <= \<const0>\;
  S00_AXI_rdata(185) <= \<const0>\;
  S00_AXI_rdata(184) <= \<const0>\;
  S00_AXI_rdata(183) <= \<const0>\;
  S00_AXI_rdata(182) <= \<const0>\;
  S00_AXI_rdata(181) <= \<const0>\;
  S00_AXI_rdata(180) <= \<const0>\;
  S00_AXI_rdata(179) <= \<const0>\;
  S00_AXI_rdata(178) <= \<const0>\;
  S00_AXI_rdata(177) <= \<const0>\;
  S00_AXI_rdata(176) <= \<const0>\;
  S00_AXI_rdata(175) <= \<const0>\;
  S00_AXI_rdata(174) <= \<const0>\;
  S00_AXI_rdata(173) <= \<const0>\;
  S00_AXI_rdata(172) <= \<const0>\;
  S00_AXI_rdata(171) <= \<const0>\;
  S00_AXI_rdata(170) <= \<const0>\;
  S00_AXI_rdata(169) <= \<const0>\;
  S00_AXI_rdata(168) <= \<const0>\;
  S00_AXI_rdata(167) <= \<const0>\;
  S00_AXI_rdata(166) <= \<const0>\;
  S00_AXI_rdata(165) <= \<const0>\;
  S00_AXI_rdata(164) <= \<const0>\;
  S00_AXI_rdata(163) <= \<const0>\;
  S00_AXI_rdata(162) <= \<const0>\;
  S00_AXI_rdata(161) <= \<const0>\;
  S00_AXI_rdata(160) <= \<const0>\;
  S00_AXI_rdata(159) <= \<const0>\;
  S00_AXI_rdata(158) <= \<const0>\;
  S00_AXI_rdata(157) <= \<const0>\;
  S00_AXI_rdata(156) <= \<const0>\;
  S00_AXI_rdata(155) <= \<const0>\;
  S00_AXI_rdata(154) <= \<const0>\;
  S00_AXI_rdata(153) <= \<const0>\;
  S00_AXI_rdata(152) <= \<const0>\;
  S00_AXI_rdata(151) <= \<const0>\;
  S00_AXI_rdata(150) <= \<const0>\;
  S00_AXI_rdata(149) <= \<const0>\;
  S00_AXI_rdata(148) <= \<const0>\;
  S00_AXI_rdata(147) <= \<const0>\;
  S00_AXI_rdata(146) <= \<const0>\;
  S00_AXI_rdata(145) <= \<const0>\;
  S00_AXI_rdata(144) <= \<const0>\;
  S00_AXI_rdata(143) <= \<const0>\;
  S00_AXI_rdata(142) <= \<const0>\;
  S00_AXI_rdata(141) <= \<const0>\;
  S00_AXI_rdata(140) <= \<const0>\;
  S00_AXI_rdata(139) <= \<const0>\;
  S00_AXI_rdata(138) <= \<const0>\;
  S00_AXI_rdata(137) <= \<const0>\;
  S00_AXI_rdata(136) <= \<const0>\;
  S00_AXI_rdata(135) <= \<const0>\;
  S00_AXI_rdata(134) <= \<const0>\;
  S00_AXI_rdata(133) <= \<const0>\;
  S00_AXI_rdata(132) <= \<const0>\;
  S00_AXI_rdata(131) <= \<const0>\;
  S00_AXI_rdata(130) <= \<const0>\;
  S00_AXI_rdata(129) <= \<const0>\;
  S00_AXI_rdata(128) <= \<const0>\;
  S00_AXI_rdata(127) <= \<const0>\;
  S00_AXI_rdata(126) <= \<const0>\;
  S00_AXI_rdata(125) <= \<const0>\;
  S00_AXI_rdata(124) <= \<const0>\;
  S00_AXI_rdata(123) <= \<const0>\;
  S00_AXI_rdata(122) <= \<const0>\;
  S00_AXI_rdata(121) <= \<const0>\;
  S00_AXI_rdata(120) <= \<const0>\;
  S00_AXI_rdata(119) <= \<const0>\;
  S00_AXI_rdata(118) <= \<const0>\;
  S00_AXI_rdata(117) <= \<const0>\;
  S00_AXI_rdata(116) <= \<const0>\;
  S00_AXI_rdata(115) <= \<const0>\;
  S00_AXI_rdata(114) <= \<const0>\;
  S00_AXI_rdata(113) <= \<const0>\;
  S00_AXI_rdata(112) <= \<const0>\;
  S00_AXI_rdata(111) <= \<const0>\;
  S00_AXI_rdata(110) <= \<const0>\;
  S00_AXI_rdata(109) <= \<const0>\;
  S00_AXI_rdata(108) <= \<const0>\;
  S00_AXI_rdata(107) <= \<const0>\;
  S00_AXI_rdata(106) <= \<const0>\;
  S00_AXI_rdata(105) <= \<const0>\;
  S00_AXI_rdata(104) <= \<const0>\;
  S00_AXI_rdata(103) <= \<const0>\;
  S00_AXI_rdata(102) <= \<const0>\;
  S00_AXI_rdata(101) <= \<const0>\;
  S00_AXI_rdata(100) <= \<const0>\;
  S00_AXI_rdata(99) <= \<const0>\;
  S00_AXI_rdata(98) <= \<const0>\;
  S00_AXI_rdata(97) <= \<const0>\;
  S00_AXI_rdata(96) <= \<const0>\;
  S00_AXI_rdata(95) <= \<const0>\;
  S00_AXI_rdata(94) <= \<const0>\;
  S00_AXI_rdata(93) <= \<const0>\;
  S00_AXI_rdata(92) <= \<const0>\;
  S00_AXI_rdata(91) <= \<const0>\;
  S00_AXI_rdata(90) <= \<const0>\;
  S00_AXI_rdata(89) <= \<const0>\;
  S00_AXI_rdata(88) <= \<const0>\;
  S00_AXI_rdata(87) <= \<const0>\;
  S00_AXI_rdata(86) <= \<const0>\;
  S00_AXI_rdata(85) <= \<const0>\;
  S00_AXI_rdata(84) <= \<const0>\;
  S00_AXI_rdata(83) <= \<const0>\;
  S00_AXI_rdata(82) <= \<const0>\;
  S00_AXI_rdata(81) <= \<const0>\;
  S00_AXI_rdata(80) <= \<const0>\;
  S00_AXI_rdata(79) <= \<const0>\;
  S00_AXI_rdata(78) <= \<const0>\;
  S00_AXI_rdata(77) <= \<const0>\;
  S00_AXI_rdata(76) <= \<const0>\;
  S00_AXI_rdata(75) <= \<const0>\;
  S00_AXI_rdata(74) <= \<const0>\;
  S00_AXI_rdata(73) <= \<const0>\;
  S00_AXI_rdata(72) <= \<const0>\;
  S00_AXI_rdata(71) <= \<const0>\;
  S00_AXI_rdata(70) <= \<const0>\;
  S00_AXI_rdata(69) <= \<const0>\;
  S00_AXI_rdata(68) <= \<const0>\;
  S00_AXI_rdata(67) <= \<const0>\;
  S00_AXI_rdata(66) <= \<const0>\;
  S00_AXI_rdata(65) <= \<const0>\;
  S00_AXI_rdata(64) <= \<const0>\;
  S00_AXI_rdata(63) <= \<const0>\;
  S00_AXI_rdata(62) <= \<const0>\;
  S00_AXI_rdata(61) <= \<const0>\;
  S00_AXI_rdata(60) <= \<const0>\;
  S00_AXI_rdata(59) <= \<const0>\;
  S00_AXI_rdata(58) <= \<const0>\;
  S00_AXI_rdata(57) <= \<const0>\;
  S00_AXI_rdata(56) <= \<const0>\;
  S00_AXI_rdata(55) <= \<const0>\;
  S00_AXI_rdata(54) <= \<const0>\;
  S00_AXI_rdata(53) <= \<const0>\;
  S00_AXI_rdata(52) <= \<const0>\;
  S00_AXI_rdata(51) <= \<const0>\;
  S00_AXI_rdata(50) <= \<const0>\;
  S00_AXI_rdata(49) <= \<const0>\;
  S00_AXI_rdata(48) <= \<const0>\;
  S00_AXI_rdata(47) <= \<const0>\;
  S00_AXI_rdata(46) <= \<const0>\;
  S00_AXI_rdata(45) <= \<const0>\;
  S00_AXI_rdata(44) <= \<const0>\;
  S00_AXI_rdata(43) <= \<const0>\;
  S00_AXI_rdata(42) <= \<const0>\;
  S00_AXI_rdata(41) <= \<const0>\;
  S00_AXI_rdata(40) <= \<const0>\;
  S00_AXI_rdata(39) <= \<const0>\;
  S00_AXI_rdata(38) <= \<const0>\;
  S00_AXI_rdata(37) <= \<const0>\;
  S00_AXI_rdata(36) <= \<const0>\;
  S00_AXI_rdata(35) <= \<const0>\;
  S00_AXI_rdata(34) <= \<const0>\;
  S00_AXI_rdata(33) <= \<const0>\;
  S00_AXI_rdata(32) <= \<const0>\;
  S00_AXI_rdata(31) <= \<const0>\;
  S00_AXI_rdata(30) <= \<const0>\;
  S00_AXI_rdata(29) <= \<const0>\;
  S00_AXI_rdata(28) <= \<const0>\;
  S00_AXI_rdata(27) <= \<const0>\;
  S00_AXI_rdata(26) <= \<const0>\;
  S00_AXI_rdata(25) <= \<const0>\;
  S00_AXI_rdata(24) <= \<const0>\;
  S00_AXI_rdata(23) <= \<const0>\;
  S00_AXI_rdata(22) <= \<const0>\;
  S00_AXI_rdata(21) <= \<const0>\;
  S00_AXI_rdata(20) <= \<const0>\;
  S00_AXI_rdata(19) <= \<const0>\;
  S00_AXI_rdata(18) <= \<const0>\;
  S00_AXI_rdata(17) <= \<const0>\;
  S00_AXI_rdata(16) <= \<const0>\;
  S00_AXI_rdata(15) <= \<const0>\;
  S00_AXI_rdata(14) <= \<const0>\;
  S00_AXI_rdata(13) <= \<const0>\;
  S00_AXI_rdata(12) <= \<const0>\;
  S00_AXI_rdata(11) <= \<const0>\;
  S00_AXI_rdata(10) <= \<const0>\;
  S00_AXI_rdata(9) <= \<const0>\;
  S00_AXI_rdata(8) <= \<const0>\;
  S00_AXI_rdata(7) <= \<const0>\;
  S00_AXI_rdata(6) <= \<const0>\;
  S00_AXI_rdata(5) <= \<const0>\;
  S00_AXI_rdata(4) <= \<const0>\;
  S00_AXI_rdata(3) <= \<const0>\;
  S00_AXI_rdata(2) <= \<const0>\;
  S00_AXI_rdata(1) <= \<const0>\;
  S00_AXI_rdata(0) <= \<const0>\;
  S00_AXI_rid(3) <= \<const0>\;
  S00_AXI_rid(2) <= \<const0>\;
  S00_AXI_rid(1) <= \<const0>\;
  S00_AXI_rid(0) <= \<const0>\;
  S00_AXI_rlast <= \<const0>\;
  S00_AXI_rresp(1) <= \<const0>\;
  S00_AXI_rresp(0) <= \<const0>\;
  S00_AXI_rvalid <= \<const0>\;
  S00_AXI_wready <= \<const0>\;
  S03_AXI_arready <= \<const0>\;
  S03_AXI_awready <= \<const0>\;
  S03_AXI_bid(3) <= \<const0>\;
  S03_AXI_bid(2) <= \<const0>\;
  S03_AXI_bid(1) <= \<const0>\;
  S03_AXI_bid(0) <= \<const0>\;
  S03_AXI_bresp(1) <= \<const0>\;
  S03_AXI_bresp(0) <= \<const0>\;
  S03_AXI_bvalid <= \<const0>\;
  S03_AXI_rdata(511) <= \<const0>\;
  S03_AXI_rdata(510) <= \<const0>\;
  S03_AXI_rdata(509) <= \<const0>\;
  S03_AXI_rdata(508) <= \<const0>\;
  S03_AXI_rdata(507) <= \<const0>\;
  S03_AXI_rdata(506) <= \<const0>\;
  S03_AXI_rdata(505) <= \<const0>\;
  S03_AXI_rdata(504) <= \<const0>\;
  S03_AXI_rdata(503) <= \<const0>\;
  S03_AXI_rdata(502) <= \<const0>\;
  S03_AXI_rdata(501) <= \<const0>\;
  S03_AXI_rdata(500) <= \<const0>\;
  S03_AXI_rdata(499) <= \<const0>\;
  S03_AXI_rdata(498) <= \<const0>\;
  S03_AXI_rdata(497) <= \<const0>\;
  S03_AXI_rdata(496) <= \<const0>\;
  S03_AXI_rdata(495) <= \<const0>\;
  S03_AXI_rdata(494) <= \<const0>\;
  S03_AXI_rdata(493) <= \<const0>\;
  S03_AXI_rdata(492) <= \<const0>\;
  S03_AXI_rdata(491) <= \<const0>\;
  S03_AXI_rdata(490) <= \<const0>\;
  S03_AXI_rdata(489) <= \<const0>\;
  S03_AXI_rdata(488) <= \<const0>\;
  S03_AXI_rdata(487) <= \<const0>\;
  S03_AXI_rdata(486) <= \<const0>\;
  S03_AXI_rdata(485) <= \<const0>\;
  S03_AXI_rdata(484) <= \<const0>\;
  S03_AXI_rdata(483) <= \<const0>\;
  S03_AXI_rdata(482) <= \<const0>\;
  S03_AXI_rdata(481) <= \<const0>\;
  S03_AXI_rdata(480) <= \<const0>\;
  S03_AXI_rdata(479) <= \<const0>\;
  S03_AXI_rdata(478) <= \<const0>\;
  S03_AXI_rdata(477) <= \<const0>\;
  S03_AXI_rdata(476) <= \<const0>\;
  S03_AXI_rdata(475) <= \<const0>\;
  S03_AXI_rdata(474) <= \<const0>\;
  S03_AXI_rdata(473) <= \<const0>\;
  S03_AXI_rdata(472) <= \<const0>\;
  S03_AXI_rdata(471) <= \<const0>\;
  S03_AXI_rdata(470) <= \<const0>\;
  S03_AXI_rdata(469) <= \<const0>\;
  S03_AXI_rdata(468) <= \<const0>\;
  S03_AXI_rdata(467) <= \<const0>\;
  S03_AXI_rdata(466) <= \<const0>\;
  S03_AXI_rdata(465) <= \<const0>\;
  S03_AXI_rdata(464) <= \<const0>\;
  S03_AXI_rdata(463) <= \<const0>\;
  S03_AXI_rdata(462) <= \<const0>\;
  S03_AXI_rdata(461) <= \<const0>\;
  S03_AXI_rdata(460) <= \<const0>\;
  S03_AXI_rdata(459) <= \<const0>\;
  S03_AXI_rdata(458) <= \<const0>\;
  S03_AXI_rdata(457) <= \<const0>\;
  S03_AXI_rdata(456) <= \<const0>\;
  S03_AXI_rdata(455) <= \<const0>\;
  S03_AXI_rdata(454) <= \<const0>\;
  S03_AXI_rdata(453) <= \<const0>\;
  S03_AXI_rdata(452) <= \<const0>\;
  S03_AXI_rdata(451) <= \<const0>\;
  S03_AXI_rdata(450) <= \<const0>\;
  S03_AXI_rdata(449) <= \<const0>\;
  S03_AXI_rdata(448) <= \<const0>\;
  S03_AXI_rdata(447) <= \<const0>\;
  S03_AXI_rdata(446) <= \<const0>\;
  S03_AXI_rdata(445) <= \<const0>\;
  S03_AXI_rdata(444) <= \<const0>\;
  S03_AXI_rdata(443) <= \<const0>\;
  S03_AXI_rdata(442) <= \<const0>\;
  S03_AXI_rdata(441) <= \<const0>\;
  S03_AXI_rdata(440) <= \<const0>\;
  S03_AXI_rdata(439) <= \<const0>\;
  S03_AXI_rdata(438) <= \<const0>\;
  S03_AXI_rdata(437) <= \<const0>\;
  S03_AXI_rdata(436) <= \<const0>\;
  S03_AXI_rdata(435) <= \<const0>\;
  S03_AXI_rdata(434) <= \<const0>\;
  S03_AXI_rdata(433) <= \<const0>\;
  S03_AXI_rdata(432) <= \<const0>\;
  S03_AXI_rdata(431) <= \<const0>\;
  S03_AXI_rdata(430) <= \<const0>\;
  S03_AXI_rdata(429) <= \<const0>\;
  S03_AXI_rdata(428) <= \<const0>\;
  S03_AXI_rdata(427) <= \<const0>\;
  S03_AXI_rdata(426) <= \<const0>\;
  S03_AXI_rdata(425) <= \<const0>\;
  S03_AXI_rdata(424) <= \<const0>\;
  S03_AXI_rdata(423) <= \<const0>\;
  S03_AXI_rdata(422) <= \<const0>\;
  S03_AXI_rdata(421) <= \<const0>\;
  S03_AXI_rdata(420) <= \<const0>\;
  S03_AXI_rdata(419) <= \<const0>\;
  S03_AXI_rdata(418) <= \<const0>\;
  S03_AXI_rdata(417) <= \<const0>\;
  S03_AXI_rdata(416) <= \<const0>\;
  S03_AXI_rdata(415) <= \<const0>\;
  S03_AXI_rdata(414) <= \<const0>\;
  S03_AXI_rdata(413) <= \<const0>\;
  S03_AXI_rdata(412) <= \<const0>\;
  S03_AXI_rdata(411) <= \<const0>\;
  S03_AXI_rdata(410) <= \<const0>\;
  S03_AXI_rdata(409) <= \<const0>\;
  S03_AXI_rdata(408) <= \<const0>\;
  S03_AXI_rdata(407) <= \<const0>\;
  S03_AXI_rdata(406) <= \<const0>\;
  S03_AXI_rdata(405) <= \<const0>\;
  S03_AXI_rdata(404) <= \<const0>\;
  S03_AXI_rdata(403) <= \<const0>\;
  S03_AXI_rdata(402) <= \<const0>\;
  S03_AXI_rdata(401) <= \<const0>\;
  S03_AXI_rdata(400) <= \<const0>\;
  S03_AXI_rdata(399) <= \<const0>\;
  S03_AXI_rdata(398) <= \<const0>\;
  S03_AXI_rdata(397) <= \<const0>\;
  S03_AXI_rdata(396) <= \<const0>\;
  S03_AXI_rdata(395) <= \<const0>\;
  S03_AXI_rdata(394) <= \<const0>\;
  S03_AXI_rdata(393) <= \<const0>\;
  S03_AXI_rdata(392) <= \<const0>\;
  S03_AXI_rdata(391) <= \<const0>\;
  S03_AXI_rdata(390) <= \<const0>\;
  S03_AXI_rdata(389) <= \<const0>\;
  S03_AXI_rdata(388) <= \<const0>\;
  S03_AXI_rdata(387) <= \<const0>\;
  S03_AXI_rdata(386) <= \<const0>\;
  S03_AXI_rdata(385) <= \<const0>\;
  S03_AXI_rdata(384) <= \<const0>\;
  S03_AXI_rdata(383) <= \<const0>\;
  S03_AXI_rdata(382) <= \<const0>\;
  S03_AXI_rdata(381) <= \<const0>\;
  S03_AXI_rdata(380) <= \<const0>\;
  S03_AXI_rdata(379) <= \<const0>\;
  S03_AXI_rdata(378) <= \<const0>\;
  S03_AXI_rdata(377) <= \<const0>\;
  S03_AXI_rdata(376) <= \<const0>\;
  S03_AXI_rdata(375) <= \<const0>\;
  S03_AXI_rdata(374) <= \<const0>\;
  S03_AXI_rdata(373) <= \<const0>\;
  S03_AXI_rdata(372) <= \<const0>\;
  S03_AXI_rdata(371) <= \<const0>\;
  S03_AXI_rdata(370) <= \<const0>\;
  S03_AXI_rdata(369) <= \<const0>\;
  S03_AXI_rdata(368) <= \<const0>\;
  S03_AXI_rdata(367) <= \<const0>\;
  S03_AXI_rdata(366) <= \<const0>\;
  S03_AXI_rdata(365) <= \<const0>\;
  S03_AXI_rdata(364) <= \<const0>\;
  S03_AXI_rdata(363) <= \<const0>\;
  S03_AXI_rdata(362) <= \<const0>\;
  S03_AXI_rdata(361) <= \<const0>\;
  S03_AXI_rdata(360) <= \<const0>\;
  S03_AXI_rdata(359) <= \<const0>\;
  S03_AXI_rdata(358) <= \<const0>\;
  S03_AXI_rdata(357) <= \<const0>\;
  S03_AXI_rdata(356) <= \<const0>\;
  S03_AXI_rdata(355) <= \<const0>\;
  S03_AXI_rdata(354) <= \<const0>\;
  S03_AXI_rdata(353) <= \<const0>\;
  S03_AXI_rdata(352) <= \<const0>\;
  S03_AXI_rdata(351) <= \<const0>\;
  S03_AXI_rdata(350) <= \<const0>\;
  S03_AXI_rdata(349) <= \<const0>\;
  S03_AXI_rdata(348) <= \<const0>\;
  S03_AXI_rdata(347) <= \<const0>\;
  S03_AXI_rdata(346) <= \<const0>\;
  S03_AXI_rdata(345) <= \<const0>\;
  S03_AXI_rdata(344) <= \<const0>\;
  S03_AXI_rdata(343) <= \<const0>\;
  S03_AXI_rdata(342) <= \<const0>\;
  S03_AXI_rdata(341) <= \<const0>\;
  S03_AXI_rdata(340) <= \<const0>\;
  S03_AXI_rdata(339) <= \<const0>\;
  S03_AXI_rdata(338) <= \<const0>\;
  S03_AXI_rdata(337) <= \<const0>\;
  S03_AXI_rdata(336) <= \<const0>\;
  S03_AXI_rdata(335) <= \<const0>\;
  S03_AXI_rdata(334) <= \<const0>\;
  S03_AXI_rdata(333) <= \<const0>\;
  S03_AXI_rdata(332) <= \<const0>\;
  S03_AXI_rdata(331) <= \<const0>\;
  S03_AXI_rdata(330) <= \<const0>\;
  S03_AXI_rdata(329) <= \<const0>\;
  S03_AXI_rdata(328) <= \<const0>\;
  S03_AXI_rdata(327) <= \<const0>\;
  S03_AXI_rdata(326) <= \<const0>\;
  S03_AXI_rdata(325) <= \<const0>\;
  S03_AXI_rdata(324) <= \<const0>\;
  S03_AXI_rdata(323) <= \<const0>\;
  S03_AXI_rdata(322) <= \<const0>\;
  S03_AXI_rdata(321) <= \<const0>\;
  S03_AXI_rdata(320) <= \<const0>\;
  S03_AXI_rdata(319) <= \<const0>\;
  S03_AXI_rdata(318) <= \<const0>\;
  S03_AXI_rdata(317) <= \<const0>\;
  S03_AXI_rdata(316) <= \<const0>\;
  S03_AXI_rdata(315) <= \<const0>\;
  S03_AXI_rdata(314) <= \<const0>\;
  S03_AXI_rdata(313) <= \<const0>\;
  S03_AXI_rdata(312) <= \<const0>\;
  S03_AXI_rdata(311) <= \<const0>\;
  S03_AXI_rdata(310) <= \<const0>\;
  S03_AXI_rdata(309) <= \<const0>\;
  S03_AXI_rdata(308) <= \<const0>\;
  S03_AXI_rdata(307) <= \<const0>\;
  S03_AXI_rdata(306) <= \<const0>\;
  S03_AXI_rdata(305) <= \<const0>\;
  S03_AXI_rdata(304) <= \<const0>\;
  S03_AXI_rdata(303) <= \<const0>\;
  S03_AXI_rdata(302) <= \<const0>\;
  S03_AXI_rdata(301) <= \<const0>\;
  S03_AXI_rdata(300) <= \<const0>\;
  S03_AXI_rdata(299) <= \<const0>\;
  S03_AXI_rdata(298) <= \<const0>\;
  S03_AXI_rdata(297) <= \<const0>\;
  S03_AXI_rdata(296) <= \<const0>\;
  S03_AXI_rdata(295) <= \<const0>\;
  S03_AXI_rdata(294) <= \<const0>\;
  S03_AXI_rdata(293) <= \<const0>\;
  S03_AXI_rdata(292) <= \<const0>\;
  S03_AXI_rdata(291) <= \<const0>\;
  S03_AXI_rdata(290) <= \<const0>\;
  S03_AXI_rdata(289) <= \<const0>\;
  S03_AXI_rdata(288) <= \<const0>\;
  S03_AXI_rdata(287) <= \<const0>\;
  S03_AXI_rdata(286) <= \<const0>\;
  S03_AXI_rdata(285) <= \<const0>\;
  S03_AXI_rdata(284) <= \<const0>\;
  S03_AXI_rdata(283) <= \<const0>\;
  S03_AXI_rdata(282) <= \<const0>\;
  S03_AXI_rdata(281) <= \<const0>\;
  S03_AXI_rdata(280) <= \<const0>\;
  S03_AXI_rdata(279) <= \<const0>\;
  S03_AXI_rdata(278) <= \<const0>\;
  S03_AXI_rdata(277) <= \<const0>\;
  S03_AXI_rdata(276) <= \<const0>\;
  S03_AXI_rdata(275) <= \<const0>\;
  S03_AXI_rdata(274) <= \<const0>\;
  S03_AXI_rdata(273) <= \<const0>\;
  S03_AXI_rdata(272) <= \<const0>\;
  S03_AXI_rdata(271) <= \<const0>\;
  S03_AXI_rdata(270) <= \<const0>\;
  S03_AXI_rdata(269) <= \<const0>\;
  S03_AXI_rdata(268) <= \<const0>\;
  S03_AXI_rdata(267) <= \<const0>\;
  S03_AXI_rdata(266) <= \<const0>\;
  S03_AXI_rdata(265) <= \<const0>\;
  S03_AXI_rdata(264) <= \<const0>\;
  S03_AXI_rdata(263) <= \<const0>\;
  S03_AXI_rdata(262) <= \<const0>\;
  S03_AXI_rdata(261) <= \<const0>\;
  S03_AXI_rdata(260) <= \<const0>\;
  S03_AXI_rdata(259) <= \<const0>\;
  S03_AXI_rdata(258) <= \<const0>\;
  S03_AXI_rdata(257) <= \<const0>\;
  S03_AXI_rdata(256) <= \<const0>\;
  S03_AXI_rdata(255) <= \<const0>\;
  S03_AXI_rdata(254) <= \<const0>\;
  S03_AXI_rdata(253) <= \<const0>\;
  S03_AXI_rdata(252) <= \<const0>\;
  S03_AXI_rdata(251) <= \<const0>\;
  S03_AXI_rdata(250) <= \<const0>\;
  S03_AXI_rdata(249) <= \<const0>\;
  S03_AXI_rdata(248) <= \<const0>\;
  S03_AXI_rdata(247) <= \<const0>\;
  S03_AXI_rdata(246) <= \<const0>\;
  S03_AXI_rdata(245) <= \<const0>\;
  S03_AXI_rdata(244) <= \<const0>\;
  S03_AXI_rdata(243) <= \<const0>\;
  S03_AXI_rdata(242) <= \<const0>\;
  S03_AXI_rdata(241) <= \<const0>\;
  S03_AXI_rdata(240) <= \<const0>\;
  S03_AXI_rdata(239) <= \<const0>\;
  S03_AXI_rdata(238) <= \<const0>\;
  S03_AXI_rdata(237) <= \<const0>\;
  S03_AXI_rdata(236) <= \<const0>\;
  S03_AXI_rdata(235) <= \<const0>\;
  S03_AXI_rdata(234) <= \<const0>\;
  S03_AXI_rdata(233) <= \<const0>\;
  S03_AXI_rdata(232) <= \<const0>\;
  S03_AXI_rdata(231) <= \<const0>\;
  S03_AXI_rdata(230) <= \<const0>\;
  S03_AXI_rdata(229) <= \<const0>\;
  S03_AXI_rdata(228) <= \<const0>\;
  S03_AXI_rdata(227) <= \<const0>\;
  S03_AXI_rdata(226) <= \<const0>\;
  S03_AXI_rdata(225) <= \<const0>\;
  S03_AXI_rdata(224) <= \<const0>\;
  S03_AXI_rdata(223) <= \<const0>\;
  S03_AXI_rdata(222) <= \<const0>\;
  S03_AXI_rdata(221) <= \<const0>\;
  S03_AXI_rdata(220) <= \<const0>\;
  S03_AXI_rdata(219) <= \<const0>\;
  S03_AXI_rdata(218) <= \<const0>\;
  S03_AXI_rdata(217) <= \<const0>\;
  S03_AXI_rdata(216) <= \<const0>\;
  S03_AXI_rdata(215) <= \<const0>\;
  S03_AXI_rdata(214) <= \<const0>\;
  S03_AXI_rdata(213) <= \<const0>\;
  S03_AXI_rdata(212) <= \<const0>\;
  S03_AXI_rdata(211) <= \<const0>\;
  S03_AXI_rdata(210) <= \<const0>\;
  S03_AXI_rdata(209) <= \<const0>\;
  S03_AXI_rdata(208) <= \<const0>\;
  S03_AXI_rdata(207) <= \<const0>\;
  S03_AXI_rdata(206) <= \<const0>\;
  S03_AXI_rdata(205) <= \<const0>\;
  S03_AXI_rdata(204) <= \<const0>\;
  S03_AXI_rdata(203) <= \<const0>\;
  S03_AXI_rdata(202) <= \<const0>\;
  S03_AXI_rdata(201) <= \<const0>\;
  S03_AXI_rdata(200) <= \<const0>\;
  S03_AXI_rdata(199) <= \<const0>\;
  S03_AXI_rdata(198) <= \<const0>\;
  S03_AXI_rdata(197) <= \<const0>\;
  S03_AXI_rdata(196) <= \<const0>\;
  S03_AXI_rdata(195) <= \<const0>\;
  S03_AXI_rdata(194) <= \<const0>\;
  S03_AXI_rdata(193) <= \<const0>\;
  S03_AXI_rdata(192) <= \<const0>\;
  S03_AXI_rdata(191) <= \<const0>\;
  S03_AXI_rdata(190) <= \<const0>\;
  S03_AXI_rdata(189) <= \<const0>\;
  S03_AXI_rdata(188) <= \<const0>\;
  S03_AXI_rdata(187) <= \<const0>\;
  S03_AXI_rdata(186) <= \<const0>\;
  S03_AXI_rdata(185) <= \<const0>\;
  S03_AXI_rdata(184) <= \<const0>\;
  S03_AXI_rdata(183) <= \<const0>\;
  S03_AXI_rdata(182) <= \<const0>\;
  S03_AXI_rdata(181) <= \<const0>\;
  S03_AXI_rdata(180) <= \<const0>\;
  S03_AXI_rdata(179) <= \<const0>\;
  S03_AXI_rdata(178) <= \<const0>\;
  S03_AXI_rdata(177) <= \<const0>\;
  S03_AXI_rdata(176) <= \<const0>\;
  S03_AXI_rdata(175) <= \<const0>\;
  S03_AXI_rdata(174) <= \<const0>\;
  S03_AXI_rdata(173) <= \<const0>\;
  S03_AXI_rdata(172) <= \<const0>\;
  S03_AXI_rdata(171) <= \<const0>\;
  S03_AXI_rdata(170) <= \<const0>\;
  S03_AXI_rdata(169) <= \<const0>\;
  S03_AXI_rdata(168) <= \<const0>\;
  S03_AXI_rdata(167) <= \<const0>\;
  S03_AXI_rdata(166) <= \<const0>\;
  S03_AXI_rdata(165) <= \<const0>\;
  S03_AXI_rdata(164) <= \<const0>\;
  S03_AXI_rdata(163) <= \<const0>\;
  S03_AXI_rdata(162) <= \<const0>\;
  S03_AXI_rdata(161) <= \<const0>\;
  S03_AXI_rdata(160) <= \<const0>\;
  S03_AXI_rdata(159) <= \<const0>\;
  S03_AXI_rdata(158) <= \<const0>\;
  S03_AXI_rdata(157) <= \<const0>\;
  S03_AXI_rdata(156) <= \<const0>\;
  S03_AXI_rdata(155) <= \<const0>\;
  S03_AXI_rdata(154) <= \<const0>\;
  S03_AXI_rdata(153) <= \<const0>\;
  S03_AXI_rdata(152) <= \<const0>\;
  S03_AXI_rdata(151) <= \<const0>\;
  S03_AXI_rdata(150) <= \<const0>\;
  S03_AXI_rdata(149) <= \<const0>\;
  S03_AXI_rdata(148) <= \<const0>\;
  S03_AXI_rdata(147) <= \<const0>\;
  S03_AXI_rdata(146) <= \<const0>\;
  S03_AXI_rdata(145) <= \<const0>\;
  S03_AXI_rdata(144) <= \<const0>\;
  S03_AXI_rdata(143) <= \<const0>\;
  S03_AXI_rdata(142) <= \<const0>\;
  S03_AXI_rdata(141) <= \<const0>\;
  S03_AXI_rdata(140) <= \<const0>\;
  S03_AXI_rdata(139) <= \<const0>\;
  S03_AXI_rdata(138) <= \<const0>\;
  S03_AXI_rdata(137) <= \<const0>\;
  S03_AXI_rdata(136) <= \<const0>\;
  S03_AXI_rdata(135) <= \<const0>\;
  S03_AXI_rdata(134) <= \<const0>\;
  S03_AXI_rdata(133) <= \<const0>\;
  S03_AXI_rdata(132) <= \<const0>\;
  S03_AXI_rdata(131) <= \<const0>\;
  S03_AXI_rdata(130) <= \<const0>\;
  S03_AXI_rdata(129) <= \<const0>\;
  S03_AXI_rdata(128) <= \<const0>\;
  S03_AXI_rdata(127) <= \<const0>\;
  S03_AXI_rdata(126) <= \<const0>\;
  S03_AXI_rdata(125) <= \<const0>\;
  S03_AXI_rdata(124) <= \<const0>\;
  S03_AXI_rdata(123) <= \<const0>\;
  S03_AXI_rdata(122) <= \<const0>\;
  S03_AXI_rdata(121) <= \<const0>\;
  S03_AXI_rdata(120) <= \<const0>\;
  S03_AXI_rdata(119) <= \<const0>\;
  S03_AXI_rdata(118) <= \<const0>\;
  S03_AXI_rdata(117) <= \<const0>\;
  S03_AXI_rdata(116) <= \<const0>\;
  S03_AXI_rdata(115) <= \<const0>\;
  S03_AXI_rdata(114) <= \<const0>\;
  S03_AXI_rdata(113) <= \<const0>\;
  S03_AXI_rdata(112) <= \<const0>\;
  S03_AXI_rdata(111) <= \<const0>\;
  S03_AXI_rdata(110) <= \<const0>\;
  S03_AXI_rdata(109) <= \<const0>\;
  S03_AXI_rdata(108) <= \<const0>\;
  S03_AXI_rdata(107) <= \<const0>\;
  S03_AXI_rdata(106) <= \<const0>\;
  S03_AXI_rdata(105) <= \<const0>\;
  S03_AXI_rdata(104) <= \<const0>\;
  S03_AXI_rdata(103) <= \<const0>\;
  S03_AXI_rdata(102) <= \<const0>\;
  S03_AXI_rdata(101) <= \<const0>\;
  S03_AXI_rdata(100) <= \<const0>\;
  S03_AXI_rdata(99) <= \<const0>\;
  S03_AXI_rdata(98) <= \<const0>\;
  S03_AXI_rdata(97) <= \<const0>\;
  S03_AXI_rdata(96) <= \<const0>\;
  S03_AXI_rdata(95) <= \<const0>\;
  S03_AXI_rdata(94) <= \<const0>\;
  S03_AXI_rdata(93) <= \<const0>\;
  S03_AXI_rdata(92) <= \<const0>\;
  S03_AXI_rdata(91) <= \<const0>\;
  S03_AXI_rdata(90) <= \<const0>\;
  S03_AXI_rdata(89) <= \<const0>\;
  S03_AXI_rdata(88) <= \<const0>\;
  S03_AXI_rdata(87) <= \<const0>\;
  S03_AXI_rdata(86) <= \<const0>\;
  S03_AXI_rdata(85) <= \<const0>\;
  S03_AXI_rdata(84) <= \<const0>\;
  S03_AXI_rdata(83) <= \<const0>\;
  S03_AXI_rdata(82) <= \<const0>\;
  S03_AXI_rdata(81) <= \<const0>\;
  S03_AXI_rdata(80) <= \<const0>\;
  S03_AXI_rdata(79) <= \<const0>\;
  S03_AXI_rdata(78) <= \<const0>\;
  S03_AXI_rdata(77) <= \<const0>\;
  S03_AXI_rdata(76) <= \<const0>\;
  S03_AXI_rdata(75) <= \<const0>\;
  S03_AXI_rdata(74) <= \<const0>\;
  S03_AXI_rdata(73) <= \<const0>\;
  S03_AXI_rdata(72) <= \<const0>\;
  S03_AXI_rdata(71) <= \<const0>\;
  S03_AXI_rdata(70) <= \<const0>\;
  S03_AXI_rdata(69) <= \<const0>\;
  S03_AXI_rdata(68) <= \<const0>\;
  S03_AXI_rdata(67) <= \<const0>\;
  S03_AXI_rdata(66) <= \<const0>\;
  S03_AXI_rdata(65) <= \<const0>\;
  S03_AXI_rdata(64) <= \<const0>\;
  S03_AXI_rdata(63) <= \<const0>\;
  S03_AXI_rdata(62) <= \<const0>\;
  S03_AXI_rdata(61) <= \<const0>\;
  S03_AXI_rdata(60) <= \<const0>\;
  S03_AXI_rdata(59) <= \<const0>\;
  S03_AXI_rdata(58) <= \<const0>\;
  S03_AXI_rdata(57) <= \<const0>\;
  S03_AXI_rdata(56) <= \<const0>\;
  S03_AXI_rdata(55) <= \<const0>\;
  S03_AXI_rdata(54) <= \<const0>\;
  S03_AXI_rdata(53) <= \<const0>\;
  S03_AXI_rdata(52) <= \<const0>\;
  S03_AXI_rdata(51) <= \<const0>\;
  S03_AXI_rdata(50) <= \<const0>\;
  S03_AXI_rdata(49) <= \<const0>\;
  S03_AXI_rdata(48) <= \<const0>\;
  S03_AXI_rdata(47) <= \<const0>\;
  S03_AXI_rdata(46) <= \<const0>\;
  S03_AXI_rdata(45) <= \<const0>\;
  S03_AXI_rdata(44) <= \<const0>\;
  S03_AXI_rdata(43) <= \<const0>\;
  S03_AXI_rdata(42) <= \<const0>\;
  S03_AXI_rdata(41) <= \<const0>\;
  S03_AXI_rdata(40) <= \<const0>\;
  S03_AXI_rdata(39) <= \<const0>\;
  S03_AXI_rdata(38) <= \<const0>\;
  S03_AXI_rdata(37) <= \<const0>\;
  S03_AXI_rdata(36) <= \<const0>\;
  S03_AXI_rdata(35) <= \<const0>\;
  S03_AXI_rdata(34) <= \<const0>\;
  S03_AXI_rdata(33) <= \<const0>\;
  S03_AXI_rdata(32) <= \<const0>\;
  S03_AXI_rdata(31) <= \<const0>\;
  S03_AXI_rdata(30) <= \<const0>\;
  S03_AXI_rdata(29) <= \<const0>\;
  S03_AXI_rdata(28) <= \<const0>\;
  S03_AXI_rdata(27) <= \<const0>\;
  S03_AXI_rdata(26) <= \<const0>\;
  S03_AXI_rdata(25) <= \<const0>\;
  S03_AXI_rdata(24) <= \<const0>\;
  S03_AXI_rdata(23) <= \<const0>\;
  S03_AXI_rdata(22) <= \<const0>\;
  S03_AXI_rdata(21) <= \<const0>\;
  S03_AXI_rdata(20) <= \<const0>\;
  S03_AXI_rdata(19) <= \<const0>\;
  S03_AXI_rdata(18) <= \<const0>\;
  S03_AXI_rdata(17) <= \<const0>\;
  S03_AXI_rdata(16) <= \<const0>\;
  S03_AXI_rdata(15) <= \<const0>\;
  S03_AXI_rdata(14) <= \<const0>\;
  S03_AXI_rdata(13) <= \<const0>\;
  S03_AXI_rdata(12) <= \<const0>\;
  S03_AXI_rdata(11) <= \<const0>\;
  S03_AXI_rdata(10) <= \<const0>\;
  S03_AXI_rdata(9) <= \<const0>\;
  S03_AXI_rdata(8) <= \<const0>\;
  S03_AXI_rdata(7) <= \<const0>\;
  S03_AXI_rdata(6) <= \<const0>\;
  S03_AXI_rdata(5) <= \<const0>\;
  S03_AXI_rdata(4) <= \<const0>\;
  S03_AXI_rdata(3) <= \<const0>\;
  S03_AXI_rdata(2) <= \<const0>\;
  S03_AXI_rdata(1) <= \<const0>\;
  S03_AXI_rdata(0) <= \<const0>\;
  S03_AXI_rid(3) <= \<const0>\;
  S03_AXI_rid(2) <= \<const0>\;
  S03_AXI_rid(1) <= \<const0>\;
  S03_AXI_rid(0) <= \<const0>\;
  S03_AXI_rlast <= \<const0>\;
  S03_AXI_rresp(1) <= \<const0>\;
  S03_AXI_rresp(0) <= \<const0>\;
  S03_AXI_rvalid <= \<const0>\;
  S03_AXI_wready <= \<const0>\;
  ddr4_mem00_ui_clk <= \<const0>\;
  ddr4_mem01_ui_clk <= \^ddr4_mem01_ui_clk\;
  ddr4_mem02_ui_clk <= \<const0>\;
GND: unisim.vcomponents.GND
     port map (
      G => \<const0>\
    );
interconnect: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_interconnect_imp_6HQKUY
     port map (
      M00_AXI_araddr(33 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARADDR(33 downto 0),
      M00_AXI_arburst(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARBURST(1 downto 0),
      M00_AXI_arcache(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARCACHE(3 downto 0),
      M00_AXI_arlen(7 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARLEN(7 downto 0),
      M00_AXI_arlock(0) => interconnect_DDR4_MEM01_M00_AXI_ARLOCK,
      M00_AXI_arprot(2 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARPROT(2 downto 0),
      M00_AXI_arqos(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARQOS(3 downto 0),
      M00_AXI_arready => M00_AXI_arready,
      M00_AXI_arregion(3 downto 0) => M00_AXI_arregion(3 downto 0),
      M00_AXI_arsize(2 downto 0) => M00_AXI_arsize(2 downto 0),
      M00_AXI_arvalid => interconnect_DDR4_MEM01_M00_AXI_ARVALID,
      M00_AXI_awaddr(33 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWADDR(33 downto 0),
      M00_AXI_awburst(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWBURST(1 downto 0),
      M00_AXI_awcache(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWCACHE(3 downto 0),
      M00_AXI_awlen(7 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWLEN(7 downto 0),
      M00_AXI_awlock(0) => interconnect_DDR4_MEM01_M00_AXI_AWLOCK,
      M00_AXI_awprot(2 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWPROT(2 downto 0),
      M00_AXI_awqos(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWQOS(3 downto 0),
      M00_AXI_awready => M00_AXI_awready,
      M00_AXI_awregion(3 downto 0) => M00_AXI_awregion(3 downto 0),
      M00_AXI_awsize(2 downto 0) => M00_AXI_awsize(2 downto 0),
      M00_AXI_awvalid => interconnect_DDR4_MEM01_M00_AXI_AWVALID,
      M00_AXI_bready => interconnect_DDR4_MEM01_M00_AXI_BREADY,
      M00_AXI_bresp(1 downto 0) => M00_AXI_bresp(1 downto 0),
      M00_AXI_bvalid => M00_AXI_bvalid,
      M00_AXI_rdata(511 downto 0) => M00_AXI_rdata(511 downto 0),
      M00_AXI_rlast => M00_AXI_rlast,
      M00_AXI_rready => interconnect_DDR4_MEM01_M00_AXI_RREADY,
      M00_AXI_rresp(1 downto 0) => M00_AXI_rresp(1 downto 0),
      M00_AXI_rvalid => M00_AXI_rvalid,
      M00_AXI_wdata(511 downto 0) => interconnect_DDR4_MEM01_M00_AXI_WDATA(511 downto 0),
      M00_AXI_wlast => interconnect_DDR4_MEM01_M00_AXI_WLAST,
      M00_AXI_wready => M00_AXI_wready,
      M00_AXI_wstrb(63 downto 0) => interconnect_DDR4_MEM01_M00_AXI_WSTRB(63 downto 0),
      M00_AXI_wvalid => interconnect_DDR4_MEM01_M00_AXI_WVALID,
      S01_AXI_araddr(38 downto 0) => S01_AXI_araddr(38 downto 0),
      S01_AXI_arburst(1 downto 0) => S01_AXI_arburst(1 downto 0),
      S01_AXI_arcache(3 downto 0) => S01_AXI_arcache(3 downto 0),
      S01_AXI_arid(3 downto 0) => S01_AXI_arid(3 downto 0),
      S01_AXI_arlen(7 downto 0) => S01_AXI_arlen(7 downto 0),
      S01_AXI_arlock(0) => S01_AXI_arlock(0),
      S01_AXI_arprot(2 downto 0) => S01_AXI_arprot(2 downto 0),
      S01_AXI_arqos(3 downto 0) => S01_AXI_arqos(3 downto 0),
      S01_AXI_arready => S01_AXI_arready,
      S01_AXI_arregion(3 downto 0) => S01_AXI_arregion(3 downto 0),
      S01_AXI_arvalid => S01_AXI_arvalid,
      S01_AXI_awaddr(38 downto 0) => S01_AXI_awaddr(38 downto 0),
      S01_AXI_awburst(1 downto 0) => S01_AXI_awburst(1 downto 0),
      S01_AXI_awcache(3 downto 0) => S01_AXI_awcache(3 downto 0),
      S01_AXI_awid(3 downto 0) => S01_AXI_awid(3 downto 0),
      S01_AXI_awlen(7 downto 0) => S01_AXI_awlen(7 downto 0),
      S01_AXI_awlock(0) => S01_AXI_awlock(0),
      S01_AXI_awprot(2 downto 0) => S01_AXI_awprot(2 downto 0),
      S01_AXI_awqos(3 downto 0) => S01_AXI_awqos(3 downto 0),
      S01_AXI_awready => S01_AXI_awready,
      S01_AXI_awregion(3 downto 0) => S01_AXI_awregion(3 downto 0),
      S01_AXI_awvalid => S01_AXI_awvalid,
      S01_AXI_bid(3 downto 0) => S01_AXI_bid(3 downto 0),
      S01_AXI_bready => S01_AXI_bready,
      S01_AXI_bresp(1 downto 0) => S01_AXI_bresp(1 downto 0),
      S01_AXI_bvalid => S01_AXI_bvalid,
      S01_AXI_rdata(511 downto 0) => S01_AXI_rdata(511 downto 0),
      S01_AXI_rid(3 downto 0) => S01_AXI_rid(3 downto 0),
      S01_AXI_rlast => S01_AXI_rlast,
      S01_AXI_rready => S01_AXI_rready,
      S01_AXI_rresp(1 downto 0) => S01_AXI_rresp(1 downto 0),
      S01_AXI_rvalid => S01_AXI_rvalid,
      S01_AXI_wdata(511 downto 0) => S01_AXI_wdata(511 downto 0),
      S01_AXI_wlast => S01_AXI_wlast,
      S01_AXI_wready => S01_AXI_wready,
      S01_AXI_wstrb(63 downto 0) => S01_AXI_wstrb(63 downto 0),
      S01_AXI_wvalid => S01_AXI_wvalid,
      S02_AXI_araddr(38 downto 0) => S02_AXI_araddr(38 downto 0),
      S02_AXI_arburst(1 downto 0) => S02_AXI_arburst(1 downto 0),
      S02_AXI_arcache(3 downto 0) => S02_AXI_arcache(3 downto 0),
      S02_AXI_arid(3 downto 0) => S02_AXI_arid(3 downto 0),
      S02_AXI_arlen(7 downto 0) => S02_AXI_arlen(7 downto 0),
      S02_AXI_arlock(0) => S02_AXI_arlock(0),
      S02_AXI_arprot(2 downto 0) => S02_AXI_arprot(2 downto 0),
      S02_AXI_arqos(3 downto 0) => S02_AXI_arqos(3 downto 0),
      S02_AXI_arready => S02_AXI_arready,
      S02_AXI_arregion(3 downto 0) => S02_AXI_arregion(3 downto 0),
      S02_AXI_arvalid => S02_AXI_arvalid,
      S02_AXI_awaddr(38 downto 0) => S02_AXI_awaddr(38 downto 0),
      S02_AXI_awburst(1 downto 0) => S02_AXI_awburst(1 downto 0),
      S02_AXI_awcache(3 downto 0) => S02_AXI_awcache(3 downto 0),
      S02_AXI_awid(3 downto 0) => S02_AXI_awid(3 downto 0),
      S02_AXI_awlen(7 downto 0) => S02_AXI_awlen(7 downto 0),
      S02_AXI_awlock(0) => S02_AXI_awlock(0),
      S02_AXI_awprot(2 downto 0) => S02_AXI_awprot(2 downto 0),
      S02_AXI_awqos(3 downto 0) => S02_AXI_awqos(3 downto 0),
      S02_AXI_awready => S02_AXI_awready,
      S02_AXI_awregion(3 downto 0) => S02_AXI_awregion(3 downto 0),
      S02_AXI_awvalid => S02_AXI_awvalid,
      S02_AXI_bid(3 downto 0) => S02_AXI_bid(3 downto 0),
      S02_AXI_bready => S02_AXI_bready,
      S02_AXI_bresp(1 downto 0) => S02_AXI_bresp(1 downto 0),
      S02_AXI_bvalid => S02_AXI_bvalid,
      S02_AXI_rdata(511 downto 0) => S02_AXI_rdata(511 downto 0),
      S02_AXI_rid(3 downto 0) => S02_AXI_rid(3 downto 0),
      S02_AXI_rlast => S02_AXI_rlast,
      S02_AXI_rready => S02_AXI_rready,
      S02_AXI_rresp(1 downto 0) => S02_AXI_rresp(1 downto 0),
      S02_AXI_rvalid => S02_AXI_rvalid,
      S02_AXI_wdata(511 downto 0) => S02_AXI_wdata(511 downto 0),
      S02_AXI_wlast => S02_AXI_wlast,
      S02_AXI_wready => S02_AXI_wready,
      S02_AXI_wstrb(63 downto 0) => S02_AXI_wstrb(63 downto 0),
      S02_AXI_wvalid => S02_AXI_wvalid,
      S04_AXI_araddr(38 downto 0) => S04_AXI_araddr(38 downto 0),
      S04_AXI_arcache(3 downto 0) => S04_AXI_arcache(3 downto 0),
      S04_AXI_arlen(7 downto 0) => S04_AXI_arlen(7 downto 0),
      S04_AXI_arlock(0) => S04_AXI_arlock(0),
      S04_AXI_arprot(2 downto 0) => S04_AXI_arprot(2 downto 0),
      S04_AXI_arqos(3 downto 0) => S04_AXI_arqos(3 downto 0),
      S04_AXI_arready => S04_AXI_arready,
      S04_AXI_arregion(3 downto 0) => S04_AXI_arregion(3 downto 0),
      S04_AXI_arvalid => S04_AXI_arvalid,
      S04_AXI_awaddr(38 downto 0) => S04_AXI_awaddr(38 downto 0),
      S04_AXI_awcache(3 downto 0) => S04_AXI_awcache(3 downto 0),
      S04_AXI_awlen(7 downto 0) => S04_AXI_awlen(7 downto 0),
      S04_AXI_awlock(0) => S04_AXI_awlock(0),
      S04_AXI_awprot(2 downto 0) => S04_AXI_awprot(2 downto 0),
      S04_AXI_awqos(3 downto 0) => S04_AXI_awqos(3 downto 0),
      S04_AXI_awready => S04_AXI_awready,
      S04_AXI_awregion(3 downto 0) => S04_AXI_awregion(3 downto 0),
      S04_AXI_awvalid => S04_AXI_awvalid,
      S04_AXI_bready => S04_AXI_bready,
      S04_AXI_bresp(1 downto 0) => S04_AXI_bresp(1 downto 0),
      S04_AXI_bvalid => S04_AXI_bvalid,
      S04_AXI_rdata(511 downto 0) => S04_AXI_rdata(511 downto 0),
      S04_AXI_rlast => S04_AXI_rlast,
      S04_AXI_rready => S04_AXI_rready,
      S04_AXI_rresp(1 downto 0) => S04_AXI_rresp(1 downto 0),
      S04_AXI_rvalid => S04_AXI_rvalid,
      S04_AXI_wdata(511 downto 0) => S04_AXI_wdata(511 downto 0),
      S04_AXI_wlast => S04_AXI_wlast,
      S04_AXI_wready => S04_AXI_wready,
      S04_AXI_wstrb(63 downto 0) => S04_AXI_wstrb(63 downto 0),
      S04_AXI_wvalid => S04_AXI_wvalid,
      S05_AXI_araddr(38 downto 0) => S05_AXI_araddr(38 downto 0),
      S05_AXI_arcache(3 downto 0) => S05_AXI_arcache(3 downto 0),
      S05_AXI_arlen(7 downto 0) => S05_AXI_arlen(7 downto 0),
      S05_AXI_arlock(0) => S05_AXI_arlock(0),
      S05_AXI_arprot(2 downto 0) => S05_AXI_arprot(2 downto 0),
      S05_AXI_arqos(3 downto 0) => S05_AXI_arqos(3 downto 0),
      S05_AXI_arready => S05_AXI_arready,
      S05_AXI_arregion(3 downto 0) => S05_AXI_arregion(3 downto 0),
      S05_AXI_arvalid => S05_AXI_arvalid,
      S05_AXI_awaddr(38 downto 0) => S05_AXI_awaddr(38 downto 0),
      S05_AXI_awcache(3 downto 0) => S05_AXI_awcache(3 downto 0),
      S05_AXI_awlen(7 downto 0) => S05_AXI_awlen(7 downto 0),
      S05_AXI_awlock(0) => S05_AXI_awlock(0),
      S05_AXI_awprot(2 downto 0) => S05_AXI_awprot(2 downto 0),
      S05_AXI_awqos(3 downto 0) => S05_AXI_awqos(3 downto 0),
      S05_AXI_awready => S05_AXI_awready,
      S05_AXI_awregion(3 downto 0) => S05_AXI_awregion(3 downto 0),
      S05_AXI_awvalid => S05_AXI_awvalid,
      S05_AXI_bready => S05_AXI_bready,
      S05_AXI_bresp(1 downto 0) => S05_AXI_bresp(1 downto 0),
      S05_AXI_bvalid => S05_AXI_bvalid,
      S05_AXI_rdata(511 downto 0) => S05_AXI_rdata(511 downto 0),
      S05_AXI_rlast => S05_AXI_rlast,
      S05_AXI_rready => S05_AXI_rready,
      S05_AXI_rresp(1 downto 0) => S05_AXI_rresp(1 downto 0),
      S05_AXI_rvalid => S05_AXI_rvalid,
      S05_AXI_wdata(511 downto 0) => S05_AXI_wdata(511 downto 0),
      S05_AXI_wlast => S05_AXI_wlast,
      S05_AXI_wready => S05_AXI_wready,
      S05_AXI_wstrb(63 downto 0) => S05_AXI_wstrb(63 downto 0),
      S05_AXI_wvalid => S05_AXI_wvalid,
      S_AXI_arready => interconnect_DDR4_MEM01_M00_AXI_ARREADY,
      S_AXI_awready => interconnect_DDR4_MEM01_M00_AXI_AWREADY,
      S_AXI_bresp(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_BRESP(1 downto 0),
      S_AXI_bvalid => interconnect_DDR4_MEM01_M00_AXI_BVALID,
      S_AXI_rdata(511 downto 0) => interconnect_DDR4_MEM01_M00_AXI_RDATA(511 downto 0),
      S_AXI_rlast => interconnect_DDR4_MEM01_M00_AXI_RLAST,
      S_AXI_rresp(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_RRESP(1 downto 0),
      S_AXI_rvalid => interconnect_DDR4_MEM01_M00_AXI_RVALID,
      S_AXI_wready => interconnect_DDR4_MEM01_M00_AXI_WREADY,
      aclk => aclk,
      aclk2 => aclk2,
      \bbstub_m_axi_araddr[38]\(38 downto 0) => M00_AXI_araddr(38 downto 0),
      \bbstub_m_axi_arburst[1]\(1 downto 0) => M00_AXI_arburst(1 downto 0),
      \bbstub_m_axi_arcache[3]\(3 downto 0) => M00_AXI_arcache(3 downto 0),
      \bbstub_m_axi_arlen[7]\(7 downto 0) => M00_AXI_arlen(7 downto 0),
      \bbstub_m_axi_arlock[0]\(0) => M00_AXI_arlock(0),
      \bbstub_m_axi_arprot[2]\(2 downto 0) => M00_AXI_arprot(2 downto 0),
      \bbstub_m_axi_arqos[3]\(3 downto 0) => M00_AXI_arqos(3 downto 0),
      bbstub_m_axi_arvalid => M00_AXI_arvalid,
      \bbstub_m_axi_awaddr[38]\(38 downto 0) => M00_AXI_awaddr(38 downto 0),
      \bbstub_m_axi_awburst[1]\(1 downto 0) => M00_AXI_awburst(1 downto 0),
      \bbstub_m_axi_awcache[3]\(3 downto 0) => M00_AXI_awcache(3 downto 0),
      \bbstub_m_axi_awlen[7]\(7 downto 0) => M00_AXI_awlen(7 downto 0),
      \bbstub_m_axi_awlock[0]\(0) => M00_AXI_awlock(0),
      \bbstub_m_axi_awprot[2]\(2 downto 0) => M00_AXI_awprot(2 downto 0),
      \bbstub_m_axi_awqos[3]\(3 downto 0) => M00_AXI_awqos(3 downto 0),
      bbstub_m_axi_awvalid => M00_AXI_awvalid,
      bbstub_m_axi_bready => M00_AXI_bready,
      bbstub_m_axi_rready => M00_AXI_rready,
      \bbstub_m_axi_wdata[511]\(511 downto 0) => M00_AXI_wdata(511 downto 0),
      bbstub_m_axi_wlast => M00_AXI_wlast,
      \bbstub_m_axi_wstrb[63]\(63 downto 0) => M00_AXI_wstrb(63 downto 0),
      bbstub_m_axi_wvalid => M00_AXI_wvalid,
      ddr4_mem01_ui_clk => \^ddr4_mem01_ui_clk\,
      interconnect_aresetn(0) => psr_aclk_SLR1_interconnect_aresetn,
      interconnect_aresetn1(0) => psr_aclk2_SLR1_interconnect_aresetn
    );
memory: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_memory_imp_1K98CM8
     port map (
      DDR4_MEM01_DIFF_CLK_clk_n => DDR4_MEM01_DIFF_CLK_clk_n,
      DDR4_MEM01_DIFF_CLK_clk_p => DDR4_MEM01_DIFF_CLK_clk_p,
      DDR4_MEM01_act_n => DDR4_MEM01_act_n,
      DDR4_MEM01_adr(16 downto 0) => DDR4_MEM01_adr(16 downto 0),
      DDR4_MEM01_ba(1 downto 0) => DDR4_MEM01_ba(1 downto 0),
      DDR4_MEM01_bg(1 downto 0) => DDR4_MEM01_bg(1 downto 0),
      DDR4_MEM01_ck_c(0) => DDR4_MEM01_ck_c(0),
      DDR4_MEM01_ck_t(0) => DDR4_MEM01_ck_t(0),
      DDR4_MEM01_cke(0) => DDR4_MEM01_cke(0),
      DDR4_MEM01_cs_n(0) => DDR4_MEM01_cs_n(0),
      DDR4_MEM01_dq(71 downto 0) => DDR4_MEM01_dq(71 downto 0),
      DDR4_MEM01_dqs_c(17 downto 0) => DDR4_MEM01_dqs_c(17 downto 0),
      DDR4_MEM01_dqs_t(17 downto 0) => DDR4_MEM01_dqs_t(17 downto 0),
      DDR4_MEM01_odt(0) => DDR4_MEM01_odt(0),
      DDR4_MEM01_par => DDR4_MEM01_par,
      DDR4_MEM01_reset_n => DDR4_MEM01_reset_n,
      S_AXI_CTRL_araddr(24 downto 0) => S_AXI_CTRL_araddr(24 downto 0),
      S_AXI_CTRL_arprot(2 downto 0) => S_AXI_CTRL_arprot(2 downto 0),
      S_AXI_CTRL_arready => S_AXI_CTRL_arready,
      S_AXI_CTRL_arvalid => S_AXI_CTRL_arvalid,
      S_AXI_CTRL_awaddr(24 downto 0) => S_AXI_CTRL_awaddr(24 downto 0),
      S_AXI_CTRL_awprot(2 downto 0) => S_AXI_CTRL_awprot(2 downto 0),
      S_AXI_CTRL_awready => S_AXI_CTRL_awready,
      S_AXI_CTRL_awvalid => S_AXI_CTRL_awvalid,
      S_AXI_CTRL_bready => S_AXI_CTRL_bready,
      S_AXI_CTRL_bresp(1 downto 0) => S_AXI_CTRL_bresp(1 downto 0),
      S_AXI_CTRL_bvalid => S_AXI_CTRL_bvalid,
      S_AXI_CTRL_rdata(31 downto 0) => S_AXI_CTRL_rdata(31 downto 0),
      S_AXI_CTRL_rready => S_AXI_CTRL_rready,
      S_AXI_CTRL_rresp(1 downto 0) => S_AXI_CTRL_rresp(1 downto 0),
      S_AXI_CTRL_rvalid => S_AXI_CTRL_rvalid,
      S_AXI_CTRL_wdata(31 downto 0) => S_AXI_CTRL_wdata(31 downto 0),
      S_AXI_CTRL_wready => S_AXI_CTRL_wready,
      S_AXI_CTRL_wstrb(3 downto 0) => S_AXI_CTRL_wstrb(3 downto 0),
      S_AXI_CTRL_wvalid => S_AXI_CTRL_wvalid,
      S_AXI_araddr(33 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARADDR(33 downto 0),
      S_AXI_arburst(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARBURST(1 downto 0),
      S_AXI_arcache(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARCACHE(3 downto 0),
      S_AXI_arlen(7 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARLEN(7 downto 0),
      S_AXI_arlock(0) => interconnect_DDR4_MEM01_M00_AXI_ARLOCK,
      S_AXI_arprot(2 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARPROT(2 downto 0),
      S_AXI_arqos(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_ARQOS(3 downto 0),
      S_AXI_arready => interconnect_DDR4_MEM01_M00_AXI_ARREADY,
      S_AXI_arvalid => interconnect_DDR4_MEM01_M00_AXI_ARVALID,
      S_AXI_awaddr(33 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWADDR(33 downto 0),
      S_AXI_awburst(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWBURST(1 downto 0),
      S_AXI_awcache(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWCACHE(3 downto 0),
      S_AXI_awlen(7 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWLEN(7 downto 0),
      S_AXI_awlock(0) => interconnect_DDR4_MEM01_M00_AXI_AWLOCK,
      S_AXI_awprot(2 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWPROT(2 downto 0),
      S_AXI_awqos(3 downto 0) => interconnect_DDR4_MEM01_M00_AXI_AWQOS(3 downto 0),
      S_AXI_awready => interconnect_DDR4_MEM01_M00_AXI_AWREADY,
      S_AXI_awvalid => interconnect_DDR4_MEM01_M00_AXI_AWVALID,
      S_AXI_bready => interconnect_DDR4_MEM01_M00_AXI_BREADY,
      S_AXI_bresp(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_BRESP(1 downto 0),
      S_AXI_bvalid => interconnect_DDR4_MEM01_M00_AXI_BVALID,
      S_AXI_rdata(511 downto 0) => interconnect_DDR4_MEM01_M00_AXI_RDATA(511 downto 0),
      S_AXI_rlast => interconnect_DDR4_MEM01_M00_AXI_RLAST,
      S_AXI_rready => interconnect_DDR4_MEM01_M00_AXI_RREADY,
      S_AXI_rresp(1 downto 0) => interconnect_DDR4_MEM01_M00_AXI_RRESP(1 downto 0),
      S_AXI_rvalid => interconnect_DDR4_MEM01_M00_AXI_RVALID,
      S_AXI_wdata(511 downto 0) => interconnect_DDR4_MEM01_M00_AXI_WDATA(511 downto 0),
      S_AXI_wlast => interconnect_DDR4_MEM01_M00_AXI_WLAST,
      S_AXI_wready => interconnect_DDR4_MEM01_M00_AXI_WREADY,
      S_AXI_wstrb(63 downto 0) => interconnect_DDR4_MEM01_M00_AXI_WSTRB(63 downto 0),
      S_AXI_wvalid => interconnect_DDR4_MEM01_M00_AXI_WVALID,
      aclk1 => aclk1,
      aresetn => aresetn,
      ddr4_mem01_sys_rst => ddr4_mem01_sys_rst,
      ddr4_mem01_ui_clk => \^ddr4_mem01_ui_clk\,
      ddr4_mem_calib_complete => ddr4_mem_calib_complete,
      ddr4_mem_calib_vec(2 downto 0) => ddr4_mem_calib_vec(2 downto 0)
    );
reset: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_reset_imp_1YKOSPE
     port map (
      aclk => aclk,
      aclk2 => aclk2,
      aresetn => aresetn,
      interconnect_aresetn(0) => psr_aclk_SLR1_interconnect_aresetn,
      interconnect_aresetn1(0) => psr_aclk2_SLR1_interconnect_aresetn
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    aclk : in STD_LOGIC;
    aclk1 : in STD_LOGIC;
    aclk2 : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    S_AXI_CTRL_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    S_AXI_CTRL_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_CTRL_awvalid : in STD_LOGIC;
    S_AXI_CTRL_awready : out STD_LOGIC;
    S_AXI_CTRL_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    S_AXI_CTRL_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S_AXI_CTRL_wvalid : in STD_LOGIC;
    S_AXI_CTRL_wready : out STD_LOGIC;
    S_AXI_CTRL_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_CTRL_bvalid : out STD_LOGIC;
    S_AXI_CTRL_bready : in STD_LOGIC;
    S_AXI_CTRL_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    S_AXI_CTRL_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S_AXI_CTRL_arvalid : in STD_LOGIC;
    S_AXI_CTRL_arready : out STD_LOGIC;
    S_AXI_CTRL_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    S_AXI_CTRL_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S_AXI_CTRL_rvalid : out STD_LOGIC;
    S_AXI_CTRL_rready : in STD_LOGIC;
    S00_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_awvalid : in STD_LOGIC;
    S00_AXI_awready : out STD_LOGIC;
    S00_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S00_AXI_wlast : in STD_LOGIC;
    S00_AXI_wvalid : in STD_LOGIC;
    S00_AXI_wready : out STD_LOGIC;
    S00_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_bvalid : out STD_LOGIC;
    S00_AXI_bready : in STD_LOGIC;
    S00_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S00_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S00_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S00_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S00_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_arvalid : in STD_LOGIC;
    S00_AXI_arready : out STD_LOGIC;
    S00_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S00_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S00_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S00_AXI_rlast : out STD_LOGIC;
    S00_AXI_rvalid : out STD_LOGIC;
    S00_AXI_rready : in STD_LOGIC;
    S01_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awvalid : in STD_LOGIC;
    S01_AXI_awready : out STD_LOGIC;
    S01_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S01_AXI_wlast : in STD_LOGIC;
    S01_AXI_wvalid : in STD_LOGIC;
    S01_AXI_wready : out STD_LOGIC;
    S01_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_bvalid : out STD_LOGIC;
    S01_AXI_bready : in STD_LOGIC;
    S01_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S01_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S01_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S01_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S01_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_arvalid : in STD_LOGIC;
    S01_AXI_arready : out STD_LOGIC;
    S01_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S01_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S01_AXI_rlast : out STD_LOGIC;
    S01_AXI_rvalid : out STD_LOGIC;
    S01_AXI_rready : in STD_LOGIC;
    S02_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awvalid : in STD_LOGIC;
    S02_AXI_awready : out STD_LOGIC;
    S02_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S02_AXI_wlast : in STD_LOGIC;
    S02_AXI_wvalid : in STD_LOGIC;
    S02_AXI_wready : out STD_LOGIC;
    S02_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_bvalid : out STD_LOGIC;
    S02_AXI_bready : in STD_LOGIC;
    S02_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S02_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S02_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S02_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S02_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arvalid : in STD_LOGIC;
    S02_AXI_arready : out STD_LOGIC;
    S02_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S02_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S02_AXI_rlast : out STD_LOGIC;
    S02_AXI_rvalid : out STD_LOGIC;
    S02_AXI_rready : in STD_LOGIC;
    S03_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S03_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S03_AXI_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S03_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_awvalid : in STD_LOGIC;
    S03_AXI_awready : out STD_LOGIC;
    S03_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S03_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S03_AXI_wlast : in STD_LOGIC;
    S03_AXI_wvalid : in STD_LOGIC;
    S03_AXI_wready : out STD_LOGIC;
    S03_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_bvalid : out STD_LOGIC;
    S03_AXI_bready : in STD_LOGIC;
    S03_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S03_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S03_AXI_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S03_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S03_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_arvalid : in STD_LOGIC;
    S03_AXI_arready : out STD_LOGIC;
    S03_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S03_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S03_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S03_AXI_rlast : out STD_LOGIC;
    S03_AXI_rvalid : out STD_LOGIC;
    S03_AXI_rready : in STD_LOGIC;
    S04_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S04_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S04_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S04_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S04_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_awvalid : in STD_LOGIC;
    S04_AXI_awready : out STD_LOGIC;
    S04_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S04_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S04_AXI_wlast : in STD_LOGIC;
    S04_AXI_wvalid : in STD_LOGIC;
    S04_AXI_wready : out STD_LOGIC;
    S04_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S04_AXI_bvalid : out STD_LOGIC;
    S04_AXI_bready : in STD_LOGIC;
    S04_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S04_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S04_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S04_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S04_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S04_AXI_arvalid : in STD_LOGIC;
    S04_AXI_arready : out STD_LOGIC;
    S04_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S04_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S04_AXI_rlast : out STD_LOGIC;
    S04_AXI_rvalid : out STD_LOGIC;
    S04_AXI_rready : in STD_LOGIC;
    S05_AXI_awaddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S05_AXI_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S05_AXI_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S05_AXI_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S05_AXI_awregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_awvalid : in STD_LOGIC;
    S05_AXI_awready : out STD_LOGIC;
    S05_AXI_wdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    S05_AXI_wstrb : in STD_LOGIC_VECTOR ( 63 downto 0 );
    S05_AXI_wlast : in STD_LOGIC;
    S05_AXI_wvalid : in STD_LOGIC;
    S05_AXI_wready : out STD_LOGIC;
    S05_AXI_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S05_AXI_bvalid : out STD_LOGIC;
    S05_AXI_bready : in STD_LOGIC;
    S05_AXI_araddr : in STD_LOGIC_VECTOR ( 38 downto 0 );
    S05_AXI_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    S05_AXI_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    S05_AXI_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    S05_AXI_arregion : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S05_AXI_arvalid : in STD_LOGIC;
    S05_AXI_arready : out STD_LOGIC;
    S05_AXI_rdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    S05_AXI_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    S05_AXI_rlast : out STD_LOGIC;
    S05_AXI_rvalid : out STD_LOGIC;
    S05_AXI_rready : in STD_LOGIC;
    DDR4_MEM00_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM00_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM00_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM00_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM00_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM00_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM00_act_n : out STD_LOGIC;
    DDR4_MEM00_reset_n : out STD_LOGIC;
    DDR4_MEM00_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM00_par : out STD_LOGIC;
    DDR4_MEM01_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM01_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM01_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM01_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM01_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM01_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM01_act_n : out STD_LOGIC;
    DDR4_MEM01_reset_n : out STD_LOGIC;
    DDR4_MEM01_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM01_par : out STD_LOGIC;
    DDR4_MEM02_dq : inout STD_LOGIC_VECTOR ( 71 downto 0 );
    DDR4_MEM02_dqs_t : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM02_dqs_c : inout STD_LOGIC_VECTOR ( 17 downto 0 );
    DDR4_MEM02_adr : out STD_LOGIC_VECTOR ( 16 downto 0 );
    DDR4_MEM02_ba : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM02_bg : out STD_LOGIC_VECTOR ( 1 downto 0 );
    DDR4_MEM02_act_n : out STD_LOGIC;
    DDR4_MEM02_reset_n : out STD_LOGIC;
    DDR4_MEM02_ck_t : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_ck_c : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_cke : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_cs_n : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_odt : out STD_LOGIC_VECTOR ( 0 to 0 );
    DDR4_MEM02_par : out STD_LOGIC;
    DDR4_MEM00_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM00_DIFF_CLK_clk_n : in STD_LOGIC;
    DDR4_MEM01_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM01_DIFF_CLK_clk_n : in STD_LOGIC;
    DDR4_MEM02_DIFF_CLK_clk_p : in STD_LOGIC;
    DDR4_MEM02_DIFF_CLK_clk_n : in STD_LOGIC;
    ddr4_mem00_ui_clk : out STD_LOGIC;
    ddr4_mem01_ui_clk : out STD_LOGIC;
    ddr4_mem02_ui_clk : out STD_LOGIC;
    ddr4_mem00_sys_rst : in STD_LOGIC;
    ddr4_mem01_sys_rst : in STD_LOGIC;
    ddr4_mem02_sys_rst : in STD_LOGIC;
    ddr4_mem_calib_complete : out STD_LOGIC;
    ddr4_mem_calib_vec : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 63 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wvalid : out STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC;
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 38 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arlock : out STD_LOGIC_VECTOR ( 0 to 0 );
    M00_AXI_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arregion : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 511 downto 0 );
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_rvalid : in STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC;
    S01_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S01_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_arid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_awid : in STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_bid : out STD_LOGIC_VECTOR ( 3 downto 0 );
    S02_AXI_rid : out STD_LOGIC_VECTOR ( 3 downto 0 )
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "pfm_dynamic_memory_subsystem_0,bd_d216,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "bd_d216,Vivado 2020.1";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of inst : label is "pfm_dynamic_memory_subsystem_0.hwdef";
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of DDR4_MEM00_DIFF_CLK_clk_n : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_N";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of DDR4_MEM00_DIFF_CLK_clk_n : signal is "XIL_INTERFACENAME DDR4_MEM00_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000";
  attribute X_INTERFACE_INFO of DDR4_MEM00_DIFF_CLK_clk_p : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM00_DIFF_CLK CLK_P";
  attribute X_INTERFACE_INFO of DDR4_MEM00_act_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ACT_N";
  attribute X_INTERFACE_INFO of DDR4_MEM00_par : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 PAR";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM00_par : signal is "XIL_INTERFACENAME DDR4_MEM00, CAN_DEBUG false, TIMEPERIOD_PS 1250, MEMORY_TYPE COMPONENTS, DATA_WIDTH 8, CS_ENABLED true, DATA_MASK_ENABLED true, SLOT Single, MEM_ADDR_MAP ROW_COLUMN_BANK, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME TDM, CAS_LATENCY 11, CAS_WRITE_LATENCY 11";
  attribute X_INTERFACE_INFO of DDR4_MEM00_reset_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 RESET_N";
  attribute X_INTERFACE_INFO of DDR4_MEM01_DIFF_CLK_clk_n : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM01_DIFF_CLK_clk_n : signal is "XIL_INTERFACENAME DDR4_MEM01_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000";
  attribute X_INTERFACE_INFO of DDR4_MEM01_DIFF_CLK_clk_p : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM01_DIFF_CLK CLK_P";
  attribute X_INTERFACE_INFO of DDR4_MEM01_act_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ACT_N";
  attribute X_INTERFACE_INFO of DDR4_MEM01_par : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 PAR";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM01_par : signal is "XIL_INTERFACENAME DDR4_MEM01, CAN_DEBUG false, TIMEPERIOD_PS 833, MEMORY_TYPE RDIMMs, MEMORY_PART MTA18ASF2G72PZ-2G3, DATA_WIDTH 72, CS_ENABLED true, DATA_MASK_ENABLED NONE, SLOT Single, CUSTOM_PARTS no_file_loaded, MEM_ADDR_MAP ROW_COLUMN_BANK_INTLV, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME RD_PRI_REG, CAS_LATENCY 17, CAS_WRITE_LATENCY 12";
  attribute X_INTERFACE_INFO of DDR4_MEM01_reset_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 RESET_N";
  attribute X_INTERFACE_INFO of DDR4_MEM02_DIFF_CLK_clk_n : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_N";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM02_DIFF_CLK_clk_n : signal is "XIL_INTERFACENAME DDR4_MEM02_DIFF_CLK, CAN_DEBUG false, FREQ_HZ 300000000";
  attribute X_INTERFACE_INFO of DDR4_MEM02_DIFF_CLK_clk_p : signal is "xilinx.com:interface:diff_clock:1.0 DDR4_MEM02_DIFF_CLK CLK_P";
  attribute X_INTERFACE_INFO of DDR4_MEM02_act_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ACT_N";
  attribute X_INTERFACE_INFO of DDR4_MEM02_par : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 PAR";
  attribute X_INTERFACE_PARAMETER of DDR4_MEM02_par : signal is "XIL_INTERFACENAME DDR4_MEM02, CAN_DEBUG false, TIMEPERIOD_PS 1250, MEMORY_TYPE COMPONENTS, DATA_WIDTH 8, CS_ENABLED true, DATA_MASK_ENABLED true, SLOT Single, MEM_ADDR_MAP ROW_COLUMN_BANK, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME TDM, CAS_LATENCY 11, CAS_WRITE_LATENCY 11";
  attribute X_INTERFACE_INFO of DDR4_MEM02_reset_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 RESET_N";
  attribute X_INTERFACE_INFO of M00_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY";
  attribute X_INTERFACE_INFO of M00_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID";
  attribute X_INTERFACE_INFO of M00_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY";
  attribute X_INTERFACE_INFO of M00_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID";
  attribute X_INTERFACE_INFO of M00_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY";
  attribute X_INTERFACE_INFO of M00_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BVALID";
  attribute X_INTERFACE_INFO of M00_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RLAST";
  attribute X_INTERFACE_INFO of M00_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY";
  attribute X_INTERFACE_PARAMETER of M00_AXI_rready : signal is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 0, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 1, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of M00_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RVALID";
  attribute X_INTERFACE_INFO of M00_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WLAST";
  attribute X_INTERFACE_INFO of M00_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WREADY";
  attribute X_INTERFACE_INFO of M00_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID";
  attribute X_INTERFACE_INFO of S00_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARREADY";
  attribute X_INTERFACE_INFO of S00_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARVALID";
  attribute X_INTERFACE_INFO of S00_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWREADY";
  attribute X_INTERFACE_INFO of S00_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWVALID";
  attribute X_INTERFACE_INFO of S00_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BREADY";
  attribute X_INTERFACE_INFO of S00_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BVALID";
  attribute X_INTERFACE_INFO of S00_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RLAST";
  attribute X_INTERFACE_INFO of S00_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RREADY";
  attribute X_INTERFACE_PARAMETER of S00_AXI_rready : signal is "XIL_INTERFACENAME S00_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S00_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RVALID";
  attribute X_INTERFACE_INFO of S00_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WLAST";
  attribute X_INTERFACE_INFO of S00_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WREADY";
  attribute X_INTERFACE_INFO of S00_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WVALID";
  attribute X_INTERFACE_INFO of S01_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARREADY";
  attribute X_INTERFACE_INFO of S01_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARVALID";
  attribute X_INTERFACE_INFO of S01_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWREADY";
  attribute X_INTERFACE_INFO of S01_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWVALID";
  attribute X_INTERFACE_INFO of S01_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BREADY";
  attribute X_INTERFACE_INFO of S01_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BVALID";
  attribute X_INTERFACE_INFO of S01_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RLAST";
  attribute X_INTERFACE_INFO of S01_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RREADY";
  attribute X_INTERFACE_INFO of S01_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RVALID";
  attribute X_INTERFACE_INFO of S01_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WLAST";
  attribute X_INTERFACE_INFO of S01_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WREADY";
  attribute X_INTERFACE_INFO of S01_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WVALID";
  attribute X_INTERFACE_INFO of S02_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARREADY";
  attribute X_INTERFACE_INFO of S02_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARVALID";
  attribute X_INTERFACE_INFO of S02_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWREADY";
  attribute X_INTERFACE_INFO of S02_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWVALID";
  attribute X_INTERFACE_INFO of S02_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BREADY";
  attribute X_INTERFACE_INFO of S02_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BVALID";
  attribute X_INTERFACE_INFO of S02_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RLAST";
  attribute X_INTERFACE_INFO of S02_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RREADY";
  attribute X_INTERFACE_INFO of S02_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RVALID";
  attribute X_INTERFACE_INFO of S02_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WLAST";
  attribute X_INTERFACE_INFO of S02_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WREADY";
  attribute X_INTERFACE_INFO of S02_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WVALID";
  attribute X_INTERFACE_INFO of S03_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARREADY";
  attribute X_INTERFACE_INFO of S03_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARVALID";
  attribute X_INTERFACE_INFO of S03_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWREADY";
  attribute X_INTERFACE_INFO of S03_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWVALID";
  attribute X_INTERFACE_INFO of S03_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BREADY";
  attribute X_INTERFACE_INFO of S03_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BVALID";
  attribute X_INTERFACE_INFO of S03_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RLAST";
  attribute X_INTERFACE_INFO of S03_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RREADY";
  attribute X_INTERFACE_PARAMETER of S03_AXI_rready : signal is "XIL_INTERFACENAME S03_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S03_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RVALID";
  attribute X_INTERFACE_INFO of S03_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WLAST";
  attribute X_INTERFACE_INFO of S03_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WREADY";
  attribute X_INTERFACE_INFO of S03_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WVALID";
  attribute X_INTERFACE_INFO of S04_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARREADY";
  attribute X_INTERFACE_INFO of S04_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARVALID";
  attribute X_INTERFACE_INFO of S04_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWREADY";
  attribute X_INTERFACE_INFO of S04_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWVALID";
  attribute X_INTERFACE_INFO of S04_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI BREADY";
  attribute X_INTERFACE_INFO of S04_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI BVALID";
  attribute X_INTERFACE_INFO of S04_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RLAST";
  attribute X_INTERFACE_INFO of S04_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RREADY";
  attribute X_INTERFACE_PARAMETER of S04_AXI_rready : signal is "XIL_INTERFACENAME S04_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 0, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S04_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RVALID";
  attribute X_INTERFACE_INFO of S04_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WLAST";
  attribute X_INTERFACE_INFO of S04_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WREADY";
  attribute X_INTERFACE_INFO of S04_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WVALID";
  attribute X_INTERFACE_INFO of S05_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARREADY";
  attribute X_INTERFACE_INFO of S05_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARVALID";
  attribute X_INTERFACE_INFO of S05_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWREADY";
  attribute X_INTERFACE_INFO of S05_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWVALID";
  attribute X_INTERFACE_INFO of S05_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI BREADY";
  attribute X_INTERFACE_INFO of S05_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI BVALID";
  attribute X_INTERFACE_INFO of S05_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RLAST";
  attribute X_INTERFACE_INFO of S05_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RREADY";
  attribute X_INTERFACE_PARAMETER of S05_AXI_rready : signal is "XIL_INTERFACENAME S05_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 0, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S05_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RVALID";
  attribute X_INTERFACE_INFO of S05_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WLAST";
  attribute X_INTERFACE_INFO of S05_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WREADY";
  attribute X_INTERFACE_INFO of S05_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_arready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_arvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_bready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_bvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RREADY";
  attribute X_INTERFACE_PARAMETER of S_AXI_CTRL_rready : signal is "XIL_INTERFACENAME S_AXI_CTRL, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RVALID";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wready : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WREADY";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wvalid : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WVALID";
  attribute X_INTERFACE_INFO of aclk : signal is "xilinx.com:signal:clock:1.0 CLK.aclk CLK";
  attribute X_INTERFACE_PARAMETER of aclk : signal is "XIL_INTERFACENAME CLK.aclk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, ASSOCIATED_BUSIF S00_AXI:S01_AXI:S02_AXI:S03_AXI:S04_AXI:S05_AXI, INSERT_VIP 0, ASSOCIATED_CLKEN m_sc_aclken";
  attribute X_INTERFACE_INFO of aclk1 : signal is "xilinx.com:signal:clock:1.0 CLK.aclk1 CLK";
  attribute X_INTERFACE_PARAMETER of aclk1 : signal is "XIL_INTERFACENAME CLK.aclk1, FREQ_HZ 50925925, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, ASSOCIATED_BUSIF S_AXI_CTRL, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of aclk2 : signal is "xilinx.com:signal:clock:1.0 CLK.aclk2 CLK";
  attribute X_INTERFACE_PARAMETER of aclk2 : signal is "XIL_INTERFACENAME CLK.aclk2, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_ddrmem_1_c0_ddr4_ui_clk, ASSOCIATED_BUSIF M00_AXI, INSERT_VIP 0, ASSOCIATED_CLKEN m_sc_aclken";
  attribute X_INTERFACE_INFO of aresetn : signal is "xilinx.com:signal:reset:1.0 RST.aresetn RST";
  attribute X_INTERFACE_PARAMETER of aresetn : signal is "XIL_INTERFACENAME RST.aresetn, POLARITY ACTIVE_LOW, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem00_sys_rst : signal is "xilinx.com:signal:reset:1.0 RST.ddr4_mem00_sys_rst RST";
  attribute X_INTERFACE_PARAMETER of ddr4_mem00_sys_rst : signal is "XIL_INTERFACENAME RST.ddr4_mem00_sys_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem00_ui_clk : signal is "xilinx.com:signal:clock:1.0 CLK.ddr4_mem00_ui_clk CLK";
  attribute X_INTERFACE_PARAMETER of ddr4_mem00_ui_clk : signal is "XIL_INTERFACENAME CLK.ddr4_mem00_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem00_ui_clk, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem01_sys_rst : signal is "xilinx.com:signal:reset:1.0 RST.ddr4_mem01_sys_rst RST";
  attribute X_INTERFACE_PARAMETER of ddr4_mem01_sys_rst : signal is "XIL_INTERFACENAME RST.ddr4_mem01_sys_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem01_ui_clk : signal is "xilinx.com:signal:clock:1.0 CLK.ddr4_mem01_ui_clk CLK";
  attribute X_INTERFACE_PARAMETER of ddr4_mem01_ui_clk : signal is "XIL_INTERFACENAME CLK.ddr4_mem01_ui_clk, FREQ_HZ 300000000, FREQ_TOLERANCE_HZ 0, PHASE 0.00, CLK_DOMAIN bd_d216_ddr4_mem01_0_c0_ddr4_ui_clk, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem02_sys_rst : signal is "xilinx.com:signal:reset:1.0 RST.ddr4_mem02_sys_rst RST";
  attribute X_INTERFACE_PARAMETER of ddr4_mem02_sys_rst : signal is "XIL_INTERFACENAME RST.ddr4_mem02_sys_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem02_ui_clk : signal is "xilinx.com:signal:clock:1.0 CLK.ddr4_mem02_ui_clk CLK";
  attribute X_INTERFACE_PARAMETER of ddr4_mem02_ui_clk : signal is "XIL_INTERFACENAME CLK.ddr4_mem02_ui_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_memory_subsystem_0_ddr4_mem02_ui_clk, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ddr4_mem_calib_complete : signal is "xilinx.com:signal:data:1.0 DATA.ddr4_mem_calib_complete DATA";
  attribute X_INTERFACE_PARAMETER of ddr4_mem_calib_complete : signal is "XIL_INTERFACENAME DATA.ddr4_mem_calib_complete, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of DDR4_MEM00_adr : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ADR";
  attribute X_INTERFACE_INFO of DDR4_MEM00_ba : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BA";
  attribute X_INTERFACE_INFO of DDR4_MEM00_bg : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 BG";
  attribute X_INTERFACE_INFO of DDR4_MEM00_ck_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_C";
  attribute X_INTERFACE_INFO of DDR4_MEM00_ck_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CK_T";
  attribute X_INTERFACE_INFO of DDR4_MEM00_cke : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CKE";
  attribute X_INTERFACE_INFO of DDR4_MEM00_cs_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 CS_N";
  attribute X_INTERFACE_INFO of DDR4_MEM00_dq : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQ";
  attribute X_INTERFACE_INFO of DDR4_MEM00_dqs_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_C";
  attribute X_INTERFACE_INFO of DDR4_MEM00_dqs_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 DQS_T";
  attribute X_INTERFACE_INFO of DDR4_MEM00_odt : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM00 ODT";
  attribute X_INTERFACE_INFO of DDR4_MEM01_adr : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ADR";
  attribute X_INTERFACE_INFO of DDR4_MEM01_ba : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BA";
  attribute X_INTERFACE_INFO of DDR4_MEM01_bg : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 BG";
  attribute X_INTERFACE_INFO of DDR4_MEM01_ck_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_C";
  attribute X_INTERFACE_INFO of DDR4_MEM01_ck_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CK_T";
  attribute X_INTERFACE_INFO of DDR4_MEM01_cke : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CKE";
  attribute X_INTERFACE_INFO of DDR4_MEM01_cs_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 CS_N";
  attribute X_INTERFACE_INFO of DDR4_MEM01_dq : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQ";
  attribute X_INTERFACE_INFO of DDR4_MEM01_dqs_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_C";
  attribute X_INTERFACE_INFO of DDR4_MEM01_dqs_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 DQS_T";
  attribute X_INTERFACE_INFO of DDR4_MEM01_odt : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM01 ODT";
  attribute X_INTERFACE_INFO of DDR4_MEM02_adr : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ADR";
  attribute X_INTERFACE_INFO of DDR4_MEM02_ba : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BA";
  attribute X_INTERFACE_INFO of DDR4_MEM02_bg : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 BG";
  attribute X_INTERFACE_INFO of DDR4_MEM02_ck_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_C";
  attribute X_INTERFACE_INFO of DDR4_MEM02_ck_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CK_T";
  attribute X_INTERFACE_INFO of DDR4_MEM02_cke : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CKE";
  attribute X_INTERFACE_INFO of DDR4_MEM02_cs_n : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 CS_N";
  attribute X_INTERFACE_INFO of DDR4_MEM02_dq : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQ";
  attribute X_INTERFACE_INFO of DDR4_MEM02_dqs_c : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_C";
  attribute X_INTERFACE_INFO of DDR4_MEM02_dqs_t : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 DQS_T";
  attribute X_INTERFACE_INFO of DDR4_MEM02_odt : signal is "xilinx.com:interface:ddr4:1.0 DDR4_MEM02 ODT";
  attribute X_INTERFACE_INFO of M00_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_INFO of M00_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST";
  attribute X_INTERFACE_INFO of M00_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARCACHE";
  attribute X_INTERFACE_INFO of M00_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN";
  attribute X_INTERFACE_INFO of M00_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARLOCK";
  attribute X_INTERFACE_INFO of M00_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARPROT";
  attribute X_INTERFACE_INFO of M00_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARQOS";
  attribute X_INTERFACE_INFO of M00_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREGION";
  attribute X_INTERFACE_INFO of M00_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE";
  attribute X_INTERFACE_INFO of M00_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR";
  attribute X_INTERFACE_INFO of M00_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST";
  attribute X_INTERFACE_INFO of M00_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWCACHE";
  attribute X_INTERFACE_INFO of M00_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN";
  attribute X_INTERFACE_INFO of M00_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWLOCK";
  attribute X_INTERFACE_INFO of M00_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWPROT";
  attribute X_INTERFACE_INFO of M00_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWQOS";
  attribute X_INTERFACE_INFO of M00_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREGION";
  attribute X_INTERFACE_INFO of M00_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE";
  attribute X_INTERFACE_INFO of M00_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BRESP";
  attribute X_INTERFACE_INFO of M00_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RDATA";
  attribute X_INTERFACE_INFO of M00_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RRESP";
  attribute X_INTERFACE_INFO of M00_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of M00_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of S00_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARADDR";
  attribute X_INTERFACE_INFO of S00_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARBURST";
  attribute X_INTERFACE_INFO of S00_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S00_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARID";
  attribute X_INTERFACE_INFO of S00_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARLEN";
  attribute X_INTERFACE_INFO of S00_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S00_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARPROT";
  attribute X_INTERFACE_INFO of S00_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARQOS";
  attribute X_INTERFACE_INFO of S00_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARREGION";
  attribute X_INTERFACE_INFO of S00_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARSIZE";
  attribute X_INTERFACE_INFO of S00_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWADDR";
  attribute X_INTERFACE_INFO of S00_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWBURST";
  attribute X_INTERFACE_INFO of S00_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S00_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWID";
  attribute X_INTERFACE_INFO of S00_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWLEN";
  attribute X_INTERFACE_INFO of S00_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S00_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWPROT";
  attribute X_INTERFACE_INFO of S00_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWQOS";
  attribute X_INTERFACE_INFO of S00_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWREGION";
  attribute X_INTERFACE_INFO of S00_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWSIZE";
  attribute X_INTERFACE_INFO of S00_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BID";
  attribute X_INTERFACE_INFO of S00_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BRESP";
  attribute X_INTERFACE_INFO of S00_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RDATA";
  attribute X_INTERFACE_INFO of S00_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RID";
  attribute X_INTERFACE_INFO of S00_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RRESP";
  attribute X_INTERFACE_INFO of S00_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WDATA";
  attribute X_INTERFACE_INFO of S00_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WSTRB";
  attribute X_INTERFACE_INFO of S01_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARADDR";
  attribute X_INTERFACE_INFO of S01_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARBURST";
  attribute X_INTERFACE_INFO of S01_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S01_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARID";
  attribute X_INTERFACE_INFO of S01_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARLEN";
  attribute X_INTERFACE_INFO of S01_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S01_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARPROT";
  attribute X_INTERFACE_INFO of S01_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARQOS";
  attribute X_INTERFACE_INFO of S01_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S01_AXI ARREGION";
  attribute X_INTERFACE_INFO of S01_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWADDR";
  attribute X_INTERFACE_INFO of S01_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWBURST";
  attribute X_INTERFACE_INFO of S01_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S01_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWID";
  attribute X_INTERFACE_INFO of S01_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWLEN";
  attribute X_INTERFACE_INFO of S01_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S01_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWPROT";
  attribute X_INTERFACE_INFO of S01_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWQOS";
  attribute X_INTERFACE_INFO of S01_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S01_AXI AWREGION";
  attribute X_INTERFACE_INFO of S01_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BID";
  attribute X_INTERFACE_INFO of S01_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S01_AXI BRESP";
  attribute X_INTERFACE_INFO of S01_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RDATA";
  attribute X_INTERFACE_INFO of S01_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RID";
  attribute X_INTERFACE_PARAMETER of S01_AXI_rid : signal is "XIL_INTERFACENAME S01_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S01_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S01_AXI RRESP";
  attribute X_INTERFACE_INFO of S01_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WDATA";
  attribute X_INTERFACE_INFO of S01_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S01_AXI WSTRB";
  attribute X_INTERFACE_INFO of S02_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARADDR";
  attribute X_INTERFACE_INFO of S02_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARBURST";
  attribute X_INTERFACE_INFO of S02_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S02_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARID";
  attribute X_INTERFACE_INFO of S02_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARLEN";
  attribute X_INTERFACE_INFO of S02_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S02_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARPROT";
  attribute X_INTERFACE_INFO of S02_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARQOS";
  attribute X_INTERFACE_INFO of S02_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S02_AXI ARREGION";
  attribute X_INTERFACE_INFO of S02_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWADDR";
  attribute X_INTERFACE_INFO of S02_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWBURST";
  attribute X_INTERFACE_INFO of S02_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S02_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWID";
  attribute X_INTERFACE_INFO of S02_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWLEN";
  attribute X_INTERFACE_INFO of S02_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S02_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWPROT";
  attribute X_INTERFACE_INFO of S02_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWQOS";
  attribute X_INTERFACE_INFO of S02_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S02_AXI AWREGION";
  attribute X_INTERFACE_INFO of S02_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BID";
  attribute X_INTERFACE_INFO of S02_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S02_AXI BRESP";
  attribute X_INTERFACE_INFO of S02_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RDATA";
  attribute X_INTERFACE_INFO of S02_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RID";
  attribute X_INTERFACE_PARAMETER of S02_AXI_rid : signal is "XIL_INTERFACENAME S02_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 300000000, ID_WIDTH 4, ADDR_WIDTH 39, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 1, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 16, NUM_WRITE_OUTSTANDING 16, MAX_BURST_LENGTH 256, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_kernel_clk_out1, NUM_READ_THREADS 2, NUM_WRITE_THREADS 2, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of S02_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S02_AXI RRESP";
  attribute X_INTERFACE_INFO of S02_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WDATA";
  attribute X_INTERFACE_INFO of S02_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S02_AXI WSTRB";
  attribute X_INTERFACE_INFO of S03_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARADDR";
  attribute X_INTERFACE_INFO of S03_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARBURST";
  attribute X_INTERFACE_INFO of S03_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S03_AXI_arid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARID";
  attribute X_INTERFACE_INFO of S03_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARLEN";
  attribute X_INTERFACE_INFO of S03_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S03_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARPROT";
  attribute X_INTERFACE_INFO of S03_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARQOS";
  attribute X_INTERFACE_INFO of S03_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARREGION";
  attribute X_INTERFACE_INFO of S03_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 S03_AXI ARSIZE";
  attribute X_INTERFACE_INFO of S03_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWADDR";
  attribute X_INTERFACE_INFO of S03_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWBURST";
  attribute X_INTERFACE_INFO of S03_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S03_AXI_awid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWID";
  attribute X_INTERFACE_INFO of S03_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWLEN";
  attribute X_INTERFACE_INFO of S03_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S03_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWPROT";
  attribute X_INTERFACE_INFO of S03_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWQOS";
  attribute X_INTERFACE_INFO of S03_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWREGION";
  attribute X_INTERFACE_INFO of S03_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 S03_AXI AWSIZE";
  attribute X_INTERFACE_INFO of S03_AXI_bid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BID";
  attribute X_INTERFACE_INFO of S03_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S03_AXI BRESP";
  attribute X_INTERFACE_INFO of S03_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RDATA";
  attribute X_INTERFACE_INFO of S03_AXI_rid : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RID";
  attribute X_INTERFACE_INFO of S03_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S03_AXI RRESP";
  attribute X_INTERFACE_INFO of S03_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WDATA";
  attribute X_INTERFACE_INFO of S03_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S03_AXI WSTRB";
  attribute X_INTERFACE_INFO of S04_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARADDR";
  attribute X_INTERFACE_INFO of S04_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S04_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARLEN";
  attribute X_INTERFACE_INFO of S04_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S04_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARPROT";
  attribute X_INTERFACE_INFO of S04_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARQOS";
  attribute X_INTERFACE_INFO of S04_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S04_AXI ARREGION";
  attribute X_INTERFACE_INFO of S04_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWADDR";
  attribute X_INTERFACE_INFO of S04_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S04_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWLEN";
  attribute X_INTERFACE_INFO of S04_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S04_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWPROT";
  attribute X_INTERFACE_INFO of S04_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWQOS";
  attribute X_INTERFACE_INFO of S04_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S04_AXI AWREGION";
  attribute X_INTERFACE_INFO of S04_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S04_AXI BRESP";
  attribute X_INTERFACE_INFO of S04_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RDATA";
  attribute X_INTERFACE_INFO of S04_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S04_AXI RRESP";
  attribute X_INTERFACE_INFO of S04_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WDATA";
  attribute X_INTERFACE_INFO of S04_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S04_AXI WSTRB";
  attribute X_INTERFACE_INFO of S05_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARADDR";
  attribute X_INTERFACE_INFO of S05_AXI_arcache : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARCACHE";
  attribute X_INTERFACE_INFO of S05_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARLEN";
  attribute X_INTERFACE_INFO of S05_AXI_arlock : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARLOCK";
  attribute X_INTERFACE_INFO of S05_AXI_arprot : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARPROT";
  attribute X_INTERFACE_INFO of S05_AXI_arqos : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARQOS";
  attribute X_INTERFACE_INFO of S05_AXI_arregion : signal is "xilinx.com:interface:aximm:1.0 S05_AXI ARREGION";
  attribute X_INTERFACE_INFO of S05_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWADDR";
  attribute X_INTERFACE_INFO of S05_AXI_awcache : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWCACHE";
  attribute X_INTERFACE_INFO of S05_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWLEN";
  attribute X_INTERFACE_INFO of S05_AXI_awlock : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWLOCK";
  attribute X_INTERFACE_INFO of S05_AXI_awprot : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWPROT";
  attribute X_INTERFACE_INFO of S05_AXI_awqos : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWQOS";
  attribute X_INTERFACE_INFO of S05_AXI_awregion : signal is "xilinx.com:interface:aximm:1.0 S05_AXI AWREGION";
  attribute X_INTERFACE_INFO of S05_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 S05_AXI BRESP";
  attribute X_INTERFACE_INFO of S05_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RDATA";
  attribute X_INTERFACE_INFO of S05_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 S05_AXI RRESP";
  attribute X_INTERFACE_INFO of S05_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WDATA";
  attribute X_INTERFACE_INFO of S05_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 S05_AXI WSTRB";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_araddr : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARADDR";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_arprot : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL ARPROT";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awaddr : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWADDR";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_awprot : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL AWPROT";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_bresp : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL BRESP";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rdata : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RDATA";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_rresp : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL RRESP";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wdata : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WDATA";
  attribute X_INTERFACE_INFO of S_AXI_CTRL_wstrb : signal is "xilinx.com:interface:aximm:1.0 S_AXI_CTRL WSTRB";
  attribute X_INTERFACE_INFO of ddr4_mem_calib_vec : signal is "xilinx.com:signal:data:1.0 DATA.ddr4_mem_calib_vec DATA";
  attribute X_INTERFACE_PARAMETER of ddr4_mem_calib_vec : signal is "XIL_INTERFACENAME DATA.ddr4_mem_calib_vec, LAYERED_METADATA undef, PortWidth 3";
begin
inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_bd_d216
     port map (
      DDR4_MEM00_DIFF_CLK_clk_n => DDR4_MEM00_DIFF_CLK_clk_n,
      DDR4_MEM00_DIFF_CLK_clk_p => DDR4_MEM00_DIFF_CLK_clk_p,
      DDR4_MEM00_act_n => DDR4_MEM00_act_n,
      DDR4_MEM00_adr(16 downto 0) => DDR4_MEM00_adr(16 downto 0),
      DDR4_MEM00_ba(1 downto 0) => DDR4_MEM00_ba(1 downto 0),
      DDR4_MEM00_bg(1 downto 0) => DDR4_MEM00_bg(1 downto 0),
      DDR4_MEM00_ck_c(0) => DDR4_MEM00_ck_c(0),
      DDR4_MEM00_ck_t(0) => DDR4_MEM00_ck_t(0),
      DDR4_MEM00_cke(0) => DDR4_MEM00_cke(0),
      DDR4_MEM00_cs_n(0) => DDR4_MEM00_cs_n(0),
      DDR4_MEM00_dq(71 downto 0) => DDR4_MEM00_dq(71 downto 0),
      DDR4_MEM00_dqs_c(17 downto 0) => DDR4_MEM00_dqs_c(17 downto 0),
      DDR4_MEM00_dqs_t(17 downto 0) => DDR4_MEM00_dqs_t(17 downto 0),
      DDR4_MEM00_odt(0) => DDR4_MEM00_odt(0),
      DDR4_MEM00_par => DDR4_MEM00_par,
      DDR4_MEM00_reset_n => DDR4_MEM00_reset_n,
      DDR4_MEM01_DIFF_CLK_clk_n => DDR4_MEM01_DIFF_CLK_clk_n,
      DDR4_MEM01_DIFF_CLK_clk_p => DDR4_MEM01_DIFF_CLK_clk_p,
      DDR4_MEM01_act_n => DDR4_MEM01_act_n,
      DDR4_MEM01_adr(16 downto 0) => DDR4_MEM01_adr(16 downto 0),
      DDR4_MEM01_ba(1 downto 0) => DDR4_MEM01_ba(1 downto 0),
      DDR4_MEM01_bg(1 downto 0) => DDR4_MEM01_bg(1 downto 0),
      DDR4_MEM01_ck_c(0) => DDR4_MEM01_ck_c(0),
      DDR4_MEM01_ck_t(0) => DDR4_MEM01_ck_t(0),
      DDR4_MEM01_cke(0) => DDR4_MEM01_cke(0),
      DDR4_MEM01_cs_n(0) => DDR4_MEM01_cs_n(0),
      DDR4_MEM01_dq(71 downto 0) => DDR4_MEM01_dq(71 downto 0),
      DDR4_MEM01_dqs_c(17 downto 0) => DDR4_MEM01_dqs_c(17 downto 0),
      DDR4_MEM01_dqs_t(17 downto 0) => DDR4_MEM01_dqs_t(17 downto 0),
      DDR4_MEM01_odt(0) => DDR4_MEM01_odt(0),
      DDR4_MEM01_par => DDR4_MEM01_par,
      DDR4_MEM01_reset_n => DDR4_MEM01_reset_n,
      DDR4_MEM02_DIFF_CLK_clk_n => DDR4_MEM02_DIFF_CLK_clk_n,
      DDR4_MEM02_DIFF_CLK_clk_p => DDR4_MEM02_DIFF_CLK_clk_p,
      DDR4_MEM02_act_n => DDR4_MEM02_act_n,
      DDR4_MEM02_adr(16 downto 0) => DDR4_MEM02_adr(16 downto 0),
      DDR4_MEM02_ba(1 downto 0) => DDR4_MEM02_ba(1 downto 0),
      DDR4_MEM02_bg(1 downto 0) => DDR4_MEM02_bg(1 downto 0),
      DDR4_MEM02_ck_c(0) => DDR4_MEM02_ck_c(0),
      DDR4_MEM02_ck_t(0) => DDR4_MEM02_ck_t(0),
      DDR4_MEM02_cke(0) => DDR4_MEM02_cke(0),
      DDR4_MEM02_cs_n(0) => DDR4_MEM02_cs_n(0),
      DDR4_MEM02_dq(71 downto 0) => DDR4_MEM02_dq(71 downto 0),
      DDR4_MEM02_dqs_c(17 downto 0) => DDR4_MEM02_dqs_c(17 downto 0),
      DDR4_MEM02_dqs_t(17 downto 0) => DDR4_MEM02_dqs_t(17 downto 0),
      DDR4_MEM02_odt(0) => DDR4_MEM02_odt(0),
      DDR4_MEM02_par => DDR4_MEM02_par,
      DDR4_MEM02_reset_n => DDR4_MEM02_reset_n,
      M00_AXI_araddr(38 downto 0) => M00_AXI_araddr(38 downto 0),
      M00_AXI_arburst(1 downto 0) => M00_AXI_arburst(1 downto 0),
      M00_AXI_arcache(3 downto 0) => M00_AXI_arcache(3 downto 0),
      M00_AXI_arlen(7 downto 0) => M00_AXI_arlen(7 downto 0),
      M00_AXI_arlock(0) => M00_AXI_arlock(0),
      M00_AXI_arprot(2 downto 0) => M00_AXI_arprot(2 downto 0),
      M00_AXI_arqos(3 downto 0) => M00_AXI_arqos(3 downto 0),
      M00_AXI_arready => M00_AXI_arready,
      M00_AXI_arregion(3 downto 0) => M00_AXI_arregion(3 downto 0),
      M00_AXI_arsize(2 downto 0) => M00_AXI_arsize(2 downto 0),
      M00_AXI_arvalid => M00_AXI_arvalid,
      M00_AXI_awaddr(38 downto 0) => M00_AXI_awaddr(38 downto 0),
      M00_AXI_awburst(1 downto 0) => M00_AXI_awburst(1 downto 0),
      M00_AXI_awcache(3 downto 0) => M00_AXI_awcache(3 downto 0),
      M00_AXI_awlen(7 downto 0) => M00_AXI_awlen(7 downto 0),
      M00_AXI_awlock(0) => M00_AXI_awlock(0),
      M00_AXI_awprot(2 downto 0) => M00_AXI_awprot(2 downto 0),
      M00_AXI_awqos(3 downto 0) => M00_AXI_awqos(3 downto 0),
      M00_AXI_awready => M00_AXI_awready,
      M00_AXI_awregion(3 downto 0) => M00_AXI_awregion(3 downto 0),
      M00_AXI_awsize(2 downto 0) => M00_AXI_awsize(2 downto 0),
      M00_AXI_awvalid => M00_AXI_awvalid,
      M00_AXI_bready => M00_AXI_bready,
      M00_AXI_bresp(1 downto 0) => M00_AXI_bresp(1 downto 0),
      M00_AXI_bvalid => M00_AXI_bvalid,
      M00_AXI_rdata(511 downto 0) => M00_AXI_rdata(511 downto 0),
      M00_AXI_rlast => M00_AXI_rlast,
      M00_AXI_rready => M00_AXI_rready,
      M00_AXI_rresp(1 downto 0) => M00_AXI_rresp(1 downto 0),
      M00_AXI_rvalid => M00_AXI_rvalid,
      M00_AXI_wdata(511 downto 0) => M00_AXI_wdata(511 downto 0),
      M00_AXI_wlast => M00_AXI_wlast,
      M00_AXI_wready => M00_AXI_wready,
      M00_AXI_wstrb(63 downto 0) => M00_AXI_wstrb(63 downto 0),
      M00_AXI_wvalid => M00_AXI_wvalid,
      S00_AXI_araddr(38 downto 0) => S00_AXI_araddr(38 downto 0),
      S00_AXI_arburst(1 downto 0) => S00_AXI_arburst(1 downto 0),
      S00_AXI_arcache(3 downto 0) => S00_AXI_arcache(3 downto 0),
      S00_AXI_arid(3 downto 0) => S00_AXI_arid(3 downto 0),
      S00_AXI_arlen(7 downto 0) => S00_AXI_arlen(7 downto 0),
      S00_AXI_arlock(0) => S00_AXI_arlock(0),
      S00_AXI_arprot(2 downto 0) => S00_AXI_arprot(2 downto 0),
      S00_AXI_arqos(3 downto 0) => S00_AXI_arqos(3 downto 0),
      S00_AXI_arready => S00_AXI_arready,
      S00_AXI_arregion(3 downto 0) => S00_AXI_arregion(3 downto 0),
      S00_AXI_arsize(2 downto 0) => S00_AXI_arsize(2 downto 0),
      S00_AXI_arvalid => S00_AXI_arvalid,
      S00_AXI_awaddr(38 downto 0) => S00_AXI_awaddr(38 downto 0),
      S00_AXI_awburst(1 downto 0) => S00_AXI_awburst(1 downto 0),
      S00_AXI_awcache(3 downto 0) => S00_AXI_awcache(3 downto 0),
      S00_AXI_awid(3 downto 0) => S00_AXI_awid(3 downto 0),
      S00_AXI_awlen(7 downto 0) => S00_AXI_awlen(7 downto 0),
      S00_AXI_awlock(0) => S00_AXI_awlock(0),
      S00_AXI_awprot(2 downto 0) => S00_AXI_awprot(2 downto 0),
      S00_AXI_awqos(3 downto 0) => S00_AXI_awqos(3 downto 0),
      S00_AXI_awready => S00_AXI_awready,
      S00_AXI_awregion(3 downto 0) => S00_AXI_awregion(3 downto 0),
      S00_AXI_awsize(2 downto 0) => S00_AXI_awsize(2 downto 0),
      S00_AXI_awvalid => S00_AXI_awvalid,
      S00_AXI_bid(3 downto 0) => S00_AXI_bid(3 downto 0),
      S00_AXI_bready => S00_AXI_bready,
      S00_AXI_bresp(1 downto 0) => S00_AXI_bresp(1 downto 0),
      S00_AXI_bvalid => S00_AXI_bvalid,
      S00_AXI_rdata(511 downto 0) => S00_AXI_rdata(511 downto 0),
      S00_AXI_rid(3 downto 0) => S00_AXI_rid(3 downto 0),
      S00_AXI_rlast => S00_AXI_rlast,
      S00_AXI_rready => S00_AXI_rready,
      S00_AXI_rresp(1 downto 0) => S00_AXI_rresp(1 downto 0),
      S00_AXI_rvalid => S00_AXI_rvalid,
      S00_AXI_wdata(511 downto 0) => S00_AXI_wdata(511 downto 0),
      S00_AXI_wlast => S00_AXI_wlast,
      S00_AXI_wready => S00_AXI_wready,
      S00_AXI_wstrb(63 downto 0) => S00_AXI_wstrb(63 downto 0),
      S00_AXI_wvalid => S00_AXI_wvalid,
      S01_AXI_araddr(38 downto 0) => S01_AXI_araddr(38 downto 0),
      S01_AXI_arburst(1 downto 0) => S01_AXI_arburst(1 downto 0),
      S01_AXI_arcache(3 downto 0) => S01_AXI_arcache(3 downto 0),
      S01_AXI_arid(3 downto 0) => S01_AXI_arid(3 downto 0),
      S01_AXI_arlen(7 downto 0) => S01_AXI_arlen(7 downto 0),
      S01_AXI_arlock(0) => S01_AXI_arlock(0),
      S01_AXI_arprot(2 downto 0) => S01_AXI_arprot(2 downto 0),
      S01_AXI_arqos(3 downto 0) => S01_AXI_arqos(3 downto 0),
      S01_AXI_arready => S01_AXI_arready,
      S01_AXI_arregion(3 downto 0) => S01_AXI_arregion(3 downto 0),
      S01_AXI_arvalid => S01_AXI_arvalid,
      S01_AXI_awaddr(38 downto 0) => S01_AXI_awaddr(38 downto 0),
      S01_AXI_awburst(1 downto 0) => S01_AXI_awburst(1 downto 0),
      S01_AXI_awcache(3 downto 0) => S01_AXI_awcache(3 downto 0),
      S01_AXI_awid(3 downto 0) => S01_AXI_awid(3 downto 0),
      S01_AXI_awlen(7 downto 0) => S01_AXI_awlen(7 downto 0),
      S01_AXI_awlock(0) => S01_AXI_awlock(0),
      S01_AXI_awprot(2 downto 0) => S01_AXI_awprot(2 downto 0),
      S01_AXI_awqos(3 downto 0) => S01_AXI_awqos(3 downto 0),
      S01_AXI_awready => S01_AXI_awready,
      S01_AXI_awregion(3 downto 0) => S01_AXI_awregion(3 downto 0),
      S01_AXI_awvalid => S01_AXI_awvalid,
      S01_AXI_bid(3 downto 0) => S01_AXI_bid(3 downto 0),
      S01_AXI_bready => S01_AXI_bready,
      S01_AXI_bresp(1 downto 0) => S01_AXI_bresp(1 downto 0),
      S01_AXI_bvalid => S01_AXI_bvalid,
      S01_AXI_rdata(511 downto 0) => S01_AXI_rdata(511 downto 0),
      S01_AXI_rid(3 downto 0) => S01_AXI_rid(3 downto 0),
      S01_AXI_rlast => S01_AXI_rlast,
      S01_AXI_rready => S01_AXI_rready,
      S01_AXI_rresp(1 downto 0) => S01_AXI_rresp(1 downto 0),
      S01_AXI_rvalid => S01_AXI_rvalid,
      S01_AXI_wdata(511 downto 0) => S01_AXI_wdata(511 downto 0),
      S01_AXI_wlast => S01_AXI_wlast,
      S01_AXI_wready => S01_AXI_wready,
      S01_AXI_wstrb(63 downto 0) => S01_AXI_wstrb(63 downto 0),
      S01_AXI_wvalid => S01_AXI_wvalid,
      S02_AXI_araddr(38 downto 0) => S02_AXI_araddr(38 downto 0),
      S02_AXI_arburst(1 downto 0) => S02_AXI_arburst(1 downto 0),
      S02_AXI_arcache(3 downto 0) => S02_AXI_arcache(3 downto 0),
      S02_AXI_arid(3 downto 0) => S02_AXI_arid(3 downto 0),
      S02_AXI_arlen(7 downto 0) => S02_AXI_arlen(7 downto 0),
      S02_AXI_arlock(0) => S02_AXI_arlock(0),
      S02_AXI_arprot(2 downto 0) => S02_AXI_arprot(2 downto 0),
      S02_AXI_arqos(3 downto 0) => S02_AXI_arqos(3 downto 0),
      S02_AXI_arready => S02_AXI_arready,
      S02_AXI_arregion(3 downto 0) => S02_AXI_arregion(3 downto 0),
      S02_AXI_arvalid => S02_AXI_arvalid,
      S02_AXI_awaddr(38 downto 0) => S02_AXI_awaddr(38 downto 0),
      S02_AXI_awburst(1 downto 0) => S02_AXI_awburst(1 downto 0),
      S02_AXI_awcache(3 downto 0) => S02_AXI_awcache(3 downto 0),
      S02_AXI_awid(3 downto 0) => S02_AXI_awid(3 downto 0),
      S02_AXI_awlen(7 downto 0) => S02_AXI_awlen(7 downto 0),
      S02_AXI_awlock(0) => S02_AXI_awlock(0),
      S02_AXI_awprot(2 downto 0) => S02_AXI_awprot(2 downto 0),
      S02_AXI_awqos(3 downto 0) => S02_AXI_awqos(3 downto 0),
      S02_AXI_awready => S02_AXI_awready,
      S02_AXI_awregion(3 downto 0) => S02_AXI_awregion(3 downto 0),
      S02_AXI_awvalid => S02_AXI_awvalid,
      S02_AXI_bid(3 downto 0) => S02_AXI_bid(3 downto 0),
      S02_AXI_bready => S02_AXI_bready,
      S02_AXI_bresp(1 downto 0) => S02_AXI_bresp(1 downto 0),
      S02_AXI_bvalid => S02_AXI_bvalid,
      S02_AXI_rdata(511 downto 0) => S02_AXI_rdata(511 downto 0),
      S02_AXI_rid(3 downto 0) => S02_AXI_rid(3 downto 0),
      S02_AXI_rlast => S02_AXI_rlast,
      S02_AXI_rready => S02_AXI_rready,
      S02_AXI_rresp(1 downto 0) => S02_AXI_rresp(1 downto 0),
      S02_AXI_rvalid => S02_AXI_rvalid,
      S02_AXI_wdata(511 downto 0) => S02_AXI_wdata(511 downto 0),
      S02_AXI_wlast => S02_AXI_wlast,
      S02_AXI_wready => S02_AXI_wready,
      S02_AXI_wstrb(63 downto 0) => S02_AXI_wstrb(63 downto 0),
      S02_AXI_wvalid => S02_AXI_wvalid,
      S03_AXI_araddr(38 downto 0) => S03_AXI_araddr(38 downto 0),
      S03_AXI_arburst(1 downto 0) => S03_AXI_arburst(1 downto 0),
      S03_AXI_arcache(3 downto 0) => S03_AXI_arcache(3 downto 0),
      S03_AXI_arid(3 downto 0) => S03_AXI_arid(3 downto 0),
      S03_AXI_arlen(7 downto 0) => S03_AXI_arlen(7 downto 0),
      S03_AXI_arlock(0) => S03_AXI_arlock(0),
      S03_AXI_arprot(2 downto 0) => S03_AXI_arprot(2 downto 0),
      S03_AXI_arqos(3 downto 0) => S03_AXI_arqos(3 downto 0),
      S03_AXI_arready => S03_AXI_arready,
      S03_AXI_arregion(3 downto 0) => S03_AXI_arregion(3 downto 0),
      S03_AXI_arsize(2 downto 0) => S03_AXI_arsize(2 downto 0),
      S03_AXI_arvalid => S03_AXI_arvalid,
      S03_AXI_awaddr(38 downto 0) => S03_AXI_awaddr(38 downto 0),
      S03_AXI_awburst(1 downto 0) => S03_AXI_awburst(1 downto 0),
      S03_AXI_awcache(3 downto 0) => S03_AXI_awcache(3 downto 0),
      S03_AXI_awid(3 downto 0) => S03_AXI_awid(3 downto 0),
      S03_AXI_awlen(7 downto 0) => S03_AXI_awlen(7 downto 0),
      S03_AXI_awlock(0) => S03_AXI_awlock(0),
      S03_AXI_awprot(2 downto 0) => S03_AXI_awprot(2 downto 0),
      S03_AXI_awqos(3 downto 0) => S03_AXI_awqos(3 downto 0),
      S03_AXI_awready => S03_AXI_awready,
      S03_AXI_awregion(3 downto 0) => S03_AXI_awregion(3 downto 0),
      S03_AXI_awsize(2 downto 0) => S03_AXI_awsize(2 downto 0),
      S03_AXI_awvalid => S03_AXI_awvalid,
      S03_AXI_bid(3 downto 0) => S03_AXI_bid(3 downto 0),
      S03_AXI_bready => S03_AXI_bready,
      S03_AXI_bresp(1 downto 0) => S03_AXI_bresp(1 downto 0),
      S03_AXI_bvalid => S03_AXI_bvalid,
      S03_AXI_rdata(511 downto 0) => S03_AXI_rdata(511 downto 0),
      S03_AXI_rid(3 downto 0) => S03_AXI_rid(3 downto 0),
      S03_AXI_rlast => S03_AXI_rlast,
      S03_AXI_rready => S03_AXI_rready,
      S03_AXI_rresp(1 downto 0) => S03_AXI_rresp(1 downto 0),
      S03_AXI_rvalid => S03_AXI_rvalid,
      S03_AXI_wdata(511 downto 0) => S03_AXI_wdata(511 downto 0),
      S03_AXI_wlast => S03_AXI_wlast,
      S03_AXI_wready => S03_AXI_wready,
      S03_AXI_wstrb(63 downto 0) => S03_AXI_wstrb(63 downto 0),
      S03_AXI_wvalid => S03_AXI_wvalid,
      S04_AXI_araddr(38 downto 0) => S04_AXI_araddr(38 downto 0),
      S04_AXI_arcache(3 downto 0) => S04_AXI_arcache(3 downto 0),
      S04_AXI_arlen(7 downto 0) => S04_AXI_arlen(7 downto 0),
      S04_AXI_arlock(0) => S04_AXI_arlock(0),
      S04_AXI_arprot(2 downto 0) => S04_AXI_arprot(2 downto 0),
      S04_AXI_arqos(3 downto 0) => S04_AXI_arqos(3 downto 0),
      S04_AXI_arready => S04_AXI_arready,
      S04_AXI_arregion(3 downto 0) => S04_AXI_arregion(3 downto 0),
      S04_AXI_arvalid => S04_AXI_arvalid,
      S04_AXI_awaddr(38 downto 0) => S04_AXI_awaddr(38 downto 0),
      S04_AXI_awcache(3 downto 0) => S04_AXI_awcache(3 downto 0),
      S04_AXI_awlen(7 downto 0) => S04_AXI_awlen(7 downto 0),
      S04_AXI_awlock(0) => S04_AXI_awlock(0),
      S04_AXI_awprot(2 downto 0) => S04_AXI_awprot(2 downto 0),
      S04_AXI_awqos(3 downto 0) => S04_AXI_awqos(3 downto 0),
      S04_AXI_awready => S04_AXI_awready,
      S04_AXI_awregion(3 downto 0) => S04_AXI_awregion(3 downto 0),
      S04_AXI_awvalid => S04_AXI_awvalid,
      S04_AXI_bready => S04_AXI_bready,
      S04_AXI_bresp(1 downto 0) => S04_AXI_bresp(1 downto 0),
      S04_AXI_bvalid => S04_AXI_bvalid,
      S04_AXI_rdata(511 downto 0) => S04_AXI_rdata(511 downto 0),
      S04_AXI_rlast => S04_AXI_rlast,
      S04_AXI_rready => S04_AXI_rready,
      S04_AXI_rresp(1 downto 0) => S04_AXI_rresp(1 downto 0),
      S04_AXI_rvalid => S04_AXI_rvalid,
      S04_AXI_wdata(511 downto 0) => S04_AXI_wdata(511 downto 0),
      S04_AXI_wlast => S04_AXI_wlast,
      S04_AXI_wready => S04_AXI_wready,
      S04_AXI_wstrb(63 downto 0) => S04_AXI_wstrb(63 downto 0),
      S04_AXI_wvalid => S04_AXI_wvalid,
      S05_AXI_araddr(38 downto 0) => S05_AXI_araddr(38 downto 0),
      S05_AXI_arcache(3 downto 0) => S05_AXI_arcache(3 downto 0),
      S05_AXI_arlen(7 downto 0) => S05_AXI_arlen(7 downto 0),
      S05_AXI_arlock(0) => S05_AXI_arlock(0),
      S05_AXI_arprot(2 downto 0) => S05_AXI_arprot(2 downto 0),
      S05_AXI_arqos(3 downto 0) => S05_AXI_arqos(3 downto 0),
      S05_AXI_arready => S05_AXI_arready,
      S05_AXI_arregion(3 downto 0) => S05_AXI_arregion(3 downto 0),
      S05_AXI_arvalid => S05_AXI_arvalid,
      S05_AXI_awaddr(38 downto 0) => S05_AXI_awaddr(38 downto 0),
      S05_AXI_awcache(3 downto 0) => S05_AXI_awcache(3 downto 0),
      S05_AXI_awlen(7 downto 0) => S05_AXI_awlen(7 downto 0),
      S05_AXI_awlock(0) => S05_AXI_awlock(0),
      S05_AXI_awprot(2 downto 0) => S05_AXI_awprot(2 downto 0),
      S05_AXI_awqos(3 downto 0) => S05_AXI_awqos(3 downto 0),
      S05_AXI_awready => S05_AXI_awready,
      S05_AXI_awregion(3 downto 0) => S05_AXI_awregion(3 downto 0),
      S05_AXI_awvalid => S05_AXI_awvalid,
      S05_AXI_bready => S05_AXI_bready,
      S05_AXI_bresp(1 downto 0) => S05_AXI_bresp(1 downto 0),
      S05_AXI_bvalid => S05_AXI_bvalid,
      S05_AXI_rdata(511 downto 0) => S05_AXI_rdata(511 downto 0),
      S05_AXI_rlast => S05_AXI_rlast,
      S05_AXI_rready => S05_AXI_rready,
      S05_AXI_rresp(1 downto 0) => S05_AXI_rresp(1 downto 0),
      S05_AXI_rvalid => S05_AXI_rvalid,
      S05_AXI_wdata(511 downto 0) => S05_AXI_wdata(511 downto 0),
      S05_AXI_wlast => S05_AXI_wlast,
      S05_AXI_wready => S05_AXI_wready,
      S05_AXI_wstrb(63 downto 0) => S05_AXI_wstrb(63 downto 0),
      S05_AXI_wvalid => S05_AXI_wvalid,
      S_AXI_CTRL_araddr(24 downto 0) => S_AXI_CTRL_araddr(24 downto 0),
      S_AXI_CTRL_arprot(2 downto 0) => S_AXI_CTRL_arprot(2 downto 0),
      S_AXI_CTRL_arready => S_AXI_CTRL_arready,
      S_AXI_CTRL_arvalid => S_AXI_CTRL_arvalid,
      S_AXI_CTRL_awaddr(24 downto 0) => S_AXI_CTRL_awaddr(24 downto 0),
      S_AXI_CTRL_awprot(2 downto 0) => S_AXI_CTRL_awprot(2 downto 0),
      S_AXI_CTRL_awready => S_AXI_CTRL_awready,
      S_AXI_CTRL_awvalid => S_AXI_CTRL_awvalid,
      S_AXI_CTRL_bready => S_AXI_CTRL_bready,
      S_AXI_CTRL_bresp(1 downto 0) => S_AXI_CTRL_bresp(1 downto 0),
      S_AXI_CTRL_bvalid => S_AXI_CTRL_bvalid,
      S_AXI_CTRL_rdata(31 downto 0) => S_AXI_CTRL_rdata(31 downto 0),
      S_AXI_CTRL_rready => S_AXI_CTRL_rready,
      S_AXI_CTRL_rresp(1 downto 0) => S_AXI_CTRL_rresp(1 downto 0),
      S_AXI_CTRL_rvalid => S_AXI_CTRL_rvalid,
      S_AXI_CTRL_wdata(31 downto 0) => S_AXI_CTRL_wdata(31 downto 0),
      S_AXI_CTRL_wready => S_AXI_CTRL_wready,
      S_AXI_CTRL_wstrb(3 downto 0) => S_AXI_CTRL_wstrb(3 downto 0),
      S_AXI_CTRL_wvalid => S_AXI_CTRL_wvalid,
      aclk => aclk,
      aclk1 => aclk1,
      aclk2 => aclk2,
      aresetn => aresetn,
      ddr4_mem00_sys_rst => ddr4_mem00_sys_rst,
      ddr4_mem00_ui_clk => ddr4_mem00_ui_clk,
      ddr4_mem01_sys_rst => ddr4_mem01_sys_rst,
      ddr4_mem01_ui_clk => ddr4_mem01_ui_clk,
      ddr4_mem02_sys_rst => ddr4_mem02_sys_rst,
      ddr4_mem02_ui_clk => ddr4_mem02_ui_clk,
      ddr4_mem_calib_complete => ddr4_mem_calib_complete,
      ddr4_mem_calib_vec(2 downto 0) => ddr4_mem_calib_vec(2 downto 0)
    );
end STRUCTURE;
