# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Top-level Makefile for VSS->VMA->NPM Integration Flow
#
# This Makefile orchestrates the complete build process:
#   1. AIE kernel compilation (generates libadf.a)
#   2. VSS integration (platform, VSS, VMA, post-link, NPM)
#
# Build Order:
#   aie_dummy -> vss_integration (platform -> vss -> link_vma -> post_link_iteration -> npm)

# =============================================================================
# Configuration
# =============================================================================
SHELL := /bin/bash
MAKE := make

# =============================================================================
# Default Target
# =============================================================================
.PHONY: all
all: aie_dummy vss_integration
	@echo ""
	@echo "=========================================="
	@echo "Build completed successfully!"
	@echo "=========================================="

# =============================================================================
# AIE Dummy Target
# =============================================================================
.PHONY: aie_dummy
aie_dummy:
	@echo ""
	@echo "=========================================="
	@echo "Building AIE Dummy Kernel..."
	@echo "=========================================="
	@if [ ! -d "aie_dummy" ]; then \
		echo "ERROR: aie_dummy directory not found!"; \
		exit 1; \
	fi
	$(MAKE) -C aie_dummy all || (echo "ERROR: AIE build failed!" && exit 1)
	@if [ ! -f "aie_dummy/libadf.a" ]; then \
		echo "ERROR: libadf.a not generated!"; \
		exit 1; \
	fi
	@echo "AIE build completed: libadf.a generated"

# =============================================================================
# VSS Integration Target
# =============================================================================
.PHONY: vss_integration
vss_integration: aie_dummy
	@echo ""
	@echo "=========================================="
	@echo "Building Vitis-Vivado Integration..."
	@echo "=========================================="
	@if [ ! -d "vss_integration" ]; then \
		echo "ERROR: vss_integration directory not found!"; \
		exit 1; \
	fi
	@if [ ! -f "aie_dummy/libadf.a" ]; then \
		echo "ERROR: libadf.a must be built before VSS integration!"; \
		echo "Run 'make aie_dummy' first."; \
		exit 1; \
	fi
	$(MAKE) -C vss_integration all || (echo "ERROR: VSS integration build failed!" && exit 1)
	@echo "VSS integration build completed"

# =============================================================================
# Clean Targets
# =============================================================================
.PHONY: clean cleanall
clean:
	@echo "Cleaning all components..."
	@if [ -d "aie_dummy" ]; then \
		$(MAKE) -C aie_dummy clean || true; \
	fi
	@if [ -d "vss_integration" ]; then \
		$(MAKE) -C vss_integration clean || true; \
	fi
	@echo "Clean completed"

cleanall: clean
	@echo "Performing deep clean..."
	@rm -rf .Xil *.log *.jou

# =============================================================================
# Help Target
# =============================================================================
.PHONY: help
help:
	@echo "======================================================================"
	@echo "VSS->VMA->NPM Integration Flow - Build System"
	@echo "======================================================================"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build all components (AIE + VSS Integration)"
	@echo "  aie_dummy        - Build AIE dummy kernel (generates libadf.a)"
	@echo "  vss_integration  - Build complete Vitis-Vivado Integration flow"
	@echo "  clean            - Clean all build artifacts"
	@echo "  cleanall         - Deep clean (includes logs and temporary files)"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Build Dependencies:"
	@echo "  vss_integration requires aie_dummy to be built first"
	@echo ""
	@echo "For individual VSS integration targets, use:"
	@echo "  cd vss_integration && make help"
	@echo ""
	@echo "======================================================================" 
