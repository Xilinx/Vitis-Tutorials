# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# VSS Build Makefile
#
# This Makefile builds the Vitis Subsystem (VSS) with AIE graph integration.

.PHONY: clean vss 

# =============================================================================
# Configuration
# =============================================================================
ECHO         := @echo
PROJECT_DIR  := ./
PROJECT_REPO := $(shell readlink -f $(PROJECT_DIR))

# AIE paths (relative to vss_integration/vss)
AIE_DIR      := $(shell dirname $(shell dirname $(CURDIR)))/aie_dummy
LIBADF_FILE  := $(AIE_DIR)/libadf.a
LIBADF_NAME  := libadf

# Build configuration
PART         := xcve2802-vsvh1760-2MP-e-S
VSS_INST     := $(PROJECT_REPO)/vss/vss_ss.vss
MODE         := --mode vss

LINK_OPTIONS := --save-temps
LINK_OPTIONS += --out_dir vss
LINK_OPTIONS += --config vss.cfg 

# =============================================================================
# Build Targets
# =============================================================================
all: vss

vss: ${VSS_INST}

${VSS_INST}: vss.cfg $(LIBADF_FILE)
	$(ECHO) "Running v++ linker with vss mode"
	$(ECHO) "Using AIE library: $(LIBADF_FILE)"
	v++ --link ${MODE} --part ${PART} ${LINK_OPTIONS} $(LIBADF_FILE)

# Dependency rule to ensure AIE is built
$(LIBADF_FILE):
	@echo "Building AIE library..."
	@if [ ! -d "$(AIE_DIR)" ]; then \
		echo "ERROR: AIE directory not found: $(AIE_DIR)"; \
		exit 1; \
	fi
	$(MAKE) -C $(AIE_DIR) compile || (echo "ERROR: AIE build failed!" && exit 1)

# =============================================================================
# Clean Target
# =============================================================================
clean:
	$(ECHO) "Cleaning VSS build artifacts..."
	rm -rf v++* *.log _x vss .Xil .ipcache

clean:
	$(ECHO) "cleaning vss"
	rm -rf v++* *.log _x vss .Xil .ipcache

