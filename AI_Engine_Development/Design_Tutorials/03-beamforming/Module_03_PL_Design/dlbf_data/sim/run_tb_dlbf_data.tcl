

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


set project "project_tb_dlbf_data"
create_project $project ./$project -part xcvc1902-vsva2197-2MP-e-S

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
