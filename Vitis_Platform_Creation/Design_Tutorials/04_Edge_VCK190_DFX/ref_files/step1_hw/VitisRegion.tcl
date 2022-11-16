
################################################################
# This is a generated script based on design: VitisRegion
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2022.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source VitisRegion_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xcvc1902-vsva2197-2MP-e-S
   set_property BOARD_PART xilinx.com:vck190:part0:3.0 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name VitisRegion

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
xilinx.com:ip:axi_noc:1.0\
xilinx.com:ip:axi_register_slice:2.1\
xilinx.com:ip:proc_sys_reset:5.0\
xilinx.com:ip:ai_engine:2.0\
xilinx.com:ip:clk_wizard:1.0\
xilinx.com:ip:smartconnect:1.0\
xilinx.com:ip:axi_vip:1.1\
xilinx.com:ip:xlconcat:2.1\
"

   set list_ips_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2011 -severity "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2012 -severity "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

if { $bCheckIPsPassed != 1 } {
  common::send_gid_msg -ssname BD::TCL -id 2023 -severity "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################



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
  set AIE_CTRL_INI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:inimm_rtl:1.0 AIE_CTRL_INI ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $AIE_CTRL_INI

  set DDR_0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 -portmaps { \
   INTERNOC { physical_name DDR_0_internoc direction O } \
   } \
  DDR_0 ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {64} \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $DDR_0
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports DDR_0]
  set_property HDL_ATTRIBUTE.LOCKED {TRUE} [get_bd_intf_ports DDR_0]

  set DDR_1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR_1 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $DDR_1
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports DDR_1]

  set DDR_2 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR_2 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $DDR_2
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports DDR_2]

  set DDR_3 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR_3 ]
  set_property -dict [ list \
   CONFIG.COMPUTED_STRATEGY {load} \
   CONFIG.INI_STRATEGY {load} \
   ] $DDR_3
  set_property APERTURES {{0x0 2G} {0x8_0000_0000 6G}} [get_bd_intf_ports DDR_3]

  set PL_CTRL_S_AXI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 -portmaps { \
   ARADDR { physical_name PL_CTRL_S_AXI_araddr direction I left 43 right 0 } \
   ARBURST { physical_name PL_CTRL_S_AXI_arburst direction I left 1 right 0 } \
   ARCACHE { physical_name PL_CTRL_S_AXI_arcache direction I left 3 right 0 } \
   ARID { physical_name PL_CTRL_S_AXI_arid direction I left 15 right 0 } \
   ARLEN { physical_name PL_CTRL_S_AXI_arlen direction I left 7 right 0 } \
   ARLOCK { physical_name PL_CTRL_S_AXI_arlock direction I left 0 right 0 } \
   ARPROT { physical_name PL_CTRL_S_AXI_arprot direction I left 2 right 0 } \
   ARQOS { physical_name PL_CTRL_S_AXI_arqos direction I left 3 right 0 } \
   ARREADY { physical_name PL_CTRL_S_AXI_arready direction O } \
   ARSIZE { physical_name PL_CTRL_S_AXI_arsize direction I left 2 right 0 } \
   ARUSER { physical_name PL_CTRL_S_AXI_aruser direction I left 15 right 0 } \
   ARVALID { physical_name PL_CTRL_S_AXI_arvalid direction I } \
   AWADDR { physical_name PL_CTRL_S_AXI_awaddr direction I left 43 right 0 } \
   AWBURST { physical_name PL_CTRL_S_AXI_awburst direction I left 1 right 0 } \
   AWCACHE { physical_name PL_CTRL_S_AXI_awcache direction I left 3 right 0 } \
   AWID { physical_name PL_CTRL_S_AXI_awid direction I left 15 right 0 } \
   AWLEN { physical_name PL_CTRL_S_AXI_awlen direction I left 7 right 0 } \
   AWLOCK { physical_name PL_CTRL_S_AXI_awlock direction I left 0 right 0 } \
   AWPROT { physical_name PL_CTRL_S_AXI_awprot direction I left 2 right 0 } \
   AWQOS { physical_name PL_CTRL_S_AXI_awqos direction I left 3 right 0 } \
   AWREADY { physical_name PL_CTRL_S_AXI_awready direction O } \
   AWSIZE { physical_name PL_CTRL_S_AXI_awsize direction I left 2 right 0 } \
   AWUSER { physical_name PL_CTRL_S_AXI_awuser direction I left 15 right 0 } \
   AWVALID { physical_name PL_CTRL_S_AXI_awvalid direction I } \
   BID { physical_name PL_CTRL_S_AXI_bid direction O left 15 right 0 } \
   BREADY { physical_name PL_CTRL_S_AXI_bready direction I } \
   BRESP { physical_name PL_CTRL_S_AXI_bresp direction O left 1 right 0 } \
   BVALID { physical_name PL_CTRL_S_AXI_bvalid direction O } \
   RDATA { physical_name PL_CTRL_S_AXI_rdata direction O left 31 right 0 } \
   RID { physical_name PL_CTRL_S_AXI_rid direction O left 15 right 0 } \
   RLAST { physical_name PL_CTRL_S_AXI_rlast direction O } \
   RREADY { physical_name PL_CTRL_S_AXI_rready direction I } \
   RRESP { physical_name PL_CTRL_S_AXI_rresp direction O left 1 right 0 } \
   RUSER { physical_name PL_CTRL_S_AXI_ruser direction O left 3 right 0 } \
   RVALID { physical_name PL_CTRL_S_AXI_rvalid direction O } \
   WDATA { physical_name PL_CTRL_S_AXI_wdata direction I left 31 right 0 } \
   WLAST { physical_name PL_CTRL_S_AXI_wlast direction I } \
   WREADY { physical_name PL_CTRL_S_AXI_wready direction O } \
   WSTRB { physical_name PL_CTRL_S_AXI_wstrb direction I left 3 right 0 } \
   WUSER { physical_name PL_CTRL_S_AXI_wuser direction I left 3 right 0 } \
   WVALID { physical_name PL_CTRL_S_AXI_wvalid direction I } \
   } \
  PL_CTRL_S_AXI ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {44} \
   CONFIG.ARUSER_WIDTH {16} \
   CONFIG.AWUSER_WIDTH {16} \
   CONFIG.BUSER_WIDTH {0} \
   CONFIG.DATA_WIDTH {32} \
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
   CONFIG.INSERT_VIP {0} \
   CONFIG.MAX_BURST_LENGTH {256} \
   CONFIG.NUM_READ_OUTSTANDING {1} \
   CONFIG.NUM_READ_THREADS {1} \
   CONFIG.NUM_WRITE_OUTSTANDING {1} \
   CONFIG.NUM_WRITE_THREADS {1} \
   CONFIG.PROTOCOL {AXI4} \
   CONFIG.READ_WRITE_MODE {READ_WRITE} \
   CONFIG.RUSER_BITS_PER_BYTE {0} \
   CONFIG.RUSER_WIDTH {4} \
   CONFIG.SUPPORTS_NARROW_BURST {1} \
   CONFIG.WUSER_BITS_PER_BYTE {0} \
   CONFIG.WUSER_WIDTH {4} \
   ] $PL_CTRL_S_AXI
  set_property APERTURES {{0xA700_0000 144M}} [get_bd_intf_ports PL_CTRL_S_AXI]
  set_property HDL_ATTRIBUTE.LOCKED {TRUE} [get_bd_intf_ports PL_CTRL_S_AXI]


  # Create ports
  set ExtClk [ create_bd_port -dir I -type clk ExtClk ]
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {PL_CTRL_S_AXI} \
 ] $ExtClk
  set ExtReset [ create_bd_port -dir I -type rst ExtReset ]
  set Interrupt [ create_bd_port -dir O -from 31 -to 0 -type intr Interrupt ]

  # Create instance: ConfigNoC, and set properties
  set ConfigNoC [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 ConfigNoC ]
  set_property -dict [ list \
   CONFIG.NUM_NSI {1} \
   CONFIG.NUM_SI {0} \
 ] $ConfigNoC

  set_property -dict [ list \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins /ConfigNoC/M00_AXI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {M00_AXI { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /ConfigNoC/S00_INI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins /ConfigNoC/aclk0]

  # Create instance: DDRNoc, and set properties
  set DDRNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 DDRNoc ]
  set_property -dict [ list \
   CONFIG.NUM_CLKS {0} \
   CONFIG.NUM_MI {0} \
   CONFIG.NUM_NMI {4} \
   CONFIG.NUM_SI {0} \
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

  # Create instance: IsoRegDynamic, and set properties
  set IsoRegDynamic [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_register_slice:2.1 IsoRegDynamic ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {44} \
   CONFIG.ARUSER_WIDTH {16} \
   CONFIG.AWUSER_WIDTH {16} \
   CONFIG.BUSER_WIDTH {0} \
   CONFIG.DATA_WIDTH {32} \
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
   CONFIG.NUM_READ_OUTSTANDING {1} \
   CONFIG.NUM_READ_THREADS {1} \
   CONFIG.NUM_WRITE_OUTSTANDING {1} \
   CONFIG.NUM_WRITE_THREADS {1} \
   CONFIG.PROTOCOL {AXI4} \
   CONFIG.READ_WRITE_MODE {READ_WRITE} \
   CONFIG.REG_AR {0} \
   CONFIG.REG_AW {0} \
   CONFIG.REG_B {0} \
   CONFIG.REG_R {0} \
   CONFIG.REG_W {0} \
   CONFIG.RUSER_BITS_PER_BYTE {0} \
   CONFIG.RUSER_WIDTH {4} \
   CONFIG.SUPPORTS_NARROW_BURST {1} \
   CONFIG.WUSER_BITS_PER_BYTE {0} \
   CONFIG.WUSER_WIDTH {4} \
 ] $IsoRegDynamic

  # Create instance: IsoReset, and set properties
  set IsoReset [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 IsoReset ]

  # Create instance: ai_engine_0, and set properties
  set ai_engine_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ai_engine:2.0 ai_engine_0 ]

  set_property -dict [ list \
   CONFIG.CATEGORY {NOC} \
 ] [get_bd_intf_pins /ai_engine_0/S00_AXI]

  # Create instance: clk_wizard_0, and set properties
  set clk_wizard_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wizard:1.0 clk_wizard_0 ]
  set_property -dict [ list \
   CONFIG.JITTER_SEL {Min_O_Jitter} \
   CONFIG.RESET_TYPE {ACTIVE_LOW} \
   CONFIG.USE_LOCKED {true} \
   CONFIG.USE_PHASE_ALIGNMENT {true} \
   CONFIG.USE_RESET {true} \
 ] $clk_wizard_0

  # Create instance: icn_ctrl_1, and set properties
  set icn_ctrl_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 icn_ctrl_1 ]
  set_property -dict [ list \
   CONFIG.NUM_CLKS {2} \
   CONFIG.NUM_SI {1} \
 ] $icn_ctrl_1

  # Create instance: rst_clk_wizard_0_100M, and set properties
  set rst_clk_wizard_0_100M [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 rst_clk_wizard_0_100M ]

  # Create instance: to_delete_kernel_ctrl_0, and set properties
  set to_delete_kernel_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip:1.1 to_delete_kernel_ctrl_0 ]
  set_property -dict [ list \
   CONFIG.INTERFACE_MODE {SLAVE} \
 ] $to_delete_kernel_ctrl_0

  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property -dict [ list \
   CONFIG.NUM_PORTS {32} \
 ] $xlconcat_0

  # Create interface connections
  connect_bd_intf_net -intf_net ConfigNoC_M00_AXI [get_bd_intf_pins ConfigNoC/M00_AXI] [get_bd_intf_pins ai_engine_0/S00_AXI]
  connect_bd_intf_net -intf_net Conn2 [get_bd_intf_ports AIE_CTRL_INI] [get_bd_intf_pins ConfigNoC/S00_INI]
  connect_bd_intf_net -intf_net DDRNoc_M00_INI [get_bd_intf_ports DDR_0] [get_bd_intf_pins DDRNoc/M00_INI]
  connect_bd_intf_net -intf_net DDRNoc_M01_INI [get_bd_intf_ports DDR_1] [get_bd_intf_pins DDRNoc/M01_INI]
  connect_bd_intf_net -intf_net DDRNoc_M02_INI [get_bd_intf_ports DDR_2] [get_bd_intf_pins DDRNoc/M02_INI]
  connect_bd_intf_net -intf_net DDRNoc_M03_INI [get_bd_intf_ports DDR_3] [get_bd_intf_pins DDRNoc/M03_INI]
  connect_bd_intf_net -intf_net IsoRegDynamic_M_AXI [get_bd_intf_pins IsoRegDynamic/M_AXI] [get_bd_intf_pins icn_ctrl_1/S00_AXI]
  connect_bd_intf_net -intf_net S_AXI_0_1 [get_bd_intf_ports PL_CTRL_S_AXI] [get_bd_intf_pins IsoRegDynamic/S_AXI]
  connect_bd_intf_net -intf_net icn_ctrl_1_M00_AXI [get_bd_intf_pins icn_ctrl_1/M00_AXI] [get_bd_intf_pins to_delete_kernel_ctrl_0/S_AXI]

  # Create port connections
  connect_bd_net -net IsoReset_peripheral_aresetn [get_bd_pins IsoRegDynamic/aresetn] [get_bd_pins IsoReset/peripheral_aresetn]
  connect_bd_net -net ai_engine_0_s00_axi_aclk [get_bd_pins ConfigNoC/aclk0] [get_bd_pins ai_engine_0/s00_axi_aclk]
  connect_bd_net -net clk_in1_1 [get_bd_ports ExtClk] [get_bd_pins IsoRegDynamic/aclk] [get_bd_pins IsoReset/slowest_sync_clk] [get_bd_pins clk_wizard_0/clk_in1] [get_bd_pins icn_ctrl_1/aclk1]
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins clk_wizard_0/clk_out1] [get_bd_pins icn_ctrl_1/aclk] [get_bd_pins rst_clk_wizard_0_100M/slowest_sync_clk] [get_bd_pins to_delete_kernel_ctrl_0/aclk]
  connect_bd_net -net clk_wizard_0_locked [get_bd_pins clk_wizard_0/locked] [get_bd_pins rst_clk_wizard_0_100M/dcm_locked]
  connect_bd_net -net resetn_1 [get_bd_ports ExtReset] [get_bd_pins IsoReset/ext_reset_in] [get_bd_pins clk_wizard_0/resetn] [get_bd_pins rst_clk_wizard_0_100M/ext_reset_in]
  connect_bd_net -net rst_clk_wizard_0_100M_peripheral_aresetn [get_bd_pins icn_ctrl_1/aresetn] [get_bd_pins rst_clk_wizard_0_100M/peripheral_aresetn] [get_bd_pins to_delete_kernel_ctrl_0/aresetn]
  connect_bd_net -net xlconcat_0_dout [get_bd_ports Interrupt] [get_bd_pins xlconcat_0/dout]

  # Create address segments
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces AIE_CTRL_INI] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xA7000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces PL_CTRL_S_AXI] [get_bd_addr_segs to_delete_kernel_ctrl_0/S_AXI/Reg] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  # Create PFM attributes
  set_property PFM_NAME {xilinx.com:xd:VCK190_Custom_DFX:1.0} [get_files [current_bd_design].bd]
  set_property PFM.AXI_PORT {S00_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S01_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S02_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S03_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S04_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S05_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S06_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S07_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S08_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"} S09_AXI {memport "S_AXI_NOC" sptag "" memory "" is_range "true"}} [get_bd_cells /DDRNoc]
  set_property PFM.CLOCK {clk_out1 {id "0" is_default "true" proc_sys_reset "/rst_clk_wizard_0_100M" status "fixed" freq_hz "100000000"}} [get_bd_cells /clk_wizard_0]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M02_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M03_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M04_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M05_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M06_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M07_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M08_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M09_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} M10_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"}} [get_bd_cells /icn_ctrl_1]
