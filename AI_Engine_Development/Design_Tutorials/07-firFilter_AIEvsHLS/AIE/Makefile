#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

%.PHONY: help
help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make sd_card"
	@echo  "  Command calls rules: kernels graph xsa application package, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make run"
	@echo  "  Command calls rules: sd_card run_emu, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make all"
	@echo  "  Command calls rules: sd_card report_metrics vcd run_emu, in sequence"
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
	@echo  "  make xsa"
	@echo  "  Command to generate the xsa for specified target"
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
	@echo  "  make clean_firWinSz_dir"
	@echo  "  Command to remove the generated files for a specific no. of firs"
	@echo  ""
	@echo  "  make clean_aiesPerFir_dir"
	@echo  "  Command to remove the generated files for a specific no. of firs"
	@echo  ""
	@echo  "  make cleanall_blds"
	@echo  "  Command to remove the generated build files for all variations"
	@echo  ""
	@echo  "  make cleanall_vivado_reports"
	@echo  "  Command to remove the generated vivado reports files for all variations"
	@echo  ""
	@echo  "  make cleanall_xpe_reports"
	@echo  "  Command to remove the generated xpe files for all variations"
	@echo  ""
	@echo  "  make cleanall_reports"
	@echo  "  Command to remove all the generated reports for all variations"
	@echo  ""
	@echo  "  make cleanall"
	@echo  "  Command to remove all the generated builds files  and reports for all variations"
	@echo  ""
	@echo  " OPTIONS:"
	@echo  " Use the make recipes with required values for options mentioned below-"
	@echo  "    TARGET             -specifies target: Hardware or Emulation. Values: hw_emu(default), hw"
	@echo  "    N_FIR_FILTERS      -specifies number of FIR Filters in cascade chain. Values: 1(default) and 10"
	@echo  "    N_FIR_TAPS         -specifies number of FIR Filter Taps. Values: 15(default), 64, 129 and 240"
	@echo  "    FIR_WINDOW_SIZE    -specifies AIE buffer window size. Values: 64, 256, 1024 and 2048(default)"
	@echo  "    N_AIES_PER_FIR     -specifies no. of AIEs Per FIR(kernel cascade length). Values: 1(default), 2 and 4"
	@echo  "    PL_FREQ        	 -specifies HLS kernel frequencies. Values: 300Mhz(default)"
	@echo  "    ITER_CNT           -specifies design iteration count: Values: 8(default)"
	@echo  "    EN_TRACE           -specifies whether to enable profiling for the design. Values: 0(disabled, default) and 1(enabled)"
	@echo  "    LAUNCH_HW_EMU_EXEC -specifies whether to launch hw emulator when running make run_emu rule. Values: 0(disabled, default) and 1(enabled)"
	@echo  ""

# Print all options passed to Makefile
print-%  : ; @echo $* = $($*)

# For SPRITE
DSPLIB_ROOT=$(DSPLIB_VITIS)/dsp

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

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
N_AIES_PER_FIR  :=    1
N_FIR_FILTERS   :=    1
N_FIR_TAPS      :=   15
FIR_WINDOW_SIZE :=  256
INPUT_SAMPLES   := 2048
N_AIE_ITERS     := $(shell echo $$(( $(INPUT_SAMPLES) * $(ITER_CNT) / $(FIR_WINDOW_SIZE) )))

# =========================================================
# PL Frequency in MHz:
# 300 MHz(default)
# =========================================================
PL_FREQ        := 300
VPP_CLOCK_FREQ := $(PL_FREQ)000000

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
IMPULSE_AREA   := 16383

# =========================================================
# Source directories
# =========================================================

#Relative fir directory
RELATIVE_PROJECT_DIR := ./

#Absolute fir directory = <user path>/Tutorials/AI_Engine/fir
PROJECT_REPO	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
AIE_SRC_REPO := $(DESIGN_REPO)/aie_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src

SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
EXEC_SCRIPTS_REPO      := $(DESIGN_REPO)/exec_scripts
PYTHON_SCRIPTS_REPO    := $(DESIGN_REPO)/python_scripts
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

BASE_BLD_DIR        := $(PROJECT_REPO)/build
FIR_TAPS_BLD_DIR    := $(BASE_BLD_DIR)/fir_$(N_FIR_TAPS)_taps
FIR_FILTERS_DIR     := $(FIR_TAPS_BLD_DIR)/x$(N_FIR_FILTERS)_firs
FIR_WINDOW_SIZE_DIR := $(FIR_FILTERS_DIR)/winSz_$(FIR_WINDOW_SIZE)
AIES_PER_FIR_DIR    := $(FIR_WINDOW_SIZE_DIR)/x$(N_AIES_PER_FIR)_aie_per_fir
BUILD_TARGET_DIR    := $(AIES_PER_FIR_DIR)/$(TARGET)

