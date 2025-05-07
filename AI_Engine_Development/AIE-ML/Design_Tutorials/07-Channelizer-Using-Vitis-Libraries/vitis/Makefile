#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO                  := @echo

TARGET                ?= hw

IP_DIR                := ../aie/channelizer
DATA_DIR              := ../aie/channelizer/data
XO_VSS                := ../hls/split_1x16/split_1x16_wrapper.xo \
                         ../hls/merge_4x1/merge_4x1_wrapper.xo \
                         ../hls/ifft_front_transpose/ifft_front_transpose_wrapper.xo \
                         ../hls/ifft_transpose/ifft_transpose_wrapper.xo \
                         ../hls/ifft_back_transpose/ifft_back_transpose_wrapper.xo \
                         ../hls/merge_8x4/merge_8x4_wrapper.xo

XO                    := ../hls/channelizer_dma_src/channelizer_dma_src_wrapper.xo \
                         ../hls/channelizer_dma_snk/channelizer_dma_snk_wrapper.xo

GRAPH_O               := ${IP_DIR}/libadf.a
XSA                   := my.xsa
STIMULUS_IO           := ${DATA_DIR}/sig_o.txt ${DATA_DIR}/sig_i.txt
PS_EXE                := ../ps_apps/host.exe

VSS                   := channelizer
VSSFILE               := ${VSS}/${VSS}.vss
VSSCFG                := vss.cfg
PART                  := xcve2802-vsvh1760-2MP-e-S

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

${BUILD_DIR}/${VSS}: ${BUILD_DIR} ${GRAPH_O} ${XO_VSS} ${VSSCFG}
	v++ --link --mode vss --part ${PART} --save-temps --out_dir ${VSS} --config ${VSSCFG} ${GRAPH_O} ${XO_VSS}

${BUILD_DIR}/${XSA}: ${BUILD_DIR}/${VSS} ${XO} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			$(addprefix ../, ${VSSFILE}) \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

${SD_CARD}: ${BUILD_DIR}/${XSA} ${PS_EXE} ${KERNEL_IMAGE} ${ROOTFS} ${STIMULUS_IO}
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
	rm -rf ${SD_CARD} ${BUILD_DIR} _x *.xo ${VSS} xcd.log v++* .Xil