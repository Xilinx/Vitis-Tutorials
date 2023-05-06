# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# Author: Daniele Bagni, Xilinx Inc

if 0 {
puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 3) REFERENCE NAIVE MEDIAN FILTER 3x3: SOLUTION0"
puts "************************************************************************"
puts "\n"
open_project hls_naive_median3x3_prj
set_top ref_median
add_files ./src/median.cpp -cflags "-DKMED3"
add_files -tb ./src/test_data
add_files -tb ./src/main_test.cpp -cflags "-DKMED3 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb ./src/ap_bmp.cpp    -cflags "-DKMED3 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution0" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
csim_design -clean
csynth_design
close_project
}


puts "\n"
puts "************************************************************************"
puts "* 3) MEDIAN FILTER 3x3 WITH LINE BUFFERS: SOLUTION1_3x3"
puts "************************************************************************"
puts "\n"
open_project hls_median_prj
set_top top_median
add_files ./src/median.cpp  -cflags "-DKMED3"
add_files -tb ./src/test_data
add_files -tb ./src/main_test.cpp -cflags "-DKMED3 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb ./src/ap_bmp.cpp    -cflags "-DKMED3 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1_3x3" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_pipeline -II 1 "top_median/L2"
#csim_design -clean
csynth_design
close_project


puts "\n"
puts "************************************************************************"
puts "* 3) MEDIAN FILTER 5x5 WITH LINE BUFFERS: SOLUTION1_5x5"
puts "************************************************************************"
puts "\n"
open_project hls_median_prj
set_top top_median
add_files ./src/median.cpp  -cflags "-DKMED5"
add_files -tb ./src/test_data
add_files -tb ./src/main_test.cpp -cflags "-DKMED5 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb ./src/ap_bmp.cpp    -cflags "-DKMED5 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1_5x5" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_pipeline -II 1 "top_median/L2"
csim_design -clean
csynth_design
close_project


puts "\n"
puts "************************************************************************"
puts "* 3) MEDIAN FILTER 7x7 WITH LINE BUFFERS: SOLUTION1_7x7"
puts "************************************************************************"
puts "\n"
open_project hls_median_prj
set_top top_median
add_files ./src/median.cpp  -cflags "-DKMED7"
add_files -tb ./src/test_data
add_files -tb ./src/main_test.cpp -cflags "-DKMED7 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb ./src/ap_bmp.cpp    -cflags "-DKMED7 -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1_7x7" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_pipeline -II 1 "top_median/L2"
csim_design -clean
csynth_design
close_project


puts "\n"
puts "************************************************************************"
puts "* 4) empty solution just to remove CFLAGS from the GUI"
puts "************************************************************************"
puts "\n"
open_project hls_median_prj
set_top top_median
add_files ./src/median.cpp
add_files -tb ./src/test_data
add_files -tb ./src/main_test.cpp
add_files -tb ./src/ap_bmp.cpp
open_solution "solution_empty" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
#set_directive_pipeline -II 1 "top_median/L2"
#csim_design -clean
#csynth_design
close_project
