#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
VERSION=2026.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT=$(ROOT_DIR)/sysroot
XSA_PATH=$(ROOT_DIR)/../step1_vivado/build/vivado/
XSA_NAME=custom_hardware_platform
PLATFORM_NAME=custom_platform
PLATFORM = $(ROOT_DIR)/ws/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOTDIR=$(ROOT_DIR)/$(SYSROOT)


.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run vadd application in software emulation mode"
	

all: check-common-image pfm
sd_card:all

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm:
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	@echo "set place for vitis to store configuration file "
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path $(XSA_PATH)/$(XSA_NAME)_hw.xsa --xsa-emu_path $(XSA_PATH)/$(XSA_NAME)_hwemu.xsa --boot $(COMMON_IMAGE_VERSAL) --user_dtsi $(ROOT_DIR)/system-user.dtsi
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
	echo "Platform created:" && \
	find . -name "*.xpfm"

getplatforminfo:
	$(MAKE) -C platforminfo

clean:
	$(RM) -rf  $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE* tmp ws
	-$(MAKE) -C platforminfo  clean

ultraclean:clean
	rm -rf  ws/
	
