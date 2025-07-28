#####################################################################
# Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#####################################################################

CURRENT_DIR = $(shell pwd)
WKSPC_DIR = $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# AIE compiler tools
AIE_SRC_DIR = $(shell pwd)/src
AIE_CC = aiecompiler
AIE_TOP_FILE = ../src/tb.cpp
AIE_OUT_DIR = ./Work

PLATFORM_REPO_PATHS = $(XILINX_VITIS)/base_platforms
PLATFORM_VERSION = 202420_1
PLATFORM ?= $(PLATFORM_REPO_PATHS)/xilinx_vck190_base_$(PLATFORM_VERSION)/xilinx_vck190_base_$(PLATFORM_VERSION).xpfm

# Compiler settings
AIE_FLAGS = -v -Xchess=main:darts.xargs=-nb -include=$(XILINX_VITIS)/aietools/include -include="../" -include="../src" -include="../data" -platform=$(PLATFORM) -workdir=$(AIE_OUT_DIR)

#
# primary build targets
#

x86sim: $(AIE_SRC_DIR)/matmul.cpp
	mkdir -p Emulation-SW && \
	cd Emulation-SW && \
	$(AIE_CC) $(AIE_FLAGS) -target=x86sim $(AIE_TOP_FILE) | tee build.log && \
	x86simulator --pkg-dir=./Work -i=.. --dump --trace | tee x86sim.log
	@echo
	@echo Comparing x86sim and reference results...
	@echo
	diff -ws ./Emulation-SW/x86simulator_output/Pout.dat ./julia/Pout.dat
	@echo
	@echo "***************************** x86sim completed! *****************************"
	@echo

aiesim: $(AIE_SRC_DIR)/matmul.cpp
	mkdir -p Emulation-HW && \
	cd Emulation-HW && \
	$(AIE_CC) --xlopt=0 $(AIE_FLAGS) -target=hw $(AIE_TOP_FILE) | tee build.log && \
	aiesimulator --pkg-dir=./Work -i.. --dump-vcd=abs_sq.vcd --profile | tee aiesim.log
	@echo
	@echo Removing timing info from Pout.dat...
	@echo
	cd Emulation-HW/aiesimulator_output && \
	sed '/^T/d' < Pout.dat > Pout_new.dat
	@echo
	@echo Comparing aiesim and reference results...
	@echo
	diff -ws ./Emulation-HW/aiesimulator_output/Pout_new.dat ./julia/Pout.dat
	@echo
	@echo "***************************** aiesim completed! *****************************"
	@echo

all: x86sim aiesim

clean:
	rm -rf Emulation-*
	rm -rf *.log
	rm -rf _ide
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf .logger

.PHONY: incremental clean
incremental: clean all

