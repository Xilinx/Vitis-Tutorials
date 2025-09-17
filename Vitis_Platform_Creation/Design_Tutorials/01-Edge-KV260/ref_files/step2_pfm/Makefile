#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=kv260_custom

XSA_NAME=kv260_hardware_platform
VERSION=2025.2
COMMON_IMAGE_ZYNQMP ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/




.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_ZYNQMP=<path/to/common_image/>  	## Generate platform with pre-built software components"
	

all: check-common-image  pfm 

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_ZYNQMP)/Image),)
	@echo "'COMMON_IMAGE_ZYNQMP' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif



#create platfrom 
pfm:
	
	@echo "INFO: Creating  Platform $(PLATFORM_NAME)"
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path ../step1_vivado/build/vivado/$(XSA_NAME).xsa --boot $(COMMON_IMAGE_ZYNQMP) 
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"

 

clean:
	rm -rf IDE.log $(PLATFORM_NAME) $(DT_PLAT_NAME) 

ultraclean: clean
	rm -rf   device-tree-xlnx tmp logs ws
