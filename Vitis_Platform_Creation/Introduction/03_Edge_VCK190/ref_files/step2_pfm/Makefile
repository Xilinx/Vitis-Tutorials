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
PETALINUX_PROJ_NAME=petalinux
XSA_NAME=vck190_custom

.PHONY: all prep_dtb pfm prep_sd_dir prep_sw_comp prep_sysroot ultraclean clean

# Since making the platform doesn't require sysroot, sometimes we can skip running prep_sysroot by target main
all:  prep_common prep_sw_comp prep_dtb pfm prep_sysroot
main: prep_common prep_sd_dir prep_sw_comp prep_dtb pfm 

	

# Target: prep_sw_comp
# extract the common image 
prep_common:
	@echo "Extract the common image, if you do not have please download it"
	tar xvf xilinx-versal-common-v2022.1.tar.gz 


# Copy rootfs and kernel image  and rootfs to sw_comp directory
#copy boot.scr to sd_dir
prep_sw_comp:  $(prep_common)
	mkdir -p sw_comp 
	cp -f xilinx-versal-common-v2022.1/rootfs.ext4 ./sw_comp 
	mkdir -p sd_dir && cp xilinx-versal-common-v2022.1/boot.scr ./sd_dir/
	cp -f xilinx-versal-common-v2022.1/Image ./sw_comp

# Target: prep_sysroot
# Install sysroot to sw_comp directory
prep_sysroot: 
	rm -rf ./sysroot && mkdir sysroot && sh xilinx-versal-common-v2022.1/sdk.sh -d sysroot -y

# target :prep_dtb
prep_dtb:
	@echo "execute createdts to generate dtb file"
	-xsct -eval " createdts -hw ../step0_bootable_design/build/vivado/$(XSA_NAME)_hw.xsa -out . -zocl  -platform-name mydevice  -git-branch xlnx_rel_v2021.2 -board  versal-vck190-reva-x-ebm-02-reva -compile"
	cp mydevice/psv_cortexa72_0/device_tree_domain/bsp/system.dtb xilinx-versal-common-v2022.1/
	cp mydevice/psv_cortexa72_0/device_tree_domain/bsp/system.dtb sd_dir/


# Target: pfm
# Generate the Vitis platform with XSCT
# Input: vck190_custom.xsa in ../step1_vivado/build/vivado directory
pfm:
	@echo "INFO: Creating Platform $(XSA_NAME)"
	xsct xsct_create_pfm.tcl $(XSA_NAME) ../step0_bootable_design/build/vivado && \
	echo "Platform created:" && \
	find . -name "*.xpfm"

clean:
	$(RM) -r vitis_* xrc.log build platform_repo sd_dir xilinx-versal-common-v2022.1 sysroot -rf

ultraclean:
	$(RM) -r vitis_* xrc.log build platform_repo  sw_comp mydevice device-tree-xlnx xilinx-versal-common-v2022.1.tar.gz 
