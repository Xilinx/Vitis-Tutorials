#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Chandra Sekar Balakrishnan

ECHO                               = @echo
export TARGET                      ?= hw_emu

export AIE_SIM_ONLY                := false

PLATFORM_USE	  := xilinx_vek280_base_202410_1
PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

export COMMON_IMAGE_VERSAL = /proj/xbuilds/2024.1_daily_latest/internal_platforms/sw/versal/xilinx-versal-common-v2024.1
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
	$(MAKE) compile -C aie_ml
	$(MAKE) all     -C ps_apps/hw_emu
	$(MAKE) all     -C vitis

clean:
	$(MAKE) clean -C aie_ml
	$(MAKE) clean -C ps_apps/hw_emu
	$(MAKE) clean -C vitis
	@/bin/rm -rf .crashReporter* .Xil vivado.jou vivado.log