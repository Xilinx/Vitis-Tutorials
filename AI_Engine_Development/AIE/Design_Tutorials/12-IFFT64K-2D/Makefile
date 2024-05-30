#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                               = @echo
export TARGET                      ?= hw_emu

RELEASE=2024.1

TOP_DIR                   ?= $(shell readlink -f .)
PLATFORM_NAME              = xilinx_vck190_base_202410_1
PLATFORM_PATH              = ${TOP_DIR}/platform/platform_repo

export PLATFORM            = ${PLATFORM_PATH}/${PLATFORM_NAME}/export/${PLATFORM_NAME}/${PLATFORM_NAME}.xpfm
export SYSROOT             = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export KERNEL_IMAGE        = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS              = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export PREBUILT_LINUX_PATH = ${COMMON_IMAGE_VERSAL}

##############
# Build Flow #
##############
.PHONY: clean help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all:	ifft

ifft:
	$(MAKE) all -C platform
	$(MAKE) all -C aie/ifft64k
	$(MAKE) all -C hls/ifft_dma_src
	$(MAKE) all -C hls/ifft_transpose
	$(MAKE) all -C hls/ifft_dma_snk
	$(MAKE) all -C ps_apps
	$(MAKE) all -C vitis

clean:
	$(MAKE) clean -C platform
	$(MAKE) clean -C aie/ifft64k
	$(MAKE) clean -C hls/ifft_dma_src
	$(MAKE) clean -C hls/ifft_transpose
	$(MAKE) clean -C hls/ifft_dma_snk
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter*


