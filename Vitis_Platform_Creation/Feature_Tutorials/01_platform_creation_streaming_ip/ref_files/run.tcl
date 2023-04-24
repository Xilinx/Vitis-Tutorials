#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*

proc numberOfCPUs {} {
    # Windows puts it in an environment variable
    global tcl_platform env
    if {$tcl_platform(platform) eq "windows"} {
        return $env(NUMBER_OF_PROCESSORS)
    }

    # Check for sysctl (OSX, BSD)
    set sysctl [auto_execok "sysctl"]
    if {[llength $sysctl]} {
        if {![catch {exec {*}$sysctl -n "hw.ncpu"} cores]} {
            return $cores
        }
    }

    # Assume Linux, which has /proc/cpuinfo, but be careful
    if {![catch {open "/proc/cpuinfo"} f]} {
        set cores [regexp -all -line {^processor\s} [read $f]]
        close $f
        if {$cores > 0} {
            return $cores
        }
    }

    # No idea what the actual number of cores is; exhausted all our options
    # Fall back to returning 1; there must be at least that because we're running on it!
    return 1
}
if {[file exists local_user_repo]} {
               file delete -force local_user_repo
               }

#unset ::env(HTTP_PROXY) 
file mkdir local_user_repo

# The environment variable XILINX_XHUB_USERAREA_ROOT_DIR is responsible for redirecting downloaded board files to local_user_repo
set ::env(XILINX_XHUB_USERAREA_ROOT_DIR) local_user_repo

xhub::get_xstores




xhub::refresh_catalog [xhub::get_xstores Vivado_example_project]
get_property LOCAL_ROOT_DIR [xhub::get_xstores Vivado_example_project]
set_param ced.repoPaths [get_property LOCAL_ROOT_DIR [xhub::get_xstores Vivado_example_project]]
xhub::install [xhub::get_xitems *ext_platform_part*]



# ------ Create Vivado Project ------
create_project project_1 ./project_1 -part xcvc1902-vsva2197-2MP-e-S 
create_bd_design "ext_platform_part" -mode batch
instantiate_example_design -template xilinx.com:design:ext_platform_part:1.0 -design ext_platform_part -options { Include_AIE.VALUE true Include_DDR.VALUE true}
# Other options are default
# - Three clocks
# - 32 interrupt




