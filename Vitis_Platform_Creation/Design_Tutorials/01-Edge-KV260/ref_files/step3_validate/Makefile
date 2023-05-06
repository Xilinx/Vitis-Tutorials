#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=kv260_custom
PLATFORM= $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SW_COMP = $(ROOT_DIR)/../step2_pfm/sw_comp/
SYSROOTDIR=$(ROOT_DIR)/../step2_pfm/sysroot/

PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/

.PHONY: help  

help:
	@echo "Makefile usage"
	@echo "make all 	## Generate and package application related files"

all: getplatforminfo vadd_hw_package


getplatforminfo:
	$(MAKE) -C platforminfo/ all


Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git

# Quick command for generating vadd hw xclbin
vadd_hw: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux
# vadd_hw generates the files for running on hardware. User needs to pick the files required by KV260, rename thr file and transfer them to the board
# Do not use the generated sd_card.img
$(PL_EXAMPLE_PATH)/simple_vadd:   Vitis_Accel_Examples $(PLATFORM)  
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=zynquplus HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux


$(PL_EXAMPLE_PATH)/shell.json:
	cd $(PL_EXAMPLE_PATH) && printf '{\n  "shell_type" : "XRT_FLAT",\n  "num_slots": "1"\n}' > shell.json

vadd_hw_package: $(PL_EXAMPLE_PATH)/simple_vadd  $(PL_EXAMPLE_PATH)/shell.json
	@echo "INFO: Packaging Completed"
	mkdir -p vadd
	cp ../step2_pfm/dtbo_output/pl.dtbo vadd/
	cp $(PL_EXAMPLE_PATH)/shell.json vadd/
	cp $(PL_EXAMPLE_PATH)/build_dir.hw.$(PLATFORM_NAME)/krnl_vadd.link.xclbin vadd/krnl_vadd.bin
	cp $(PL_EXAMPLE_PATH)/simple_vadd vadd/
	@echo "Please  copy vadd to /lib/firmware/xilinx/ on target board"
	@echo "then use xmutil to load hardware."
	@echo "xmutil unloadapp"
	@echo "xmutil loadapp vadd"
	@echo "Go to /lib/firmware/xilinx/vadd directory and run application"
	@echo "$ ./simple_vadd ./krnl_vadd.bin"




clean:
	-$(MAKE) -C $(PL_EXAMPLE_PATH) clean
	-$(MAKE) -C platforminfo/ clean
	rm -rf vadd/

ultraclean:clean
	rm -rf Vitis_Accel_Examples
