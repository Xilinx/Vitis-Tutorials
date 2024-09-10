# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_DIR	   := $(shell readlink -f $(RELATIVE_PROJECT_DIR))
PROJECT_SYSTEM_DIR := $(PROJECT_DIR)
PROJECT_PS_DIR     := $(PROJECT_SYSTEM_DIR)/../mmult
PROJECT_PL_DIR     := $(PROJECT_SYSTEM_DIR)/../mmult_kernels
PROJECT_HWLINK_DIR := $(PROJECT_SYSTEM_DIR)/../mmult_system_hw_link

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


BOARD=vck190

#
# tools
#

RM = rm -f
RMDIR = rm -rf


VPP ?= ${XILINX_VITIS}/bin/v++

KERNEL_IMAGE = $(COMMON_IMAGE_VERSAL)/Image
ROOTFS = $(COMMON_IMAGE_VERSAL)/rootfs.ext4

SYS_XCLBIN = $(PROJECT_SYSTEM_DIR)/Hardware/binary_container_1.xclbin

EMBEDDED_PACKAGE_OUT = $(PROJECT_SYSTEM_DIR)/Hardware/package
EMBEDDED_EXEC_SCRIPT = $(PROJECT_SYSTEM_DIR)/Hardware/embedded_exec.sh

#
# variables
#


#
# primary build targets
#

package:
	-@echo Running Package Step for the System Project.
	-@echo $(VPP) --package --config $(PROJECT_SYSTEM_DIR)/Hardware/package.cfg  $(PROJECT_HWLINK_DIR)/Hardware/binary_container_1.xsa  --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE) -o $(SYS_XCLBIN)
	$(VPP) --package --config $(PROJECT_SYSTEM_DIR)/Hardware/package.cfg \
	$(PROJECT_HWLINK_DIR)/Hardware/binary_container_1.xsa  \
	--platform $(VITIS_PLATFORM_XPFM) \
	--package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE) \
	--package.sd_file $(EMBEDDED_EXEC_SCRIPT)					\
	--package.boot_mode=sd                            \
	-o $(SYS_XCLBIN)


clean:
	${RMDIR} $(EMBEDDED_PACKAGE_OUT)
	${RMDIR} $(EMBEDDED_PACKAGE_OUT).build .Xil
	${RM} $(SYS_XCLBIN)*
	${RM} *.xclbin *.log *~

.PHONY: all package clean show_dir

all: show_dir package
