#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

SIM_FIFO          := false

MY_APP            := hough_tile_app
MY_SOURCES        := ${MY_APP}.cpp hough_tile.h hough_tile.cpp hough_tile_shared.h hough_tile_graph.h

PLATFORM_USE      := xilinx_vck190_base_202410_1
PLATFORM          ?= ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

AIE_OUTPUT        := libadf.a

AIE_FLAGS         := --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif


.PHONY: help clean all

help::
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to generate everything for this design"
	@echo ""
	@echo "  make compile"
	@echo "      Run AIE compiler and build the design"
	@echo ""
	@echo "  make profile"
	@echo "      Run AIE simulator with profiling and generate outputs suitable for Vitis Analyzer"
	@echo ""
	@echo "  make analyze"
	@echo "      Run Vitis Analyzer to inspect results"
	@echo ""
	@echo "  make throughput"
	@echo "      Evaluate the achieved throughput at the output"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all: 	x86com x86sim ${AIE_OUTPUT}

${AIE_OUTPUT}:	 ${MY_APP}.cpp $(SOURCES)
	v++ --compile --mode aie --config aie.cfg --target=hw $(AIE_FLAGS) 2>&1| tee log

x86com:
	v++ --compile --mode aie --config aie.cfg --target=x86sim ${AIE_FLAGS} 2>&1| tee log

sim:
	aiesimulator 2>&1| tee -a log

x86sim:
	x86simulator 2>&1| tee -a log

trace:
	aiesimulator --online -wdb -ctf 2>&1| tee -a log

profile:
	aiesimulator --profile 2>&1| tee -a log

loop_II:
	@../scripts/get_loop_II.py Work/aie

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir
