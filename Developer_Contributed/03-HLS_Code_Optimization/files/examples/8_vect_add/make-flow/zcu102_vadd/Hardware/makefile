# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# compiler tools
#XILINX_VITIS ?= /media/danieleb/DATA/2022.2/tools/Xilinx/Vitis/2022.2

# compiler tools
HOST_CXX ?= $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
RM = rm -f
RMDIR = rm -rf


#VITIS_PLATFORM = xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_DIR = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1
#VITIS_PLATFORM_PATH = $(XILINX_VITIS)/base_platforms/xilinx_zcu102_base_202220_1/xilinx_zcu102_base_202220_1.xpfm

# host compiler global settings
CXXFLAGS += -std=c++17 -DVITIS_PLATFORM=$(ZYNQMP_VITIS_PLATFORM) -D__USE_XOPEN2K8 -I$(XILINX_HLS)/include/ -I$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux/usr/include/xrt/ -O2 -g -Wall -c -fmessage-length=0 --sysroot=$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux

LDFLAGS += -lxilinxopencl -lxrt_core -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux/usr/lib/ --sysroot=$(COMMON_IMAGE_ZYNQMP)/sysroots/cortexa72-cortexa53-amd-linux

#
# host files
#

HOST_OBJECTS += src/cl_host_vadd.o
HOST_OBJECTS += src/xrt_host_vadd.o

HOST_EXE = zcu102_vadd

BUILD_SUBDIRS += src/

#
# primary build targets
#

.PHONY: all clean
all:  $(HOST_EXE)

clean:
	$(RM) -rf $(HOST_EXE) $(HOST_OBJECTS) ./src *~

.PHONY: incremental
incremental: all


nothing:

#
# host rules
#

src/cl_host_vadd.o: ../src/cl_host_vadd.cpp ../src/cl_vadd.h
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

src/xrt_host_vadd.o: ../src/xrt_host_vadd.cpp ../src/vadd_include.h
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
