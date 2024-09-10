# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 8) VECTOR ADDITION DEMO"
puts "************************************************************************"
puts "\n"

open_project hls_vadd_prj
set_top krnl_vadd
add_files src/hls/hls_krnl_vadd.cpp
add_files -tb src/hls/hls_tb_vadd.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
#add_files -tb src/vadd_include.h  -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"

open_solution "solution_vivado" -flow_target vivado
set_part xczu9eg-ffvb1156-2-e
create_clock -period 150.000000MHz -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
#source "./hls_vadd_prj/solution1/directives.tcl"
csim_design -clean
csynth_design
#cosim_design

open_solution "solution_vitis" -flow_target vitis
set_part xczu9eg-ffvb1156-2-e
create_clock -period 150.000000MHz -name default
# v++ --advanced.param compiler.hlsDataflowStrictMode
config_dataflow -strict_mode warning
# v++ --advanced.param compiler.deadlockDetection
config_export -deadlock_detection sim
# v++ --advanced.param compiler.axiDeadLockFree
config_interface -m_axi_conservative_mode=1
config_interface -m_axi_addr64
# v++ --hls.max_memory_ports
config_interface -m_axi_auto_max_ports=0
config_export -format xo -ipname krnl_vadd
csynth_design
#cosim_design
#export_design
close_project
