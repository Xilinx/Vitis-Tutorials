#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

USE_JSON          := true

MY_APP            := radioml_top_app
MY_SOURCES        := ${MY_APP}.cpp radioml_top_graph.h ../conv1d_w1/conv1d_w1.h ../conv1d_w1/conv1d_w1.cpp ../conv1d_w1/conv1d_w1_graph.h ../max_pool1d_w2/max_pool1d_w2.h ../max_pool1d_w2/max_pool1d_w2.cpp ../max_pool1d_w2/max_pool1d_w2_graph.h ../conv1d_template/conv1d_template.cpp ../conv1d_template/conv1d_template.h ../conv1d_template/conv1d_graph_template.h ../max_pool1d_template/max_pool1d_template.cpp ../max_pool1d_template/max_pool1d_template.h ../max_pool1d_template/max_pool1d_graph_template.h ../dense_w16/dense_w16_graph.h ../dense_w16/dense_w16.h ../dense_w16/dense_w16.cpp ../dense_w17/dense_w17_graph.h ../dense_w17/dense_w17.h ../dense_w17/dense_w17.cpp ../dense_w18/dense_w18_graph.h ../dense_w18/dense_w18.h ../dense_w18/dense_w18.cpp

PART              ?= xc2ve3858-ssva2112-2MP-e-S

INCLUDES 	  := --include=../conv1d_w1 \
	             --include=../max_pool1d_w2 \
	             --include=../conv1d_template \
	             --include=../max_pool1d_template \
	             --include=../dense_w16 \
	             --include=../dense_w17 \
	             --include=../dense_w18 \
	             --include=../utility

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	--part=${PART} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT} ${INCLUDES}

ifeq (${USE_JSON},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.constraints=${MY_APP}_aie_routed.aiecst
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

${AIE_OUTPUT}:	${MY_SOURCES}
	v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} 2>&1 | tee log

x86com:
	v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} 2>&1 | tee log

sim:
	aiesimulator 2>&1 | tee -a log

x86sim:
	x86simulator 2>&1 | tee -a log

profile:
	aiesimulator --online -wdb -text --profile 2>&1 | tee -a log

trace:
	aiesimulator --hang-detect-time=20000 --online -wdb -text 2>&1 | tee -a log

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir .wsdata _ide vfs_work .ipynb_checkpoints data_o_sim.txt