set_property -dict [list CONFIG.PS_PMC_CONFIG { BOOT_MODE Custom  CLOCK_MODE Custom  DDR_MEMORY_MODE Custom  IO_CONFIG_MODE Custom  PMC_CRP_PL0_REF_CTRL_FREQMHZ 99.999992  PMC_QSPI_FBCLK {{ENABLE 1} {IO {PMC_MIO 6}}}  PMC_QSPI_PERIPHERAL_ENABLE 0  PMC_REF_CLK_FREQMHZ 33.3333  PMC_SD0 {{CD_ENABLE 0} {CD_IO {PMC_MIO 24}} {POW_ENABLE 0} {POW_IO {PMC_MIO 17}} {RESET_ENABLE 0} {RESET_IO {PMC_MIO 17}} {WP_ENABLE 0} {WP_IO {PMC_MIO 25}}}  PMC_SD0_PERIPHERAL {{CLK_100_SDR_OTAP_DLY 0x00} {CLK_200_SDR_OTAP_DLY 0x00} {CLK_50_DDR_ITAP_DLY 0x00} {CLK_50_DDR_OTAP_DLY 0x00} {CLK_50_SDR_ITAP_DLY 0x00} {CLK_50_SDR_OTAP_DLY 0x00} {ENABLE 0} {IO {PMC_MIO 13 .. 25}}}  PMC_SD0_SLOT_TYPE {SD 2.0}  PMC_SD1 {{CD_ENABLE 1} {CD_IO {PMC_MIO 28}} {POW_ENABLE 1} {POW_IO {PMC_MIO 51}} {RESET_ENABLE 0} {RESET_IO {PMC_MIO 12}} {WP_ENABLE 0} {WP_IO {PMC_MIO 1}}}  PMC_SD1_PERIPHERAL {{CLK_100_SDR_OTAP_DLY 0x3} {CLK_200_SDR_OTAP_DLY 0x2} {CLK_50_DDR_ITAP_DLY 0x36} {CLK_50_DDR_OTAP_DLY 0x3} {CLK_50_SDR_ITAP_DLY 0x2C} {CLK_50_SDR_OTAP_DLY 0x4} {ENABLE 1} {IO {PMC_MIO 26 .. 36}}}  PMC_SD1_SLOT_TYPE {SD 3.0}  PMC_SMAP_PERIPHERAL {{ENABLE 0} {IO {32 Bit}}}  PMC_USE_PMC_NOC_AXI0 1  PS_CAN1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 40 .. 41}}}  PS_ENET0_MDIO {{ENABLE 1} {IO {PS_MIO 24 .. 25}}}  PS_ENET0_PERIPHERAL {{ENABLE 1} {IO {PS_MIO 0 .. 11}}}  PS_GEN_IPI0_ENABLE 1  PS_GEN_IPI0_MASTER A72  PS_GEN_IPI1_ENABLE 1  PS_GEN_IPI1_MASTER A72  PS_GEN_IPI2_ENABLE 1  PS_GEN_IPI2_MASTER A72  PS_GEN_IPI3_ENABLE 1  PS_GEN_IPI3_MASTER A72  PS_GEN_IPI4_ENABLE 1  PS_GEN_IPI4_MASTER A72  PS_GEN_IPI5_ENABLE 1  PS_GEN_IPI5_MASTER A72  PS_GEN_IPI6_ENABLE 1  PS_GEN_IPI6_MASTER A72  PS_I2C0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 46 .. 47}}}  PS_I2C1_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 44 .. 45}}}  PS_IRQ_USAGE {{CH0 1} {CH1 0} {CH10 0} {CH11 0} {CH12 0} {CH13 0} {CH14 0} {CH15 0} {CH2 0} {CH3 0} {CH4 0} {CH5 0} {CH6 0} {CH7 0} {CH8 0} {CH9 0}}  PS_NUM_FABRIC_RESETS 1  PS_PL_CONNECTIVITY_MODE Custom  PS_UART0_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 42 .. 43}}}  PS_USB3_PERIPHERAL {{ENABLE 1} {IO {PMC_MIO 13 .. 25}}}  PS_USE_FPD_AXI_NOC0 1  PS_USE_FPD_AXI_NOC1 1  PS_USE_FPD_CCI_NOC 1  PS_USE_M_AXI_FPD 1  PS_USE_NOC_LPD_AXI0 1  PS_USE_PMCPL_CLK0 1  PS_USE_PMCPL_CLK1 0  PS_USE_PMCPL_CLK2 0  PS_USE_PMCPL_CLK3 0  SMON_ALARMS Set_Alarms_On  SMON_ENABLE_TEMP_AVERAGING 0  SMON_TEMP_AVERAGING_SAMPLES 0 } CONFIG.PS_PMC_CONFIG_APPLIED {1} CONFIG.IO_CONFIG_MODE {Custom}] [get_bd_cells CIPS_0]

set_property -dict [list CONFIG.MC0_CONFIG_NUM {config17} CONFIG.MC1_CONFIG_NUM {config17} CONFIG.MC2_CONFIG_NUM {config17} CONFIG.MC3_CONFIG_NUM {config17} CONFIG.MC_INPUT_FREQUENCY0 {200.000} CONFIG.MC_INPUTCLK0_PERIOD {5000} CONFIG.MC_MEMORY_DEVICETYPE {UDIMMs} CONFIG.MC_MEMORY_SPEEDGRADE {DDR4-3200AA(22-22-22)} CONFIG.MC_TRCD {13750} CONFIG.MC_TRP {13750} CONFIG.MC_DDR4_2T {Disable} CONFIG.MC_CASLATENCY {22} CONFIG.MC_TRC {45750} CONFIG.MC_TRPMIN {13750} CONFIG.MC_CONFIG_NUM {config17} CONFIG.MC_F1_TRCD {13750} CONFIG.MC_F1_TRCDMIN {13750} CONFIG.MC_F1_LPDDR4_MR1 {0x000} CONFIG.MC_F1_LPDDR4_MR2 {0x000} CONFIG.MC_F1_LPDDR4_MR3 {0x000} CONFIG.MC_F1_LPDDR4_MR13 {0x000}] [get_bd_cells noc_ddr4]


