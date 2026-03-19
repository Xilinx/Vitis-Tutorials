# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#*****************************************************************************************
enable_beta_device *  

xhub::refresh_catalog [xhub::get_xstores xilinx_board_store]
xhub::install [xhub::get_xitems xilinx.com:xilinx_board_store:vck190:*] -quiet


# global variables
set ::platform "vck190"
set ::silicon "e-S"

# local variables
set project_dir "edt_versal"
set scripts_dir "scripts"
set constrs_dir "constrs"

variable design_name
set design_name edt_versal


set part "xcvc1902-vsva2197-2MP-${::silicon}"
puts "INFO: Target part selected: '$part'"

# set up project
# set_param board.repoPaths ./board_files
 create_project $design_name $project_dir -part $part -force

set board_lat [ get_board_parts -latest_file_version  {*vck190:*} ]
set_property board_part $board_lat [current_project]


# set up bd design
create_bd_design $design_name
source edt_versal_bd.tcl


# add hdl sources to project
make_wrapper -files [get_files ./$project_dir/edt_versal.srcs/sources_1/bd/edt_versal/edt_versal.bd] -top
add_files -norecurse ./$project_dir/edt_versal.gen/sources_1/bd/edt_versal/hdl/edt_versal_wrapper.v
set_property top edt_versal_wrapper [current_fileset]

add_files -fileset constrs_1 -norecurse $constrs_dir/gpio.xdc
update_compile_order -fileset sources_1


validate_bd_design
save_bd_design
update_compile_order -fileset sources_1
regenerate_bd_layout
save_bd_design

open_bd_design {./$project_dir/edt_versal.srcs/sources_1/bd/edt_versal/edt_versal.bd}
set_property synth_checkpoint_mode None [get_files  ./$project_dir/edt_versal.srcs/sources_1/bd/edt_versal/edt_versal.bd]
generate_target all [get_files  ./$project_dir/edt_versal.srcs/sources_1/bd/edt_versal/edt_versal.bd]

export_ip_user_files -of_objects [get_files ./$project_dir/edt_versal.srcs/sources_1/bd/edt_versal/edt_versal.bd] -no_script -sync -force -quiet
export_simulation -lib_map_path [list {modelsim=./$project_dir/edt_versal.cache/compile_simlib/modelsim} {questa=./$project_dir/edt_versal.cache/compile_simlib/questa} {xcelium=./$project_dir/edt_versal.cache/compile_simlib/xcelium} {vcs=./$project_dir/edt_versal.cache/compile_simlib/vcs} {riviera=./$project_dir/edt_versal.cache/compile_simlib/riviera}] -of_objects [get_files ./$project_dir/edt_versal.srcs/sources_1/bd/edt_versal/edt_versal.bd] -directory ./$project_dir/edt_versal.ip_user_files/sim_scripts -ip_user_files_dir ./$project_dir/edt_versal.ip_user_files -ipstatic_source_dir ./$project_dir/edt_versal.ip_user_files/ipstatic -use_ip_compiled_libs -force -quiet

launch_runs synth_1 -jobs 4
launch_runs impl_1 -to_step write_device_image -jobs 4
wait_on_run impl_1
write_hw_platform -fixed -include_bit -force -file ./$project_dir/edt_versal_wrapper.xsa

