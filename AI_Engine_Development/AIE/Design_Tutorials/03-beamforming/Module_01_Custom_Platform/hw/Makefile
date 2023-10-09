
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# =========================================================
# DEVICE SELECTION
# =========================================================
DEVICE_NAME=xcvc1902-vsva2197-2MP-e-S

# =========================================================
# PLATFORM NAME
# =========================================================
PLATFORM_USE := vck190_v1_0

# =========================================================
# XSA NAME
# =========================================================
XSA = $(PLATFORM_USE}.xsa

# =========================================================
# SOURCE DIRECTORIES
# =========================================================
SRC_DIR := src
QOR_SCRIPTS_DIR := $(SRC_DIR)/qor_scripts

# =========================================================
# XSA CREATION SCRIPT
# =========================================================
CREATE_XSA_TCL := $(PLATFORM_USE)_xsa.tcl

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS := $(CREATE_XSA_TCL)
DEPS += pfm_decls.tcl
DEPS += $(QOR_SCRIPTS_DIR)/post_place.tcl
DEPS += $(QOR_SCRIPTS_DIR)/pre_place.tcl
DEPS += $(QOR_SCRIPTS_DIR)/post_route.tcl
DEPS += $(QOR_SCRIPTS_DIR)/post_physopt.tcl
DEPS += $(QOR_SCRIPTS_DIR)/timing.xdc
DEPS += $(SRC_DIR)/post_link_pfm_config.tcl
DEPS += $(SRC_DIR)/dr.bd.tcl
DEPS += $(SRC_DIR)/ddr4_dmc_3200_triplet_1.xdc

# =========================================================
# BUILD PRODUCTS 
# ========================================================
BUILD_DIR := build
XSA := $(PLATFORM_USE).xsa

xsa: $(BUILD_DIR)/$(XSA)

$(BUILD_DIR)/$(XSA): $(DEPS)
	vivado -mode batch -source $(CREATE_XSA_TCL) -tclargs $(DEVICE_NAME)

clean:
	rm -rf $(BUILD_DIR)/$(XSA)
	rm -rf $(BUILD_DIR) vivado*.jou vivado*.log
	rm -rf .Xil