set_property -dict [list \
  CONFIG.MC0_FLIPPED_PINOUT {true} \
  CONFIG.MC1_FLIPPED_PINOUT {true} \
  CONFIG.MC_INPUTCLK0_PERIOD {4992} \
  CONFIG.MC_MEMORY_TIMEPERIOD0 {512} \
  CONFIG.NUM_MC {2} \
] [get_bd_cells noc_lpddr4]

apply_bd_automation -rule xilinx.com:bd_rule:board -config { Manual_Source {Auto}}  [get_bd_intf_pins noc_lpddr4/CH0_LPDDR4_1]
apply_bd_automation -rule xilinx.com:bd_rule:board -config { Manual_Source {Auto}}  [get_bd_intf_pins noc_lpddr4/CH1_LPDDR4_1]
apply_bd_automation -rule xilinx.com:bd_rule:board -config { Manual_Source {Auto}}  [get_bd_intf_pins noc_lpddr4/sys_clk1]

set_property -dict [list CONFIG.FREQ_HZ 200321000] [get_bd_intf_ports sys_clk0_1]
set_property -dict [list CONFIG.FREQ_HZ {200000000}] [get_bd_intf_ports sys_clk0_0]

create_bd_cell -type ip -vlnv xilinx.com:ip:dds_compiler:6.0 dds_compiler_0

set_property -dict [list CONFIG.DDS_Clock_Rate {300} CONFIG.Spurious_Free_Dynamic_Range {90} CONFIG.Output_Selection {Sine} CONFIG.Output_Frequency1 {10} CONFIG.Frequency_Resolution {0.4} CONFIG.Noise_Shaping {Auto} CONFIG.Phase_Width {30} CONFIG.Output_Width {15} CONFIG.Latency {8} CONFIG.PINC1 {1010001111010111000010100}] [get_bd_cells dds_compiler_0]
set_property -dict [list CONFIG.Has_Phase_Out {false} CONFIG.M_DATA_Has_TUSER {Not_Required}] [get_bd_cells dds_compiler_0]
connect_bd_net [get_bd_pins clk_wizard_0/clk_out1] [get_bd_pins dds_compiler_0/aclk]

set_property PFM.AXIS_PORT {M_AXIS_DATA {type "M_AXIS" sptag "AXIS" is_range "false"}} [get_bd_cells /dds_compiler_0]



validate_bd_design

add_files -fileset constrs_1 -norecurse ../../ddr.xdc
import_files -fileset constrs_1 ../../ddr.xdc

remove_files ./project_1/project_1.srcs/sources_1/imports/hdl/ext_platform_part_wrapper.v
file delete -force ./project_1/project_1.srcs/sources_1/imports/hdl/ext_platform_part_wrapper.v
update_compile_order -fileset sources_1
make_wrapper -files [get_files ./project_1/project_1.srcs/sources_1/bd/ext_platform_part/ext_platform_part.bd] -top
add_files -norecurse ./project_1/project_1.gen/sources_1/bd/ext_platform_part/hdl/ext_platform_part_wrapper.v

generate_target all [get_files ext_platform_part.bd]
update_compile_order -fileset sources_1
write_hw_platform -force ./custom_hardware_platform_hw.xsa
write_hw_platform -hw_emu -force ./custom_hardware_platform_hwemu.xsa


