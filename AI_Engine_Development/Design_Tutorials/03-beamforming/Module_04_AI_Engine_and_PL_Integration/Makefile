#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_01_DIR := $(PROJECT_DIR)/Module_01_Custom_Platform
MODULE_02_DIR := $(PROJECT_DIR)/Module_02_AI_Engine_Design
MODULE_03_DIR := $(PROJECT_DIR)/Module_03_PL_Design
MODULE_04_DIR := $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration

# =========================================================
# PLATFORM SELECTION
# =========================================================
PLATFORM := $(MODULE_01_DIR)/sw/build/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# AI ENGINE GRAPH
# =========================================================
TEST_BF_O := $(MODULE_02_DIR)/build/Work/temp/test_bf_3x.o

# =========================================================
# XCLBIN Revision Selection 
#  rev0: creates XCLBIN rev0 without timing closure
#  rev1: creates XCLBIN rev1 with timing closure
# =========================================================
REV := rev1

# =========================================================
# PL RTL KERNELS 
# =========================================================
PL_KERNELS_REPO 	:= $(MODULE_03_DIR)/build
DLBF_SLAVE_XO 		:= $(PL_KERNELS_REPO)/dlbf_slave.xo
DLBF_DATA_XO		:= $(PL_KERNELS_REPO)/dlbf_data.xo
DLBF_COEFFS_XO		:= $(PL_KERNELS_REPO)/dlbf_coeffs.xo
ULBF_SLAVE_XO		:= $(PL_KERNELS_REPO)/ulbf_slave.xo
ULBF_DATA_XO		:= $(PL_KERNELS_REPO)/ulbf_data.xo
ULBF_COEFFS_XO		:= $(PL_KERNELS_REPO)/ulbf_coeffs.xo
AXI4S_REGSLICE_XO  	:= $(PL_KERNELS_REPO)/axi4s_regslice_64b.xo

ifeq ($(REV),rev0)
	XOS := $(DLBF_SLAVE_XO) 
	XOS += $(DLBF_DATA_XO) 
	XOS += $(DLBF_COEFFS_XO) 
	XOS += $(ULBF_SLAVE_XO) 
	XOS += $(ULBF_DATA_XO) 
	XOS += $(ULBF_COEFFS_XO)
else ifeq ($(REV),rev1)
	XOS := $(DLBF_SLAVE_XO) 
	XOS += $(DLBF_DATA_XO) 
	XOS += $(DLBF_COEFFS_XO) 
	XOS += $(ULBF_SLAVE_XO) 
	XOS += $(ULBF_DATA_XO) 
	XOS += $(ULBF_COEFFS_XO)
	XOS += $(AXI4S_REGSLICE_XO)	
endif

# =========================================================
# CONNECTIVITY SPECS
# =========================================================
ifeq ($(REV),rev0)
	CONFIG_INI := $(MODULE_04_DIR)/config.ini
else ifeq ($(REV),rev1)
	CONFIG_INI := $(MODULE_04_DIR)/config_2regslice.ini
endif

# =========================================================
# DEPENDENCIES
# ========================================================
DEPS := $(TEST_BF_O) 
DEPS += $(XOS) 
DEPS += $(CONFIG_INI)
DEPS += $(PLATFORM) 

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
XCLBIN	  := beamforming.$(REV).$(TARGET).xclbin 

xclbin_prebuilt: $(BUILD_DIR)/README.md

$(BUILD_DIR)/README.md:
	cp -vr support/$(BUILD_DIR) .
	unzip $(BUILD_DIR)/rev1/hw_emu.zip -d $(BUILD_DIR)/rev1 
	unzip $(BUILD_DIR)/rev1/hw.zip -d $(BUILD_DIR)/rev1 

xclbin_all:
	$(MAKE) xclbin TARGET=hw REV=rev0
	$(MAKE) xclbin TARGET=hw REV=rev1
	$(MAKE) xclbin TARGET=hw_emu REV=rev1

xclbin: $(BUILD_DIR)/$(REV)/$(TARGET)/$(XCLBIN)

$(BUILD_DIR)/$(REV)/$(TARGET)/$(XCLBIN): $(DEPS)
	mkdir -p $(BUILD_DIR)/$(REV)/$(TARGET); \
	cd $(BUILD_DIR)/$(REV)/$(TARGET);	\
	v++ -l					\
		-t $(TARGET) 			\
		-f $(PLATFORM) 			\
		--save-temps 			\
		-g 				\
		$(TEST_BF_O)			\
		$(XOS)				\
		--config $(CONFIG_INI)		\
		-o $(XCLBIN)

clean:
	rm -rf $(BUILD_DIR)/$(REV)/$(TARGET)/$(XCLBIN)
	rm -rf $(BUILD_DIR)/$(REV)/$(TARGET)
	rm -rf $(BUILD_DIR)/$(REV)
	rm -rf $(BUILD_DIR)/rev0
	rm -rf $(BUILD_DIR)
