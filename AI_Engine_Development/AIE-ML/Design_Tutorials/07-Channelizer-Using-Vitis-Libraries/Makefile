#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO                               = @echo
export TARGET                      ?= hw_emu

# Platform Selection...
VERSAL_VITIS_PLATFORM      = xilinx_vek280_base_202520_1
VITIS_PLATFORM_DIR         = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# Set SysRoot, RootFS and Image
export VITIS_SYSROOTS   = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-amd-linux
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
	$(MAKE) compile -C aie/channelizer
	$(MAKE) all -C hls/channelizer_dma_src
	$(MAKE) all -C hls/packet_sender
	$(MAKE) all -C hls/packet_receiver
	$(MAKE) all -C hls/ifft_mid_splitter
	$(MAKE) all -C hls/ifft_mid_transpose
	$(MAKE) all -C hls/ifft_mid_joiner
	$(MAKE) all -C hls/merge_8x4
	$(MAKE) all -C hls/channelizer_dma_snk
	$(MAKE) all -C ps_apps
	$(MAKE) all -C vitis

clean:
	$(MAKE) clean -C aie/channelizer
	$(MAKE) clean -C hls/channelizer_dma_src
	$(MAKE) clean -C hls/packet_sender
	$(MAKE) clean -C hls/packet_receiver
	$(MAKE) clean -C hls/ifft_mid_splitter
	$(MAKE) clean -C hls/ifft_mid_transpose
	$(MAKE) clean -C hls/ifft_mid_joiner
	$(MAKE) clean -C hls/merge_8x4
	$(MAKE) clean -C hls/channelizer_dma_snk
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter* _ide vivado.jou vivado.log vivado_*