REPORTS_REPO := $(PROJECT_REPO)/reports_dir
BLD_REPORTS_DIR := $(REPORTS_REPO)/fir_$(N_FIR_TAPS)_taps/x$(N_FIR_FILTERS)_firs/winSz_$(FIR_WINDOW_SIZE)/x$(N_AIES_PER_FIR)_aie_per_fir

XPE_REPO := $(PROJECT_REPO)/xpe_dir
BLD_XPE_DIR := $(XPE_REPO)/fir_$(N_FIR_TAPS)_taps/x$(N_FIR_FILTERS)_firs/winSz_$(FIR_WINDOW_SIZE)/x$(N_AIES_PER_FIR)_aie_per_fir/$(TARGET)
VCD_FILE_NAME := fir_$(N_FIR_TAPS)_taps_x$(N_FIR_FILTERS)_firs_winSz_$(FIR_WINDOW_SIZE)_x$(N_AIES_PER_FIR)_aie_per_fir
BLD_TGT_VCD_FILE := $(BUILD_TARGET_DIR)/$(VCD_FILE_NAME).vcd
XPE_FILE := $(BLD_XPE_DIR)/graph_$(VCD_FILE_NAME).xpe

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

WORK_DIR := Work
AIESIM_DATA_DIR := $(AIE_SRC_REPO)/aiesim_data
AIESIM_INPUT_FILE := $(AIESIM_DATA_DIR)/input_impulse.txt

###########################################################
# Variable Definitions...
#
# =========================================================
# Kernel Source Files repository
# =========================================================
DATAMOVER_KERNEL_NAME := datamover
DATAMOVER_KERNEL_SRC  := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_NAME).cpp

# =========================================================
# Graph Source files repository
# =========================================================
GRAPH_SRC_CPP    := $(AIE_SRC_REPO)/fir_aie_graph.cpp
GRAPH_SETTINGS_H := $(AIE_SRC_REPO)/system_settings_comm.h

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP     := $(HOST_APP_SRC)/fir_aie_app.cpp
AIE_CONTROL_CPP := $(BUILD_TARGET_DIR)/Work/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# Below are the names for SDF graph, application executable,
# kernel executables, and xsa
# =========================================================
LIBADF_A := $(BUILD_TARGET_DIR)/libadf.a
XSA      := vck190_aie_fir.$(TARGET).xsa
APP_ELF  := fir_aie_xrt.elf
#APP_ELF_INF_RUN     := fir_aie_xrt_inf_run.elf
DATAMOVER_KERNEL_XO := $(DATAMOVER_KERNEL_NAME).$(TARGET)

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/src/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/include/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L2/include/aie
AIE_FLAGS += -include=$(AIE_SRC_REPO)
AIE_FLAGS += --platform=$(PLATFORM)
AIE_FLAGS += --workdir=$(WORK_DIR)
AIE_FLAGS += --Xpreproc="-DITER_CNT=$(ITER_CNT)"
AIE_FLAGS += --Xpreproc="-DN_FIR_FILTERS=$(N_FIR_FILTERS)"
AIE_FLAGS += --Xpreproc="-DN_FIR_TAPS=$(N_FIR_TAPS)"
AIE_FLAGS += --Xpreproc="-DN_AIES_PER_FIR=$(N_AIES_PER_FIR)"
AIE_FLAGS += --Xpreproc="-DFIR_WINDOW_SIZE=$(FIR_WINDOW_SIZE)"
AIE_FLAGS += --Xpreproc="-DN_AIE_ITERS=$(N_AIE_ITERS)"
#AIE_FLAGS += --Xpreproc="-DAIESIM_INPUT_FILE=$(AIESIM_INPUT_FILE)"

AIE_FLAGS += --Xmapper=BufferOptLevel9
AIE_FLAGS += --Xrouter=DMAFIFOsInFreeBankOnly

AIE_FLAGS += --log-level=5
AIE_FLAGS += --pl-freq=$(PL_FREQ)
AIE_FLAGS += --verbose
AIE_FLAGS += --dataflow

