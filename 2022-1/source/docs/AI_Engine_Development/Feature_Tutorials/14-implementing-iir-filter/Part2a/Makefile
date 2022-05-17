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
AIE_OUT_DIR = ./Work
AIE_GRAPH_FILES = ../src/tb.cpp
RMDIR = rm -rf

PLATFORM_VERSION = 202210_1
PLATFORM = $(PLATFORM_REPO_PATHS)/xilinx_vck190_base_$(PLATFORM_VERSION)/xilinx_vck190_base_$(PLATFORM_VERSION).xpfm

# AIE compiler settings
AIE_FLAGS = -v -Xchess=main:darts.xargs=-nb -include=$(XILINX_VITIS)/aietools/include -include=$(XILINX_HLS)/include -include="../" -include="../src" -include="../data" --enable-ecc-scrubbing=false -target=hw   -platform=$(PLATFORM) -workdir=$(AIE_OUT_DIR)

JULIA_AVAILABLE := $(shell command -v julia 2> /dev/null)

# primary build targets
aiesim: ./src/kernel.cpp ./src/kernel.hpp ./src/tb.cpp ./src/graph.hpp ./src/C1.h
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
	@echo
	@echo "Checking if Julia is available..."
	
ifdef JULIA_AVAILABLE
	@eval "julia check.jl aie"
else
	@echo "  Julia not found. Skipping comparison with reference."
endif

	@echo
	@echo "Done!"
	@echo

all: aiesim

clean:
	$(RMDIR) Emulation-AIE

.PHONY: clean incremental
incremental: clean all

