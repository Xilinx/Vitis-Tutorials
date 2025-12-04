#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# DEVICE SELECTION
# =========================================================
DEVICE_NAME ?= xc2ve3858-ssva2112-2MP-e-S

# =========================================================
# PLATFORM NAME
# =========================================================
PLATFORM_CUSTOM ?= custom_platform_vek385
PRE_SYNTH       := true

# =========================================================
# XSA NAME
# =========================================================
XSA_PLATFORM    := xsa_platform/${PLATFORM_CUSTOM}.xsa

# =========================================================
# BUILD PRODUCTS
# ========================================================
BUILD_DIR := build

# =========================================================
# SOURCE DIRECTORIES
# =========================================================
SRC_DIR := src

# =========================================================
# XSA CREATION SCRIPT
# =========================================================
XSA_PLATFORM_TCL  := $(SRC_DIR)/xsa_platform_classic.tcl

# =========================================================
#  BOARD PART SELECTION
# =========================================================
  BOARD_LABEL:=vek385
  BOARD_VER:=1.1

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS_PLATFORM   := $(XSA_PLATFORM_TCL)

platform_vivado: $(BUILD_DIR)/$(XSA_PLATFORM)

$(BUILD_DIR)/$(XSA_PLATFORM): $(DEPS_PLATFORM)
	vivado -mode batch -source $(XSA_PLATFORM_TCL) -tclargs $(PLATFORM_CUSTOM) $(DEVICE_NAME) $(PRE_SYNTH) $(BOARD_LABEL) $(BOARD_VER) $(XSA_PLATFORM)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil vivado* .crash*
