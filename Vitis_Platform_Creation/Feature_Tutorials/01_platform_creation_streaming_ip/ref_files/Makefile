#/*
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
VERSION=2025.2
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)
XSA_PATH=$(ROOT_DIR)/build/vivado/
XSA_NAME=custom_hardware_platform
XSA=$(XSA_PATH)/$(XSA_NAME)_hw.xsa
PLATFORM_NAME=custom_platform
CUSTOM_PLATFORM = $(ROOT_DIR)/ws/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOT := $(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-amd-linux
SDKTARGETSYSROOT ?= ${SYSROOT}

SRC=$(ROOT_DIR)/vadd
TARGET=hw_emu

EXECUTABLE = ./simple_vadd
EMCONFIG_DIR = $(TEMP_DIR)
.PHONY: help

all: ultraclean hw check-common-image pfm vadd_emu
sd_card: ultraclean hw check-common-image pfm vadd_hw

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>	## to run the APPS with streaming IP support"



#generate the XSA file 
$(XSA): hw
hw:
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -source 


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
$(CUSTOM_PLATFORM): pfm
pfm:  
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	@echo "set place for vitis configuration file "
	rm -rf ./ws/$(PLATFORM_NAME)
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path $(XSA_PATH)/$(XSA_NAME)_hw.xsa --xsa-emu_path $(XSA_PATH)/$(XSA_NAME)_hwemu.xsa --boot $(COMMON_IMAGE_VERSAL)
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
	echo "Platform created:" && \
	find . -name "*.xpfm"

cpCode:
	mkdir -p vadd
	cp -r $(XILINX_VITIS)/samples/vadd/* vadd
	cp makefile_vadd vadd/Makefile
	cd vadd && patch -p2  < ../code_modified.patch

vadd_emu: cpCode 
	$(MAKE) -C $(SRC) run TARGET=hw_emu PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: cpCode 
	$(MAKE) -C $(SRC) sd_card TARGET=hw PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/


clean:
	-$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou *.log ./ws/$(PLATFORM_NAME) system.dtb
	-$(RM) -r  $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE*
	
ultraclean: clean
	$(RM) -rf $(SRC)  device-tree-xlnx logs tmp 
