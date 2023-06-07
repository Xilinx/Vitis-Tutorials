
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

%.PHONY: help
help::
	@echo  ""
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
	@echo  "  make clean_tgt"
	@echo  "  Command to remove the generated files for a specific target"
	@echo  ""
	@echo  "  make clean_insts"
	@echo  "  Command to remove the generated files for a specific instance of the design."
	@echo  ""
	@echo  "  make clean_gemmsize"
	@echo  "  Command to remove the generated files for a specific GeMM Matrix Dimension"
	@echo  ""
	@echo  "  make cleanall_blds"
	@echo  "  Command to remove the generated files for all variations"
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
	@echo  "  Command to remove the generated files and reports for all variations"
	@echo  ""
	@echo  " OPTIONS:"
	@echo  " Use the make recipes with required values for options mentioned below-"
	@echo  "    TARGET     -specifies target: Hardware or Emulation. Values: hw_emu(default), hw"
	@echo  "    GEMM_SIZE  -specifies GeMM Matrix Dimension. Values: 32(default), 64, 128, 256, 512, 1024"
	@echo  "    GEMM_INSTS -specifies number of GeMM instances. Values: 1(default)"
	@echo  "    PL_FREQ    -specifies HLS kernel frequencies. Values: 250Mhz(default)"
	@echo  "    ITER_CNT   -specifies design iteration count: Values: 1(default)"
	@echo  "    EN_TRACE   -specifies whether to enable profiling for the design. Values: 0(disabled, default) and 1(enabled)"
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
# Gemm Output matrix size can be set as:
# 32(32x32x32)<default>, 64(64x64x64), 128(128x128x128),
# 256(256x256x256), 512(512x512x512) and 1024(1kx1kx1k)
# =========================================================
GEMM_SIZE := 32
MAT_DIMS  := $(GEMM_SIZE)x$(GEMM_SIZE)x$(GEMM_SIZE)

# =========================================================
# No of GEMM Instances can be set as:
#   1(default)
# =========================================================
GEMM_INSTS := 1

# =========================================================
# PL Frequency in Mhz:
#   312.5Mhz(default)
# =========================================================
PL_FREQ := 312.5
HZ_UNIT := 1000000
VPP_CLOCK_FREQ := $(shell printf "%.0f" `echo "${PL_FREQ} * $(HZ_UNIT)" | bc`)
#VPP_CLOCK_FREQ := $(PL_FREQ)000000

# =========================================================
# Iteration Count:
# 16(default)
# =========================================================
ITER_CNT := 16

# =========================================================
# Profiling/Trace Switches...
# =========================================================
EN_TRACE := 0

######################################
# When TARGET=hw_emu, hw_emu launch...
# 1 = Enabled (Default).
# 0 = Disabled.
######################################
LAUNCH_HW_EMU_EXEC := 0

# =========================================================
# Source directories
# =========================================================
# Relative gemm directory
RELATIVE_PROJECT_DIR := ./

# Absolute gemm directory = <user path>/Tutorials/AI_Engine/gemm
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

## Setting other folders relative to PROJECT_REPO
DESIGN_REPO  := $(PROJECT_REPO)/design
AIE_SRC_REPO := $(DESIGN_REPO)/aie_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src

SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
EXEC_SCRIPTS_REPO      := $(DESIGN_REPO)/exec_scripts
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts
DIRECTIVES_REPO        := $(DESIGN_REPO)/directives

# Build...
BASE_BLD_DIR     := $(PROJECT_REPO)/build
GEMM_BLD_DIR     := $(BASE_BLD_DIR)/gemm_$(MAT_DIMS)
INSTS_BLD_DIR    := $(GEMM_BLD_DIR)/x$(GEMM_INSTS)
BUILD_TARGET_DIR := $(INSTS_BLD_DIR)/$(TARGET)

# Vivado Utilization and Power Reports...
REPORTS_REPO := $(PROJECT_REPO)/reports_dir
BLD_REPORTS_DIR := $(REPORTS_REPO)/gemm_$(MAT_DIMS)/x$(GEMM_INSTS)

