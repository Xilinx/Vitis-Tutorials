#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# DEVICE SELECTION
# =========================================================
DEVICE_NAME ?= xcvc1902-vsva2197-2MP-e-S

# =========================================================
# PLATFORM NAME
# =========================================================
PLATFORM_CUSTOM ?= custom_platform_vck190
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
#  BOARD PART SELECTION (Production)
# =========================================================
  BOARD_LABEL:=vck190
  BOARD_VER:=3.3

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS_PLATFORM   := $(XSA_PLATFORM_TCL)

platform_classic: $(BUILD_DIR)/$(XSA_PLATFORM)

$(BUILD_DIR)/$(XSA_PLATFORM): $(DEPS_PLATFORM)
	vivado -mode batch -source $(XSA_PLATFORM_TCL) -tclargs $(PLATFORM_CUSTOM) $(DEVICE_NAME) $(PRE_SYNTH) $(BOARD_LABEL) $(BOARD_VER) $(XSA_PLATFORM)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil vivado* .crash*
