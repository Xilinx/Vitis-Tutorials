# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# =========================================================
# DEVICE SELECTION
# =========================================================
DEVICE_NAME ?= xcvc1902-vsva2197-2MP-e-S

# =========================================================
# TARGET SELECTION
# =========================================================
TARGET ?= hw

# =========================================================
# PLATFORM NAME
# =========================================================
PLATFORM_CUSTOM ?= vck190_thin

# =========================================================
# XSA SELECTION
# =========================================================
#relative hardware (XSA) directory
RELATIVE_XSA_DIR := ../hw/build/

XSA_DIR	:= $(shell readlink -f $(RELATIVE_XSA_DIR))
XSA := $(XSA_DIR)/$(PLATFORM_CUSTOM).xsa

# =========================================================
# SOURCE DIRECTORY
# =========================================================
SRC_DIR   := src

# =========================================================
# XPFM CREATION SCRIPT
# =========================================================
CREATE_XPFM_TCL := platform_sw.tcl

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS := $(CREATE_XPFM_TCL)
DEPS += $(XSA)
DEPS += $(SRC_DIR)/qemu/aie/pmc_args.txt
DEPS += $(SRC_DIR)/qemu/aie/qemu_args.txt
DEPS += $(SRC_DIR)/qemu/lnx/pmc_args.txt
DEPS += $(SRC_DIR)/qemu/lnx/qemu_args.txt

# =========================================================
# BUILD PRODUCTS
# ========================================================
BUILD_DIR := build
XPFM := $(PLATFORM_CUSTOM).xpfm

xpfm: $(XSA) $(BUILD_DIR)/$(PLATFORM_CUSTOM)/export/$(PLATFORM_CUSTOM)/$(XPFM)

$(XSA):
	$(MAKE) xsa -C ../hw

$(BUILD_DIR)/$(PLATFORM_CUSTOM)/export/$(PLATFORM_CUSTOM)/$(XPFM): $(DEPS)
	xsct -nodisp -sdx ./$(CREATE_XPFM_TCL) $(PLATFORM_CUSTOM) $(XSA) $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/$(XPFM)
	rm -rf $(BUILD_DIR) ./.Xil
