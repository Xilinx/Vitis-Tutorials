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

.PHONY: sw_comp pfm prep_sw_comp prep_sysroot all

PETALINUX_PROJ_NAME=petalinux
XSA_NAME=zcu104_custom_platform

all: sw_comp pfm prep_sw_comp prep_sysroot
main: sw_comp pfm prep_sw_comp

sw_comp:
	@echo "INFO: Copying software components to local directory ./boot and ./sd_dir"
	mkdir -p ./sd_dir && mkdir -p ./boot && \
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/zynqmp_fsbl.elf ./boot/
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/pmufw.elf ./boot/
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/bl31.elf ./boot/
	# Bug workaround: default PetaLinux generated u-boot.elf doesn't include dtb from 2021.1.
	# U-boot.elf can read dtb from boot.bin, but the default emulation script qemu_args.txt assumes u-boot.elf contains dtb.
	# So we use the u-boot that contains dtb here to workaround this issue.
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/u-boot-dtb.elf ./boot/u-boot.elf
	# system.dtb for u-boot in hw
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/system.dtb ./boot/
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/boot.scr ./sd_dir/
	# system.dtb for Linux
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/system.dtb ./sd_dir/

pfm:
	@echo "INFO: Creating Platform $(XSA_NAME)"
	xsct xsct_create_pfm.tcl $(XSA_NAME) ../step1_vivado/build/vivado
	@echo "INFO: Checking created platforms:" && find . -name "*.xpfm"

prep_sw_comp:
	mkdir -p sw_comp && \
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/rootfs.ext4 ./sw_comp && \
	cp ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/Image ./sw_comp

prep_sysroot: ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/sdk.sh
	rm -rf ./sysroot && ../step2_petalinux/build/$(PETALINUX_PROJ_NAME)/images/linux/sdk.sh -d ./sw_comp -y

clean:
	$(RM) -r vitis_* xrc.log build platform_repo sd_dir boot

ultraclean: clean
	$(RM) -r sw_comp