#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz

SRC_DIR           := ./src
MY_SOURCES        := $(shell find $(SRC_DIR) -name *.cpp) $(shell find $(SRC_DIR) -name *.h)

PLATFORM_USE	  := xilinx_vck190_base_202310_1
PLATFORM          := --platform=${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

COMPILE_OPTS      := --aie.pl-freq=625 --aie.adf-api-log-level=2 --aie.kernel-linting \
                     --aie.verbose ${PLATFORM} --aie.workdir=./Work --aie.xlopt=1 \
					 --aie.Xmapper=BufferOptLevel9

X86_SIM_OPTS      := --pkg-dir=./Work --i=./ --o=./x86simulator_output
AIE_SIM_OPTS      := --pkg-dir=./Work --i=./ --o=./aiesimulator_output --display-run-interval=1000

.PHONY: help x86com aiesim x86sim profile trace gen_vectors check_aiesim check_x86sim analyze clean

help::
	@echo "Makefile Usage:"
	@echo "  make aiecom"
	@echo "      Compile design for AIE emulation"
	@echo ""
	@echo "  make x86com"
	@echo "      Compile design for x86 simulation"
	@echo ""
	@echo "  make aiesim"
	@echo "      Run simulation using AIE emulation"
	@echo ""
	@echo "  make x86sim"
	@echo "      Run x86 simulation for functional verification"
	@echo ""
	@echo "  make check_x86sim"
	@echo "      Use MATLAB to compare simulation results with golden reference"
	@echo ""
	@echo "  make gen_vectors"
	@echo "      Use MATLAB to generate test vectors for AIE simulation"
	@echo ""
	@echo "  make check_aiesim"
	@echo "      Use MATLAB to compare AIE simulation with golden reference"
	@echo ""
	@echo "  make check_x86sim"
	@echo "      Use MATLAB to compare x86 simulation with golden reference"
	@echo ""
	@echo "  make analyze"
	@echo "      Run Vitis Analyzer to view simulation report and profiling data"
	@echo ""
	@echo "  make clean"
	@echo "      Delete files generated during compilation and simulation"

all:       gen_vectors aiecom aiesim check_aiesim

aiecom:    ${MY_SOURCES}
	v++ --compile --target=hw --mode aie ${COMPILE_OPTS} ${SRC_DIR}/blint_app.cpp

x86com:
	v++ --compile --target=x86sim --mode aie ${COMPILE_OPTS} ${SRC_DIR}/blint_app.cpp

aiesim:
	aiesimulator ${AIE_SIM_OPTS}

x86sim:
	x86simulator ${X86_SIM_OPTS}

profile:
	aiesimulator ${AIE_SIM_OPTS} --dump-vcd=aiesim --profile

trace:
	aiesimulator ${AIE_SIM_OPTS} --online -wdb

gen_vectors:
	matlab -nodisplay -r "try; run('../matlab/image_transform.m'); catch; end; quit"
	matlab -nodisplay -r "try; run('../matlab/genvectors_bilinear_interp.m'); catch; end; quit"

check_aiesim:
	matlab -nodisplay -r "try; run('../matlab/check_aiesim.m'); catch; end; quit"

check_x86sim:
	matlab -nodisplay -r "try; run('../matlab/check_x86sim.m'); catch; end; quit"

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	rm -rf .Xil Work libadf.a
	rm -rf *.log *.csv *.db *.vcd logs
	rm -rf x86simulator_output aiesimulator_output pl_sample_counts
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS ISS_RPC_SERVER_PORT
	rm -rf pl_sample_count* ./data/*txt
