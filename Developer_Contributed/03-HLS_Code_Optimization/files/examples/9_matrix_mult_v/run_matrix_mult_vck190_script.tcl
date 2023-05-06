# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 9) 32x32 REAL MATRIX MULTIPLICATION DEMO"
puts "************************************************************************"
puts "\n"


# STANDALONE MATRIX MULTIPLIER HLS PROJECT
###########################################################################

open_project hls_mmult_prj
set_top standalone_mmult
add_files src/mmult_accel.cpp
add_files -tb src/mmult_test.cpp
open_solution "solution1"   -flow_target vivado
set_part {xcvc1902-vsva2197-2MP-e-S}
create_clock -period 3.0 -name default
#config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
csim_design -clean
csynth_design

open_solution "solution2"    -flow_target vivado
set_part {xcvc1902-vsva2197-2MP-e-S}
create_clock -period 3.0 -name default
#config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_pipeline -II 1 "mmult_hw/L2"
csynth_design

open_solution "solution3"     -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
#config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_pipeline -II 1 "mmult_hw/L2"
set_directive_array_partition -type block -factor 16 -dim 2 "mmult_hw" a
set_directive_array_partition -type block -factor 16 -dim 1 "mmult_hw" b
csynth_design
#cosim_design

close_project


# WRAPPED AXI4-STREAM MATRIX MULTIPLIER HLS PROJECT
###########################################################################

open_project hls_wrapped_mmult_prj
set_top HLS_accel
add_files src/mmult_accel.cpp -cflags "-DDB_DEBUG"
add_files -tb src/mmult_test.cpp -cflags "-DDB_DEBUG"
open_solution "solution0"    -flow_target vivado
set_part {xcvc1902-vsva2197-2MP-e-S}
create_clock -period 3.0 -name default
set_clock_uncertainty 0.5
set_directive_inline "mmult_hw"
set_directive_pipeline -II 1 "mmult_hw/L2"
set_directive_array_partition -type block -factor 16 -dim 2 "mmult_hw" a
set_directive_array_partition -type block -factor 16 -dim 1 "mmult_hw" b
csim_design -clean
#-setup
csynth_design
#export_design -evaluate verilog -format ip_catalog
close_project
