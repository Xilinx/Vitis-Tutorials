#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz

SRC_DIR           := ./src
MY_SOURCES        := $(shell find $(SRC_DIR) -name *.cpp) $(shell find $(SRC_DIR) -name *.h)

PLATFORM_USE      := xilinx_vck190_base_202410_1

PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

COMPILE_OPTS      := --platform=${PLATFORM} 

X86_SIM_OPTS      := --pkg-dir=./Work --i=./ --o=./x86simulator_output
AIE_SIM_OPTS      := --pkg-dir=./Work --input-dir=./ --output-dir=./aiesimulator_output --display-run-interval=1000

.PHONY: help aiesim x86sim profile trace gen_vectors check_aiesim check_x86sim analyze clean

help::
	@echo "Makefile Usage:"
	@echo "  make aiecom"
	@echo "      Compile design for AIE emulation"
	@echo ""
	@echo "  make x86com"
	@echo "      Compile design for x86 simulation"
	@echo ""
	@echo "  make aiesim"
	@echo "      Run simulation using AIE emulation"
	@echo ""
	@echo "  make x86sim"
	@echo "      Run x86 simulation for functional verification"
	@echo ""
	@echo "  make check_x86sim"
	@echo "      Use MATLAB to compare simulation results with golden reference"
	@echo ""
	@echo "  make gen_vectors"
	@echo "      Use MATLAB to generate test vectors for AIE simulation"
	@echo ""
	@echo "  make check_aiesim"
	@echo "      Use MATLAB to compare AIE simulation with golden reference"
	@echo ""
	@echo "  make check_x86sim"
	@echo "      Use MATLAB to compare x86 simulation with golden reference"
	@echo ""
	@echo "  make clean"
	@echo "      Delete files generated during compilation and simulation"

all:       aiecom aiesim

aiecom:    ${MY_SOURCES}
	v++ -c --mode aie --config=aie.cfg --target=hw ${COMPILE_OPTS} ${SRC_DIR}/softmax_app.cpp

x86com:    ${MY_SOURCES}
	v++ -c --mode aie --config=aie.cfg --target=x86sim ${COMPILE_OPTS} ${SRC_DIR}/softmax_app.cpp

aiesim:
	aiesimulator ${AIE_SIM_OPTS}

x86sim:
	x86simulator ${X86_SIM_OPTS}

profile:
	aiesimulator ${AIE_SIM_OPTS} --dump-vcd=aiesim --profile

loop_ii:
	./get_loop_II.py ./Work/aie

gen_vectors:
	matlab -nodisplay -r "try; run('../matlab/genvectors_softmax_aie_float32.m'); catch; end; quit"

check_aiesim:
	matlab -nodisplay -r "try; run('../matlab/check_aiesim_aie_float32.m'); catch; end; quit"

check_x86sim:
	matlab -nodisplay -r "try; run('../matlab/check_x86sim_aie_float32.m'); catch; end; quit"

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	rm -rf .Xil Work libadf.a
	rm -rf *.log *.csv *.db *.vcd logs
	rm -rf x86simulator_output aiesimulator_output pl_sample_counts
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS ISS_RPC_SERVER_PORT
	rm -rf pl_sample_count* .wsdata
	rm -rf component_ide plio_throughput_info.json