#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

# Testbench (note relative path):
add_files -tb ../../../tb_wrapper.cpp -cflags "-g -I../../..  -I../../../data"
add_files -tb ../../../data/stream0_i.txt
add_files -tb ../../../data/stream1_i.txt
add_files -tb ../../../data/stream2_i.txt
add_files -tb ../../../data/stream3_i.txt
add_files -tb ../../../data/stream4_i.txt
add_files -tb ../../../data/seq_o.txt

# Run C-simulation:
csim_design

