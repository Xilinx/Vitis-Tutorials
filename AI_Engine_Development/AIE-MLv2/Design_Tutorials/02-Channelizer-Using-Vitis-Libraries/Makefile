#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO                               = @echo

export TARGET          ?= hw_emu
export SYSROOT          = ${YOCTO_ARTIFACTS}/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk/sysroots/cortexa72-cortexa53-amd-linux

export PLATFORM         = ${PLATFORM_REPO_PATHS}/vek385_base_reva/vek385_base_reva.xpfm

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
	@/bin/rm -rf .crashReporter* _ide vivado.jou vivado.log vivado_* .Xil
