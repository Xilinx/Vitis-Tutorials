#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/


ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
DTB_PLT_NAME=mydevice
DTB_LOCATION=$(ROOT_DIR)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
DTB=$(DTB_LOCATION)/system.dtb								  
VERSION=2024.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT=sysroot
XSA_NAME=vck190_custom
PLATFORM_NAME=vck190_custom
PLATFORM ?= $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
#PLATFORM ?=$(ROOT_DIR)/test/vck190_custom/export/vck190_custom/vck190_custom.xpfm
SYSROOTDIR=$(ROOT_DIR)/$(SYSROOT)
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/
AIE_EXAMPLE_PATH = Vitis_Accel_Examples/aie_kernels/aie_adder/

.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run vadd application in software emulation mode"
	@echo "make sd_card COMMON_IMAGE_VERSAL=<path/to/common_image/>  	## to generate platform with pre-built software components and run vadd application on board"

all: check-common-image hw  dtb sysroot pfm vadd 
sd_card:check-common-image hw  dtb sysroot pfm vadd_hw 

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif

# target: hw
# Build  Vivado design and export XSA
hw: $(ABS_BD_TCL)
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -tclargs VCK190

# target: dtb
#generate the DTB file
#echo command is to avoid the return error code caused by the createdts command warning
dtb: $(ABS_HW_PATH)/$(XSA_NAME).xsa
	@echo "execute createdts to generate dts file"
	-xsct -eval " createdts -hw $(ABS_HW_PATH)/$(XSA_NAME).xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -git-branch xlnx_rel_v2024.1 -board  versal-vck190-reva-x-ebm-02-reva -dtsi system-user.dtsi -compile" 
	
	
# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(SYSROOT)

$(SYSROOT): $(COMMON_IMAGE_VERSAL)/sdk.sh
	mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_VERSAL)/sdk.sh -d  $(SYSROOT)/ -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm: $(ABS_HW_PATH)/$(XSA_NAME).xsa
	@echo "INFO: Creating Platform $(XSA_NAME)"
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path $(ABS_HW_PATH)/$(XSA_NAME).xsa --boot $(COMMON_IMAGE_VERSAL) --dtb $(DTB)

Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git


$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(SYSROOT)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: Vitis_Accel_Examples $(SYSROOT)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux
aie_adder: Vitis_Accel_Examples $(SYSROOT)/
	$(MAKE) -C $(AIE_EXAMPLE_PATH) run TARGET=hw_emu PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux
aie_adder_hw: Vitis_Accel_Examples $(SYSROOT)/
	$(MAKE) -C $(AIE_EXAMPLE_PATH) clean
	$(MAKE) -C $(AIE_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

clean:
	-$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou 
	-$(MAKE) -C $(PL_EXAMPLE_PATH) clean
	-$(MAKE) -C $(AIE_EXAMPLE_PATH) clean

ultraclean:clean
	rm -rf device-tree-xlnx/ $(DTB_PLT_NAME) $(PLATFORM_NAME) $(SYSROOT) Vitis_Accel_Examples *journal.py __pycache__ logs ip_cache vitis_journal* 
	
