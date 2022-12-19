# © Copyright 2021–2022 Xilinx, Inc.
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
	@echo  "  make clean_fftpt"
	@echo  "  Command to remove the generated files for a specific fft point size"
	@echo  ""
	@echo  "  make clean_fft2d_dt"
	@echo  "  Command to remove the generated files for a specific fft datatype"
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
	@echo  "    FFT_2D_DT          -specifies FFT Datatype. Values: 0(cint16, default), 1(cfloat)"
	@echo  "    FFT_2D_PT          -specifies FFT Point. Values: 64, 128, 256, 512, 2048(default)"
	@echo  "    FFT_2D_INSTS       -specifies number of FFT instances. Values: 1(default), 5 and 10"
	@echo  "    PL_FREQ        	 -specifies HLS kernel frequencies. Values: 250Mhz(default)"
	@echo  "    ITER_CNT           -specifies design iteration count: Values: 16(default)"
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

######################################
# FFT 2D Datatype can be,
# cint16 (Default) or cfloat...
######################################
FFT_2D_DT := cint16

# =========================================================
# 2 Dimentional FFT Point: MAT_ROWS x MAT_COLS(Mat Dims):
#   FFT_2D_PT =   64:   32 x   64
#   FFT_2D_PT =  128:   64 x  128
#   FFT_2D_PT =  256:  128 x  256
#   FFT_2D_PT =  512:  256 x  512
#   FFT_2D_PT = 2048: 1024 x 2048 (default)
#
#   MAT_ROWS: No. of rows in the Inp Matrix (default 1024)
#   MAT_COLS: No. of cols in the Inp Matrix (default 2048)
# =========================================================
FFT_2D_PT := 2048

MAT_COLS := $(FFT_2D_PT)
MAT_ROWS := $(shell echo $$(( $(MAT_COLS) / 2 )))

# =========================================================
# No of FFT2D Instances can be set as:
#   1(default), 5 and 10.
# =========================================================
FFT_2D_INSTS := 1

# =========================================================
# PL Frequency in Mhz:
#   250Mhz(default)
# =========================================================
PL_FREQ := 250
HZ_UNIT := 1000000
VPP_CLOCK_FREQ := $(shell printf "%.0f" `echo "${PL_FREQ} * $(HZ_UNIT)" | bc`)

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
# Relative fft_2d directory
RELATIVE_PROJECT_DIR := ./

# Absolute fft_2d directory = <user path>/Tutorials/AI_Engine/fft_2d
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
AIE_SRC_REPO := $(DESIGN_REPO)/aie_src
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src

DIRECTIVES_REPO        := $(DESIGN_REPO)/directives
SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
EXEC_SCRIPTS_REPO      := $(DESIGN_REPO)/exec_scripts
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

BASE_BLD_DIR     := $(PROJECT_REPO)/build
FFT_2D_DT_DIR    := $(BASE_BLD_DIR)/$(FFT_2D_DT)_dsns
FFTPT_BLD_DIR    := $(FFT_2D_DT_DIR)/fft2d_$(MAT_ROWS)x$(MAT_COLS)
INSTS_BLD_DIR    := $(FFTPT_BLD_DIR)/x$(FFT_2D_INSTS)
BUILD_TARGET_DIR := $(INSTS_BLD_DIR)/$(TARGET)

REPORTS_REPO := $(PROJECT_REPO)/reports_dir
BLD_REPORTS_DIR := $(REPORTS_REPO)/$(FFT_2D_DT)_dsns/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)

XPE_REPO := $(PROJECT_REPO)/xpe_dir
BLD_XPE_DIR := $(XPE_REPO)/$(FFT_2D_DT)_dsns/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/$(TARGET)
VCD_FILE_NAME := $(FFT_2D_DT)_fft2d_$(MAT_ROWS)x$(MAT_COLS)_x$(FFT_2D_INSTS)
BLD_TGT_VCD_FILE := $(BUILD_TARGET_DIR)/$(VCD_FILE_NAME).vcd
XPE_FILE := $(BLD_XPE_DIR)/graph_$(VCD_FILE_NAME).xpe

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

WORK_DIR := Work

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
VCD_FILE := $(BUILD_TARGET_DIR)/x$(FFT_2D_INSTS).vcd

APP_ELF := fft_2d_aie_xrt.elf
XSA := vck190_aie_fft_2d.$(TARGET).xsa

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
APP_SRC_CPP := $(HOST_APP_SRC)/fft_2d_aie_app.cpp

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
AIE_FLAGS += --Xpreproc="-DFFT2D_INSTS=$(FFT_2D_INSTS)"
AIE_FLAGS += --Xpreproc="-DMAT_ROWS=$(MAT_ROWS)"
AIE_FLAGS += --Xpreproc="-DMAT_COLS=$(MAT_COLS)"
AIE_FLAGS += --Xpreproc="-DFFT_2D_DT=$(FFT_2D_DT)"
AIE_FLAGS += --Xpreproc="-DITER_CNT=16"
#AIE_FLAGS += --Xpreproc="-DITER_CNT=$(ITER_CNT)"
AIE_FLAGS += --platform=$(PLATFORM)

AIE_FLAGS += --log-level=5
AIE_FLAGS += --test-iterations=2
AIE_FLAGS += --pl-freq=$(PL_FREQ)
AIE_FLAGS += --dataflow

# Constraints for same shim solution for all variations
# aids in timeing closure of the design...
AIE_FLAGS += --constraints=$(AIE_SRC_REPO)/constraints.aiecst

AIE_FLAGS += --Xmapper=BufferOptLevel9
AIE_FLAGS += --Xrouter=DMAFIFOsInFreeBankOnly

