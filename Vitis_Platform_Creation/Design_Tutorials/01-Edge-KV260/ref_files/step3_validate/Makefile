#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=kv260_custom
PLATFORM= $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SW_COMP = $(ROOT_DIR)/../step2_pfm/sw_comp/
VERSION=2023.2
COMMON_IMAGE_ZYNQMP ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
SYSROOT := $(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-xilinx-linux
VADD_WORK=$(ROOT_DIR)/vadd_work

.PHONY: help  

help:
	@echo "Makefile usage"
	@echo "make all 	## Generate and package application related files"

all: getplatforminfo vadd_hw_package


getplatforminfo:
	$(MAKE) -C platforminfo/ all
cpVaddCode:
	mkdir -p vadd_work
	cp $(XILINX_VITIS)/examples/vadd/* vadd_work
	cp makefile_vadd vadd_work/Makefile

# vadd_hw is a place-holder for manual build and test on hardware
$(VADD_WORK)/simple_vadd: vadd_hw
vadd_hw: cpVaddCode $(PLATFORM)
	$(MAKE) -C $(VADD_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_ZYNQMP) SYSROOT=$(SYSROOT)/
# vadd_hw generates the files for running on hardware. User needs to pick the files required by KV260, rename thr file and transfer them to the board
# Do not use the generated sd_card.img

$(VADD_WORK)/shell.json:
	cd $(VADD_WORK) && printf '{\n  "shell_type" : "XRT_FLAT",\n  "num_slots": "1"\n}' > shell.json

vadd_hw_package: $(VADD_WORK)/simple_vadd  $(VADD_WORK)/shell.json
	@echo "INFO: Packaging Completed"
	mkdir -p vadd
	cp ../step2_pfm/dtbo_output/pl.dtbo vadd/
	cp $(VADD_WORK)/shell.json vadd/
	cp $(VADD_WORK)/build_dir.hw/krnl_vadd.link.xclbin vadd/krnl_vadd.bin
	cp $(VADD_WORK)/simple_vadd vadd/
	@echo "Please  copy vadd to /lib/firmware/xilinx/ on target board"
	@echo "then use xmutil to load hardware."
	@echo "xmutil unloadapp"
	@echo "xmutil loadapp vadd"
	@echo "Go to /lib/firmware/xilinx/vadd directory and run application"
	@echo "$ ./simple_vadd ./krnl_vadd.bin"

getplatforminfo:
	$(MAKE) -C platforminfo/ all


clean:
	-$(MAKE) -C $(VADD_WORK) clean
	-$(MAKE) -C platforminfo/ clean
	rm -rf vadd/

ultraclean:clean
	rm -rf $(VADD_WORK)
