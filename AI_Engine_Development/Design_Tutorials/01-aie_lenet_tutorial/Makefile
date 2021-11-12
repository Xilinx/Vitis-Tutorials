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





print-%  : ; @echo $* = $($*)



# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu



# =========================================================
# Profiling/Trace and Datamover type Switches...
# =========================================================
EN_TRACE	:= 0

######################################
# When TARGET=hw_emu, hw_emu launch...
# 1 = Enabled (Default).
# 0 = Disabled.
######################################
LAUNCH_HW_EMU_EXEC := 1

# =========================================================
# Source directory
# =========================================================
#relative lenet directory
RELATIVE_PROJECT_DIR := ./

#absolute lenet directory = <user path>/Tutorials/AI_Engine/lenet
PROJECT_REPO	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

AIE_SRC_REPO 	:= $(PROJECT_REPO)/design/aie_src
PL_SRC_REPO 	:= $(PROJECT_REPO)/design/pl_src
MAKEFILES_REPO 	:= $(PROJECT_REPO)
BUILD_DIR 	:= $(PROJECT_REPO)/build
WORK_DIR	:= Work
BUILD_TARGET_DIR := $(BUILD_DIR)/$(TARGET)

EMBEDDED_PACKAGE_OUT = $(BUILD_DIR)/$(TARGET)/package
EMBEDDED_EXEC_SCRIPT = run_script.sh


# ==========================================================
# Below are the names for SDF graph, application executable,
# kernel executables, and xclbin
# ==========================================================
DMA_KERNEL_XO 	:= dma_hls.$(TARGET)
LIBADF_A 	:= $(BUILD_DIR)/libadf.a
APP_ELF 	:= lenet_xrt.elf
LENET_O 	:= $(BUILD_DIR)/Work/temp/graph.o 
LENET_KERNEL_XO := $(PL_SRC_REPO)/lenet_kernel/lenet_kernel.xo
XCLBIN 		:= vck190_aie_lenet.$(TARGET).xclbin

# =========================================================
# Kernel Source Files repository
# =========================================================
DMA_KERNEL_SRC := $(PL_SRC_REPO)/datamover/dma_hls.cpp

# =========================================================
# Graph Source files repository
# ========================================================
GRAPH_SRC_CPP := $(AIE_SRC_REPO)/graph.cpp

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP 	:= $(AIE_SRC_REPO)/main.cpp
AIE_CONTROL_CPP := $(BUILD_DIR)/Work/ps/c_rts/aie_control_xrt.cpp

# =========================================================
# AIE Compiler Global Settings and Include Libraries
# =========================================================
AIE_FLAGS := --platform=$(PLATFORM)
AIE_FLAGS += -include=$(AIE_SRC_REPO) 
AIE_FLAGS += -include=$(AIE_SRC_REPO)/data 
AIE_FLAGS += --verbose 

AIE_FLAGS += --log-level=5 
AIE_FLAGS += --test-iterations=100 
AIE_FLAGS += --dataflow 

AIE_FLAGS += --heapsize=2048 
AIE_FLAGS += --workdir=$(WORK_DIR)

AIE_FLAGS += --pl-freq=500


# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__

GCC_FLAGS += -DXAIE_DEBUG
GCC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include/xrt

GCC_FLAGS += -I$(XILINX_VITIS)/aietools/include/
GCC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include 
GCC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib




GCC_LIB += -L$(SDKTARGETSYSROOT)/usr/lib 
GCC_LIB += -L$(XILINX_VITIS)/aietools/lib/aarch64.o 
GCC_LIB += -L$(XILINX_VITIS)/aietools/lib/lnx64.o 
GCC_LIB += -ladf_api_xrt 
GCC_LIB += -lxrt_coreutil 

GCC_LIB += -std=c++14


CXX_FLAGS :=-mcpu=cortex-a72.cortex-a53
CXX_FLAGS +=-march=armv8-a+crc
CXX_FLAGS +=-fstack-protector-strong
CXX_FLAGS +=-D_FORTIFY_SOURCE=2
CXX_FLAGS +=-Wformat
CXX_FLAGS +=-Wformat-security
CXX_FLAGS +=-Werror=format-security
CXX_FLAGS +=--sysroot=${SDKTARGETSYSROOT}

# =========================================================
# Kernel Compiler and Linker Flags
# ========================================================
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose 
VPP_FLAGS += -g

VPP_LINK_FLAGS := --config $(MAKEFILES_REPO)/system.cfg


ifneq ($(EN_TRACE),0)
	VPP_LINK_FLAGS += --profile.data dma_hls:all:all
	VPP_LINK_FLAGS += --profile.trace_memory DDR
endif


PKG_FLAGS := 
ifneq ($(EN_TRACE),0)

	PKG_FLAGS += --package.sd_file $(MAKEFILES_REPO)/xrt.ini

endif




# =========================================================
# Step 1. Kernel XO File Generation
# ========================================================
# This step compiles the HLS C PL kernels. 
# Outputs: in build/[hw_emu | hw]/ directory
# 	dma_hls.[hw_emu | hw].xo  
#	dma_hls.[hw_emu | hw].xo.compile_summary  
#	v++_dma_hls.[hw_emu | hw].log
#	_x
kernels: $(BUILD_TARGET_DIR)/$(DMA_KERNEL_XO).xo

