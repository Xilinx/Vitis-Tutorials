#
# Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

.PHONY: help

help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make build TARGET=sw_emu|hw_emu|hw "
	@echo  "      Command to build the application design "
	@echo  ""
	@echo  "  make run TARGET=sw_emu|hw_emu|hw "
	@echo  "      command to run the application design "
	@echo  ""
	@echo  "  make clean "
	@echo  "      Command to remove the generated files."
	@echo  ""

## platform selection
## Several supported platforms are liste here. You may choose the one you want and comment out the others
## Or you can add other platfom and set PLATFORM variable to it

PLATFORM := xilinx_u50_gen3x16_xdma_201920_3


## TARGET can be set as:
## sw_emu: software emulation
## hw_emu: hardware Emulation
## hw: hardware run
TARGET := hw_emu

## Below are the names for host executable and xclbin.
## Please keep it unchanged.
HOST_EXE := test.exe
XO_NAME := cholesky_kernel_$(TARGET)
XCLBIN := cholesky_kernel_$(TARGET).xclbin



## BUILD_DIR are used for the generated files, including .exe, .xclbin, and the reports/logs. BUILD_DIR selection should be matched to SRC_REPO for better structure.
PROJECT_DIR := .


SRC_REPO := $(PROJECT_DIR)/src
BUILD_DIR := $(PROJECT_DIR)/build


## Host Application files repository

HOST_SRC_CPP := $(SRC_REPO)/test.cpp
HOST_SRC_CPP += $(SRC_REPO)/xcl2.cpp
HOST_SRC_H := $(SRC_REPO)/matrixUtility.hpp
HOST_SRC_H += $(SRC_REPO)/xcl2.hpp


## Kernel Source Files repository

KERNEL_SRC_CPP := $(SRC_REPO)/cholesky_kernel.cpp
KERNEL_SRC_H := $(SRC_REPO)/cholesky_kernel.hpp
KERNEL_SRC_H_DIR := $(SRC_REPO)


# Host Compiler Global Settings and Include Libraries

#CXXFLAGS :=
CXXFLAGS += -D__USE_XOPEN2K8
CXXFLAGS += -I$(XILINX_XRT)/include/
CXXFLAGS += -I$(XILINX_VIVADO)/include/
CXXFLAGS += -I$(SRC_REPO)
CXXFLAGS += -O2 -g -Wall -fmessage-length=0 -std=c++0x


CXXLDFLAGS := -L$(XILINX_XRT)/lib/
#CXXLDFLAGS += -lxilinxopencl -lpthread -lrt -lstdc++
CXXLDFLAGS += -lOpenCL -pthread -lrt -lstdc++

## Kernel Compiler and Linker Flags

VPPFLAGS := -t $(TARGET)
VPPFLAGS += --platform $(PLATFORM)
VPPFLAGS += --config common.cfg



## Host Executable File Generation

$(BUILD_DIR)/$(HOST_EXE): $(HOST_SRC_CPP) $(HOST_SRC_H)
	mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(CXXLDFLAGS) -o $@ $(HOST_SRC_CPP)


## Kernel XO and Xclbin File Generation

$(BUILD_DIR)/$(XO_NAME).xo: $(KERNEL_SRC_CPP) $(KERNEL_SRC_H)
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -c $(KERNEL_SRC_CPP) $(KERNEL_SRC_H) -o $@

$(BUILD_DIR)/$(XCLBIN): $(BUILD_DIR)/$(XO_NAME).xo
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -l -o $@ $(BUILD_DIR)/$(XO_NAME).xo



## Emulation Files Generation

EMCONFIG_FILE = emconfig.json

$(BUILD_DIR)/$(EMCONFIG_FILE):
	 emconfigutil --nd 1  --platform $(PLATFORM) --od $(BUILD_DIR)


#
# primary build targets
#

.PHONY: all clean

## build the design without running host application

#build: $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(XCLBIN) $(BUILD_DIR)/$(EMCONFIG_FILE)
build: host xclbin emconfig
#pre_build: host emconfig
host: $(BUILD_DIR)/$(HOST_EXE)
xclbin: $(BUILD_DIR)/$(XCLBIN)
emconfig: $(BUILD_DIR)/$(EMCONFIG_FILE)


## build the design and then run host application
## run with 512x512 matrix size for hardware and 64x64 for emulation

run: build
	cp xrt.ini $(BUILD_DIR);
ifeq ($(TARGET), hw)
	cd $(BUILD_DIR) && unset XCL_EMULATION_MODE; ./test.exe  -xclbin $(XCLBIN) -runs 1 -M 512 -N 512 -seed 12;
else
	cd $(BUILD_DIR) && export XCL_EMULATION_MODE=$(TARGET); ./test.exe  -xclbin $(XCLBIN) -runs 1 -M 64 -N 64 -seed 12;
endif


## Clean generated files

clean:
	rm -rf $(BUILD_DIR)/$(XCLBIN) $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(EMCONFIG_FILE) $(BUILD_DIR)/*.xo $(BUILD_DIR)/*.ltx $(BUILD_DIR)/*_*.log $(BUILD_DIR)/_x* $(BUILD_DIR)/*.info $(BUILD_DIR)/cholesky_kernel_* $(BUILD_DIR)/link $(BUILD_DIR)/reports/cholesky_kernel_*
