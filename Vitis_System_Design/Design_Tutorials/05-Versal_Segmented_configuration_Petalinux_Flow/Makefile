#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

# =============================================================================
# Project Configuration
# =============================================================================
ROOT_DIR := $(shell pwd)
TARGET ?= hw

# =============================================================================
# Path Definitions
# =============================================================================
STEP1_DIR := $(ROOT_DIR)/step1_vivado
STEP2_DIR := $(ROOT_DIR)/step2_vitis_integration
STEP3_DIR := $(ROOT_DIR)/step3_application

STEP1_BUILD_DIR := $(STEP1_DIR)/build/vivado
STEP2_OUTPUT_DIR := $(STEP2_DIR)

# =============================================================================
# File Names
# =============================================================================
XSA_NAME := vck190
FIXED_XSA_NAME := sys_gm2aie

# Step 1 outputs (extensible XSA)
XSA_HW := $(STEP1_BUILD_DIR)/$(XSA_NAME)_hw.xsa

# Step 2 outputs (fixed XSA)
FIXED_XSA_HW := $(STEP2_OUTPUT_DIR)/$(FIXED_XSA_NAME)_hw.xsa

# =============================================================================
# Phony Targets
# =============================================================================
.PHONY: all sd_card clean ultraclean help step1 step2_hw step3_hw

# =============================================================================
# Top-Level Targets
# =============================================================================

all: sd_card

sd_card: step1 step2_hw step3_hw
	@echo "INFO: Hardware (SD card) flow completed successfully"

# =============================================================================
# Step 1: Generate Extensible XSA (Vivado)
# =============================================================================

step1: $(XSA_HW)
	@echo "INFO: Step 1 completed - Extensible XSA file generated"

$(XSA_HW):
	@echo "INFO: Building Step 1 - Generating extensible XSA..."
	$(MAKE) -C $(STEP1_DIR) all

# =============================================================================
# Step 2: Kernel Integration (Vitis)
# =============================================================================

step2_hw: $(FIXED_XSA_HW)
	@echo "INFO: Step 2 completed - Fixed XSA generated"

$(FIXED_XSA_HW): $(XSA_HW)
	@echo "INFO: Building Step 2 - Integrating kernels..."
	$(MAKE) -C $(STEP2_DIR) build TARGET=hw

# =============================================================================
# Step 3: Application Build (Petalinux)
# =============================================================================

step3_hw: $(FIXED_XSA_HW)
	@echo "INFO: Building Step 3 - Building application and SD card..."
	$(MAKE) -C $(STEP3_DIR) sd_card TARGET=hw

# =============================================================================
# Cleanup Targets
# =============================================================================

clean:
	@echo "INFO: Cleaning build artifacts..."
	$(MAKE) -C $(STEP1_DIR) clean
	$(MAKE) -C $(STEP2_DIR) clean
	$(MAKE) -C $(STEP3_DIR) clean

ultraclean: clean
	@echo "INFO: Performing complete cleanup..."
	$(MAKE) -C $(STEP2_DIR) ultraclean
	$(MAKE) -C $(STEP3_DIR) ultraclean

# =============================================================================
# Help
# =============================================================================

help:
	@echo "============================================================================"
	@echo "Vitis Flow Makefile Help"
	@echo "============================================================================"
	@echo ""
	@echo "Main Targets:"
	@echo "  make all        - Run complete hardware flow (alias for sd_card)"
	@echo "  make sd_card    - Run complete hardware flow (for SD card)"
	@echo ""
	@echo "Individual Steps:"
	@echo "  make step1      - Generate extensible XSA (Vivado)"
	@echo "  make step2_hw   - Integrate kernels for hardware"
	@echo "  make step3_hw   - Build application for hardware"
	@echo ""
	@echo "Cleanup:"
	@echo "  make clean      - Clean all intermediate files"
	@echo "  make ultraclean - Clean everything including outputs"
	@echo ""
	@echo "============================================================================"
