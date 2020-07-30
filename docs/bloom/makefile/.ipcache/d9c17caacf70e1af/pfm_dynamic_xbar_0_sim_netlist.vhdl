-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
-- Date        : Wed May  6 15:17:13 2020
-- Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
-- Command     : write_vhdl -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_xbar_0_sim_netlist.vhdl
-- Design      : pfm_dynamic_xbar_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xcu200-fsgd2104-2-e
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_addr_arbiter_sasd is
  port (
    m_valid_i : out STD_LOGIC;
    SR : out STD_LOGIC_VECTOR ( 0 to 0 );
    aa_grant_rnw : out STD_LOGIC;
    any_error : out STD_LOGIC;
    Q : out STD_LOGIC_VECTOR ( 27 downto 0 );
    D : out STD_LOGIC_VECTOR ( 2 downto 0 );
    m_ready_d0 : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wready : out STD_LOGIC_VECTOR ( 0 to 0 );
    \m_ready_d_reg[2]\ : out STD_LOGIC_VECTOR ( 2 downto 0 );
    \m_atarget_enc_reg[1]\ : out STD_LOGIC;
    m_axi_bready : out STD_LOGIC_VECTOR ( 1 downto 0 );
    \gen_no_arbiter.grant_rnw_reg_0\ : out STD_LOGIC;
    m_axi_wvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_wvalid_0_sp_1 : out STD_LOGIC;
    \gen_no_arbiter.grant_rnw_reg_1\ : out STD_LOGIC;
    E : out STD_LOGIC_VECTOR ( 0 to 0 );
    aresetn_d_reg : out STD_LOGIC_VECTOR ( 0 to 0 );
    r_transfer_en : out STD_LOGIC;
    m_axi_arvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    mi_arvalid_en : out STD_LOGIC;
    s_axi_awready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    aresetn_d_reg_0 : out STD_LOGIC;
    \gen_axilite.s_axi_bvalid_i_reg\ : out STD_LOGIC;
    aclk : in STD_LOGIC;
    \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0\ : in STD_LOGIC;
    \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1\ : in STD_LOGIC;
    s_axi_awvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    aresetn_d : in STD_LOGIC;
    \gen_axilite.s_axi_bvalid_i_reg_0\ : in STD_LOGIC_VECTOR ( 2 downto 0 );
    \gen_axilite.s_axi_bvalid_i_reg_1\ : in STD_LOGIC_VECTOR ( 2 downto 0 );
    \f_mux_return__3\ : in STD_LOGIC;
    \f_mux_return__1\ : in STD_LOGIC;
    f_mux_return : in STD_LOGIC;
    m_atarget_enc : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    mi_wready : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bready : in STD_LOGIC_VECTOR ( 0 to 0 );
    mi_bvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    \m_axi_arvalid[0]\ : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rready : in STD_LOGIC_VECTOR ( 0 to 0 );
    sr_rvalid : in STD_LOGIC;
    \m_ready_d_reg[0]\ : in STD_LOGIC_VECTOR ( 0 to 0 );
    \f_mux_return__0\ : in STD_LOGIC;
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_addr_arbiter_sasd;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_addr_arbiter_sasd is
  signal \^q\ : STD_LOGIC_VECTOR ( 27 downto 0 );
  signal \^sr\ : STD_LOGIC_VECTOR ( 0 to 0 );
  signal \^aa_grant_rnw\ : STD_LOGIC;
  signal \gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I\ : STD_LOGIC_VECTOR ( 0 to 0 );
  signal \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\ : STD_LOGIC;
  signal \gen_axilite.s_axi_bvalid_i_i_2_n_0\ : STD_LOGIC;
  signal \gen_no_arbiter.grant_rnw_i_1_n_0\ : STD_LOGIC;
  signal \gen_no_arbiter.m_grant_hot_i[0]_inv_i_1_n_0\ : STD_LOGIC;
  signal \gen_no_arbiter.m_grant_hot_i[0]_inv_i_2_n_0\ : STD_LOGIC;
  signal \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0\ : STD_LOGIC;
  signal \gen_no_arbiter.m_valid_i_i_1_n_0\ : STD_LOGIC;
  signal \gen_no_arbiter.s_ready_i[0]_i_1_n_0\ : STD_LOGIC;
  signal \^m_atarget_enc_reg[1]\ : STD_LOGIC;
  signal \m_atarget_hot[2]_i_4_n_0\ : STD_LOGIC;
  signal \^m_ready_d0\ : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal \m_ready_d[2]_i_5_n_0\ : STD_LOGIC;
  signal \^m_valid_i\ : STD_LOGIC;
  signal p_0_in1_in : STD_LOGIC;
  signal p_0_out : STD_LOGIC;
  signal s_amesg : STD_LOGIC_VECTOR ( 41 downto 1 );
  signal s_arvalid_reg : STD_LOGIC;
  signal \s_arvalid_reg_reg_n_0_[0]\ : STD_LOGIC;
  signal s_awvalid_reg : STD_LOGIC;
  signal s_awvalid_reg0 : STD_LOGIC;
  signal s_axi_wvalid_0_sn_1 : STD_LOGIC;
  signal s_ready_i : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \gen_axilite.s_axi_bvalid_i_i_3\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \gen_axilite.s_axi_rvalid_i_i_2\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \gen_no_arbiter.m_grant_hot_i[0]_inv_i_2\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \gen_no_arbiter.s_ready_i[0]_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \m_atarget_hot[0]_i_1\ : label is "soft_lutpair11";
  attribute SOFT_HLUTNM of \m_axi_arvalid[0]_INST_0\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \m_axi_arvalid[1]_INST_0\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \m_axi_awvalid[0]_INST_0\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \m_axi_awvalid[1]_INST_0\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \m_axi_wvalid[0]_INST_0\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \m_payload_i[34]_i_1\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \m_ready_d[0]_i_1\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \m_ready_d[1]_i_1\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \m_ready_d[1]_i_2\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \m_ready_d[2]_i_5\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \m_ready_d[2]_i_6\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \m_ready_d[2]_i_7\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of m_valid_i_i_3 : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \s_arvalid_reg[0]_i_2\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \s_awvalid_reg[0]_i_1\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \s_axi_awready[0]_INST_0\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \s_axi_bvalid[0]_INST_0\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \s_axi_rvalid[0]_INST_0\ : label is "soft_lutpair11";
begin
  Q(27 downto 0) <= \^q\(27 downto 0);
  SR(0) <= \^sr\(0);
  aa_grant_rnw <= \^aa_grant_rnw\;
  \m_atarget_enc_reg[1]\ <= \^m_atarget_enc_reg[1]\;
  m_ready_d0(1 downto 0) <= \^m_ready_d0\(1 downto 0);
  m_valid_i <= \^m_valid_i\;
  s_axi_wvalid_0_sp_1 <= s_axi_wvalid_0_sn_1;
\gen_axilite.s_axi_bvalid_i_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5A5A5E5A50505050"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_i_2_n_0\,
      I1 => s_axi_wvalid_0_sn_1,
      I2 => mi_bvalid(0),
      I3 => \gen_axilite.s_axi_bvalid_i_reg_1\(2),
      I4 => \gen_axilite.s_axi_bvalid_i_reg_0\(2),
      I5 => mi_wready(0),
      O => \gen_axilite.s_axi_bvalid_i_reg\
    );
\gen_axilite.s_axi_bvalid_i_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000008000000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_1\(2),
      I1 => mi_bvalid(0),
      I2 => \gen_axilite.s_axi_bvalid_i_reg_0\(0),
      I3 => s_axi_bready(0),
      I4 => \^m_valid_i\,
      I5 => \^aa_grant_rnw\,
      O => \gen_axilite.s_axi_bvalid_i_i_2_n_0\
    );
\gen_axilite.s_axi_bvalid_i_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0008"
    )
        port map (
      I0 => s_axi_wvalid(0),
      I1 => \^m_valid_i\,
      I2 => \^aa_grant_rnw\,
      I3 => \gen_axilite.s_axi_bvalid_i_reg_0\(1),
      O => s_axi_wvalid_0_sn_1
    );
\gen_axilite.s_axi_rvalid_i_i_2\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"08"
    )
        port map (
      I0 => \^aa_grant_rnw\,
      I1 => \^m_valid_i\,
      I2 => \m_axi_arvalid[0]\(1),
      O => mi_arvalid_en
    );
\gen_no_arbiter.grant_rnw_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"DFCFDFFF10001000"
    )
        port map (
      I0 => s_awvalid_reg,
      I1 => \^m_valid_i\,
      I2 => p_0_in1_in,
      I3 => s_axi_arvalid(0),
      I4 => s_axi_awvalid(0),
      I5 => \^aa_grant_rnw\,
      O => \gen_no_arbiter.grant_rnw_i_1_n_0\
    );
\gen_no_arbiter.grant_rnw_reg\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => \gen_no_arbiter.grant_rnw_i_1_n_0\,
      Q => \^aa_grant_rnw\,
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i[10]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(9),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(9),
      O => s_amesg(10)
    );
\gen_no_arbiter.m_amesg_i[11]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(10),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(10),
      O => s_amesg(11)
    );
\gen_no_arbiter.m_amesg_i[12]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(11),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(11),
      O => s_amesg(12)
    );
\gen_no_arbiter.m_amesg_i[13]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(12),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(12),
      O => s_amesg(13)
    );
\gen_no_arbiter.m_amesg_i[14]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(13),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(13),
      O => s_amesg(14)
    );
\gen_no_arbiter.m_amesg_i[15]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(14),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(14),
      O => s_amesg(15)
    );
\gen_no_arbiter.m_amesg_i[16]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(15),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(15),
      O => s_amesg(16)
    );
\gen_no_arbiter.m_amesg_i[17]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(16),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(16),
      O => s_amesg(17)
    );
\gen_no_arbiter.m_amesg_i[18]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(17),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(17),
      O => s_amesg(18)
    );
\gen_no_arbiter.m_amesg_i[19]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(18),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(18),
      O => s_amesg(19)
    );
\gen_no_arbiter.m_amesg_i[1]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(0),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(0),
      O => s_amesg(1)
    );
\gen_no_arbiter.m_amesg_i[20]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(19),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(19),
      O => s_amesg(20)
    );
\gen_no_arbiter.m_amesg_i[21]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(20),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(20),
      O => s_amesg(21)
    );
\gen_no_arbiter.m_amesg_i[22]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(21),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(21),
      O => s_amesg(22)
    );
\gen_no_arbiter.m_amesg_i[23]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(22),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(22),
      O => s_amesg(23)
    );
\gen_no_arbiter.m_amesg_i[24]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(23),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(23),
      O => s_amesg(24)
    );
\gen_no_arbiter.m_amesg_i[25]_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => aresetn_d,
      O => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i[25]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(24),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(24),
      O => s_amesg(25)
    );
\gen_no_arbiter.m_amesg_i[2]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(1),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(1),
      O => s_amesg(2)
    );
\gen_no_arbiter.m_amesg_i[39]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_arprot(0),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awprot(0),
      O => s_amesg(39)
    );
\gen_no_arbiter.m_amesg_i[3]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(2),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(2),
      O => s_amesg(3)
    );
\gen_no_arbiter.m_amesg_i[40]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_arprot(1),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awprot(1),
      O => s_amesg(40)
    );
\gen_no_arbiter.m_amesg_i[41]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_arprot(2),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awprot(2),
      O => s_amesg(41)
    );
\gen_no_arbiter.m_amesg_i[4]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(3),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(3),
      O => s_amesg(4)
    );
\gen_no_arbiter.m_amesg_i[5]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(4),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(4),
      O => s_amesg(5)
    );
\gen_no_arbiter.m_amesg_i[6]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(5),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(5),
      O => s_amesg(6)
    );
\gen_no_arbiter.m_amesg_i[7]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(6),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(6),
      O => s_amesg(7)
    );
\gen_no_arbiter.m_amesg_i[8]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(7),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(7),
      O => s_amesg(8)
    );
\gen_no_arbiter.m_amesg_i[9]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FB08"
    )
        port map (
      I0 => s_axi_araddr(8),
      I1 => s_axi_arvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_awaddr(8),
      O => s_amesg(9)
    );
\gen_no_arbiter.m_amesg_i_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(10),
      Q => \^q\(9),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(11),
      Q => \^q\(10),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(12),
      Q => \^q\(11),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(13),
      Q => \^q\(12),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(14),
      Q => \^q\(13),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(15),
      Q => \^q\(14),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(16),
      Q => \^q\(15),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(17),
      Q => \^q\(16),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(18),
      Q => \^q\(17),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(19),
      Q => \^q\(18),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(1),
      Q => \^q\(0),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(20),
      Q => \^q\(19),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(21),
      Q => \^q\(20),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(22),
      Q => \^q\(21),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(23),
      Q => \^q\(22),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(24),
      Q => \^q\(23),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(25),
      Q => \^q\(24),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(2),
      Q => \^q\(1),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[39]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(39),
      Q => \^q\(25),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(3),
      Q => \^q\(2),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[40]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(40),
      Q => \^q\(26),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[41]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(41),
      Q => \^q\(27),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(4),
      Q => \^q\(3),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(5),
      Q => \^q\(4),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(6),
      Q => \^q\(5),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(7),
      Q => \^q\(6),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(8),
      Q => \^q\(7),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_amesg_i_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => p_0_in1_in,
      D => s_amesg(9),
      Q => \^q\(8),
      R => \^sr\(0)
    );
