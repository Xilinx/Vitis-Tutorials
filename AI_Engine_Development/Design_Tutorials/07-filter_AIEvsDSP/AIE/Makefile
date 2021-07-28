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
	@echo  "        FIR_WINDOW_SIZE        -specifies AIE buffer window size. Default=1024"
	@echo  ""
	

print-%  : ; @echo $* = $($*)

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET          := hw_emu

# =========================================================
# FIR Chain Parameters:
# N_FIR_FILTERS : Number for FIR filters in chain
# N_FIR_TAPS    : Number of taps in each filter
# =========================================================
N_FIR_FILTERS   :=    1
N_FIR_TAPS      :=   15
FIR_WINDOW_SIZE :=  256

# =========================================================
# Profiling and Trace Switches
# =========================================================
EN_TRACE        := 0

# =========================================================
# Platform
# =========================================================
PLATFORM        := xilinx_vck190_base_202110_1

# =========================================================
# PL Frequency in MHz:
# 300 MHz(default)
# =========================================================
PL_FREQ        := 300
VPP_CLOCK_FREQ := $(PL_FREQ)000000

# =========================================================
# FIR Impulse Response Area
# =========================================================
IMPULSE_AREA   := 16383

# =========================================================
# Choose the compiler based on the A72 Processor in the
# Versal device
# =========================================================
export LD_LIBRARY_PATH := $(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/lib64:$(LD_LIBRARY_PATH)
CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++

# =========================================================
# Choose the XRT and XAIENGINE
# =========================================================
XLNX_VERSAL	:= $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal
SYSROOT_PATH	:= $(XLNX_VERSAL)/sysroots/aarch64-xilinx-linux
XRT_PATH 	:= $(SYSROOT_PATH)/usr/include/xrt

# =========================================================
# Source directory
# =========================================================
#Version Name
BUILD_VERSION   := fir_aie_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps

#relative fir directory
RELATIVE_PROJECT_DIR := ./

#absolute fir directory = <user path>/Tutorials/AI_Engine/fir
PROJECT_REPO	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO	:= $(PROJECT_REPO)/design
AIE_SRC_REPO 	:= $(PROJECT_REPO)/design/aie_src
APP_SRC_REPO	:= $(PROJECT_REPO)/design/app_src
PL_SRC_REPO 	:= $(PROJECT_REPO)/design/pl_src
MAKEFILES_REPO	:= $(PROJECT_REPO)

BASE_BUILD_DIR	:= $(PROJECT_REPO)/build
BUILD_DIR 	:= $(BASE_BUILD_DIR)/$(BUILD_VERSION)
WORK_DIR	:= Work
BUILD_TARGET_DIR := $(BUILD_DIR)/$(TARGET)

BASE_RUN_DIR	:= $(PROJECT_REPO)/run_dir
RUN_DIR	        := $(BASE_RUN_DIR)/$(BUILD_VERSION)

REPORT_DIR      := $(PROJECT_REPO)/../report_dir

# =========================================================
# Kernel Source Files repository
# =========================================================
S2MM_KERNEL_NAME := s2mm
MM2S_KERNEL_NAME := mm2s
S2MM_KERNEL_SRC	 := $(PL_SRC_REPO)/$(S2MM_KERNEL_NAME).cpp
MM2S_KERNEL_SRC	 := $(PL_SRC_REPO)/$(MM2S_KERNEL_NAME).cpp

# =========================================================
# Graph Source files repository
# =========================================================
GRAPH_SRC_CPP    := $(AIE_SRC_REPO)/fir_aie_graph.cpp
GRAPH_SETTINGS_H := $(AIE_SRC_REPO)/system_settings.h

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP 	 := $(APP_SRC_REPO)/fir_aie_app.cpp
AIE_CONTROL_CPP  := $(BUILD_DIR)/Work/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# Below are the names for SDF graph, application executable,
# kernel executables, and xclbin
# =========================================================
LIBADF_A  	:= $(BUILD_DIR)/libadf.a
S2MM_KERNEL_XO 	:= $(BUILD_TARGET_DIR)/$(S2MM_KERNEL_NAME).$(TARGET).xo
MM2S_KERNEL_XO 	:= $(BUILD_TARGET_DIR)/$(MM2S_KERNEL_NAME).$(TARGET).xo
XCLBIN 		:= $(BUILD_TARGET_DIR)/vck190_aie_fir.$(TARGET).xclbin
APP_ELF 	:= $(BUILD_TARGET_DIR)/fir_aie_xrt.elf

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/src/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/include/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L2/include/aie
AIE_FLAGS += -include=$(AIE_SRC_REPO)
AIE_FLAGS += --platform=$(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
AIE_FLAGS += --workdir=$(WORK_DIR)

AIE_FLAGS += --log-level=5
AIE_FLAGS += --pl-freq=$(PL_FREQ)
AIE_FLAGS += --verbose
AIE_FLAGS += --dataflow

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__
GCC_FLAGS += -D__PS_ENABLE_AIE__
GCC_FLAGS += -DXAIE_DEBUG

ifeq ($(TARGET),hw_emu)
GCC_FLAGS += -DUSE_SMALL_DATASET
endif

GCC_INC_FLAGS := -I$(XRT_PATH)
GCC_INC_FLAGS += -I$(XILINX_VITIS)/aietools/include/
GCC_INC_FLAGS += -I$(SYSROOT_PATH)/usr/include
GCC_INC_FLAGS += -I$(SYSROOT_PATH)/usr/lib
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)/inc
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)/src
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)/xrt_inputs
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/src/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/include/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L2/include/aie


