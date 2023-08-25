#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

# Testbench (note relative path):
add_files -tb ../../../tb_wrapper.cpp -cflags "-g -I../../.."

# Run C-simulation:
csim_design

