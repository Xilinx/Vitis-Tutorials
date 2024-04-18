#
# Copyright 2019-2021 Xilinx, Inc.
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
# makefile-generator v1.0.3
#

############################## Help Section ##############################
ifneq ($(findstring Makefile, $(MAKEFILE_LIST)), Makefile)
help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to generate the design for specified Target and Shell."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> EMU_PS=<X86/QEMU> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to run application in emulation.Default sw_emu will run on x86 ,to launch on qemu specify EMU_PS=QEMU."
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) ""
	$(ECHO) "  make sd_card TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to prepare sd_card files."
	$(ECHO) ""
	$(ECHO) "  make host TARGET=<sw_emu/hw_emu/hw> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to build host application."
	$(ECHO) "      EDGE_COMMON_SW is required for SoC shells. Please download and use the pre-built image from - "
	$(ECHO) "      https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html"
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
endif

############################## Setting up Project Variables ##############################
TARGET := hw_emu
SYSROOT := $(EDGE_COMMON_SW)/sysroots/cortexa72-cortexa53-xilinx-linux
SD_IMAGE_FILE := $(EDGE_COMMON_SW)/Image
include ./utils.mk

EMU_PS := QEMU
ifeq ($(TARGET), sw_emu)
EMU_PS := X86
endif

ifeq ($(EMU_PS),X86)
CXX := g++
else
CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
endif

#Check for EMU_PS
ifeq ($(TARGET), $(filter $(TARGET),hw_emu hw))
ifeq ($(EMU_PS), X86)
$(error For hw_emu and hw, the design has to run on QEMU. Thus, please give EMU_PS=QEMU for these targets.)
endif
endif

# Makefile input options
LINK_OUTPUT := adder.xsa
XCLBIN := krnl_adder.xclbin

dfx_hw := false
dfx_chk := $(shell $(XF_PROJ_ROOT)/common/utility/custom_dfx_check.sh $(PLATFORM) $(XF_PROJ_ROOT))
ifeq ($(dfx_chk), true)
ifeq ($(TARGET), hw)
dfx_hw := true
endif
endif

# File names and locations
GRAPH := src/aie/graph.cpp
GRAPH_O := libadf.a

KERNEL := s2mm.cpp mm2s.cpp 
KERNEL_XO := s2mm.xo mm2s.xo 

HOST_SRCS := ./src/sw/host.cpp
EXECUTABLE = ./aie_adder

# SoC variables
RUN_APP_SCRIPT = ./run_app.sh
PACKAGE_OUT = ./package.$(TARGET)

LAUNCH_EMULATOR = $(PACKAGE_OUT)/launch_$(TARGET).sh
RESULT_STRING = TEST PASSED
CMD_ARGS = krnl_adder.xclbin

CONFIG_FILE := system.cfg

########################## Checking if PLATFORM in allowlist #######################
PLATFORM_BLOCKLIST += zc vck5000 aws-vu9p-f1 samsung u2_ dma 
PLATFORM_ALLOWLIST += vck190

# Command-line options
VPP := v++
AIECC := aiecompiler
AIESIM := aiesimulator

AIE_INCLUDE_FLAGS := -include="$(XILINX_VITIS)/aietools/include" -include="./src/aie" -include="./data" -include="./"
AIE_FLAGS := --platform $(PLATFORM) $(AIE_INCLUDE_FLAGS) --pl-freq=100 -workdir=./Work

ifeq ($(TARGET),sw_emu)
	AIE_FLAGS += --target=x86sim -Xpreproc='-DNDEBUG'       
else
	AIE_FLAGS += --target=hw
endif

VPP_XO_FLAGS := -c -t $(TARGET) --platform $(PLATFORM) --save-temps -g 
VPP_LINK_FLAGS := -l --platform $(PLATFORM) $(KERNEL_XO) $(GRAPH_O) -t $(TARGET) --save-temps -g --config $(CONFIG_FILE) -o $(LINK_OUTPUT)

