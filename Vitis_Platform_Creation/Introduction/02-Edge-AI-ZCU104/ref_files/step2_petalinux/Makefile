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
PETALINUX_PROJ_NAME=petalinux
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
PETALINUX_DIR=$(ABS_SW_PATH)
BD_TCL=system_step0.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
PETALINUX_CONFIG=$(PETALINUX_DIR)/project-spec/configs/config
PETALINUX_ROOTFS_CONFIG=$(PETALINUX_DIR)/project-spec/configs/rootfs_config

.PHONY: all get_xsa petalinux clean main

all: get_xsa petalinux_proj petalinux_build petalinux_build_sdk
main: get_xsa petalinux_proj petalinux_build

get_xsa:
	cd $(ROOT_DIR); cp ../step1_vivado/build/vivado/*hw.xsa .


update_dtsi:
	@cp -f system-user.dtsi $(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

update_config:
	# PetaLinux Config: Use EXT4 as rootfs format
	echo "CONFIG_SUBSYSTEM_ROOTFS_EXT4=y" >> $(PETALINUX_CONFIG)
	# PetaLinux Config: Use ZCU104 device tree
	echo 'CONFIG_SUBSYSTEM_MACHINE_NAME="zcu104-revc"' >> $(PETALINUX_CONFIG)
	echo 'CONFIG_YOCTO_MACHINE_NAME="zcu104-zynqmp"' >> $(PETALINUX_CONFIG)
	# For GCC < 6.0, enable prebuilt build tools
	echo "CONFIG_YOCTO_BUILDTOOLS_EXTENDED=y" >> $(PETALINUX_CONFIG)

update_rootfs_config:
	# PetaLinux Rootfs: Enable XRT
	echo 'CONFIG_xrt=y' >> $(PETALINUX_ROOTFS_CONFIG)
	# echo 'CONFIG_xrt-dev=y' >> $(PETALINUX_ROOTFS_CONFIG)
	# PetaLinux Rootfs: Enable DNF
	echo 'CONFIG_dnf=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_imagefeature-package-management=y' >> $(PETALINUX_ROOTFS_CONFIG)
	# PetaLinux Rootfs: Enable packages for EoU
	echo 'CONFIG_e2fsprogs-resize2fs=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_parted=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_resize-part=y' >> $(PETALINUX_ROOTFS_CONFIG)
	# PetaLinux Rootfs: Enable Vitis AI demo dependencies
	echo 'CONFIG_packagegroup-petalinux-opencv=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_mesa-megadriver=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_packagegroup-petalinux-x11=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_packagegroup-petalinux-v4lutils=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_packagegroup-petalinux-matchbox=y' >> $(PETALINUX_ROOTFS_CONFIG)



petalinux_proj:
	mkdir -p $(ABS_BUILD_PATH);cd $(ABS_BUILD_PATH); petalinux-create -t project --template zynqMP --force -n $(PETALINUX_PROJ_NAME)
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_config
	petalinux-config -p $(PETALINUX_DIR) --get-hw-description=$(ROOT_DIR) --silentconfig
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_rootfs_config
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_dtsi
	petalinux-config -p $(PETALINUX_DIR) --silentconfig

petalinux_build:
	petalinux-build -p $(PETALINUX_DIR)
	cd $(PETALINUX_DIR) && petalinux-package --boot --u-boot

petalinux_build_sdk:
	petalinux-build -p $(PETALINUX_DIR) --sdk
	cd $(PETALINUX_DIR) && petalinux-package --sysroot

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou *.xsa
