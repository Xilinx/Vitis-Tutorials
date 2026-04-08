#
# Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#



# AIE compiler tools
AIE_TOP_DIR = .
AIE_BUILD_DIR = .
AIE_CC = v++ --compile --mode aie
UTILS := $(PWD)/../Utils

AIE_GRAPH_FILES = ${AIE_TOP_DIR}/aie/SSRgraph.cpp
AIE_OUT_DIR = ${AIE_BUILD_DIR}/Work
AIE_DATA_DIR = ${AIE_BUILD_DIR}/data
AIE_HW_DIR = ${AIE_BUILD_DIR}/hw
RM = rm -f
RMDIR = rm -rf

PFM_NAME ?= xilinx_vck190_base_202520_1
PLATFORM ?= $(PLATFORM_REPO_PATHS)/$(PFM_NAME)/$(PFM_NAME).xpfm

# AIE compiler settings
AIE_FLAGS = --aie.verbose --include "${XILINX_VITIS}/aietools/include"  --include "${AIE_TOP_DIR}/" --include "${AIE_TOP_DIR}/aie" --include "${AIE_TOP_DIR}/data" --target hw   --platform ${PLATFORM} --aie.workdir $(AIE_OUT_DIR)

AIE_FLAGS += --aie.Xrouter dmaFIFOsInFreeBankOnly

# Sources
SRCS = ${AIE_TOP_DIR}/aie/aie_kernels/FirDoubleStream.cpp ${AIE_TOP_DIR}/aie/aie_kernels/FirDoubleStream.h ${AIE_TOP_DIR}/aie/SSRgraph.cpp  ${AIE_TOP_DIR}/aie/SSRgraph.h  ${AIE_TOP_DIR}/aie/system_settings.h  ${AIE_TOP_DIR}/aie/aie_kernels.h


#
# primary build targets
#

all: clean  data aie aiesim aieviz throughput waveform

run: clean data aie aiesim

NUM_SAMPLES ?= 512
NFRAMES ?= 4

SYSTEM_VAR_D := -DNUM_SAMPLES=$(NUM_SAMPLES) -DNFRAMES=$(NFRAMES)
SYSTEM_VAR_D_Preproc := $(foreach word,$(SYSTEM_VAR_D),--aie.Xpreproc $(word))

AIE_FLAGS += $(SYSTEM_VAR_D_Preproc)

NSamples := $(shell echo " $(NUM_SAMPLES) / 2" | bc)
NFrames := $(shell echo " $(NFRAMES) + 1" | bc)

data:
	rm -rf data
	mkdir -p data
	../Utils/GenerateStreams cint16 64 8 2 $(NSamples) $(NFrames) Dirac data/PhaseIn

aie:   $(SRCS)
	$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES)

aiesim: libadf.a
	aiesimulator --pkg-dir=Work --dump-vcd=foo

aieviz:
	vitis_analyzer aiesimulator_output/default.aierun_summary
	
throughput:
	cd aiesimulator_output/data/ && $(UTILS)/StreamThroughput PhaseOut_*.txt
	
waveform:
	cd aiesimulator_output/data/ && $(UTILS)/ProcessAIEOutput PhaseOut_*.txt

clean:
	$(RMDIR) $(AIE_OUT_DIR) $(AIE_DATA_DIR) *.vcd _ide libadf.a *.log *.json aiesimulator_output *.csv *.db pl_sampl* .Xil .AIE_SIM*

.PHONY: all run data clean  aie aiesim aieviz throughput waveform

.PHONY: incremental
incremental: clean all
