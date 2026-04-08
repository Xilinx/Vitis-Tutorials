#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO                               = @echo
export TARGET                      ?= hw_emu

export AIE_SIM_ONLY                := false

VERSAL_VITIS_PLATFORM      = xilinx_vck190_base_202610_1
VITIS_PLATFORM_DIR         = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

export PLATFORM            = ${VITIS_PLATFORM_XPFM}
export SYSROOT             = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-amd-linux
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
	$(MAKE) compile -C aie/farrow_final
	$(MAKE) all     -C hls/farrow_dma_snk
	$(MAKE) all     -C hls/farrow_dma_src
	$(MAKE) all     -C ps_apps
	$(MAKE) all     -C vitis

clean:
	$(MAKE) clean -C aie/farrow_final
	$(MAKE) clean -C hls/farrow_dma_snk
	$(MAKE) clean -C hls/farrow_dma_src
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter* .Xil vivado.jou vivado.log vfs_work AIECompiler.log
