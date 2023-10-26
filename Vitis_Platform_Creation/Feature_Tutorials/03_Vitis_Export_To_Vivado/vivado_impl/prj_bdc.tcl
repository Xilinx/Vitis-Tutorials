# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
create_project -force prj_bdc prj_bdc -part xcvc1902-vsva2197-2MP-e-S
set_property board_part xilinx.com:vck190:part0:3.1 [current_project]
create_bd_design "prj_bdc"
make_wrapper -files [get_files ./prj_bdc/prj_bdc.srcs/sources_1/bd/prj_bdc/prj_bdc.bd] -top
add_files -norecurse ./prj_bdc/prj_bdc.gen/sources_1/bd/prj_bdc/hdl/prj_bdc_wrapper.v

source ./vivado_impl/pre_create_bdc.tcl
source ./vivado_impl/create_bdc.tcl

set_property -dict [list CONFIG.LOCK_PROPAGATE {true}] [get_bd_cells ExtensibleRegion]

generate_target all [get_files prj_bdc.bd]

set_property platform.extensible true [current_project]
set_property platform.platform_state "pre_synth" [current_project]
set_property platform.design_intent.embedded "true" [current_project]
set_property platform.design_intent.server_managed "false" [current_project]
set_property platform.design_intent.external_host "false" [current_project]
set_property platform.design_intent.datacenter "false" [current_project]

update_compile_order -fileset sources_1 
save_bd_design
    
write_hw_platform -force ./vivado_impl/prj_bdc_ext_hw.xsa
validate_hw_platform -verbose ./vivado_impl/prj_bdc_ext_hw.xsa



