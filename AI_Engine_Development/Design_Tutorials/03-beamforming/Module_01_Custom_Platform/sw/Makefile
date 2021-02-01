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
DEVICE_NAME=xcvc1902-vsva2197-1LP-e-S-es1

# =========================================================
# PLATFORM NAME
# =========================================================
PLATFORM := vck190_v1_0

# =========================================================
# XSA SELECTION
# =========================================================
#relative hardware (XSA) directory
RELATIVE_XSA_DIR := ../hw/build/

#absolute XSA directory = <user path>/Tutorials/AI_Engine/beamforming/Module_01_Custom_Platform/hw/build
XSA_DIR	:= $(shell readlink -f $(RELATIVE_XSA_DIR))
XSA := $(XSA_DIR)/$(PLATFORM).xsa

# =========================================================
# SOURCE DIRECTORY
# =========================================================
SRC_DIR := src

# =========================================================
# XPFM CREATION SCRIPT
# =========================================================
CREATE_XPFM_TCL := $(PLATFORM)_pfm.tcl

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
XPFM := $(PLATFORM).xpfm

xpfm: $(XSA) $(BUILD_DIR)/$(PLATFORM)/export/$(PLATFORM)/$(XPFM)

$(XSA): 
	$(MAKE) xsa -C ../hw

$(BUILD_DIR)/$(PLATFORM)/export/$(PLATFORM)/$(XPFM): $(DEPS)
	xsct -nodisp -sdx ./$(CREATE_XPFM_TCL) $(PLATFORM) $(XSA) $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/$(XPFM)
	rm -rf $(BUILD_DIR)
