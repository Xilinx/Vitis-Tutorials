# © Copyright 2020 Xilinx, Inc.
# 
# Licensed under the Apache License, Version 2.0 (the 'License');
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an 'AS IS' BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


set project "project_tb_dlbf_data"
create_project $project ./$project -part xcvc1902-vsva2197-1LP-e-S-es1

set repoPath { ../../ip_repo }
set_property ip_repo_paths $repoPath [current_project]
update_ip_catalog

set design_name "design_1"
create_bd_design ${design_name}
source ./bd_tb_dlbf_data.tcl

make_wrapper -files [get_files ./$project/$project.srcs/sources_1/bd/${design_name}/${design_name}.bd] -top
add_files -norecurse ./$project/$project.srcs/sources_1/bd/${design_name}/hdl/${design_name}_wrapper.v

import_files

set_property SOURCE_SET sources_1 [get_filesets sim_1]
add_files -fileset sim_1 -norecurse {./tb_master.sv}

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

regenerate_bd_layout

launch_simulation
run all
