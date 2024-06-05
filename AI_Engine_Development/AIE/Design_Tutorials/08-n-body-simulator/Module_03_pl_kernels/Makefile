#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
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
# PROJECT DIRECTORIES
# =========================================================
#relative project directory
RELATIVE_PROJECT_DIR := ../

#absolute project directory
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#Design Directory 
DESIGN_DIR := $(PROJECT_DIR)

PL_KERNELS_DIR := $(DESIGN_DIR)/Module_03_pl_kernels
AIE_DIR := $(DESIGN_DIR)/Module_02_aie

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))


XFLIB_DIR_REL_PATH :=$(DSPLIB_VITIS)/data_mover
XFLIB_DIR := $(shell readlink -f $(XFLIB_DIR_REL_PATH))
VITIS_PYTHON3 := python3
# =========================================================
# TARGET SELECTION
# TARGET=hw (default)
# TARGET=hw_emu
# =========================================================
TARGET ?=hw

# =========================================================
# NUMBER OF COMPUTE UNITS INSTANCES 
# x1   = 1   AIE TILE
# x10  = 10  AIE TILES
# X100 = 100 AIE TILES
# =========================================================
INST		= x100

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
VPP_FLAGS += -I$(XFLIB_DIR)/L2/include
VPP_FLAGS += -I$(CUR_DIR)/kernel
mm2s_mp_VPP_FLAGS := --hls.clock 150000000:mm2s_mp
packet_sender_VPP_FLAGS := --hls.clock 150000000:packet_sender
packet_receiver_VPP_FLAGS := --hls.clock 150000000:packet_receiver
s2mm_mp_VPP_FLAGS := --hls.clock 150000000:s2mm_mp

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
all:
	#$(MAKE) vitis_lib  
	$(MAKE) pl_kernels

# ################ TARGET: make vitis_libraries ################
vitis_lib: Vitis_Libraries
Vitis_Libraries:	
	@echo "### ***** Downloading Vitis Libraries ... *****"
	git clone https://github.com/Xilinx/Vitis_Libraries.git
	@echo "### ***** Vitis Libraries download done! *****"

# ################ TARGET: make pre_build ################
pre_build: gen_instances 
#vitis_lib
GENERATED_FILES += kernel/mm2s_mp.cpp
kernel/mm2s_mp.cpp: 
	$(VITIS_PYTHON3) $(XFLIB_DIR)/scripts/instance_generator.py -o kernel/mm2s_mp.cpp --param-set mm2s_mp --param-file config_params.json --spec "$(XFLIB_DIR)/L2/meta/mm2s_mp.json" --api "xf::data_mover::mm2s_mp"
GENERATED_FILES += kernel/s2mm_mp.cpp
kernel/s2mm_mp.cpp:
	$(VITIS_PYTHON3) $(XFLIB_DIR)/scripts/instance_generator.py -o kernel/s2mm_mp.cpp --param-set s2mm_mp --param-file config_params.json --spec "$(XFLIB_DIR)/L2/meta/s2mm_mp.json" --api "xf::data_mover::s2mm_mp"

gen_instances: $(GENERATED_FILES)
# ################ TARGET: make pl_kernels ################
pl_kernels: pre_build $(TEMP_DIR)/mm2s_mp.xo $(TEMP_DIR)/packet_sender.xo $(TEMP_DIR)/packet_receiver.xo $(TEMP_DIR)/s2mm_mp.xo

$(TEMP_DIR)/mm2s_mp.xo: $(CUR_DIR)/kernel/mm2s_mp.cpp
	@echo "### ***** Compiling mm2s_mp kernel ... *****"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) $(mm2s_mp_VPP_FLAGS) -k mm2s_mp --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $< | tee $(BUILD_DIR)/mm2s_mp.log
	@echo "### ***** mm2s_mp compilation done! *****"

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

$(TEMP_DIR)/s2mm_mp.xo: $(CUR_DIR)/kernel/s2mm_mp.cpp
	@echo "### ***** Compiling s2mm_mp kernel ... *****"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	mkdir -p $(TEMP_DIR); \
	v++ -c $(VPP_FLAGS) $(s2mm_mp_VPP_FLAGS) -k s2mm_mp -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' $< | tee $(BUILD_DIR)/s2mm_mp.log
	@echo "### ***** s2mm_mp compilation done! *****"

# ################ TARGET: make clean ################
clean:
	rm -rf ./kernel/mm2s_mp.cpp
	rm -rf ./kernel/s2mm_mp.cpp
	rm -rf *.log
	rm -rf *~
	rm -rf *#
	rm -rf .stamp
	rm -rf *~
	rm -rf $(BUILD_DIR)
	rm -rf Vitis_Libraries
	rm -rf *~
	rm -rf *.log
