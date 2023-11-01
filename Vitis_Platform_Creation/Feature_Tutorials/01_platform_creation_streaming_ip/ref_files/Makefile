#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
DTB_PLT_NAME=mydevice
DTB_LOCATION=$(ROOT_DIR)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
DTB=$(DTB_LOCATION)/system.dtb
VERSION=2023.2
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT=sysroot
XSA_PATH=$(ROOT_DIR)/build/vivado/
XSA_NAME=custom_hardware_platform
PLATFORM_NAME=custom_platform
CUSTOM_PLATFORM = $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOTDIR=$(ROOT_DIR)/$(SYSROOT)
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/


.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>	## to run the APPS with streaming IP support"


all: hw check-common-image  dtb sysroot pfm vadd
sd_card:hw check-common-image  dtb sysroot pfm vadd_hw

#generate the XSA file 
hw:
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -source 


#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_VERSAL)/Image),)
	@echo "'COMMON_IMAGE_VERSAL' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif


# target: dtb
#generate the DTB file
#echo command is to avoid the return error code caused by the createdts command warning
dtb: $(XSA_PATH)/$(XSA_NAME)_hw.xsa
	@echo "execute createdts to generate dts file"
	xsct -eval " createdts -hw $(XSA_PATH)/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -dtsi system-user.dtsi -git-branch xlnx_rel_v2023.2 -compile" 
	
# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(SYSROOT)

$(SYSROOT): $(COMMON_IMAGE_VERSAL)/sdk.sh
	mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_VERSAL)/sdk.sh -d  $(SYSROOT)/ -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm:
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	@echo "set place for vitis configuration file "
	mkdir -p tmp && export XILINX_VITIS_DATA_DIR="./tmp" && vitis -s platform_creation.py --platform_name $(PLATFORM_NAME) --xsa_path $(XSA_PATH)/$(XSA_NAME)_hw.xsa --xsa-emu_path $(XSA_PATH)/$(XSA_NAME)_hwemu.xsa --boot $(COMMON_IMAGE_VERSAL) --dtb $(DTB)
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
	echo "Platform created:" && \
	find . -name "*.xpfm"

Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git
	cd Vitis_Accel_Examples/  && patch -p1 < ../code_modification.patch

$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(CUSTOM_PLATFORM)  DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(SYSROOTDIR)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=hw_emu PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd_hw: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(SYSROOTDIR)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux


clean:
	-$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou
	-$(RM) -r  $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE*
	-$(MAKE) -C $(PL_EXAMPLE_PATH) clean
ultraclean: clean
	$(RM) -rf Vitis_Accel_Examples  $(SYSROOT) device-tree-xlnx
