CURRENT_DIR = $(shell pwd)
WKSPC_DIR = $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# AIE compiler tools
AIE_SRC_DIR = $(shell pwd)/src
AIE_CC = aiecompiler
AIE_GRAPH_FILES = ../src/tb.cpp

AIE_OUT_DIR = ./Work
AIE_DATA_DIR = ./data
RM = rm -f
RMDIR = rm -rf

PLATFORM = $(PLATFORM_REPO_PATHS)/xilinx_vck190_base_202120_1/xilinx_vck190_base_202120_1.xpfm

# AIE compiler settings
AIE_FLAGS = -v -Xchess=main:darts.xargs=-nb -include=$(XILINX_VITIS)/aietools/include -include=$(XILINX_HLS)/include -include="../" -include="../src" -include="../data" -target=x86sim -platform=$(PLATFORM) -workdir=$(AIE_OUT_DIR)
#
# primary build targets
#

all:./Work/pthread/sim.out

./Work/pthread/sim.out:  $(AIE_SRC_DIR)/kernel.cpp
	mkdir -p Emulation-SW && \
	cd Emulation-SW && \
	$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES) | tee build.log && \
	x86simulator --pkg-dir=./Work --i=..

clean:
	rm -rf Emulation-SW

.PHONY: all clean

.PHONY: incremental
incremental: clean all
