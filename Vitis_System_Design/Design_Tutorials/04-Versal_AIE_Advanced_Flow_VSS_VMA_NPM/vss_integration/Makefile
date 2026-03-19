# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# VSS Integration Level Makefile
#
# This Makefile orchestrates the complete VSS->VMA->NPM integration flow:
#   1. VSS Build: Build Vitis Subsystem with AIE graph (platform-independent)
#   2. Platform Build: Create custom platform with AXI Stream interfaces
#   3. Link VMA: Generate VMA (Vitis Export Archive) for Vivado
#   4. Post-Link Iteration: Import VMA into Vivado and add custom RTL
#   5. NPM: Build using Vivado Non-Project Mode
#
# Build Order (enforced by dependencies):
#   vss -> platform -> link_vma -> post_link_iteration -> npm
# Note: VSS and platform can be built in parallel, but VSS is listed first
#       to emphasize that VSS is platform-independent.

# =============================================================================
# Configuration
# =============================================================================
SHELL := /bin/bash
MAKE := make

# AIE directory and library paths (relative to vss_integration)
# These variables can be used by subdirectory Makefiles
AIE_DIR := $(shell dirname $(CURDIR))/aie_dummy
LIBADF := $(AIE_DIR)/libadf.a
LIBADF_NAME := libadf.a

# Directory definitions
VSS_DIR := vss
PLATFORM_DIR := platform/hw
LINK_VMA_DIR := link_vma
POST_LINK_DIR := post_link_iteration
NPM_DIR := npm

# Output file definitions (used for dependency checking)
VSS_OUTPUT := $(VSS_DIR)/vss/vss_ss.vss
PLATFORM_OUTPUT := $(PLATFORM_DIR)/build/hw.xsa
LINK_VMA_OUTPUT := $(LINK_VMA_DIR)/post_link.vma
POST_LINK_OUTPUT := $(POST_LINK_DIR)/project_1
NPM_OUTPUT := $(NPM_DIR)/package_hw/a.pdi

# =============================================================================
# Default Target
# =============================================================================
.PHONY: all
all: vss platform link_vma post_link_iteration npm
	@echo ""
	@echo "=========================================="
	@echo "VSS Integration build completed successfully!"
	@echo "=========================================="

# =============================================================================
# VSS Build Target
# =============================================================================
.PHONY: vss
vss: $(VSS_OUTPUT)
$(VSS_OUTPUT):
	@echo ""
	@echo "=========================================="
	@echo "Step 1/5: Building VSS..."
	@echo "=========================================="
	@if [ ! -d "$(VSS_DIR)" ]; then \
		echo "ERROR: VSS directory not found: $(VSS_DIR)"; \
		exit 1; \
	fi
	@if [ ! -f "$(LIBADF)" ]; then \
		echo "ERROR: libadf.a not found! Build AIE first."; \
		echo "Expected location: $(LIBADF)"; \
		exit 1; \
	fi
	$(MAKE) -C $(VSS_DIR) all || (echo "ERROR: VSS build failed!" && exit 1)
	@if [ ! -f "$(VSS_OUTPUT)" ]; then \
		echo "ERROR: VSS file not generated: $(VSS_OUTPUT)"; \
		exit 1; \
	fi
	@echo "VSS build completed: $(VSS_OUTPUT)"

# =============================================================================
# Platform Build Target
# =============================================================================
.PHONY: platform
platform: $(PLATFORM_OUTPUT)
$(PLATFORM_OUTPUT):
	@echo ""
	@echo "=========================================="
	@echo "Step 2/5: Building Platform..."
	@echo "=========================================="
	@if [ ! -d "$(PLATFORM_DIR)" ]; then \
		echo "ERROR: Platform directory not found: $(PLATFORM_DIR)"; \
		exit 1; \
	fi
	$(MAKE) -C $(PLATFORM_DIR) all || (echo "ERROR: Platform build failed!" && exit 1)
	@if [ ! -f "$(PLATFORM_OUTPUT)" ]; then \
		echo "ERROR: Platform XSA not generated: $(PLATFORM_OUTPUT)"; \
		exit 1; \
	fi
	@echo "Platform build completed: $(PLATFORM_OUTPUT)"

