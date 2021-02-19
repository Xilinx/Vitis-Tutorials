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


.PHONY: all pfm prep_image prep_sw_comp prep_sysroot ultraclean clean

# Since making the platform doesn't require sysroot, sometimes we can skip running prep_sysroot by target main
all: prep_image prep_sw_comp pfm prep_sysroot
main: prep_image prep_sw_comp pfm 

# Target: sw_comp
# Copy PetaLinux genreated software components to platform preparation directory
prep_image:
	mkdir -p image && cp ../step2_petalinux/build/petalinux/images/linux/boot.scr ./image/

# Target: prep_sw_comp
# Copy rootfs and kernel image to sw_comp directory
prep_sw_comp:
	mkdir -p sw_comp && \
	cp ../step2_petalinux/build/petalinux/images/linux/rootfs.ext4 ./sw_comp && \
	cp ../step2_petalinux/build/petalinux/images/linux/Image ./sw_comp

# Target: prep_sysroot
# Install sysroot to sw_comp directory
prep_sysroot: ../step2_petalinux/build/petalinux/images/linux/sdk.sh
	rm -rf ./sysroot && ../step2_petalinux/build/petalinux/images/linux/sdk.sh -d ./sw_comp -y

# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in ../step1_vivado/build/vivado directory
pfm:
	xsct xsct_create_pfm.tcl vck190_custom ../step0_bootable_design/build/vivado

clean:
	$(RM) -r vitis_* xrc.log build platform_repo ./image/boot.scr rootfs

ultraclean:
	$(RM) -r vitis_* xrc.log build platform_repo ./image/boot.scr rootfs sw_comp