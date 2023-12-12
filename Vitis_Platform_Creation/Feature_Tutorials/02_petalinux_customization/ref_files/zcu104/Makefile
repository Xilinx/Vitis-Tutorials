#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
PETALINUX_PROJ_NAME=petalinux
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
PETALINUX_DIR=$(ABS_SW_PATH)
PETALINUX_CONFIG=$(PETALINUX_DIR)/project-spec/configs/config
PETALINUX_ROOTFS_CONFIG=$(PETALINUX_DIR)/project-spec/configs/rootfs_config
XSA	?=${PLATFORM_REPO_PATHS}/xilinx_zcu104_base_202320_1/hw/hw.xsa

IMAGE_DIR=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)/images/linux/
.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all XSA=<path/to/base_platform/>  	## to generate Linux Image and SDK tool by using Petalinux with pre-exported XSA file or base paltform "
	

all: check-xsa petalinux_proj petalinux_build 
sd_card:check-xsa petalinux_proj petalinux_build sdimg

check-xsa:
ifeq ($(wildcard $(XSA)),)
	@echo "'XSA' is not accesible, please either download base platform  from Xilinx official website and set this flag to sepcify the PLATFORM_PATH which contains xsa file, or provide yourself  xsa file"
	@make help
	@echo "exit ....." && exit 1
endif
	mkdir hw -p && cp $(XSA) hw/
#Extensible xsa dose not include the bit file. using  extnesible XSA to build the linux image would cause hung in boot stage.  So transfer the extensible XSA to a fixed XSA for building the Linux image and test it.
	cd hw; vivado -mode batch -notrace -source ../convert_to_fixed_xsa.tcl


update_dtsi:
	@cp -f system-user.dtsi $(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

update_config:
	# PetaLinux Config: Use EXT4 as rootfs format
	echo "CONFIG_SUBSYSTEM_ROOTFS_EXT4=y" >> $(PETALINUX_CONFIG)
	@echo "set Machine name according to board "
	# PetaLinux Config: Use ZCU104 device tree  
	echo 'CONFIG_SUBSYSTEM_MACHINE_NAME="zcu104-revc"' >> $(PETALINUX_CONFIG)
	echo 'CONFIG_YOCTO_MACHINE_NAME="zcu104-zynqmp"' >> $(PETALINUX_CONFIG)
	# For GCC < 6.0, enable prebuilt build tools
	echo "CONFIG_YOCTO_BUILDTOOLS_EXTENDED=y" >> $(PETALINUX_CONFIG)

update_rootfs_config:
	# PetaLinux Rootfs: Enable XRT
	echo 'CONFIG_packagegroup-petalinux-vitis-acceleration-essential=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_packagegroup-petalinux-vitis-acceleration-essential-dev=y' >> $(PETALINUX_ROOTFS_CONFIG)



petalinux_proj:
	mkdir -p $(ABS_BUILD_PATH);cd $(ABS_BUILD_PATH); petalinux-create -t project --template zynqMP --force -n $(PETALINUX_PROJ_NAME)
	$(MAKE)  update_config
	petalinux-config -p $(PETALINUX_DIR) --get-hw-description=hw/xsa_file_for_petalinux.xsa --silentconfig
	$(MAKE)  update_rootfs_config
	$(MAKE)  update_dtsi
	petalinux-config -p $(PETALINUX_DIR) --silentconfig

petalinux_build:
	petalinux-build -p $(PETALINUX_DIR)
	cd $(PETALINUX_DIR) && petalinux-package --fpga --boot --u-boot
petalinux_build_sdk:
	petalinux-build -p $(PETALINUX_DIR) --sdk
	cd $(PETALINUX_DIR) && petalinux-package --sysroot
sdimg:
	mkdir -p sd_card 
	cp $(IMAGE_DIR)/BOOT.BIN sd_card/
	cp $(IMAGE_DIR)/boot.scr sd_card/
	cp $(IMAGE_DIR)/system.dtb sd_card/
	cp $(IMAGE_DIR)/Image sd_card/
	cp run_app.sh sd_card/
	$(XILINX_VITIS)/scripts/vitis/util/mkfsImage.sh -s sd_card -o sd_card.img -m 1 -e $(IMAGE_DIR)/rootfs.ext4
clean:
	$(RM) -rf $(ABS_BUILD_PATH) hw* sd* -rf *.log _x vivado* 

