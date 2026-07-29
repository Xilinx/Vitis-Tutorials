
################################################################
# This is a generated script based on design: versal_comn_platform
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
set scripts_vivado_version 2026.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   if { [string compare $scripts_vivado_version $current_vivado_version] > 0 } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2042 -severity "ERROR" " This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Sourcing the script failed since it was created with a future version of Vivado."}

   } else {
     catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   }

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source dr.db.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc2ve3858-ssva2112-2MP-e-S
   set_property BOARD_PART xilinx.com:vek385:part0:1.1 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name vek385_thin

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
xilinx.com:ip:ps_wizard:1.0\
xilinx.com:ip:axi_noc2:1.1\
xilinx.com:ip:util_ds_buf:2.2\
xilinx.com:ip:ai_engine:2.0\
xilinx.com:inline_hdl:ilconstant:1.0\
xilinx.com:ip:smartconnect:1.0\
xilinx.com:ip:axi_intc:4.1\
xilinx.com:ip:clkx5_wiz:1.0\
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
  set gt_refclk0 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 gt_refclk0 ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {100000000} \
   ] $gt_refclk0

  set gt_refclk1 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 gt_refclk1 ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {100000000} \
   ] $gt_refclk1

  set MMI_GT [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gt_rtl:1.0 MMI_GT ]

  set C0_LPDDR5X_bank700_701 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:lpddr5_rtl:1.0 C0_LPDDR5X_bank700_701 ]

  set C1_LPDDR5X_bank703_704 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:lpddr5_rtl:1.0 C1_LPDDR5X_bank703_704 ]

  set C2_LPDDR5X_bank708_709 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:lpddr5_rtl:1.0 C2_LPDDR5X_bank708_709 ]

  set C3_LPDDR5X_bank710_711 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:lpddr5_rtl:1.0 C3_LPDDR5X_bank710_711 ]

  set C4_LPDDR5X_bank714_715 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:lpddr5_rtl:1.0 C4_LPDDR5X_bank714_715 ]

  set lpddr5_clk0_1 [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 lpddr5_clk0_1 ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {320000000} \
   ] $lpddr5_clk0_1


  # Create ports

  # Create instance: ps_wizard_0, and set properties
  set ps_wizard_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ps_wizard:1.0 ps_wizard_0 ]
  set_property -dict [list \
    CONFIG.MMI_CONFIG(DPDC_PRESENTATION_MODE) {Non_Live} \
    CONFIG.MMI_CONFIG(MDB5_GT) {PCIe0_x2_10GbE} \
    CONFIG.MMI_CONFIG(MMI_DP_ENABLE_BEFORE_PL) {1} \
    CONFIG.MMI_CONFIG(MMI_DP_HPD) {PMC_MIO_48} \
    CONFIG.MMI_CONFIG(MMI_GPU_ENABLE) {1} \
    CONFIG.MMI_CONFIG(MMI_PCIE0_PERST) {None} \
    CONFIG.MMI_CONFIG(MMI_PCIE0_PORT_TYPE) {Root_Port_of_PCIe_Root_Port_Complex} \
    CONFIG.MMI_CONFIG(MMI_USB3_ENABLE) {1} \
    CONFIG.MMI_CONFIG(PCIE0_TYPE1_MEMBASE_MEMLIMIT) {Enabled} \
    CONFIG.MMI_CONFIG(PMC_REF_CLK_FREQMHZ) {33.3333333} \
    CONFIG.MMI_CONFIG(PS_CRL_GEM_TSU_REF_CTRL_FREQMHZ) {250} \
    CONFIG.MMI_CONFIG(PS_GEM_TSU) {ENABLE 1 IO PS_MIO_24 IO_TYPE MIO} \
    CONFIG.MMI_CONFIG(UDH_GT) {DP_X2_+_USB} \
    CONFIG.PS11_CONFIG(MDB5_GT) {PCIe0_x2_10GbE} \
    CONFIG.PS11_CONFIG(MMI_DP_HPD) {PMC_MIO_48} \
    CONFIG.PS11_CONFIG(MMI_GPU_ENABLE) {1} \
    CONFIG.PS11_CONFIG(MMI_PCIE0_PERST) {None} \
    CONFIG.PS11_CONFIG(MMI_PCIE0_PORT_TYPE) {Root_Port_of_PCIe_Root_Port_Complex} \
    CONFIG.PS11_CONFIG(PL_FPD_IRQ_USAGE) {CH0 1 CH1 1 CH2 1 CH3 1 CH4 0 CH5 0 CH6 0 CH7 0} \
    CONFIG.PS11_CONFIG(PL_LPD_IRQ_USAGE) {CH0 1 CH1 1 CH2 1 CH3 1 CH4 0 CH5 0 CH6 0 CH7 0 CH8 0 CH9 0 CH10 0 CH11 0 CH12 0 CH13 0 CH14 0 CH15 0 CH16 0 CH17 0 CH18 0 CH19 0 CH20 0 CH21 0 CH22 0 CH23 0}\
\
    CONFIG.PS11_CONFIG(PMC_CRP_OSPI_REF_CTRL_FREQMHZ) {200} \
    CONFIG.PS11_CONFIG(PMC_CRP_PL0_REF_CTRL_FREQMHZ) {100} \
    CONFIG.PS11_CONFIG(PMC_CRP_PL1_REF_CTRL_FREQMHZ) {134} \
    CONFIG.PS11_CONFIG(PMC_CRP_PL2_REF_CTRL_FREQMHZ) {250} \
    CONFIG.PS11_CONFIG(PMC_HSM1_CLK_OUT_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PMC_MIO26) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PMC_MIO40) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PMC_OSPI_ECC_FAIL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PMC_OSPI_ECC_FAIL_IO) {PMC_MIO_13} \
    CONFIG.PS11_CONFIG(PMC_OSPI_PERIPHERAL) {ENABLE 1 IO PMC_MIO_0:13 MODE Single} \
    CONFIG.PS11_CONFIG(PMC_REF_CLK_FREQMHZ) {33.333} \
    CONFIG.PS11_CONFIG(PMC_UFS) {CD_ENABLE 0 CD_IO PMC_MIO_26 RSTN_IO PMC_MIO_25 CFGCLK_IO PMC_MIO_24} \
    CONFIG.PS11_CONFIG(PMC_UFS_PERIPHERAL) {ENABLE 1 IO PMC_MIO_24:26 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PMC_USE_PMC_AXI_NOC0) {1} \
    CONFIG.PS11_CONFIG(PS_ASU_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_CAN0_PERIPHERAL) {ENABLE 1 IO PMC_MIO_16:17 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_CAN1_PERIPHERAL) {ENABLE 1 IO PMC_MIO_18:19 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_CAN2_PERIPHERAL) {ENABLE 1 IO PMC_MIO_20:21 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_CAN3_PERIPHERAL) {ENABLE 1 IO PMC_MIO_22:23 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_CRL_GEM_TSU_REF_CTRL_FREQMHZ) {250} \
    CONFIG.PS11_CONFIG(PS_CRL_I3C_I2C0_REF_CTRL_FREQMHZ) {160} \
    CONFIG.PS11_CONFIG(PS_ENET0_MDIO) {ENABLE 1 IO PMC_MIO_50:51 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_ENET0_PERIPHERAL) {ENABLE 1 IO PS_MIO_0:11 IO_TYPE MIO MODE RGMII} \
    CONFIG.PS11_CONFIG(PS_FPD_AXI_PL_DATA_WIDTH) {64} \
    CONFIG.PS11_CONFIG(PS_GEM_TSU) {ENABLE 1 IO PS_MIO_24 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_GEM_TSU_CLK_PORT_PAIR) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI1_NOBUF_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI2_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI2_MASTER) {R52_1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI2_NOBUF_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI2_NOBUF_MASTER) {R52_0} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI3_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI3_NOBUF_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI3_NOBUF_MASTER) {R52_6} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI4_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI4_NOBUF_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI4_NOBUF_MASTER) {R52_7} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI5_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI5_NOBUF_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI5_NOBUF_MASTER) {R52_8} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI6_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI6_NOBUF_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_GEN_IPI6_NOBUF_MASTER) {R52_9} \
    CONFIG.PS11_CONFIG(PS_I2CSYSMON_PERIPHERAL) {ENABLE 0 IO_TYPE MIO IO PS_MIO_18:19} \
    CONFIG.PS11_CONFIG(PS_I3C_I2C0_PERIPHERAL) {ENABLE 1 IO PMC_MIO_42:43 IO_TYPE MIO TYPE I2C} \
    CONFIG.PS11_CONFIG(PS_I3C_I2C1_PERIPHERAL) {ENABLE 1 IO PMC_MIO_44:45 IO_TYPE MIO TYPE I2C} \
    CONFIG.PS11_CONFIG(PS_LPD_AXI_PL_DATA_WIDTH) {64} \
    CONFIG.PS11_CONFIG(PS_MIO11) {DRIVE_STRENGTH 8mA SLEW slow PULL disable SCHMITT 0 AUX_IO 0 USAGE Reserved OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_MIO12) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_MIO13) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION out} \
    CONFIG.PS11_CONFIG(PS_MIO14) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION out} \
    CONFIG.PS11_CONFIG(PS_MIO15) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_MIO20) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION inout} \
    CONFIG.PS11_CONFIG(PS_MIO21) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_MIO22) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION out} \
    CONFIG.PS11_CONFIG(PS_MIO23) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_MIO25) {DRIVE_STRENGTH 8mA SLEW slow PULL pullup SCHMITT 0 AUX_IO 0 USAGE GPIO OUTPUT_DATA default DIRECTION out} \
    CONFIG.PS11_CONFIG(PS_MIO7) {DRIVE_STRENGTH 8mA SLEW slow PULL disable SCHMITT 0 AUX_IO 0 USAGE Reserved OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_MIO9) {DRIVE_STRENGTH 8mA SLEW slow PULL disable SCHMITT 0 AUX_IO 0 USAGE Reserved OUTPUT_DATA default DIRECTION in} \
    CONFIG.PS11_CONFIG(PS_NUM_FABRIC_RESETS) {1} \
    CONFIG.PS11_CONFIG(PS_TTC0_CLK) {ENABLE 0 IO PS_MIO_6 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_TTC0_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC0_WAVEOUT) {ENABLE 1 IO PMC_MIO_49 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_TTC1_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC2_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC3_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC4_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC5_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC6_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_TTC7_PERIPHERAL_ENABLE) {1} \
    CONFIG.PS11_CONFIG(PS_UART0_PERIPHERAL) {ENABLE 1 IO PS_MIO_16:17 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_UART1_PERIPHERAL) {ENABLE 1 IO PMC_MIO_46:47 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_USB1_PERIPHERAL) {ENABLE 1 IO PMC_MIO_27:39 IO_TYPE MIO} \
    CONFIG.PS11_CONFIG(PS_USE_FPD_AXI_NOC) {1} \
    CONFIG.PS11_CONFIG(PS_USE_FPD_AXI_PL) {1} \
    CONFIG.PS11_CONFIG(PS_USE_LPD_AXI_NOC) {1} \
    CONFIG.PS11_CONFIG(PS_USE_LPD_AXI_PL) {1} \
    CONFIG.PS11_CONFIG(PS_USE_PMCPL_CLK0) {1} \
    CONFIG.PS11_CONFIG(PS_USE_PMCPL_CLK1) {1} \
    CONFIG.PS11_CONFIG(PS_USE_PMCPL_CLK2) {1} \
    CONFIG.PS11_CONFIG(SECONDARY_BOOT_DEVICE) {None} \
    CONFIG.PS11_CONFIG(SMON_INTERFACE_TO_USE) {I2C} \
    CONFIG.PS11_CONFIG(SMON_MEAS18) {ENABLE 1 MODE 2V_unipolar NAME VCCAUX AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 32} \
    CONFIG.PS11_CONFIG(SMON_MEAS19) {ENABLE 1 MODE 2V_unipolar NAME VCCAUX_LPD AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 33} \
    CONFIG.PS11_CONFIG(SMON_MEAS21) {ENABLE 1 MODE 2V_unipolar NAME VCCINT AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 40} \
    CONFIG.PS11_CONFIG(SMON_MEAS22) {ENABLE 1 MODE 2V_unipolar NAME VCCINT_MMI_MMI AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 41} \
    CONFIG.PS11_CONFIG(SMON_MEAS39) {ENABLE 1 MODE 2V_unipolar NAME VCC_PMC AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 34} \
    CONFIG.PS11_CONFIG(SMON_MEAS40) {ENABLE 1 MODE 2V_unipolar NAME VCC_PSFP AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 35} \
    CONFIG.PS11_CONFIG(SMON_MEAS41) {ENABLE 1 MODE 2V_unipolar NAME VCC_PSLP AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 36} \
    CONFIG.PS11_CONFIG(SMON_MEAS42) {ENABLE 1 MODE 2V_unipolar NAME VCC_RAM AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 37} \
    CONFIG.PS11_CONFIG(SMON_MEAS43) {ENABLE 1 MODE 2V_unipolar NAME VCC_SOC AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 2.00 SUPPLY_NUM 38} \
    CONFIG.PS11_CONFIG(SMON_MEAS44) {ENABLE 1 MODE 1V_unipolar NAME VP_VN AVERAGE_EN 0 ALARM_ENABLE 0 ALARM_LOWER 0.00 ALARM_UPPER 1.00 SUPPLY_NUM 39} \
    CONFIG.PS11_CONFIG(SMON_PMBUS_ADDRESS) {0x18} \
    CONFIG.PS11_CONFIG(UDH_GT) {DP_X2_+_USB} \
    CONFIG.PS_BOARD_INTERFACE {ps_pmc_fixed_io} \
  ] $ps_wizard_0


  set_property SELECTED_SIM_MODEL tlm  $ps_wizard_0

  # Create instance: Master_NoC, and set properties
  set Master_NoC [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc2:1.1 Master_NoC ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {11} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NMI {7} \
    CONFIG.NUM_SI {11} \
    CONFIG.SI_SIDEBAND_PINS {} \
  ] $Master_NoC


  set_property SELECTED_SIM_MODEL tlm  $Master_NoC

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M03_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M04_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M05_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $Master_NoC/M06_INI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}} M00_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S00_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M01_INI {read_bw {500} write_bw {500} initial_boot {true}} M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S01_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M02_INI {read_bw {500} write_bw {500} initial_boot {true}} M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S02_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M03_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S03_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}} M00_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S04_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M01_INI {read_bw {500} write_bw {500} initial_boot {true}} M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S05_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M02_INI {read_bw {500} write_bw {500} initial_boot {true}} M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S06_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M03_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_cci} \
 ] [get_bd_intf_pins $Master_NoC/S07_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}} M00_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_rpu} \
 ] [get_bd_intf_pins $Master_NoC/S08_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}} M00_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_pmc} \
 ] [get_bd_intf_pins $Master_NoC/S09_AXI]

  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CONNECTIONS {M06_INI {read_bw {500} write_bw {500} initial_boot {true}} M04_INI {read_bw {500} write_bw {500} initial_boot {true}} M05_INI {read_bw {500} write_bw {500} initial_boot {true}} M00_INI {read_bw {500} write_bw {500} initial_boot {true}}} \
   CONFIG.DEST_IDS {} \
   CONFIG.NOC_PARAMS {} \
   CONFIG.CATEGORY {ps_mmi} \
 ] [get_bd_intf_pins $Master_NoC/S10_AXI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI} \
 ] [get_bd_pins $Master_NoC/aclk0]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S01_AXI} \
 ] [get_bd_pins $Master_NoC/aclk1]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S02_AXI} \
 ] [get_bd_pins $Master_NoC/aclk2]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S03_AXI} \
 ] [get_bd_pins $Master_NoC/aclk3]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S04_AXI} \
 ] [get_bd_pins $Master_NoC/aclk4]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S05_AXI} \
 ] [get_bd_pins $Master_NoC/aclk5]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S06_AXI} \
 ] [get_bd_pins $Master_NoC/aclk6]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S07_AXI} \
 ] [get_bd_pins $Master_NoC/aclk7]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S08_AXI} \
 ] [get_bd_pins $Master_NoC/aclk8]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S09_AXI} \
 ] [get_bd_pins $Master_NoC/aclk9]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S10_AXI} \
 ] [get_bd_pins $Master_NoC/aclk10]

  # Create instance: NoC_C0_C1, and set properties
  set NoC_C0_C1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc2:1.1 NoC_C0_C1 ]
  set_property -dict [list \
    CONFIG.C0_CH0_LPDDR5_BOARD_INTERFACE {lpddr5_Controller_c0} \
    CONFIG.C1_CH0_LPDDR5_BOARD_INTERFACE {lpddr5_Controller_c1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ADDRESS_MAP) {NA,NA,NA,NA,NA,NA,NA,NA,RA14,RA13,RA12,RA11,RA10,RA9,RA8,RA7,RA6,RA5,RA4,RA3,RA2,RA1,RA0,BA1,BA0,BG1,BG0,CA5,CA4,CA3,CA2,CA1,NC,CA0,NC,NC,NC,NC,NA,NA} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_AUTO_PRECHARGE) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BACKGROUND_SCRUB) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BA_ADDR_WIDTH) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BG_WIDTH) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BOARD_INTRF_EN) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BURST_ADDR_WIDTH) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CAL_MASK_POLL) {ENABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_COL_ADDR_WIDTH) {6} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CONTROLLERTYPE) {LPDDR5_SDRAM} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CRYPTO) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DATA_WIDTH) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_2T) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_PAR_RCD_EN) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_RDIMM_ADDR_MODE) {DDR} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TFAW_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD_SLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1_DPR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2_DPR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFCSB) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFCSB_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DEVICE_TYPE) {Components} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DM_EN) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DQS_OSCI_EN) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DRAM_SIZE) {8Gb} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DRAM_WIDTH) {x16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_EXTENDED_DDRMC5E) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_CL) {64} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_CWL) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR2_RU) {16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR_RU) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TPD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TRP) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TRRD_L) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_BANK_ARCH) {BG} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TCSPD) {10938} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRPAB) {21000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRPPB) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRRD) {3750} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_RL) {25} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TCCD_L) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TCK) {938} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TFAW) {15000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRAS) {42000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRCD) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRTP) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRTP_RU) {24} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TXP) {7000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TZQLAT) {30000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_WL) {12} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_CL) {64} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_CWL) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR2_RU) {16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR_RU) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TPD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TRP) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TRRD_L) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_BANK_ARCH) {BG} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TCSPD) {10938} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRPAB) {21000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRPPB) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRRD) {3750} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_RL) {25} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TCCD_L) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TCK) {938} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TFAW) {15000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRAS) {42000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRCD) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRTP) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRTP_RU) {24} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TXP) {7000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TZQLAT) {30000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_WL) {12} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_FPGA_DEVICE_TYPE) {NON_KSB} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_FREQ_SWITCHING) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INLINE_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INPUTCLK0_PERIOD) {3127} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INTERLEAVE_SIZE) {128} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LATENCY_MODE) {x16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LBDQ_SWAP) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LOW_TRFC_DPR) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TPBR2ACT) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TPBR2PBR) {90000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFCAB) {210000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFCPB) {120000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFMAB) {210000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFMPB) {170000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_X64_EN) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MAIN_DEVICE_TYPE) {Components} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC0_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC1_CONFIG_SEL) {config12_opt} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC2_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC3_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC4_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC5_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC6_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC7_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MEMORY_DENSITY) {2GB} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MEM_FILL) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_CH) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_CK) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_MC) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_MCP) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_RANKS) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_SLOTS) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ON_DIE_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_OP_TEMPERATURE) {LOW} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_OTF_SCRUB) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_PERIODIC_READ) {ENABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_PRE_DEF_ADDR_MAP_SEL) {ROW_BANK_COLUMN} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_RD_DBI) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REFRESH_MODE) {NORMAL} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REFRESH_TYPE) {ALL_BANK} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REF_AND_PER_CAL_INTF) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REG_SCRUB_INTVL) {0x015180} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ROW_ADDR_WIDTH) {15} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SCRUB_SIZE) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SELF_REFRESH) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SIDE_BAND_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SILICON_REVISION) {NA} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SPEED_GRADE) {LPDDR5X-8533} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_STACK_HEIGHT) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SYSTEM_CLOCK) {No_Buffer} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_TREFI) {3906000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_UBLAZE_BLI_INTF) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_USER_DEFINED_ADDRESS_MAP) {None} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_USER_REFRESH) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_WL_SET) {A} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_WR_DBI) {true} \
    CONFIG.MC_CHAN_REGION1 {DDR_CH0_MED} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NSI {5} \
    CONFIG.NUM_SI {0} \
  ] $NoC_C0_C1


  set_property SELECTED_SIM_MODEL tlm  $NoC_C0_C1

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C0_C1/S00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C0_C1/S01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C0_C1/S02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C0_C1/S03_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C0_C1/S04_INI]

  # Create instance: NoC_C2_C3, and set properties
  set NoC_C2_C3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc2:1.1 NoC_C2_C3 ]
  set_property -dict [list \
    CONFIG.C0_CH0_LPDDR5_BOARD_INTERFACE {lpddr5_Controller_c2_int} \
    CONFIG.C1_CH0_LPDDR5_BOARD_INTERFACE {lpddr5_Controller_c3_int} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ADDRESS_MAP) {NA,NA,NA,NA,NA,NA,NA,NA,RA14,RA13,RA12,RA11,BA1,BA0,BG1,RA10,RA9,RA8,RA7,RA6,RA5,RA4,RA3,RA2,RA1,RA0,CA5,CA4,CA3,CA2,CA1,CA0,NC,BG0,NC,NC,NC,NC,NA,NA} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_AUTO_PRECHARGE) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BACKGROUND_SCRUB) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BA_ADDR_WIDTH) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BG_WIDTH) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BOARD_INTRF_EN) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BURST_ADDR_WIDTH) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CAL_MASK_POLL) {ENABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_COL_ADDR_WIDTH) {6} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CONTROLLERTYPE) {LPDDR5_SDRAM} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CRYPTO) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DATA_WIDTH) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_2T) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_PAR_RCD_EN) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_RDIMM_ADDR_MODE) {DDR} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TFAW_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD_SLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1_DPR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2_DPR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFCSB) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFCSB_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DEVICE_TYPE) {Components} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DM_EN) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DQS_OSCI_EN) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DRAM_SIZE) {8Gb} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DRAM_WIDTH) {x16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_EXTENDED_DDRMC5E) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_CL) {64} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_CWL) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR2_RU) {16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR_RU) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TPD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TRP) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TRRD_L) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_BANK_ARCH) {BG} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TCSPD) {10952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRPAB) {21000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRPPB) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRRD) {3750} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_RL) {25} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TCCD_L) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TCK) {952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TFAW) {15000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRAS) {42000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRCD) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRTP) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRTP_RU) {24} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TXP) {7000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TZQLAT) {30000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_WL) {12} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_CL) {64} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_CWL) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR2_RU) {16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR_RU) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TPD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TRP) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TRRD_L) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_BANK_ARCH) {BG} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TCSPD) {10952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRPAB) {21000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRPPB) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRRD) {3750} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_RL) {25} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TCCD_L) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TCK) {952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TFAW) {15000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRAS) {42000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRCD) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRTP) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRTP_RU) {24} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TXP) {7000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TZQLAT) {30000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_WL) {12} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_FPGA_DEVICE_TYPE) {NON_KSB} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_FREQ_SWITCHING) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INLINE_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INPUTCLK0_PERIOD) {4998} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INTERLEAVE_SIZE) {128} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LATENCY_MODE) {x16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LBDQ_SWAP) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LOW_TRFC_DPR) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TPBR2ACT) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TPBR2PBR) {90000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFCAB) {210000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFCPB) {120000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFMAB) {210000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFMPB) {170000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_X64_EN) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MAIN_DEVICE_TYPE) {Components} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC0_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC1_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC2_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC3_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC4_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC5_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC6_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC7_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MEMORY_DENSITY) {2GB} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MEM_FILL) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_CH) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_CK) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_MC) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_MCP) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_RANKS) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_SLOTS) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ON_DIE_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_OP_TEMPERATURE) {LOW} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_OTF_SCRUB) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_PERIODIC_READ) {ENABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_PRE_DEF_ADDR_MAP_SEL) {ROW_BANK_COLUMN} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_RD_DBI) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REFRESH_MODE) {NORMAL} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REFRESH_TYPE) {ALL_BANK} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REF_AND_PER_CAL_INTF) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REG_SCRUB_INTVL) {0x015180} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ROW_ADDR_WIDTH) {15} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SCRUB_SIZE) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SELF_REFRESH) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SIDE_BAND_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SILICON_REVISION) {NA} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SPEED_GRADE) {LPDDR5X-8533} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_STACK_HEIGHT) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SYSTEM_CLOCK) {Internal} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_TREFI) {3906000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_UBLAZE_BLI_INTF) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_USER_DEFINED_ADDRESS_MAP) {None} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_USER_REFRESH) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_WL_SET) {A} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_WR_DBI) {true} \
    CONFIG.MC_CHAN_REGION0 {DDR_CH1} \
    CONFIG.NUM_MCP {2} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NSI {3} \
    CONFIG.NUM_SI {0} \
  ] $NoC_C2_C3


  set_property SELECTED_SIM_MODEL tlm  $NoC_C2_C3

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C2_C3/S00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C2_C3/S01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_1 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C2_C3/S02_INI]

  # Create instance: NoC_C4, and set properties
  set NoC_C4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc2:1.1 NoC_C4 ]
  set_property -dict [list \
    CONFIG.C0_CH0_LPDDR5_BOARD_INTERFACE {lpddr5_Controller_c4_int} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ADDRESS_MAP) {NA,NA,NA,NA,NA,NA,NA,NA,NA,RA14,RA13,RA12,RA11,BA1,BA0,BG1,RA10,RA9,RA8,RA7,RA6,RA5,RA4,RA3,RA2,RA1,RA0,CA5,CA4,CA3,CA2,CA1,CA0,BG0,NC,NC,NC,NC,NA,NA} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_AUTO_PRECHARGE) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BACKGROUND_SCRUB) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BA_ADDR_WIDTH) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BG_WIDTH) {2} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BOARD_INTRF_EN) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_BURST_ADDR_WIDTH) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CAL_MASK_POLL) {ENABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_COL_ADDR_WIDTH) {6} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CONTROLLERTYPE) {LPDDR5_SDRAM} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_CRYPTO) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DATA_WIDTH) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_2T) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_PAR_RCD_EN) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_RDIMM_ADDR_MODE) {DDR} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TFAW_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TREFSBRD_SLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC1_DPR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFC2_DPR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFCSB) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DDR5_TRFCSB_DLR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DEVICE_TYPE) {Components} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DM_EN) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DQS_OSCI_EN) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DRAM_SIZE) {8Gb} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_DRAM_WIDTH) {x16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_EXTENDED_DDRMC5E) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_CL) {64} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_CWL) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR2_RU) {16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TCCD_L_WR_RU) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TPD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TRP) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_DDR5_TRRD_L) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_BANK_ARCH) {BG} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TCSPD) {10952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRPAB) {21000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRPPB) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_LP5_TRRD) {3750} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_RL) {25} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TCCD_L) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TCK) {952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TFAW) {15000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRAS) {42000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRCD) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRTP) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TRTP_RU) {24} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TXP) {7000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_TZQLAT) {30000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F0_WL) {12} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_CL) {64} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_CWL) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR2) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR2_RU) {16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TCCD_L_WR_RU) {32} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TPD) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TRP) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_DDR5_TRRD_L) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_BANK_ARCH) {BG} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TCSPD) {10952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRPAB) {21000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRPPB) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_LP5_TRRD) {3750} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_RL) {25} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TCCD_L) {4} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TCK) {952} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TFAW) {15000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRAS) {42000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRCD) {18000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRTP) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TRTP_RU) {24} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TXP) {7000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_TZQLAT) {30000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_F1_WL) {12} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_FPGA_DEVICE_TYPE) {NON_KSB} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_FREQ_SWITCHING) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INLINE_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INPUTCLK0_PERIOD) {4998} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_INTERLEAVE_SIZE) {0} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LATENCY_MODE) {x16} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LBDQ_SWAP) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LOW_TRFC_DPR) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TPBR2ACT) {7500} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TPBR2PBR) {90000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFCAB) {210000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFCPB) {120000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFMAB) {210000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_TRFMPB) {170000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_LP5_X64_EN) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MAIN_DEVICE_TYPE) {Components} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC0_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC1_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC2_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC3_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC4_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC5_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC6_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MC7_CONFIG_SEL) {config13} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MEMORY_DENSITY) {2GB} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_MEM_FILL) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_CH) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_CK) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_MC) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_MCP) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_RANKS) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_NUM_SLOTS) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ON_DIE_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_OP_TEMPERATURE) {LOW} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_OTF_SCRUB) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_PERIODIC_READ) {ENABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_PRE_DEF_ADDR_MAP_SEL) {ROW_BANK_COLUMN} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_RD_DBI) {true} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REFRESH_MODE) {NORMAL} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REFRESH_TYPE) {ALL_BANK} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REF_AND_PER_CAL_INTF) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_REG_SCRUB_INTVL) {0x015180} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_ROW_ADDR_WIDTH) {15} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SCRUB_SIZE) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SELF_REFRESH) {DISABLE} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SIDE_BAND_ECC) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SILICON_REVISION) {NA} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SPEED_GRADE) {LPDDR5X-8533} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_STACK_HEIGHT) {1} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_SYSTEM_CLOCK) {Internal} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_TREFI) {3906000} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_UBLAZE_BLI_INTF) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_USER_DEFINED_ADDRESS_MAP) {None} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_USER_REFRESH) {false} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_WL_SET) {A} \
    CONFIG.DDRMC5_CONFIG(DDRMC5_WR_DBI) {true} \
    CONFIG.MC_CHAN_REGION0 {DDR_CH2} \
    CONFIG.NUM_MCP {2} \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NSI {3} \
    CONFIG.NUM_SI {0} \
  ] $NoC_C4


  set_property SELECTED_SIM_MODEL tlm  $NoC_C4

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C4/S00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_0 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C4/S01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {MC_1 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4} initial_boot {true}}} \
 ] [get_bd_intf_pins $NoC_C4/S02_INI]

  # Create instance: util_ds_buf_0, and set properties
  set util_ds_buf_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.2 util_ds_buf_0 ]
  set_property -dict [list \
    CONFIG.DIFF_CLK_IN_BOARD_INTERFACE {lpddr5_clk0_1} \
    CONFIG.USE_BOARD_FLOW {true} \
  ] $util_ds_buf_0


  # Create instance: aggr_noc, and set properties
  set aggr_noc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc2:1.1 aggr_noc ]
  set_property -dict [list \
    CONFIG.NUM_MI {0} \
    CONFIG.NUM_NMI {5} \
    CONFIG.NUM_NSI {0} \
    CONFIG.NUM_SI {0} \
  ] $aggr_noc


  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $aggr_noc/M00_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $aggr_noc/M01_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $aggr_noc/M02_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $aggr_noc/M03_INI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
 ] [get_bd_intf_pins $aggr_noc/M04_INI]

  # Create instance: ai_engine_0, and set properties
  set ai_engine_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ai_engine:2.0 ai_engine_0 ]

  set_property -dict [ list \
   CONFIG.CATEGORY {NOC} \
 ] [get_bd_intf_pins $ai_engine_0/S00_AXI]

  # Create instance: ConfigNoc, and set properties
  set ConfigNoc [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc2:1.1 ConfigNoc ]
  set_property -dict [list \
    CONFIG.MI_SIDEBAND_PINS {} \
    CONFIG.NUM_NSI {1} \
    CONFIG.NUM_SI {0} \
  ] $ConfigNoc


  set_property -dict [ list \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.CATEGORY {aie} \
 ] [get_bd_intf_pins $ConfigNoc/M00_AXI]

  set_property -dict [ list \
   CONFIG.INI_STRATEGY {load} \
   CONFIG.CONNECTIONS {M00_AXI {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4}}} \
 ] [get_bd_intf_pins $ConfigNoc/S00_INI]

  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {M00_AXI} \
 ] [get_bd_pins $ConfigNoc/aclk0]

  # Create instance: ilconstant_0, and set properties
  set ilconstant_0 [ create_bd_cell -type inline_hdl -vlnv xilinx.com:inline_hdl:ilconstant:1.0 ilconstant_0 ]
  set_property -dict [list \
    CONFIG.CONST_VAL {3} \
    CONFIG.CONST_WIDTH {2} \
  ] $ilconstant_0


  # Create instance: ilconstant_1, and set properties
  set ilconstant_1 [ create_bd_cell -type inline_hdl -vlnv xilinx.com:inline_hdl:ilconstant:1.0 ilconstant_1 ]

  # Create instance: ctrl_smc, and set properties
  set ctrl_smc [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 ctrl_smc ]
  set_property -dict [list \
    CONFIG.NUM_CLKS {1} \
    CONFIG.NUM_MI {1} \
    CONFIG.NUM_SI {1} \
  ] $ctrl_smc


  # Create instance: axi_intc_0, and set properties
  set axi_intc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:4.1 axi_intc_0 ]
  set_property -dict [list \
    CONFIG.C_ASYNC_INTR {0xFFFFFFFF} \
    CONFIG.C_IRQ_CONNECTION {1} \
  ] $axi_intc_0


  # Create instance: clk_wizard_0, and set properties
  set clk_wizard_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clkx5_wiz:1.0 clk_wizard_0 ]
  set_property -dict [list \
    CONFIG.CE_TYPE {HARDSYNC} \
    CONFIG.CLKOUT_DRIVES {MBUFGCE,BUFG,BUFG,BUFG,BUFG,BUFG,BUFG} \
    CONFIG.CLKOUT_DYN_PS {None,None,None,None,None,None,None} \
    CONFIG.CLKOUT_GROUPING {Auto,Auto,Auto,Auto,Auto,Auto,Auto} \
    CONFIG.CLKOUT_MATCHED_ROUTING {false,false,false,false,false,false,false} \
    CONFIG.CLKOUT_PORT {clk_out1,clk_out2,clk_out3,clk_out4,clk_out5,clk_out6,clk_out7} \
    CONFIG.CLKOUT_REQUESTED_DUTY_CYCLE {50.000,50.000,50.000,50.000,50.000,50.000,50.000} \
    CONFIG.CLKOUT_REQUESTED_OUT_FREQUENCY {500,333.33,100.000,100.000,100.000,100.000,100.000} \
    CONFIG.CLKOUT_REQUESTED_PHASE {0.000,0.000,0.000,0.000,0.000,0.000,0.000} \
    CONFIG.CLKOUT_USED {true,true,false,false,false,false,false} \
    CONFIG.PRIM_SOURCE {No_buffer} \
    CONFIG.RESET_TYPE {ACTIVE_LOW} \
    CONFIG.USE_PHASE_ALIGNMENT {true} \
  ] $clk_wizard_0


  # Create instance: proc_sys_reset_o4, and set properties
  set proc_sys_reset_o4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_o4 ]

  # Create instance: proc_sys_reset_1, and set properties
  set proc_sys_reset_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_1 ]

  # Create interface connections
  connect_bd_intf_net -intf_net ConfigNoc_M00_AXI [get_bd_intf_pins ConfigNoc/M00_AXI] [get_bd_intf_pins ai_engine_0/S00_AXI]
  connect_bd_intf_net -intf_net Master_NoC_M00_INI [get_bd_intf_pins Master_NoC/M00_INI] [get_bd_intf_pins NoC_C0_C1/S00_INI]
  connect_bd_intf_net -intf_net Master_NoC_M01_INI [get_bd_intf_pins Master_NoC/M01_INI] [get_bd_intf_pins NoC_C0_C1/S01_INI]
  connect_bd_intf_net -intf_net Master_NoC_M02_INI [get_bd_intf_pins Master_NoC/M02_INI] [get_bd_intf_pins NoC_C0_C1/S02_INI]
  connect_bd_intf_net -intf_net Master_NoC_M03_INI [get_bd_intf_pins Master_NoC/M03_INI] [get_bd_intf_pins NoC_C0_C1/S03_INI]
  connect_bd_intf_net -intf_net Master_NoC_M04_INI [get_bd_intf_pins Master_NoC/M04_INI] [get_bd_intf_pins NoC_C2_C3/S00_INI]
  connect_bd_intf_net -intf_net Master_NoC_M05_INI [get_bd_intf_pins Master_NoC/M05_INI] [get_bd_intf_pins NoC_C4/S00_INI]
  connect_bd_intf_net -intf_net Master_NoC_M06_INI [get_bd_intf_pins ConfigNoc/S00_INI] [get_bd_intf_pins Master_NoC/M06_INI]
  connect_bd_intf_net -intf_net NoC_C0_C1_C0_CH0_LPDDR5 [get_bd_intf_ports C0_LPDDR5X_bank700_701] [get_bd_intf_pins NoC_C0_C1/C0_CH0_LPDDR5]
  connect_bd_intf_net -intf_net NoC_C0_C1_C1_CH0_LPDDR5 [get_bd_intf_ports C1_LPDDR5X_bank703_704] [get_bd_intf_pins NoC_C0_C1/C1_CH0_LPDDR5]
  connect_bd_intf_net -intf_net NoC_C2_C3_C0_CH0_LPDDR5 [get_bd_intf_ports C2_LPDDR5X_bank708_709] [get_bd_intf_pins NoC_C2_C3/C0_CH0_LPDDR5]
  connect_bd_intf_net -intf_net NoC_C2_C3_C1_CH0_LPDDR5 [get_bd_intf_ports C3_LPDDR5X_bank710_711] [get_bd_intf_pins NoC_C2_C3/C1_CH0_LPDDR5]
  connect_bd_intf_net -intf_net NoC_C4_C0_CH0_LPDDR5 [get_bd_intf_ports C4_LPDDR5X_bank714_715] [get_bd_intf_pins NoC_C4/C0_CH0_LPDDR5]
  connect_bd_intf_net -intf_net aggr_noc_M00_INI [get_bd_intf_pins aggr_noc/M00_INI] [get_bd_intf_pins NoC_C0_C1/S04_INI]
  connect_bd_intf_net -intf_net aggr_noc_M01_INI [get_bd_intf_pins aggr_noc/M01_INI] [get_bd_intf_pins NoC_C2_C3/S01_INI]
  connect_bd_intf_net -intf_net aggr_noc_M02_INI [get_bd_intf_pins aggr_noc/M02_INI] [get_bd_intf_pins NoC_C2_C3/S02_INI]
  connect_bd_intf_net -intf_net aggr_noc_M03_INI [get_bd_intf_pins aggr_noc/M03_INI] [get_bd_intf_pins NoC_C4/S01_INI]
  connect_bd_intf_net -intf_net aggr_noc_M04_INI [get_bd_intf_pins aggr_noc/M04_INI] [get_bd_intf_pins NoC_C4/S02_INI]
  connect_bd_intf_net -intf_net gt_refclk0_1 [get_bd_intf_pins ps_wizard_0/gt_refclk0] [get_bd_intf_ports gt_refclk0]
  connect_bd_intf_net -intf_net gt_refclk1_1 [get_bd_intf_pins ps_wizard_0/gt_refclk1] [get_bd_intf_ports gt_refclk1]
  connect_bd_intf_net -intf_net icn_ctrl_M00_AXI [get_bd_intf_pins axi_intc_0/s_axi] [get_bd_intf_pins ctrl_smc/M00_AXI]
  connect_bd_intf_net -intf_net lpddr5_clk0_1_1 [get_bd_intf_ports lpddr5_clk0_1] [get_bd_intf_pins util_ds_buf_0/CLK_IN_D]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC0 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC0] [get_bd_intf_pins Master_NoC/S00_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC1 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC1] [get_bd_intf_pins Master_NoC/S01_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC2 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC2] [get_bd_intf_pins Master_NoC/S02_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC3 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC3] [get_bd_intf_pins Master_NoC/S03_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC4 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC4] [get_bd_intf_pins Master_NoC/S04_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC5 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC5] [get_bd_intf_pins Master_NoC/S05_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC6 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC6] [get_bd_intf_pins Master_NoC/S06_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_NOC7 [get_bd_intf_pins ps_wizard_0/FPD_AXI_NOC7] [get_bd_intf_pins Master_NoC/S07_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_FPD_AXI_PL [get_bd_intf_pins ps_wizard_0/FPD_AXI_PL] [get_bd_intf_pins ctrl_smc/S00_AXI]
  set_property HDL_ATTRIBUTE.DONT_TOUCH {true} [get_bd_intf_nets ps_wizard_0_FPD_AXI_PL]
  connect_bd_intf_net -intf_net ps_wizard_0_LPD_AXI_NOC0 [get_bd_intf_pins ps_wizard_0/LPD_AXI_NOC0] [get_bd_intf_pins Master_NoC/S08_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_MMI_DC_AXI_NOC0 [get_bd_intf_pins ps_wizard_0/MMI_DC_AXI_NOC0] [get_bd_intf_pins Master_NoC/S10_AXI]
  connect_bd_intf_net -intf_net ps_wizard_0_MMI_GT [get_bd_intf_pins ps_wizard_0/MMI_GT] [get_bd_intf_ports MMI_GT]
  connect_bd_intf_net -intf_net ps_wizard_0_PMC_AXI_NOC0 [get_bd_intf_pins ps_wizard_0/PMC_AXI_NOC0] [get_bd_intf_pins Master_NoC/S09_AXI]

  # Create port connections
  connect_bd_net -net ai_engine_0_s00_axi_aclk  [get_bd_pins ai_engine_0/s00_axi_aclk] \
  [get_bd_pins ConfigNoc/aclk0]
  connect_bd_net -net axi_intc_0_irq  [get_bd_pins axi_intc_0/irq] \
  [get_bd_pins ps_wizard_0/pl_fpd_irq0]
  connect_bd_net -net clk_wizard_0_clk_out1_o4  [get_bd_pins clk_wizard_0/clk_out1_o4] \
  [get_bd_pins axi_intc_0/s_axi_aclk] \
  [get_bd_pins ps_wizard_0/fpd_axi_pl_aclk] \
  [get_bd_pins ctrl_smc/aclk] \
  [get_bd_pins ps_wizard_0/lpd_axi_pl_aclk] \
  [get_bd_pins proc_sys_reset_o4/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_clk_out2  [get_bd_pins clk_wizard_0/clk_out2] \
  [get_bd_pins proc_sys_reset_1/slowest_sync_clk]
  connect_bd_net -net clk_wizard_0_locked  [get_bd_pins clk_wizard_0/locked] \
  [get_bd_pins proc_sys_reset_o4/dcm_locked] \
  [get_bd_pins proc_sys_reset_1/dcm_locked] \
  [get_bd_pins clk_wizard_0/clk_out1_clr_n] \
  [get_bd_pins clk_wizard_0/clk_out1_ce]
  connect_bd_net -net ilconstant_0_dout  [get_bd_pins ilconstant_0/dout] \
  [get_bd_pins ps_wizard_0/gem0_tsu_inc_ctrl]
  connect_bd_net -net ilconstant_1_dout  [get_bd_pins ilconstant_1/dout] \
  [get_bd_pins ps_wizard_0/pl_mmi_dc_1x_clk] \
  [get_bd_pins ps_wizard_0/pl_mmi_dc_2x_clk] \
  [get_bd_pins ps_wizard_0/pl_mmi_dc_i2s_s0_clk]
  connect_bd_net -net proc_sys_reset_o4_peripheral_aresetn  [get_bd_pins proc_sys_reset_o4/peripheral_aresetn] \
  [get_bd_pins axi_intc_0/s_axi_aresetn] \
  [get_bd_pins ctrl_smc/aresetn]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc0_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc0_clk] \
  [get_bd_pins Master_NoC/aclk0]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc1_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc1_clk] \
  [get_bd_pins Master_NoC/aclk1]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc2_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc2_clk] \
  [get_bd_pins Master_NoC/aclk2]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc3_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc3_clk] \
  [get_bd_pins Master_NoC/aclk3]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc4_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc4_clk] \
  [get_bd_pins Master_NoC/aclk4]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc5_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc5_clk] \
  [get_bd_pins Master_NoC/aclk5]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc6_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc6_clk] \
  [get_bd_pins Master_NoC/aclk6]
  connect_bd_net -net ps_wizard_0_fpd_axi_noc7_clk  [get_bd_pins ps_wizard_0/fpd_axi_noc7_clk] \
  [get_bd_pins Master_NoC/aclk7]
  connect_bd_net -net ps_wizard_0_hsm1_ref_clk  [get_bd_pins ps_wizard_0/hsm1_ref_clk] \
  [get_bd_pins NoC_C2_C3/sys_clk0] \
  [get_bd_pins NoC_C4/sys_clk0] \
  [get_bd_pins NoC_C2_C3/sys_clk1]
  connect_bd_net -net ps_wizard_0_lpd_axi_noc0_clk  [get_bd_pins ps_wizard_0/lpd_axi_noc0_clk] \
  [get_bd_pins Master_NoC/aclk8]
  connect_bd_net -net ps_wizard_0_mmi_dc_axi_noc0_clk  [get_bd_pins ps_wizard_0/mmi_dc_axi_noc0_clk] \
  [get_bd_pins Master_NoC/aclk10]
  connect_bd_net -net ps_wizard_0_pl0_ref_clk  [get_bd_pins ps_wizard_0/pl0_ref_clk] \
  [get_bd_pins clk_wizard_0/clk_in1]
  connect_bd_net -net ps_wizard_0_pl0_resetn  [get_bd_pins ps_wizard_0/pl0_resetn] \
  [get_bd_pins clk_wizard_0/resetn] \
  [get_bd_pins proc_sys_reset_o4/ext_reset_in] \
  [get_bd_pins proc_sys_reset_1/ext_reset_in]
  connect_bd_net -net ps_wizard_0_pl2_ref_clk  [get_bd_pins ps_wizard_0/pl2_ref_clk] \
  [get_bd_pins ps_wizard_0/emio_gem_tsu_clk_from_pl]
  connect_bd_net -net ps_wizard_0_pmc_axi_noc0_clk  [get_bd_pins ps_wizard_0/pmc_axi_noc0_clk] \
  [get_bd_pins Master_NoC/aclk9]
  connect_bd_net -net util_ds_buf_0_IBUF_OUT  [get_bd_pins util_ds_buf_0/IBUF_OUT] \
  [get_bd_pins NoC_C0_C1/sys_clk0] \
  [get_bd_pins NoC_C0_C1/sys_clk1]

  # Create address segments
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_10gbe_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_10gbe_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_10gbe_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_10gbe_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_dc_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_dc_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_dc_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_dc_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_gpu_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_gpu_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_pcie_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_pcie_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_pcie_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_pcie_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_usb_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_usb_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_usb_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_usb_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_asu] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_0] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_1] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_1] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_1] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_1] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_1] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_1] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_2] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_2] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_2] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_2] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_2] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_2] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_3] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_3] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_3] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_3] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_3] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_3] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_4] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_4] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_4] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_4] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_4] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_4] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_5] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_5] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_5] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_5] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_5] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_5] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_6] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_6] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_6] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_6] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_6] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_6] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_7] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_7] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_7] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_7] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_7] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0xB0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexa78_7] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_1] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_2] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_3] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_4] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_5] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_6] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_7] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_8] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_9] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_1] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_1] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_1] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_1] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dma_pmc_1] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dpc] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dpc] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dpc] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dpc] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_dpc] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_lpd_dma_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_lpd_dma_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_lpd_dma_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_lpd_dma_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_lpd_dma_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_pmc_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force
  assign_bd_address -offset 0x000800000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_pmc_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2] -force
  assign_bd_address -offset 0x050000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_pmc_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2] -force
  assign_bd_address -offset 0x060000000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_pmc_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2] -force
  assign_bd_address -offset 0x020000000000 -range 0x000100000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_pmc_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
  assign_bd_address -offset 0x00000000 -range 0x80000000 -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_ppu_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_LEGACYx2] -force

  # Exclude Address Segments
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_10gbe_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_dc_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_gpu_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_gpu_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_gpu_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_pcie_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/mmi_0_mmi_usb_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_asu] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_asu] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_asu] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_asu] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_1] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_1] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_1] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_1] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_2] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_2] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_2] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_2] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_3] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_3] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_3] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_3] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_4] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_4] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_4] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_4] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_5] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_5] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_5] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_5] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_6] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_6] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_6] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_6] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_7] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_7] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_7] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_7] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_8] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_8] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_8] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_8] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_9] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_9] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_9] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_cortexr52_9] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_ppu_0] [get_bd_addr_segs NoC_C0_C1/DDR_MC_PORTS/DDR_CH0_MEDx2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_ppu_0] [get_bd_addr_segs NoC_C2_C3/DDR_MC_PORTS/DDR_CH1x2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_ppu_0] [get_bd_addr_segs NoC_C4/DDR_MC_PORTS/DDR_CH2]
  exclude_bd_addr_seg -target_address_space [get_bd_addr_spaces ps_wizard_0/ps11_0_ppu_0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0]


  # Restore current instance
  current_bd_instance $oldCurInst

  # Create PFM attributes
  set_property PFM_NAME {xilinx.com:vek385:vek385_thin:1.0} [get_files [current_bd_design].bd]
  set_property PFM.AXI_PORT { S00_AXI {sptag LPDDR01 auto false} S01_AXI {sptag LPDDR01 auto false} S02_AXI {sptag LPDDR01 auto false} S03_AXI {sptag LPDDR01 auto false} S04_AXI {sptag LPDDR01 auto false} S05_AXI {sptag LPDDR01 auto false} S06_AXI {sptag LPDDR01 auto false} S07_AXI {sptag LPDDR01 auto false} S08_AXI {sptag LPDDR01 auto false} S09_AXI {sptag LPDDR01 auto false} S10_AXI {sptag LPDDR01 auto false} S11_AXI {sptag LPDDR01 auto false} S12_AXI {sptag LPDDR01 auto false} S13_AXI {sptag LPDDR01 auto false} S14_AXI {sptag LPDDR01 auto false} S15_AXI {sptag LPDDR01 auto false} S16_AXI {sptag LPDDR01 auto false} S17_AXI {sptag LPDDR01 auto false} S18_AXI {sptag LPDDR01 auto false} S19_AXI {sptag LPDDR01 auto false} } [get_bd_cells /NoC_C0_C1]
  set_property PFM.AXI_PORT { S00_AXI {sptag LPDDR23} S01_AXI {sptag LPDDR23} S02_AXI {sptag LPDDR23} S03_AXI {sptag LPDDR23} S04_AXI {sptag LPDDR23} S05_AXI {sptag LPDDR23} S06_AXI {sptag LPDDR23} S07_AXI {sptag LPDDR23} S08_AXI {sptag LPDDR23} S09_AXI {sptag LPDDR23} S10_AXI {sptag LPDDR23} S11_AXI {sptag LPDDR23} S12_AXI {sptag LPDDR23} S13_AXI {sptag LPDDR23} S14_AXI {sptag LPDDR23} S15_AXI {sptag LPDDR23} S16_AXI {sptag LPDDR23} S17_AXI {sptag LPDDR23} S18_AXI {sptag LPDDR23} S19_AXI {sptag LPDDR23} S20_AXI {sptag LPDDR23} S21_AXI {sptag LPDDR23} S22_AXI {sptag LPDDR23} S23_AXI {sptag LPDDR23} S24_AXI {sptag LPDDR23} S25_AXI {sptag LPDDR23} S26_AXI {sptag LPDDR23} S27_AXI {sptag LPDDR23} S28_AXI {sptag LPDDR23} S29_AXI {sptag LPDDR23} S30_AXI {sptag LPDDR23} S31_AXI {sptag LPDDR23} S32_AXI {sptag LPDDR23} S33_AXI {sptag LPDDR23} S34_AXI {sptag LPDDR23} S35_AXI {sptag LPDDR23} S36_AXI {sptag LPDDR23} S37_AXI {sptag LPDDR23} S38_AXI {sptag LPDDR23} S39_AXI {sptag LPDDR23} S40_AXI {sptag LPDDR23} S41_AXI {sptag LPDDR23} S42_AXI {sptag LPDDR23} S43_AXI {sptag LPDDR23} S44_AXI {sptag LPDDR23} S45_AXI {sptag LPDDR23} S46_AXI {sptag LPDDR23} S47_AXI {sptag LPDDR23} S48_AXI {sptag LPDDR23} S49_AXI {sptag LPDDR23} S50_AXI {sptag LPDDR23} S51_AXI {sptag LPDDR23} S52_AXI {sptag LPDDR23} S53_AXI {sptag LPDDR23} S54_AXI {sptag LPDDR23} S55_AXI {sptag LPDDR23} } [get_bd_cells /NoC_C2_C3]
  set_property PFM.AXI_PORT { S00_AXI {sptag LPDDR4} S01_AXI {sptag LPDDR4} S02_AXI {sptag LPDDR4} S03_AXI {sptag LPDDR4} S04_AXI {sptag LPDDR4} S05_AXI {sptag LPDDR4} S06_AXI {sptag LPDDR4} S07_AXI {sptag LPDDR4} S08_AXI {sptag LPDDR4} S09_AXI {sptag LPDDR4} S10_AXI {sptag LPDDR4} S11_AXI {sptag LPDDR4} S12_AXI {sptag LPDDR4} S13_AXI {sptag LPDDR4} S14_AXI {sptag LPDDR4} S15_AXI {sptag LPDDR4} S16_AXI {sptag LPDDR4} S17_AXI {sptag LPDDR4} S18_AXI {sptag LPDDR4} S19_AXI {sptag LPDDR4} S20_AXI {sptag LPDDR4} S21_AXI {sptag LPDDR4} S22_AXI {sptag LPDDR4} S23_AXI {sptag LPDDR4} S24_AXI {sptag LPDDR4} S25_AXI {sptag LPDDR4} S26_AXI {sptag LPDDR4} S27_AXI {sptag LPDDR4} S28_AXI {sptag LPDDR4} S29_AXI {sptag LPDDR4} S30_AXI {sptag LPDDR4} S31_AXI {sptag LPDDR4} S32_AXI {sptag LPDDR4} S33_AXI {sptag LPDDR4} S34_AXI {sptag LPDDR4} S35_AXI {sptag LPDDR4} S36_AXI {sptag LPDDR4} S37_AXI {sptag LPDDR4} S38_AXI {sptag LPDDR4} S39_AXI {sptag LPDDR4} S40_AXI {sptag LPDDR4} S41_AXI {sptag LPDDR4} S42_AXI {sptag LPDDR4} S43_AXI {sptag LPDDR4} S44_AXI {sptag LPDDR4} S45_AXI {sptag LPDDR4} S46_AXI {sptag LPDDR4} S47_AXI {sptag LPDDR4} S48_AXI {sptag LPDDR4} S49_AXI {sptag LPDDR4} S50_AXI {sptag LPDDR4} S51_AXI {sptag LPDDR4} S52_AXI {sptag LPDDR4} S53_AXI {sptag LPDDR4} S54_AXI {sptag LPDDR4} S55_AXI {sptag LPDDR4} } [get_bd_cells /NoC_C4]
  set_property PFM.AXI_PORT { S00_AXI {sptag LPDDR auto preferred} S01_AXI {sptag LPDDR auto preferred} S02_AXI {sptag LPDDR auto preferred} S03_AXI {sptag LPDDR auto preferred} S04_AXI {sptag LPDDR auto preferred} S05_AXI {sptag LPDDR auto preferred} S06_AXI {sptag LPDDR auto preferred} S07_AXI {sptag LPDDR auto preferred} S08_AXI {sptag LPDDR auto preferred} S09_AXI {sptag LPDDR auto preferred} S10_AXI {sptag LPDDR auto preferred} S11_AXI {sptag LPDDR auto preferred} S12_AXI {sptag LPDDR auto preferred} S13_AXI {sptag LPDDR auto preferred} S14_AXI {sptag LPDDR auto preferred} S15_AXI {sptag LPDDR auto preferred} S16_AXI {sptag LPDDR auto preferred} S17_AXI {sptag LPDDR auto preferred} S18_AXI {sptag LPDDR auto preferred} S19_AXI {sptag LPDDR auto preferred} S20_AXI {sptag LPDDR auto preferred} S21_AXI {sptag LPDDR auto preferred} S22_AXI {sptag LPDDR auto preferred} S23_AXI {sptag LPDDR auto preferred} S24_AXI {sptag LPDDR auto preferred} S25_AXI {sptag LPDDR auto preferred} S26_AXI {sptag LPDDR auto preferred} S27_AXI {sptag LPDDR auto preferred} S28_AXI {sptag LPDDR auto preferred} S29_AXI {sptag LPDDR auto preferred} S30_AXI {sptag LPDDR auto preferred} S31_AXI {sptag LPDDR auto preferred} S32_AXI {sptag LPDDR auto preferred} S33_AXI {sptag LPDDR auto preferred} S34_AXI {sptag LPDDR auto preferred} S35_AXI {sptag LPDDR auto preferred} S36_AXI {sptag LPDDR auto preferred} S37_AXI {sptag LPDDR auto preferred} S38_AXI {sptag LPDDR auto preferred} S39_AXI {sptag LPDDR auto preferred} S40_AXI {sptag LPDDR auto preferred} S41_AXI {sptag LPDDR auto preferred} S42_AXI {sptag LPDDR auto preferred} S43_AXI {sptag LPDDR auto preferred} S44_AXI {sptag LPDDR auto preferred} S45_AXI {sptag LPDDR auto preferred} S46_AXI {sptag LPDDR auto preferred} S47_AXI {sptag LPDDR auto preferred} S48_AXI {sptag LPDDR auto preferred} S49_AXI {sptag LPDDR auto preferred} S50_AXI {sptag LPDDR auto preferred} S51_AXI {sptag LPDDR auto preferred} S52_AXI {sptag LPDDR auto preferred} S53_AXI {sptag LPDDR auto preferred} S54_AXI {sptag LPDDR auto preferred} S55_AXI {sptag LPDDR auto preferred} } [get_bd_cells /aggr_noc]
  set_property PFM.AXI_PORT { S00_AXI {sptag AIE auto false} S01_AXI {sptag AIE auto false} S02_AXI {sptag AIE auto false} S03_AXI {sptag AIE auto false} S04_AXI {sptag AIE auto false} S05_AXI {sptag AIE auto false} S06_AXI {sptag AIE auto false} S07_AXI {sptag AIE auto false} S08_AXI {sptag AIE auto false} S09_AXI {sptag AIE auto false} S10_AXI {sptag AIE auto false} S11_AXI {sptag AIE auto false} S12_AXI {sptag AIE auto false} S13_AXI {sptag AIE auto false} S14_AXI {sptag AIE auto false} S15_AXI {sptag AIE auto false} S16_AXI {sptag AIE auto false} S17_AXI {sptag AIE auto false} S18_AXI {sptag AIE auto false} S19_AXI {sptag AIE auto false} S20_AXI {sptag AIE auto false} S21_AXI {sptag AIE auto false} } [get_bd_cells /ConfigNoc]
  set_property PFM.AXI_PORT { M01_AXI {} M02_AXI {} M03_AXI {} M04_AXI {} M05_AXI {} M06_AXI {} M07_AXI {} M08_AXI {} M09_AXI {} M10_AXI {} M11_AXI {} M12_AXI {} M13_AXI {} M14_AXI {} M15_AXI {} } [get_bd_cells /ctrl_smc]
  set_property PFM.IRQ {intr {id 0 range 31}} [get_bd_cells /axi_intc_0]
  set_property PFM.CLOCK {clk_out1_o1 {id "0" is_default false proc_sys_reset "/proc_sys_reset_o4" status "fixed_non_ref"} clk_out1_o2 {id "1" is_default true proc_sys_reset "/proc_sys_reset_o4" status "fixed_non_ref"} clk_out1_o3 {id "2" is_default false proc_sys_reset "/proc_sys_reset_o4" status "fixed_non_ref"} clk_out1_o4 {id "3" is_default false proc_sys_reset "/proc_sys_reset_o4" status "fixed_non_ref"} clk_out2 {id "4" is_default false proc_sys_reset "/proc_sys_reset_1" status "fixed"}} [get_bd_cells /clk_wizard_0]


  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


