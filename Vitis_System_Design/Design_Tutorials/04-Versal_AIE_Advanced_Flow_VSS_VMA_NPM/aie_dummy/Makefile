# Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# AIE Dummy Kernel Makefile
#
# This Makefile compiles the AIE dummy kernel and generates libadf.a
# which is required for the VSS integration flow.
#
# Targets:
#   all      - Clean, compile, and simulate (default)
#   compile  - Compile AIE graph only (generates libadf.a)
#   sim      - Run AIE simulator
#   x86      - Compile for x86 simulation
#   x86sim   - Run x86 simulator
#   clean    - Remove build artifacts
#   cleanall - Deep clean (includes data files)

# =============================================================================
# Configuration
# =============================================================================
SHELL := /bin/bash

# Source files
GRAPH   = src/test.cpp
LIBADF  = libadf.a

# AIE Compiler command
AIE_CMPL_CMD := aiecompiler \
	-include="./src" \
	-include="./src/inc" \
	-include="./src/kernels" \
	-workdir=./Work \
	${GRAPH}

# AIE Compiler flags
AIE_FLAGS += --part=xcve2802-vsvh1760-2MP-e-S
AIE_FLAGS += -v -log-level=5
AIE_FLAGS += -xlopt=0
# Optional flags (commented out - uncomment if needed):
#AIE_FLAGS += -constraints=./src/shim_loc.json
#AIE_FLAGS += --enable-dma-fifo
#AIE_FLAGS += --Xchess=layernorm_kernel:backend.mist2.maxfoldk=500
AIE_FLAGS += --Xmapper=disableFloorplanning
AIE_FLAGS += --Xrouter=enableSplitAsBroadcast
AIE_FLAGS += --Xrouter=disablepathbalancing

# AIE Simulator command
AIE_SIM_CMD := aiesimulator --pkg-dir=./Work
AIE_SIM_CMD += --profile
# Optional simulator flags:
#AIE_SIM_CMD += --dump-vcd foo
#AIE_SIM_CMD += --simulation-cycle-timeout=40000

# Test parameters (currently unused, reserved for future use)
N_batch := 1
D_dim1  := 40
D_dim2  := 512

# =============================================================================
# Default Target
# =============================================================================
.PHONY: all
all: clean compile sim
	@echo ""
	@echo "=========================================="
	@echo "AIE build and simulation completed!"
	@echo "Output: $(LIBADF)"
	@echo "=========================================="

# =============================================================================
# Compilation Target
# =============================================================================
.PHONY: compile
compile: $(LIBADF)

$(LIBADF): $(GRAPH)
	@echo ""
	@echo "=========================================="
	@echo "Compiling AIE Graph..."
	@echo "=========================================="
	@if [ ! -f "$(GRAPH)" ]; then \
		echo "ERROR: Graph source not found: $(GRAPH)"; \
		exit 1; \
	fi
	@echo "Source: $(GRAPH)"
	@echo "Output: $(LIBADF)"
	${AIE_CMPL_CMD} ${AIE_FLAGS} || (echo "ERROR: AIE compilation failed!" && exit 1)
	@if [ ! -f "$(LIBADF)" ]; then \
		echo "ERROR: $(LIBADF) not generated!"; \
		exit 1; \
	fi
	@echo "Compilation successful: $(LIBADF)"

# =============================================================================
# Simulation Targets
# =============================================================================
.PHONY: sim
sim: $(LIBADF)
	@echo ""
	@echo "=========================================="
	@echo "Running AIE Simulator..."
	@echo "=========================================="
	@if [ ! -d "./Work" ]; then \
		echo "ERROR: Work directory not found. Run 'make compile' first."; \
		exit 1; \
	fi
	${AIE_SIM_CMD}

.PHONY: x86
x86: $(GRAPH)
	@echo ""
	@echo "=========================================="
	@echo "Compiling for x86 Simulation..."
	@echo "=========================================="
	${AIE_CMPL_CMD} --target=x86sim ${AIE_FLAGS}

.PHONY: x86sim
x86sim:
	@echo ""
	@echo "=========================================="
	@echo "Running x86 Simulator..."
	@echo "=========================================="
	@if [ ! -d "./Work" ]; then \
		echo "ERROR: Work directory not found. Run 'make x86' first."; \
		exit 1; \
	fi
	x86simulator --pkg-dir=./Work --trace

# =============================================================================
# Clean Targets
# =============================================================================
.PHONY: clean
clean:
	@echo "Cleaning AIE build artifacts..."
	@rm -rf _x $(LIBADF) Work .Xil vitis_analyzer* foo.vcd *.log
	@rm -rf pl_sample* aiesimulator_output
	@rm -rf x86simulator_output output_data.txt
	@rm -rf temp diff_output a.txt
	@rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db *.soln
	@rm -rf ISS_RPC_SERVER_PORT *.wdb *.wcfg trdata.aiesim
	@rm -rf *.txt
	@echo "Clean completed"

.PHONY: cleanall
cleanall: clean
	@echo "Performing deep clean..."
	@rm -rf data
	@echo "Deep clean completed"

# =============================================================================
# Help Target
# =============================================================================
.PHONY: help
help:
	@echo "======================================================================"
	@echo "AIE Dummy Kernel - Build System"
	@echo "======================================================================"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Clean, compile, and simulate (default)"
	@echo "  compile  - Compile AIE graph (generates $(LIBADF))"
	@echo "  sim      - Run AIE simulator"
	@echo "  x86      - Compile for x86 simulation"
	@echo "  x86sim   - Run x86 simulator"
	@echo "  clean    - Remove build artifacts"
	@echo "  cleanall - Deep clean (includes data files)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Output:"
	@echo "  $(LIBADF) - AIE library archive (required for VSS build)"
	@echo ""
	@echo "Device: xcve2802-vsvh1760-2MP-e-S (VEK280)"
	@echo ""
	@echo "======================================================================"
