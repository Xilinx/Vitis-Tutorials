#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT


set implement 1

create_project zcu102 ../zcu102 -part xczu9eg-ffvb1156-2-e

set_property board_part xilinx.com:zcu102:part0:3.4 [current_project]
create_bd_design "mpsoc_preset" -mode batch

instantiate_example_design -template xilinx.com:design:mpsoc_preset:1.0 -design mpsoc_preset -options { Preset.VALUE MPSoC_PL}

generate_target all [get_files  ../zcu102/zcu102.srcs/sources_1/bd/mpsoc_preset/mpsoc_preset.bd]

if {$implement == 1} {
	launch_runs impl_1 -to_step write_bitstream
	write_hw_platform -include_bit -fixed -force -file ../zcu102/mpsoc_preset_wrapper.xsa
} else {
	write_hw_platform -fixed -force -file ../zcu102/mpsoc_preset_wrapper.xsa
}