GCC_INC_LIB := --sysroot=$(SYSROOT_PATH)
GCC_INC_LIB += -L$(SYSROOT_PATH)/usr/lib
GCC_INC_LIB += -L$(XILINX_VITIS)/aietools/lib/aarch64.o
GCC_INC_LIB += -L$(XILINX_VITIS)/aietools/lib/lnx64.o

GCC_LIB := -ladf_api_xrt
GCC_LIB += -lxrt_coreutil

# =========================================================
# Kernel Compiler and Linker Flags
# =========================================================
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose
VPP_FLAGS += -g

MM2S_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(MM2S_KERNEL_NAME)
S2MM_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(S2MM_KERNEL_NAME)

VPP_LINK_FLAGS := --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(MM2S_KERNEL_NAME)_0
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(S2MM_KERNEL_NAME)_0

ifeq ($(EN_TRACE),1)
VPP_LINK_FLAGS += --profile.trace_memory DDR
VPP_LINK_FLAGS += --profile.data $(S2MM_KERNEL_NAME):$(S2MM_KERNEL_NAME)_0:s
VPP_LINK_FLAGS += --profile.data $(MM2S_KERNEL_NAME):$(MM2S_KERNEL_NAME)_0:s
VPP_LINK_FLAGS += --profile.data ai_engine_0.DataIn
VPP_LINK_FLAGS += --profile.data ai_engine_0.DataOut
endif

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
LDCLFLAGS=

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
PKG_FLAGS += --package.sd_file $(APP_ELF) $(XCLBIN) $(LIBADF_A)
PKG_FLAGS += --package.sd_file $(EMBEDDED_EXEC_PATH)
PKG_FLAGS += --package.defer_aie_run

ifeq ($(EN_TRACE),1)
PKG_FLAGS += --package.sd_file $(MAKEFILES_REPO)/xrt.ini
endif


# =========================================================
# Step 1. Kernel XO File Generation
# =========================================================
# This step compiles the HLS C PL kernels.
# Outputs: in build/[hw_emu | hw]/ directory
# 	[mm2s | s2mm].[hw_emu | hw].xo 
#	[mm2s | s2mm].[hw_emu | hw].xo.compile_summary 
#	v++_[mm2s | s2mm].[hw_emu | hw].log
#	_x
kernels: $(S2MM_KERNEL_XO) 	\
	 $(MM2S_KERNEL_XO)

