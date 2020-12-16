# © Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

################################################################
# This is a generated script based on design: vck190_v1_0
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

## ===================================================================================
## SETUP and ERROR CHECKING
## ===================================================================================
namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
  }
}

#get path to this script 
variable script_folder
set script_folder [_tcl::get_script_folder]

#Check if script is running in correct Vivado version.
set scripts_vivado_version 2020.2
set current_vivado_version [version -short]
if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   common::send_gid_msg -ssname BD::TCL -id 2040 -severity "WARNING" "This script was generated using Vivado <$scripts_vivado_version> without IP versions in the create_bd_cell commands, but is now being run in <$current_vivado_version> of Vivado. There may have been major IP version changes between Vivado <$scripts_vivado_version> and <$current_vivado_version>, which could impact the parameter settings of the IPs."

}

#Create project if a project is not already open
set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part [lindex $argv 0] 
}

#Set design name
variable design_name
set design_name vck190_v1_0

#Error checking
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

## ===============================================================
## Create Platform Block Design 
## ===============================================================
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


  # Create system clock port. We will connect this to the clk_gen_sim IP later on. 
  set SYS_CLK1_IN_0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 SYS_CLK1_IN_0 ]

  # Create DDR4 Interface. We will connect this to the NoC later on. 
  set ddr4_dimm1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddr4_rtl:1.0 ddr4_dimm1 ]

## ===============================================================
## Create instance: ai_engine_0, and set properties
## ===============================================================
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
 ] $ai_engine_0

  # Set ai_engine S00_AXI interface as NoC 
  set_property -dict [ list \
   CONFIG.CATEGORY {NOC} \
 ] [get_bd_intf_pins /ai_engine_0/S00_AXI]

## ===============================================================
## Create instance: axi_dbg_hub_0, and set properties
## ===============================================================
  set axi_dbg_hub_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dbg_hub axi_dbg_hub_0 ]
  set_property -dict [ list \
   CONFIG.C_AXI_ADDR_WIDTH {44} \
   CONFIG.C_AXI_DATA_WIDTH {128} \
   CONFIG.C_AXI_ID_WIDTH {16} \
   CONFIG.C_NUM_DEBUG_CORES {0} \
 ] $axi_dbg_hub_0


## ===============================================================
## Create instance: clk_gen_sim_0, and set properties
## ===============================================================
  set clk_gen_sim_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_gen_sim clk_gen_sim_0 ]
  set_property -dict [ list \
   CONFIG.USER_NUM_OF_AXI_CLK {0} \
   CONFIG.USER_NUM_OF_SYS_CLK {1} \
   CONFIG.USER_SYS_CLK0_FREQ {200.000} \
   CONFIG.USER_SYS_CLK1_FREQ {200.000} \
   CONFIG.USER_SYS_CLK2_FREQ {201.694} \
 ] $clk_gen_sim_0

