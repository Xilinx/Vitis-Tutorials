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
BOARD_NAME      ?= vck190
PLATFORM_CUSTOM ?= ${BOARD_NAME}_thin
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
SRC_DIR := ${BOARD_NAME}

# =========================================================
# XSA CREATION SCRIPT
# =========================================================
XSA_PLATFORM_TCL  := $(SRC_DIR)/xsa_platform_classic.tcl

# =========================================================
#  BOARD PART SELECTION (ES1/Production)
# =========================================================
ifeq ($(BOARD_NAME), vek280)
  BOARD_LABEL := vek280
  BOARD_VER   := 1.0
else ifeq ($(BOARD_NAME), vek280_es1)
  BOARD_LABEL := vek280_es
  BOARD_VER   := 1.1
else ifeq ($(BOARD_NAME), vck190)
  BOARD_LABEL := vck190
  BOARD_VER   := 3.1
endif

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS_PLATFORM   := $(XSA_PLATFORM_TCL)

platform_classic: $(BUILD_DIR)/$(XSA_PLATFORM)

$(BUILD_DIR)/$(XSA_PLATFORM): $(DEPS_PLATFORM)
	vivado -mode batch -source $(XSA_PLATFORM_TCL) -tclargs $(BOARD_NAME) $(PLATFORM_CUSTOM) $(DEVICE_NAME) $(PRE_SYNTH) $(BOARD_LABEL) $(BOARD_VER)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil vivado* .crash*
