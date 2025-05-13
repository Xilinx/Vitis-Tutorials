#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

export TARGET        ?= hw

PLATFORM_NAME		= xilinx_vck190_base_202510_1
PLATFORM_PATH		= ${PLATFORM_REPO_PATHS}

export PLATFORM            = ${PLATFORM_PATH}/${PLATFORM_NAME}/${PLATFORM_NAME}.xpfm
export SYSROOT             = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-amd-linux
export KERNEL_IMAGE        = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS              = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export PREBUILT_LINUX_PATH = ${COMMON_IMAGE_VERSAL}

.PHONY:		all dev1 dev8 clean_dev1 clean_dev8 clean_all

all:	dev1 dev8

dev1:
	$(MAKE) all -C aie/sar_top_1engine
	$(MAKE) all -C hls/image_buffer_1engine
	$(MAKE) all -C device1

dev8:
	$(MAKE) all -C aie/sar_top_8engine
	$(MAKE) all -C hls/image_buffer_8engine
	$(MAKE) all -C device8

clean_dev1:
	$(MAKE) clean -C aie/sar_top_1engine
	$(MAKE) clean -C hls/image_buffer_1engine
	$(MAKE) clean -C device1

clean_dev8:
	$(MAKE) clean -C aie/sar_top_8engine
	$(MAKE) clean -C hls/image_buffer_8engine
	$(MAKE) clean -C device8

clean_all:
	$(MAKE) clean -C aie/sar_top_1engine
	$(MAKE) clean -C aie/sar_top_8engine
	$(MAKE) clean -C aie/bp_update
	$(MAKE) clean -C aie/cos_lib
	$(MAKE) clean -C aie/diff3dsq
	$(MAKE) clean -C aie/dR_comp
	$(MAKE) clean -C aie/expjx
	$(MAKE) clean -C aie/fmod_floor
	$(MAKE) clean -C aie/ifft2k_async
	$(MAKE) clean -C aie/interp1
	$(MAKE) clean -C aie/sar_engine
	$(MAKE) clean -C aie/sin_lib
	$(MAKE) clean -C aie/sqrt_lib
	$(MAKE) clean -C hls/image_buffer_1engine
	$(MAKE) clean -C hls/image_buffer_8engine
	$(MAKE) clean -C device1
	$(MAKE) clean -C device8
	@/bin/rm -rf matlab/vfs_work

