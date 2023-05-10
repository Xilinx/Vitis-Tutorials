
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# Examples of make calls
#
# make TARGET=sw_emu EXTIO=false aie aiesim
#   --> standard X86 simulation
#
# make TARGET=hw_emu EXTIO=false aie aiesim
#   --> standard AIE simulation
#
# make TARGET=sw_emu EXTIO=true TRAFFIC_GEN=PYTHON PYTHON_INSTALL=<> aie aiesim
#   -->  X86 simulation using a Python Traffic generator
#
# make TARGET=hw_emu EXTIO=true TRAFFIC_GEN=PYTHON PYTHON_INSTALL=<> aie aiesim
#   -->  AIE simulation using a Python Traffic generator
#
# make TARGET=hw_emu EXTIO=true TRAFFIC_GEN=CPP aie traffic_gen aiesim
#   -->  AIE simulation using a C++ Traffic generator
#
# make TARGET=sw_emu EXTIO=true TRAFFIC_GEN=CPP aie traffic_gen aiesim
#   -->  X86 simulation using a C++ Traffic generator
#
# make TARGET=hw_emu EXTIO=true TRAFFIC_GEN=PYTHON PYTHON_INSTALL=<> clean aie xclbin host package run_emu
#   -->  HW simulation using a PYTHON Traffic generator
#   
# make TARGET=hw_emu EXTIO=true TRAFFIC_GEN=CPP traffic_gen run_emu
#   -->  Runs hw_emu with C++ external TG. Here, you take advantage of the already compiled xclbin and created package.

export PFM_NAME := xilinx_vck190_base_202310_1

# Makefile input options if not defined from top-level stage

# TARGET --> sw_emu | hw_emu
# sw_emu means x86 simulation (or sw_emu for system simulation)
# hw_emu means aie simulation (or hw_emu for system simulation)
TARGET ?= hw_emu

# EXTIO --> true | false
# true: simulation needs an external traffic Generator
# false: simulation will use provide data files
EXTIO ?= true

# TRAFFIC_GEN --> PYTHON | CPP
# PYTHON: A Python based traffic generator is used
# CPP: A C++ traffic generator is used
TRAFFIC_GEN ?= PYTHON
#will update to empty once regression gets successful run
PYTHON_INSTALL ?= /tools/batonroot/rodin/engkits/lnx64/python-3.6.5_metropolis/bin
ifeq (${TRAFFIC_GEN}, PYTHON)
ifndef PYTHON_INSTALL
$(error PYTHON_INSTALL is not set)
else
    PYTHONHOME ?= ${PYTHON_INSTALL}/..
endif
endif
# Full flows
all_aiesim: aie traffic_gen aiesim
all_run_emu:  aie kernels xclbin host package traffic_gen run_emu


#Check environment setup
ifndef ROOTFS
  ROOTFS = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2023.1/rootfs.ext4
  export ROOTFS
endif
ifndef IMAGE
  IMAGE = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2023.1/Image
  export IMAGE
endif

SDKTARGETSYSROOT ?= ${SYSROOT}
PYTHONPATH += ${XILINX_VIVADO}/data/emulation/hw_em/lib/python:${XILINX_VIVADO}/data/emulation/python/xtlm_ipc:${XILINX_VIVADO}/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/python


# Makefile input options
XCLBIN := tutorial


## File names and locations
BASE_DIR := $(PWD)
GRAPH := $(BASE_DIR)/aie/src/graph.cpp
GRAPH_O := $(BASE_DIR)/aie/libadf.a

