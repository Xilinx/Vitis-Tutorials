#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/
open_hw_platform hw.xsa
set_property platform.extensible false [current_project]


launch_runs impl_1 -to_step write_bitstream -jobs 32
wait_on_run impl_1
generate_target all [get_files [current_bd_design].bd]
write_hw_platform -fixed -include_bit -force -file  xsa_file_for_petalinux.xsa