# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# Author: Daniele Bagni, Xilinx Inc

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 6) ABOUT Z = SQRT( X^2 + Y^2)"
puts "************************************************************************"
puts "\n"


# to enable TOTALLY floating point sqrt reference function for SOLUTION 0
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_DOUBLE"
add_files float_sqrt.cpp    -cflags "-DSQRT_DOUBLE"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_DOUBLE"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_DOUBLE"
add_files -tb test_main.cpp -cflags "-DSQRT_DOUBLE"
open_solution "solution0"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
#csim_design -ldflags {{-Wl,--stack=268435456}} -clean #ONLY FOR WINDOWS
csim_design -clean
csynth_design
#cosim_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc #ONLY FOR WINDOWS
close_project

if 1 {
# to enable TOTALLY floating point sqrt reference function for SOLUTION 1
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_REF"
add_files float_sqrt.cpp    -cflags "-DSQRT_REF"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_REF"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_REF"
add_files -tb test_main.cpp -cflags "-DSQRT_REF"
open_solution "solution1"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
#csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csim_design -clean
csynth_design
#cosim_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
close_project

# to enable floating point sqrt function while the rest is integer for SOLUTION 2
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_FLOAT"
add_files float_sqrt.cpp    -cflags "-DSQRT_FLOAT"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_FLOAT"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_FLOAT"
add_files -tb test_main.cpp -cflags "-DSQRT_FLOAT"
open_solution "solution2"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -clean
#csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
close_project

# to enable full version of Cordic sqrt function for SOLUTION3
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_CORDIC"
add_files float_sqrt.cpp    -cflags "-DSQRT_CORDIC"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_CORDIC"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_CORDIC"
add_files -tb test_main.cpp -cflags "-DSQRT_CORDIC"
open_solution "solution3"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -clean
#csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
close_project

# to enable arbitrary precision datatypes of Cordic sqrt function for SOLUTION4
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files float_sqrt.cpp    -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files -tb test_main.cpp -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
open_solution "solution4"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -clean
#csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
close_project
}




# empty solution just to remove CFLAGS from the GUI"
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp
add_files float_sqrt.cpp
add_files cordic_sqrt.cpp
add_files cordic_isqrt.cpp
add_files -tb test_main.cpp
open_solution "solution_empty"  -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3.0 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
close_project
