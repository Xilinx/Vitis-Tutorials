# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


#
# tools
#

RM = rm -f
RMDIR = rm -rf


#XILINX_VITIS ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vitis/2022.2

VPP ?= ${XILINX_VITIS}/bin/v++


#VITIS_PLATFORM = xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_DIR = /media/danieleb/DATA/2022.2/tools/Xilinx/Vitis/2022.2/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(VITIS_PLATFORM_DIR)/xilinx_zcu102_base_202220_1.xpfm
KERNEL_IMAGE = $(COMMON_IMAGE_ZYNQMP)/Image
ROOTFS = $(COMMON_IMAGE_ZYNQMP)/rootfs.ext4

#
# variables
#


#
# primary build targets
#

package:
	-@echo Running Package Step for the System Project.
	-@echo $(VPP) --package --config package.cfg  ../../zcu102_vadd_system_hw_link/Hardware/binary_container_1.xclbin  --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE) -o binary_container_1.xclbin
	$(VPP) --package --config package.cfg  ../../zcu102_vadd_system_hw_link/Hardware/binary_container_1.xclbin  --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE) -o binary_container_1.xclbin

clean:
	$(RMDIR) ./package.build
	${RMDIR} package

.PHONY: all package clean

all: package
