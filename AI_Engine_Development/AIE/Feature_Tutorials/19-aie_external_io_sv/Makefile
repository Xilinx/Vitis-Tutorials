# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

SIMULATOR := xsim
AIE_WORK_DIR := Work

compile:
	v++ -c --mode aie --platform=$(PLATFORM) --aie.pl-freq 500 --aie.adf-api-log-level 3 --include=./aie  --output=graph.json aie/graph.cpp

run_xsim: compile 
	python3 ${XILINX_VITIS}/data/emulation/scripts/gen_aie_wrapper.py -json $(AIE_WORK_DIR)/config/scsim_config.json
	vivado -mode batch -source aie_wrapper_ext_tb_proj.tcl
	
aiesim:
	aiesimulator --pkg-dir Work --profile
	
clean:
	rm -f *.log libadf.a me.soln
	rm -rf Work aiesimulator_output regression_results ISS_RPC_SERVER_PORT pl_sample_counts