GCC_FLAGS := -Wall -c \
			 -std=c++14 \
			 -Wno-int-to-pointer-cast \
			 --sysroot=$(SYSROOT) \

GCC_INCLUDES := -I$(SYSROOT)/usr/include/xrt \
				-I$(SYSROOT)/usr/include \
				-I./ -I./src/aie \
				-I${XILINX_VITIS}/aietools/include \
				-I${XILINX_VITIS}/include


GCC_LIB := -lxrt_core -lxrt_coreutil \
		   -L$(SYSROOT)/usr/lib \
		   --sysroot=$(SYSROOT) \
		   -L${XILINX_VITIS}/aietools/lib/aarch64.o

ifeq ($(TARGET),$(filter $(TARGET),sw_emu))
ifeq ($(EMU_PS), X86)
       GCC_LIB := -lxrt_coreutil -L${XILINX_VITIS}/aietools/lib/lnx64.o -L${XILINX_XRT}/lib
       GCC_FLAGS := -Wall -c -std=c++14 -Wno-int-to-pointer-cast -I${XILINX_XRT}/include
       GCC_INCLUDES := -I./src/aie -I./ -I${XILINX_VITIS}/aietools/include
else
       GCC_LIB := -lxrt_coreutil -L$(SYSROOT)/usr/lib --sysroot=$(SYSROOT) -L${XILINX_VITIS}/aietools/lib/aarch64.o
endif
endif

.PHONY: all clean cleanall docs emconfig
ifeq ($(EMU_PS), X86)
all: check-device kernels graph build host emconfig sd_card
else
all:  check-device check_edge_sw kernels graph build host emconfig sd_card
endif

emconfig:
	emconfigutil --platform $(PLATFORM) --od .

######################################################
# This step compiles the HLS C kernels and creates an ADF Graph
# the %.xo is used as the output and creates from the %.cpp files
# The graph is generated by having the -target=hw
kernels: check-device $(KERNEL_XO)
	@echo "COMPLETE: Kernels Created."	

s2mm.xo: src/pl_kernels/s2mm.cpp
	$(VPP) $(VPP_XO_FLAGS) -k $(basename $(notdir $<)) $< -o $@
mm2s.xo: src/pl_kernels/mm2s.cpp
	$(VPP) $(VPP_XO_FLAGS) -k $(basename $(notdir $<)) $< -o $@

graph: check-device $(GRAPH_O)

aiesim: $(GRAPH_O)
	$(AIESIM) --pkg-dir=./Work 

$(GRAPH_O): $(GRAPH)
	$(AIECC) $(AIE_FLAGS) $(GRAPH)
	@echo "COMPLETE: libadf.a created."      
#####################################################

########################################################
# Once the kernels and graph are generated, you can build
# the hardware part of the design. This creates an xclbin
# that will be used to run the design on the platform.
build: check-device $(LINK_OUTPUT)

$(LINK_OUTPUT): $(KERNEL_XO) $(GRAPH_O) 
	$(VPP) $(VPP_LINK_FLAGS) || (echo "task: [xclbin] failed error code: $$?"; exit 1)       
	@echo "COMPLETE:.xsa created"
########################################################

############################################################################################################################
# For hardware and hardware emulation, compile the PS code and generate the aie_adder executable. This is needed for creating the sd_card.
host: $(EXECUTABLE)

$(EXECUTABLE): $(HOST_SRCS) 
ifneq ($(EMU_PS), X86)
	make check_edge_sw
endif 
		$(CXX) $(HOST_SRCS) $(GCC_FLAGS) $(GCC_INCLUDES) -o main.o
		$(CXX) *.o $(GCC_LIB) -o $(EXECUTABLE)
	@echo "COMPLETE: Host application created."
############################################################################################################################

##################################################################################################
# Depending on the TARGET, it'll either generate the PDI for hw_emu or hw.
sd_card: check-device gen_run_app $(XCLBIN)	

