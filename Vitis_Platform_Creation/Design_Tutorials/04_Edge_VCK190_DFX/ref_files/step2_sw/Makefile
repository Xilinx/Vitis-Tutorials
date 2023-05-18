#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

# User editing area start
XSA_NAME=vck190_custom_dfx
DTB_PLT_NAME=vck190_custom_dt
PLATFORM_NAME=vck190_custom_dfx


# User editing area end
# -------------------
ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
STATIC_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_static.xsa
RP_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_rp.xsa
HW_EMU_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_hw_emu.xsa
DT_PATH=$(ABS_BUILD_PATH)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
DTB_FILE=$(DT_PATH)/system.dtb
BOOT_DIR=$(ABS_BUILD_PATH)/boot
SD_DIR=$(ABS_BUILD_PATH)/sd_dir
VERSION=2023.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/

SW_COMP_PATH=$(ABS_BUILD_PATH)/sw_comp
SYSROOT_DIR=$(SW_COMP_PATH)/sysroot
SYSROOT=$(SYSROOT_DIR)

BOOT_BIN_DIR=$(ABS_BUILD_PATH)/boot_bin
BOOT_IMAGE=$(ABS_BUILD_PATH)/boot_bin/BOOT.BIN
SW_FILES=$(SD_DIR)/boot.scr $(BOOT_DIR)/u-boot.elf $(BOOT_DIR)/bl31.elf
PDI_FILE=$(BOOT_BIN_DIR)/boot_static.pdi
PLATFORM_OUT_PATH=$(ABS_BUILD_PATH)/pfm

.PHONY: all sw_comp dtb help pfm bootimage sysroot

# --------------------------------------
# Public targets
# all: prepare software components and build the platform
# sysroot: install sysroot from sdk.sh
# --------------------------------------

all: $(DTB_FILE) sw_comp pfm sysroot
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
	@echo "INFO: Using COMMON_IMAGE_VERSAL=$(COMMON_IMAGE_VERSAL)"

# Target: dtb
# generate the device tree blob
dtb: $(DTB_FILE)
#echo command is to avoid the return error code caused by the createdts command warning
$(DTB_FILE): $(STATIC_XSA) system-user.dtsi
	$(info executing createdts to generate dts file)
	xsct -eval "createdts -hw $(STATIC_XSA) -out $(ABS_BUILD_PATH) -zocl  -platform-name $(DTB_PLT_NAME)  -git-branch xlnx_rel_v2023.1 -board versal-vck190-reva-x-ebm-01-reva -dtsi system-user.dtsi -compile" 
	


# target :software components
# copy software components to local directory
sw_comp: $(SW_FILES)

$(SW_FILES): $(DTB_FILE)
	@echo "INFO: Copying components from common image and dtb output to boot and SD dir for FAT32 partition"
	mkdir -p $(BOOT_DIR) $(SD_DIR) $(SW_COMP_PATH)
	cp -f $(DTB_FILE) $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/bl31.elf $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/u-boot.elf $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/boot.scr $(SD_DIR)
	cp -f $(COMMON_IMAGE_VERSAL)/Image  $(SW_COMP_PATH)
	cp -f $(COMMON_IMAGE_VERSAL)/rootfs.ext4  $(SW_COMP_PATH)

# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(SYSROOT)

$(SYSROOT): $(COMMON_IMAGE_VERSAL)/sdk.sh
	mkdir -p $(SYSROOT_DIR) && sh $(COMMON_IMAGE_VERSAL)/sdk.sh -d $(SYSROOT_DIR)/ -y

#----------
$(PDI_FILE): $(STATIC_XSA)
	$(info "Extracting PDI from static XSA")
	mkdir -p $(BOOT_BIN_DIR) 
	cp $(STATIC_XSA) $(BOOT_BIN_DIR)
	cd $(BOOT_BIN_DIR) && xsct -eval "openhw $(XSA_NAME)_static.xsa" && mv *.pdi boot_static.pdi
	

pdi: $(PDI_FILE)

$(BOOT_IMAGE): $(PDI_FILE) $(SW_FILES) $(DTB_FILE)
	$(info "BOOT image for platforms")
	bootgen -arch versal -image bootgen.bif -o $(BOOT_IMAGE) -w;

bootimage: $(BOOT_IMAGE)
#----------


# 
pfm: $(STATIC_XSA) $(RP_XSA) $(SW_FILES) ${BOOT_IMAGE}
	$(info Creating Platform $(XSA_NAME))
	xsct xsct_create_pfm.tcl platform_name "${PLATFORM_NAME}" \
		static_xsa_path "${STATIC_XSA}" \
		rp_xsa_path "${RP_XSA}" \
		boot_dir_path "${BOOT_DIR}" \
		boot_bin "${BOOT_IMAGE}" \
		emu_xsa_path "${HW_EMU_XSA}" \
		sd_dir_path "${SD_DIR}" \
		platform_out "${PLATFORM_OUT_PATH}"

clean:
	$(RM) -r $(ABS_BUILD_PATH)/$(DTB_PLT_NAME) $(BOOT_DIR) $(SD_DIR) $(BOOT_BIN_DIR) $(PLATFORM_OUT_PATH)

ultraclean:
	$(RM) -r $(ABS_BUILD_PATH) 
