#*****************************************************************************************
# Copyright 2021 Xilinx Inc.
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
#*****************************************************************************************

source ../../VitisRegion.tcl
source ../../design_1.tcl
#source ../../dfx_pfm_setup.tcl

add_files -norecurse [make_wrapper -files [get_files -norecurse design_1.bd] -top]
set_property top design_1_wrapper [current_fileset] 


puts "Generate Block Design"
generate_target all [get_files [current_bd_design].bd]
# update_compile_order -fileset sources_1
# update_compile_order -fileset sim_1

puts "Add Pblock Constraints"
add_files -fileset constrs_1 -norecurse ../../pblock.xdc
import_files -fileset constrs_1 ../../pblock.xdc


# -------------------
# Platform Properties
# -------------------
set_property platform.default_output_type "sd_card" [current_project]
set_property platform.design_intent.embedded "true" [current_project]
set_property platform.design_intent.server_managed "false" [current_project]
set_property platform.design_intent.external_host "false" [current_project]
set_property platform.num_compute_units 32 [current_project]
set_property platform.design_intent.datacenter "false" [current_project]

# --------------
# hw-emu XSA
# --------------
puts "Generate hw-emu XSA"
set_property PREFERRED_SIM_MODEL "tlm" [current_project]
generate_switch_network_for_noc
#update_compile_order -fileset sim_1

# These steps are automatically done in GUI
set_property top design_1_wrapper_sim_wrapper [get_filesets sim_1]
import_files -fileset sim_1 -norecurse ./myproj/project_1.srcs/sources_1/common/hdl/design_1_wrapper_sim_wrapper.v
update_compile_order -fileset sim_1


launch_simulation -scripts_only
launch_simulation -step compile
launch_simulation -step elaborate

set_property platform.platform_state "pre_synth" [current_project] 
# Default Value
write_hw_platform -hw_emu -force -file vck190_custom_dfx_hw_emu.xsa


# ----------------------
# HW XSA for DFX
# ----------------------
puts "Generate HW XSA"
# Partial Reconfiguration - Project Level
set_property PR_FLOW 1 [current_project]

# Doesn't seem to work

# create_partition_def -name config_1 -module VitisRegion_inst_0
# create_reconfig_module -name VitisRegion_inst_0 -partition_def [get_partition_defs config_1 ]  -define_from VitisRegion_inst_0

# BDC level
# set_property CONFIG.ENABLE_DFX 1 [get_bd_cells /VitisRegion] 
# set_property -dict [list CONFIG.LOCK_PROPAGATE {true} CONFIG.ENABLE_DFX {true}] [get_bd_cells VitisRegion]
set_property -dict [list CONFIG.LOCK_PROPAGATE {true}] [get_bd_cells VitisRegion]

# PR Config
create_pr_configuration -name config_1 -partitions [list design_1_i/VitisRegion:VitisRegion_inst_0 ]
set_property PR_CONFIGURATION config_1 [get_runs impl_1]


# Platform Settings
set_property platform.extensible true [current_project]
# set_property PFM_NAME "xilinx.com:xd:VCK190_Custom_DFX:1.0" [get_files VitisRegion.bd] # Platform Setup already in bd.tcl





# PR in Platform Settings

set_property platform.uses_pr true [current_project]
set_property platform.dr_inst_path {design_1_i/VitisRegion} [current_project]
set_property platform.emu.dr_bd_inst_path {design_1_wrapper_sim_wrapper/design_1_wrapper_i/design_1_i/VitisRegion} [current_project]

# Run
launch_runs synth_1 -jobs 20
wait_on_run synth_1
launch_runs impl_1 -to_step write_device_image -jobs 10
wait_on_run impl_1
open_run impl_1


# Impl XSA
set_property platform.platform_state "impl" [current_project]
write_hw_platform -force -fixed -static -file vck190_custom_dfx_static.xsa
write_hw_platform -force -rp design_1_i/VitisRegion vck190_custom_dfx_rp.xsa
