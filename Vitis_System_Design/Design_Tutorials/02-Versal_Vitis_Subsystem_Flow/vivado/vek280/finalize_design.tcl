#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

## ===================================================================================
## Create Platform Vivado Project
## This script takes in two argument:
## PLATFORM_NAME
## DEVICE_NAME (e.g. xcvc1902-vsva2197-1LP-e-S)
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
set BOARD_NAME    [lindex $argv 0]
set PLATFORM_NAME [lindex $argv 1]
set PLATFORM_TYPE ${PLATFORM_NAME}_custom
set VER "1.0"
puts "Creating HW Platform project for : \"$PLATFORM_NAME\""
set DEVICE_NAME [lindex $argv 2]
puts "Using : \"$DEVICE_NAME\""
set BOARD_LABEL [lindex $argv 4]
set BOARD_VER [lindex $argv 5]
set BUILD_DIR build

open_project ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.xpr

# Open Vivado design and import VMA
update_compile_order -fileset sources_1
open_bd_design ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/${PLATFORM_NAME}.bd
vitis::import_archive ../vitis/build_hw/${PLATFORM_NAME}.vma
close_bd_design [get_bd_designs ${PLATFORM_NAME}]

# Open new BD with VMA included
open_bd_design ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}_vma/${PLATFORM_NAME}_vma.bd


# Generate new wrapper and set as top
make_wrapper -files [get_files ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}_vma/${PLATFORM_NAME}_vma.bd] -top
update_compile_order -fileset sources_1
add_files -norecurse ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.gen/sources_1/bd/${PLATFORM_NAME}_vma/hdl/${PLATFORM_NAME}_vma_wrapper.v
update_compile_order -fileset sources_1
set_property top ${PLATFORM_NAME}_vma_wrapper [current_fileset]
#set_property top ${PLATFORM_NAME}_wrapper [current_fileset]
update_compile_order -fileset sources_1


## ===================================================================================
## Full Synthesis and implementation
## ===================================================================================
launch_runs -jobs 8 synth_1
wait_on_run synth_1
puts "Synthesis done!"

launch_runs -jobs 8 impl_1 -to_step write_device_image
wait_on_run impl_1
puts "Implementation done!"

# Write and validate fixed XSA
open_run impl_1
write_hw_platform -fixed -force build/xsa_platform/${PLATFORM_NAME}_fixed.xsa
validate_hw_platform build/xsa_platform/${PLATFORM_NAME}_fixed.xsa
puts "Writing fixed XSA done!"

