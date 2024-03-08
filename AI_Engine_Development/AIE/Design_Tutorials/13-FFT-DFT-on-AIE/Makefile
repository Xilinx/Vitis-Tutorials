#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                               = @echo

export TARGET := hw

RELEASE=2024.1
BOARD=vck190
BASE_NUM=202410_1

# Platform Selection...
VERSAL_VITIS_PLATFORM      = xilinx_${BOARD}\_base_${BASE_NUM}
VITIS_PLATFORM_DIR         = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# Set SysRoot, RootFS and Image
export VITIS_SYSROOTS   = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT = ${VITIS_SYSROOTS}
export KERNEL_IMAGE     = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS           = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export XLNX_VERSAL      = ${COMMON_IMAGE_VERSAL}
export PLATFORM         = ${VITIS_PLATFORM_XPFM}
export SYSROOT          = ${SDKTARGETSYSROOT}

##############
# Build Flow #
##############
.PHONY: clean help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to compile and simulate all designs"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all:	all_fft

all_fft:
	$(MAKE) all -C dft16 all
	$(MAKE) all -C fft32_dsplib all
	$(MAKE) all -C fft32_dsplib_split  all
	$(MAKE) all -C fft32_dsplib_ssr all
	$(MAKE) all -C fft32_r2 all

clean:
	$(MAKE) clean -C dft16
	$(MAKE) clean -C fft32_dsplib
	$(MAKE) clean -C fft32_dsplib_split
	$(MAKE) clean -C fft32_dsplib_ssr
	$(MAKE) clean -C fft32_r2
	@/bin/rm -rf .crashReporter*

