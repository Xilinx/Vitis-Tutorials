# © Copyright 2021 Xilinx, Inc.
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

%.PHONY: help
help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "    make kernels [OPTIONS]"
	@echo  "        Command to generate the PL kernels"
	@echo  ""
	@echo  "    make graph [OPTIONS]"
	@echo  "        Command to generate the AI Engine graph"
	@echo  ""
	@echo  "    make xclbin [OPTIONS]"
	@echo  "        Command to generate the xclbin for specified target"
	@echo  ""
	@echo  "    make application [OPTIONS]"
	@echo  "        Command to generate the A72 application"
	@echo  ""
	@echo  "    make package [OPTIONS]"
	@echo  "        Command to package design into an sd_card"
	@echo  ""
	@echo  "    make build [OPTIONS]"
	@echo  "        Executes the previous five targets (kernels, graph, xclbin, application, package) to build the design"
	@echo  ""
	@echo  "    make run_emu [OPTIONS]"
	@echo  "        Runs Hardware Emulation (if hardware target is specified, moves files to run_dir)"
	@echo  ""	
	@echo  "    make clean"
	@echo  "        Command to remove the generated files"
	@echo  ""
	@echo  "    OPTIONS:"
	@echo  "        TARGET=[hw|hw_emu]     -specifies target: Hardware or Emulation. Default=hw_emu"
	@echo  "        N_FIR_FILTERS          -specifies number of FIR Filters. Default=1"
	@echo  "        N_FIR_TAPS             -specifies number of FIR Filter Taps. Default=15"
	@echo  ""


print-%  : ; @echo $* = $($*)

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET        := hw_emu

# =========================================================
# FIR Chain Parameters:
# N_FIR_FILTERS : Number for FIR filters in chain
# N_FIR_TAPS    : Number of taps in each filter
# =========================================================
N_FIR_FILTERS :=   1
N_FIR_TAPS    :=  15

# =========================================================
# Profiling and Trace Switches
# =========================================================
EN_TRACE       := 0

# =========================================================
# Platform
# =========================================================
PLATFORM      := xilinx_vck190_base_202110_1

# =========================================================
# PL Frequency in MHz:
# 300 MHz(default)
# =========================================================
PL_FREQ        := 300
VPP_CLOCK_FREQ := $(PL_FREQ)000000

# =========================================================
# FIR Impulse Response Area
# =========================================================
IMPULSE_AREA	:= 16383

# =========================================================
# Choose the compiler based on the A72 Processor in the 
# Versal device
# =========================================================
HOST_CXX        := aarch64-linux-gnu-g++

# =========================================================
# Choose the XRT and XAIENGINE
# =========================================================
XLNX_VERSAL	:= $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal
SYSROOT_PATH	:= $(XLNX_VERSAL)/sysroots/aarch64-xilinx-linux
XRT_PATH        := $(SYSROOT_PATH)/usr/include/xrt

# =========================================================
# Source directory
# =========================================================
#Version Name
BUILD_VERSION   := fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps

#relative FIR filter directory
RELATIVE_PROJECT_DIR := ./

#absolute FIR filter directory = <user path>/Tutorials/AI_Engine/filter_AIEvsDSP
PROJECT_REPO	 := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO 	 := $(PROJECT_REPO)/design
PL_SRC_REPO 	 := $(PROJECT_REPO)/design/pl_src
APP_SRC_REPO 	 := $(PROJECT_REPO)/design/app_src
MAKEFILES_REPO   := $(PROJECT_REPO)

BASE_BUILD_DIR 	 := $(PROJECT_REPO)/build
BUILD_DIR        := $(BASE_BUILD_DIR)/$(BUILD_VERSION)
BUILD_TARGET_DIR := $(BUILD_DIR)/$(TARGET)

BASE_RUN_DIR     := $(PROJECT_REPO)/run_dir
RUN_DIR		 := $(BASE_RUN_DIR)/$(BUILD_VERSION)

