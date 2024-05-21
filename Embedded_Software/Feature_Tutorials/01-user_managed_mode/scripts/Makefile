#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

.PHONY: all clean_all clean_atf clean_dtb clean_boot_script clean_platform clean_uboot create_xsa dtb make_platform

SHELL := /bin/bash
VERSION ?= 2024.1
BOARD ?= zcu102-rev1.0
XSA ?= ./zcu102/design_1_wrapper.xsa

ATF_REPO ?= https://github.com/Xilinx/arm-trusted-firmware.git
ATF_CHECKOUT ?= xilinx-v2023.2
UBOOT_REPO ?= https://github.com/Xilinx/u-boot-xlnx.git
UBOOT_CHECKOUT ?= xilinx-v2023.2
DEVICE_TREE_REPO ?= https://github.com/Xilinx/device-tree-xlnx.git
DEVICE_TREE_CHECKOUT ?= xlnx_rel_v2024.1

all: create_xsa make_platform dtb atf uboot

u-boot-xlnx:
	git clone --branch $(UBOOT_CHECKOUT) $(UBOOT_REPO)

arm-trusted-firmware:
	git clone --branch $(ATF_CHECKOUT) $(ATF_REPO)

create_xsa: $(XSA)

$(XSA): create_xsa.tcl
	$(RM) -r ./zcu102
	vivado -mode tcl -source create_xsa.tcl

make_platform: $(XSA)
	rm -rf tmp/base_platform tmp/zynqmp_fsbl tmp/zynqmp_pmufw
	vitis -s ./xsct_script.py make_platform $(XSA)

dtb: system.dtb

system.dtb:
	$(RM) -rf mydevice
	xsct -eval " createdts -hw $(XSA) -board $(BOARD) -git-url $(DEVICE_TREE_REPO) -git-branch $(DEVICE_TREE_CHECKOUT) -out . -platform-name mydevice -compile"
	cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb .

atf: arm-trusted-firmware $(XSA) xsct_script.py
	$(MAKE) -C arm-trusted-firmware clean
	export CROSS_COMPILE=aarch64-none-elf-; \
	vitis -s ./xsct_script.py build_atf $(XSA)

uboot: u-boot-xlnx $(XSA)
	$(MAKE) -C u-boot-xlnx clean 
	export CROSS_COMPILE=aarch64-linux-gnu- && \
	export ARCH=aarch64 && \
	export CC=aarch64-linux-gnu-gcc && \
	export PATH=$$PATH:$(shell pwd)/dtc && \
	vitis -s ./xsct_script.py build_uboot $(XSA)

generate_boot_script:
	$(RM) -rf boot_script.py
	vitis -s ./xsct_script.py generate_boot_script $(XSA)

jtag_boot:
	vitis -s ./boot_script.py

clean_platform:
	$(RM) -rf tmp/base_platform tmp/zynqmp_fsbl tmp/zynqmp_pmufw

clean_dtb:
	$(RM) -rf system.dtb mydevice

clean_atf:
	$(MAKE) -C arm-trusted-firmware clean

clean_uboot:
	$(MAKE) -C u-boot-xlnx

clean_boot_script:
	$(RM) -rf boot_script.py

clean_all:
	$(RM) -rf arm-trusted-firmware u-boot-xlnx zcu102 tmp mydevice system.dtb boot_script.py .Xil vivado*.jou vivado*.log logs device-tree-xlnx
