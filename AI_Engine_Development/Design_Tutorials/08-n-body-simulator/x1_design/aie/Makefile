#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
.PHONY: help

help::
	@echo "Makefile Usage:"
	@echo "  make all "
	@echo "       Runs make aie"
	@echo ""
	@echo "  make aie "
	@echo "       Command to run aiecompiler and create libadf.a"
	@echo ""
	@echo "  make sim "
	@echo "       Command to run aiesimulator with data/input_i.txt and"
	@echo "       data/input_j.txt"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

# #########################################################################
#            Makefile used for AIE compilation and simulation 
# #########################################################################

# =========================================================
# NUMBER OF COMPUTE UNITS INSTANCES 
# x1   = 1   AIE TILE
# x10  = 10  AIE TILES
# X100 = 100 AIE TILES
# =========================================================
INST		= x1

# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative directory
RELATIVE_PROJECT_DIR := ../../

#absolute directory
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#Design Directory 
DESIGN_DIR := $(PROJECT_DIR)/$(INST)_design

AIE_DIR := $(DESIGN_DIR)/aie

# =========================================================
# AIE GRAPH
# =========================================================
GRAPH_CPP := $(AIE_DIR)/nbody_x4_$(INST).cpp 

# =========================================================
# DEPENDENCIES for make aie
# =========================================================
DEPS := $(GRAPH_CPP)
DEPS += $(AIE_DIR)/nbody_x4_$(INST).h
DEPS += $(AIE_DIR)/src/globals.h
DEPS += $(AIE_DIR)/src/include.h
DEPS += $(AIE_DIR)/src/kernels.h
DEPS += $(AIE_DIR)/src/nbody.cc
DEPS += $(AIE_DIR)/src/nbody_subsystem.h
DEPS += $(AIE_DIR)/src/transmit_new_i.cc 

# =========================================================
# OUTPUT PRODUCTS 
# =========================================================
BUILD_DIR := $(AIE_DIR)/build
WORK_DIR := $(BUILD_DIR)/Work_x4_$(INST)

# ################ TARGET: make all ################
all: aie

# ################ TARGET: make aie ################
aie: $(BUILD_DIR)/libadf.a

$(BUILD_DIR)/libadf.a: $(DEPS)
	@echo "### ***** Compiling AI Engine Graph... ***** ###"
	@mkdir -p $(BUILD_DIR);				\
	cd $(BUILD_DIR);				\
	aiecompiler -v  --target=hw			\
	--stacksize=2000				\
	-include="$(XILINX_VITIS)/aietools/include" 	\
	-include="$(AIE_DIR)" 				\
	-include="$(AIE_DIR)/src"			\
	-include="$(DESIGN_DIR)/data" 			\
	--optimize-pktids=false                         \
        $(GRAPH_CPP)					\
	-workdir=$(WORK_DIR) | tee  $(BUILD_DIR)/aiecompiler_$(INST).log
	@echo "### ***** AI Engine Graph Compilation done! ***** ###"

# ################ TARGET: make sim ################
sim: $(BUILD_DIR)/aiesimulator_$(INST).log

$(BUILD_DIR)/aiesimulator_$(INST).log: aie
	@echo "### ***** Simulating AI Engine Graph... ***** ###"
	cd $(BUILD_DIR);\
	aiesimulator --pkg-dir=$(WORK_DIR) | tee aiesimulator_$(INST).log
	@echo "### ***** AI Engine Graph Simulation done! ***** ###"

# ################ TARGET: make clean_sim ################
clean_sim: 
	rm -rf $(BUILD_DIR)/aiesimulator*
	rm -rf $(BUILD_DIR)/pl_sample_count*

# ################ TARGET: make clean ################
clean: 
	rm -rf $(BUILD_DIR)
	rm -rf *~
	rm -rf *#
