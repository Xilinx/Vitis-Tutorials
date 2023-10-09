#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                  := @echo

TARGET                ?= hw

IP_DIR                := ../aie/pfa1008
XO                    := ../hls/pfa1008_dma_src/pfa1008_dma_src_wrapper.xo \
                         ../hls/pfa1008_dma_snk/pfa1008_dma_snk_wrapper.xo \
                         ../hls/pfa1008_permute_i/pfa1008_permute_i_wrapper.xo \
			 ../hls/pfa1008_permute_o/pfa1008_permute_o_wrapper.xo \
			 ../hls/pfa1008_transpose1/pfa1008_transpose1_wrapper.xo \
			 ../hls/pfa1008_transpose2/pfa1008_transpose2_wrapper.xo

GRAPH_O               := ${IP_DIR}/libadf.a
XSA                   := my.xsa
STIMULUS_IO           := ${IP_DIR}/data/sig_i.txt ${IP_DIR}/data/sig_o.txt
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

all: ${BUILD_DIR} ${BUILD_DIR}/${XSA} ${SD_CARD}

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${XSA}: ${XO} ${GRAPH_O} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			../${GRAPH_O} \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

${SD_CARD}: ${BUILD_DIR}/${XSA} ${PS_EXE} ${GRAPH_O} ${KERNEL_IMAGE} ${ROOTFS} ${STIMULUS_IO}
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
			$(addprefix --package.sd_file=../,${STIMULUS_IO}) \
			$(addprefix --package.sd_file=../,embedded_exec.sh) \
			2>&1 | tee vpp_pack.log
	echo ${PLATFORM_CUSTOM} > ${SD_CARD}/sd_card/platform_desc.txt

clean:
	rm -rf ${SD_CARD} ${BUILD_DIR}

