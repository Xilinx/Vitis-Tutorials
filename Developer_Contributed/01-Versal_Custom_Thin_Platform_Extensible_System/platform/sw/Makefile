# © Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
SRC_DIR := src
PLNX_DIR := ../../petalinux

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
DEPS += $(PLNX_DIR)/linux/images/linux/bl31.elf
DEPS += $(PLNX_DIR)/linux/images/linux/system.dtb
DEPS += $(PLNX_DIR)/linux/images/linux/u-boot.elf
DEPS += $(PLNX_DIR)/src/boot_custom.bif

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
