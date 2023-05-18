#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=zcu104_custom
PLATFORM ?= $(ROOT_DIR)/../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SW_COMP = $(ROOT_DIR)/../step2_pfm/sw_comp/
SYSROOT=$(ROOT_DIR)/../step2_pfm/sysroot/
TARGET ?=sw_emu 
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/

VAI_DPU_TRD_PATH = Vitis-AI/dsa/DPU-TRD/prj/Vitis/

.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all TARGET=hw_emu/sw_emu	## to build vadd application and run hw/sw emulation on the platform created in former step"
	@echo "make sd_card 					## to build vadd application and run hw test on the platform created in former step"

all: platforminfo vadd 
sd_card:vadd_hw

Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git

vadd: Vitis_Accel_Examples $(PLATFORM)
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=$(TARGET) PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus  EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOT)sysroots/cortexa72-cortexa53-xilinx-linux
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=$(TARGET) PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus  EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOT)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd_hw: Vitis_Accel_Examples $(PLATFORM)
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus  EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOT)sysroots/cortexa72-cortexa53-xilinx-linux
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=zynquplus  EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SYSROOT)/sysroots/cortexa72-cortexa53-xilinx-linux

#Vitis AI 2.0 is under preparation. We will update it as soon as possible.
Vitis-AI:
	git clone https://github.com/Xilinx/Vitis-AI.git -b 2.0

dpu-trd: Vitis-AI
	sed -i '37 s/`define URAM_DISABLE/`define URAM_ENABLE/' $(VAI_DPU_TRD_PATH)/dpu_conf.vh
	sed -i 's/Image/Image --package.sd_dir=..\/..\/app/' $(VAI_DPU_TRD_PATH)/Makefile
	$(MAKE) -C $(VAI_DPU_TRD_PATH) all DEVICE=zcu104 SDX_PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(SW_COMP)
	

platforminfo:
	$(MAKE) -C platforminfo

clean:
	-$(MAKE) -C $(PL_EXAMPLE_PATH) clean
	-$(MAKE) -C $(VAI_DPU_TRD_PATH) clean
	-$(MAKE) -C platforminfo  clean

ultraclean:clean
	rm -rf Vitis_Accel_Examples Vitis-AI
