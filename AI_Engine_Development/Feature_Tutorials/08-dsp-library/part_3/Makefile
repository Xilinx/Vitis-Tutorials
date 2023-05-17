# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

# Directories and Filenames
AIE_TOP_DIR := $(PWD)/aie
AIE_SRC_DIR := $(AIE_TOP_DIR)/src
AIE_OUT_DIR := $(AIE_TOP_DIR)/Work
AIE_DATA_DIR := $(PWD)/data


AIE_GRAPH_FILES := $(AIE_SRC_DIR)/test.cpp
PFM_NAME := xilinx_vck190_base_202310_1
PLATFORM := $(PLATFORM_REPO_PATHS)/$(PFM_NAME)/$(PFM_NAME).xpfm


# AIE compiler settings
AIE_FLAGS = -v
AIE_FLAGS += --include=$(XILINX_VITIS)/aietools/include --include=$(AIE_SRC_DIR) --include=$(AIE_DATA_DIR)
AIE_FLAGS += --platform=$(PLATFORM) --workdir=$(AIE_OUT_DIR)

X86_FLAGS := $(AIE_FLAGS)


#########  Add DSP include files location   #########
DSP_FLAGS := --include=$(DSPLIB_ROOT)/L1/src/aie
DSP_FLAGS += --include=$(DSPLIB_ROOT)/L1/include/aie
DSP_FLAGS += --include=$(DSPLIB_ROOT)/L2/include/aie



#
# primary build targets
#
.PHONY: all-aie all-x86 clean cleanall x86 aie x86sim aiesim

all-aie: clean aie aiesim

all-x86: clean x86 x86sim

x86:
	aiecompiler -target=x86sim $(X86_FLAGS) $(DSP_FLAGS) $(AIE_GRAPH_FILES)

aie:
	aiecompiler -target=hw $(AIE_FLAGS) $(DSP_FLAGS) $(AIE_GRAPH_FILES)


aiesim:
	aiesimulator --pkg-dir=$(AIE_OUT_DIR) --dump-vcd=sim

x86sim:
	x86simulator --pkg-dir=$(AIE_OUT_DIR)

compviz:
	vitis_analyzer $(AIE_OUT_DIR)/test.aiecompile_summary

aieviz:
	vitis_analyzer aiesimulator_output/default.aierun_summary

x86viz:
	vitis_analyzer  x86simulator_output/x86sim.aierun_summary

cleanall: clean
	rm -rf $(AIE_OUT_DIR)

clean:
	rm -rf *.log pl_sample* aiesimulator_output x86simulator_output libadf.a
	rm -rf temp *.jou .AIE_SIM_CMD_LINE_OPTIONS .Xil aie/Work *.vcd *.csv *.db

	
