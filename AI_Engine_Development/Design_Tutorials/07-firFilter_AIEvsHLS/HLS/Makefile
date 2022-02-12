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
	@echo  "  make sd_card"
	@echo  "  Command calls rules: kernels xclbin application package, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make run"
	@echo  "  Command calls rules: sd_card run_emu, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make all"
	@echo  "  Command calls rules: sd_card report_metrics run_emu, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make kernels"
	@echo  "  Command to generate the PL kernels"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make graph"
	@echo  "  Command to generate the AI Engine graph"
	@echo  ""
	@echo  "  make aiesim"
	@echo  "  Command to run aie simulator for the AI Engine graph"
	@echo  ""
	@echo  "  make aiesim_profile"
	@echo  "  Command to run aie simulator for the AI Engine graph and enable kernel print statements"
	@echo  ""
	@echo  "  make vcd"
	@echo  "  Command to run aie simulator for the AI Engine graph and create the vcd and xpe file for the same"
	@echo  ""
	@echo  "  make xclbin"
	@echo  "  Command to generate the xclbin for specified target"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make application"
	@echo  "  Command to generate the A72 application"
	@echo  ""
	@echo  "  make package"
	@echo  "  Command to package design into an sd_card for specified target"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make run_emu"
	@echo  "  Command to run the hw emulator if LAUNCH_HW_EMU_EXEC variable is set to 1"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make report_metrics"
	@echo  "  Command to run and get vivado utilisation and power reports."
	@echo  "  Works only for TARGET=hw"
	@echo  ""
	@echo  "  make clean_target"
	@echo  "  Command to remove the generated files for a specific target"
	@echo  ""
	@echo  "  make clean_firFilters_dir"
	@echo  "  Command to remove the generated files for a specific no. of firs"
	@echo  ""
	@echo  "  make clean_firTaps_dir"
	@echo  "  Command to remove the generated files for a specific no. of fir taps"
	@echo  ""
	@echo  "  make cleanall"
	@echo  "  Command to remove the generated files for all variations"
	@echo  ""
	@echo  "  make clean_vivado_reports"
	@echo  "  Command to remove the generated vivado reports files for all variations"
	@echo  ""
	@echo  "  make cleanall_reports"
	@echo  "  Command to remove all the generated reports for all variations"
	@echo  ""
	@echo  " OPTIONS:"
	@echo  " Use the make recipes with required values for options mentioned below-"
	@echo  "    TARGET             -specifies target: Hardware or Emulation. Values: hw_emu(default), hw"
	@echo  "    N_FIR_FILTERS      -specifies number of FIR Filters in cascade chain. Values: 1(default) and 10"
	@echo  "    N_FIR_TAPS         -specifies number of FIR Filter Taps. Values: 15(default), 64, 129 and 240"
	@echo  "    PL_FREQ        	 -specifies HLS kernel frequencies. Values: 300Mhz(default)"
	@echo  "    ITER_CNT           -specifies design iteration count: Values: 8(default)"
	@echo  "    EN_TRACE           -specifies whether to enable profiling for the design. Values: 0(disabled, default) and 1(enabled)"
	@echo  "    LAUNCH_HW_EMU_EXEC -specifies whether to launch hw emulator when running make run_emu rule. Values: 0(disabled, default) and 1(enabled)"
	@echo  ""

# Print all options passed to Makefile
print-%  : ; @echo $* = $($*)

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# FIR Chain Parameters:
# N_FIR_FILTERS : Number for FIR filters in chain
# N_FIR_TAPS    : Number of taps in each filter
# =========================================================
N_FIR_FILTERS :=   1
N_FIR_TAPS    :=  15

# =========================================================
# PL Frequency in MHz:
# 300 MHz(default for datamover)
# 500 MHz(default for fir kernel)
# =========================================================
PL_FREQ        := 300
VPP_CLOCK_FREQ := $(PL_FREQ)000000

FIR_PL_FREQ        := 500
FIR_VPP_CLOCK_FREQ := $(FIR_PL_FREQ)000000

# =========================================================
# Iteration Count:
# 8(default)
# =========================================================
ITER_CNT := 8

# =========================================================
# Profiling and Trace Switches
# =========================================================
EN_TRACE := 0

######################################
# When TARGET=hw_emu, hw_emu launch...
# 0 = Disabled (Default).
# 1 = Enabled.
######################################
LAUNCH_HW_EMU_EXEC := 0

# =========================================================
# FIR Impulse Response Area
# =========================================================
IMPULSE_AREA := 16383

# =========================================================
# Source directories
# =========================================================
#Relative FIR filter directory
RELATIVE_PROJECT_DIR := ./

#Absolute FIR filter directory = <user path>/Tutorials/AI_Engine/filter_AIEvsDSP
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

DIRECTIVES_REPO        := $(DESIGN_REPO)/directives
SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
EXEC_SCRIPTS_REPO      := $(DESIGN_REPO)/exec_scripts
PYTHON_SCRIPTS_REPO    := $(DESIGN_REPO)/python_scripts

