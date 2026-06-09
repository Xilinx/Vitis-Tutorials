#/*
#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
VERSION=2026.1
COMMON_IMAGE_VERSAL ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT ?= $(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-amd-linux/
PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202610_1/xilinx_vek280_base_202610_1.xpfm

AIE_WORK = $(ROOT_DIR)/aieml_work/

.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run AIE ML application in software emulation mode"
	@echo "make sd_card COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run AIE ML application on board"

sd_card:check-common-image aieml_hw
all: check-common-image aieml_emu
#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

cpAieCode:
	rm aieml_work -rf
	mkdir -p aieml_work
	cp $(XILINX_VITIS)//samples/aie_system_examples/aie-ml_sys_design/* aieml_work -a
	cp makefile_aieml aieml_work/Makefile

aieml_emu:  cpAieCode
	cp makefile_aieml $(AIE_WORK)/Makefile
	$(MAKE) -C $(AIE_WORK) run TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/
aieml_hw:  cpAieCode
	cp makefile_aieml $(AIE_WORK)/Makefile
	$(MAKE) -C $(AIE_WORK) clean
	$(MAKE) -C $(AIE_WORK) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)

clean:
	rm -rf $(DTB_PLT_NAME) $(PLATFORM_NAME) tmp  *journal.py __pycache__ logs ip_cache vitis_journal* 
	-$(MAKE) -C $(AIE_WORK) clean

ultraclean: clean
	-rm -rf $(ABS_BUILD_PATH) device-tree-xlnx/ logs tmp ws ./aieml-work
	make ultraclean  -C  $(AIE_WORK) 
