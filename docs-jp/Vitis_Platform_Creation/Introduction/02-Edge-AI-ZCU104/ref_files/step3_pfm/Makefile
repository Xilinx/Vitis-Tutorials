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

all: sw_comp pfm prep_sw_comp prep_sysroot
main: sw_comp pfm prep_sw_comp

sw_comp:
	cp ../step2_petalinux/build/petalinux/images/linux/zynqmp_fsbl.elf ./boot/
	cp ../step2_petalinux/build/petalinux/images/linux/pmufw.elf ./boot/
	cp ../step2_petalinux/build/petalinux/images/linux/bl31.elf ./boot/
	cp ../step2_petalinux/build/petalinux/images/linux/u-boot.elf ./boot/
	cp ../step2_petalinux/build/petalinux/images/linux/boot.scr ./image/
	cp ../step2_petalinux/build/petalinux/images/linux/system.dtb ./image/

pfm:
	xsct xsct_create_pfm.tcl zcu104_custom_platform ../step1_vivado/build/vivado

prep_sw_comp:
	mkdir -p sw_comp && \
	cp ../step2_petalinux/build/petalinux/images/linux/rootfs.ext4 ./sw_comp && \
	cp ../step2_petalinux/build/petalinux/images/linux/Image ./sw_comp

prep_sysroot:
	rm -rf ./sysroot && ../step2_petalinux/build/petalinux/images/linux/sdk.sh -d ./sw_comp -y

clean:
	$(RM) -r vitis_* xrc.log build platform_repo image/boot.scr boot/*.elf

ultraclean: clean
	$(RM) -r sw_comp