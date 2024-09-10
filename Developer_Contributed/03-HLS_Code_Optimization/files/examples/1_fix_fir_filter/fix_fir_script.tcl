# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# Author: Daniele Bagni, Xilinx Inc

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 1) FIXED POINT FIR FILTER DEMO"
puts "************************************************************************"
puts "\n"

puts " "
puts "Solution8: Using ANSI-C Integers Datatypes instead of HLS Fractional Fixed Point"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol8_integers" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives8_integers.tcl"
csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project





puts " "
puts "Solution1: Baseline"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol1_baseline" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives1.tcl"
csim_design -clean
csynth_design
#cosim_design
#export_design -rtl vhdl -format ip_catalog
close_project

if 1 {

puts " "
puts "Solution2: as 1 + Pipelining the Loop"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol2_LoopPipeline" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives2.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project

puts " "
puts "Solution3: as 2 + Partitioning shift_reg Array"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol3_shiftregPartition" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives3.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project


puts " "
puts "Solution4: as 3 + Complete Loop Unrolling"
puts " "
open_project vhls_fix_fir_prj
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol4_loopUnroll" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives4.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project

puts " "
puts "Solution5: as 4 + Partitioning coeff Array"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol5_coeffPartition" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives5.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project


puts " "
puts "Solution6: as 5 + Top Level Pipeline"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol6_II1_top" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives6.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project

puts " "
puts "Solution6b: as 1 + Top Level Pipeline and coeff Array Partitioning"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol6b_II1_top" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives6b.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project

puts " "
puts "Solution7_2_8: Partial Unroll by 2 to get 8 DSP48"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol7_II2_8DSP" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives7_2_8.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project

puts " "
puts "Solution7_4_4: Partial Unroll by 4 to get 4 DSP48"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol7_II4_4DSP" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives7_4_4.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project

puts " "
puts "Solution7_8_2: Partial Unroll by 8 to get 2 DSP48"
puts " "
open_project vhls_fix_fir_prj
set_top fir_filter
add_files src/fir.cpp -cflags "-DHLS_FIXED_POINT"
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas -DHLS_FIXED_POINT" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "sol7_II8_2DSP" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./fix_fir_directives/directives7_8_2.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -format ip_catalog
close_project
}

