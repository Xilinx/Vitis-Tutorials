#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO                               = @echo
export TARGET                      ?= hw_emu

export AIE_SIM_ONLY                := false

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
.PHONY: clean help gen_vectors

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

gen_vectors:
	matlab -batch "gen_vectors"

all:	farrow_design

farrow_design:
	$(MAKE) all     -C platform
	$(MAKE) compile -C aie/farrow_final
	$(MAKE) all     -C hls/farrow_dma_snk
	$(MAKE) all     -C hls/farrow_dma_src
	$(MAKE) all     -C ps_apps/hw_emu
	$(MAKE) all     -C vitis

clean:
	$(MAKE) clean -C platform
	$(MAKE) clean -C aie/farrow_final
	$(MAKE) clean -C hls/farrow_dma_snk
	$(MAKE) clean -C hls/farrow_dma_src
	$(MAKE) clean -C ps_apps/hw_emu
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter* .Xil vivado.jou vivado.log