ifeq (${EXTIO}, true) # WITH External Traffic Generator
KERNEL :=
KERNEL_XO := $(XILINX_VITIS)/data/emulation/XO/sim_ipc_axis_master_32.xo $(XILINX_VITIS)/data/emulation/XO/sim_ipc_axis_slave_32.xo
CONFIG_FILE ?= system_etg.cfg
else # WITHOUT External Traffic Generator
KERNEL := $(wildcard pl_kernels/*.cpp)
KERNEL_XO := $(subst .cpp,.xo,$(KERNEL))
CONFIG_FILE ?= system.cfg
endif

disp:
	echo $(GCC_FLAGS)

BASE_PLATFORM := ${PLATFORM_REPO_PATHS}/${PFM_NAME}/${PFM_NAME}.xpfm

# Command-line options
VPP := v++


#VPP_XO_FLAGS := -c --platform $(BASE_PLATFORM) --save-temps -g
VPP_LINK_FLAGS := -l --platform $(BASE_PLATFORM) $(KERNEL_XO) $(GRAPH_O) -t $(TARGET) --save-temps -g --config $(CONFIG_FILE) -I $(BASE_DIR)/pl_kernels -o $(XCLBIN).xsa
VPP_FLAGS := $(VPP_LINK_FLAGS)
LDCLFLAGS :=

GCC_FLAGS := -Wall -c \
			 -std=c++14 \
			 -Wno-int-to-pointer-cast \
			 --sysroot=${SDKTARGETSYSROOT}

ifeq (${EXTIO}, true)
	GCC_FLAGS += -DEXTERNAL_IO
endif

ifeq (${TARGET}, sw_emu)
	GCC_FLAGS += -D__SYNCBO_ENABLE__
endif



GCC_INCLUDES := -I$(SDKTARGETSYSROOT)/usr/include/xrt \
				-I$(SDKTARGETSYSROOT)/usr/include \
				-I${XILINX_VITIS}/include


GCC_LIB :=  -lxrt_coreutil \
		   -L$(SDKTARGETSYSROOT)/usr/lib \
		   --sysroot=${SDKTARGETSYSROOT}

LDCLFLAGS := $(GCC_LIB)

.ONESHELL:
.PHONY: clean kernels traffic_gen aie aiesim xclbin host package run_emu
.PHONY: all_aiesim all_run_emu



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

kernels:
ifeq (${EXTIO}, true)
	cp -rf $(KERNEL_XO) pl_kernels
else
	make -C pl_kernels BASE_PLATFORM=$(BASE_PLATFORM) TARGET=$(TARGET) all
endif

traffic_gen:
ifeq (${EXTIO}, true)
ifeq (${TRAFFIC_GEN}, CPP)
	make -C TrafficGenerator/Cpp compile
endif
endif

aie:
	make -C aie TARGET=$(TARGET) EXTIO=$(EXTIO) TRAFFIC_GEN=$(TRAFFIC_GEN) PYTHON_INSTALL=${PYTHON_INSTALL} aie

aiesim:
	make -C aie TARGET=$(TARGET) EXTIO=$(EXTIO) TRAFFIC_GEN=$(TRAFFIC_GEN) PYTHON_INSTALL=${PYTHON_INSTALL} aiesim

#####################################################

########################################################
# Once the kernels and graph are generated, you can build
# the hardware part of the design. This creates an xclbin
# that will be used to run the design on the platform.
xclbin: guard-PLATFORM_REPO_PATHS $(GRAPH_O) $(KERNEL_XO)
	$(VPP) $(VPP_LINK_FLAGS)
########################################################

############################################################################################################################
# For hardware and hardware emulation, compile the PS code and generate the host.exe. This is needed for creating the sd_card.
host: guard-CXX guard-SDKTARGETSYSROOT
	cd ./sw
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o main.o host.cpp
	$(CXX) *.o $(GCC_LIB) -std=c++14 -o host.exe


############################################################################################################################

##################################################################################################
# Depending on the TARGET, it'll either generate the PDI for hw_emu or hw.
package: guard-PLATFORM_REPO_PATHS guard-IMAGE guard-ROOTFS
	cd ./sw
	v++ -p -t ${TARGET} \
		-f ${BASE_PLATFORM} \
		--package.rootfs=${ROOTFS} \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=${IMAGE} \
		--package.defer_aie_run \
		--package.sd_file host.exe \
		../tutorial.xsa  ../aie/libadf.a \
		--package.sd_file ../addon_files/run_sw_emu.sh \
		--package.sd_file ../addon_files/run_hw_emu.sh

###################################################################################################

# Hardware Emulation

HW_EMU := ./launch_hw_emu.sh -run-app run_hw_emu.sh -pid-file pid.txt
SW_EMU := ./launch_sw_emu.sh

run_emu:
ifeq (${TARGET}, hw_emu)
ifeq (${EXTIO}, true)
ifeq (${TRAFFIC_GEN}, PYTHON)
	${PYTHON_INSTALL}/python3 TrafficGenerator/Python/xtg_aie.py  & cd sw; $(HW_EMU)
	../check_op 2 ../data/s2mm_ref.txt ../DataOut1.txt
else
	TrafficGenerator/Cpp/chain  & cd sw; $(HW_EMU)
	../check_op 2 ../data/s2mm_ref.txt ../DataOut1.txt
endif
else
	cd sw; $(HW_EMU)
endif
else
ifeq (${EXTIO}, true)
ifeq (${TRAFFIC_GEN}, PYTHON)
	${PYTHON_INSTALL}/python3 TrafficGenerator/Python/xtg_aie.py  &	cd sw; $(SW_EMU)
	../check_op 2 ../data/s2mm_ref.txt ../DataOut1.txt
else
	TrafficGenerator/Cpp/chain  & cd sw; $(SW_EMU)
	../check_op 2 ../data/s2mm_ref.txt ../DataOut1.txt
endif
else
	cd sw; $(SW_EMU)
endif
endif

###########################################################################

clean:
	make -C aie clean
	make -C TrafficGenerator/Cpp clean
	make -C pl_kernels clean
	rm -rf *.txt *.log  _x .Xil
	rm -rf  link_summary tutorial.xsa.info tutorial.xsa tutorial.xclbin
	rm -rf tutorial.xclbin.link_summary tutorial.xsa.link_summary
	rm -rf .run
	cd sw;
	rm -rf *.txt *.log *.bin *.img *summary *.wcfg *.wdb *.o *.exe .Xil
	rm -rf a.xsa a.xclbin BOOT.BIN  boot_image.bif  cfg  cips_config_file.json
	rm -rf launch_sw_emu.sh  launch_hw_emu.sh
	rm -rf qemu* x86simulator_output pl_script.sh
	rm -rf dtb_creation.sh  emu_qemu_scripts  sd_card  sim  _x
