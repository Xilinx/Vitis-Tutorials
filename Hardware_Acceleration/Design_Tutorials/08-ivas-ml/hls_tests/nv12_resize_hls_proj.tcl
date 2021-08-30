############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2020 Xilinx, Inc. All Rights Reserved.
############################################################
open_project nv12_resize
set_top resize_accel_rgb
add_files ../hw_src/resize_rgb.h
add_files ../hw_src/resize_rgb.cpp -cflags "-I../hw_src/Vitis_Libraries/vision/L1/include -D__SDSVHLS__ -std=c++0x" -csimflags "-D__SDSVHLS__ -I../hw_src/Vitis_Libraries/vision/L1/include -std=c++0x"
add_files -tb ../hw_src/resize_rgb_tb.cpp -cflags "-I../hw_src/Vitis_Libraries/vision/L1/include -D__SDSVHLS__ -std=c++0x -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb ../hw_src/resize_rgb.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb ../hw_src/resize_rgb.cpp -cflags "-I../hw_src/Vitis_Libraries/vision/L1/include -D__SDSVHLS__ -D__XFCV_HLS_MODE__ -std=c++0x -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1"
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.3333333 -name default
config_sdx -optimization_level none -target xocc
config_export -vivado_optimization_level 0 -vivado_phys_opt none
config_compile -name_max_length 80 -pipeline_loops 64
config_schedule -enable_dsp_full_reg
config_rtl -module_auto_prefix -register_reset_num 3
config_interface -m_axi_addr64
set_clock_uncertainty 27%
csim_design -ldflags {-lpng -lxml2 -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs} -argv {../../../../test_data/piano_1920x1080_raw_stride_2k.nv12} -clean -setup
csynth_design
cosim_design -wave_debug -ldflags {-lpng -lxml2 -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs} -trace_level all -argv {../../../../test_data/piano_1920x1080_raw_stride_2k.nv12} -tool xsim -setup
exit
