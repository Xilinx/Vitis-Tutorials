#
# This file was created by a Xilinx tool. trust it.
#

# AIE compiler tools
AIE_SRC_DIR = ..
AIE_CC = aiecompiler
AIE_GRAPH_FILES = ../src/fir_graph.cpp
AIE_OUT_DIR = ./Work
AIE_DATA_DIR = ./data
AIE_HW_DIR = ./hw
AIE_TRACE_DIR = ./Traces
RM = rm -f
RMDIR = rm -rf

# AIE compiler settings
AIE_FLAGS = -v -Xchess=main:darts.xargs=-nb \
	-include="$(XILINX_VITIS)/aietools/include" 	-include="$(XILINX_HLS)/include" \
	-include="$(DSPLIB_ROOT)/L1/src/aie" -include="$(DSPLIB_ROOT)/L1/include/aie" -include="$(DSPLIB_ROOT)/L2/include/aie" \
	-include="../data" -include="../src/graphs" -include="../src" \
	-target=hw   -platform=$(VITIS_PLATFORM_XPFM) -workdir=$(AIE_OUT_DIR)
#
# primary build targets
#

all:./libadf.a

./libadf.a:  ../src/fir_graph.cpp  ../src/graphs/fir_graph.h
	-$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES)

clean:
	$(RMDIR) $(AIE_OUT_DIR) $(AIE_DATA_DIR) $(AIE_TRACE_DIR) .Xil ./aiesim* ./ess* ./profile* ../data/out*
	find ./ -type f | grep -v makefile | xargs rm -Rf

.PHONY: all clean


.PHONY: incremental
incremental: all