\gen_no_arbiter.m_grant_hot_i[0]_inv_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"DDDDD555D555D555"
    )
        port map (
      I0 => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_2_n_0\,
      I1 => \^m_valid_i\,
      I2 => \^m_ready_d0\(1),
      I3 => \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0\,
      I4 => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0\,
      I5 => \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1\,
      O => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_1_n_0\
    );
\gen_no_arbiter.m_grant_hot_i[0]_inv_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"0FEF0000"
    )
        port map (
      I0 => s_axi_awvalid(0),
      I1 => s_axi_arvalid(0),
      I2 => p_0_in1_in,
      I3 => \^m_valid_i\,
      I4 => aresetn_d,
      O => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_2_n_0\
    );
\gen_no_arbiter.m_grant_hot_i[0]_inv_i_4\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0F08"
    )
        port map (
      I0 => f_mux_return,
      I1 => \^m_valid_i\,
      I2 => \^aa_grant_rnw\,
      I3 => \gen_axilite.s_axi_bvalid_i_reg_0\(2),
      O => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0\
    );
\gen_no_arbiter.m_grant_hot_i_reg[0]_inv\: unisim.vcomponents.FDRE
    generic map(
      INIT => '1'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_1_n_0\,
      Q => p_0_in1_in,
      R => '0'
    );
\gen_no_arbiter.m_valid_i_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"11111DDD1DDD1DDD"
    )
        port map (
      I0 => p_0_in1_in,
      I1 => \^m_valid_i\,
      I2 => \^m_ready_d0\(1),
      I3 => \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0\,
      I4 => \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0\,
      I5 => \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1\,
      O => \gen_no_arbiter.m_valid_i_i_1_n_0\
    );
\gen_no_arbiter.m_valid_i_reg\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_no_arbiter.m_valid_i_i_1_n_0\,
      Q => \^m_valid_i\,
      R => \^sr\(0)
    );
\gen_no_arbiter.s_ready_i[0]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"10"
    )
        port map (
      I0 => \^m_valid_i\,
      I1 => p_0_in1_in,
      I2 => aresetn_d,
      O => \gen_no_arbiter.s_ready_i[0]_i_1_n_0\
    );
\gen_no_arbiter.s_ready_i_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_no_arbiter.s_ready_i[0]_i_1_n_0\,
      Q => s_ready_i,
      R => '0'
    );
\m_atarget_enc[0]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"08000000"
    )
        port map (
      I0 => aresetn_d,
      I1 => \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\,
      I2 => \^q\(17),
      I3 => \^q\(18),
      I4 => \^q\(16),
      O => aresetn_d_reg_0
    );
\m_atarget_enc[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"0000F7FF"
    )
        port map (
      I0 => \^q\(16),
      I1 => \^q\(18),
      I2 => \^q\(17),
      I3 => \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\,
      I4 => \gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I\(0),
      O => any_error
    );
\m_atarget_hot[0]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"4"
    )
        port map (
      I0 => p_0_in1_in,
      I1 => \gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I\(0),
      O => D(0)
    );
\m_atarget_hot[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"04000000"
    )
        port map (
      I0 => p_0_in1_in,
      I1 => \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\,
      I2 => \^q\(17),
      I3 => \^q\(18),
      I4 => \^q\(16),
      O => D(1)
    );
\m_atarget_hot[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"000000000000F7FF"
    )
        port map (
      I0 => \^q\(16),
      I1 => \^q\(18),
      I2 => \^q\(17),
      I3 => \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\,
      I4 => p_0_in1_in,
      I5 => \gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I\(0),
      O => D(2)
    );
\m_atarget_hot[2]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0001000000000000"
    )
        port map (
      I0 => \^q\(21),
      I1 => \^q\(22),
      I2 => \^q\(19),
      I3 => \^q\(20),
      I4 => \^q\(24),
      I5 => \^q\(23),
      O => \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\
    );
\m_atarget_hot[2]_i_3\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"1010F000"
    )
        port map (
      I0 => \^q\(16),
      I1 => \^q\(17),
      I2 => \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3\,
      I3 => \m_atarget_hot[2]_i_4_n_0\,
      I4 => \^q\(18),
      O => \gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I\(0)
    );
\m_atarget_hot[2]_i_4\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFF0001"
    )
        port map (
      I0 => \^q\(14),
      I1 => \^q\(15),
      I2 => \^q\(12),
      I3 => \^q\(13),
      I4 => \^q\(16),
      I5 => \^q\(17),
      O => \m_atarget_hot[2]_i_4_n_0\
    );
\m_axi_arvalid[0]_INST_0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"2000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_1\(0),
      I1 => \m_axi_arvalid[0]\(1),
      I2 => \^m_valid_i\,
      I3 => \^aa_grant_rnw\,
      O => m_axi_arvalid(0)
    );
\m_axi_arvalid[1]_INST_0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"2000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_1\(1),
      I1 => \m_axi_arvalid[0]\(1),
      I2 => \^m_valid_i\,
      I3 => \^aa_grant_rnw\,
      O => m_axi_arvalid(1)
    );
\m_axi_awvalid[0]_INST_0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0400"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_0\(2),
      I1 => \gen_axilite.s_axi_bvalid_i_reg_1\(0),
      I2 => \^aa_grant_rnw\,
      I3 => \^m_valid_i\,
      O => m_axi_awvalid(0)
    );
\m_axi_awvalid[1]_INST_0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0400"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_0\(2),
      I1 => \gen_axilite.s_axi_bvalid_i_reg_1\(1),
      I2 => \^aa_grant_rnw\,
      I3 => \^m_valid_i\,
      O => m_axi_awvalid(1)
    );
\m_axi_bready[0]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00004000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_0\(0),
      I1 => \gen_axilite.s_axi_bvalid_i_reg_1\(0),
      I2 => s_axi_bready(0),
      I3 => \^m_valid_i\,
      I4 => \^aa_grant_rnw\,
      O => m_axi_bready(0)
    );
\m_axi_bready[1]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00004000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_0\(0),
      I1 => \gen_axilite.s_axi_bvalid_i_reg_1\(1),
      I2 => s_axi_bready(0),
      I3 => \^m_valid_i\,
      I4 => \^aa_grant_rnw\,
      O => m_axi_bready(1)
    );
\m_axi_wvalid[0]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"10000000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_0\(1),
      I1 => \^aa_grant_rnw\,
      I2 => \^m_valid_i\,
      I3 => s_axi_wvalid(0),
      I4 => \gen_axilite.s_axi_bvalid_i_reg_1\(0),
      O => m_axi_wvalid(0)
    );
\m_axi_wvalid[1]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"10000000"
    )
        port map (
      I0 => \gen_axilite.s_axi_bvalid_i_reg_0\(1),
      I1 => \^aa_grant_rnw\,
      I2 => \^m_valid_i\,
      I3 => s_axi_wvalid(0),
      I4 => \gen_axilite.s_axi_bvalid_i_reg_1\(1),
      O => m_axi_wvalid(1)
    );
\m_payload_i[34]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"0800FFFF"
    )
        port map (
      I0 => \^aa_grant_rnw\,
      I1 => \^m_valid_i\,
      I2 => \m_axi_arvalid[0]\(0),
      I3 => s_axi_rready(0),
      I4 => sr_rvalid,
      O => E(0)
    );
\m_ready_d[0]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFF2000"
    )
        port map (
      I0 => \f_mux_return__3\,
      I1 => \^aa_grant_rnw\,
      I2 => \^m_valid_i\,
      I3 => s_axi_bready(0),
      I4 => \gen_axilite.s_axi_bvalid_i_reg_0\(0),
      O => \m_ready_d_reg[2]\(0)
    );
\m_ready_d[0]_i_1__0\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFF80000000"
    )
        port map (
      I0 => s_axi_rready(0),
      I1 => \^m_valid_i\,
      I2 => \^aa_grant_rnw\,
      I3 => \m_ready_d_reg[0]\(0),
      I4 => sr_rvalid,
      I5 => \m_axi_arvalid[0]\(0),
      O => \^m_ready_d0\(0)
    );
\m_ready_d[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFF2000"
    )
        port map (
      I0 => \f_mux_return__1\,
      I1 => \^aa_grant_rnw\,
      I2 => \^m_valid_i\,
      I3 => s_axi_wvalid(0),
      I4 => \gen_axilite.s_axi_bvalid_i_reg_0\(1),
      O => \m_ready_d_reg[2]\(1)
    );
\m_ready_d[1]_i_1__0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"8F"
    )
        port map (
      I0 => \^m_ready_d0\(0),
      I1 => \^m_ready_d0\(1),
      I2 => aresetn_d,
      O => aresetn_d_reg(0)
    );
\m_ready_d[1]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FF80"
    )
        port map (
      I0 => \^aa_grant_rnw\,
      I1 => \^m_valid_i\,
      I2 => \f_mux_return__0\,
      I3 => \m_axi_arvalid[0]\(1),
      O => \^m_ready_d0\(1)
    );
\m_ready_d[2]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => \^m_atarget_enc_reg[1]\,
      I1 => \gen_axilite.s_axi_bvalid_i_reg_0\(2),
      O => \m_ready_d_reg[2]\(2)
    );
\m_ready_d[2]_i_3\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"2A0A220228082000"
    )
        port map (
      I0 => \m_ready_d[2]_i_5_n_0\,
      I1 => m_atarget_enc(1),
      I2 => m_atarget_enc(0),
      I3 => m_axi_awready(1),
      I4 => mi_wready(0),
      I5 => m_axi_awready(0),
      O => \^m_atarget_enc_reg[1]\
    );
\m_ready_d[2]_i_5\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => \^m_valid_i\,
      I1 => \^aa_grant_rnw\,
      O => \m_ready_d[2]_i_5_n_0\
    );
\m_ready_d[2]_i_6\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"40"
    )
        port map (
      I0 => \^aa_grant_rnw\,
      I1 => \^m_valid_i\,
      I2 => s_axi_bready(0),
      O => \gen_no_arbiter.grant_rnw_reg_0\
    );
\m_ready_d[2]_i_7\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"40"
    )
        port map (
      I0 => \^aa_grant_rnw\,
      I1 => \^m_valid_i\,
      I2 => s_axi_wvalid(0),
      O => \gen_no_arbiter.grant_rnw_reg_1\
    );
m_valid_i_i_3: unisim.vcomponents.LUT3
    generic map(
      INIT => X"08"
    )
        port map (
      I0 => \^aa_grant_rnw\,
      I1 => \^m_valid_i\,
      I2 => \m_axi_arvalid[0]\(0),
      O => r_transfer_en
    );
\s_arvalid_reg[0]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"B"
    )
        port map (
      I0 => s_ready_i,
      I1 => aresetn_d,
      O => s_arvalid_reg
    );
\s_arvalid_reg[0]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => s_axi_arvalid(0),
      I1 => s_awvalid_reg,
      O => p_0_out
    );
\s_arvalid_reg_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => p_0_out,
      Q => \s_arvalid_reg_reg_n_0_[0]\,
      R => s_arvalid_reg
    );
\s_awvalid_reg[0]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"4044"
    )
        port map (
      I0 => \s_arvalid_reg_reg_n_0_[0]\,
      I1 => s_axi_awvalid(0),
      I2 => s_awvalid_reg,
      I3 => s_axi_arvalid(0),
      O => s_awvalid_reg0
    );
\s_awvalid_reg_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => s_awvalid_reg0,
      Q => s_awvalid_reg,
      R => s_arvalid_reg
    );
\s_axi_arready[0]_INST_0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => s_ready_i,
      I1 => \^aa_grant_rnw\,
      O => s_axi_arready(0)
    );
\s_axi_awready[0]_INST_0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => s_ready_i,
      I1 => \^aa_grant_rnw\,
      O => s_axi_awready(0)
    );
\s_axi_bvalid[0]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00020000"
    )
        port map (
      I0 => \^m_valid_i\,
      I1 => \^aa_grant_rnw\,
      I2 => p_0_in1_in,
      I3 => \gen_axilite.s_axi_bvalid_i_reg_0\(0),
      I4 => \f_mux_return__3\,
      O => s_axi_bvalid(0)
    );
\s_axi_rvalid[0]_INST_0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"4"
    )
        port map (
      I0 => p_0_in1_in,
      I1 => sr_rvalid,
      O => s_axi_rvalid(0)
    );
