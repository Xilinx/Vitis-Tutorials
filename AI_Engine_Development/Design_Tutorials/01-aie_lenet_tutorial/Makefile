# © Copyright 2022 Xilinx, Inc.
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
	@echo  "  make clean_target"
	@echo  "  Command to remove the generated files for a specific target"
	@echo  ""
	@echo  "  make cleanall"
	@echo  "  Command to remove the generated files for all variations"
	@echo  ""
	@echo  "  make clean_vivado_reports"
	@echo  "  Command to remove the generated vivado reports files for all variations"
	@echo  ""
	@echo  "  make clean_xpe_reports"
	@echo  "  Command to remove the generated xpe files for all variations"
	@echo  ""
	@echo  "  make cleanall_reports"
	@echo  "  Command to remove all the generated reports for all variations"
	@echo  ""
	@echo  " OPTIONS:"
	@echo  " Use the make recipes with required values for options mentioned below-"
	@echo  "    TARGET             -specifies target: Hardware or Emulation. Values: hw_emu(default), hw"
	@echo  "    LENET_INSTS        -specifies number of LENET instances. Values: 1(default), 5 and 50"
	@echo  "    PL_FREQ        	 -specifies HLS kernel frequencies. Values: 250Mhz(default)"
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
# No of LENET Instances can be set as:
#   1(default).
# =========================================================
LENET_INSTS := 1

# =========================================================
# No of Design Iterations 
#   1(default).
# =========================================================
ITER_CNT := 1

# =========================================================
# PL Frequency in Mhz:
#   312.5Mhz(default)
# =========================================================
PL_FREQ := 312.5
HZ_UNIT := 1000000
VPP_CLOCK_FREQ := $(shell printf "%.0f" `echo "${PL_FREQ} * $(HZ_UNIT)" | bc`)
#VPP_CLOCK_FREQ := $(PL_FREQ)000000

# =========================================================
# Profiling/Trace and Datamover type Switches...
# =========================================================
EN_TRACE	:= 0

######################################
# When TARGET=hw_emu, hw_emu launch...
# 1 = Enabled (Default).
# 0 = Disabled.
######################################
LAUNCH_HW_EMU_EXEC := 0

############################################################
# maximum cycle count for aiesimulator to get powerfrom vcd
# default 100
# #########################################################
MAX_CYCLES   := 100000
# =========================================================
# Source directories
# =========================================================
# Relative lenet directory
RELATIVE_PROJECT_DIR := ./

# Absolute lenet directory = <user path>/Tutorials/AI_Engine/lenet
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
AIE_SRC_REPO := $(DESIGN_REPO)/aie_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
HOST_APP_SRC_REPO := $(DESIGN_REPO)/host_app_src

DIRECTIVES_REPO        := $(DESIGN_REPO)/directives
SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
EXEC_SCRIPTS_REPO      := $(DESIGN_REPO)/exec_scripts
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

BASE_BLD_DIR   	:= $(PROJECT_REPO)/build
INSTS_BLD_DIR     := $(BASE_BLD_DIR)/lenet_x$(LENET_INSTS)
BUILD_TARGET_DIR  := $(INSTS_BLD_DIR)/$(TARGET)

REPORTS_REPO := $(PROJECT_REPO)/reports_dir
BLD_REPORTS_DIR := $(REPORTS_REPO)/lenet_x$(LENET_INSTS)/$(TARGET)

XPE_REPO         := $(PROJECT_REPO)/xpe_dir
BLD_XPE_DIR      := $(XPE_REPO)/lenet_x$(LENET_INSTS)
VCD_FILE_NAME    := lenet_x$(LENET_INSTS)
BLD_TGT_VCD_FILE := $(BUILD_TARGET_DIR)/$(VCD_FILE_NAME).vcd
XPE_FILE         := $(BLD_XPE_DIR)/graph_$(VCD_FILE_NAME).xpe

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

LENET_KERNEL_TOP := lenet_kernel
LENET_KERNEL_XO  := $(PL_SRC_REPO)/$(LENET_KERNEL_TOP)/$(LENET_KERNEL_TOP).xo

LIBADF_A := $(BUILD_TARGET_DIR)/libadf.a

APP_OBJ_NAME := lenet_aie
APP_ELF := $(APP_OBJ_NAME)_xrt.elf
XSA := vck190_$(APP_OBJ_NAME).$(TARGET).xsa

# =========================================================
# Kernel Source Files repository
# =========================================================
DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/datamover/$(DATAMOVER_KERNEL_TOP).cpp

