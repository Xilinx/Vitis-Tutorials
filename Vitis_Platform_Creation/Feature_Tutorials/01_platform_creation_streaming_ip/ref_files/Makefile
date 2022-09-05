# Copyright 2022 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
DTB_PLT_NAME=mydevice
DTB_LOCATION=$(ROOT_DIR)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
VERSION=2022.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
BOOT=boot
SD_DIR=sd_dir
SW_COMP=sw_comp
SYSROOT=sysroot
XSA_PATH=$(ROOT_DIR)/build/vivado/
XSA_NAME=custom_hardware_platform
PLATFORM_NAME=custom_platform
PLATFORM = $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOTDIR=$(ROOT_DIR)/$(SYSROOT)
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/


.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_VERSAL=<path/to/common_image/>	## to run the APPS with streaming IP support"


all: hw check-common-image sw_comps dtb sysroot pfm vadd


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

#target :software components
#copy software components to local directory
sw_comps:$(COMMON_IMAGE_VERSAL)/Image
	@echo "create folder to store sw components and copy software components to local directory ./boot and ./sd_dir"
	 mkdir -p $(SW_COMP) && mkdir  -p $(BOOT) && mkdir  -p $(SD_DIR)
	cp $(COMMON_IMAGE_VERSAL)/u-boot.elf  $(BOOT)/
	cp $(COMMON_IMAGE_VERSAL)/bl31.elf  $(BOOT)/
	cp $(COMMON_IMAGE_VERSAL)/boot.scr $(SD_DIR)/
	cp $(COMMON_IMAGE_VERSAL)/Image  $(SW_COMP)/
	cp $(COMMON_IMAGE_VERSAL)/rootfs.ext4  $(SW_COMP)/

# target: dtb
#generate the DTB file
dtb: $(XSA_PATH)/$(XSA_NAME)_hw.xsa
	@echo "execute createdts to generate dts file"
	-xsct -eval " createdts -hw $(XSA_PATH)/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -git-branch xlnx_rel_v2022.1 -compile"
	@echo "add system-user.dtsi to add bootargs"
	cp system-user.dtsi $(DTB_LOCATION)/
	echo "#include \"system-user.dtsi\"" >> $(DTB_LOCATION)/system-top.dts 
	@echo "preprocess the dts"
	gcc -I my_dts -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o $(DTB_LOCATION)/system.dts $(DTB_LOCATION)/system-top.dts 
	@echo "compile the DTB"
	dtc -I  dts -O dtb -o $(DTB_LOCATION)/system.dtb $(DTB_LOCATION)/system.dts
	cp $(DTB_LOCATION)/system.dtb  $(BOOT)/
	
# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(COMMON_IMAGE_VERSAL)/sdk.sh
	rm -rf $(SYSROOT)/ && mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_VERSAL)/sdk.sh -d  $(SYSROOT)/ -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm: $(ROOT_DIR)/$(BOOT) $(ROOT_DIR)/$(SD_DIR)
	@echo "INFO: Creating Platform $(XSA_NAME)"
	xsct xsct_create_pfm.tcl $(PLATFORM_NAME) $(XSA_NAME) $(XSA_PATH) $(ROOT_DIR)/$(BOOT) $(ROOT_DIR)/$(SD_DIR) && \
	echo "Platform created:" && \
	find . -name "*.xpfm"

Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git
	cd Vitis_Accel_Examples/ && git checkout 2022.1 && patch -p1 < ../code_modification.patch

$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(PLATFORM)  DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(ROOT_DIR)/$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(SYSROOTDIR)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(ROOT_DIR)/$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd_hw: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(SYSROOTDIR)/
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(ROOT_DIR)/$(SW_COMP) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux


clean:
	-$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou
	-$(RM) -r $(BOOT) $(SD_DIR) $(DTB_PLT_NAME) $(PLATFORM_NAME) vivado*.log vivado*.jou IDE*
	-$(MAKE) -C $(PL_EXAMPLE_PATH) clean
ultraclean: clean
	$(RM) -rf Vitis_Accel_Examples $(SW_COMP) $(SYSROOT) device-tree-xlnx