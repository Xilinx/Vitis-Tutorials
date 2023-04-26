#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

help::
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<hw_emu|hw> "
	@echo "       Command runs make vitis_lib, make pre_build and make pl_kernels"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make vitis_lib "
	@echo "       Command to download the Vitis Utilty Library from Github"
	@echo ""
	@echo "  make pre_build "
	@echo "       Command to create the HLS *.cpp files from kernel/spec.json file"
	@echo ""
	@echo "  make pl_kernels TARGET=<hw_emu|hw>"
	@echo "       Command runs make vitis_lib and make pre_build and runs v++ compiler and create the *.xo HLS kernels"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""
# =========================================================
# NUMBER OF COMPUTE UNITS INSTANCES 
# x1   = 1   AIE TILE
# x10  = 10  AIE TILES
# X100 = 100 AIE TILES
# =========================================================
INST		= x1

# =========================================================
# PROJECT DIRECTORIES
# =========================================================
#relative project directory
RELATIVE_PROJECT_DIR := ../../

#absolute project directory
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#Design Directory 
DESIGN_DIR := $(PROJECT_DIR)/$(INST)_design

PL_KERNELS_DIR := $(DESIGN_DIR)/pl_kernels
AIE_DIR := $(DESIGN_DIR)/aie

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))

#XFLIB_DIR_REL_PATH :=./Vitis_Libraries/utils
XFLIB_DIR_REL_PATH :=$(DSPLIB_VITIS)/utils
XFLIB_DIR := $(shell readlink -f $(XFLIB_DIR_REL_PATH))

# =========================================================
# TARGET SELECTION
# TARGET=hw (default)
# TARGET=hw_emu
# =========================================================
TARGET ?=hw

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

m2s_x2_VPP_FLAGS := --hls.clock 150000000:m2s_x2
packet_sender_VPP_FLAGS := --hls.clock 150000000:packet_sender
packet_receiver_VPP_FLAGS := --hls.clock 150000000:packet_receiver
s2m_x4_VPP_FLAGS := --hls.clock 150000000:s2m_x4

# =========================================================
# OUTPUT PRODUCTS
# =========================================================
BUILD_DIR := $(PL_KERNELS_DIR)/build.$(TARGET)
TEMP_DIR := $(BUILD_DIR)/_x_temp
TEMP_REPORT_DIR := $(BUILD_DIR)/reports/_x

# ################ TARGET: make aie ################
aie: 
	$(MAKE) aie -C $(AIE_DIR)

# ################ TARGET: make all ################
all: aie
	#$(MAKE) vitis_lib  
	$(MAKE) pl_kernels

# ################ TARGET: make vitis_libraries ################
vitis_lib: Vitis_Libraries
Vitis_Libraries:	
	@echo "### ***** Downloading Vitis Libraries ... *****"
	git clone https://github.com/Xilinx/Vitis_Libraries.git
	@echo "### ***** Vitis Libraries download done! *****"

# ################ TARGET: make pre_build ################
pre_build: _krnlgen 
#vitis_lib

_krnlgen: $(XFLIB_DIR) $(CUR_DIR)/ksrc.mk $(CUR_DIR)/kernel/spec.json
	@echo "### ***** Generating Data-Mover Source Code *.cpp Files ... *****"
	make -f $(CUR_DIR)/ksrc.mk GENKERNEL=$(XFLIB_DIR)/L2/scripts/generate_kernels SPEC=$(CUR_DIR)/kernel/spec.json TOOLDIR=$(CUR_DIR)/_krnlgen
	@echo "### ***** Data-Mover Source Code *.cpp Generated! *****"

# ################ TARGET: make pl_kernels ################
pl_kernels: aie pre_build $(TEMP_DIR)/m2s_x2.xo $(TEMP_DIR)/packet_sender.xo $(TEMP_DIR)/packet_receiver.xo $(TEMP_DIR)/s2m_x4.xo

$(TEMP_DIR)/m2s_x2.xo: $(CUR_DIR)/kernel/m2s_x2.cpp
	@echo "### ***** Compiling m2s_x2 kernel ... *****"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) $(m2s_x2_VPP_FLAGS) -k m2s_x2 --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $< | tee $(BUILD_DIR)/m2s_x2.log
	@echo "### ***** m2s_x2 compilation done! *****"

$(TEMP_DIR)/packet_sender.xo: $(CUR_DIR)/kernel/packet_sender.cpp $(CUR_DIR)/kernel/packet_sender.hpp $(PACKET_IDS_C_H) 
	@echo "### ***** Compiling packet_sender Kernel ... *****"
	echo "Compiling Kernel: packet_sender"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) $(packet_sender_VPP_FLAGS) -k packet_sender --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $< | tee $(BUILD_DIR)/packet_sender.log
	@echo "### ***** packet_sender compilation done! *****"

$(TEMP_DIR)/packet_receiver.xo: $(CUR_DIR)/kernel/packet_receiver.cpp $(CUR_DIR)/kernel/packet_receiver.hpp $(PACKET_IDS_C_H)
	@echo "### ***** Compiling packet_receiver kernel ... *****"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) $(packet_receiver_VPP_FLAGS) -k packet_receiver --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $< | tee $(BUILD_DIR)/packet_receiver.log
	@echo "### ***** packet_receiver compilation done! *****"

$(TEMP_DIR)/s2m_x4.xo: $(CUR_DIR)/kernel/s2m_x4.cpp
	@echo "### ***** Compiling s2m_x4 kernel ... *****"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) $(s2m_x4_VPP_FLAGS) -k s2m_x4 -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $< | tee $(BUILD_DIR)/s2m_x4.log
	@echo "### ***** s2m_x4 compilation done! *****"

# ################ TARGET: make clean ################
clean:
	$(MAKE) clean -C kernel
	rm -rf $(BUILD_DIR)
	rm -rf Vitis_Libraries
	rm -rf *~
