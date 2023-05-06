# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

#relative directory
RELATIVE_PROJECT_DIR := ../

#absolute directory
PROJECT_PS_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

PROJECT_AIE_DIR= $(PROJECT_PS_DIR)/../dsp_AIE

show_dir:
		-@echo from dsp_PS, RELATIVE PROJECT DIR is:  $(RELATIVE_PROJECT_DIR)
		-@echo from dsp_PS, CURRENT  PROJECT DIR is:  $(PROJECT_PS_DIR)
		-@echo from dsp_PS, AIE      PROJECT DIR is:  $(PROJECT_AIE_DIR)


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
	-I$(PROJECT_AIE_DIR)/src -I$(PROJECT_AIE_DIR)/src/graphs -I$(PROJECT_PS_DIR)/src \
#	-I../../dsp_AIE/src -I../../dsp_AIE/src/graphs -I../dsp_PS/src \

LDFLAGS = -lxilinxopencl -lxaiengine -ladf_api_xrt -lxrt_core -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ \
	-L$(XILINX_VITIS)/aietools/lib/aarch64.o \
	-L$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux/usr/lib \
	--sysroot=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux


#
# host files
#

HOST_OBJECTS += $(PROJECT_PS_DIR)/src/xrt_host.o

#name of the executable application running on ARM CPU
HOST_EXE = $(PROJECT_PS_DIR)/Hardware/dsp_fir.exe

#BUILD_SUBDIRS += src #DB: never used ?

#
# primary build targets
#

.PHONY: all clean
all:  	show_dir	$(HOST_EXE)

clean:
	-$(RM) $(HOST_EXE) $(HOST_OBJECTS)

.PHONY: incremental
incremental: all


nothing:

#
# host rules
#

$(PROJECT_AIE_DIR)/src/xrt_host.o: $(PROJECT_AIE_DIR)/src/xrt_host.cpp $(PROJECT_AIE_DIR)/src/input.h $(PROJECT_AIE_DIR)/src/golden.h $(PROJECT_AIE_DIR)/dsp_AIE/src/fir_graph.cpp
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