# Aiesimulator flags
#-i - Alias of --input-dir=<dir> option
#-o - Alias of --output-dir=<dir> option
AIE_SIM_FLAGS := --pkg-dir $(WORK_DIR)/
AIE_SIM_FLAGS += -i=$(AIE_SRC_REPO)/aiesim_data
# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++17
GCC_FLAGS += -D__linux__
GCC_FLAGS += -D__PS_ENABLE_AIE__
GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -DN_FIR_FILTERS=$(N_FIR_FILTERS)
GCC_FLAGS += -DN_FIR_TAPS=$(N_FIR_TAPS)
GCC_FLAGS += -DN_AIES_PER_FIR=$(N_AIES_PER_FIR)
GCC_FLAGS += -DFIR_WINDOW_SIZE=$(FIR_WINDOW_SIZE)

#ifeq ($(TARGET),hw_emu)
#GCC_FLAGS += -DUSE_SMALL_DATASET
#endif

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt/
GCC_INC_FLAGS += -I$(XILINX_VITIS)/aietools/include/
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)
GCC_INC_FLAGS += -I$(HOST_APP_SRC)
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
DATAMOVER_KERNEL_VPP_FLAGS += -D N_FIR_FILTERS=$(N_FIR_FILTERS)

VPP_LINK_FLAGS := --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_NAME)_0
VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/system.cfg

ifeq ($(EN_TRACE),1)
   ifeq ($(TARGET),hw)
      VPP_LINK_FLAGS += --profile.trace_memory DDR
      VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_NAME):$(DATAMOVER_KERNEL_NAME)_0:all
      VPP_LINK_FLAGS += --profile.data ai_engine_0.DataIn
      VPP_LINK_FLAGS += --profile.data ai_engine_0.DataOut
   
   endif
endif

#endif


# Enabling Multiple Strategies For Closing Timing...
#VPP_LINK_FLAGS += --vivado.impl.strategies "ALL"
#VPP_LINK_FLAGS += --vivado.impl.lsf '{bsub -R "select[type=X86_64] rusage[mem=65536]" -N -q medium}'
#VPP_LINK_FLAGS += --advanced.param "compiler.enableMultiStrategies=1"
#VPP_LINK_FLAGS += --advanced.param "compiler.multiStrategiesWaitOnAllRuns=1"
#VPP_LINK_FLAGS += --vivado.synth.jobs 32
#VPP_LINK_FLAGS += --vivado.impl.jobs 17

# =========================================================
# Packaging Flags
# =========================================================
PKG_FLAGS := -t $(TARGET)
PKG_FLAGS += --save-temps
PKG_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
PKG_FLAGS += -f $(PLATFORM)
PKG_FLAGS += --package.rootfs $(COMMON_IMAGE_VERSAL)/rootfs.ext4
PKG_FLAGS += --package.kernel_image $(COMMON_IMAGE_VERSAL)/Image
PKG_FLAGS += --package.boot_mode=sd
PKG_FLAGS += --package.out_dir $(EMBEDDED_PACKAGE_OUT)
PKG_FLAGS += --package.image_format=ext4
PKG_FLAGS += --package.sd_file $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XSA) $(LIBADF_A)
#PKG_FLAGS += --package.sd_file $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)  
PKG_FLAGS += --package.sd_file $(EXEC_SCRIPTS_REPO)/$(EMBEDDED_EXEC_SCRIPT)

ifeq ($(EN_TRACE),1)
   ifeq ($(TARGET),hw)
      PKG_FLAGS += --package.sd_file $(PROFILING_CONFIGS_REPO)/xrt.ini
   
   endif
endif

ifdef XRT_ROOT
   PKG_FLAGS += --package.sd_dir $(XRT_ROOT)

endif

PKG_FLAGS += --package.defer_aie_run

