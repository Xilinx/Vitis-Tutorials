#
# Copyright 2021 Xilinx, Inc.
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
#

################################################################
# This is a generated script based on design: vck190
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
set scripts_vivado_version 2021.2
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
# source vck190_custom_bd_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

## CHANGE DESIGN NAME HERE
set design_name ${PLATFORM_NAME}
##set design_name [lindex $argv 0]
### WORKAROUND - Hardcode platform and device name as tcl arguments is not passed properly to Vitis
#set design_name vck190_thin
##set device_name [lindex $argv 1]
#set device_name xcvc1902-vsva2197-1LP-e-S-es1
#puts "Creating HW Block Design for : \"$design_name\" using : \"$device_name\""
##variable design_name
##set design_name vck190_dummy
##variable device_name
##set device_name xcvc1902-vsva2197-2MP-e-S-es1
#
#set list_projs [get_projects -quiet]
#if { $list_projs eq "" } {
#   create_project $design_name build/${design_name}_vivado -part $device_name
#   set_property BOARD_PART xilinx.com:vck190_es:part0:1.0 [current_project]
#}

#variable script_path
#set  script_path [file dirname [file normalize [info script]]]

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
xilinx.com:ip:ai_engine:2.0\
xilinx.com:ip:axi_intc:4.1\
xilinx.com:ip:axi_noc:1.0\
xilinx.com:ip:smartconnect:1.0\
xilinx.com:ip:versal_cips:3.1\
xilinx.com:ip:clk_wizard:1.0\
xilinx.com:ip:proc_sys_reset:5.0\
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
  set script_folder [_tcl::get_script_folder]
  # Create interface ports
  set CH0_DDR4_0_0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddr4_rtl:1.0 CH0_DDR4_0_0 ]

  set sys_clk0_0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 sys_clk0_0 ]
  set_property -dict [ list \
    CONFIG.FREQ_HZ {200000000} \
  ] $sys_clk0_0


  # Create ports

  # Create instance: ai_engine_0, and set properties
  set ai_engine_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ai_engine:2.0 ai_engine_0 ]

  set_property -dict [ list \
    CONFIG.CATEGORY {NOC} \
  ] [get_bd_intf_pins /ai_engine_0/S00_AXI]

  # Create instance: axi_intc_0, and set properties
  set axi_intc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:4.1 axi_intc_0 ]
  set_property -dict [ list \
    CONFIG.C_IRQ_CONNECTION {1} \
  ] $axi_intc_0

  # Create instance: axi_noc_0, and set properties
  set axi_noc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.0 axi_noc_0 ]
  set_property -dict [ list \
   CONFIG.CH0_DDR4_0_BOARD_INTERFACE {ddr4_dimm1} \
   CONFIG.sys_clk0_BOARD_INTERFACE {ddr4_dimm1_sma_clk} \
   CONFIG.MC0_CONFIG_NUM {config17} \
   CONFIG.MC1_CONFIG_NUM {config17} \
   CONFIG.MC2_CONFIG_NUM {config17} \
   CONFIG.MC3_CONFIG_NUM {config17} \
   CONFIG.MC_BOARD_INTRF_EN {true} \
   CONFIG.MC_CASLATENCY {22} \
   CONFIG.MC_CHAN_REGION1 {DDR_LOW1} \
   CONFIG.MC_CONFIG_NUM {config17} \
   CONFIG.MC_DDR4_2T {Disable} \
   CONFIG.MC_F1_LPDDR4_MR1 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR2 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR3 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR11 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR13 {0x0000} \
   CONFIG.MC_F1_LPDDR4_MR22 {0x0000} \
   CONFIG.MC_F1_TRCD {13750} \
   CONFIG.MC_F1_TRCDMIN {13750} \
   CONFIG.MC_INPUTCLK0_PERIOD {5000} \
   CONFIG.MC_INPUT_FREQUENCY0 {200.000} \
   CONFIG.MC_MEMORY_DEVICETYPE {UDIMMs} \
   CONFIG.MC_MEMORY_SPEEDGRADE {DDR4-3200AA(22-22-22)} \
   CONFIG.MC_TRC {45750} \
   CONFIG.MC_TRCD {13750} \
   CONFIG.MC_TRP {13750} \
   CONFIG.MC_TRPMIN {13750} \
   CONFIG.NUM_CLKS {9} \
   CONFIG.NUM_MC {1} \
   CONFIG.NUM_MCP {4} \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {8} \
 ] $axi_noc_0


  set_property SELECTED_SIM_MODEL rtl  $axi_noc_0
