#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz

SRC_DIR           := ./src
MY_SOURCES        := $(shell find $(SRC_DIR) -name *.cpp) $(shell find $(SRC_DIR) -name *.h)

PLATFORM_USE	  := xilinx_vck190_base_202520_1
PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

COMPILE_OPTS      := --aie.pl-freq=625 --aie.adf-api-log-level=2 --aie.kernel-linting \
                     --aie.verbose --platform=${PLATFORM} --aie.workdir=./Work --aie.xlopt=1 \
					 --aie.Xmapper=BufferOptLevel9

X86_SIM_OPTS      := --pkg-dir=./Work --i=./ --o=./x86simulator_output
AIE_SIM_OPTS      := --pkg-dir=./Work --input-dir=./ --output-dir=./aiesimulator_output --display-run-interval=1000

.PHONY: help x86com aiesim x86sim profile analyze clean loop_ii x86check

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
	@echo "  make x86check"
	@echo "      Check x86 simulation results against reference data"
	@echo ""
	@echo "  make profile"
	@echo "      Run AIE simulation with profiling"
	@echo ""
	@echo "  make analyze"
	@echo "      Run Vitis Analyzer to view simulation report and profiling data"
	@echo ""
	@echo "  make loop_ii"
	@echo "      Run python script to extract and report loop II."
	@echo ""
	@echo "  make clean"
	@echo "      Delete files generated during compilation and simulation"

all:	aiecom aiesim

aiecom:    ${MY_SOURCES}
	v++ --compile --target=hw --mode aie ${COMPILE_OPTS} ${SRC_DIR}/softdemod_app.cpp

x86com:
	v++ --compile --target=x86sim --mode aie ${COMPILE_OPTS} --aie.Xpreproc=-D_DUMPLOOP_ ${SRC_DIR}/softdemod_app.cpp

aiesim:
	aiesimulator ${AIE_SIM_OPTS}

x86sim:
	x86simulator ${X86_SIM_OPTS}

profile:
	aiesimulator ${AIE_SIM_OPTS} --dump-vcd=aiesim --profile

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

loop_ii:
	./get_loop_II.py ./Work/aie

x86check:
	./check_loop1.py

clean:
	rm -rf .Xil Work libadf.a
	rm -rf *.log *.csv *.db *.vcd logs
	rm -rf x86simulator_output aiesimulator_output pl_sample_counts
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS ISS_RPC_SERVER_PORT
	rm -rf pl_sample_count* _idx *.json
