# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

SIMULATOR := xsim

compile:
	v++ -c --mode aie --platform=$(PLATFORM) --aie.pl-freq 500 --aie.adf-api-log-level 3 --include=./aie  --output=graph.json aie/graph.cpp

run:
	aiesimulator --pkg-dir Work --profile
	
clean:
	rm -f *.log libadf.a me.soln
	rm -rf Work aiesimulator_output regression_results ISS_RPC_SERVER_PORT pl_sample_counts
