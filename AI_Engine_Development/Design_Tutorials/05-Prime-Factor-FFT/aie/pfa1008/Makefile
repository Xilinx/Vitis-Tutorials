#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

AIE_SIM_ONLY      := false

SIM_FIFO          := false

MY_APP            := pfa1008_app

DFT7_SOURCE       := dft7_graph.h dft7_compute.h dft7_compute.cpp dft7_combine.h dft7_combine.cpp dft7_twiddle.h

DFT9_SOURCE       := dft9_graph.h dft9_compute.h dft9_compute.cpp dft9_combine.h dft9_combine.cpp \
                     dft9_twiddle.h

DFT16_SOURCE      := dft16_graph.h dft16_compute.h dft16_compute.cpp dft16_combine.h dft16_combine.cpp \
                     dft16_twiddle.h

MY_SOURCES        := ${MY_APP}.cpp pfa1008_graph.h ${DFT7_SOURCE:%=../dft7w/%} ${DFT9_SOURCE:%=../dft9w/%} \
                     ${DFT16_SOURCE:%=../dft16/%}

PLATFORM_USE	  := xilinx_vck190_base_202310_1
PLATFORM          := -platform=${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

PL_FREQ           := --pl-freq=625
OTHER_OPTS        := --kernel-linting -Xchess="-std=c++17" -xlopt=1
MAP_OPT           := -Xmapper=BufferOptLevel9

CHECK_FIFO        := --evaluate-fifo-depth -Xrouter=disablePathBalancing

DSPLIB_OPTS 	  := -include=${DSPLIB_ROOT}/L2/include/aie \
		     -include=${DSPLIB_ROOT}/L1/include/aie \
		     -include=${DSPLIB_ROOT}/L1/src/aie \
		     -include=../dft7w -include=../dft9w -include=../dft16

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${OTHER_OPTS} ${DSPLIB_OPTS} ${MAP_OPT} ${PL_FREQ} ${PLATFORM} ${MY_APP}.cpp --output-archive=${AIE_OUTPUT}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif
ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --Xpreproc="-DAIE_SIM_ONLY"
endif

.PHONY: help clean x86com x86sim sim profile throughput

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

all:		${AIE_OUTPUT}

x86all: 	clean x86com x86sim

gen_vectors:
	matlab -batch "gen_vectors"

${AIE_OUTPUT}:	${MY_SOURCES}
	aiecompiler --target=hw ${AIE_FLAGS} |& tee log

x86com:
	aiecompiler -target=x86sim ${AIE_FLAGS} |& tee log

sim:
	aiesimulator |& tee -a log

x86sim:
	x86simulator |& tee -a log

profile:
	aiesimulator --profile |& tee -a log

trace:
	aiesimulator --online -wdb -ctf |& tee -a log

throughput:
	@../../scripts/throughput.py aiesimulator_output/data/dft7_o_0.txt --iscomplex
	@../../scripts/throughput.py aiesimulator_output/data/dft7_o_1.txt --iscomplex
	@../../scripts/throughput.py aiesimulator_output/data/dft9_o_0.txt --iscomplex
	@../../scripts/throughput.py aiesimulator_output/data/dft9_o_1.txt --iscomplex
	@../../scripts/throughput.py aiesimulator_output/data/dft16_o_0.txt --iscomplex
	@../../scripts/throughput.py aiesimulator_output/data/dft16_o_1.txt --iscomplex

regression:
	matlab -batch regression

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim

