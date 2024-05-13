# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


VERSION ?= 1
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

GRAPH    = src/graph.cpp
LIBADFAIE  = libadfaie.a
LIBADFX86  = libadfx86.a
AIE_CMPL_CMD = v++ --compile --mode aie --include "./src" --aie.workdir ./WorkAie ${GRAPH} --platform=$(PLATFORM) --aie.output-archive $(LIBADFAIE) --aie.Xpreproc "-DGV=$(VERSION)"
X86_CMPL_CMD = v++ --compile --mode aie --target x86sim --include "./src" --aie.workdir ./WorkX86 ${GRAPH} --platform $(PLATFORM) --aie.output-archive $(LIBADFX86)  --aie.Xpreproc "-DGV=$(VERSION)"

AIE_SIM_CMD = aiesimulator --pkg-dir=./WorkAie --dump-vcd foo
# AIE_SIM_CMD += --hang-detect-time=10000

X86_SIM_CMD = x86simulator --pkg-dir=./WorkX86 --trace --trace-print

BUFFER_vs_STREAM ?= 1

.PHONY: data buffer stream evalfifo disptypeio dispfifo disprepetition

run:
	make clean data aie aiesim

data:
	mkdir -p data
	CreateData.py 2 100000 1 100000 > data/Input_64.txt

disprepetition:
	DisplayRepetition

dispfifo:
	if [ -f ./AIESim_FIFO_Guidance.json ]; then more ./AIESim_FIFO_Guidance.json ; fi


disptypeio:
	echo "Interface chosen $(BUFFER_vs_STREAM) : 1-> Buffer    2-> Stream"

buffer:
	$(eval BUFFER_vs_STREAM := 1)
	echo "Interface chosen: Buffer"


stream:
	$(eval BUFFER_vs_STREAM := 2)
	echo "Interface chosen: Stream"

evalfifo:
	$(eval AIE_CMPL_CMD += --evaluate-fifo-depth)
	echo "AIE compiler set to evluate FIFO sizes"

aie: ${LIBADFAIE}
${LIBADFAIE}: src/*
	${AIE_CMPL_CMD} --aie.Xpreproc "-DBUFFER_vs_STREAM=$(BUFFER_vs_STREAM)"
	make disprepetition




x86:${LIBADFX86}
${LIBADFX86}: src/*
	${X86_CMPL_CMD} --aie.Xpreproc "-DBUFFER_vs_STREAM=$(BUFFER_vs_STREAM)"
	DisplayRepetition

aiesim: ${LIBADFAIE}
	${AIE_SIM_CMD}
	make dispfifo

STOP ?= 50000

aiesim_stop: ${LIBADFAIE}
	${AIE_SIM_CMD}  --simulation-cycle-timeout=$(STOP)
	make dispfifo


aieviz:
	vitis_analyzer aiesimulator_output/default.aierun_summary


x86sim: ${LIBADFX86}
	${X86_SIM_CMD}

aiecmp:
	make aiecmp1
	make aiecmp2

aiecmp1:
	grep -v T aiesimulator_output/data/OutputPG_V1.txt > PG1.txt
	grep -v T aiesimulator_output/data/OutputPG_V2.txt > PG2.txt
	grep -v T aiesimulator_output/data/OutputPG_V3.txt > PG3.txt
	diff PG1.txt PG2.txt
	diff PG1.txt PG3.txt
	diff PG2.txt PG3.txt

aiecmp2:
	grep -v T aiesimulator_output/data/OutputFG_V1.txt > FG1.txt
	grep -v T aiesimulator_output/data/OutputFG_V2.txt > FG2.txt
	grep -v T aiesimulator_output/data/OutputFG_V3.txt > FG3.txt
	grep -v T aiesimulator_output/data/OutputFG_V4.txt > FG4.txt
	diff FG1.txt data/RefOutput_1filt.txt
	diff FG2.txt data/RefOutput_1filt.txt
	diff FG3.txt data/RefOutput_2filt.txt
	diff FG4.txt data/RefOutput_async.txt

x86cmp:
	make x86cmp1
	make x86cmp2

x86cmp1:
	grep -v T x86simulator_output/data/OutputPG_V1.txt > PG1.txt
	grep -v T x86simulator_output/data/OutputPG_V2.txt > PG2.txt
	grep -v T x86simulator_output/data/OutputPG_V3.txt > PG3.txt
	diff PG1.txt PG2.txt
	diff PG1.txt PG3.txt
	diff PG2.txt PG3.txt

x86cmp2:
	grep -v T x86simulator_output/data/OutputFG_V1.txt > FG1.txt
	grep -v T x86simulator_output/data/OutputFG_V2.txt > FG2.txt
	grep -v T x86simulator_output/data/OutputFG_V3.txt > FG3.txt
	grep -v T x86simulator_output/data/OutputFG_V4.txt > FG4.txt
	diff FG1.txt data/RefOutput_1filt.txt
	diff FG2.txt data/RefOutput_1filt.txt
	diff FG3.txt data/RefOutput_2filt.txt
	diff FG4.txt data/RefOutput_async.txt


clean:
	rm -rf _x ${LIBADFAIE} ${LIBADFX86} WorkAie WorkX86 .Xil vitis_analyzer* foo.vcd *.log pl_sample*
	rm -rf aiesimulator_output x86simulator_output vitis_design_wrapper*
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db temp *.txt
	rm -rf  logs
	find *.json | grep -v "description.json" | xargs rm -f




clean_all: clean
	rm -rf data
