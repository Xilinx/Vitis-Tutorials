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
MODULE_07_DIR := $(PROJECT_DIR)/Module_07_Petalinux
MODULE_08_DIR := $(PROJECT_DIR)/Module_08_Linux_SW_Application
MODULE_09_DIR := $(PROJECT_DIR)/Module_09_Running_the_Linux_System

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# INPUTS
# =========================================================
LIBADF_A := $(MODULE_02_DIR)/build/libadf.a
EMBEDDED_EXEC_SCRIPT := $(MODULE_09_DIR)/embedded_exec.sh

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := $(MODULE_09_DIR)/build
EMBEDDED_PACKAGE_OUT := $(BUILD_DIR)/package_$(TARGET)
AIE_XCLBIN := $(BUILD_DIR)/aie_$(TARGET)/aie.xclbin 

####### TARGET: make aie_xclbin #######
aie_xclbin: $(BUILD_DIR)/aie_$(TARGET)/aie.xclbin

$(BUILD_DIR)/aie_$(TARGET)/aie.xclbin:
	mkdir -p $(BUILD_DIR); 		\
	cd $(BUILD_DIR);		\
	v++ -s -p -t $(TARGET) -f $(MODULE_01_DIR)/sw/build/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm \
	--package.out_dir $(BUILD_DIR)/aie_$(TARGET) \
	--config $(MODULE_09_DIR)/package.cfg \
	$(LIBADF_A)
	mv $(BUILD_DIR)/a.xclbin $(AIE_XCLBIN)

####### TARGET: make package #######
package: aie_xclbin $(BUILD_DIR)/package_$(TARGET)/sd_card.img

$(BUILD_DIR)/package_$(TARGET)/sd_card.img:
	cd $(BUILD_DIR); 								\
	v++ -p  -t $(TARGET) --save-temps                                    		\
	-f  $(MODULE_07_DIR)/build/platform_repo/vck190_custom/export/vck190_custom/vck190_custom.xpfm     \
	--package.rootfs $(MODULE_07_DIR)/build/vck190_linux/images/linux/rootfs.ext4	\
	--package.kernel_image ${MODULE_07_DIR}/build/vck190_linux/images/linux/Image   \
	--package.boot_mode=sd                                              		\
	--package.out_dir $(EMBEDDED_PACKAGE_OUT)                   	 		\
	--package.image_format=ext4                                         		\
	--package.defer_aie_run                                             		\
	--package.sd_file $(AIE_XCLBIN) 	                                    	\
	--package.sd_file $(MODULE_08_DIR)/test_bf_app.exe 				\
	--package.sd_file $(EMBEDDED_EXEC_SCRIPT)					\
	$(MODULE_04_DIR)/build/rev1/$(TARGET)/beamforming.rev1.$(TARGET).xclbin $(LIBADF_A)

####### TARGET: make run #######
run: run_emu

run_emu: package
ifeq ($(TARGET),hw_emu)
	cd $(EMBEDDED_PACKAGE_OUT); \
	$(EMBEDDED_PACKAGE_OUT)/launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT)
else
	@echo "Hardware build, no emulation executed."
endif


clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.log
