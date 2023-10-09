
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

set kernelName "polar_clip"
set project "project_polar_clip"
set part xcvc1902-vsva2197-2MP-e-S 

# Create the IP project (retain this project if the RTL changes)
create_project $project ./$project -part $part -force

# Add all the HDL files needed
#add_files -norecurse [glob [pwd]/pl_kernels/hdl/verilog/*.v]
add_files -norecurse [glob [pwd]/pl_kernels/hdl/verilog/*.dat]
add_files -norecurse [glob [pwd]/pl_kernels/hdl/vhdl/*.vhd]

# Add all the constraint files if needed
#add_files -norecurse ./pl_kernels/hdl/*.xdc

import_files

# Set the top
set_property top $kernelName [get_fileset sources_1]
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# Package IP
ipx::package_project -root_dir [pwd]/ip_repo/${kernelName} -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current false -force
ipx::unload_core [pwd]/ip_repo/${kernelName}/component.xml
ipx::edit_ip_in_project -upgrade true -name tmp_edit_project -directory [pwd]/${kernelName}/ip_repo [pwd]/ip_repo/${kernelName}/component.xml
update_compile_order -fileset sources_1

# Make sure the clock is associated with all interfaces
ipx::associate_bus_interfaces -busif in_sample -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif out_sample -clock ap_clk [ipx::current_core]

# Remove the FREQ_HZ and PHASE properties since v++ will set these based upon clocking needed by the design
ipx::remove_bus_parameter FREQ_HZ [ipx::get_bus_interfaces in_sample -of_objects [ipx::current_core]]
ipx::remove_bus_parameter FREQ_HZ [ipx::get_bus_interfaces out_sample -of_objects [ipx::current_core]]
set_property driver_value 15 [ipx::get_ports in_sample_TKEEP -of_objects [ipx::current_core]]
set_property driver_value 15 [ipx::get_ports in_sample_TSTRB -of_objects [ipx::current_core]]
set_property driver_value 1 [ipx::get_ports out_sample_TVALID -of_objects [ipx::current_core]]
set_property driver_value 0 [ipx::get_ports in_sample_TDATA -of_objects [ipx::current_core]]
set_property sdx_kernel_type rtl [ipx::current_core]
set_property sdx_kernel true [ipx::current_core]

set_property core_revision 2 [ipx::current_core]
ipx::update_source_project_archive -component [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::check_integrity [ipx::current_core]
ipx::save_core [ipx::current_core]
ipx::check_integrity -quiet [ipx::current_core]
ipx::move_temp_component_back -component [ipx::current_core]
close_project -delete
set_property ip_repo_paths [pwd]/ip_repo/${kernelName} [current_project]

# Package the IP as an XO
package_xo -kernel_name $kernelName \
    -ctrl_protocol ap_ctrl_none \
    -ip_directory [pwd]/ip_repo/$kernelName \
    -xo_path [pwd]/ip_repo/${kernelName}.xo \
    -force -output_kernel_xml [pwd]/ip_repo/kernel_${kernelName}_auto.xml

