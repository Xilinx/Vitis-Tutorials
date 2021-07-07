#    (c) Copyright 2020 Xilinx, Inc. All rights reserved.
#   
#    This file contains confidential and proprietary information
#    of Xilinx, Inc. and is protected under U.S. and
#    international copyright and other intellectual property
#    laws.
#   
#    DISCLAIMER
#    This disclaimer is not a license and does not grant any
#    rights to the materials distributed herewith. Except as
#    otherwise provided in a valid license issued to you by
#    Xilinx, and to the maximum extent permitted by applicable
#    law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#    WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#    AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#    BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#    INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#    (2) Xilinx shall not be liable (whether in contract or tort,
#    including negligence, or under any other theory of
#    liability) for any loss or damage of any kind or nature
#    related to, arising under or in connection with these
#    materials, including for any direct, or any indirect,
#    special, incidental, or consequential loss or damage
#    (including loss of data, profits, goodwill, or any type of
#    loss or damage suffered as a result of any action brought
#    by a third party) even if such damage or loss was
#    reasonably foreseeable or Xilinx had been advised of the
#    possibility of the same.
#   
#    CRITICAL APPLICATIONS
#    Xilinx products are not designed or intended to be fail-
#    safe, or for use in any application requiring fail-safe
#    performance, such as life-support or safety devices or
#    systems, Class III medical devices, nuclear facilities,
#    applications related to the deployment of airbags, or any
#    other applications that could lead to death, personal
#    injury, or severe property or environmental damage
#    (individually and collectively, "Critical
#    Applications"). Customer assumes the sole risk and
#    liability of any use of Xilinx products in Critical
#    Applications, subject only to applicable laws and
#    regulations governing limitations on product liability.
#   
#    THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#    PART OF THIS FILE AT ALL TIMES.                       

%.PHONY: help
help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make kernels TARGET=<hw_emu/hw>"
	@echo  "  Command to generate the PL kernels"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make graph"
	@echo  "  Command to generate the AI Engine graph"
	@echo  ""
	@echo  "  make xclbin TARGET=<hw_emu/hw>"
	@echo  "  Command to generate the xclbin for specified target"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make application"
	@echo  "  Command to generate the A72 application"
	@echo  ""
	@echo  "  make package TARGET=<hw_emu/hw>"
	@echo  "  Command to package design into an sd_card for specified target"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make clean"
	@echo  "  Command to remove the generated files"
	@echo  ""
	@echo  ""


EMBEDDED_PACKAGE_OUT=./out_dir
EMBEDDED_EXEC_SCRIPT=./run_script.sh

#Check environment setup
ifndef PLATFORM_REPO_PATHS
  PLATFORM_REPO_PATHS=/proj/xbuilds/${TOOLVERSION}/internal_platforms/
  export PLATFORM_REPO_PATHS
endif
ifndef XILINX_VITIS
  XILINX_VITIS = /opt/xilinx/Vitis/$(TOOL_VERSION)
  export XILINX_VITIS
endif
ifndef XILINX_XRT
  XILINX_XRT = /opt/xilinx/xrt
  export XILINX_XRT
endif


EMBEDDED_PACKAGE_OUT = $(BUILD_DIR)/$(TARGET)/package
EMBEDDED_EXEC_SCRIPT = run_script.sh
PACKAGE_FILES += $(EMBEDDED_EXEC_SCRIPT)
SD_FILES_WITH_PREFIX = $(foreach sd_file,$(PACKAGE_FILES),--package.sd_file $(sd_file))
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')
PERL ?= /tools/xgs/perl/5.8.5/bin/perl

RESULT_VAL ?= 
RUN_LEVEL ?= 7

print-%  : ; @echo $* = $($*)



# =========================================================
# platform selection
# =========================================================
PLATFORM := xilinx_vck190_base_202110_1

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw

# =========================================================
# Choose the compiler based on the A72 Processor in the 
# Versal device
# =========================================================
#HOST_CXX :=aarch64-linux-gnu-g++