###########################################################
# Make rules...
#
# =========================================================
# Step 1. Kernel XO File Generation
# =========================================================
# This step compiles the HLS C PL kernels.
# Outputs: in build/[hw_emu | hw]/ directory
# 	[datamover].[hw_emu | hw].xo 
#	[datamover].[hw_emu | hw].xo.compile_summary 
#	v++_[datamover].[hw_emu | hw].log
#	_x
KERNEL_XOS := $(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo

kernels: $(KERNEL_XOS)

$(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo: $(PL_SRC_REPO)/datamover*
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(DATAMOVER_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k $(DATAMOVER_KERNEL_NAME) \
		$(DATAMOVER_KERNEL_SRC) -o $@
	@echo ""
	@echo "HLS Kernel Compilation Complete..."
	@echo "####################################"
	@echo ""
# =========================================================
# Step 2. AI Engine SDF Graph File and Work/ Directory
#         (containing the Graph Executable) Generation
# =========================================================
# This step creates an SDF Graph and the Work/ directory.
# The Work/ directory contains the graph executable
# (fir.o) which is used in the make xsa step. 
# The aiecompiler is invoked with the -target=hw.
# Outputs: in build/ directory
#  libsdf.a
#  NOC_Power.xpe
#  Work/
#  xnwOut/
graph: $(LIBADF_A)

$(LIBADF_A): $(AIE_SRC_REPO)/fir_aie_graph.*
	#echo "$(N_FIR_FILTERS) FIR Filters in Graph; each with $(N_FIR_TAPS) Taps"
	#python $(PYTHON_SCRIPTS_REPO)/create_fir_coefs.py -aie $(N_FIR_FILTERS) $(N_FIR_TAPS) $(IMPULSE_AREA) $(FIR_WINDOW_SIZE) > $(GRAPH_SETTINGS_H)
	mkdir -p $(BUILD_TARGET_DIR); 	\
	cd $(BUILD_TARGET_DIR); 	\
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP)
	@echo ""
	@echo "AIE Compilation Complete..."
	@echo "####################################"
	@echo ""

py_test:
	echo "$(N_FIR_FILTERS) FIR Filters in Graph; each with $(N_FIR_TAPS) Taps"
	#python $(PYTHON_SCRIPTS_REPO)/create_fir_coefs.py -aie $(N_FIR_FILTERS) $(N_FIR_TAPS) $(IMPULSE_AREA) $(FIR_WINDOW_SIZE) > $(GRAPH_SETTINGS_H)
	more $(GRAPH_SETTINGS_H)

aiesim: graph
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) 2>&1 | tee -a aiesim.log
	@echo ""
	@echo "AIE Simulation, Without Profiling, Complete..."
	@echo "####################################"
	@echo ""
	
aiesim_profile: graph
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS)--profile 2>&1 | tee -a aiesim.log
	@echo ""
	@echo "AIE Simulation, With Profiling, Complete..."
	@echo "####################################"
	@echo ""

vcd: graph $(XPE_FILE)

# xpe file generation...
$(XPE_FILE): $(BLD_TGT_VCD_FILE)
	cd $(BUILD_TARGET_DIR); \
	vcdanalyze --vcd $(VCD_FILE_NAME).vcd --xpe
	rm -rf $(BLD_XPE_DIR)
	mkdir -p $(BLD_XPE_DIR)
	cp -rf $(BUILD_TARGET_DIR)/aiesim_xpe/fir_aie_graph.xpe $(XPE_FILE)
	@echo ""
	@echo "AIE XPE Generation Complete..."
	@echo "####################################"
	@echo ""

