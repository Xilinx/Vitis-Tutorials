#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))

PLATFORM_NAME=custom_platform
PLATFORM = $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
VERSION=2024.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux/

VADD_WORK=$(ROOT_DIR)/vadd_work
AIE_WORK=$(ROOT_DIR)/aie_work
.PHONY: help  

help:
	@echo "Makefile usage"
	@echo "make all 	## Generate and package application related files"

all: vadd_emu  aie_adder_emu
sd_card:vadd_hw aie_adder_hw

#Copy code from local vitis installer
cpAieCode:
	rm aie_work -rf
	mkdir -p aie_work
	cp $(XILINX_VITIS)//samples/aie_system_examples/aie_sys_design/* aie_work -a
	cp makefile_aie aie_work/Makefile

cpVaddCode:
	mkdir -p vadd_work
	cp $(XILINX_VITIS)/examples/vadd/* vadd_work
	cp makefile_vadd vadd_work/Makefile




vadd_emu: cpVaddCode
	$(MAKE) -C $(VADD_WORK) run TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: cpVaddCode
	$(MAKE) -C $(VADD_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/
aie_adder_emu: cpAieCode
	$(MAKE) -C $(AIE_WORK) run TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/

aie_adder_hw: cpAieCode
	$(MAKE) -C $(AIE_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/


clean:
	$(MAKE) -C $(VADD_WORK) clean
	$(MAKE) -C $(AIE_WORK) clean

ultraclean:
	rm -rf logs 
	rm -rf $(VADD_WORK) $(AIE_WORK)
	rm -rf $(VADD_WORK) $(VADD_WORK)