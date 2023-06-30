# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
open_project "./vivado_impl/flat/flat.xpr"
open_bd_design "./vivado_impl/flat/flat.srcs/sources_1/bd/flat/flat.bd"
::vitis::import_archive ./vitis_impl/flat_hw.vma
update_compile_order -fileset sources_1
reset_runs [get_runs -filter "IS_SYNTHESIS == 1"]
launch_runs impl_1 -to_step write_device_image -jobs 10
wait_on_runs [get_runs impl_1]
open_run [get_runs impl_1]

write_hw_platform -force -fixed flat_post.xsa



