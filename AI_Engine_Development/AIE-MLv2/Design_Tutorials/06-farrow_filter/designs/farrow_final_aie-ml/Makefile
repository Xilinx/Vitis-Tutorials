#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Chandra Sekar Balakrishnan

ECHO                               = @echo
export TARGET                      ?= hw_emu

export AIE_SIM_ONLY                := false

#PLATFORM_USE	  := xilinx_vek280_base_202610_1
PLATFORM_USE	  		:= vek385_base_reva
export PLATFORM         := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm
export SYSROOT			= ${YOCTO_ARTIFACTS}/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk/sysroots/cortexa72-cortexa53-amd-linux

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
