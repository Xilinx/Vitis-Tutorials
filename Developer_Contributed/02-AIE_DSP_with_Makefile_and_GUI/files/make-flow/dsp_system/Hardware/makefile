# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#


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
ROOTFS = $(COMMON_IMAGE_VERSAL)/rootfs.ext4

AIE_ARCHIVE = ../../dsp_AIE/Hardware/libadf.a

#
# primary build targets
#
PACKAGE_FLAGS= --platform $(VITIS_PLATFORM_XPFM) --package.rootfs $(ROOTFS) --package.kernel_image $(KERNEL_IMAGE)

package:
	-@echo Running Package Step for the System Project.
	-@echo $(VPP) --package --config package.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin
	$(VPP) --package --config package.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin
	-@echo Running Package Step for the System Project with AIE configured to not stop for Debug.
	-@echo $(VPP) --package --config package_aie_debug.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin
	$(VPP) --package --config package_aie_debug.cfg   $(AIE_ARCHIVE) ../../dsp_system_hw_link/Hardware/binary_container_1.xsa $(PACKAGE_FLAGS) -o binary_container_1.xclbin

clean:
	$(RMDIR) ./package_aie_debug.build ./package_aie_debug .Xil
	$(RMDIR) ./package ./package.build
	${RM} *.xclbin* *.log

.PHONY: all package clean

all: package