# XPE...
XPE_REPO := $(PROJECT_REPO)/xpe_dir
BLD_XPE_DIR := $(XPE_REPO)/gemm_$(MAT_DIMS)/x$(GEMM_INSTS)/$(TARGET)
VCD_FILE_NAME := gemm_$(MAT_DIMS)_x$(GEMM_INSTS)
BLD_TGT_VCD_FILE := $(BUILD_TARGET_DIR)/$(VCD_FILE_NAME).vcd
XPE_FILE := $(BLD_XPE_DIR)/graph_$(VCD_FILE_NAME).xpe

# Package...
EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

# AIE...
WORK_DIR := Work
AIE_SIM_IO_BASE_DIR := $(AIE_SRC_REPO)/aiesim_data
AIE_SIM_IO_DIR := $(AIE_SIM_IO_BASE_DIR)/gemm_$(MAT_DIMS)_ioFiles

###########################################################
# Variable Definitions...
#
# ==========================================================
# Below are the names for SDF graph, application executable,
# kernel executables, and xsa
# ==========================================================
DATAMOVER_KERNEL_TOP := dma_hls
DATAMOVER_KERNEL_XO  := $(DATAMOVER_KERNEL_TOP).$(TARGET)

LIBADF_A := $(BUILD_TARGET_DIR)/libadf.a

APP_ELF := gemm_aie_xrt.elf
APP_ELF_INF_RUN := gemm_aie_xrt_inf_run.elf
XSA  := vck190_aie_gemm.$(TARGET).xsa

# =========================================================
# Kernel Source Files repository
# =========================================================
DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_TOP).cpp

# =========================================================
# Graph Source files repository
# ========================================================
GRAPH_SRC_CPP := $(AIE_SRC_REPO)/graph.cpp

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP := $(HOST_APP_SRC)/gemm_aie_app.cpp

AIE_CONTROL_CPP := $(BUILD_TARGET_DIR)/$(WORK_DIR)/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS := -include=$(AIE_SRC_REPO)
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/include/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/src/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/tests/aie/inc
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L1/tests/aie/src
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L2/include/aie
AIE_FLAGS += -include=$(DSPLIB_ROOT)/L2/tests/aie/common/inc
AIE_FLAGS += --verbose
AIE_FLAGS += --Xpreproc="-DITER_CNT=$(ITER_CNT)"
AIE_FLAGS += --Xpreproc="-DGEMM_SIZE=$(GEMM_SIZE)"
AIE_FLAGS += --Xpreproc="-DGEMM_INSTS=$(GEMM_INSTS)"
AIE_FLAGS += --platform=$(PLATFORM)
#AIE_FLAGS += --target=$(TARGET)

AIE_FLAGS += --log-level=5
#AIE_FLAGS += --test-iterations=2
AIE_FLAGS += --pl-freq=$(PL_FREQ)
AIE_FLAGS += --dataflow

# Constraints for same shim solution for all variations
# aids in timeing closure of the design...
AIE_FLAGS += --constraints=$(AIE_SRC_REPO)/constraints.aiecst

AIE_FLAGS += --Xmapper=BufferOptLevel9
AIE_FLAGS += --Xrouter=DMAFIFOsInFreeBankOnly

#AIE_FLAGS += --heapsize=7000
#AIE_FLAGS += --Xchess="main:bridge.llibs=softfloat m"
AIE_FLAGS += --workdir=$(WORK_DIR)

# Aiesimulator flags
#-i - Alias of --input-dir=<dir> option
#-o - Alias of --output-dir=<dir> option
AIE_SIM_FLAGS := --pkg-dir $(WORK_DIR)/
AIE_SIM_FLAGS += -i=$(AIE_SIM_IO_DIR)

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__
GCC_FLAGS += -D__PS_ENABLE_AIE__
GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -DGEMM_INSTS=$(GEMM_INSTS)
GCC_FLAGS += -DGEMM_SIZE=$(GEMM_SIZE)

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt
GCC_INC_FLAGS += -I$(XILINX_VITIS)/aietools/include/
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)
GCC_INC_FLAGS += -I$(HOST_APP_SRC)
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/include/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/src/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/tests/aie/inc
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L1/tests/aie/src
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L2/include/aie
GCC_INC_FLAGS += -I$(DSPLIB_ROOT)/L2/tests/aie/common/inc

GCC_INC_LIB += -L$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_LIB += -L$(XILINX_VITIS)/aietools/lib/aarch64.o
GCC_INC_LIB += -L$(XILINX_VITIS)/aietools/lib/lnx64.o

