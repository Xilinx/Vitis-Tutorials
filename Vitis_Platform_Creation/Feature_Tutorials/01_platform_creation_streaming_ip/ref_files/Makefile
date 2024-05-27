#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
DTB_PLT_NAME=mydevice
DTB_LOCATION=$(ROOT_DIR)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
DTB=$(DTB_LOCATION)/system.dtb
VERSION=2024.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
XSA_PATH=$(ROOT_DIR)/build/vivado/
XSA_NAME=custom_hardware_platform
XSA=$(XSA_PATH)/$(XSA_NAME)_hw.xsa
PLATFORM_NAME=custom_platform
CUSTOM_PLATFORM = $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOT := $(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux

DEVICE_TREE_REPO ?=

#ifndef DEVICE_TREE_REPO
DEVICE_TREE_REPO=https://github.com/Xilinx/device-tree-xlnx.git
#endif


SRC=$(ROOT_DIR)/src
TARGET=hw_emu



EXECUTABLE = ./simple_vadd
EMCONFIG_DIR = $(TEMP_DIR)
.PHONY: help

all: hw check-common-image  dtb pfm vadd_emu
sd_card:clean hw check-common-image  dtb pfm vadd_hw

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


# target: dtb
#generate the DTB file
#echo command is to avoid the return error code caused by the createdts command warning
$(DTB): dtb
dtb: $(XSA_PATH)/$(XSA_NAME)_hw.xsa
	@echo "execute createdts to generate dts file"
	xsct -eval " createdts -hw $(XSA_PATH)/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -dtsi system-user.dtsi -git-branch xlnx_rel_v2024.1 -compile" 
	

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
$(CUSTOM_PLATFORM): pfm
pfm:  
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	@echo "set place for vitis configuration file "
	rm -rf $(PLATFORM_NAME)
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path $(XSA_PATH)/$(XSA_NAME)_hw.xsa --xsa-emu_path $(XSA_PATH)/$(XSA_NAME)_hwemu.xsa --boot $(COMMON_IMAGE_VERSAL) --dtb $(DTB)
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
	echo "Platform created:" && \
	find . -name "*.xpfm"



cpCode:
	mkdir -p src
	cp $(XILINX_VITIS)/examples/vadd/* src
	cd src && patch -p1  < ../code_modified.patch
	cp makefile_vadd src/Makefile


vadd_emu: cpCode 
	$(MAKE) -C $(SRC) run TARGET=hw_emu PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: cpCode 
	$(MAKE) -C $(SRC) sd_card TARGET=hw PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/


clean:
	-$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou *.log 
	-$(RM) -r  $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE*
	
ultraclean: clean
	$(RM) -rf $(SRC)  device-tree-xlnx logs tmp 
