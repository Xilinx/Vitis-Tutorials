#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

# =============================================================================
# Project Configuration
# =============================================================================
ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
VERSION := 2025.2
TARGET ?= hw

# =============================================================================
# File Names and Paths
# =============================================================================
XSA_NAME := vck190
FIXED_XSA_NAME := sys_gm2aie
XSA_PATH := $(ROOT_DIR)/../step1_vivado/build/vivado/
PLATFORM := $(XSA_PATH)/$(XSA_NAME)_$(TARGET).xsa

LINK_OUT := $(FIXED_XSA_NAME)_$(TARGET).xsa

# =============================================================================
# Source Files (now using local copies in step2_vitis_integration)
# =============================================================================
AIE_SRC_DIR := $(ROOT_DIR)/src
AIE_DATA_DIR := $(ROOT_DIR)/src/data

# AI Engine source files
GRAPH_SRC := $(AIE_SRC_DIR)/graph.cpp
AIE_KERNEL_FILES := $(AIE_SRC_DIR)/kernels.cc

# PL kernel source files
MM2S_SRC := $(ROOT_DIR)/pl_kernel/mm2s.cpp
S2MM_SRC := $(ROOT_DIR)/pl_kernel/s2mm.cpp

# Connectivity configuration
SYSTEM_CFG := $(ROOT_DIR)/cfg/system.cfg

# =============================================================================
# Build Directories
# =============================================================================
TEMP_DIR := _x.$(TARGET)
BUILD_DIR := build_dir.$(TARGET)
AIE_WORKDIR := ./_aie

# =============================================================================
# Compiler Tools
# =============================================================================
XCXX := v++
XAIE := aiecompiler

# =============================================================================
# Build Artifacts
# =============================================================================
GRAPH_XO := libadf.a
MM2S_XO := mm2s.xo
S2MM_XO := s2mm.xo
XOS := $(MM2S_XO) $(S2MM_XO) $(GRAPH_XO)

# =============================================================================
# Compiler Flags
# =============================================================================
# AI Engine compiler flags (from description.json)
GRAPH_FLAGS := \
	--log-level=1 \
	--stacksize=1024 \
	--heapsize=1024 \
	-include="$(AIE_SRC_DIR)" \
	-include="$(AIE_DATA_DIR)" \
	-include="${XILINX_VITIS_AIETOOLS}/include" \
	--output-archive=$(GRAPH_XO)

XAIE_FLAGS := \
	-include="${XILINX_VITIS_AIETOOLS}/include" \
	-include="$(AIE_SRC_DIR)" \
	-workdir "$(AIE_WORKDIR)"

# PL kernel compile flags (frequency 300.0 MHz as per description.json)
PL_KERNEL_FREQ := 300.0

# Link flags: use system.cfg for connectivity and add nk for kernel instances
# nk format: nk=<kernel_name>:<num_instances>:<instance_name>
LINK_FLAGS := --config $(SYSTEM_CFG) 

XCXX_COMMON_OPTS := -s -g 

# =============================================================================
# Build Rules
# =============================================================================
.PHONY: all build sd_card integration clean ultraclean help check_sources

all: build

# Check if source files exist
check_sources:
	@echo "INFO: Checking source files..."
	@for file in "$(GRAPH_SRC)" "$(SYSTEM_CFG)" "$(MM2S_SRC)"  "$(S2MM_SRC)"; do \
		if [ ! -f "$$file" ]; then \
			echo "ERROR: Required file not found: $$file"; \
			exit 1; \
		fi; \
	done
	@echo "INFO: All source files found"

build: check_sources $(LINK_OUT)
	@echo "INFO: Step 2 completed - Fixed XSA generated: $(LINK_OUT)"