\s_axi_wready[0]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00020000"
    )
        port map (
      I0 => \^m_valid_i\,
      I1 => \^aa_grant_rnw\,
      I2 => p_0_in1_in,
      I3 => \gen_axilite.s_axi_bvalid_i_reg_0\(1),
      I4 => \f_mux_return__1\,
      O => s_axi_wready(0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_decerr_slave is
  port (
    mi_bvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    mi_wready : out STD_LOGIC_VECTOR ( 0 to 0 );
    aa_rvalid : out STD_LOGIC;
    f_mux_return : out STD_LOGIC;
    \f_mux_return__1\ : out STD_LOGIC;
    \f_mux_return__3\ : out STD_LOGIC;
    \f_mux_return__0\ : out STD_LOGIC;
    SR : in STD_LOGIC_VECTOR ( 0 to 0 );
    \gen_axilite.s_axi_bvalid_i_reg_0\ : in STD_LOGIC;
    aclk : in STD_LOGIC;
    m_axi_rvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    r_transfer_en : in STD_LOGIC;
    m_atarget_enc : in STD_LOGIC_VECTOR ( 1 downto 0 );
    aresetn_d : in STD_LOGIC;
    mi_arvalid_en : in STD_LOGIC;
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    aa_rready : in STD_LOGIC;
    \gen_axilite.s_axi_awready_i_reg_0\ : in STD_LOGIC;
    \gen_axilite.s_axi_awready_i_reg_1\ : in STD_LOGIC_VECTOR ( 0 to 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_decerr_slave;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_decerr_slave is
  signal \gen_axilite.s_axi_arready_i_i_1_n_0\ : STD_LOGIC;
  signal \gen_axilite.s_axi_awready_i_i_1_n_0\ : STD_LOGIC;
  signal \gen_axilite.s_axi_rvalid_i_i_1_n_0\ : STD_LOGIC;
  signal mi_arready : STD_LOGIC_VECTOR ( 2 to 2 );
  signal \^mi_bvalid\ : STD_LOGIC_VECTOR ( 0 to 0 );
  signal mi_rvalid : STD_LOGIC_VECTOR ( 2 to 2 );
  signal \^mi_wready\ : STD_LOGIC_VECTOR ( 0 to 0 );
begin
  mi_bvalid(0) <= \^mi_bvalid\(0);
  mi_wready(0) <= \^mi_wready\(0);
\gen_axilite.s_axi_arready_i_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"AA2A00AA"
    )
        port map (
      I0 => aresetn_d,
      I1 => mi_arvalid_en,
      I2 => Q(0),
      I3 => mi_rvalid(2),
      I4 => mi_arready(2),
      O => \gen_axilite.s_axi_arready_i_i_1_n_0\
    );
\gen_axilite.s_axi_arready_i_reg\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_axilite.s_axi_arready_i_i_1_n_0\,
      Q => mi_arready(2),
      R => '0'
    );
\gen_axilite.s_axi_awready_i_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FDFF0200"
    )
        port map (
      I0 => \gen_axilite.s_axi_awready_i_reg_0\,
      I1 => \^mi_bvalid\(0),
      I2 => \gen_axilite.s_axi_awready_i_reg_1\(0),
      I3 => Q(0),
      I4 => \^mi_wready\(0),
      O => \gen_axilite.s_axi_awready_i_i_1_n_0\
    );
\gen_axilite.s_axi_awready_i_reg\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_axilite.s_axi_awready_i_i_1_n_0\,
      Q => \^mi_wready\(0),
      R => SR(0)
    );
\gen_axilite.s_axi_bvalid_i_reg\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_axilite.s_axi_bvalid_i_reg_0\,
      Q => \^mi_bvalid\(0),
      R => SR(0)
    );
\gen_axilite.s_axi_rvalid_i_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"5FC05F00"
    )
        port map (
      I0 => aa_rready,
      I1 => mi_arvalid_en,
      I2 => Q(0),
      I3 => mi_rvalid(2),
      I4 => mi_arready(2),
      O => \gen_axilite.s_axi_rvalid_i_i_1_n_0\
    );
\gen_axilite.s_axi_rvalid_i_reg\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \gen_axilite.s_axi_rvalid_i_i_1_n_0\,
      Q => mi_rvalid(2),
      R => SR(0)
    );
\gen_no_arbiter.m_grant_hot_i[0]_inv_i_5\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00CCF0AA"
    )
        port map (
      I0 => m_axi_awready(0),
      I1 => \^mi_wready\(0),
      I2 => m_axi_awready(1),
      I3 => m_atarget_enc(0),
      I4 => m_atarget_enc(1),
      O => f_mux_return
    );
\m_ready_d[1]_i_3\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00CCF0AA"
    )
        port map (
      I0 => m_axi_arready(0),
      I1 => mi_arready(2),
      I2 => m_axi_arready(1),
      I3 => m_atarget_enc(0),
      I4 => m_atarget_enc(1),
      O => \f_mux_return__0\
    );
m_valid_i_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F0C0A00000C0A0"
    )
        port map (
      I0 => m_axi_rvalid(0),
      I1 => mi_rvalid(2),
      I2 => r_transfer_en,
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => m_axi_rvalid(1),
      O => aa_rvalid
    );
\s_axi_bvalid[0]_INST_0_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00CCF0AA"
    )
        port map (
      I0 => m_axi_bvalid(0),
      I1 => \^mi_bvalid\(0),
      I2 => m_axi_bvalid(1),
      I3 => m_atarget_enc(0),
      I4 => m_atarget_enc(1),
      O => \f_mux_return__3\
    );
\s_axi_wready[0]_INST_0_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00CCF0AA"
    )
        port map (
      I0 => m_axi_wready(0),
      I1 => \^mi_wready\(0),
      I2 => m_axi_wready(1),
      I3 => m_atarget_enc(0),
      I4 => m_atarget_enc(1),
      O => \f_mux_return__1\
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter is
  port (
    Q : out STD_LOGIC_VECTOR ( 2 downto 0 );
    \m_ready_d_reg[0]_0\ : out STD_LOGIC;
    \m_ready_d_reg[0]_1\ : in STD_LOGIC;
    aresetn_d : in STD_LOGIC;
    \m_ready_d_reg[0]_2\ : in STD_LOGIC;
    \f_mux_return__3\ : in STD_LOGIC;
    \m_ready_d_reg[0]_3\ : in STD_LOGIC;
    \f_mux_return__1\ : in STD_LOGIC;
    D : in STD_LOGIC_VECTOR ( 2 downto 0 );
    aclk : in STD_LOGIC
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter is
  signal \^q\ : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal \m_ready_d[2]_i_1_n_0\ : STD_LOGIC;
  signal \^m_ready_d_reg[0]_0\ : STD_LOGIC;
begin
  Q(2 downto 0) <= \^q\(2 downto 0);
  \m_ready_d_reg[0]_0\ <= \^m_ready_d_reg[0]_0\;
\m_ready_d[2]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"E0FF"
    )
        port map (
      I0 => \m_ready_d_reg[0]_1\,
      I1 => \^q\(2),
      I2 => \^m_ready_d_reg[0]_0\,
      I3 => aresetn_d,
      O => \m_ready_d[2]_i_1_n_0\
    );
\m_ready_d[2]_i_4\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"EAEAEA00EA00EA00"
    )
        port map (
      I0 => \^q\(0),
      I1 => \m_ready_d_reg[0]_2\,
      I2 => \f_mux_return__3\,
      I3 => \^q\(1),
      I4 => \m_ready_d_reg[0]_3\,
      I5 => \f_mux_return__1\,
      O => \^m_ready_d_reg[0]_0\
    );
\m_ready_d_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => D(0),
      Q => \^q\(0),
      R => \m_ready_d[2]_i_1_n_0\
    );
\m_ready_d_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => D(1),
      Q => \^q\(1),
      R => \m_ready_d[2]_i_1_n_0\
    );
\m_ready_d_reg[2]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => D(2),
      Q => \^q\(2),
      R => \m_ready_d[2]_i_1_n_0\
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity \decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0\ is
  port (
    Q : out STD_LOGIC_VECTOR ( 1 downto 0 );
    SR : in STD_LOGIC_VECTOR ( 0 to 0 );
    D : in STD_LOGIC_VECTOR ( 1 downto 0 );
    aclk : in STD_LOGIC
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of \decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0\ : entity is "axi_crossbar_v2_1_22_splitter";
end \decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0\;

architecture STRUCTURE of \decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0\ is
begin
\m_ready_d_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => D(0),
      Q => Q(0),
      R => SR(0)
    );
\m_ready_d_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => D(1),
      Q => Q(1),
      R => SR(0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_register_slice_v2_1_21_axic_register_slice is
  port (
    sr_rvalid : out STD_LOGIC;
    aa_rready : out STD_LOGIC;
    \m_ready_d_reg[0]\ : out STD_LOGIC;
    \m_payload_i_reg[34]_0\ : out STD_LOGIC_VECTOR ( 34 downto 0 );
    m_axi_rready : out STD_LOGIC_VECTOR ( 1 downto 0 );
    aclk : in STD_LOGIC;
    aa_rvalid : in STD_LOGIC;
    E : in STD_LOGIC_VECTOR ( 0 to 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_valid_i : in STD_LOGIC;
    s_axi_rready : in STD_LOGIC_VECTOR ( 0 to 0 );
    aa_grant_rnw : in STD_LOGIC;
    m_atarget_enc : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rdata : in STD_LOGIC_VECTOR ( 63 downto 0 );
    \m_axi_rready[1]\ : in STD_LOGIC_VECTOR ( 1 downto 0 );
    SR : in STD_LOGIC_VECTOR ( 0 to 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_register_slice_v2_1_21_axic_register_slice;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_register_slice_v2_1_21_axic_register_slice is
  signal \^aa_rready\ : STD_LOGIC;
  signal \aresetn_d_reg_n_0_[0]\ : STD_LOGIC;
  signal \aresetn_d_reg_n_0_[1]\ : STD_LOGIC;
  signal \^m_payload_i_reg[34]_0\ : STD_LOGIC_VECTOR ( 34 downto 0 );
  signal m_valid_i_i_1_n_0 : STD_LOGIC;
  signal s_ready_i_i_1_n_0 : STD_LOGIC;
  signal skid_buffer : STD_LOGIC_VECTOR ( 34 downto 0 );
  signal \skid_buffer_reg_n_0_[0]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[10]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[11]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[12]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[13]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[14]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[15]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[16]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[17]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[18]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[19]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[1]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[20]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[21]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[22]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[23]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[24]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[25]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[26]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[27]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[28]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[29]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[2]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[30]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[31]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[32]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[33]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[34]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[3]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[4]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[5]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[6]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[7]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[8]\ : STD_LOGIC;
  signal \skid_buffer_reg_n_0_[9]\ : STD_LOGIC;
  signal \^sr_rvalid\ : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \m_axi_rready[1]_INST_0\ : label is "soft_lutpair13";
  attribute SOFT_HLUTNM of m_valid_i_i_1 : label is "soft_lutpair12";
  attribute SOFT_HLUTNM of s_ready_i_i_1 : label is "soft_lutpair12";
  attribute SOFT_HLUTNM of \skid_buffer[0]_i_1\ : label is "soft_lutpair13";
begin
  aa_rready <= \^aa_rready\;
  \m_payload_i_reg[34]_0\(34 downto 0) <= \^m_payload_i_reg[34]_0\(34 downto 0);
  sr_rvalid <= \^sr_rvalid\;
\aresetn_d_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => '1',
      Q => \aresetn_d_reg_n_0_[0]\,
      R => SR(0)
    );
\aresetn_d_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => \aresetn_d_reg_n_0_[0]\,
      Q => \aresetn_d_reg_n_0_[1]\,
      R => SR(0)
    );
\gen_no_arbiter.m_grant_hot_i[0]_inv_i_3\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"EAAAAAAA00000000"
    )
        port map (
      I0 => Q(0),
      I1 => \^sr_rvalid\,
      I2 => \^m_payload_i_reg[34]_0\(0),
      I3 => m_valid_i,
      I4 => s_axi_rready(0),
      I5 => aa_grant_rnw,
      O => \m_ready_d_reg[0]\
    );
\m_axi_rready[0]_INST_0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => \m_axi_rready[1]\(0),
      I1 => \^aa_rready\,
      O => m_axi_rready(0)
    );
\m_axi_rready[1]_INST_0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => \m_axi_rready[1]\(1),
      I1 => \^aa_rready\,
      O => m_axi_rready(1)
    );
\m_payload_i[10]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[10]\,
      I1 => m_axi_rdata(7),
      I2 => m_axi_rdata(39),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(10)
    );
\m_payload_i[11]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[11]\,
      I1 => m_axi_rdata(8),
      I2 => m_axi_rdata(40),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(11)
    );
\m_payload_i[12]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(9),
      I1 => m_axi_rdata(41),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[12]\,
      O => skid_buffer(12)
    );
\m_payload_i[13]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(10),
      I1 => m_axi_rdata(42),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[13]\,
      O => skid_buffer(13)
    );
\m_payload_i[14]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(11),
      I1 => m_axi_rdata(43),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[14]\,
      O => skid_buffer(14)
    );
\m_payload_i[15]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(12),
      I1 => m_axi_rdata(44),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[15]\,
      O => skid_buffer(15)
    );
\m_payload_i[16]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[16]\,
      I1 => m_axi_rdata(13),
      I2 => m_axi_rdata(45),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(16)
    );
\m_payload_i[17]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(14),
      I1 => m_axi_rdata(46),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[17]\,
      O => skid_buffer(17)
    );
\m_payload_i[18]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(15),
      I1 => m_axi_rdata(47),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[18]\,
      O => skid_buffer(18)
    );
