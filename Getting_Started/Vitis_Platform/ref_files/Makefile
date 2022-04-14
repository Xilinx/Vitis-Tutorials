# Copyright 2020 Xilinx Inc.
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
COMMON_IMAGE_NAME=xilinx-versal-common-v2022.1
XSA_NAME=vck190_custom
PLATFORM_NAME=vck190_custom
PLATFORM = $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
SYSROOTDIR=$(ROOT_DIR)/$(COMMON_IMAGE_NAME)/sysroot/
PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/


.PHONY: help all hw common dtb sysroot pfm vadd vadd_hw clean ultraclean

all: help hw common dtb sysroot pfm  vadd vadd_hw

help:
	@echo "Makefile usage"
	@echo "make hw       	## to generate XSA file "
	@echo "make common   	## to extract common image. Before extracting common image ,please perpare the common image under current directory"
	@echo "make dtb      	## to generate DTB and DTS file by using createdts command "
	@echo "make sysroot  	## to install Yocto SDK tool"
	@echo "make pfm      	## to create Vitis platform"
	@echo "make vadd	 	## to compile Vector Addition application and test hardware simulation"
	@echo "make vadd_hw  	## to cimple Vector Addition and generate SD_CARD.IMG for hardware platform"
	@echo "make clean    	## to clean the intermedia files"
	@echo "make ultrafile	## to clean all the project output file"


# target: hw
# Build bare  Vivado design and export XSA
hw: $(ABS_BD_TCL)
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -tclargs VCK190


# target: common
# extract the common image 
common: $(COMMON_IMAGE_NAME).tar.gz
	@echo "Extract the common image, if you do not have please download it"
	tar xvf $(COMMON_IMAGE_NAME).tar.gz 

# target: dtb
#generate the DTB file
dtb: $(ABS_HW_PATH)/$(XSA_NAME)_hw.xsa
	@echo "execute createdts to generate dts file"
	-xsct -eval " createdts -hw $(ABS_HW_PATH)/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name $(DTB_PLT_NAME)  -git-branch xlnx_rel_v2022.1 -board  versal-vck190-reva-x-ebm-02-reva"
	@echo "add system-user.dtsi to add bootargs"
	cp system-user.dtsi $(DTB_LOCATION)/
	echo "#include \"system-user.dtsi\"" >> $(DTB_LOCATION)/system-top.dts 
	@echo "preprocess the dts"
	gcc -I my_dts -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o $(DTB_LOCATION)/system.dts $(DTB_LOCATION)/system-top.dts 
	@echo "compile the DTB"
	dtc -I dts -O dtb -o $(DTB_LOCATION)/system.dtb $(DTB_LOCATION)/system.dts
	cp $(DTB_LOCATION)/system.dtb  $(COMMON_IMAGE_NAME)/
	

# Target: prep_sysroot
# Install sysroot to sw_comp directory
sysroot: $(COMMON_IMAGE_NAME)/sdk.sh
	rm -rf ./$(COMMON_IMAGE_NAME)/sysroot &&  sh $(COMMON_IMAGE_NAME)/sdk.sh -d  $(COMMON_IMAGE_NAME)/sysroot -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in build/vivado directory
pfm: $(COMMON_IMAGE_NAME)/Image
	rm -rf ./$(COMMON_IMAGE_NAME)/sd_dir && mkdir $(COMMON_IMAGE_NAME)/sd_dir
	cp $(COMMON_IMAGE_NAME)/boot.scr $(COMMON_IMAGE_NAME)/sd_dir
	@echo "INFO: Creating Platform $(XSA_NAME)"
	xsct xsct_create_pfm.tcl $(PLATFORM_NAME) $(ABS_HW_PATH) && \
	echo "Platform created:" && \
	find . -name "*.xpfm"

Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git
	patch -p1 Vitis_Accel_Examples/cpp_kernels/simple_vadd/Makefile meetVitisNewFeature.patch

$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(ROOT_DIR)/$(COMMON_IMAGE_NAME) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=hw_emu DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(ROOT_DIR)/$(COMMON_IMAGE_NAME) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(ROOT_DIR)/$(COMMON_IMAGE_NAME) SYSROOT=$(SYSROOTDIR)/sysroots/cortexa72-cortexa53-xilinx-linux

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou 
	$(MAKE) -C $(PL_EXAMPLE_PATH) clean

ultraclean:
	rm -rf device-tree-xlnx $(DTB_PLT_NAME) $(PLATFORM_NAME) $(COMMON_IMAGE_NAME) Vitis_Accel_Examples
	