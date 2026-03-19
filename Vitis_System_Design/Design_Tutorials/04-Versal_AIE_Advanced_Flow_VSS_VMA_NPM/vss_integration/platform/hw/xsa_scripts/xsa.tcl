#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************

file mkdir build 
cd build
source ../xsa_scripts/project.tcl
source ../xsa_scripts/dr.bd.tcl
source ../xsa_scripts/pfm_decls.tcl
#For Questa Simulator
source ../data/questa_sim.tcl 

source ../xsa_scripts/add_axis_vip.tcl
set NUM_OUT 1
create_axi_outputs $NUM_OUT

# Export clock
create_bd_port -dir O -type clk pl0_ref_clk
connect_bd_net [get_bd_pins /CIPS_0/pl0_ref_clk] [get_bd_ports pl0_ref_clk]

source ../xsa_scripts/generate_platform.tcl