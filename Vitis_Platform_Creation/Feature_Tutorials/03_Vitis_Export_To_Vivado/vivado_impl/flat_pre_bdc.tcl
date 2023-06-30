# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
# 
################################################################
# This is a generated script based on design: flat
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
# source flat_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xcvc1902-vsva2197-2MP-e-S
   set_property BOARD_PART xilinx.com:vck190:part0:3.1 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name flat

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
xilinx.com:ip:versal_cips:*\
xilinx.com:ip:axi_noc:*\
xilinx.com:ip:clk_wizard:*\
xilinx.com:ip:smartconnect:*\
xilinx.com:ip:axi_dbg_hub:*\
xilinx.com:ip:axi_intc:*\
xilinx.com:ip:proc_sys_reset:*\
xilinx.com:ip:ai_engine:*\
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


# Hierarchical cell: ExtensibleRegion
proc create_hier_cell_ExtensibleRegion { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_ExtensibleRegion() - Empty argument(s)!"}
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:inimm_rtl:1.0 AieControl
  set_property APERTURES {{0x200_0000_0000 4G}} [get_bd_intf_pins /ExtensibleRegion/AieControl]

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR0
  set_property APERTURES {{0x0 2G}} [get_bd_intf_pins /ExtensibleRegion/DDR0]

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR1
  set_property APERTURES {{0x0 2G}} [get_bd_intf_pins /ExtensibleRegion/DDR1]

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR2
  set_property APERTURES {{0x0 2G}} [get_bd_intf_pins /ExtensibleRegion/DDR2]

  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:inimm_rtl:1.0 DDR3
  set_property APERTURES {{0x0 2G}} [get_bd_intf_pins /ExtensibleRegion/DDR3]

  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:inimm_rtl:1.0 PlControl
  set_property APERTURES {{0x201_0000_0000 512M}} [get_bd_intf_pins /ExtensibleRegion/PlControl]


  # Create pins
  create_bd_pin -dir I -type clk ExtClock
  create_bd_pin -dir I -type rst ExtReset
  create_bd_pin -dir O -type intr irq

  # Create instance: ClockWiz, and set properties
  set ClockWiz [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wizard ClockWiz ]
  set_property -dict [list \
    CONFIG.CLKOUT_DRIVES {BUFG,BUFG,BUFG,BUFG,BUFG,BUFG,BUFG} \
    CONFIG.CLKOUT_DYN_PS {None,None,None,None,None,None,None} \
    CONFIG.CLKOUT_GROUPING {Auto,Auto,Auto,Auto,Auto,Auto,Auto} \
    CONFIG.CLKOUT_MATCHED_ROUTING {false,false,false,false,false,false,false} \
    CONFIG.CLKOUT_PORT {clk_out1,clk_out2,clk_out3,clk_out4,clk_out5,clk_out6,clk_out7} \
    CONFIG.CLKOUT_REQUESTED_DUTY_CYCLE {50.000,50.000,50.000,50.000,50.000,50.000,50.000} \
    CONFIG.CLKOUT_REQUESTED_OUT_FREQUENCY {100.000,150,100.000,100.000,100.000,100.000,100.000} \
    CONFIG.CLKOUT_REQUESTED_PHASE {0.000,0.000,0.000,0.000,0.000,0.000,0.000} \
    CONFIG.CLKOUT_USED {true,true,false,false,false,false,false} \
    CONFIG.USE_LOCKED {true} \
  ] $ClockWiz


  # Create instance: Control, and set properties
  set Control [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect Control ]
  set_property -dict [list \
    CONFIG.NUM_MI {2} \
    CONFIG.NUM_SI {1} \
  ] $Control


  # Create instance: ControlNoc, and set properties
  set ControlNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc ControlNoc ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {2} \
    CONFIG.NUM_MI {2} \
    CONFIG.NUM_NSI {2} \
    CONFIG.NUM_SI {0} \
  ] $ControlNoc


  set_property -dict [ list \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins /ExtensibleRegion/ControlNoc/M00_AXI]

  set_property -dict [ list \
   CONFIG.APERTURES {{0x201_0000_0000 1G}} \
   CONFIG.CATEGORY {pl} \
 ] [get_bd_intf_pins /ExtensibleRegion/ControlNoc/M01_AXI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {M00_AXI { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /ExtensibleRegion/ControlNoc/S00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {M01_AXI { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /ExtensibleRegion/ControlNoc/S01_INI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M01_AXI} \
 ] [get_bd_pins /ExtensibleRegion/ControlNoc/aclk0]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins /ExtensibleRegion/ControlNoc/aclk1]

  # Create instance: DDR, and set properties
  set DDR [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc DDR ]
  set_property -dict [list \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NMI {4} \
    CONFIG.NUM_SI {0} \
  ] $DDR


  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /ExtensibleRegion/DDR/M00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /ExtensibleRegion/DDR/M01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /ExtensibleRegion/DDR/M02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /ExtensibleRegion/DDR/M03_INI]

  # Create instance: DebugHub, and set properties
  set DebugHub [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dbg_hub DebugHub ]

  # Create instance: Interrupts, and set properties
  set Interrupts [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc Interrupts ]
  set_property CONFIG.C_IRQ_CONNECTION {1} $Interrupts


  # Create instance: PlReset0, and set properties
  set PlReset0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset PlReset0 ]

  # Create instance: PlReset1, and set properties
  set PlReset1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset PlReset1 ]

  # Create instance: ai_engine_0, and set properties
  set ai_engine_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ai_engine ai_engine_0 ]

  set_property -dict [ list \
   CONFIG.CATEGORY {NOC} \
 ] [get_bd_intf_pins /ExtensibleRegion/ai_engine_0/S00_AXI]

  # Create interface connections
  connect_bd_intf_net -intf_net axi_noc_1_M04_INI [get_bd_intf_pins AieControl] [get_bd_intf_pins ControlNoc/S00_INI]
  connect_bd_intf_net -intf_net axi_noc_1_M05_INI [get_bd_intf_pins PlControl] [get_bd_intf_pins ControlNoc/S01_INI]
  connect_bd_intf_net -intf_net axi_noc_2_M00_INI [get_bd_intf_pins DDR0] [get_bd_intf_pins DDR/M00_INI]
  connect_bd_intf_net -intf_net axi_noc_2_M01_INI [get_bd_intf_pins DDR1] [get_bd_intf_pins DDR/M01_INI]
  connect_bd_intf_net -intf_net axi_noc_2_M02_INI [get_bd_intf_pins DDR2] [get_bd_intf_pins DDR/M02_INI]
  connect_bd_intf_net -intf_net axi_noc_2_M03_INI [get_bd_intf_pins DDR3] [get_bd_intf_pins DDR/M03_INI]
  connect_bd_intf_net -intf_net axi_noc_3_M00_AXI [get_bd_intf_pins ControlNoc/M00_AXI] [get_bd_intf_pins ai_engine_0/S00_AXI]
  connect_bd_intf_net -intf_net axi_noc_3_M01_AXI [get_bd_intf_pins Control/S00_AXI] [get_bd_intf_pins ControlNoc/M01_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M00_AXI [get_bd_intf_pins Control/M00_AXI] [get_bd_intf_pins DebugHub/S_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M01_AXI [get_bd_intf_pins Control/M01_AXI] [get_bd_intf_pins Interrupts/s_axi]

  # Create port connections
  connect_bd_net -net ai_engine_0_s00_axi_aclk [get_bd_pins ControlNoc/aclk1] [get_bd_pins ai_engine_0/s00_axi_aclk]
  connect_bd_net -net axi_intc_0_irq [get_bd_pins irq] [get_bd_pins Interrupts/irq]
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins ClockWiz/clk_out1] [get_bd_pins Control/aclk] [get_bd_pins ControlNoc/aclk0] [get_bd_pins DebugHub/aclk] [get_bd_pins Interrupts/s_axi_aclk] [get_bd_pins PlReset0/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out2 [get_bd_pins ClockWiz/clk_out2] [get_bd_pins PlReset1/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_locked [get_bd_pins ClockWiz/locked] [get_bd_pins PlReset0/dcm_locked] [get_bd_pins PlReset1/dcm_locked]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins Control/aresetn] [get_bd_pins DebugHub/aresetn] [get_bd_pins Interrupts/s_axi_aresetn] [get_bd_pins PlReset0/peripheral_aresetn]
  connect_bd_net -net versal_cips_0_pl0_ref_clk [get_bd_pins ExtClock] [get_bd_pins ClockWiz/clk_in1]
  connect_bd_net -net versal_cips_0_pl0_resetn [get_bd_pins ExtReset] [get_bd_pins PlReset0/ext_reset_in] [get_bd_pins PlReset1/ext_reset_in]

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
  set ddr4_dimm1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddr4_rtl:1.0 ddr4_dimm1 ]

  set ddr4_dimm1_sma_clk [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 ddr4_dimm1_sma_clk ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {200000000} \
   ] $ddr4_dimm1_sma_clk


  # Create ports

  # Create instance: Cips, and set properties
  set Cips [ create_bd_cell -type ip -vlnv xilinx.com:ip:versal_cips Cips ]
  set_property -dict [list \
    CONFIG.CLOCK_MODE {Custom} \
    CONFIG.DDR_MEMORY_MODE {Custom} \
    CONFIG.DEBUG_MODE {Custom} \
    CONFIG.DESIGN_MODE {1} \
    CONFIG.PS_BOARD_INTERFACE {ps_pmc_fixed_io} \
    CONFIG.PS_PMC_CONFIG { \
      CLOCK_MODE {Custom} \
      DDR_MEMORY_MODE {Custom} \
      DESIGN_MODE {1} \
      PMC_CRP_PL0_REF_CTRL_FREQMHZ {100} \
      PMC_GPIO0_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 0 .. 25}}} \
      PMC_GPIO1_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 51}}} \
      PMC_MIO37 {{AUX_IO 0} {DIRECTION out} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA high} {PULL pullup} {SCHMITT 0} {SLEW slow} {USAGE GPIO}} \
      PMC_OSPI_PERIPHERAL {{ENABLE 0} {IO {PMC_MIO 0 .. 11}} {MODE Single}} \
      PMC_QSPI_COHERENCY {0} \
      PMC_QSPI_FBCLK {{ENABLE 1} {IO {PMC_MIO 6}}} \
      PMC_QSPI_PERIPHERAL_DATA_MODE {x4} \
      PMC_QSPI_PERIPHERAL_ENABLE {1} \
      PMC_QSPI_PERIPHERAL_MODE {Dual Parallel} \
      PMC_REF_CLK_FREQMHZ {33.3333} \
      PMC_SD1 {{CD_ENABLE 1} {CD_IO {PMC_MIO 28}} {POW_ENABLE 1} {POW_IO {PMC_MIO 51}} {RESET_ENABLE 0} {RESET_IO {PMC_MIO 12}} {WP_ENABLE 0} {WP_IO {PMC_MIO 1}}} \
      PMC_SD1_COHERENCY {0} \
      PMC_SD1_DATA_TRANSFER_MODE {8Bit} \
      PMC_SD1_PERIPHERAL {{CLK_100_SDR_OTAP_DLY 0x3} {CLK_200_SDR_OTAP_DLY 0x2} {CLK_50_DDR_ITAP_DLY 0x36} {CLK_50_DDR_OTAP_DLY 0x3} {CLK_50_SDR_ITAP_DLY 0x2C} {CLK_50_SDR_OTAP_DLY 0x4} {ENABLE 1} {IO\
{PMC_MIO 26 .. 36}}} \
      PMC_SD1_SLOT_TYPE {SD 3.0} \
      PMC_USE_PMC_NOC_AXI0 {1} \
      PS_BOARD_INTERFACE {ps_pmc_fixed_io} \
      PS_CAN1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 40 .. 41}}} \
      PS_ENET0_MDIO {{ENABLE 1} {IO {PS_MIO 24 .. 25}}} \
      PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 .. 11}}} \
      PS_ENET1_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 12 .. 23}}} \
      PS_GEN_IPI0_ENABLE {1} \
      PS_GEN_IPI0_MASTER {A72} \
      PS_GEN_IPI1_ENABLE {1} \
      PS_GEN_IPI2_ENABLE {1} \
      PS_GEN_IPI3_ENABLE {1} \
      PS_GEN_IPI4_ENABLE {1} \
      PS_GEN_IPI5_ENABLE {1} \
      PS_GEN_IPI6_ENABLE {1} \
      PS_I2C0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 46 .. 47}}} \
      PS_I2C1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 44 .. 45}}} \
      PS_IRQ_USAGE {{CH0 0} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0} {CH15 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 1} {CH9 0}} \
      PS_MIO19 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_MIO21 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_MIO7 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_MIO9 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} \
      PS_NUM_FABRIC_RESETS {1} \
      PS_PCIE_RESET {{ENABLE 1}} \
      PS_UART0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 42 .. 43}}} \
      PS_USB3_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 13 .. 25}}} \
      PS_USE_FPD_CCI_NOC {1} \
      PS_USE_FPD_CCI_NOC0 {1} \
      PS_USE_NOC_LPD_AXI0 {0} \
      PS_USE_PMCPL_CLK0 {1} \
      PS_USE_PMCPL_CLK1 {0} \
      PS_USE_PMCPL_CLK2 {0} \
      PS_USE_PMCPL_CLK3 {0} \
      SMON_ALARMS {Set_Alarms_On} \
      SMON_ENABLE_TEMP_AVERAGING {0} \
      SMON_TEMP_AVERAGING_SAMPLES {0} \
    } \
  ] $Cips


  # Create instance: CipsNoc, and set properties
  set CipsNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc CipsNoc ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {5} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NMI {6} \
    CONFIG.NUM_SI {5} \
  ] $CipsNoc


  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /CipsNoc/M00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /CipsNoc/M01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /CipsNoc/M02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /CipsNoc/M03_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /CipsNoc/M04_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins /CipsNoc/M05_INI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M04_INI { read_bw {1720} write_bw {1720}} M05_INI { read_bw {1720} write_bw {1720}} M00_INI { read_bw {1720} write_bw {1720}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /CipsNoc/S00_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M01_INI { read_bw {1720} write_bw {1720}} M04_INI { read_bw {1720} write_bw {1720}} M05_INI { read_bw {1720} write_bw {1720}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /CipsNoc/S01_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M02_INI { read_bw {1720} write_bw {1720}} M04_INI { read_bw {1720} write_bw {1720}} M05_INI { read_bw {1720} write_bw {1720}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /CipsNoc/S02_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M03_INI { read_bw {1720} write_bw {1720}} M04_INI { read_bw {1720} write_bw {1720}} M05_INI { read_bw {1720} write_bw {1720}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /CipsNoc/S03_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {M04_INI { read_bw {1720} write_bw {1720}} M05_INI { read_bw {1720} write_bw {1720}} M00_INI { read_bw {1720} write_bw {1720}} } \
   CONFIG.DEST_IDS {} \
   CONFIG.CATEGORY {ps_pmc} \
 ] [get_bd_intf_pins /CipsNoc/S04_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI} \
 ] [get_bd_pins /CipsNoc/aclk0]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S01_AXI} \
 ] [get_bd_pins /CipsNoc/aclk1]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S02_AXI} \
 ] [get_bd_pins /CipsNoc/aclk2]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S03_AXI} \
 ] [get_bd_pins /CipsNoc/aclk3]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S04_AXI} \
 ] [get_bd_pins /CipsNoc/aclk4]

  # Create instance: MemoryNoc, and set properties
  set MemoryNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc MemoryNoc ]
  set_property -dict [list \
    CONFIG.CH0_DDR4_0_BOARD_INTERFACE {ddr4_dimm1} \
    CONFIG.MC_CHAN_REGION1 {NONE} \
    CONFIG.MC_EN_INTR_RESP {TRUE} \
    CONFIG.MC_SYSTEM_CLOCK {Differential} \
    CONFIG.NUM_CLKS {0} \
    CONFIG.NUM_MC {1} \
    CONFIG.NUM_MCP {4} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NSI {8} \
    CONFIG.NUM_SI {0} \
    CONFIG.sys_clk0_BOARD_INTERFACE {ddr4_dimm1_sma_clk} \
  ] $MemoryNoc


  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_1 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_2 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_3 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S03_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S04_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_1 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S05_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_2 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S06_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_3 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
 ] [get_bd_intf_pins /MemoryNoc/S07_INI]

  # Create instance: ExtensibleRegion
  create_hier_cell_ExtensibleRegion [current_bd_instance .] ExtensibleRegion

  # Create interface connections
  connect_bd_intf_net -intf_net axi_noc_0_CH0_DDR4_0 [get_bd_intf_ports ddr4_dimm1] [get_bd_intf_pins MemoryNoc/CH0_DDR4_0]
  connect_bd_intf_net -intf_net axi_noc_1_M00_INI [get_bd_intf_pins CipsNoc/M00_INI] [get_bd_intf_pins MemoryNoc/S00_INI]
  connect_bd_intf_net -intf_net axi_noc_1_M01_INI [get_bd_intf_pins CipsNoc/M01_INI] [get_bd_intf_pins MemoryNoc/S01_INI]
  connect_bd_intf_net -intf_net axi_noc_1_M02_INI [get_bd_intf_pins CipsNoc/M02_INI] [get_bd_intf_pins MemoryNoc/S02_INI]
  connect_bd_intf_net -intf_net axi_noc_1_M03_INI [get_bd_intf_pins CipsNoc/M03_INI] [get_bd_intf_pins MemoryNoc/S03_INI]
  connect_bd_intf_net -intf_net axi_noc_1_M04_INI [get_bd_intf_pins CipsNoc/M04_INI] [get_bd_intf_pins ExtensibleRegion/AieControl]
  connect_bd_intf_net -intf_net axi_noc_1_M05_INI [get_bd_intf_pins CipsNoc/M05_INI] [get_bd_intf_pins ExtensibleRegion/PlControl]
  connect_bd_intf_net -intf_net axi_noc_2_M00_INI [get_bd_intf_pins MemoryNoc/S04_INI] [get_bd_intf_pins ExtensibleRegion/DDR0]
  connect_bd_intf_net -intf_net axi_noc_2_M01_INI [get_bd_intf_pins MemoryNoc/S05_INI] [get_bd_intf_pins ExtensibleRegion/DDR1]
  connect_bd_intf_net -intf_net axi_noc_2_M02_INI [get_bd_intf_pins MemoryNoc/S06_INI] [get_bd_intf_pins ExtensibleRegion/DDR2]
  connect_bd_intf_net -intf_net axi_noc_2_M03_INI [get_bd_intf_pins MemoryNoc/S07_INI] [get_bd_intf_pins ExtensibleRegion/DDR3]
  connect_bd_intf_net -intf_net ddr4_dimm1_sma_clk_1 [get_bd_intf_ports ddr4_dimm1_sma_clk] [get_bd_intf_pins MemoryNoc/sys_clk0]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_0 [get_bd_intf_pins Cips/FPD_CCI_NOC_0] [get_bd_intf_pins CipsNoc/S00_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_1 [get_bd_intf_pins Cips/FPD_CCI_NOC_1] [get_bd_intf_pins CipsNoc/S01_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_2 [get_bd_intf_pins Cips/FPD_CCI_NOC_2] [get_bd_intf_pins CipsNoc/S02_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_FPD_CCI_NOC_3 [get_bd_intf_pins Cips/FPD_CCI_NOC_3] [get_bd_intf_pins CipsNoc/S03_AXI]
  connect_bd_intf_net -intf_net versal_cips_0_PMC_NOC_AXI_0 [get_bd_intf_pins Cips/PMC_NOC_AXI_0] [get_bd_intf_pins CipsNoc/S04_AXI]

  # Create port connections
  connect_bd_net -net ExtensibleRegion_irq [get_bd_pins Cips/pl_ps_irq8] [get_bd_pins ExtensibleRegion/irq]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi0_clk [get_bd_pins Cips/fpd_cci_noc_axi0_clk] [get_bd_pins CipsNoc/aclk0]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi1_clk [get_bd_pins Cips/fpd_cci_noc_axi1_clk] [get_bd_pins CipsNoc/aclk1]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi2_clk [get_bd_pins Cips/fpd_cci_noc_axi2_clk] [get_bd_pins CipsNoc/aclk2]
  connect_bd_net -net versal_cips_0_fpd_cci_noc_axi3_clk [get_bd_pins Cips/fpd_cci_noc_axi3_clk] [get_bd_pins CipsNoc/aclk3]
  connect_bd_net -net versal_cips_0_pl0_ref_clk [get_bd_pins Cips/pl0_ref_clk] [get_bd_pins ExtensibleRegion/ExtClock]
  connect_bd_net -net versal_cips_0_pl0_resetn [get_bd_pins Cips/pl0_resetn] [get_bd_pins ExtensibleRegion/ExtReset]
  connect_bd_net -net versal_cips_0_pmc_axi_noc_axi0_clk [get_bd_pins Cips/pmc_axi_noc_axi0_clk] [get_bd_pins CipsNoc/aclk4]

  # Create address segments
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_0] [get_bd_addr_segs ExtensibleRegion/DebugHub/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0x020100200000 -range 0x00010000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_0] [get_bd_addr_segs ExtensibleRegion/Interrupts/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_0] [get_bd_addr_segs MemoryNoc/S00_INI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_0] [get_bd_addr_segs ExtensibleRegion/ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_1] [get_bd_addr_segs ExtensibleRegion/DebugHub/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0x020100200000 -range 0x00010000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_1] [get_bd_addr_segs ExtensibleRegion/Interrupts/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_1] [get_bd_addr_segs MemoryNoc/S01_INI/C1_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_1] [get_bd_addr_segs ExtensibleRegion/ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_2] [get_bd_addr_segs ExtensibleRegion/DebugHub/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0x020100200000 -range 0x00010000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_2] [get_bd_addr_segs ExtensibleRegion/Interrupts/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_2] [get_bd_addr_segs MemoryNoc/S02_INI/C2_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_2] [get_bd_addr_segs ExtensibleRegion/ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_3] [get_bd_addr_segs ExtensibleRegion/DebugHub/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0x020100200000 -range 0x00010000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_3] [get_bd_addr_segs ExtensibleRegion/Interrupts/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_3] [get_bd_addr_segs MemoryNoc/S03_INI/C3_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces Cips/FPD_CCI_NOC_3] [get_bd_addr_segs ExtensibleRegion/ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020100000000 -range 0x00200000 -target_address_space [get_bd_addr_spaces Cips/PMC_NOC_AXI_0] [get_bd_addr_segs ExtensibleRegion/DebugHub/S_AXI_DBG_HUB/Mem0] -force
  assign_bd_address -offset 0x020100200000 -range 0x00010000 -target_address_space [get_bd_addr_spaces Cips/PMC_NOC_AXI_0] [get_bd_addr_segs ExtensibleRegion/Interrupts/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces Cips/PMC_NOC_AXI_0] [get_bd_addr_segs MemoryNoc/S00_INI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces Cips/PMC_NOC_AXI_0] [get_bd_addr_segs ExtensibleRegion/ai_engine_0/S00_AXI/AIE_ARRAY_0] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  # Create PFM attributes
  set_property PFM.CLOCK {  clk_out1 {id "0" is_default "true" proc_sys_reset "PlReset0" status "fixed"}  clk_out2 {id "1" is_default "false" proc_sys_reset "PlReset1" status "fixed"}} [get_bd_cells /ExtensibleRegion/ClockWiz]
  set_property PFM.AXI_PORT {M02_AXI {} M03_AXI {} M04_AXI {} M05_AXI {} M06_AXI {} M07_AXI {} M08_AXI {} M09_AXI {} M10_AXI {} M11_AXI {} M12_AXI {} M13_AXI {} M14_AXI {} M15_AXI {}} [get_bd_cells /ExtensibleRegion/Control]
  set_property PFM.AXI_PORT {S00_AXI {sptag DDR} S01_AXI {sptag DDR} S02_AXI {sptag DDR} S03_AXI {sptag DDR} S04_AXI {sptag DDR} S05_AXI {sptag DDR} S06_AXI {sptag DDR} S07_AXI {sptag DDR} S08_AXI {sptag DDR} S09_AXI {sptag DDR} S10_AXI {sptag DDR} S11_AXI {sptag DDR} S12_AXI {sptag DDR} S13_AXI {sptag DDR} S14_AXI {sptag DDR} S15_AXI {sptag DDR} S16_AXI {sptag DDR} S17_AXI {sptag DDR} S18_AXI {sptag DDR} S19_AXI {sptag DDR}} [get_bd_cells /ExtensibleRegion/DDR]
  set_property PFM.IRQ {intr {id 0 range 31}} [get_bd_cells /ExtensibleRegion/Interrupts]


  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


