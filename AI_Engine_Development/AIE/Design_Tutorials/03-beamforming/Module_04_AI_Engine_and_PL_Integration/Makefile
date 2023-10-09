#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11



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
PLATFORM_USE := $(MODULE_01_DIR)/sw/build/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# AI ENGINE GRAPH
# =========================================================
TEST_BF_O := $(MODULE_02_DIR)/build/libadf.a

# =========================================================
# XSA Revision Selection 
#  rev0: creates XSA rev0 without timing closure
#  rev1: creates XSA rev1 with timing closure
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
DEPS += $(PLATFORM_USE) 

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
XSA	  := beamforming.$(REV).$(TARGET).xsa 

xsa_prebuilt: $(BUILD_DIR)/README.md

$(BUILD_DIR)/README.md:
	wget -O build.zip https://www.xilinx.com/bin/public/openDownload?filename=build.zip
	unzip build.zip
	unzip $(BUILD_DIR)/rev1/hw/xsa.zip -d $(BUILD_DIR)/rev1/hw
	unzip $(BUILD_DIR)/rev1/hw/xsa.zip -d $(BUILD_DIR)/rev1/hw
	unzip $(BUILD_DIR)/rev1/hw_emu/xsa.zip -d $(BUILD_DIR)/rev1/hw_emu 

xsa_all:
	$(MAKE) xsa TARGET=hw REV=rev0
	$(MAKE) xsa TARGET=hw REV=rev1
	$(MAKE) xsa TARGET=hw_emu REV=rev1

xsa: $(BUILD_DIR)/$(REV)/$(TARGET)/$(XSA)

$(BUILD_DIR)/$(REV)/$(TARGET)/$(XSA): $(DEPS)
	mkdir -p $(BUILD_DIR)/$(REV)/$(TARGET); \
	cd $(BUILD_DIR)/$(REV)/$(TARGET);	\
	v++ -l					\
		-t $(TARGET) 			\
		-f $(PLATFORM_USE) 			\
		--save-temps 			\
		-g 				\
		$(TEST_BF_O)			\
		$(XOS)				\
		--config $(CONFIG_INI)		\
		-o $(XSA)

clean:
	rm -rf $(BUILD_DIR)/$(REV)/$(TARGET)/$(XSA)
	rm -rf $(BUILD_DIR)/$(REV)/$(TARGET)
	rm -rf $(BUILD_DIR)/$(REV)
	rm -rf $(BUILD_DIR)/rev0
	rm -rf $(BUILD_DIR)
	rm -rf $(BUILD_DIR).zip
