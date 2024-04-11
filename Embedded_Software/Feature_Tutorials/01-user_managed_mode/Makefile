#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
TOOLS ?= /proj/gsd/vivado/Vitis
VERSION ?= 2023.1
BOARD ?= ""

get_sources:
	if [ ! -d "./repo" ];then \
		mkdir -p repo/my_dtg; \
		cd repo/my_dtg && git clone https://github.com/Xilinx/device-tree-xlnx; \
		cd device-tree-xlnx && git checkout xlnx_rel_v$(VERSION); \
	fi
	if [ ! -d "./u-boot-xlnx" ];then \
		git clone https://github.com/Xilinx/u-boot-xlnx; \
		cd u-boot-xlnx && git checkout xilinx-v$(VERSION); \
	fi
	if [ ! -d "./arm-trusted-firmware" ];then \
		git clone https://github.com/Xilinx/arm-trusted-firmware; \
		cd arm-trusted-firmware && git checkout xilinx-v$(VERSION); \
	fi
	if [ ! -d "./dtc" ];then \
		git clone https://git.kernel.org/pub/scm/utils/dtc/dtc.git; \
		$(MAKE) -C dtc; \
	fi

make_platform:
	rm -rf tmp/base_platform tmp/zynqmp_fsbl tmp/zynqmp_pmufw
	vitis -s ./xsct_script.py make_platform

create_dt:
	$(RM) -r my_dts
	xsct -eval "source xsct_script.tcl; build_dts -version $(VERSION) -board $(BOARD)"

compile_dt:
	$(RM) -r system.dtb
	export PATH=$$PATH:$(shell pwd)/dtc; \
	gcc -I my_dts -I repo/my_dtg/device-tree-xlnx/device_tree/data/kernel_dtsi/$(VERSION)/BOARD -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o my_dts/system-top.dts.tmp my_dts/system-top.dts; \
	dtc -I dts -O dtb -o system.dtb my_dts/system-top.dts.tmp

atf:
	$(MAKE) -C arm-trusted-firmware clean
	export CROSS_COMPILE=aarch64-none-elf-; \
	vitis -s ./xsct_script.py build_atf

uboot:
	$(MAKE) -C u-boot-xlnx clean
	export CROSS_COMPILE=aarch64-linux-gnu-; \
	export ARCH=aarch64; \
	export CC=aarch64-linux-gnu-gcc; \
	export PATH=$$PATH:$(shell pwd)/dtc; \
	vitis -s ./xsct_script.py build_uboot

generate_boot_script:
	rm -rf boot_script.py
	vitis -s ./xsct_script.py generate_boot_script

jtag_boot:
	vitis -s ./boot_script.py

clean_platform:
	rm -rf tmp/base_platform tmp/zynqmp_fsbl tmp/zynqmp_pmufw

clean_dt:
	rm -rf my_dts system.dtb

clean_atf:
	$(MAKE) -C arm-trusted-firmware clean

clean_uboot:
	$(MAKE) -C u-boot-xlnx

clean_boot_script:
	rm -rf boot_script.py

