#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

AIE_SIM_ONLY      := false

SIM_FIFO          := false

MY_APP            := mnist_app
MY_SOURCES        := ${MY_APP}.cpp mnist_graph.h \
		     ../conv2d_w1/conv2d_w1.h ../conv2d_w1/conv2d_w1.cpp ../conv2d_w1/conv2d_w1_graph.h \
		     ../max_pooling2d_w2/max_pooling2d_w2.h ../max_pooling2d_w2/max_pooling2d_w2.cpp \
                     ../max_pooling2d_w2/max_pooling2d_w2_graph.h \
		     ../conv2d_w3/conv2d_w3.h ../conv2d_w3/conv2d_w3.cpp ../conv2d_w3/conv2d_w3_graph.h \
		     ../max_pooling2d_w4/max_pooling2d_w4.h ../max_pooling2d_w4/max_pooling2d_w4.cpp \
		     ../max_pooling2d_w4/max_pooling2d_w4_graph.h \
		     ../conv2d_w5/conv2d_w5A.h ../conv2d_w5/conv2d_w5B.h ../conv2d_w5/conv2d_w5C.h \
		     ../conv2d_w5/conv2d_w5D.h ../conv2d_w5/conv2d_w5A.cpp ../conv2d_w5/conv2d_w5B.cpp \
		     ../conv2d_w5/conv2d_w5C.cpp ../conv2d_w5/conv2d_w5D.cpp ../conv2d_w5/conv2d_w5_graph.h \
		     ../dense_w7/dense_w7.h ../dense_w7/dense_w7.cpp ../dense_w7/dense_w7_graph.h \
		     ../wts_init/wts_init.h ../wts_init/wts_init.cpp ../wts_init/wts_init_graph.h

PLATFORM_USE      := xilinx_vek280_base_202510_1
PLATFORM          ?= ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

DSPLIB_OPTS 	  := --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/src/aie \
	             --include=../conv2d_w1 \
	             --include=../max_pooling2d_w2 \
	             --include=../conv2d_w3 \
	             --include=../max_pooling2d_w4 \
	             --include=../conv2d_w5 \
	             --include=../dense_w7 \
	             --include=../wts_init

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${DSPLIB_OPTS} --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

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
	x86simulator |& tee -a log

profile:
	aiesimulator --online -wdb -text --profile |& tee -a log

trace:
	aiesimulator --online -wdb -text |& tee -a log

regression:
	matlab -batch regression

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

throughput_ok:
	../../python/check_throughput.py PLIO_o 2.49 5.0

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir .wsdata vfs_work _ide .ipynb_checkpoints

