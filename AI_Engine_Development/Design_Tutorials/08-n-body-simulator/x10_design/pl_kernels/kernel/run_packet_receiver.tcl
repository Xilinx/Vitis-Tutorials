#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

#create new project
open_project -reset packet_receiver

#specify the top-level function
set_top packet_receiver

# add design files
add_files packet_receiver.cpp

# add testbench files
add_files -tb packet_receiver_test.cpp
add_files -tb input_i0.dat

# create the solution named solution1 
open_solution solution1 -reset -flow_target vitis

# associate the device to solution1
set_part {xcvc1902-vsva2197-1LHP-i-L}

# associate clock with 10ns period for solution1
create_clock -period 2.5ns

# run C simulation
csim_design

# sythesize the design 
csynth_design

# perform C/RTL Cosimulation
cosim_design

#export RTL as IP
#export_design -flow impl -format xo

exit
