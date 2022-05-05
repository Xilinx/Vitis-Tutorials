#  (c) Copyright 2021–2022 Xilinx, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


# Makefile input options
TARGET ?= hw_emu

#Directories
FILES_DIR := $(PWD)/../../Files
DATA_DIR := $(FILES_DIR)/data
PLATFORM_DIR := ${PLATFORM_REPO_PATHS}
AIE2:= $(FILES_DIR)/AIE2
PL:=$(FILES_DIR)/PL
HwLink:=$(FILES_DIR)/HwLink
PS:=$(FILES_DIR)/PS

# Platform
VERSAL_SW := ${PLATFORM_DIR}/sw/versal/xilinx-versal-common-v2022.1
BASE_PLATFORM := ${PLATFORM_DIR}/$(PFM_NAME)/$(PFM_NAME).xpfm
PLATFORM_SW := ${PLATFORM_DIR}/$(PFM_NAME)/sw/$(PFM_NAME)

IMAGE   := ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2022.1/Image

# New platform
PFM := Phase1_container
# Phase 1 output
XCLBIN := ../Phase1/$(PFM).xclbin
XSA    := ../Phase1/$(PFM).xsa

# Phase 2 platform creation outut
NEW_PLATFORM := ./platform_repo/$(PFM)/export/$(PFM)/$(PFM).xpfm
NEW_PLATFORM_SW := ./platform_repo/$(PFM)/export/$(PFM)/sw/$(PFM)

NEW_ROOTFS := $(NEW_PLATFORM_SW)/xrt/rootfs/rootfs.ext4
NEW_SYSROOT := $(NEW_PLATFORM_SW)/xrt/sysroot

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
AIE_FLAGS += -platform=$(NEW_PLATFORM)
AIE_FLAGS +=  --Xpreproc=-DDATA_DIR=$(DATA_DIR)

VPP_XO_FLAGS := -c --platform $(BASE_PLATFORM) --save-temps -g
VPP_LINK_FLAGS := -l --platform $(BASE_PLATFORM) $(KERNEL_XO) $(GRAPH_O) -t $(TARGET) --save-temps -g --config $(HwLink)/$(CONFIG_FILE) -o $(XSA)


.ONESHELL:
.PHONY: clean all pfm aie2 host package2 run_emu

phase2: pfm aie2 host package2 run_emu

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
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202210_1)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2022.1/environment-setup-aarch64-xilinx-linux)


########################################################
# This step Creates the new platform

# xsct_create_pfm has 4 parameters
# Platform name
# Directory of the xsa and xclbin
#  Generic Versal sw  directory
#  VCK190 specific sw  directory
pfm:
		xsct xsct_create_pfm.tcl ${PFM} ../Phase1 ${VERSAL_SW} ${PLATFORM_SW}


########################################################
# This step compiles the AI Engine graph
# to create the file libadf.a



check_aiesim:
	../check_op 1 $(DATA_DIR)/s2mm_ref.txt aiesimulator_output/data/DataOut1.txt


aie2: $(GRAPH2_O)

aiesim2: $(GRAPH2_O)
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
			 --sysroot=$(NEW_PLATFORM_SW)/xrt/sysroot/aarch64-xilinx-linux


GCC_INCLUDES :=  -I./ -I${AIE2}/src \
				-I$(XILINX_VITIS)/aietools/include \
				-I$(XILINX_VITIS)/include \
				-I${PS}/src \
				-I$(NEW_SYSROOT)/aarch64-xilinx-linux/usr/include \
				-I$(NEW_SYSROOT)/aarch64-xilinx-linux/usr/include/xrt \
				-I$(XILINX_VIVADO)/include


GCC_LIB := -lxaiengine -ladf_api_xrt -lxrt_core -lxrt_coreutil \
			 --sysroot=$(NEW_SYSROOT)/aarch64-xilinx-linux \
			 -L$(NEW_SYSROOT)/aarch64-xilinx-linux/usr/lib \
		   -L${XILINX_VITIS}/aietools/lib/aarch64.o \
			 -L${XILINX_VITIS}/aietools/lib/lnx64.o

LDCLFLAGS := $(GCC_LIB)


host: guard-CXX
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o aie_control_xrt.o Work2/ps/c_rts/aie_control_xrt.cpp
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o main.o $(PS)/src/host.cpp
	$(CXX) *.o $(GCC_LIB) -std=c++14 -o host.exe

########################################################

##################################################################################################
# Depending on the TARGET, it'll either generate the PDI for hw_emu or hw.

package2: guard-PLATFORM_REPO_PATHS
	v++ -p -t ${TARGET} \
		-f ${NEW_PLATFORM} \
		--package.rootfs=${NEW_ROOTFS} \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=${IMAGE} \
		--package.defer_aie_run \
		--package.sd_file host.exe  $(XSA) libadf2.a

########################################################

##################################################################################################
# Runs Hw emulation
run_emu:
	./launch_hw_emu.sh

########################################################




clean:
	rm -rf _x v++* $(KERNEL_XO) $(GRAPH_O) $(GRAPH2_O) *.o *.compile_summary* *.xpe xnwOut *.xclbin* *.log *.xsa
	rm -rf Work Work2 *.db *.csv *$(PFM)* *.jou .Xil .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf aiesimulator_output pl_sample*
	rm -rf *.bin *.BIN cfg emu_qemu_scripts launch* *.txt *.sh
	rm -rf sd_card aiesim *.wcfg *.wdb *.bif *.exe *.img  sim temp
	make clean_pfm

clean_pfm:
	rm -rf build platform_repo
