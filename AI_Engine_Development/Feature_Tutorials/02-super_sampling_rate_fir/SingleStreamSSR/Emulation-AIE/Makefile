#
# Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# AIE compiler tools
AIE_TOP_DIR = ..
AIE_BUILD_DIR = .
AIE_CC = aiecompiler

AIE_GRAPH_FILES = ${AIE_TOP_DIR}/src/SSRgraph.cpp
AIE_OUT_DIR = ${AIE_BUILD_DIR}/Work
AIE_DATA_DIR = ${AIE_BUILD_DIR}/data
AIE_HW_DIR = ${AIE_BUILD_DIR}/hw
AIE_TR_DATA_DIR = ${AIE_BUILD_DIR}/.tracecompass/.tracing
AIE_TRACE_DIR = ${AIE_BUILD_DIR}/Traces
RM = rm -f
RMDIR = rm -rf
SRC_LOG_FILE_PATH = ${AIE_TOP_DIR}/Module2.build.log
DST_LOG_FILE_PATH = ${AIE_BUILD_DIR}/build.log
PLATFORM = xilinx_vck190_es1_base_202020_1

# AIE compiler settings
AIE_FLAGS = -v -dataflow -include="${XILINX_VITIS_AIE_TOOLS}/include" -include="${XILINX_VITIS_AIE_TOOLS}/../include" -include="${AIE_TOP_DIR}/" -include="${AIE_TOP_DIR}/src" -include="${AIE_TOP_DIR}/data" -target=hw   -platform=${PLATFORM_REPO_PATHS}/${PLATFORM}/${PLATFORM}.xpfm -workdir=$(AIE_OUT_DIR)

# Sources
SRCS = ${AIE_TOP_DIR}/src/aie_kernels/FirSingleStream.cpp ${AIE_TOP_DIR}/src/aie_kernels/FirSingleStream.h ${AIE_TOP_DIR}/src/SSRgraph.cpp  ${AIE_TOP_DIR}/src/SSRgraph.h  ${AIE_TOP_DIR}/src/system_settings.h  ${AIE_TOP_DIR}/src/aie_kernels.h


#
# primary build targets
#

all: clean compile simulate visualize

compile:   $(SRCS)
	-$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES)
	if [ -f $(SRC_LOG_FILE_PATH) ]; then cp $(SRC_LOG_FILE_PATH) $(DST_LOG_FILE_PATH) 2>/dev/null; echo; echo "INFO: Look at the log file Debug/build.log for more details."; else echo "Warning: Build log file $(SRC_LOG_FILE_PATH) doesn't exist."; fi; \

simulate:
	if [ -d data ]; then echo "data exists"; else ln -s ../data data; fi;
	aiesimulator --pkg-dir=Work --dump-vcd=foo

visualize:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	$(RMDIR) $(AIE_OUT_DIR) $(AIE_DATA_DIR) $(AIE_TR_DATA_DIR) $(AIE_TRACE_DIR) ${AIE_BUILD_DIR}/aiesim* ${AIE_BUILD_DIR}/ess* ${AIE_BUILD_DIR}/profile* ${AIE_BUILD_DIR}/xnwOut ${AIE_BUILD_DIR}/.Xil
	find ./ -type f | grep -v Makefile | xargs rm -Rf

.PHONY: all clean compile  simulate visualize




.PHONY: incremental
incremental: clean all