$(S2MM_KERNEL_XO): $(S2MM_KERNEL_SRC)
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(S2MM_KERNEL_VPP_FLAGS) $(VPP_FLAGS) -c -k $(S2MM_KERNEL_NAME) $(S2MM_KERNEL_SRC) -o $@   

$(MM2S_KERNEL_XO): $(MM2S_KERNEL_SRC)
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(MM2S_KERNEL_VPP_FLAGS) $(VPP_FLAGS) -c -k $(MM2S_KERNEL_NAME) $(MM2S_KERNEL_SRC) -o $@   

# =========================================================
# Step 2. AI Engine SDF Graph File and Work/ Directory
#         (containing the Graph Executable) Generation
# =========================================================
# This step creates an SDF Graph and the Work/ directory.
# The Work/ directory contains the graph executable
# (fir.o) which is used in the make xclbin step. 
# The aiecompiler is invoked with the -target=hw.
# Outputs: in build/ directory
#	libsdf.a
#	NOC_Power.xpe
#	Work/
#	xnwOut/
graph: $(LIBADF_A)

$(LIBADF_A): $(GRAPH_SRC_CPP)
	echo "$(N_FIR_FILTERS) FIR Filters in Graph; each with $(N_FIR_TAPS) Taps"
	python create_fir_coefs.py  -aie  $(N_FIR_FILTERS)  $(N_FIR_TAPS)   $(IMPULSE_AREA)   $(FIR_WINDOW_SIZE)   > $(GRAPH_SETTINGS_H)
	mkdir -p $(BUILD_DIR); 	\
	cd $(BUILD_DIR); 	\
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP)

py_test:
	echo "$(N_FIR_FILTERS) FIR Filters in Graph; each with $(N_FIR_TAPS) Taps"
	python create_fir_coefs.py  -aie  $(N_FIR_FILTERS)  $(N_FIR_TAPS)   $(IMPULSE_AREA)   $(FIR_WINDOW_SIZE)   > $(GRAPH_SETTINGS_H)
	more $(GRAPH_SETTINGS_H)

# =========================================================
# Step 3. XCLBIN File Generation
# =========================================================
# This step links the graph executable (fir_aie.o) and
# the kernels into a xclbin file.
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_aie_fir.[hw_emu | hw].xclbin
#	vck190_aie_fir.[hw_emu | hw].xclbin.info
#	vck190_aie_fir.[hw_emu | hw].xclbin.link_summary
#	vck190_aie_fir.[hw_emu | hw].xsa
#	vck190_aie_fir.[hw_emu | hw].log
xclbin: $(XCLBIN)

$(XCLBIN): $(S2MM_KERNEL_XO)	\
	   $(MM2S_KERNEL_XO)	\
	   $(LIBADF_A)
	cd $(BUILD_TARGET_DIR);		\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) --config $(MAKEFILES_REPO)/system.cfg -t $(TARGET) -o $@ 	\
		$(S2MM_KERNEL_XO)	\
		$(MM2S_KERNEL_XO)	\
		$(LIBADF_A)

# =========================================================
# Step 4. A72 Application Executable File Generation
# =========================================================
# This step compiles the A72 application. This step is the 
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# 	aie_control.o
#	fir_aie_app.o
# 	fir_aie_xrt.elf
application:
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_DIR)/app_control.o
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_DIR)/fir_aie_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_DIR)/app_control.o $(BUILD_DIR)/fir_aie_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(APP_ELF)

# =========================================================
# Step 5. Package Generation 
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
# Step 6. Run Hardware Emulation 
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
	mkdir -p $(RUN_DIR)
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
# Step 7. Report Utilization Metrics
# =========================================================
# If the target is HW, this generates the power and resource
# utilization metrics.
report_metrics:
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"
else
	mkdir -p $(REPORT_DIR)
	vivado -mode batch -source report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
	mv utilization.txt  $(REPORT_DIR)/$(BUILD_VERSION)_utilization.txt
	mv power.txt        $(REPORT_DIR)/$(BUILD_VERSION)_power.txt	
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

sd_card: kernels graph xclbin application package
build:   kernels graph xclbin application package

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

