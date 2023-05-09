# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

SIMULATOR := xsim

compile:
	aiecompiler --aiearch=aie --platform=$(PLATFORM)  -v -log-level=3 --pl-freq=500 -include=./aie --dataflow --output=graph.json aie/graph.cpp

run:
	aiesimulator --pkg-dir Work --profile
	
clean:
	rm -f *.log libadf.a me.soln
	rm -rf Work aiesimulator_output regression_results ISS_RPC_SERVER_PORT pl_sample_counts
