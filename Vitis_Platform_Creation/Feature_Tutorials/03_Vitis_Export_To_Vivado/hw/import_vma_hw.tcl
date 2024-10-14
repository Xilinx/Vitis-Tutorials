##
##Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
##SPDX-License-Identifier: MIT
##
open_project "./hw/build/my_project/my_project.xpr"
update_compile_order -fileset sources_1
open_bd_design "./hw/build/my_project/my_project.srcs/sources_1/bd/vck190_prj/vck190_prj.bd"
open_bd_design "./hw/build/my_project/my_project.srcs/sources_1/bd/ext_bdc/ext_bdc.bd"
::vitis::import_archive ./vitis_impl/vitis_design_hw.vma
update_compile_order -fileset sources_1
open_bd_design "./hw/build/my_project/my_project.srcs/sources_1/bd/ext_bdc_vma/ext_bdc_vma.bd"
generate_target all [get_files  ./hw/build/my_project/my_project.srcs/sources_1/bd/ext_bdc_vma/ext_bdc_vma.bd]
export_ip_user_files -of_objects [get_files ./hw/build/my_project/my_project.srcs/sources_1/bd/ext_bdc_vma/ext_bdc_vma.bd] -no_script -sync -force -quiet
generate_target all [get_files  ./hw/build/my_project/my_project.srcs/sources_1/bd/vck190_prj/vck190_prj.bd]
export_ip_user_files -of_objects [get_files ./hw/build/my_project/my_project.srcs/sources_1/bd/vck190_prj/vck190_prj.bd] -no_script -sync -force -quiet
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1 
validate_bd_design
save_bd_design

reset_run synth_1
launch_runs synth_1 -jobs 4
wait_on_run synth_1
puts "Synthesis done!"

launch_runs impl_1 -jobs 8 
wait_on_run impl_1
puts "Implementation done!"

open_run impl_1
launch_runs impl_1 -to_step write_device_image -jobs 4
puts "PDI Generation is done!"

open_run impl_1
write_hw_platform -fixed -force ./hw/build/my_project/vck190_prj_fixed_hw.xsa
puts "Writing fixed XSA done!"