\m_payload_i[19]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[19]\,
      I1 => m_axi_rdata(16),
      I2 => m_axi_rdata(48),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(19)
    );
\m_payload_i[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rresp(0),
      I1 => m_axi_rresp(2),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[1]\,
      O => skid_buffer(1)
    );
\m_payload_i[20]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[20]\,
      I1 => m_axi_rdata(17),
      I2 => m_axi_rdata(49),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(20)
    );
\m_payload_i[21]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[21]\,
      I1 => m_axi_rdata(18),
      I2 => m_axi_rdata(50),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(21)
    );
\m_payload_i[22]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[22]\,
      I1 => m_axi_rdata(19),
      I2 => m_axi_rdata(51),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(22)
    );
\m_payload_i[23]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[23]\,
      I1 => m_axi_rdata(20),
      I2 => m_axi_rdata(52),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(23)
    );
\m_payload_i[24]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[24]\,
      I1 => m_axi_rdata(21),
      I2 => m_axi_rdata(53),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(24)
    );
\m_payload_i[25]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(22),
      I1 => m_axi_rdata(54),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[25]\,
      O => skid_buffer(25)
    );
\m_payload_i[26]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(23),
      I1 => m_axi_rdata(55),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[26]\,
      O => skid_buffer(26)
    );
\m_payload_i[27]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[27]\,
      I1 => m_axi_rdata(24),
      I2 => m_axi_rdata(56),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(27)
    );
\m_payload_i[28]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(25),
      I1 => m_axi_rdata(57),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[28]\,
      O => skid_buffer(28)
    );
\m_payload_i[29]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(26),
      I1 => m_axi_rdata(58),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[29]\,
      O => skid_buffer(29)
    );
\m_payload_i[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rresp(1),
      I1 => m_axi_rresp(3),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[2]\,
      O => skid_buffer(2)
    );
\m_payload_i[30]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(27),
      I1 => m_axi_rdata(59),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[30]\,
      O => skid_buffer(30)
    );
\m_payload_i[31]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(28),
      I1 => m_axi_rdata(60),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[31]\,
      O => skid_buffer(31)
    );
\m_payload_i[32]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[32]\,
      I1 => m_axi_rdata(29),
      I2 => m_axi_rdata(61),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(32)
    );
\m_payload_i[33]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(30),
      I1 => m_axi_rdata(62),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[33]\,
      O => skid_buffer(33)
    );
\m_payload_i[34]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(31),
      I1 => m_axi_rdata(63),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[34]\,
      O => skid_buffer(34)
    );
\m_payload_i[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[3]\,
      I1 => m_axi_rdata(0),
      I2 => m_axi_rdata(32),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(3)
    );
\m_payload_i[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[4]\,
      I1 => m_axi_rdata(1),
      I2 => m_axi_rdata(33),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(4)
    );
\m_payload_i[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(2),
      I1 => m_axi_rdata(34),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[5]\,
      O => skid_buffer(5)
    );
\m_payload_i[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(3),
      I1 => m_axi_rdata(35),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[6]\,
      O => skid_buffer(6)
    );
\m_payload_i[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0FCAFFFF0FCA0000"
    )
        port map (
      I0 => m_axi_rdata(4),
      I1 => m_axi_rdata(36),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      I4 => \^aa_rready\,
      I5 => \skid_buffer_reg_n_0_[7]\,
      O => skid_buffer(7)
    );
\m_payload_i[8]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[8]\,
      I1 => m_axi_rdata(5),
      I2 => m_axi_rdata(37),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(8)
    );
\m_payload_i[9]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00F000CCAAAAAAAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[9]\,
      I1 => m_axi_rdata(6),
      I2 => m_axi_rdata(38),
      I3 => m_atarget_enc(1),
      I4 => m_atarget_enc(0),
      I5 => \^aa_rready\,
      O => skid_buffer(9)
    );
\m_payload_i_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(0),
      Q => \^m_payload_i_reg[34]_0\(0),
      R => '0'
    );
\m_payload_i_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(10),
      Q => \^m_payload_i_reg[34]_0\(10),
      R => '0'
    );
\m_payload_i_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(11),
      Q => \^m_payload_i_reg[34]_0\(11),
      R => '0'
    );
\m_payload_i_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(12),
      Q => \^m_payload_i_reg[34]_0\(12),
      R => '0'
    );
\m_payload_i_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(13),
      Q => \^m_payload_i_reg[34]_0\(13),
      R => '0'
    );
\m_payload_i_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(14),
      Q => \^m_payload_i_reg[34]_0\(14),
      R => '0'
    );
\m_payload_i_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(15),
      Q => \^m_payload_i_reg[34]_0\(15),
      R => '0'
    );
\m_payload_i_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(16),
      Q => \^m_payload_i_reg[34]_0\(16),
      R => '0'
    );
\m_payload_i_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(17),
      Q => \^m_payload_i_reg[34]_0\(17),
      R => '0'
    );
\m_payload_i_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(18),
      Q => \^m_payload_i_reg[34]_0\(18),
      R => '0'
    );
\m_payload_i_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(19),
      Q => \^m_payload_i_reg[34]_0\(19),
      R => '0'
    );
\m_payload_i_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(1),
      Q => \^m_payload_i_reg[34]_0\(1),
      R => '0'
    );
\m_payload_i_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(20),
      Q => \^m_payload_i_reg[34]_0\(20),
      R => '0'
    );
\m_payload_i_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(21),
      Q => \^m_payload_i_reg[34]_0\(21),
      R => '0'
    );
\m_payload_i_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(22),
      Q => \^m_payload_i_reg[34]_0\(22),
      R => '0'
    );
\m_payload_i_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(23),
      Q => \^m_payload_i_reg[34]_0\(23),
      R => '0'
    );
\m_payload_i_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(24),
      Q => \^m_payload_i_reg[34]_0\(24),
      R => '0'
    );
\m_payload_i_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(25),
      Q => \^m_payload_i_reg[34]_0\(25),
      R => '0'
    );
\m_payload_i_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(26),
      Q => \^m_payload_i_reg[34]_0\(26),
      R => '0'
    );
\m_payload_i_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(27),
      Q => \^m_payload_i_reg[34]_0\(27),
      R => '0'
    );
\m_payload_i_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(28),
      Q => \^m_payload_i_reg[34]_0\(28),
      R => '0'
    );
\m_payload_i_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(29),
      Q => \^m_payload_i_reg[34]_0\(29),
      R => '0'
    );
\m_payload_i_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(2),
      Q => \^m_payload_i_reg[34]_0\(2),
      R => '0'
    );
\m_payload_i_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(30),
      Q => \^m_payload_i_reg[34]_0\(30),
      R => '0'
    );
\m_payload_i_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(31),
      Q => \^m_payload_i_reg[34]_0\(31),
      R => '0'
    );
\m_payload_i_reg[32]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(32),
      Q => \^m_payload_i_reg[34]_0\(32),
      R => '0'
    );
\m_payload_i_reg[33]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(33),
      Q => \^m_payload_i_reg[34]_0\(33),
      R => '0'
    );
\m_payload_i_reg[34]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(34),
      Q => \^m_payload_i_reg[34]_0\(34),
      R => '0'
    );
\m_payload_i_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(3),
      Q => \^m_payload_i_reg[34]_0\(3),
      R => '0'
    );
\m_payload_i_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(4),
      Q => \^m_payload_i_reg[34]_0\(4),
      R => '0'
    );
\m_payload_i_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(5),
      Q => \^m_payload_i_reg[34]_0\(5),
      R => '0'
    );
\m_payload_i_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(6),
      Q => \^m_payload_i_reg[34]_0\(6),
      R => '0'
    );
\m_payload_i_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(7),
      Q => \^m_payload_i_reg[34]_0\(7),
      R => '0'
    );
\m_payload_i_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(8),
      Q => \^m_payload_i_reg[34]_0\(8),
      R => '0'
    );
\m_payload_i_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => E(0),
      D => skid_buffer(9),
      Q => \^m_payload_i_reg[34]_0\(9),
      R => '0'
    );
m_valid_i_i_1: unisim.vcomponents.LUT4
    generic map(
      INIT => X"C4CC"
    )
        port map (
      I0 => E(0),
      I1 => \aresetn_d_reg_n_0_[1]\,
      I2 => aa_rvalid,
      I3 => \^aa_rready\,
      O => m_valid_i_i_1_n_0
    );
m_valid_i_reg: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => m_valid_i_i_1_n_0,
      Q => \^sr_rvalid\,
      R => '0'
    );
s_ready_i_i_1: unisim.vcomponents.LUT4
    generic map(
      INIT => X"F040"
    )
        port map (
      I0 => aa_rvalid,
      I1 => \^aa_rready\,
      I2 => \aresetn_d_reg_n_0_[0]\,
      I3 => E(0),
      O => s_ready_i_i_1_n_0
    );
s_ready_i_reg: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => s_ready_i_i_1_n_0,
      Q => \^aa_rready\,
      R => '0'
    );
\skid_buffer[0]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"3FAA"
    )
        port map (
      I0 => \skid_buffer_reg_n_0_[0]\,
      I1 => m_atarget_enc(1),
      I2 => m_atarget_enc(0),
      I3 => \^aa_rready\,
      O => skid_buffer(0)
    );
\skid_buffer_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(0),
      Q => \skid_buffer_reg_n_0_[0]\,
      R => '0'
    );
\skid_buffer_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(10),
      Q => \skid_buffer_reg_n_0_[10]\,
      R => '0'
    );
\skid_buffer_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(11),
      Q => \skid_buffer_reg_n_0_[11]\,
      R => '0'
    );
\skid_buffer_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(12),
      Q => \skid_buffer_reg_n_0_[12]\,
      R => '0'
    );
\skid_buffer_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(13),
      Q => \skid_buffer_reg_n_0_[13]\,
      R => '0'
    );
\skid_buffer_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(14),
      Q => \skid_buffer_reg_n_0_[14]\,
      R => '0'
    );
\skid_buffer_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(15),
      Q => \skid_buffer_reg_n_0_[15]\,
      R => '0'
    );
\skid_buffer_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(16),
      Q => \skid_buffer_reg_n_0_[16]\,
      R => '0'
    );
\skid_buffer_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(17),
      Q => \skid_buffer_reg_n_0_[17]\,
      R => '0'
    );
\skid_buffer_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(18),
      Q => \skid_buffer_reg_n_0_[18]\,
      R => '0'
    );
\skid_buffer_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(19),
      Q => \skid_buffer_reg_n_0_[19]\,
      R => '0'
    );
\skid_buffer_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(1),
      Q => \skid_buffer_reg_n_0_[1]\,
      R => '0'
    );
\skid_buffer_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(20),
      Q => \skid_buffer_reg_n_0_[20]\,
      R => '0'
    );
\skid_buffer_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(21),
      Q => \skid_buffer_reg_n_0_[21]\,
      R => '0'
    );
\skid_buffer_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(22),
      Q => \skid_buffer_reg_n_0_[22]\,
      R => '0'
    );
\skid_buffer_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(23),
      Q => \skid_buffer_reg_n_0_[23]\,
      R => '0'
    );
\skid_buffer_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(24),
      Q => \skid_buffer_reg_n_0_[24]\,
      R => '0'
    );
\skid_buffer_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(25),
      Q => \skid_buffer_reg_n_0_[25]\,
      R => '0'
    );
\skid_buffer_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(26),
      Q => \skid_buffer_reg_n_0_[26]\,
      R => '0'
    );
\skid_buffer_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(27),
      Q => \skid_buffer_reg_n_0_[27]\,
      R => '0'
    );
\skid_buffer_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(28),
      Q => \skid_buffer_reg_n_0_[28]\,
      R => '0'
    );
\skid_buffer_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(29),
      Q => \skid_buffer_reg_n_0_[29]\,
      R => '0'
    );
\skid_buffer_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(2),
      Q => \skid_buffer_reg_n_0_[2]\,
      R => '0'
    );
\skid_buffer_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(30),
      Q => \skid_buffer_reg_n_0_[30]\,
      R => '0'
    );
\skid_buffer_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(31),
      Q => \skid_buffer_reg_n_0_[31]\,
      R => '0'
    );
\skid_buffer_reg[32]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(32),
      Q => \skid_buffer_reg_n_0_[32]\,
      R => '0'
    );
\skid_buffer_reg[33]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(33),
      Q => \skid_buffer_reg_n_0_[33]\,
      R => '0'
    );
\skid_buffer_reg[34]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(34),
      Q => \skid_buffer_reg_n_0_[34]\,
      R => '0'
    );
\skid_buffer_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(3),
      Q => \skid_buffer_reg_n_0_[3]\,
      R => '0'
    );
\skid_buffer_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(4),
      Q => \skid_buffer_reg_n_0_[4]\,
      R => '0'
    );
\skid_buffer_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(5),
      Q => \skid_buffer_reg_n_0_[5]\,
      R => '0'
    );
\skid_buffer_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(6),
      Q => \skid_buffer_reg_n_0_[6]\,
      R => '0'
    );
\skid_buffer_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(7),
      Q => \skid_buffer_reg_n_0_[7]\,
      R => '0'
    );