$(XCLBIN): $(GRAPH_O) $(KERNEL_XO) $(EXECUTABLE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
ifeq ($(EMU_PS), X86)
	v++ $(VPP_PFLAGS) -p -t $(TARGET) \
                 --package.defer_aie_run \
                 --platform $(PLATFORM) \
                 --package.out_dir $(PACKAGE_OUT) \
                 $(LINK_OUTPUT) $(GRAPH_O) -o $(XCLBIN)
else
	v++ $(VPP_PFLAGS) -p -t $(TARGET) \
		--platform $(PLATFORM) \
		--package.out_dir $(PACKAGE_OUT) \
		--package.rootfs $(EDGE_COMMON_SW)/rootfs.ext4 \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image $(SD_IMAGE_FILE) \
		--package.defer_aie_run \
		--package.sd_file $(RUN_APP_SCRIPT) \
		--package.sd_file aie_adder $(LINK_OUTPUT) $(GRAPH_O) -o $(XCLBIN)
	@echo "COMPLETE: emulation package created."
endif
else
ifeq ($(dfx_hw), true)
	v++ $(VPP_FLAGS) -t hw --platform $(PLATFORM) -p $(LINK_OUTPUT) $(GRAPH_O) --package.defer_aie_run -o $(XCLBIN)
	v++ $(VPP_PFLAGS) -t hw --platform $(PLATFORM) -p --package.out_dir $(PACKAGE_OUT) --package.rootfs $(EDGE_COMMON_SW)/rootfs.ext4 --package.image_format=ext4 --package.boot_mode=sd --package.kernel_image=$(SD_IMAGE_FILE) --package.sd_file $(RUN_APP_SCRIPT) --package.sd_file aie_adder --package.sd_file $(XCLBIN) 

	@echo "### ***** dfx sd_card generation done! ***** ###"
else
	v++ $(VPP_PFLAGS) -p -t hw \
		--platform $(PLATFORM) \
		--package.out_dir $(PACKAGE_OUT) \
		--package.rootfs $(EDGE_COMMON_SW)/rootfs.ext4 \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=$(SD_IMAGE_FILE) \
		--package.defer_aie_run \
		--package.sd_file $(RUN_APP_SCRIPT) \
		--package.sd_file aie_adder $(LINK_OUTPUT) $(GRAPH_O) -o $(XCLBIN)

	@echo "COMPLETE: hw package created."
endif
endif
###################################################################################################

###########################################################################
# If the target is for HW_EMU, launch the emulator

run: all 
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
ifeq ($(EMU_PS), X86)
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(CMD_ARGS)
else
	bash -c '$(LAUNCH_EMULATOR) -run-app $(RUN_APP_SCRIPT) | tee run_app.log; exit $${PIPESTATUS[0]}'
endif
else
	@echo "Hardware build, no emulation executed."
endif

test: $(EXECUTABLE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
ifeq ($(EMU_PS), X86)
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(CMD_ARGS)
else
	bash -c '$(LAUNCH_EMULATOR) -run-app $(RUN_APP_SCRIPT) | tee run_app.log; exit $${PIPESTATUS[0]}'
endif
else
	@echo "Hardware build, no emulation executed."
endif

check_edge_sw:
ifndef EDGE_COMMON_SW
	$(error EDGE_COMMON_SW variable is not set, please download and use the pre-built image from https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)
endif

############################## Cleaning Rules ##############################
# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv *.o *.xo *.xpe *.xsa cfg qemu_dts_files emu_qemu_scripts *.db sim *.sh *.a 
	-$(RMDIR) /src/sw/*.ll /src/pl_kernels/*.ll src/aie/*.ll *v++* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb *bin* *summary* *.BIN *.bif *.exe Work *.log *.txt 

cleanall: clean
	-$(RMDIR) build_dir* sd_card*
	-$(RMDIR) package.* run_app.sh 
	-$(RMDIR) _x* *xclbin.run_summary qemu-memory-_* emulation _vimage start_simulation.sh *.xclbin
