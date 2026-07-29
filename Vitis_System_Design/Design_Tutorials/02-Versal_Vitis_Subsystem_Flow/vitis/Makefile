#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                  := @echo

TARGET                ?= hw
BOARD_NAME            ?= vck190
export PLATFORM_CUSTOM       ?= ${BOARD_NAME}_thin

XSA_PLATFORM          := ../vivado/build/xsa_platform/${PLATFORM_CUSTOM}.xsa
export XSA_FIXED      := ../vivado/build/xsa_platform/${PLATFORM_CUSTOM}_fixed.xsa

PFM_EXPORT            := ./build_pfm/pfm/export/pfm
XPFM                  := ${PFM_EXPORT}/pfm.xpfm
XPFM_XSA              := ${PFM_EXPORT}/hw/${PLATFORM_CUSTOM}_fixed.xsa

LINUX_PATH            := ../linux/${LINUX_MACHINE}/images/linux
LINUX_KERNEL          := ${LINUX_PATH}/Image
LINUX_DTB             := ${LINUX_PATH}/system.dtb
#LINUX_DTB             := ${PFM_EXPORT}/hw/sdt/system.dtb
LINUX_BOOTSCR         := ${LINUX_PATH}/boot.scr

ifeq (${SW_BUILD_TOOL}, petalinux)
PFM_DTB               := ${PFM_EXPORT}/sw/boot/system.dtb
LINUX_UBOOT           := ${LINUX_PATH}/u-boot.elf
LINUX_BL31            := ${LINUX_PATH}/bl31.elf
LINUX_ROOTFS          := ${LINUX_PATH}/rootfs.cpio.gz.u-boot
LINUX_ROOTFS_TYPE     := fat32
else
PFM_DTB               := ../linux/sdtgen/build/system.dtb
LINUX_DEPLOY          := ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}
LINUX_UBOOT           := ${LINUX_DEPLOY}/u-boot.elf
LINUX_BL31            := ${LINUX_DEPLOY}/arm-trusted-firmware.elf
BOOTBIN_EXT           := boot.bin-extracted
LINUX_ROOTFS_TYPE     := ext4
endif

