
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

# Examples of make calls
#
# make run TARGET=hw_emu EXTIO=false 
#   --> Standard hw emulation with PL datamovers
#
# make TARGET=hw_emu EXTIO=true TRAFFIC_GEN=<PYTHON/MATLAB/CPP>
#   -->  Runs hw_emu with external TG. 

PLATFORM := xilinx_vck190_base_202410_1

# Makefile input options if not defined from top-level stage

# TARGET --> sw_emu | hw_emu
# sw_emu means sw_emu for system simulation)
# hw_emu means hw_emu for system simulation)
TARGET ?= hw_emu

# EXTIO --> true | false
# true: simulation needs an external traffic Generator
# false: simulation will use PL datamovers
EXTIO ?= true

# TRAFFIC_GEN --> PYTHON | CPP
# PYTHON: A Python based traffic generator is used
# CPP: A C++ traffic generator is used
# MATLAB: A MATLAB based traffic generator is used
TRAFFIC_GEN ?= 

# necessary settings to run the python ETG
PYTHON_INSTALL ?= $(XILINX_VIVADO)/tps/lnx64/python-3.8.3/bin/
ifeq (${TRAFFIC_GEN}, PYTHON)
ifndef PYTHON_INSTALL
$(error PYTHON_INSTALL is not set)
else
    PYTHONHOME ?= ${PYTHON_INSTALL}/..
endif
endif

#Check environment setup
ifndef ROOTFS
  ROOTFS = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2024.1/rootfs.ext4
  export ROOTFS
endif
ifndef IMAGE
  IMAGE = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2024.1/Image
  export IMAGE
endif

SDKTARGETSYSROOT ?= ${SYSROOT}
PYTHONPATH=$(XILINX_VIVADO)/data/emulation/python/xtlm_ipc_v2/

# Makefile input options
XCLBIN := tutorial

# File names and locations
GRAPH := ./aie/src/graph.cpp
GRAPH_O := libadf.a
PYTHONPATH=$(XILINX_VIVADO)/data/emulation/python/xtlm_ipc_v2/
XSA := tutorial.xsa 

# Command-line options
AIECC := v++ -c --mode aie 

AIE_INCLUDE_FLAGS := --include="$(XILINX_VITIS)/aietools/include" --include="./aie/src" --include="./data" --include="./src/kernels" --include="./"
AIE_FLAGS := $(AIE_INCLUDE_FLAGS) --aie.xlopt=0 --work_dir=./Work --platform $(PLATFORM) 

# External IO for External Traffic Generator
ifeq (${EXTIO}, true)
AIE_FLAGS += --aie.Xpreproc=-DEXTERNAL_IO
endif


.ONESHELL:
.PHONY: clean all aie disp

###
# Guarding Checks. Do not modify.
###

disp:
	@echo "TARGET = '"${TARGET}"'"
	@echo "EXTIO = '"${EXTIO}"'"
ifeq (${EXTIO}, true)
	@echo "TESTBENCH = '"${TRAFFIC_GEN} based External Traffic"'"
else
	@echo "TESTBENCH = '"PL HLS Datamovers"'"
endif

aie:  $(GRAPH_O) 

$(GRAPH_O): $(GRAPH)
ifeq (${TARGET}, sw_emu)
	$(AIECC) --target=x86sim $(AIE_FLAGS) $(GRAPH)
else
	$(AIECC) --target=hw $(AIE_FLAGS) $(GRAPH)
endif

## File names and locations
BASE_DIR := $(PWD)
GRAPH := $(BASE_DIR)/aie/src/graph.cpp
GRAPH_O := $(BASE_DIR)/libadf.a

