-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
-- Date        : Thu May  7 13:08:48 2020
-- Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
-- Command     : write_vhdl -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_runOnfpga_1_0_stub.vhdl
-- Design      : pfm_dynamic_runOnfpga_1_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xcu200-fsgd2104-2-e
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  Port ( 
    stall_start_ext : out STD_LOGIC;
    stall_done_ext : out STD_LOGIC;
    stall_start_str : out STD_LOGIC;
    stall_done_str : out STD_LOGIC;
    stall_start_int : out STD_LOGIC;
    stall_done_int : out STD_LOGIC;
    s_axi_control_AWADDR : in STD_LOGIC_VECTOR ( 6 downto 0 );
    s_axi_control_AWVALID : in STD_LOGIC;
    s_axi_control_AWREADY : out STD_LOGIC;
    s_axi_control_WDATA : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_control_WSTRB : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_control_WVALID : in STD_LOGIC;
    s_axi_control_WREADY : out STD_LOGIC;
    s_axi_control_BRESP : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_control_BVALID : out STD_LOGIC;
    s_axi_control_BREADY : in STD_LOGIC;
    s_axi_control_ARADDR : in STD_LOGIC_VECTOR ( 6 downto 0 );
    s_axi_control_ARVALID : in STD_LOGIC;
    s_axi_control_ARREADY : out STD_LOGIC;
    s_axi_control_RDATA : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_control_RRESP : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_control_RVALID : out STD_LOGIC;
    s_axi_control_RREADY : in STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst_n : in STD_LOGIC;
    event_done : out STD_LOGIC;
    interrupt : out STD_LOGIC;
    event_start : out STD_LOGIC;
    m_axi_maxiport0_AWADDR : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_maxiport0_AWLEN : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_maxiport0_AWSIZE : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport0_AWBURST : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport0_AWLOCK : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport0_AWREGION : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport0_AWCACHE : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport0_AWPROT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport0_AWQOS : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport0_AWVALID : out STD_LOGIC;
    m_axi_maxiport0_AWREADY : in STD_LOGIC;
    m_axi_maxiport0_WDATA : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_maxiport0_WSTRB : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_maxiport0_WLAST : out STD_LOGIC;
    m_axi_maxiport0_WVALID : out STD_LOGIC;
    m_axi_maxiport0_WREADY : in STD_LOGIC;
    m_axi_maxiport0_BRESP : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport0_BVALID : in STD_LOGIC;
    m_axi_maxiport0_BREADY : out STD_LOGIC;
    m_axi_maxiport0_ARADDR : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_maxiport0_ARLEN : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_maxiport0_ARSIZE : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport0_ARBURST : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport0_ARLOCK : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport0_ARREGION : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport0_ARCACHE : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport0_ARPROT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport0_ARQOS : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport0_ARVALID : out STD_LOGIC;
    m_axi_maxiport0_ARREADY : in STD_LOGIC;
    m_axi_maxiport0_RDATA : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_maxiport0_RRESP : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport0_RLAST : in STD_LOGIC;
    m_axi_maxiport0_RVALID : in STD_LOGIC;
    m_axi_maxiport0_RREADY : out STD_LOGIC;
    m_axi_maxiport1_AWADDR : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_maxiport1_AWLEN : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_maxiport1_AWSIZE : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport1_AWBURST : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport1_AWLOCK : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport1_AWREGION : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport1_AWCACHE : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport1_AWPROT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport1_AWQOS : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport1_AWVALID : out STD_LOGIC;
    m_axi_maxiport1_AWREADY : in STD_LOGIC;
    m_axi_maxiport1_WDATA : out STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_maxiport1_WSTRB : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_maxiport1_WLAST : out STD_LOGIC;
    m_axi_maxiport1_WVALID : out STD_LOGIC;
    m_axi_maxiport1_WREADY : in STD_LOGIC;
    m_axi_maxiport1_BRESP : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport1_BVALID : in STD_LOGIC;
    m_axi_maxiport1_BREADY : out STD_LOGIC;
    m_axi_maxiport1_ARADDR : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_maxiport1_ARLEN : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_maxiport1_ARSIZE : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport1_ARBURST : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport1_ARLOCK : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport1_ARREGION : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport1_ARCACHE : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport1_ARPROT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_axi_maxiport1_ARQOS : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_maxiport1_ARVALID : out STD_LOGIC;
    m_axi_maxiport1_ARREADY : in STD_LOGIC;
    m_axi_maxiport1_RDATA : in STD_LOGIC_VECTOR ( 511 downto 0 );
    m_axi_maxiport1_RRESP : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_maxiport1_RLAST : in STD_LOGIC;
    m_axi_maxiport1_RVALID : in STD_LOGIC;
    m_axi_maxiport1_RREADY : out STD_LOGIC
  );

