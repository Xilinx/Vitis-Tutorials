# Script for AutoPilot Synthesis System
#
# Copyright (C) 2006-2009 AutoESL Design Technologies, Inc.
# AutoESL Confidential. All rights reserved.

# Project settings
open_project proj -reset

# Add the file for synthsis
add_files fft_top.cpp

# Add testbench files for co-simulation
add_files -tb fft_tb.cpp
add_files -tb data/stimulus_00.dat
add_files -tb data/stimulus_01.dat
add_files -tb data/stimulus_02.dat
add_files -tb data/stimulus_03.dat
add_files -tb data/stimulus_04.dat
add_files -tb data/stimulus_05.dat
add_files -tb data/stimulus_06.dat
add_files -tb data/stimulus_07.dat
add_files -tb data/stimulus_08.dat
add_files -tb data/stimulus_09.dat
add_files -tb data/stimulus_10.dat
add_files -tb data/stimulus_11.dat
add_files -tb data/stimulus_12.dat
add_files -tb data/stimulus_13.dat
add_files -tb data/stimulus_14.dat
add_files -tb data/stimulus_15.dat
add_files -tb data/stimulus_16.dat
add_files -tb data/stimulus_17.dat
add_files -tb data/stimulus_18.dat
add_files -tb data/stimulus_19.dat
add_files -tb data/stimulus_00.res
add_files -tb data/stimulus_01.res
add_files -tb data/stimulus_02.res
add_files -tb data/stimulus_03.res
add_files -tb data/stimulus_04.res
add_files -tb data/stimulus_05.res
add_files -tb data/stimulus_06.res
add_files -tb data/stimulus_07.res
add_files -tb data/stimulus_08.res
add_files -tb data/stimulus_09.res
add_files -tb data/stimulus_10.res
add_files -tb data/stimulus_11.res
add_files -tb data/stimulus_12.res
add_files -tb data/stimulus_13.res
add_files -tb data/stimulus_14.res
add_files -tb data/stimulus_15.res
add_files -tb data/stimulus_16.res
add_files -tb data/stimulus_17.res
add_files -tb data/stimulus_18.res
add_files -tb data/stimulus_19.res

# Set top module of the design
set_top fft_top

# Solution settings
open_solution -reset solution1

# Add library
set_part virtex7

# Set the target clock period
create_clock -period 3.3

# Check why 4 is necessary for the FIRST FIFO only
config_dataflow -start_fifo_depth 4

csim_design

# Synthesis
csynth_design 
#cosim_design 
#cosim_design -wave_debug -trace_level all
#cosim_design -rtl vhdl
#export_design -flow impl