IP_DIR                := ./ip
XO                    := $(wildcard ${IP_DIR}/xo_hw/*.xo)
VSS                   ?= vss_top
VSSFILE               ?= ../vss/build/${VSS}/${VSS}.vss
XCLBIN                := my.xclbin
XSA                   := ${PLATFORM_CUSTOM}.xsa
VMA                   := ${PLATFORM_CUSTOM}.vma
LIBADF                := $(wildcard ../vss/${IP_DIR}/aie/*_libadf.a)

PS_APPS_LINUX_DIR     := ../ps_apps/linux
PS_LINUX_EXE          := $(wildcard ${PS_APPS_LINUX_DIR}/exe/*.exe)

SD_CARD_LINUX         := ../package_linux_${TARGET}
BUILD_DIR             := build_${TARGET}
ZOCL                  := zocl.dtsi
ZOCL_ABS              := $(abspath ${BUILD_DIR}/${ZOCL})
ZOCL_SRC              := ../linux/src/${BOARD_NAME}/package_zocl
USER_DTS              := $(abspath ../linux/src/${BOARD_NAME}/recipes-bsp/device-tree/files/system-user.dtsi)

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

.PHONY: clean all vitis_ip platform

.PHONY: help
help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make vitis_ip"
	$(ECHO) "      Compile and build HLS and RTL blocks for Vitis Region"
	$(ECHO) ""
	$(ECHO) "  make link"
	$(ECHO) "      Link vitis region to extensible XSA, run synth and implementaion using Vitis Integrated Flow"
	$(ECHO) ""
	$(ECHO) "  make vma_link"
	$(ECHO) "      Link vitis region to extensible XSA and export a VMA."
	$(ECHO) ""
	$(ECHO) "  make platform"
	$(ECHO) "      Build a Vitis platform component using fixed XSA"
	$(ECHO) ""
	$(ECHO) "  make package_linux"
	$(ECHO) "      Package the design and generate SD Card for Linux application"
	$(ECHO) ""
	$(ECHO) "  make package_edf"
	$(ECHO) "      Package the design for EDF, updates BOOT.bin and collect host apps and xclbin files to be inserted in wic image."
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

vitis_ip:
	$(MAKE) all -C ${IP_DIR}

platform:
	vitis -s vitis_platform.py 2>&1 | tee vitis_pfm.log

.PHONY: link
link: ${BUILD_DIR} ${BUILD_DIR}/${XSA}

.PHONY: vma_link
vma_link: ${BUILD_DIR} ${BUILD_DIR}/${VMA}

${BUILD_DIR}/${VMA}: ${XO} ${VSSFILE} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XSA_PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			$(addprefix ../, ${VSSFILE}) \
			--export_archive \
			--output ${VMA} \
			2>&1 | tee vpp_link.log

${BUILD_DIR}/${XSA}: ${XO} ${VSSFILE} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XSA_PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			$(addprefix ../, ${VSSFILE}) \
			--export_archive \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

.PHONY: package_linux
package_linux: ${SD_CARD_LINUX}

${BUILD_DIR}/${ZOCL}: ${BUILD_DIR} ${LINUX_DEPLOY}/${BOOTBIN_EXT}
	cd ${BUILD_DIR} && \
	(cd ${LINUX_DEPLOY} && zip -r ${ZOCL_ABS} ${BOOTBIN_EXT}) && \
	(cd ../${ZOCL_SRC} && zip -r ${ZOCL_ABS} .)

.PHONY: package_zocl
package_zocl: ${BUILD_DIR} ${BUILD_DIR}/${ZOCL}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XSA_FIXED} \
			--package.defer_aie_run \
			--package.out_dir $(addprefix ../,${SD_CARD_LINUX}) \
			--advanced.param package.apuLinuxEdfMetadata=${ZOCL} \
			--advanced.param package.userDtsPath=${USER_DTS} \
			$(addprefix ../, ${LIBADF}) \
			-o ${XCLBIN} \
			2>&1 | tee vpp_pack_sd.log

.PHONY: package_edf
package_edf: ${BUILD_DIR}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XSA_FIXED} \
			--package.out_dir $(addprefix ../,${SD_CARD_LINUX}) \
			$(addprefix ../, ${LIBADF}) \
			-o ${XCLBIN} \
			2>&1 | tee vpp_pack_sd.log

.PHONY: package_seg
package_seg: ${BUILD_DIR}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XSA_FIXED} \
			--package.out_dir $(addprefix ../,${SD_CARD_LINUX}) \
			--advanced.param package.apuLinuxEdfMetadata=${ZOCL} \
			--advanced.param package.userDtsPath=${USER_DTS} \
			$(addprefix ../, ${LIBADF}) \
			-o ${XCLBIN} \
			2>&1 | tee vpp_pack_sd.log

#			--package.aie_overlay \

## Package the XCLBIN - Recipe for debug only
${BUILD_DIR}/${XCLBIN}: ${LIBADF}
	cd ${BUILD_DIR} && \
		v++ --package \
			--target ${TARGET} \
			--platform $(addprefix ../, ${XPFM}) \
			--package.defer_aie_run \
			$(addprefix ../, ${LIBADF}) \
			-o ${XCLBIN} \
			2>&1 | tee vpp_pack_xclbin.log
#	make update_ila

#${SD_CARD_LINUX}: ${BUILD_DIR}/${XSA} ${PS_LINUX_EXE} ${GRAPH_O} ${LINUX_KERNEL} ${LINUX_ROOTFS} ${LINUX_DTB}
${SD_CARD_LINUX}: ${LIBADF} ${PS_LINUX_EXE} ${LINUX_KERNEL} ${LINUX_ROOTFS} ${LINUX_DTB} ${LINUX_BL31} ${LINUX_UBOOT} ${LINUX_BOOTSCR}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform $(addprefix ../, ${XPFM}) \
			--package.defer_aie_run \
			--package.out_dir $(addprefix ../,${SD_CARD_LINUX}) \
			--package.boot_mode=sd \
			--package.no_image \
			--package.image_format=${LINUX_ROOTFS_TYPE} \
			--package.sd_file=$(addprefix ../, ${LINUX_KERNEL}) \
			--package.rootfs=$(addprefix ../, ${LINUX_ROOTFS})  \
			--package.dtb=$(addprefix ../, ${PFM_DTB})  \
			--package.bl31_elf=$(addprefix ../, ${LINUX_BL31})  \
			--package.uboot=$(addprefix ../, ${LINUX_UBOOT})  \
			$(addprefix ../, ${LIBADF}) \
			$(addprefix --package.sd_file=../, ${PS_LINUX_EXE}) \
			$(addprefix --package.sd_file=../, ${LINUX_BOOTSCR}) \
			-o ${XCLBIN} \
			2>&1 | tee vpp_pack_sd.log
	echo ${PLATFORM_CUSTOM} > ${SD_CARD_LINUX}/sd_card/platform_desc.txt
#	make update_ila



.PHONY: update_ila
update_ila:
ifeq (${TARGET}, hw)
	if [ -e ${ILA_LTX} ] ; \
		then cp -f ${ILA_LTX} $(SD_CARD_LINUX)/ ; \
		else echo "no probe_0 ltx file" ; \
	fi 
endif

clean:
	$(MAKE) clean -C ip
	rm -rf ${SD_CARD_LINUX} ${SD_CARD_BAREMETAL} ${BUILD_DIR} build_pfm
