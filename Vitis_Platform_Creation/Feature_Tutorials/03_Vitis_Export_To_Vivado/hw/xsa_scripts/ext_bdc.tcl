#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************
#
# To test this script, run the following commands from Vivado Tcl console:
# source ext_bdc.tcl

# CHANGE DESIGN NAME HERE
variable design_name
set design_name ext_bdc


##################################################################
# DESIGN PROCs
##################################################################


# Hierarchical cell: axi_smc_vip_hier
proc create_hier_cell_axi_smc_vip_hier { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_axi_smc_vip_hier() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S00_AXI

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M00_AXI

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M01_AXI


  # Create pins
  create_bd_pin -dir I -type clk aclk
  create_bd_pin -dir I -type rst aresetn
  create_bd_pin -dir I -type clk clk_in1

  # Create instance: to_delete_kernel_ctrl_0, and set properties
  set to_delete_kernel_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip:1.1 to_delete_kernel_ctrl_0 ]
  set_property CONFIG.INTERFACE_MODE {SLAVE} $to_delete_kernel_ctrl_0


  # Create instance: to_delete_kernel_ctrl_1, and set properties
  set to_delete_kernel_ctrl_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip:1.1 to_delete_kernel_ctrl_1 ]
  set_property CONFIG.INTERFACE_MODE {SLAVE} $to_delete_kernel_ctrl_1


  # Create instance: to_delete_kernel_ctrl_2, and set properties
  set to_delete_kernel_ctrl_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip:1.1 to_delete_kernel_ctrl_2 ]
  set_property CONFIG.INTERFACE_MODE {SLAVE} $to_delete_kernel_ctrl_2


  # Create instance: to_delete_kernel_ctrl_3, and set properties
  set to_delete_kernel_ctrl_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip:1.1 to_delete_kernel_ctrl_3 ]
  set_property CONFIG.INTERFACE_MODE {SLAVE} $to_delete_kernel_ctrl_3


  # Create instance: icn_ctrl, and set properties
  set icn_ctrl [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 icn_ctrl ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {2} \
    CONFIG.NUM_MI {6} \
    CONFIG.NUM_SI {1} \
  ] $icn_ctrl


  # Create instance: icn_ctrl_0, and set properties
  set icn_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 icn_ctrl_0 ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_SI {1} \
  ] $icn_ctrl_0


  # Create instance: icn_ctrl_1, and set properties
  set icn_ctrl_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 icn_ctrl_1 ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_SI {1} \
  ] $icn_ctrl_1


  # Create instance: icn_ctrl_2, and set properties
  set icn_ctrl_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 icn_ctrl_2 ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_SI {1} \
  ] $icn_ctrl_2


  # Create instance: icn_ctrl_3, and set properties
  set icn_ctrl_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 icn_ctrl_3 ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_SI {1} \
  ] $icn_ctrl_3


  # Create interface connections
  connect_bd_intf_net -intf_net CIPS_0_M_AXI_GP0 [get_bd_intf_pins S00_AXI] [get_bd_intf_pins icn_ctrl/S00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_0_M00_AXI [get_bd_intf_pins to_delete_kernel_ctrl_0/S_AXI] [get_bd_intf_pins icn_ctrl_0/M00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_1_M00_AXI [get_bd_intf_pins to_delete_kernel_ctrl_1/S_AXI] [get_bd_intf_pins icn_ctrl_1/M00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_2_M00_AXI [get_bd_intf_pins to_delete_kernel_ctrl_2/S_AXI] [get_bd_intf_pins icn_ctrl_2/M00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_3_M00_AXI [get_bd_intf_pins to_delete_kernel_ctrl_3/S_AXI] [get_bd_intf_pins icn_ctrl_3/M00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M00_AXI [get_bd_intf_pins M00_AXI] [get_bd_intf_pins icn_ctrl/M00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M01_AXI [get_bd_intf_pins M01_AXI] [get_bd_intf_pins icn_ctrl/M01_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M02_AXI [get_bd_intf_pins icn_ctrl/M02_AXI] [get_bd_intf_pins icn_ctrl_0/S00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M03_AXI [get_bd_intf_pins icn_ctrl/M03_AXI] [get_bd_intf_pins icn_ctrl_1/S00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M04_AXI [get_bd_intf_pins icn_ctrl/M04_AXI] [get_bd_intf_pins icn_ctrl_2/S00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M05_AXI [get_bd_intf_pins icn_ctrl/M05_AXI] [get_bd_intf_pins icn_ctrl_3/S00_AXI]

  # Create port connections
  connect_bd_net -net clk_in1_1 [get_bd_pins clk_in1] [get_bd_pins icn_ctrl/aclk]
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins aclk] [get_bd_pins icn_ctrl_0/aclk] [get_bd_pins icn_ctrl_1/aclk] [get_bd_pins to_delete_kernel_ctrl_1/aclk] [get_bd_pins icn_ctrl_2/aclk] [get_bd_pins to_delete_kernel_ctrl_2/aclk] [get_bd_pins icn_ctrl_3/aclk] [get_bd_pins to_delete_kernel_ctrl_3/aclk] [get_bd_pins icn_ctrl/aclk1] [get_bd_pins to_delete_kernel_ctrl_0/aclk]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins aresetn] [get_bd_pins icn_ctrl_0/aresetn] [get_bd_pins to_delete_kernel_ctrl_1/aresetn] [get_bd_pins icn_ctrl_1/aresetn] [get_bd_pins to_delete_kernel_ctrl_2/aresetn] [get_bd_pins icn_ctrl_2/aresetn] [get_bd_pins to_delete_kernel_ctrl_3/aresetn] [get_bd_pins icn_ctrl_3/aresetn] [get_bd_pins icn_ctrl/aresetn] [get_bd_pins to_delete_kernel_ctrl_0/aresetn]

  # Restore current instance
  current_bd_instance $oldCurInst
}


# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set M00_INI [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M00_INI ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M00_INI
  set_property APERTURES {{0x500_0000_0000 8G}} [get_bd_intf_ports M00_INI]

  set M00_INI1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M00_INI1 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M00_INI1
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports M00_INI1]

  set M01_INI [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M01_INI ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M01_INI
  set_property APERTURES {{0x500_0000_0000 8G}} [get_bd_intf_ports M01_INI]

  set M01_INI1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M01_INI1 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M01_INI1
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports M01_INI1]

  set M02_INI [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M02_INI ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M02_INI
  set_property APERTURES {{0x500_0000_0000 8G}} [get_bd_intf_ports M02_INI]

  set M02_INI1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M02_INI1 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M02_INI1
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports M02_INI1]

  set M03_INI [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M03_INI ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M03_INI
  set_property APERTURES {{0x500_0000_0000 8G}} [get_bd_intf_ports M03_INI]

  set M03_INI1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 M03_INI1 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $M03_INI1
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports M03_INI1]

  set S00_AXI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S00_AXI ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {44} \
   CONFIG.ARUSER_WIDTH {16} \
   CONFIG.AWUSER_WIDTH {16} \
   CONFIG.BUSER_WIDTH {0} \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.HAS_BRESP {1} \
   CONFIG.HAS_BURST {1} \
   CONFIG.HAS_CACHE {1} \
   CONFIG.HAS_LOCK {1} \
   CONFIG.HAS_PROT {1} \
   CONFIG.HAS_QOS {1} \
   CONFIG.HAS_REGION {0} \
   CONFIG.HAS_RRESP {1} \
   CONFIG.HAS_WSTRB {1} \
   CONFIG.ID_WIDTH {16} \
   CONFIG.MAX_BURST_LENGTH {256} \
   CONFIG.NUM_READ_OUTSTANDING {2} \
   CONFIG.NUM_READ_THREADS {1} \
   CONFIG.NUM_WRITE_OUTSTANDING {2} \
   CONFIG.NUM_WRITE_THREADS {1} \
   CONFIG.PROTOCOL {AXI4} \
   CONFIG.READ_WRITE_MODE {READ_WRITE} \
   CONFIG.RUSER_BITS_PER_BYTE {0} \
   CONFIG.RUSER_WIDTH {0} \
   CONFIG.SUPPORTS_NARROW_BURST {1} \
   CONFIG.WUSER_BITS_PER_BYTE {0} \
   CONFIG.WUSER_WIDTH {0} \
   ] $S00_AXI
  set_property APERTURES {{0xA400_0000 144M}} [get_bd_intf_ports S00_AXI]

  set S00_INI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:inimm_rtl:1.0 S00_INI ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $S00_INI


  # Create ports
  set clk_in1 [ create_bd_port -dir I -type clk clk_in1 ]
  set ext_reset_in [ create_bd_port -dir I -type rst ext_reset_in ]
  set irq [ create_bd_port -dir O -type intr irq ]

  # Create instance: ConfigNoc, and set properties
  set ConfigNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 ConfigNoc ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_NSI {1} \
    CONFIG.NUM_SI {0} \
  ] $ConfigNoc


  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins /ConfigNoc/M00_AXI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {M00_AXI {read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}}} \
 ] [get_bd_intf_pins /ConfigNoc/S00_INI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins /ConfigNoc/aclk0]

  # Create instance: ai_engine_0, and set properties
  set ai_engine_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ai_engine:2.0 ai_engine_0 ]
  set_property -dict [list \
    CONFIG.CLK_NAMES {} \
    CONFIG.NAME_MI_AXI {} \
    CONFIG.NAME_MI_AXIS {} \
    CONFIG.NAME_SI_AXI {S00_AXI,} \
    CONFIG.NAME_SI_AXIS {} \
    CONFIG.NUM_CLKS {0} \
    CONFIG.NUM_MI_AXI {0} \
    CONFIG.NUM_MI_AXIS {0} \
    CONFIG.NUM_SI_AXI {1} \
    CONFIG.NUM_SI_AXIS {0} \
  ] $ai_engine_0


  set_property -dict [ list \
   CONFIG.CATEGORY {NOC} \
 ] [get_bd_intf_pins /ai_engine_0/S00_AXI]

  # Create instance: DDRNoc, and set properties
  set DDRNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 DDRNoc ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NMI {4} \
    CONFIG.NUM_SI {4} \
  ] $DDRNoc


  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /DDRNoc/M00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /DDRNoc/M01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /DDRNoc/M02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /DDRNoc/M03_INI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /DDRNoc/S00_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M01_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /DDRNoc/S01_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M02_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /DDRNoc/S02_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M03_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /DDRNoc/S03_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI:S01_AXI:S02_AXI:S03_AXI} \
 ] [get_bd_pins /DDRNoc/aclk0]

  # Create instance: LPDDRNoc, and set properties
  set LPDDRNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 LPDDRNoc ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NMI {4} \
    CONFIG.NUM_SI {4} \
  ] $LPDDRNoc


  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /LPDDRNoc/M00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /LPDDRNoc/M01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /LPDDRNoc/M02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /LPDDRNoc/M03_INI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /LPDDRNoc/S00_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M01_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /LPDDRNoc/S01_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M02_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /LPDDRNoc/S02_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M03_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /LPDDRNoc/S03_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI:S01_AXI:S02_AXI:S03_AXI} \
 ] [get_bd_pins /LPDDRNoc/aclk0]

  # Create instance: clk_wizard_0, and set properties
  set clk_wizard_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wizard:1.0 clk_wizard_0 ]
  set_property -dict [list \
    CONFIG.CLKOUT_DRIVES {BUFG,BUFG,BUFG,BUFG,BUFG,BUFG,BUFG} \
    CONFIG.CLKOUT_DYN_PS {None,None,None,None,None,None,None} \
    CONFIG.CLKOUT_MATCHED_ROUTING {false,false,false,false,false,false,false} \
    CONFIG.CLKOUT_PORT {clk_out1,clk_out2,clk_out3,clk_out4,clk_out5,clk_out6,clk_out7} \
    CONFIG.CLKOUT_REQUESTED_DUTY_CYCLE {50.000,50.000,50.000,50.000,50.000,50.000,50.000} \
    CONFIG.CLKOUT_REQUESTED_OUT_FREQUENCY {104.167,156.25,312.5,78.125,208.33,416.67,625} \
    CONFIG.CLKOUT_REQUESTED_PHASE {0.000,0.000,0.000,0.000,0.000,0.000,0.000} \
    CONFIG.CLKOUT_USED {true,true,true,true,true,true,true} \
    CONFIG.JITTER_SEL {Min_O_Jitter} \
    CONFIG.PRIM_SOURCE {No_buffer} \
    CONFIG.RESET_TYPE {ACTIVE_LOW} \
    CONFIG.USE_LOCKED {true} \
    CONFIG.USE_PHASE_ALIGNMENT {true} \
    CONFIG.USE_RESET {true} \
  ] $clk_wizard_0


  # Create instance: axi_intc_cascaded_1, and set properties
  set axi_intc_cascaded_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:4.1 axi_intc_cascaded_1 ]
  set_property -dict [list \
    CONFIG.C_ASYNC_INTR {0xFFFFFFFF} \
    CONFIG.C_IRQ_CONNECTION {1} \
  ] $axi_intc_cascaded_1


  # Create instance: axi_intc_parent, and set properties
  set axi_intc_parent [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:4.1 axi_intc_parent ]
  set_property -dict [list \
    CONFIG.C_ASYNC_INTR {0xFFFFFFFF} \
    CONFIG.C_CASCADE_MASTER {1} \
    CONFIG.C_EN_CASCADE_MODE {1} \
    CONFIG.C_IRQ_CONNECTION {1} \
  ] $axi_intc_parent


  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property -dict [list \
    CONFIG.IN0_WIDTH {1} \
    CONFIG.NUM_PORTS {32} \
  ] $xlconcat_0


  # Create instance: psr_104mhz, and set properties
  set psr_104mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_104mhz ]

  # Create instance: psr_156mhz, and set properties
  set psr_156mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_156mhz ]

  # Create instance: psr_312mhz, and set properties
  set psr_312mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_312mhz ]

  # Create instance: psr_78mhz, and set properties
  set psr_78mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_78mhz ]

  # Create instance: psr_208mhz, and set properties
  set psr_208mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_208mhz ]

  # Create instance: psr_416mhz, and set properties
  set psr_416mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_416mhz ]

  # Create instance: psr_625mhz, and set properties
  set psr_625mhz [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 psr_625mhz ]

  # Create instance: axi_smc_vip_hier
  create_hier_cell_axi_smc_vip_hier [current_bd_instance .] axi_smc_vip_hier

  # Create interface connections
  connect_bd_intf_net -intf_net CIPS_0_M_AXI_GP0 [get_bd_intf_ports S00_AXI] [get_bd_intf_pins axi_smc_vip_hier/S00_AXI]
  connect_bd_intf_net -intf_net ConfigNoc_M00_AXI [get_bd_intf_pins ConfigNoc/M00_AXI] [get_bd_intf_pins ai_engine_0/S00_AXI]
  connect_bd_intf_net -intf_net DDRNoc_M00_INI [get_bd_intf_ports M00_INI1] [get_bd_intf_pins DDRNoc/M00_INI]
  connect_bd_intf_net -intf_net DDRNoc_M01_INI [get_bd_intf_ports M01_INI1] [get_bd_intf_pins DDRNoc/M01_INI]
  connect_bd_intf_net -intf_net DDRNoc_M02_INI [get_bd_intf_ports M02_INI1] [get_bd_intf_pins DDRNoc/M02_INI]
  connect_bd_intf_net -intf_net DDRNoc_M03_INI [get_bd_intf_ports M03_INI1] [get_bd_intf_pins DDRNoc/M03_INI]
  connect_bd_intf_net -intf_net LPDDRNoc_M00_INI [get_bd_intf_ports M00_INI] [get_bd_intf_pins LPDDRNoc/M00_INI]
  connect_bd_intf_net -intf_net LPDDRNoc_M01_INI [get_bd_intf_ports M01_INI] [get_bd_intf_pins LPDDRNoc/M01_INI]
  connect_bd_intf_net -intf_net LPDDRNoc_M02_INI [get_bd_intf_ports M02_INI] [get_bd_intf_pins LPDDRNoc/M02_INI]
  connect_bd_intf_net -intf_net LPDDRNoc_M03_INI [get_bd_intf_ports M03_INI] [get_bd_intf_pins LPDDRNoc/M03_INI]
  connect_bd_intf_net -intf_net cips_noc_M08_INI [get_bd_intf_ports S00_INI] [get_bd_intf_pins ConfigNoc/S00_INI]
  connect_bd_intf_net -intf_net icn_ctrl_M00_AXI [get_bd_intf_pins axi_intc_cascaded_1/s_axi] [get_bd_intf_pins axi_smc_vip_hier/M00_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_M01_AXI [get_bd_intf_pins axi_intc_parent/s_axi] [get_bd_intf_pins axi_smc_vip_hier/M01_AXI]

  # Create port connections
  connect_bd_net -net CIPS_0_pl_resetn1 [get_bd_ports ext_reset_in] [get_bd_pins psr_104mhz/ext_reset_in] [get_bd_pins psr_156mhz/ext_reset_in] [get_bd_pins psr_312mhz/ext_reset_in] [get_bd_pins psr_78mhz/ext_reset_in] [get_bd_pins psr_208mhz/ext_reset_in] [get_bd_pins psr_416mhz/ext_reset_in] [get_bd_pins psr_625mhz/ext_reset_in] [get_bd_pins clk_wizard_0/resetn]
  connect_bd_net -net ai_engine_0_s00_axi_aclk [get_bd_pins ai_engine_0/s00_axi_aclk] [get_bd_pins ConfigNoc/aclk0]
  connect_bd_net -net axi_intc_cascaded_1_irq [get_bd_pins axi_intc_cascaded_1/irq] [get_bd_pins xlconcat_0/In31]
  connect_bd_net -net axi_intc_parent_irq [get_bd_pins axi_intc_parent/irq] [get_bd_ports irq]
  connect_bd_net -net clk_in1_1 [get_bd_ports clk_in1] [get_bd_pins clk_wizard_0/clk_in1] [get_bd_pins LPDDRNoc/aclk0] [get_bd_pins DDRNoc/aclk0] [get_bd_pins axi_smc_vip_hier/clk_in1]
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins clk_wizard_0/clk_out1] [get_bd_pins psr_104mhz/slowest_sync_clk] [get_bd_pins axi_intc_cascaded_1/s_axi_aclk] [get_bd_pins axi_intc_parent/s_axi_aclk] [get_bd_pins axi_smc_vip_hier/aclk]
  connect_bd_net -net clk_wizard_0_clk_out2 [get_bd_pins clk_wizard_0/clk_out2] [get_bd_pins psr_156mhz/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out3 [get_bd_pins clk_wizard_0/clk_out3] [get_bd_pins psr_312mhz/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out4 [get_bd_pins clk_wizard_0/clk_out4] [get_bd_pins psr_78mhz/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out5 [get_bd_pins clk_wizard_0/clk_out5] [get_bd_pins psr_208mhz/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out6 [get_bd_pins clk_wizard_0/clk_out6] [get_bd_pins psr_416mhz/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out7 [get_bd_pins clk_wizard_0/clk_out7] [get_bd_pins psr_625mhz/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_locked [get_bd_pins clk_wizard_0/locked] [get_bd_pins psr_104mhz/dcm_locked] [get_bd_pins psr_156mhz/dcm_locked] [get_bd_pins psr_312mhz/dcm_locked] [get_bd_pins psr_78mhz/dcm_locked] [get_bd_pins psr_208mhz/dcm_locked] [get_bd_pins psr_416mhz/dcm_locked] [get_bd_pins psr_625mhz/dcm_locked]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins psr_104mhz/peripheral_aresetn] [get_bd_pins axi_intc_cascaded_1/s_axi_aresetn] [get_bd_pins axi_intc_parent/s_axi_aresetn] [get_bd_pins axi_smc_vip_hier/aresetn]
  connect_bd_net -net xlconcat_0_dout [get_bd_pins xlconcat_0/dout] [get_bd_pins axi_intc_parent/intr]

  # Create address segments
  assign_bd_address -offset 0xA4000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_intc_cascaded_1/S_AXI/Reg] -force
  assign_bd_address -offset 0xA5000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_intc_parent/S_AXI/Reg] -force
  assign_bd_address -offset 0xA6000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_smc_vip_hier/to_delete_kernel_ctrl_0/S_AXI/Reg] -force
  assign_bd_address -offset 0xA7000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_smc_vip_hier/to_delete_kernel_ctrl_1/S_AXI/Reg] -force
  assign_bd_address -offset 0xA8000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_smc_vip_hier/to_delete_kernel_ctrl_2/S_AXI/Reg] -force
  assign_bd_address -offset 0xA9000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_smc_vip_hier/to_delete_kernel_ctrl_3/S_AXI/Reg] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces S00_INI] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""




