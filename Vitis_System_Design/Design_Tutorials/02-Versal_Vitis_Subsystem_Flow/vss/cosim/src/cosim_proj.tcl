#
# Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

## ===================================================================================
## Create a Vivado project for cosimulation
## This script takes in two argument:
## VSS_NAME
## DEVICE_NAME (e.g. xcvc1902-vsva2197-2MP-e-S)
## ===================================================================================
namespace eval _tcl {
  proc get_script_folder {} {
    set script_path [file normalize [info script]]
    set script_folder [file dirname $script_path]
    return $script_folder
  }
}

variable script_folder
set script_folder [_tcl::get_script_folder]

#set_param board.repoPaths ../board_repo/boards/Xilinx/vck190

## WORKAROUND - Hardcode platform and device name as tcl arguments is not passed properly to Vitis
set VSS_NAME [lindex $argv 0]
puts "Creating VSS cosim project for : \"$VSS_NAME\""
set DEVICE_NAME [lindex $argv 1]
puts "Using : \"$DEVICE_NAME\""
set BUILD_DIR build

create_project -f ${VSS_NAME} ${BUILD_DIR}/${VSS_NAME}_cosim -part $DEVICE_NAME
#update_compile_order -fileset sources_1

## ===================================================================================
## Import VSS, create a RTL wrapper for the Vitis Region and add to files
## ===================================================================================
vitis::import_vss ${script_folder}/../../build/${VSS_NAME}/${VSS_NAME}.vss
open_bd_design ${BUILD_DIR}/${VSS_NAME}_cosim/${VSS_NAME}.srcs/sources_1/bd/${VSS_NAME}/${VSS_NAME}.bd
validate_bd_design
save_bd_design
current_bd_design [get_bd_designs ${VSS_NAME}]
make_wrapper -files [get_files ${BUILD_DIR}/${VSS_NAME}_cosim/${VSS_NAME}.srcs/sources_1/bd/${VSS_NAME}/${VSS_NAME}.bd] -top
add_files -norecurse ${BUILD_DIR}/${VSS_NAME}_cosim/${VSS_NAME}.gen/sources_1/bd/${VSS_NAME}/hdl/${VSS_NAME}_wrapper.v
update_compile_order -fileset sources_1

## ===================================================================================
## Add testbench
## ===================================================================================
#break
add_files -fileset sim_1 -norecurse ${script_folder}/driver_axis.sv
add_files -fileset sim_1 -norecurse ${script_folder}/monitor_axis.sv
add_files -fileset sim_1 -norecurse ${script_folder}/my_diff.v
add_files -fileset sim_1 -norecurse ${script_folder}/my_sub.v
add_files -fileset sim_1 -norecurse ${script_folder}/testbench.sv
set_property top tb_top [get_filesets sim_1]
update_compile_order -fileset sim_1

## ===================================================================================
## Setup simulation
## ===================================================================================
#set_property generate_scripts_only 1 [current_fileset -simset]
set_property -name {xsim.simulate.runtime} -value {0us} -objects [get_filesets sim_1]
set_property -name {xsim.simulate.xsim.more_options} -value {-testplusarg VERBOSITY_FATAL_TO_WARN} -objects [get_filesets sim_1]

## ===================================================================================
## Launch simulation
## ===================================================================================
launch_simulation
open_wave_config ${script_folder}/tb_top_behav.wcfg
open_wave_config ${script_folder}/datamovers.wcfg
open_wave_config ${script_folder}/datamovers2.wcfg
open_wave_config ${script_folder}/subtractor.wcfg
open_wave_config ${script_folder}/fir_filters.wcfg
log_wave -r *
run 5us
