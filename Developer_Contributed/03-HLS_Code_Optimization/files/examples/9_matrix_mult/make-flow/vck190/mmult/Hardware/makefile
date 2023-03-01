# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


BOARD=vck190

# compiler tools
HOST_CXX ?= $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
RM = rm -f
RMDIR = rm -rf

#VITIS_PLATFORM = xilinx_$(BOARD)_base_202220_1
#VITIS_PLATFORM_DIR = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(XILINX_VITIS)/base_platforms/$(VITIS_PLATFORM)/$(VITIS_PLATFORM).xpfm

# host compiler global settings
CXXFLAGS = -std=c++1y -DVITIS_PLATFORM=$(VERSAL_VITIS_PLATFORM) -D__USE_XOPEN2K8 -I$(XILINX_HLS)/include -I$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux/usr/include/xrt -O2 -g -Wall -c -fmessage-length=0 --sysroot=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux

LDFLAGS = -lxilinxopencl -lxrt_core -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux/usr/lib --sysroot=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux


#
# host files
#

HOST_OBJECTS += src/xrt_mmult_test.o

HOST_EXE = mmult

BUILD_SUBDIRS += src/

#
# primary build targets
#

.PHONY: all clean
all:  $(HOST_EXE)

clean:
	-$(RM) $(HOST_EXE) $(HOST_OBJECTS)

.PHONY: incremental
incremental: all


nothing:

#
# host rules
#

src/xrt_mmult_test.o: ../src/xrt_mmult_test.cpp ../src/mmult.h
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
