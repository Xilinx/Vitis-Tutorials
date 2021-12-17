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
	@echo  "        FIR_WINDOW_SIZE        -specifies AIE buffer window size. Default=256"
	@echo  ""
	
# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET          := hw_emu

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
N_FIR_FILTERS   :=    1
N_FIR_TAPS      :=   15
FIR_WINDOW_SIZE :=  256

# =========================================================
# Profiling and Trace Switches
# =========================================================
EN_TRACE        := 0

# =========================================================
# PL Frequency in MHz:
# 300 MHz(default)
# =========================================================
PL_FREQ        := 250
VPP_CLOCK_FREQ := $(PL_FREQ)000000

# =========================================================
# FIR Impulse Response Area
# =========================================================
IMPULSE_AREA   := 16383

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

BASE_BUILD_DIR   := $(PROJECT_REPO)/build
BUILD_DIR        := $(BASE_BUILD_DIR)/$(BUILD_VERSION)
WORK_DIR         := Work
BUILD_TARGET_DIR := $(BUILD_DIR)/$(TARGET)

BASE_RUN_DIR := $(PROJECT_REPO)/run_dir
RUN_DIR      := $(BASE_RUN_DIR)/$(BUILD_VERSION)

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
DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_NAME).cpp

# =========================================================
# Graph Source files repository
# =========================================================
GRAPH_SRC_CPP    := $(AIE_SRC_REPO)/fir_aie_graph.cpp
GRAPH_SETTINGS_H := $(AIE_SRC_REPO)/system_settings.h

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP     := $(APP_SRC_REPO)/fir_aie_app.cpp
AIE_CONTROL_CPP := $(BUILD_TARGET_DIR)/Work/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# Below are the names for SDF graph, application executable,
# kernel executables, and xclbin
# =========================================================
LIBADF_A := $(BUILD_TARGET_DIR)/libadf.a
VCD_FILE := $(BUILD_TARGET_DIR)/$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps.vcd
XCLBIN   := $(BUILD_TARGET_DIR)/vck190_aie_fir.$(TARGET).xclbin
APP_ELF  := $(BUILD_TARGET_DIR)/fir_aie_xrt.elf
DATAMOVER_KERNEL_XO := $(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_NAME).$(TARGET).xo

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/src/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/include/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L2/include/aie
AIE_FLAGS += -include=$(AIE_SRC_REPO)
AIE_FLAGS += --platform=$(PLATFORM)
AIE_FLAGS += --workdir=$(WORK_DIR)
AIE_FLAGS += --Xpreproc="-DN_FIR_FILTERS=$(N_FIR_FILTERS)"
AIE_FLAGS += --Xpreproc="-DN_FIR_TAPS=$(N_FIR_TAPS)"
AIE_FLAGS += --Xpreproc="-DFIR_WINDOW_SIZE=$(FIR_WINDOW_SIZE)"

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
GCC_FLAGS += -DITER_CNT=$(ITER_CNT)
GCC_FLAGS += -DN_FIR_FILTERS=$(N_FIR_FILTERS)
GCC_FLAGS += -DN_FIR_TAPS=$(N_FIR_TAPS)
GCC_FLAGS += -DFIR_WINDOW_SIZE=$(FIR_WINDOW_SIZE)

#ifeq ($(TARGET),hw_emu)
#GCC_FLAGS += -DUSE_SMALL_DATASET
#endif

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt/
GCC_INC_FLAGS += -I$(XILINX_VITIS)/aietools/include/
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)/inc
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)/src
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)/xrt_inputs
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/src/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/include/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L2/include/aie


GCC_INC_LIB += -L$(SDKTARGETSYSROOT)/usr/lib
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

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)
DATAMOVER_KERNEL_VPP_FLAGS += -D N_FIR_TAPS=$(N_FIR_TAPS)

VPP_LINK_FLAGS := --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)_0

ifeq ($(EN_TRACE),1)
   VPP_LINK_FLAGS += --profile.trace_memory DDR
   VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_NAME):$(DATAMOVER_KERNEL_NAME)_0:all
   VPP_LINK_FLAGS += --profile.data ai_engine_0.DataIn
   VPP_LINK_FLAGS += --profile.data ai_engine_0.DataOut
