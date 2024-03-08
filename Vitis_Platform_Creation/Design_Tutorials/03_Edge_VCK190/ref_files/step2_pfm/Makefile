#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
DTB_PLT_NAME=mydevice
DTB_LOCATION=$(ROOT_DIR)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
DTB=$(DTB_LOCATION)/system.dtb
VERSION=2024.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
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
	

all: check-common-image dtb pfm sysroot 
sd_card:all

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

# target: dtb
#generate the DTB file
#echo command is to avoid the return error code caused by the createdts command warning
dtb: $(XSA_PATH)/$(XSA_NAME)_hw.xsa
	@echo "execute createdts to generate dts file"
	xsct -eval " createdts -hw $(XSA_PATH)/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -git-branch xlnx_rel_v2024.1 -dtsi system-user.dtsi -compile"

	
# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(SYSROOT)

$(SYSROOT): $(COMMON_IMAGE_VERSAL)/sdk.sh
	mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_VERSAL)/sdk.sh -d  $(SYSROOT)/ -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm:
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	@echo "set place for vitis to store configuration file "
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path $(XSA_PATH)/$(XSA_NAME)_hw.xsa --xsa-emu_path $(XSA_PATH)/$(XSA_NAME)_hwemu.xsa --boot $(COMMON_IMAGE_VERSAL) --dtb $(DTB)
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
	echo "Platform created:" && \
	find . -name "*.xpfm"

getplatforminfo:
	$(MAKE) -C platforminfo

clean:
	$(RM) -rf  $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE* tmp 
	-$(MAKE) -C platforminfo  clean

ultraclean:clean
	rm -rf device-tree-xlnx/   $(SYSROOT) 
	