## ===============================================================
## Create Top-Level AXI SmartConnect: ctrl_sm, and set properties
## ===============================================================
  set ctrl_sm [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {16} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm


### ===============================================================
## Create 16 AXI SmartConnects: ctrl_sm_0 to ctrl_sm_15
## ===============================================================
  # Create instance: ctrl_sm_0, and set properties
  set ctrl_sm_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_0 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_0


  # Create instance: ctrl_sm_1, and set properties
  set ctrl_sm_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_1 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_1

  # Create instance: ctrl_sm_2, and set properties
  set ctrl_sm_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_2 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_2

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

  # Create instance: ctrl_sm_8, and set properties
  set ctrl_sm_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_8 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_8

 # Create instance: ctrl_sm_9, and set properties
  set ctrl_sm_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_9 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_9

  # Create instance: ctrl_sm_10, and set properties
  set ctrl_sm_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_10 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_10

  # Create instance: ctrl_sm_11, and set properties
  set ctrl_sm_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_11 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_11

  # Create instance: ctrl_sm_12, and set properties
  set ctrl_sm_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_12 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_12

  # Create instance: ctrl_sm_13, and set properties
  set ctrl_sm_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_13 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_13

  # Create instance: ctrl_sm_14, and set properties
  set ctrl_sm_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_14 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_14

  # Create instance: ctrl_sm_15, and set properties
  set ctrl_sm_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect ctrl_sm_15 ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.NUM_CLKS {1} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $ctrl_sm_15

## ===============================================================
## Create 16 AXI Verification IPS: ctrl_sm_0_tie to ctrl_sm_15_tie
## ===============================================================
  # Create instance: ctrl_sm_0_tie, and set properties
  set ctrl_sm_0_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_0_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_0_tie

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

  # Create instance: ctrl_sm_8_tie, and set properties
  set ctrl_sm_8_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_8_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_8_tie

  # Create instance: ctrl_sm_9_tie, and set properties
  set ctrl_sm_9_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_9_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_9_tie

  # Create instance: ctrl_sm_10_tie, and set properties
  set ctrl_sm_10_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_10_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_10_tie

  # Create instance: ctrl_sm_11_tie, and set properties
  set ctrl_sm_11_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_11_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_11_tie

  # Create instance: ctrl_sm_12_tie, and set properties
  set ctrl_sm_12_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_12_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_12_tie

  # Create instance: ctrl_sm_13_tie, and set properties
  set ctrl_sm_13_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_13_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_13_tie

  # Create instance: ctrl_sm_14_tie, and set properties
  set ctrl_sm_14_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_14_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_14_tie

  # Create instance: ctrl_sm_15_tie, and set properties
  set ctrl_sm_15_tie [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vip ctrl_sm_15_tie ]
  set_property -dict [ list \
   CONFIG.HAS_ARESETN {0} \
   CONFIG.INTERFACE_MODE {SLAVE} \
   CONFIG.PROTOCOL {AXI4LITE} \
 ] $ctrl_sm_15_tie

## =====================================================================
## Create 7 Processor System Resets: pl_clk_out1_rst to pl_clk_out6_rst
## =====================================================================
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

## =====================================================================
## Create instance: pl_clkwiz, and set properties
## =====================================================================
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

## =====================================================================
## Create instance: ps_cips, and set properties
## =====================================================================
  set ps_cips [ create_bd_cell -type ip -vlnv xilinx.com:ip:versal_cips ps_cips ]
  set_property -dict [ list \
   CONFIG.CPM_PCIE0_MODE0_FOR_POWER {NONE} \
   CONFIG.CPM_PCIE1_MODE1_FOR_POWER {NONE} \
   CONFIG.PMC_CRP_EFUSE_REF_CTRL_ACT_FREQMHZ {80.000000} \
   CONFIG.PMC_CRP_EFUSE_REF_CTRL_FREQMHZ {80.000000} \
   CONFIG.PMC_CRP_I2C_REF_CTRL_ACT_FREQMHZ {99.999001} \
   CONFIG.PMC_CRP_NOC_REF_CTRL_ACT_FREQMHZ {949.990479} \
   CONFIG.PMC_CRP_NOC_REF_CTRL_FREQMHZ {950} \
   CONFIG.PMC_CRP_NPLL_CTRL_FBDIV {114} \
   CONFIG.PMC_CRP_PL0_REF_CTRL_ACT_FREQMHZ {49.999500} \
   CONFIG.PMC_CRP_PL0_REF_CTRL_DIVISOR0 {24} \
   CONFIG.PMC_CRP_PL0_REF_CTRL_FREQMHZ {50} \
   CONFIG.PMC_CRP_PL5_REF_CTRL_FREQMHZ {400} \
   CONFIG.PMC_CRP_QSPI_REF_CTRL_ACT_FREQMHZ {299.997009} \
   CONFIG.PMC_CRP_SDIO1_REF_CTRL_ACT_FREQMHZ {199.998001} \
   CONFIG.PMC_CRP_SD_DLL_REF_CTRL_ACT_FREQMHZ {1199.988037} \
   CONFIG.PMC_CRP_SWITCH_TIMEOUT_CTRL_DIVISOR0 {80} \
   CONFIG.PMC_CRP_USB_SUSPEND_CTRL_DIVISOR0 {400} \
   CONFIG.PMC_GPIO0_MIO_PERIPHERAL_ENABLE {1} \
   CONFIG.PMC_GPIO1_MIO_PERIPHERAL_ENABLE {1} \
   CONFIG.PMC_I2CPMC_PERIPHERAL_ENABLE {1} \
   CONFIG.PMC_I2CPMC_PERIPHERAL_IO {PMC_MIO 46 .. 47} \
   CONFIG.PMC_MIO_0_DIRECTION {out} \
   CONFIG.PMC_MIO_0_SCHMITT {1} \
   CONFIG.PMC_MIO_10_DIRECTION {inout} \
   CONFIG.PMC_MIO_11_DIRECTION {inout} \
   CONFIG.PMC_MIO_12_DIRECTION {out} \
   CONFIG.PMC_MIO_12_SCHMITT {1} \
   CONFIG.PMC_MIO_13_DIRECTION {out} \
   CONFIG.PMC_MIO_13_SCHMITT {1} \
   CONFIG.PMC_MIO_14_DIRECTION {inout} \
   CONFIG.PMC_MIO_15_DIRECTION {inout} \
   CONFIG.PMC_MIO_16_DIRECTION {inout} \
   CONFIG.PMC_MIO_17_DIRECTION {inout} \
   CONFIG.PMC_MIO_19_DIRECTION {inout} \
   CONFIG.PMC_MIO_1_DIRECTION {inout} \
   CONFIG.PMC_MIO_20_DIRECTION {inout} \
   CONFIG.PMC_MIO_21_DIRECTION {inout} \
   CONFIG.PMC_MIO_22_DIRECTION {inout} \
   CONFIG.PMC_MIO_24_DIRECTION {out} \
   CONFIG.PMC_MIO_24_SCHMITT {1} \
   CONFIG.PMC_MIO_26_DIRECTION {inout} \
   CONFIG.PMC_MIO_27_DIRECTION {inout} \
   CONFIG.PMC_MIO_29_DIRECTION {inout} \
   CONFIG.PMC_MIO_2_DIRECTION {inout} \
   CONFIG.PMC_MIO_30_DIRECTION {inout} \
   CONFIG.PMC_MIO_31_DIRECTION {inout} \
   CONFIG.PMC_MIO_32_DIRECTION {inout} \
   CONFIG.PMC_MIO_33_DIRECTION {inout} \
   CONFIG.PMC_MIO_34_DIRECTION {inout} \
   CONFIG.PMC_MIO_35_DIRECTION {inout} \
   CONFIG.PMC_MIO_36_DIRECTION {inout} \
   CONFIG.PMC_MIO_37_DIRECTION {out} \
   CONFIG.PMC_MIO_37_OUTPUT_DATA {high} \
   CONFIG.PMC_MIO_37_USAGE {GPIO} \
   CONFIG.PMC_MIO_3_DIRECTION {inout} \
   CONFIG.PMC_MIO_40_DIRECTION {out} \
   CONFIG.PMC_MIO_40_SCHMITT {1} \
   CONFIG.PMC_MIO_43_DIRECTION {out} \
   CONFIG.PMC_MIO_43_SCHMITT {1} \
   CONFIG.PMC_MIO_44_DIRECTION {inout} \
   CONFIG.PMC_MIO_45_DIRECTION {inout} \
   CONFIG.PMC_MIO_46_DIRECTION {inout} \
   CONFIG.PMC_MIO_47_DIRECTION {inout} \
   CONFIG.PMC_MIO_4_DIRECTION {inout} \
   CONFIG.PMC_MIO_5_DIRECTION {out} \
   CONFIG.PMC_MIO_5_SCHMITT {1} \
   CONFIG.PMC_MIO_6_DIRECTION {out} \
   CONFIG.PMC_MIO_6_SCHMITT {1} \
   CONFIG.PMC_MIO_7_DIRECTION {out} \
   CONFIG.PMC_MIO_7_SCHMITT {1} \
   CONFIG.PMC_MIO_8_DIRECTION {inout} \
   CONFIG.PMC_MIO_9_DIRECTION {inout} \
   CONFIG.PMC_MIO_TREE_PERIPHERALS {QSPI#QSPI#QSPI#QSPI#QSPI#QSPI#Loopback Clk#QSPI#QSPI#QSPI#QSPI#QSPI#QSPI#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#USB 0#SD1/eMMC1#SD1/eMMC1#SD1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#SD1/eMMC1#GPIO 1###CAN 1#CAN 1#UART 0#UART 0#I2C 1#I2C 1#i2c_pmc#i2c_pmc####SD1/eMMC1#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 0#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 1#Enet 0#Enet 0} \
   CONFIG.PMC_MIO_TREE_SIGNALS {qspi0_clk#qspi0_io[1]#qspi0_io[2]#qspi0_io[3]#qspi0_io[0]#qspi0_cs_b#qspi_lpbk#qspi1_cs_b#qspi1_io[0]#qspi1_io[1]#qspi1_io[2]#qspi1_io[3]#qspi1_clk#usb2phy_reset#ulpi_tx_data[0]#ulpi_tx_data[1]#ulpi_tx_data[2]#ulpi_tx_data[3]#ulpi_clk#ulpi_tx_data[4]#ulpi_tx_data[5]#ulpi_tx_data[6]#ulpi_tx_data[7]#ulpi_dir#ulpi_stp#ulpi_nxt#clk#dir1/data[7]#detect#cmd#data[0]#data[1]#data[2]#data[3]#sel/data[4]#dir_cmd/data[5]#dir0/data[6]#gpio_1_pin[37]###phy_tx#phy_rx#rxd#txd#scl#sda#scl#sda####buspwr/rst#rgmii_tx_clk#rgmii_txd[0]#rgmii_txd[1]#rgmii_txd[2]#rgmii_txd[3]#rgmii_tx_ctl#rgmii_rx_clk#rgmii_rxd[0]#rgmii_rxd[1]#rgmii_rxd[2]#rgmii_rxd[3]#rgmii_rx_ctl#rgmii_tx_clk#rgmii_txd[0]#rgmii_txd[1]#rgmii_txd[2]#rgmii_txd[3]#rgmii_tx_ctl#rgmii_rx_clk#rgmii_rxd[0]#rgmii_rxd[1]#rgmii_rxd[2]#rgmii_rxd[3]#rgmii_rx_ctl#gem0_mdc#gem0_mdio} \
   CONFIG.PMC_OSPI_PERIPHERAL_ENABLE {0} \
   CONFIG.PMC_QSPI_GRP_FBCLK_ENABLE {1} \
   CONFIG.PMC_QSPI_PERIPHERAL_DATA_MODE {x4} \
   CONFIG.PMC_QSPI_PERIPHERAL_ENABLE {1} \
   CONFIG.PMC_QSPI_PERIPHERAL_MODE {Dual Parallel} \
   CONFIG.PMC_SD1_DATA_TRANSFER_MODE {8Bit} \
   CONFIG.PMC_SD1_GRP_CD_ENABLE {1} \
   CONFIG.PMC_SD1_GRP_CD_IO {PMC_MIO 28} \
   CONFIG.PMC_SD1_GRP_POW_ENABLE {1} \
   CONFIG.PMC_SD1_GRP_WP_ENABLE {0} \
   CONFIG.PMC_SD1_PERIPHERAL_ENABLE {1} \
   CONFIG.PMC_SD1_PERIPHERAL_IO {PMC_MIO 26 .. 36} \
   CONFIG.PMC_SD1_SLOT_TYPE {SD 3.0} \
   CONFIG.PMC_SD1_SPEED_MODE {high speed} \
   CONFIG.PMC_USE_PMC_NOC_AXI0 {1} \
   CONFIG.PS_CAN1_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_CAN1_PERIPHERAL_IO {PMC_MIO 40 .. 41} \
   CONFIG.PS_CRL_CAN0_REF_CTRL_FREQMHZ {160} \
   CONFIG.PS_CRL_CAN0_REF_CTRL_SRCSEL {NPLL} \
   CONFIG.PS_CRL_CAN1_REF_CTRL_ACT_FREQMHZ {149.998505} \
   CONFIG.PS_CRL_CAN1_REF_CTRL_DIVISOR0 {4} \
   CONFIG.PS_CRL_CAN1_REF_CTRL_FREQMHZ {150} \
   CONFIG.PS_CRL_CAN1_REF_CTRL_SRCSEL {PPLL} \
   CONFIG.PS_CRL_CPM_TOPSW_REF_CTRL_ACT_FREQMHZ {474.995239} \
   CONFIG.PS_CRL_CPM_TOPSW_REF_CTRL_FREQMHZ {475} \
   CONFIG.PS_CRL_GEM0_REF_CTRL_ACT_FREQMHZ {124.998749} \
   CONFIG.PS_CRL_GEM1_REF_CTRL_ACT_FREQMHZ {124.998749} \
   CONFIG.PS_CRL_GEM_TSU_REF_CTRL_ACT_FREQMHZ {249.997498} \
   CONFIG.PS_CRL_I2C1_REF_CTRL_ACT_FREQMHZ {99.999001} \
   CONFIG.PS_CRL_UART0_REF_CTRL_ACT_FREQMHZ {99.999001} \
   CONFIG.PS_CRL_USB0_BUS_REF_CTRL_ACT_FREQMHZ {19.999800} \
   CONFIG.PS_CRL_USB3_DUAL_REF_CTRL_ACT_FREQMHZ {19.999800} \
   CONFIG.PS_CRL_USB3_DUAL_REF_CTRL_DIVISOR0 {30} \
   CONFIG.PS_CRL_USB3_DUAL_REF_CTRL_FREQMHZ {20} \
   CONFIG.PS_ENET0_GRP_MDIO_ENABLE {1} \
   CONFIG.PS_ENET0_GRP_MDIO_IO {PS_MIO 24 .. 25} \
   CONFIG.PS_ENET0_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_ENET0_PERIPHERAL_IO {PS_MIO 0 .. 11} \
   CONFIG.PS_ENET1_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_ENET1_PERIPHERAL_IO {PS_MIO 12 .. 23} \
   CONFIG.PS_GEM0_ROUTE_THROUGH_FPD {1} \
   CONFIG.PS_GEN_IPI_0_ENABLE {1} \
   CONFIG.PS_GEN_IPI_1_ENABLE {1} \
   CONFIG.PS_GEN_IPI_2_ENABLE {1} \
   CONFIG.PS_GEN_IPI_3_ENABLE {1} \
   CONFIG.PS_GEN_IPI_4_ENABLE {1} \
   CONFIG.PS_GEN_IPI_5_ENABLE {1} \
   CONFIG.PS_GEN_IPI_6_ENABLE {1} \
   CONFIG.PS_GEN_IPI_PMCNOBUF_ENABLE {1} \
   CONFIG.PS_GEN_IPI_PMC_ENABLE {1} \
   CONFIG.PS_GEN_IPI_PSM_ENABLE {1} \
   CONFIG.PS_GPIO2_MIO_PERIPHERAL_ENABLE {0} \
   CONFIG.PS_I2C1_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_I2C1_PERIPHERAL_IO {PMC_MIO 44 .. 45} \
   CONFIG.PS_MIO_0_DIRECTION {out} \
   CONFIG.PS_MIO_0_SCHMITT {1} \
   CONFIG.PS_MIO_12_DIRECTION {out} \
   CONFIG.PS_MIO_12_SCHMITT {1} \
   CONFIG.PS_MIO_13_DIRECTION {out} \
   CONFIG.PS_MIO_13_SCHMITT {1} \
   CONFIG.PS_MIO_14_DIRECTION {out} \
   CONFIG.PS_MIO_14_SCHMITT {1} \
   CONFIG.PS_MIO_15_DIRECTION {out} \
   CONFIG.PS_MIO_15_SCHMITT {1} \
   CONFIG.PS_MIO_16_DIRECTION {out} \
   CONFIG.PS_MIO_16_SCHMITT {1} \
   CONFIG.PS_MIO_17_DIRECTION {out} \
   CONFIG.PS_MIO_17_SCHMITT {1} \
   CONFIG.PS_MIO_1_DIRECTION {out} \
   CONFIG.PS_MIO_1_SCHMITT {1} \
   CONFIG.PS_MIO_24_DIRECTION {out} \
   CONFIG.PS_MIO_24_SCHMITT {1} \
   CONFIG.PS_MIO_25_DIRECTION {inout} \
   CONFIG.PS_MIO_2_DIRECTION {out} \
   CONFIG.PS_MIO_2_SCHMITT {1} \
   CONFIG.PS_MIO_3_DIRECTION {out} \
   CONFIG.PS_MIO_3_SCHMITT {1} \
   CONFIG.PS_MIO_4_DIRECTION {out} \
   CONFIG.PS_MIO_4_SCHMITT {1} \
   CONFIG.PS_MIO_5_DIRECTION {out} \
   CONFIG.PS_MIO_5_SCHMITT {1} \
   CONFIG.PS_NUM_FABRIC_RESETS {1} \
   CONFIG.PS_PCIE_RESET_ENABLE {1} \
   CONFIG.PS_PCIE_RESET_IO {PS_MIO 18 .. 19} \
   CONFIG.PS_TTC0_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_TTC0_REF_CTRL_ACT_FREQMHZ {99.999001} \
   CONFIG.PS_TTC0_REF_CTRL_FREQMHZ {99.999001} \
   CONFIG.PS_TTC3_PERIPHERAL_ENABLE {0} \
   CONFIG.PS_UART0_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_UART0_PERIPHERAL_IO {PMC_MIO 42 .. 43} \
   CONFIG.PS_USB3_PERIPHERAL_ENABLE {1} \
   CONFIG.PS_USE_M_AXI_GP0 {1} \
   CONFIG.PS_M_AXI_GP0_DATA_WIDTH {32} \
   CONFIG.PS_USE_M_AXI_GP2 {0} \
   CONFIG.PS_USE_PMCPL_CLK0 {1} \
   CONFIG.PS_USE_PMCPL_CLK1 {0} \
   CONFIG.PS_USE_PMCPL_CLK2 {0} \
   CONFIG.PS_USE_PMCPL_CLK3 {0} \
   CONFIG.PS_USE_PS_NOC_CCI {1} \
   CONFIG.PS_USE_PS_NOC_NCI_0 {1} \
   CONFIG.PS_USE_PS_NOC_NCI_1 {1} \
   CONFIG.PS_USE_PS_NOC_RPU_0 {1} \
   CONFIG.PS_USE_S_AXI_GP0 {0} \
   CONFIG.PS_USE_S_AXI_GP2 {0} \
   CONFIG.PS_USE_S_AXI_GP4 {0} \
   CONFIG.SMON_ALARMS {Set_Alarms_On} \
   CONFIG.SMON_MEAS37_ENABLE {1} \
   CONFIG.SMON_MEAS40_ENABLE {1} \
   CONFIG.SMON_MEAS41_ENABLE {1} \
   CONFIG.SMON_MEAS42_ENABLE {1} \
   CONFIG.SMON_MEAS61_ENABLE {1} \
   CONFIG.SMON_MEAS62_ENABLE {1} \
   CONFIG.SMON_MEAS63_ENABLE {1} \
   CONFIG.SMON_MEAS64_ENABLE {1} \
   CONFIG.SMON_MEAS65_ENABLE {1} \
   CONFIG.SMON_MEAS66_ENABLE {1} \
   CONFIG.SMON_OT_THRESHOLD_LOWER {-55} \
   CONFIG.SMON_OT_THRESHOLD_UPPER {125} \
   CONFIG.SMON_USER_TEMP_THRESHOLD_LOWER {-55} \
   CONFIG.SMON_USER_TEMP_THRESHOLD_UPPER {125} \
   CONFIG.SMON_MEAS37_ENABLE {1} \
   CONFIG.SMON_MEAS40_ENABLE {1} \
   CONFIG.SMON_MEAS41_ENABLE {1} \
   CONFIG.SMON_MEAS42_ENABLE {1} \
   CONFIG.SMON_MEAS61_ENABLE {1} \
   CONFIG.SMON_MEAS62_ENABLE {1} \
   CONFIG.SMON_MEAS63_ENABLE {1} \
   CONFIG.SMON_MEAS64_ENABLE {1} \
   CONFIG.SMON_MEAS65_ENABLE {1} \
   CONFIG.SMON_MEAS66_ENABLE {1} \
 ] $ps_cips

## =====================================================================
## Create instance: ps_noc, and set properties
## =====================================================================
  set ps_noc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc ps_noc ]
  set_property -dict [ list \
   CONFIG.LOGO_FILE {data/noc_mc.png} \
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
   CONFIG.NUM_CLKS {10} \
   CONFIG.NUM_MC {1} \
   CONFIG.NUM_MCP {4} \
   CONFIG.NUM_MI {2} \
   CONFIG.NUM_NMI {0} \
   CONFIG.NUM_SI {8} \
 ] $ps_noc

  # Configure ps_noc's M00_AXI Interface to connect with PL
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.APERTURES {{0x201_0000_0000 1G}} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /ps_noc/M00_AXI]

  # Configure ps_noc's M01_AXI Interface to connect with AI Engine 
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins /ps_noc/M01_AXI]

  # Configure ps_noc's S00_AXI Interface to connect PS_CCI 
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S00_AXI]

  # Configure ps_noc's S01_AXI Interface to connect PS_CCI 
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S01_AXI]

  # Configure ps_noc's S02_AXI Interface to connect PS_CCI 
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S02_AXI]

  # Configure ps_noc's S03_AXI Interface to connect PS_CCI 
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {M01_AXI:0x40} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /ps_noc/S03_AXI]

  # Configure ps_noc's S04_AXI Interface to connect PS_PMC 
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M01_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_AXI { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {M01_AXI:0x40:M00_AXI:0x0} \
   CONFIG.CATEGORY {ps_pmc} \
 ] [get_bd_intf_pins /ps_noc/S04_AXI]

  # Configure ps_noc's S05_AXI Interface to connect PS_NCI
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_nci} \
 ] [get_bd_intf_pins /ps_noc/S05_AXI]

  # Configure ps_noc's S06_AXI Interface to connect PS_NCI
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {128} write_bw {128}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_nci} \
 ] [get_bd_intf_pins /ps_noc/S06_AXI]

  # Configure ps_noc's S07_AXI Interface to connect PS_RPU
  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {128} write_bw {128} read_avg_burst {4} write_avg_burst {4}} M00_INI { read_bw {64} write_bw {64}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_rpu} \
 ] [get_bd_intf_pins /ps_noc/S07_AXI]

  # Configure ps_noc's aclk0 to S00_AXI (PS_CCI)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI} \
 ] [get_bd_pins /ps_noc/aclk0]

  # Configure ps_noc's aclk1 to S01_AXI (PS_CCI)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S01_AXI} \
 ] [get_bd_pins /ps_noc/aclk1]

  # Configure ps_noc's aclk2 to S02_AXI (PS_CCI)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S02_AXI} \
 ] [get_bd_pins /ps_noc/aclk2]

  # Configure ps_noc's aclk3 to S03_AXI (PS_CCI)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S03_AXI} \
 ] [get_bd_pins /ps_noc/aclk3]

  # Configure ps_noc's aclk4 to S04_AXI (PS_PMC)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S04_AXI} \
 ] [get_bd_pins /ps_noc/aclk4]

  # Configure ps_noc's aclk5 to M00_AXI (PL)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins /ps_noc/aclk5]

  # Configure ps_noc's aclk6 to S05_AXI (PS_NCI)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S05_AXI} \
 ] [get_bd_pins /ps_noc/aclk6]

  # Configure ps_noc's aclk7 to S07_AXI (PS_RPU)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S07_AXI} \
 ] [get_bd_pins /ps_noc/aclk7]

  # Configure ps_noc's aclk8 to S06_AXI (PS_NCI)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S06_AXI} \
 ] [get_bd_pins /ps_noc/aclk8]

  # Configure ps_noc's aclk9 to M01_AXI (AI Engine)
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M01_AXI} \
 ] [get_bd_pins /ps_noc/aclk9]

