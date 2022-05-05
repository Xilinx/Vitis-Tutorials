#Copyright 2022 Xilinx Inc.
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


XSA_NAME=zcu104_custom_platform
PLATFORM_NAME=zcu104_custom
PLATFORM_NAME_FSBL=zcu104_custom_fsbl
VERSION=2022.1
COMMON_IMAGE_PATH ?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/
BOOT=boot
SD_DIR=sd_dir
SYSROOT=sysroot
SW_COMP=sw_comp


.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_PATH=<path/to/common_image/>  	## to generate platform with pre-built software components"
	

all: check-common-image sw_comps dtb pfm_fsbl pfm  sysroot 

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_PATH)/Image),)
	@echo "'COMMON_IMAGE_PATH' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif


#target :software components
#copy software components to local directory
sw_comps:$(COMMON_IMAGE_PATH)/Image
	@echo "create folder to store sw components and copy software components to local directory ./boot and ./sd_dir"
	mkdir -p $(SW_COMP) && mkdir  -p $(BOOT) && mkdir  -p $(SD_DIR)
	cp $(COMMON_IMAGE_PATH)/u-boot.elf  $(BOOT)/
	cp $(COMMON_IMAGE_PATH)/bl31.elf  $(BOOT)/
	cp $(COMMON_IMAGE_PATH)/boot.scr $(SD_DIR)/
	cp $(COMMON_IMAGE_PATH)/Image  $(SW_COMP)/
	cp $(COMMON_IMAGE_PATH)/rootfs.ext4  $(SW_COMP)/

#:DTB 
#generate dtb file	
dtb:
	@echo "execute createdts to generate dtb file"
	-xsct -eval " createdts -hw ../step1_vivado/build/vivado/$(XSA_NAME)_hw.xsa -zocl  -platform-name mydevice -out .  -git-branch xlnx_rel_v2022.1 -board  zcu104-revc"
	@echo "add system-user.dtsi to add bootargs"
	cp system-user.dtsi mydevice/psu_cortexa53_0/device_tree_domain/bsp/
	echo "#include \"system-user.dtsi\"" >> mydevice/psu_cortexa53_0/device_tree_domain/bsp/system-top.dts 
	@echo "preprocess the dts"
	gcc -I my_dts -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dts mydevice/psu_cortexa53_0/device_tree_domain/bsp/system-top.dts 
	@echo "compile the DTB"
	dtc -I dts -O dtb -o mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dts
	cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb $(BOOT)/
	cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb $(SD_DIR)/

#create a platform with bare-metal domain.
pfm_fsbl:
	@echo "INFO: Creating simple Platform $(PLATFORM_NAME_FSBL) to get FSBL and PMU"
#FSBL option is to convince XSCT to create a simple platform to get FSBL and PMU.
	xsct xsct_create_pfm.tcl  $(PLATFORM_NAME_FSBL)  $(XSA_NAME)  ../step1_vivado/build/vivado fsbl
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"

#create platfrom 
pfm:
	cp $(PLATFORM_NAME_FSBL)/zynqmp_fsbl/fsbl_a53.elf $(BOOT)/fsbl.elf
	cp $(PLATFORM_NAME_FSBL)/zynqmp_pmufw/pmufw.elf $(BOOT)/
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	xsct xsct_create_pfm.tcl $(PLATFORM_NAME) $(XSA_NAME) ../step1_vivado/build/vivado
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
 
#install SDK tool
sysroot: $(COMMON_IMAGE_PATH)/sdk.sh
	rm -rf $(SYSROOT)/ && mkdir -p $(SYSROOT) && sh $(COMMON_IMAGE_PATH)/sdk.sh -d  $(SYSROOT)/ -y

clean:
	rm -rf  IDE.log $(PLATFORM_NAME) $(PLATFORM_NAME_FSBL) device-tree-xlnx  mydevice
ultraclean:clean
	rm -rf   $(SW_COMP) $(BOOT) $(SD_DIR)   $(SYSROOT)
