#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

puts "\[vivado_npm.tcl\] Starting Non-Project Mode"



# Set part and board information
set part "xcve2802-vsvh1760-2MP-e-S"
set_part "xcve2802-vsvh1760-2MP-e-S"
# set board_part "xilinx.com:vek280:part0:1.2"

set_property TARGET_LANGUAGE "Verilog" [current_project]
set_property BOARD_PART "xilinx.com:vek280:part0:1.2" [current_project]
set_property DEFAULT_LIB work [current_project]

# Read the block design
set BD "../post_link_iteration/project_1/project_1.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd"

# Generate all targets for the block design
generate_target all [get_files $BD]
read_bd $BD

# Make wrapper for the block design
make_wrapper -files [get_files $BD] -top

# Read the generated wrapper
read_verilog ../post_link_iteration/project_1/project_1.gen/sources_1/bd/vitis_design_vma/hdl/vitis_design_vma_wrapper.v

# Read additional Verilog files
read_verilog ../post_link_iteration/axis_sink.v
read_verilog ../post_link_iteration/top.v

# Create and configure ILA IP
read_ip ../post_link_iteration/project_1/project_1.srcs/sources_1/ip/axis_ila_0/axis_ila_0.xci

# Generate the ILA IP
# generate_target {instantiation_template} [get_files ./project_1/project_1.srcs/sources_1/ip/axis_ila_0/axis_ila_0.xci]

# Read constraint files (XDC)
# Note: The project uses automatically generated constraints from IP cores and block design
# No user-defined constraint files are present in the project sources
# The following constraint files are automatically included when the IPs are generated:
# - CIPS constraints from pspmc_v1_4/constraints/usr_constraints.xdc
# - NoC constraints from various bd_*_*.xdc files  
# - ILA constraints from axis_ila_v1_3/constraints/axis_ila.xdc and axis_ila_impl.xdc
# - Various IP-specific constraints from generated IP cores
# All constraint files are automatically read during synthesis and implementation

# Synthesize design
synth_design -top top -part $part

# Place and route
opt_design
place_design
route_design
write_device_image -force -file ./top.pdi

# Generate bitstream
# write_bitstream -force top.bit

# Export fixed XSA
# set_property platform.extensible false [current_design]
write_hw_platform -fixed -include_bit -force -file ./top.xsa

puts "\[vivado_npm.tcl\] Non-Project Mode Complete"
