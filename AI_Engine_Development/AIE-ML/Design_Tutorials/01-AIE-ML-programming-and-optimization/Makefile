#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#


#################################################################################
##     Directories
#################################################################################
AIE_DIR := $(PWD)/aie
KERNELS_DIR := $(PWD)/pl_kernels
HOST_DIR := $(PWD)/sw
ADDON_DIR := $(PWD)/addon_files

#################################################################################
## Project Parameters
#################################################################################

# Optimization
OPT ?= 0

# hw_emu|hw

TARGET   ?= hw_emu
PFM_NAME := xilinx_vek280_es1_base_202410_1
PFM_NAME := $(strip $(PFM_NAME))
PLATFORM := ${PLATFORM_REPO_PATHS}/${PFM_NAME}/${PFM_NAME}.xpfm
PNAME := aieml_pl_${TARGET}
XCLBIN   := ${PNAME}.xsa
HOST_EXE := host.exe

LIBADF  := $(AIE_DIR)/libadf$(OPT).a

# Default values for A, B, C matrix sizes
# A:MxK    B:KxN    C:MxN
sizeM ?= 64
sizeK ?= 64
sizeN ?= 64

# Default for A, B and C sub matrices
# 4x16x8
subM ?= 4
subK ?= 16
subN ?= 8

#Default Number of iterations
NIterations ?= 4

# Width of the PLIOs
PLIOW ?= 128


SIZES := sizeM=$(sizeM) sizeK=$(sizeK) sizeN=$(sizeN) subM=$(subM) subK=$(subK) subN=$(subN) NIterations=$(NIterations) PLIOW=$(PLIOW)

FREQ_HZ := 250000000 # PL Frequency (PL kernels)


##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################


AIE_FLAGS :=  $(SIZES) OPT=$(OPT) PLATFORM=$(PLATFORM)


##XOS      = $(subst .cpp,.xo,$(wildcard $(KERNELS_DIR)/*.cpp))
## XOS		= $(wildcard $(KERNELS_DIR)/*_$(strip $(PLIOW)).cpp)

KERNEL_CPP := $(KERNELS_DIR)/mm2s_8_$(PLIOW).cpp $(KERNELS_DIR)/s2mm_16_$(PLIOW).cpp $(KERNELS_DIR)/s2mm_32_$(PLIOW).cpp
XO_FILES := $(subst .cpp,.xo,$(KERNEL_CPP))

XOS      = $(XO_FILES)
VCC      = v++
VPP_SPEC = $(ADDON_DIR)/system_$(strip $(PLIOW)).cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}


.PHONY: clean run_hw_emu data

disp:
	echo $(KERNELS_DIR)
	echo $(XOS)

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
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded your platform)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2024.1/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2024.1/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2024.1/environment-setup-cortexa72-cortexa53-xilinx-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2024.1/environment-setup-cortexa72-cortexa53-xilinx-linux)

###
.PHONY: all_hw all_hw_emu run upd_host_hw aie postaie data aiesim aieviz aiesim-fifo compareaie x86 x86sim comparex86

build_hw:
	make TARGET=hw  clean-all OPT=$(OPT) data aie kernels xclbin host package

build_hw_emu:
	make TARGET=hw_emu  clean-all OPT=$(OPT) data aie kernels xclbin host package

launch_hw_emu:
	./launch_hw_emu.sh -add-env AIE_COMPILER_WORKDIR=$(AIE_DIR)/Work$(OPT) -aie-sim-options $(ADDON_DIR)/aiesim_options.txt -run-app embedded_exec.sh

run_hw_emu:
	make OPT=$(OPT)  build_hw_emu launch_hw_emu

upd_host_hw:
	make TARGET=hw host package

aie:
	make -C $(AIE_DIR) aie $(AIE_FLAGS)

postaie:
	make -C $(AIE_DIR) aie $(AIE_FLAGS) XSA=../${PNAME}.xsa

data:
	make -C aie data

aiesim:
	make -C $(AIE_DIR) $(AIE_FLAGS) aiesim

aieviz:
	make -C $(AIE_DIR) $(AIE_FLAGS) aieviz

aiesim-fifo:
	make -C $(AIE_DIR) $(AIE_FLAGS) aiesim-fifo

compareaie:
	make -C $(AIE_DIR) OPT=$(OPT) compareaie

x86:
	make -C $(AIE_DIR) x86 $(AIE_FLAGS)

x86sim:
	make -C $(AIE_DIR) OPT=$(OPT) x86sim

comparex86:
	make -C $(AIE_DIR) OPT=$(OPT) comparex86


xclbin: guard-PLATFORM_REPO_PATHS ${XCLBIN}
${XCLBIN}: ${LIBADF} ${VPP_SPEC} ${XOS}
	make disp
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

kernels: guard-PLATFORM_REPO_PATHS ${XOS}
${XOS}:
	make -C pl_kernels/ PLATFORM=${PLATFORM} FREQ_HZ=${FREQ_HZ} PLIOW=${PLIOW} $(SIZES) all

host: guard-CXX guard-SDKTARGETSYSROOT ${HOST_EXE}
${HOST_EXE}: $(HOST_DIR)/*.cpp
	make -C $(HOST_DIR) AIE_DIR=$(AIE_DIR) $(SIZES)

package: guard-ROOTFS guard-IMAGE guard-PLATFORM_REPO_PATHS package_${TARGET}
package_${TARGET}: ${LIBADF} ${XCLBIN} ${HOST_EXE}
	${VCC} -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_dir aie/data \
		--package.sd_file addon_files/embedded_exec.sh \
		--package.sd_file ${HOST_EXE} ${XCLBIN} ${LIBADF}



clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.*  \
	       vek280*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil *bin *BIN *.bif launch_hw_emu.sh cfg emu_qemu_scripts \
				 [!d]*.json  *.txt *.wdb *.wcfg *.pdi v++.package_summary sim qemu_dts_files sd_card sd_card.img \
				 dtb_creation.sh .ipcache *summary *.sh .AIE_SIM_CMD_LINE_OPTIONS .crashReporter
	make -C ${AIE_DIR} clean
	make -C ${KERNELS_DIR} clean
	make -C ${HOST_DIR} clean

clean-all:
	make clean
	make -C ${AIE_DIR} clean-all

soft-clean:
	make -C ${AIE_DIR} soft-clean
