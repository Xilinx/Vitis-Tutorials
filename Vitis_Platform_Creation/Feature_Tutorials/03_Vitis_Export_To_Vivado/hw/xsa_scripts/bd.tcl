#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************
################################################################
# This is a generated script based on design: vitis_design
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

set_property board_part xilinx.com:vck190:part0:* [current_project]


# Instatiate CED design with configurable properties
instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design vitis_design -options { Clock_Options.VALUE {clk_out1 625.000 0 true clk_out2 104.167 1 false clk_out3 208.33 2 false clk_out4 416.67 3 false} Include_AIE.VALUE true Include_BDC.VALUE false IRQS.VALUE 63}

# Update clock ids in CED to make it equivalent to base_platform
set_property PFM.CLOCK {clk_out1_o1 {id "6" is_default "false" proc_sys_reset "/proc_sys_reset_0" status "fixed_non_ref" freq_hz "625000000"} clk_out1_o2 {id "2" is_default "true" proc_sys_reset "/proc_sys_reset_1" status "fixed_non_ref" freq_hz "312500000"} clk_out1_o3 {id "0" is_default "false" proc_sys_reset "/proc_sys_reset_2" status "fixed_non_ref" freq_hz "156250000"} clk_out1_o4 {id "3" is_default "false" proc_sys_reset "/proc_sys_reset_3" status "fixed_non_ref" freq_hz "78125000"} clk_out2 {id "1" is_default "false" proc_sys_reset "/proc_sys_reset_4" status "fixed" freq_hz "104166666"} clk_out3 {id "4" is_default "false" proc_sys_reset "/proc_sys_reset_5" status "fixed" freq_hz "208333333"} clk_out4 {id "5" is_default "false" proc_sys_reset "/proc_sys_reset_6" status "fixed" freq_hz "416666666"}} [get_bd_cells /clk_wizard_0]

validate_bd_design
save_bd_design

#write_bd_tcl -f ../xsa_scripts/reference_bd.tcl

update_compile_order -fileset sim_1
update_compile_order -fileset sources_1



