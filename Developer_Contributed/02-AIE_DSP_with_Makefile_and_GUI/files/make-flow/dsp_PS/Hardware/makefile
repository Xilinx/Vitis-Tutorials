# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# compiler tools
HOST_CXX ?= $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
RM = rm -f
RMDIR = rm -rf

# host compiler global settings
CXXFLAGS += -std=c++14 -DVITIS_PLATFORM=$(VERSAL_VITIS_PLATFORM) -D__USE_XOPEN2K8 \
	-I$(XILINX_VITIS)/aietools/include -I$(XILINX_HLS)/include -I$(XILINX_VITIS)/include \
	-I$(DSPLIB_ROOT)/L1/src/aie -I$(DSPLIB_ROOT)/L1/include/aie -I$(DSPLIB_ROOT)/L2/include/aie \
	-I$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux/usr/include/xrt/ \
	-O2 -g -Wall -c -fmessage-length=0 \
	--sysroot=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux \
	-I../../dsp_AIE/src -I../../dsp_AIE/src/graphs -I../dsp_PS/src \

LDFLAGS = -lxilinxopencl -lxaiengine -ladf_api_xrt -lxrt_core -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ \
	-L$(XILINX_VITIS)/aietools/lib/aarch64.o \
	-L$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux/usr/lib \
	--sysroot=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux


#
# host files
#

HOST_OBJECTS += src/xrt_host.o

#name of the executable application running on ARM CPU
HOST_EXE = dsp_fir.exe

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

src/xrt_host.o: ../src/xrt_host.cpp ../src/input.h ../src/golden.h ../../dsp_AIE/src/fir_graph.cpp
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
