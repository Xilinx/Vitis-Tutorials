#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#



# AIE compiler tools
AIE_TOP_DIR = ..
AIE_BUILD_DIR = .
AIE_CC = aiecompiler

AIE_GRAPH_FILES = ${AIE_TOP_DIR}/aie/SSRgraph.cpp
AIE_OUT_DIR = ${AIE_BUILD_DIR}/Work
AIE_DATA_DIR = ${AIE_BUILD_DIR}/data
AIE_HW_DIR = ${AIE_BUILD_DIR}/hw
AIE_TR_DATA_DIR = ${AIE_BUILD_DIR}/.tracecompass/.tracing
AIE_TRACE_DIR = ${AIE_BUILD_DIR}/Traces
RM = rm -f
RMDIR = rm -rf
SRC_LOG_FILE_PATH = ${AIE_TOP_DIR}/Module2.build.log
DST_LOG_FILE_PATH = ${AIE_BUILD_DIR}/build.log
PLATFORM := $(PFM_NAME)

# AIE compiler settings
AIE_FLAGS = -v -include="${XILINX_VITIS}/aietools/include" -include="${AIE_TOP_DIR}/" -include="${AIE_TOP_DIR}/aie" -include="${AIE_TOP_DIR}/data" -target=hw   -platform ${PLATFORM_REPO_PATHS}/${PLATFORM}/${PLATFORM}.xpfm -workdir=$(AIE_OUT_DIR)

AIE_FLAGS += --Xrouter=dmaFIFOsInFreeBankOnly

# Sources
SRCS = ${AIE_TOP_DIR}/aie/aie_kernels/FirSingleStream.cpp ${AIE_TOP_DIR}/aie/aie_kernels/FirSingleStream.h ${AIE_TOP_DIR}/aie/SSRgraph.cpp  ${AIE_TOP_DIR}/aie/SSRgraph.h  ${AIE_TOP_DIR}/aie/system_settings.h  ${AIE_TOP_DIR}/aie/aie_kernels.h


#
# primary build targets
#

all: clean aie aiesim aieviz

aie:   $(SRCS)
	-$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES)
	if [ -f $(SRC_LOG_FILE_PATH) ]; then cp $(SRC_LOG_FILE_PATH) $(DST_LOG_FILE_PATH) 2>/dev/null; echo; echo "INFO: Look at the log file Debug/build.log for more details."; else echo "Warning: Build log file $(SRC_LOG_FILE_PATH) doesn't exist."; fi; \

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
