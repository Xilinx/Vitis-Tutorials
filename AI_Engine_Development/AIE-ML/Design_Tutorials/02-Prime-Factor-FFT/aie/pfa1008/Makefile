#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

AIE_SIM_ONLY      := false
SIM_FIFO          := false

MY_APP            := pfa1008_app

DFT7_SOURCE       := dft7_graph.h dft7_mmul0.h dft7_mmul0.cpp dft7_mmul1.h dft7_mmul1.cpp dft7_mmul2.h \
	             dft7_mmul2.cpp dft7_twiddle.h

TRANSPOSE0_SOURCE := transpose0_graph.h

DFT9_SOURCE       := dft9_graph.h dft9_mmul0.h dft9_mmul0.cpp dft9_mmul1.h dft9_mmul1.cpp dft9_mmul2.h \
                     dft9_mmul2.cpp dft9_mmul3.h dft9_mmul3.cpp dft9_twiddle.h

TRANSPOSE1_SOURCE := transpose1_graph.h

DFT16_SOURCE      := dft16_graph.h dft16_mmul0.h dft16_mmul0.cpp dft16_mmul1.h dft16_mmul1.cpp dft16_twiddle.h

MY_SOURCES        := ${MY_APP}.cpp pfa1008_graph.h ${DFT7_SOURCE:%=../dft7/%} ${DFT9_SOURCE:%=../dft9/%} \
	             ${DFT16_SOURCE:%=../dft16/%} \
	             ${TRANSPOSE0_SOURCE:%=../transpose0/%} ${TRANSPOSE1_SOURCE:%=../transpose1/%}

PLATFORM_USE	  := xilinx_vek280_es1_base_202320_1
PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

MY_INCLUDES 	  := --include=../dft7 \
                     --include=../transpose0 \
                     --include=../dft9 \
                     --include=../dft16 \
                     --include=../transpose1

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${MY_INCLUDES} --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif
ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.Xpreproc="-DAIE_SIM_ONLY"
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
	v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} |& tee log

x86com:
	v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} |& tee log

sim:
	aiesimulator |& tee -a log

x86sim:
	x86simulator --trace |& tee -a log

profile:
	aiesimulator --profile |& tee -a log

trace:
	aiesimulator --online -wdb -ctf |& tee -a log

trace-hang:
	aiesimulator --profile --online -wdb -text --hang-detect-time=50000 --simulation-cycle-timeout=40000 |& tee -a log

throughput:
	@../../scripts/throughput.py aiesimulator_output/data/sig_o_aie.txt --iscomplex

loop_II:
	@../../scripts/get_loop_II.py Work/aie

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
	rm -rf system*.* trdata.aiesim function_wdb_dir

