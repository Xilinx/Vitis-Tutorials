#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=zcu104_custom
PLATFORM ?= $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOT=$(ROOT_DIR)/../step2_pfm/sysroot/
TARGET ?=hw_emu 
VERSION=2023.2
COMMON_IMAGE_ZYNQMP ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/



.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all TARGET=hw_emu/sw_emu	## to build vadd application and run hw/sw emulation on the platform created in former step"
	@echo "make sd_card 					## to build vadd application and run hw test on the platform created in former step"

all:check-common-image vadd 
sd_card:check-common-image vadd_hw

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_ZYNQMP)/Image),)
	@echo "'COMMON_IMAGE_ZYNQMP' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git

vadd: Vitis_Accel_Examples $(PLATFORM)
	$(MAKE) -C $(PL_EXAMPLE_PATH) run TARGET=$(TARGET) PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus  EDGE_COMMON_SW=$(COMMON_IMAGE_ZYNQMP) SYSROOT=$(SYSROOT)sysroots/cortexa72-cortexa53-xilinx-linux

vadd_hw: Vitis_Accel_Examples $(PLATFORM)
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus  EDGE_COMMON_SW=$(COMMON_IMAGE_ZYNQMP) SYSROOT=$(SYSROOT)sysroots/cortexa72-cortexa53-xilinx-linux


platforminfo:
	$(MAKE) -C platforminfo

clean:
	-$(MAKE) -C $(PL_EXAMPLE_PATH) clean
	-$(MAKE) -C platforminfo  clean

ultraclean:clean
	rm -rf Vitis_Accel_Examples 
