# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_DIR	   := $(shell readlink -f $(RELATIVE_PROJECT_DIR))
PROJECT_HWLINK_DIR := $(PROJECT_DIR)
PROJECT_PS_DIR     := $(PROJECT_HWLINK_DIR)/../mmult
PROJECT_PL_DIR     := $(PROJECT_HWLINK_DIR)/../mmult_kernels
PROJECT_SYSTEM_DIR := $(PROJECT_HWLINK_DIR)/../mmult_system

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

VPP_LINKER ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

VPP_OPTS = --target hw

#
# Hw kernel files
#

BINARY_CONTAINERS += binary_container_1.xsa

BUILD_SUBDIRS += binary_container_1.build
BINARY_CONTAINER_1_OBJS += $(PROJECT_PL_DIR)/Hardware/build/HLS_accel.xo
BINARY_CONTAINER_1_OBJS += $(PROJECT_PL_DIR)/Hardware/build/mm2s.xo
BINARY_CONTAINER_1_OBJS += $(PROJECT_PL_DIR)/Hardware/build/s2mm.xo

ALL_MESSAGE_FILES = $(subst .xsa,.mdb,$(BINARY_CONTAINERS))

#
# primary build targets
#

.PHONY: all clean show_dir
all: 	show_dir	$(BINARY_CONTAINERS)


clean:
	-$(RM) $(BINARY_CONTAINERS) $(ALL_MESSAGE_FILES)
	-$(RM) *.xsa.sh *.xsa.info *.xsa.link_summary*
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil v++*.log *.log


.PHONY: incremental
incremental: all


#nothing:


binary_container_1.xsa: $(BINARY_CONTAINER_1_OBJS) $(PROJECT_HWLINK_DIR)/Hardware/binary_container_1-link.cfg $(PROJECT_HWLINK_DIR)/system.cfg
	-@echo $(VPP_LINKER) $(VPP_OPTS) --link --config $(PROJECT_HWLINK_DIR)/system.cfg  --platform $(VITIS_PLATFORM_XPFM)  --config $(PROJECT_HWLINK_DIR)/Hardware/binary_container_1-link.cfg -o"$@" $(BINARY_CONTAINER_1_OBJS) > binary_container_1.xsa.sh
	$(VPP_LINKER) $(VPP_OPTS) --link --config $(PROJECT_HWLINK_DIR)/system.cfg --platform $(VITIS_PLATFORM_XPFM) --config $(PROJECT_HWLINK_DIR)/Hardware/binary_container_1-link.cfg -o"$@" $(BINARY_CONTAINER_1_OBJS)