REPORT_DIR     	:= $(PROJECT_REPO)/../report_dir

# =========================================================
# Kernel Source Files repository
# =========================================================
S2MM_KERNEL_NAME  := s2mm
MM2S_KERNEL_NAME  := mm2s
FILTER_KERNEL_NAME:= fir_dsp

S2MM_KERNEL_SRC	  := $(PL_SRC_REPO)/$(S2MM_KERNEL_NAME).cpp
MM2S_KERNEL_SRC	  := $(PL_SRC_REPO)/$(MM2S_KERNEL_NAME).cpp

SYSTEM_SETTINGS_H := $(PL_SRC_REPO)/system_settings.h
FILTER_KERNEL_H   := $(PL_SRC_REPO)/$(FILTER_KERNEL_NAME).h
FILTER_KERNEL_SRC := $(PL_SRC_REPO)/$(FILTER_KERNEL_NAME).cpp

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP       := $(APP_SRC_REPO)/fir_dsp_app.cpp

# =========================================================
# Below are the names, application executable,
# kernel executables, and xclbin
# =========================================================
FILTER_KERNEL_XO  := $(BUILD_TARGET_DIR)/fir_dsp.$(TARGET).xo
S2MM_KERNEL_XO    := $(BUILD_TARGET_DIR)/$(S2MM_KERNEL_NAME).$(TARGET).xo
MM2S_KERNEL_XO 	  := $(BUILD_TARGET_DIR)/$(MM2S_KERNEL_NAME).$(TARGET).xo
XCLBIN 	          := $(BUILD_TARGET_DIR)/vck190_dsp_fir_dsp.$(TARGET).xclbin
APP_ELF           := $(BUILD_TARGET_DIR)/fir_dsp_xrt.elf

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__
GCC_FLAGS += -DXAIE_DEBUG

ifeq ($(TARGET),hw_emu)
GCC_FLAGS += -DUSE_SMALL_DATASET
endif

GCC_INC_FLAGS := -I$(XRT_PATH)/
GCC_INC_FLAGS += -I$(SYSROOT_PATH)/usr/include/
GCC_INC_FLAGS += -I$(SYSROOT_PATH)/usr/lib/
GCC_INC_FLAGS += -I$(APP_SRC_REPO)/
GCC_INC_FLAGS += -I$(PL_SRC_REPO)/

GCC_LIB_INC := --sysroot=$(SYSROOT_PATH)
GCC_LIB_INC += -L=$(SYSROOT_PATH)/usr/lib

GCC_LIB := -lxrt_coreutil 

# =========================================================
# Kernel Compiler and Linker Flags
# =========================================================
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --include $(PL_SRC_REPO)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose 
VPP_FLAGS += -g

FIR_KERNEL_VPP_FLAGS  := --hls.pre_tcl $(MAKEFILES_REPO)/hls_pre.tcl
FIR_KERNEL_VPP_FLAGS  += --hls.clock $(VPP_CLOCK_FREQ):$(FILTER_KERNEL_NAME)

MM2S_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(MM2S_KERNEL_NAME)
S2MM_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(S2MM_KERNEL_NAME)

VPP_LINK_FLAGS := --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(MM2S_KERNEL_NAME)_0
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(S2MM_KERNEL_NAME)_0
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(FILTER_KERNEL_NAME)_0

ifeq ($(EN_TRACE),1)
VPP_LINK_FLAGS += --profile.trace_memory DDR
VPP_LINK_FLAGS += --profile.data $(S2MM_KERNEL_NAME):$(S2MM_KERNEL_NAME)_0:s
VPP_LINK_FLAGS += --profile.data $(MM2S_KERNEL_NAME):$(MM2S_KERNEL_NAME)_0:s
VPP_LINK_FLAGS += --profile.data fir_dsp:fir_dsp_0:StreamIn
VPP_LINK_FLAGS += --profile.data fir_dsp:fir_dsp_0:StreamOut
endif

