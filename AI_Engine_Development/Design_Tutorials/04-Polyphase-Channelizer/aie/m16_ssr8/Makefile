#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

DO_GRAPH_SIM      := false

MY_APP            := m16_ssr8_app
SOURCES	          := ../m16_ssr8_filterbank/m16_ssr8_filterbank_graph.h \
                     ../m16_ssr8_filterbank/polyphase_fir_graph.h \
                     ../m16_ssr8_filterbank/polyphase_fir.h \
                     ../m16_ssr8_filterbank/polyphase_fir.cpp \
                     ../m16_ssr8_dft/m16_ssr8_dft_graph.h \
                     ../m16_ssr8_dft/dft_1xN_widget_graph.h \
                     ../m16_ssr8_dft/dft_1xN_widget.h \
                     ../m16_ssr8_dft/dft_1xN_widget.cpp \
                     ../m16_ssr8_dft/twiddle_m16_ssr8_dft.h \
		     m16_ssr8_graph.h

PLATFORM_USE      := xilinx_vck190_base_202310_1
PL_FREQ           := --pl-freq=625.0

PLATFORM          := --platform=${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

LOG_FILE          := log

MAP_OPT           := -Xmapper=BufferOptLevel9
OTHER_OPTS        := -Xchess="-std=c++17" -xlopt=1 --log-level=5
CHECK_FIFO        := --evaluate-fifo-depth -Xrouter=disablePathBalancing
AIE_SIM_ONLY      := --Xpreproc="-DAIE_SIM_ONLY"

ALL_INCLUDE       := -include=${PWD} -include=../m16_ssr8_filterbank -include=../m16_ssr8_dft

AIE_OUTPUT        := libadf.a

AIE_FLAGS         := --dataflow -v ${ALL_INCLUDE} ${MY_APP}.cpp ${OTHER_OPTS} ${MAP_OPT} ${PLATFORM} \
			${PL_FREQ}

ifeq (${DO_GRAPH_SIM},true)
	AIE_FLAGS := ${AIE_FLAGS} ${AIE_SIM_ONLY}
endif

.PHONY: help clean all

help:
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to generate everything for this design"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all: 	${AIE_OUTPUT}

${AIE_OUTPUT}:	${MY_APP}.cpp ${SOURCES}
	aiecompiler --target=hw ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

x86compile:
	aiecompiler --target=x86sim ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

sim:
	aiesimulator 2>&1 | tee -a ${LOG_FILE}

x86sim:
	x86simulator 2>&1 | tee -a ${LOG_FILE}

profile:
	aiesimulator --profile --online -wdb -ctf 2>&1 | tee -a ${LOG_FILE}

throughput:
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_0.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_1.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_2.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_3.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_4.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_5.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_6.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/fir_o_7.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_0.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_1.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_2.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_3.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_4.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_5.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_6.txt --iscomplex
	@../../../tools/python/throughput.py aiesimulator_output/data/dft_o_7.txt --iscomplex

clean:
	rm -rf Work Work* ${WORK_DIR} libadf.a
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf x86simulator_output
	rm -rf log log* ${LOG_FILE}
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* vck190_thin_wrapper*
	rm -rf temp ISS_RPC_SERVER_PORT
	rm -rf system*.* vitis_design_wrapper* .crashReporter* .AIE_SIM_CMD_LINE_OPTIONS
