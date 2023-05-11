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
PLATFORM_CUSTOM ?= vck190_thin
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
#  BOARD PART SELECTION (ES1/Production)
# =========================================================
ifeq ($(DEVICE_NAME), xcvc1902-vsva2197-2MP-e-S)
  BOARD_LABEL:=vck190
  BOARD_VER:=3.1
else ifeq ($(DEVICE_NAME), xcvc1902-vsva2197-2MP-e-S)
  BOARD_LABEL:=vck190_es
  BOARD_VER:=1.3
endif

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS_PLATFORM   := $(XSA_PLATFORM_TCL)

platform_classic: $(BUILD_DIR)/$(XSA_PLATFORM)

$(BUILD_DIR)/$(XSA_PLATFORM): $(DEPS_PLATFORM)
	vivado -mode batch -source $(XSA_PLATFORM_TCL) -tclargs $(PLATFORM_CUSTOM) $(DEVICE_NAME) $(PRE_SYNTH) $(BOARD_LABEL) $(BOARD_VER)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil vivado* .crash*
