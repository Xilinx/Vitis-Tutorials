-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
-- Date        : Thu May  7 16:53:57 2020
-- Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
-- Command     : write_vhdl -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_d216_ddr4_mem01_0_stub.vhdl
-- Design      : bd_d216_ddr4_mem01_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xcu200-fsgd2104-2-e
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  Port ( 
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

end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture stub of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "sys_rst,c0_sys_clk_p,c0_sys_clk_n,c0_ddr4_act_n,c0_ddr4_adr[16:0],c0_ddr4_ba[1:0],c0_ddr4_bg[1:0],c0_ddr4_cke[0:0],c0_ddr4_odt[0:0],c0_ddr4_cs_n[0:0],c0_ddr4_ck_t[0:0],c0_ddr4_ck_c[0:0],c0_ddr4_reset_n,c0_ddr4_parity,c0_ddr4_dq[71:0],c0_ddr4_dqs_c[17:0],c0_ddr4_dqs_t[17:0],c0_init_calib_complete,c0_ddr4_ui_clk,c0_ddr4_ui_clk_sync_rst,addn_ui_clkout1,dbg_clk,c0_ddr4_s_axi_ctrl_awvalid,c0_ddr4_s_axi_ctrl_awready,c0_ddr4_s_axi_ctrl_awaddr[31:0],c0_ddr4_s_axi_ctrl_wvalid,c0_ddr4_s_axi_ctrl_wready,c0_ddr4_s_axi_ctrl_wdata[31:0],c0_ddr4_s_axi_ctrl_bvalid,c0_ddr4_s_axi_ctrl_bready,c0_ddr4_s_axi_ctrl_bresp[1:0],c0_ddr4_s_axi_ctrl_arvalid,c0_ddr4_s_axi_ctrl_arready,c0_ddr4_s_axi_ctrl_araddr[31:0],c0_ddr4_s_axi_ctrl_rvalid,c0_ddr4_s_axi_ctrl_rready,c0_ddr4_s_axi_ctrl_rdata[31:0],c0_ddr4_s_axi_ctrl_rresp[1:0],c0_ddr4_interrupt,c0_ddr4_aresetn,c0_ddr4_s_axi_awid[0:0],c0_ddr4_s_axi_awaddr[33:0],c0_ddr4_s_axi_awlen[7:0],c0_ddr4_s_axi_awsize[2:0],c0_ddr4_s_axi_awburst[1:0],c0_ddr4_s_axi_awlock[0:0],c0_ddr4_s_axi_awcache[3:0],c0_ddr4_s_axi_awprot[2:0],c0_ddr4_s_axi_awqos[3:0],c0_ddr4_s_axi_awvalid,c0_ddr4_s_axi_awready,c0_ddr4_s_axi_wdata[511:0],c0_ddr4_s_axi_wstrb[63:0],c0_ddr4_s_axi_wlast,c0_ddr4_s_axi_wvalid,c0_ddr4_s_axi_wready,c0_ddr4_s_axi_bready,c0_ddr4_s_axi_bid[0:0],c0_ddr4_s_axi_bresp[1:0],c0_ddr4_s_axi_bvalid,c0_ddr4_s_axi_arid[0:0],c0_ddr4_s_axi_araddr[33:0],c0_ddr4_s_axi_arlen[7:0],c0_ddr4_s_axi_arsize[2:0],c0_ddr4_s_axi_arburst[1:0],c0_ddr4_s_axi_arlock[0:0],c0_ddr4_s_axi_arcache[3:0],c0_ddr4_s_axi_arprot[2:0],c0_ddr4_s_axi_arqos[3:0],c0_ddr4_s_axi_arvalid,c0_ddr4_s_axi_arready,c0_ddr4_s_axi_rready,c0_ddr4_s_axi_rid[0:0],c0_ddr4_s_axi_rdata[511:0],c0_ddr4_s_axi_rresp[1:0],c0_ddr4_s_axi_rlast,c0_ddr4_s_axi_rvalid,dbg_bus[511:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "ddr4_v2_2_9,Vivado 2020.1";
begin
end;