# =============================================================================
# Link VMA Target
# =============================================================================
.PHONY: link_vma
link_vma: $(LINK_VMA_OUTPUT)
$(LINK_VMA_OUTPUT): $(VSS_OUTPUT) $(PLATFORM_OUTPUT)
	@echo ""
	@echo "=========================================="
	@echo "Step 3/5: Building Link VMA..."
	@echo "=========================================="
	@if [ ! -d "$(LINK_VMA_DIR)" ]; then \
		echo "ERROR: Link VMA directory not found: $(LINK_VMA_DIR)"; \
		exit 1; \
	fi
	$(MAKE) -C $(LINK_VMA_DIR) all || (echo "ERROR: Link VMA build failed!" && exit 1)
	@if [ ! -f "$(LINK_VMA_OUTPUT)" ]; then \
		echo "ERROR: VMA file not generated: $(LINK_VMA_OUTPUT)"; \
		exit 1; \
	fi
	@echo "Link VMA build completed: $(LINK_VMA_OUTPUT)"

# =============================================================================
# Post-Link Iteration Target
# =============================================================================
.PHONY: post_link_iteration
post_link_iteration: $(POST_LINK_OUTPUT)
$(POST_LINK_OUTPUT): $(LINK_VMA_OUTPUT)
	@echo ""
	@echo "=========================================="
	@echo "Step 4/5: Building Post-Link Iteration..."
	@echo "=========================================="
	@if [ ! -d "$(POST_LINK_DIR)" ]; then \
		echo "ERROR: Post-link directory not found: $(POST_LINK_DIR)"; \
		exit 1; \
	fi
	$(MAKE) -C $(POST_LINK_DIR) all || (echo "ERROR: Post-link iteration build failed!" && exit 1)
	@echo "Post-link iteration build completed"

# =============================================================================
# NPM Target
# =============================================================================
.PHONY: npm
npm: $(NPM_OUTPUT)
$(NPM_OUTPUT): $(POST_LINK_OUTPUT)
	@echo ""
	@echo "=========================================="
	@echo "Step 5/5: Building NPM..."
	@echo "=========================================="
	@if [ ! -d "$(NPM_DIR)" ]; then \
		echo "ERROR: NPM directory not found: $(NPM_DIR)"; \
		exit 1; \
	fi
	$(MAKE) -C $(NPM_DIR) all || (echo "ERROR: NPM build failed!" && exit 1)
	@if [ ! -f "$(NPM_OUTPUT)" ]; then \
		echo "WARNING: NPM PDI file not found: $(NPM_OUTPUT)"; \
	fi
	@echo "NPM build completed"

# =============================================================================
# Clean Targets
# =============================================================================
.PHONY: clean
clean:
	@echo "Cleaning all VSS integration components..."
	@if [ -d "$(VSS_DIR)" ]; then \
		$(MAKE) -C $(VSS_DIR) clean || true; \
	fi
	@if [ -d "$(PLATFORM_DIR)" ]; then \
		$(MAKE) -C $(PLATFORM_DIR) clean || true; \
	fi
	@if [ -d "$(LINK_VMA_DIR)" ]; then \
		$(MAKE) -C $(LINK_VMA_DIR) clean || true; \
	fi
	@if [ -d "$(POST_LINK_DIR)" ]; then \
		$(MAKE) -C $(POST_LINK_DIR) clean || true; \
	fi
	@if [ -d "$(NPM_DIR)" ]; then \
		$(MAKE) -C $(NPM_DIR) clean || true; \
	fi
	@echo "Clean completed"

# =============================================================================
# Help Target
# =============================================================================
.PHONY: help
help:
	@echo "======================================================================"
	@echo "VSS Integration Flow - Build System"
	@echo "======================================================================"
	@echo ""
	@echo "Available targets:"
	@echo "  all                  - Build all components (complete flow)"
	@echo "  vss                  - Build VSS component (Step 1/5, platform-independent)"
	@echo "  platform             - Build Platform component (Step 2/5)"
	@echo "  link_vma             - Build Link VMA component (Step 3/5)"
	@echo "  post_link_iteration  - Build Post-Link Iteration (Step 4/5)"
	@echo "  npm                  - Build NPM component (Step 5/5)"
	@echo "  clean                - Clean all components"
	@echo "  help                 - Show this help message"
	@echo ""
	@echo "Build Order (automatic dependency resolution):"
	@echo "  vss -> platform -> link_vma -> post_link_iteration -> npm"
	@echo "  Note: VSS is platform-independent and can be built before platform"
	@echo ""
	@echo "Output Files:"
	@echo "  Platform:  $(PLATFORM_OUTPUT)"
	@echo "  VSS:       $(VSS_OUTPUT)"
	@echo "  VMA:       $(LINK_VMA_OUTPUT)"
	@echo "  NPM:       $(NPM_OUTPUT)"
	@echo ""
	@echo "======================================================================"
