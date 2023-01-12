#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# to start
cd examples

# simple examples
cd 0_basic
cd 01_Demo_SimpleLoop
vitis_hls 01_hls_script.tcl
cd ../02_Demo_GlobalView
vitis_hls 02_hls_script.tcl
cd ../03_Demo_Dataflow
vitis_hls 03_hls_script.tcl
cd ../04_Demo_FloatLatency
vitis_hls 04_hls_script.tcl
cd ../05_Demo_ArbitraryIntegers
vitis_hls 05_hls_script.tcl
cd ../06_Demo_Imperfect-vs-PerfectLoop
vitis_hls 06_hls_script.tcl
cd ../..

# serious examples
cd 1_fix_fir_filter
vitis_hls fix_fir_script.tcl
cd ../2_float_fir_filter
vitis_hls float_fir_script.tcl
cd ../3_img_median_filter
vitis_hls run_median_script.tcl
cd ../4_dependency
rm -r *_dep_prj
vitis_hls run_dependence_script.tcl
cd ../5_img_histEq
rm -r vhls_hist_*_prj
vitis_hls run_src3_hls_script.tcl
vitis_hls run_src4_hls_script.tcl
cd ../6_sqrt
vitis_hls run_sqrt_script.tcl
cd ../7_atan2
vitis_hls run_atan2_script.tcl
cd ../8_vect_add
vitis_hls ./run_vect_add_script.tcl
cd ../9_matrix_mult
vitis_hls run_matrix_mult_script.tcl


# to finish
cd ../..
