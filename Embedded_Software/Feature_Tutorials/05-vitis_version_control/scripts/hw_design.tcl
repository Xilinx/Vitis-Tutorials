#
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

set implement 0

create_project zcu104 ./zcu104 -part xczu7ev-ffvc1156-2-e 

set_property board_part xilinx.com:zcu104:part0:1.1 [current_project]
create_bd_design "mpsoc_preset" -mode batch

instantiate_example_design -template xilinx.com:design:mpsoc_preset:1.0 -design mpsoc_preset -options {Preset.VALUE MPSoC_PL}

generate_target all [get_files ./zcu104/zcu104.srcs/sources_1/bd/mpsoc_preset/mpsoc_preset.bd]

if {$implement ==1} {
        launch_runs impl_1 -to_step write_bitstream
        write_hw_platform -include_bit -fixed -force -file ./zcu104/design_1_wrapper.xsa
        exit
} else {
       write_hw_platform -fixed -force -file ./zcu104/design_1_wrapper.xsa
       exit
}