export LD_LIBRARY_PATH := $(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/lib64:$(LD_LIBRARY_PATH)
CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++

# =========================================================
# Choose the XRT and XAIENGINE
# =========================================================
SYSROOT_PATH :=$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal-common-v2021.1/sysroots/aarch64-xilinx-linux
XRT_PATH :=$(SYSROOT_PATH)/usr/include/xrt

# =========================================================
# Source directory
# =========================================================
#relative tx_chain directory
RELATIVE_PROJECT_DIR := ./

#absolute tx_chain directory = <user path>/Tutorials/AI_Engine/tx_chain
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))
AIE_SRC_REPO	:= $(PROJECT_DIR)/design/aie_src
KERNEL_SRC_REPO := $(PROJECT_DIR)/design/kernel_src 
MAKEFILES_DIR 	:= $(PROJECT_DIR)
BUILD_DIR 	:= $(PROJECT_DIR)/build
WORK_DIR	:= $(PROJECT_DIR)/build/Work
BUILD_TARGET_DIR := $(BUILD_DIR)/$(TARGET)

# =========================================================
# COMMSLIB and Local Libraries
# =========================================================
COMM_INC	:= $(COMMSLIB_ROOT)/common/inc
COMM_SRC	:= $(COMMSLIB_ROOT)/common/src 
FILTERS_SRC	:= $(COMMSLIB_ROOT)/filters/src
FILTERS_INC	:= $(COMMSLIB_ROOT)/filters/inc
RADIO_SRC	:= $(COMMSLIB_ROOT)/radio/src
RADIO_INC	:= $(COMMSLIB_ROOT)/radio/inc
LOCAL_INC	:= $(AIE_SRC_REPO)/local/inc
LOCAL_SRC	:= $(AIE_SRC_REPO)/local/src
LOCAL_COMM_INC	:= $(AIE_SRC_REPO)/common/inc
LOCAL_COMM_SRC	:= $(AIE_SRC_REPO)/common/src
DSP_INC	        := $(DSPLIB_ROOT)/L2/include/hw
DSP_SRC	        := $(DSPLIB_ROOT)/L1/src/hw
DSP_TESTS       := $(DSPLIB_ROOT)/L1/tests/inc
DSP_HW          := $(DSPLIB_ROOT)/L1/include/hw


# ==========================================================
# Below are the names for SDF graph, application executable,
# kernel executables, and xclbin
# ==========================================================
LIBADF_A		:= $(BUILD_DIR)/libadf.a
TX_CHAIN_O 		:= $(BUILD_DIR)/Work/temp/tx_chain.o
APP_ELF 		:= tx_chain_xrt.elf
PCCFR_KERNEL1_XO 	:= pccfr_pl_pds_itr1.$(TARGET)
PCCFR_KERNEL2_XO 	:= pccfr_pl_pds_itr2.$(TARGET)
WCFR_KERNEL_XO 		:= wcfr_peak_d.$(TARGET)
S2MM_XO 		:= s2mm.$(TARGET)
MM2S_XO 		:= mm2s.$(TARGET)
XCLBIN 			:= vck190_aie_tx_chain.$(TARGET).xclbin

# =========================================================
# Kernel Source Files repository
# =========================================================
PCCFR_KERNEL1_SRC	:= ./design/kernel_src/pccfr_pds_itr1.cpp
PCCFR_KERNEL2_SRC	:= ./design/kernel_src/pccfr_pds_itr2.cpp
WCFR_KERNEL_SRC		:= ./design/kernel_src/wcfr_pd.cpp
S2MM_SRC		:= ./design/kernel_src/s2mm.cpp
MM2S_SRC		:= ./design/kernel_src/mm2s.cpp