ifeq (${EXTIO}, true) # WITH External Traffic Generator
KERNEL := $(wildcard pl_kernels/*.cpp)
KERNEL_XO := $(XILINX_VITIS)/data/emulation/XO/sim_ipc_axis_master_32.xo $(XILINX_VITIS)/data/emulation/XO/sim_ipc_axis_slave_32.xo
ifeq (${TARGET}, sw_emu)
KERNEL_XO += polar_clip.xo
else
KERNEL_XO += pl_kernels/polar_clip.xo
endif
CONFIG_FILE ?= system_etg.cfg
else # WITHOUT External Traffic Generator
KERNEL := $(wildcard pl_kernels/*.cpp)
ifeq ifeq (${TARGET}, sw_emu)
KERNEL_XO := mm2s.xo s2mm.xo polar_clip.xo
else
KERNEL_XO := pl_kernels/mm2s.xo pl_kernels/s2mm.xo pl_kernels/polar_clip.xo
endif
CONFIG_FILE ?= system.cfg
endif

# Command-line options
VPP := v++

BASE_PLATFORM := $(PLATFORM)
VPP_LINK_FLAGS := -l --platform $(BASE_PLATFORM) $(KERNEL_XO) $(GRAPH_O) -t $(TARGET) --save-temps -g --config $(CONFIG_FILE) -I $(BASE_DIR)/pl_kernels -o $(XCLBIN).xsa
VPP_FLAGS := $(VPP_LINK_FLAGS)
LDCLFLAGS :=

ifeq ($(TARGET),sw_emu)
	VPP_XO_FLAGS := -c --platform $(BASE_PLATFORM) -t $(TARGET) --save-temps -g -DDATA_WIDTH=32
else
	VPP_XO_FLAGS := -c --mode hls --platform $(BASE_PLATFORM) 
endif
			 
ifeq (${EXTIO}, true)
GCC_FLAGS += -DEXTERNAL_IO
endif

ifeq (${TARGET}, sw_emu)
GCC_FLAGS += -Wall -c \
		-std=c++14 \
		-Wno-int-to-pointer-cast 
GCC_INCLUDES := -I${XILINX_XRT}/include
GCC_LIB := -lxrt_coreutil \
    		-L${XILINX_XRT}/lib
else
GCC_FLAGS += -Wall -c \
			 -std=c++14 \
			 -Wno-int-to-pointer-cast \
			 --sysroot=${SDKTARGETSYSROOT} 

GCC_INCLUDES := -I$(SDKTARGETSYSROOT)/usr/include/xrt 
GCC_LIB :=  -lxrt_coreutil \
		   -L$(SDKTARGETSYSROOT)/usr/lib \
		   --sysroot=${SDKTARGETSYSROOT}
endif

LDCLFLAGS := $(GCC_LIB)
RUN_APP_SCRIPT := embedded_exec.sh

.ONESHELL:
.PHONY: clean kernels xsa host package 

###
# Guarding Checks. Do not modify.
###
check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202120_1)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2021.2/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2021.2/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2021.2/environment-setup-aarch64-xilinx-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2021.2/environment-setup-aarch64-xilinx-linux)

######################################################
# This step compiles the HLS C kernels and creates an ADF Graph
# the %.xo is used as the output and creates from the %.cpp files
# The graph is generated by having the -target=hw

kernels: $(KERNEL_XO)

%.xo:

ifeq ($(TARGET),sw_emu)
ifeq (${EXTIO}, true)
	$(VPP) $(VPP_XO_FLAGS) -k polar_clip pl_kernels/polar_clip.cpp -o polar_clip.xo
else
	$(VPP) $(VPP_XO_FLAGS) -k s2mm pl_kernels/s2mm.cpp -o s2mm.xo
	$(VPP) $(VPP_XO_FLAGS) -k mm2s pl_kernels/mm2s.cpp -o mm2s.xo
	$(VPP) $(VPP_XO_FLAGS) -k polar_clip pl_kernels/polar_clip.cpp -o polar_clip.xo
endif
else
ifeq (${EXTIO}, true)
	$(VPP) $(VPP_XO_FLAGS) --config pl_kernels/polar_clip.cfg
else
	$(VPP) $(VPP_XO_FLAGS) --config pl_kernels/s2mm.cfg 
	$(VPP) $(VPP_XO_FLAGS) --config pl_kernels/mm2s.cfg
	$(VPP) $(VPP_XO_FLAGS) --config pl_kernels/polar_clip.cfg
endif
endif
	

#####################################################

########################################################
# Once the kernels and graph are generated, you can build
# the hardware part of the design. This creates an xclbin
# that will be used to run the design on the platform.
xsa: guard-PLATFORM_REPO_PATHS $(GRAPH_O) $(KERNEL_XO) aie
	$(VPP) $(VPP_LINK_FLAGS) 
########################################################
ifeq ($(TARGET),sw_emu)
CXX := g++
else
CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
endif
############################################################################################################################
# For hardware and hardware emulation, compile the PS code and generate the host.exe. This is needed for creating the sd_card.
host: guard-CXX guard-SDKTARGETSYSROOT
	cd ./sw
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o main.o host.cpp
	$(CXX) *.o $(GCC_LIB) -std=c++14 -o host.exe

############################################################################################################################

##################################################################################################
# Depending on the TARGET, it'll either generate the PDI for hw_emu or hw.
package: guard-PLATFORM_REPO_PATHS guard-IMAGE guard-ROOTFS $(XSA) host
ifeq ($(TARGET), sw_emu)
	cd ./sw
	v++ -p -t ${TARGET} \
		-f ${BASE_PLATFORM} \
		--package.defer_aie_run \
		../tutorial.xsa  ../libadf.a 
else
	cd ./sw
	v++ -p -t ${TARGET} \
		-f ${BASE_PLATFORM} \
		--package.rootfs=${ROOTFS} \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=${IMAGE} \
		--package.defer_aie_run \
		--package.sd_file ../$(RUN_APP_SCRIPT) \
		--package.sd_file host.exe \
		../tutorial.xsa  ../libadf.a 
endif
###################################################################################################

# Setting HW Emulation Targets
HW_EMU := cd sw; ./launch_hw_emu.sh -no-reboot -run-app embedded_exec.sh
SW_EMU := cd sw; ./host.exe a.xclbin 

all: disp aie host kernels xsa package 
ifeq (${TRAFFIC_GEN}, PYTHON)
#For Python External Traffic Generator
HW_EMU_ETG := ./scripts/etg_py.sh & $(HW_EMU)
SW_EMU_ETG := ./scripts/etg_py.sh & $(SW_EMU)
endif
ifeq (${TRAFFIC_GEN}, CPP)
# For C++ Traffic Generator
HW_EMU_ETG := ./scripts/etg_cpp.sh & $(HW_EMU)
SW_EMU_ETG := ./scripts/etg_cpp.sh & $(SW_EMU)
endif
ifeq (${TRAFFIC_GEN}, MATLAB)
#For MATLAB External Traffic Generator
HW_EMU_ETG := ./scripts/etg_matlab.sh & $(HW_EMU)
SW_EMU_ETG := ./scripts/etg_matlab.sh & $(SW_EMU)
endif

run_test: all
ifeq (${TARGET}, sw_emu)
ifeq (${EXTIO}, false)
	$(SW_EMU)
else  # Use External Traffic Generator
	$(SW_EMU_ETG)
endif
else # hw_emu
ifeq (${EXTIO}, false)
	$(HW_EMU)
else # Use External Traffic Generator
	$(HW_EMU_ETG)
endif
endif 

compare: 
ifeq (${EXTIO}, true)
	@diff -w ./data/classifier_golden.txt ./out_classifier_data.txt; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
	    echo "TEST PASSED: Classifier validated successfully"; \
	else \
	    echo "ERROR: Classifier validation failed"; \
	fi
endif
###########################################################################
run: run_test 
clean:
	make -C aie clean Work
	make -C TrafficGenerator/Cpp clean
	make -C pl_kernels clean
	rm -rf *.txt *.log  _x .Xil
	rm -rf  link_summary tutorial.xsa.info tutorial.xsa tutorial.xclbin
	rm -rf tutorial.xclbin.link_summary tutorial.xsa.link_summary
	rm -rf .run
	rm libadf.a *.xo *.compile_summary
	cd sw;
	rm -rf *.txt *.log *.bin *.img *summary *.wcfg *.wdb *.o *.exe .Xil
	rm -rf a.xsa a.xclbin BOOT.BIN  boot_image.bif  cfg  cips_config_file.json
	rm -rf launch_sw_emu.sh  launch_hw_emu.sh
	rm -rf qemu* x86simulator_output pl_script.sh
	rm -rf dtb_creation.sh  emu_qemu_scripts  sd_card  sim  _x
