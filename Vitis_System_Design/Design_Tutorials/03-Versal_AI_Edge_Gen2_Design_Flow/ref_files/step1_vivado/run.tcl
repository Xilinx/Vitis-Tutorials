# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#*****************************************************************************************
create_project project_1 ./project_1 -part xc2ve3858-ssva2112-2MP-e-S
set_property board_part xilinx.com:vek385:part0:1.1 [current_project]
create_bd_design "versal_gen2_platform" -mode batch
#instantiate_example_design -template xilinx.com:design:versal_gen2_platform:1.0 -design versal_gen2_platform -options { Board_selection.VALUE VEK385 Design_type.VALUE Extensible}
#dinstantiate_example_design -template xilinx.com:design:versal_comn_platform:2.0 -design versal_comn_platform -options { Board_selection.VALUE VEK385 Design_type.VALUE Extensible}
instantiate_example_design -template xilinx.com:design:versal_comn_platform:2.0 -design versal_gen2_platform -options { Design_type.VALUE Extensible Include_AIE.VALUE true}

generate_target all [get_files  ./project_1/project_1.srcs/sources_1/bd/versal_gen2_platform/versal_gen2_platform.bd]

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1
#set_property platform.name {name} [current_project]
#set_property pfm_name {xilinx:board:name:0.0} [get_files -all {./project_1/project_1.srcs/sources_1/bd/versal_gen2_platform/versal_gen2_platform.bd}]
#set_property platform.uses_pr {false} [current_project]
#set_property platform.board_id {board} [current_project]
launch_simulation -scripts_only
launch_simulation -step compile
launch_simulation -step elaborate

set_property platform.platform_state "pre_synth" [current_project]
set golden_ncr [glob -nocomplain $env(XILINX_VIVADO)/data/xhub/ced/XilinxCEDStore/ced/Xilinx/IPI/versal_common_platform/2.0/golden_ncr/vek385_reva_*.ncr]
set_property NOC_SOLUTION_FILE [file normalize $golden_ncr] [get_runs impl_1]
write_hw_platform -hw -force -file ./project_1/vek385_hw.xsa
write_hw_platform -hw_emu -force -file ./project_1/vek385_hw_emu.xsa
