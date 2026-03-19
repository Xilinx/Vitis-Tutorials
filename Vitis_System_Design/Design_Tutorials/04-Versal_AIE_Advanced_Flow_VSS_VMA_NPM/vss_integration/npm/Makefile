# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Vivado Non-Project Mode (NPM) Makefile
#
# This Makefile builds the design using Vivado Non-Project Mode.

.PHONY: all clean build package

# =============================================================================
# Configuration
# =============================================================================
# AIE paths (relative to vss_integration/npm)
AIE_DIR      := $(shell dirname $(shell dirname $(CURDIR)))/aie_dummy
LIBADF_FILE  := $(AIE_DIR)/libadf.a

# VMA path
VMA_FILE     := ../link_vma/post_link.vma

# =============================================================================
# Build Targets
# =============================================================================
all: build package

build:
	@echo "Building with Vivado Non-Project Mode..."
	vivado -mode batch -source ./vivado_npm.tcl

package: $(LIBADF_FILE) $(VMA_FILE)
	@echo "Packaging design with AIE library..."
	@echo "Using AIE library: $(LIBADF_FILE)"
	v++ -p -t hw -f ./top.xsa \
        --package.no_image \
        --package.out_dir package_hw \
		$(VMA_FILE) \
		$(LIBADF_FILE)

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
	@echo "Cleaning NPM build artifacts..."
	rm -rf _x .cache .gen .tmpCRC .Xil gen_files package_hw static_files \
		a.xclbin clockInfo.txt top* v++* *.xml *.log *.jou
