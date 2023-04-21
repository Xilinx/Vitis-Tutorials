##
## Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
## SPDX-License-Identifier: X11
##

# Create a project
open_project proj -reset

# Add design files
add_files diamond.cpp
# Add test bench & files
add_files -tb diamond_test.cpp
add_files -tb result.golden.dat
# Set the top-level function
set_top diamond

# ########################################################
# Create a solution
open_solution -reset solution1
# Define technology and clock rate
set_part {xcvu9p-flga2104-2-i}
create_clock -period 5 -name default

csim_design
csynth_design
cosim_design

exit