BASE_BLD_DIR := $(PROJECT_REPO)/build
FIR_TAPS_BLD_DIR := $(BASE_BLD_DIR)/fir_$(N_FIR_TAPS)_taps
FIR_FILTERS_DIR  := $(FIR_TAPS_BLD_DIR)/x$(N_FIR_FILTERS)_firs
BUILD_TARGET_DIR := $(FIR_FILTERS_DIR)/$(TARGET)

VIVADO_REPORTS_REPO := $(PROJECT_REPO)/vivado_reports_dir
VIVADO_BLD_REPORTS_DIR := $(REPORTS_REPO)/fir_$(N_FIR_TAPS)_taps/x$(N_FIR_FILTERS)_firs

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

###########################################################
# Variable Definitions...
#
# =========================================================
# Kernel Source Files repository
# =========================================================
DATAMOVER_KERNEL_NAME := datamover
FILTER_KERNEL_NAME    := fir_hls

DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_NAME).cpp

SYSTEM_SETTINGS_H := $(PL_SRC_REPO)/system_settings_comm.h
FILTER_KERNEL_H   := $(PL_SRC_REPO)/$(FILTER_KERNEL_NAME).h
FILTER_KERNEL_SRC := $(PL_SRC_REPO)/$(FILTER_KERNEL_NAME).cpp

DATAMOVER_KERNEL_NAME := datamover
DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_NAME).cpp
# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP := $(HOST_APP_SRC)/fir_hls_app.cpp

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
GCC_INC_FLAGS += -I$(HOST_APP_SRC)/
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

FIR_KERNEL_VPP_FLAGS  := --hls.pre_tcl $(DIRECTIVES_REPO)/hls_pre.tcl
FIR_KERNEL_VPP_FLAGS  += --hls.clock $(FIR_VPP_CLOCK_FREQ):$(FILTER_KERNEL_NAME)
FIR_KERNEL_VPP_FLAGS  += -D N_FIR_FILTERS=$(N_FIR_FILTERS)
FIR_KERNEL_VPP_FLAGS  += -D N_FIR_TAPS=$(N_FIR_TAPS)

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)
DATAMOVER_KERNEL_VPP_FLAGS += -D N_FIR_TAPS=$(N_FIR_TAPS)
DATAMOVER_KERNEL_VPP_FLAGS += -D N_FIR_FILTERS=$(N_FIR_FILTERS)


VPP_LINK_FLAGS := --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)_0
VPP_LINK_FLAGS += --clock.freqHz $(FIR_VPP_CLOCK_FREQ):$(FILTER_KERNEL_NAME)_0
VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/system.cfg
VPP_LINK_FLAGS += -t $(TARGET)

ifeq ($(EN_TRACE),1)
   VPP_LINK_FLAGS += --profile.trace_memory DDR
   VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_NAME):$(DATAMOVER_KERNEL_NAME)_0:all
   VPP_LINK_FLAGS += --profile.data fir_hls:fir_hls_0:all

endif

VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD}={16}

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
PKG_FLAGS += --package.sd_file $(EXEC_SCRIPTS_REPO)/$(EMBEDDED_EXEC_SCRIPT)

ifneq ($(EN_TRACE),0)
   PKG_FLAGS += --package.sd_file $(PROFILING_CONFIGS_REPO)/xrt.ini

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
	#python $(PYTHON_SCRIPTS_REPO)/create_fir_coefs.py -hls $(N_FIR_FILTERS) $(N_FIR_TAPS) $(IMPULSE_AREA) > $(SYSTEM_SETTINGS_H)
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

$(XCLBIN): $(DATAMOVER_KERNEL_XO)	\
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
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) -timeout -10800 | tee embedded_run.log

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
	rm -rf $(BLD_VIVADO_REPORTS_DIR)
	mkdir -p $(BLD_VIVADO_REPORTS_DIR)
	cd $(BLD_VIVADO_REPORTS_DIR); \
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr

endif

###########################################################

# =========================================================
# Primary Build Targets
# ==> sd_card
# ==> run
# ==> all
# ==> clean_target
# ==> clean_firFilters_dir
# ==> clean_firTaps_dir
# ==> cleanall
# ==> clean_vivado_reports
# ==> cleanall_reports
# =========================================================

#Build the design without running the hardware emulation

sd_card: kernels xclbin application package

#Build the design and then run hardware emulation 
run: sd_card run_emu

#Same as "run" above, but include metrics generation
all: sd_card report_metrics run_emu

#Clean target dir files...
clean_target:
	@echo "Cleaning Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

#Clean fir dir files...
clean_firFilters_dir:
	@echo "Cleaning $(N_FIR_FILTERS) FIR FILTERS Build Dir..."
	rm -rf $(FIR_FILTERS_DIR)

#Clean fir taps dir files...
clean_firTaps_dir:
	@echo "Cleaning FIR $(N_FIR_TAPS) TAPS Build Dir..."
	rm -rf $(FIR_TAPS_BLD_DIR)

#Clean all builds...
cleanall:
	rm -rf $(BASE_BLD_DIR)
	rm -rf vivado* .Xil *.log vitis* core.*

#Clean_all vivado reports...
clean_vivado_reports:
	rm -rf $(VIVADO_REPORTS_REPO)

#Clean_all reports...
cleanall_reports: clean_vivado_reports
