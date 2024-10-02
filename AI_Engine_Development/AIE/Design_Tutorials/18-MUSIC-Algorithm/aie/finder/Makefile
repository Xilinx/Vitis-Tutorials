#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Peifang Zhou, Bachir Berkane, Mark Rollins

AIE_SIM_ONLY      := false

SIM_FIFO          := false

MY_APP            := finder_app
MY_SOURCES        := ${MY_APP}.cpp finder.h finder.cpp finder_graph.h

PLATFORM_USE      := xilinx_vck190_base_202410_1
PLATFORM          ?= ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

DSPLIB_OPTS 	  := --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/src/aie \
                     --include=..

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${DSPLIB_OPTS} --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif
ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.Xpreproc="-DAIE_SIM_ONLY"
endif


.PHONY: help

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
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all:		${AIE_OUTPUT}

x86all: 	clean x86com x86sim

${AIE_OUTPUT}:	${MY_SOURCES}
	time v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} 2>&1| tee log

x86com:
	time v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} 2>&1| tee log

sim:
	time aiesimulator 2>&1| tee -a log

x86sim:
	time x86simulator 2>&1| tee -a log

profile:
	time aiesimulator --online -wdb -text --profile 2>&1| tee -a log

trace:
	time aiesimulator --online -wdb -text 2>&1| tee -a log

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir

