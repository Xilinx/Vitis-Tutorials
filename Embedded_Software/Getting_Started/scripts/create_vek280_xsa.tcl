#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

set implement 0

create_project vek280 vek280 -part xcve2802-vsvh1760-2MP-e-S

set_property board_part xilinx.com:vek280:part0:1.2 [current_project]
create_bd_design "versal_comn_platform" -mode batch

instantiate_example_design -template xilinx.com:design:versal_comn_platform:2.0 -design versal_comn_platform -options { Include_AIE.VALUE true }

generate_target all [get_files  vek280/vek280.srcs/sources_1/bd/versal_comn_platform/versal_comn_platform.bd]

if {$implement == 1} {
	launch_runs impl_1 -to_step write_bitstream
	write_hw_platform -include_bit -fixed -force -file vek280/design_1_wrapper.xsa
	exit
} else {
	write_hw_platform -fixed -force -file vek280/design_1_wrapper.xsa
	exit
}
