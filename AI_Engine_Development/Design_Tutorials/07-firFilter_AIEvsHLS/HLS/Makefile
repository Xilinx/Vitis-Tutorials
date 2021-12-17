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

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET        := hw_emu

# =========================================================
# Iteration Count:
# 8(default)
# =========================================================
ITER_CNT := 8

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
# PL Frequency in MHz:
# 300 MHz(default)
# =========================================================
PL_FREQ        := 250
VPP_CLOCK_FREQ := $(PL_FREQ)000000

FIR_PL_FREQ        := 500
FIR_VPP_CLOCK_FREQ := $(FIR_PL_FREQ)000000

# =========================================================
# FIR Impulse Response Area
# =========================================================
IMPULSE_AREA	:= 16383

# =========================================================
# Source directory
# =========================================================
#Version Name
BUILD_VERSION   := fir_hls_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps

#relative FIR filter directory
RELATIVE_PROJECT_DIR := ./

#absolute FIR filter directory = <user path>/Tutorials/AI_Engine/filter_AIEvsDSP
PROJECT_REPO	 := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO 	:= $(PROJECT_REPO)/design
PL_SRC_REPO 	:= $(PROJECT_REPO)/design/pl_src
APP_SRC_REPO 	:= $(PROJECT_REPO)/design/app_src
MAKEFILES_REPO := $(PROJECT_REPO)

BASE_BUILD_DIR   := $(PROJECT_REPO)/build
BUILD_DIR        := $(BASE_BUILD_DIR)/$(BUILD_VERSION)
BUILD_TARGET_DIR := $(BUILD_DIR)/$(TARGET)

BASE_RUN_DIR := $(PROJECT_REPO)/run_dir
RUN_DIR		 := $(BASE_RUN_DIR)/$(BUILD_VERSION)

REPORT_DIR   := $(PROJECT_REPO)/../report_dir

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

######################################
# When TARGET=hw_emu, hw_emu launch...
# 0 = Disabled (Default).
# 1 = Enabled.
######################################
LAUNCH_HW_EMU_EXEC := 0

# Print all options passed to Makefile
print-%  : ; @echo $* = $($*)

# =========================================================
# Kernel Source Files repository
# =========================================================
DATAMOVER_KERNEL_NAME := datamover
FILTER_KERNEL_NAME    := fir_hls

DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_NAME).cpp

SYSTEM_SETTINGS_H := $(PL_SRC_REPO)/system_settings.h
FILTER_KERNEL_H   := $(PL_SRC_REPO)/$(FILTER_KERNEL_NAME).h
FILTER_KERNEL_SRC := $(PL_SRC_REPO)/$(FILTER_KERNEL_NAME).cpp

DATAMOVER_KERNEL_NAME := datamover
DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_NAME).cpp
# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP       := $(APP_SRC_REPO)/fir_hls_app.cpp

# =========================================================
# Below are the names, application executable,
# kernel executables, and xclbin
# =========================================================
FILTER_KERNEL_XO    := $(BUILD_TARGET_DIR)/fir_hls.$(TARGET).xo
DATAMOVER_KERNEL_XO := $(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_NAME).$(TARGET).xo
XCLBIN              := $(BUILD_TARGET_DIR)/vck190_hls_fir.$(TARGET).xclbin
APP_ELF             := fir_hls_xrt.elf

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__
GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -DITER_CNT=$(ITER_CNT)
GCC_FLAGS += -DN_FIR_FILTERS=$(N_FIR_FILTERS)
GCC_FLAGS += -DN_FIR_TAPS=$(N_FIR_TAPS)

#ifeq ($(TARGET),hw_emu)
#   GCC_FLAGS += -DUSE_SMALL_DATASET
#
#endif

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt/
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include/
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib/
GCC_INC_FLAGS += -I$(APP_SRC_REPO)/
GCC_INC_FLAGS += -I$(PL_SRC_REPO)/

GCC_LIB_INC += -L=$(SDKTARGETSYSROOT)/usr/lib

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

FIR_KERNEL_VPP_FLAGS  := --hls.pre_tcl $(DESIGN_REPO)/hls_pre.tcl
FIR_KERNEL_VPP_FLAGS  += --hls.clock $(FIR_VPP_CLOCK_FREQ):$(FILTER_KERNEL_NAME)
FIR_KERNEL_VPP_FLAGS  += -D N_FIR_FILTERS=$(N_FIR_FILTERS)
FIR_KERNEL_VPP_FLAGS  += -D N_FIR_TAPS=$(N_FIR_TAPS)

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)
DATAMOVER_KERNEL_VPP_FLAGS += -D N_FIR_TAPS=$(N_FIR_TAPS)

VPP_LINK_FLAGS := --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)_0
VPP_LINK_FLAGS += --clock.freqHz $(FIR_VPP_CLOCK_FREQ):$(FILTER_KERNEL_NAME)_0
VPP_LINK_FLAGS += --config $(DESIGN_REPO)/system.cfg
VPP_LINK_FLAGS += -t $(TARGET)

