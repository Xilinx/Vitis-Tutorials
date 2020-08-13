// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.1 (lin64) Build 2883519 Mon May  4 20:18:30 MDT 2020
// Date        : Wed May  6 15:17:13 2020
// Host        : xsjrdevl100 running 64-bit CentOS Linux release 7.4.1708 (Core)
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ pfm_dynamic_xbar_0_sim_netlist.v
// Design      : pfm_dynamic_xbar_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xcu200-fsgd2104-2-e
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_addr_arbiter_sasd
   (m_valid_i,
    SR,
    aa_grant_rnw,
    any_error,
    Q,
    D,
    m_ready_d0,
    m_axi_awvalid,
    s_axi_bvalid,
    s_axi_wready,
    \m_ready_d_reg[2] ,
    \m_atarget_enc_reg[1] ,
    m_axi_bready,
    \gen_no_arbiter.grant_rnw_reg_0 ,
    m_axi_wvalid,
    s_axi_wvalid_0_sp_1,
    \gen_no_arbiter.grant_rnw_reg_1 ,
    E,
    aresetn_d_reg,
    r_transfer_en,
    m_axi_arvalid,
    mi_arvalid_en,
    s_axi_awready,
    s_axi_arready,
    s_axi_rvalid,
    aresetn_d_reg_0,
    \gen_axilite.s_axi_bvalid_i_reg ,
    aclk,
    \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0 ,
    \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1 ,
    s_axi_awvalid,
    s_axi_arvalid,
    aresetn_d,
    \gen_axilite.s_axi_bvalid_i_reg_0 ,
    \gen_axilite.s_axi_bvalid_i_reg_1 ,
    f_mux_return__3,
    f_mux_return__1,
    f_mux_return,
    m_atarget_enc,
    m_axi_awready,
    mi_wready,
    s_axi_bready,
    mi_bvalid,
    s_axi_wvalid,
    \m_axi_arvalid[0] ,
    s_axi_rready,
    sr_rvalid,
    \m_ready_d_reg[0] ,
    f_mux_return__0,
    s_axi_arprot,
    s_axi_awprot,
    s_axi_araddr,
    s_axi_awaddr);
  output m_valid_i;
  output [0:0]SR;
  output aa_grant_rnw;
  output any_error;
  output [27:0]Q;
  output [2:0]D;
  output [1:0]m_ready_d0;
  output [1:0]m_axi_awvalid;
  output [0:0]s_axi_bvalid;
  output [0:0]s_axi_wready;
  output [2:0]\m_ready_d_reg[2] ;
  output \m_atarget_enc_reg[1] ;
  output [1:0]m_axi_bready;
  output \gen_no_arbiter.grant_rnw_reg_0 ;
  output [1:0]m_axi_wvalid;
  output s_axi_wvalid_0_sp_1;
  output \gen_no_arbiter.grant_rnw_reg_1 ;
  output [0:0]E;
  output [0:0]aresetn_d_reg;
  output r_transfer_en;
  output [1:0]m_axi_arvalid;
  output mi_arvalid_en;
  output [0:0]s_axi_awready;
  output [0:0]s_axi_arready;
  output [0:0]s_axi_rvalid;
  output aresetn_d_reg_0;
  output \gen_axilite.s_axi_bvalid_i_reg ;
  input aclk;
  input \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0 ;
  input \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1 ;
  input [0:0]s_axi_awvalid;
  input [0:0]s_axi_arvalid;
  input aresetn_d;
  input [2:0]\gen_axilite.s_axi_bvalid_i_reg_0 ;
  input [2:0]\gen_axilite.s_axi_bvalid_i_reg_1 ;
  input f_mux_return__3;
  input f_mux_return__1;
  input f_mux_return;
  input [1:0]m_atarget_enc;
  input [1:0]m_axi_awready;
  input [0:0]mi_wready;
  input [0:0]s_axi_bready;
  input [0:0]mi_bvalid;
  input [0:0]s_axi_wvalid;
  input [1:0]\m_axi_arvalid[0] ;
  input [0:0]s_axi_rready;
  input sr_rvalid;
  input [0:0]\m_ready_d_reg[0] ;
  input f_mux_return__0;
  input [2:0]s_axi_arprot;
  input [2:0]s_axi_awprot;
  input [24:0]s_axi_araddr;
  input [24:0]s_axi_awaddr;

  wire [2:0]D;
  wire [0:0]E;
  wire [27:0]Q;
  wire [0:0]SR;
  wire aa_grant_rnw;
  wire aclk;
  wire any_error;
  wire aresetn_d;
  wire [0:0]aresetn_d_reg;
  wire aresetn_d_reg_0;
  wire f_mux_return;
  wire f_mux_return__0;
  wire f_mux_return__1;
  wire f_mux_return__3;
  wire [0:0]\gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I ;
  wire \gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ;
  wire \gen_axilite.s_axi_bvalid_i_i_2_n_0 ;
  wire \gen_axilite.s_axi_bvalid_i_reg ;
  wire [2:0]\gen_axilite.s_axi_bvalid_i_reg_0 ;
  wire [2:0]\gen_axilite.s_axi_bvalid_i_reg_1 ;
  wire \gen_no_arbiter.grant_rnw_i_1_n_0 ;
  wire \gen_no_arbiter.grant_rnw_reg_0 ;
  wire \gen_no_arbiter.grant_rnw_reg_1 ;
  wire \gen_no_arbiter.m_grant_hot_i[0]_inv_i_1_n_0 ;
  wire \gen_no_arbiter.m_grant_hot_i[0]_inv_i_2_n_0 ;
  wire \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0 ;
  wire \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0 ;
  wire \gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1 ;
  wire \gen_no_arbiter.m_valid_i_i_1_n_0 ;
  wire \gen_no_arbiter.s_ready_i[0]_i_1_n_0 ;
  wire [1:0]m_atarget_enc;
  wire \m_atarget_enc_reg[1] ;
  wire \m_atarget_hot[2]_i_4_n_0 ;
  wire [1:0]m_axi_arvalid;
  wire [1:0]\m_axi_arvalid[0] ;
  wire [1:0]m_axi_awready;
  wire [1:0]m_axi_awvalid;
  wire [1:0]m_axi_bready;
  wire [1:0]m_axi_wvalid;
  wire [1:0]m_ready_d0;
  wire \m_ready_d[2]_i_5_n_0 ;
  wire [0:0]\m_ready_d_reg[0] ;
  wire [2:0]\m_ready_d_reg[2] ;
  wire m_valid_i;
  wire mi_arvalid_en;
  wire [0:0]mi_bvalid;
  wire [0:0]mi_wready;
  wire p_0_in1_in;
  wire p_0_out;
  wire r_transfer_en;
  wire [41:1]s_amesg;
  wire s_arvalid_reg;
  wire \s_arvalid_reg_reg_n_0_[0] ;
  wire s_awvalid_reg;
  wire s_awvalid_reg0;
  wire [24:0]s_axi_araddr;
  wire [2:0]s_axi_arprot;
  wire [0:0]s_axi_arready;
  wire [0:0]s_axi_arvalid;
  wire [24:0]s_axi_awaddr;
  wire [2:0]s_axi_awprot;
  wire [0:0]s_axi_awready;
  wire [0:0]s_axi_awvalid;
  wire [0:0]s_axi_bready;
  wire [0:0]s_axi_bvalid;
  wire [0:0]s_axi_rready;
  wire [0:0]s_axi_rvalid;
  wire [0:0]s_axi_wready;
  wire [0:0]s_axi_wvalid;
  wire s_axi_wvalid_0_sn_1;
  wire s_ready_i;
  wire sr_rvalid;

  assign s_axi_wvalid_0_sp_1 = s_axi_wvalid_0_sn_1;
  LUT6 #(
    .INIT(64'h5A5A5E5A50505050)) 
    \gen_axilite.s_axi_bvalid_i_i_1 
       (.I0(\gen_axilite.s_axi_bvalid_i_i_2_n_0 ),
        .I1(s_axi_wvalid_0_sn_1),
        .I2(mi_bvalid),
        .I3(\gen_axilite.s_axi_bvalid_i_reg_1 [2]),
        .I4(\gen_axilite.s_axi_bvalid_i_reg_0 [2]),
        .I5(mi_wready),
        .O(\gen_axilite.s_axi_bvalid_i_reg ));
  LUT6 #(
    .INIT(64'h0000000008000000)) 
    \gen_axilite.s_axi_bvalid_i_i_2 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_1 [2]),
        .I1(mi_bvalid),
        .I2(\gen_axilite.s_axi_bvalid_i_reg_0 [0]),
        .I3(s_axi_bready),
        .I4(m_valid_i),
        .I5(aa_grant_rnw),
        .O(\gen_axilite.s_axi_bvalid_i_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h0008)) 
    \gen_axilite.s_axi_bvalid_i_i_3 
       (.I0(s_axi_wvalid),
        .I1(m_valid_i),
        .I2(aa_grant_rnw),
        .I3(\gen_axilite.s_axi_bvalid_i_reg_0 [1]),
        .O(s_axi_wvalid_0_sn_1));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT3 #(
    .INIT(8'h08)) 
    \gen_axilite.s_axi_rvalid_i_i_2 
       (.I0(aa_grant_rnw),
        .I1(m_valid_i),
        .I2(\m_axi_arvalid[0] [1]),
        .O(mi_arvalid_en));
  LUT6 #(
    .INIT(64'hDFCFDFFF10001000)) 
    \gen_no_arbiter.grant_rnw_i_1 
       (.I0(s_awvalid_reg),
        .I1(m_valid_i),
        .I2(p_0_in1_in),
        .I3(s_axi_arvalid),
        .I4(s_axi_awvalid),
        .I5(aa_grant_rnw),
        .O(\gen_no_arbiter.grant_rnw_i_1_n_0 ));
  FDRE \gen_no_arbiter.grant_rnw_reg 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_no_arbiter.grant_rnw_i_1_n_0 ),
        .Q(aa_grant_rnw),
        .R(SR));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[10]_i_1 
       (.I0(s_axi_araddr[9]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[9]),
        .O(s_amesg[10]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[11]_i_1 
       (.I0(s_axi_araddr[10]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[10]),
        .O(s_amesg[11]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[12]_i_1 
       (.I0(s_axi_araddr[11]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[11]),
        .O(s_amesg[12]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[13]_i_1 
       (.I0(s_axi_araddr[12]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[12]),
        .O(s_amesg[13]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[14]_i_1 
       (.I0(s_axi_araddr[13]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[13]),
        .O(s_amesg[14]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[15]_i_1 
       (.I0(s_axi_araddr[14]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[14]),
        .O(s_amesg[15]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[16]_i_1 
       (.I0(s_axi_araddr[15]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[15]),
        .O(s_amesg[16]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[17]_i_1 
       (.I0(s_axi_araddr[16]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[16]),
        .O(s_amesg[17]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[18]_i_1 
       (.I0(s_axi_araddr[17]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[17]),
        .O(s_amesg[18]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[19]_i_1 
       (.I0(s_axi_araddr[18]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[18]),
        .O(s_amesg[19]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[1]_i_1 
       (.I0(s_axi_araddr[0]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[0]),
        .O(s_amesg[1]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[20]_i_1 
       (.I0(s_axi_araddr[19]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[19]),
        .O(s_amesg[20]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[21]_i_1 
       (.I0(s_axi_araddr[20]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[20]),
        .O(s_amesg[21]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[22]_i_1 
       (.I0(s_axi_araddr[21]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[21]),
        .O(s_amesg[22]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[23]_i_1 
       (.I0(s_axi_araddr[22]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[22]),
        .O(s_amesg[23]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[24]_i_1 
       (.I0(s_axi_araddr[23]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[23]),
        .O(s_amesg[24]));
  LUT1 #(
    .INIT(2'h1)) 
    \gen_no_arbiter.m_amesg_i[25]_i_1 
       (.I0(aresetn_d),
        .O(SR));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[25]_i_2 
       (.I0(s_axi_araddr[24]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[24]),
        .O(s_amesg[25]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[2]_i_1 
       (.I0(s_axi_araddr[1]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[1]),
        .O(s_amesg[2]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[39]_i_1 
       (.I0(s_axi_arprot[0]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awprot[0]),
        .O(s_amesg[39]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[3]_i_1 
       (.I0(s_axi_araddr[2]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[2]),
        .O(s_amesg[3]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[40]_i_1 
       (.I0(s_axi_arprot[1]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awprot[1]),
        .O(s_amesg[40]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[41]_i_1 
       (.I0(s_axi_arprot[2]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awprot[2]),
        .O(s_amesg[41]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[4]_i_1 
       (.I0(s_axi_araddr[3]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[3]),
        .O(s_amesg[4]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[5]_i_1 
       (.I0(s_axi_araddr[4]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[4]),
        .O(s_amesg[5]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[6]_i_1 
       (.I0(s_axi_araddr[5]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[5]),
        .O(s_amesg[6]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[7]_i_1 
       (.I0(s_axi_araddr[6]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[6]),
        .O(s_amesg[7]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[8]_i_1 
       (.I0(s_axi_araddr[7]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[7]),
        .O(s_amesg[8]));
  LUT4 #(
    .INIT(16'hFB08)) 
    \gen_no_arbiter.m_amesg_i[9]_i_1 
       (.I0(s_axi_araddr[8]),
        .I1(s_axi_arvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_awaddr[8]),
        .O(s_amesg[9]));
  FDRE \gen_no_arbiter.m_amesg_i_reg[10] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[10]),
        .Q(Q[9]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[11] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[11]),
        .Q(Q[10]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[12] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[12]),
        .Q(Q[11]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[13] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[13]),
        .Q(Q[12]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[14] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[14]),
        .Q(Q[13]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[15] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[15]),
        .Q(Q[14]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[16] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[16]),
        .Q(Q[15]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[17] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[17]),
        .Q(Q[16]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[18] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[18]),
        .Q(Q[17]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[19] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[19]),
        .Q(Q[18]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[1] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[1]),
        .Q(Q[0]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[20] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[20]),
        .Q(Q[19]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[21] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[21]),
        .Q(Q[20]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[22] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[22]),
        .Q(Q[21]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[23] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[23]),
        .Q(Q[22]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[24] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[24]),
        .Q(Q[23]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[25] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[25]),
        .Q(Q[24]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[2] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[2]),
        .Q(Q[1]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[39] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[39]),
        .Q(Q[25]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[3] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[3]),
        .Q(Q[2]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[40] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[40]),
        .Q(Q[26]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[41] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[41]),
        .Q(Q[27]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[4] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[4]),
        .Q(Q[3]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[5] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[5]),
        .Q(Q[4]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[6] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[6]),
        .Q(Q[5]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[7] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[7]),
        .Q(Q[6]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[8] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[8]),
        .Q(Q[7]),
        .R(SR));
  FDRE \gen_no_arbiter.m_amesg_i_reg[9] 
       (.C(aclk),
        .CE(p_0_in1_in),
        .D(s_amesg[9]),
        .Q(Q[8]),
        .R(SR));
  LUT6 #(
    .INIT(64'hDDDDD555D555D555)) 
    \gen_no_arbiter.m_grant_hot_i[0]_inv_i_1 
       (.I0(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_2_n_0 ),
        .I1(m_valid_i),
        .I2(m_ready_d0[1]),
        .I3(\gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0 ),
        .I4(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0 ),
        .I5(\gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1 ),
        .O(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h0FEF0000)) 
    \gen_no_arbiter.m_grant_hot_i[0]_inv_i_2 
       (.I0(s_axi_awvalid),
        .I1(s_axi_arvalid),
        .I2(p_0_in1_in),
        .I3(m_valid_i),
        .I4(aresetn_d),
        .O(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT4 #(
    .INIT(16'h0F08)) 
    \gen_no_arbiter.m_grant_hot_i[0]_inv_i_4 
       (.I0(f_mux_return),
        .I1(m_valid_i),
        .I2(aa_grant_rnw),
        .I3(\gen_axilite.s_axi_bvalid_i_reg_0 [2]),
        .O(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0 ));
  FDRE #(
    .INIT(1'b1)) 
    \gen_no_arbiter.m_grant_hot_i_reg[0]_inv 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_1_n_0 ),
        .Q(p_0_in1_in),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h11111DDD1DDD1DDD)) 
    \gen_no_arbiter.m_valid_i_i_1 
       (.I0(p_0_in1_in),
        .I1(m_valid_i),
        .I2(m_ready_d0[1]),
        .I3(\gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0 ),
        .I4(\gen_no_arbiter.m_grant_hot_i[0]_inv_i_4_n_0 ),
        .I5(\gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1 ),
        .O(\gen_no_arbiter.m_valid_i_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \gen_no_arbiter.m_valid_i_reg 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_no_arbiter.m_valid_i_i_1_n_0 ),
        .Q(m_valid_i),
        .R(SR));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'h10)) 
    \gen_no_arbiter.s_ready_i[0]_i_1 
       (.I0(m_valid_i),
        .I1(p_0_in1_in),
        .I2(aresetn_d),
        .O(\gen_no_arbiter.s_ready_i[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \gen_no_arbiter.s_ready_i_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_no_arbiter.s_ready_i[0]_i_1_n_0 ),
        .Q(s_ready_i),
        .R(1'b0));
  LUT5 #(
    .INIT(32'h08000000)) 
    \m_atarget_enc[0]_i_1 
       (.I0(aresetn_d),
        .I1(\gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ),
        .I2(Q[17]),
        .I3(Q[18]),
        .I4(Q[16]),
        .O(aresetn_d_reg_0));
  LUT5 #(
    .INIT(32'h0000F7FF)) 
    \m_atarget_enc[1]_i_1 
       (.I0(Q[16]),
        .I1(Q[18]),
        .I2(Q[17]),
        .I3(\gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ),
        .I4(\gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I ),
        .O(any_error));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT2 #(
    .INIT(4'h4)) 
    \m_atarget_hot[0]_i_1 
       (.I0(p_0_in1_in),
        .I1(\gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I ),
        .O(D[0]));
  LUT5 #(
    .INIT(32'h04000000)) 
    \m_atarget_hot[1]_i_1 
       (.I0(p_0_in1_in),
        .I1(\gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ),
        .I2(Q[17]),
        .I3(Q[18]),
        .I4(Q[16]),
        .O(D[1]));
  LUT6 #(
    .INIT(64'h000000000000F7FF)) 
    \m_atarget_hot[2]_i_1 
       (.I0(Q[16]),
        .I1(Q[18]),
        .I2(Q[17]),
        .I3(\gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ),
        .I4(p_0_in1_in),
        .I5(\gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I ),
        .O(D[2]));
  LUT6 #(
    .INIT(64'h0001000000000000)) 
    \m_atarget_hot[2]_i_2 
       (.I0(Q[21]),
        .I1(Q[22]),
        .I2(Q[19]),
        .I3(Q[20]),
        .I4(Q[24]),
        .I5(Q[23]),
        .O(\gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ));
  LUT5 #(
    .INIT(32'h1010F000)) 
    \m_atarget_hot[2]_i_3 
       (.I0(Q[16]),
        .I1(Q[17]),
        .I2(\gen_addr_decoder.addr_decoder_inst/gen_target[1].gen_region[0].gen_comparator_static.gen_addr_range.addr_decode_comparator/sel_3 ),
        .I3(\m_atarget_hot[2]_i_4_n_0 ),
        .I4(Q[18]),
        .O(\gen_addr_decoder.addr_decoder_inst/TARGET_HOT_I ));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFF0001)) 
    \m_atarget_hot[2]_i_4 
       (.I0(Q[14]),
        .I1(Q[15]),
        .I2(Q[12]),
        .I3(Q[13]),
        .I4(Q[16]),
        .I5(Q[17]),
        .O(\m_atarget_hot[2]_i_4_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT4 #(
    .INIT(16'h2000)) 
    \m_axi_arvalid[0]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_1 [0]),
        .I1(\m_axi_arvalid[0] [1]),
        .I2(m_valid_i),
        .I3(aa_grant_rnw),
        .O(m_axi_arvalid[0]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT4 #(
    .INIT(16'h2000)) 
    \m_axi_arvalid[1]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_1 [1]),
        .I1(\m_axi_arvalid[0] [1]),
        .I2(m_valid_i),
        .I3(aa_grant_rnw),
        .O(m_axi_arvalid[1]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT4 #(
    .INIT(16'h0400)) 
    \m_axi_awvalid[0]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_0 [2]),
        .I1(\gen_axilite.s_axi_bvalid_i_reg_1 [0]),
        .I2(aa_grant_rnw),
        .I3(m_valid_i),
        .O(m_axi_awvalid[0]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT4 #(
    .INIT(16'h0400)) 
    \m_axi_awvalid[1]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_0 [2]),
        .I1(\gen_axilite.s_axi_bvalid_i_reg_1 [1]),
        .I2(aa_grant_rnw),
        .I3(m_valid_i),
        .O(m_axi_awvalid[1]));
  LUT5 #(
    .INIT(32'h00004000)) 
    \m_axi_bready[0]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_0 [0]),
        .I1(\gen_axilite.s_axi_bvalid_i_reg_1 [0]),
        .I2(s_axi_bready),
        .I3(m_valid_i),
        .I4(aa_grant_rnw),
        .O(m_axi_bready[0]));
  LUT5 #(
    .INIT(32'h00004000)) 
    \m_axi_bready[1]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_0 [0]),
        .I1(\gen_axilite.s_axi_bvalid_i_reg_1 [1]),
        .I2(s_axi_bready),
        .I3(m_valid_i),
        .I4(aa_grant_rnw),
        .O(m_axi_bready[1]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT5 #(
    .INIT(32'h10000000)) 
    \m_axi_wvalid[0]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_0 [1]),
        .I1(aa_grant_rnw),
        .I2(m_valid_i),
        .I3(s_axi_wvalid),
        .I4(\gen_axilite.s_axi_bvalid_i_reg_1 [0]),
        .O(m_axi_wvalid[0]));
  LUT5 #(
    .INIT(32'h10000000)) 
    \m_axi_wvalid[1]_INST_0 
       (.I0(\gen_axilite.s_axi_bvalid_i_reg_0 [1]),
        .I1(aa_grant_rnw),
        .I2(m_valid_i),
        .I3(s_axi_wvalid),
        .I4(\gen_axilite.s_axi_bvalid_i_reg_1 [1]),
        .O(m_axi_wvalid[1]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h0800FFFF)) 
    \m_payload_i[34]_i_1 
       (.I0(aa_grant_rnw),
        .I1(m_valid_i),
        .I2(\m_axi_arvalid[0] [0]),
        .I3(s_axi_rready),
        .I4(sr_rvalid),
        .O(E));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT5 #(
    .INIT(32'hFFFF2000)) 
    \m_ready_d[0]_i_1 
       (.I0(f_mux_return__3),
        .I1(aa_grant_rnw),
        .I2(m_valid_i),
        .I3(s_axi_bready),
        .I4(\gen_axilite.s_axi_bvalid_i_reg_0 [0]),
        .O(\m_ready_d_reg[2] [0]));
  LUT6 #(
    .INIT(64'hFFFFFFFF80000000)) 
    \m_ready_d[0]_i_1__0 
       (.I0(s_axi_rready),
        .I1(m_valid_i),
        .I2(aa_grant_rnw),
        .I3(\m_ready_d_reg[0] ),
        .I4(sr_rvalid),
        .I5(\m_axi_arvalid[0] [0]),
        .O(m_ready_d0[0]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT5 #(
    .INIT(32'hFFFF2000)) 
    \m_ready_d[1]_i_1 
       (.I0(f_mux_return__1),
        .I1(aa_grant_rnw),
        .I2(m_valid_i),
        .I3(s_axi_wvalid),
        .I4(\gen_axilite.s_axi_bvalid_i_reg_0 [1]),
        .O(\m_ready_d_reg[2] [1]));
  LUT3 #(
    .INIT(8'h8F)) 
    \m_ready_d[1]_i_1__0 
       (.I0(m_ready_d0[0]),
        .I1(m_ready_d0[1]),
        .I2(aresetn_d),
        .O(aresetn_d_reg));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT4 #(
    .INIT(16'hFF80)) 
    \m_ready_d[1]_i_2 
       (.I0(aa_grant_rnw),
        .I1(m_valid_i),
        .I2(f_mux_return__0),
        .I3(\m_axi_arvalid[0] [1]),
        .O(m_ready_d0[1]));
  LUT2 #(
    .INIT(4'hE)) 
    \m_ready_d[2]_i_2 
       (.I0(\m_atarget_enc_reg[1] ),
        .I1(\gen_axilite.s_axi_bvalid_i_reg_0 [2]),
        .O(\m_ready_d_reg[2] [2]));
  LUT6 #(
    .INIT(64'h2A0A220228082000)) 
    \m_ready_d[2]_i_3 
       (.I0(\m_ready_d[2]_i_5_n_0 ),
        .I1(m_atarget_enc[1]),
        .I2(m_atarget_enc[0]),
        .I3(m_axi_awready[1]),
        .I4(mi_wready),
        .I5(m_axi_awready[0]),
        .O(\m_atarget_enc_reg[1] ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \m_ready_d[2]_i_5 
       (.I0(m_valid_i),
        .I1(aa_grant_rnw),
        .O(\m_ready_d[2]_i_5_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT3 #(
    .INIT(8'h40)) 
    \m_ready_d[2]_i_6 
       (.I0(aa_grant_rnw),
        .I1(m_valid_i),
        .I2(s_axi_bready),
        .O(\gen_no_arbiter.grant_rnw_reg_0 ));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT3 #(
    .INIT(8'h40)) 
    \m_ready_d[2]_i_7 
       (.I0(aa_grant_rnw),
        .I1(m_valid_i),
        .I2(s_axi_wvalid),
        .O(\gen_no_arbiter.grant_rnw_reg_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT3 #(
    .INIT(8'h08)) 
    m_valid_i_i_3
       (.I0(aa_grant_rnw),
        .I1(m_valid_i),
        .I2(\m_axi_arvalid[0] [0]),
        .O(r_transfer_en));
  LUT2 #(
    .INIT(4'hB)) 
    \s_arvalid_reg[0]_i_1 
       (.I0(s_ready_i),
        .I1(aresetn_d),
        .O(s_arvalid_reg));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \s_arvalid_reg[0]_i_2 
       (.I0(s_axi_arvalid),
        .I1(s_awvalid_reg),
        .O(p_0_out));
  FDRE #(
    .INIT(1'b0)) 
    \s_arvalid_reg_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(p_0_out),
        .Q(\s_arvalid_reg_reg_n_0_[0] ),
        .R(s_arvalid_reg));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT4 #(
    .INIT(16'h4044)) 
    \s_awvalid_reg[0]_i_1 
       (.I0(\s_arvalid_reg_reg_n_0_[0] ),
        .I1(s_axi_awvalid),
        .I2(s_awvalid_reg),
        .I3(s_axi_arvalid),
        .O(s_awvalid_reg0));
  FDRE #(
    .INIT(1'b0)) 
    \s_awvalid_reg_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(s_awvalid_reg0),
        .Q(s_awvalid_reg),
        .R(s_arvalid_reg));
  LUT2 #(
    .INIT(4'h8)) 
    \s_axi_arready[0]_INST_0 
       (.I0(s_ready_i),
        .I1(aa_grant_rnw),
        .O(s_axi_arready));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \s_axi_awready[0]_INST_0 
       (.I0(s_ready_i),
        .I1(aa_grant_rnw),
        .O(s_axi_awready));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT5 #(
    .INIT(32'h00020000)) 
    \s_axi_bvalid[0]_INST_0 
       (.I0(m_valid_i),
        .I1(aa_grant_rnw),
        .I2(p_0_in1_in),
        .I3(\gen_axilite.s_axi_bvalid_i_reg_0 [0]),
        .I4(f_mux_return__3),
        .O(s_axi_bvalid));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT2 #(
    .INIT(4'h4)) 
    \s_axi_rvalid[0]_INST_0 
       (.I0(p_0_in1_in),
        .I1(sr_rvalid),
        .O(s_axi_rvalid));
  LUT5 #(
    .INIT(32'h00020000)) 
    \s_axi_wready[0]_INST_0 
       (.I0(m_valid_i),
        .I1(aa_grant_rnw),
        .I2(p_0_in1_in),
        .I3(\gen_axilite.s_axi_bvalid_i_reg_0 [1]),
        .I4(f_mux_return__1),
        .O(s_axi_wready));
endmodule

(* C_AXI_ADDR_WIDTH = "25" *) (* C_AXI_ARUSER_WIDTH = "1" *) (* C_AXI_AWUSER_WIDTH = "1" *) 
(* C_AXI_BUSER_WIDTH = "1" *) (* C_AXI_DATA_WIDTH = "32" *) (* C_AXI_ID_WIDTH = "1" *) 
(* C_AXI_PROTOCOL = "2" *) (* C_AXI_RUSER_WIDTH = "1" *) (* C_AXI_SUPPORTS_USER_SIGNALS = "0" *) 
(* C_AXI_WUSER_WIDTH = "1" *) (* C_CONNECTIVITY_MODE = "0" *) (* C_DEBUG = "1" *) 
(* C_FAMILY = "virtexuplus" *) (* C_M_AXI_ADDR_WIDTH = "320'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000010000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000001100" *) (* C_M_AXI_BASE_ADDR = "640'b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000000000000000000000000000000000000000110000101000000000000000000000000000000000000000000000000000000011000010000000000000000000000000000000000000000000000000000000001100000110000000000000000000000000000000000000000000000000000000110000010000000000000000000000000000000000000000000000000000000011000000100000000000000000000000000000000000000000000000000000001100000000000000000000000" *) 
(* C_M_AXI_READ_CONNECTIVITY = "64'b1111111111111111111111111111111111111111111111111111111111111111" *) (* C_M_AXI_READ_ISSUING = "64'b0000000000000000000000000000000100000000000000000000000000000001" *) (* C_M_AXI_SECURE = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) 
(* C_M_AXI_WRITE_CONNECTIVITY = "64'b1111111111111111111111111111111111111111111111111111111111111111" *) (* C_M_AXI_WRITE_ISSUING = "64'b0000000000000000000000000000000100000000000000000000000000000001" *) (* C_NUM_ADDR_RANGES = "5" *) 
(* C_NUM_MASTER_SLOTS = "2" *) (* C_NUM_SLAVE_SLOTS = "1" *) (* C_R_REGISTER = "1" *) 
(* C_S_AXI_ARB_PRIORITY = "0" *) (* C_S_AXI_BASE_ID = "0" *) (* C_S_AXI_READ_ACCEPTANCE = "1" *) 
(* C_S_AXI_SINGLE_THREAD = "1" *) (* C_S_AXI_THREAD_ID_WIDTH = "0" *) (* C_S_AXI_WRITE_ACCEPTANCE = "1" *) 
(* DowngradeIPIdentifiedWarnings = "yes" *) (* P_ADDR_DECODE = "1" *) (* P_AXI3 = "1" *) 
(* P_AXI4 = "0" *) (* P_AXILITE = "2" *) (* P_AXILITE_SIZE = "3'b010" *) 
(* P_FAMILY = "rtl" *) (* P_INCR = "2'b01" *) (* P_LEN = "8" *) 
(* P_LOCK = "1" *) (* P_M_AXI_ERR_MODE = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) (* P_M_AXI_SUPPORTS_READ = "2'b11" *) 
(* P_M_AXI_SUPPORTS_WRITE = "2'b11" *) (* P_ONES = "65'b11111111111111111111111111111111111111111111111111111111111111111" *) (* P_RANGE_CHECK = "1" *) 
(* P_S_AXI_BASE_ID = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) (* P_S_AXI_HIGH_ID = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) (* P_S_AXI_SUPPORTS_READ = "1'b1" *) 
(* P_S_AXI_SUPPORTS_WRITE = "1'b1" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar
   (aclk,
    aresetn,
    s_axi_awid,
    s_axi_awaddr,
    s_axi_awlen,
    s_axi_awsize,
    s_axi_awburst,
    s_axi_awlock,
    s_axi_awcache,
    s_axi_awprot,
    s_axi_awqos,
    s_axi_awuser,
    s_axi_awvalid,
    s_axi_awready,
    s_axi_wid,
    s_axi_wdata,
    s_axi_wstrb,
    s_axi_wlast,
    s_axi_wuser,
    s_axi_wvalid,
    s_axi_wready,
    s_axi_bid,
    s_axi_bresp,
    s_axi_buser,
    s_axi_bvalid,
    s_axi_bready,
    s_axi_arid,
    s_axi_araddr,
    s_axi_arlen,
    s_axi_arsize,
    s_axi_arburst,
    s_axi_arlock,
    s_axi_arcache,
    s_axi_arprot,
    s_axi_arqos,
    s_axi_aruser,
    s_axi_arvalid,
    s_axi_arready,
    s_axi_rid,
    s_axi_rdata,
    s_axi_rresp,
    s_axi_rlast,
    s_axi_ruser,
    s_axi_rvalid,
    s_axi_rready,
    m_axi_awid,
    m_axi_awaddr,
    m_axi_awlen,
    m_axi_awsize,
    m_axi_awburst,
    m_axi_awlock,
    m_axi_awcache,
    m_axi_awprot,
    m_axi_awregion,
    m_axi_awqos,
    m_axi_awuser,
    m_axi_awvalid,
    m_axi_awready,
    m_axi_wid,
    m_axi_wdata,
    m_axi_wstrb,
    m_axi_wlast,
    m_axi_wuser,
    m_axi_wvalid,
    m_axi_wready,
    m_axi_bid,
    m_axi_bresp,
    m_axi_buser,
    m_axi_bvalid,
    m_axi_bready,
    m_axi_arid,
    m_axi_araddr,
    m_axi_arlen,
    m_axi_arsize,
    m_axi_arburst,
    m_axi_arlock,
    m_axi_arcache,
    m_axi_arprot,
    m_axi_arregion,
    m_axi_arqos,
    m_axi_aruser,
    m_axi_arvalid,
    m_axi_arready,
    m_axi_rid,
    m_axi_rdata,
    m_axi_rresp,
    m_axi_rlast,
    m_axi_ruser,
    m_axi_rvalid,
    m_axi_rready);
  input aclk;
  input aresetn;
  input [0:0]s_axi_awid;
  input [24:0]s_axi_awaddr;
  input [7:0]s_axi_awlen;
  input [2:0]s_axi_awsize;
  input [1:0]s_axi_awburst;
  input [0:0]s_axi_awlock;
  input [3:0]s_axi_awcache;
  input [2:0]s_axi_awprot;
  input [3:0]s_axi_awqos;
  input [0:0]s_axi_awuser;
  input [0:0]s_axi_awvalid;
  output [0:0]s_axi_awready;
  input [0:0]s_axi_wid;
  input [31:0]s_axi_wdata;
  input [3:0]s_axi_wstrb;
  input [0:0]s_axi_wlast;
  input [0:0]s_axi_wuser;
  input [0:0]s_axi_wvalid;
  output [0:0]s_axi_wready;
  output [0:0]s_axi_bid;
  output [1:0]s_axi_bresp;
  output [0:0]s_axi_buser;
  output [0:0]s_axi_bvalid;
  input [0:0]s_axi_bready;
  input [0:0]s_axi_arid;
  input [24:0]s_axi_araddr;
  input [7:0]s_axi_arlen;
  input [2:0]s_axi_arsize;
  input [1:0]s_axi_arburst;
  input [0:0]s_axi_arlock;
  input [3:0]s_axi_arcache;
  input [2:0]s_axi_arprot;
  input [3:0]s_axi_arqos;
  input [0:0]s_axi_aruser;
  input [0:0]s_axi_arvalid;
  output [0:0]s_axi_arready;
  output [0:0]s_axi_rid;
  output [31:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output [0:0]s_axi_rlast;
  output [0:0]s_axi_ruser;
  output [0:0]s_axi_rvalid;
  input [0:0]s_axi_rready;
  output [1:0]m_axi_awid;
  output [49:0]m_axi_awaddr;
  output [15:0]m_axi_awlen;
  output [5:0]m_axi_awsize;
  output [3:0]m_axi_awburst;
  output [1:0]m_axi_awlock;
  output [7:0]m_axi_awcache;
  output [5:0]m_axi_awprot;
  output [7:0]m_axi_awregion;
  output [7:0]m_axi_awqos;
  output [1:0]m_axi_awuser;
  output [1:0]m_axi_awvalid;
  input [1:0]m_axi_awready;
  output [1:0]m_axi_wid;
  output [63:0]m_axi_wdata;
  output [7:0]m_axi_wstrb;
  output [1:0]m_axi_wlast;
  output [1:0]m_axi_wuser;
  output [1:0]m_axi_wvalid;
  input [1:0]m_axi_wready;
  input [1:0]m_axi_bid;
  input [3:0]m_axi_bresp;
  input [1:0]m_axi_buser;
  input [1:0]m_axi_bvalid;
  output [1:0]m_axi_bready;
  output [1:0]m_axi_arid;
  output [49:0]m_axi_araddr;
  output [15:0]m_axi_arlen;
  output [5:0]m_axi_arsize;
  output [3:0]m_axi_arburst;
  output [1:0]m_axi_arlock;
  output [7:0]m_axi_arcache;
  output [5:0]m_axi_arprot;
  output [7:0]m_axi_arregion;
  output [7:0]m_axi_arqos;
  output [1:0]m_axi_aruser;
  output [1:0]m_axi_arvalid;
  input [1:0]m_axi_arready;
  input [1:0]m_axi_rid;
  input [63:0]m_axi_rdata;
  input [3:0]m_axi_rresp;
  input [1:0]m_axi_rlast;
  input [1:0]m_axi_ruser;
  input [1:0]m_axi_rvalid;
  output [1:0]m_axi_rready;

  wire \<const0> ;
  wire aclk;
  wire aresetn;
  wire [11:0]\^m_axi_araddr ;
  wire [2:0]\^m_axi_arprot ;
  wire [1:0]m_axi_arready;
  wire [1:0]m_axi_arvalid;
  wire [49:37]\^m_axi_awaddr ;
  wire [1:0]m_axi_awready;
  wire [1:0]m_axi_awvalid;
  wire [1:0]m_axi_bready;
  wire [3:0]m_axi_bresp;
  wire [1:0]m_axi_bvalid;
  wire [63:0]m_axi_rdata;
  wire [1:0]m_axi_rready;
  wire [3:0]m_axi_rresp;
  wire [1:0]m_axi_rvalid;
  wire [1:0]m_axi_wready;
  wire [1:0]m_axi_wvalid;
  wire [24:0]s_axi_araddr;
  wire [2:0]s_axi_arprot;
  wire [0:0]s_axi_arready;
  wire [0:0]s_axi_arvalid;
  wire [24:0]s_axi_awaddr;
  wire [2:0]s_axi_awprot;
  wire [0:0]s_axi_awready;
  wire [0:0]s_axi_awvalid;
  wire [0:0]s_axi_bready;
  wire [1:0]s_axi_bresp;
  wire [0:0]s_axi_bvalid;
  wire [31:0]s_axi_rdata;
  wire [0:0]s_axi_rready;
  wire [1:0]s_axi_rresp;
  wire [0:0]s_axi_rvalid;
  wire [31:0]s_axi_wdata;
  wire [0:0]s_axi_wready;
  wire [3:0]s_axi_wstrb;
  wire [0:0]s_axi_wvalid;

  assign m_axi_araddr[49:37] = \^m_axi_awaddr [49:37];
  assign m_axi_araddr[36:25] = \^m_axi_araddr [11:0];
  assign m_axi_araddr[24:12] = \^m_axi_awaddr [49:37];
  assign m_axi_araddr[11:0] = \^m_axi_araddr [11:0];
  assign m_axi_arburst[3] = \<const0> ;
  assign m_axi_arburst[2] = \<const0> ;
  assign m_axi_arburst[1] = \<const0> ;
  assign m_axi_arburst[0] = \<const0> ;
  assign m_axi_arcache[7] = \<const0> ;
  assign m_axi_arcache[6] = \<const0> ;
  assign m_axi_arcache[5] = \<const0> ;
  assign m_axi_arcache[4] = \<const0> ;
  assign m_axi_arcache[3] = \<const0> ;
  assign m_axi_arcache[2] = \<const0> ;
  assign m_axi_arcache[1] = \<const0> ;
  assign m_axi_arcache[0] = \<const0> ;
  assign m_axi_arid[1] = \<const0> ;
  assign m_axi_arid[0] = \<const0> ;
  assign m_axi_arlen[15] = \<const0> ;
  assign m_axi_arlen[14] = \<const0> ;
  assign m_axi_arlen[13] = \<const0> ;
  assign m_axi_arlen[12] = \<const0> ;
  assign m_axi_arlen[11] = \<const0> ;
  assign m_axi_arlen[10] = \<const0> ;
  assign m_axi_arlen[9] = \<const0> ;
  assign m_axi_arlen[8] = \<const0> ;
  assign m_axi_arlen[7] = \<const0> ;
  assign m_axi_arlen[6] = \<const0> ;
  assign m_axi_arlen[5] = \<const0> ;
  assign m_axi_arlen[4] = \<const0> ;
  assign m_axi_arlen[3] = \<const0> ;
  assign m_axi_arlen[2] = \<const0> ;
  assign m_axi_arlen[1] = \<const0> ;
  assign m_axi_arlen[0] = \<const0> ;
  assign m_axi_arlock[1] = \<const0> ;
  assign m_axi_arlock[0] = \<const0> ;
  assign m_axi_arprot[5:3] = \^m_axi_arprot [2:0];
  assign m_axi_arprot[2:0] = \^m_axi_arprot [2:0];
  assign m_axi_arqos[7] = \<const0> ;
  assign m_axi_arqos[6] = \<const0> ;
  assign m_axi_arqos[5] = \<const0> ;
  assign m_axi_arqos[4] = \<const0> ;
  assign m_axi_arqos[3] = \<const0> ;
  assign m_axi_arqos[2] = \<const0> ;
  assign m_axi_arqos[1] = \<const0> ;
  assign m_axi_arqos[0] = \<const0> ;
  assign m_axi_arregion[7] = \<const0> ;
  assign m_axi_arregion[6] = \<const0> ;
  assign m_axi_arregion[5] = \<const0> ;
  assign m_axi_arregion[4] = \<const0> ;
  assign m_axi_arregion[3] = \<const0> ;
  assign m_axi_arregion[2] = \<const0> ;
  assign m_axi_arregion[1] = \<const0> ;
  assign m_axi_arregion[0] = \<const0> ;
  assign m_axi_arsize[5] = \<const0> ;
  assign m_axi_arsize[4] = \<const0> ;
  assign m_axi_arsize[3] = \<const0> ;
  assign m_axi_arsize[2] = \<const0> ;
  assign m_axi_arsize[1] = \<const0> ;
  assign m_axi_arsize[0] = \<const0> ;
  assign m_axi_aruser[1] = \<const0> ;
  assign m_axi_aruser[0] = \<const0> ;
  assign m_axi_awaddr[49:37] = \^m_axi_awaddr [49:37];
  assign m_axi_awaddr[36:25] = \^m_axi_araddr [11:0];
  assign m_axi_awaddr[24:12] = \^m_axi_awaddr [49:37];
  assign m_axi_awaddr[11:0] = \^m_axi_araddr [11:0];
  assign m_axi_awburst[3] = \<const0> ;
  assign m_axi_awburst[2] = \<const0> ;
  assign m_axi_awburst[1] = \<const0> ;
  assign m_axi_awburst[0] = \<const0> ;
  assign m_axi_awcache[7] = \<const0> ;
  assign m_axi_awcache[6] = \<const0> ;
  assign m_axi_awcache[5] = \<const0> ;
  assign m_axi_awcache[4] = \<const0> ;
  assign m_axi_awcache[3] = \<const0> ;
  assign m_axi_awcache[2] = \<const0> ;
  assign m_axi_awcache[1] = \<const0> ;
  assign m_axi_awcache[0] = \<const0> ;
  assign m_axi_awid[1] = \<const0> ;
  assign m_axi_awid[0] = \<const0> ;
  assign m_axi_awlen[15] = \<const0> ;
  assign m_axi_awlen[14] = \<const0> ;
  assign m_axi_awlen[13] = \<const0> ;
  assign m_axi_awlen[12] = \<const0> ;
  assign m_axi_awlen[11] = \<const0> ;
  assign m_axi_awlen[10] = \<const0> ;
  assign m_axi_awlen[9] = \<const0> ;
  assign m_axi_awlen[8] = \<const0> ;
  assign m_axi_awlen[7] = \<const0> ;
  assign m_axi_awlen[6] = \<const0> ;
  assign m_axi_awlen[5] = \<const0> ;
  assign m_axi_awlen[4] = \<const0> ;
  assign m_axi_awlen[3] = \<const0> ;
  assign m_axi_awlen[2] = \<const0> ;
  assign m_axi_awlen[1] = \<const0> ;
  assign m_axi_awlen[0] = \<const0> ;
  assign m_axi_awlock[1] = \<const0> ;
  assign m_axi_awlock[0] = \<const0> ;
  assign m_axi_awprot[5:3] = \^m_axi_arprot [2:0];
  assign m_axi_awprot[2:0] = \^m_axi_arprot [2:0];
  assign m_axi_awqos[7] = \<const0> ;
  assign m_axi_awqos[6] = \<const0> ;
  assign m_axi_awqos[5] = \<const0> ;
  assign m_axi_awqos[4] = \<const0> ;
  assign m_axi_awqos[3] = \<const0> ;
  assign m_axi_awqos[2] = \<const0> ;
  assign m_axi_awqos[1] = \<const0> ;
  assign m_axi_awqos[0] = \<const0> ;
  assign m_axi_awregion[7] = \<const0> ;
  assign m_axi_awregion[6] = \<const0> ;
  assign m_axi_awregion[5] = \<const0> ;
  assign m_axi_awregion[4] = \<const0> ;
  assign m_axi_awregion[3] = \<const0> ;
  assign m_axi_awregion[2] = \<const0> ;
  assign m_axi_awregion[1] = \<const0> ;
  assign m_axi_awregion[0] = \<const0> ;
  assign m_axi_awsize[5] = \<const0> ;
  assign m_axi_awsize[4] = \<const0> ;
  assign m_axi_awsize[3] = \<const0> ;
  assign m_axi_awsize[2] = \<const0> ;
  assign m_axi_awsize[1] = \<const0> ;
  assign m_axi_awsize[0] = \<const0> ;
  assign m_axi_awuser[1] = \<const0> ;
  assign m_axi_awuser[0] = \<const0> ;
  assign m_axi_wdata[63:32] = s_axi_wdata;
  assign m_axi_wdata[31:0] = s_axi_wdata;
  assign m_axi_wid[1] = \<const0> ;
  assign m_axi_wid[0] = \<const0> ;
  assign m_axi_wlast[1] = \<const0> ;
  assign m_axi_wlast[0] = \<const0> ;
  assign m_axi_wstrb[7:4] = s_axi_wstrb;
  assign m_axi_wstrb[3:0] = s_axi_wstrb;
  assign m_axi_wuser[1] = \<const0> ;
  assign m_axi_wuser[0] = \<const0> ;
  assign s_axi_bid[0] = \<const0> ;
  assign s_axi_buser[0] = \<const0> ;
  assign s_axi_rid[0] = \<const0> ;
  assign s_axi_rlast[0] = \<const0> ;
  assign s_axi_ruser[0] = \<const0> ;
  GND GND
       (.G(\<const0> ));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_crossbar_sasd \gen_sasd.crossbar_sasd_0 
       (.Q({\^m_axi_arprot ,\^m_axi_awaddr ,\^m_axi_araddr }),
        .aclk(aclk),
        .aresetn(aresetn),
        .m_axi_arready(m_axi_arready),
        .m_axi_arvalid(m_axi_arvalid),
        .m_axi_awready(m_axi_awready),
        .m_axi_awvalid(m_axi_awvalid),
        .m_axi_bready(m_axi_bready),
        .m_axi_bresp(m_axi_bresp),
        .m_axi_bvalid(m_axi_bvalid),
        .m_axi_rdata(m_axi_rdata),
        .m_axi_rready(m_axi_rready),
        .m_axi_rresp(m_axi_rresp),
        .m_axi_rvalid(m_axi_rvalid),
        .m_axi_wready(m_axi_wready),
        .m_axi_wvalid(m_axi_wvalid),
        .\m_payload_i_reg[34] ({s_axi_rdata,s_axi_rresp}),
        .s_axi_araddr(s_axi_araddr),
        .s_axi_arprot(s_axi_arprot),
        .s_axi_arready(s_axi_arready),
        .s_axi_arvalid(s_axi_arvalid),
        .s_axi_awaddr(s_axi_awaddr),
        .s_axi_awprot(s_axi_awprot),
        .s_axi_awready(s_axi_awready),
        .s_axi_awvalid(s_axi_awvalid),
        .s_axi_bready(s_axi_bready),
        .s_axi_bresp(s_axi_bresp),
        .s_axi_bvalid(s_axi_bvalid),
        .s_axi_rready(s_axi_rready),
        .s_axi_rvalid(s_axi_rvalid),
        .s_axi_wready(s_axi_wready),
        .s_axi_wvalid(s_axi_wvalid));
endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_crossbar_sasd
   (Q,
    \m_payload_i_reg[34] ,
    m_axi_awvalid,
    s_axi_bvalid,
    s_axi_wready,
    m_axi_bready,
    m_axi_wvalid,
    m_axi_arvalid,
    s_axi_bresp,
    s_axi_awready,
    s_axi_arready,
    s_axi_rvalid,
    m_axi_rready,
    aresetn,
    aclk,
    s_axi_awvalid,
    s_axi_arvalid,
    m_axi_awready,
    s_axi_bready,
    s_axi_wvalid,
    m_axi_rvalid,
    s_axi_rready,
    m_axi_wready,
    m_axi_bvalid,
    m_axi_arready,
    m_axi_bresp,
    m_axi_rresp,
    m_axi_rdata,
    s_axi_arprot,
    s_axi_awprot,
    s_axi_araddr,
    s_axi_awaddr);
  output [27:0]Q;
  output [33:0]\m_payload_i_reg[34] ;
  output [1:0]m_axi_awvalid;
  output [0:0]s_axi_bvalid;
  output [0:0]s_axi_wready;
  output [1:0]m_axi_bready;
  output [1:0]m_axi_wvalid;
  output [1:0]m_axi_arvalid;
  output [1:0]s_axi_bresp;
  output [0:0]s_axi_awready;
  output [0:0]s_axi_arready;
  output [0:0]s_axi_rvalid;
  output [1:0]m_axi_rready;
  input aresetn;
  input aclk;
  input [0:0]s_axi_awvalid;
  input [0:0]s_axi_arvalid;
  input [1:0]m_axi_awready;
  input [0:0]s_axi_bready;
  input [0:0]s_axi_wvalid;
  input [1:0]m_axi_rvalid;
  input [0:0]s_axi_rready;
  input [1:0]m_axi_wready;
  input [1:0]m_axi_bvalid;
  input [1:0]m_axi_arready;
  input [3:0]m_axi_bresp;
  input [3:0]m_axi_rresp;
  input [63:0]m_axi_rdata;
  input [2:0]s_axi_arprot;
  input [2:0]s_axi_awprot;
  input [24:0]s_axi_araddr;
  input [24:0]s_axi_awaddr;

  wire [27:0]Q;
  wire aa_grant_rnw;
  wire aa_rready;
  wire aa_rvalid;
  wire aclk;
  wire addr_arbiter_inst_n_44;
  wire addr_arbiter_inst_n_47;
  wire addr_arbiter_inst_n_50;
  wire addr_arbiter_inst_n_51;
  wire addr_arbiter_inst_n_53;
  wire addr_arbiter_inst_n_61;
  wire addr_arbiter_inst_n_62;
  wire any_error;
  wire aresetn;
  wire aresetn_d;
  wire f_mux_return;
  wire f_mux_return__0;
  wire f_mux_return__1;
  wire f_mux_return__3;
  wire [1:0]m_atarget_enc;
  wire [2:0]m_atarget_hot;
  wire [2:0]m_atarget_hot0;
  wire [1:0]m_axi_arready;
  wire [1:0]m_axi_arvalid;
  wire [1:0]m_axi_awready;
  wire [1:0]m_axi_awvalid;
  wire [1:0]m_axi_bready;
  wire [3:0]m_axi_bresp;
  wire [1:0]m_axi_bvalid;
  wire [63:0]m_axi_rdata;
  wire [1:0]m_axi_rready;
  wire [3:0]m_axi_rresp;
  wire [1:0]m_axi_rvalid;
  wire [1:0]m_axi_wready;
  wire [1:0]m_axi_wvalid;
  wire [33:0]\m_payload_i_reg[34] ;
  wire [1:0]m_ready_d;
  wire [2:0]m_ready_d0;
  wire [1:0]m_ready_d0_0;
  wire [2:0]m_ready_d_1;
  wire m_valid_i;
  wire mi_arvalid_en;
  wire [2:2]mi_bvalid;
  wire [2:2]mi_wready;
  wire p_1_in;
  wire r_transfer_en;
  wire reg_slice_r_n_2;
  wire reg_slice_r_n_37;
  wire reset;
  wire [24:0]s_axi_araddr;
  wire [2:0]s_axi_arprot;
  wire [0:0]s_axi_arready;
  wire [0:0]s_axi_arvalid;
  wire [24:0]s_axi_awaddr;
  wire [2:0]s_axi_awprot;
  wire [0:0]s_axi_awready;
  wire [0:0]s_axi_awvalid;
  wire [0:0]s_axi_bready;
  wire [1:0]s_axi_bresp;
  wire [0:0]s_axi_bvalid;
  wire [0:0]s_axi_rready;
  wire [0:0]s_axi_rvalid;
  wire [0:0]s_axi_wready;
  wire [0:0]s_axi_wvalid;
  wire splitter_aw_n_3;
  wire sr_rvalid;

  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_addr_arbiter_sasd addr_arbiter_inst
       (.D(m_atarget_hot0),
        .E(p_1_in),
        .Q(Q),
        .SR(reset),
        .aa_grant_rnw(aa_grant_rnw),
        .aclk(aclk),
        .any_error(any_error),
        .aresetn_d(aresetn_d),
        .aresetn_d_reg(addr_arbiter_inst_n_53),
        .aresetn_d_reg_0(addr_arbiter_inst_n_61),
        .f_mux_return(f_mux_return),
        .f_mux_return__0(f_mux_return__0),
        .f_mux_return__1(f_mux_return__1),
        .f_mux_return__3(f_mux_return__3),
        .\gen_axilite.s_axi_bvalid_i_reg (addr_arbiter_inst_n_62),
        .\gen_axilite.s_axi_bvalid_i_reg_0 (m_ready_d_1),
        .\gen_axilite.s_axi_bvalid_i_reg_1 (m_atarget_hot),
        .\gen_no_arbiter.grant_rnw_reg_0 (addr_arbiter_inst_n_47),
        .\gen_no_arbiter.grant_rnw_reg_1 (addr_arbiter_inst_n_51),
        .\gen_no_arbiter.m_grant_hot_i_reg[0]_inv_0 (reg_slice_r_n_2),
        .\gen_no_arbiter.m_grant_hot_i_reg[0]_inv_1 (splitter_aw_n_3),
        .m_atarget_enc(m_atarget_enc),
        .\m_atarget_enc_reg[1] (addr_arbiter_inst_n_44),
        .m_axi_arvalid(m_axi_arvalid),
        .\m_axi_arvalid[0] (m_ready_d),
        .m_axi_awready(m_axi_awready),
        .m_axi_awvalid(m_axi_awvalid),
        .m_axi_bready(m_axi_bready),
        .m_axi_wvalid(m_axi_wvalid),
        .m_ready_d0(m_ready_d0_0),
        .\m_ready_d_reg[0] (reg_slice_r_n_37),
        .\m_ready_d_reg[2] (m_ready_d0),
        .m_valid_i(m_valid_i),
        .mi_arvalid_en(mi_arvalid_en),
        .mi_bvalid(mi_bvalid),
        .mi_wready(mi_wready),
        .r_transfer_en(r_transfer_en),
        .s_axi_araddr(s_axi_araddr),
        .s_axi_arprot(s_axi_arprot),
        .s_axi_arready(s_axi_arready),
        .s_axi_arvalid(s_axi_arvalid),
        .s_axi_awaddr(s_axi_awaddr),
        .s_axi_awprot(s_axi_awprot),
        .s_axi_awready(s_axi_awready),
        .s_axi_awvalid(s_axi_awvalid),
        .s_axi_bready(s_axi_bready),
        .s_axi_bvalid(s_axi_bvalid),
        .s_axi_rready(s_axi_rready),
        .s_axi_rvalid(s_axi_rvalid),
        .s_axi_wready(s_axi_wready),
        .s_axi_wvalid(s_axi_wvalid),
        .s_axi_wvalid_0_sp_1(addr_arbiter_inst_n_50),
        .sr_rvalid(sr_rvalid));
  FDRE #(
    .INIT(1'b0)) 
    aresetn_d_reg
       (.C(aclk),
        .CE(1'b1),
        .D(aresetn),
        .Q(aresetn_d),
        .R(1'b0));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_decerr_slave \gen_decerr.decerr_slave_inst 
       (.Q(m_atarget_hot[2]),
        .SR(reset),
        .aa_rready(aa_rready),
        .aa_rvalid(aa_rvalid),
        .aclk(aclk),
        .aresetn_d(aresetn_d),
        .f_mux_return(f_mux_return),
        .f_mux_return__0(f_mux_return__0),
        .f_mux_return__1(f_mux_return__1),
        .f_mux_return__3(f_mux_return__3),
        .\gen_axilite.s_axi_awready_i_reg_0 (addr_arbiter_inst_n_50),
        .\gen_axilite.s_axi_awready_i_reg_1 (m_ready_d_1[2]),
        .\gen_axilite.s_axi_bvalid_i_reg_0 (addr_arbiter_inst_n_62),
        .m_atarget_enc(m_atarget_enc),
        .m_axi_arready(m_axi_arready),
        .m_axi_awready(m_axi_awready),
        .m_axi_bvalid(m_axi_bvalid),
        .m_axi_rvalid(m_axi_rvalid),
        .m_axi_wready(m_axi_wready),
        .mi_arvalid_en(mi_arvalid_en),
        .mi_bvalid(mi_bvalid),
        .mi_wready(mi_wready),
        .r_transfer_en(r_transfer_en));
  FDRE #(
    .INIT(1'b0)) 
    \m_atarget_enc_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(addr_arbiter_inst_n_61),
        .Q(m_atarget_enc[0]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \m_atarget_enc_reg[1] 
       (.C(aclk),
        .CE(1'b1),
        .D(any_error),
        .Q(m_atarget_enc[1]),
        .R(reset));
  FDRE #(
    .INIT(1'b0)) 
    \m_atarget_hot_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(m_atarget_hot0[0]),
        .Q(m_atarget_hot[0]),
        .R(reset));
  FDRE #(
    .INIT(1'b0)) 
    \m_atarget_hot_reg[1] 
       (.C(aclk),
        .CE(1'b1),
        .D(m_atarget_hot0[1]),
        .Q(m_atarget_hot[1]),
        .R(reset));
  FDRE #(
    .INIT(1'b0)) 
    \m_atarget_hot_reg[2] 
       (.C(aclk),
        .CE(1'b1),
        .D(m_atarget_hot0[2]),
        .Q(m_atarget_hot[2]),
        .R(reset));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_register_slice_v2_1_21_axic_register_slice reg_slice_r
       (.E(p_1_in),
        .Q(m_ready_d[0]),
        .SR(reset),
        .aa_grant_rnw(aa_grant_rnw),
        .aa_rready(aa_rready),
        .aa_rvalid(aa_rvalid),
        .aclk(aclk),
        .m_atarget_enc(m_atarget_enc),
        .m_axi_rdata(m_axi_rdata),
        .m_axi_rready(m_axi_rready),
        .\m_axi_rready[1] (m_atarget_hot[1:0]),
        .m_axi_rresp(m_axi_rresp),
        .\m_payload_i_reg[34]_0 ({\m_payload_i_reg[34] ,reg_slice_r_n_37}),
        .\m_ready_d_reg[0] (reg_slice_r_n_2),
        .m_valid_i(m_valid_i),
        .s_axi_rready(s_axi_rready),
        .sr_rvalid(sr_rvalid));
  LUT4 #(
    .INIT(16'h0FCA)) 
    \s_axi_bresp[0]_INST_0 
       (.I0(m_axi_bresp[0]),
        .I1(m_axi_bresp[2]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .O(s_axi_bresp[0]));
  LUT4 #(
    .INIT(16'h0FCA)) 
    \s_axi_bresp[1]_INST_0 
       (.I0(m_axi_bresp[1]),
        .I1(m_axi_bresp[3]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .O(s_axi_bresp[1]));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0 splitter_ar
       (.D(m_ready_d0_0),
        .Q(m_ready_d),
        .SR(addr_arbiter_inst_n_53),
        .aclk(aclk));
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter splitter_aw
       (.D(m_ready_d0),
        .Q(m_ready_d_1),
        .aclk(aclk),
        .aresetn_d(aresetn_d),
        .f_mux_return__1(f_mux_return__1),
        .f_mux_return__3(f_mux_return__3),
        .\m_ready_d_reg[0]_0 (splitter_aw_n_3),
        .\m_ready_d_reg[0]_1 (addr_arbiter_inst_n_44),
        .\m_ready_d_reg[0]_2 (addr_arbiter_inst_n_47),
        .\m_ready_d_reg[0]_3 (addr_arbiter_inst_n_51));
endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_decerr_slave
   (mi_bvalid,
    mi_wready,
    aa_rvalid,
    f_mux_return,
    f_mux_return__1,
    f_mux_return__3,
    f_mux_return__0,
    SR,
    \gen_axilite.s_axi_bvalid_i_reg_0 ,
    aclk,
    m_axi_rvalid,
    r_transfer_en,
    m_atarget_enc,
    aresetn_d,
    mi_arvalid_en,
    Q,
    m_axi_awready,
    m_axi_wready,
    m_axi_bvalid,
    m_axi_arready,
    aa_rready,
    \gen_axilite.s_axi_awready_i_reg_0 ,
    \gen_axilite.s_axi_awready_i_reg_1 );
  output [0:0]mi_bvalid;
  output [0:0]mi_wready;
  output aa_rvalid;
  output f_mux_return;
  output f_mux_return__1;
  output f_mux_return__3;
  output f_mux_return__0;
  input [0:0]SR;
  input \gen_axilite.s_axi_bvalid_i_reg_0 ;
  input aclk;
  input [1:0]m_axi_rvalid;
  input r_transfer_en;
  input [1:0]m_atarget_enc;
  input aresetn_d;
  input mi_arvalid_en;
  input [0:0]Q;
  input [1:0]m_axi_awready;
  input [1:0]m_axi_wready;
  input [1:0]m_axi_bvalid;
  input [1:0]m_axi_arready;
  input aa_rready;
  input \gen_axilite.s_axi_awready_i_reg_0 ;
  input [0:0]\gen_axilite.s_axi_awready_i_reg_1 ;

  wire [0:0]Q;
  wire [0:0]SR;
  wire aa_rready;
  wire aa_rvalid;
  wire aclk;
  wire aresetn_d;
  wire f_mux_return;
  wire f_mux_return__0;
  wire f_mux_return__1;
  wire f_mux_return__3;
  wire \gen_axilite.s_axi_arready_i_i_1_n_0 ;
  wire \gen_axilite.s_axi_awready_i_i_1_n_0 ;
  wire \gen_axilite.s_axi_awready_i_reg_0 ;
  wire [0:0]\gen_axilite.s_axi_awready_i_reg_1 ;
  wire \gen_axilite.s_axi_bvalid_i_reg_0 ;
  wire \gen_axilite.s_axi_rvalid_i_i_1_n_0 ;
  wire [1:0]m_atarget_enc;
  wire [1:0]m_axi_arready;
  wire [1:0]m_axi_awready;
  wire [1:0]m_axi_bvalid;
  wire [1:0]m_axi_rvalid;
  wire [1:0]m_axi_wready;
  wire [2:2]mi_arready;
  wire mi_arvalid_en;
  wire [0:0]mi_bvalid;
  wire [2:2]mi_rvalid;
  wire [0:0]mi_wready;
  wire r_transfer_en;

  LUT5 #(
    .INIT(32'hAA2A00AA)) 
    \gen_axilite.s_axi_arready_i_i_1 
       (.I0(aresetn_d),
        .I1(mi_arvalid_en),
        .I2(Q),
        .I3(mi_rvalid),
        .I4(mi_arready),
        .O(\gen_axilite.s_axi_arready_i_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \gen_axilite.s_axi_arready_i_reg 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_axilite.s_axi_arready_i_i_1_n_0 ),
        .Q(mi_arready),
        .R(1'b0));
  LUT5 #(
    .INIT(32'hFDFF0200)) 
    \gen_axilite.s_axi_awready_i_i_1 
       (.I0(\gen_axilite.s_axi_awready_i_reg_0 ),
        .I1(mi_bvalid),
        .I2(\gen_axilite.s_axi_awready_i_reg_1 ),
        .I3(Q),
        .I4(mi_wready),
        .O(\gen_axilite.s_axi_awready_i_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \gen_axilite.s_axi_awready_i_reg 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_axilite.s_axi_awready_i_i_1_n_0 ),
        .Q(mi_wready),
        .R(SR));
  FDRE #(
    .INIT(1'b0)) 
    \gen_axilite.s_axi_bvalid_i_reg 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_axilite.s_axi_bvalid_i_reg_0 ),
        .Q(mi_bvalid),
        .R(SR));
  LUT5 #(
    .INIT(32'h5FC05F00)) 
    \gen_axilite.s_axi_rvalid_i_i_1 
       (.I0(aa_rready),
        .I1(mi_arvalid_en),
        .I2(Q),
        .I3(mi_rvalid),
        .I4(mi_arready),
        .O(\gen_axilite.s_axi_rvalid_i_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \gen_axilite.s_axi_rvalid_i_reg 
       (.C(aclk),
        .CE(1'b1),
        .D(\gen_axilite.s_axi_rvalid_i_i_1_n_0 ),
        .Q(mi_rvalid),
        .R(SR));
  LUT5 #(
    .INIT(32'h00CCF0AA)) 
    \gen_no_arbiter.m_grant_hot_i[0]_inv_i_5 
       (.I0(m_axi_awready[0]),
        .I1(mi_wready),
        .I2(m_axi_awready[1]),
        .I3(m_atarget_enc[0]),
        .I4(m_atarget_enc[1]),
        .O(f_mux_return));
  LUT5 #(
    .INIT(32'h00CCF0AA)) 
    \m_ready_d[1]_i_3 
       (.I0(m_axi_arready[0]),
        .I1(mi_arready),
        .I2(m_axi_arready[1]),
        .I3(m_atarget_enc[0]),
        .I4(m_atarget_enc[1]),
        .O(f_mux_return__0));
  LUT6 #(
    .INIT(64'h00F0C0A00000C0A0)) 
    m_valid_i_i_2
       (.I0(m_axi_rvalid[0]),
        .I1(mi_rvalid),
        .I2(r_transfer_en),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(m_axi_rvalid[1]),
        .O(aa_rvalid));
  LUT5 #(
    .INIT(32'h00CCF0AA)) 
    \s_axi_bvalid[0]_INST_0_i_1 
       (.I0(m_axi_bvalid[0]),
        .I1(mi_bvalid),
        .I2(m_axi_bvalid[1]),
        .I3(m_atarget_enc[0]),
        .I4(m_atarget_enc[1]),
        .O(f_mux_return__3));
  LUT5 #(
    .INIT(32'h00CCF0AA)) 
    \s_axi_wready[0]_INST_0_i_1 
       (.I0(m_axi_wready[0]),
        .I1(mi_wready),
        .I2(m_axi_wready[1]),
        .I3(m_atarget_enc[0]),
        .I4(m_atarget_enc[1]),
        .O(f_mux_return__1));
endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter
   (Q,
    \m_ready_d_reg[0]_0 ,
    \m_ready_d_reg[0]_1 ,
    aresetn_d,
    \m_ready_d_reg[0]_2 ,
    f_mux_return__3,
    \m_ready_d_reg[0]_3 ,
    f_mux_return__1,
    D,
    aclk);
  output [2:0]Q;
  output \m_ready_d_reg[0]_0 ;
  input \m_ready_d_reg[0]_1 ;
  input aresetn_d;
  input \m_ready_d_reg[0]_2 ;
  input f_mux_return__3;
  input \m_ready_d_reg[0]_3 ;
  input f_mux_return__1;
  input [2:0]D;
  input aclk;

  wire [2:0]D;
  wire [2:0]Q;
  wire aclk;
  wire aresetn_d;
  wire f_mux_return__1;
  wire f_mux_return__3;
  wire \m_ready_d[2]_i_1_n_0 ;
  wire \m_ready_d_reg[0]_0 ;
  wire \m_ready_d_reg[0]_1 ;
  wire \m_ready_d_reg[0]_2 ;
  wire \m_ready_d_reg[0]_3 ;

  LUT4 #(
    .INIT(16'hE0FF)) 
    \m_ready_d[2]_i_1 
       (.I0(\m_ready_d_reg[0]_1 ),
        .I1(Q[2]),
        .I2(\m_ready_d_reg[0]_0 ),
        .I3(aresetn_d),
        .O(\m_ready_d[2]_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hEAEAEA00EA00EA00)) 
    \m_ready_d[2]_i_4 
       (.I0(Q[0]),
        .I1(\m_ready_d_reg[0]_2 ),
        .I2(f_mux_return__3),
        .I3(Q[1]),
        .I4(\m_ready_d_reg[0]_3 ),
        .I5(f_mux_return__1),
        .O(\m_ready_d_reg[0]_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \m_ready_d_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(D[0]),
        .Q(Q[0]),
        .R(\m_ready_d[2]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \m_ready_d_reg[1] 
       (.C(aclk),
        .CE(1'b1),
        .D(D[1]),
        .Q(Q[1]),
        .R(\m_ready_d[2]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \m_ready_d_reg[2] 
       (.C(aclk),
        .CE(1'b1),
        .D(D[2]),
        .Q(Q[2]),
        .R(\m_ready_d[2]_i_1_n_0 ));
endmodule

(* ORIG_REF_NAME = "axi_crossbar_v2_1_22_splitter" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_splitter__parameterized0
   (Q,
    SR,
    D,
    aclk);
  output [1:0]Q;
  input [0:0]SR;
  input [1:0]D;
  input aclk;

  wire [1:0]D;
  wire [1:0]Q;
  wire [0:0]SR;
  wire aclk;

  FDRE #(
    .INIT(1'b0)) 
    \m_ready_d_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(D[0]),
        .Q(Q[0]),
        .R(SR));
  FDRE #(
    .INIT(1'b0)) 
    \m_ready_d_reg[1] 
       (.C(aclk),
        .CE(1'b1),
        .D(D[1]),
        .Q(Q[1]),
        .R(SR));
endmodule

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_register_slice_v2_1_21_axic_register_slice
   (sr_rvalid,
    aa_rready,
    \m_ready_d_reg[0] ,
    \m_payload_i_reg[34]_0 ,
    m_axi_rready,
    aclk,
    aa_rvalid,
    E,
    Q,
    m_valid_i,
    s_axi_rready,
    aa_grant_rnw,
    m_atarget_enc,
    m_axi_rresp,
    m_axi_rdata,
    \m_axi_rready[1] ,
    SR);
  output sr_rvalid;
  output aa_rready;
  output \m_ready_d_reg[0] ;
  output [34:0]\m_payload_i_reg[34]_0 ;
  output [1:0]m_axi_rready;
  input aclk;
  input aa_rvalid;
  input [0:0]E;
  input [0:0]Q;
  input m_valid_i;
  input [0:0]s_axi_rready;
  input aa_grant_rnw;
  input [1:0]m_atarget_enc;
  input [3:0]m_axi_rresp;
  input [63:0]m_axi_rdata;
  input [1:0]\m_axi_rready[1] ;
  input [0:0]SR;

  wire [0:0]E;
  wire [0:0]Q;
  wire [0:0]SR;
  wire aa_grant_rnw;
  wire aa_rready;
  wire aa_rvalid;
  wire aclk;
  wire \aresetn_d_reg_n_0_[0] ;
  wire \aresetn_d_reg_n_0_[1] ;
  wire [1:0]m_atarget_enc;
  wire [63:0]m_axi_rdata;
  wire [1:0]m_axi_rready;
  wire [1:0]\m_axi_rready[1] ;
  wire [3:0]m_axi_rresp;
  wire [34:0]\m_payload_i_reg[34]_0 ;
  wire \m_ready_d_reg[0] ;
  wire m_valid_i;
  wire m_valid_i_i_1_n_0;
  wire [0:0]s_axi_rready;
  wire s_ready_i_i_1_n_0;
  wire [34:0]skid_buffer;
  wire \skid_buffer_reg_n_0_[0] ;
  wire \skid_buffer_reg_n_0_[10] ;
  wire \skid_buffer_reg_n_0_[11] ;
  wire \skid_buffer_reg_n_0_[12] ;
  wire \skid_buffer_reg_n_0_[13] ;
  wire \skid_buffer_reg_n_0_[14] ;
  wire \skid_buffer_reg_n_0_[15] ;
  wire \skid_buffer_reg_n_0_[16] ;
  wire \skid_buffer_reg_n_0_[17] ;
  wire \skid_buffer_reg_n_0_[18] ;
  wire \skid_buffer_reg_n_0_[19] ;
  wire \skid_buffer_reg_n_0_[1] ;
  wire \skid_buffer_reg_n_0_[20] ;
  wire \skid_buffer_reg_n_0_[21] ;
  wire \skid_buffer_reg_n_0_[22] ;
  wire \skid_buffer_reg_n_0_[23] ;
  wire \skid_buffer_reg_n_0_[24] ;
  wire \skid_buffer_reg_n_0_[25] ;
  wire \skid_buffer_reg_n_0_[26] ;
  wire \skid_buffer_reg_n_0_[27] ;
  wire \skid_buffer_reg_n_0_[28] ;
  wire \skid_buffer_reg_n_0_[29] ;
  wire \skid_buffer_reg_n_0_[2] ;
  wire \skid_buffer_reg_n_0_[30] ;
  wire \skid_buffer_reg_n_0_[31] ;
  wire \skid_buffer_reg_n_0_[32] ;
  wire \skid_buffer_reg_n_0_[33] ;
  wire \skid_buffer_reg_n_0_[34] ;
  wire \skid_buffer_reg_n_0_[3] ;
  wire \skid_buffer_reg_n_0_[4] ;
  wire \skid_buffer_reg_n_0_[5] ;
  wire \skid_buffer_reg_n_0_[6] ;
  wire \skid_buffer_reg_n_0_[7] ;
  wire \skid_buffer_reg_n_0_[8] ;
  wire \skid_buffer_reg_n_0_[9] ;
  wire sr_rvalid;

  FDRE #(
    .INIT(1'b0)) 
    \aresetn_d_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(1'b1),
        .Q(\aresetn_d_reg_n_0_[0] ),
        .R(SR));
  FDRE #(
    .INIT(1'b0)) 
    \aresetn_d_reg[1] 
       (.C(aclk),
        .CE(1'b1),
        .D(\aresetn_d_reg_n_0_[0] ),
        .Q(\aresetn_d_reg_n_0_[1] ),
        .R(SR));
  LUT6 #(
    .INIT(64'hEAAAAAAA00000000)) 
    \gen_no_arbiter.m_grant_hot_i[0]_inv_i_3 
       (.I0(Q),
        .I1(sr_rvalid),
        .I2(\m_payload_i_reg[34]_0 [0]),
        .I3(m_valid_i),
        .I4(s_axi_rready),
        .I5(aa_grant_rnw),
        .O(\m_ready_d_reg[0] ));
  LUT2 #(
    .INIT(4'h8)) 
    \m_axi_rready[0]_INST_0 
       (.I0(\m_axi_rready[1] [0]),
        .I1(aa_rready),
        .O(m_axi_rready[0]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \m_axi_rready[1]_INST_0 
       (.I0(\m_axi_rready[1] [1]),
        .I1(aa_rready),
        .O(m_axi_rready[1]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[10]_i_1 
       (.I0(\skid_buffer_reg_n_0_[10] ),
        .I1(m_axi_rdata[7]),
        .I2(m_axi_rdata[39]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[10]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[11]_i_1 
       (.I0(\skid_buffer_reg_n_0_[11] ),
        .I1(m_axi_rdata[8]),
        .I2(m_axi_rdata[40]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[11]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[12]_i_1 
       (.I0(m_axi_rdata[9]),
        .I1(m_axi_rdata[41]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[12] ),
        .O(skid_buffer[12]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[13]_i_1 
       (.I0(m_axi_rdata[10]),
        .I1(m_axi_rdata[42]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[13] ),
        .O(skid_buffer[13]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[14]_i_1 
       (.I0(m_axi_rdata[11]),
        .I1(m_axi_rdata[43]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[14] ),
        .O(skid_buffer[14]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[15]_i_1 
       (.I0(m_axi_rdata[12]),
        .I1(m_axi_rdata[44]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[15] ),
        .O(skid_buffer[15]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[16]_i_1 
       (.I0(\skid_buffer_reg_n_0_[16] ),
        .I1(m_axi_rdata[13]),
        .I2(m_axi_rdata[45]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[16]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[17]_i_1 
       (.I0(m_axi_rdata[14]),
        .I1(m_axi_rdata[46]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[17] ),
        .O(skid_buffer[17]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[18]_i_1 
       (.I0(m_axi_rdata[15]),
        .I1(m_axi_rdata[47]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[18] ),
        .O(skid_buffer[18]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[19]_i_1 
       (.I0(\skid_buffer_reg_n_0_[19] ),
        .I1(m_axi_rdata[16]),
        .I2(m_axi_rdata[48]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[19]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[1]_i_1 
       (.I0(m_axi_rresp[0]),
        .I1(m_axi_rresp[2]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[1] ),
        .O(skid_buffer[1]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[20]_i_1 
       (.I0(\skid_buffer_reg_n_0_[20] ),
        .I1(m_axi_rdata[17]),
        .I2(m_axi_rdata[49]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[20]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[21]_i_1 
       (.I0(\skid_buffer_reg_n_0_[21] ),
        .I1(m_axi_rdata[18]),
        .I2(m_axi_rdata[50]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[21]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[22]_i_1 
       (.I0(\skid_buffer_reg_n_0_[22] ),
        .I1(m_axi_rdata[19]),
        .I2(m_axi_rdata[51]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[22]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[23]_i_1 
       (.I0(\skid_buffer_reg_n_0_[23] ),
        .I1(m_axi_rdata[20]),
        .I2(m_axi_rdata[52]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[23]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[24]_i_1 
       (.I0(\skid_buffer_reg_n_0_[24] ),
        .I1(m_axi_rdata[21]),
        .I2(m_axi_rdata[53]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[24]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[25]_i_1 
       (.I0(m_axi_rdata[22]),
        .I1(m_axi_rdata[54]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[25] ),
        .O(skid_buffer[25]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[26]_i_1 
       (.I0(m_axi_rdata[23]),
        .I1(m_axi_rdata[55]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[26] ),
        .O(skid_buffer[26]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[27]_i_1 
       (.I0(\skid_buffer_reg_n_0_[27] ),
        .I1(m_axi_rdata[24]),
        .I2(m_axi_rdata[56]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[27]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[28]_i_1 
       (.I0(m_axi_rdata[25]),
        .I1(m_axi_rdata[57]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[28] ),
        .O(skid_buffer[28]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[29]_i_1 
       (.I0(m_axi_rdata[26]),
        .I1(m_axi_rdata[58]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[29] ),
        .O(skid_buffer[29]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[2]_i_1 
       (.I0(m_axi_rresp[1]),
        .I1(m_axi_rresp[3]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[2] ),
        .O(skid_buffer[2]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[30]_i_1 
       (.I0(m_axi_rdata[27]),
        .I1(m_axi_rdata[59]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[30] ),
        .O(skid_buffer[30]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[31]_i_1 
       (.I0(m_axi_rdata[28]),
        .I1(m_axi_rdata[60]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[31] ),
        .O(skid_buffer[31]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[32]_i_1 
       (.I0(\skid_buffer_reg_n_0_[32] ),
        .I1(m_axi_rdata[29]),
        .I2(m_axi_rdata[61]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[32]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[33]_i_1 
       (.I0(m_axi_rdata[30]),
        .I1(m_axi_rdata[62]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[33] ),
        .O(skid_buffer[33]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[34]_i_2 
       (.I0(m_axi_rdata[31]),
        .I1(m_axi_rdata[63]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[34] ),
        .O(skid_buffer[34]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[3]_i_1 
       (.I0(\skid_buffer_reg_n_0_[3] ),
        .I1(m_axi_rdata[0]),
        .I2(m_axi_rdata[32]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[3]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[4]_i_1 
       (.I0(\skid_buffer_reg_n_0_[4] ),
        .I1(m_axi_rdata[1]),
        .I2(m_axi_rdata[33]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[4]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[5]_i_1 
       (.I0(m_axi_rdata[2]),
        .I1(m_axi_rdata[34]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[5] ),
        .O(skid_buffer[5]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[6]_i_1 
       (.I0(m_axi_rdata[3]),
        .I1(m_axi_rdata[35]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[6] ),
        .O(skid_buffer[6]));
  LUT6 #(
    .INIT(64'h0FCAFFFF0FCA0000)) 
    \m_payload_i[7]_i_1 
       (.I0(m_axi_rdata[4]),
        .I1(m_axi_rdata[36]),
        .I2(m_atarget_enc[0]),
        .I3(m_atarget_enc[1]),
        .I4(aa_rready),
        .I5(\skid_buffer_reg_n_0_[7] ),
        .O(skid_buffer[7]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[8]_i_1 
       (.I0(\skid_buffer_reg_n_0_[8] ),
        .I1(m_axi_rdata[5]),
        .I2(m_axi_rdata[37]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[8]));
  LUT6 #(
    .INIT(64'h00F000CCAAAAAAAA)) 
    \m_payload_i[9]_i_1 
       (.I0(\skid_buffer_reg_n_0_[9] ),
        .I1(m_axi_rdata[6]),
        .I2(m_axi_rdata[38]),
        .I3(m_atarget_enc[1]),
        .I4(m_atarget_enc[0]),
        .I5(aa_rready),
        .O(skid_buffer[9]));
  FDRE \m_payload_i_reg[0] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[0]),
        .Q(\m_payload_i_reg[34]_0 [0]),
        .R(1'b0));
  FDRE \m_payload_i_reg[10] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[10]),
        .Q(\m_payload_i_reg[34]_0 [10]),
        .R(1'b0));
  FDRE \m_payload_i_reg[11] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[11]),
        .Q(\m_payload_i_reg[34]_0 [11]),
        .R(1'b0));
  FDRE \m_payload_i_reg[12] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[12]),
        .Q(\m_payload_i_reg[34]_0 [12]),
        .R(1'b0));
  FDRE \m_payload_i_reg[13] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[13]),
        .Q(\m_payload_i_reg[34]_0 [13]),
        .R(1'b0));
  FDRE \m_payload_i_reg[14] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[14]),
        .Q(\m_payload_i_reg[34]_0 [14]),
        .R(1'b0));
  FDRE \m_payload_i_reg[15] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[15]),
        .Q(\m_payload_i_reg[34]_0 [15]),
        .R(1'b0));
  FDRE \m_payload_i_reg[16] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[16]),
        .Q(\m_payload_i_reg[34]_0 [16]),
        .R(1'b0));
  FDRE \m_payload_i_reg[17] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[17]),
        .Q(\m_payload_i_reg[34]_0 [17]),
        .R(1'b0));
  FDRE \m_payload_i_reg[18] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[18]),
        .Q(\m_payload_i_reg[34]_0 [18]),
        .R(1'b0));
  FDRE \m_payload_i_reg[19] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[19]),
        .Q(\m_payload_i_reg[34]_0 [19]),
        .R(1'b0));
  FDRE \m_payload_i_reg[1] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[1]),
        .Q(\m_payload_i_reg[34]_0 [1]),
        .R(1'b0));
  FDRE \m_payload_i_reg[20] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[20]),
        .Q(\m_payload_i_reg[34]_0 [20]),
        .R(1'b0));
  FDRE \m_payload_i_reg[21] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[21]),
        .Q(\m_payload_i_reg[34]_0 [21]),
        .R(1'b0));
  FDRE \m_payload_i_reg[22] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[22]),
        .Q(\m_payload_i_reg[34]_0 [22]),
        .R(1'b0));
  FDRE \m_payload_i_reg[23] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[23]),
        .Q(\m_payload_i_reg[34]_0 [23]),
        .R(1'b0));
  FDRE \m_payload_i_reg[24] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[24]),
        .Q(\m_payload_i_reg[34]_0 [24]),
        .R(1'b0));
  FDRE \m_payload_i_reg[25] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[25]),
        .Q(\m_payload_i_reg[34]_0 [25]),
        .R(1'b0));
  FDRE \m_payload_i_reg[26] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[26]),
        .Q(\m_payload_i_reg[34]_0 [26]),
        .R(1'b0));
  FDRE \m_payload_i_reg[27] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[27]),
        .Q(\m_payload_i_reg[34]_0 [27]),
        .R(1'b0));
  FDRE \m_payload_i_reg[28] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[28]),
        .Q(\m_payload_i_reg[34]_0 [28]),
        .R(1'b0));
  FDRE \m_payload_i_reg[29] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[29]),
        .Q(\m_payload_i_reg[34]_0 [29]),
        .R(1'b0));
  FDRE \m_payload_i_reg[2] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[2]),
        .Q(\m_payload_i_reg[34]_0 [2]),
        .R(1'b0));
  FDRE \m_payload_i_reg[30] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[30]),
        .Q(\m_payload_i_reg[34]_0 [30]),
        .R(1'b0));
  FDRE \m_payload_i_reg[31] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[31]),
        .Q(\m_payload_i_reg[34]_0 [31]),
        .R(1'b0));
  FDRE \m_payload_i_reg[32] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[32]),
        .Q(\m_payload_i_reg[34]_0 [32]),
        .R(1'b0));
  FDRE \m_payload_i_reg[33] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[33]),
        .Q(\m_payload_i_reg[34]_0 [33]),
        .R(1'b0));
  FDRE \m_payload_i_reg[34] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[34]),
        .Q(\m_payload_i_reg[34]_0 [34]),
        .R(1'b0));
  FDRE \m_payload_i_reg[3] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[3]),
        .Q(\m_payload_i_reg[34]_0 [3]),
        .R(1'b0));
  FDRE \m_payload_i_reg[4] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[4]),
        .Q(\m_payload_i_reg[34]_0 [4]),
        .R(1'b0));
  FDRE \m_payload_i_reg[5] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[5]),
        .Q(\m_payload_i_reg[34]_0 [5]),
        .R(1'b0));
  FDRE \m_payload_i_reg[6] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[6]),
        .Q(\m_payload_i_reg[34]_0 [6]),
        .R(1'b0));
  FDRE \m_payload_i_reg[7] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[7]),
        .Q(\m_payload_i_reg[34]_0 [7]),
        .R(1'b0));
  FDRE \m_payload_i_reg[8] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[8]),
        .Q(\m_payload_i_reg[34]_0 [8]),
        .R(1'b0));
  FDRE \m_payload_i_reg[9] 
       (.C(aclk),
        .CE(E),
        .D(skid_buffer[9]),
        .Q(\m_payload_i_reg[34]_0 [9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT4 #(
    .INIT(16'hC4CC)) 
    m_valid_i_i_1
       (.I0(E),
        .I1(\aresetn_d_reg_n_0_[1] ),
        .I2(aa_rvalid),
        .I3(aa_rready),
        .O(m_valid_i_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    m_valid_i_reg
       (.C(aclk),
        .CE(1'b1),
        .D(m_valid_i_i_1_n_0),
        .Q(sr_rvalid),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT4 #(
    .INIT(16'hF040)) 
    s_ready_i_i_1
       (.I0(aa_rvalid),
        .I1(aa_rready),
        .I2(\aresetn_d_reg_n_0_[0] ),
        .I3(E),
        .O(s_ready_i_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    s_ready_i_reg
       (.C(aclk),
        .CE(1'b1),
        .D(s_ready_i_i_1_n_0),
        .Q(aa_rready),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT4 #(
    .INIT(16'h3FAA)) 
    \skid_buffer[0]_i_1 
       (.I0(\skid_buffer_reg_n_0_[0] ),
        .I1(m_atarget_enc[1]),
        .I2(m_atarget_enc[0]),
        .I3(aa_rready),
        .O(skid_buffer[0]));
  FDRE \skid_buffer_reg[0] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[0]),
        .Q(\skid_buffer_reg_n_0_[0] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[10] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[10]),
        .Q(\skid_buffer_reg_n_0_[10] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[11] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[11]),
        .Q(\skid_buffer_reg_n_0_[11] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[12] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[12]),
        .Q(\skid_buffer_reg_n_0_[12] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[13] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[13]),
        .Q(\skid_buffer_reg_n_0_[13] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[14] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[14]),
        .Q(\skid_buffer_reg_n_0_[14] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[15] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[15]),
        .Q(\skid_buffer_reg_n_0_[15] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[16] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[16]),
        .Q(\skid_buffer_reg_n_0_[16] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[17] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[17]),
        .Q(\skid_buffer_reg_n_0_[17] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[18] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[18]),
        .Q(\skid_buffer_reg_n_0_[18] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[19] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[19]),
        .Q(\skid_buffer_reg_n_0_[19] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[1] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[1]),
        .Q(\skid_buffer_reg_n_0_[1] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[20] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[20]),
        .Q(\skid_buffer_reg_n_0_[20] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[21] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[21]),
        .Q(\skid_buffer_reg_n_0_[21] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[22] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[22]),
        .Q(\skid_buffer_reg_n_0_[22] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[23] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[23]),
        .Q(\skid_buffer_reg_n_0_[23] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[24] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[24]),
        .Q(\skid_buffer_reg_n_0_[24] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[25] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[25]),
        .Q(\skid_buffer_reg_n_0_[25] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[26] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[26]),
        .Q(\skid_buffer_reg_n_0_[26] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[27] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[27]),
        .Q(\skid_buffer_reg_n_0_[27] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[28] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[28]),
        .Q(\skid_buffer_reg_n_0_[28] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[29] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[29]),
        .Q(\skid_buffer_reg_n_0_[29] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[2] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[2]),
        .Q(\skid_buffer_reg_n_0_[2] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[30] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[30]),
        .Q(\skid_buffer_reg_n_0_[30] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[31] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[31]),
        .Q(\skid_buffer_reg_n_0_[31] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[32] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[32]),
        .Q(\skid_buffer_reg_n_0_[32] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[33] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[33]),
        .Q(\skid_buffer_reg_n_0_[33] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[34] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[34]),
        .Q(\skid_buffer_reg_n_0_[34] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[3] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[3]),
        .Q(\skid_buffer_reg_n_0_[3] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[4] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[4]),
        .Q(\skid_buffer_reg_n_0_[4] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[5] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[5]),
        .Q(\skid_buffer_reg_n_0_[5] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[6] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[6]),
        .Q(\skid_buffer_reg_n_0_[6] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[7] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[7]),
        .Q(\skid_buffer_reg_n_0_[7] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[8] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[8]),
        .Q(\skid_buffer_reg_n_0_[8] ),
        .R(1'b0));
  FDRE \skid_buffer_reg[9] 
       (.C(aclk),
        .CE(1'b1),
        .D(skid_buffer[9]),
        .Q(\skid_buffer_reg_n_0_[9] ),
        .R(1'b0));
endmodule

(* CHECK_LICENSE_TYPE = "pfm_dynamic_xbar_0,axi_crossbar_v2_1_22_axi_crossbar,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* X_CORE_INFO = "axi_crossbar_v2_1_22_axi_crossbar,Vivado 2020.1" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
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
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLKIF CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLKIF, FREQ_HZ 50925925, FREQ_TOLERANCE_HZ 0, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, ASSOCIATED_BUSIF M00_AXI:M01_AXI:M02_AXI:M03_AXI:M04_AXI:M05_AXI:M06_AXI:M07_AXI:M08_AXI:M09_AXI:M10_AXI:M11_AXI:M12_AXI:M13_AXI:M14_AXI:M15_AXI:S00_AXI:S01_AXI:S02_AXI:S03_AXI:S04_AXI:S05_AXI:S06_AXI:S07_AXI:S08_AXI:S09_AXI:S10_AXI:S11_AXI:S12_AXI:S13_AXI:S14_AXI:S15_AXI, ASSOCIATED_RESET ARESETN, INSERT_VIP 0" *) input aclk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RSTIF RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RSTIF, POLARITY ACTIVE_LOW, INSERT_VIP 0, TYPE INTERCONNECT" *) input aresetn;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWADDR" *) input [24:0]s_axi_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWPROT" *) input [2:0]s_axi_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWVALID" *) input [0:0]s_axi_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI AWREADY" *) output [0:0]s_axi_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WDATA" *) input [31:0]s_axi_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WSTRB" *) input [3:0]s_axi_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WVALID" *) input [0:0]s_axi_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI WREADY" *) output [0:0]s_axi_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BRESP" *) output [1:0]s_axi_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BVALID" *) output [0:0]s_axi_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI BREADY" *) input [0:0]s_axi_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARADDR" *) input [24:0]s_axi_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARPROT" *) input [2:0]s_axi_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARVALID" *) input [0:0]s_axi_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI ARREADY" *) output [0:0]s_axi_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RDATA" *) output [31:0]s_axi_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RRESP" *) output [1:0]s_axi_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RVALID" *) output [0:0]s_axi_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S00_AXI RREADY" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME S00_AXI, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input [0:0]s_axi_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR [24:0] [24:0], xilinx.com:interface:aximm:1.0 M01_AXI AWADDR [24:0] [49:25]" *) output [49:0]m_axi_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWPROT [2:0] [2:0], xilinx.com:interface:aximm:1.0 M01_AXI AWPROT [2:0] [5:3]" *) output [5:0]m_axi_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI AWVALID [0:0] [1:1]" *) output [1:0]m_axi_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI AWREADY [0:0] [1:1]" *) input [1:0]m_axi_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WDATA [31:0] [31:0], xilinx.com:interface:aximm:1.0 M01_AXI WDATA [31:0] [63:32]" *) output [63:0]m_axi_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB [3:0] [3:0], xilinx.com:interface:aximm:1.0 M01_AXI WSTRB [3:0] [7:4]" *) output [7:0]m_axi_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI WVALID [0:0] [1:1]" *) output [1:0]m_axi_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI WREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI WREADY [0:0] [1:1]" *) input [1:0]m_axi_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BRESP [1:0] [1:0], xilinx.com:interface:aximm:1.0 M01_AXI BRESP [1:0] [3:2]" *) input [3:0]m_axi_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI BVALID [0:0] [1:1]" *) input [1:0]m_axi_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI BREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI BREADY [0:0] [1:1]" *) output [1:0]m_axi_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR [24:0] [24:0], xilinx.com:interface:aximm:1.0 M01_AXI ARADDR [24:0] [49:25]" *) output [49:0]m_axi_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARPROT [2:0] [2:0], xilinx.com:interface:aximm:1.0 M01_AXI ARPROT [2:0] [5:3]" *) output [5:0]m_axi_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI ARVALID [0:0] [1:1]" *) output [1:0]m_axi_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI ARREADY [0:0] [1:1]" *) input [1:0]m_axi_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RDATA [31:0] [31:0], xilinx.com:interface:aximm:1.0 M01_AXI RDATA [31:0] [63:32]" *) input [63:0]m_axi_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RRESP [1:0] [1:0], xilinx.com:interface:aximm:1.0 M01_AXI RRESP [1:0] [3:2]" *) input [3:0]m_axi_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RVALID [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI RVALID [0:0] [1:1]" *) input [1:0]m_axi_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M00_AXI RREADY [0:0] [0:0], xilinx.com:interface:aximm:1.0 M01_AXI RREADY [0:0] [1:1]" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0, XIL_INTERFACENAME M01_AXI, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 50925925, ID_WIDTH 0, ADDR_WIDTH 25, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 1, NUM_WRITE_OUTSTANDING 1, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN pfm_dynamic_clkwiz_sysclks_clk_out2, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) output [1:0]m_axi_rready;

  wire aclk;
  wire aresetn;
  wire [49:0]m_axi_araddr;
  wire [5:0]m_axi_arprot;
  wire [1:0]m_axi_arready;
  wire [1:0]m_axi_arvalid;
  wire [49:0]m_axi_awaddr;
  wire [5:0]m_axi_awprot;
  wire [1:0]m_axi_awready;
  wire [1:0]m_axi_awvalid;
  wire [1:0]m_axi_bready;
  wire [3:0]m_axi_bresp;
  wire [1:0]m_axi_bvalid;
  wire [63:0]m_axi_rdata;
  wire [1:0]m_axi_rready;
  wire [3:0]m_axi_rresp;
  wire [1:0]m_axi_rvalid;
  wire [63:0]m_axi_wdata;
  wire [1:0]m_axi_wready;
  wire [7:0]m_axi_wstrb;
  wire [1:0]m_axi_wvalid;
  wire [24:0]s_axi_araddr;
  wire [2:0]s_axi_arprot;
  wire [0:0]s_axi_arready;
  wire [0:0]s_axi_arvalid;
  wire [24:0]s_axi_awaddr;
  wire [2:0]s_axi_awprot;
  wire [0:0]s_axi_awready;
  wire [0:0]s_axi_awvalid;
  wire [0:0]s_axi_bready;
  wire [1:0]s_axi_bresp;
  wire [0:0]s_axi_bvalid;
  wire [31:0]s_axi_rdata;
  wire [0:0]s_axi_rready;
  wire [1:0]s_axi_rresp;
  wire [0:0]s_axi_rvalid;
  wire [31:0]s_axi_wdata;
  wire [0:0]s_axi_wready;
  wire [3:0]s_axi_wstrb;
  wire [0:0]s_axi_wvalid;
  wire [3:0]NLW_inst_m_axi_arburst_UNCONNECTED;
  wire [7:0]NLW_inst_m_axi_arcache_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_arid_UNCONNECTED;
  wire [15:0]NLW_inst_m_axi_arlen_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_arlock_UNCONNECTED;
  wire [7:0]NLW_inst_m_axi_arqos_UNCONNECTED;
  wire [7:0]NLW_inst_m_axi_arregion_UNCONNECTED;
  wire [5:0]NLW_inst_m_axi_arsize_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_aruser_UNCONNECTED;
  wire [3:0]NLW_inst_m_axi_awburst_UNCONNECTED;
  wire [7:0]NLW_inst_m_axi_awcache_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_awid_UNCONNECTED;
  wire [15:0]NLW_inst_m_axi_awlen_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_awlock_UNCONNECTED;
  wire [7:0]NLW_inst_m_axi_awqos_UNCONNECTED;
  wire [7:0]NLW_inst_m_axi_awregion_UNCONNECTED;
  wire [5:0]NLW_inst_m_axi_awsize_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_awuser_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_wid_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_wlast_UNCONNECTED;
  wire [1:0]NLW_inst_m_axi_wuser_UNCONNECTED;
  wire [0:0]NLW_inst_s_axi_bid_UNCONNECTED;
  wire [0:0]NLW_inst_s_axi_buser_UNCONNECTED;
  wire [0:0]NLW_inst_s_axi_rid_UNCONNECTED;
  wire [0:0]NLW_inst_s_axi_rlast_UNCONNECTED;
  wire [0:0]NLW_inst_s_axi_ruser_UNCONNECTED;

  (* C_AXI_ADDR_WIDTH = "25" *) 
  (* C_AXI_ARUSER_WIDTH = "1" *) 
  (* C_AXI_AWUSER_WIDTH = "1" *) 
  (* C_AXI_BUSER_WIDTH = "1" *) 
  (* C_AXI_DATA_WIDTH = "32" *) 
  (* C_AXI_ID_WIDTH = "1" *) 
  (* C_AXI_PROTOCOL = "2" *) 
  (* C_AXI_RUSER_WIDTH = "1" *) 
  (* C_AXI_SUPPORTS_USER_SIGNALS = "0" *) 
  (* C_AXI_WUSER_WIDTH = "1" *) 
  (* C_CONNECTIVITY_MODE = "0" *) 
  (* C_DEBUG = "1" *) 
  (* C_FAMILY = "virtexuplus" *) 
  (* C_M_AXI_ADDR_WIDTH = "320'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000010000000000000000000000000000000100000000000000000000000000000001000000000000000000000000000000001100" *) 
  (* C_M_AXI_BASE_ADDR = "640'b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000000000000000000000000000000000000000110000101000000000000000000000000000000000000000000000000000000011000010000000000000000000000000000000000000000000000000000000001100000110000000000000000000000000000000000000000000000000000000110000010000000000000000000000000000000000000000000000000000000011000000100000000000000000000000000000000000000000000000000000001100000000000000000000000" *) 
  (* C_M_AXI_READ_CONNECTIVITY = "64'b1111111111111111111111111111111111111111111111111111111111111111" *) 
  (* C_M_AXI_READ_ISSUING = "64'b0000000000000000000000000000000100000000000000000000000000000001" *) 
  (* C_M_AXI_SECURE = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) 
  (* C_M_AXI_WRITE_CONNECTIVITY = "64'b1111111111111111111111111111111111111111111111111111111111111111" *) 
  (* C_M_AXI_WRITE_ISSUING = "64'b0000000000000000000000000000000100000000000000000000000000000001" *) 
  (* C_NUM_ADDR_RANGES = "5" *) 
  (* C_NUM_MASTER_SLOTS = "2" *) 
  (* C_NUM_SLAVE_SLOTS = "1" *) 
  (* C_R_REGISTER = "1" *) 
  (* C_S_AXI_ARB_PRIORITY = "0" *) 
  (* C_S_AXI_BASE_ID = "0" *) 
  (* C_S_AXI_READ_ACCEPTANCE = "1" *) 
  (* C_S_AXI_SINGLE_THREAD = "1" *) 
  (* C_S_AXI_THREAD_ID_WIDTH = "0" *) 
  (* C_S_AXI_WRITE_ACCEPTANCE = "1" *) 
  (* DowngradeIPIdentifiedWarnings = "yes" *) 
  (* P_ADDR_DECODE = "1" *) 
  (* P_AXI3 = "1" *) 
  (* P_AXI4 = "0" *) 
  (* P_AXILITE = "2" *) 
  (* P_AXILITE_SIZE = "3'b010" *) 
  (* P_FAMILY = "rtl" *) 
  (* P_INCR = "2'b01" *) 
  (* P_LEN = "8" *) 
  (* P_LOCK = "1" *) 
  (* P_M_AXI_ERR_MODE = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) 
  (* P_M_AXI_SUPPORTS_READ = "2'b11" *) 
  (* P_M_AXI_SUPPORTS_WRITE = "2'b11" *) 
  (* P_ONES = "65'b11111111111111111111111111111111111111111111111111111111111111111" *) 
  (* P_RANGE_CHECK = "1" *) 
  (* P_S_AXI_BASE_ID = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) 
  (* P_S_AXI_HIGH_ID = "64'b0000000000000000000000000000000000000000000000000000000000000000" *) 
  (* P_S_AXI_SUPPORTS_READ = "1'b1" *) 
  (* P_S_AXI_SUPPORTS_WRITE = "1'b1" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_axi_crossbar_v2_1_22_axi_crossbar inst
       (.aclk(aclk),
        .aresetn(aresetn),
        .m_axi_araddr(m_axi_araddr),
        .m_axi_arburst(NLW_inst_m_axi_arburst_UNCONNECTED[3:0]),
        .m_axi_arcache(NLW_inst_m_axi_arcache_UNCONNECTED[7:0]),
        .m_axi_arid(NLW_inst_m_axi_arid_UNCONNECTED[1:0]),
        .m_axi_arlen(NLW_inst_m_axi_arlen_UNCONNECTED[15:0]),
        .m_axi_arlock(NLW_inst_m_axi_arlock_UNCONNECTED[1:0]),
        .m_axi_arprot(m_axi_arprot),
        .m_axi_arqos(NLW_inst_m_axi_arqos_UNCONNECTED[7:0]),
        .m_axi_arready(m_axi_arready),
        .m_axi_arregion(NLW_inst_m_axi_arregion_UNCONNECTED[7:0]),
        .m_axi_arsize(NLW_inst_m_axi_arsize_UNCONNECTED[5:0]),
        .m_axi_aruser(NLW_inst_m_axi_aruser_UNCONNECTED[1:0]),
        .m_axi_arvalid(m_axi_arvalid),
        .m_axi_awaddr(m_axi_awaddr),
        .m_axi_awburst(NLW_inst_m_axi_awburst_UNCONNECTED[3:0]),
        .m_axi_awcache(NLW_inst_m_axi_awcache_UNCONNECTED[7:0]),
        .m_axi_awid(NLW_inst_m_axi_awid_UNCONNECTED[1:0]),
        .m_axi_awlen(NLW_inst_m_axi_awlen_UNCONNECTED[15:0]),
        .m_axi_awlock(NLW_inst_m_axi_awlock_UNCONNECTED[1:0]),
        .m_axi_awprot(m_axi_awprot),
        .m_axi_awqos(NLW_inst_m_axi_awqos_UNCONNECTED[7:0]),
        .m_axi_awready(m_axi_awready),
        .m_axi_awregion(NLW_inst_m_axi_awregion_UNCONNECTED[7:0]),
        .m_axi_awsize(NLW_inst_m_axi_awsize_UNCONNECTED[5:0]),
        .m_axi_awuser(NLW_inst_m_axi_awuser_UNCONNECTED[1:0]),
        .m_axi_awvalid(m_axi_awvalid),
        .m_axi_bid({1'b0,1'b0}),
        .m_axi_bready(m_axi_bready),
        .m_axi_bresp(m_axi_bresp),
        .m_axi_buser({1'b0,1'b0}),
        .m_axi_bvalid(m_axi_bvalid),
        .m_axi_rdata(m_axi_rdata),
        .m_axi_rid({1'b0,1'b0}),
        .m_axi_rlast({1'b1,1'b1}),
        .m_axi_rready(m_axi_rready),
        .m_axi_rresp(m_axi_rresp),
        .m_axi_ruser({1'b0,1'b0}),
        .m_axi_rvalid(m_axi_rvalid),
        .m_axi_wdata(m_axi_wdata),
        .m_axi_wid(NLW_inst_m_axi_wid_UNCONNECTED[1:0]),
        .m_axi_wlast(NLW_inst_m_axi_wlast_UNCONNECTED[1:0]),
        .m_axi_wready(m_axi_wready),
        .m_axi_wstrb(m_axi_wstrb),
        .m_axi_wuser(NLW_inst_m_axi_wuser_UNCONNECTED[1:0]),
        .m_axi_wvalid(m_axi_wvalid),
        .s_axi_araddr(s_axi_araddr),
        .s_axi_arburst({1'b0,1'b0}),
        .s_axi_arcache({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arid(1'b0),
        .s_axi_arlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arlock(1'b0),
        .s_axi_arprot(s_axi_arprot),
        .s_axi_arqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arready(s_axi_arready),
        .s_axi_arsize({1'b0,1'b0,1'b0}),
        .s_axi_aruser(1'b0),
        .s_axi_arvalid(s_axi_arvalid),
        .s_axi_awaddr(s_axi_awaddr),
        .s_axi_awburst({1'b0,1'b0}),
        .s_axi_awcache({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awid(1'b0),
        .s_axi_awlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awlock(1'b0),
        .s_axi_awprot(s_axi_awprot),
        .s_axi_awqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(s_axi_awready),
        .s_axi_awsize({1'b0,1'b0,1'b0}),
        .s_axi_awuser(1'b0),
        .s_axi_awvalid(s_axi_awvalid),
        .s_axi_bid(NLW_inst_s_axi_bid_UNCONNECTED[0]),
        .s_axi_bready(s_axi_bready),
        .s_axi_bresp(s_axi_bresp),
        .s_axi_buser(NLW_inst_s_axi_buser_UNCONNECTED[0]),
        .s_axi_bvalid(s_axi_bvalid),
        .s_axi_rdata(s_axi_rdata),
        .s_axi_rid(NLW_inst_s_axi_rid_UNCONNECTED[0]),
        .s_axi_rlast(NLW_inst_s_axi_rlast_UNCONNECTED[0]),
        .s_axi_rready(s_axi_rready),
        .s_axi_rresp(s_axi_rresp),
        .s_axi_ruser(NLW_inst_s_axi_ruser_UNCONNECTED[0]),
        .s_axi_rvalid(s_axi_rvalid),
        .s_axi_wdata(s_axi_wdata),
        .s_axi_wid(1'b0),
        .s_axi_wlast(1'b1),
        .s_axi_wready(s_axi_wready),
        .s_axi_wstrb(s_axi_wstrb),
        .s_axi_wuser(1'b0),
        .s_axi_wvalid(s_axi_wvalid));
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