GCC_LIB := -ladf_api_xrt
GCC_LIB += -lxrt_coreutil

# =========================================================
# Kernel Compiler and Linker Flags
# ========================================================

#VPP_LINK_FLAGS += --advanced.param "compiler.worstNegativeSlack=-1.0"
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose
VPP_FLAGS += -g

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)
DATAMOVER_KERNEL_VPP_FLAGS += -D GEMM_SIZE=$(GEMM_SIZE)
#DATAMOVER_KERNEL_VPP_FLAGS += --hls.jobs 32

VPP_LINK_FLAGS := --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_0
VPP_LINK_FLAGS += --clock.defaultTolerance 0.001

## If Profiling for Performance Measurement is enabled..
ifeq ($(EN_TRACE),1)
   ifeq ($(TARGET),hw)
      VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_TOP):all:all
      VPP_LINK_FLAGS += --profile.trace_memory DDR
      
   endif
endif

VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/x$(GEMM_INSTS).cfg
VPP_LINK_FLAGS += --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true

VPP_LINK_FLAGS += --vivado.prop run.impl_1.STEPS.PLACE_DESIGN.TCL.PRE=$(DIRECTIVES_REPO)/prohibit_select_bli_bels_for_hold.tcl

VPP_LINK_FLAGS += --vivado.prop run.synth_1.STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD=16
VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExplorePostRoutePhysOpt}
#VPP_LINK_FLAGS += --vivado.prop run.synth_1.STEPS.SYNTH_DESIGN.ARGS.RETIMING=true
#VPP_LINK_FLAGS += --advanced.param "compiler.worstNegativeSlack=-1.0"
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.OPT_DESIGN.ARGS.IS_ENABLED}={true}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.OPT_DESIGN.ARGS.DIRECTIVE}={Default}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PLACE_DESIGN.ARGS.DIRECTIVE}={SSI_BalanceSLLs}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={Default}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={false}
######################################################################################
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Auto_3}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Explore} 

#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_WLBlockPlacement}
#VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_EarlyBlockPlacement}
#####################################################################################
#VPP_LINK_FLAGS += --vivado.synth.jobs 8
#VPP_LINK_FLAGS += --vivado.impl.jobs 8

# =========================================================
# Packaging Flags
# ========================================================
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
PKG_FLAGS += --package.sd_file $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN) 
PKG_FLAGS += --package.sd_file $(EXEC_SCRIPTS_REPO)/$(EMBEDDED_EXEC_SCRIPT)

## If Profiling for Performance Measurement is enabled..
ifeq ($(EN_TRACE),1)
   ifeq ($(TARGET),hw)
      PKG_FLAGS += --package.sd_file $(PROFILING_CONFIGS_REPO)/xrt.ini
   
   endif
endif

## If XRT_ROOT is set...
ifdef XRT_ROOT
   PKG_FLAGS += --package.sd_dir $(XRT_ROOT)

endif

PKG_FLAGS += --package.defer_aie_run

