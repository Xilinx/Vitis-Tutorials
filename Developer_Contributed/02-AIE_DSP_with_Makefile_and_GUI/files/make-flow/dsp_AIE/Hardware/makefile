# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_AIE_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

show_dir:
		-@echo from dsp_AIE, RELATIVE PROJECT DIR is: $(RELATIVE_PROJECT_DIR)
		-@echo from dsp_AIE, CURRENT  PROJECT DIR is: $(PROJECT_AIE_DIR)


# AIE compiler tools
#AIE_SRC_DIR = .. #DB: old
AIE_SRC_DIR = $(PROJECT_AIE_DIR)
AIE_CC = aiecompiler
#AIE_GRAPH_FILES = ../src/fir_graph.cpp #DB: old
AIE_GRAPH_FILES = $(PROJECT_AIE_DIR)/src/fir_graph.cpp
#AIE_OUT_DIR = ./Work #DB: old
AIE_OUT_DIR = $(PROJECT_AIE_DIR)/Hardware/Work
#AIE_DATA_DIR = ./data #DB: old
AIE_DATA_DIR = $(PROJECT_AIE_DIR)/data
#AIE_HW_DIR = ./hw    #DB: never used?
#AIE_TRACE_DIR = ./Trace #DB: never used?
RM = rm -f
RMDIR = rm -rf

# AIE compiler settings
AIE_FLAGS = -v -Xchess=main:darts.xargs=-nb \
	-include="$(XILINX_VITIS)/aietools/include" 	-include="$(XILINX_HLS)/include" \
	-include="$(DSPLIB_ROOT)/L1/src/aie" -include="$(DSPLIB_ROOT)/L1/include/aie" -include="$(DSPLIB_ROOT)/L2/include/aie" \
	-include="$(PROJECT_AIE_DIR)/data" -include="$(PROJECT_AIE_DIR)/src/graphs" -include="$(PROJECT_AIE_DIR)/src/" \
	-target=hw   -platform=$(VITIS_PLATFORM_XPFM) -workdir=$(AIE_OUT_DIR)
#
# primary build targets
#


all:	show_dir	./libadf.a

./libadf.a:  $(PROJECT_AIE_DIR)/src/fir_graph.cpp  $(PROJECT_AIE_DIR)/src/graphs/fir_graph.h
	-$(AIE_CC) $(AIE_FLAGS) $(AIE_GRAPH_FILES)

clean:
	$(RMDIR) $(AIE_OUT_DIR) $(AIE_DATA_DIR)  .Xil ./aiesim* ./ess* ./profile* ../data/out*
	find ./ -type f | grep -v makefile | xargs rm -Rf

.PHONY: all clean


.PHONY: incremental
incremental: all
