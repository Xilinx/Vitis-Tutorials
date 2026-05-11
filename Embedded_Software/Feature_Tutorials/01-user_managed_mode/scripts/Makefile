#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

.PHONY: all clean_all clean_atf clean_dtb clean_boot_script clean_platform clean_uboot create_xsa dtb make_platform

SHELL := /bin/bash
VERSION ?= 2025.2
BOARD ?= zcu102-rev1.0
XSA ?= ./zcu102/design_1_wrapper.xsa

ATF_REPO ?= https://github.com/Xilinx/arm-trusted-firmware.git
ATF_CHECKOUT ?= xilinx-v2025.1
UBOOT_REPO ?= https://github.com/Xilinx/u-boot-xlnx.git
UBOOT_CHECKOUT ?= xilinx-v2025.1
SDT_OUT ?= sdt_out 
USER_DTSI ?= prebuilt_linux/user_dts/system-user.dtsi 

all: create_xsa make_platform dtb dtc atf uboot

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

dtb: gen_sdt 

gen_sdt:
	$(RM) -rf $(SDT_OUT)
	$(info "DTB generation started using stdgen+lopper")
	sdtgen -eval "sdtgen set_dt_param -dir $(SDT_OUT) -xsa $(XSA) -board_dts $(BOARD); sdtgen generate_sdt"

dtc: dtc_clone dtc_build dtc_compile

dtc_clone:
	if [ ! -d "dtc" ]; then \
		git clone https://git.kernel.org/pub/scm/utils/dtc/dtc.git; \
	fi

dtc_build:
	cd dtc && \
	make clean && \
	make && \
	make install DESTDIR=$(shell pwd)/dtc_install PREFIX=/usr

dtc_compile: dtc_build
	export PATH="$(shell pwd)/dtc_install/usr/bin:$(shell pwd)/dtc:$$PATH" && \
	gcc -I my_dts -E -nostdinc -I include -undef -D__DTS__ -x assembler-with-cpp -o system.dts sdt_out/system-top.dts && \
	$(shell pwd)/dtc/dtc -I dts -O dtb -o system.dtb system.dts

atf: arm-trusted-firmware $(XSA) xsct_script.py
	$(MAKE) -C arm-trusted-firmware clean
	export CROSS_COMPILE=aarch64-none-elf-; \
	vitis -s ./xsct_script.py build_atf $(XSA)

uboot: u-boot-xlnx $(XSA)
	$(MAKE) -C u-boot-xlnx clean 
	export CROSS_COMPILE=aarch64-linux-gnu- && \
	export ARCH=aarch64 && \
	export CC=aarch64-linux-gnu-gcc && \
	export PATH="$(shell pwd)/dtc_install/usr/bin:$(shell pwd)/dtc:$$PATH" && \
	cd u-boot-xlnx && \
	make xilinx_zynqmp_virt_defconfig && \
	make -f Makefile all -j 32

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
