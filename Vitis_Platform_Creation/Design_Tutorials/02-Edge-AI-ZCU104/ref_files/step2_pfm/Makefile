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

.PHONY: prep_common sw_comp pfm prep_sw_comp prep_sysroot all

XSA_NAME=zcu104_custom_platform
PLATFORM_NAME=zcu104_custom
PLATFORM_NAME_FSBL=zcu104_custom_fsbl
COMMON_IMAGE_NAME=xilinx-zynqmp-common-v2022.1
COMMON_IMAGE=$(COMMON_IMAGE_NAME)/Image
all: prep_common sw_comp dtb pfm_fsbl pfm  prep_sw_comp  prep_sysroot 
main: prep_common sw_comp pfm_partial prep_sw_comp pfm dtb

prep_common:
	@echo "Extract the common Image.(if no common image ,please download it)"
	tar xvf $(COMMON_IMAGE_NAME).tar.gz -C .


sw_comp: $(COMMON_IMAGE)
	@echo "INFO: Copying software components from common image DIR  to local directory ./boot and ./sd_dir"
	mkdir -p ./sd_dir && mkdir -p ./boot && \
	cp $(COMMON_IMAGE_NAME)/bl31.elf boot/
	cp $(COMMON_IMAGE_NAME)/u-boot.elf boot/
	cp $(COMMON_IMAGE_NAME)/boot.scr sd_dir/
	
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
	cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb boot/
	cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb sd_dir/

pfm_fsbl:
	@echo "INFO: Creating simple Platform $(PLATFORM_NAME_FSBL) to get FSBL and PMU"
#FSBL option is to convince XSCT to create a simple platform to get FSBL and PMU.
	xsct xsct_create_pfm.tcl  $(PLATFORM_NAME_FSBL)  $(XSA_NAME)  ../step1_vivado/build/vivado fsbl
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"

pfm:
	cp $(PLATFORM_NAME_FSBL)/zynqmp_fsbl/fsbl_a53.elf boot/fsbl.elf
	cp $(PLATFORM_NAME_FSBL)/zynqmp_pmufw/pmufw.elf boot/
	@echo "INFO: Creating Final Platform $(PLATFORM_NAME)"
	xsct xsct_create_pfm.tcl $(PLATFORM_NAME) $(XSA_NAME) ../step1_vivado/build/vivado
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"
prep_sw_comp:
	mkdir -p sw_comp && \
	cp $(COMMON_IMAGE_NAME)/rootfs.ext4 ./sw_comp && \
	cp $(COMMON_IMAGE_NAME)/Image ./sw_comp

prep_sysroot: 
	rm -rf ./sysroot && mkdir sysroot && sh $(COMMON_IMAGE_NAME)/sdk.sh -d sysroot -y
clean:
	$(RM) -r vitis_* xrc.log build  sd_dir boot $(COMMON_IMAGE_NAME) sysroot
ultraclean: clean
	$(RM) -r sw_comp $(PLATFORM_NAME) $(PLATFORM_NAME_FSBL) device-tree-xlnx $(COMMON_IMAGE_NAME).tar.gz mydevice
