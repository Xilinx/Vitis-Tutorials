# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

CASE ?= 1
ARCH ?= aie

ifeq ($(ARCH), aie)
PFM_NAME := xilinx_vck190_base_202410_1
$(info "ARCH set for AI Engine Architecture   (ARCH = $(ARCH))")
else ifeq ($(ARCH), aie-ml)
PFM_NAME := xilinx_vek280_base_202410_1
$(info "ARCH set for AI Engine ML Architecture   (ARCH = $(ARCH))")
else
$(error "ARCH should be either aie for AI Engine or aie-ml for AI Engine ML   (ARCH = $(ARCH))")
endif

PFM_NAME := $(strip $(PFM_NAME))
PLATFORM := ${PLATFORM_REPO_PATHS}/${PFM_NAME}/${PFM_NAME}.xpfm

GRAPH    = src_$(CASE)/graph.cpp
LIBADF  = libadf_$(CASE).a
AIE_CMPL_CMD = v++ --compile --mode aie --include "./src_$(CASE)" --aie.workdir ./Work_$(CASE) ${GRAPH} --platform=$(PLATFORM) --aie.output-archive $(LIBADF)
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work_$(CASE) --dump-vcd foo

all: clean data aie aieviz

run: clean data aie aiesim


.PHONY: data help _help

data:
	mkdir -p data
	CreateLinearData.py int16 32 4096 > data/input0.txt
	CreateLinearData.py int16 32 4096 > data/input1.txt
	CreateLinearData.py int16 32 4096 > data/input2.txt
	CreateLinearData.py int16 32 4096 > data/input3.txt
	CreateLinearData.py int16 32 4096 > data/input.txt


aie: ${LIBADF}
${LIBADF}: src_$(CASE)/*
	${AIE_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

aieviz: $(LIBADF)
	vitis_analyzer ./Work_$(CASE)/graph.aiecompile_summary

help:
	make -s _help

_help:
	echo -e "\n\n===========================================\n"
	echo "Targeted Architectures:"
	echo "ARCH=1: AI Engine (default)"
	echo "ARCH=2: AI Engine ML"
	echo " "
	echo "Example list:"
	echo "CASE=1: conditional Cascade port  (default)"
	echo "CASE=2: conditional array of SubGraphs"
	echo "CASE=3: conditional sequential SubGraph"
	echo "CASE=4: conditional RTP ports"
	echo -e "\n===========================================\n"


clean:
	rm -rf _x libadf*.a Work_* .Xil vitis_analyzer* foo.vcd *.log pl_sample* aiesimulator_output
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db temp
	rm -rf data logs
