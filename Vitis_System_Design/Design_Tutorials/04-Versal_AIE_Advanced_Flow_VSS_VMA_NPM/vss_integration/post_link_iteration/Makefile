# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Post-Link Iteration Makefile
#
# This Makefile imports VMA into Vivado, adds custom RTL, and generates
# the final device image.

.PHONY: all build clean rebuild package

# =============================================================================
# Configuration
# =============================================================================
# AIE paths (relative to vss_integration/post_link_iteration)
AIE_DIR      := $(shell dirname $(shell dirname $(CURDIR)))/aie_dummy
LIBADF_FILE  := $(AIE_DIR)/libadf.a

# VMA path
VMA_FILE     := ../link_vma/post_link.vma

# Platform project path
PLATFORM_PROJ := ../platform/hw/build/my_project/my_project.xpr

# =============================================================================
# Build Targets
# =============================================================================
all: build package

# Import VMA, add custom RTL, generate device image and export fixed XSA
build:
	@echo "Importing VMA and building Vivado project..."
	vivado -mode batch $(PLATFORM_PROJ) -source ./import_vma.tcl

# Debugging Purpose
# Assume the project is already imported and the custom RTL is already added
# Only run the vivado_build.tcl to run through Vivado synthesize and implementation steps.
rebuild:
	@echo "Rebuilding Vivado project..."
	vivado -mode batch ./project_1/project_1.xpr -source ./vivado_build.tcl

# Generate final PDI with PL device image and AIE CDO
package: $(LIBADF_FILE) $(VMA_FILE)
	@echo "Packaging design with AIE library..."
	@echo "Using AIE library: $(LIBADF_FILE)"
	v++ -p -t hw -f top.xsa \
        --package.no_image \
        --package.out_dir package_hw \
		$(VMA_FILE) \
		$(LIBADF_FILE) && \
	echo "Package output: package_hw/a.pdi"

# Dependency check for AIE library
$(LIBADF_FILE):
	@if [ ! -f "$(LIBADF_FILE)" ]; then \
		echo "ERROR: AIE library not found: $(LIBADF_FILE)"; \
		echo "Please build AIE first: cd $(AIE_DIR) && make compile"; \
		exit 1; \
	fi

# =============================================================================
# Clean Target
# =============================================================================
clean:
	@echo "Cleaning post-link iteration build artifacts..."
	rm -rf project_1*
	rm -f vivado*.log vivado*.jou
	rm -rf .Xil .gen
	rm -rf package_hw _x *.xclbin
	rm -rf *.log *.package_summary
