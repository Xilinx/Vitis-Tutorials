# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# =============================================================================
# Project Configuration
# =============================================================================
ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
VERSION := 2025.2
TARGET ?= hw_emu

# =============================================================================
# File Names and Paths
# =============================================================================
XSA_NAME := vek385
FIXED_XSA_NAME := sys_gm2aie
XSA_PATH := $(ROOT_DIR)/../step1_vivado/build/vivado/project_1
PLATFORM := $(XSA_PATH)/$(XSA_NAME)_$(TARGET).xsa

LINK_OUT := $(FIXED_XSA_NAME)_$(TARGET).xsa
XCLBIN := gm2aie.xclbin

# =============================================================================
# Build Directories
# =============================================================================
TEMP_DIR := _x.$(TARGET)
BUILD_DIR := build_dir.$(TARGET)
PKG_DIR := package.$(TARGET)
AIE_WORKDIR := ./_aie

# =============================================================================
# Compiler Tools
# =============================================================================
XCXX := v++
XAIE := aiecompiler

# =============================================================================
# Source Files
# =============================================================================
GRAPH_SRC := src/graph.cpp
S2MM_SRC := pl_kernel/s2mm.cpp

# =============================================================================
# Build Artifacts
# =============================================================================
GRAPH_XO := libsdf.a
S2MM_XO := s2mm.xo
XOS := $(S2MM_XO) $(GRAPH_XO)

# =============================================================================
# Compiler Flags
# =============================================================================
GRAPH_FLAGS := \
	--nodot-graph=true \
	-log-level=5 \
	-v \
	-include="./src" \
	-include="./data" \
	-include="./src/kernels" \
	-include="./" \
	--output-archive=$(GRAPH_XO)

S2MM_FLAGS := --config cfg/s2mm.cfg
LINK_FLAGS := --config cfg/binary_container_1-link.cfg
PKG_FLAGS := --package.defer_aie_run --config cfg/package.cfg --package.out_dir $(PKG_DIR)

XAIE_FLAGS := \
	-include="${XILINX_VITIS_AIETOOLS}/include" \
	-include="./" \
	-workdir "$(AIE_WORKDIR)"

# =============================================================================
# Build Rules
# =============================================================================
.PHONY: all build sd_card integration package clean ultraclean help

all: build

build: $(LINK_OUT)

$(GRAPH_XO): $(GRAPH_SRC)
	@echo "Building AI Engine graph..."
	$(XAIE) $(XAIE_FLAGS) --platform $(PLATFORM) $(GRAPH_FLAGS) $<

$(S2MM_XO): $(S2MM_SRC)
	@echo "Building PL kernel..."
	@mkdir -p $(TEMP_DIR)
	$(XCXX) $(XCXX_COMMON_OPTS) -t $(TARGET) --platform $(PLATFORM) -c -o $@ $< -k s2mm $(S2MM_FLAGS)

$(LINK_OUT): $(GRAPH_XO) $(S2MM_XO)
	@echo "Linking kernels..."
	@mkdir -p $(BUILD_DIR)
	$(XCXX) $(XCXX_COMMON_OPTS) -t $(TARGET) --platform $(PLATFORM) \
		--temp_dir $(TEMP_DIR) -l -o $@ $^ $(LINK_FLAGS)

$(XCLBIN): $(LINK_OUT)
	@echo "Generating XCLBIN..."
	$(XCXX) $(PKG_FLAGS) -s -f $< $(GRAPH_XO) -t $(TARGET) -p -o $@

sd_card: integration

integration:
	@echo "Starting integration for target: $(TARGET)"
	$(RM) -rf $(LINK_OUT)
	$(MAKE) build TARGET=$(TARGET)
	@echo "Integration complete for target: $(TARGET)"

package: $(XCLBIN)
	@echo "XCLBIN and PDI generated for target: $(TARGET)"

# =============================================================================
# Cleanup Rules
# =============================================================================
clean:
	@echo "Cleaning build artifacts..."
	$(RM) -rf _aie *.xsa tmp binary_container_1 drivers extracted \
		libsdf.a s2mm.xo .ipcache .Xil $(XCLBIN) *.pdi \
		*.log *.summary *.db build_dir.* _x.* *.csv *.isd *.package_summary \
		analyzer_input

ultraclean: clean
	$(RM) package.* -rf *.link_summary *_x *.txt *.json *summary sysroots \
		environment-setup-x86_64-petalinux-linux \
		version-x86_64-petalinux-linux \
		x86_64-qemu-xilinx-nativesdk-standalone-2025.2+release-stable.sh

# =============================================================================
# Help
# =============================================================================
help:
	@echo "Makefile usage:"
	@echo "  make build TARGET=<hw_emu|hw>     Build kernels and link"
	@echo "  make integration TARGET=<hw_emu|hw>  Clean and rebuild all"
	@echo "  make package TARGET=<hw_emu|hw>     Generate XCLBIN and PDI"
	@echo "  make clean                         Clean build and temp files"
	@echo "  make ultraclean                    Clean everything"
	@echo "  make help                          Show this help message"
