#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#



# AIE compiler tools
AIE_TOP_DIR = ..
AIE_BUILD_DIR = .
AIE_CC = v++ --compile --mode aie

AIE_GRAPH_FILES = ${AIE_TOP_DIR}/aie/SSRgraph.cpp
AIE_OUT_DIR = ${AIE_BUILD_DIR}/Work
AIE_DATA_DIR = ${AIE_BUILD_DIR}/data
AIE_HW_DIR = ${AIE_BUILD_DIR}/hw
RM = rm -f
RMDIR = rm -rf
PLATFORM ?= $(PLATFORM_REPO_PATHS)/$(PFM_NAME)/$(PFM_NAME).xpfm

# AIE compiler settings
AIE_FLAGS = --aie.verbose --include "${XILINX_VITIS}/aietools/include" --include "${AIE_TOP_DIR}/" --include "${AIE_TOP_DIR}/aie" --include "${AIE_TOP_DIR}/data" --target hw   --platform $(PLATFORM) --aie.workdir $(AIE_OUT_DIR)

AIE_FLAGS += --aie.Xrouter dmaFIFOsInFreeBankOnly

# Sources
SRCS = ${AIE_TOP_DIR}/aie/aie_kernels/FirSingleStream.cpp ${AIE_TOP_DIR}/aie/aie_kernels/FirSingleStream.h ${AIE_TOP_DIR}/aie/SSRgraph.cpp  ${AIE_TOP_DIR}/aie/SSRgraph.h  ${AIE_TOP_DIR}/aie/system_settings.h  ${AIE_TOP_DIR}/aie/aie_kernels.h


#
# primary build targets
#

all: clean aie aiesim aieviz

aie:   $(SRCS)
	-$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES)

aiesim:
	if [ -d data ]; then echo "data exists"; else ln -s ../data data; fi;
	aiesimulator --pkg-dir=Work --dump-vcd=foo

aieviz:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	$(RMDIR) $(AIE_OUT_DIR) $(AIE_DATA_DIR) $(AIE_TR_DATA_DIR) $(AIE_TRACE_DIR) ${AIE_BUILD_DIR}/aiesim* ${AIE_BUILD_DIR}/ess* ${AIE_BUILD_DIR}/profile* ${AIE_BUILD_DIR}/xnwOut ${AIE_BUILD_DIR}/.Xil
	find ./ -type f | grep -v Makefile | xargs rm -Rf

.PHONY: all clean aie aiesim aieviz




.PHONY: incremental
incremental: clean all