\skid_buffer_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(8),
      Q => \skid_buffer_reg_n_0_[8]\,
      R => '0'
    );
\skid_buffer_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => aclk,
      CE => '1',
      D => skid_buffer(9),
      Q => \skid_buffer_reg_n_0_[9]\,
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_crossbar_sasd is
  port (
    Q : out STD_LOGIC_VECTOR ( 27 downto 0 );
    \m_payload_i_reg[34]\ : out STD_LOGIC_VECTOR ( 33 downto 0 );
    m_axi_awvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wready : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_bready : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_rready : out STD_LOGIC_VECTOR ( 1 downto 0 );
    aresetn : in STD_LOGIC;
    aclk : in STD_LOGIC;
    s_axi_awvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bready : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_rvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rready : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_wready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rdata : in STD_LOGIC_VECTOR ( 63 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_crossbar_sasd;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_crossbar_sasd is
  signal aa_grant_rnw : STD_LOGIC;
  signal aa_rready : STD_LOGIC;
  signal aa_rvalid : STD_LOGIC;
  signal addr_arbiter_inst_n_44 : STD_LOGIC;
  signal addr_arbiter_inst_n_47 : STD_LOGIC;
  signal addr_arbiter_inst_n_50 : STD_LOGIC;
  signal addr_arbiter_inst_n_51 : STD_LOGIC;
  signal addr_arbiter_inst_n_53 : STD_LOGIC;
  signal addr_arbiter_inst_n_61 : STD_LOGIC;
  signal addr_arbiter_inst_n_62 : STD_LOGIC;
  signal any_error : STD_LOGIC;
  signal aresetn_d : STD_LOGIC;
  signal f_mux_return : STD_LOGIC;
  signal \f_mux_return__0\ : STD_LOGIC;
  signal \f_mux_return__1\ : STD_LOGIC;
  signal \f_mux_return__3\ : STD_LOGIC;
  signal m_atarget_enc : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal m_atarget_hot : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal m_atarget_hot0 : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal m_ready_d : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal m_ready_d0 : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal m_ready_d0_0 : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal m_ready_d_1 : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal m_valid_i : STD_LOGIC;
  signal mi_arvalid_en : STD_LOGIC;
  signal mi_bvalid : STD_LOGIC_VECTOR ( 2 to 2 );
  signal mi_wready : STD_LOGIC_VECTOR ( 2 to 2 );
  signal p_1_in : STD_LOGIC;
  signal r_transfer_en : STD_LOGIC;
  signal reg_slice_r_n_2 : STD_LOGIC;
  signal reg_slice_r_n_37 : STD_LOGIC;
  signal reset : STD_LOGIC;
  signal splitter_aw_n_3 : STD_LOGIC;
  signal sr_rvalid : STD_LOGIC;
begin
addr_arbiter_inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_addr_arbiter_sasd
     port map (
      D(2 downto 0) => m_atarget_hot0(2 downto 0),
      E(0) => p_1_in,
      Q(27 downto 0) => Q(27 downto 0),
      SR(0) => reset,
      aa_grant_rnw => aa_grant_rnw,
      aclk => aclk,
      any_error => any_error,
      aresetn_d => aresetn_d,
      aresetn_d_reg(0) => addr_arbiter_inst_n_53,
      aresetn_d_reg_0 => addr_arbiter_inst_n_61,
      f_mux_return => f_mux_return,
      \f_mux_return__0\ => \f_mux_return__0\,
      \f_mux_return__1\ => \f_mux_return__1\,
      \f_mux_return__3\ => \f_mux_return__3\,
      \gen_axilite.s_axi_bvalid_i_reg\ => addr_arbiter_inst_n_62,
      \gen_axilite.s_axi_bvalid_i_reg_0\(2 downto 0) => m_ready_d_1(2 downto 0),
      \gen_axilite.s_axi_bvalid_i_reg_1\(2 downto 0) => m_atarget_hot(2 downto 0),
      \gen_no_arbiter.grant_rnw_reg_0\ => addr_arbiter_inst_n_47,
      \gen_no_arbiter.grant_rnw_reg_1\ => addr_arbiter_inst_n_51,
      \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0\ => reg_slice_r_n_2,
      \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1\ => splitter_aw_n_3,
      m_atarget_enc(1 downto 0) => m_atarget_enc(1 downto 0),
      \m_atarget_enc_reg[1]\ => addr_arbiter_inst_n_44,
      m_axi_arvalid(1 downto 0) => m_axi_arvalid(1 downto 0),
      \m_axi_arvalid[0]\(1 downto 0) => m_ready_d(1 downto 0),
      m_axi_awready(1 downto 0) => m_axi_awready(1 downto 0),
      m_axi_awvalid(1 downto 0) => m_axi_awvalid(1 downto 0),
      m_axi_bready(1 downto 0) => m_axi_bready(1 downto 0),
      m_axi_wvalid(1 downto 0) => m_axi_wvalid(1 downto 0),
      m_ready_d0(1 downto 0) => m_ready_d0_0(1 downto 0),
      \m_ready_d_reg[0]\(0) => reg_slice_r_n_37,
      \m_ready_d_reg[2]\(2 downto 0) => m_ready_d0(2 downto 0),
      m_valid_i => m_valid_i,
      mi_arvalid_en => mi_arvalid_en,
      mi_bvalid(0) => mi_bvalid(2),
      mi_wready(0) => mi_wready(2),
      r_transfer_en => r_transfer_en,
      s_axi_araddr(24 downto 0) => s_axi_araddr(24 downto 0),
      s_axi_arprot(2 downto 0) => s_axi_arprot(2 downto 0),
      s_axi_arready(0) => s_axi_arready(0),
      s_axi_arvalid(0) => s_axi_arvalid(0),
      s_axi_awaddr(24 downto 0) => s_axi_awaddr(24 downto 0),
      s_axi_awprot(2 downto 0) => s_axi_awprot(2 downto 0),
      s_axi_awready(0) => s_axi_awready(0),
      s_axi_awvalid(0) => s_axi_awvalid(0),
      s_axi_bready(0) => s_axi_bready(0),
      s_axi_bvalid(0) => s_axi_bvalid(0),
      s_axi_rready(0) => s_axi_rready(0),
      s_axi_rvalid(0) => s_axi_rvalid(0),
      s_axi_wready(0) => s_axi_wready(0),
      s_axi_wvalid(0) => s_axi_wvalid(0),
      s_axi_wvalid_0_sp_1 => addr_arbiter_inst_n_50,
      sr_rvalid => sr_rvalid
    );
aresetn_d_reg: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => aresetn,
      Q => aresetn_d,
      R => '0'
    );
\gen_decerr.decerr_slave_inst\: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_decerr_slave
     port map (
      Q(0) => m_atarget_hot(2),
      SR(0) => reset,
      aa_rready => aa_rready,
      aa_rvalid => aa_rvalid,
      aclk => aclk,
      aresetn_d => aresetn_d,
      f_mux_return => f_mux_return,
      \f_mux_return__0\ => \f_mux_return__0\,
      \f_mux_return__1\ => \f_mux_return__1\,
      \f_mux_return__3\ => \f_mux_return__3\,
      \gen_axilite.s_axi_awready_i_reg_0\ => addr_arbiter_inst_n_50,
      \gen_axilite.s_axi_awready_i_reg_1\(0) => m_ready_d_1(2),
      \gen_axilite.s_axi_bvalid_i_reg_0\ => addr_arbiter_inst_n_62,
      m_atarget_enc(1 downto 0) => m_atarget_enc(1 downto 0),
      m_axi_arready(1 downto 0) => m_axi_arready(1 downto 0),
      m_axi_awready(1 downto 0) => m_axi_awready(1 downto 0),
      m_axi_bvalid(1 downto 0) => m_axi_bvalid(1 downto 0),
      m_axi_rvalid(1 downto 0) => m_axi_rvalid(1 downto 0),
      m_axi_wready(1 downto 0) => m_axi_wready(1 downto 0),
      mi_arvalid_en => mi_arvalid_en,
      mi_bvalid(0) => mi_bvalid(2),
      mi_wready(0) => mi_wready(2),
      r_transfer_en => r_transfer_en
    );
\m_atarget_enc_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => addr_arbiter_inst_n_61,
      Q => m_atarget_enc(0),
      R => '0'
    );
\m_atarget_enc_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => any_error,
      Q => m_atarget_enc(1),
      R => reset
    );
\m_atarget_hot_reg[0]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => m_atarget_hot0(0),
      Q => m_atarget_hot(0),
      R => reset
    );
\m_atarget_hot_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => m_atarget_hot0(1),
      Q => m_atarget_hot(1),
      R => reset
    );
\m_atarget_hot_reg[2]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => aclk,
      CE => '1',
      D => m_atarget_hot0(2),
      Q => m_atarget_hot(2),
      R => reset
    );
reg_slice_r: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_register_slice_v2_1_21_axic_register_slice
     port map (
      E(0) => p_1_in,
      Q(0) => m_ready_d(0),
      SR(0) => reset,
      aa_grant_rnw => aa_grant_rnw,
      aa_rready => aa_rready,
      aa_rvalid => aa_rvalid,
      aclk => aclk,
      m_atarget_enc(1 downto 0) => m_atarget_enc(1 downto 0),
      m_axi_rdata(63 downto 0) => m_axi_rdata(63 downto 0),
      m_axi_rready(1 downto 0) => m_axi_rready(1 downto 0),
      \m_axi_rready[1]\(1 downto 0) => m_atarget_hot(1 downto 0),
      m_axi_rresp(3 downto 0) => m_axi_rresp(3 downto 0),
      \m_payload_i_reg[34]_0\(34 downto 1) => \m_payload_i_reg[34]\(33 downto 0),
      \m_payload_i_reg[34]_0\(0) => reg_slice_r_n_37,
      \m_ready_d_reg[0]\ => reg_slice_r_n_2,
      m_valid_i => m_valid_i,
      s_axi_rready(0) => s_axi_rready(0),
      sr_rvalid => sr_rvalid
    );
\s_axi_bresp[0]_INST_0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0FCA"
    )
        port map (
      I0 => m_axi_bresp(0),
      I1 => m_axi_bresp(2),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      O => s_axi_bresp(0)
    );
\s_axi_bresp[1]_INST_0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0FCA"
    )
        port map (
      I0 => m_axi_bresp(1),
      I1 => m_axi_bresp(3),
      I2 => m_atarget_enc(0),
      I3 => m_atarget_enc(1),
      O => s_axi_bresp(1)
    );
splitter_ar: entity work.\decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0\
     port map (
      D(1 downto 0) => m_ready_d0_0(1 downto 0),
      Q(1 downto 0) => m_ready_d(1 downto 0),
      SR(0) => addr_arbiter_inst_n_53,
      aclk => aclk
    );