end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture stub of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "stall_start_ext,stall_done_ext,stall_start_str,stall_done_str,stall_start_int,stall_done_int,s_axi_control_AWADDR[6:0],s_axi_control_AWVALID,s_axi_control_AWREADY,s_axi_control_WDATA[31:0],s_axi_control_WSTRB[3:0],s_axi_control_WVALID,s_axi_control_WREADY,s_axi_control_BRESP[1:0],s_axi_control_BVALID,s_axi_control_BREADY,s_axi_control_ARADDR[6:0],s_axi_control_ARVALID,s_axi_control_ARREADY,s_axi_control_RDATA[31:0],s_axi_control_RRESP[1:0],s_axi_control_RVALID,s_axi_control_RREADY,ap_clk,ap_rst_n,event_done,interrupt,event_start,m_axi_maxiport0_AWADDR[63:0],m_axi_maxiport0_AWLEN[7:0],m_axi_maxiport0_AWSIZE[2:0],m_axi_maxiport0_AWBURST[1:0],m_axi_maxiport0_AWLOCK[1:0],m_axi_maxiport0_AWREGION[3:0],m_axi_maxiport0_AWCACHE[3:0],m_axi_maxiport0_AWPROT[2:0],m_axi_maxiport0_AWQOS[3:0],m_axi_maxiport0_AWVALID,m_axi_maxiport0_AWREADY,m_axi_maxiport0_WDATA[511:0],m_axi_maxiport0_WSTRB[63:0],m_axi_maxiport0_WLAST,m_axi_maxiport0_WVALID,m_axi_maxiport0_WREADY,m_axi_maxiport0_BRESP[1:0],m_axi_maxiport0_BVALID,m_axi_maxiport0_BREADY,m_axi_maxiport0_ARADDR[63:0],m_axi_maxiport0_ARLEN[7:0],m_axi_maxiport0_ARSIZE[2:0],m_axi_maxiport0_ARBURST[1:0],m_axi_maxiport0_ARLOCK[1:0],m_axi_maxiport0_ARREGION[3:0],m_axi_maxiport0_ARCACHE[3:0],m_axi_maxiport0_ARPROT[2:0],m_axi_maxiport0_ARQOS[3:0],m_axi_maxiport0_ARVALID,m_axi_maxiport0_ARREADY,m_axi_maxiport0_RDATA[511:0],m_axi_maxiport0_RRESP[1:0],m_axi_maxiport0_RLAST,m_axi_maxiport0_RVALID,m_axi_maxiport0_RREADY,m_axi_maxiport1_AWADDR[63:0],m_axi_maxiport1_AWLEN[7:0],m_axi_maxiport1_AWSIZE[2:0],m_axi_maxiport1_AWBURST[1:0],m_axi_maxiport1_AWLOCK[1:0],m_axi_maxiport1_AWREGION[3:0],m_axi_maxiport1_AWCACHE[3:0],m_axi_maxiport1_AWPROT[2:0],m_axi_maxiport1_AWQOS[3:0],m_axi_maxiport1_AWVALID,m_axi_maxiport1_AWREADY,m_axi_maxiport1_WDATA[511:0],m_axi_maxiport1_WSTRB[63:0],m_axi_maxiport1_WLAST,m_axi_maxiport1_WVALID,m_axi_maxiport1_WREADY,m_axi_maxiport1_BRESP[1:0],m_axi_maxiport1_BVALID,m_axi_maxiport1_BREADY,m_axi_maxiport1_ARADDR[63:0],m_axi_maxiport1_ARLEN[7:0],m_axi_maxiport1_ARSIZE[2:0],m_axi_maxiport1_ARBURST[1:0],m_axi_maxiport1_ARLOCK[1:0],m_axi_maxiport1_ARREGION[3:0],m_axi_maxiport1_ARCACHE[3:0],m_axi_maxiport1_ARPROT[2:0],m_axi_maxiport1_ARQOS[3:0],m_axi_maxiport1_ARVALID,m_axi_maxiport1_ARREADY,m_axi_maxiport1_RDATA[511:0],m_axi_maxiport1_RRESP[1:0],m_axi_maxiport1_RLAST,m_axi_maxiport1_RVALID,m_axi_maxiport1_RREADY";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "runOnfpga,Vivado 2020.1";
begin
end;
