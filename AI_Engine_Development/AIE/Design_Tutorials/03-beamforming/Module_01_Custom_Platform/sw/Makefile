
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
# XSA SELECTION
# =========================================================
#relative hardware (XSA) directory
RELATIVE_XSA_DIR := ../hw/build/

#absolute XSA directory = <user path>/Tutorials/AI_Engine/beamforming/Module_01_Custom_Platform/hw/build
XSA_DIR	:= $(shell readlink -f $(RELATIVE_XSA_DIR))
XSA := $(XSA_DIR)/$(PLATFORM_USE).xsa

# =========================================================
# SOURCE DIRECTORY
# =========================================================
SRC_DIR := src

# =========================================================
# XPFM CREATION SCRIPT
# =========================================================
CREATE_XPFM_TCL := $(PLATFORM_USE)_pfm.tcl

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS := $(CREATE_XPFM_TCL)
DEPS += $(XSA)
DEPS += $(SRC_DIR)/qemu/aie/pmc_args.txt
DEPS += $(SRC_DIR)/qemu/aie/qemu_args.txt
DEPS += $(SRC_DIR)/qemu/lnx/pmc_args.txt
DEPS += $(SRC_DIR)/qemu/lnx/qemu_args.txt
DEPS += $(SRC_DIR)/boot/bl31.elf
DEPS += $(SRC_DIR)/boot/system.dtb
DEPS += $(SRC_DIR)/boot/u-boot.elf
DEPS += $(SRC_DIR)/a72/xrt/linux.bif

# =========================================================
# BUILD PRODUCTS 
# ========================================================
BUILD_DIR := build
XPFM := $(PLATFORM_USE).xpfm

xpfm: $(XSA) $(BUILD_DIR)/$(PLATFORM_USE)/export/$(PLATFORM_USE)/$(XPFM)

$(XSA): 
	$(MAKE) xsa -C ../hw

$(BUILD_DIR)/$(PLATFORM_USE)/export/$(PLATFORM_USE)/$(XPFM): $(DEPS)
	xsct -nodisp -sdx ./$(CREATE_XPFM_TCL) $(PLATFORM_USE) $(XSA) $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/$(XPFM)
	rm -rf $(BUILD_DIR) .Xil