# =========================================================
# Graph Source files repository
# ========================================================
GRAPH_SRC_CPP	:= $(AIE_SRC_REPO)/tx_chain.cpp 
GRAPH_SRC_H	:= $(AIE_SRC_REPO)/tx_chain.h 

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP := $(AIE_SRC_REPO)/tx_chain_app.cpp 
APP_CONTROL_CPP := $(BUILD_DIR)/Work/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS := --include=$(COMM_INC) 
AIE_FLAGS += --include=$(COMM_SRC) 
AIE_FLAGS += --include=$(FILTERS_INC) 
AIE_FLAGS += --include=$(FILTERS_SRC) 
AIE_FLAGS += --include=$(RADIO_INC) 
AIE_FLAGS += --include=$(RADIO_SRC) 
AIE_FLAGS += --include=$(LOCAL_INC) 
AIE_FLAGS += --include=$(LOCAL_SRC) 
AIE_FLAGS += --include=$(LOCAL_COMM_INC)
AIE_FLAGS += --include=$(LOCAL_COMM_SRC)
AIE_FLAGS += --include=$(DSP_INC) 
AIE_FLAGS += --include=$(DSP_SRC) 
AIE_FLAGS += --include=$(DSP_TESTS) 
AIE_FLAGS += --include=$(DSP_HW)
AIE_FLAGS += --nodot-graph=true
AIE_FLAGS += -v
AIE_FLAGS += --Xchess=main:llvm.xargs=-Wno-shift-negative-value 
AIE_FLAGS += --Xchess="gain_magsqr:cpp.define=USER_PARAMS"
AIE_FLAGS += --Xpreproc=-DDOPWRSQRT 
AIE_FLAGS += --Xpreproc=-DABSCONSTRAINTS 
AIE_FLAGS += --Xmapper=BufferOptLevel7 
AIE_FLAGS += --log-level=5 
AIE_FLAGS += --pl-axi-lite=false 
AIE_FLAGS += --workdir=$(WORK_DIR) 

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__PS_ENABLE_AIE__ 
GCC_FLAGS += -D__linux__ 
GCC_FLAGS += --sysroot=$(SYSROOT_PATH)
GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -I$(AIE_SRC_REPO)/local/inc/ 
GCC_FLAGS += -I$(COMM_INC) 
GCC_FLAGS += -I$(COMM_SRC) 
GCC_FLAGS += -I$(FILTERS_INC) 
GCC_FLAGS += -I$(FILTERS_SRC) 
GCC_FLAGS += -I$(RADIO_INC) 
GCC_FLAGS += -I$(RADIO_SRC) 
GCC_FLAGS += -I$(LOCAL_INC) 
GCC_FLAGS += -I$(LOCAL_SRC) 
GCC_FLAGS += -I$(LOCAL_COMM_INC)  
GCC_FLAGS += -I$(LOCAL_COMM_SRC)
GCC_FLAGS += -I$(DSP_INC) 
GCC_FLAGS += -I$(DSP_SRC) 
GCC_FLAGS += -I$(DSP_TESTS)  
GCC_FLAGS += -I$(DSP_HW)
GCC_FLAGS += -I$(XRT_PATH) 
GCC_FLAGS += -I$(XILINX_VITIS)/aietools/include/ 
GCC_FLAGS += -I=$(SYSROOT_PATH)/usr/include 
GCC_FLAGS += -I=$(SYSROOT_PATH)/usr/lib 
GCC_FLAGS += -I$(XILINX_HLS)/include

GCC_LIB := --sysroot=$(SYSROOT_PATH) 
GCC_LIB += -L$(SYSROOT_PATH)/usr/lib 
GCC_LIB += -L$(XILINX_VITIS)/aietools/lib/aarch64.o 
GCC_LIB += -L$(XILINX_VITIS)/aietools/lib/lnx64.o 
GCC_LIB += -ladf_api_xrt 
GCC_LIB += -lxrt_coreutil 
GCC_LIB += -std=c++14 

# =========================================================
# Kernel Compiler and Linker Flags
# ========================================================
VPP_FLAGS := --platform $(PLATFORM) 
VPP_FLAGS += --save-temps 
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x 
VPP_FLAGS += --verbose 
VPP_FLAGS += -g

# =========================================================
# Step 1. Kernel XO File Generation
# ========================================================
# This step compiles the HLS C PL kernels. 
# Outputs: 
#	mm2s.xo 		mm2s.compile_summary
#	pccfr_pl_psd_itr1.xo 	pccfr_pl_psd_itr1.compile_summary
#	pccfr_pl_psd_itr2.xo 	pccfr_pl_psd_itr2.compile_summary
#	s2mm.xo			s2mm.compile_summary
#	tx_chain.xo
#	wcfr_peak_d.xo		wcfr_peak_d.compile_summary
#	_x/
#	v++<KERNEL_NAME>.log
kernels: $(BUILD_TARGET_DIR)/$(PCCFR_KERNEL1_XO).xo		\
		$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL2_XO).xo 	\
		$(BUILD_TARGET_DIR)/$(WCFR_KERNEL_XO).xo 	\
		$(BUILD_TARGET_DIR)/$(S2MM_XO).xo 		\
		$(BUILD_TARGET_DIR)/$(MM2S_XO).xo