## =====================================================================
## Create interface connections
## =====================================================================
  #Clocking Connections
  connect_bd_intf_net -intf_net SYS_CLK1_IN_0_1 [get_bd_intf_ports SYS_CLK1_IN_0] [get_bd_intf_pins clk_gen_sim_0/SYS_CLK0_IN]
  connect_bd_intf_net -intf_net clk_gen_sim_0_SYS_CLK0 [get_bd_intf_pins clk_gen_sim_0/SYS_CLK0] [get_bd_intf_pins ps_noc/sys_clk0]
  
  #Connect 16 AXI SmartConnects (ctrl_sm_1 to ctrl_sm_15) to AXI Verification IPs (ctrl_sm_1_tie to ctrl_sm_15_tie) 
  connect_bd_intf_net -intf_net ctrl_sm_0_M00_AXI [get_bd_intf_pins ctrl_sm_0/M00_AXI] [get_bd_intf_pins ctrl_sm_0_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_1_M00_AXI [get_bd_intf_pins ctrl_sm_1/M00_AXI] [get_bd_intf_pins ctrl_sm_1_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_2_M00_AXI [get_bd_intf_pins ctrl_sm_2/M00_AXI] [get_bd_intf_pins ctrl_sm_2_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_3_M00_AXI [get_bd_intf_pins ctrl_sm_3/M00_AXI] [get_bd_intf_pins ctrl_sm_3_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_4_M00_AXI [get_bd_intf_pins ctrl_sm_4/M00_AXI] [get_bd_intf_pins ctrl_sm_4_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_5_M00_AXI [get_bd_intf_pins ctrl_sm_5/M00_AXI] [get_bd_intf_pins ctrl_sm_5_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_6_M00_AXI [get_bd_intf_pins ctrl_sm_6/M00_AXI] [get_bd_intf_pins ctrl_sm_6_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_7_M00_AXI [get_bd_intf_pins ctrl_sm_7/M00_AXI] [get_bd_intf_pins ctrl_sm_7_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_8_M00_AXI [get_bd_intf_pins ctrl_sm_8/M00_AXI] [get_bd_intf_pins ctrl_sm_8_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_9_M00_AXI [get_bd_intf_pins ctrl_sm_9/M00_AXI] [get_bd_intf_pins ctrl_sm_9_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_10_M00_AXI [get_bd_intf_pins ctrl_sm_10/M00_AXI] [get_bd_intf_pins ctrl_sm_10_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_11_M00_AXI [get_bd_intf_pins ctrl_sm_11/M00_AXI] [get_bd_intf_pins ctrl_sm_11_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_12_M00_AXI [get_bd_intf_pins ctrl_sm_12/M00_AXI] [get_bd_intf_pins ctrl_sm_12_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_13_M00_AXI [get_bd_intf_pins ctrl_sm_13/M00_AXI] [get_bd_intf_pins ctrl_sm_13_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_14_M00_AXI [get_bd_intf_pins ctrl_sm_14/M00_AXI] [get_bd_intf_pins ctrl_sm_14_tie/S_AXI]
  connect_bd_intf_net -intf_net ctrl_sm_15_M00_AXI [get_bd_intf_pins ctrl_sm_15/M00_AXI] [get_bd_intf_pins ctrl_sm_15_tie/S_AXI]

  #Connect Top-Level AXI SmartConnect (ctrl_sm) to the 16 AXI SmartConnects (S00_AXI to S15_AXI)  
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

  #Connect CIPs M_AXI_FPD interface to the Top-Level AXI SmartConnect (ctrl_sm) S00_AXI Interface 
  connect_bd_intf_net -intf_net ps_cips_M_AXI_GP0 [get_bd_intf_pins ctrl_sm/S00_AXI] [get_bd_intf_pins ps_cips/M_AXI_FPD]

  #Connect CIPs Interfaces with NoC AXI Interfaces(S00_AXI to S07_AXI)   
  connect_bd_intf_net -intf_net ps_cips_IF_PMC_NOC_AXI_0 [get_bd_intf_pins ps_cips/PMC_NOC_AXI_0] [get_bd_intf_pins ps_noc/S04_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_0 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_0] [get_bd_intf_pins ps_noc/S00_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_1 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_1] [get_bd_intf_pins ps_noc/S01_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_2 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_2] [get_bd_intf_pins ps_noc/S02_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_CCI_3 [get_bd_intf_pins ps_cips/FPD_CCI_NOC_3] [get_bd_intf_pins ps_noc/S03_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_NCI_0 [get_bd_intf_pins ps_cips/FPD_AXI_NOC_0] [get_bd_intf_pins ps_noc/S05_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_NCI_1 [get_bd_intf_pins ps_cips/FPD_AXI_NOC_1] [get_bd_intf_pins ps_noc/S06_AXI]
  connect_bd_intf_net -intf_net ps_cips_IF_PS_NOC_RPU_0 [get_bd_intf_pins ps_cips/NOC_LPD_AXI_0] [get_bd_intf_pins ps_noc/S07_AXI]

   #Connect NOC DDR4 Interface with DDR4_DIMM1 port 
  connect_bd_intf_net -intf_net ps_noc_CH0_DDR4_0 [get_bd_intf_ports ddr4_dimm1] [get_bd_intf_pins ps_noc/CH0_DDR4_0]
  
  #Connect AXI Debug Hug IP AXI to NoC AXI  
  connect_bd_intf_net -intf_net ps_noc_M00_AXI [get_bd_intf_pins axi_dbg_hub_0/S_AXI] [get_bd_intf_pins ps_noc/M00_AXI]

  #Connect AI Engine AXI to NoC AXI
  connect_bd_intf_net -intf_net ps_noc_M01_AXI [get_bd_intf_pins ai_engine_0/S00_AXI] [get_bd_intf_pins ps_noc/M01_AXI]

  #Connect AI Engine Clock to NoC Clock
  connect_bd_net -net ai_engine_0_s00_axi_aclk [get_bd_pins ai_engine_0/s00_axi_aclk] [get_bd_pins ps_noc/aclk9]
  
  #Connect Clocking Wizard output clock 6 to AXI Debug Hub clock, Clock Reset IP #6 (pl_clk_out6_rst), and NoC PL Master
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins axi_dbg_hub_0/aclk] [get_bd_pins pl_clk_out6_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out6] [get_bd_pins ps_noc/aclk5]

  #Connect aresetn pin of the Clock Reset IP #6 (pl_clk_out6_rst) to AXI Debug Hub IP's aresetn pin 
  connect_bd_net -net pl_clk_out6_rst_peripheral_aresetn [get_bd_pins axi_dbg_hub_0/aresetn] [get_bd_pins pl_clk_out6_rst/peripheral_aresetn]

  #Connect Clocking Wizard output clock 1 (clk_out1) to the following:
  #        * Top-Level AXI SmartConnect (ctrl_sm)
  #        * 16 AXI SmartConnects       (ctrl_sm_0 to ctrl_sm_15)
  #        * 16 AXI Verification IPs    (ctrl_sm_0_tie to ctrl_sm_15_tie) 
  #        * Clock Reset IP #1          (pl_clk_out1_rst)
  #        * CIPs FPD Interface Clock   (m_axi_fpd_aclk)
  connect_bd_net -net pl_clkwiz_clk_out1 [get_bd_pins ctrl_sm/aclk] \
    [get_bd_pins ctrl_sm_0/aclk] [get_bd_pins ctrl_sm_0_tie/aclk]   \
    [get_bd_pins ctrl_sm_1/aclk] [get_bd_pins ctrl_sm_1_tie/aclk]   \
    [get_bd_pins ctrl_sm_2/aclk] [get_bd_pins ctrl_sm_2_tie/aclk]   \
    [get_bd_pins ctrl_sm_3/aclk] [get_bd_pins ctrl_sm_3_tie/aclk]   \
    [get_bd_pins ctrl_sm_4/aclk] [get_bd_pins ctrl_sm_4_tie/aclk]   \
    [get_bd_pins ctrl_sm_5/aclk] [get_bd_pins ctrl_sm_5_tie/aclk]   \
    [get_bd_pins ctrl_sm_6/aclk] [get_bd_pins ctrl_sm_6_tie/aclk]   \
    [get_bd_pins ctrl_sm_7/aclk] [get_bd_pins ctrl_sm_7_tie/aclk]   \
    [get_bd_pins ctrl_sm_8/aclk] [get_bd_pins ctrl_sm_8_tie/aclk]   \
    [get_bd_pins ctrl_sm_9/aclk] [get_bd_pins ctrl_sm_9_tie/aclk]   \
    [get_bd_pins ctrl_sm_10/aclk] [get_bd_pins ctrl_sm_10_tie/aclk] \
    [get_bd_pins ctrl_sm_11/aclk] [get_bd_pins ctrl_sm_11_tie/aclk] \
    [get_bd_pins ctrl_sm_12/aclk] [get_bd_pins ctrl_sm_12_tie/aclk] \
    [get_bd_pins ctrl_sm_13/aclk] [get_bd_pins ctrl_sm_13_tie/aclk] \
    [get_bd_pins ctrl_sm_14/aclk] [get_bd_pins ctrl_sm_14_tie/aclk] \
    [get_bd_pins ctrl_sm_15/aclk] [get_bd_pins ctrl_sm_15_tie/aclk] \
    [get_bd_pins pl_clk_out1_rst/slowest_sync_clk]                  \
    [get_bd_pins pl_clkwiz/clk_out1]                                \
    [get_bd_pins ps_cips/m_axi_fpd_aclk]                        

  #Connect Clocking Wizard output clocks 2-5 (pl_clkout2-pl_clk_out5) to the Clock Reset IPs (pl_clk_out2_rst to pl_clk_out5_rst)
  connect_bd_net -net pl_clkwiz_clk_out2 [get_bd_pins pl_clk_out2_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out2]
  connect_bd_net -net pl_clkwiz_clk_out3 [get_bd_pins pl_clk_out3_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out3]
  connect_bd_net -net pl_clkwiz_clk_out4 [get_bd_pins pl_clk_out4_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out4]
  connect_bd_net -net pl_clkwiz_clk_out5 [get_bd_pins pl_clk_out5_rst/slowest_sync_clk] [get_bd_pins pl_clkwiz/clk_out5]

  #Connect Clocking Wizard locked pin to the Clock Reset IPs dcm_locked pins
  connect_bd_net -net pl_clkwiz_locked [get_bd_pins pl_clk_out1_rst/dcm_locked] \
    [get_bd_pins pl_clk_out2_rst/dcm_locked] \
    [get_bd_pins pl_clk_out3_rst/dcm_locked] \
    [get_bd_pins pl_clk_out4_rst/dcm_locked] \
    [get_bd_pins pl_clk_out5_rst/dcm_locked] \
    [get_bd_pins pl_clkwiz/locked]
  
  #Connect CIPs pl0_ref_clk signal as the input to the Clocking Wizard    
  connect_bd_net -net ps_cips_pl_clk0 [get_bd_pins pl_clkwiz/clk_in1] [get_bd_pins ps_cips/pl0_ref_clk]

  #Connect CIPS pl0_resetn pin to the Clock Reset IPs ext_reset_in pins    
  connect_bd_net -net ps_cips_pl_resetn0 [get_bd_pins pl_clk_out1_rst/ext_reset_in] \
    [get_bd_pins pl_clk_out2_rst/ext_reset_in] \
    [get_bd_pins pl_clk_out3_rst/ext_reset_in] \
    [get_bd_pins pl_clk_out4_rst/ext_reset_in] \
    [get_bd_pins pl_clk_out5_rst/ext_reset_in] \
    [get_bd_pins pl_clk_out6_rst/ext_reset_in] \
    [get_bd_pins ps_cips/pl0_resetn]

  #Connect CIPS PMC Clock to the NoC   
  connect_bd_net -net ps_cips_ps_pmc_noc_axi0_clk [get_bd_pins ps_cips/pmc_axi_noc_axi0_clk] [get_bd_pins ps_noc/aclk4]

  #Connect CIPS FPD CCI Clocks to the NoC 
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi0_clk [get_bd_pins ps_cips/fpd_cci_noc_axi0_clk] [get_bd_pins ps_noc/aclk0]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi1_clk [get_bd_pins ps_cips/fpd_cci_noc_axi1_clk] [get_bd_pins ps_noc/aclk1]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi2_clk [get_bd_pins ps_cips/fpd_cci_noc_axi2_clk] [get_bd_pins ps_noc/aclk2]
  connect_bd_net -net ps_cips_ps_ps_noc_cci_axi3_clk [get_bd_pins ps_cips/fpd_cci_noc_axi3_clk] [get_bd_pins ps_noc/aclk3]

  #Connect CIPS FPD AXI Clocks to the NoC
  connect_bd_net -net ps_cips_ps_ps_noc_nci_axi0_clk [get_bd_pins ps_cips/fpd_axi_noc_axi0_clk] [get_bd_pins ps_noc/aclk6]
  connect_bd_net -net ps_cips_ps_ps_noc_nci_axi1_clk [get_bd_pins ps_cips/fpd_axi_noc_axi1_clk] [get_bd_pins ps_noc/aclk8]

  #Connects CIPS LPD AXI Clock to the NoC
  connect_bd_net -net ps_cips_ps_ps_noc_rpu_axi0_clk [get_bd_pins ps_cips/lpd_axi_noc_clk] [get_bd_pins ps_noc/aclk7]

## =====================================================================
## Create Address Segments
## =====================================================================
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI3] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI2] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_PMC] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI1] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_PMC] [get_bd_addr_segs axi_dbg_hub_0/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0xA4000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_0_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40A0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_10_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40B0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_11_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40C0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_12_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40D0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_13_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40E0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_14_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA40F0000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_15_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_1_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4020000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_2_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_3_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_4_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4050000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_5_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4060000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_6_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4070000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_7_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4080000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_8_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0xA4090000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_cips/Data1] [get_bd_addr_segs ctrl_sm_9_tie/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI2] [get_bd_addr_segs ps_noc/S02_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI3] [get_bd_addr_segs ps_noc/S03_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_NCI0] [get_bd_addr_segs ps_noc/S05_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_NCI1] [get_bd_addr_segs ps_noc/S06_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI1] [get_bd_addr_segs ps_noc/S01_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_CCI0] [get_bd_addr_segs ps_noc/S00_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_PMC] [get_bd_addr_segs ps_noc/S04_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_cips/DATA_RPU0] [get_bd_addr_segs ps_noc/S07_AXI/C0_DDR_LOW0] -force

  # Restore current instance
  current_bd_instance $oldCurInst

## =====================================================================
## Create PFM attributes
## =====================================================================

  #Set Platform Name
  set_property PFM_NAME {xilinx.com:vck190_v1_0:vck190_v1_0:1.0} [get_files [current_bd_design].bd]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_0
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_0]
  
  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_1
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_1]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_2
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_2]
  
  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_3
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_3]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_4
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_4]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_5
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_5]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_6
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_6]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_7
  set_property PFM.AXI_PORT {\
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_7]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_8
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_8]

#Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_9
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_9]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_10
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_10]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_11
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_11]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_12
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_12]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_13
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_13]

  #Expose 15 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_14
  set_property PFM.AXI_PORT {
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
    M15_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_14]

  #Expose 14 Control Interface AXI Masters on the AXI SmartConnect ctrl_sm_15
  set_property PFM.AXI_PORT {               \
    M01_AXI {memport "M_AXI_GP" sptag "GP"} \
    M02_AXI {memport "M_AXI_GP" sptag "GP"} \
    M03_AXI {memport "M_AXI_GP" sptag "GP"} \
    M04_AXI {memport "M_AXI_GP" sptag "GP"} \
    M05_AXI {memport "M_AXI_GP" sptag "GP"} \
    M06_AXI {memport "M_AXI_GP" sptag "GP"} \
    M07_AXI {memport "M_AXI_GP" sptag "GP"} \
    M08_AXI {memport "M_AXI_GP" sptag "GP"} \
    M09_AXI {memport "M_AXI_GP" sptag "GP"} \
    M10_AXI {memport "M_AXI_GP" sptag "GP"} \
    M11_AXI {memport "M_AXI_GP" sptag "GP"} \
    M12_AXI {memport "M_AXI_GP" sptag "GP"} \
    M13_AXI {memport "M_AXI_GP" sptag "GP"} \
    M14_AXI {memport "M_AXI_GP" sptag "GP"} \
  } [get_bd_cells /ctrl_sm_15]

  #Expose Memory Interfaces
  set_property PFM.AXI_PORT {                 \
    M01_AXI {memport "M_AXI_NOC" sptag "GP"}  \
    S08_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S09_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S10_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S11_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S12_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S13_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S14_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S15_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S16_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S17_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S18_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S19_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S20_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S21_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S22_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S23_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S24_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S25_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S26_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S27_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S28_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S29_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S30_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S31_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S32_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S33_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S34_AXI {memport "S_AXI_NOC" sptag "DDR"} \
    S35_AXI {memport "S_AXI_NOC" sptag "DDR"} \
  } [get_bd_cells /ps_noc]

  #Enable Platform Clocks
  set_property PFM.CLOCK { \
    clk_out1 {id "0" is_default "true" proc_sys_reset "/pl_clk_out1_rst" status "fixed"}  \
    clk_out2 {id "1" is_default "false" proc_sys_reset "/pl_clk_out2_rst" status "fixed"} \
    clk_out3 {id "2" is_default "false" proc_sys_reset "/pl_clk_out3_rst" status "fixed"} \
    clk_out4 {id "3" is_default "false" proc_sys_reset "/pl_clk_out4_rst" status "fixed"} \
    clk_out5 {id "4" is_default "false" proc_sys_reset "/pl_clk_out5_rst" status "fixed"} \
  } [get_bd_cells /pl_clkwiz]

  #Save Block Design 
  validate_bd_design
  save_bd_design
}
# End of create_root_design()

##################################################################
# MAIN FLOW
##################################################################
create_root_design ""
