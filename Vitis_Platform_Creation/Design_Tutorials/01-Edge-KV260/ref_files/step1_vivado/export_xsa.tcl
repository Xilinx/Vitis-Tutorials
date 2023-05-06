#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

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

add_files -norecurse [make_wrapper -files [get_files -norecurse *.bd] -top]
update_compile_order -fileset sources_1

# For Emulation Support
set_property SELECTED_SIM_MODEL tlm [get_bd_cells /zynq_ultra_ps_e_0]


# Setup Platform Properties
set_property pfm_name {xilinx:kv260:kv260_hardware_Platform:0.0} [get_files -norecurse *.bd]
set_property platform.default_output_type "sd_card" [current_project]
set_property platform.design_intent.embedded "true" [current_project]
set_property platform.design_intent.server_managed "false" [current_project]
set_property platform.design_intent.external_host "false" [current_project]
set_property platform.design_intent.datacenter "false" [current_project]

# Generate Block Design
generate_target all [get_files [current_bd_design].bd]


# Write expandable XSA to vivado directory
# update_compile_order -fileset sources_1
# launch_runs impl_1 -to_step write_bitstream -jobs [numberOfCPUs]
# wait_on_run impl_1
# open_run impl_1
# write_hw_platform -include_bit -force -file ./kv260_hardware_platform.xsa
write_hw_platform -force -file ./kv260_hardware_platform.xsa


