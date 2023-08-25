#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                               = @echo
export TARGET                      ?= hw_emu

RELEASE=2023.1
BOARD=vck190
BASE_NUM=202310_1

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
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all:	pfa1008

pfa1008:
	$(MAKE) all -C aie/pfa1008
	$(MAKE) all -C hls/pfa1008_dma_src
	$(MAKE) all -C hls/pfa1008_permute_i
	$(MAKE) all -C hls/pfa1008_transpose1
	$(MAKE) all -C hls/pfa1008_transpose2
	$(MAKE) all -C hls/pfa1008_permute_o
	$(MAKE) all -C hls/pfa1008_dma_snk
	$(MAKE) all -C ps_apps/hw_emu
	$(MAKE) all -C vitis

clean:
	$(MAKE) clean -C aie/pfa1008
	$(MAKE) clean -C hls/pfa1008_dma_src
	$(MAKE) clean -C hls/pfa1008_permute_i
	$(MAKE) clean -C hls/pfa1008_transpose1
	$(MAKE) clean -C hls/pfa1008_transpose2
	$(MAKE) clean -C hls/pfa1008_permute_o
	$(MAKE) clean -C hls/pfa1008_dma_snk
	$(MAKE) clean -C ps_apps/hw_emu
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter*


