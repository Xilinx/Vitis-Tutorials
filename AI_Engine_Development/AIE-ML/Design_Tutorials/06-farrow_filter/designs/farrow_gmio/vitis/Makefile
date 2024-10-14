#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Chandra Sekar Balakrishnan

ECHO                  := @echo

TARGET                ?= hw

# Platform Selection...
VERSAL_VITIS_PLATFORM  = xilinx_vek280_base_202410_1
VITIS_PLATFORM_DIR     = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
VITIS_PLATFORM_XPFM    = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# Set SysRoot, RootFS and Image
# COMMON_IMAGE_VERSAL  = " This path is setup in the designs > farrow_gmio > Makefile " << To run this file separately, set the path here. >> 
VITIS_SYSROOTS   	= ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
SDKTARGETSYSROOT 	= ${VITIS_SYSROOTS}
KERNEL_IMAGE     	= ${COMMON_IMAGE_VERSAL}/Image
ROOTFS           	= ${COMMON_IMAGE_VERSAL}/rootfs.ext4
XLNX_VERSAL      	= ${COMMON_IMAGE_VERSAL}
PLATFORM         	= ${VITIS_PLATFORM_XPFM}
SYSROOT          	= ${SDKTARGETSYSROOT}

IP_DIR                := ../aie_ml
DATA_DIR              := ../aie_ml


GRAPH_O               := ${IP_DIR}/libadf.a
XSA                   := my.xsa
# STIMULUS_IO           := ${DATA_DIR}/input_sig_i.h ${DATA_DIR}/del_i_optimized.txt ${DATA_DIR}/sig_o.txt
XRT_INI			  	  := xrt.ini
PS_EXE                := ../ps_apps/hw_emu/host.exe

SD_CARD               := $(abspath ../package)
BUILD_DIR             := build_${TARGET}

SYSTEM_CFG            := system.cfg

.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${SD_CARD}

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${XSA}: ${BUILD_DIR} ${GRAPH_O} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			../${GRAPH_O} \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

${SD_CARD}: ${BUILD_DIR}/${XSA} ${PS_EXE} ${GRAPH_O} ${KERNEL_IMAGE} ${ROOTFS} 
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			--package.out_dir ${SD_CARD} \
			--package.boot_mode=sd \
			--package.image_format=ext4 \
			--package.kernel_image=${KERNEL_IMAGE} \
			--package.rootfs=${ROOTFS}  \
			--package.defer_aie_run \
			../${GRAPH_O} \
			${XSA} \
			$(addprefix --package.sd_file=../, ${PS_EXE}) \
			$(addprefix --package.sd_file=../,embedded_exec.sh) \
			$(addprefix --package.sd_file=../, ${XRT_INI}) \
			2>&1 | tee vpp_pack.log
	echo ${PLATFORM_CUSTOM} > ${SD_CARD}/sd_card/platform_desc.txt

clean:
	rm -rf ${SD_CARD} ${BUILD_DIR}