VPP_LINK_FLAGS += --vivado.synth.jobs 16
VPP_LINK_FLAGS += --vivado.impl.jobs  16

# =========================================================
# Regression Variables
# =========================================================

EMBEDDED_EXEC_SCRIPT = run_script.sh
EMBEDDED_EXEC_PATH   = $(MAKEFILES_REPO)/$(EMBEDDED_EXEC_SCRIPT)
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')
PERL ?= /tools/xgs/perl/5.8.5/bin/perl

RESULT_VAL ?= 
RUN_LEVEL  ?= 7

# =========================================================
# Packaging Flags
# =========================================================
PKG_FLAGS := -t $(TARGET)
PKG_FLAGS += --save-temps
PKG_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
PKG_FLAGS += -f $(PLATFORM)
PKG_FLAGS += --package.sd_dir $(PLATFORM_REPO_PATHS)/sw/versal/xrt
PKG_FLAGS += --package.rootfs $(XLNX_VERSAL)/rootfs.ext4
PKG_FLAGS += --package.kernel_image $(XLNX_VERSAL)/Image
PKG_FLAGS += --package.boot_mode=sd
PKG_FLAGS += --package.out_dir $(BUILD_TARGET_DIR)/package
PKG_FLAGS += --package.image_format=ext4
PKG_FLAGS += --package.sd_file $(APP_ELF) $(XCLBIN)
PKG_FLAGS += --package.sd_file $(EMBEDDED_EXEC_PATH)

ifeq ($(EN_TRACE),1)
PKG_FLAGS += --package.sd_file $(MAKEFILES_REPO)/xrt.ini
endif


# =========================================================
# Step 1. Kernel XO File Generation
# =========================================================
# This step compiles the HLS C PL kernels. 
# Outputs: in build/[hw_emu | hw]/ directory
# 	[fir_dsp | mm2s | s2mm].[hw_emu | hw].xo  
#	[fir_dsp | mm2s | s2mm].[hw_emu | hw].xo.compile_summary  
#	v++_[fir_dsp | mm2s | s2mm].[hw_emu | hw].log
#	_x
kernels: $(FILTER_KERNEL_XO)	\
	 $(S2MM_KERNEL_XO)	\
	 $(MM2S_KERNEL_XO)

$(FILTER_KERNEL_XO): $(FILTER_KERNEL_SRC)   $(FILTER_KERNEL_H)
	python create_fir_coefs.py -dsp  $(N_FIR_FILTERS)  $(N_FIR_TAPS)  $(IMPULSE_AREA)   > $(SYSTEM_SETTINGS_H)
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(FIR_KERNEL_VPP_FLAGS)  $(VPP_FLAGS) -c -k $(FILTER_KERNEL_NAME) $(FILTER_KERNEL_SRC) -o $@

$(S2MM_KERNEL_XO): $(S2MM_KERNEL_SRC)
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(S2MM_KERNEL_VPP_FLAGS) $(VPP_FLAGS) -c -k $(S2MM_KERNEL_NAME)   $(S2MM_KERNEL_SRC) -o $@   

$(MM2S_KERNEL_XO): $(MM2S_KERNEL_SRC)
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(MM2S_KERNEL_VPP_FLAGS) $(VPP_FLAGS) -c -k $(MM2S_KERNEL_NAME)   $(MM2S_KERNEL_SRC) -o $@   

# =========================================================
# Step 2. XCLBIN File Generation
# =========================================================
# This step links the kernels into a xclbin file. 
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_dsp_fir.[hw_emu | hw].xclbin
#	vck190_dsp_fir.[hw_emu | hw].xclbin.info
#	vck190_dsp_fir.[hw_emu | hw].xclbin.link_summary
#	vck190_dsp_fir.[hw_emu | hw].xsa
#	vck190_dsp_fir.[hw_emu | hw].log
xclbin: $(XCLBIN)

