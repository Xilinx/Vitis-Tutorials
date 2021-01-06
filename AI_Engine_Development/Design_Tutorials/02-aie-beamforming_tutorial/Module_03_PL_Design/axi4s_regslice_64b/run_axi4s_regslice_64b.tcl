#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

set IP_NAME "axi4s_regslice_64b"
set IP_REPO "build"
set project "project_axi4s_regslice_64b"
create_project $project ./$project -part [lindex $argv 1] -force

create_bd_design "design_axi4s_regslice_64b"
#set argv [lindex $argv 1]
#set argc 1
source ./bd_axi4s_regslice_64b.tcl

make_wrapper -files [get_files ./$project/$project.srcs/sources_1/bd/design_axi4s_regslice_64b/design_axi4s_regslice_64b.bd] -top
add_files -norecurse ./$project/$project.srcs/sources_1/bd/design_axi4s_regslice_64b/hdl/design_axi4s_regslice_64b_wrapper.v

import_files

set_property top design_axi4s_regslice_64b_wrapper [current_fileset]
update_compile_order -fileset sources_1

save_bd_design
validate_bd_design

reset_target all [get_files  ./$project/$project.srcs/sources_1/bd/${design_name}/${design_name}.bd]
generate_target all [get_files ./$project/$project.srcs/sources_1/bd/${design_name}/${design_name}.bd]

export_ip_user_files -of_objects [get_files ./$project/$project.srcs/sources_1/bd/${design_name}/${design_name}.bd] -no_script -sync -force -quiet
set bypass [lindex $argv 0]
if {$bypass!="NO_SIM"} {
  create_ip_run [get_files -of_objects [get_fileset sources_1] ./$project/$project.srcs/sources_1/bd/${design_name}/${design_name}.bd]
  launch_runs -jobs 32 {design_axi4s_regslice_64b_axis_register_slice_0_0_synth_1}
  foreach my_run {design_axi4s_regslice_64b_axis_register_slice_0_0_synth_1} {wait_on_run $my_run}
  export_simulation -of_objects [get_files ./$project/$project.srcs/sources_1/bd/${design_name}/${design_name}.bd] -directory ./$project/$project.ip_user_files/sim_scripts -ip_user_files_dir ./$project/$project.ip_user_files -ipstatic_source_dir ./$project/$project.ip_user_files/ipstatic -lib_map_path [list {modelsim=./$project/$project.cache/compile_simlib/modelsim} {questa=./$project/$project.cache/compile_simlib/questa} {ies=./$project/$project.cache/compile_simlib/ies} {xcelium=./$project/$project.cache/compile_simlib/xcelium} {vcs=./$project/$project.cache/compile_simlib/vcs} {riviera=./$project/$project.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet
}
unset bypass

ipx::package_project -root_dir [pwd]/../${IP_REPO}/axi4s_regslice_64b -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current false -force
ipx::unload_core [pwd]/../${IP_REPO}/axi4s_regslice_64b/component.xml
ipx::edit_ip_in_project -upgrade true -name tmp_edit_project -directory ../${IP_REPO}/axi4s_regslice_64b [pwd]/../${IP_REPO}/axi4s_regslice_64b/component.xml
update_compile_order -fileset sources_1
ipx::associate_bus_interfaces -busif M_AXIS -clock aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif S_AXIS -clock aclk [ipx::current_core]
set_property core_revision 2 [ipx::current_core]
ipx::update_source_project_archive -component [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::save_core [ipx::current_core]
ipx::move_temp_component_back -component [ipx::current_core]
close_project -delete
set_property  ip_repo_paths  [pwd]/../${IP_REPO}/axi4s_regslice_64b [current_project]
update_ip_catalog

#package_xo -kernel_name axi4s_regslice_64b -kernel_xml ./kernel_axi4s_regslice_64b.xml -ip_directory [pwd]/../${IP_REPO}/axi4s_regslice_64b -xo_path [pwd]/../${IP_REPO}/axi4s_regslice_64b/axi4s_regslice_64b.xo -force
#package_xo -kernel_name axi4s_regslice_64b -ctrl_protocol user_managed -ip_directory [pwd]/../${IP_REPO}/axi4s_regslice_64b -xo_path [pwd]/../${IP_REPO}/axi4s_regslice_64b/axi4s_regslice_64b.xo -force -output_kernel_xml [pwd]/kernel_axi4s_regslice_64b_auto.xml
package_xo -kernel_name ${IP_NAME} -ctrl_protocol user_managed -ip_directory [pwd]/../${IP_REPO}/${IP_NAME} -xo_path [pwd]/../${IP_REPO}/${IP_NAME}.xo -force -output_kernel_xml [pwd]/kernel_${IP_NAME}_auto.xml
