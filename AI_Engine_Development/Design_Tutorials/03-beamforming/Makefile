
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# ---------------------------------
# Target all: Generate the Vitis platform and verify it including base bootable design
# Target main: Generate the Vitis platform and verify it without base bootable design
# ---------------------------------
.PHONY: all step0 step1 step2 step3 step3b step4 step4b step5 step6 step7 step8


RELATIVE_PROJECT_DIR := ./
TARGET := hw
#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))
BUILD_DIR 	:= $(PROJECT_DIR)/build

EMBEDDED_PACKAGE_OUT = $(PROJECT_DIR)/Module_09_Running_the_Linux_System/build/package_$(TARGET)
EMBEDDED_EXEC_SCRIPT = $(PROJECT_DIR)/Module_09_Running_the_Linux_System/embedded_exec.sh
PACKAGE_FILES += $(EMBEDDED_EXEC_SCRIPT)
SD_FILES_WITH_PREFIX = $(foreach sd_file,$(PACKAGE_FILES),--package.sd_file $(sd_file))
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')

RESULT_VAL ?= 
RUN_LEVEL ?= 7

run: all run_emu
sd_card: all


ifeq (${TEST}, SPRITE)
all: step0 step1 step2 step3 step3b step4 step4b step5 step5a step5b step6 step7 step8 step8b
else
all: step0 step1 step2 step3 step3b step4 step4b step5 step5a step5b step6 step7 step8 step8b
endif


step0:
	$(MAKE) -C Module_01_Custom_Platform platform

step1:
	$(MAKE) -C Module_02_AI_Engine_Design graph

step2:
	$(MAKE) -C Module_03_PL_Design kernels

step3:
	$(MAKE) -C Module_04_AI_Engine_and_PL_Integration xsa TARGET=hw REV=rev1

SHELL := /bin/bash
step3b:
	$(MAKE) -C Module_04_AI_Engine_and_PL_Integration xsa TARGET=hw_emu REV=rev1

step4:
	$(MAKE) -C Module_05_Baremetal_Host_Application application MAIN=main_partial

step4b:
	$(MAKE) -C Module_05_Baremetal_Host_Application application MAIN=main_full

step5:
	$(MAKE) -C Module_06_Running_the_Baremetal_System package TARGET=hw_emu MAIN=main_partial
step5a:
	$(MAKE) -C Module_06_Running_the_Baremetal_System package TARGET=hw MAIN=main_partial
step5b:
	$(MAKE) -C Module_06_Running_the_Baremetal_System package TARGET=hw MAIN=main_full

SHELL := /bin/bash
step6:
	$(MAKE) -C Module_07_Petalinux petalinux

step7:
	$(MAKE) -C Module_08_Linux_SW_Application exe

step8:
	$(MAKE) -C Module_09_Running_the_Linux_System package TARGET=hw

step8b:
	$(MAKE) -C Module_09_Running_the_Linux_System package TARGET=hw_emu


run_emu:
	$(MAKE) -C Module_09_Running_the_Linux_System run_emu

clean:
	$(MAKE) -C Module_09_Running_the_Linux_System clean
	$(MAKE) -C Module_08_Linux_SW_Application clean
	$(MAKE) -C Module_07_Petalinux clean
	$(MAKE) -C Module_06_Running_the_Baremetal_System clean
	$(MAKE) -C Module_05_Baremetal_Host_Application clean

ultra_clean: clean
	$(MAKE) -C Module_04_AI_Engine_and_PL_Integration clean
	$(MAKE) -C Module_03_PL_Design clean
	$(MAKE) -C Module_02_AI_Engine_Design clean
	$(MAKE) -C Module_01_Custom_Platform clean
