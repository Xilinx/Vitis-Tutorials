#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
XSA_NAME=zcu104_custom_platform
PLATFORM_NAME=zcu104_custom
VERSION=2023.2
COMMON_IMAGE_ZYNQMP ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
DTB=$(ROOT_DIR)/mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb
SYSROOT=$(ROOT_DIR)/sysroot



.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_ZYNQMP=<path/to/common_image/>  	## to generate platform with pre-built software components"
	

all: check-common-image  dtb pfm sysroot
sd_card:all

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_ZYNQMP)/Image),)
	@echo "'COMMON_IMAGE_ZYNQMP' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif


#:DTB 
#generate dtb file
#echo command is to avoid the return error code caused by the createdts command warning	
dtb:
	@echo "execute createdts to generate dtb file"
	xsct -eval " createdts -hw ../step1_vivado/build/vivado/$(XSA_NAME)_hw.xsa -zocl  -platform-name mydevice -out .  -git-branch xlnx_rel_v2023.2 -board  zcu104-revc -dtsi system-user.dtsi -compile" 

#create platfrom 
pfm:
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	@echo "set place for vitis configuration file "
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path ../step1_vivado/build/vivado/$(XSA_NAME)_hw.xsa --xsa-emu_path ../step1_vivado/build/vivado/$(XSA_NAME)_hwemu.xsa --boot $(COMMON_IMAGE_ZYNQMP) --dtb $(DTB)
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
#install SDK tool
sysroot: $(SYSROOT)

$(SYSROOT): $(COMMON_IMAGE_ZYNQMP)/sdk.sh
	mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_ZYNQMP)/sdk.sh -d  $(SYSROOT)/ -y

clean:
	rm -rf  IDE.log $(PLATFORM_NAME) $(PLATFORM_NAME_FSBL) device-tree-xlnx  mydevice .Xil tmp/
ultraclean:clean
	rm -rf    $(SYSROOT)
