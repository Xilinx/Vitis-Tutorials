# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# Author: Daniele Bagni, Xilinx Inc

puts "\n"
puts "************************************************************************"
puts "* EXAMPLE 5) IMAGE HISTOGRAM EQUALIZATION DEMO"
puts "************************************************************************"
puts "\n"

open_project vhls_hist_src3_prj
set_top top_histogram_equaliz
add_files img_hist_v3.cpp             -cflags "-I./common"
add_files -tb common/ap_bmp.cpp       -cflags "-I./common -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb common/main_test.cpp    -cflags "-I./common -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb common/ref_img_hist.cpp -cflags "-I./common -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb common/test_data        -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"


################## SOLUTION1: Baseline ####################
open_solution "solution1" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_export -rtl vhdl
set_clock_uncertainty 0.5
set_directive_top -name top_histogram_equaliz "top_histogram_equaliz"
csim_design -clean
csynth_design

################## SOLUTION2: DATAFLOW ####################
open_solution "solution2" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_export -rtl vhdl
set_clock_uncertainty 0.5
set_directive_top -name top_histogram_equaliz "top_histogram_equaliz"
set_directive_dataflow "top_histogram_equaliz"
csynth_design

################## SOLUTION3: DATAFLOW + CONFIG_DATAFLOW ####################
open_solution "solution3" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_export -rtl vhdl
set_clock_uncertainty 0.5
config_dataflow -default_channel fifo
set_directive_top -name top_histogram_equaliz "top_histogram_equaliz"
set_directive_dataflow "top_histogram_equaliz"
csynth_design

################## SOLUTION4: as Solution3 + PIPELINE  all the innermost loops ####################
open_solution "solution4" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_export -rtl vhdl
set_clock_uncertainty 0.5
config_dataflow -default_channel fifo
set_directive_top -name top_histogram_equaliz "top_histogram_equaliz"
set_directive_dataflow "top_histogram_equaliz"
set_directive_stream -depth 2 -type fifo "top_histogram_equaliz" yuv_R
set_directive_stream -depth 2 -type fifo "top_histogram_equaliz" yuv_G
set_directive_stream -depth 2 -type fifo "top_histogram_equaliz" yuv_B
set_directive_stream -depth 2 -type fifo "top_histogram_equaliz" yeq_R
set_directive_stream -depth 2 -type fifo "top_histogram_equaliz" yeq_G
set_directive_stream -depth 2 -type fifo "top_histogram_equaliz" yeq_B
#set_directive_array_partition -dim 0 -type complete "yuv2rgb" Wyuv
#set_directive_array_partition -dim 0 -type complete "rgb2yuv" Wrgb
csynth_design

close_project