#   CONFIG.CH0_DDR4_0_BOARD_INTERFACE {ddr4_dimm1} \
#   CONFIG.sys_clk0_BOARD_INTERFACE {ddr4_dimm1_sma_clk} \
#   CONFIG.MC_BA_WIDTH {2} \
#   CONFIG.MC_BG_WIDTH {2} \
#   CONFIG.MC_CHAN_REGION0 {DDR_LOW0} \
#   CONFIG.MC_CHAN_REGION1 {NONE} \
#   CONFIG.MC_COMPONENT_WIDTH {x8} \
#   CONFIG.MC_CONFIG_NUM {config17} \
#   CONFIG.MC_DATAWIDTH {64} \
#   CONFIG.MC_DDR4_2T {Disable} \
#   CONFIG.MC_F1_LPDDR4_MR1 {0x0000} \
#   CONFIG.MC_F1_LPDDR4_MR2 {0x0000} \
#   CONFIG.MC_F1_TRCD {13750} \
#   CONFIG.MC_F1_TRCDMIN {13750} \
#   CONFIG.MC_INPUTCLK0_PERIOD {5000} \
#   CONFIG.MC_INPUT_FREQUENCY0 {200.000} \
#   CONFIG.MC_INTERLEAVE_SIZE {128} \
#   CONFIG.MC_MEMORY_DEVICETYPE {UDIMMs} \
#   CONFIG.MC_MEMORY_SPEEDGRADE {DDR4-3200AA(22-22-22)} \
#   CONFIG.MC_MEMORY_TIMEPERIOD0 {625} \
#   CONFIG.MC_NO_CHANNELS {Single} \
#   CONFIG.MC_PRE_DEF_ADDR_MAP_SEL {ROW_COLUMN_BANK} \
#   CONFIG.MC_RANK {1} \
#   CONFIG.MC_ROWADDRESSWIDTH {16} \
#   CONFIG.MC_STACKHEIGHT {1} \
#   CONFIG.MC_SYSTEM_CLOCK {Differential} \
#   CONFIG.MC_TRC {45750} \
#   CONFIG.MC_TRCD {13750} \
#   CONFIG.MC_TRCDMIN {13750} \
#   CONFIG.MC_TRCMIN {45750} \
#   CONFIG.MC_TRP {13750} \
#   CONFIG.MC_TRPMIN {13750} \

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins /axi_noc_0/M00_AXI]

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}} M00_AXI {read_bw {5} write_bw {5}}} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S00_AXI]

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}} M00_AXI { read_bw {5} write_bw {5}} } \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S01_AXI]

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}} M00_AXI { read_bw {5} write_bw {5}} } \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S02_AXI]

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CONNECTIONS {MC_0 { read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}} M00_AXI { read_bw {5} write_bw {5}} } \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins /axi_noc_0/S03_AXI]

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}}} \
   CONFIG.CATEGORY {ps_rpu} \
 ] [get_bd_intf_pins /axi_noc_0/S04_AXI]

  set_property -dict [ list \
   CONFIG.REGION {0} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {100} write_bw {100} read_avg_burst {4} write_avg_burst {4}} M00_AXI {read_bw {5} write_bw {5}}} \
   CONFIG.CATEGORY {ps_pmc} \
 ] [get_bd_intf_pins /axi_noc_0/S05_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_0 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
   CONFIG.CATEGORY {ps_nci} \
 ] [get_bd_intf_pins /axi_noc_0/S06_AXI]

  set_property -dict [ list \
   CONFIG.CONNECTIONS {MC_0 { read_bw {1720} write_bw {1720} read_avg_burst {4} write_avg_burst {4}} } \
   CONFIG.CATEGORY {ps_nci} \
 ] [get_bd_intf_pins /axi_noc_0/S07_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk0]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S01_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk1]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S02_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk2]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S03_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk3]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S04_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk4]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S05_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk5]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S06_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk6]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S07_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk7]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins /axi_noc_0/aclk8]

  # Create instance: axi_smc_0, and set properties
  set axi_smc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 axi_smc_0 ]
  set_property -dict [ list \
    CONFIG.NUM_CLKS {2} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_SI {1} \
  ] $axi_smc_0

  # Create instance: cips_0, and set properties
  set cips_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:versal_cips:3.1 cips_0 ]
  set_property -dict [ list \
    CONFIG.CLOCK_MODE {Custom} \
    CONFIG.DDR_MEMORY_MODE {Custom} \
   CONFIG.PS_BOARD_INTERFACE {Custom} \
    CONFIG.PS_PL_CONNECTIVITY_MODE {Custom} \
    CONFIG.PS_PMC_CONFIG {PS_GEN_IPI0_ENABLE 1 PS_GEN_IPI0_MASTER A72 PS_GEN_IPI1_ENABLE 1\
PS_GEN_IPI1_MASTER A72 PS_GEN_IPI2_ENABLE 1 PS_GEN_IPI2_MASTER A72\
PS_GEN_IPI3_ENABLE 1 PS_GEN_IPI3_MASTER A72 PS_GEN_IPI4_ENABLE 1\
PS_GEN_IPI4_MASTER A72 PS_GEN_IPI5_ENABLE 1 PS_GEN_IPI5_MASTER A72\
PS_GEN_IPI6_ENABLE 1 PS_GEN_IPI6_MASTER A72 CLOCK_MODE Custom DDR_MEMORY_MODE\
Custom PMC_CRP_PL0_REF_CTRL_FREQMHZ 99.999992 PMC_USE_PMC_NOC_AXI0 1\
PS_IRQ_USAGE {{CH0 1} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0}\
{CH15 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 0} {CH9 0}}\
PS_M_AXI_FPD_DATA_WIDTH 32 PS_NUM_FABRIC_RESETS 1 PS_PL_CONNECTIVITY_MODE Custom PS_USE_FPD_AXI_NOC0 1\
PS_USE_FPD_AXI_NOC1 1 PS_USE_FPD_CCI_NOC 1 PS_USE_M_AXI_FPD 1\
PS_USE_NOC_LPD_AXI0 1 PS_USE_PMCPL_CLK0 1 PS_MIO7 {{AUX_IO 0} {DIRECTION in}\
{DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW\
slow} {USAGE Reserved}} PS_MIO9 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA}\
{OUTPUT_DATA default} {PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}}\
PS_MIO19 {{AUX_IO 0} {DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default}\
{PULL disable} {SCHMITT 0} {SLEW slow} {USAGE Reserved}} PS_MIO21 {{AUX_IO 0}\
{DIRECTION in} {DRIVE_STRENGTH 8mA} {OUTPUT_DATA default} {PULL disable}\
{SCHMITT 0} {SLEW slow} {USAGE Reserved}} PMC_MIO37 {{AUX_IO 0} {DIRECTION out}\
{DRIVE_STRENGTH 8mA} {OUTPUT_DATA high} {PULL pullup} {SCHMITT 0} {SLEW slow}\
{USAGE GPIO}} PMC_SD1 {{CD_ENABLE 1} {CD_IO {PMC_MIO 28}} {POW_ENABLE 1}\
{POW_IO {PMC_MIO 51}} {RESET_ENABLE 0} {RESET_IO {PMC_MIO 1}} {WP_ENABLE 0}\
{WP_IO {PMC_MIO 1}}} PMC_SD1_COHERENCY 0 PMC_SD1_DATA_TRANSFER_MODE 8Bit\
PMC_SD1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 36}}} PMC_SD1_SLOT_TYPE {SD\
3.0} PMC_GPIO0_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 0 .. 25}}}\
PMC_GPIO1_MIO_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 51}}}\
PMC_I2CPMC_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 46 .. 47}}} PMC_OSPI_PERIPHERAL\
{{ENABLE 0} {IO {PMC_MIO 0 .. 11}} {MODE Single}} PMC_QSPI_COHERENCY 0\
PMC_QSPI_FBCLK {{ENABLE 1} {IO {PMC_MIO 6}}} PMC_QSPI_PERIPHERAL_DATA_MODE x4\
PMC_QSPI_PERIPHERAL_ENABLE 1 PMC_QSPI_PERIPHERAL_MODE {Dual Parallel}\
PS_CAN1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 40 .. 41}}} PS_ENET0_MDIO {{ENABLE\
1} {IO {PS_MIO 24 .. 25}}} PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 ..\
11}}} PS_ENET1_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 12 .. 23}}}\
PS_I2C1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 44 .. 45}}} PS_UART0_PERIPHERAL\
{{ENABLE 1} {IO {PMC_MIO 42 .. 43}}} PS_USB3_PERIPHERAL {{ENABLE 1} {IO\
{PMC_MIO 13 .. 25}}} PMC_REF_CLK_FREQMHZ 33.3333 PS_PCIE_RESET {{ENABLE 1} {IO\
{PMC_MIO 38 .. 39}}} PS_BOARD_INTERFACE cips_fixed_io SMON_ALARMS Set_Alarms_On\
SMON_ENABLE_TEMP_AVERAGING 0 SMON_TEMP_AVERAGING_SAMPLES 8 DESIGN_MODE 2\
PS_PCIE1_PERIPHERAL_ENABLE 0 PS_PCIE2_PERIPHERAL_ENABLE 0\
PCIE_APERTURES_SINGLE_ENABLE 0 PCIE_APERTURES_DUAL_ENABLE 0}\
    ] $cips_0

    #set_property -dict [list CONFIG.PS_PMC_CONFIG { CLOCK_MODE Custom  DDR_MEMORY_MODE {Connectivity to DDR via NOC}  DEBUG_MODE JTAG  DESIGN_MODE 2  IO_CONFIG_MODE Custom  PCIE_APERTURES_DUAL_ENABLE 0  PCIE_APERTURES_SINGLE_ENABLE 0  PMC_CRP_PL0_REF_CTRL_FREQMHZ 100  PMC_USE_PMC_NOC_AXI0 1  PS_BOARD_INTERFACE Custom  PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 37}}}  PS_HSDP_EGRESS_TRAFFIC JTAG  PS_HSDP_INGRESS_TRAFFIC JTAG  PS_HSDP_MODE None  PS_IRQ_USAGE {{CH0 0} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0} {CH15 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 1} {CH9 0}}  PS_M_AXI_FPD_DATA_WIDTH 32  PS_NUM_FABRIC_RESETS 1  PS_PCIE1_PERIPHERAL_ENABLE 0  PS_PCIE2_PERIPHERAL_ENABLE 0  PS_PL_CONNECTIVITY_MODE Custom  PS_UART0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 .. 1}}}  PS_USE_FPD_AXI_NOC0 1  PS_USE_FPD_AXI_NOC1 1  PS_USE_FPD_CCI_NOC 1  PS_USE_FPD_CCI_NOC0 1  PS_USE_M_AXI_FPD 1  PS_USE_M_AXI_LPD 0  PS_USE_NOC_LPD_AXI0 1  PS_USE_PMCPL_CLK0 1  PS_USE_PMCPL_CLK1 0  PS_USE_PMCPL_CLK2 0  PS_USE_S_AXI_FPD 0  PS_USE_S_AXI_LPD 0  SMON_ALARMS Set_Alarms_On  SMON_ENABLE_TEMP_AVERAGING 0  SMON_TEMP_AVERAGING_SAMPLES 8 } CONFIG.PS_PMC_CONFIG_APPLIED {1} CONFIG.IO_CONFIG_MODE {Custom}] [get_bd_cells cips_0]
