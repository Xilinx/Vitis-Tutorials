#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

AIE_SIM_ONLY      ?= true
SIM_FIFO          := false

MY_APP            := channelizer_app
MY_SOURCES        := ${MY_APP}.cpp channelizer_graph.h channelizer_init_taps.h ../tdm_fir/firbank_graph.h ../ifft4096_2d/ifft4096_2d_graph.h

PART              := xcve2802-vsvh1760-2MP-e-S

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

DSPLIB_OPTS 	  := --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/vss/common \
		     --include=${DSPLIB_ROOT}/L1/include/vss/vss_fft_ifft_1d \
		     --include=${DSPLIB_ROOT}/L1/src/aie

OTHER_INCLUDE	  := --include=../ifft4096_2d \
		     --include=../tdm_fir

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${DSPLIB_OPTS} ${OTHER_INCLUDE} --part=${PART} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

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

all:		compile profile throughput

x86all: 	clean x86com x86sim

gen_vectors:
	matlab -batch "gen_vectors"

compile:	${MY_SOURCES}
	v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} 2>&1 |  tee log

x86com:
	v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} 2>&1 |  tee log

sim:
	aiesimulator 2>&1 |  tee -a log

x86sim:
	x86simulator 2>&1 |  tee -a log

profile:
	aiesimulator --profile --online -wdb -text 2>&1 | tee -a ${LOG_FILE}

throughput:
	@for z in `seq 0 31`; do \
		../../scripts/throughput.py aiesimulator_output/data/filterbank_o_$${z}.txt --iscomplex;\
	done;
	@for z in `seq 0 7`; do \
		../../scripts/throughput.py aiesimulator_output/data/fft_front_o_$${z}.txt --iscomplex;\
	done;
	@for z in `seq 0 7`; do \
		../../scripts/throughput.py aiesimulator_output/data/fft_back_o_$${z}.txt --iscomplex;\
	done;

get_II:
	@scripts/get_loop_II.py Work/aie/

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
	rm -rf system*.* trdata.aiesim function_wdb_dir .wsdata _ide