#  set_property PFM.IRQ {In0 {is_range "true"} In1 {is_range "true"} In2 {is_range "true"} In3 {is_range "true"} In4 {is_range "true"} In5 {is_range "true"} In6 {is_range "true"} In7 {is_range "true"} In8 {is_range "true"} In9 {is_range "true"} In10 {is_range "true"} In11 {is_range "true"} In12 {is_range "true"} In13 {is_range "true"} In14 {is_range "true"} In15 {is_range "true"} In16 {is_range "true"} In17 {is_range "true"} In18 {is_range "true"} In19 {is_range "true"} In20 {is_range "true"} In21 {is_range "true"} In22 {is_range "true"} In23 {is_range "true"} In24 {is_range "true"} In25 {is_range "true"} In26 {is_range "true"} In27 {is_range "true"} In28 {is_range "true"} In29 {is_range "true"} In30 {is_range "true"} In31 {is_range "true"}} [get_bd_cells /xlconcat_0]
  set_property PFM.IRQ {In0 {id 0} In1 {id 1} In2 {id 2} In3 {id 3} In4 {id 4} In5 {id 5} In6 {id 6} In7 {id 7} In8 {id 8} In9 {id 9}  In10 {id 10} In11 {id 11} In12 {id 12} In13 {id 13} In14 {id 14} In15 {id 15} In16 {id 16} In17 {id 17} In18 {id 18} In19 {id 19}  In20 {id 20} In21 {id 21} In22 {id 22} In23 {id 23} In24 {id 24} In25 {id 25} In26 {id 26} In27 {id 27} In28 {id 28} In29 {id 29}  In30 {id 30} In31 {id 31}} [get_bd_cells /xlconcat_0]

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