# backup settings #    CONFIG.PS_PMC_CONFIG {
# backup settings #      CLOCK_MODE Custom \
# backup settings #      DDR_MEMORY_MODE {Custom} \
# backup settings #      DEBUG_MODE JTAG \
# backup settings #      DESIGN_MODE 2 \
# backup settings #      IO_CONFIG_MODE Custom \
# backup settings #      PCIE_APERTURES_DUAL_ENABLE 0 \
# backup settings #      PCIE_APERTURES_SINGLE_ENABLE 0 \
# backup settings #      PMC_CRP_PL0_REF_CTRL_FREQMHZ 100 \
# backup settings #      PMC_USE_PMC_NOC_AXI0 1 \
# backup settings #      PS_BOARD_INTERFACE Custom \
# backup settings #      PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 26 .. 37}}} \
# backup settings #      PS_HSDP_EGRESS_TRAFFIC JTAG \
# backup settings #      PS_HSDP_INGRESS_TRAFFIC JTAG \
# backup settings #      PS_HSDP_MODE None \
# backup settings #      PS_IRQ_USAGE {{CH0 0} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0} {CH15 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 1} {CH9 0}} \
# backup settings #      PS_M_AXI_FPD_DATA_WIDTH 32 \
# backup settings #      PS_NUM_FABRIC_RESETS 1 \
# backup settings #      PS_PCIE1_PERIPHERAL_ENABLE 0 \
# backup settings #      PS_PCIE2_PERIPHERAL_ENABLE 0 \
# backup settings #      PS_PL_CONNECTIVITY_MODE Custom \
# backup settings #      PS_UART0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 .. 1}}} \
# backup settings #      PS_USE_FPD_AXI_NOC0 1 \
# backup settings #      PS_USE_FPD_AXI_NOC1 1 \
# backup settings #      PS_USE_FPD_CCI_NOC 1 \
# backup settings #      PS_USE_FPD_CCI_NOC0 1 \
# backup settings #      PS_USE_M_AXI_FPD 1 \
# backup settings #      PS_USE_M_AXI_LPD 0 \
# backup settings #      PS_USE_NOC_LPD_AXI0 1 \
# backup settings #      PS_USE_PMCPL_CLK0 1 \
# backup settings #      PS_USE_PMCPL_CLK1 0 \
# backup settings #      PS_USE_PMCPL_CLK2 0 \
# backup settings #      PS_USE_S_AXI_FPD 0 \
# backup settings #      PS_USE_S_AXI_LPD 0 \
# backup settings #      SMON_ALARMS Set_Alarms_On \
# backup settings #      SMON_ENABLE_TEMP_AVERAGING 0 \
# backup settings #      SMON_TEMP_AVERAGING_SAMPLES 8 \
# backup settings #    } \

  # Create instance: clk_wizard_0, and set properties
  set clk_wizard_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wizard:1.0 clk_wizard_0 ]
  set_property -dict [ list \
   CONFIG.JITTER_SEL {No_Jitter} \
   CONFIG.OVERRIDE_PRIMITIVE {true} \
   CONFIG.BANDWIDTH {HIGH} \
   CONFIG.CLKFBOUT_MULT {40.000000} \
   CONFIG.CLKOUT1_DIVIDE {8.000000} \
   CONFIG.CLKOUT2_DIVIDE {12.000000} \
   CONFIG.CLKOUT3_DIVIDE {30.000000} \
   CONFIG.CLKOUT_DRIVES {MBUFGCE,BUFG,BUFG,BUFG,BUFG,BUFG,BUFG} \
   CONFIG.CLKOUT_DYN_PS {None,None,None,None,None,None,None} \
   CONFIG.CLKOUT_GROUPING {Auto,Auto,Auto,Auto,Auto,Auto,Auto} \
   CONFIG.CLKOUT_MATCHED_ROUTING {false,false,false,false,false,false,false} \
   CONFIG.CLKOUT_MBUFGCE_MODE {PERFORMANCE,PERFORMANCE,PERFORMANCE,PERFORMANCE,PERFORMANCE,PERFORMANCE,PERFORMANCE} \
   CONFIG.CLKOUT_PORT {clk_out1,clk_out2,clk_out3,clk_out4,clk_out5,clk_out6,clk_out7} \
   CONFIG.CLKOUT_REQUESTED_DUTY_CYCLE {50.000,50.000,50.000,50.000,50.000,50.000,50.000} \
   CONFIG.CLKOUT_REQUESTED_OUT_FREQUENCY {500.000,250.000,100.000,100.000,100.000,100.000,100.000} \
   CONFIG.CLKOUT_REQUESTED_PHASE {0.000,0.000,0.000,0.000,0.000,0.000,0.000} \
   CONFIG.CLKOUT_USED {true,false,false,false,false,false,false} \
   CONFIG.RESET_TYPE {ACTIVE_LOW} \
   CONFIG.USE_LOCKED {true} \
   CONFIG.USE_RESET {true} \
  ] $clk_wizard_0

  # Create instance: sys_reset_2, and set properties
  set sys_reset_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 sys_reset_2 ]

  # Create instance: sys_reset_1, and set properties
  set sys_reset_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 sys_reset_1 ]

  # Create instance: sys_reset_0, and set properties
  set sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 sys_reset_0 ]

  # Create interface connections
  connect_bd_intf_net -intf_net axi_noc_0_CH0_DDR4_0 [get_bd_intf_ports CH0_DDR4_0_0] [get_bd_intf_pins axi_noc_0/CH0_DDR4_0]
  connect_bd_intf_net -intf_net axi_noc_0_M00_AXI [get_bd_intf_pins ai_engine_0/S00_AXI] [get_bd_intf_pins axi_noc_0/M00_AXI]
  connect_bd_intf_net -intf_net axi_smc_0_M00_AXI [get_bd_intf_pins axi_intc_0/s_axi] [get_bd_intf_pins axi_smc_0/M00_AXI]
  connect_bd_intf_net -intf_net cips_0_FPD_AXI_NOC_0 [get_bd_intf_pins axi_noc_0/S06_AXI] [get_bd_intf_pins cips_0/FPD_AXI_NOC_0]
  connect_bd_intf_net -intf_net cips_0_FPD_AXI_NOC_1 [get_bd_intf_pins axi_noc_0/S07_AXI] [get_bd_intf_pins cips_0/FPD_AXI_NOC_1]
  connect_bd_intf_net -intf_net cips_0_FPD_CCI_NOC_0 [get_bd_intf_pins axi_noc_0/S00_AXI] [get_bd_intf_pins cips_0/FPD_CCI_NOC_0]
  connect_bd_intf_net -intf_net cips_0_FPD_CCI_NOC_1 [get_bd_intf_pins axi_noc_0/S01_AXI] [get_bd_intf_pins cips_0/FPD_CCI_NOC_1]
  connect_bd_intf_net -intf_net cips_0_FPD_CCI_NOC_2 [get_bd_intf_pins axi_noc_0/S02_AXI] [get_bd_intf_pins cips_0/FPD_CCI_NOC_2]
  connect_bd_intf_net -intf_net cips_0_FPD_CCI_NOC_3 [get_bd_intf_pins axi_noc_0/S03_AXI] [get_bd_intf_pins cips_0/FPD_CCI_NOC_3]
  connect_bd_intf_net -intf_net cips_0_LPD_AXI_NOC_0 [get_bd_intf_pins axi_noc_0/S04_AXI] [get_bd_intf_pins cips_0/LPD_AXI_NOC_0]
  connect_bd_intf_net -intf_net cips_0_M_AXI_FPD [get_bd_intf_pins axi_smc_0/S00_AXI] [get_bd_intf_pins cips_0/M_AXI_FPD]
  connect_bd_intf_net -intf_net cips_0_PMC_NOC_AXI_0 [get_bd_intf_pins axi_noc_0/S05_AXI] [get_bd_intf_pins cips_0/PMC_NOC_AXI_0]
  connect_bd_intf_net -intf_net sys_clk0_0_1 [get_bd_intf_ports sys_clk0_0] [get_bd_intf_pins axi_noc_0/sys_clk0]

  # Create port connections
  connect_bd_net -net ai_engine_0_s00_axi_aclk [get_bd_pins ai_engine_0/s00_axi_aclk] [get_bd_pins axi_noc_0/aclk8]
  connect_bd_net -net axi_intc_0_irq [get_bd_pins axi_intc_0/irq] [get_bd_pins cips_0/pl_ps_irq0]
  connect_bd_net -net clk_wizard_0_clk_out1 [get_bd_pins clk_wizard_0/clk_out1_o1] [get_bd_pins sys_reset_0/slowest_sync_clk] [get_bd_pins axi_smc_0/aclk1]
  connect_bd_net -net clk_wizard_0_clk_out2 [get_bd_pins clk_wizard_0/clk_out1_o2] [get_bd_pins sys_reset_1/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out3 [get_bd_pins clk_wizard_0/clk_out1_o3] [get_bd_pins sys_reset_2/slowest_sync_clk] [get_bd_pins axi_smc_0/aclk] [get_bd_pins cips_0/m_axi_fpd_aclk] [get_bd_pins axi_intc_0/s_axi_aclk]
  connect_bd_net -net clk_wizard_0_locked [get_bd_pins clk_wizard_0/locked] [get_bd_pins sys_reset_0/dcm_locked] [get_bd_pins sys_reset_0/dcm_locked] [get_bd_pins sys_reset_1/dcm_locked]
  connect_bd_net -net sys_reset_0_peripheral_aresetn [get_bd_pins sys_reset_0/peripheral_aresetn] [get_bd_pins axi_intc_0/s_axi_aresetn] [get_bd_pins axi_smc_0/aresetn]
  connect_bd_net -net cips_0_fpd_axi_noc_axi0_clk [get_bd_pins axi_noc_0/aclk6] [get_bd_pins cips_0/fpd_axi_noc_axi0_clk]
  connect_bd_net -net cips_0_fpd_axi_noc_axi1_clk [get_bd_pins axi_noc_0/aclk7] [get_bd_pins cips_0/fpd_axi_noc_axi1_clk]
  connect_bd_net -net cips_0_fpd_cci_noc_axi0_clk [get_bd_pins axi_noc_0/aclk0] [get_bd_pins cips_0/fpd_cci_noc_axi0_clk]
  connect_bd_net -net cips_0_fpd_cci_noc_axi1_clk [get_bd_pins axi_noc_0/aclk1] [get_bd_pins cips_0/fpd_cci_noc_axi1_clk]
  connect_bd_net -net cips_0_fpd_cci_noc_axi2_clk [get_bd_pins axi_noc_0/aclk2] [get_bd_pins cips_0/fpd_cci_noc_axi2_clk]
  connect_bd_net -net cips_0_fpd_cci_noc_axi3_clk [get_bd_pins axi_noc_0/aclk3] [get_bd_pins cips_0/fpd_cci_noc_axi3_clk]
  connect_bd_net -net cips_0_lpd_axi_noc_clk [get_bd_pins axi_noc_0/aclk4] [get_bd_pins cips_0/lpd_axi_noc_clk]
  connect_bd_net -net cips_0_pl0_ref_clk [get_bd_pins cips_0/pl0_ref_clk] [get_bd_pins clk_wizard_0/clk_in1]
  connect_bd_net -net cips_0_pl0_resetn [get_bd_pins cips_0/pl0_resetn] [get_bd_pins clk_wizard_0/resetn] [get_bd_pins clk_wizard_0/clk_out1_ce] [get_bd_pins clk_wizard_0/clk_out1_clr_n] [get_bd_pins sys_reset_2/ext_reset_in] [get_bd_pins sys_reset_1/ext_reset_in] [get_bd_pins sys_reset_0/ext_reset_in]
  connect_bd_net -net cips_0_pmc_axi_noc_axi0_clk [get_bd_pins axi_noc_0/aclk5] [get_bd_pins cips_0/pmc_axi_noc_axi0_clk]

  # Create address segments
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_0] [get_bd_addr_segs axi_noc_0/S00_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_1] [get_bd_addr_segs axi_noc_0/S01_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_2] [get_bd_addr_segs axi_noc_0/S02_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_3] [get_bd_addr_segs axi_noc_0/S03_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_AXI_NOC_0] [get_bd_addr_segs axi_noc_0/S06_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_AXI_NOC_1] [get_bd_addr_segs axi_noc_0/S07_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/LPD_AXI_NOC_0] [get_bd_addr_segs axi_noc_0/S04_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces cips_0/PMC_NOC_AXI_0] [get_bd_addr_segs axi_noc_0/S05_AXI/C0_DDR_LOW0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_1] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_2] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces cips_0/FPD_CCI_NOC_3] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces cips_0/PMC_NOC_AXI_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xA4000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces cips_0/M_AXI_FPD] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force


  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


