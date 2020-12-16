# © Copyright 2020 Xilinx, Inc.
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

# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ./

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_01_DIR := $(PROJECT_DIR)/Module_01_Custom_Platform
MODULE_02_DIR := $(PROJECT_DIR)/Module_02_AI_Engine_Design
MODULE_03_DIR := $(PROJECT_DIR)/Module_03_PL_Design
MODULE_04_DIR := $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration
MODULE_05_DIR := $(PROJECT_DIR)/Module_05_Baremetal_Host_Application
MODULE_06_DIR := $(PROJECT_DIR)/Module_06_System_Integration
MODULE_07_DIR := $(PROJECT_DIR)/Module_07_Petalinux
MODULE_08_DIR := $(PROJECT_DIR)/Module_08_Linux_SW_Application
MODULE_09_DIR := $(PROJECT_DIR)/Module_09

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation (default)
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# XCLBIN Revision Selection 
#  rev0: Select XCLBIN and PS Host APP without timing closure
#  rev1: Select XCLBIN and PS Host APP with timing closure
# =========================================================
REV := rev1

# =========================================================
# MAIN can be set as:
#   main_partial:  check only 10 samples of data (default)
#   main_full   :  check all 4 iterations with entire dataset
# =========================================================
MAIN := main_partial

run: build
	$(MAKE) run_emu -C $(MODULE_06_DIR) TARGET=hw_emu REV=rev1 MAIN=main_partial

build:
	$(MAKE) platform -C $(MODULE_01_DIR)
	$(MAKE) graph -C $(MODULE_02_DIR)
	$(MAKE) kernels -C $(MODULE_03_DIR)
	$(MAKE) xclbin_prebuilt -C $(MODULE_04_DIR)
	$(MAKE) application -C $(MODULE_05_DIR) MAIN=main_partial
	$(MAKE) package -C $(MODULE_06_DIR) TARGET=hw_emu MAIN=main_partial
	$(MAKE) package -C $(MODULE_06_DIR) TARGET=hw MAIN=main_partial
	$(MAKE) application -C $(MODULE_05_DIR) MAIN=main_full
	$(MAKE) package -C $(MODULE_06_DIR) TARGET=hw MAIN=main_full
	$(MAKE) petalinux -C $(MODULE_07_DIR)
	$(MAKE) exe -C $(MODULE_08_DIR)
	$(MAKE) all -C $(MODULE_09_DIR)

build_all:
	$(MAKE) platform -C $(MODULE_01_DIR)
	$(MAKE) graph -C $(MODULE_02_DIR)
	$(MAKE) kernels -C $(MODULE_03_DIR)
	$(MAKE) xclbin -C $(MODULE_04_DIR) TARGET=hw_emu REV=rev0
	$(MAKE) xclbin -C $(MODULE_04_DIR) TARGET=hw REV=rev0
	$(MAKE) xclbin -C $(MODULE_04_DIR) TARGET=hw_emu REV=rev1
	$(MAKE) xclbin -C $(MODULE_04_DIR) TARGET=hw REV=rev1
	$(MAKE) application -C $(MODULE_05_DIR) MAIN=main_partial
	$(MAKE) package -C $(MODULE_06_DIR) TARGET=hw_emu MAIN=main_partial
	$(MAKE) package -C $(MODULE_06_DIR) TARGET=hw MAIN=main_partial
	$(MAKE) application -C $(MODULE_05_DIR) MAIN=main_full
	$(MAKE) package -C $(MODULE_06_DIR) TARGET=hw MAIN=main_full
	$(MAKE) petalinux -C $(MODULE_07_DIR)
	$(MAKE) exe -C $(MODULE_08_DIR)
	$(MAKE) all -C $(MODULE_09_DIR)

clean:
	$(MAKE) clean_all -C Module_06_System_Integration
