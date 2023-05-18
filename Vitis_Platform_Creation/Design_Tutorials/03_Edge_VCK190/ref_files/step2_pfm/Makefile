#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
DTB_PLT_NAME=mydevice
DTB_LOCATION=$(ROOT_DIR)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
VERSION=2023.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
BOOT=boot
SD_DIR=sd_dir
SW_COMP=sw_comp
SYSROOT=$(ROOT_DIR)/sysroot
XSA_PATH=$(ROOT_DIR)/../step1_vivado/build/vivado/
XSA_NAME=custom_hardware_platform
PLATFORM_NAME=custom_platform
PLATFORM = $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOTDIR=$(ROOT_DIR)/$(SYSROOT)


.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run vadd application in software emulation mode"
	

all: check-common-image sw_comps dtb pfm sysroot getplatforminfo
sd_card:all

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

#target :software components
#copy software components to local directory
sw_comps:$(COMMON_IMAGE_VERSAL)/Image
	@echo "create folder to store sw components and copy software components to local directory ./boot and ./sd_dir"
	 mkdir -p $(SW_COMP) && mkdir  -p $(BOOT) && mkdir  -p $(SD_DIR)
	cp $(COMMON_IMAGE_VERSAL)/u-boot.elf  $(BOOT)/
	cp $(COMMON_IMAGE_VERSAL)/bl31.elf  $(BOOT)/
	cp $(COMMON_IMAGE_VERSAL)/boot.scr $(SD_DIR)/
	cp $(COMMON_IMAGE_VERSAL)/Image  $(SW_COMP)/
	cp $(COMMON_IMAGE_VERSAL)/rootfs.ext4  $(SW_COMP)/

# target: dtb
#generate the DTB file
#echo command is to avoid the return error code caused by the createdts command warning
dtb: $(XSA_PATH)/$(XSA_NAME)_hw.xsa
	@echo "execute createdts to generate dts file"
	xsct -eval " createdts -hw $(XSA_PATH)/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -git-branch xlnx_rel_v2023.1 -dtsi system-user.dtsi -compile"
	cp $(DTB_LOCATION)/system.dtb  $(BOOT)/
	
# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(SYSROOT)

$(SYSROOT): $(COMMON_IMAGE_VERSAL)/sdk.sh
	mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_VERSAL)/sdk.sh -d  $(SYSROOT)/ -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm: $(ROOT_DIR)/$(BOOT) $(ROOT_DIR)/$(SD_DIR)
	@echo "INFO: Creating Platform $(XSA_NAME)"
	xsct xsct_create_pfm.tcl $(PLATFORM_NAME) $(XSA_NAME) $(XSA_PATH) $(ROOT_DIR)/$(BOOT) $(ROOT_DIR)/$(SD_DIR) && \
	echo "Platform created:" && \
	find . -name "*.xpfm"

getplatforminfo:
	$(MAKE) -C platforminfo

clean:
	$(RM) -r $(BOOT) $(SD_DIR) $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE*
	 $(MAKE) -C platforminfo  clean

ultraclean:
	rm -rf device-tree-xlnx/  $(SW_COMP)  $(SYSROOT) Vitis_Accel_Examples
	