splitter_aw: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter
     port map (
      D(2 downto 0) => m_ready_d0(2 downto 0),
      Q(2 downto 0) => m_ready_d_1(2 downto 0),
      aclk => aclk,
      aresetn_d => aresetn_d,
      \f_mux_return__1\ => \f_mux_return__1\,
      \f_mux_return__3\ => \f_mux_return__3\,
      \m_ready_d_reg[0]_0\ => splitter_aw_n_3,
      \m_ready_d_reg[0]_1\ => addr_arbiter_inst_n_44,
      \m_ready_d_reg[0]_2\ => addr_arbiter_inst_n_47,
      \m_ready_d_reg[0]_3\ => addr_arbiter_inst_n_51
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_awlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_awsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_awlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_awuser : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_wlast : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wuser : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_buser : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bready : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_arlen : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axi_arsize : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arburst : in STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_arlock : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arcache : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arqos : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_aruser : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rlast : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_ruser : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rready : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 49 downto 0 );
    m_axi_awlen : out STD_LOGIC_VECTOR ( 15 downto 0 );
    m_axi_awsize : out STD_LOGIC_VECTOR ( 5 downto 0 );
    m_axi_awburst : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_awlock : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awcache : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 5 downto 0 );
    m_axi_awregion : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awqos : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_awuser : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wdata : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_wlast : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wuser : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_buser : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bready : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_araddr : out STD_LOGIC_VECTOR ( 49 downto 0 );
    m_axi_arlen : out STD_LOGIC_VECTOR ( 15 downto 0 );
    m_axi_arsize : out STD_LOGIC_VECTOR ( 5 downto 0 );
    m_axi_arburst : out STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_arlock : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arcache : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 5 downto 0 );
    m_axi_arregion : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_arqos : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_aruser : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rdata : in STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rlast : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_ruser : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rready : out STD_LOGIC_VECTOR ( 1 downto 0 )
  );
  attribute C_AXI_ADDR_WIDTH : integer;
  attribute C_AXI_ADDR_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 25;
  attribute C_AXI_ARUSER_WIDTH : integer;
  attribute C_AXI_ARUSER_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_AXI_AWUSER_WIDTH : integer;
  attribute C_AXI_AWUSER_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_AXI_BUSER_WIDTH : integer;
  attribute C_AXI_BUSER_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_AXI_DATA_WIDTH : integer;
  attribute C_AXI_DATA_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 32;
  attribute C_AXI_ID_WIDTH : integer;
  attribute C_AXI_ID_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_AXI_PROTOCOL : integer;
  attribute C_AXI_PROTOCOL of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 2;
  attribute C_AXI_RUSER_WIDTH : integer;
  attribute C_AXI_RUSER_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_AXI_SUPPORTS_USER_SIGNALS : integer;
  attribute C_AXI_SUPPORTS_USER_SIGNALS of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 0;
  attribute C_AXI_WUSER_WIDTH : integer;
  attribute C_AXI_WUSER_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_CONNECTIVITY_MODE : integer;
  attribute C_CONNECTIVITY_MODE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 0;
  attribute C_DEBUG : integer;
  attribute C_DEBUG of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_FAMILY : string;
  attribute C_FAMILY of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "virtexuplus";
  attribute C_M_AXI_ADDR_WIDTH : string;
  attribute C_M_AXI_ADDR_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "320'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000010000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000001100";
  attribute C_M_AXI_BASE_ADDR : string;
  attribute C_M_AXI_BASE_ADDR of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "640'b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000000000000000000000000000000000000000110000101000000000000000000000000000000000000000000000000000000011000010000000000000000000000000000000000000000000000000000000001100000110000000000000000000000000000000000000000000000000000000110000010000000000000000000000000000000000000000000000000000000011000000100000000000000000000000000000000000000000000000000000001100000000000000000000000";
  attribute C_M_AXI_READ_CONNECTIVITY : string;
  attribute C_M_AXI_READ_CONNECTIVITY of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b1111111111111111111111111111111111111111111111111111111111111111";
  attribute C_M_AXI_READ_ISSUING : string;
  attribute C_M_AXI_READ_ISSUING of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b0000000000000000000000000000000100000000000000000000000000000001";
  attribute C_M_AXI_SECURE : string;
  attribute C_M_AXI_SECURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute C_M_AXI_WRITE_CONNECTIVITY : string;
  attribute C_M_AXI_WRITE_CONNECTIVITY of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b1111111111111111111111111111111111111111111111111111111111111111";
  attribute C_M_AXI_WRITE_ISSUING : string;
  attribute C_M_AXI_WRITE_ISSUING of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b0000000000000000000000000000000100000000000000000000000000000001";
  attribute C_NUM_ADDR_RANGES : integer;
  attribute C_NUM_ADDR_RANGES of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 5;
  attribute C_NUM_MASTER_SLOTS : integer;
  attribute C_NUM_MASTER_SLOTS of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 2;
  attribute C_NUM_SLAVE_SLOTS : integer;
  attribute C_NUM_SLAVE_SLOTS of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_R_REGISTER : integer;
  attribute C_R_REGISTER of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_S_AXI_ARB_PRIORITY : integer;
  attribute C_S_AXI_ARB_PRIORITY of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 0;
  attribute C_S_AXI_BASE_ID : integer;
  attribute C_S_AXI_BASE_ID of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 0;
  attribute C_S_AXI_READ_ACCEPTANCE : integer;
  attribute C_S_AXI_READ_ACCEPTANCE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_S_AXI_SINGLE_THREAD : integer;
  attribute C_S_AXI_SINGLE_THREAD of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute C_S_AXI_THREAD_ID_WIDTH : integer;
  attribute C_S_AXI_THREAD_ID_WIDTH of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 0;
  attribute C_S_AXI_WRITE_ACCEPTANCE : integer;
  attribute C_S_AXI_WRITE_ACCEPTANCE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "yes";
  attribute P_ADDR_DECODE : integer;
  attribute P_ADDR_DECODE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute P_AXI3 : integer;
  attribute P_AXI3 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute P_AXI4 : integer;
  attribute P_AXI4 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 0;
  attribute P_AXILITE : integer;
  attribute P_AXILITE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 2;
  attribute P_AXILITE_SIZE : string;
  attribute P_AXILITE_SIZE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "3'b010";
  attribute P_FAMILY : string;
  attribute P_FAMILY of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "rtl";
  attribute P_INCR : string;
  attribute P_INCR of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "2'b01";
  attribute P_LEN : integer;
  attribute P_LEN of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 8;
  attribute P_LOCK : integer;
  attribute P_LOCK of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute P_M_AXI_ERR_MODE : string;
  attribute P_M_AXI_ERR_MODE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute P_M_AXI_SUPPORTS_READ : string;
  attribute P_M_AXI_SUPPORTS_READ of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "2'b11";
  attribute P_M_AXI_SUPPORTS_WRITE : string;
  attribute P_M_AXI_SUPPORTS_WRITE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "2'b11";
  attribute P_ONES : string;
  attribute P_ONES of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "65'b11111111111111111111111111111111111111111111111111111111111111111";
  attribute P_RANGE_CHECK : integer;
  attribute P_RANGE_CHECK of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is 1;
  attribute P_S_AXI_BASE_ID : string;
  attribute P_S_AXI_BASE_ID of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute P_S_AXI_HIGH_ID : string;
  attribute P_S_AXI_HIGH_ID of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute P_S_AXI_SUPPORTS_READ : string;
  attribute P_S_AXI_SUPPORTS_READ of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "1'b1";
  attribute P_S_AXI_SUPPORTS_WRITE : string;
  attribute P_S_AXI_SUPPORTS_WRITE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar : entity is "1'b1";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar is
  signal \<const0>\ : STD_LOGIC;
  signal \^m_axi_araddr\ : STD_LOGIC_VECTOR ( 11 downto 0 );
  signal \^m_axi_arprot\ : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal \^m_axi_awaddr\ : STD_LOGIC_VECTOR ( 49 downto 37 );
  signal \^s_axi_wdata\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal \^s_axi_wstrb\ : STD_LOGIC_VECTOR ( 3 downto 0 );
begin
  \^s_axi_wdata\(31 downto 0) <= s_axi_wdata(31 downto 0);
  \^s_axi_wstrb\(3 downto 0) <= s_axi_wstrb(3 downto 0);
  m_axi_araddr(49 downto 37) <= \^m_axi_awaddr\(49 downto 37);
  m_axi_araddr(36 downto 25) <= \^m_axi_araddr\(11 downto 0);
  m_axi_araddr(24 downto 12) <= \^m_axi_awaddr\(49 downto 37);
  m_axi_araddr(11 downto 0) <= \^m_axi_araddr\(11 downto 0);
  m_axi_arburst(3) <= \<const0>\;
  m_axi_arburst(2) <= \<const0>\;
  m_axi_arburst(1) <= \<const0>\;
  m_axi_arburst(0) <= \<const0>\;
  m_axi_arcache(7) <= \<const0>\;
  m_axi_arcache(6) <= \<const0>\;
  m_axi_arcache(5) <= \<const0>\;
  m_axi_arcache(4) <= \<const0>\;
  m_axi_arcache(3) <= \<const0>\;
  m_axi_arcache(2) <= \<const0>\;
  m_axi_arcache(1) <= \<const0>\;
  m_axi_arcache(0) <= \<const0>\;
  m_axi_arid(1) <= \<const0>\;
  m_axi_arid(0) <= \<const0>\;
  m_axi_arlen(15) <= \<const0>\;
  m_axi_arlen(14) <= \<const0>\;
  m_axi_arlen(13) <= \<const0>\;
  m_axi_arlen(12) <= \<const0>\;
  m_axi_arlen(11) <= \<const0>\;
  m_axi_arlen(10) <= \<const0>\;
  m_axi_arlen(9) <= \<const0>\;
  m_axi_arlen(8) <= \<const0>\;
  m_axi_arlen(7) <= \<const0>\;
  m_axi_arlen(6) <= \<const0>\;
  m_axi_arlen(5) <= \<const0>\;
  m_axi_arlen(4) <= \<const0>\;
  m_axi_arlen(3) <= \<const0>\;
  m_axi_arlen(2) <= \<const0>\;
  m_axi_arlen(1) <= \<const0>\;
  m_axi_arlen(0) <= \<const0>\;
  m_axi_arlock(1) <= \<const0>\;
  m_axi_arlock(0) <= \<const0>\;
  m_axi_arprot(5 downto 3) <= \^m_axi_arprot\(2 downto 0);
  m_axi_arprot(2 downto 0) <= \^m_axi_arprot\(2 downto 0);
  m_axi_arqos(7) <= \<const0>\;
  m_axi_arqos(6) <= \<const0>\;
  m_axi_arqos(5) <= \<const0>\;
  m_axi_arqos(4) <= \<const0>\;
  m_axi_arqos(3) <= \<const0>\;
  m_axi_arqos(2) <= \<const0>\;
  m_axi_arqos(1) <= \<const0>\;
  m_axi_arqos(0) <= \<const0>\;
  m_axi_arregion(7) <= \<const0>\;
  m_axi_arregion(6) <= \<const0>\;
  m_axi_arregion(5) <= \<const0>\;
  m_axi_arregion(4) <= \<const0>\;
  m_axi_arregion(3) <= \<const0>\;
  m_axi_arregion(2) <= \<const0>\;
  m_axi_arregion(1) <= \<const0>\;
  m_axi_arregion(0) <= \<const0>\;
  m_axi_arsize(5) <= \<const0>\;
  m_axi_arsize(4) <= \<const0>\;
  m_axi_arsize(3) <= \<const0>\;
  m_axi_arsize(2) <= \<const0>\;
  m_axi_arsize(1) <= \<const0>\;
  m_axi_arsize(0) <= \<const0>\;
  m_axi_aruser(1) <= \<const0>\;
  m_axi_aruser(0) <= \<const0>\;
  m_axi_awaddr(49 downto 37) <= \^m_axi_awaddr\(49 downto 37);
  m_axi_awaddr(36 downto 25) <= \^m_axi_araddr\(11 downto 0);
  m_axi_awaddr(24 downto 12) <= \^m_axi_awaddr\(49 downto 37);
  m_axi_awaddr(11 downto 0) <= \^m_axi_araddr\(11 downto 0);
  m_axi_awburst(3) <= \<const0>\;
  m_axi_awburst(2) <= \<const0>\;
  m_axi_awburst(1) <= \<const0>\;
  m_axi_awburst(0) <= \<const0>\;
  m_axi_awcache(7) <= \<const0>\;
  m_axi_awcache(6) <= \<const0>\;
  m_axi_awcache(5) <= \<const0>\;
  m_axi_awcache(4) <= \<const0>\;
  m_axi_awcache(3) <= \<const0>\;
  m_axi_awcache(2) <= \<const0>\;
  m_axi_awcache(1) <= \<const0>\;
  m_axi_awcache(0) <= \<const0>\;
  m_axi_awid(1) <= \<const0>\;
  m_axi_awid(0) <= \<const0>\;
  m_axi_awlen(15) <= \<const0>\;
  m_axi_awlen(14) <= \<const0>\;
  m_axi_awlen(13) <= \<const0>\;
  m_axi_awlen(12) <= \<const0>\;
  m_axi_awlen(11) <= \<const0>\;
  m_axi_awlen(10) <= \<const0>\;
  m_axi_awlen(9) <= \<const0>\;
  m_axi_awlen(8) <= \<const0>\;
  m_axi_awlen(7) <= \<const0>\;
  m_axi_awlen(6) <= \<const0>\;
  m_axi_awlen(5) <= \<const0>\;
  m_axi_awlen(4) <= \<const0>\;
  m_axi_awlen(3) <= \<const0>\;
  m_axi_awlen(2) <= \<const0>\;
  m_axi_awlen(1) <= \<const0>\;
  m_axi_awlen(0) <= \<const0>\;
  m_axi_awlock(1) <= \<const0>\;
  m_axi_awlock(0) <= \<const0>\;
  m_axi_awprot(5 downto 3) <= \^m_axi_arprot\(2 downto 0);
  m_axi_awprot(2 downto 0) <= \^m_axi_arprot\(2 downto 0);
  m_axi_awqos(7) <= \<const0>\;
  m_axi_awqos(6) <= \<const0>\;
  m_axi_awqos(5) <= \<const0>\;
  m_axi_awqos(4) <= \<const0>\;
  m_axi_awqos(3) <= \<const0>\;
  m_axi_awqos(2) <= \<const0>\;
  m_axi_awqos(1) <= \<const0>\;
  m_axi_awqos(0) <= \<const0>\;
  m_axi_awregion(7) <= \<const0>\;
  m_axi_awregion(6) <= \<const0>\;
  m_axi_awregion(5) <= \<const0>\;
  m_axi_awregion(4) <= \<const0>\;
  m_axi_awregion(3) <= \<const0>\;
  m_axi_awregion(2) <= \<const0>\;
  m_axi_awregion(1) <= \<const0>\;
  m_axi_awregion(0) <= \<const0>\;
  m_axi_awsize(5) <= \<const0>\;
  m_axi_awsize(4) <= \<const0>\;
  m_axi_awsize(3) <= \<const0>\;
  m_axi_awsize(2) <= \<const0>\;
  m_axi_awsize(1) <= \<const0>\;
  m_axi_awsize(0) <= \<const0>\;
  m_axi_awuser(1) <= \<const0>\;
  m_axi_awuser(0) <= \<const0>\;
  m_axi_wdata(63 downto 32) <= \^s_axi_wdata\(31 downto 0);
  m_axi_wdata(31 downto 0) <= \^s_axi_wdata\(31 downto 0);
  m_axi_wid(1) <= \<const0>\;
  m_axi_wid(0) <= \<const0>\;
  m_axi_wlast(1) <= \<const0>\;
  m_axi_wlast(0) <= \<const0>\;
  m_axi_wstrb(7 downto 4) <= \^s_axi_wstrb\(3 downto 0);
  m_axi_wstrb(3 downto 0) <= \^s_axi_wstrb\(3 downto 0);
  m_axi_wuser(1) <= \<const0>\;
  m_axi_wuser(0) <= \<const0>\;
  s_axi_bid(0) <= \<const0>\;
  s_axi_buser(0) <= \<const0>\;
  s_axi_rid(0) <= \<const0>\;
  s_axi_rlast(0) <= \<const0>\;
  s_axi_ruser(0) <= \<const0>\;