$(XCLBIN): $(S2MM_KERNEL_XO)	\
	   $(MM2S_KERNEL_XO)	\
           $(FILTER_KERNEL_XO)
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) --config $(MAKEFILES_REPO)/system.cfg -t $(TARGET) -o $@ 	\
		$(S2MM_KERNEL_XO)	\
		$(MM2S_KERNEL_XO)	\
		$(FILTER_KERNEL_XO)

# =========================================================
# Step 3. A72 Application Executable File Generation
# =========================================================
# This step compiles the A72 application. This step is the  
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
#	fir_dsp_app.o 
# 	fir_dsp_xrt.elf
application:
	$(HOST_CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_DIR)/fir_dsp_app.o $(GCC_LIB_INC) $(GCC_LIB)
	$(HOST_CXX) $(BUILD_DIR)/fir_dsp_app.o $(GCC_LIB_INC) $(GCC_LIB) -o $(APP_ELF)

# =========================================================
# Step 4. Package Generation  
# =========================================================
# This step generates the package folder which contains the 
# ./launch_hw_emu.sh script to launch hardware emulation 
# if TARGET=hw_emu and the sd_card.img file.  
# Outputs: in build/[hw_emu | hw]/ directory 
# 	a.xclbin
# 	package/ directory
# 	v++.package_summary
# 	v++_package.log 
package:
	cd $(BUILD_TARGET_DIR);	\
	v++ -p $(PKG_FLAGS)

# =========================================================
# Step 5. Run Hardware Emulation  
# =========================================================
# If the target is for HW_EMU, launch the emulator
# If the target is for HW, you'll have to follow the 
# instructions in the README.md 
run_emu: 
ifeq ($(TARGET),hw_emu)
	cd $(BUILD_TARGET_DIR)/package;	\
	$(PERL) $(XF_PROJ_ROOT)/ext/make_utility/embedded_run_emulation.pl "$(BUILD_TARGET_DIR)/package/launch_hw_emu.sh -no-reboot" "./$(EMBEDDED_EXEC_SCRIPT)" "$(RESULT_VAL)" "$(RUN_LEVEL)" | tee embedded_run.log 
	./launch_hw_emu.sh
else
	@echo ""
	@echo "Hardware build, no emulation executed."
	mkdir -p $(RUN_DIR); \
	mv -f $(BUILD_TARGET_DIR)/package/sd_card.img $(RUN_DIR)/
	zip -j $(RUN_DIR)/$(BUILD_VERSION)_sd_card.zip $(RUN_DIR)/sd_card.img
	rm $(RUN_DIR)/sd_card.img
	@echo ""
	@echo "**************************************************"
	@echo "SD Card Image is located in:"
	@echo "    $(RUN_DIR)"
	@echo "Go to the run directory and follow the steps in README.md for execution on board."
	@echo ""
endif

# =========================================================
# Step 6. Report Utilization Metrics
# =========================================================
# If the target is HW, this generates the power and resource
# utilization metrics.
report_metrics:
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"
else
	mkdir -p $(REPORT_DIR)
	vivado -mode batch -source report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
	mv utilization.txt $(REPORT_DIR)/$(BUILD_VERSION)_utilization.txt
	mv power.txt       $(REPORT_DIR)/$(BUILD_VERSION)_power.txt	
endif


###########################################################

# =========================================================
# Primary Build Targets
# ==> build
# ==> run
# ==> all
# ==> clean
# ==> cleanall
# =========================================================

#Build the design without running the hardware emulation
build: kernels xclbin application package

#Build the design and then run hardware emulation 
run: build run_emu

#Same as "run" above, but include metrics generation
all: build report_metrics run_emu

#Clean generated files
clean:
	rm -rf .Xil/
	rm -rf $(BUILD_DIR)
	rm -rf $(RUN_DIR)
	rm -rf vivado*

cleanall:
	rm -rf $(BASE_BUILD_DIR)
	rm -rf $(BASE_RUN_DIR)
	rm -rf vivado* .Xil ../vivado* ../.Xil *.log

