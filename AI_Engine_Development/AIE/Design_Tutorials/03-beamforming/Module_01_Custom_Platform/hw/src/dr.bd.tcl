#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

################################################################
# This is a generated script based on design: vck190_v1_0
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
set scripts_vivado_version 2023.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   common::send_gid_msg -ssname BD::TCL -id 2040 -severity "WARNING" "This script was generated using Vivado <$scripts_vivado_version> without IP versions in the create_bd_cell commands, but is now being run in <$current_vivado_version> of Vivado. There may have been major IP version changes between Vivado <$scripts_vivado_version> and <$current_vivado_version>, which could impact the parameter settings of the IPs."

}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source vck190_v1_0_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xcvc1902-vsva2197-2MP-e-S
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name vck190_v1_0

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
xilinx.com:ip:ai_engine:*\
xilinx.com:ip:axi_dbg_hub:*\
xilinx.com:ip:clk_gen_sim:*\
xilinx.com:ip:smartconnect:*\
xilinx.com:ip:axi_vip:*\
xilinx.com:ip:proc_sys_reset:*\
xilinx.com:ip:clk_wizard:*\
xilinx.com:ip:versal_cips:*\
xilinx.com:ip:axi_noc:*\
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
  set SYS_CLK1_IN_0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 SYS_CLK1_IN_0 ]

  set ddr4_dimm1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddr4_rtl:1.0 ddr4_dimm1 ]


  # Create ports

  # Create instance: ai_engine_0, and set properties
  set ai_engine_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ai_engine ai_engine_0 ]
  set_property -dict [ list \
   CONFIG.CLK_NAMES {} \
   CONFIG.FIFO_TYPE_MI_AXIS {} \
   CONFIG.FIFO_TYPE_SI_AXIS {} \
   CONFIG.NAME_MI_AXIS {} \
   CONFIG.NAME_SI_AXIS {} \
   CONFIG.NUM_CLKS {0} \
   CONFIG.NUM_MI_AXI {0} \
   CONFIG.NUM_MI_AXIS {0} \
   CONFIG.NUM_SI_AXIS {0} \
   CONFIG.is_npi_enable {false} \
 ] $ai_engine_0

  set_property -dict [ list \
   CONFIG.CATEGORY {NOC} \
 ] [get_bd_intf_pins /ai_engine_0/S00_AXI]

  # Create instance: axi_dbg_hub_0, and set properties
  set axi_dbg_hub_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dbg_hub axi_dbg_hub_0 ]
  set_property -dict [ list \
   CONFIG.C_AXI_ADDR_WIDTH {44} \
   CONFIG.C_AXI_DATA_WIDTH {128} \
   CONFIG.C_AXI_ID_WIDTH {16} \
   CONFIG.C_NUM_DEBUG_CORES {0} \
 ] $axi_dbg_hub_0

  # Create instance: clk_gen_sim_0, and set properties
  set clk_gen_sim_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_gen_sim clk_gen_sim_0 ]
  set_property -dict [ list \
   CONFIG.USER_AXI_CLK_0_FREQ {300} \
   CONFIG.USER_AXI_CLK_1_FREQ {300} \
   CONFIG.USER_AXI_CLK_2_FREQ {300} \
   CONFIG.USER_AXI_CLK_3_FREQ {300} \
   CONFIG.USER_AXI_CLK_4_FREQ {300} \
   CONFIG.USER_AXI_CLK_5_FREQ {300} \
   CONFIG.USER_AXI_CLK_6_FREQ {300} \
   CONFIG.USER_AXI_CLK_7_FREQ {300} \
   CONFIG.USER_AXI_CLK_8_FREQ {300} \
   CONFIG.USER_AXI_CLK_9_FREQ {300} \
   CONFIG.USER_NUM_OF_AXI_CLK {0} \
   CONFIG.USER_NUM_OF_SYS_CLK {1} \
   CONFIG.USER_SYS_CLK0_FREQ {200.000} \
   CONFIG.USER_SYS_CLK1_FREQ {200.000} \
   CONFIG.USER_SYS_CLK2_FREQ {201.694} \
 ] $clk_gen_sim_0

  # Create instance: ctrl_sm, and set properties
  set ctrl_sm [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {16} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm

  # Create instance: ctrl_sm_0, and set properties
  set ctrl_sm_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_0 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_0

  # Create instance: ctrl_sm_0_tie, and set properties
  set ctrl_sm_0_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_0_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_0_tie

  # Create instance: ctrl_sm_1, and set properties
  set ctrl_sm_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_1 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_1

  # Create instance: ctrl_sm_3, and set properties
  set ctrl_sm_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_3 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_3

  # Create instance: ctrl_sm_4, and set properties
  set ctrl_sm_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_4 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_4

  # Create instance: ctrl_sm_5, and set properties
  set ctrl_sm_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_5 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_5

  # Create instance: ctrl_sm_6, and set properties
  set ctrl_sm_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_6 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_6

  # Create instance: ctrl_sm_7, and set properties
  set ctrl_sm_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_7 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_7

  # Create instance: ctrl_sm_10, and set properties
  set ctrl_sm_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_10 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_10

  # Create instance: ctrl_sm_10_tie, and set properties
  set ctrl_sm_10_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_10_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_10_tie

  # Create instance: ctrl_sm_11, and set properties
  set ctrl_sm_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_11 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_11

  # Create instance: ctrl_sm_11_tie, and set properties
  set ctrl_sm_11_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_11_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_11_tie

  # Create instance: ctrl_sm_2, and set properties
  set ctrl_sm_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_2 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_2

  # Create instance: ctrl_sm_12, and set properties
  set ctrl_sm_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_12 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_12

  # Create instance: ctrl_sm_12_tie, and set properties
  set ctrl_sm_12_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_12_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_12_tie

  # Create instance: ctrl_sm_13, and set properties
  set ctrl_sm_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_13 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_13

  # Create instance: ctrl_sm_13_tie, and set properties
  set ctrl_sm_13_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_13_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_13_tie

  # Create instance: ctrl_sm_14, and set properties
  set ctrl_sm_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_14 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_14

  # Create instance: ctrl_sm_14_tie, and set properties
  set ctrl_sm_14_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_14_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_14_tie

  # Create instance: ctrl_sm_15, and set properties
  set ctrl_sm_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_15 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_15

  # Create instance: ctrl_sm_15_tie, and set properties
  set ctrl_sm_15_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_15_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_15_tie

  # Create instance: ctrl_sm_1_tie, and set properties
  set ctrl_sm_1_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_1_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_1_tie

  # Create instance: ctrl_sm_2_tie, and set properties
  set ctrl_sm_2_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_2_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_2_tie

  # Create instance: ctrl_sm_3_tie, and set properties
  set ctrl_sm_3_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_3_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_3_tie

  # Create instance: ctrl_sm_4_tie, and set properties
  set ctrl_sm_4_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_4_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_4_tie

  # Create instance: ctrl_sm_5_tie, and set properties
  set ctrl_sm_5_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_5_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_5_tie

  # Create instance: ctrl_sm_6_tie, and set properties
  set ctrl_sm_6_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_6_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_6_tie

  # Create instance: ctrl_sm_7_tie, and set properties
  set ctrl_sm_7_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_7_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_7_tie

  # Create instance: ctrl_sm_8, and set properties
  set ctrl_sm_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_8 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_8

  # Create instance: ctrl_sm_8_tie, and set properties
  set ctrl_sm_8_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_8_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_8_tie

  # Create instance: ctrl_sm_9, and set properties
  set ctrl_sm_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_9 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_9

  # Create instance: ctrl_sm_9_tie, and set properties
  set ctrl_sm_9_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_9_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_9_tie

  # Create instance: pl_clk_out1_rst, and set properties
  set pl_clk_out1_rst [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset pl_clk_out1_rst ]

  # Create instance: pl_clk_out2_rst, and set properties
  set pl_clk_out2_rst [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset pl_clk_out2_rst ]

  # Create instance: pl_clk_out3_rst, and set properties
  set pl_clk_out3_rst [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset pl_clk_out3_rst ]

  # Create instance: pl_clk_out4_rst, and set properties
  set pl_clk_out4_rst [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset pl_clk_out4_rst ]

  # Create instance: pl_clk_out5_rst, and set properties
  set pl_clk_out5_rst [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset pl_clk_out5_rst ]

  # Create instance: pl_clk_out6_rst, and set properties
  set pl_clk_out6_rst [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset pl_clk_out6_rst ]

  # Create instance: pl_clkwiz, and set properties
  set pl_clkwiz [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wizard pl_clkwiz ]
  set_property -dict [ list \
   CONFIG.CLKFBOUT_MULT {80.000000} \
   CONFIG.CLKOUT1_DIVIDE {40.000000} \
   CONFIG.CLKOUT2_DIVIDE {16.000000} \
   CONFIG.CLKOUT3_DIVIDE {8.000000} \
   CONFIG.CLKOUT4_DIVIDE {10.000000} \
   CONFIG.CLKOUT5_DIVIDE {9.000000} \
   CONFIG.CLKOUT6_DIVIDE {16.000000} \
   CONFIG.CLKOUT_DRIVES {BUFG,BUFG,BUFG,BUFG,BUFG,BUFG,BUFG} \
   CONFIG.CLKOUT_DYN_PS {None,None,None,None,None,None,None} \
   CONFIG.CLKOUT_MATCHED_ROUTING {false,false,false,false,false,false,false} \
   CONFIG.CLKOUT_PORT {clk_out1,clk_out2,clk_out3,clk_out4,clk_out5,clk_out6,clk_out7} \
   CONFIG.CLKOUT_REQUESTED_DUTY_CYCLE {50.000,50.000,50.000,50.000,50.000,50.000,50.000} \
   CONFIG.CLKOUT_REQUESTED_OUT_FREQUENCY {100,250,500,400,450,250,100.000} \
   CONFIG.CLKOUT_REQUESTED_PHASE {0.000,0.000,0.000,0.000,0.000,0.000,0.000} \
   CONFIG.CLKOUT_USED {true,true,true,true,true,true,false} \
   CONFIG.SECONDARY_IN_FREQ {54.000} \
   CONFIG.USE_LOCKED {true} \
 ] $pl_clkwiz

  # Create instance: ps_cips, and set properties
  set ps_cips [ create_bd_cell -type ip -vlnv xilinx.com:ip:versal_cips ps_cips ]
  set_property -dict [ list \
   CONFIG.CPM_CONFIG {\
     PS_HSDP_MODE {NONE}\
     GT_REFCLK_MHZ {156.25}\
     PS_HSDP0_REFCLK {0}\
     PS_HSDP1_REFCLK {0}\
     AURORA_LINE_RATE_GPBS {10.0}\
     PS_HSDP_INGRESS_TRAFFIC {JTAG}\
     PS_HSDP_EGRESS_TRAFFIC {JTAG}\
     PS_USE_NOC_PS_PCI_0 {0}\
     PS_USE_PS_NOC_PCI_0 {0}\
     PS_USE_PS_NOC_PCI_1 {0}\
     CPM_CLRERR_LANE_MARGIN {0}\
     CPM_DMA_CREDIT_INIT_DEMUX {1}\
     BOOT_SECONDARY_PCIE_ENABLE {0}\
     CPM_CDO_EN {0}\
     CPM_USE_MODES {None}\
     CPM_DESIGN_USE_MODE {0}\
     CPM_PERIPHERAL_EN {0}\
     CPM_PERIPHERAL_TEST_EN {0}\
     CPM_REQ_AGENTS_0_ENABLE {0}\
     CPM_REQ_AGENTS_1_ENABLE {0}\
     CPM_REQ_AGENTS_0_L2_ENABLE {0}\
     CPM_NUM_HNF_AGENTS {0}\
     CPM_NUM_REQ_AGENTS {0}\
     CPM_NUM_SLAVE_AGENTS {0}\
     CPM_NUM_HOME_OR_SLAVE_AGENTS {0}\
     CPM_SELECT_GTOUTCLK {TXOUTCLK}\
     CPM_DMA_IS_MM_ONLY {0}\
     CPM_CCIX_IS_MM_ONLY {0}\
     CPM_TYPE1_MEMBASE_MEMLIMIT_ENABLE {Disabled}\
     CPM_TYPE1_PREFETCHABLE_MEMBASE_MEMLIMIT {Disabled}\
     CPM_A0_REFCLK {0}\
     CPM_A1_REFCLK {0}\
     CPM_CPLL_CTRL_FBDIV {86}\
     CPM_CPLL_CTRL_SRCSEL {REF_CLK}\
     CPM_CORE_REF_CTRL_ACT_FREQMHZ {716.659485}\
     CPM_AUX0_REF_CTRL_ACT_FREQMHZ {716.659485}\
     CPM_AUX1_REF_CTRL_ACT_FREQMHZ {716.659485}\
     CPM_DBG_REF_CTRL_ACT_FREQMHZ {358.329742}\
     CPM_LSBUS_REF_CTRL_ACT_FREQMHZ {143.331894}\
     CPM_CORE_REF_CTRL_FREQMHZ {717}\
     CPM_AUX0_REF_CTRL_FREQMHZ {717}\
     CPM_AUX1_REF_CTRL_FREQMHZ {717}\
     CPM_DBG_REF_CTRL_FREQMHZ {359}\
     CPM_LSBUS_REF_CTRL_FREQMHZ {150}\
     CPM_CORE_REF_CTRL_DIVISOR0 {2}\
     CPM_AUX0_REF_CTRL_DIVISOR0 {2}\
     CPM_AUX1_REF_CTRL_DIVISOR0 {2}\
     CPM_DBG_REF_CTRL_DIVISOR0 {4}\
     CPM_LSBUS_REF_CTRL_DIVISOR0 {10}\
     CPM_AXI_SLV_XDMA_BASE_ADDRR_L {0x11000000}\
     CPM_AXI_SLV_MULTQ_BASE_ADDRR_L {0x10000000}\
     CPM_AXI_SLV_BRIDGE_BASE_ADDRR_L {0x00000000}\
     CPM_AXI_SLV_XDMA_BASE_ADDRR_H {0x00000006}\
     CPM_AXI_SLV_MULTQ_BASE_ADDRR_H {0x00000006}\
     CPM_AXI_SLV_BRIDGE_BASE_ADDRR_H {0x00000006}\
     CPM_NUM_CCIX_CREDIT_LINKS {0}\
     CPM_CCIX_SELECT_AGENT {None}\
     CPM_CCIX_PORT_AGGREGATION_ENABLE {0}\
     CPM_CCIX_PARTIAL_CACHELINE_SUPPORT {0}\
     CPM_PCIE0_CCIX_EN {0}\
     CPM_PCIE1_CCIX_EN {0}\
     CPM_PCIE0_CCIX_VENDOR_ID {0}\
     CPM_PCIE1_CCIX_VENDOR_ID {0}\
     CPM_PCIE0_CCIX_OPT_TLP_GEN_AND_RECEPT_EN_CONTROL_INTERNAL {0}\
     CPM_PCIE1_CCIX_OPT_TLP_GEN_AND_RECEPT_EN_CONTROL_INTERNAL {0}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_0 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_0 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_0 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_0 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_0 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_0 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_1 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_1 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_1 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_1 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_1 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_1 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_2 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_2 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_2 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_2 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_2 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_2 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_3 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_3 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_3 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_3 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_3 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_3 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_4 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_4 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_4 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_4 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_4 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_4 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_5 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_5 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_5 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_5 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_5 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_5 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_6 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_6 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_6 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_6 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_6 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_6 {Normal_Non_Cacheable_Memory}\
     CPM_CCIX_RSVRD_MEMORY_BASEADDRESS_7 {0x00000000}\
     CPM_CCIX_RSVRD_MEMORY_REGION_7 {0}\
     CPM_CCIX_RSVRD_MEMORY_SIZE_7 {4GB}\
     CPM_CCIX_RSVRD_MEMORY_AGENT_TYPE_7 {HA0}\
     CPM_CCIX_RSVRD_MEMORY_TYPE_7 {Other_or_Non_Specified_Memory_Type}\
     CPM_CCIX_RSVRD_MEMORY_ATTRIB_7 {Normal_Non_Cacheable_Memory}\
     CPM_XPIPE_0_MODE {0}\
     CPM_XPIPE_1_MODE {0}\
     CPM_XPIPE_2_MODE {0}\
     CPM_XPIPE_3_MODE {0}\
     CPM_XPIPE_0_RSVD {0}\
     CPM_XPIPE_1_RSVD {0}\
     CPM_XPIPE_2_RSVD {0}\
     CPM_XPIPE_3_RSVD {0}\
     CPM_XPIPE_0_LOC {QUAD0}\
     CPM_XPIPE_1_LOC {QUAD1}\
     CPM_XPIPE_2_LOC {QUAD2}\
     CPM_XPIPE_3_LOC {QUAD3}\
     CPM_XPIPE_0_INSTANTIATED {0}\
     CPM_XPIPE_1_INSTANTIATED {0}\
     CPM_XPIPE_2_INSTANTIATED {0}\
     CPM_XPIPE_3_INSTANTIATED {0}\
     CPM_XPIPE_0_CLK_CFG {0}\
     CPM_XPIPE_1_CLK_CFG {0}\
     CPM_XPIPE_2_CLK_CFG {0}\
     CPM_XPIPE_3_CLK_CFG {0}\
     CPM_XPIPE_0_CLKDLY_CFG {0}\
     CPM_XPIPE_1_CLKDLY_CFG {0}\
     CPM_XPIPE_2_CLKDLY_CFG {0}\
     CPM_XPIPE_3_CLKDLY_CFG {0}\
     CPM_XPIPE_0_REG_CFG {0}\
     CPM_XPIPE_1_REG_CFG {0}\
     CPM_XPIPE_2_REG_CFG {0}\
     CPM_XPIPE_3_REG_CFG {0}\
     CPM_XPIPE_0_LINK0_CFG {DISABLE}\
     CPM_XPIPE_1_LINK0_CFG {DISABLE}\
     CPM_XPIPE_2_LINK0_CFG {DISABLE}\
     CPM_XPIPE_3_LINK0_CFG {DISABLE}\
     CPM_XPIPE_0_LINK1_CFG {DISABLE}\
     CPM_XPIPE_1_LINK1_CFG {DISABLE}\
     CPM_XPIPE_2_LINK1_CFG {DISABLE}\
     CPM_XPIPE_3_LINK1_CFG {DISABLE}\
     CPM_PCIE0_CFG_SPEC_4_0 {0}\
     CPM_PCIE1_CFG_SPEC_4_0 {0}\
     CPM_PCIE0_CFG_VEND_ID {10EE}\
     CPM_PCIE1_CFG_VEND_ID {10EE}\
     CPM_PCIE0_LINK_DEBUG_EN {0}\
     CPM_PCIE1_LINK_DEBUG_EN {0}\
     CPM_PCIE0_LINK_DEBUG_AXIST_EN {0}\
     CPM_PCIE1_LINK_DEBUG_AXIST_EN {0}\
     CPM_PCIE0_PL_USER_SPARE {0}\
     CPM_PCIE1_PL_USER_SPARE {0}\
     CPM_PCIE0_PL_UPSTREAM_FACING {1}\
     CPM_PCIE1_PL_UPSTREAM_FACING {1}\
     CPM_PCIE0_ASYNC_MODE {SRNS}\
     CPM_PCIE1_ASYNC_MODE {SRNS}\
     CPM_PCIE0_AXIBAR_NUM {1}\
     CPM_PCIE1_AXIBAR_NUM {1}\
     CPM_PCIE0_PORT_TYPE {PCI_Express_Endpoint_device}\
     CPM_PCIE1_PORT_TYPE {PCI_Express_Endpoint_device}\
     CPM_PCIE0_FUNCTIONAL_MODE {None}\
     CPM_PCIE1_FUNCTIONAL_MODE {None}\
     CPM_PCIE0_MODES {None}\
     CPM_PCIE1_MODES {None}\
     CPM_PCIE0_MODE_SELECTION {Basic}\
     CPM_PCIE1_MODE_SELECTION {Basic}\
     CPM_PCIE0_EXT_PCIE_CFG_SPACE_ENABLED {None}\
     CPM_PCIE1_EXT_PCIE_CFG_SPACE_ENABLED {None}\
     CPM_PCIE0_LEGACY_EXT_PCIE_CFG_SPACE_ENABLED {0}\
     CPM_PCIE1_LEGACY_EXT_PCIE_CFG_SPACE_ENABLED {0}\
     CPM_PCIE0_TL_PF_ENABLE_REG {1}\
     CPM_PCIE1_TL_PF_ENABLE_REG {1}\
     CPM_PCIE0_TL_USER_SPARE {0}\
     CPM_PCIE1_TL_USER_SPARE {0}\
     CPM_PCIE0_EN_PARITY {0}\
     CPM_PCIE1_EN_PARITY {0}\
     CPM_PCIE0_EDR_LINK_SPEED {None}\
     CPM_PCIE1_EDR_LINK_SPEED {None}\
     CPM_PCIE0_CONTROLLER_ENABLE {0}\
     CPM_PCIE1_CONTROLLER_ENABLE {0}\
     CPM_PCIE0_TL2CFG_IF_PARITY_CHK {0}\
     CPM_PCIE1_TL2CFG_IF_PARITY_CHK {0}\
     CPM_PCIE0_TYPE1_MEMBASE_MEMLIMIT_ENABLE {Disabled}\
     CPM_PCIE1_TYPE1_MEMBASE_MEMLIMIT_ENABLE {Disabled}\
     CPM_PCIE0_TYPE1_PREFETCHABLE_MEMBASE_MEMLIMIT {Disabled}\
     CPM_PCIE1_TYPE1_PREFETCHABLE_MEMBASE_MEMLIMIT {Disabled}\
     CPM_PCIE0_TYPE1_MEMBASE_MEMLIMIT_BRIDGE_ENABLE {Disabled}\
     CPM_PCIE0_TYPE1_PREFETCHABLE_MEMBASE_BRIDGE_MEMLIMIT {Disabled}\
     CPM_PCIE0_USER_CLK2_FREQ {125_MHz}\
     CPM_PCIE1_USER_CLK2_FREQ {125_MHz}\
     CPM_PCIE0_USER_EDR_CLK2_FREQ {312.5_MHz}\
     CPM_PCIE1_USER_EDR_CLK2_FREQ {312.5_MHz}\
     CPM_PCIE0_CORE_CLK_FREQ {500}\
     CPM_PCIE1_CORE_CLK_FREQ {500}\
     CPM_PCIE0_CORE_EDR_CLK_FREQ {625}\
     CPM_PCIE1_CORE_EDR_CLK_FREQ {625}\
     CPM_PCIE0_REF_CLK_FREQ {100_MHz}\
     CPM_PCIE1_REF_CLK_FREQ {100_MHz}\
     CPM_PCIE0_USER_CLK_FREQ {125_MHz}\
     CPM_PCIE1_USER_CLK_FREQ {125_MHz}\
     CPM_PCIE0_USER_EDR_CLK_FREQ {312.5_MHz}\
     CPM_PCIE1_USER_EDR_CLK_FREQ {312.5_MHz}\
     CPM_PCIE_CHANNELS_FOR_POWER {0}\
     CPM_PCIE0_MODE0_FOR_POWER {NONE}\
     CPM_PCIE0_LINK_WIDTH0_FOR_POWER {0}\
     CPM_PCIE0_LINK_SPEED0_FOR_POWER {GEN1}\
     CPM_PCIE1_MODE1_FOR_POWER {NONE}\
     CPM_PCIE1_LINK_WIDTH1_FOR_POWER {0}\
     CPM_PCIE1_LINK_SPEED1_FOR_POWER {GEN1}\
     CPM_PCIE0_PM_ASPML0S_TIMEOUT {0}\
     CPM_PCIE1_PM_ASPML0S_TIMEOUT {0}\
     CPM_PCIE0_PF0_PM_CSR_NOSOFTRESET {1}\
     CPM_PCIE1_PF0_PM_CSR_NOSOFTRESET {1}\
     CPM_PCIE0_PF0_PM_CAP_SUPP_D1_STATE {1}\
     CPM_PCIE1_PF0_PM_CAP_SUPP_D1_STATE {1}\
     CPM_PCIE0_PF0_LINK_CAP_ASPM_SUPPORT {No_ASPM}\
     CPM_PCIE1_PF0_LINK_CAP_ASPM_SUPPORT {No_ASPM}\
     CPM_PCIE0_PM_ENABLE_L23_ENTRY {0}\
     CPM_PCIE1_PM_ENABLE_L23_ENTRY {0}\
     CPM_PCIE0_PM_ENABLE_SLOT_POWER_CAPTURE {1}\
     CPM_PCIE1_PM_ENABLE_SLOT_POWER_CAPTURE {1}\
     CPM_PCIE0_PF0_PM_CAP_PMESUPPORT_D0 {1}\
     CPM_PCIE1_PF0_PM_CAP_PMESUPPORT_D0 {1}\
     CPM_PCIE0_PF0_PM_CAP_PMESUPPORT_D1 {1}\
     CPM_PCIE1_PF0_PM_CAP_PMESUPPORT_D1 {1}\
     CPM_PCIE0_PF0_PM_CAP_PMESUPPORT_D3COLD {1}\
     CPM_PCIE1_PF0_PM_CAP_PMESUPPORT_D3COLD {1}\
     CPM_PCIE0_PF0_PM_CAP_PMESUPPORT_D3HOT {1}\
     CPM_PCIE1_PF0_PM_CAP_PMESUPPORT_D3HOT {1}\
     CPM_PCIE0_PM_L1_REENTRY_DELAY {0}\
     CPM_PCIE1_PM_L1_REENTRY_DELAY {0}\
     CPM_PCIE0_PM_ASPML1_ENTRY_DELAY {0}\
     CPM_PCIE1_PM_ASPML1_ENTRY_DELAY {0}\
     CPM_PCIE0_PM_PME_TURNOFF_ACK_DELAY {0}\
     CPM_PCIE1_PM_PME_TURNOFF_ACK_DELAY {0}\
     CPM_PCIE0_PL_LINK_CAP_MAX_LINK_WIDTH {NONE}\
     CPM_PCIE1_PL_LINK_CAP_MAX_LINK_WIDTH {NONE}\
     CPM_PCIE0_PL_LINK_CAP_MAX_LINK_SPEED {Gen3}\
     CPM_PCIE1_PL_LINK_CAP_MAX_LINK_SPEED {Gen3}\
     CPM_PCIE0_MAX_LINK_SPEED {2.5_GT/s}\
     CPM_PCIE1_MAX_LINK_SPEED {2.5_GT/s}\
     CPM_PCIE0_LANE_REVERSAL_EN {0}\
     CPM_PCIE1_LANE_REVERSAL_EN {0}\
     CPM_PCIE0_MAILBOX_ENABLE {0}\
     CPM_PCIE0_PF0_CLASS_CODE {0}\
     CPM_PCIE1_PF0_CLASS_CODE {58000}\
     CPM_PCIE0_PF1_CLASS_CODE {0x000}\
     CPM_PCIE1_PF1_CLASS_CODE {0x000}\
     CPM_PCIE0_PF2_CLASS_CODE {0x000}\
     CPM_PCIE1_PF2_CLASS_CODE {0x000}\
     CPM_PCIE0_PF3_CLASS_CODE {0x000}\
     CPM_PCIE1_PF3_CLASS_CODE {0x000}\
     CPM_PCIE0_PF0_SUB_CLASS_VALUE {80}\
     CPM_PCIE1_PF0_SUB_CLASS_VALUE {80}\
     CPM_PCIE0_PF1_SUB_CLASS_VALUE {80}\
     CPM_PCIE1_PF1_SUB_CLASS_VALUE {80}\
     CPM_PCIE0_PF2_SUB_CLASS_VALUE {80}\
     CPM_PCIE1_PF2_SUB_CLASS_VALUE {80}\
     CPM_PCIE0_PF3_SUB_CLASS_VALUE {80}\
     CPM_PCIE1_PF3_SUB_CLASS_VALUE {80}\
     CPM_PCIE0_PF0_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE1_PF0_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE0_PF1_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE1_PF1_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE0_PF2_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE1_PF2_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE0_PF3_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE1_PF3_SUB_CLASS_INTF_MENU {Other_memory_controller}\
     CPM_PCIE0_PF0_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE1_PF0_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE0_PF1_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE1_PF1_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE0_PF2_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE1_PF2_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE0_PF3_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE1_PF3_BASE_CLASS_MENU {Memory_controller}\
     CPM_PCIE0_PF0_BASE_CLASS_VALUE {05}\
     CPM_PCIE1_PF0_BASE_CLASS_VALUE {05}\
     CPM_PCIE0_PF1_BASE_CLASS_VALUE {05}\
     CPM_PCIE1_PF1_BASE_CLASS_VALUE {05}\
     CPM_PCIE0_PF2_BASE_CLASS_VALUE {05}\
     CPM_PCIE1_PF2_BASE_CLASS_VALUE {05}\
     CPM_PCIE0_PF3_BASE_CLASS_VALUE {05}\
     CPM_PCIE1_PF3_BASE_CLASS_VALUE {05}\
     CPM_PCIE0_PF0_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE1_PF0_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE0_PF1_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE1_PF1_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE0_PF2_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE1_PF2_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE0_PF3_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE1_PF3_USE_CLASS_CODE_LOOKUP_ASSISTANT {1}\
     CPM_PCIE0_PF0_DEV_CAP_ENDPOINT_L0S_LATENCY {less_than_64ns}\
     CPM_PCIE1_PF0_DEV_CAP_ENDPOINT_L0S_LATENCY {less_than_64ns}\
     CPM_PCIE0_PF0_DEV_CAP_ENDPOINT_L1S_LATENCY {less_than_1us}\
     CPM_PCIE1_PF0_DEV_CAP_ENDPOINT_L1S_LATENCY {less_than_1us}\
     CPM_PCIE0_PF0_DEV_CAP_EXT_TAG_EN {0}\
     CPM_PCIE1_PF0_DEV_CAP_EXT_TAG_EN {0}\
     CPM_PCIE0_PF0_DEV_CAP_10B_TAG_EN {0}\
     CPM_PCIE1_PF0_DEV_CAP_10B_TAG_EN {0}\
     CPM_PCIE0_PF0_DEV_CAP_FUNCTION_LEVEL_RESET_CAPABLE {0}\
     CPM_PCIE1_PF0_DEV_CAP_FUNCTION_LEVEL_RESET_CAPABLE {0}\
     CPM_PCIE0_PF0_DEV_CAP_MAX_PAYLOAD {1024_bytes}\
     CPM_PCIE1_PF0_DEV_CAP_MAX_PAYLOAD {1024_bytes}\
     CPM_PCIE0_PF1_VEND_ID {10EE}\
     CPM_PCIE1_PF1_VEND_ID {10EE}\
     CPM_PCIE0_PF2_VEND_ID {10EE}\
     CPM_PCIE1_PF2_VEND_ID {10EE}\
     CPM_PCIE0_PF3_VEND_ID {10EE}\
     CPM_PCIE1_PF3_VEND_ID {10EE}\
     CPM_PCIE0_PF0_CFG_DEV_ID {B03F}\
     CPM_PCIE1_PF0_CFG_DEV_ID {B034}\
     CPM_PCIE0_PF1_CFG_DEV_ID {B13F}\
     CPM_PCIE1_PF1_CFG_DEV_ID {B134}\
     CPM_PCIE0_PF2_CFG_DEV_ID {B23F}\
     CPM_PCIE1_PF2_CFG_DEV_ID {B234}\
     CPM_PCIE0_PF3_CFG_DEV_ID {B33F}\
     CPM_PCIE1_PF3_CFG_DEV_ID {B334}\
     CPM_PCIE0_PF0_CFG_REV_ID {0}\
     CPM_PCIE1_PF0_CFG_REV_ID {0}\
     CPM_PCIE0_PF1_CFG_REV_ID {0}\
     CPM_PCIE1_PF1_CFG_REV_ID {0}\
     CPM_PCIE0_PF2_CFG_REV_ID {0}\
     CPM_PCIE1_PF2_CFG_REV_ID {0}\
     CPM_PCIE0_PF3_CFG_REV_ID {0}\
     CPM_PCIE1_PF3_CFG_REV_ID {0}\
     CPM_PCIE0_PF0_CFG_SUBSYS_ID {7}\
     CPM_PCIE1_PF0_CFG_SUBSYS_ID {7}\
     CPM_PCIE0_PF1_CFG_SUBSYS_ID {7}\
     CPM_PCIE1_PF1_CFG_SUBSYS_ID {7}\
     CPM_PCIE0_PF2_CFG_SUBSYS_ID {7}\
     CPM_PCIE1_PF2_CFG_SUBSYS_ID {7}\
     CPM_PCIE0_PF3_CFG_SUBSYS_ID {7}\
     CPM_PCIE1_PF3_CFG_SUBSYS_ID {7}\
     CPM_PCIE0_PF0_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE1_PF0_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE0_PF1_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE1_PF1_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE0_PF2_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE1_PF2_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE0_PF3_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE1_PF3_CFG_SUBSYS_VEND_ID {10EE}\
     CPM_PCIE0_PF0_LINK_STATUS_SLOT_CLOCK_CONFIG {1}\
     CPM_PCIE1_PF0_LINK_STATUS_SLOT_CLOCK_CONFIG {1}\
     CPM_PCIE0_PF0_INTERFACE_VALUE {0}\
     CPM_PCIE1_PF0_INTERFACE_VALUE {0}\
     CPM_PCIE0_PF1_INTERFACE_VALUE {00}\
     CPM_PCIE1_PF1_INTERFACE_VALUE {00}\
     CPM_PCIE0_PF2_INTERFACE_VALUE {00}\
     CPM_PCIE1_PF2_INTERFACE_VALUE {00}\
     CPM_PCIE0_PF3_INTERFACE_VALUE {00}\
     CPM_PCIE1_PF3_INTERFACE_VALUE {00}\
     CPM_PCIE0_PF0_CAPABILITY_POINTER {80}\
     CPM_PCIE1_PF0_CAPABILITY_POINTER {80}\
     CPM_PCIE0_PF1_CAPABILITY_POINTER {80}\
     CPM_PCIE1_PF1_CAPABILITY_POINTER {80}\
     CPM_PCIE0_PF2_CAPABILITY_POINTER {80}\
     CPM_PCIE1_PF2_CAPABILITY_POINTER {80}\
     CPM_PCIE0_PF3_CAPABILITY_POINTER {80}\
     CPM_PCIE1_PF3_CAPABILITY_POINTER {80}\
     CPM_PCIE0_PF0_INTERRUPT_PIN {NONE}\
     CPM_PCIE1_PF0_INTERRUPT_PIN {NONE}\
     CPM_PCIE0_PF1_INTERRUPT_PIN {NONE}\
     CPM_PCIE1_PF1_INTERRUPT_PIN {NONE}\
     CPM_PCIE0_PF2_INTERRUPT_PIN {NONE}\
     CPM_PCIE1_PF2_INTERRUPT_PIN {NONE}\
     CPM_PCIE0_PF3_INTERRUPT_PIN {NONE}\
     CPM_PCIE1_PF3_INTERRUPT_PIN {NONE}\
     CPM_PCIE0_VC1_BASE_DISABLE {0}\
     CPM_PCIE1_VC1_BASE_DISABLE {0}\
     CPM_PCIE0_VC0_CAPABILITY_POINTER {80}\
     CPM_PCIE1_VC0_CAPABILITY_POINTER {80}\
     CPM_PCIE0_PF0_VC_EXTENDED_COUNT {0}\
     CPM_PCIE1_PF0_VC_EXTENDED_COUNT {0}\
     CPM_PCIE0_PF0_VC_LOW_PRIORITY_EXTENDED_COUNT {0}\
     CPM_PCIE1_PF0_VC_LOW_PRIORITY_EXTENDED_COUNT {0}\
     CPM_PCIE0_PF0_VC_ARB_CAPABILITY {0}\
     CPM_PCIE1_PF0_VC_ARB_CAPABILITY {0}\
     CPM_PCIE0_PF0_VC_ARB_TBL_OFFSET {0}\
     CPM_PCIE1_PF0_VC_ARB_TBL_OFFSET {0}\
     CPM_PCIE0_BRIDGE_AXI_SLAVE_IF {0}\
     CPM_PCIE0_XDMA_AXILITE_SLAVE_IF {0}\
     CPM_PCIE0_EDR_IF {0}\
     CPM_PCIE1_EDR_IF {0}\
     CPM_PCIE0_PASID_IF {0}\
     CPM_PCIE1_PASID_IF {0}\
     CPM_PCIE0_TX_FC_IF {0}\
     CPM_PCIE1_TX_FC_IF {0}\
     CPM_PCIE0_MESG_RSVD_IF {0}\
     CPM_PCIE1_MESG_RSVD_IF {0}\
     CPM_PCIE0_MESG_TRANSMIT_IF {0}\
     CPM_PCIE1_MESG_TRANSMIT_IF {0}\
     CPM_PCIE0_CFG_STS_IF {0}\
     CPM_PCIE1_CFG_STS_IF {0}\
     CPM_PCIE0_CFG_CTL_IF {0}\
     CPM_PCIE1_CFG_CTL_IF {0}\
     CPM_PCIE0_CFG_FC_IF {0}\
     CPM_PCIE1_CFG_FC_IF {0}\
     CPM_PCIE0_CFG_EXT_IF {0}\
     CPM_PCIE1_CFG_EXT_IF {0}\
     CPM_PCIE0_CFG_MGMT_IF {0}\
     CPM_PCIE1_CFG_MGMT_IF {0}\
     CPM_PCIE0_COPY_XDMA_PF0_ENABLED {0}\
     CPM_PCIE0_COPY_PF0_QDMA_ENABLED {1}\
     CPM_PCIE0_COPY_PF0_SRIOV_QDMA_ENABLED {1}\
     CPM_PCIE0_COPY_PF0_ENABLED {0}\
     CPM_PCIE1_COPY_PF0_ENABLED {0}\
     CPM_PCIE0_COPY_SRIOV_PF0_ENABLED {1}\
     CPM_PCIE1_COPY_SRIOV_PF0_ENABLED {1}\
     CPM_PCIE0_PF0_BAR0_BRIDGE_64BIT {0}\
     CPM_PCIE0_PF0_BAR2_BRIDGE_64BIT {0}\
     CPM_PCIE0_PF0_BAR4_BRIDGE_64BIT {0}\
     CPM_PCIE0_PF0_BAR0_BRIDGE_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR2_BRIDGE_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR4_BRIDGE_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR0_BRIDGE_ENABLED {0}\
     CPM_PCIE0_PF0_BAR1_BRIDGE_ENABLED {0}\
     CPM_PCIE0_PF0_BAR2_BRIDGE_ENABLED {0}\
     CPM_PCIE0_PF0_BAR3_BRIDGE_ENABLED {0}\
     CPM_PCIE0_PF0_BAR4_BRIDGE_ENABLED {0}\
     CPM_PCIE0_PF0_BAR5_BRIDGE_ENABLED {0}\
     CPM_PCIE0_PF0_BAR0_BRIDGE_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR1_BRIDGE_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR2_BRIDGE_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR3_BRIDGE_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR4_BRIDGE_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR5_BRIDGE_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR0_BRIDGE_SIZE {4}\
     CPM_PCIE0_PF0_BAR1_BRIDGE_SIZE {4}\
     CPM_PCIE0_PF0_BAR2_BRIDGE_SIZE {4}\
     CPM_PCIE0_PF0_BAR3_BRIDGE_SIZE {4}\
     CPM_PCIE0_PF0_BAR4_BRIDGE_SIZE {4}\
     CPM_PCIE0_PF0_BAR5_BRIDGE_SIZE {4}\
     CPM_PCIE0_PF0_BAR0_BRIDGE_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR1_BRIDGE_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR2_BRIDGE_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR3_BRIDGE_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR4_BRIDGE_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR5_BRIDGE_TYPE {Memory}\
     CPM_PCIE0_PF0_XDMA_64BIT {0}\
     CPM_PCIE0_PF1_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR0_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR0_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR0_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR0_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR1_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR1_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR1_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR1_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR2_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR2_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR2_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR2_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR3_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR3_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR3_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR3_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR4_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR4_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR4_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR4_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR5_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR5_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR5_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR5_XDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR0_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR0_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR0_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR0_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR1_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR1_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR1_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR1_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR2_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR2_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR2_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR2_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR3_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR3_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR3_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR3_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR4_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR4_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR4_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR4_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_BAR5_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF1_BAR5_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF2_BAR5_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF3_BAR5_QDMA_AXCACHE {0}\
     CPM_PCIE0_PF0_XDMA_ENABLED {0}\
     CPM_PCIE0_PF1_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_EXPANSION_ROM_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_EXPANSION_ROM_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_EXPANSION_ROM_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_EXPANSION_ROM_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_EXPANSION_ROM_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_EXPANSION_ROM_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_EXPANSION_ROM_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_EXPANSION_ROM_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_XDMA_SIZE {128}\
     CPM_PCIE0_PF1_XDMA_SIZE {128}\
     CPM_PCIE0_PF2_XDMA_SIZE {128}\
     CPM_PCIE0_PF3_XDMA_SIZE {128}\
     CPM_PCIE0_PF0_EXPANSION_ROM_QDMA_SIZE {2}\
     CPM_PCIE0_PF1_EXPANSION_ROM_QDMA_SIZE {2}\
     CPM_PCIE0_PF2_EXPANSION_ROM_QDMA_SIZE {2}\
     CPM_PCIE0_PF3_EXPANSION_ROM_QDMA_SIZE {2}\
     CPM_PCIE0_PF0_BAR0_QDMA_64BIT {1}\
     CPM_PCIE0_PF1_BAR0_QDMA_64BIT {1}\
     CPM_PCIE0_PF2_BAR0_QDMA_64BIT {1}\
     CPM_PCIE0_PF3_BAR0_QDMA_64BIT {1}\
     CPM_PCIE0_PF0_BAR1_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR1_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR1_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR1_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR2_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR2_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR2_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR2_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR3_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR3_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR3_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR3_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR4_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR4_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR4_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR4_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR5_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR5_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR5_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR5_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR0_XDMA_64BIT {1}\
     CPM_PCIE0_PF1_BAR0_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR0_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR0_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR1_XDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR1_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR1_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR1_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR2_XDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR2_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR2_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR2_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR3_XDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR3_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR3_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR3_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR4_XDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR4_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR4_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR4_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR5_XDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR5_XDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR5_XDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR5_XDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR0_SRIOV_QDMA_64BIT {1}\
     CPM_PCIE0_PF1_BAR0_SRIOV_QDMA_64BIT {1}\
     CPM_PCIE0_PF2_BAR0_SRIOV_QDMA_64BIT {1}\
     CPM_PCIE0_PF3_BAR0_SRIOV_QDMA_64BIT {1}\
     CPM_PCIE0_PF0_BAR1_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR1_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR1_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR1_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR2_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR2_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR2_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR2_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR3_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR3_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR3_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR3_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR4_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR4_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR4_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR4_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR5_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF1_BAR5_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF2_BAR5_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF3_BAR5_SRIOV_QDMA_64BIT {0}\
     CPM_PCIE0_PF0_BAR0_QDMA_ENABLED {1}\
     CPM_PCIE0_PF1_BAR0_QDMA_ENABLED {1}\
     CPM_PCIE0_PF2_BAR0_QDMA_ENABLED {1}\
     CPM_PCIE0_PF3_BAR0_QDMA_ENABLED {1}\
     CPM_PCIE0_PF0_BAR1_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR1_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR1_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR1_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR2_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR2_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR2_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR2_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR3_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR3_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR3_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR3_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR4_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR4_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR4_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR4_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR5_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR5_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR5_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR5_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR0_XDMA_ENABLED {1}\
     CPM_PCIE0_PF1_BAR0_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR0_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR0_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR1_XDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR1_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR1_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR1_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR2_XDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR2_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR2_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR2_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR3_XDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR3_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR3_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR3_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR4_XDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR4_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR4_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR4_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR5_XDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR5_XDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR5_XDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR5_XDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR0_SRIOV_QDMA_ENABLED {1}\
     CPM_PCIE0_PF1_BAR0_SRIOV_QDMA_ENABLED {1}\
     CPM_PCIE0_PF2_BAR0_SRIOV_QDMA_ENABLED {1}\
     CPM_PCIE0_PF3_BAR0_SRIOV_QDMA_ENABLED {1}\
     CPM_PCIE0_PF0_BAR1_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR1_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR1_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR1_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR2_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR2_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR2_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR2_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR3_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR3_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR3_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR3_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR4_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR4_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR4_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR4_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR5_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF1_BAR5_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF2_BAR5_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF3_BAR5_SRIOV_QDMA_ENABLED {0}\
     CPM_PCIE0_PF0_BAR0_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF1_BAR0_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF2_BAR0_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF3_BAR0_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF0_BAR1_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR1_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR1_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR1_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR2_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR2_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR2_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR2_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR3_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR3_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR3_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR3_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR4_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR4_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR4_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR4_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR5_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR5_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR5_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR5_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR0_XDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF1_BAR0_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR0_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR0_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR1_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR1_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR1_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR1_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR2_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR2_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR2_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR2_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR3_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR3_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR3_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR3_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR4_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR4_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR4_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR4_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR5_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR5_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR5_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR5_XDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR0_SRIOV_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF1_BAR0_SRIOV_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF2_BAR0_SRIOV_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF3_BAR0_SRIOV_QDMA_PREFETCHABLE {1}\
     CPM_PCIE0_PF0_BAR1_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR1_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR1_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR1_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR2_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR2_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR2_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR2_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR3_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR3_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR3_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR3_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR4_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR4_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR4_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR4_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR5_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR5_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR5_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR5_SRIOV_QDMA_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR0_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR0_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR0_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR0_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR1_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR1_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR1_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR1_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR2_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR2_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR2_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR2_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR3_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR3_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR3_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR3_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR4_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR4_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR4_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR4_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR5_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR5_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR5_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR5_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR0_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR0_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR0_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR0_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR1_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR1_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR1_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR1_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR2_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR2_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR2_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR2_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR3_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR3_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR3_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR3_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR4_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR4_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR4_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR4_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR5_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR5_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR5_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR5_XDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR0_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR0_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR0_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR0_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR1_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR1_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR1_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR1_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR2_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR2_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR2_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR2_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR3_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR3_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR3_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR3_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR4_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR4_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR4_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR4_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR5_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR5_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR5_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR5_SRIOV_QDMA_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR0_QDMA_SIZE {128}\
     CPM_PCIE0_PF1_BAR0_QDMA_SIZE {128}\
     CPM_PCIE0_PF2_BAR0_QDMA_SIZE {128}\
     CPM_PCIE0_PF3_BAR0_QDMA_SIZE {128}\
     CPM_PCIE0_PF0_BAR1_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR1_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR1_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR1_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR2_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR2_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR2_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR2_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR3_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR3_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR3_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR3_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR4_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR4_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR4_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR4_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR5_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR5_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR5_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR5_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR0_XDMA_SIZE {64}\
     CPM_PCIE0_PF1_BAR0_XDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR0_XDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR0_XDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR1_XDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR1_XDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR1_XDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR1_XDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR2_XDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR2_XDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR2_XDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR2_XDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR3_XDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR3_XDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR3_XDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR3_XDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR4_XDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR4_XDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR4_XDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR4_XDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR5_XDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR5_XDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR5_XDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR5_XDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR0_SRIOV_QDMA_SIZE {16}\
     CPM_PCIE0_PF1_BAR0_SRIOV_QDMA_SIZE {16}\
     CPM_PCIE0_PF2_BAR0_SRIOV_QDMA_SIZE {16}\
     CPM_PCIE0_PF3_BAR0_SRIOV_QDMA_SIZE {16}\
     CPM_PCIE0_PF0_BAR1_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR1_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR1_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR1_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR2_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR2_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR2_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR2_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR3_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR3_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR3_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR3_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR4_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR4_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR4_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR4_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR5_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF1_BAR5_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF2_BAR5_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF3_BAR5_SRIOV_QDMA_SIZE {4}\
     CPM_PCIE0_PF0_BAR0_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF1_BAR0_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF2_BAR0_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF3_BAR0_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF0_BAR1_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR1_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR1_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR1_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR2_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR2_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR2_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR2_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR3_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR3_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR3_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR3_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR4_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR4_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR4_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR4_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR5_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR5_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR5_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR5_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR0_XDMA_TYPE {DMA}\
     CPM_PCIE0_PF1_BAR0_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR0_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR0_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR1_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR1_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR1_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR1_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR2_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR2_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR2_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR2_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR3_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR3_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR3_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR3_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR4_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR4_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR4_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR4_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR5_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR5_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR5_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR5_XDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR0_SRIOV_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF1_BAR0_SRIOV_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF2_BAR0_SRIOV_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF3_BAR0_SRIOV_QDMA_TYPE {DMA}\
     CPM_PCIE0_PF0_BAR1_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR1_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR1_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR1_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR2_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR2_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR2_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR2_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR3_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR3_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR3_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR3_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR4_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR4_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR4_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR4_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR5_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF1_BAR5_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF2_BAR5_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF3_BAR5_SRIOV_QDMA_TYPE {AXI_Bridge_Master}\
     CPM_PCIE0_PF0_BAR0_64BIT {0}\
     CPM_PCIE1_PF0_BAR0_64BIT {0}\
     CPM_PCIE0_PF1_BAR0_64BIT {0}\
     CPM_PCIE1_PF1_BAR0_64BIT {0}\
     CPM_PCIE0_PF2_BAR0_64BIT {0}\
     CPM_PCIE1_PF2_BAR0_64BIT {0}\
     CPM_PCIE0_PF3_BAR0_64BIT {0}\
     CPM_PCIE1_PF3_BAR0_64BIT {0}\
     CPM_PCIE0_PF0_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE1_PF0_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE0_PF1_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE1_PF1_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE0_PF2_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE1_PF2_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE0_PF3_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE1_PF3_SRIOV_BAR0_64BIT {0}\
     CPM_PCIE0_PF0_BAR1_64BIT {0}\
     CPM_PCIE1_PF0_BAR1_64BIT {0}\
     CPM_PCIE0_PF1_BAR1_64BIT {0}\
     CPM_PCIE1_PF1_BAR1_64BIT {0}\
     CPM_PCIE0_PF2_BAR1_64BIT {0}\
     CPM_PCIE1_PF2_BAR1_64BIT {0}\
     CPM_PCIE0_PF3_BAR1_64BIT {0}\
     CPM_PCIE1_PF3_BAR1_64BIT {0}\
     CPM_PCIE0_PF0_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE1_PF0_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE0_PF1_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE1_PF1_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE0_PF2_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE1_PF2_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE0_PF3_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE1_PF3_SRIOV_BAR1_64BIT {0}\
     CPM_PCIE0_PF0_BAR2_64BIT {0}\
     CPM_PCIE1_PF0_BAR2_64BIT {0}\
     CPM_PCIE0_PF1_BAR2_64BIT {0}\
     CPM_PCIE1_PF1_BAR2_64BIT {0}\
     CPM_PCIE0_PF2_BAR2_64BIT {0}\
     CPM_PCIE1_PF2_BAR2_64BIT {0}\
     CPM_PCIE0_PF3_BAR2_64BIT {0}\
     CPM_PCIE1_PF3_BAR2_64BIT {0}\
     CPM_PCIE0_PF0_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE1_PF0_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE0_PF1_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE1_PF1_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE0_PF2_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE1_PF2_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE0_PF3_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE1_PF3_SRIOV_BAR2_64BIT {0}\
     CPM_PCIE0_PF0_BAR3_64BIT {0}\
     CPM_PCIE1_PF0_BAR3_64BIT {0}\
     CPM_PCIE0_PF1_BAR3_64BIT {0}\
     CPM_PCIE1_PF1_BAR3_64BIT {0}\
     CPM_PCIE0_PF2_BAR3_64BIT {0}\
     CPM_PCIE1_PF2_BAR3_64BIT {0}\
     CPM_PCIE0_PF3_BAR3_64BIT {0}\
     CPM_PCIE1_PF3_BAR3_64BIT {0}\
     CPM_PCIE0_PF0_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE1_PF0_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE0_PF1_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE1_PF1_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE0_PF2_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE1_PF2_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE0_PF3_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE1_PF3_SRIOV_BAR3_64BIT {0}\
     CPM_PCIE0_PF0_BAR4_64BIT {0}\
     CPM_PCIE1_PF0_BAR4_64BIT {0}\
     CPM_PCIE0_PF1_BAR4_64BIT {0}\
     CPM_PCIE1_PF1_BAR4_64BIT {0}\
     CPM_PCIE0_PF2_BAR4_64BIT {0}\
     CPM_PCIE1_PF2_BAR4_64BIT {0}\
     CPM_PCIE0_PF3_BAR4_64BIT {0}\
     CPM_PCIE1_PF3_BAR4_64BIT {0}\
     CPM_PCIE0_PF0_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE1_PF0_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE0_PF1_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE1_PF1_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE0_PF2_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE1_PF2_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE0_PF3_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE1_PF3_SRIOV_BAR4_64BIT {0}\
     CPM_PCIE0_PF0_BAR5_64BIT {0}\
     CPM_PCIE1_PF0_BAR5_64BIT {0}\
     CPM_PCIE0_PF1_BAR5_64BIT {0}\
     CPM_PCIE1_PF1_BAR5_64BIT {0}\
     CPM_PCIE0_PF2_BAR5_64BIT {0}\
     CPM_PCIE1_PF2_BAR5_64BIT {0}\
     CPM_PCIE0_PF3_BAR5_64BIT {0}\
     CPM_PCIE1_PF3_BAR5_64BIT {0}\
     CPM_PCIE0_PF0_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE1_PF0_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE0_PF1_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE1_PF1_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE0_PF2_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE1_PF2_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE0_PF3_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE1_PF3_SRIOV_BAR5_64BIT {0}\
     CPM_PCIE0_PF0_BAR0_ENABLED {1}\
     CPM_PCIE1_PF0_BAR0_ENABLED {1}\
     CPM_PCIE0_PF1_BAR0_ENABLED {1}\
     CPM_PCIE1_PF1_BAR0_ENABLED {1}\
     CPM_PCIE0_PF2_BAR0_ENABLED {1}\
     CPM_PCIE1_PF2_BAR0_ENABLED {1}\
     CPM_PCIE0_PF3_BAR0_ENABLED {1}\
     CPM_PCIE1_PF3_BAR0_ENABLED {1}\
     CPM_PCIE0_PF0_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE1_PF0_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE0_PF1_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE1_PF1_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE0_PF2_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE1_PF2_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE0_PF3_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE1_PF3_SRIOV_BAR0_ENABLED {1}\
     CPM_PCIE0_PF0_BAR1_ENABLED {0}\
     CPM_PCIE1_PF0_BAR1_ENABLED {0}\
     CPM_PCIE0_PF1_BAR1_ENABLED {0}\
     CPM_PCIE1_PF1_BAR1_ENABLED {0}\
     CPM_PCIE0_PF2_BAR1_ENABLED {0}\
     CPM_PCIE1_PF2_BAR1_ENABLED {0}\
     CPM_PCIE0_PF3_BAR1_ENABLED {0}\
     CPM_PCIE1_PF3_BAR1_ENABLED {0}\
     CPM_PCIE0_PF0_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE1_PF0_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE0_PF1_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE1_PF1_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE0_PF2_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE1_PF2_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE0_PF3_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE1_PF3_SRIOV_BAR1_ENABLED {0}\
     CPM_PCIE0_PF0_BAR2_ENABLED {0}\
     CPM_PCIE1_PF0_BAR2_ENABLED {0}\
     CPM_PCIE0_PF1_BAR2_ENABLED {0}\
     CPM_PCIE1_PF1_BAR2_ENABLED {0}\
     CPM_PCIE0_PF2_BAR2_ENABLED {0}\
     CPM_PCIE1_PF2_BAR2_ENABLED {0}\
     CPM_PCIE0_PF3_BAR2_ENABLED {0}\
     CPM_PCIE1_PF3_BAR2_ENABLED {0}\
     CPM_PCIE0_PF0_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE1_PF0_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE0_PF1_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE1_PF1_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE0_PF2_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE1_PF2_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE0_PF3_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE1_PF3_SRIOV_BAR2_ENABLED {0}\
     CPM_PCIE0_PF0_BAR3_ENABLED {0}\
     CPM_PCIE1_PF0_BAR3_ENABLED {0}\
     CPM_PCIE0_PF1_BAR3_ENABLED {0}\
     CPM_PCIE1_PF1_BAR3_ENABLED {0}\
     CPM_PCIE0_PF2_BAR3_ENABLED {0}\
     CPM_PCIE1_PF2_BAR3_ENABLED {0}\
     CPM_PCIE0_PF3_BAR3_ENABLED {0}\
     CPM_PCIE1_PF3_BAR3_ENABLED {0}\
     CPM_PCIE0_PF0_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE1_PF0_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE0_PF1_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE1_PF1_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE0_PF2_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE1_PF2_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE0_PF3_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE1_PF3_SRIOV_BAR3_ENABLED {0}\
     CPM_PCIE0_PF0_BAR4_ENABLED {0}\
     CPM_PCIE1_PF0_BAR4_ENABLED {0}\
     CPM_PCIE0_PF1_BAR4_ENABLED {0}\
     CPM_PCIE1_PF1_BAR4_ENABLED {0}\
     CPM_PCIE0_PF2_BAR4_ENABLED {0}\
     CPM_PCIE1_PF2_BAR4_ENABLED {0}\
     CPM_PCIE0_PF3_BAR4_ENABLED {0}\
     CPM_PCIE1_PF3_BAR4_ENABLED {0}\
     CPM_PCIE0_PF0_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE1_PF0_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE0_PF1_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE1_PF1_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE0_PF2_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE1_PF2_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE0_PF3_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE1_PF3_SRIOV_BAR4_ENABLED {0}\
     CPM_PCIE0_PF0_BAR5_ENABLED {0}\
     CPM_PCIE1_PF0_BAR5_ENABLED {0}\
     CPM_PCIE0_PF1_BAR5_ENABLED {0}\
     CPM_PCIE1_PF1_BAR5_ENABLED {0}\
     CPM_PCIE0_PF2_BAR5_ENABLED {0}\
     CPM_PCIE1_PF2_BAR5_ENABLED {0}\
     CPM_PCIE0_PF3_BAR5_ENABLED {0}\
     CPM_PCIE1_PF3_BAR5_ENABLED {0}\
     CPM_PCIE0_PF0_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE1_PF0_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE0_PF1_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE1_PF1_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE0_PF2_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE1_PF2_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE0_PF3_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE1_PF3_SRIOV_BAR5_ENABLED {0}\
     CPM_PCIE0_PF0_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_SRIOV_BAR0_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_SRIOV_BAR1_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_SRIOV_BAR2_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_SRIOV_BAR3_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_SRIOV_BAR4_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_SRIOV_BAR5_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_SRIOV_BAR0_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_SRIOV_BAR1_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_SRIOV_BAR2_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_SRIOV_BAR3_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_SRIOV_BAR4_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_SRIOV_BAR5_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_BAR0_SIZE {128}\
     CPM_PCIE1_PF0_BAR0_SIZE {128}\
     CPM_PCIE0_PF1_BAR0_SIZE {128}\
     CPM_PCIE1_PF1_BAR0_SIZE {128}\
     CPM_PCIE0_PF2_BAR0_SIZE {128}\
     CPM_PCIE1_PF2_BAR0_SIZE {128}\
     CPM_PCIE0_PF3_BAR0_SIZE {128}\
     CPM_PCIE1_PF3_BAR0_SIZE {128}\
     CPM_PCIE0_PF0_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE1_PF0_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE0_PF1_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE1_PF1_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE0_PF2_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE1_PF2_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE0_PF3_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE1_PF3_SRIOV_BAR0_SIZE {4}\
     CPM_PCIE0_PF0_BAR1_SIZE {4}\
     CPM_PCIE1_PF0_BAR1_SIZE {4}\
     CPM_PCIE0_PF1_BAR1_SIZE {4}\
     CPM_PCIE1_PF1_BAR1_SIZE {4}\
     CPM_PCIE0_PF2_BAR1_SIZE {4}\
     CPM_PCIE1_PF2_BAR1_SIZE {4}\
     CPM_PCIE0_PF3_BAR1_SIZE {4}\
     CPM_PCIE1_PF3_BAR1_SIZE {4}\
     CPM_PCIE0_PF0_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE1_PF0_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE0_PF1_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE1_PF1_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE0_PF2_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE1_PF2_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE0_PF3_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE1_PF3_SRIOV_BAR1_SIZE {4}\
     CPM_PCIE0_PF0_BAR2_SIZE {4}\
     CPM_PCIE1_PF0_BAR2_SIZE {4}\
     CPM_PCIE0_PF1_BAR2_SIZE {4}\
     CPM_PCIE1_PF1_BAR2_SIZE {4}\
     CPM_PCIE0_PF2_BAR2_SIZE {4}\
     CPM_PCIE1_PF2_BAR2_SIZE {4}\
     CPM_PCIE0_PF3_BAR2_SIZE {4}\
     CPM_PCIE1_PF3_BAR2_SIZE {4}\
     CPM_PCIE0_PF0_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE1_PF0_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE0_PF1_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE1_PF1_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE0_PF2_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE1_PF2_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE0_PF3_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE1_PF3_SRIOV_BAR2_SIZE {4}\
     CPM_PCIE0_PF0_BAR3_SIZE {4}\
     CPM_PCIE1_PF0_BAR3_SIZE {4}\
     CPM_PCIE0_PF1_BAR3_SIZE {4}\
     CPM_PCIE1_PF1_BAR3_SIZE {4}\
     CPM_PCIE0_PF2_BAR3_SIZE {4}\
     CPM_PCIE1_PF2_BAR3_SIZE {4}\
     CPM_PCIE0_PF3_BAR3_SIZE {4}\
     CPM_PCIE1_PF3_BAR3_SIZE {4}\
     CPM_PCIE0_PF0_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE1_PF0_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE0_PF1_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE1_PF1_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE0_PF2_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE1_PF2_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE0_PF3_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE1_PF3_SRIOV_BAR3_SIZE {4}\
     CPM_PCIE0_PF0_BAR4_SIZE {4}\
     CPM_PCIE1_PF0_BAR4_SIZE {4}\
     CPM_PCIE0_PF1_BAR4_SIZE {4}\
     CPM_PCIE1_PF1_BAR4_SIZE {4}\
     CPM_PCIE0_PF2_BAR4_SIZE {4}\
     CPM_PCIE1_PF2_BAR4_SIZE {4}\
     CPM_PCIE0_PF3_BAR4_SIZE {4}\
     CPM_PCIE1_PF3_BAR4_SIZE {4}\
     CPM_PCIE0_PF0_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE1_PF0_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE0_PF1_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE1_PF1_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE0_PF2_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE1_PF2_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE0_PF3_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE1_PF3_SRIOV_BAR4_SIZE {4}\
     CPM_PCIE0_PF0_BAR5_SIZE {4}\
     CPM_PCIE1_PF0_BAR5_SIZE {4}\
     CPM_PCIE0_PF1_BAR5_SIZE {4}\
     CPM_PCIE1_PF1_BAR5_SIZE {4}\
     CPM_PCIE0_PF2_BAR5_SIZE {4}\
     CPM_PCIE1_PF2_BAR5_SIZE {4}\
     CPM_PCIE0_PF3_BAR5_SIZE {4}\
     CPM_PCIE1_PF3_BAR5_SIZE {4}\
     CPM_PCIE0_PF0_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE1_PF0_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE0_PF1_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE1_PF1_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE0_PF2_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE1_PF2_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE0_PF3_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE1_PF3_SRIOV_BAR5_SIZE {4}\
     CPM_PCIE0_PF0_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF0_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF1_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF1_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF2_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF2_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF3_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF3_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF0_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF0_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF1_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF1_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF2_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF2_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF3_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE1_PF3_SRIOV_BAR0_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF0_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF1_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF1_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF2_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF2_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF3_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF3_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF0_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF0_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF1_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF1_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF2_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF2_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF3_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE1_PF3_SRIOV_BAR1_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF0_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF1_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF1_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF2_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF2_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF3_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF3_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF0_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF0_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF1_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF1_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF2_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF2_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF3_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE1_PF3_SRIOV_BAR2_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF0_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF1_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF1_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF2_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF2_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF3_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF3_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF0_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF0_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF1_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF1_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF2_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF2_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF3_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE1_PF3_SRIOV_BAR3_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF0_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF1_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF1_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF2_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF2_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF3_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF3_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF0_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF0_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF1_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF1_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF2_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF2_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF3_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE1_PF3_SRIOV_BAR4_TYPE {Memory}\
     CPM_PCIE0_PF0_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF0_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF1_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF1_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF2_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF2_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF3_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF3_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF0_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF0_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF1_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF1_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF2_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF2_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF3_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE1_PF3_SRIOV_BAR5_TYPE {Memory}\
     CPM_PCIE0_PF0_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE1_PF0_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE0_PF1_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE1_PF1_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE0_PF2_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE1_PF2_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE0_PF3_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE1_PF3_AXIST_BYPASS_64BIT {0}\
     CPM_PCIE0_PF0_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE1_PF0_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE0_PF1_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE1_PF1_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE0_PF2_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE1_PF2_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE0_PF3_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE1_PF3_AXILITE_MASTER_64BIT {0}\
     CPM_PCIE0_PF0_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_AXIST_BYPASS_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE1_PF0_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE0_PF1_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE1_PF1_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE0_PF2_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE1_PF2_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE0_PF3_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE1_PF3_AXILITE_MASTER_PREFETCHABLE {0}\
     CPM_PCIE0_PF0_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE1_PF0_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE0_PF1_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE1_PF1_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE0_PF2_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE1_PF2_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE0_PF3_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE1_PF3_EXPANSION_ROM_ENABLED {0}\
     CPM_PCIE0_PF0_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE1_PF0_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE0_PF1_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE1_PF1_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE0_PF2_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE1_PF2_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE0_PF3_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE1_PF3_AXIST_BYPASS_ENABLED {0}\
     CPM_PCIE0_PF0_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE1_PF0_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE0_PF1_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE1_PF1_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE0_PF2_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE1_PF2_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE0_PF3_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE1_PF3_AXILITE_MASTER_ENABLED {0}\
     CPM_PCIE0_PF0_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_EXPANSION_ROM_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_AXIST_BYPASS_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE1_PF0_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE0_PF1_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE1_PF1_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE0_PF2_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE1_PF2_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE0_PF3_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE1_PF3_AXILITE_MASTER_SCALE {Kilobytes}\
     CPM_PCIE0_PF0_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE1_PF0_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE0_PF1_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE1_PF1_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE0_PF2_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE1_PF2_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE0_PF3_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE1_PF3_EXPANSION_ROM_SIZE {2}\
     CPM_PCIE0_PF0_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE1_PF0_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE0_PF1_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE1_PF1_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE0_PF2_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE1_PF2_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE0_PF3_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE1_PF3_AXIST_BYPASS_SIZE {128}\
     CPM_PCIE0_PF0_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE1_PF0_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE0_PF1_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE1_PF1_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE0_PF2_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE1_PF2_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE0_PF3_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE1_PF3_AXILITE_MASTER_SIZE {128}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_BRIDGE_0 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_BRIDGE_1 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_BRIDGE_2 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_BRIDGE_3 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_BRIDGE_4 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_BRIDGE_5 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BASEADDR_0 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BRIDGE_0 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_HIGHADDR_0 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BASEADDR_1 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BRIDGE_1 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_HIGHADDR_1 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BASEADDR_2 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BRIDGE_2 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_HIGHADDR_2 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BASEADDR_3 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BRIDGE_3 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_HIGHADDR_3 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BASEADDR_4 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BRIDGE_4 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_HIGHADDR_4 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BASEADDR_5 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_BRIDGE_5 {0x0000000000000000}\
     CPM_PCIE0_PF0_AXIBAR2PCIE_HIGHADDR_5 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_SRIOV_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_SRIOV_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_SRIOV_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_SRIOV_QDMA_0 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_XDMA_0 {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_XDMA_0 {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_XDMA_0 {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_XDMA_0 {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_SRIOV_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_SRIOV_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_SRIOV_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_SRIOV_QDMA_1 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_XDMA_1 {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_XDMA_1 {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_XDMA_1 {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_XDMA_1 {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_SRIOV_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_SRIOV_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_SRIOV_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_SRIOV_QDMA_2 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_XDMA_2 {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_XDMA_2 {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_XDMA_2 {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_XDMA_2 {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_SRIOV_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_SRIOV_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_SRIOV_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_SRIOV_QDMA_3 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_XDMA_3 {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_XDMA_3 {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_XDMA_3 {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_XDMA_3 {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_SRIOV_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_SRIOV_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_SRIOV_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_SRIOV_QDMA_4 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_XDMA_4 {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_XDMA_4 {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_XDMA_4 {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_XDMA_4 {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_SRIOV_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_SRIOV_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_SRIOV_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_SRIOV_QDMA_5 {0x0000000000000000}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_XDMA_5 {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_XDMA_5 {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_XDMA_5 {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_XDMA_5 {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE1_PF0_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE1_PF1_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE1_PF2_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE1_PF3_PCIEBAR2AXIBAR_AXIL_MASTER {0}\
     CPM_PCIE0_PF0_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE1_PF0_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE0_PF1_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE1_PF1_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE0_PF2_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE1_PF2_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE0_PF3_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE1_PF3_PCIEBAR2AXIBAR_AXIST_BYPASS {0}\
     CPM_PCIE0_SRIOV_FIRST_VF_OFFSET {4}\
     CPM_PCIE1_SRIOV_FIRST_VF_OFFSET {4}\
     CPM_PCIE0_PF0_SRIOV_VF_DEVICE_ID {C03F}\
     CPM_PCIE1_PF0_SRIOV_VF_DEVICE_ID {C034}\
     CPM_PCIE0_PF1_SRIOV_VF_DEVICE_ID {C13F}\
     CPM_PCIE1_PF1_SRIOV_VF_DEVICE_ID {C134}\
     CPM_PCIE0_PF2_SRIOV_VF_DEVICE_ID {C23F}\
     CPM_PCIE1_PF2_SRIOV_VF_DEVICE_ID {C234}\
     CPM_PCIE0_PF3_SRIOV_VF_DEVICE_ID {C33F}\
     CPM_PCIE1_PF3_SRIOV_VF_DEVICE_ID {C334}\
     CPM_PCIE0_PF0_SRIOV_FIRST_VF_OFFSET {4}\
     CPM_PCIE1_PF0_SRIOV_FIRST_VF_OFFSET {4}\
     CPM_PCIE0_PF1_SRIOV_FIRST_VF_OFFSET {7}\
     CPM_PCIE1_PF1_SRIOV_FIRST_VF_OFFSET {7}\
     CPM_PCIE0_PF2_SRIOV_FIRST_VF_OFFSET {10}\
     CPM_PCIE1_PF2_SRIOV_FIRST_VF_OFFSET {10}\
     CPM_PCIE0_PF3_SRIOV_FIRST_VF_OFFSET {13}\
     CPM_PCIE1_PF3_SRIOV_FIRST_VF_OFFSET {13}\
     CPM_PCIE0_PF0_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE1_PF0_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE0_PF1_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE1_PF1_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE0_PF2_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE1_PF2_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE0_PF3_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE1_PF3_SRIOV_FUNC_DEP_LINK {0}\
     CPM_PCIE0_PF0_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE1_PF0_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE0_PF1_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE1_PF1_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE0_PF2_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE1_PF2_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE0_PF3_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE1_PF3_SRIOV_SUPPORTED_PAGE_SIZE {553}\
     CPM_PCIE0_PF0_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE1_PF0_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE0_PF1_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE1_PF1_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE0_PF2_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE1_PF2_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE0_PF3_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE1_PF3_SRIOV_ARI_CAPBL_HIER_PRESERVED {0}\
     CPM_PCIE0_PF0_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE1_PF0_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE0_PF1_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE1_PF1_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE0_PF2_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE1_PF2_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE0_PF3_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE1_PF3_SRIOV_CAP_TOTAL_VF {0}\
     CPM_PCIE0_PF0_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE1_PF0_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE0_PF1_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE1_PF1_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE0_PF2_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE1_PF2_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE0_PF3_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE1_PF3_SRIOV_CAP_INITIAL_VF {4}\
     CPM_PCIE0_MSI_X_OPTIONS {None}\
     CPM_PCIE1_MSI_X_OPTIONS {None}\
     CPM_PCIE0_MSIX_RP_ENABLED {1}\
     CPM_PCIE1_MSIX_RP_ENABLED {1}\
     CPM_PCIE0_PF0_MSIX_ENABLED {1}\
     CPM_PCIE1_PF0_MSIX_ENABLED {1}\
     CPM_PCIE0_VFG0_MSIX_ENABLED {1}\
     CPM_PCIE1_VFG0_MSIX_ENABLED {1}\
     CPM_PCIE0_PF1_MSIX_ENABLED {1}\
     CPM_PCIE1_PF1_MSIX_ENABLED {1}\
     CPM_PCIE0_VFG1_MSIX_ENABLED {1}\
     CPM_PCIE1_VFG1_MSIX_ENABLED {1}\
     CPM_PCIE0_PF2_MSIX_ENABLED {1}\
     CPM_PCIE1_PF2_MSIX_ENABLED {1}\
     CPM_PCIE0_VFG2_MSIX_ENABLED {1}\
     CPM_PCIE1_VFG2_MSIX_ENABLED {1}\
     CPM_PCIE0_PF3_MSIX_ENABLED {1}\
     CPM_PCIE1_PF3_MSIX_ENABLED {1}\
     CPM_PCIE0_VFG3_MSIX_ENABLED {1}\
     CPM_PCIE1_VFG3_MSIX_ENABLED {1}\
     CPM_PCIE0_PF0_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_PF0_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_VFG0_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_VFG0_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_PF1_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_PF1_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_VFG1_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_VFG1_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_PF2_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_PF2_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_VFG2_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_VFG2_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_PF3_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_PF3_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_VFG3_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE1_VFG3_MSIX_CAP_PBA_BIR {BAR_0}\
     CPM_PCIE0_PF0_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_PF0_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_VFG0_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_VFG0_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_PF1_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_PF1_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_VFG1_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_VFG1_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_PF2_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_PF2_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_VFG2_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_VFG2_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_PF3_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_PF3_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_VFG3_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE1_VFG3_MSIX_CAP_PBA_OFFSET {50}\
     CPM_PCIE0_PF0_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_PF0_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_VFG0_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_VFG0_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_PF1_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_PF1_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_VFG1_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_VFG1_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_PF2_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_PF2_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_VFG2_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_VFG2_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_PF3_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_PF3_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_VFG3_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE1_VFG3_MSIX_CAP_TABLE_BIR {BAR_0}\
     CPM_PCIE0_PF0_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_PF0_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_VFG0_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_VFG0_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_PF1_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_PF1_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_VFG1_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_VFG1_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_PF2_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_PF2_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_VFG2_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_VFG2_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_PF3_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_PF3_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_VFG3_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE1_VFG3_MSIX_CAP_TABLE_OFFSET {40}\
     CPM_PCIE0_PF0_MSIX_CAP_TABLE_SIZE {007}\
     CPM_PCIE1_PF0_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_VFG0_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE1_VFG0_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_PF1_MSIX_CAP_TABLE_SIZE {007}\
     CPM_PCIE1_PF1_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_VFG1_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE1_VFG1_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_PF2_MSIX_CAP_TABLE_SIZE {007}\
     CPM_PCIE1_PF2_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_VFG2_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE1_VFG2_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_PF3_MSIX_CAP_TABLE_SIZE {007}\
     CPM_PCIE1_PF3_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_VFG3_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE1_VFG3_MSIX_CAP_TABLE_SIZE {001}\
     CPM_PCIE0_PF0_MSI_ENABLED {1}\
     CPM_PCIE1_PF0_MSI_ENABLED {1}\
     CPM_PCIE0_PF1_MSI_ENABLED {1}\
     CPM_PCIE1_PF1_MSI_ENABLED {1}\
     CPM_PCIE0_PF2_MSI_ENABLED {1}\
     CPM_PCIE1_PF2_MSI_ENABLED {1}\
     CPM_PCIE0_PF3_MSI_ENABLED {1}\
     CPM_PCIE1_PF3_MSI_ENABLED {1}\
     CPM_PCIE0_PF0_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE1_PF0_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE0_PF1_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE1_PF1_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE0_PF2_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE1_PF2_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE0_PF3_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE1_PF3_MSI_CAP_PERVECMASKCAP {0}\
     CPM_PCIE0_PF0_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE1_PF0_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE0_PF1_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE1_PF1_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE0_PF2_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE1_PF2_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE0_PF3_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE1_PF3_MSI_CAP_MULTIMSGCAP {1_vector}\
     CPM_PCIE0_PF0_SRIOV_CAP_VER {1}\
     CPM_PCIE1_PF0_SRIOV_CAP_VER {1}\
     CPM_PCIE0_PF1_SRIOV_CAP_VER {1}\
     CPM_PCIE1_PF1_SRIOV_CAP_VER {1}\
     CPM_PCIE0_PF2_SRIOV_CAP_VER {1}\
     CPM_PCIE1_PF2_SRIOV_CAP_VER {1}\
     CPM_PCIE0_PF3_SRIOV_CAP_VER {1}\
     CPM_PCIE1_PF3_SRIOV_CAP_VER {1}\
     CPM_PCIE0_PF0_PM_CAP_VER_ID {3}\
     CPM_PCIE1_PF0_PM_CAP_VER_ID {3}\
     CPM_PCIE0_PF0_VC_CAP_VER {1}\
     CPM_PCIE1_PF0_VC_CAP_VER {1}\
     CPM_PCIE0_PF0_MARGINING_CAP_VER {1}\
     CPM_PCIE1_PF0_MARGINING_CAP_VER {1}\
     CPM_PCIE0_PF0_DLL_FEATURE_CAP_VER {1}\
     CPM_PCIE1_PF0_DLL_FEATURE_CAP_VER {1}\
     CPM_PCIE0_PF0_ARI_CAP_VER {1}\
     CPM_PCIE1_PF0_ARI_CAP_VER {1}\
     CPM_PCIE0_PF0_TPHR_CAP_VER {1}\
     CPM_PCIE1_PF0_TPHR_CAP_VER {1}\
     CPM_PCIE0_PF0_PL16_CAP_VER {1}\
     CPM_PCIE1_PF0_PL16_CAP_VER {1}\
     CPM_PCIE0_PF0_PM_CAP_ID {0}\
     CPM_PCIE1_PF0_PM_CAP_ID {0}\
     CPM_PCIE0_PF0_PL16_CAP_ID {0}\
     CPM_PCIE1_PF0_PL16_CAP_ID {0}\
     CPM_PCIE0_PF0_MARGINING_CAP_ID {0}\
     CPM_PCIE1_PF0_MARGINING_CAP_ID {0}\
     CPM_PCIE0_PF0_DLL_FEATURE_CAP_ID {0x0025}\
     CPM_PCIE1_PF0_DLL_FEATURE_CAP_ID {0}\
     CPM_PCIE0_PF0_TPHR_ENABLE {0}\
     CPM_PCIE1_PF0_TPHR_ENABLE {0}\
     CPM_PCIE0_PF0_TPHR_CAP_ST_TABLE_SIZE {16}\
     CPM_PCIE1_PF0_TPHR_CAP_ST_TABLE_SIZE {16}\
     CPM_PCIE0_PF0_TPHR_CAP_ST_TABLE_LOC {ST_Table_not_present}\
     CPM_PCIE1_PF0_TPHR_CAP_ST_TABLE_LOC {ST_Table_not_present}\
     CPM_PCIE0_PF0_TPHR_CAP_INT_VEC_MODE {1}\
     CPM_PCIE1_PF0_TPHR_CAP_INT_VEC_MODE {1}\
     CPM_PCIE0_PF0_TPHR_CAP_DEV_SPECIFIC_MODE {1}\
     CPM_PCIE1_PF0_TPHR_CAP_DEV_SPECIFIC_MODE {1}\
     CPM_PCIE0_PF0_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE1_PF0_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE0_PF1_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE1_PF1_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE0_PF2_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE1_PF2_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE0_PF3_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE1_PF3_ARI_CAP_NEXT_FUNC {0}\
     CPM_PCIE0_PF0_PASID_CAP_MAX_PASID_WIDTH {1}\
     CPM_PCIE1_PF0_PASID_CAP_MAX_PASID_WIDTH {1}\
     CPM_PCIE0_PF2_PASID_CAP_MAX_PASID_WIDTH {1}\
     CPM_PCIE1_PF2_PASID_CAP_MAX_PASID_WIDTH {1}\
     CPM_PCIE0_PF0_AER_CAP_ECRC_GEN_AND_CHECK_CAPABLE {0}\
     CPM_PCIE1_PF0_AER_CAP_ECRC_GEN_AND_CHECK_CAPABLE {0}\
     CPM_PCIE0_MCAP_ENABLE {0}\
     CPM_PCIE1_MCAP_ENABLE {0}\
     CPM_PCIE0_PF0_TPHR_CAP_ENABLE {0}\
     CPM_PCIE1_PF0_TPHR_CAP_ENABLE {0}\
     CPM_PCIE0_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE1_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE0_AER_CAP_ENABLED {1}\
     CPM_PCIE1_AER_CAP_ENABLED {0}\
     CPM_PCIE0_ARI_CAP_ENABLED {1}\
     CPM_PCIE1_ARI_CAP_ENABLED {1}\
     CPM_PCIE0_PF0_VC_CAP_ENABLED {0}\
     CPM_PCIE1_PF0_VC_CAP_ENABLED {0}\
     CPM_PCIE0_PF0_DSN_CAP_ENABLE {0}\
     CPM_PCIE1_PF0_DSN_CAP_ENABLE {0}\
     CPM_PCIE0_PF1_DSN_CAP_ENABLE {0}\
     CPM_PCIE1_PF1_DSN_CAP_ENABLE {0}\
     CPM_PCIE0_PF2_DSN_CAP_ENABLE {0}\
     CPM_PCIE1_PF2_DSN_CAP_ENABLE {0}\
     CPM_PCIE0_PF3_DSN_CAP_ENABLE {0}\
     CPM_PCIE1_PF3_DSN_CAP_ENABLE {0}\
     CPM_PCIE0_PF0_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE1_PF0_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE0_PF1_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE1_PF1_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE0_PF2_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE1_PF2_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE0_PF3_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE1_PF3_SRIOV_CAP_ENABLE {0}\
     CPM_PCIE0_ATS_PRI_CAP_ON {0}\
     CPM_PCIE1_ATS_PRI_CAP_ON {0}\
     CPM_PCIE0_PF0_ATS_CAP_ON {0}\
     CPM_PCIE1_PF0_ATS_CAP_ON {0}\
     CPM_PCIE0_VFG0_ATS_CAP_ON {0}\
     CPM_PCIE1_VFG0_ATS_CAP_ON {0}\
     CPM_PCIE0_PF1_ATS_CAP_ON {0}\
     CPM_PCIE1_PF1_ATS_CAP_ON {0}\
     CPM_PCIE0_VFG1_ATS_CAP_ON {0}\
     CPM_PCIE1_VFG1_ATS_CAP_ON {0}\
     CPM_PCIE0_PF2_ATS_CAP_ON {0}\
     CPM_PCIE1_PF2_ATS_CAP_ON {0}\
     CPM_PCIE0_VFG2_ATS_CAP_ON {0}\
     CPM_PCIE1_VFG2_ATS_CAP_ON {0}\
     CPM_PCIE0_PF3_ATS_CAP_ON {0}\
     CPM_PCIE1_PF3_ATS_CAP_ON {0}\
     CPM_PCIE0_VFG3_ATS_CAP_ON {0}\
     CPM_PCIE1_VFG3_ATS_CAP_ON {0}\
     CPM_PCIE0_PF0_PRI_CAP_ON {0}\
     CPM_PCIE1_PF0_PRI_CAP_ON {0}\
     CPM_PCIE0_VFG0_PRI_CAP_ON {0}\
     CPM_PCIE1_VFG0_PRI_CAP_ON {0}\
     CPM_PCIE0_PF1_PRI_CAP_ON {0}\
     CPM_PCIE1_PF1_PRI_CAP_ON {0}\
     CPM_PCIE0_VFG1_PRI_CAP_ON {0}\
     CPM_PCIE1_VFG1_PRI_CAP_ON {0}\
     CPM_PCIE0_PF2_PRI_CAP_ON {0}\
     CPM_PCIE1_PF2_PRI_CAP_ON {0}\
     CPM_PCIE0_VFG2_PRI_CAP_ON {0}\
     CPM_PCIE1_VFG2_PRI_CAP_ON {0}\
     CPM_PCIE0_PF3_PRI_CAP_ON {0}\
     CPM_PCIE1_PF3_PRI_CAP_ON {0}\
     CPM_PCIE0_VFG3_PRI_CAP_ON {0}\
     CPM_PCIE1_VFG3_PRI_CAP_ON {0}\
     CPM_PCIE0_PF0_PL16_CAP_ON {0}\
     CPM_PCIE1_PF0_PL16_CAP_ON {0}\
     CPM_PCIE0_PF0_DLL_FEATURE_CAP_ON {1}\
     CPM_PCIE1_PF0_DLL_FEATURE_CAP_ON {0}\
     CPM_PCIE0_PF0_MARGINING_CAP_ON {0}\
     CPM_PCIE1_PF0_MARGINING_CAP_ON {0}\
     CPM_PCIE0_PF0_PASID_CAP_ON {0}\
     CPM_PCIE1_PF0_PASID_CAP_ON {0}\
     CPM_PCIE0_AXISTEN_USER_SPARE {0}\
     CPM_PCIE1_AXISTEN_USER_SPARE {0}\
     CPM_PCIE0_AXISTEN_IF_TX_PARITY_EN {0}\
     CPM_PCIE1_AXISTEN_IF_TX_PARITY_EN {0}\
     CPM_PCIE0_AXISTEN_IF_RX_PARITY_EN {1}\
     CPM_PCIE1_AXISTEN_IF_RX_PARITY_EN {1}\
     CPM_PCIE0_AXISTEN_IF_COMPL_TIMEOUT_REG0 {BEBC20}\
     CPM_PCIE1_AXISTEN_IF_COMPL_TIMEOUT_REG0 {BEBC20}\
     CPM_PCIE0_AXISTEN_IF_COMPL_TIMEOUT_REG1 {2FAF080}\
     CPM_PCIE1_AXISTEN_IF_COMPL_TIMEOUT_REG1 {2FAF080}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_RX_TAG_SCALING {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_RX_TAG_SCALING {0}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_TX_TAG_SCALING {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_TX_TAG_SCALING {0}\
     CPM_PCIE0_AXISTEN_IF_RQ_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE1_AXISTEN_IF_RQ_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE0_AXISTEN_IF_RC_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE1_AXISTEN_IF_RC_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE0_AXISTEN_IF_CQ_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE1_AXISTEN_IF_CQ_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE0_AXISTEN_IF_CC_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE1_AXISTEN_IF_CC_ALIGNMENT_MODE {DWORD_Aligned}\
     CPM_PCIE0_AXISTEN_IF_EXT_512_RQ_STRADDLE {1}\
     CPM_PCIE1_AXISTEN_IF_EXT_512_RQ_STRADDLE {1}\
     CPM_PCIE0_AXISTEN_IF_EXT_512_RC_STRADDLE {1}\
     CPM_PCIE1_AXISTEN_IF_EXT_512_RC_STRADDLE {1}\
     CPM_PCIE0_AXISTEN_IF_EXT_512_CQ_STRADDLE {0}\
     CPM_PCIE1_AXISTEN_IF_EXT_512_CQ_STRADDLE {0}\
     CPM_PCIE0_AXISTEN_IF_EXT_512_CC_STRADDLE {0}\
     CPM_PCIE1_AXISTEN_IF_EXT_512_CC_STRADDLE {0}\
     CPM_PCIE0_AXISTEN_IF_WIDTH {64}\
     CPM_PCIE1_AXISTEN_IF_WIDTH {64}\
     CPM_PCIE0_AXISTEN_IF_EXT_512 {0}\
     CPM_PCIE1_AXISTEN_IF_EXT_512 {0}\
     CPM_PCIE0_AXISTEN_IF_RC_STRADDLE {0}\
     CPM_PCIE1_AXISTEN_IF_RC_STRADDLE {0}\
     CPM_PCIE0_AXISTEN_IF_EXT_512_RC_4TLP_STRADDLE {1}\
     CPM_PCIE1_AXISTEN_IF_EXT_512_RC_4TLP_STRADDLE {1}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_256_TAGS {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_256_TAGS {0}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_CLIENT_TAG {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_CLIENT_TAG {0}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_RX_MSG_INTFC {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_RX_MSG_INTFC {0}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_INTERNAL_MSIX_TABLE {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_INTERNAL_MSIX_TABLE {0}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_MESSAGE_RID_CHECK {1}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_MESSAGE_RID_CHECK {1}\
     CPM_PCIE0_AXISTEN_IF_ENABLE_MSG_ROUTE {0}\
     CPM_PCIE1_AXISTEN_IF_ENABLE_MSG_ROUTE {0}\
     CPM_PCIE0_AXISTEN_MSIX_VECTORS_PER_FUNCTION {8}\
     CPM_PCIE1_AXISTEN_MSIX_VECTORS_PER_FUNCTION {8}\
     CPM_PCIE0_AXISTEN_IF_SIM_SHORT_CPL_TIMEOUT {0}\
     CPM_PCIE1_AXISTEN_IF_SIM_SHORT_CPL_TIMEOUT {0}\
     CPM_PCIE0_AXISTEN_IF_EXTEND_CPL_TIMEOUT {16ms_to_1s}\
     CPM_PCIE1_AXISTEN_IF_EXTEND_CPL_TIMEOUT {16ms_to_1s}\
     CPM_PCIE0_DSC_BYPASS_RD {0}\
     CPM_PCIE1_DSC_BYPASS_RD {0}\
     CPM_PCIE0_DSC_BYPASS_WR {0}\
     CPM_PCIE1_DSC_BYPASS_WR {0}\
     CPM_PCIE0_QDMA_MULTQ_MAX {2048}\
     CPM_PCIE0_QDMA_PARITY_SETTINGS {None}\
     CPM_PCIE0_DMA_ROOT_PORT {0}\
     CPM_PCIE0_DMA_MSI_RX_PIN_ENABLED {FALSE}\
     CPM_PCIE0_DMA_ENABLE_SECURE {0}\
     CPM_PCIE0_DMA_DATA_WIDTH {256bits}\
     CPM_PCIE0_DMA_INTF {AXI4}\
     CPM_PCIE0_DMA_METERING_ENABLE {1}\
     CPM_PCIE0_DMA_MASK {256bits}\
     CPM_XDMA_TL_PF_VISIBLE {1}\
     CPM_XDMA_2PF_INTERRUPT_ENABLE {0}\
     CPM_PCIE0_XDMA_DSC_BYPASS_RD {0000}\
     CPM_PCIE0_XDMA_DSC_BYPASS_WR {0000}\
     CPM_PCIE0_XDMA_AXI_ID_WIDTH {2}\
     CPM_PCIE0_XDMA_PARITY_SETTINGS {None}\
     CPM_PCIE0_XDMA_STS_PORTS {0}\
     CPM_PCIE0_XDMA_RNUM_CHNL {1}\
     CPM_PCIE0_XDMA_WNUM_CHNL {1}\
     CPM_PCIE0_XDMA_RNUM_RIDS {2}\
     CPM_PCIE0_XDMA_WNUM_RIDS {2}\
     CPM_PCIE0_XDMA_IRQ {1}\
     CPM_PCIE0_NUM_USR_IRQ {1}\
   } \
   CONFIG.PS_PMC_CONFIG {\
     SMON_ALARMS {Set_Alarms_On}\
     SMON_ENABLE_INT_VOLTAGE_MONITORING {0}\
     SMON_ENABLE_TEMP_AVERAGING {0}\
     SMON_INTERFACE_TO_USE {None}\
     SMON_INT_MEASUREMENT_ENABLE {0}\
     SMON_INT_MEASUREMENT_MODE {0}\
     SMON_INT_MEASUREMENT_ALARM_ENABLE {0}\
     SMON_INT_MEASUREMENT_AVG_ENABLE {0}\
     SMON_INT_MEASUREMENT_TH_HIGH {0}\
     SMON_INT_MEASUREMENT_TH_LOW {0}\
     SMON_MEASUREMENT_COUNT {62}\
     SMON_MEASUREMENT_LIST {BANK_VOLTAGE:GTY_AVTT-GTY_AVTT_103,GTY_AVTT_104,GTY_AVTT_105,GTY_AVTT_106,GTY_AVTT_200,GTY_AVTT_201,GTY_AVTT_202,GTY_AVTT_203,GTY_AVTT_204,GTY_AVTT_205,GTY_AVTT_206#VCC-GTY_AVCC_103,GTY_AVCC_104,GTY_AVCC_105,GTY_AVCC_106,GTY_AVCC_200,GTY_AVCC_201,GTY_AVCC_202,GTY_AVCC_203,GTY_AVCC_204,GTY_AVCC_205,GTY_AVCC_206#VCCAUX-GTY_AVCCAUX_103,GTY_AVCCAUX_104,GTY_AVCCAUX_105,GTY_AVCCAUX_106,GTY_AVCCAUX_200,GTY_AVCCAUX_201,GTY_AVCCAUX_202,GTY_AVCCAUX_203,GTY_AVCCAUX_204,GTY_AVCCAUX_205,GTY_AVCCAUX_206#VCCO-VCCO_306,VCCO_406,VCCO_500,VCCO_501,VCCO_502,VCCO_503,VCCO_700,VCCO_701,VCCO_702,VCCO_703,VCCO_704,VCCO_705,VCCO_706,VCCO_707,VCCO_708,VCCO_709,VCCO_710,VCCO_711|DEDICATED_PAD:VP-VP_VN|SUPPLY_VOLTAGE:VCC-VCC_BATT,VCC_PMC,VCC_PSFP,VCC_PSLP,VCC_RAM,VCC_SOC#VCCAUX-VCCAUX,VCCAUX_PMC,VCCAUX_SMON#VCCINT-VCCINT}\
     SMON_OT {{THRESHOLD_LOWER -55} {THRESHOLD_UPPER 125}}\
     SMON_PMBUS_ADDRESS {0x0}\
     SMON_PMBUS_UNRESTRICTED {0}\
     SMON_REFERENCE_SOURCE {Internal}\
     SMON_TEMP_AVERAGING_SAMPLES {0}\
     SMON_USER_TEMP {{THRESHOLD_LOWER -55} {THRESHOLD_UPPER 125} {USER_ALARM_TYPE\
window}}\
     SMON_VOLTAGE_AVERAGING_SAMPLES {None}\
     PMC_CRP_SYSMON_REF_CTRL_ACT_FREQMHZ {299.997009}\
     PMC_CRP_SYSMON_REF_CTRL_FREQMHZ {299.997009}\
     PMC_CRP_SYSMON_REF_CTRL_SRCSEL {NPI_REF_CLK}\
     PS_I2CSYSMON_PERIPHERAL {{ENABLE 0} {IO {PS_MIO 23 .. 24}}}\
     PS_SMON_PL_PORTS_ENABLE {0}\
     SMON_TEMP_THRESHOLD {0}\
     SMON_VAUX_IO_BANK {MIO_BANK0}\
     PMC_QSPI_PERIPHERAL_ENABLE {1}\
     PMC_QSPI_PERIPHERAL_MODE {Dual Parallel}\
     PMC_QSPI_PERIPHERAL_DATA_MODE {x4}\
     PMC_CRP_QSPI_REF_CTRL_FREQMHZ {300}\
     PMC_CRP_QSPI_REF_CTRL_ACT_FREQMHZ {299.997009}\
     PMC_QSPI_FBCLK {{ENABLE 1} {IO {PMC_MIO 6}}}\
     PMC_QSPI_COHERENCY {0}\
     PMC_QSPI_ROUTE_THROUGH_FPD {0}\
     PMC_CRP_QSPI_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_QSPI_REF_CTRL_DIVISOR0 {4}\
     PMC_REF_CLK_FREQMHZ {33.333}\
     PMC_OSPI_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 0 .. 11}} {MODE Single}}\
     PMC_CRP_OSPI_REF_CTRL_FREQMHZ {200}\
     PMC_CRP_OSPI_REF_CTRL_ACT_FREQMHZ {200}\
     PMC_OSPI_COHERENCY {0}\
     PMC_OSPI_ROUTE_THROUGH_FPD {0}\
     PMC_CRP_OSPI_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_OSPI_REF_CTRL_DIVISOR0 {4}\
     PMC_SD0_PERIPHERAL {{CLK_100_SDR_OTAP_DLY 0x00} {CLK_200_SDR_OTAP_DLY 0x00}\
{CLK_50_DDR_ITAP_DLY 0x00} {CLK_50_DDR_OTAP_DLY 0x00}\
{CLK_50_SDR_ITAP_DLY 0x00} {CLK_50_SDR_OTAP_DLY 0x00} {ENABLE\
0} {IO {PMC_MIO 13 .. 25}}}\
     PMC_SD0_SLOT_TYPE {SD 2.0}\
     PMC_SD0_COHERENCY {0}\
     PMC_SD0_ROUTE_THROUGH_FPD {0}\
     PMC_SD0_DATA_TRANSFER_MODE {4Bit}\
     PMC_SD0_SPEED_MODE {default speed}\
     PMC_CRP_SDIO0_REF_CTRL_FREQMHZ {200}\
     PMC_CRP_SDIO0_REF_CTRL_ACT_FREQMHZ {200}\
     PMC_CRP_SDIO0_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_SDIO0_REF_CTRL_DIVISOR0 {6}\
     PMC_SD1_PERIPHERAL {{CLK_100_SDR_OTAP_DLY 0x3} {CLK_200_SDR_OTAP_DLY 0x2}\
{CLK_50_DDR_ITAP_DLY 0x36} {CLK_50_DDR_OTAP_DLY 0x3}\
{CLK_50_SDR_ITAP_DLY 0x2C} {CLK_50_SDR_OTAP_DLY 0x4} {ENABLE\
1} {IO {PMC_MIO 26 .. 36}}}\
     PMC_SD1_SLOT_TYPE {SD 3.0}\
     PMC_SD1_COHERENCY {0}\
     PMC_SD1_ROUTE_THROUGH_FPD {0}\
     PMC_SD1_DATA_TRANSFER_MODE {8Bit}\
     PMC_SD1_SPEED_MODE {high speed}\
     PMC_CRP_SDIO1_REF_CTRL_FREQMHZ {200}\
     PMC_CRP_SDIO1_REF_CTRL_ACT_FREQMHZ {199.998001}\
     PMC_CRP_SDIO1_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_CPM_TOPSW_REF_CTRL_FREQMHZ {475}\
     PS_CRL_CPM_TOPSW_REF_CTRL_ACT_FREQMHZ {474.995239}\
     PS_CRL_CPM_TOPSW_REF_CTRL_SRCSEL {NPLL}\
     PS_CRL_CPM_TOPSW_REF_CTRL_DIVISOR0 {1}\
     PMC_CRP_SDIO1_REF_CTRL_DIVISOR0 {6}\
     PMC_SMAP_PERIPHERAL {{ENABLE 0} {IO {32 Bit}}}\
     BOOT_SECONDARY_PCIE_ENABLE {0}\
     USE_UART0_IN_DEVICE_BOOT {0}\
     PMC_SD0 {{CD_ENABLE 0} {CD_IO {PMC_MIO 24}} {POW_ENABLE 0} {POW_IO {PMC_MIO 17}}\
{RESET_ENABLE 0} {RESET_IO {PMC_MIO 17}} {WP_ENABLE 0} {WP_IO {PMC_MIO\
25}}}\
     PMC_SD1 {{CD_ENABLE 1} {CD_IO {PMC_MIO 28}} {POW_ENABLE 1} {POW_IO {PMC_MIO 51}}\
{RESET_ENABLE 0} {RESET_IO {PMC_MIO 12}} {WP_ENABLE 0} {WP_IO {PMC_MIO\
1}}}\
     PS_USB3_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 13 .. 25}}}\
     PMC_I2CPMC_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 46 .. 47}}}\
     PS_SPI0 {{GRP_SS0_ENABLE 0} {GRP_SS0_IO {PMC_MIO 15}} {GRP_SS1_ENABLE 0}\
{GRP_SS1_IO {PMC_MIO 14}} {GRP_SS2_ENABLE 0} {GRP_SS2_IO {PMC_MIO 13}}\
{PERIPHERAL_ENABLE 0} {PERIPHERAL_IO {PMC_MIO 12 .. 17}}}\
     PS_SPI1 {{GRP_SS0_ENABLE 0} {GRP_SS0_IO {PS_MIO 9}} {GRP_SS1_ENABLE 0}\
{GRP_SS1_IO {PS_MIO 8}} {GRP_SS2_ENABLE 0} {GRP_SS2_IO {PS_MIO 7}}\
{PERIPHERAL_ENABLE 0} {PERIPHERAL_IO {PS_MIO 6 .. 11}}}\
     PMC_EXTERNAL_TAMPER {{ENABLE 0} {IO NONE}}\
     PMC_TAMPER_EXTMIO_ENABLE {0}\
     PMC_TAMPER_EXTMIO_ERASE_BBRAM {0}\
     PMC_TAMPER_EXTMIO_RESPONSE {SYS INTERRUPT}\
     PMC_TAMPER_GLITCHDETECT_ENABLE {0}\
     PMC_TAMPER_GLITCHDETECT_ERASE_BBRAM {0}\
     PMC_TAMPER_GLITCHDETECT_RESPONSE {SYS INTERRUPT}\
     PMC_TAMPER_JTAGDETECT_ENABLE {0}\
     PMC_TAMPER_JTAGDETECT_ERASE_BBRAM {0}\
     PMC_TAMPER_JTAGDETECT_RESPONSE {SYS INTERRUPT}\
     PMC_TAMPER_TEMPERATURE_ENABLE {0}\
     PMC_TAMPER_TEMPERATURE_ERASE_BBRAM {0}\
     PMC_TAMPER_TEMPERATURE_RESPONSE {SYS INTERRUPT}\
     PMC_TAMPER_TRIGGER_REGISTER {0}\
     PMC_TAMPER_TRIGGER_ERASE_BBRAM {0}\
     PMC_TAMPER_TRIGGER_RESPONSE {SYS INTERRUPT}\
     PS_LPD_DMA_CHANNEL_ENABLE {{CH0 0} {CH1 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6\
0} {CH7 0}}\
     PS_M_AXI_FPD_DATA_WIDTH {32}\
     PS_M_AXI_LPD_DATA_WIDTH {128}\
     PS_NUM_FABRIC_RESETS {1}\
     PS_S_AXI_FPD_DATA_WIDTH {128}\
     PS_S_AXI_GP2_DATA_WIDTH {128}\
     PS_USE_ENET0_PTP {0}\
     PS_USE_ENET1_PTP {0}\
     PS_USE_FIFO_ENET0 {0}\
     PS_USE_FIFO_ENET1 {0}\
     PS_USE_M_AXI_FPD {1}\
     PS_USE_M_AXI_LPD {0}\
     PS_USE_S_AXI_ACE {0}\
     PS_USE_S_ACP_FPD {0}\
     PS_USE_S_AXI_FPD {0}\
     PS_USE_S_AXI_GP2 {0}\
     PS_USE_S_AXI_LPD {0}\
     PS_GEN_IPI0_ENABLE {1}\
     PS_GEN_IPI1_ENABLE {1}\
     PS_GEN_IPI2_ENABLE {1}\
     PS_GEN_IPI3_ENABLE {1}\
     PS_GEN_IPI4_ENABLE {1}\
     PS_GEN_IPI5_ENABLE {1}\
     PS_GEN_IPI6_ENABLE {1}\
     PS_GEN_IPI_PMCNOBUF_ENABLE {1}\
     PS_GEN_IPI_PMC_ENABLE {1}\
     PS_GEN_IPI_PSM_ENABLE {1}\
     PS_GEN_IPI0_MASTER {A72}\
     PS_GEN_IPI1_MASTER {A72}\
     PS_GEN_IPI2_MASTER {A72}\
     PS_GEN_IPI3_MASTER {A72}\
     PS_GEN_IPI4_MASTER {A72}\
     PS_GEN_IPI5_MASTER {A72}\
     PS_GEN_IPI6_MASTER {A72}\
     PS_GEN_IPI_PMCNOBUF_MASTER {PMC}\
     PS_GEN_IPI_PMC_MASTER {PMC}\
     PS_GEN_IPI_PSM_MASTER {PSM}\
     PS_USE_APU_INTERRUPT {0}\
     PS_USE_FTM_GPI {0}\
     PS_IRQ_USAGE {{CH0 0} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0} {CH15\
0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 0} {CH9 0}}\
     PS_USE_APU_EVENT_BUS {0}\
     PS_USE_PSPL_IRQ_FPD {0}\
     PS_USE_PSPL_IRQ_LPD {0}\
     PS_USE_PSPL_IRQ_PMC {0}\
     PS_USE_RPU_EVENT {0}\
     PS_USE_RPU_INTERRUPT {0}\
     PMC_USE_NOC_PMC_AXI0 {0}\
     PMC_USE_PMC_NOC_AXI0 {1}\
     PS_USE_NOC_FPD_CCI0 {0}\
     PS_USE_NOC_FPD_CCI1 {0}\
     PS_USE_NOC_FPD_AXI0 {0}\
     PS_USE_NOC_FPD_AXI1 {0}\
     PS_USE_FPD_CCI_NOC {1}\
     PS_USE_FPD_CCI_NOC0 {0}\
     PS_USE_FPD_CCI_NOC1 {0}\
     PS_USE_FPD_CCI_NOC2 {0}\
     PS_USE_FPD_CCI_NOC3 {0}\
     PS_USE_FPD_AXI_NOC0 {1}\
     PS_USE_FPD_AXI_NOC1 {1}\
     PS_USE_NOC_LPD_AXI0 {1}\
     AURORA_LINE_RATE_GPBS {10.0}\
     DIS_AUTO_POL_CHECK {0}\
     GT_REFCLK_MHZ {156.25}\
     INIT_CLK_MHZ {125}\
     INV_POLARITY {0}\
     PS_FTM_CTI_IN0 {0}\
     PS_FTM_CTI_IN1 {0}\
     PS_FTM_CTI_IN2 {0}\
     PS_FTM_CTI_IN3 {0}\
     PS_FTM_CTI_OUT0 {0}\
     PS_FTM_CTI_OUT1 {0}\
     PS_FTM_CTI_OUT2 {0}\
     PS_FTM_CTI_OUT3 {0}\
     PS_HSDP0_REFCLK {0}\
     PS_HSDP1_REFCLK {0}\
     PS_HSDP_EGRESS_TRAFFIC {JTAG}\
     PS_HSDP_INGRESS_TRAFFIC {JTAG}\
     PS_HSDP_MODE {NONE}\
     PS_TRACE_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 30 .. 47}}}\
     PS_TRACE_WIDTH {2Bit}\
     PS_USE_BSCAN_USER1 {0}\
     PS_USE_BSCAN_USER2 {0}\
     PS_USE_BSCAN_USER3 {0}\
     PS_USE_BSCAN_USER4 {0}\
     PS_USE_CAPTURE {0}\
     PS_USE_STM {0}\
     PS_USE_TRACE_ATB {0}\
     PMC_CRP_CFU_REF_CTRL_ACT_FREQMHZ {299.997009}\
     PMC_CRP_CFU_REF_CTRL_DIVISOR0 {4}\
     PMC_CRP_CFU_REF_CTRL_FREQMHZ {300}\
     PMC_CRP_CFU_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_DFT_OSC_REF_CTRL_ACT_FREQMHZ {400}\
     PMC_CRP_DFT_OSC_REF_CTRL_DIVISOR0 {3}\
     PMC_CRP_DFT_OSC_REF_CTRL_FREQMHZ {400}\
     PMC_CRP_DFT_OSC_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_EFUSE_REF_CTRL_ACT_FREQMHZ {100.000000}\
     PMC_CRP_EFUSE_REF_CTRL_FREQMHZ {100.000000}\
     PMC_CRP_EFUSE_REF_CTRL_SRCSEL {IRO_CLK/4}\
     PMC_CRP_HSM0_REF_CTRL_ACT_FREQMHZ {33.333000}\
     PMC_CRP_HSM0_REF_CTRL_DIVISOR0 {36}\
     PMC_CRP_HSM0_REF_CTRL_FREQMHZ {33.333}\
     PMC_CRP_HSM0_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_HSM1_REF_CTRL_ACT_FREQMHZ {133.332001}\
     PMC_CRP_HSM1_REF_CTRL_DIVISOR0 {9}\
     PMC_CRP_HSM1_REF_CTRL_FREQMHZ {133.333}\
     PMC_CRP_HSM1_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_I2C_REF_CTRL_ACT_FREQMHZ {99.999001}\
     PMC_CRP_I2C_REF_CTRL_DIVISOR0 {12}\
     PMC_CRP_I2C_REF_CTRL_FREQMHZ {100}\
     PMC_CRP_I2C_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_LSBUS_REF_CTRL_ACT_FREQMHZ {99.999001}\
     PMC_CRP_LSBUS_REF_CTRL_DIVISOR0 {12}\
     PMC_CRP_LSBUS_REF_CTRL_FREQMHZ {100}\
     PMC_CRP_LSBUS_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_NPI_REF_CTRL_ACT_FREQMHZ {299.997009}\
     PMC_CRP_NPI_REF_CTRL_DIVISOR0 {4}\
     PMC_CRP_NPI_REF_CTRL_FREQMHZ {300}\
     PMC_CRP_NPI_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_NPLL_CTRL_CLKOUTDIV {4}\
     PMC_CRP_NPLL_CTRL_FBDIV {114}\
     PMC_CRP_NPLL_CTRL_SRCSEL {REF_CLK}\
     PMC_CRP_NPLL_TO_XPD_CTRL_DIVISOR0 {2}\
     PMC_CRP_PL0_REF_CTRL_ACT_FREQMHZ {49.999500}\
     PMC_CRP_PL0_REF_CTRL_DIVISOR0 {24}\
     PMC_CRP_PL0_REF_CTRL_FREQMHZ {50}\
     PMC_CRP_PL0_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_PL1_REF_CTRL_ACT_FREQMHZ {100}\
     PMC_CRP_PL1_REF_CTRL_DIVISOR0 {3}\
     PMC_CRP_PL1_REF_CTRL_FREQMHZ {334}\
     PMC_CRP_PL1_REF_CTRL_SRCSEL {NPLL}\
     PMC_CRP_PL2_REF_CTRL_ACT_FREQMHZ {100}\
     PMC_CRP_PL2_REF_CTRL_DIVISOR0 {3}\
     PMC_CRP_PL2_REF_CTRL_FREQMHZ {334}\
     PMC_CRP_PL2_REF_CTRL_SRCSEL {NPLL}\
     PMC_CRP_PL3_REF_CTRL_ACT_FREQMHZ {100}\
     PMC_CRP_PL3_REF_CTRL_DIVISOR0 {3}\
     PMC_CRP_PL3_REF_CTRL_FREQMHZ {334}\
     PMC_CRP_PL3_REF_CTRL_SRCSEL {NPLL}\
     PMC_CRP_PPLL_CTRL_CLKOUTDIV {2}\
     PMC_CRP_PPLL_CTRL_FBDIV {72}\
     PMC_CRP_PPLL_CTRL_SRCSEL {REF_CLK}\
     PMC_CRP_PPLL_TO_XPD_CTRL_DIVISOR0 {4}\
     PMC_CRP_SD_DLL_REF_CTRL_ACT_FREQMHZ {1199.988037}\
     PMC_CRP_SD_DLL_REF_CTRL_DIVISOR0 {1}\
     PMC_CRP_SD_DLL_REF_CTRL_FREQMHZ {1200}\
     PMC_CRP_SD_DLL_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_SWITCH_TIMEOUT_CTRL_ACT_FREQMHZ {1.000000}\
     PMC_CRP_SWITCH_TIMEOUT_CTRL_DIVISOR0 {100}\
     PMC_CRP_SWITCH_TIMEOUT_CTRL_FREQMHZ {1}\
     PMC_CRP_SWITCH_TIMEOUT_CTRL_SRCSEL {IRO_CLK/4}\
     PMC_CRP_TEST_PATTERN_REF_CTRL_ACT_FREQMHZ {200}\
     PMC_CRP_TEST_PATTERN_REF_CTRL_DIVISOR0 {6}\
     PMC_CRP_TEST_PATTERN_REF_CTRL_FREQMHZ {200}\
     PMC_CRP_TEST_PATTERN_REF_CTRL_SRCSEL {PPLL}\
     PMC_CRP_USB_SUSPEND_CTRL_ACT_FREQMHZ {0.200000}\
     PMC_CRP_USB_SUSPEND_CTRL_DIVISOR0 {500}\
     PMC_CRP_USB_SUSPEND_CTRL_FREQMHZ {0.2}\
     PMC_CRP_USB_SUSPEND_CTRL_SRCSEL {IRO_CLK/4}\
     PSPMC_MANUAL_CLK_ENABLE {0}\
     PS_CRF_ACPU_CTRL_ACT_FREQMHZ {999.989990}\
     PS_CRF_ACPU_CTRL_DIVISOR0 {1}\
     PS_CRF_ACPU_CTRL_FREQMHZ {1000}\
     PS_CRF_ACPU_CTRL_SRCSEL {APLL}\
     PS_CRF_APLL_CTRL_CLKOUTDIV {4}\
     PS_CRF_APLL_CTRL_FBDIV {120}\
     PS_CRF_APLL_CTRL_SRCSEL {REF_CLK}\
     PS_CRF_APLL_TO_XPD_CTRL_DIVISOR0 {1}\
     PS_CRF_DBG_FPD_CTRL_ACT_FREQMHZ {299.997009}\
     PS_CRF_DBG_FPD_CTRL_DIVISOR0 {1}\
     PS_CRF_DBG_FPD_CTRL_FREQMHZ {300}\
     PS_CRF_DBG_FPD_CTRL_SRCSEL {PPLL}\
     PS_CRF_DBG_TRACE_CTRL_ACT_FREQMHZ {300}\
     PS_CRF_DBG_TRACE_CTRL_DIVISOR0 {3}\
     PS_CRF_DBG_TRACE_CTRL_FREQMHZ {300}\
     PS_CRF_DBG_TRACE_CTRL_SRCSEL {PPLL}\
     PS_CRF_FPD_LSBUS_CTRL_ACT_FREQMHZ {99.999001}\
     PS_CRF_FPD_LSBUS_CTRL_DIVISOR0 {3}\
     PS_CRF_FPD_LSBUS_CTRL_FREQMHZ {100}\
     PS_CRF_FPD_LSBUS_CTRL_SRCSEL {PPLL}\
     PS_CRF_FPD_TOP_SWITCH_CTRL_ACT_FREQMHZ {499.994995}\
     PS_CRF_FPD_TOP_SWITCH_CTRL_DIVISOR0 {2}\
     PS_CRF_FPD_TOP_SWITCH_CTRL_FREQMHZ {500}\
     PS_CRF_FPD_TOP_SWITCH_CTRL_SRCSEL {APLL}\
     PS_CRL_CAN0_REF_CTRL_ACT_FREQMHZ {100}\
     PS_CRL_CAN0_REF_CTRL_DIVISOR0 {12}\
     PS_CRL_CAN0_REF_CTRL_FREQMHZ {100}\
     PS_CRL_CAN0_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_CAN1_REF_CTRL_ACT_FREQMHZ {149.998505}\
     PS_CRL_CAN1_REF_CTRL_DIVISOR0 {2}\
     PS_CRL_CAN1_REF_CTRL_FREQMHZ {150}\
     PS_CRL_CAN1_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_CPU_R5_CTRL_ACT_FREQMHZ {374.996246}\
     PS_CRL_CPU_R5_CTRL_DIVISOR0 {2}\
     PS_CRL_CPU_R5_CTRL_FREQMHZ {375}\
     PS_CRL_CPU_R5_CTRL_SRCSEL {RPLL}\
     PS_CRL_DBG_LPD_CTRL_ACT_FREQMHZ {299.997009}\
     PS_CRL_DBG_LPD_CTRL_DIVISOR0 {1}\
     PS_CRL_DBG_LPD_CTRL_FREQMHZ {300}\
     PS_CRL_DBG_LPD_CTRL_SRCSEL {PPLL}\
     PS_CRL_DBG_TSTMP_CTRL_ACT_FREQMHZ {299.997009}\
     PS_CRL_DBG_TSTMP_CTRL_DIVISOR0 {1}\
     PS_CRL_DBG_TSTMP_CTRL_FREQMHZ {300}\
     PS_CRL_DBG_TSTMP_CTRL_SRCSEL {PPLL}\
     PS_CRL_GEM0_REF_CTRL_ACT_FREQMHZ {124.998749}\
     PS_CRL_GEM0_REF_CTRL_DIVISOR0 {6}\
     PS_CRL_GEM0_REF_CTRL_FREQMHZ {125}\
     PS_CRL_GEM0_REF_CTRL_SRCSEL {RPLL}\
     PS_CRL_GEM1_REF_CTRL_ACT_FREQMHZ {124.998749}\
     PS_CRL_GEM1_REF_CTRL_DIVISOR0 {6}\
     PS_CRL_GEM1_REF_CTRL_FREQMHZ {125}\
     PS_CRL_GEM1_REF_CTRL_SRCSEL {RPLL}\
     PS_CRL_GEM_TSU_REF_CTRL_ACT_FREQMHZ {249.997498}\
     PS_CRL_GEM_TSU_REF_CTRL_DIVISOR0 {3}\
     PS_CRL_GEM_TSU_REF_CTRL_FREQMHZ {250}\
     PS_CRL_GEM_TSU_REF_CTRL_SRCSEL {RPLL}\
     PS_CRL_I2C0_REF_CTRL_ACT_FREQMHZ {100}\
     PS_CRL_I2C0_REF_CTRL_DIVISOR0 {12}\
     PS_CRL_I2C0_REF_CTRL_FREQMHZ {100}\
     PS_CRL_I2C0_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_I2C1_REF_CTRL_ACT_FREQMHZ {99.999001}\
     PS_CRL_I2C1_REF_CTRL_DIVISOR0 {3}\
     PS_CRL_I2C1_REF_CTRL_FREQMHZ {100}\
     PS_CRL_I2C1_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_IOU_SWITCH_CTRL_ACT_FREQMHZ {249.997498}\
     PS_CRL_IOU_SWITCH_CTRL_DIVISOR0 {3}\
     PS_CRL_IOU_SWITCH_CTRL_FREQMHZ {250}\
     PS_CRL_IOU_SWITCH_CTRL_SRCSEL {RPLL}\
     PS_CRL_LPD_LSBUS_CTRL_ACT_FREQMHZ {99.999001}\
     PS_CRL_LPD_LSBUS_CTRL_DIVISOR0 {3}\
     PS_CRL_LPD_LSBUS_CTRL_FREQMHZ {100}\
     PS_CRL_LPD_LSBUS_CTRL_SRCSEL {PPLL}\
     PS_CRL_LPD_TOP_SWITCH_CTRL_ACT_FREQMHZ {374.996246}\
     PS_CRL_LPD_TOP_SWITCH_CTRL_DIVISOR0 {2}\
     PS_CRL_LPD_TOP_SWITCH_CTRL_FREQMHZ {375}\
     PS_CRL_LPD_TOP_SWITCH_CTRL_SRCSEL {RPLL}\
     PS_CRL_PSM_REF_CTRL_ACT_FREQMHZ {299.997009}\
     PS_CRL_PSM_REF_CTRL_DIVISOR0 {1}\
     PS_CRL_PSM_REF_CTRL_FREQMHZ {300}\
     PS_CRL_PSM_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_RPLL_CTRL_CLKOUTDIV {4}\
     PS_CRL_RPLL_CTRL_FBDIV {90}\
     PS_CRL_RPLL_CTRL_SRCSEL {REF_CLK}\
     PS_CRL_RPLL_TO_XPD_CTRL_DIVISOR0 {4}\
     PS_CRL_SPI0_REF_CTRL_ACT_FREQMHZ {200}\
     PS_CRL_SPI0_REF_CTRL_DIVISOR0 {6}\
     PS_CRL_SPI0_REF_CTRL_FREQMHZ {200}\
     PS_CRL_SPI0_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_SPI1_REF_CTRL_ACT_FREQMHZ {200}\
     PS_CRL_SPI1_REF_CTRL_DIVISOR0 {6}\
     PS_CRL_SPI1_REF_CTRL_FREQMHZ {200}\
     PS_CRL_SPI1_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_TIMESTAMP_REF_CTRL_ACT_FREQMHZ {99.999001}\
     PS_CRL_TIMESTAMP_REF_CTRL_DIVISOR0 {3}\
     PS_CRL_TIMESTAMP_REF_CTRL_FREQMHZ {100}\
     PS_CRL_TIMESTAMP_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_UART0_REF_CTRL_ACT_FREQMHZ {99.999001}\
     PS_CRL_UART0_REF_CTRL_DIVISOR0 {3}\
     PS_CRL_UART0_REF_CTRL_FREQMHZ {100}\
     PS_CRL_UART0_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_UART1_REF_CTRL_ACT_FREQMHZ {100}\
     PS_CRL_UART1_REF_CTRL_DIVISOR0 {12}\
     PS_CRL_UART1_REF_CTRL_FREQMHZ {100}\
     PS_CRL_UART1_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_USB0_BUS_REF_CTRL_ACT_FREQMHZ {19.999800}\
     PS_CRL_USB0_BUS_REF_CTRL_DIVISOR0 {15}\
     PS_CRL_USB0_BUS_REF_CTRL_FREQMHZ {20}\
     PS_CRL_USB0_BUS_REF_CTRL_SRCSEL {PPLL}\
     PS_CRL_USB3_DUAL_REF_CTRL_ACT_FREQMHZ {100}\
     PS_CRL_USB3_DUAL_REF_CTRL_DIVISOR0 {100}\
     PS_CRL_USB3_DUAL_REF_CTRL_FREQMHZ {100}\
     PS_CRL_USB3_DUAL_REF_CTRL_SRCSEL {PPLL}\
     PS_TTC0_REF_CTRL_ACT_FREQMHZ {99.999001}\
     PS_TTC0_REF_CTRL_FREQMHZ {99.999001}\
     PS_TTC1_REF_CTRL_ACT_FREQMHZ {100}\
     PS_TTC1_REF_CTRL_FREQMHZ {100}\
     PS_TTC2_REF_CTRL_ACT_FREQMHZ {100}\
     PS_TTC2_REF_CTRL_FREQMHZ {100}\
     PS_TTC3_REF_CTRL_ACT_FREQMHZ {100}\
     PS_TTC3_REF_CTRL_FREQMHZ {100}\
     PS_TTC_APB_CLK_TTC0_SEL {APB}\
     PS_TTC_APB_CLK_TTC1_SEL {APB}\
     PS_TTC_APB_CLK_TTC2_SEL {APB}\
     PS_TTC_APB_CLK_TTC3_SEL {APB}\
     Component_Name {vck190_v1_0_ps_cips_0}\
     PMC_ALT_REF_CLK_FREQMHZ {33.333}\
     PMC_BANK_0_IO_STANDARD {LVCMOS1.8}\
     PMC_BANK_1_IO_STANDARD {LVCMOS1.8}\
     PMC_CIPS_MODE {ADVANCE}\
     PMC_CRP_NOC_REF_CTRL_ACT_FREQMHZ {949.990479}\
     PMC_CRP_NOC_REF_CTRL_FREQMHZ {950}\
     PMC_CRP_NOC_REF_CTRL_SRCSEL {NPLL}\
     PMC_CRP_PL5_REF_CTRL_FREQMHZ {400}\
     PMC_GPIO0_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 0 .. 25}}}\
     PMC_GPIO1_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 51}}}\
     PMC_GPIO_EMIO_PERIPHERAL_ENABLE {0}\
     PMC_GPIO_EMIO_WIDTH {64}\
     PMC_GPIO_EMIO_WIDTH_HDL {64}\
     PMC_HSM0_CLK_ENABLE {1}\
     PMC_HSM1_CLK_ENABLE {1}\
     PMC_MIO_EN_FOR_PL_PCIE {0}\
     PMC_MIO_TREE_PERIPHERALS {QSPI#QSPI#QSPI#QSPI#QSPI#QSPI#Loopback\
Clk#QSPI#QSPI#QSPI#QSPI#QSPI#QSPI#USB 2.0#USB 2.0#USB\
2.0#USB 2.0#USB 2.0#USB 2.0#USB 2.0#USB 2.0#USB 2.0#USB\
2.0#USB 2.0#USB 2.0#USB\
2.0#SD1/eMMC1#SD1/eMMC1#SD1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#GPIO\
1###CANFD1#CANFD1#UART 0#UART\
0#LPD_I2C1#LPD_I2C1#pmc_i2c#pmc_i2c####SD1/eMMC1#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem0#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem1#Gem0#Gem0}\
     PMC_MIO_TREE_SIGNALS {qspi0_clk#qspi0_io[1]#qspi0_io[2]#qspi0_io[3]#qspi0_io[0]#qspi0_cs_b#qspi_lpbk#qspi1_cs_b#qspi1_io[0]#qspi1_io[1]#qspi1_io[2]#qspi1_io[3]#qspi1_clk#usb2phy_reset#ulpi_tx_data[0]#ulpi_tx_data[1]#ulpi_tx_data[2]#ulpi_tx_data[3]#ulpi_clk#ulpi_tx_data[4]#ulpi_tx_data[5]#ulpi_tx_data[6]#ulpi_tx_data[7]#ulpi_dir#ulpi_stp#ulpi_nxt#clk#dir1/data[7]#detect#cmd#data[0]#data[1]#data[2]#data[3]#sel/data[4]#dir_cmd/data[5]#dir0/data[6]#gpio_1_pin[37]###phy_tx#phy_rx#rxd#txd#scl#sda#scl#sda####buspwr/rst#rgmii_tx_clk#rgmii_txd[0]#rgmii_txd[1]#rgmii_txd[2]#rgmii_txd[3]#rgmii_tx_ctl#rgmii_rx_clk#rgmii_rxd[0]#rgmii_rxd[1]#rgmii_rxd[2]#rgmii_rxd[3]#rgmii_rx_ctl#rgmii_tx_clk#rgmii_txd[0]#rgmii_txd[1]#rgmii_txd[2]#rgmii_txd[3]#rgmii_tx_ctl#rgmii_rx_clk#rgmii_rxd[0]#rgmii_rxd[1]#rgmii_rxd[2]#rgmii_rxd[3]#rgmii_rx_ctl#gem0_mdc#gem0_mdio}\
     PMC_PL_ALT_REF_CLK_FREQMHZ {33.333}\
     PMC_SHOW_CCI_SMMU_SETTINGS {0}\
     PMC_USE_CFU_SEU {0}\
     PMC_USE_PL_PMC_AUX_REF_CLK {0}\
     PS_LPD_DMA_CH_TZ {{CH0 NonSecure} {CH1 NonSecure} {CH2 NonSecure} {CH3 NonSecure}\
{CH4 NonSecure} {CH5 NonSecure} {CH6 NonSecure} {CH7 NonSecure}}\
     PS_LPD_DMA_ENABLE {0}\
     PS_BANK_2_IO_STANDARD {LVCMOS1.8}\
     PS_BANK_3_IO_STANDARD {LVCMOS1.8}\
     PS_CAN0_CLK {{ENABLE 0} {IO {PMC_MIO 0}}}\
     PS_CAN0_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 8 .. 9}}}\
     PS_CAN1_CLK {{ENABLE 0} {IO {PMC_MIO 0}}}\
     PS_CAN1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 40 .. 41}}}\
     PS_DDRC_ENABLE {1}\
     PS_DDR_RAM_HIGHADDR_OFFSET {34359738368}\
     PS_DDR_RAM_LOWADDR_OFFSET {2147483648}\
     PS_ENET0_MDIO {{ENABLE 1} {IO {PS_MIO 24 .. 25}}}\
     PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 .. 11}}}\
     PS_ENET1_MDIO {{ENABLE 0} {IO {PMC_MIO 50 .. 51}}}\
     PS_ENET1_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 12 .. 23}}}\
     PS_EN_AXI_STATUS_PORTS {0}\
     PS_EN_PORTS_CONTROLLER_BASED {0}\
     PS_EXPAND_CORESIGHT {0}\
     PS_EXPAND_FPD_SLAVES {0}\
     PS_EXPAND_GIC {0}\
     PS_EXPAND_LPD_SLAVES {0}\
     PS_GEM0_COHERENCY {0}\
     PS_GEM0_ROUTE_THROUGH_FPD {1}\
     PS_GEM1_COHERENCY {0}\
     PS_GEM1_ROUTE_THROUGH_FPD {0}\
     PS_GEM_TSU_CLK_PORT_PAIR {0}\
     PS_GEM_TSU {{ENABLE 0} {IO {PS_MIO 24}}}\
     PS_GPIO2_MIO_PERIPHERAL {{ENABLE 0} {IO {PS_MIO 0 .. 25}}}\
     PS_GPIO_EMIO_PERIPHERAL_ENABLE {0}\
     PS_GPIO_EMIO_WIDTH {32}\
     PS_HSDP_SAME_EGRESS_AS_INGRESS_TRAFFIC {1}\
     PS_I2C0_PERIPHERAL {{ENABLE 0} {IO {PS_MIO 2 .. 3}}}\
     PS_I2C1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 44 .. 45}}}\
     PS_LPDMA0_COHERENCY {0}\
     PS_LPDMA0_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA1_COHERENCY {0}\
     PS_LPDMA1_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA2_COHERENCY {0}\
     PS_LPDMA2_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA3_COHERENCY {0}\
     PS_LPDMA3_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA4_COHERENCY {0}\
     PS_LPDMA4_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA5_COHERENCY {0}\
     PS_LPDMA5_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA6_COHERENCY {0}\
     PS_LPDMA6_ROUTE_THROUGH_FPD {0}\
     PS_LPDMA7_COHERENCY {0}\
     PS_LPDMA7_ROUTE_THROUGH_FPD {0}\
     PS_M_AXI_GP4_DATA_WIDTH {128}\
     PS_NOC_PS_CCI_DATA_WIDTH {128}\
     PS_NOC_PS_NCI_DATA_WIDTH {128}\
     PS_NOC_PS_PCI_DATA_WIDTH {128}\
     PS_NOC_PS_PMC_DATA_WIDTH {128}\
     PS_NUM_F2P0_INTR_INPUTS {1}\
     PS_NUM_F2P1_INTR_INPUTS {1}\
     PS_PCIE_EP_RESET1_IO {None}\
     PS_PCIE_EP_RESET2_IO {None}\
     PS_PCIE1_PERIPHERAL_ENABLE {0}\
     PS_PCIE2_PERIPHERAL_ENABLE {0}\
     PS_PCIE_PERIPHERAL_ENABLE {0}\
     PS_PCIE_RESET {{ENABLE 1}}\
     PS_PCIE_ROOT_RESET1_IO {None}\
     PS_PCIE_ROOT_RESET1_POLARITY {Active Low}\
     PS_PCIE_ROOT_RESET2_IO {None}\
     PS_PCIE_ROOT_RESET2_POLARITY {Active Low}\
     PS_PL_DONE {0}\
     PS_PMCPL_CLK0_BUF {1}\
     PS_PMCPL_CLK1_BUF {1}\
     PS_PMCPL_CLK2_BUF {1}\
     PS_PMCPL_CLK3_BUF {1}\
     PS_PMCPL_IRO_CLK_BUF {1}\
     PS_PMU_PERIPHERAL_ENABLE {0}\
     PS_PS_ENABLE {0}\
     PS_PS_NOC_CCI_DATA_WIDTH {128}\
     PS_PS_NOC_NCI_DATA_WIDTH {128}\
     PS_PS_NOC_PCI_DATA_WIDTH {128}\
     PS_PS_NOC_PMC_DATA_WIDTH {128}\
     PS_PS_NOC_RPU_DATA_WIDTH {128}\
     PS_RPU_COHERENCY {0}\
     PS_SLR_TYPE {master}\
     PS_S_AXI_ACE_DATA_WIDTH {128}\
     PS_S_AXI_ACP_DATA_WIDTH {128}\
     PS_S_AXI_LPD_DATA_WIDTH {128}\
     PS_TRISTATE_INVERTED {0}\
     PS_TTC0_CLK {{ENABLE 0} {IO {PS_MIO 6}}}\
     PS_TTC0_PERIPHERAL_ENABLE {1}\
     PS_TTC0_WAVEOUT {{ENABLE 0} {IO {PS_MIO 7}}}\
     PS_TTC1_CLK {{ENABLE 0} {IO {PS_MIO 12}}}\
     PS_TTC1_PERIPHERAL_ENABLE {0}\
     PS_TTC1_WAVEOUT {{ENABLE 0} {IO {PS_MIO 13}}}\
     PS_TTC2_CLK {{ENABLE 0} {IO {PS_MIO 2}}}\
     PS_TTC2_PERIPHERAL_ENABLE {0}\
     PS_TTC2_WAVEOUT {{ENABLE 0} {IO {PS_MIO 3}}}\
     PS_TTC3_CLK {{ENABLE 0} {IO {PS_MIO 16}}}\
     PS_TTC3_PERIPHERAL_ENABLE {0}\
     PS_TTC3_WAVEOUT {{ENABLE 0} {IO {PS_MIO 17}}}\
     PS_UART0_BAUD_RATE {115200}\
     PS_UART0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 42 .. 43}}}\
     PS_UART0_RTS_CTS {{ENABLE 0} {IO {PS_MIO 2 .. 3}}}\
     PS_UART1_BAUD_RATE {115200}\
     PS_UART1_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 4 .. 5}}}\
     PS_UART1_RTS_CTS {{ENABLE 0} {IO {PMC_MIO 6 .. 7}}}\
     PS_USB_COHERENCY {0}\
     PS_USB_ROUTE_THROUGH_FPD {0}\
     PS_USE_ACE_LITE {0}\
     PS_USE_AXI4_EXT_USER_BITS {0}\
     PS_USE_CLK {0}\
     PS_USE_DEBUG_TEST {0}\
     PS_USE_DIFF_RW_CLK_S_AXI_FPD {0}\
     PS_USE_DIFF_RW_CLK_S_AXI_GP2 {0}\
     PS_USE_DIFF_RW_CLK_S_AXI_LPD {0}\
     PS_USE_FTM_GPO {0}\
     PS_USE_HSDP_PL {0}\
     PS_USE_NOC_PS_PCI_0 {0}\
     PS_USE_NOC_PS_PMC_0 {0}\
     PS_USE_NPI_CLK {0}\
     PS_USE_NPI_RST {0}\
     PS_USE_PL_FPD_AUX_REF_CLK {0}\
     PS_USE_PL_LPD_AUX_REF_CLK {0}\
     PS_USE_PMC {0}\
     PS_USE_PMCPL_CLK0 {1}\
     PS_USE_PMCPL_CLK1 {0}\
     PS_USE_PMCPL_CLK2 {0}\
     PS_USE_PMCPL_CLK3 {0}\
     PS_USE_PMCPL_IRO_CLK {0}\
     PS_USE_PS_NOC_PCI_0 {0}\
     PS_USE_PS_NOC_PCI_1 {0}\
     PS_USE_PS_NOC_PMC_0 {0}\
     PS_USE_PS_NOC_PMC_1 {0}\
     PS_USE_RTC {0}\
     PS_USE_SMMU {0}\
     PS_USE_STARTUP {0}\
     PS_WDT0_REF_CTRL_ACT_FREQMHZ {100}\
     PS_WDT0_REF_CTRL_FREQMHZ {100}\
     PS_WDT0_REF_CTRL_SEL {NONE}\
     PS_WDT1_REF_CTRL_ACT_FREQMHZ {100}\
     PS_WDT1_REF_CTRL_FREQMHZ {100}\
     PS_WDT1_REF_CTRL_SEL {NONE}\
     PS_WWDT0_CLK {{ENABLE 0} {IO {PMC_MIO 0}}}\
     PS_WWDT0_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 0 .. 5}}}\
     PS_WWDT1_CLK {{ENABLE 0} {IO {PMC_MIO 6}}}\
     PS_WWDT1_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 6 .. 11}}}\
     SEM_MEM_SCAN {0}\
     SEM_NPI_SCAN {0}\
     PL_SEM_GPIO_ENABLE {0}\
     SEM_ERROR_HANDLE_OPTIONS {Detect & Correct}\
     SEM_EVENT_LOG_OPTIONS {Log & Notify}\
     SEM_MEM_BUILT_IN_SELF_TEST {0}\
     SEM_MEM_ENABLE_ALL_TEST_FEATURE {0}\
     SEM_MEM_ENABLE_SCAN_AFTER {0}\
     SEM_MEM_GOLDEN_ECC {0}\
     SEM_MEM_GOLDEN_ECC_SW {0}\
     SEM_NPI_BUILT_IN_SELF_TEST {0}\
     SEM_NPI_ENABLE_ALL_TEST_FEATURE {0}\
     SEM_NPI_ENABLE_SCAN_AFTER {0}\
     SEM_NPI_GOLDEN_CHECKSUM_SW {0}\
     SEM_TIME_INTERVAL_BETWEEN_SCANS {0}\
     SPP_PSPMC_FROM_CORE_WIDTH {12000}\
     SPP_PSPMC_TO_CORE_WIDTH {12000}\
     DEVICE_INTEGRITY_MODE {Custom}\
     PMC_PMC_NOC_ADDR_WIDTH {64}\
     PMC_PMC_NOC_DATA_WIDTH {128}\
     PMC_NOC_PMC_ADDR_WIDTH {64}\
     PMC_NOC_PMC_DATA_WIDTH {128}\
     SMON_MEAS0 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_103} {SUPPLY_NUM 0}}\
     SMON_MEAS1 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_104} {SUPPLY_NUM 0}}\
     SMON_MEAS2 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_105} {SUPPLY_NUM 0}}\
     SMON_MEAS3 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_106} {SUPPLY_NUM 0}}\
     SMON_MEAS4 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_200} {SUPPLY_NUM 0}}\
     SMON_MEAS5 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_201} {SUPPLY_NUM 0}}\
     SMON_MEAS6 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_202} {SUPPLY_NUM 0}}\
     SMON_MEAS7 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_203} {SUPPLY_NUM 0}}\
     SMON_MEAS8 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_204} {SUPPLY_NUM 0}}\
     SMON_MEAS9 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN 0}\
{ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_205} {SUPPLY_NUM 0}}\
     SMON_MEAS10 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCCAUX_206}\
{SUPPLY_NUM 0}}\
     SMON_MEAS11 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_103} {SUPPLY_NUM\
0}}\
     SMON_MEAS12 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_104} {SUPPLY_NUM\
0}}\
     SMON_MEAS13 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_105} {SUPPLY_NUM\
0}}\
     SMON_MEAS14 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_106} {SUPPLY_NUM\
0}}\
     SMON_MEAS15 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_200} {SUPPLY_NUM\
0}}\
     SMON_MEAS16 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_201} {SUPPLY_NUM\
0}}\
     SMON_MEAS17 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_202} {SUPPLY_NUM\
0}}\
     SMON_MEAS18 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_203} {SUPPLY_NUM\
0}}\
     SMON_MEAS19 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_204} {SUPPLY_NUM\
0}}\
     SMON_MEAS20 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_205} {SUPPLY_NUM\
0}}\
     SMON_MEAS21 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVCC_206} {SUPPLY_NUM\
0}}\
     SMON_MEAS22 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_103} {SUPPLY_NUM\
0}}\
     SMON_MEAS23 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_104} {SUPPLY_NUM\
0}}\
     SMON_MEAS24 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_105} {SUPPLY_NUM\
0}}\
     SMON_MEAS25 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_106} {SUPPLY_NUM\
0}}\
     SMON_MEAS26 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_200} {SUPPLY_NUM\
0}}\
     SMON_MEAS27 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_201} {SUPPLY_NUM\
0}}\
     SMON_MEAS28 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_202} {SUPPLY_NUM\
0}}\
     SMON_MEAS29 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_203} {SUPPLY_NUM\
0}}\
     SMON_MEAS30 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_204} {SUPPLY_NUM\
0}}\
     SMON_MEAS31 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_205} {SUPPLY_NUM\
0}}\
     SMON_MEAS32 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME GTY_AVTT_206} {SUPPLY_NUM\
0}}\
     SMON_MEAS33 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCAUX} {SUPPLY_NUM 0}}\
     SMON_MEAS34 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCAUX_PMC} {SUPPLY_NUM 0}}\
     SMON_MEAS35 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCAUX_SMON} {SUPPLY_NUM 0}}\
     SMON_MEAS36 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCINT} {SUPPLY_NUM 0}}\
     SMON_MEAS37 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {4 V unipolar}} {NAME VCCO_306} {SUPPLY_NUM 0}}\
     SMON_MEAS38 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {4 V unipolar}} {NAME VCCO_406} {SUPPLY_NUM 0}}\
     SMON_MEAS39 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {4 V unipolar}} {NAME VCCO_500} {SUPPLY_NUM 0}}\
     SMON_MEAS40 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {2 V unipolar}} {NAME VCCO_501} {SUPPLY_NUM 1}}\
     SMON_MEAS41 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {4 V unipolar}} {NAME VCCO_502} {SUPPLY_NUM 2}}\
     SMON_MEAS42 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {4 V unipolar}} {NAME VCCO_503} {SUPPLY_NUM 3}}\
     SMON_MEAS43 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_700} {SUPPLY_NUM 0}}\
     SMON_MEAS44 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_701} {SUPPLY_NUM 0}}\
     SMON_MEAS45 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_702} {SUPPLY_NUM 0}}\
     SMON_MEAS46 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_703} {SUPPLY_NUM 0}}\
     SMON_MEAS47 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_704} {SUPPLY_NUM 0}}\
     SMON_MEAS48 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_705} {SUPPLY_NUM 0}}\
     SMON_MEAS49 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_706} {SUPPLY_NUM 0}}\
     SMON_MEAS50 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_707} {SUPPLY_NUM 0}}\
     SMON_MEAS51 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_708} {SUPPLY_NUM 0}}\
     SMON_MEAS52 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_709} {SUPPLY_NUM 0}}\
     SMON_MEAS53 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_710} {SUPPLY_NUM 0}}\
     SMON_MEAS54 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCO_711} {SUPPLY_NUM 0}}\
     SMON_MEAS55 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_BATT} {SUPPLY_NUM 0}}\
     SMON_MEAS56 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_PMC} {SUPPLY_NUM 0}}\
     SMON_MEAS57 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_PSFP} {SUPPLY_NUM 0}}\
     SMON_MEAS58 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_PSLP} {SUPPLY_NUM 0}}\
     SMON_MEAS59 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_RAM} {SUPPLY_NUM 0}}\
     SMON_MEAS60 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_SOC} {SUPPLY_NUM 0}}\
     SMON_MEAS61 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {2 V unipolar}} {NAME VP_VN} {SUPPLY_NUM 4}}\
     SMON_MEAS62 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {2 V unipolar}} {NAME VCCO_711} {SUPPLY_NUM 0}}\
     SMON_MEAS63 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {4 V unipolar}} {NAME VCCO_PKG_306} {SUPPLY_NUM\
0}}\
     SMON_MEAS64 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {4 V unipolar}} {NAME VCCO_PKG_406} {SUPPLY_NUM\
0}}\
     SMON_MEAS65 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {2 V unipolar}} {NAME VCC_BATT} {SUPPLY_NUM 0}}\
     SMON_MEAS66 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 1} {MODE {2 V unipolar}} {NAME VCC_IO_700} {SUPPLY_NUM 0}}\
     SMON_MEAS67 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_701} {SUPPLY_NUM 0}}\
     SMON_MEAS68 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_702} {SUPPLY_NUM 0}}\
     SMON_MEAS69 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_703} {SUPPLY_NUM 0}}\
     SMON_MEAS70 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_704} {SUPPLY_NUM 0}}\
     SMON_MEAS71 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_705} {SUPPLY_NUM 0}}\
     SMON_MEAS72 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_706} {SUPPLY_NUM 0}}\
     SMON_MEAS73 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_707} {SUPPLY_NUM 0}}\
     SMON_MEAS74 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_708} {SUPPLY_NUM 0}}\
     SMON_MEAS75 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_709} {SUPPLY_NUM 0}}\
     SMON_MEAS76 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_710} {SUPPLY_NUM 0}}\
     SMON_MEAS77 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_711} {SUPPLY_NUM 0}}\
     SMON_MEAS78 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_PKG_306} {SUPPLY_NUM 0}}\
     SMON_MEAS79 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_IO_PKG_406} {SUPPLY_NUM 0}}\
     SMON_MEAS80 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_PMC} {SUPPLY_NUM 0}}\
     SMON_MEAS81 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_PSFP} {SUPPLY_NUM 0}}\
     SMON_MEAS82 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_PSLP} {SUPPLY_NUM 0}}\
     SMON_MEAS83 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_RAM} {SUPPLY_NUM 0}}\
     SMON_MEAS84 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VCC_SOC} {SUPPLY_NUM 0}}\
     SMON_MEAS85 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME VP_VN} {SUPPLY_NUM 0}}\
     SMON_MEAS86 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS87 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS88 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS89 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS90 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS91 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS92 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS93 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS94 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS95 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS96 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS97 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS98 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS99 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS100 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS101 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS102 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS103 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS104 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS105 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS106 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS107 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS108 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS109 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS110 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS111 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS112 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS113 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS114 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS115 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS116 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS117 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS118 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS119 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS120 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS121 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS122 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS123 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS124 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS125 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS126 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS127 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS128 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS129 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS130 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS131 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS132 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS133 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS134 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS135 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS136 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS137 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS138 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS139 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS140 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS141 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS142 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS143 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS144 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS145 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS146 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS147 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS148 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS149 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS150 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS151 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS152 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS153 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS154 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS155 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS156 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS157 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS158 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS159 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0} {SUPPLY_NUM 0}}\
     SMON_MEAS160 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS161 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS162 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCINT}}\
     SMON_MEAS163 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCAUX}}\
     SMON_MEAS164 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_RAM}}\
     SMON_MEAS165 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_SOC}}\
     SMON_MEAS166 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_PSFP}}\
     SMON_MEAS167 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_PSLP}}\
     SMON_MEAS168 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCCAUX_PMC}}\
     SMON_MEAS169 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE {2 V unipolar}} {NAME VCC_PMC}}\
     SMON_MEAS170 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS171 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS172 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS173 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS174 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_MEAS175 {{ALARM_ENABLE 0} {ALARM_LOWER 0.00} {ALARM_UPPER 2.00} {AVERAGE_EN\
0} {ENABLE 0} {MODE None} {NAME 0}}\
     SMON_VAUX_CH0 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH0} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH1 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH1} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH2 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH2} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH3 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH3} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH4 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH4} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH5 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH5} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH6 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH6} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH7 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH7} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH8 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH8} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH9 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH9} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH10 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH10} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH11 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH11} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH12 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH12} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH13 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH13} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH14 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH14} {SUPPLY_NUM 0}}\
     SMON_VAUX_CH15 {{ALARM_ENABLE 0} {ALARM_LOWER 0} {ALARM_UPPER 0} {AVERAGE_EN 0}\
{ENABLE 0} {IO_N PMC_MIO1_500} {IO_P PMC_MIO0_500} {MODE {1 V\
unipolar}} {NAME VAUX_CH15} {SUPPLY_NUM 0}}\
     PMC_MIO0 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO1 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO2 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO3 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO4 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO5 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO6 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO7 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO8 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO9 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO10 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO11 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO12 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 12mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW fast} {USAGE Unassigned}}\
     PMC_MIO13 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO14 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO15 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO16 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO17 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO18 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO19 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO20 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO21 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO22 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO23 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO24 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO25 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO26 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO27 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO28 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO29 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO30 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO31 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO32 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO33 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO34 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO35 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO36 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO37 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA high}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE GPIO}}\
     PMC_MIO38 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Unassigned}}\
     PMC_MIO39 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Unassigned}}\
     PMC_MIO40 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO41 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO42 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO43 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO44 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO45 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO46 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO47 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA\
default} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_MIO48 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Unassigned}}\
     PMC_MIO49 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Unassigned}}\
     PMC_MIO50 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Unassigned}}\
     PMC_MIO51 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO0 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO1 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO2 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO3 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO4 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO5 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO6 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO7 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO8 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO9 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO10 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO11 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO12 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO13 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO14 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO15 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO16 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO17 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO18 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO19 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO20 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO21 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO22 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO23 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PS_MIO24 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 1} {SLEW slow} {USAGE Reserved}}\
     PS_MIO25 {{AUX_IO 0} {DIRECTION inout} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
     PMC_CORE_SUBSYSTEM_LOAD {10}\
     PS_R5_LOAD {90}\
     PS_LPD_INTERCONNECT_LOAD {90}\
     PS_FPD_INTERCONNECT_LOAD {90}\
     PS_A72_LOAD {90}\
     PS_R5_ACTIVE_BLOCKS {2}\
     PS_TCM_ACTIVE_BLOCKS {2}\
     PS_OCM_ACTIVE_BLOCKS {1}\
     PS_A72_ACTIVE_BLOCKS {2}\
     PS_PL_PASS_AXPROT_VALUE {0}\
     PS_BOARD_INTERFACE {Custom}\
     DESIGN_MODE {0}\
     PCIE_APERTURES_SINGLE_ENABLE {0}\
     PCIE_APERTURES_DUAL_ENABLE {0}\
     PMC_OT_CHECK {{DELAY 0} {ENABLE 0}}\
   } \
   CONFIG.PS_PMC_CONFIG_APPLIED {1} \
 ] $ps_cips

  set_property SELECTED_SIM_MODEL tlm  $ps_cips

  # Create instance: ps_noc, and set properties
  set ps_noc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc ps_noc ]
  set_property -dict [ list \
   CONFIG.HBM_CHNL0_CONFIG {\
HBM_PC0_PRE_DEFINED_ADDRESS_MAP ROW_BANK_COLUMN HBM_PC1_PRE_DEFINED_ADDRESS_MAP\
ROW_BANK_COLUMN HBM_PC0_USER_DEFINED_ADDRESS_MAP NONE\
HBM_PC1_USER_DEFINED_ADDRESS_MAP NONE} \
   CONFIG.HBM_DENSITY_PER_CHNL {1GB} \
   CONFIG.LOGO_FILE {data/noc_mc.png} \
   CONFIG.MC0_CONFIG_NUM {config17} \
   CONFIG.MC1_CONFIG_NUM {config17} \
   CONFIG.MC2_CONFIG_NUM {config17} \
   CONFIG.MC3_CONFIG_NUM {config17} \
   CONFIG.MC_CONFIG_NUM {config17} \
   CONFIG.MC_DDR4_2T {Disable} \
   CONFIG.MC_F1_LPDDR4_MR1 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR2 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR3 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR13 {0x0000} \
   CONFIG.MC_INPUTCLK0_PERIOD {5000} \
   CONFIG.MC_INPUT_FREQUENCY0 {200.000} \
   CONFIG.MC_MEMORY_DEVICETYPE {UDIMMs} \
   CONFIG.MC_PER_RD_INTVL {20000000} \
   CONFIG.MC_READ_BANDWIDTH {6400.0} \
   CONFIG.MC_WRITE_BANDWIDTH {6400.0} \
   CONFIG.MC_XPLL_CLKOUT1_PHASE {238.176} \
   CONFIG.NUM_CLKS {10} \
   CONFIG.NUM_MC {1} \
   CONFIG.NUM_MCP {4} \
   CONFIG.NUM_MI {2} \
   CONFIG.NUM_NMI {0} \
   CONFIG.NUM_SI {8} \
 ] $ps_noc

  set_property SELECTED_SIM_MODEL tlm  $ps_noc

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.APERTURES {{0x201_0000_0000 1G}} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /ps_noc/M00_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins /ps_noc/M01_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S00_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S01_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S02_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S03_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {M01_AXI:0x40:M00_AXI:0x0} \
   CONFIG.CATEGORY {ps_pmc} \
 ] [get_bd_intf_pins /ps_noc/S04_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_nci} \
 ] [get_bd_intf_pins /ps_noc/S05_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_nci} \
 ] [get_bd_intf_pins /ps_noc/S06_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_rpu} \
 ] [get_bd_intf_pins /ps_noc/S07_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI} \
 ] [get_bd_pins /ps_noc/aclk0]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S01_AXI} \
 ] [get_bd_pins /ps_noc/aclk1]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S02_AXI} \
 ] [get_bd_pins /ps_noc/aclk2]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S03_AXI} \
 ] [get_bd_pins /ps_noc/aclk3]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S04_AXI} \
 ] [get_bd_pins /ps_noc/aclk4]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins /ps_noc/aclk5]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S05_AXI} \
 ] [get_bd_pins /ps_noc/aclk6]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S07_AXI} \
 ] [get_bd_pins /ps_noc/aclk7]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S06_AXI} \
 ] [get_bd_pins /ps_noc/aclk8]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M01_AXI} \
 ] [get_bd_pins /ps_noc/aclk9]

  # Create interface connections
  connect_bd_intf_net -intf_net SYS_CLK1_IN_0_1 [get_bd_intf_ports SYS_CLK1_IN_0] [get_bd_intf_pins clk_gen_sim_0/SYS_CLK0_IN]
  connect_bd_intf_net -intf_net clk_gen_sim_0_SYS_CLK0 [get_bd_intf_pins clk_gen_sim_0/SYS_CLK0] [get_bd_intf_pins ps_noc/sys_clk0]
  connect_bd_intf_net -intf_net ctrl_sm_0_M00_AXI [get_bd_intf_pins ctrl_sm_0/M00_AXI] [get_bd_intf_pins ctrl_sm_0_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_10_M00_AXI [get_bd_intf_pins ctrl_sm_10/M00_AXI] [get_bd_intf_pins ctrl_sm_10_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_11_M00_AXI [get_bd_intf_pins ctrl_sm_11/M00_AXI] [get_bd_intf_pins ctrl_sm_11_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_12_M00_AXI [get_bd_intf_pins ctrl_sm_12/M00_AXI] [get_bd_intf_pins ctrl_sm_12_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_13_M00_AXI [get_bd_intf_pins ctrl_sm_13/M00_AXI] [get_bd_intf_pins ctrl_sm_13_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_14_M00_AXI [get_bd_intf_pins ctrl_sm_14/M00_AXI] [get_bd_intf_pins ctrl_sm_14_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_15_M00_AXI [get_bd_intf_pins ctrl_sm_15/M00_AXI] [get_bd_intf_pins ctrl_sm_15_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_1_M00_AXI [get_bd_intf_pins ctrl_sm_1/M00_AXI] [get_bd_intf_pins ctrl_sm_1_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_2_M00_AXI [get_bd_intf_pins ctrl_sm_2/M00_AXI] [get_bd_intf_pins ctrl_sm_2_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_3_M00_AXI [get_bd_intf_pins ctrl_sm_3/M00_AXI] [get_bd_intf_pins ctrl_sm_3_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_4_M00_AXI [get_bd_intf_pins ctrl_sm_4/M00_AXI] [get_bd_intf_pins ctrl_sm_4_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_5_M00_AXI [get_bd_intf_pins ctrl_sm_5/M00_AXI] [get_bd_intf_pins ctrl_sm_5_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_6_M00_AXI [get_bd_intf_pins ctrl_sm_6/M00_AXI] [get_bd_intf_pins ctrl_sm_6_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_7_M00_AXI [get_bd_intf_pins ctrl_sm_7/M00_AXI] [get_bd_intf_pins ctrl_sm_7_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_8_M00_AXI [get_bd_intf_pins ctrl_sm_8/M00_AXI] [get_bd_intf_pins ctrl_sm_8_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_9_M00_AXI [get_bd_intf_pins ctrl_sm_9/M00_AXI] [get_bd_intf_pins ctrl_sm_9_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M00_AXI [get_bd_intf_pins ctrl_sm/M00_AXI] [get_bd_intf_pins ctrl_sm_0/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M01_AXI [get_bd_intf_pins ctrl_sm/M01_AXI] [get_bd_intf_pins ctrl_sm_1/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M02_AXI [get_bd_intf_pins ctrl_sm/M02_AXI] [get_bd_intf_pins ctrl_sm_2/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M03_AXI [get_bd_intf_pins ctrl_sm/M03_AXI] [get_bd_intf_pins ctrl_sm_3/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M04_AXI [get_bd_intf_pins ctrl_sm/M04_AXI] [get_bd_intf_pins ctrl_sm_4/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M05_AXI [get_bd_intf_pins ctrl_sm/M05_AXI] [get_bd_intf_pins ctrl_sm_5/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M06_AXI [get_bd_intf_pins ctrl_sm/M06_AXI] [get_bd_intf_pins ctrl_sm_6/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M07_AXI [get_bd_intf_pins ctrl_sm/M07_AXI] [get_bd_intf_pins ctrl_sm_7/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M08_AXI [get_bd_intf_pins ctrl_sm/M08_AXI] [get_bd_intf_pins ctrl_sm_8/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M09_AXI [get_bd_intf_pins ctrl_sm/M09_AXI] [get_bd_intf_pins ctrl_sm_9/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M10_AXI [get_bd_intf_pins ctrl_sm/M10_AXI] [get_bd_intf_pins ctrl_sm_10/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M11_AXI [get_bd_intf_pins ctrl_sm/M11_AXI] [get_bd_intf_pins ctrl_sm_11/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M12_AXI [get_bd_intf_pins ctrl_sm/M12_AXI] [get_bd_intf_pins ctrl_sm_12/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M13_AXI [get_bd_intf_pins ctrl_sm/M13_AXI] [get_bd_intf_pins ctrl_sm_13/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M14_AXI [get_bd_intf_pins ctrl_sm/M14_AXI] [get_bd_intf_pins ctrl_sm_14/S00_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_M15_AXI [get_bd_intf_pins ctrl_sm/M15_AXI] [get_bd_intf_pins ctrl_sm_15/S00_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PMC_NOC_AXI_0 [get_bd_intf_pins ps_cips/PMC_NOC_AXI_0] [get_bd_intf_pins ps_noc/S04_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_0 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_0] [get_bd_intf_pins ps_noc/S00_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_1 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_1] [get_bd_intf_pins ps_noc/S01_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_2 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_2] [get_bd_intf_pins ps_noc/S02_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_3 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_3] [get_bd_intf_pins ps_noc/S03_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_NCI_0 [get_bd_intf_pins ps_cips/FPD_AXI_NOC_0] [get_bd_intf_pins ps_noc/S05_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_NCI_1 [get_bd_intf_pins ps_cips/FPD_AXI_NOC_1] [get_bd_intf_pins ps_noc/S06_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_RPU_0 [get_bd_intf_pins ps_cips/LPD_AXI_NOC_0] [get_bd_intf_pins ps_noc/S07_AXI]
  connect_bd_intf_net -intf_net ps_cips_M_AXI_GP0 [get_bd_intf_pins ctrl_sm/S00_AXI] [get_bd_intf_pins ps_cips/M_AXI_FPD]
  connect_bd_intf_net -intf_net ps_noc_CH0_DDR4_0 [get_bd_intf_ports ddr4_dimm1] [get_bd_intf_pins ps_noc/CH0_DDR4_0]
  connect_bd_intf_net -intf_net ps_noc_M00_AXI [get_bd_intf_pins axi_dbg_hub_0/S_AXI] [get_bd_intf_pins ps_noc/M00_AXI]
  connect_bd_intf_net -intf_net ps_noc_M01_AXI [get_bd_intf_pins ai_engine_0/S00_AXI] [get_bd_intf_pins ps_noc/M01_AXI]

  # Create port connections
  connect_bd_net -net ai_engine_0_s00_axi_aclk [get_bd_pins ai_engine_0/s00_axi_aclk] [get_bd_pins ps_noc/aclk9]
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins axi_dbg_hub_0/aclk] [get_bd_pins pl_clk_out6_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out6] [get_bd_pins ps_noc/aclk5]
  connect_bd_net -net pl_clk_out6_rst_peripheral_aresetn [get_bd_pins axi_dbg_hub_0/aresetn] [get_bd_pins pl_clk_out6_rst/peripheral_aresetn]
  connect_bd_net -net pl_clkwiz_clk_out1 [get_bd_pins ctrl_sm/aclk] [get_bd_pins ctrl_sm_0/aclk] [get_bd_pins ctrl_sm_0_tie/aclk] [get_bd_pins ctrl_sm_1/aclk] [get_bd_pins ctrl_sm_10/aclk] [get_bd_pins ctrl_sm_10_tie/aclk] [get_bd_pins ctrl_sm_11/aclk] [get_bd_pins ctrl_sm_11_tie/aclk] [get_bd_pins ctrl_sm_12/aclk] [get_bd_pins ctrl_sm_12_tie/aclk] [get_bd_pins ctrl_sm_13/aclk] [get_bd_pins ctrl_sm_13_tie/aclk] [get_bd_pins ctrl_sm_14/aclk] [get_bd_pins ctrl_sm_14_tie/aclk] [get_bd_pins ctrl_sm_15/aclk] [get_bd_pins ctrl_sm_15_tie/aclk] [get_bd_pins ctrl_sm_1_tie/aclk] [get_bd_pins ctrl_sm_2/aclk] [get_bd_pins ctrl_sm_2_tie/aclk] [get_bd_pins ctrl_sm_3/aclk] [get_bd_pins ctrl_sm_3_tie/aclk] [get_bd_pins ctrl_sm_4/aclk] [get_bd_pins ctrl_sm_4_tie/aclk] [get_bd_pins ctrl_sm_5/aclk] [get_bd_pins ctrl_sm_5_tie/aclk] [get_bd_pins ctrl_sm_6/aclk] [get_bd_pins ctrl_sm_6_tie/aclk] [get_bd_pins ctrl_sm_7/aclk] [get_bd_pins ctrl_sm_7_tie/aclk] [get_bd_pins ctrl_sm_8/aclk] [get_bd_pins ctrl_sm_8_tie/aclk] [get_bd_pins ctrl_sm_9/aclk] [get_bd_pins ctrl_sm_9_tie/aclk] [get_bd_pins pl_clk_out1_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out1] [get_bd_pins ps_cips/m_axi_fpd_aclk]
  connect_bd_net -net pl_clkwiz_clk_out2 [get_bd_pins pl_clk_out2_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out2]
  connect_bd_net -net pl_clkwiz_clk_out3 [get_bd_pins pl_clk_out3_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out3]
  connect_bd_net -net pl_clkwiz_clk_out4 [get_bd_pins pl_clk_out4_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out4]
  connect_bd_net -net pl_clkwiz_clk_out5 [get_bd_pins pl_clk_out5_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out5]
  connect_bd_net -net pl_clkwiz_locked [get_bd_pins pl_clk_out1_rst/dcm_locked] [get_bd_pins pl_clk_out2_rst/dcm_locked] [get_bd_pins pl_clk_out3_rst/dcm_locked] [get_bd_pins pl_clk_out4_rst/dcm_locked] [get_bd_pins pl_clk_out5_rst/dcm_locked] [get_bd_pins pl_clkwiz/locked]
  connect_bd_net -net ps_cips_pl_clk0 [get_bd_pins pl_clkwiz/clk_in1] [get_bd_pins ps_cips/pl0_ref_clk]
  connect_bd_net -net ps_cips_pl_resetn0 [get_bd_pins pl_clk_out1_rst/ext_reset_in] [get_bd_pins pl_clk_out2_rst/ext_reset_in] [get_bd_pins pl_clk_out3_rst/ext_reset_in] [get_bd_pins pl_clk_out4_rst/ext_reset_in] [get_bd_pins pl_clk_out5_rst/ext_reset_in] [get_bd_pins pl_clk_out6_rst/ext_reset_in] [get_bd_pins ps_cips/pl0_resetn]
  connect_bd_net -net ps_cips_ps_pmc_noc_axi0_clk [get_bd_pins ps_cips/pmc_axi_noc_axi0_clk] [get_bd_pins ps_noc/aclk4]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi0_clk [get_bd_pins ps_cips/fpd_cci_noc_axi0_clk] [get_bd_pins ps_noc/aclk0]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi1_clk [get_bd_pins ps_cips/fpd_cci_noc_axi1_clk] [get_bd_pins ps_noc/aclk1]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi2_clk [get_bd_pins ps_cips/fpd_cci_noc_axi2_clk] [get_bd_pins ps_noc/aclk2]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi3_clk [get_bd_pins ps_cips/fpd_cci_noc_axi3_clk] [get_bd_pins ps_noc/aclk3]
  connect_bd_net -net ps_cips_ps_ps_noc_nci_axi0_clk [get_bd_pins ps_cips/fpd_axi_noc_axi0_clk] [get_bd_pins ps_noc/aclk6]
  connect_bd_net -net ps_cips_ps_ps_noc_nci_axi1_clk [get_bd_pins ps_cips/fpd_axi_noc_axi1_clk] [get_bd_pins ps_noc/aclk8]
  connect_bd_net -net ps_cips_ps_ps_noc_rpu_axi0_clk [get_bd_pins ps_cips/lpd_axi_noc_clk] [get_bd_pins ps_noc/aclk7]

  # Create address segments
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_AXI_NOC_0] [get_bd_addr_segs ps_noc/S05_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_AXI_NOC_1] [get_bd_addr_segs ps_noc/S06_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_0] [get_bd_addr_segs ps_noc/S00_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_1] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_1] [get_bd_addr_segs ps_noc/S01_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_2] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_2] [get_bd_addr_segs ps_noc/S02_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_3] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/FPD_CCI_NOC_3] [get_bd_addr_segs ps_noc/S03_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/LPD_AXI_NOC_0] [get_bd_addr_segs ps_noc/S07_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0xA4000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_0_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_10_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4020000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_11_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_12_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_13_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4050000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_14_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4060000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_15_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4070000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_1_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4080000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_2_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4090000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_3_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40A0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_4_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40B0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_5_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40C0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_6_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40D0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_7_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40E0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_8_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40F0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/M_AXI_FPD] [get_bd_addr_segs ctrl_sm_9_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/PMC_NOC_AXI_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces ps_cips/PMC_NOC_AXI_0] [get_bd_addr_segs axi_dbg_hub_0/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/PMC_NOC_AXI_0] [get_bd_addr_segs ps_noc/S04_AXI/C0_DDR_LOW0] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  # Create PFM attributes
  set_property PFM_NAME {xilinx.com:vck190_v1_0:vck190_v1_0:1.0} [get_files [current_bd_design].bd]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_0]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_1]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_3]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_4]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_5]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_6]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_7]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_10]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_11]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_2]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_12]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_13]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_14]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_15]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_8]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_GP" sptag "GP"} M02_AXI {memport "M_AXI_GP" sptag "GP"} M03_AXI {memport "M_AXI_GP" sptag "GP"} M04_AXI {memport "M_AXI_GP" sptag "GP"} M05_AXI {memport "M_AXI_GP" sptag "GP"} M06_AXI {memport "M_AXI_GP" sptag "GP"} M07_AXI {memport "M_AXI_GP" sptag "GP"} M08_AXI {memport "M_AXI_GP" sptag "GP"} M09_AXI {memport "M_AXI_GP" sptag "GP"} M10_AXI {memport "M_AXI_GP" sptag "GP"} M11_AXI {memport "M_AXI_GP" sptag "GP"} M12_AXI {memport "M_AXI_GP" sptag "GP"} M13_AXI {memport "M_AXI_GP" sptag "GP"} M14_AXI {memport "M_AXI_GP" sptag "GP"} M15_AXI {memport "M_AXI_GP" sptag "GP"}} [get_bd_cells /ctrl_sm_9]
  set_property PFM.CLOCK {clk_out1 {id "0" is_default "true" proc_sys_reset "/pl_clk_out1_rst" status "fixed"} clk_out2 {id "1" is_default "false" proc_sys_reset "/pl_clk_out2_rst" status "fixed"} clk_out3 {id "2" is_default "false" proc_sys_reset "/pl_clk_out3_rst" status "fixed"} clk_out4 {id "3" is_default "false" proc_sys_reset "/pl_clk_out4_rst" status "fixed"} clk_out5 {id "4" is_default "false" proc_sys_reset "/pl_clk_out5_rst" status "fixed"}} [get_bd_cells /pl_clkwiz]
  set_property PFM.AXI_PORT {M01_AXI {memport "M_AXI_NOC" sptag "GP"} S08_AXI {memport "S_AXI_NOC" sptag "DDR"} S09_AXI {memport "S_AXI_NOC" sptag "DDR"} S10_AXI {memport "S_AXI_NOC" sptag "DDR"} S11_AXI {memport "S_AXI_NOC" sptag "DDR"} S12_AXI {memport "S_AXI_NOC" sptag "DDR"} S13_AXI {memport "S_AXI_NOC" sptag "DDR"} S14_AXI {memport "S_AXI_NOC" sptag "DDR"} S15_AXI {memport "S_AXI_NOC" sptag "DDR"} S16_AXI {memport "S_AXI_NOC" sptag "DDR"} S17_AXI {memport "S_AXI_NOC" sptag "DDR"} S18_AXI {memport "S_AXI_NOC" sptag "DDR"} S19_AXI {memport "S_AXI_NOC" sptag "DDR"} S20_AXI {memport "S_AXI_NOC" sptag "DDR"} S21_AXI {memport "S_AXI_NOC" sptag "DDR"} S22_AXI {memport "S_AXI_NOC" sptag "DDR"} S23_AXI {memport "S_AXI_NOC" sptag "DDR"} S24_AXI {memport "S_AXI_NOC" sptag "DDR"} S25_AXI {memport "S_AXI_NOC" sptag "DDR"} S26_AXI {memport "S_AXI_NOC" sptag "DDR"} S27_AXI {memport "S_AXI_NOC" sptag "DDR"} S28_AXI {memport "S_AXI_NOC" sptag "DDR"} S29_AXI {memport "S_AXI_NOC" sptag "DDR"} S30_AXI {memport "S_AXI_NOC" sptag "DDR"} S31_AXI {memport "S_AXI_NOC" sptag "DDR"} S32_AXI {memport "S_AXI_NOC" sptag "DDR"} S33_AXI {memport "S_AXI_NOC" sptag "DDR"} S34_AXI {memport "S_AXI_NOC" sptag "DDR"} S35_AXI {memport "S_AXI_NOC" sptag "DDR"}} [get_bd_cells /ps_noc]


  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


