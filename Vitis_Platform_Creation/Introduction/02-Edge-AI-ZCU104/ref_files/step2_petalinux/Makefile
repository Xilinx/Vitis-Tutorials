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

.PHONY: all get_xsa petalinux clean

all: get_xsa petalinux_proj petalinux_build

get_xsa:
	cd $(ROOT_DIR); cp ../step1_vivado/build/vivado/*.xsa .


update_dtsi:
	@cp -f system-user.dtsi $(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

update_config:
	# PetaLinux Config: Use EXT4 as rootfs format
	@sed -i -E 's/.*CONFIG_SUBSYSTEM_ROOTFS_EXT4 .+/CONFIG_SUBSYSTEM_ROOTFS_EXT4=y/' $(PETALINUX_DIR)/project-spec/configs/config
	@sed -i -E 's/.*CONFIG_SUBSYSTEM_ROOTFS_INITRAMFS .+/# CONFIG_SUBSYSTEM_ROOTFS_INITRAMFS is not set/' $(PETALINUX_DIR)/project-spec/configs/config
	@sed -i -E 's/CONFIG_SUBSYSTEM_RFS_FORMATS.*/CONFIG_SUBSYSTEM_RFS_FORMATS="cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4"/' $(PETALINUX_DIR)/project-spec/configs/config
	# PetaLinux Config: Use ZCU104 device tree
	@sed -i -E 's/CONFIG_SUBSYSTEM_MACHINE_NAME.+/CONFIG_SUBSYSTEM_MACHINE_NAME="zcu104-revc"/' $(PETALINUX_DIR)/project-spec/configs/config
	@sed -i -E 's/CONFIG_YOCTO_MACHINE_NAME.+/CONFIG_YOCTO_MACHINE_NAME="zcu104-zynqmp"/' $(PETALINUX_DIR)/project-spec/configs/config

update_rootfs_config:
	# PetaLinux Rootfs: Enable XRT
	@sed -i -E 's/.*CONFIG_packagegroup-petalinux-xrt .+/CONFIG_packagegroup-petalinux-xrt=y/' $(PETALINUX_DIR)/project-spec/configs/rootfs_config
	@sed -i -E 's/.*CONFIG_xrt-dev.+/CONFIG_xrt-dev=y/' $(PETALINUX_DIR)/project-spec/configs/rootfs_config

petalinux_proj:
	mkdir -p $(ABS_BUILD_PATH);cd $(ABS_BUILD_PATH); petalinux-create -t project --template zynqMP --force -n $(PETALINUX_PROJ_NAME)
	petalinux-config -p $(PETALINUX_DIR) --get-hw-description=$(ROOT_DIR) --silentconfig
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_config
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_rootfs_config
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_dtsi

petalinux_build:
	petalinux-build -p $(PETALINUX_DIR)
	petalinux-build -p $(PETALINUX_DIR) --sdk
	cd $(PETALINUX_DIR) && petalinux-package --sysroot
	cd $(PETALINUX_DIR) && petalinux-package --boot --u-boot

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou *.xsa
