# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Author: Daniele Bagni, Xilinx Inc

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 4) ABOUT PRAGMA DEPENDENCE on RAW TRUE DUAL PORT BRAMs"
puts "************************************************************************"
puts "\n"


############## ORIGINAL C CODE PURELY SEQUENTIAL  #####################

# solution0: default compiler behavior
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp  -cflags "-DDB_ORIGINAL"
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol0_orig_default" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
csim_design -clean
csynth_design
cosim_design
close_project

open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp  -cflags "-DDB_ORIGINAL"
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
# solution1: what if set_directive_dependence -variable hist -type inter WAR -dependent false
open_solution "sol1_orig_depINTER_WAR" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
set_directive_dependence -dependent false -direction WAR -type inter -variable hist "top_histogram"
csynth_design
cosim_design
close_project

# solution1: what if set_directive_dependence -variable hist -type inter RAW -dependent false
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp  -cflags "-DDB_ORIGINAL"
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol1_orig_depINTER_RAW" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
set_directive_dependence -dependent false -direction RAW -type inter -variable hist "top_histogram"
csynth_design
cosim_design
close_project

# solution1: set_directive_dependence -variable hist -type intra RAW -dependent false
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp  -cflags "-DDB_ORIGINAL"
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol1_orig_depINTRA" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
set_directive_dependence -dependent false -direction RAW -type intra -variable hist "top_histogram"
csynth_design
cosim_design
close_project

############## OPTIMIZED C CODE WITH SMALL CACHE  #####################

# solution2: default compiler behavior
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol2_cache_default" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
csim_design -clean
csynth_design
cosim_design
close_project

# solution3: set_directive_dependence -dependent false -direction RAW -type intra -variable hist "top_histogram"
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol3_cache_depINTRA_RAW" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
set_directive_dependence -dependent false -direction RAW -type intra -variable hist "top_histogram"
csim_design -clean
csynth_design
cosim_design
close_project

# solution3: set_directive_dependence -dependent false -direction RAW -type inter -variable hist "top_histogram"
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol3_cache_depINTER_RAW" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
set_directive_dependence -dependent false -direction RAW -type inter -variable hist "top_histogram"
csim_design -clean
csynth_design
cosim_design
close_project

# solution3: set_directive_dependence -dependent false -direction WAR -type inter -variable hist "top_histogram"
open_project hls_dep_prj
set_top top_histogram
add_files top_dependency2.cpp
add_files -tb main_dependency.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "sol3_cache_depINTER_WAR" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
set_directive_top -name top_histogram "top_histogram"
set_directive_dependence -dependent false -direction WAR -type inter -variable hist "top_histogram"
csim_design -clean
csynth_design
cosim_design
close_project
