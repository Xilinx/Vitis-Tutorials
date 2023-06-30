# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
create_project -force flat flat -part xcvc1902-vsva2197-2MP-e-S
set_property board_part xilinx.com:vck190:part0:3.1 [current_project]
create_bd_design "flat"
make_wrapper -files [get_files ./flat/flat.srcs/sources_1/bd/flat/flat.bd] -top
add_files -norecurse ./flat/flat.gen/sources_1/bd/flat/hdl/flat_wrapper.v

source ./flat_pre_bdc.tcl
source ./create_bdc.tcl

set_property -dict [list CONFIG.LOCK_PROPAGATE {true}] [get_bd_cells ExtensibleRegion]

generate_target all [get_files flat.bd]

set_property platform.extensible true [current_project]
set_property platform.platform_state "pre_synth" [current_project]
set_property platform.design_intent.embedded "true" [current_project]
set_property platform.design_intent.server_managed "false" [current_project]
set_property platform.design_intent.external_host "false" [current_project]
set_property platform.design_intent.datacenter "false" [current_project]

update_compile_order -fileset sources_1 
save_bd_design
    
write_hw_platform -force flat_wa.xsa
validate_hw_platform -verbose flat_wa.xsa