# =========================================================
# Graph Source files repository
# ========================================================
GRAPH_SRC_CPP := $(AIE_SRC_REPO)/graph.cpp

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP 	 := $(HOST_APP_SRC_REPO)/lenet_aie_app.cpp
AIE_CONTROL_CPP := $(BUILD_TARGET_DIR)/$(WORK_DIR)/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS := -include=$(AIE_SRC_REPO) 
AIE_FLAGS += -include=$(AIE_SRC_REPO)/data 
AIE_FLAGS += --verbose 
AIE_FLAGS += --Xpreproc="-DLENET_INSTS=$(LENET_INSTS)"
AIE_FLAGS += --Xpreproc="-DITER_CNT=$(ITER_CNT)"
AIE_FLAGS += --platform=$(PLATFORM)

AIE_FLAGS += --log-level=5 
#AIE_FLAGS += --test-iterations=100 
AIE_FLAGS += --pl-freq=500
AIE_FLAGS += --dataflow 

AIE_FLAGS += --heapsize=2048 
AIE_FLAGS += --workdir=$(WORK_DIR)

#AIE_FLAGS += --Xmapper=BufferOptLevel9
#AIE_FLAGS += --Xrouter=DMAFIFOsInFreeBankOnly

# Aiesimulator flags
#-i - Alias of --input-dir=<dir> option
#-o - Alias of --output-dir=<dir> option
AIE_SIM_FLAGS := --pkg-dir $(WORK_DIR)/
AIE_SIM_FLAGS += -i=$(AIE_SRC_REPO)/aiesim_data
AIE_SIM_FLAGS += --simulation-cycle-timeout $(MAX_CYCLES)
# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++17
GCC_FLAGS += -D__linux__
GCC_FLAGS += -D__PS_ENABLE_AIE__
GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -DITER_CNT=$(ITER_CNT)
GCC_FLAGS += -DLENET_INSTS=$(LENET_INSTS)

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt
GCC_INC_FLAGS += -I$(XILINX_VITIS)/aietools/include/
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include 
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_FLAGS += -I$(AIE_SRC_REPO)
GCC_INC_FLAGS += -I$(HOST_APP_SRC_REPO)

GCC_INC_LIB := -L$(SDKTARGETSYSROOT)/usr/lib 
GCC_INC_LIB += -L$(XILINX_VITIS)/aietools/lib/aarch64.o 
GCC_INC_LIB += -L$(XILINX_VITIS)/aietools/lib/lnx64.o

GCC_INC_LIB += -ladf_api_xrt 
GCC_INC_LIB += -lxrt_coreutil 

# =========================================================
# Kernel Compiler and Linker Flags
# ========================================================
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose 
VPP_FLAGS += -g

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)

VPP_LINK_FLAGS := --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_0
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(LENET_KERNEL_TOP)_0

VPP_LINK_FLAGS += --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --advanced.param compiler.userPostSysLinkOverlayTcl=$(DIRECTIVES_REPO)/noc_qos.tcl
VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/lenet_x$(LENET_INSTS).cfg

VPP_LINK_FLAGS += --vivado.prop run.synth_1.STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD=16

ifeq ($(EN_TRACE),1)
   ifeq ($(TARGET),hw)
      VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_TOP):all:all
      VPP_LINK_FLAGS += --profile.trace_memory DDR
   
   endif
endif

## Enabling Multiple Strategies For Closing Timing...
#VPP_LINK_FLAGS += --vivado.impl.strategies "ALL"
##VPP_LINK_FLAGS += --vivado.impl.lsf '{bsub -R "select[type=X86_64] rusage[mem=65536]" -N -q medium}'
#VPP_LINK_FLAGS += --advanced.param "compiler.enableMultiStrategies=1"
#VPP_LINK_FLAGS += --advanced.param "compiler.multiStrategiesWaitOnAllRuns=1"
#VPP_LINK_FLAGS += --vivado.synth.jobs 32
#VPP_LINK_FLAGS += --vivado.impl.jobs 20

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

