# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


include ../common.mk

$(info "-------------------------------------------------------------------------")
$(info "AI Engine Build")
$(info "-------------------------------------------------------------------------")
$(info "Number of Antenna      : $(NAntenna)")
$(info "Multirate Enabled      : $(MULTIRATE)")
$(info "Number of Iterations   : $(NIterations)")
$(info "Width of PLIO I/F      : $(PLIOW)")
$(info "Data Width (Nb Bits)   : $(DATAW)")
$(info "Data Frame Length      : $(FRAME_LENGTH)")
$(info "Filter Margin Length   : $(MARGIN_LENGTH)")
$(info "PL Kernel Frequency    : $(FREQ_HZ)")
$(info "Ready for Hw/Sw Tracing: $(ENABLE_TRACE)")
$(info "Ready for Sim Profiling: $(ENABLE_PROFILE)")
$(info "-------------------------------------------------------------------------")

# Event trace: Simulation and Hardware
ifeq ($(strip $(ARCH)),aie)
	ARCH_AIECMP := 10
else ifeq ($(strip $(ARCH)),aie-ml)
	ARCH_AIECMP := 20
else
	$(error "ARCH should be either aie for AI Engine or aie-ml for AI Engine ML   (ARCH = $(ARCH))")
endif



SYSTEM_VAR :=  NAntenna=$(NAntenna) MULTIRATE=$(MULTIRATE) NIterations=$(NIterations) PLIOW=$(PLIOW) DATAW=$(DATAW) FREQ_HZ=$(FREQ_HZ) FRAME_LENGTH=$(FRAME_LENGTH) MARGIN_LENGTH=$(MARGIN_LENGTH)

SYSTEM_VAR_D := -DNAntenna=$(NAntenna) -DMULTIRATE=$(MULTIRATE) -DNIterations=$(NIterations) -DPLIOW=$(PLIOW) -DFREQ_HZ=$(FREQ_HZ) -DFRAME_LENGTH=$(FRAME_LENGTH) -DMARGIN_LENGTH=$(MARGIN_LENGTH) -DARCH_AIECMP=$(ARCH_AIECMP)

GRAPH    = src/graph.cpp
LIBADF  = libadf.a
AIE_CMPL_CMD := v++ --compile --mode aie --include "./src" --include "./src/kernels" --aie.workdir ./Work ${GRAPH} --platform $(PLATFORM)
AIE_FLAGS :=  --aie.log-level 5
AIE_FLAGS += --aie.Xpreproc "\"$(SYSTEM_VAR_D)\""
AIE_FLAGS += --aie.output-archive $(LIBADF)

AIE_SIM_CMD := aiesimulator --pkg-dir=./Work

# Event trace: Simulation and Hardware
ifeq ($(strip $(ENABLE_TRACE)),true)
	AIE_FLAGS += --config aiecompiler_trace_$(TRACE_CHANNEL)_options.cfg
	AIE_SIM_CMD += --dump-vcd foo
	AIE_SIM_CMD +=  --options-file=aiesim-options.txt
endif

# Profiling
ifeq ($(strip $(ENABLE_PROFILE)),true)
	AIE_SIM_CMD += --profile
endif


all: clean data aie aiesim simviz

run: clean data aie aiesim


.PHONY: data help _help

NSamples := $(shell echo " 2 * 256 * $(NIterations)" | bc)
disp:
	@echo $(NSamples)

data:
	mkdir -p data
	echo $(PWD)
	echo $(ROOTDIR)
	$(UTILS_DIR)/CreateLinearData.py int16 $(PLIOW) $(NSamples)  > data/input.txt
	cp -rf data $(ROOTDIR)

aie: ${LIBADF}
${LIBADF}: src/*
	$(AIE_CMPL_CMD) $(AIE_FLAGS)

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

events:
	vcdanalyze  -v --pkg-dir ./Work --vcd foo.vcd

extract:
	grep Events < trdata.aiesim/events.txt


aieviz: $(LIBADF)
	vitis_analyzer ./Work/graph.aiecompile_summary

simviz: $(LIBADF)
	vitis_analyzer aiesimulator_output/default.aierun_summary

help:
	make -s _help

_help:
	echo -e "\n\n===========================================\n"
	echo "Targeted Architectures (Can be modified in common.mk):"
	echo "ARCH=aie: AI Engine (default)"
	echo "ARCH=aie-ml: AI Engine ML"
	echo " "
	echo -e "\n===========================================\n"


clean:
	rm -rf _x libadf.a Work .Xil vitis_analyzer* foo.vcd *.log pl_sample* aiesimulator_output
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db temp
	rm -rf  logs trdata.aiesim ISS_RPC_SERVER_PORT .wsdata plio_throughput_info.json
	rm -rf

clean-all:
	make clean
	rm -rf data
