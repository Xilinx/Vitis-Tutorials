#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
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


set PLATFORM_NAME [lindex $argv 0]
set VER "1.0"
puts "Creating HW Platform project for : \"$PLATFORM_NAME\""
set DEVICE_NAME [lindex $argv 1]
puts "Using : \"$DEVICE_NAME\""
set BOARD_LABEL [lindex $argv 3]
set BOARD_VER [lindex $argv 4]
set BUILD_DIR build
set XSA_PLATFORM [lindex $argv 5]
set BD_NAME versal_comn_platform

create_project -f ${PLATFORM_NAME} ${BUILD_DIR}/${PLATFORM_NAME}_vivado -part $DEVICE_NAME

# set board part
set_property BOARD_PART xilinx.com:${BOARD_LABEL}:part0:${BOARD_VER} [current_project]

set_property coreContainer.enable 1 [current_project]
create_bd_design ${BD_NAME} -mode batch

## ===================================================================================
## Load Board example design
## ===================================================================================

instantiate_example_design -template xilinx.com:design:versal_comn_platform:2.0 -design versal_comn_platform -options { Design_type.VALUE Extensible Include_AIE.VALUE true}

set_property preferred_sim_model "tlm" [current_project]

update_compile_order -fileset sources_1

## ===================================================================================
## Generate Block Design
## ===================================================================================

generate_target all [get_files  ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${BD_NAME}/${BD_NAME}.bd]

## ===================================================================================
## Set output type to hw_export
## ===================================================================================
set_property platform.default_output_type           "sd_card" [current_project]
# Help by explicitly categorizing intended platform
set_property platform.design_intent.server_managed  "false"   [current_project]
set_property platform.design_intent.external_host   "false"   [current_project]
set_property platform.design_intent.embedded        "true"    [current_project]
set_property platform.design_intent.datacenter      "false"   [current_project]
set_property platform.extensible                    "true"    [current_project]

## ===================================================================================
## Add hardware emulation support
## ===================================================================================
set_property PLATFORM.LINK_XP_SWITCHES_DEFAULT [list param:hw_emu.enableProfiling=false] [current_project]
#set_property SELECTED_SIM_MODEL tlm [get_bd_cells /Cips]
#set_property SELECTED_SIM_MODEL tlm [get_bd_cells /CipsNoc]
#set_property SELECTED_SIM_MODEL tlm [get_bd_cells /MemoryNoc]
#set_property SELECTED_SIM_MODEL tlm [get_bd_cells /ExtensibleRegion/ControlNoc]
#set_property SELECTED_SIM_MODEL tlm [get_bd_cells /ExtensibleRegion/DDR]


## ===================================================================================
## Export Platform XSA
## ===================================================================================
set_property platform.name $PLATFORM_NAME [current_project]
set_property platform.board_id {board} [current_project]
set_property platform.uses_pr {false} [current_project]


set_property pfm_name xilinx:${PLATFORM_NAME}:${PLATFORM_NAME}_custom:0.0 [get_files  ${BUILD_DIR}/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${BD_NAME}/${BD_NAME}.bd]
write_hw_platform -force -file ${BUILD_DIR}/${XSA_PLATFORM}
