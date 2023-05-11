#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                  := @echo

TARGET                ?= hw
PLATFORM_CUSTOM       ?= vck190_thin
LINUX_PRE_BUILDS      ?= false

XSA_PLATFORM          := ../vivado/build/xsa_platform/${PLATFORM_CUSTOM}.xsa

XPFM_VIVADO_EXPORT    := $(abspath ../vitis/xpfm_export/build/platform/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM})
XPFM_VIVADO           := ${XPFM_VIVADO_EXPORT}/${PLATFORM_CUSTOM}.xpfm

LINUX_ABS_PATH        := $(abspath ${XPFM_VIVADO_EXPORT}/sw)
LINUX_KERNEL          := ${LINUX_ABS_PATH}/Image
LINUX_DTB             := ${LINUX_ABS_PATH}/system.dtb

ifeq (${LINUX_PRE_BUILDS}, false)
LINUX_ROOTFS          := ${LINUX_ABS_PATH}/rootfs.cpio.gz.u-boot
LINUX_ROOTFS_TYPE     := fat32
else
LINUX_ROOTFS          := ${LINUX_ABS_PATH}/rootfs.ext4
LINUX_ROOTFS_TYPE     := ext4
endif

IP_DIR                := ./ip
XO                    := $(wildcard ${IP_DIR}/xo_hw/*.xo)
GRAPH_O               := ${IP_DIR}/aie/libadf.a
XCLBIN                := my.xclbin
XSA                   := ${PLATFORM_CUSTOM}.xsa

PS_APPS_LINUX_DIR     := ../ps_apps/linux
PS_LINUX_EXE          := $(wildcard ${PS_APPS_LINUX_DIR}/exe/*.exe)

SD_CARD_LINUX         := $(abspath ../package_linux_${TARGET})
BUILD_DIR             := build_${TARGET}

ILA_LTX               := ${BUILD_DIR}/_x/link/vivado/vpl/prj/probe_0.ltx

# Force ILA_EN=0 (Disabled) for TARGET=hw_emu
ifeq (${TARGET}, hw_emu)
    ILA_EN  = 0
else
    ILA_EN ?= 0
endif

ifeq (${ILA_EN}, 1)  
    SYSTEM_CFG := $(wildcard ./src/*.cfg)
else 
    SYSTEM_CFG := ./src/system.cfg
endif

.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

link: ${BUILD_DIR} ${BUILD_DIR}/${XSA}

${BUILD_DIR}/${XSA}: ${XO} ${GRAPH_O} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XSA_PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			../${GRAPH_O} \
			--output ${XSA} \
			|& tee vpp_link.log

package_linux: ${SD_CARD_LINUX}

${SD_CARD_LINUX}: ${BUILD_DIR}/${XSA} ${PS_LINUX_EXE} ${GRAPH_O} ${LINUX_KERNEL} ${LINUX_ROOTFS} ${LINUX_DTB}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ${XPFM_VIVADO} \
			--package.out_dir ${SD_CARD_LINUX} \
			--package.boot_mode=sd \
			--package.image_format=${LINUX_ROOTFS_TYPE} \
			--package.kernel_image=${LINUX_KERNEL} \
			--package.rootfs=${LINUX_ROOTFS}  \
			../${GRAPH_O} \
			${XSA} \
			$(addprefix --package.sd_file=../, ${PS_LINUX_EXE}) \
			|& tee vpp_pack.log
	echo ${PLATFORM_CUSTOM} > ${SD_CARD_LINUX}/sd_card/platform_desc.txt
	make update_ila

update_ila:
ifeq (${TARGET}, hw)
	if [ -e ${ILA_LTX} ] ; \
		then cp -f ${ILA_LTX} $(SD_CARD)/ ; \
		else echo "no probe_0 ltx file" ; \
	fi 
endif

clean:
	rm -rf ${SD_CARD_LINUX} ${SD_CARD_BAREMETAL} ${BUILD_DIR}
