# Script for AutoPilot Synthesis System
#
# Copyright (C) 2006-2009 AutoESL Design Technologies, Inc.
# AutoESL Confidential. All rights reserved.

# Project settings
open_project proj -reset

# Add the file for synthsis
add_files fir_chan_config.cpp
add_files -tb fir_tb_chan_config.cpp

# Add testbench files for co-simulation
# Set top module of the design
set_top fir_top

# Solution settings
open_solution -reset solution1

# Add library
set_part virtex7

# Set the target clock period
create_clock -period 10

csim_design

csynth_design 

#cosim_design
#cosim_design -rtl vhdl
#export_design -flow impl