endif

# =========================================================
# Packaging Flags
# =========================================================
PKG_FLAGS := -t $(TARGET)
PKG_FLAGS += --save-temps
PKG_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
PKG_FLAGS += -f $(PLATFORM)
PKG_FLAGS += --package.rootfs $(XLNX_VERSAL)/rootfs.ext4
PKG_FLAGS += --package.kernel_image $(XLNX_VERSAL)/Image
PKG_FLAGS += --package.boot_mode=sd
PKG_FLAGS += --package.out_dir $(EMBEDDED_PACKAGE_OUT)
PKG_FLAGS += --package.image_format=ext4
PKG_FLAGS += --package.sd_file $(APP_ELF) $(XCLBIN) $(LIBADF_A)
PKG_FLAGS += --package.sd_file $(DESIGN_REPO)/$(EMBEDDED_EXEC_SCRIPT)

ifeq ($(EN_TRACE),1)

   PKG_FLAGS += --package.sd_file $(PROJECT_REPO)/xrt.ini

endif

ifdef XRT_ROOT

   PKG_FLAGS += --package.sd_dir $(XRT_ROOT)

endif

PKG_FLAGS += --package.defer_aie_run


# =========================================================
# Step 1. Kernel XO File Generation
# =========================================================
# This step compiles the HLS C PL kernels.
# Outputs: in build/[hw_emu | hw]/ directory
# 	[datamover].[hw_emu | hw].xo 
#	[datamover].[hw_emu | hw].xo.compile_summary 
#	v++_[datamover].[hw_emu | hw].log
#	_x
kernels: $(DATAMOVER_KERNEL_XO)

$(DATAMOVER_KERNEL_XO):
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(DATAMOVER_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k $(DATAMOVER_KERNEL_NAME) \
		$(DATAMOVER_KERNEL_SRC) -o $@

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
	python $(DESIGN_REPO)/create_fir_coefs.py -aie $(N_FIR_FILTERS) $(N_FIR_TAPS) $(IMPULSE_AREA) $(FIR_WINDOW_SIZE) > $(GRAPH_SETTINGS_H)
	mkdir -p $(BUILD_TARGET_DIR); 	\
	cd $(BUILD_TARGET_DIR); 	\
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP)

py_test:
	echo "$(N_FIR_FILTERS) FIR Filters in Graph; each with $(N_FIR_TAPS) Taps"
	python $(DESIGN_REPO)/create_fir_coefs.py -aie $(N_FIR_FILTERS) $(N_FIR_TAPS) $(IMPULSE_AREA) $(FIR_WINDOW_SIZE) > $(GRAPH_SETTINGS_H)
	more $(GRAPH_SETTINGS_H)

aiesim: $(LIBADF_A)
	#mkdir -p $(BUILD_TARGET_DIR);
	cd $(BUILD_TARGET_DIR); \
	aiesimulator --pkg-dir $(WORK_DIR)/ 2>&1 | tee -a aiesim.log
	#aiesimulator --pkg-dir $(WORK_DIR)/ --profile 2>&1 | tee -a aiesim.log

vcd: $(VCD_FILE) vcd_util_n_power

vcd_util_n_power: $(VCD_FILE)
	cd $(BUILD_TARGET_DIR); \
	vcdanalyze --vcd $(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps.vcd --xpe

$(VCD_FILE): $(LIBADF_A)
	cd $(BUILD_TARGET_DIR); \
	aiesimulator --pkg-dir $(WORK_DIR)/ --dump-vcd $(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps 2>&1 | tee -a vcd.log

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

$(XCLBIN): $(DATAMOVER_KERNEL_XO) $(LIBADF_A)
	cd $(BUILD_TARGET_DIR);		\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) --config $(DESIGN_REPO)/system.cfg -t $(TARGET) -o $@ 	\
		$(DATAMOVER_KERNEL_XO) $(LIBADF_A)

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
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control.o
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/fir_aie_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/fir_aie_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(APP_ELF)

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
# ==> sd_card
# ==> run
# ==> all
# ==> clean
# ==> cleanall
# =========================================================

#Build the design without running the hardware emulation

sd_card: kernels graph xclbin application package

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

