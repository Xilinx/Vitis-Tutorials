# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# Author: Daniele Bagni, Xilinx Inc

puts "\n"
puts "************************************************************************"
puts "* 2) FLOATING POINT FIR FILTER DEMO"
puts "************************************************************************"
puts "\n"


puts " "
puts "Solution0: fadd/fmul with FABRIC"
puts " "
open_project vhls_float_fir_prj
set_top fir_filter
add_files src/fir.cpp
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "solution0" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./float_fir_directives/directives0.tcl"
csim_design -clean
csynth_design
#cosim_design
#export_design -rtl vhdl -format ip_catalog
close_project


puts " "
puts "Solution1: fadd/fmul with DSP (full/med)"
puts " "
open_project vhls_float_fir_prj
set_top fir_filter
add_files src/fir.cpp
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "solution1" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./float_fir_directives/directives1.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -rtl vhdl -format ip_catalog
close_project



puts " "
puts "Solution2: fadd/fmul with DSP (full/full)"
puts " "
open_project vhls_float_fir_prj
set_top fir_filter
add_files src/fir.cpp
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "solution2" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./float_fir_directives/directives2.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -rtl vhdl -format ip_catalog
close_project


puts " "
puts "Solution3: fadd/fmul with DSP (full/max)"
puts " "
open_project vhls_float_fir_prj
set_top fir_filter
add_files src/fir.cpp
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "solution3" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./float_fir_directives/directives3.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -rtl vhdl -format ip_catalog
close_project



puts " "
puts "Solution4: as 3 + top II=1 and array c partitioned"
puts " "
open_project vhls_float_fir_prj
set_top fir_filter
add_files src/fir.cpp
add_files -tb src/fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/data
open_solution "solution4" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 5 -name default
config_interface -m_axi_latency 0
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 1
source "./float_fir_directives/directives4.tcl"
#csim_design -clean
csynth_design
#cosim_design
#export_design -rtl vhdl -format ip_catalog
close_project