# Build AI Engine graph
$(GRAPH_XO): $(GRAPH_SRC) $(AIE_KERNEL_FILES)
	@echo "INFO: Building AI Engine graph..."
	@echo "INFO: Using sources from: $(AIE_SRC_DIR)"
	@mkdir -p $(AIE_WORKDIR)
	@mkdir -p $(AIE_WORKDIR)/data
	@if [ -d "$(AIE_DATA_DIR)" ]; then \
		cp -r $(AIE_DATA_DIR)/* $(AIE_WORKDIR)/data/ 2>/dev/null || true; \
	fi
	$(XAIE) $(XAIE_FLAGS) --platform $(PLATFORM) $(GRAPH_FLAGS) $(GRAPH_SRC)
	@if [ -f "$(AIE_WORKDIR)/$(GRAPH_XO)" ]; then \
		cp $(AIE_WORKDIR)/$(GRAPH_XO) .; \
		echo "INFO: AI Engine graph copied to: $(GRAPH_XO)"; \
	elif [ -f "$(GRAPH_XO)" ]; then \
		echo "INFO: AI Engine graph found at: $(GRAPH_XO)"; \
	else \
		echo "WARNING: AI Engine graph $(GRAPH_XO) not found in expected location"; \
	fi
	@echo "INFO: AI Engine graph build complete: $(GRAPH_XO)"

# Build PL kernels (using pattern rule for common build steps)
$(MM2S_XO): $(MM2S_SRC) | $(TEMP_DIR)
	@echo "INFO: Building PL kernel: mm2s"
	$(XCXX) $(XCXX_COMMON_OPTS) -t $(TARGET) --platform $(PLATFORM) \
		-c -o $@ $< -k mm2s --kernel_frequency $(PL_KERNEL_FREQ)
	@echo "INFO: PL kernel mm2s complete: $@"

$(S2MM_XO): $(S2MM_SRC) | $(TEMP_DIR)
	@echo "INFO: Building PL kernel: s2mm"
	$(XCXX) $(XCXX_COMMON_OPTS) -t $(TARGET) --platform $(PLATFORM) \
		-c -o $@ $< -k s2mm --kernel_frequency $(PL_KERNEL_FREQ)
	@echo "INFO: PL kernel s2mm complete: $@"

# Create temp directory (order-only prerequisite)
$(TEMP_DIR):
	@mkdir -p $(TEMP_DIR)

# Link all kernels to generate fixed XSA
$(LINK_OUT): $(GRAPH_XO) $(MM2S_XO) $(S2MM_XO) | $(BUILD_DIR)
	@echo "INFO: Linking kernels to generate fixed XSA..."
	@echo "INFO: Platform: $(PLATFORM)"
	@echo "INFO: Connectivity config: $(SYSTEM_CFG)"
	@echo "INFO: Linking: mm2s, s2mm, AIE graph"
	$(XCXX) $(XCXX_COMMON_OPTS) -t $(TARGET) --platform $(PLATFORM) \
		--temp_dir $(TEMP_DIR) -l -o $@ \
		$(MM2S_XO) $(S2MM_XO) $(GRAPH_XO) \
		$(LINK_FLAGS)
	@echo "INFO: Fixed XSA generated: $@"

# Create build directory (order-only prerequisite)
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

sd_card: build
	@echo "INFO: Step 2 (sd_card) completed for target: $(TARGET)"

# =============================================================================
# Cleanup Rules
# =============================================================================
clean:
	@echo "INFO: Cleaning build artifacts..."
	$(RM) -rf _aie *.xsa tmp binary_container_1 drivers extracted \
		libadf.a *.xo .ipcache .Xil \
		*.log *.summary *.db build_dir.* _x.* *.csv *.isd *.link_summary

ultraclean: clean
	$(RM) *.link_summary

# =============================================================================
# Help
# =============================================================================
help:
	@echo "Makefile usage:"
	@echo "  make build TARGET=hw     Build kernels and link to fixed XSA"
	@echo "  make integration TARGET=hw  Clean and rebuild all"
	@echo "  make clean                         Clean build and temp files"
	@echo "  make ultraclean                    Clean everything"
	@echo "  make help                          Show this help message"
	@echo ""
	@echo "Note: Package (XCLBIN generation) has been moved to step3"
	@echo ""
	@echo "Source files are located in: $(ROOT_DIR)"
	@echo "  - AIE Graph: $(GRAPH_SRC)"
	@echo "  - PL Kernels: $(MM2S_SRC), $(S2MM_SRC)"
	@echo "  - Connectivity: $(SYSTEM_CFG)"
