#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

XSA=step1_vivado/build/vivado/zcu104_custom_platform_hw.xsa
XPFM=step2_pfm/zcu104_custom/export/zcu104_custom/zcu104_custom.xpfm
VERSION				?=2023.2
COMMON_IMAGE_ZYNQMP 	?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
TARGET=
.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all  COMMON_IMAGE_ZYNQMP=<path/to/common_image/> 	## to generate platform with pre-built software components and do sw emulation with running vadd application to test this platform "
	@echo "make sd_card  COMMON_IMAGE_ZYNQMP=<path/to/common_image/> 	## to generate platform with pre-built software components and do hw test on board with running vadd application to test this platform "

all: check-common-image step1 step2  step3_hwemu_test

sd_card: check-common-image step1 step2 step3_hw_test

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_ZYNQMP)/Image),)
	@echo "'COMMON_IMAGE_ZYNQMP' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

# Step 1: Generate XSA with Vivado
step1: $(XSA)

$(XSA):
	$(MAKE) -C step1_vivado all


# Step 2: Generate Platform
step2: $(XPFM)

$(XPFM): $(XSA) 
	$(MAKE) -C step2_pfm all

# Step 3: Validate Design
step3_hwemu_test: $(XPFM)
	$(MAKE) all TARGET=hw_emu -C step3_validate

step3_hw_test: $(XPFM)
	$(MAKE) sd_card TARGET=hw -C step3_validate 

clean:
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_pfm clean
	$(MAKE) -C step3_validate clean
ultraclean:clean
	$(MAKE) -C step2_pfm ultraclean
	$(MAKE) -C step3_validate ultraclean
