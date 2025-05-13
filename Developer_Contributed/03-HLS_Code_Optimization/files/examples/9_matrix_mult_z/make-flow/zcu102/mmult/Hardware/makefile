# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_DIR	   := $(shell readlink -f $(RELATIVE_PROJECT_DIR))
PROJECT_PS_DIR	   := $(PROJECT_DIR)
PROJECT_PL_DIR     := $(PROJECT_PS_DIR)/../mmult_kernels
PROJECT_HWLINK_DIR := $(PROJECT_PS_DIR)/../mmult_system_hw_link
PROJECT_SYSTEM_DIR := $(PROJECT_PS_DIR)/../mmult_system

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
HOST_CXX ?= $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
RM = rm -f
RMDIR = rm -rf

# compiler flags
CXXFLAGS = -std=c++17 -DVITIS_PLATFORM=$(ZYNQMP_VITIS_PLATFORM) -D__USE_XOPEN2K8 -I$(XILINX_HLS)/include -I$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux/usr/include/xrt -O2 -g -Wall -c -fmessage-length=0 --sysroot=$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux

LDFLAGS = -lxilinxopencl -lxrt_core -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux/usr/lib --sysroot=$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux


#
# host files
#

HOST_OBJECTS += $(PROJECT_PS_DIR)/src/xrt_mmult_test.o

HOST_EXE = $(PROJECT_PS_DIR)/Hardware/mmult


#
# primary build targets
#

.PHONY: all clean
all:  show_dir $(HOST_EXE)

clean:
	-$(RM) $(HOST_EXE) $(HOST_OBJECTS) *.log guidance.* $(PROJECT_PS_DIR)/src/*.o *~
	-$(RMDIR) $(PROJECT_PS_DIR)/Hardware/src

.PHONY: incremental
incremental: all


nothing:

#
# host rules
#

$(PROJECT_PS_DIR)/src/xrt_mmult_test.o: $(PROJECT_PS_DIR)/src/xrt_mmult_test.cpp $(PROJECT_PS_DIR)/src/mmult.h
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
