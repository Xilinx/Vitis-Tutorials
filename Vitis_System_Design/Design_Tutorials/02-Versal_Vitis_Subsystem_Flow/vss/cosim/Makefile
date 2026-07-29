#
# Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

# =========================================================
# Part and DUT setup
# =========================================================
VSS             ?= vss_top
PART            ?= xcvc1902-vsva2197-2MP-e-S

# =========================================================
# BUILD PRODUCTS
# ========================================================
BUILD_DIR := build
SIM_DIR   := $(BUILD_DIR)/$(VSS)_cosim/$(VSS).sim/sim_1/behav/xsim

# =========================================================
# SOURCE DIRECTORIES
# =========================================================
SRC_DIR := src

# =========================================================
# Paths needed by xsim
# =========================================================
export AIE_WORK_DIR :=$(AIE_WORK)
export AIESIM_OPTIONS=$(shell pwd)/$(SRC_DIR)/aiesim.txt
#export LD_LIBRARY_PATH :=$(XILINX_VITIS)/aietools/lib/lnx64.o:$(LD_LIBRARY_PATH)

# =========================================================
# Testbench project script
# =========================================================
COSIM_TCL       := $(SRC_DIR)/cosim_proj.tcl


# =========================================================
# DEPENDENCIES
# =========================================================
DEPS_VSS     := ../build/$(VSS)/$(VSS).vss
DEPS_COSIM   := $(SRC_DIR)/cosim_proj.tcl
DEPS_COSIM   += $(wildcard ${SRC_DIR}/*.sv)

# =========================================================
# Recipes
# =========================================================
.PHONY: crts
crts: $(AIE_WORK_DIR)
	cd $(AIE_WORK_DIR)/ps/c_rts/systemC && \
	$(MAKE) -f Makefile.xsim

.PHONY: cosim_vss
cosim_vss: crts $(DEPS_VSS) $(DEPS_COSIM)
	vivado -source $(COSIM_TCL) -tclargs $(VSS) $(PART)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil vivado* .crash*
