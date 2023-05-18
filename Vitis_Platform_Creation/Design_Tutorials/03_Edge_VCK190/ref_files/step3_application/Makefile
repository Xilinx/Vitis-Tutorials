#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))

PLATFORM_NAME=custom_platform
PLATFORM = $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SW_COMP = $(ROOT_DIR)/../step2_pfm/sw_comp
SYSROOTDIR=$(ROOT_DIR)/../step2_pfm/sysroot
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/
AIE_EXAMPLE_PATH = Vitis_Accel_Examples/aie_kernels/aie_adder/

.PHONY: help  

help:
	@echo "Makefile usage"
	@echo "make all 	## Generate and package application related files"

all: vadd  aie_adder
sd_card:vadd_hw aie_adder_hw



Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git
	cd Vitis_Accel_Examples/  && sed -i "s/vck190_dfx_hw := true/	vck190_dfx_hw := true/g" cpp_kernels/simple_vadd/makefile_versal_ps.mk

$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(PLATFORM)  DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(SYSROOTDIR)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=hw_emu DEVICE=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux
aie_adder: Vitis_Accel_Examples
	$(MAKE) -C $(AIE_EXAMPLE_PATH) clean
	$(MAKE) -C $(AIE_EXAMPLE_PATH) run TARGET=hw_emu PLATFORM=$(PLATFORM) HOST_ARCH=aarch64  EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux
aie_adder_hw: Vitis_Accel_Examples
	$(MAKE) -C $(AIE_EXAMPLE_PATH) clean
	$(MAKE) -C $(AIE_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64  EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

clean:
	$(MAKE) -C $(PL_EXAMPLE_PATH) clean
	$(MAKE) -C $(AIE_EXAMPLE_PATH) clean

ultraclean:
	rm -rf Vitis_Accel_Examples
