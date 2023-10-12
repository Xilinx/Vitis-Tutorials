#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#

TARGET   = hw_emu
HOST_EXE = host.exe

# Default values for A, B, C matrix sizes
# A:MxK    B:KxN    C:MxN
sizeM ?= 64
sizeK ?= 64
sizeN ?= 64

# Default for A, B and C sub matrices
# 4x16x8
subM ?= 4
subK ?= 16
subN ?= 8

#Default Number of iterations
NIterations ?= 4

# Width of the PLIOs
PLIOW ?= 128

# Optimization
OPT ?= 0


SIZES := sizeM=$(sizeM) sizeK=$(sizeK) sizeN=$(sizeN) subM=$(subM) subK=$(subK) subN=$(subN) NIterations=$(NIterations) PLIOW=$(PLIOW) OPT=$(OPT)
SIZES_D := -DsizeM=$(sizeM) -DsizeK=$(sizeK) -DsizeN=$(sizeN) -DsubM=$(subM) -DsubK=$(subK) -DsubN=$(subN) -DNIterations=$(NIterations) -DPLIOW=$(PLIOW) -DOPTIMIZED_SOURCE=$(OPT)

WORKDIR := ./Work$(OPT)
AIEOUTDIR := aiesimulator_output$(OPT)
X86OUTDIR := x86simulator_output$(OPT)


GRAPH    = src/graph.cpp
LIBADF  = libadf$(OPT).a
AIE_CMPL_CMD := aiecompiler -include="./src" --workdir=$(WORKDIR) ${GRAPH}
AIE_FLAGS += -v -log-level=5
AIE_FLAGS += --Xpreproc="$(SIZES_D)"
AIE_FLAGS += --Xchess="ClassicMatMult:backend.mist2.xargs=+Omodo"
AIE_FLAGS += --output-archive=$(LIBADF)

ifdef PLATFORM
AIE_FLAGS += --platform=$(PLATFORM)
endif


AIE_SIM_CMD := aiesimulator --pkg-dir=$(WORKDIR)
AIE_SIM_CMD +=  --dump-vcd foo --profile
AIE_SIM_CMD +=  --output-dir=$(AIEOUTDIR)
# AIE_SIM_CMD += --simulation-cycle-timeout=50000


X86_SIM_CMD := x86simulator --pkg-dir=$(WORKDIR) --output-dir=$(X86OUTDIR) --trace

all: ${XSA} ${HOST_EXE} package
run: all run_hw_emu
sd_card: all

.PHONY: data

data:
	if [ ! -d "data" ]; then mkdir data ; fi
	../Utils/GenerateMatrices.py $(sizeM) $(sizeK) $(sizeN) $(subM) $(subK) $(subN) 8 $(NIterations) 1 data $(PLIOW)

all_aie: clean data aie aiesim

all_x86: clean data x86 x86sim

aie: ${LIBADF}
${LIBADF}: src/*
	${AIE_CMPL_CMD} ${AIE_FLAGS}

x86: src/*
	${AIE_CMPL_CMD} --target=x86sim ${AIE_FLAGS}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

x86sim:
	${X86_SIM_CMD}

aieviz:
	vitis_analyzer $(AIEOUTDIR)/default.aierun_summary

compareaie:
	grep -v T $(AIEOUTDIR)/data/outputCns_${PLIOW}_32b.txt > $(AIEOUTDIR)/data/outuintCns_$(PLIOW)_32b.txt
	../Utils/ChangeType.py $(AIEOUTDIR)/data/outuintCns_$(PLIOW)_32b.txt $(AIEOUTDIR)/data/outintCns_$(PLIOW)_32b.txt
	../Utils/CompareOutput.py data/outputC_ref_$(PLIOW)_32b.txt $(AIEOUTDIR)/data/outintCns_$(PLIOW)_32b.txt
	grep -v T $(AIEOUTDIR)/data/outputCns_${PLIOW}_16b.txt > $(AIEOUTDIR)/data/outuintCns_$(PLIOW)_16b.txt
	../Utils/ChangeType.py $(AIEOUTDIR)/data/outuintCns_$(PLIOW)_16b.txt $(AIEOUTDIR)/data/outintCns_$(PLIOW)_16b.txt
	../Utils/CompareOutput.py data/outputC_ref_$(PLIOW)_16b.txt $(AIEOUTDIR)/data/outintCns_$(PLIOW)_16b.txt

comparex86:
	grep -v T $(X86OUTDIR)/data/outputCns_${PLIOW}_32b.txt > $(X86OUTDIR)/data/outuintCns_$(PLIOW)_32b.txt
	../Utils/ChangeType.py $(X86OUTDIR)/data/outuintCns_$(PLIOW)_32b.txt $(X86OUTDIR)/data/outintCns_$(PLIOW)_32b.txt
	../Utils/CompareOutput.py data/outputC_ref_$(PLIOW)_32b.txt $(X86OUTDIR)/data/outintCns_$(PLIOW)_32b.txt
	grep -v T $(X86OUTDIR)/data/outputCns_${PLIOW}_16b.txt > $(X86OUTDIR)/data/outuintCns_$(PLIOW)_16b.txt
	../Utils/ChangeType.py $(X86OUTDIR)/data/outuintCns_$(PLIOW)_16b.txt $(X86OUTDIR)/data/outintCns_$(PLIOW)_16b.txt
	../Utils/CompareOutput.py data/outputC_ref_$(PLIOW)_16b.txt $(X86OUTDIR)/data/outintCns_$(PLIOW)_16b.txt


clean:
	rm -rf _x libadf*.a Work* .Xil vitis_analyzer* foo.vcd *.log
	rm -rf pl_sample* aiesimulator_output*
	rm -rf x86simulator_output* output_data.txt
	rm -rf temp
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db *.soln
	rm -rf ISS_RPC_SERVER_PORT *.wdb *.wcfg trdata.aiesim
	rm -rf data logs

clean-all:
	make clean
