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

PLATFORM_USE      := xilinx_vck190_base_202410_1
PL_FREQ           := --aie.pl-freq=625.0

PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

LOG_FILE          := log

MAP_OPT           := --aie.Xmapper=BufferOptLevel9
OTHER_OPTS        := --aie.Xchess="-std=c++17" --aie.xlopt=1 --aie.log-level=5
CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing
AIE_SIM_ONLY      := --aie.Xpreproc="-DAIE_SIM_ONLY"

ALL_INCLUDE       := --include=${PWD} --include=../m16_ssr8_filterbank --include=../m16_ssr8_dft

AIE_OUTPUT        := libadf.a

AIE_FLAGS         :=  --aie.verbose ${ALL_INCLUDE} ${MY_APP}.cpp ${OTHER_OPTS} ${MAP_OPT} ${PL_FREQ} --platform=${PLATFORM} 

XPE_FILE_NAME     := ${MY_APP}.xpe
VCD_FILE_NAME     := ${MY_APP}

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
	v++ -c --mode aie --target=hw ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

x86compile:
	v++ -c --mode aie --target=x86sim ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

sim:
	aiesimulator 2>&1 | tee -a ${LOG_FILE}

x86sim:
	x86simulator 2>&1 | tee -a ${LOG_FILE}

profile:
	aiesimulator --profile --online -wdb -text 2>&1 | tee -a ${LOG_FILE}

throughput:
	@../common/throughput.py aiesimulator_output/data/fir_o_0.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_1.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_2.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_3.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_4.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_5.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_6.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/fir_o_7.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_0.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_1.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_2.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_3.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_4.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_5.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_6.txt --iscomplex
	@../common/throughput.py aiesimulator_output/data/dft_o_7.txt --iscomplex

vitis_xpe: ${VCD_FILE_NAME}.vcd

${VCD_FILE_NAME}.vcd:
	@echo "Generating m16_ssr8_app.xpe for PDM"
	@v++ -c --mode aie --target=hw ${AIE_FLAGS} ${AIE_SIM_ONLY} 2>&1 | tee ${LOG_FILE}
	@aiesimulator --pkg-dir Work --profile --dump-vcd ${VCD_FILE_NAME} 2>&1 | tee -a vcd.log
	@vcdanalyze --vcd ${VCD_FILE_NAME}.vcd --xpe

clean:
	rm -rf Work Work* ${WORK_DIR} libadf.a
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf x86simulator_output
	rm -rf log log* ${LOG_FILE}
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json *.jou *.ir
	rm -rf vitis_analyzer* pl_sample_counts* vck190_thin_wrapper* trdata.aiesim
	rm -rf temp ISS_RPC_SERVER_PORT
	rm -rf system*.* vitis_design_wrapper* .crashReporter* .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf aiesim_xpe .Pdm
