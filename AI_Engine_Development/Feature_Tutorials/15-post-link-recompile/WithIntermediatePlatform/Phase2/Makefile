#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#



# ROOTFS ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2023.1/rootfs.ext4
# IMAGE ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2023.1/Image
# SDKTARGETSYSROOT ?= ${SYSROOT}
#
# Makefile input options
TARGET ?= hw_emu
PFM_NAME ?= xilinx_vck190_base_202310_1

#Directories
FILES_DIR ?= ../../Files
DATA_DIR := $(FILES_DIR)/data
PLATFORM_DIR := ${PLATFORM_REPO_PATHS}
AIE2:= $(FILES_DIR)/AIE2
PL:=$(FILES_DIR)/PL
HwLink:=$(FILES_DIR)/HwLink
PS:=$(FILES_DIR)/PS

# Initial Base PLATFORM
BASE_PLATFORM := ${PLATFORM_DIR}/$(PFM_NAME)/$(PFM_NAME).xpfm

# Phase1 Generated platform
PFM := Phase1_container
XCLBIN := ../Phase1/$(PFM).xclbin
XSA    := ../Phase1/$(PFM).xsa


# File names and locations
GRAPH2 := ${AIE2}/src/graph.cpp

GRAPH2_O := libadf2.a

KERNEL := s2mm.cpp mm2s.cpp polar_clip.cpp
KERNEL_XO := s2mm.xo mm2s.xo polar_clip.xo
CONFIG_FILE := system.cfg


# Command-line options
VPP := v++
AIECC := aiecompiler

AIE_INCLUDE_FLAGS := -include="$(XILINX_VITIS)/aietools/include" -include="${AIE2}/src" -include="${AIE2}/data" -include="${AIE2}/src/aie_kernels"
AIE_FLAGS := --target=hw $(AIE_INCLUDE_FLAGS) --pl-freq=200
AIE_FLAGS += -platform=$(XSA)
AIE_FLAGS +=  --Xpreproc=-DDATA_DIR=$(DATA_DIR)


.ONESHELL:
.PHONY: clean all aie2 host package2 run_emu

phase2: aie2 host package2 run_emu

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
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202310_1)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2023.1/environment-setup-aarch64-xilinx-linux)


########################################################
# This step Creates the new platform

# xsct_create_pfm has 4 parameters
# Platform name
# Directory of the xsa and xclbin
#  Generic Versal sw  directory
#  VCK190 specific sw  directory
# pfm:
# 		xsct xsct_create_pfm.tcl ${PFM} ../Phase1 ${VERSAL_SW} ${PLATFORM_SW}


########################################################
# This step compiles the AI Engine graph
# to create the file libadf.a



check_aiesim:
	../../check_op 1 $(DATA_DIR)/s2mm_ref.txt $(DATA_DIR)/DataOut1.txt


aie2: $(GRAPH2_O)

aiesim2: $(GRAPH2_O)
	rm -f $(DATA_DIR)/DataOut1.txt
	aiesimulator --pkg-dir=./Work2
	make check_aiesim

$(GRAPH2_O): $(GRAPH2)
	$(AIECC) $(AIE_FLAGS)  --output-archive=$(GRAPH2_O) --workdir=./Work2 $(GRAPH2)
	@echo "COMPLETE: libadf2.a created."

########################################################


########################################################
# The host application is then compiledto be used
# either in hw/sw_emu or directly hw

GCC_FLAGS := -Wall -c \
			 -std=c++14 \
			 -Wno-int-to-pointer-cast \
			 --sysroot=$(SDKTARGETSYSROOT)


GCC_INCLUDES :=  -I./ -I${AIE2}/src \
				-I$(XILINX_VITIS)/aietools/include \
				-I$(XILINX_VITIS)/include \
				-I${PS}/src \
				-I$(SDKTARGETSYSROOT)/usr/include/xrt \
				-I$(SDKTARGETSYSROOT)/usr/include



ifeq ($(TARGET),sw_emu)
	GCC_LIB := -lxaiengine -ladf_api_xrt -lxrt_coreutil -L$(SDKTARGETSYSROOT)/usr/lib -L${XILINX_VITIS}/aietools/lib/aarch64.o  --sysroot=${SDKTARGETSYSROOT}
else
	GCC_LIB := -lxaiengine -ladf_api_xrt -lxrt_core -lxrt_coreutil -L$(SDKTARGETSYSROOT)/usr/lib -L${XILINX_VITIS}/aietools/lib/aarch64.o  --sysroot=${SDKTARGETSYSROOT}
endif


host: guard-CXX
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o aie_control_xrt.o Work2/ps/c_rts/aie_control_xrt.cpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o main.o $(PS)/src/host.cpp
	$(CXX) *.o $(GCC_LIB) -std=c++14 -o host.exe

########################################################

##################################################################################################
# Depending on the TARGET, it'll either generate the PDI for hw_emu or hw.

package2: guard-PLATFORM_REPO_PATHS
	v++ -p -t ${TARGET} \
		-f ${BASE_PLATFORM} \
		--package.rootfs=${ROOTFS} \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=${IMAGE} \
		--package.defer_aie_run \
		--package.sd_file host.exe  $(XSA) libadf2.a \
		--package.sd_file ${FILES_DIR}/embedded_exec.sh

########################################################

##################################################################################################
# Runs Hw emulation
run_emu:
	./launch_hw_emu.sh -run-app ./embedded_exec.sh

########################################################




clean:
	rm -rf _x v++* $(KERNEL_XO) $(GRAPH_O) $(GRAPH2_O) *.o *.compile_summary* *.xpe xnwOut *.xclbin* *.log *.xsa
	rm -rf Work Work2 *.db *.csv *$(PFM)* *.jou .Xil .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf aiesimulator_output pl_sample*
	rm -rf *.bin *.BIN cfg emu_qemu_scripts launch* *.txt *.sh
	rm -rf sd_card aiesim *.wcfg *.wdb *.bif *.exe *.img  sim temp