$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL1_XO).xo: $(PCCFR_KERNEL1_SRC)
	mkdir -p $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(VPP_FLAGS) --config $(MAKEFILES_DIR)/pccfr_pds_itr1.cfg -c -k pccfr_pl_pds_itr1 $(PCCFR_KERNEL1_SRC) -o $@  
	mv v++_pccfr_pl_pds_itr1.$(TARGET).log $(BUILD_TARGET_DIR)/.

$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL2_XO).xo: $(PCCFR_KERNEL2_SRC)
	v++ --target $(TARGET) $(VPP_FLAGS) --config $(MAKEFILES_DIR)/pccfr_pds_itr2.cfg -c -k pccfr_pl_pds_itr2 $(PCCFR_KERNEL2_SRC) -o $@  
	mv v++_pccfr_pl_pds_itr2.$(TARGET).log $(BUILD_TARGET_DIR)/.

$(BUILD_TARGET_DIR)/$(WCFR_KERNEL_XO).xo: $(WCFR_KERNEL_SRC)
	v++ --target $(TARGET) $(VPP_FLAGS) --config $(MAKEFILES_DIR)/wcfr_peak_d.cfg -c -k wcfr_peak_d $(WCFR_KERNEL_SRC) -o $@   
	mv v++_wcfr_peak_d.$(TARGET).log $(BUILD_TARGET_DIR)/.

$(BUILD_TARGET_DIR)/$(S2MM_XO).xo: $(S2MM_SRC)
	v++ --target $(TARGET) $(VPP_FLAGS) -c -k s2mm $(S2MM_SRC) -o $@   
	mv v++_s2mm.$(TARGET).log $(BUILD_TARGET_DIR)/.

$(BUILD_TARGET_DIR)/$(MM2S_XO).xo: $(MM2S_SRC)
	v++ --target $(TARGET) $(VPP_FLAGS) -c -k mm2s $(MM2S_SRC) -o $@   
	mv v++_mm2s.$(TARGET).log $(BUILD_TARGET_DIR)/.

# =========================================================
# Step 2. AI Engine SDF Graph File and Work/ Directory 
#         (containing the Graph Executable) Generation
# ========================================================
# This step creates an SDF Graph and the Work/ directory.
# The Work/ directory contains the graph executable 
# (tx_chain.o) which is used in the make xclbin step.  
# The aiecompiler is invoked with the -target=aiesim. 
# Outputs: 
#	build/libadf.a 
#	build/NOC_Power.xpe
#	build/xnwOut/
#	build/Work/ 
graph: $(BUILD_DIR)/$(LIBADF_A) 

$(BUILD_DIR)/$(LIBADF_A): $(GRAPH_SRC_CPP) $(GRAPH_SRC_H)
	mkdir -p $(BUILD_DIR);\
	cd $(BUILD_DIR); \
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP) 2>&1 | tee -a aiecompiler.log


# =========================================================
# Step 3. XCLBIN File Generation
# ========================================================
# This step links the graph executable (tx_chain.o) and 
# the kernels into a xclbin file. 
# Outputs: 
#	vck190_aie_tx_chain.xsa
#  	v++_vck190_aie_tx_chain.log
#	vck190_aie_tx_chain.xclbin	
#	vck190_aie_tx_chain.xclbin.info
#	vck190_aie_tx_chain.xclbin.link_summary
xclbin: $(BUILD_TARGET_DIR)/$(XCLBIN)

$(BUILD_TARGET_DIR)/$(XCLBIN): 	$(BUILD_TARGET_DIR)/$(S2MM_XO).xo		\
				$(BUILD_TARGET_DIR)/$(MM2S_XO).xo 		\
				$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL1_XO).xo	\
				$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL2_XO).xo 	\
				$(BUILD_TARGET_DIR)/$(WCFR_KERNEL_XO).xo 	\
				$(LIBADF_A) 					
	v++ -l $(VPP_FLAGS) --config $(MAKEFILES_DIR)/system.cfg -t $(TARGET) -o $@\
		$(BUILD_TARGET_DIR)/$(S2MM_XO).xo		\
		$(BUILD_TARGET_DIR)/$(MM2S_XO).xo 		\
		$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL1_XO).xo 	\
		$(BUILD_TARGET_DIR)/$(PCCFR_KERNEL2_XO).xo 	\
		$(BUILD_TARGET_DIR)/$(WCFR_KERNEL_XO).xo 	\
		$(LIBADF_A)
	mv vck190_aie_tx_chain.$(TARGET).xsa $(BUILD_TARGET_DIR)/.
	mv v++_vck190_aie_tx_chain.$(TARGET).log $(BUILD_TARGET_DIR)/.

