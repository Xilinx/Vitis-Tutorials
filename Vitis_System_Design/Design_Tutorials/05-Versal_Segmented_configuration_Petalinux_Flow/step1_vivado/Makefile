#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

# =============================================================================
# Project Configuration
# =============================================================================
ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR := $(ROOT_DIR)/build
VIVADO_DIR := $(BUILD_DIR)/vivado
TCL_SCRIPT := $(ROOT_DIR)/run.tcl

# Output files
XSA_HW := $(VIVADO_DIR)/vck190_hw.xsa
VIVADO_LOG := $(VIVADO_DIR)/vivado.log

# =============================================================================
# Phony Targets
# =============================================================================
.PHONY: all hw sd_card clean ultraclean help

# =============================================================================
# Build Targets
# =============================================================================

all: hw
sd_card: all

hw: $(XSA_HW)
	@echo "INFO: Step 1 completed - Extensible XSA file generated"

$(XSA_HW): $(TCL_SCRIPT)
	@echo "INFO: Generating extensible XSA for VCK190 with segmented configuration..."
	@mkdir -p $(VIVADO_DIR)
	@cd $(VIVADO_DIR) && vivado -mode batch -notrace -source $(TCL_SCRIPT) -log $(VIVADO_LOG)
	@if [ ! -f "$(XSA_HW)" ]; then \
		echo "ERROR: XSA generation failed. Check $(VIVADO_LOG) for details."; \
		exit 1; \
	fi
	@echo "INFO: XSA file generated successfully:"
	@echo "  - $(XSA_HW)"

# =============================================================================
# Cleanup Targets
# =============================================================================

clean:
	@echo "INFO: Cleaning build artifacts..."
	$(RM) -rf $(BUILD_DIR) vivado*.log vivado*.jou

ultraclean: clean

# =============================================================================
# Help
# =============================================================================

help:
	@echo "============================================================================"
	@echo "Step 1 Makefile - Vivado Platform Generation"
	@echo "============================================================================"
	@echo ""
	@echo "Targets:"
	@echo "  make all        - Generate extensible XSA file"
	@echo "  make hw         - Generate extensible XSA file"
	@echo "  make clean      - Clean build directory and log files"
	@echo "  make ultraclean - Clean everything"
	@echo ""
	@echo "Outputs:"
	@echo "  - $(XSA_HW)"
	@echo "============================================================================"
