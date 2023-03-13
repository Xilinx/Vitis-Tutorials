# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

BOARD=vck190

# compiler tools
#XILINX_VITIS ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vitis/2022.2
#XILINX_VIVADO ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vivado/2022.2
#XILINX_HLS ?= $(XILINX_VITIS)/Vivado_HLS

VPP_LINKER ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf


#VITIS_PLATFORM = xilinx_$(BOARD)_base_202220_1
#VITIS_PLATFORM_DIR = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(XILINX_VITIS)/base_platforms/$(VITIS_PLATFORM)/$(VITIS_PLATFORM).xpfm

VPP_OPTS = --target hw

#
# Hw kernel files
#

BINARY_CONTAINERS += binary_container_1.xsa

BUILD_SUBDIRS += binary_container_1.build
BINARY_CONTAINER_1_OBJS += ../../mmult_kernels/Hardware/build/HLS_accel.xo
BINARY_CONTAINER_1_OBJS += ../../mmult_kernels/Hardware/build/mm2s.xo
BINARY_CONTAINER_1_OBJS += ../../mmult_kernels/Hardware/build/s2mm.xo

ALL_MESSAGE_FILES = $(subst .xsa,.mdb,$(BINARY_CONTAINERS))

#
# primary build targets
#

.PHONY: all clean
all: $(BINARY_CONTAINERS)


clean:
	-$(RM) $(BINARY_CONTAINERS) $(ALL_MESSAGE_FILES)
	-$(RM) *.xsa.sh *.xsa.info *.xsa.link_summary*
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil v++*.log *.log

ifeq ($(MAKECMDGOALS),incremental)
.PHONY: binary_container_1.xsa
.PHONY: ../../mmult_kernels/Hardware/build/HLS_accel.xo
.PHONY: ../../mmult_kernels/Hardware/build/mm2s.xo
.PHONY: ../../mmult_kernels/Hardware/build/s2mm.xo
endif

.PHONY: incremental
incremental: all


nothing:

#
# binary container: binary_container_1.xsa
#


binary_container_1.xsa: $(BINARY_CONTAINER_1_OBJS) binary_container_1-link.cfg ../system.cfg
	-@echo $(VPP_LINKER) $(VPP_OPTS) --link --config ../system.cfg  --platform $(VITIS_PLATFORM_XPFM)  --config binary_container_1-link.cfg -o"$@" $(BINARY_CONTAINER_1_OBJS) > binary_container_1.xsa.sh
	$(VPP_LINKER) $(VPP_OPTS) --link --config ../system.cfg --platform $(VITIS_PLATFORM_XPFM) --config binary_container_1-link.cfg -o"$@" $(BINARY_CONTAINER_1_OBJS)