# =========================================================
# Step 4. A72 Application Executable File Generation
# ========================================================
# This step compiles the A72 application. This step is the  
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: aie_control.o, tx_chain_app.o, tx_chain_xrt.elf
application: $(APP_SRC_CPP) $(APP_CONTROL_CPP)
	 $(CXX) $(GCC_FLAGS) $(APP_CONTROL_CPP) -o $(BUILD_DIR)/aie_control_xrt.o
	 $(CXX) $(GCC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_DIR)/tx_chain_app.o $(GCC_LIB)
	 $(CXX) $(BUILD_DIR)/aie_control_xrt.o $(BUILD_DIR)/tx_chain_app.o $(GCC_LIB) -o $(BUILD_DIR)/$(APP_ELF)

# =========================================================
# Step 5. Package Generation  
# ========================================================
# This step generates the package folder which contains the 
# ./launch_hw_emu.sh script to launch hardware emulation 
# if TARGET=hw_emu and the sd_card.img file.  
# Outputs: 
# 	a.xclbin
# 	v++.package_summary
# 	v++_package.log 
# 	package/
package:
	cp $(MAKEFILES_DIR)/run_script.sh $(BUILD_TARGET_DIR)/
	v++ -p  -t $(TARGET) --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x 					\
		-f $(PLATFORM) 											\
		--package.sd_dir $(PLATFORM_REPO_PATHS)/sw/versal/xrt 						\
		--package.rootfs $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal-common-v2021.1/rootfs.ext4 	\
		--package.kernel_image $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal-common-v2021.1/Image 	\
		--package.boot_mode=sd 										\
		--package.out_dir $(BUILD_TARGET_DIR)/package 							\
		--package.sd_dir $(AIE_SRC_REPO)/data 								\
		--package.sd_dir $(AIE_SRC_REPO)/versal_run    					            	\
		--package.image_format=ext4 									\
		--package.sd_file $(BUILD_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XCLBIN) $(LIBADF_A) $(SD_FILES_WITH_PREFIX) \
		--package.defer_aie_run 
	mv a.xclbin $(BUILD_TARGET_DIR)/.
	mv v++.package_summary $(BUILD_TARGET_DIR)/.
	mv v++_package.log $(BUILD_TARGET_DIR)/.

# =========================================================
# Step 6. Run Hardware Emulation  
# ========================================================
# If the target is for HW_EMU, launch the emulator
# If the target is for HW, you'll have to follow the 
# instructions in the README.md 


sd_card: kernels graph xclbin application package

run_emu: 
ifeq ($(TARGET),hw_emu)
	cd $(BUILD_TARGET_DIR)/package;\
	$(PERL) $(XF_PROJ_ROOT)/ext/make_utility/embedded_run_emulation.pl "$(EMBEDDED_PACKAGE_OUT)/launch_$(TARGET).sh -no-reboot" "./run_script.sh" "$(RESULT_VAL)" "$(RUN_LEVEL)" | tee embedded_run.log 
else
	@echo "Hardware build, no emulation executed."
endif
###########################################################################


# =========================================================
# Primary Build Targets
# ==> build
# ==> run
# ==> clean
# ========================================================

.PHONY: all clean

#Build the design without running the hardware emulation
build: kernels graph xclbin application package

#Build the design and then run hardware emulation 
run: build run_emu

#Clean generated files
clean:
	rm -rf .Xil/
	rm -rf $(WORK_DIR)
	rm -rf $(BUILD_DIR)/hw_emu
	rm -rf $(BUILD_DIR)/hw
	rm -rf $(BUILD_DIR)
	rm -f xcd.log
	rm -f xrc.log

# =========================================================
# Other Tutorial Targets
# => aiesimulator
# => x86simulator
# ========================================================
aiesim: 
	cd $(BUILD_DIR);\
	aiesimulator --pkg-dir=$(WORK_DIR) --ps-app-cycle-timeout 500000
