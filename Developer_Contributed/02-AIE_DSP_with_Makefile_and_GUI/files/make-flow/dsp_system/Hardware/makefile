# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_SYSTEM_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

PROJECT_AIE_DIR    = $(PROJECT_SYSTEM_DIR)/../dsp_AIE
PROJECT_PS_DIR     = $(PROJECT_SYSTEM_DIR)/../dsp_PS
PROJECT_PL_DIR     = $(PROJECT_SYSTEM_DIR)/../dsp_PL
PROJECT_HWLINK_DIR = $(PROJECT_SYSTEM_DIR)/../dsp_system_hw_link

show_dir:
		-@echo from dsp_system, RELATIVE PROJECT DIR is:  $(RELATIVE_PROJECT_DIR)
		-@echo from dsp_system, CURRENT  PROJECT DIR is:  $(PROJECT_SYSTEM_DIR)
		-@echo from dsp_system, PL	     PROJECT DIR is:  $(PROJECT_PL_DIR)
		-@echo from dsp_system, PS	     PROJECT DIR is:  $(PROJECT_PS_DIR)
		-@echo from dsp_system, AIE	     PROJECT DIR is:  $(PROJECT_AIE_DIR)
		-@echo from dsp_system, HW LINK	 PROJECT DIR is:  $(PROJECT_HWLINK_DIR)


#
# tools
#

RM = rm -f
RMDIR = rm -rf

VPP ?= ${XILINX_VITIS}/bin/v++

#
# variables
#

KERNEL_IMAGE = $(COMMON_IMAGE_VERSAL)/Image
ROOTFS       = $(COMMON_IMAGE_VERSAL)/rootfs.ext4

AIE_ARCHIVE = $(PROJECT_AIE_DIR)/Hardware/libadf.a

AIE_XCLBIN = $(PROJECT_SYSTEM_DIR)/Hardware/binary_container_1.xclbin

EMBEDDED_PACKAGE_OUT = $(PROJECT_SYSTEM_DIR)/Hardware/package
EMBEDDED_EXEC_SCRIPT = $(PROJECT_SYSTEM_DIR)/Hardware/embedded_exec.sh

#
# primary build targets
#
PACKAGE_FLAGS= --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE)

#package:
#	-@echo Running Package Step for the System Project.
#	-@echo $(VPP) --package --config package.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin
#	$(VPP) --package --config package.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin
#	-@echo Running Package Step for the System Project with AIE configured to not stop for Debug.
#	-@echo $(VPP) --package --config package_aie_debug.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin
#	$(VPP) --package --config package_aie_debug.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin

package:
	-@echo Running Package Step for the System Project.
	$(VPP) --package --config $(PROJECT_SYSTEM_DIR)/Hardware/package.cfg \
	$(PACKAGE_FLAGS) -o $(AIE_XCLBIN) \
	--package.defer_aie_run                                             		\
	--package.sd_file $(EMBEDDED_EXEC_SCRIPT)					\
	--package.boot_mode=sd                                              		\
	$(AIE_ARCHIVE)  $(PROJECT_HWLINK_DIR)/Hardware/binary_container_1.xsa
#	--package.sd_file $(AIE_XCLBIN) 	                                    	\

clean:
	$(RMDIR) ./package_aie_debug.build ./package_aie_debug .Xil
	$(RMDIR) ./package ./package.build
	${RM} *.xclbin* *.log

.PHONY: all package clean

all: package
