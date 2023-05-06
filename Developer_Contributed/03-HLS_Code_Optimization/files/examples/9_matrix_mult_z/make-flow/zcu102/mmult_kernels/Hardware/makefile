# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_DIR	  := $(shell readlink -f $(RELATIVE_PROJECT_DIR))
PROJECT_PL_DIR	  := $(PROJECT_DIR)
PROJECT_PS_DIR     = $(PROJECT_PL_DIR)/../mmult
PROJECT_SYSTEM_DIR = $(PROJECT_PL_DIR)/../mmult_system
PROJECT_HWLINK_DIR = $(PROJECT_PL_DIR)/../mmult_system_hw_link

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


BOARD=zcu102

# compiler tools
VPP ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

TARGET = hw
VPP_OPTS = --target $(TARGET)

#
# Hw kernel files
#


BUILD_SUBDIRS    = $(PROJECT_PL_DIR)/Hardware/build
HW_KERNEL_OBJS  += $(PROJECT_PL_DIR)/Hardware/build/HLS_accel.xo
SRC_KERNEL_OBJS += $(PROJECT_PL_DIR)/Hardware/build/HLS_accel.xo
HW_KERNEL_OBJS  += $(PROJECT_PL_DIR)/Hardware/build/mm2s.xo
SRC_KERNEL_OBJS += $(PROJECT_PL_DIR)/Hardware/build/mm2s.xo
HW_KERNEL_OBJS  += $(PROJECT_PL_DIR)/Hardware/build/s2mm.xo
SRC_KERNEL_OBJS += $(PROJECT_PL_DIR)/Hardware/build/s2mm.xo

ALL_MESSAGE_FILES = $(subst .xo,.o,.mdb,$(SRC_KERNEL_OBJS))

#
# primary build targets
#

.PHONY: all clean
all: show_dir $(HW_KERNEL_OBJS)


clean:
	-$(RM) $(SRC_KERNEL_OBJS) $(ALL_MESSAGE_FILES)
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil guidance.* *.log *~

.PHONY: incremental
incremental: all


nothing:

#
# Hw Kernels compile;
#

$(PROJECT_PL_DIR)/Hardware/build/HLS_accel.xo: $(PROJECT_PL_DIR)/src/mmult_accel.cpp $(PROJECT_PL_DIR)/src/mmult.h $(PROJECT_PL_DIR)/Hardware/HLS_accel-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config $(PROJECT_PL_DIR)/Hardware/HLS_accel-compile.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" "$<"
#--platform $(VITIS_PLATFORM_PATH)

$(PROJECT_PL_DIR)/Hardware/build/mm2s.xo: $(PROJECT_PL_DIR)/src/mm2s.cpp $(PROJECT_PL_DIR)/Hardware/mm2s-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config $(PROJECT_PL_DIR)/Hardware/mm2s-compile.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" "$<"

$(PROJECT_PL_DIR)/Hardware/build/s2mm.xo: $(PROJECT_PL_DIR)/src/s2mm.cpp $(PROJECT_PL_DIR)/Hardware/s2mm-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config $(PROJECT_PL_DIR)/Hardware/s2mm-compile.cfg --platform $(VITIS_PLATFORM_XPFM) -o"$@" "$<"
