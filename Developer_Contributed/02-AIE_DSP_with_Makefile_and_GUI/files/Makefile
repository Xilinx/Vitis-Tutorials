# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

SHELL := /bin/bash

PHONY  = help all clean
PHONY += kernels hw_link system host aie
PHONY += prepare_files build
.PHONY: $(PHONY)


# =========================================================
# Source directories
# =========================================================
RELATIVE_PROJECT_DIR := ./
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))


# =======================================================
# global variables for this tutorial
# =======================================================
#RELEASE=2022.2
RELEASE=2023.1
BOARD=vck190
BASE_NUM=202310_1
#BASE_NUM=202220_1

# Platform Selection...
VERSAL_VITIS_PLATFORM = xilinx_${BOARD}\_base_${BASE_NUM}
VITIS_PLATFORM_DIR = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm
# Set SysRoot, RootFS and Image
export VITIS_SYSROOTS      = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT = ${VITIS_SYSROOTS}
export KERNEL_IMAGE = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export XLNX_VERSAL = ${COMMON_IMAGE_VERSAL}
export PLATFORM = ${VITIS_PLATFORM_XPFM}




# =========================================================
# commands directories
# =========================================================

hw:
		$(MAKE) all

all:
		$(MAKE) prepare_files
		$(MAKE) sd_card


prepare_files:
	source $(PROJECT_REPO)/make-flow/local_var_setup.sh
	cp -fr $(PROJECT_REPO)/src/ip/aie/data  							$(PROJECT_REPO)/make-flow/dsp_AIE/
	cp -fr $(PROJECT_REPO)/src/ip/aie/src               	$(PROJECT_REPO)/make-flow/dsp_AIE/
	cp -fr $(PROJECT_REPO)/src/ip/mm2s_aie/src          	$(PROJECT_REPO)/make-flow/dsp_PL/
	cp -f  $(PROJECT_REPO)/src/ip/s2mm_aie/src/s2mm.cpp 	$(PROJECT_REPO)/make-flow/dsp_PL/src/
	cp -fr $(PROJECT_REPO)/src/ps_apps/aie_test/src     	$(PROJECT_REPO)/make-flow/dsp_PS/
	cp -f  $(PROJECT_REPO)/src/vitis/src/system.cfg				$(PROJECT_REPO)/make-flow/dsp_system_hw_link/


sd_card:
		$(MAKE) kernels
		$(MAKE) aie
		$(MAKE) host
		$(MAKE) hw_link
		$(MAKE) system

clean:
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_system_hw_link/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_system/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_PS/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_PL/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_AIE/Hardware clean
	rm -rf $(PROJECT_REPO)/make-flow/dsp_AIE/src
	rm -rf $(PROJECT_REPO)/make-flow/dsp_AIE/data
	rm -rf $(PROJECT_REPO)/make-flow/dsp_PL/src
	rm -rf $(PROJECT_REPO)/make-flow/dsp_PS/src


aie:
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_AIE/Hardware all

kernels:
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_PL/Hardware all

hw_link:
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_system_hw_link/Hardware all

system:
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_system/Hardware all

host:
	$(MAKE) -C $(PROJECT_REPO)/make-flow/dsp_PS/Hardware all
