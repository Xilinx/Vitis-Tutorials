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

## ===================================================================================
## Create Platform Vivado Project
## This script takes in two argument:
## PLATFORM_NAME
## DEVICE_NAME (e.g. xcvc1902-vsva2197-1LP-e-S-es1)
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

set_param board.repoPaths ../board_repo/boards/Xilinx/vck190

## WORKAROUND - Hardcode platform and device name as tcl arguments is not passed properly to Vitis
#set DEVICE_NAME [lindex $argv 0]
set PLATFORM_NAME [lindex $argv 0]
set PLATFORM_TYPE vck190_custom
set VER "1.0"
puts "Creating HW Platform project for : \"$PLATFORM_NAME\""
set DEVICE_NAME [lindex $argv 1]
#set DEVICE_NAME xcvc1902-vsva2197-1LP-e-S-es1
puts "Using : \"$DEVICE_NAME\""
set BOARD_LABEL [lindex $argv 3]
set BOARD_VER [lindex $argv 4]

create_project -f ${PLATFORM_NAME} build/${PLATFORM_NAME}_vivado -part $DEVICE_NAME

# set board part 
set_property BOARD_PART xilinx.com:${BOARD_LABEL}:part0:${BOARD_VER} [current_project]

## ===================================================================================
## Create Block Design
## ===================================================================================
# pass arguments to tcl
#set argv [list $PLATFORM_NAME $DEVICE_NAME]
#set argc 2
source src/dr.bd.tcl

## ===================================================================================
## Set Platform Properties (e.g. PFM.*)
## ===================================================================================
# this is currently done in dr.bd.tcl #source pfm_decls.tcl
source pfm_decls.tcl

## ===================================================================================
## Add constraints to DDR4
## ===================================================================================
#add_files -fileset constrs_1 -norecurse src/ddr4_dmc_3200_triplet_1.xdc

## ===================================================================================
## Create a wrapper for block design. Set the block design as top-level wrapper.
## ===================================================================================
make_wrapper -files [get_files build/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/${PLATFORM_NAME}.bd] -top
add_files -norecurse build/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/hdl/${PLATFORM_NAME}_wrapper.v
update_compile_order -fileset sources_1

## ===================================================================================
## Apply platform specific post-link tcl commands.
## Note: This design does not have have any post-link tcl commands.
## ===================================================================================
# Deprecated syntax, still neede ?? #set_property dsa.post_sys_link_tcl_hook src/post_link_pfm_config.tcl [current_project]

## ===================================================================================
## Apply timing constraints
## ===================================================================================
add_files -fileset constrs_1 -norecurse src/qor_scripts/timing.xdc

## ===================================================================================
## Apply scripts to close timing
## ===================================================================================
import_files -fileset utils_1 -norecurse src/qor_scripts/pre_place.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/post_place.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/post_route.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/post_physopt.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/prohibitCascDspAcrossRbrk.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/prohibitCascBramAcrossRbrk.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/prohibitCascUramAcrossRbrk.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/waive_BLI_AIE_timing_violations_postplace.tcl
import_files -fileset utils_1 -norecurse src/qor_scripts/waive_BLI_AIE_timing_violations_preplace.tcl

set_property platform.run.steps.place_design.tcl.pre [get_files pre_place.tcl] [current_project]
set_property platform.run.steps.place_design.tcl.post [get_files post_place.tcl] [current_project]
set_property platform.run.steps.route_design.tcl.post [get_files post_route.tcl] [current_project]
set_property platform.run.steps.phys_opt_design.tcl.post [get_files post_physopt.tcl] [current_project]

## ===================================================================================
## Add hardware emulation support
## ===================================================================================
set_property PLATFORM.LINK_XP_SWITCHES_DEFAULT [list param:compiler.skipTimingCheckAndFrequencyScaling=true param:hw_emu.enableProfiling=false] [current_project]
set_property SELECTED_SIM_MODEL tlm [get_bd_cells /cips_0]
set_property SELECTED_SIM_MODEL tlm [get_bd_cells /axi_noc_0]

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
## Wrap up Vivado Platform Project
## ===================================================================================
update_compile_order
assign_bd_address
regenerate_bd_layout
validate_bd_design
# Deprecated syntax, still neede ?? #set_param dsa.includeMEContent true
import_files

## ===================================================================================
## Generate files necessary to support block design through design flow
## ===================================================================================
generate_target all [get_files build/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/${PLATFORM_NAME}.bd]

variable pre_synth
set pre_synth ""

if { $argc > 1} {
  set pre_synth [lindex $argv 2]
}
#Pre_synth Platform Flow
if {$pre_synth} {
  set_property platform.platform_state "pre_synth" [current_project]
  write_hw_platform -force build/${PLATFORM_NAME}.xsa
  validate_hw_platform build/${PLATFORM_NAME}.xsa
} else {
  set_property generate_synth_checkpoint true [get_files -norecurse *.bd]
  ## ===================================================================================
  ## Full Synthesis and implementation
  ## ===================================================================================
  launch_runs -jobs 8 synth_1
  wait_on_run synth_1
  puts "Synthesis done!"

  #launch_runs impl_1 -to_step write_bitstream
  launch_runs -jobs 8 impl_1 -to_step write_device_image
  wait_on_run impl_1
  puts "Implementation done!"

  # ===================================================================================
  # Write the XSA for current design for use as a hardware platform
  # ===================================================================================
  open_run impl_1
  write_hw_platform -unified -include_bit -force build/${PLATFORM_NAME}.xsa
  validate_hw_platform build/${PLATFORM_NAME}.xsa
}
