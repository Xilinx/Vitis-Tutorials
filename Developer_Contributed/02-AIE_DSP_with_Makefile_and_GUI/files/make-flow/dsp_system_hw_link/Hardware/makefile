# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

BOARD=vck190


VPP_LINKER ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

VPP_OPTS = --target hw

#
# Hw kernel files
#

BINARY_CONTAINERS += binary_container_1.xsa

BUILD_SUBDIRS += binary_container_1.build
BINARY_CONTAINER_1_OBJS += ../../dsp_PL/Hardware/build/mm2s.xo
BINARY_CONTAINER_1_OBJS += ../../dsp_PL/Hardware/build/s2mm.xo
BINARY_CONTAINER_1_OBJS += ../../dsp_AIE/Hardware/libadf.a

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
	-$(RMDIR) .Xil .ipcache *.mdb *.log .crash*

ifeq ($(MAKECMDGOALS),incremental)
.PHONY: binary_container_1.xsa
.PHONY: ../../dsplib_fir_PL/Hardware/build/mm2s.xo
.PHONY: ../../dsplib_fir_PL/Hardware/build/s2mm.xo
endif

.PHONY: incremental
incremental: all


nothing:

#
# binary container: binary_container_1.xsa
#

binary_container_1.xsa: $(BINARY_CONTAINER_1_OBJS) binary_container_1-link.cfg ../system.cfg
	-@echo $(VPP_LINKER) $(VPP_OPTS) --link --config ../system.cfg  --platform $(VITIS_PLATFORM_XPFM) --config binary_container_1-link.cfg -o"$@" $(BINARY_CONTAINER_1_OBJS) > binary_container_1.xsa.sh
	$(VPP_LINKER) $(VPP_OPTS) --link --config ../system.cfg  --platform $(VITIS_PLATFORM_XPFM) --config binary_container_1-link.cfg -o"$@" $(BINARY_CONTAINER_1_OBJS)
