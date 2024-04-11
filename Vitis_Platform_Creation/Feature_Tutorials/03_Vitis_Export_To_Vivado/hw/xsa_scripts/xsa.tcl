#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************
file mkdir build 
cd build
source ../xsa_scripts/project.tcl
source ../xsa_scripts/ext_bdc.tcl
source ../xsa_scripts/vck190_bd.tcl
source ../xsa_scripts/pfm_decls.tcl

#Generating Wrapper
make_wrapper -files [get_files ./my_project/my_project.srcs/sources_1/bd/vck190_prj/vck190_prj.bd] -top
add_files -norecurse ./my_project/my_project.gen/sources_1/bd/vck190_prj/hdl/vck190_prj_wrapper.v

#Generating Target
generate_target all [get_files ./my_project/my_project.srcs/sources_1/bd/vck190_prj/vck190_prj.bd]
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1
set_property top vck190_prj_wrapper [current_fileset] 

# Ensure that your top of synthesis module is also set as top for simulation
set_property top vck190_prj_wrapper [get_filesets sim_1]

# Generate simulation top for your entire design which would include
# aggregated NOC in the form of xlnoc.bd
generate_switch_network_for_noc
update_compile_order -fileset sim_1

# Set the auto-generated <rtl_top>_sim_wrapper as the sim top
set_property top vck190_prj_wrapper_sim_wrapper [get_filesets sim_1]
import_files -fileset sim_1 -norecurse ./my_project/my_project.srcs/sources_1/common/hdl/vck190_prj_wrapper_sim_wrapper.v
update_compile_order -fileset sim_1

set_property platform.platform_state "pre_synth" [current_project]
set_param bd.generateHybridSystemC true

puts "Vivado Project is created successfully."



