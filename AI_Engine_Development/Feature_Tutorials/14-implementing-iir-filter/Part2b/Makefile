#################################################################
# Support
# GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).
#
# License
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
# You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )
#
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
#
# Copyright 2022 Xilinx, Inc.
#################################################################

# AIE compiler tools
AIE_CC = aiecompiler
AIE_GRAPH_FILES = ../src/tb.cpp
AIE_OUT_DIR = ./Work
RMDIR = rm -rf

# AIE compiler settings
AIE_FLAGS = -v -Xchess=main:darts.xargs=-nb -include=$(XILINX_VITIS)/aietools/include -include=$(XILINX_HLS)/include -include="../" -include="../src" -include="../data" -target=hw -platform=$(PLATFORM_REPO_PATHS)/xilinx_vck190_base_202120_1/xilinx_vck190_base_202120_1.xpfm -workdir=$(AIE_OUT_DIR)
#
# primary build targets
#

all: ./src/kernel.cpp ./src/kernel.hpp ./src/tb.cpp ./src/graph.hpp ./src/C1_e.h ./src/C1_o.h
	echo && \
	echo "Building..." && \
	echo && \
	mkdir -p Emulation-AIE && \
	cd Emulation-AIE && \
	$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES) | tee build.log && \
	echo && \
	echo "Running..." && \
	echo && \
	aiesimulator --pkg-dir=./Work --i=.. --profile | tee aiesim.log
	# perform check if Julia is available
	echo && \
	if [ -e $(shell which julia) ] ; \
	then \
		echo ; \
		julia check.jl aie ; \
	fi

clean:
	$(RMDIR) Emulation-AIE

.PHONY: all clean

.PHONY: incremental
incremental: clean all