# vcd file generation...
$(BLD_TGT_VCD_FILE): $(AIE_SRC_REPO)/aiesim_data/*
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) --profile --dump-vcd $(VCD_FILE_NAME) 2>&1 | tee -a vcd.log
	@echo ""
	@echo "AIE VCD Trace Generation Complete..."
	@echo "####################################"
	@echo ""

# =========================================================
# Step 3. XSA File Generation
# =========================================================
# This step links the graph executable (fir_aie.o) and
# the kernels into a xsa file.
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_aie_fir.[hw_emu | hw].xsa
#	vck190_aie_fir.[hw_emu | hw].xsa.info
#	vck190_aie_fir.[hw_emu | hw].xsa.link_summary
#	vck190_aie_fir.[hw_emu | hw].xsa
#	vck190_aie_fir.[hw_emu | hw].log
xsa: kernels graph $(BUILD_TARGET_DIR)/$(XSA)

$(BUILD_TARGET_DIR)/$(XSA):$(KERNEL_XOS) $(SYSTEM_CONFIGS_REPO)/*
	cd $(BUILD_TARGET_DIR);		\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -t $(TARGET) -o $@ 	\
		$(KERNEL_XOS) $(LIBADF_A)
	@echo ""
	@echo "XSA Compilation Complete..."
	@echo "####################################"
	@echo ""
# =========================================================
# Step 4. A72 Application Executable File Generation
# =========================================================
# This step compiles the A72 application. This step is the 
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# 	aie_control.o
#	fir_aie_app.o
# 	fir_aie_xrt.elf

REG_GCC_FLAGS := $(GCC_FLAGS)
REG_GCC_FLAGS += -DITER_CNT=$(ITER_CNT)

application: graph $(BUILD_TARGET_DIR)/$(APP_ELF)

$(BUILD_TARGET_DIR)/$(APP_ELF): $(HOST_APP_SRC)/* $(LIBADF_A)
	$(CXX) $(REG_GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control.o
	$(CXX) $(REG_GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/fir_aie_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/fir_aie_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF)
	@echo ""
	@echo "Host Application Compilation Complete..."
	@echo "####################################"
	@echo ""

# This step compiles the A72 application. This step is the 
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# 	aie_control_inf_run.o
#	fir_aie_app_inf_run.o
# 	fir_aie_xrt_inf_run.elf

#INF_RUN_GCC_FLAGS := $(GCC_FLAGS)
#INF_RUN_GCC_FLAGS += -DITER_CNT=-1
#
#application_inf_run: graph $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)
#
#$(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN): $(HOST_APP_SRC)/* $(LIBADF_A)
#	$(CXX) $(INF_RUN_GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control_inf_run.o
#	$(CXX) $(INF_RUN_GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/fir_aie_app_inf_run.o $(GCC_INC_LIB) $(GCC_LIB)
#	$(CXX) $(BUILD_TARGET_DIR)/app_control_inf_run.o $(BUILD_TARGET_DIR)/fir_aie_app_inf_run.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)
#	@echo ""
#	@echo "Host Application Infinite Run Compilation Complete..."
#	@echo "####################################"
#	@echo ""
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
package:application xsa $(EMBEDDED_PACKAGE_OUT)

$(EMBEDDED_PACKAGE_OUT): $(PROFILING_CONFIGS_REPO)/* $(EXEC_SCRIPTS_REPO)/* $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XSA)
	cd $(BUILD_TARGET_DIR);	\
	v++ -p $(PKG_FLAGS)
	@echo ""
	@echo "Design Packaging Complete..."
	@echo "####################################"
	@echo ""
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
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) | tee embedded_run.log

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
report_metrics: xsa $(BLD_REPORTS_DIR)

$(BLD_REPORTS_DIR): $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"

else
	rm -rf $(BLD_REPORTS_DIR)
	mkdir -p $(BLD_REPORTS_DIR)
	cd $(BLD_REPORTS_DIR); \
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
	@echo ""
	@echo "Vivado Utilization/Power Report Generation Complete..."
	@echo "####################################"
	@echo ""

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
# ==> cleanall_blds
# ==> cleanall_vivado_reports
# ==> cleanall_xpe_reports
# ==> cleanall_reports
# ==> cleanall
# =========================================================

#Build the design without running the hardware emulation

sd_card: package

#Build the design and then run hardware emulation
run: sd_card run_emu

#Same as "run" above, but include metrics generation and
#vcd generation
all: run report_metrics vcd 

#Clean target dir files...
clean_target:
	@echo "Cleaning Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

#Clean no. of firs dir files...
clean_firFilters_dir:
	@echo "Cleaning $(N_FIR_FILTERS) FIR FILTERS Build Dir..."
	rm -rf $(FIR_FILTERS_DIR)

#Clean fir taps dir files...
clean_firTaps_dir:
	@echo "Cleaning FIR $(N_FIR_TAPS) TAPS Build Dir..."
	rm -rf $(FIR_TAPS_BLD_DIR)

#Clean fir window size dir files...
clean_firWinSz_dir:
	@echo "Cleaning FIR $(FIR_WINDOW_SIZE) TAPS Build Dir..."
	rm -rf $(FIR_WINDOW_SIZE_DIR)

#Clean no. aies per fir dir files...
clean_aiesPerFir_dir:
	@echo "Cleaning FIR $(N_AIES_PER_FIR) TAPS Build Dir..."
	rm -rf $(AIES_PER_FIR_DIR)

#Clean all builds...
cleanall_blds:
	rm -rf $(BASE_BLD_DIR)
	rm -rf vivado* .Xil *.log vitis* core.*

#Clean_all vivado reports...
cleanall_vivado_reports:
	rm -rf $(VIVADO_REPORTS_REPO)

#Clean_all xpes...
cleanall_xpe_reports:
	rm -rf $(XPE_REPO)

#Clean_all reports...
cleanall_reports: cleanall_vivado_reports cleanall_xpe_reports

#Clean_all builds and reports...
cleanall: cleanall_blds cleanall_reports

#####################################################################
# Impl strategies script...
# Strategies Repo with the impl Script Has to be provided...
IMPL_STRATEGY_OPT := 1
IMPL_STRATEGIES_TCL := $(STRATEGIES_REPO)/impl_strategies_explore.tcl
run_impl_strategies: $(IMPL_STRATEGIES_TCL)
ifeq ($(TARGET),hw_emu)
	@echo "This build target (run_impl_strategies) not valid when design target is hw_emu"

else
	vivado -mode tcl -source $(IMPL_STRATEGIES_TCL) \
	$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr -tclargs $(IMPL_STRATEGY_OPT)
	#$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr -tclargs $(BUILD_TARGET_DIR) $(IMPL_STRATEGY_OPT)

endif

open_vivado_proj: $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
	vivado $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
