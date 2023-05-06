# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
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
PROJECT_DIR := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#PROJECT_AIE_DIR    := $(PROJECT_DIR)/make-flow/dsp_AIE
#PROJECT_PL_DIR     := $(PROJECT_DIR)/make-flow/dsp_PL
#PROJECT_PS_DIR     := $(PROJECT_DIR)/make-flow/dsp_PS
#PROJECT_HWLINK_DIR := $(PROJECT_DIR)/make-flow/dsp_system_hw_link
#PROJECT_SYSTEM_DIR := $(PROJECT_DIR)/make-flow/dsp_system

EMBEDDED_PACKAGE_OUT = $(PROJECT_DIR)/make-flow/dsp_system/Hardware/package
EMBEDDED_EXEC_SCRIPT = $(PROJECT_DIR)/make-flow/dsp_system/Hardware/embedded_exec.sh
PACKAGE_FILES += $(EMBEDDED_EXEC_SCRIPT)
SD_FILES_WITH_PREFIX = $(foreach sd_file,$(PACKAGE_FILES),--package.sd_file $(sd_file))


# =======================================================
# global variables for this tutorial
# =======================================================
#RELEASE=2022.2
RELEASE=2023.1
BOARD=vck190
BASE_NUM=202310_1
#BASE_NUM=202220_1

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



# =========================================================
# commands directories
# =========================================================

show_dir:
		-@echo " "
		-@echo starting folder PROJECT_DIR is: ${PROJECT_DIR}
		-@echo " "
		$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_AIE/Hardware show_dir
		-@echo " "
		$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_PL/Hardware show_dir
		-@echo " "
		$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_PS/Hardware show_dir
		-@echo " "
		$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_system_hw_link/Hardware show_dir
		-@echo " "
		$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_system/Hardware show_dir
		-@echo " "



run:		all
sd_card:	all

all:
		$(MAKE) prepare_files
		$(MAKE) build


prepare_files:
#	source $(PROJECT_DIR)/make-flow/local_var_setup.sh
	cp -fr $(PROJECT_DIR)/src/ip/aie/data  			$(PROJECT_DIR)/make-flow/dsp_AIE/
	cp -fr $(PROJECT_DIR)/src/ip/aie/src               	$(PROJECT_DIR)/make-flow/dsp_AIE/
	cp -fr $(PROJECT_DIR)/src/ip/mm2s_aie/src          	$(PROJECT_DIR)/make-flow/dsp_PL/
	cp -f  $(PROJECT_DIR)/src/ip/s2mm_aie/src/s2mm.cpp 	$(PROJECT_DIR)/make-flow/dsp_PL/src/
	cp -fr $(PROJECT_DIR)/src/ps_apps/aie_test/src     	$(PROJECT_DIR)/make-flow/dsp_PS/
	cp -f  $(PROJECT_DIR)/src/vitis/src/system.cfg		$(PROJECT_DIR)/make-flow/dsp_system_hw_link/


build:
		$(MAKE) kernels
		$(MAKE) aie
		$(MAKE) host
		$(MAKE) hw_link
		$(MAKE) system

clean:
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_system_hw_link/Hardware clean
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_system/Hardware clean
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_PS/Hardware clean
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_PL/Hardware clean
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_AIE/Hardware clean
	rm -rf $(PROJECT_DIR)/make-flow/dsp_AIE/src
	rm -rf $(PROJECT_DIR)/make-flow/dsp_AIE/data
	rm -rf $(PROJECT_DIR)/make-flow/dsp_PL/src
	rm -rf $(PROJECT_DIR)/make-flow/dsp_PS/src


aie:
	$(MAKE) show_dir
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_AIE/Hardware all

kernels:
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_PL/Hardware all

hw_link:
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_system_hw_link/Hardware all

system:
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_system/Hardware all

host:
	$(MAKE) -C $(PROJECT_DIR)/make-flow/dsp_PS/Hardware all
