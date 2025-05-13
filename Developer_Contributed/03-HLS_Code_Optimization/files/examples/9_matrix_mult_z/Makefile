# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

SHELL := /bin/bash

.PHONY: help hls_kernels hls_clean zcu102_all zcu102_clean all clean sd_card run show_dir

#========================================================
# Source directories
# =========================================================
RELATIVE_PROJECT_DIR := ./
PROJECT_DIR := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

PROJECT_PL_DIR     := $(PROJECT_DIR)/make-flow/zcu102/mmult_kernels
PROJECT_PS_DIR     := $(PROJECT_DIR)/make-flow/zcu102/mmult
PROJECT_HWLINK_DIR := $(PROJECT_DIR)/make-flow/zcu102/mmult_system_hw_link
PROJECT_SYSTEM_DIR := $(PROJECT_DIR)/make-flow/zcu102/mmult_system


show_dir:
		-@echo " "
		-@echo " "
		-@echo "from $(realpath .)"
		-@echo "RELATIVE    PROJECT DIR is:  $(RELATIVE_PROJECT_DIR)"
		-@echo "CURRENT     PROJECT DIR is:  $(PROJECT_DIR)"
		-@echo "PL KERNELS  PROJECT DIR is:  $(PROJECT_PL_DIR)"
		-@echo "PS ARM HOST PROJECT DIR is:  $(PROJECT_PS_DIR)"
		-@echo "HW LINK     PROJECT DIR is:  $(PROJECT_HWLINK_DIR)"
		-@echo "SYSTEM      PROJECT DIR is:  $(PROJECT_SYSTEM_DIR)"
		-@echo " "
		-@echo " "


EMBEDDED_PACKAGE_OUT = $(PROJECT_SYSTEM_DIR)/Hardware/package
EMBEDDED_EXEC_SCRIPT = $(PROJECT_SYSTEM_DIR)/Hardware/embedded_exec.sh
PACKAGE_FILES += $(EMBEDDED_EXEC_SCRIPT)
SD_FILES_WITH_PREFIX = $(foreach sd_file,$(PACKAGE_FILES),--package.sd_file $(sd_file))

# =======================================================
# global variables for this tutorial
# =======================================================
#RELEASE=2022.2
RELEASE=2025.1
BOARD=zcu102
BASE_NUM=202510_1
#BASE_NUM=202220_1

# Platform Selection...
ZYNQMP_VITIS_PLATFORM      = xilinx_${BOARD}\_base_${BASE_NUM}
VITIS_PLATFORM_DIR         = ${PLATFORM_REPO_PATHS}/${ZYNQMP_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${ZYNQMP_VITIS_PLATFORM}.xpfm
# Set SysRoot, RootFS and Image
export VITIS_SYSROOTS   = ${COMMON_IMAGE_ZYNQMP}/sysroots/cortexa72-cortexa53-amd-linux
export SDKTARGETSYSROOT = ${VITIS_SYSROOTS}
export KERNEL_IMAGE     = ${COMMON_IMAGE_ZYNQMP}/Image
export ROOTFS           = ${COMMON_IMAGE_ZYNQMP}/rootfs.ext4
export XLNX_VERSAL      = ${COMMON_IMAGE_ZYNQMP}
export PLATFORM         = ${VITIS_PLATFORM_XPFM}




hls_clean:
	rm -rf *_prj *.log *~ ./logs

hls_kernels:
	vitis-run --mode hls --tcl ./run_matrix_mult_script_zcu102.tcl

zcu102_all:
	$(MAKE) -C ./make-flow/zcu102 all

zcu102_clean:
	$(MAKE) -C ./make-flow/zcu102 clean

run:		all
sd_card:	all

kernels:		hls_kernels
all:			show_dir zcu102_all
clean:			show_dir hls_clean zcu102_clean
