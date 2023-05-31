#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
PETALINUX_PROJ_NAME=petalinux
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
PETALINUX_DIR=$(ABS_SW_PATH)
PETALINUX_CONFIG=$(PETALINUX_DIR)/project-spec/configs/config
PETALINUX_ROOTFS_CONFIG=$(PETALINUX_DIR)/project-spec/configs/rootfs_config
#XSA_NAME	?=xsa_file_for_petalinux
PDI_DIR		=pdi_dir
#PDI_PATH	=$(ROOT_DIR)/$(PDI_DIR)
XSA 	?=${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202310_1/hw/hw.xsa
#PDI=$(wildcard $(PDI_PATH)/*.pdi)
IMAGE_DIR=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)/images/linux/
# --------------------------------------
# Note: This makefile is not designed with dependent awareness. 
# If you change a source file, please run `make clean; make all` to rebuild from scratch.
# --------------------------------------

.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all XSA=<path/to/vivado project/>  	## to generate Linux Image and SDK tool by using Petalinux with pre-exported XSA file with bit file inside "
	

all: check-xsa petalinux_proj petalinux_build petalinux_build_sdk
sd_card:check-xsa petalinux_proj petalinux_build sdimg
check-xsa:
ifeq ($(wildcard $(XSA)),)
	@echo "'XSA' is not accesible, please either download base platform  from Xilinx official website and set this flag to sepcify the PLATFORM_PATH which contains xsa file, or provide yourself  xsa file"
	@make help
	@echo "exit ....." && exit 1
endif
	mkdir -p $(PDI_DIR) && cd $(PDI_DIR) && cp $(XSA) . && unzip $(XSA)
	@echo "check if your XSA file contains PDI file, if not please go to your Vivado project, set the projest to a normal project (not a extensible project) and regenerate the XSA file including the bit file"
	@ls $(ROOT_DIR)/$(PDI_DIR)/*.pdi
	@echo "Transfer the XSA to fixed XSA for petalinux building "
	v++ -l --platform $(ROOT_DIR)/$(PDI_DIR)/hw.xsa -t hw --save-temps -g  -o $(PDI_DIR)/fixed.xsa

	

	

# update dtsi:
update_dtsi:
	@cp -f system-user.dtsi $(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi


update_config:
# PetaLinux Config: Use EXT4 as rootfs format
	echo "CONFIG_SUBSYSTEM_ROOTFS_EXT4=y" >> $(PETALINUX_CONFIG)
# PetaLinux Config: Use VCK190 device tree
	echo 'CONFIG_SUBSYSTEM_MACHINE_NAME="versal-vck190-reva-x-ebm-02-reva"' >> $(PETALINUX_CONFIG)
# For GCC < 6.0, enable prebuilt build tools
	echo "CONFIG_YOCTO_BUILDTOOLS_EXTENDED=y" >> $(PETALINUX_CONFIG)


update_rootfs_config:
# PetaLinux Rootfs: Enable XRT
	echo 'CONFIG_packagegroup-petalinux-vitis-acceleration-essential=y' >> $(PETALINUX_ROOTFS_CONFIG)
	echo 'CONFIG_packagegroup-petalinux-vitis-acceleration-essential-dev=y' >> $(PETALINUX_ROOTFS_CONFIG)

petalinux_proj:
	mkdir -p $(ABS_BUILD_PATH);cd $(ABS_BUILD_PATH); petalinux-create -t project --template versal --force -n $(PETALINUX_PROJ_NAME)
	$(MAKE)  update_config
	petalinux-config -p $(PETALINUX_DIR) --get-hw-description=$(ROOT_DIR)/$(PDI_DIR)/fixed.xsa --silentconfig
	$(MAKE)  update_rootfs_config
	$(MAKE)  update_dtsi
	petalinux-config -p $(PETALINUX_DIR) --silentconfig

petalinux_build:
	petalinux-build -p $(PETALINUX_DIR)
	cd $(PETALINUX_DIR) && petalinux-package --boot --u-boot

petalinux_build_sdk:
	petalinux-build -p $(PETALINUX_DIR) --sdk
	# cd $(PETALINUX_DIR) && petalinux-package --sysroot
#to test the image
sdimg:
	mkdir -p sd_card 
	cp $(IMAGE_DIR)/BOOT.BIN sd_card/
	cp $(IMAGE_DIR)/boot.scr sd_card/
	cp $(IMAGE_DIR)/Image sd_card/
	cp run_app.sh sd_card/
	$(XILINX_VITIS)/scripts/vitis/util/mkfsImage.sh -s sd_card -o sd_card.img -m 1 -e $(IMAGE_DIR)/rootfs.ext4
clean:
	$(RM)  $(ABS_BUILD_PATH) vivado*.log *.log vivado*.jou *.xsa _x v++* fixed* $(PDI_DIR)/ sd_card/ sd_card.img boot/  -rf
