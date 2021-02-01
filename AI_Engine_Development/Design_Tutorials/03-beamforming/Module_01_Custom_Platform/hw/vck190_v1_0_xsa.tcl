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
## This script takes in one argument: device_name (e.g. xcvc1902-vsva2197-1LP-e-S-es1)
## ===================================================================================
create_project vck190_v1_0 build/vck190_v1_0_vivado -part [lindex $argv 0] 

set argv [lindex $argv 0]
set argc 1

## ===================================================================================
## Create Block Design
## ===================================================================================
source src/dr.bd.tcl

## ===================================================================================
## Set Platform Properties (e.g. PFM.*)
## ===================================================================================
source pfm_decls.tcl

## ===================================================================================
## Add constraints to DDR4 
## ===================================================================================
add_files -fileset constrs_1 -norecurse src/ddr4_dmc_3200_triplet_1.xdc

## ===================================================================================
## Create a wrapper for block design. Set the block design as top-level wrapper.  
## ===================================================================================
make_wrapper -files [get_files build/vck190_v1_0_vivado/vck190_v1_0.srcs/sources_1/bd/vck190_v1_0/vck190_v1_0.bd] -top
add_files -norecurse build/vck190_v1_0_vivado/vck190_v1_0.srcs/sources_1/bd/vck190_v1_0/hdl/vck190_v1_0_wrapper.v
update_compile_order -fileset sources_1

## ===================================================================================
## Apply platform specific post-link tcl commands. 
## Note: This design does not have have any post-link tcl commands. 
## ===================================================================================
set_property dsa.post_sys_link_tcl_hook src/post_link_pfm_config.tcl [current_project]

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
set_property SELECTED_SIM_MODEL tlm [get_bd_cells /ps_cips]
set_property SELECTED_SIM_MODEL tlm [get_bd_cells /ps_noc]

## ===================================================================================
## Set output type to hw_export
## ===================================================================================
set_property platform.default_output_type "hw_export" [current_project]

## ===================================================================================
## Wrap up Vivado Platform Project
## ===================================================================================
update_compile_order
assign_bd_address
regenerate_bd_layout
validate_bd_design
set_param dsa.includeMEContent true
import_files

## ===================================================================================
## Generate files necessary to support block design through design flow
## ===================================================================================
generate_target all [get_files build/vck190_v1_0_vivado/vck190_v1_0.srcs/sources_1/bd/vck190_v1_0/vck190_v1_0.bd]

## ===================================================================================
## Write the XSA for current design for use as a hardware platform
## ===================================================================================
write_hw_platform -force build/vck190_v1_0.xsa
