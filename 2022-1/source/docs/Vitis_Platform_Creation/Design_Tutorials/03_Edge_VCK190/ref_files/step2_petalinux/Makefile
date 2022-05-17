# Copyright 2021 Xilinx Inc.
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
PETALINUX_PROJ_NAME=petalinux
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
PETALINUX_DIR=$(ABS_SW_PATH)
BD_TCL=system_step0.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
PETALINUX_CONFIG=$(PETALINUX_DIR)/project-spec/configs/config
PETALINUX_ROOTFS_CONFIG=$(PETALINUX_DIR)/project-spec/configs/rootfs_config

.PHONY: all get_xsa petalinux clean main

# --------------------------------------
# Public targets
# all: create PetaLinux project from XSA, update configuration, build PetaLinux project and sysroot
# main: create PetaLinux project from XSA, update configuration, build PetaLinux project, without sysroot
#
# Note: This makefile is not designed with dependent awareness. 
# If you change a source file, please run `make clean; make all` to rebuild from scratch.
# --------------------------------------

all: get_xsa petalinux_proj petalinux_build petalinux_build_sdk
main: get_xsa petalinux_proj petalinux_build


get_xsa:
	cd $(ROOT_DIR); cp ../step0_bootable_design/build/vivado/*hw.xsa .

# update dtsi:
# - update bootargs for ext4 rootfs
# update_dtsi:
# 	@cp -f system-user.dtsi $(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

update_config:
# PetaLinux Config: Use EXT4 as rootfs format
	echo "CONFIG_SUBSYSTEM_ROOTFS_EXT4=y" >> $(PETALINUX_CONFIG)
# PetaLinux Config: Use VCK190 device tree
	echo 'CONFIG_SUBSYSTEM_MACHINE_NAME="versal-vck190-reva-x-ebm-02-reva"' >> $(PETALINUX_CONFIG)
# For GCC < 6.0, enable prebuilt build tools
	echo "CONFIG_YOCTO_BUILDTOOLS_EXTENDED=y" >> $(PETALINUX_CONFIG)


update_rootfs_config:
# PetaLinux Rootfs: Enable XRT
	echo 'CONFIG_xrt=y' >> $(PETALINUX_ROOTFS_CONFIG)
	#echo 'CONFIG_xrt-dev=y' >> $(PETALINUX_ROOTFS_CONFIG)
# PetaLinux Rootfs: Enable DNF
	echo 'CONFIG_dnf=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_imagefeature-package-management=y' >> $(PETALINUX_ROOTFS_CONFIG)
# PetaLinux Rootfs: Enable packages for EoU
	echo 'CONFIG_e2fsprogs-resize2fs=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_parted=y' >> $(PETALINUX_ROOTFS_CONFIG)

petalinux_proj:
	mkdir -p $(ABS_BUILD_PATH);cd $(ABS_BUILD_PATH); petalinux-create -t project --template versal --force -n $(PETALINUX_PROJ_NAME)
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_config
	petalinux-config -p $(PETALINUX_DIR) --get-hw-description=$(ROOT_DIR) --silentconfig
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_rootfs_config
# $(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_dtsi
	petalinux-config -p $(PETALINUX_DIR) --silentconfig

petalinux_build:
	petalinux-build -p $(PETALINUX_DIR)
	cd $(PETALINUX_DIR) && petalinux-package --boot --u-boot

petalinux_build_sdk:
	petalinux-build -p $(PETALINUX_DIR) --sdk
	# cd $(PETALINUX_DIR) && petalinux-package --sysroot

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou *.xsa