# =========================================================
# Step 1. Kernel XO File Generation
# ========================================================
# This step compiles the HLS C PL kernels. 
# Outputs: in build/[hw_emu | hw]/ directory
# 	dma_hls.[hw_emu | hw].xo  
#	dma_hls.[hw_emu | hw].xo.compile_summary  
#	v++_dma_hls.[hw_emu | hw].log
#	_x
KERNEL_XOS := $(LENET_KERNEL_XO) $(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo

kernels: $(KERNEL_XOS)

$(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo:
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(DATAMOVER_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k $(DATAMOVER_KERNEL_TOP) \
		$(DATAMOVER_KERNEL_SRC) -o $@

# =========================================================
# Step 2. AI Engine SDF Graph File and Work/ Directory 
#         (containing the Graph Executable) Generation
# ========================================================
# This step creates an SDF Graph and the Work/ directory.
# The Work/ directory contains the graph executable 
# (lenet.o) which is used in the make xsa step.  
# The aiecompiler is invoked with the -target=hw. 
# Outputs: in build/ directory
#	libsdf.a 
#	NOC_Power.xpe
#	Work/ 
#	xnwOut/
graph: $(LIBADF_A)

$(LIBADF_A): $(AIE_SRC_REPO)/*
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP) 2>&1 | tee -a aiecompiler.log

aiesim: graph
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) 2>&1 | tee -a aiesim.log

aiesim_profile: graph
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) --profile 2>&1 | tee -a aiesim.log

vcd: graph $(XPE_FILE)

# xpe file generation...
$(XPE_FILE): $(BLD_TGT_VCD_FILE)
	cd $(BUILD_TARGET_DIR); \
	vcdanalyze --vcd $(VCD_FILE_NAME).vcd --xpe
	rm -rf $(BLD_XPE_DIR)
	mkdir -p $(BLD_XPE_DIR)
	cp -rf $(BUILD_TARGET_DIR)/aiesim_xpe/*.xpe $(XPE_FILE)

# vcd file generation...
$(BLD_TGT_VCD_FILE):
	cd $(BUILD_TARGET_DIR); \
	aiesimulator $(AIE_SIM_FLAGS) --dump-vcd $(VCD_FILE_NAME) 2>&1 | tee -a vcd.log

# =========================================================
# Step 3. XSA File Generation
# ========================================================
# This step links the graph executable (tx_chain.o) and 
# the kernels into a xsa file. 
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_aie_lenet.[hw_emu | hw].xsa
#	vck190_aie_lenet.[hw_emu | hw].xsa.info
#	vck190_aie_lenet.[hw_emu | hw].xsa.link_summary
#	vck190_aie_lenet.[hw_emu | hw].xsa
#	vck190_aie_lenet.[hw_emu | hw].log
xsa: kernels graph $(BUILD_TARGET_DIR)/$(XSA)

$(BUILD_TARGET_DIR)/$(XSA):$(KERNEL_XOS) $(SYSTEM_CONFIGS_REPO)/*
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -t $(TARGET) -o $@ \
		    $(KERNEL_XOS) $(LIBADF_A)

# =========================================================
# Step 4. A72 Application Executable File Generation
# ========================================================
# This step compiles the A72 application. This step is the  
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# For finite run...
# 	aie_control.o
#	lenet_app.o 
# 	lenet_xrt.elf
application: graph $(BUILD_TARGET_DIR)/$(APP_ELF)

$(BUILD_TARGET_DIR)/$(APP_ELF): $(HOST_APP_SRC)/* $(LIBADF_A)
	@rm -rf $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/$(APP_OBJ_NAME)_app.o $(BUILD_TARGET_DIR)/$(APP_ELF)
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_TARGET_DIR)/app_control.o
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/$(APP_OBJ_NAME)_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/$(APP_OBJ_NAME)_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF)

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

$(EMBEDDED_PACKAGE_OUT): $(PROFILING_CONFIGS_REPO)/* $(EXEC_SCRIPTS_REPO)/* $(ALL_APP_ELFS)
	rm -rf $(EMBEDDED_PACKAGE_OUT)
	cd $(BUILD_TARGET_DIR);	\
	v++ -p $(PKG_FLAGS)
	#cp -rf $(EMBEDDED_PACKAGE_OUT) $(EMBEDDED_PACKAGE_OUT)_backup

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
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) -no-reboot -timeout 4000 | tee embedded_run.log

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
report_metrics: $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl 
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"

else
	rm -rf $(BLD_REPORTS_DIR)
	mkdir -p $(BLD_REPORTS_DIR)
	cd $(BLD_REPORTS_DIR); \
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr

endif

###########################################################################

# =========================================================
# Primary Build Targets
# ==> sd_card
# ==> run
# ==> all
# ==> clean_target
# ==> cleanall
# ==> clean_vivado_reports
# ==> clean_xpe_reports
# ==> cleanall_reports
# ========================================================

#Build the design without running the hardware emulation
sd_card: package

#Build the design and then run hardware emulation 
run: sd_card run_emu

#Same as "run" above, but include metrics generation and
#vcd generation
all: run report_metrics vcd

#Clean generated files
clean_target:
	@echo "Cleaning $(TARGET) Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

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
