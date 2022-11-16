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
