#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/
#*****************************************************************************************
create_project project_1 ./project_1 -part xcvc1902-vsva2197-2MP-e-S
set_property board_part xilinx.com:vck190:part0:3.4 [current_project]
create_bd_design "versal_comn_platform" -mode batch
instantiate_example_design -template xilinx.com:design:versal_comn_platform:2.0 -design versal_comn_platform -options { Design_type.VALUE Extensible Include_AIE.VALUE true}

update_compile_order -fileset sources_1
set_property synth_checkpoint_mode None [get_files  ./project_1/project_1.srcs/sources_1/bd/versal_comn_platform/versal_comn_platform.bd]
generate_target all [get_files  ./project_1/project_1.srcs/sources_1/bd/versal_comn_platform/versal_comn_platform.bd]

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

set pre_synth ""
if { $argc > 1} {
  set pre_synth [lindex $argv 2]
}

#Implementation Run
  launch_runs impl_1 -to_step write_device_image -job  20
  wait_on_run impl_1

  open_run impl_1

  
  
  # Generating dynamic reload extensible XSA as default hardware platform
  write_hw_platform -hw -force -include_bit -file vck190_hw.xsa

