#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=kv260_custom

XSA_NAME=kv260_hardware_platform
VERSION=2024.1
COMMON_IMAGE_ZYNQMP ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
DT_PLAT_NAME=mydeivice
DT_PATH=$(ROOT_DIR)/$(DT_PLAT_NAME)/psu_cortexa53_0/device_tree_domain/bsp
DTB =$(ROOT_DIR)/$(DT_PLAT_NAME)/psu_cortexa53_0/device_tree_domain/bsp/system.dtb

DEVICE_TREE_REPO ?=

#ifndef DEVICE_TREE_REPO
DEVICE_TREE_REPO=https://github.com/Xilinx/device-tree-xlnx.git
#endif


.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_ZYNQMP=<path/to/common_image/>  	## Generate platform with pre-built software components"
	

all: check-common-image dtb dtbo pfm 

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_ZYNQMP)/Image),)
	@echo "'COMMON_IMAGE_ZYNQMP' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif



dtb:
	@echo "execute createdts to generate dtb file"
	xsct -eval " createdts -hw ../step1_vivado/build/vivado/$(XSA_NAME).xsa -zocl  -platform-name $(DT_PLAT_NAME) -out .  -git-branch xlnx_rel_v2024.1  -overlay -compile"

dtbo:$(DT_PATH)/pl.dtsi
	dtc -@ -O dtb -o $(DT_PATH)/pl.dtbo $(DT_PATH)/pl.dtsi
	mkdir -p dtbo_output && cp $(DT_PATH)/pl.dtbo dtbo_output/


#create platfrom 
pfm:
	
	@echo "INFO: Creating  Platform $(PLATFORM_NAME)"
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path ../step1_vivado/build/vivado/$(XSA_NAME).xsa --boot $(COMMON_IMAGE_ZYNQMP) --dtb $(DTB)
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"

 

clean:
	rm -rf IDE.log $(PLATFORM_NAME) $(DT_PLAT_NAME) dt_output dtbo_output

ultraclean: clean
	rm -rf   device-tree-xlnx tmp logs