ifeq ($(EN_TRACE),1)
   VPP_LINK_FLAGS += --profile.trace_memory DDR
   VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_NAME):$(DATAMOVER_KERNEL_NAME)_0:all
   VPP_LINK_FLAGS += --profile.data fir_hls:fir_hls_0:all

endif

#VPP_LINK_FLAGS += --vivado.synth.jobs 16
#VPP_LINK_FLAGS += --vivado.impl.jobs  16

# =========================================================
# Packaging Flags
# ========================================================
PKG_FLAGS := -t $(TARGET)
PKG_FLAGS += --save-temps
PKG_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
PKG_FLAGS += -f $(PLATFORM)
PKG_FLAGS += --package.rootfs $(XLNX_VERSAL)/rootfs.ext4
PKG_FLAGS += --package.kernel_image $(XLNX_VERSAL)/Image
PKG_FLAGS += --package.boot_mode=sd
PKG_FLAGS += --package.out_dir $(EMBEDDED_PACKAGE_OUT)
PKG_FLAGS += --package.image_format=ext4
PKG_FLAGS += --package.sd_file $(APP_ELF) $(XCLBIN)
PKG_FLAGS += --package.sd_file $(DESIGN_REPO)/$(EMBEDDED_EXEC_SCRIPT)

ifneq ($(EN_TRACE),0)

   PKG_FLAGS += --package.sd_file $(DESIGN_REPO)/xrt.ini

endif

ifdef XRT_ROOT

   PKG_FLAGS += --package.sd_dir $(XRT_ROOT)

endif

# =========================================================
# Step 1. Kernel XO File Generation
# =========================================================
# This step compiles the HLS C PL kernels. 
# Outputs: in build/[hw_emu | hw]/ directory
# 	[fir_hls | datamover].[hw_emu | hw].xo 
#	[fir_hls | datamover].[hw_emu | hw].xo.compile_summary 
#	v++_[fir_hls | datamover].[hw_emu | hw].log
#	_x
kernels: $(FILTER_KERNEL_XO) \
         $(DATAMOVER_KERNEL_XO)

$(FILTER_KERNEL_XO): $(FILTER_KERNEL_SRC) $(FILTER_KERNEL_H)
	python $(DESIGN_REPO)/create_fir_coefs.py -hls $(N_FIR_FILTERS) $(N_FIR_TAPS) $(IMPULSE_AREA) > $(SYSTEM_SETTINGS_H)
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(FIR_KERNEL_VPP_FLAGS) $(VPP_FLAGS) -c -k $(FILTER_KERNEL_NAME) $(FILTER_KERNEL_SRC) -o $@

$(DATAMOVER_KERNEL_XO):
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(DATAMOVER_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k $(DATAMOVER_KERNEL_NAME) \
		$(DATAMOVER_KERNEL_SRC) -o $@

# =========================================================
# Step 2. XCLBIN File Generation
# =========================================================
# This step links the kernels into a xclbin file. 
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_hls_fir.[hw_emu | hw].xclbin
#	vck190_hls_fir.[hw_emu | hw].xclbin.info
#	vck190_hls_fir.[hw_emu | hw].xclbin.link_summary
#	vck190_hls_fir.[hw_emu | hw].xsa
#	vck190_hls_fir.[hw_emu | hw].log
xclbin: $(XCLBIN)

$(XCLBIN): $(S2MM_KERNEL_XO)	\
	   $(MM2S_KERNEL_XO)	\
      $(FILTER_KERNEL_XO)
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -o $@ \
		$(DATAMOVER_KERNEL_XO)	\
		$(FILTER_KERNEL_XO)

# =========================================================
# Step 3. A72 Application Executable File Generation
# =========================================================
# This step compiles the A72 application. This step is the  
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
#	fir_hls_app.o 
# 	fir_hls_xrt.elf
application:
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o fir_hls_app.o $(GCC_LIB_INC) $(GCC_LIB); \
	$(CXX) fir_hls_app.o $(GCC_LIB_INC) $(GCC_LIB) -o $(APP_ELF)

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
	@echo ""
	@echo "###########################################################################"
ifeq ($(LAUNCH_HW_EMU_EXEC),0)
	@echo "To Run Hardware Emulation Manually Goto:"
	@echo "$(EMBEDDED_PACKAGE_OUT)"
	@echo ""
	@echo "and do:"
	@echo "./launch_hw_emu.sh or ./launch_hw_emu.sh -g (for waveform viewer)..."
	@echo ""

else
	cd $(EMBEDDED_PACKAGE_OUT); \
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) -timeout 86400 | tee embedded_run.log

endif
else
	@echo ""
	@echo "###########################################################################"
	@echo "Hardware build, no emulation executed."
	@echo ""
	@echo "Use sd_card.img from below directory and, follow the steps in README.md for execution on board."
	@echo "$(EMBEDDED_PACKAGE_OUT)"
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
# ==> sd_card
# ==> run
# ==> all
# ==> clean
# ==> cleanall
# =========================================================

#Build the design without running the hardware emulation

sd_card: kernels xclbin application package

#Build the design and then run hardware emulation 
run: sd_card run_emu

#Same as "run" above, but include metrics generation
all: sd_card report_metrics run_emu

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

