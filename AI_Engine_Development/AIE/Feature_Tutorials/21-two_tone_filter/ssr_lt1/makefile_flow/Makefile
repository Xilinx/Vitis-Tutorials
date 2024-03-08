# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

PLATFORM_VER	:= xilinx_vck190_base_202410_1
PLATFORM		:= platform=${PLATFORM_REPO_PATHS}/${PLATFORM_VER}/${PLATFORM_VER}.xpfm
PROJECT_PATH	:= $(shell pwd)
SIM_OUTPUT  	:= aiesimulator_output
LOG_FILE    	:= log
FIR1_APP		:= fir1_app.cpp
AIE_OUTPUT		:= libadf.a
SRC_PATH 		:= src
TARGET			:= hw
WORK_AIESIM		:= ${PROJECT_PATH}/build/hw/Work
WORK_X86SIM		:= ${PROJECT_PATH}/build/x86sim/Work

DSPLIB_INCLUDE 	:= --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/src/aie

define genConfig
${PLATFORM}
work_dir=${PROJECT_PATH}/build/$(TARGET_AIE)/Work
include=${PROJECT_PATH}/src
include=${PROJECT_PATH}/data
input_files=${PROJECT_PATH}/src/fir1_app.cpp
target=$(TARGET_AIE)
[aie]
Xchess=-std=c++17
xlopt=1
pl-freq=500
Xmapper=BufferOptLevel7

endef

gen_file: target1 target2
target1: export TARGET_AIE=x86sim
target1: export genConfig:=$(genConfig)
target1:
	@echo "$${genConfig}" > aie_config_x86sim.cfg

target2: export TARGET_AIE=hw
target2: export genConfig:=$(genConfig)
target2:
	@echo "$${genConfig}" > aie_config_hw.cfg

.PHONY: help clean all

help:
	echo "Makefile Usage:"
	echo "  make all"
	echo "      Command to generate everything for this design"
	echo ""
	echo "  make aiecompile TARGET=x86sim|hw"
	echo ""
	echo "  make aiecompile TARGET=x86sim"
	echo "      Run AIE compiler and build the design for x86sim as target"
	echo ""
	echo "  make aiecompile TARGET=hw"
	echo "      Run AIE compiler and build the design for aiesim as target"
	echo ""
	echo "  make x86sim"
	echo "      Run the x86simulator for the target as x86sim compiled design"
	echo ""
	echo "  make aiesim"
	echo "      Run the aiesimulator for the target as aiesim compiled design"
	echo ""
	echo "  make profile"
	echo "      Run AIE simulator with profiling and generate outputs suitable for Vitis Analyzer"
	echo ""
	echo "  make analyze"
	echo "      Run Vitis Analyzer to inspect results"
	echo ""
	echo "  make throughput"
	echo "      Evaluate the achieved throughput at the output"
	echo ""
	echo "  make clean"
	echo "      Command to remove all the generated files."

all: genConfigFile ${AIE_OUTPUT} profile throughput

aiecompile: genConfigFile ${AIE_OUTPUT}

aiecom_sim: genConfigFile ${AIE_OUTPUT} aiesim

genConfigFile: gen_file

${AIE_OUTPUT}: ${SRC_PATH}/fir1_graph.h ${SRC_PATH}/${FIR1_APP}

ifeq ($(TARGET), x86sim)
	v++ -c --mode aie --config aie_config_x86sim.cfg ${DSPLIB_INCLUDE}
else ifeq ($(TARGET), hw)
	v++ -c --mode aie --config aie_config_hw.cfg ${DSPLIB_INCLUDE}
endif

x86sim:
	x86simulator --pkg-dir=${WORK_X86SIM} --dump |& tee -a ${LOG_FILE}

aiesim:
	aiesimulator --pkg-dir=${WORK_AIESIM} -o ${SIM_OUTPUT} |& tee -a ${LOG_FILE}

profile:
	aiesimulator --pkg-dir=${WORK_AIESIM} -o ${SIM_OUTPUT} --profile --online -wdb -ctf |& tee -a ${LOG_FILE}

throughput:
	@../../scripts/throughput.py aiesimulator_output/data/fir_o.txt --iscomplex
	@../../scripts/throughput.py aiesimulator_output/data/fft_o.txt --iscomplex

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	rm -rf build build* libadf.a vitis_design_wrapper*
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf x86simulator_output
	rm -rf log log* ${LOG_FILE}
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* function_wdb_dir
	rm -rf temp ISS_RPC_SERVER_PORT
	rm -rf system*.* .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
