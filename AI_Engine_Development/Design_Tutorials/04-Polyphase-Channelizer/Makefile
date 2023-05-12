#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                               = @echo

export TARGET                      = hw_emu

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

all:	channelizer

channelizer:
	$(MAKE) all -C aie/m16_ssr8
	$(MAKE) all -C hls/m16_ssr8_permute_fb_i
	$(MAKE) all -C hls/m16_ssr8_permute_fb_o
	$(MAKE) all -C hls/m16_ssr8_cyclic_shift
	$(MAKE) all -C hls/dma_stream_src
	$(MAKE) all -C hls/dma_stream_snk
	$(MAKE) all -C ps_apps/hw_emu
	$(MAKE) all -C vitis/final

clean:
	$(MAKE) clean -C aie/m16_ssr8
	$(MAKE) clean -C aie/m16_ssr8_dft
	$(MAKE) clean -C aie/m16_ssr8_filterbank
	$(MAKE) clean -C hls/m16_ssr8_permute_fb_i
	$(MAKE) clean -C hls/m16_ssr8_permute_fb_o
	$(MAKE) clean -C hls/m16_ssr8_cyclic_shift
	$(MAKE) clean -C hls/dma_stream_src
	$(MAKE) clean -C hls/dma_stream_snk
	$(MAKE) clean -C ps_apps/hw_emu
	$(MAKE) clean -C vitis/final
	@/bin/rm -rf .crashReporter*


