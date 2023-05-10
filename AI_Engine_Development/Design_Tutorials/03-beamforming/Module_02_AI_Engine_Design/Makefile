#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# #########################################################################
#            Makefile used for AIE compilation and simulation 
# #########################################################################

# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_01_DIR := $(PROJECT_DIR)/Module_01_Custom_Platform
MODULE_02_DIR := $(PROJECT_DIR)/Module_02_AI_Engine_Design

# =========================================================
# PLATFORM SELECTION
# =========================================================
PLATFORM_USE := $(MODULE_01_DIR)/sw/build/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm

# =========================================================
# SOURCE DIRECTORIES
# =========================================================
SRC_DIR := $(MODULE_02_DIR)/src
INCLUDE_DIR := $(SRC_DIR)/inc
KERNELS_DIR := $(SRC_DIR)/kernels

# =========================================================
# AI ENGINE APPLICATION
# =========================================================
AIE_APP_CPP	:= $(SRC_DIR)/test_bf_3x.cpp

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS := $(AIE_APP_CPP)
DEPS += $(INCLUDE_DIR)/include.h
DEPS += $(INCLUDE_DIR)/kernels.h
DEPS += $(INCLUDE_DIR)/subsys.h
DEPS += $(KERNELS_DIR)/bf8x8_fst.cc
DEPS += $(KERNELS_DIR)/bf8x8_mid.cc
DEPS += $(KERNELS_DIR)/bf8x8_lst.cc

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR 	:= build
WORK_DIR 	:= Work
AIE_APP_O 	:= libadf.a
SIM_LOG 	:= aiesim.log
XPE_REPO := $(BUILD_DIR)/xpe_dir
BLD_XPE_DIR := $(XPE_REPO)/beamforming
VCD_FILE_NAME := beamforming
BLD_TGT_VCD_FILE := $(BUILD_DIR)/$(VCD_FILE_NAME).vcd
XPE_FILE := $(BLD_XPE_DIR)/$(VCD_FILE_NAME).xpe
# =========================================================
# SIMULATION OPTIONS
# =========================================================
MAX_CYCLES := 350000
LOG_FILE    := log.txt  
TEMP_FILE   := t.txt

all : graph sim vcd check_op3x get_tp3x

graph: $(BUILD_DIR)/libadf.a

$(BUILD_DIR)/libadf.a: $(DEPS)
	mkdir -p $(BUILD_DIR); 		\
	cd $(BUILD_DIR);		\
	aiecompiler -v 			\
		$(AIE_APP_CPP) 		\
		-include=$(SRC_DIR) 	\
		-include=$(KERNELS_DIR) \
		-include=$(INCLUDE_DIR) \
		--platform=$(PLATFORM_USE)	\
		-workdir=$(WORK_DIR)

sim: $(BUILD_DIR)/aiesimulator_output/$(SIM_LOG)

$(BUILD_DIR)/aiesimulator_output/$(SIM_LOG):
	cd $(BUILD_DIR); \
	aiesimulator --pkg-dir=$(WORK_DIR) --output-dir=aiesimulator_output |& tee $(SIM_LOG)

vcd: $(XPE_FILE)

# xpe file generation...
$(XPE_FILE): $(BLD_TGT_VCD_FILE)
	cd $(BUILD_DIR); \
	vcdanalyze --vcd $(VCD_FILE_NAME).vcd --xpe
	rm -rf $(BLD_XPE_DIR)
	mkdir -p $(BLD_XPE_DIR)
	cp -rf $(BUILD_DIR)/aiesim_xpe/*.xpe $(XPE_FILE)
	@echo ""
	@echo "AIE XPE Generation Complete..."
	@echo "####################################"
	@echo ""

# vcd file generation...
$(BLD_TGT_VCD_FILE): 
	cd $(BUILD_DIR); \
	aiesimulator --pkg-dir=$(WORK_DIR) --output-dir=aiesimulator_output  --profile --dump-vcd $(VCD_FILE_NAME) 2>&1 | tee -a vcd.log
	@echo ""
	@echo "AIE VCD Trace Generation Complete..."
	@echo "####################################"
	@echo ""

get_tp3x:
	@for p in {0..23} ; do \
		grep -ve '[XT]' build/aiesimulator_output/data/dlbf_out$${p}.txt > tmp;\
		x=`wc -l tmp | awk '{print $$1}'`;\
		grep 'T ' build/aiesimulator_output/data/dlbf_out$${p}.txt > tmp;\
		t0=`sed -ne '1p;$$p' tmp | awk '(NR==1){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}'`;\
		r=`sed -ne '1p;$$p' tmp | awk '(NR==2){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}' | awk -v a=$$x -v b0=$$t0 '{print a*2/($$1-b0)*1000}'`;\
		echo "DLBF" $${p} "Throughput=" $$r "Msps (>800Msps)"; \
	done; \
	for p in {0..11} ; do \
		grep -ve '[XT]' build/aiesimulator_output/data/ulbf_out$${p}.txt > tmp;\
		x=`wc -l tmp | awk '{print $$1}'`;\
		grep 'T ' build/aiesimulator_output/data/ulbf_out$${p}.txt > tmp;\
		t0=`sed -ne '1p;$$p' tmp | awk '(NR==1){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}'`;\
		r=`sed -ne '1p;$$p' tmp | awk '(NR==2){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}' | awk -v a=$$x -v b0=$$t0 '{print a*2/($$1-b0)*1000}'`;\
		echo "ULBF" $${p} "Throughput=" $$r "Msps (>800Msps)"; \
	done


check_op3x:
	@for p in {0..23} ; do \
		grep -ve '[XT]' build/aiesimulator_output/data/dlbf_out$${p}.txt > $(TEMP_FILE);\
		diff -w $(TEMP_FILE) data/dlbf_gold$$((p%8)).txt > diff_output;\
		r0=`wc diff_output | awk '{print "diff=" $$1 }'`;\
		r1=`head -1 diff_output`;\
		echo "DLBF" $$p "-" $$r0 "-" $$r1;\
	done; \
	for p in {0..11} ; do \
		grep -ve '[XT]' build/aiesimulator_output/data/ulbf_out$${p}.txt > $(TEMP_FILE);\
		diff -w $(TEMP_FILE) data/ulbf_gold$$((p%4)).txt > diff_output;\
		r0=`wc diff_output | awk '{print "diff=" $$1 }'`;\
		r1=`head -1 diff_output`;\
		echo "ULBF" $$p "-" $$r0 "-" $$r1;\
	done


clean:
	rm -rf $(BUILD_DIR)/$(WORK_DIR)/$(AIE_APP_O)
	rm -rf $(BUILD_DIR)/$(WORK_DIR)
	rm -rf $(BUILD_DIR)/aiesimulator_output/$(SIM_LOG)
	rm -rf $(BUILD_DIR)/aiesimulator_output
	rm -rf $(BUILD_DIR)
