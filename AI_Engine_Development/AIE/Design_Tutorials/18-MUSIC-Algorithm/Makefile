#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                               = @echo
export TARGET                      ?= hw

RELEASE=2024.1

PLATFORM_NAME              = xilinx_vck190_base_202410_1
PLATFORM_PATH              = ${PLATFORM_REPO_PATHS}

export PLATFORM            = ${PLATFORM_PATH}/${PLATFORM_NAME}/${PLATFORM_NAME}.xpfm
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

all:	music

music:
	$(MAKE) all -C aie
	$(MAKE) all -C hls/mm2s
	$(MAKE) all -C hls/s2mm
	$(MAKE) all -C ps_apps
	$(MAKE) all -C vitis

clean:
	$(MAKE) clean -C aie
	$(MAKE) clean -C hls/mm2s
	$(MAKE) clean -C hls/s2mm
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter*