GND: unisim.vcomponents.GND
     port map (
      G => \<const0>\
    );
\gen_sasd.crossbar_sasd_0\: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_crossbar_sasd
     port map (
      Q(27 downto 25) => \^m_axi_arprot\(2 downto 0),
      Q(24 downto 12) => \^m_axi_awaddr\(49 downto 37),
      Q(11 downto 0) => \^m_axi_araddr\(11 downto 0),
      aclk => aclk,
      aresetn => aresetn,
      m_axi_arready(1 downto 0) => m_axi_arready(1 downto 0),
      m_axi_arvalid(1 downto 0) => m_axi_arvalid(1 downto 0),
      m_axi_awready(1 downto 0) => m_axi_awready(1 downto 0),
      m_axi_awvalid(1 downto 0) => m_axi_awvalid(1 downto 0),
      m_axi_bready(1 downto 0) => m_axi_bready(1 downto 0),
      m_axi_bresp(3 downto 0) => m_axi_bresp(3 downto 0),
      m_axi_bvalid(1 downto 0) => m_axi_bvalid(1 downto 0),
      m_axi_rdata(63 downto 0) => m_axi_rdata(63 downto 0),
      m_axi_rready(1 downto 0) => m_axi_rready(1 downto 0),
      m_axi_rresp(3 downto 0) => m_axi_rresp(3 downto 0),
      m_axi_rvalid(1 downto 0) => m_axi_rvalid(1 downto 0),
      m_axi_wready(1 downto 0) => m_axi_wready(1 downto 0),
      m_axi_wvalid(1 downto 0) => m_axi_wvalid(1 downto 0),
      \m_payload_i_reg[34]\(33 downto 2) => s_axi_rdata(31 downto 0),
      \m_payload_i_reg[34]\(1 downto 0) => s_axi_rresp(1 downto 0),
      s_axi_araddr(24 downto 0) => s_axi_araddr(24 downto 0),
      s_axi_arprot(2 downto 0) => s_axi_arprot(2 downto 0),
      s_axi_arready(0) => s_axi_arready(0),
      s_axi_arvalid(0) => s_axi_arvalid(0),
      s_axi_awaddr(24 downto 0) => s_axi_awaddr(24 downto 0),
      s_axi_awprot(2 downto 0) => s_axi_awprot(2 downto 0),
      s_axi_awready(0) => s_axi_awready(0),
      s_axi_awvalid(0) => s_axi_awvalid(0),
      s_axi_bready(0) => s_axi_bready(0),
      s_axi_bresp(1 downto 0) => s_axi_bresp(1 downto 0),
      s_axi_bvalid(0) => s_axi_bvalid(0),
      s_axi_rready(0) => s_axi_rready(0),
      s_axi_rvalid(0) => s_axi_rvalid(0),
      s_axi_wready(0) => s_axi_wready(0),
      s_axi_wvalid(0) => s_axi_wvalid(0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    aclk : in STD_LOGIC;
    aresetn : in STD_LOGIC;
    s_axi_awaddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_awvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_awready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_wvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_wready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_bvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_bready : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_araddr : in STD_LOGIC_VECTOR ( 24 downto 0 );
    s_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    s_axi_arvalid : in STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_arready : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_rvalid : out STD_LOGIC_VECTOR ( 0 to 0 );
    s_axi_rready : in STD_LOGIC_VECTOR ( 0 to 0 );
    m_axi_awaddr : out STD_LOGIC_VECTOR ( 49 downto 0 );
    m_axi_awprot : out STD_LOGIC_VECTOR ( 5 downto 0 );
    m_axi_awvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_awready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wdata : out STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_wstrb : out STD_LOGIC_VECTOR ( 7 downto 0 );
    m_axi_wvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_wready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_bvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_bready : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_araddr : out STD_LOGIC_VECTOR ( 49 downto 0 );
    m_axi_arprot : out STD_LOGIC_VECTOR ( 5 downto 0 );
    m_axi_arvalid : out STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_arready : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rdata : in STD_LOGIC_VECTOR ( 63 downto 0 );
    m_axi_rresp : in STD_LOGIC_VECTOR ( 3 downto 0 );
    m_axi_rvalid : in STD_LOGIC_VECTOR ( 1 downto 0 );
    m_axi_rready : out STD_LOGIC_VECTOR ( 1 downto 0 )
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "pfm_dynamic_xbar_0,axi_crossbar_v2_1_22_axi_crossbar,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "axi_crossbar_v2_1_22_axi_crossbar,Vivado 2020.1";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  signal NLW_inst_m_axi_arburst_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_inst_m_axi_arcache_UNCONNECTED : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_inst_m_axi_arid_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_arlen_UNCONNECTED : STD_LOGIC_VECTOR ( 15 downto 0 );
  signal NLW_inst_m_axi_arlock_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_arqos_UNCONNECTED : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_inst_m_axi_arregion_UNCONNECTED : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_inst_m_axi_arsize_UNCONNECTED : STD_LOGIC_VECTOR ( 5 downto 0 );
  signal NLW_inst_m_axi_aruser_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_awburst_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_inst_m_axi_awcache_UNCONNECTED : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_inst_m_axi_awid_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_awlen_UNCONNECTED : STD_LOGIC_VECTOR ( 15 downto 0 );
  signal NLW_inst_m_axi_awlock_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_awqos_UNCONNECTED : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_inst_m_axi_awregion_UNCONNECTED : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_inst_m_axi_awsize_UNCONNECTED : STD_LOGIC_VECTOR ( 5 downto 0 );
  signal NLW_inst_m_axi_awuser_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_wid_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_wlast_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_m_axi_wuser_UNCONNECTED : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal NLW_inst_s_axi_bid_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_inst_s_axi_buser_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_inst_s_axi_rid_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_inst_s_axi_rlast_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  signal NLW_inst_s_axi_ruser_UNCONNECTED : STD_LOGIC_VECTOR ( 0 to 0 );
  attribute C_AXI_ADDR_WIDTH : integer;
  attribute C_AXI_ADDR_WIDTH of inst : label is 25;
  attribute C_AXI_ARUSER_WIDTH : integer;
  attribute C_AXI_ARUSER_WIDTH of inst : label is 1;
  attribute C_AXI_AWUSER_WIDTH : integer;
  attribute C_AXI_AWUSER_WIDTH of inst : label is 1;
  attribute C_AXI_BUSER_WIDTH : integer;
  attribute C_AXI_BUSER_WIDTH of inst : label is 1;
  attribute C_AXI_DATA_WIDTH : integer;
  attribute C_AXI_DATA_WIDTH of inst : label is 32;
  attribute C_AXI_ID_WIDTH : integer;
  attribute C_AXI_ID_WIDTH of inst : label is 1;
  attribute C_AXI_PROTOCOL : integer;
  attribute C_AXI_PROTOCOL of inst : label is 2;
  attribute C_AXI_RUSER_WIDTH : integer;
  attribute C_AXI_RUSER_WIDTH of inst : label is 1;
  attribute C_AXI_SUPPORTS_USER_SIGNALS : integer;
  attribute C_AXI_SUPPORTS_USER_SIGNALS of inst : label is 0;
  attribute C_AXI_WUSER_WIDTH : integer;
  attribute C_AXI_WUSER_WIDTH of inst : label is 1;
  attribute C_CONNECTIVITY_MODE : integer;
  attribute C_CONNECTIVITY_MODE of inst : label is 0;
  attribute C_DEBUG : integer;
  attribute C_DEBUG of inst : label is 1;
  attribute C_FAMILY : string;
  attribute C_FAMILY of inst : label is "virtexuplus";
  attribute C_M_AXI_ADDR_WIDTH : string;
  attribute C_M_AXI_ADDR_WIDTH of inst : label is "320'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000010000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000001100";
  attribute C_M_AXI_BASE_ADDR : string;
  attribute C_M_AXI_BASE_ADDR of inst : label is "640'b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000000000000000000000000000000000000000110000101000000000000000000000000000000000000000000000000000000011000010000000000000000000000000000000000000000000000000000000001100000110000000000000000000000000000000000000000000000000000000110000010000000000000000000000000000000000000000000000000000000011000000100000000000000000000000000000000000000000000000000000001100000000000000000000000";
  attribute C_M_AXI_READ_CONNECTIVITY : string;
  attribute C_M_AXI_READ_CONNECTIVITY of inst : label is "64'b1111111111111111111111111111111111111111111111111111111111111111";
  attribute C_M_AXI_READ_ISSUING : string;
  attribute C_M_AXI_READ_ISSUING of inst : label is "64'b0000000000000000000000000000000100000000000000000000000000000001";
  attribute C_M_AXI_SECURE : string;
  attribute C_M_AXI_SECURE of inst : label is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute C_M_AXI_WRITE_CONNECTIVITY : string;
  attribute C_M_AXI_WRITE_CONNECTIVITY of inst : label is "64'b1111111111111111111111111111111111111111111111111111111111111111";
  attribute C_M_AXI_WRITE_ISSUING : string;
  attribute C_M_AXI_WRITE_ISSUING of inst : label is "64'b0000000000000000000000000000000100000000000000000000000000000001";
  attribute C_NUM_ADDR_RANGES : integer;
  attribute C_NUM_ADDR_RANGES of inst : label is 5;
  attribute C_NUM_MASTER_SLOTS : integer;
  attribute C_NUM_MASTER_SLOTS of inst : label is 2;
  attribute C_NUM_SLAVE_SLOTS : integer;
  attribute C_NUM_SLAVE_SLOTS of inst : label is 1;
  attribute C_R_REGISTER : integer;
  attribute C_R_REGISTER of inst : label is 1;
  attribute C_S_AXI_ARB_PRIORITY : integer;
  attribute C_S_AXI_ARB_PRIORITY of inst : label is 0;
  attribute C_S_AXI_BASE_ID : integer;
  attribute C_S_AXI_BASE_ID of inst : label is 0;
  attribute C_S_AXI_READ_ACCEPTANCE : integer;
  attribute C_S_AXI_READ_ACCEPTANCE of inst : label is 1;
  attribute C_S_AXI_SINGLE_THREAD : integer;
  attribute C_S_AXI_SINGLE_THREAD of inst : label is 1;
  attribute C_S_AXI_THREAD_ID_WIDTH : integer;
  attribute C_S_AXI_THREAD_ID_WIDTH of inst : label is 0;
  attribute C_S_AXI_WRITE_ACCEPTANCE : integer;
  attribute C_S_AXI_WRITE_ACCEPTANCE of inst : label is 1;
  attribute DowngradeIPIdentifiedWarnings of inst : label is "yes";
  attribute P_ADDR_DECODE : integer;
  attribute P_ADDR_DECODE of inst : label is 1;
  attribute P_AXI3 : integer;
  attribute P_AXI3 of inst : label is 1;
  attribute P_AXI4 : integer;
  attribute P_AXI4 of inst : label is 0;
  attribute P_AXILITE : integer;
  attribute P_AXILITE of inst : label is 2;
  attribute P_AXILITE_SIZE : string;
  attribute P_AXILITE_SIZE of inst : label is "3'b010";
  attribute P_FAMILY : string;
  attribute P_FAMILY of inst : label is "rtl";
  attribute P_INCR : string;
  attribute P_INCR of inst : label is "2'b01";
  attribute P_LEN : integer;
  attribute P_LEN of inst : label is 8;
  attribute P_LOCK : integer;
  attribute P_LOCK of inst : label is 1;
  attribute P_M_AXI_ERR_MODE : string;
  attribute P_M_AXI_ERR_MODE of inst : label is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute P_M_AXI_SUPPORTS_READ : string;
  attribute P_M_AXI_SUPPORTS_READ of inst : label is "2'b11";
  attribute P_M_AXI_SUPPORTS_WRITE : string;
  attribute P_M_AXI_SUPPORTS_WRITE of inst : label is "2'b11";
  attribute P_ONES : string;
  attribute P_ONES of inst : label is "65'b11111111111111111111111111111111111111111111111111111111111111111";
  attribute P_RANGE_CHECK : integer;
  attribute P_RANGE_CHECK of inst : label is 1;
  attribute P_S_AXI_BASE_ID : string;
  attribute P_S_AXI_BASE_ID of inst : label is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute P_S_AXI_HIGH_ID : string;
  attribute P_S_AXI_HIGH_ID of inst : label is "64'b0000000000000000000000000000000000000000000000000000000000000000";
  attribute P_S_AXI_SUPPORTS_READ : string;
  attribute P_S_AXI_SUPPORTS_READ of inst : label is "1'b1";
  attribute P_S_AXI_SUPPORTS_WRITE : string;
  attribute P_S_AXI_SUPPORTS_WRITE of inst : label is "1'b1";
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of aclk : signal is "xilinx.com:signal:clock:1.0 CLKIF CLK";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of aclk : signal is "XIL_INTERFACENAME CLKIF, FREQ_HZ 50925925, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, ASSOCIATED_BUSIF M00_AXI:M01_AXI:M02_AXI:M03_AXI:M04_AXI:M05_AXI:M06_AXI:M07_AXI:M08_AXI:M09_AXI:M10_AXI:M11_AXI:M12_AXI:M13_AXI:M14_AXI:M15_AXI:S00_AXI:S01_AXI:S02_AXI:S03_AXI:S04_AXI:S05_AXI:S06_AXI:S07_AXI:S08_AXI:S09_AXI:S10_AXI:S11_AXI:S12_AXI:S13_AXI:S14_AXI:S15_AXI, ASSOCIATED_RESET ARESETN, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of aresetn : signal is "xilinx.com:signal:reset:1.0 RSTIF RST";
  attribute X_INTERFACE_PARAMETER of aresetn : signal is "XIL_INTERFACENAME RSTIF, POLARITY ACTIVE_LOW, INSERT_VIP 0, TYPE INTERCONNECT";
  attribute X_INTERFACE_INFO of m_axi_araddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR [24:0] [24:0], xilinx.com:interface:aximm:1.0 M01_AXI ARADDR [24:0] [49:25]";
  attribute X_INTERFACE_INFO of m_axi_arprot : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARPROT [2:0] [2:0], xilinx.com:interface:aximm:1.0 M01_AXI ARPROT [2:0] [5:3]";
  attribute X_INTERFACE_INFO of m_axi_arready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI ARREADY [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_arvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI ARVALID [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_awaddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR [24:0] [24:0], xilinx.com:interface:aximm:1.0 M01_AXI AWADDR [24:0] [49:25]";
  attribute X_INTERFACE_INFO of m_axi_awprot : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWPROT [2:0] [2:0], xilinx.com:interface:aximm:1.0 M01_AXI AWPROT [2:0] [5:3]";
  attribute X_INTERFACE_INFO of m_axi_awready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI AWREADY [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_awvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI AWVALID [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_bready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI BREADY [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_bresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BRESP [1:0] [1:0], xilinx.com:interface:aximm:1.0 M01_AXI BRESP [1:0] [3:2]";
  attribute X_INTERFACE_INFO of m_axi_bvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI BVALID [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_rdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RDATA [31:0] [31:0], xilinx.com:interface:aximm:1.0 M01_AXI RDATA [31:0] [63:32]";
  attribute X_INTERFACE_INFO of m_axi_rready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI RREADY [0:0] [1:1]";
  attribute X_INTERFACE_PARAMETER of m_axi_rready : signal is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0, XIL_INTERFACENAME M01_AXI, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 1, NUM_WRITE_OUTSTANDING 1, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of m_axi_rresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RRESP [1:0] [1:0], xilinx.com:interface:aximm:1.0 M01_AXI RRESP [1:0] [3:2]";
  attribute X_INTERFACE_INFO of m_axi_rvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI RVALID [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_wdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA [31:0] [31:0], xilinx.com:interface:aximm:1.0 M01_AXI WDATA [31:0] [63:32]";
  attribute X_INTERFACE_INFO of m_axi_wready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI WREADY [0:0] [1:1]";
  attribute X_INTERFACE_INFO of m_axi_wstrb : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB [3:0] [3:0], xilinx.com:interface:aximm:1.0 M01_AXI WSTRB [3:0] [7:4]";
  attribute X_INTERFACE_INFO of m_axi_wvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI WVALID [0:0] [1:1]";
  attribute X_INTERFACE_INFO of s_axi_araddr : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARADDR";
  attribute X_INTERFACE_INFO of s_axi_arprot : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARPROT";
  attribute X_INTERFACE_INFO of s_axi_arready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARREADY";
  attribute X_INTERFACE_INFO of s_axi_arvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI ARVALID";
  attribute X_INTERFACE_INFO of s_axi_awaddr : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWADDR";
  attribute X_INTERFACE_INFO of s_axi_awprot : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWPROT";
  attribute X_INTERFACE_INFO of s_axi_awready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWREADY";
  attribute X_INTERFACE_INFO of s_axi_awvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI AWVALID";
  attribute X_INTERFACE_INFO of s_axi_bready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BREADY";
  attribute X_INTERFACE_INFO of s_axi_bresp : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BRESP";
  attribute X_INTERFACE_INFO of s_axi_bvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI BVALID";
  attribute X_INTERFACE_INFO of s_axi_rdata : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RDATA";
  attribute X_INTERFACE_INFO of s_axi_rready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RREADY";
  attribute X_INTERFACE_PARAMETER of s_axi_rready : signal is "XIL_INTERFACENAME S00_AXI, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of s_axi_rresp : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RRESP";
  attribute X_INTERFACE_INFO of s_axi_rvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI RVALID";
  attribute X_INTERFACE_INFO of s_axi_wdata : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WDATA";
  attribute X_INTERFACE_INFO of s_axi_wready : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WREADY";
  attribute X_INTERFACE_INFO of s_axi_wstrb : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WSTRB";
  attribute X_INTERFACE_INFO of s_axi_wvalid : signal is "xilinx.com:interface:aximm:1.0 S00_AXI WVALID";
begin
inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar
     port map (
      aclk => aclk,
      aresetn => aresetn,
      m_axi_araddr(49 downto 0) => m_axi_araddr(49 downto 0),
      m_axi_arburst(3 downto 0) => NLW_inst_m_axi_arburst_UNCONNECTED(3 downto 0),
      m_axi_arcache(7 downto 0) => NLW_inst_m_axi_arcache_UNCONNECTED(7 downto 0),
      m_axi_arid(1 downto 0) => NLW_inst_m_axi_arid_UNCONNECTED(1 downto 0),
      m_axi_arlen(15 downto 0) => NLW_inst_m_axi_arlen_UNCONNECTED(15 downto 0),
      m_axi_arlock(1 downto 0) => NLW_inst_m_axi_arlock_UNCONNECTED(1 downto 0),
      m_axi_arprot(5 downto 0) => m_axi_arprot(5 downto 0),
      m_axi_arqos(7 downto 0) => NLW_inst_m_axi_arqos_UNCONNECTED(7 downto 0),
      m_axi_arready(1 downto 0) => m_axi_arready(1 downto 0),
      m_axi_arregion(7 downto 0) => NLW_inst_m_axi_arregion_UNCONNECTED(7 downto 0),
      m_axi_arsize(5 downto 0) => NLW_inst_m_axi_arsize_UNCONNECTED(5 downto 0),
      m_axi_aruser(1 downto 0) => NLW_inst_m_axi_aruser_UNCONNECTED(1 downto 0),
      m_axi_arvalid(1 downto 0) => m_axi_arvalid(1 downto 0),
      m_axi_awaddr(49 downto 0) => m_axi_awaddr(49 downto 0),
      m_axi_awburst(3 downto 0) => NLW_inst_m_axi_awburst_UNCONNECTED(3 downto 0),
      m_axi_awcache(7 downto 0) => NLW_inst_m_axi_awcache_UNCONNECTED(7 downto 0),
      m_axi_awid(1 downto 0) => NLW_inst_m_axi_awid_UNCONNECTED(1 downto 0),
      m_axi_awlen(15 downto 0) => NLW_inst_m_axi_awlen_UNCONNECTED(15 downto 0),
      m_axi_awlock(1 downto 0) => NLW_inst_m_axi_awlock_UNCONNECTED(1 downto 0),
      m_axi_awprot(5 downto 0) => m_axi_awprot(5 downto 0),
      m_axi_awqos(7 downto 0) => NLW_inst_m_axi_awqos_UNCONNECTED(7 downto 0),
      m_axi_awready(1 downto 0) => m_axi_awready(1 downto 0),
      m_axi_awregion(7 downto 0) => NLW_inst_m_axi_awregion_UNCONNECTED(7 downto 0),
      m_axi_awsize(5 downto 0) => NLW_inst_m_axi_awsize_UNCONNECTED(5 downto 0),
      m_axi_awuser(1 downto 0) => NLW_inst_m_axi_awuser_UNCONNECTED(1 downto 0),
      m_axi_awvalid(1 downto 0) => m_axi_awvalid(1 downto 0),
      m_axi_bid(1 downto 0) => B"00",
      m_axi_bready(1 downto 0) => m_axi_bready(1 downto 0),
      m_axi_bresp(3 downto 0) => m_axi_bresp(3 downto 0),
      m_axi_buser(1 downto 0) => B"00",
      m_axi_bvalid(1 downto 0) => m_axi_bvalid(1 downto 0),
      m_axi_rdata(63 downto 0) => m_axi_rdata(63 downto 0),
      m_axi_rid(1 downto 0) => B"00",
      m_axi_rlast(1 downto 0) => B"11",
      m_axi_rready(1 downto 0) => m_axi_rready(1 downto 0),
      m_axi_rresp(3 downto 0) => m_axi_rresp(3 downto 0),
      m_axi_ruser(1 downto 0) => B"00",
      m_axi_rvalid(1 downto 0) => m_axi_rvalid(1 downto 0),
      m_axi_wdata(63 downto 0) => m_axi_wdata(63 downto 0),
      m_axi_wid(1 downto 0) => NLW_inst_m_axi_wid_UNCONNECTED(1 downto 0),
      m_axi_wlast(1 downto 0) => NLW_inst_m_axi_wlast_UNCONNECTED(1 downto 0),
      m_axi_wready(1 downto 0) => m_axi_wready(1 downto 0),
      m_axi_wstrb(7 downto 0) => m_axi_wstrb(7 downto 0),
      m_axi_wuser(1 downto 0) => NLW_inst_m_axi_wuser_UNCONNECTED(1 downto 0),
      m_axi_wvalid(1 downto 0) => m_axi_wvalid(1 downto 0),
      s_axi_araddr(24 downto 0) => s_axi_araddr(24 downto 0),
      s_axi_arburst(1 downto 0) => B"00",
      s_axi_arcache(3 downto 0) => B"0000",
      s_axi_arid(0) => '0',
      s_axi_arlen(7 downto 0) => B"00000000",
      s_axi_arlock(0) => '0',
      s_axi_arprot(2 downto 0) => s_axi_arprot(2 downto 0),
      s_axi_arqos(3 downto 0) => B"0000",
      s_axi_arready(0) => s_axi_arready(0),
      s_axi_arsize(2 downto 0) => B"000",
      s_axi_aruser(0) => '0',
      s_axi_arvalid(0) => s_axi_arvalid(0),
      s_axi_awaddr(24 downto 0) => s_axi_awaddr(24 downto 0),
      s_axi_awburst(1 downto 0) => B"00",
      s_axi_awcache(3 downto 0) => B"0000",
      s_axi_awid(0) => '0',
      s_axi_awlen(7 downto 0) => B"00000000",
      s_axi_awlock(0) => '0',
      s_axi_awprot(2 downto 0) => s_axi_awprot(2 downto 0),
      s_axi_awqos(3 downto 0) => B"0000",
      s_axi_awready(0) => s_axi_awready(0),
      s_axi_awsize(2 downto 0) => B"000",
      s_axi_awuser(0) => '0',
      s_axi_awvalid(0) => s_axi_awvalid(0),
      s_axi_bid(0) => NLW_inst_s_axi_bid_UNCONNECTED(0),
      s_axi_bready(0) => s_axi_bready(0),
      s_axi_bresp(1 downto 0) => s_axi_bresp(1 downto 0),
      s_axi_buser(0) => NLW_inst_s_axi_buser_UNCONNECTED(0),
      s_axi_bvalid(0) => s_axi_bvalid(0),
      s_axi_rdata(31 downto 0) => s_axi_rdata(31 downto 0),
      s_axi_rid(0) => NLW_inst_s_axi_rid_UNCONNECTED(0),
      s_axi_rlast(0) => NLW_inst_s_axi_rlast_UNCONNECTED(0),
      s_axi_rready(0) => s_axi_rready(0),
      s_axi_rresp(1 downto 0) => s_axi_rresp(1 downto 0),
      s_axi_ruser(0) => NLW_inst_s_axi_ruser_UNCONNECTED(0),
      s_axi_rvalid(0) => s_axi_rvalid(0),
      s_axi_wdata(31 downto 0) => s_axi_wdata(31 downto 0),
      s_axi_wid(0) => '0',
      s_axi_wlast(0) => '1',
      s_axi_wready(0) => s_axi_wready(0),
      s_axi_wstrb(3 downto 0) => s_axi_wstrb(3 downto 0),
      s_axi_wuser(0) => '0',
      s_axi_wvalid(0) => s_axi_wvalid(0)
    );
end STRUCTURE;
