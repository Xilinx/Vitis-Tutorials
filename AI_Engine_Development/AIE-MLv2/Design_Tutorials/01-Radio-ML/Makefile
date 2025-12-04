#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani
#

export TARGET          ?= hw_emu
export SYSROOT          = ${YOCTO_ARTIFACTS}/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk/sysroots/cortexa72-cortexa53-amd-linux

export PLATFORM         = ${PLATFORM_REPO_PATHS}/vek385_base_reva/vek385_base_reva.xpfm

##############
# Build Flow #
##############
.PHONY:  help clean all

help:
	@echo "Makefile Usage:"
	@echo "Run 'make my-venv' to build a Python Virtual Environment for this tutorial"
	@echo "Afterwards run 'make all' to build the design for AIE-ML v2"
	@echo "Run 'make clean' to remove all the generated files"

all:
	$(MAKE) all -C aie/radioml_top
	$(MAKE) all -C ps_apps
	$(MAKE) all -C vitis

my-venv:	create_venv.sh
	./create_venv.sh

clean:
	$(MAKE) clean -C aie/radioml_top
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis
	@/bin/rm -rf .Xil vivado*
#	@/bin/rm -rf my-venv .ipynb_checkpoints