$(BUILD_TARGET_DIR)/$(DMA_KERNEL_XO).xo:
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(VPP_FLAGS) --hls.clock 250000000:dma_hls -c -k dma_hls $(DMA_KERNEL_SRC) -o $@

# =========================================================
# Step 2. AI Engine SDF Graph File and Work/ Directory 
#         (containing the Graph Executable) Generation
# ========================================================
# This step creates an SDF Graph and the Work/ directory.
# The Work/ directory contains the graph executable 
# (lenet.o) which is used in the make xclbin step.  
# The aiecompiler is invoked with the -target=hw. 
# Outputs: in build/ directory
#	libsdf.a 
#	NOC_Power.xpe
#	Work/ 
#	xnwOut/
graph: $(BUILD_DIR)/$(LIBADF_A) 

$(BUILD_DIR)/$(LIBADF_A): $(GRAPH_SRC_CPP)
	mkdir -p $(BUILD_DIR); 	\
	cd $(BUILD_DIR); 	\
	aiecompiler $(AIE_FLAGS) $(GRAPH_SRC_CPP) 

# =========================================================
# Step 3. XCLBIN File Generation
# ========================================================
# This step links the graph executable (tx_chain.o) and 
# the kernels into a xclbin file. 
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_aie_lenet.[hw_emu | hw].xclbin
#	vck190_aie_lenet.[hw_emu | hw].xclbin.info
#	vck190_aie_lenet.[hw_emu | hw].xclbin.link_summary
#	vck190_aie_lenet.[hw_emu | hw].xsa
#	vck190_aie_lenet.[hw_emu | hw].log
xclbin: $(BUILD_TARGET_DIR)/$(XCLBIN)

$(BUILD_TARGET_DIR)/$(XCLBIN): 	$(BUILD_TARGET_DIR)/$(DMA_KERNEL_XO).xo \
				$(LENET_KERNEL_XO) 			\
				$(LIBADF_A)
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) --advanced.param compiler.userPostSysLinkTcl=$(PROJECT_REPO)/noc_qos.tcl -t $(TARGET) -o $@ 	\
		$(BUILD_TARGET_DIR)/$(DMA_KERNEL_XO).xo 		\
		$(LENET_KERNEL_XO) 					\
		$(LIBADF_A) 

# =========================================================
# Step 4. A72 Application Executable File Generation
# ========================================================
# This step compiles the A72 application. This step is the  
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
# 	aie_control.o
#	lenet_app.o 
# 	lenet_xrt.elf
application:
	$(CXX) $(CXX_FLAGS) $(GCC_FLAGS) $(AIE_CONTROL_CPP) -o $(BUILD_DIR)/app_control.o
	$(CXX) $(CXX_FLAGS) $(GCC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_DIR)/lenet_app.o $(GCC_LIB)
	$(CXX) $(CXX_FLAGS) $(BUILD_DIR)/app_control.o $(BUILD_DIR)/lenet_app.o $(GCC_LIB) -o $(BUILD_DIR)/$(APP_ELF) 


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
package:
	cp $(MAKEFILES_REPO)/run_script.sh $(BUILD_TARGET_DIR)/
	cd $(BUILD_TARGET_DIR);	\
	v++ -p -t $(TARGET) --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x	\
		-f $(PLATFORM) 							\
		--package.rootfs $(EDGE_COMMON_SW)/rootfs.ext4                  \
		--package.kernel_image $(EDGE_COMMON_SW)/Image                  \
		--package.boot_mode=sd 						\
		--package.out_dir $(BUILD_TARGET_DIR)/package 			\
		--package.sd_dir $(AIE_SRC_REPO)/data 				\
		--package.image_format=ext4 					\
		--package.sd_file $(BUILD_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XCLBIN) $(LIBADF_A) $(SD_FILES_WITH_PREFIX) \
		--package.sd_file $(EMBEDDED_EXEC_SCRIPT) \
		--package.defer_aie_run                                                                                     \
		$(PKG_FLAGS)


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

endif
else
	@echo ""
	@echo "###########################################################################"
	@echo "Hardware build, no emulation executed."
	@echo ""
	@echo "Use sd_card.img from below directory and, follow the steps in README.md for execution on board."
	@echo "$(EMBEDDED_PACKAGE_OUT)"
	@echo ""

endif###########################################################################

# =========================================================
# Primary Build Targets
# ==> build
# ==> run
# ==> clean
# ========================================================

#Build the design without running the hardware emulation
sd_card: kernels graph xclbin application package

#Build the design and then run hardware emulation 
run: sd_card run_emu

#Clean generated files
clean:
	rm -rf .Xil/
	rm -rf $(BUILD_DIR)/$(WORK_DIR)
	rm -rf $(BUILD_DIR)/hw_emu
	rm -rf $(BUILD_DIR)/hw
	rm -rf $(BUILD_DIR)
	rm -f xcd.log
	rm -f xrc.log
