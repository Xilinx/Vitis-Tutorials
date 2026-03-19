# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Link VMA Makefile
#
# This Makefile links the VSS with the platform and generates VMA
# (Vitis Export Archive) for Vivado import.

.PHONY: hw_link all clean

# =============================================================================
# Configuration
# =============================================================================
PLATFORMNAME := ../platform/hw/build/hw.xsa
VSS_FILE     := ../vss/vss/vss_ss.vss
CFG_FILE     := ./system.cfg
VMA_OUTPUT   := post_link.vma

# =============================================================================
# Build Targets
# =============================================================================
all: hw_link

hw_link: $(VSS_FILE) $(CFG_FILE) $(PLATFORMNAME)
	@echo "Linking VSS with platform to generate VMA..."
	@echo "VSS file: $(VSS_FILE)"
	@echo "Platform: $(PLATFORMNAME)"
	@echo "Output: $(VMA_OUTPUT)"
	v++ -l -t hw -s \
		--platform $(PLATFORMNAME) \
		$(VSS_FILE) \
		--config $(CFG_FILE) \
		--export_archive -o $(VMA_OUTPUT)

# Dependency checks
$(VSS_FILE):
	@if [ ! -f "$(VSS_FILE)" ]; then \
		echo "ERROR: VSS file not found: $(VSS_FILE)"; \
		echo "Please build VSS first: cd ../vss && make all"; \
		exit 1; \
	fi

$(PLATFORMNAME):
	@if [ ! -f "$(PLATFORMNAME)" ]; then \
		echo "ERROR: Platform XSA not found: $(PLATFORMNAME)"; \
		echo "Please build platform first: cd ../platform/hw && make all"; \
		exit 1; \
	fi

# =============================================================================
# Clean Target
# =============================================================================
clean:
	@echo "Cleaning link_vma build artifacts..."
	rm -rf $(VMA_OUTPUT)
	rm -rf _x
	rm -rf .Xil .ipcache .gen
	rm -rf *.log *.link_summary