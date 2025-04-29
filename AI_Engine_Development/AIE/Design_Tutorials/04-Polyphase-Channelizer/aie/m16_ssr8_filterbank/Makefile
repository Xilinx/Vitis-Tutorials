#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

SIM_FIFO          := false

MY_APP            := m16_ssr8_filterbank_app
SOURCES           := m16_ssr8_filterbank_graph.h polyphase_fir_graph.h polyphase_fir.h polyphase_fir.cpp

PLATFORM_USE      := xilinx_vck190_base_202510_1
PL_FREQ           := --aie.pl-freq=625

PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

LOG_FILE          := log

MAP_OPT           := --aie.Xmapper=BufferOptLevel9
OTHER_OPTS        := --aie.Xchess="-std=c++17" --aie.xlopt=1
CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

ALL_INCLUDE       := --include=${PWD}

AIE_OUTPUT        := libadf.a

AIE_FLAGS         :=  --aie.verbose ${ALL_INCLUDE} ${MY_APP}.cpp ${OTHER_OPTS} ${MAP_OPT} ${PL_FREQ} --platform=${PLATFORM}

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

all: 	${AIE_OUTPUT} sim throughput_ok

${AIE_OUTPUT}:	 ${MY_APP}.cpp $(SOURCES)
	v++ -c --mode aie --target=hw $(AIE_FLAGS) 2>&1 | tee ${LOG_FILE}

x86compile:
	v++ -c --mode aie -target=x86sim ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

sim:
	aiesimulator 2>&1 | tee -a ${LOG_FILE}

x86sim:
	x86simulator 2>&1 | tee -a ${LOG_FILE}

profile:
	aiesimulator --profile --online -wdb -text 2>&1 | tee -a ${LOG_FILE}

loopII:
	@../common/get_loop_II.py Work/aie

throughput_ok:
	../common/check_throughput.py PLIO_fbank_o_0 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_1 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_2 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_3 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_4 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_5 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_6 4985.00 5.0
	../common/check_throughput.py PLIO_fbank_o_7 4985.00 5.0

clean:
	rm -rf Work Work*
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf x86simulator_output
	rm -rf log log* ${LOG_FILE}
	rm -rf libadf.a *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* trdata.aiesim
	rm -rf temp ISS_RPC_SERVER_PORT vitis_design_wrapper*
	rm -rf system*.* .crashReporter* .AIE_SIM_CMD_LINE_OPTIONS function_wdb_dir

