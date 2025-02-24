set vma [lindex $argv 0]
set target [lindex $argv 1]
open_project my_project/my_project.xpr
update_compile_order -fileset sources_1
vitis::import_archive ${vma}
update_compile_order -fileset sources_1
open_bd_design {my_project/my_project.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd}
make_wrapper -files [get_files my_project/my_project.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd] -top
add_files -norecurse  my_project/my_project.srcs/sources_1/bd/vitis_design_vma/hdl/vitis_design_vma_wrapper.v
update_compile_order -fileset sources_1
set_property top vitis_design_vma_wrapper [current_fileset]
update_compile_order -fileset sources_1
reset_target all [get_files  my_project/my_project.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd]
export_ip_user_files -of_objects  [get_files my_project/my_project.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd] -sync -no_script -force -quiet
generate_target all [get_files  my_project/my_project.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd]
set_property top vitis_design_vma_wrapper [get_filesets sim_1]
update_compile_order -fileset sim_1
#generate_switch_network_for_noc
#update_compile_order -fileset sim_1
#set_property top vitis_design_vma_wrapper_sim_wrapper [get_filesets sim_1]
#import_files -fileset sim_1 -norecurse my_project/my_project.srcs/sources_1/common/hdl/vitis_design_vma_wrapper_sim_wrapper.v
#reset_simulation -simset sim_1 -mode behavioral
#set_property -name {xsim.simulate.runtime} -value {all} -objects [get_filesets sim_1]
#launch_simulation -scripts_only
#cd ./my_project/my_project.sim/sim_1/behav/xsim/
#exec ./compile.sh
#exec ./elaborate.sh
#cd ../../../../../

if {$target eq "hw"} {
	reset_run synth_1
	launch_runs synth_1 -jobs 1
	wait_on_run synth_1
	launch_runs impl_1 -to_step write_device_image
	wait_on_run impl_1
	open_run impl_1
	write_hw_platform -fixed -include_bit hw_fixed.xsa -force
} else {
}
