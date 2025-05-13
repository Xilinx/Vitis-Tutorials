#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

TARGET                ?= hw
IP_DIR                := $(abspath ../aie/sar_top_1engine)
XO                    := ../hls/image_buffer_1engine/image_buffer_1engine_wrapper.xo
GRAPH_O               := ${IP_DIR}/libadf.a
XSA                   := my.xsa
STIMULUS_IO           := ${IP_DIR}/data/antenna_coord_data.txt \
                         ${IP_DIR}/data/pulse_data.txt \
                         ${IP_DIR}/data/R0_range_data.txt \
			 ${IP_DIR}/data/vfs_imA_raw_2_pulses.txt \
			 ${IP_DIR}/data/vfs_imA_raw_586_pulses.txt

APP_EXE          := host.exe
APP_SRC          := host.cpp
APP_SRC_CTRL     := ${IP_DIR}/Work/ps/c_rts/aie_control_xrt.cpp

GCC              := ${XILINX_VITIS}/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++

GCC_INCLUDES     := -I${SYSROOT}/usr/include \
                    -I${SYSROOT}/usr/include/xrt \
                    -I${SYSROOT}/usr/include/xaiengine \
                    -I${XILINX_VITIS}/aietools/include \
	            -I${DSPLIB_ROOT}/L2/include/aie \
	            -I${DSPLIB_ROOT}/L1/include/aie \
	            -I${DSPLIB_ROOT}/L1/src/aie \
                    -I../aie/sar_top_1engine \
                    -I../aie/sar_engine \
                    -I../aie/ifft2k_async \
                    -I../aie/range_gen \
                    -I../aie/diff3dsq \
                    -I../aie/sqrt_lib \
                    -I../aie/dR_comp \
                    -I../aie/fmod_floor \
                    -I../aie/expjx \
                    -I../aie/cos_lib \
                    -I../aie/sin_lib \
                    -I../aie/bp_update \
                    -I../aie/interp1

GCC_C_FLAGS      := -O -Wall -std=c++17 -DXAIE_DEBUG -D__AIE_ARCH__=10\
                    -pthread \
                    --sysroot=${SYSROOT}

GCC_L_FLAGS      := -L$(SYSROOT)/usr/lib \
                    -L${XILINX_VITIS}/aietools/lib/aarch64.o \
                    -ladf_api_xrt -lxaiengine -lxrt_core -lxrt_coreutil \
                    --sysroot=$(SYSROOT)

SD_CARD               := $(abspath ../package1)
BUILD_DIR             := build_${TARGET}

SYSTEM_CFG            := system.cfg

.PHONY: help all clean

help::
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all:	${APP_EXE} ${BUILD_DIR} ${BUILD_DIR}/${XSA} ${SD_CARD}

${APP_EXE}:	${APP_SRC_CTRL} ${SYSROOT} ${APP_SRC}
	${GCC} ${GCC_C_FLAGS} ${APP_SRC} ${APP_SRC_CTRL} ${GCC_INCLUDES} ${GCC_L_FLAGS} -o ${APP_EXE}

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${XSA}: ${XO} ${GRAPH_O} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			${GRAPH_O} \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

${SD_CARD}: ${KERNEL_IMAGE} ${ROOTFS} ${STIMULUS_IO} ${BUILD_DIR}/${XSA} ${APP_EXE}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			--package.out_dir ${SD_CARD} \
			--package.boot_mode=sd \
			--package.kernel_image=${KERNEL_IMAGE} \
			--package.rootfs=${ROOTFS}  \
			--package.defer_aie_run \
			--package.image_format=ext4 \
			$(addprefix --package.sd_file=..//,${APP_EXE}) \
			$(addprefix --package.sd_file=,${STIMULUS_IO}) \
			$(addprefix --package.sd_file=..//,embedded_exec.sh) \
			${GRAPH_O} \
			${XSA} \
			2>&1 | tee vpp_pack.log
	echo AIE_PKG_DIR=${IP_DIR}/Work > ${SD_CARD}/aiesim_options.txt
	echo AIE_DUMP_VCD=sar_dump >> ${SD_CARD}/aiesim_options.txt



clean:
	/bin/rm -rf ${SD_CARD} ${BUILD_DIR} *.o $(APP_EXE)

