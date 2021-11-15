# Copyright 2021 Xilinx Inc.
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

help::
	@echo "Makefile Usage:"
	@echo "  make all "
	@echo "       Command runs make host"
	@echo ""
	@echo "  make host "
	@echo "       Command to compile the host application"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

# =========================================================
# PROJECT DIRECTORIES
# =========================================================
#relative project directory
RELATIVE_PROJECT_DIR := ../

#absolute project directory
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

AIE_DIR := $(PROJECT_DIR)/aie
HOST_SW_DIR := $(PROJECT_DIR)/host_sw

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))

XFLIB_DIR_REL_PATH :=../../Module_03_pl_kernels/Vitis_Libraries/utils/
XFLIB_DIR := $(shell readlink -f $(XFLIB_DIR_REL_PATH))

# =========================================================
# NUMBER OF COMPUTE UNITS INSTANCES 
# x1   = 1   AIE TILE
# x10  = 10  AIE TILES
# X100 = 100 AIE TILES
# =========================================================
INST := x1

# =========================================================
# PLATFORM SELECTION
# =========================================================
PLATFORM :=xilinx_vck190_base_202110_1

# =========================================================
# TARGET SELECTION
# TARGET=hw_emu (default)
# TARGET=hw
# =========================================================
TARGET :=hw_emu

# =========================================================
# GCC COMPILER FLAGS
# =========================================================
CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
SYSROOT := $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal-common-v2021.1/sysroots/aarch64-xilinx-linux

GCC_FLAGS := -Wall -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -Wno-int-to-pointer-cast
GCC_FLAGS += --sysroot=$(SYSROOT)

GCC_INCLUDES := -I$(SYSROOT)/usr/include/xrt
GCC_INCLUDES +=	-I$(SYSROOT)/usr/include
GCC_INCLUDES +=	-I$(HOST_SW_DIR) -I$(AIE_DIR) -I$(AIE_DIR)/src
GCC_INCLUDES +=	-I$(XILINX_VITIS)/aietools/include
GCC_INCLUDES +=	-I$(XILINX_VITIS)/include

GCC_LIB := -lxaiengine -ladf_api_xrt -lxrt_core -lxrt_coreutil
GCC_LIB += -L$(SYSROOT)/usr/lib
GCC_LIB += --sysroot=$(SYSROOT)
GCC_LIB += -L$(XILINX_VITIS)/aietools/lib/aarch64.o

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := $(HOST_SW_DIR)/build

# ################ TARGET: make aie ################
aie: 
	$(MAKE) aie -C $(AIE_DIR)

# ################ TARGET: make all ################
all: host

# ################ TARGET: make host ################
.PHONY: host
host: aie $(BUILD_DIR) $(BUILD_DIR)/ps_app.exe $(BUILD_DIR)/ps_app_animate.exe

$(BUILD_DIR)/ps_app.exe: $(BUILD_DIR)/aie_control_xrt.o $(BUILD_DIR)/ps_app.o $(BUILD_DIR)/log.o $(BUILD_DIR)/nbody.o   
	$(CXX) $^ $(GCC_LIB) -std=c++14 -o $(BUILD_DIR)/ps_app.exe
	@echo "COMPLETE: Host application ps_app.exe created."

$(BUILD_DIR)/ps_app_animate.exe: $(BUILD_DIR)/aie_control_xrt.o $(BUILD_DIR)/ps_app_animate.o $(BUILD_DIR)/log.o $(BUILD_DIR)/nbody.o   
	$(CXX) $^ $(GCC_LIB) -std=c++14 -o $(BUILD_DIR)/ps_app_animate.exe
	@echo "COMPLETE: Host application ps_app_animate.exe created."

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR);

$(BUILD_DIR)/aie_control_xrt.o: $(AIE_DIR)/build/Work_x4_$(INST)/ps/c_rts/aie_control_xrt.cpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o $(BUILD_DIR)/aie_control_xrt.o $(AIE_DIR)/build/Work_x4_$(INST)/ps/c_rts/aie_control_xrt.cpp

$(BUILD_DIR)/ps_app.o: $(HOST_SW_DIR)/host/main_xrt.cpp $(HOST_SW_DIR)/host/main_xrt.hpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o $(BUILD_DIR)/ps_app.o $(HOST_SW_DIR)/host/main_xrt.cpp

$(BUILD_DIR)/ps_app_animate.o: $(HOST_SW_DIR)/host/main_animate.cpp $(HOST_SW_DIR)/host/main_xrt.hpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o $(BUILD_DIR)/ps_app_animate.o $(HOST_SW_DIR)/host/main_animate.cpp

$(BUILD_DIR)/log.o: $(HOST_SW_DIR)/host/log.cpp $(HOST_SW_DIR)/host/log.hpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o $(BUILD_DIR)/log.o $(HOST_SW_DIR)/host/log.cpp

$(BUILD_DIR)/nbody.o: $(HOST_SW_DIR)/host/nbody.cpp $(HOST_SW_DIR)/host/nbody.hpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o $(BUILD_DIR)/nbody.o $(HOST_SW_DIR)/host/nbody.cpp


# ################ TARGET: make clean ################
clean:
	rm -rf $(BUILD_DIR)
