#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=zcu104_custom
PLATFORM ?= $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm

TARGET ?=hw_emu 
VERSION=2023.2

COMMON_IMAGE_ZYNQMP ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
SYSROOT := $(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-xilinx-linux
VADD_WORK=$(ROOT_DIR)/vadd_work



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

cpVaddCode:
	mkdir -p vadd_work
	cp $(XILINX_VITIS)/examples/vadd/* vadd_work
	cp makefile_vadd vadd_work/Makefile


vadd_emu: cpVaddCode 
	$(MAKE) -C $(VADD_WORK) sd_card TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_ZYNQMP) SYSROOT=$(SYSROOT)/

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: cpVaddCode 
	$(MAKE) -C $(VADD_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_ZYNQMP) SYSROOT=$(SYSROOT)/

platforminfo:
	$(MAKE) -C platforminfo

clean:
	-$(MAKE) -C $(VADD_WORK) clean
	-$(MAKE) -C platforminfo  clean



ultraclean:
	rm -rf logs 
	rm -rf $(VADD_WORK)
