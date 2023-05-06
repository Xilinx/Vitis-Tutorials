# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


open_project vhls_hist_src4_prj
set_top top_histogram_equaliz
add_files img_hist_v4.cpp -cflags "-I./common"
add_files -tb common/ap_bmp.cpp -cflags "-Icommon -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb common/main_test.cpp -cflags "-Icommon -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb common/ref_img_hist.cpp -cflags "-Icommon -Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb common/test_data -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-i}
create_clock -period 3 -name default
config_export -format ip_catalog -rtl vhdl
set_clock_uncertainty 0.5
set_directive_top -name top_histogram_equaliz "top_histogram_equaliz"
csim_design -clean
csynth_design