###########################################################
# Make rules...
#
# =========================================================
# Step 1. Kernel XO File Generation
# ========================================================
# This step compiles the HLS C PL kernels.
# Outputs: in build/[hw_emu | hw]/ directory
# 	$(DATAMOVER_KERNEL_TOP).[hw_emu | hw].xo 
#	$(DATAMOVER_KERNEL_TOP).[hw_emu | hw].xo.compile_summary 
#	v++_$(DATAMOVER_KERNEL_TOP).[hw_emu | hw].log
#	_x
KERNEL_XOS := $(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo

kernels: $(KERNEL_XOS)

$(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo: $(PL_SRC_REPO)/dma_hls* 
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(DATAMOVER_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k $(DATAMOVER_KERNEL_TOP) \
		$(DATAMOVER_KERNEL_SRC) -o $@
	@echo ""
	@echo "HLS Kernel Compilation Complete..."
	@echo "####################################"
	@echo ""

# =========================================================
# Step 2. AI Engine SDF Graph File and $(WORK_DIR)/ Directory
#         (containing the Graph Executable) Generation
# ========================================================
# This step creates an SDF Graph and the $(WORK_DIR)/ directory.
# The $(WORK_DIR)/ directory contains the graph executable
# (gemm.o) which is used in the make xsa step. 
# The aiecompiler is invoked with the -target=hw.
# Outputs: in build/ directory
#	libsdf.a
#	NOC_Power.xpe
#	$(WORK_DIR)/
#	xnwOut/
graph: $(LIBADF_A)

$(LIBADF_A):  $(AIE_SRC_REPO)/graph.*
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP) 2>&1 | tee -a aiecompiler.log
	@echo ""
	@echo "AIE Compilation Complete..."
	@echo "####################################"
	@echo ""

# IO Input Type...
# 0 - MatA constant 1 and MatB constant 2(default)...
# 1 - MatA increasing element Matrix upto $(GEMM_SIZE) x $(GEMM_SIZE) \
# 	   and MatB is Identity Matrix...
# 2 - MatA and MatB both random element (each element is random value \
# 		between 0 to 9) Matrix... 		
IO_INP_TYPE := 0
create_ioFiles:
	rm -rf $(AIE_SIM_IO_DIR)
	cd $(AIE_SIM_IO_BASE_DIR); \
	python $(AIE_SIM_IO_BASE_DIR)/plioGen.py $(GEMM_SIZE) $(IO_INP_TYPE) $(ITER_CNT)
	chmod 755 -R $(AIE_SIM_IO_DIR)

aiesim: graph
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) 2>&1 | tee -a aiesim.log
	@echo ""
	@echo "AIE Simulation, Without Profiling, Complete..."
	@echo "####################################"
	@echo ""

aiesim_profile: graph
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) --profile 2>&1 | tee -a aiesim.log
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
	cp -rf $(BUILD_TARGET_DIR)/aiesim_xpe/*.xpe $(XPE_FILE)
	chmod 755 -R $(XPE_REPO)
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
# ========================================================
# This step links the graph executable and
# the kernels into a xsa file.
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_aie_gemm.[hw_emu | hw].xsa
#	vck190_aie_gemm.[hw_emu | hw].xsa.info
#	vck190_aie_gemm.[hw_emu | hw].xsa.link_summary
#	vck190_aie_gemm.[hw_emu | hw].xsa
#	vck190_aie_gemm.[hw_emu | hw].log
xsa:  kernels graph $(BUILD_TARGET_DIR)/$(XSA)

$(BUILD_TARGET_DIR)/$(XSA):$(KERNEL_XOS) $(SYSTEM_CONFIGS_REPO)/*
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -t $(TARGET) -o $@ $(KERNEL_XOS) $(LIBADF_A)
	@echo ""
	@echo "XSA Compilation Complete..."
	@echo "####################################"
	@echo ""

# =========================================================
# Step 4. A72 Application Executable File Generation
# ========================================================
# This step compiles the A72 application. This step is the 
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# 	aie_control.o
#	gemm_aie_app.o
# 	gemm_aie_xrt.elf

application: graph $(BUILD_TARGET_DIR)/$(APP_ELF)

REG_GCC_FLAGS := $(GCC_FLAGS)
REG_GCC_FLAGS += -DITER_CNT=$(ITER_CNT)

$(BUILD_TARGET_DIR)/$(APP_ELF): $(HOST_APP_SRC)/* $(LIBADF_A)
	@rm -rf $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/gemm_aie_app.o $(BUILD_TARGET_DIR)/$(APP_ELF)
	$(CXX) $(REG_GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control.o
	$(CXX) $(REG_GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/gemm_aie_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/gemm_aie_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF)
	@echo ""
	@echo "Host Application Compilation Complete..."
	@echo "####################################"
	@echo ""

# This step compiles the A72 application. This step is the 
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# 	aie_control_inf_run.o
#	gemm_aie_app_inf_run.o
# 	gemm_aie_xrt_inf_run.elf

application_inf_run: graph $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)

INF_RUN_GCC_FLAGS := $(GCC_FLAGS)
INF_RUN_GCC_FLAGS += -DITER_CNT=-1

$(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN): $(HOST_APP_SRC)/* $(LIBADF_A)
	@rm -rf $(BUILD_TARGET_DIR)/app_control_inf_run.o $(BUILD_TARGET_DIR)/gemm_aie_app_inf_run.o $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)
	$(CXX) $(INF_RUN_GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control_inf_run.o
	$(CXX) $(INF_RUN_GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/gemm_aie_app_inf_run.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/app_control_inf_run.o $(BUILD_TARGET_DIR)/gemm_aie_app_inf_run.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)
	@echo ""
	@echo "Host Application Infinite Run Compilation Complete..."
	@echo "####################################"
	@echo ""

# =========================================================
# Step 5. Package Generation 
# ========================================================
# This step generates the package folder which contains the
# ./launch_hw_emu.sh script to launch hardware emulation
# if TARGET=hw_emu and the sd_card.img file. 
# Outputs: in build/[hw_emu | hw]/ directory
# 	a.xclbin
# 	package/ directory
# 	v++.package_summary
# 	v++_package.log
package: application application_inf_run xsa $(EMBEDDED_PACKAGE_OUT)

$(EMBEDDED_PACKAGE_OUT): $(PROFILING_CONFIGS_REPO)/* $(EXEC_SCRIPTS_REPO)/* $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XSA) $(BUILD_TARGET_DIR)/$(APP_ELF_INF_RUN)
	rm -rf $(EMBEDDED_PACKAGE_OUT)
	cd $(BUILD_TARGET_DIR);	\
	v++ -p $(PKG_FLAGS)
	#cp -rf $(EMBEDDED_PACKAGE_OUT) $(EMBEDDED_PACKAGE_OUT)_backup
	@echo ""
	@echo "Design Packaging Complete..."
	@echo "####################################"
	@echo ""

# =========================================================
# Step 6. Run Hardware Emulation 
# ========================================================
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
	@echo ""
	@echo "HW Emulation Complete..."
	@echo "####################################"
	@echo ""

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
# utilization metrics from vivado.
report_metrics: xsa $(BLD_REPORTS_DIR)

$(BLD_REPORTS_DIR): $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"

else
	rm -rf $(BLD_REPORTS_DIR)
	mkdir -p $(BLD_REPORTS_DIR)
	cd $(BLD_REPORTS_DIR); \
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl \
	$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
	chmod 755 -R $(REPORTS_REPO)
	@echo ""
	@echo "Vivado Utilization/Power Report Generation Complete..."
	@echo "####################################"
	@echo ""

endif

###########################################################################

# =========================================================
# Primary Build Targets
# ==> sd_card
# ==> run
# ==> all
# ==> clean_tgt
# ==> clean_insts
# ==> clean_gemmsize
# ==> cleanall_blds
# ==> cleanall_vivado_reports
# ==> cleanall_xpe_reports
# ==> cleanall_reports
# ==> cleanall
# ========================================================

#Build the design without running the hardware emulation
sd_card: package

#Build the design and then run hardware emulation
run: sd_card run_emu 

#Same as "run" above, but include metrics generation and
#vcd generation
all: run report_metrics vcd

#Clean build target output directory...
clean_tgt:
	@echo "Cleaning $(TARGET) Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

#Clean GeMM instance directory...
clean_insts:
	@echo "Cleaning $(GEMM_INSTS)x Builds..."
	rm -rf $(INSTS_BLD_DIR)

#Clean GeMM matrix dimension output directory...
clean_gemmsize:
	@echo "Cleaning Gemm Size Builds..."
	rm -rf $(GEMM_BLD_DIR)

#Clean all builds...
cleanall_blds:
	rm -rf $(BASE_BLD_DIR)
	rm -rf vivado* .Xil *.log vitis* core.*

#Clean_all vivado reports...
cleanall_vivado_reports:
	rm -rf $(REPORTS_REPO)

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
BASE_OPTION       := 2
IMPL_DEPTH_OPTION := 2
PIPELINE_SIZE     := 500

IMPL_STRATEGIES_TCL := $(STRATEGIES_REPO)/impl_strategies_explore_best3.tcl

run_impl_strategies: $(IMPL_STRATEGIES_TCL) $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
ifeq ($(TARGET),hw_emu)
	@echo "This build target (run_impl_strategies) not valid when design target is hw_emu"

else
	cd $(BUILD_TARGET_DIR); \
	vivado -mode tcl -source $(IMPL_STRATEGIES_TCL) \
	_x/link/vivado/vpl/prj/prj.xpr -tclargs $(BASE_OPTION) $(IMPL_DEPTH_OPTION) $(PIPELINE_SIZE)

endif

open_vivado_proj: $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
	cd $(BUILD_TARGET_DIR); \
	vivado _x/link/vivado/vpl/prj/prj.xpr
