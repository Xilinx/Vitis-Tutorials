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
MODULE_05_DIR := $(PROJECT_DIR)/Module_05_Baremetal_Host_Application

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# MAIN can be set as:
#   main_partial:  check only 10 samples of data
#   main_full   :  check all 4 iterations with entire dataset
# =========================================================
MAIN := main_partial

# =========================================================
# PLATFORM SELECTION
# =========================================================
PLATFORM := $(MODULE_01_DIR)/sw/build/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm

# =========================================================
# AI ENGINE APPLICATION SELECTION
# =========================================================
LIBADF_A := $(MODULE_02_DIR)/build/libadf.a

# =========================================================
# XCLBIN SELECTION
# =========================================================
XCLBIN := $(MODULE_04_DIR)/build/rev1/$(TARGET)/beamforming.rev1.$(TARGET).xclbin

# =========================================================
# PS HOST APPLICATION SELECTION
# =========================================================
PS_ELF := $(MODULE_05_DIR)/build/$(MAIN)_ps.elf

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
BUILD_TARGET_MAIN_DIR := $(BUILD_DIR)/$(TARGET)_$(MAIN)
PACKAGE_DIR := $(TARGET)_$(MAIN).package
SD_CARD_IMG := sd_card.img

platform: 
	$(MAKE) platform -C $(MODULE_01_DIR)

graph: 
	$(MAKE) graph -C $(MODULE_02_DIR)

kernels: 
	$(MAKE) kernels -C $(MODULE_03_DIR)

xclbin: 
	$(MAKE) xclbin_prebuilt -C $(MODULE_04_DIR)

application: 
	$(MAKE) application -C $(MODULE_05_DIR)

package: $(BUILD_TARGET_MAIN_DIR)/$(PACKAGE_DIR)/$(SD_CARD_IMG)

$(BUILD_TARGET_MAIN_DIR)/$(PACKAGE_DIR)/$(SD_CARD_IMG):
	mkdir $(BUILD_TARGET_MAIN_DIR) -p;		\
	cd $(BUILD_TARGET_MAIN_DIR);			\
	v++ -p -t $(TARGET) 				\
		-f $(PLATFORM) 				\
		$(XCLBIN) $(LIBADF_A) 			\
		--package.out_dir $(PACKAGE_DIR)	\
		--package.domain aiengine 		\
		--package.boot_mode=sd 			\
		--package.image_format=fat32 		\
		--package.ps_elf $(PS_ELF),a72-0	

run_emu: 
	cd $(BUILD_TARGET_MAIN_DIR)/$(PACKAGE_DIR); \
	./launch_hw_emu.sh

build: platform graph kernels xclbin application package 

run: build run_emu

clean:
	rm -fr $(BUILD_TARGET_MAIN_DIR)/$(PACKAGE_DIR)
	rm -fr $(BUILD_TARGET_MAIN_DIR)/hw_main_partial
	rm -fr $(BUILD_TARGET_MAIN_DIR)/hw_main_full
	rm -fr $(BUILD_DIR)

clean_all: clean
	$(MAKE) clean -C $(PROJECT_DIR)/Module_05_Baremetal_Host_Application
	$(MAKE) clean -C $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration
	$(MAKE) clean -C $(PROJECT_DIR)/Module_03_PL_Design 
	$(MAKE) clean -C $(PROJECT_DIR)/Module_02_AI_Engine_Design
	$(MAKE) clean -C $(PROJECT_DIR)/Module_01_Custom_Platform
