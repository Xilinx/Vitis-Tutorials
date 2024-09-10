# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Author: Daniele Bagni, Xilinx Inc

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 7) ABOUT ATAN2()"
puts "************************************************************************"
puts "\n"

################## SOLUTION1_DOUBLE: 64-bit floating point ####################

open_project hls_atan2_prj
set_top top_atan2
add_files cordic_atan2.cpp -cflags "-DDB_DOUBLE_PRECISION"
add_files -tb cordic_test.cpp -cflags "-DDB_DOUBLE_PRECISION"
add_files -tb test_data
open_solution "solution1_double"   -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
csim_design -clean
csynth_design
#cosim_design
close_project

################## SOLUTION1_FLOAT: 32-bit floating point ####################

open_project hls_atan2_prj
set_top top_atan2
add_files cordic_atan2.cpp -cflags "-DDB_SINGLE_PRECISION"
add_files -tb cordic_test.cpp -cflags "-DDB_SINGLE_PRECISION"
add_files -tb test_data
open_solution "solution1_single"   -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
csim_design -clean
csynth_design
#cosim_design
close_project

################## SOLUTION1_CORDIC: cordic algorithm ####################

open_project hls_atan2_prj
set_top top_atan2
add_files cordic_atan2.cpp -cflags "-DDB_CORDIC"
add_files -tb cordic_test.cpp -cflags "-DDB_CORDIC"
add_files -tb test_data
open_solution "solution1_cordic"   -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
csim_design -clean
csynth_design
#cosim_design
#export_design -evaluate verilog -format ip_catalog
close_project


################## SOLUTION2_CORDIC: simplified cordic algorithm ####################

open_project hls_atan2_prj
set_top top_atan2
add_files cordic_atan2.cpp -cflags "-DDB_CORDIC -DBIT_ACCURATE"
add_files -tb cordic_test.cpp -cflags "-DDB_CORDIC -DBIT_ACCURATE"
add_files -tb test_data
open_solution "solution2_cordic_bitaccurate"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
csim_design -clean
csynth_design
#cosim_design
#export_design -evaluate verilog -format ip_catalog
close_project


################## SOLUTION_EMPTY: to remove CFLAGS from GUI ####################

open_project hls_atan2_prj
set_top top_atan2
add_files cordic_atan2.cpp
add_files -tb cordic_test.cpp
add_files -tb test_data
open_solution "solution_empty"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
close_project
