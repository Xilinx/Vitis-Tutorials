#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

set implement 0

create_project vck190 vck190 -part xcvc1902-vsva2197-2MP-e-S

set_property board_part xilinx.com:vck190:part0:3.4 [current_project]
create_bd_design "ext_platform" -mode batch

instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design ext_platform -options { Include_AIE.VALUE true Include_BDC.Value false }

generate_target all [get_files  vck190/vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd]

if {$implement == 1} {
	launch_runs impl_1 -to_step write_bitstream
	write_hw_platform -include_bit -fixed -force -file vck190/design_1_wrapper.xsa
	exit
} else {
	write_hw_platform -fixed -force -file vck190/design_1_wrapper.xsa
	exit
}
