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
	@echo "       Command runs make pre_build and and make kernels"
	@echo ""
	@echo "  make pre_build "
	@echo "       Command to create the HLS *.cpp files from kernel/spec.json file"
	@echo ""
	@echo "  make pl_kernels TARGET=<hw_emu|hw>"
	@echo "       Command to run v++ compiler and create the *.xo HLS kernels"
	@echo "       Default TARGET=hw_emu"
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
PL_KERNELS_DIR := $(PROJECT_DIR)/pl_kernels
AIE_DIR := $(PROJECT_DIR)/aie

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))
XFLIB_DIR_REL_PATH :=../../Module_03_pl_kernels/Vitis_Libraries/utils/
XFLIB_DIR := $(shell readlink -f $(XFLIB_DIR_REL_PATH))

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
# NUMBER OF COMPUTE UNITS INSTANCES 
# x1   = 1   AIE TILE
# x10  = 10  AIE TILES
# X100 = 100 AIE TILES
# =========================================================
INST		= x1

# =========================================================
# AIE SOURCES 
# =========================================================
PACKET_IDS_C_H := $(AIE_DIR)/build/Work_x4_$(INST)/temp/packet_ids_c.h

# =========================================================
# V++ COMPILER FLAGS
# =========================================================
VPP_FLAGS += -t $(TARGET) --platform $(PLATFORM) --save-temps --optimize 2
VPP_FLAGS += --hls.jobs 8
VPP_FLAGS += -I$(XFLIB_DIR)/L1/include
VPP_FLAGS += -I$(XFLIB_DIR)/L1/include/hw
VPP_FLAGS += -I$(CUR_DIR)/kernel

# =========================================================
# OUTPUT PRODUCTS
# =========================================================
BUILD_DIR := $(PL_KERNELS_DIR)/build
TEMP_DIR := $(BUILD_DIR)/_x_temp.$(TARGET).$(PLATFORM)
TEMP_REPORT_DIR := $(BUILD_DIR)/reports/_x.$(TARGET).$(PLATFORM)

# ################ TARGET: make aie ################
aie: 
	$(MAKE) aie -C $(AIE_DIR)

# ################ TARGET: make all ################
all: pre_build kernels

# ################ TARGET: make pre_build ################
pre_build: _krnlgen

_krnlgen: $(XFLIB_DIR) $(CUR_DIR)/ksrc.mk $(CUR_DIR)/kernel/spec.json
	make -f $(CUR_DIR)/ksrc.mk GENKERNEL=$(XFLIB_DIR)/L2/scripts/generate_kernels SPEC=$(CUR_DIR)/kernel/spec.json TOOLDIR=$(CUR_DIR)/_krnlgen

# ################ TARGET: make pl_kernels ################
pl_kernels: pre_build aie $(TEMP_DIR)/packet_sender.xo $(TEMP_DIR)/packet_receiver.xo $(TEMP_DIR)/m2s_x2.xo $(TEMP_DIR)/s2m_x4.xo

$(TEMP_DIR)/packet_sender.xo: $(CUR_DIR)/kernel/packet_sender.cpp $(CUR_DIR)/kernel/packet_sender.hpp $(PACKET_IDS_C_H) 
	echo "Compiling Kernel: packet_sender"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) -k packet_sender -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $<

$(TEMP_DIR)/m2s_x2.xo: $(CUR_DIR)/kernel/m2s_x2.cpp
	echo "Compiling Kernel: m2s_x2"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) -k m2s_x2 -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $<

pr: $(TEMP_DIR)/packet_receiver.xo
$(TEMP_DIR)/packet_receiver.xo: $(CUR_DIR)/kernel/packet_receiver.cpp $(CUR_DIR)/kernel/packet_receiver.hpp $(PACKET_IDS_C_H)
	echo "Compiling Kernel: packet_receiver"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) -k packet_receiver -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $<

$(TEMP_DIR)/s2m_x4.xo: $(CUR_DIR)/kernel/s2m_x4.cpp
	echo "Compiling Kernel: s2m_x4"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) -k s2m_x4 -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $<

# ################ TARGET: make clean ################
clean:
	$(MAKE) clean -C kernel
	rm -rf $(BUILD_DIR)
