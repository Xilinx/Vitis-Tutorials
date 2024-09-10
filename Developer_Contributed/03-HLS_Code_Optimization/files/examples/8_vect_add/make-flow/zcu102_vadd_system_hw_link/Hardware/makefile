# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# compiler tools
#XILINX_VITIS ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vitis/2022.2
#XILINX_VIVADO ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vivado/2022.2
#XILINX_HLS ?= $(XILINX_VITIS)/Vivado_HLS

VPP_LINKER ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

#VITIS_PLATFORM = xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_DIR = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1/xilinx_zcu102_base_202220_1.xpfm

VPP_OPTS = --target hw

#
# Hw kernel files
#

BINARY_CONTAINERS += binary_container_1.xclbin

BUILD_SUBDIRS += binary_container_1.build
BINARY_CONTAINER_1_OBJS += ../../zcu102_vadd_kernels/Hardware/build/krnl_vadd.xo

ALL_MESSAGE_FILES = $(subst .xclbin,.mdb,$(BINARY_CONTAINERS))

#
# primary build targets
#

.PHONY: all clean
all: $(BINARY_CONTAINERS)


clean:
	-$(RM) $(BINARY_CONTAINERS) $(ALL_MESSAGE_FILES)
	-$(RM) *.xclbin.sh *.xclbin.info *.xclbin.link_summary* *.log *~
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil

.PHONY: incremental
incremental: all


nothing:

#
# binary container: binary_container_1.xclbin
#

binary_container_1.xclbin: $(BINARY_CONTAINER_1_OBJS) binary_container_1-link.cfg
	-@echo $(VPP_LINKER) $(VPP_OPTS) --link --config binary_container_1-link.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" $(BINARY_CONTAINER_1_OBJS) > binary_container_1.xclbin.sh
	$(VPP_LINKER) $(VPP_OPTS) --link --config binary_container_1-link.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" $(BINARY_CONTAINER_1_OBJS)
