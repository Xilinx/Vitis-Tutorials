# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT



BOARD=vck190

#
# tools
#

RM = rm -f
RMDIR = rm -rf


#XILINX_VITIS ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vitis/2022.2

VPP ?= ${XILINX_VITIS}/bin/v++



#VITIS_PLATFORM = xilinx_$(BOARD)_base_202220_1
#VITIS_PLATFORM_DIR = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(XILINX_VITIS)/base_platforms/$(VITIS_PLATFORM)/$(VITIS_PLATFORM).xpfm
KERNEL_IMAGE = $(COMMON_IMAGE_VERSAL)/Image
ROOTFS = $(COMMON_IMAGE_VERSAL)/rootfs.ext4

#
# variables
#


#
# primary build targets
#

package:
	-@echo Running Package Step for the System Project.
	-@echo $(VPP) --package --config package.cfg  ../../mmult_system_hw_link/Hardware/binary_container_1.xsa  --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE) -o binary_container_1.xclbin
	$(VPP) --package --config package.cfg   ../../mmult_system_hw_link/Hardware/binary_container_1.xsa  --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE)  -o binary_container_1.xclbin


clean:
	$(RMDIR) ./package.build
	${RMDIR} package

.PHONY: all package clean

all: package