#AIE_FLAGS += --heapsize=7000
AIE_FLAGS += --Xchess="main:bridge.llibs=softfloat m"
AIE_FLAGS += --workdir=$(WORK_DIR)

# Aiesimulator flags
#-i - Alias of --input-dir=<dir> option
#-o - Alias of --output-dir=<dir> option
AIE_SIM_FLAGS := --pkg-dir $(WORK_DIR)/
AIE_SIM_FLAGS += -i=$(AIE_SRC_REPO)/aiesim_data/$(FFT_2D_DT)_IOfiles/fft2d_$(MAT_ROWS)x$(MAT_COLS)_ioFiles

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++17
GCC_FLAGS += -D__linux__
GCC_FLAGS += -D__PS_ENABLE_AIE__
GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -DFFT2D_INSTS=$(FFT_2D_INSTS)
GCC_FLAGS += -DMAT_ROWS=$(MAT_ROWS)
GCC_FLAGS += -DMAT_COLS=$(MAT_COLS)
GCC_FLAGS += -DFFT_2D_DT=$(FFT_2D_DT)
GCC_FLAGS += -DITER_CNT=$(ITER_CNT)

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
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose
VPP_FLAGS += -g

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)
DATAMOVER_KERNEL_VPP_FLAGS += -D FFT_2D_DT=$(FFT_2D_DT)
#DATAMOVER_KERNEL_VPP_FLAGS += --hls.jobs 32

VPP_LINK_FLAGS := --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_0

ifeq ($(FFT_2D_INSTS),$(filter $(FFT_2D_INSTS),5 10))
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_1
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_2
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_3
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_4
   
endif

ifeq ($(FFT_2D_INSTS),10)
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_5
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_6
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_7
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_8
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_9
   
endif

VPP_LINK_FLAGS += --clock.defaultTolerance 0.001

ifeq ($(EN_TRACE),1)
   ifeq ($(TARGET),hw)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_TOP):all:all
      
      else
         VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_TOP):all:strmInp_from_colwiseFFT
      
      endif
      
      VPP_LINK_FLAGS += --profile.trace_memory DDR
   
   endif
endif

VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/x$(FFT_2D_INSTS).cfg
#VPP_LINK_FLAGS += --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true

VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD}={16}
#VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.RETIMING}={true}

VPP_LINK_FLAGS += --advanced.param compiler.userPostSysLinkOverlayTcl=$(DIRECTIVES_REPO)/post_sys_link.tcl

#VPP_LINK_FLAGS += --vivado.synth.jobs 32
#VPP_LINK_FLAGS += --vivado.impl.jobs 32

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
# (fft_2d.o) which is used in the make xsa step. 
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
	@echo ""
	@echo "AIE XPE Generation Complete..."
	@echo "####################################"
	@echo ""

# vcd file generation...
$(BLD_TGT_VCD_FILE): $(AIE_SRC_REPO)/aiesim_data/*
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) --profile --dump-vcd $(VCD_FILE_NAME) \
	--simulation-cycle-timeout 200000 2>&1 | tee -a vcd.log
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
#	vck190_aie_fft_2d.[hw_emu | hw].xsa
#	vck190_aie_fft_2d.[hw_emu | hw].xsa.info
#	vck190_aie_fft_2d.[hw_emu | hw].xsa.link_summary
#	vck190_aie_fft_2d.[hw_emu | hw].xsa
#	vck190_aie_fft_2d.[hw_emu | hw].log
xsa:  kernels graph $(BUILD_TARGET_DIR)/$(XSA)

$(BUILD_TARGET_DIR)/$(XSA):$(KERNEL_XOS) $(SYSTEM_CONFIGS_REPO)/*
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -t $(TARGET) -o $@ \
		    $(KERNEL_XOS) $(LIBADF_A)
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
#	fft_2d_aie_app.o
# 	fft_2d_aie_xrt.elf

application: graph $(BUILD_TARGET_DIR)/$(APP_ELF)

$(BUILD_TARGET_DIR)/$(APP_ELF): $(HOST_APP_SRC)/* $(LIBADF_A)
	@rm -rf $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/fft_2d_aie_app.o $(BUILD_TARGET_DIR)/$(APP_ELF)
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control.o
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/fft_2d_aie_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/fft_2d_aie_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF)
	@echo ""
	@echo "Host Application Compilation Complete..."
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
package: application xsa $(EMBEDDED_PACKAGE_OUT)

$(EMBEDDED_PACKAGE_OUT): $(PROFILING_CONFIGS_REPO)/* $(EXEC_SCRIPTS_REPO)/* $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XSA)
	rm -rf $(EMBEDDED_PACKAGE_OUT)
	cd $(BUILD_TARGET_DIR);	\
	v++ -p $(PKG_FLAGS)
	cp -rf $(BUILD_TARGET_DIR)/package $(BUILD_TARGET_DIR)/package_backup
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
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) -timeout 10800 | tee embedded_run.log
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
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
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
# ==> clean_fftpt
# ==> clean_fft2d_dt
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

#Clean generated files
clean_tgt:
	@echo "Cleaning $(TARGET) Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

#Clean fft2d instances dir files...
clean_insts:
	@echo "Cleaning $(FFT_2D_INSTS)x Builds..."
	rm -rf $(INSTS_BLD_DIR)

#Clean fft2d point dir files...
clean_fftpt:
	@echo "Cleaning $(MAT_ROWS)x$(MAT_COLS) FFT Builds..."
	rm -rf $(FFTPT_BLD_DIR)

#Clean fft2d datatype dir files...
clean_fft2d_dt:
	@echo "Cleaning $(FFT_2D_DT)_impl Builds..."
	rm -rf $(FFT_2D_DT_DIR)

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
