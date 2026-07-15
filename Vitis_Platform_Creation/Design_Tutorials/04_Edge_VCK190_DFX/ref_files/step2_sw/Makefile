#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

# User editing area start
XSA_NAME=vck190_custom_dfx
DTB_PLT_NAME=vck190_custom_dt
PLATFORM_NAME=vck190_custom_dfx
PLATFORM := $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm

# User editing area end
# -------------------
ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
STATIC_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_static.xsa
RP_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_rp.xsa
HW_EMU_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_hw_emu.xsa
BOOT_DIR=$(ABS_BUILD_PATH)/boot
SD_DIR=$(ABS_BUILD_PATH)/sd_dir
USER_DTSI=$(ROOT_DIR)/system-user.dtsi
VERSION=2026.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/

SW_COMP_PATH=$(ABS_BUILD_PATH)/sw_comp
SYSROOT_DIR=$(SW_COMP_PATH)/sysroot
SYSROOT=$(SYSROOT_DIR)

BOOT_BIN_DIR=$(ABS_BUILD_PATH)/boot_bin
BOOT_IMAGE=$(ABS_BUILD_PATH)/boot/BOOT.BIN
SW_FILES=$(SD_DIR)/boot.scr $(BOOT_DIR)/u-boot.elf $(BOOT_DIR)/bl31.elf
PLATFORM_OUT_PATH=$(ABS_BUILD_PATH)

DEVICE_TREE_REPO ?=

ifndef DEVICE_TREE
DEVICE_TREE_REPO=https://github.com/Xilinx/device-tree-xlnx.git
endif
.PHONY: all sw_comp dtb help pfm bootimage sysroot

# --------------------------------------
# Public targets
# all: prepare software components and build the platform
# sysroot: install sysroot from sdk.sh
# --------------------------------------

all: sw_comp pfm sysroot
sd_card: all

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run vadd application in software emulation mode"
	


check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif
	@ec:who "INFO: Using COMMON_IMAGE_VERSAL=$(COMMON_IMAGE_VERSAL)"

# target :software components
# copy software components to local directory
sw_comp: $(SW_FILES)

$(SW_FILES):
	@echo "INFO: Copying components from common image and dtb output to boot and SD dir for FAT32 partition"
	mkdir -p $(BOOT_DIR) $(SD_DIR) $(SW_COMP_PATH)
	cp -f $(COMMON_IMAGE_VERSAL)/bl31.elf $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/u-boot.elf $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/boot.scr $(SD_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/Image  $(SW_COMP_PATH)
	cp -f $(COMMON_IMAGE_VERSAL)/rootfs.ext4  $(SW_COMP_PATH)

 
pfm:
	@echo "INFO: Creating Platform $(PLATFORM_NAME) in mode: $(MODE)"
	rm -rf $(PLATFORM_NAME)
	@echo "Set place for Vitis to store configuration files"
	@mkdir -p tmp && export XILINX_VITIS_DATA_DIR="tmp"
	mkdir -p $(PLATFORM_OUT_PATH)
	vitis -s generate_platform.py \
		--platform_name $(PLATFORM_NAME) \
		--static_xsa_path $(STATIC_XSA) \
		--emu_xsa_path "${HW_EMU_XSA}" \
		--platform_out "${PLATFORM_OUT_PATH}" \
		--boot_dir_path "${BOOT_DIR}" \
		--sd_dir_path "${SD_DIR}" \
		--rp_xsa_path "${RP_XSA}" \
		--user_dtsi "$(USER_DTSI)"	
	@echo "INFO: Platform created:" && find . -name "*.xpfm"

	$(info "Generating BOOT image for platforms")
	cp -rf $(ROOT_DIR)/$(PLATFORM_NAME)/hw/linux_dt/system.dtb $(BOOT_DIR)
	bootgen -arch versal -image $(ROOT_DIR)/bootgen.bif -o $(BOOT_IMAGE) -w

	@echo 'Platform build started'
	vitis -s ./build_platform.py --platform_name $(PLATFORM_NAME) --platform_out $(ROOT_DIR)  --boot_image $(BOOT_IMAGE)

	@echo 'Platform build complete'
	
clean:
	$(RM) -r $(ABS_BUILD_PATH)/$(DTB_PLT_NAME) $(BOOT_DIR) $(SD_DIR) $(BOOT_BIN_DIR) $(PLATFORM_OUT_PATH) $(PLATFORM_NAME)

ultraclean:
	$(RM) -r $(ABS_BUILD_PATH) 
