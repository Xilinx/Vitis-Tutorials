# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

BOARD=vck190

# compiler tools
VPP ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

#VITIS_PLATFORM = xilinx_$(BOARD)_base_202220_1
#VITIS_PLATFORM_DIR = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(XILINX_VITIS)/base_platforms/$(VITIS_PLATFORM)/$(VITIS_PLATFORM).xpfm


TARGET = hw
VPP_OPTS = --target $(TARGET)

#
# Hw kernel files
#


BUILD_SUBDIRS += build
HW_KERNEL_OBJS += build/HLS_accel.xo
SRC_KERNEL_OBJS += build/HLS_accel.xo
BUILD_SUBDIRS += build
HW_KERNEL_OBJS += build/mm2s.xo
SRC_KERNEL_OBJS += build/mm2s.xo
BUILD_SUBDIRS += build
HW_KERNEL_OBJS += build/s2mm.xo
SRC_KERNEL_OBJS += build/s2mm.xo

ALL_MESSAGE_FILES = $(subst .xo,.o,.mdb,$(SRC_KERNEL_OBJS))

#
# primary build targets
#

.PHONY: all clean
all: $(HW_KERNEL_OBJS)


clean:
	-$(RM) $(SRC_KERNEL_OBJS) $(ALL_MESSAGE_FILES) *.log *summary*
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil

.PHONY: incremental
incremental: all


nothing:

#
# Hw Kernels compile;
#

build/HLS_accel.xo: ../src/mmult_accel.cpp ../src/mmult.h HLS_accel-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config HLS_accel-compile.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" "$<"

build/mm2s.xo: ../src/mm2s.cpp mm2s-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config mm2s-compile.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" "$<"

build/s2mm.xo: ../src/s2mm.cpp s2mm-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config s2mm-compile.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" "$<"
