#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#


#################################################################################
##     Directories
#################################################################################
# AIE_DIR := $(PWD)/aie
# KERNELS_DIR := $(PWD)/pl_kernels
# HOST_DIR := $(PWD)/sw
# ADDON_DIR := $(PWD)/addon_files

AIE_DIR := ./aie
KERNELS_DIR := ./pl_kernels
HOST_DIR := ./sw
ADDON_DIR := ./addon_files

#################################################################################
## Project Parameters
#################################################################################

# hw_emu|hw

TARGET   ?= hw
PFM_NAME := xilinx_vck190_base_202510_1
PFM_NAME := $(strip $(PFM_NAME))
PLATFORM := ${PLATFORM_REPO_PATHS}/${PFM_NAME}/${PFM_NAME}.xpfm
PNAME := vck190_aie_base_graph_${TARGET}
XCLBIN   := ${PNAME}.xsa
HOST_EXE := host.exe

LIBADF  := $(AIE_DIR)/libadf.a
EMU_CMD := ./launch_hw_emu.sh -add-env AIE_COMPILER_WORKDIR=$(AIEDIR)/Work -aie-sim-options $(ADDON_DIR)/aiesim_options.txt

FREQ_HZ := 312500000 # PL Frequency (PL kernels)


PLIOW := 128 # Width of the PLIOs



##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################





##XOS      = $(subst .cpp,.xo,$(wildcard $(KERNELS_DIR)/*.cpp))
XOS      = $(subst .cpp,.xo,$(wildcard $(KERNELS_DIR)/*_dual_$(strip $(PLIOW)).cpp))
VCC      = v++
VPP_SPEC = $(ADDON_DIR)/system_dual_$(strip $(PLIOW)).cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}

.PHONY: clean run_hw_emu data



###
.PHONY: all_hw all_hw_emu run

run: all_hw

all_hw:
	make TARGET=hw clean data aie kernels xclbin host package



all_hw_emu:
	make TARGET=hw_emu clean data aie kernels xclbin host package


upd_host_hw:
	make TARGET=hw host package

aie:  ${LIBADF}
${LIBADF}:
	make -C $(AIE_DIR) aie $(AIE_FLAGS) PLATFORM=$(PLATFORM)

postaie: 
	make -C $(AIE_DIR) aie $(AIE_FLAGS) XSA=../${PNAME}.xsa

	
data:
	rm -rf data
	rm -rf aie/data
	mkdir -p data
	../Utils/GenerateStreams cint16 $(PLIOW) 8 2 256 4 Dirac data/PhaseIn
	./rename_files.sh 15 25
	cp -r data aie/data
	
aiesim: ${LIBADF}
	make -C $(AIE_DIR) aiesim

aiesim-fifo: ${LIBADF}
	make -C $(AIE_DIR) aiesim-fifo

aieviz:
	vitis_analyzer aie/Work/SSRgraph.aiecompile_summary
	
aiesimviz:
	vitis_analyzer aie/aiesimulator_output/default.aierun_summary

x86: 
	make -C $(AIE_DIR) x86 $(AIE_FLAGS) PLATFORM=$(PLATFORM)

x86sim: ${LIBADF}
	make -C $(AIE_DIR) x86sim



xclbin:  ${XCLBIN}
${XCLBIN}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

kernels:  ${XOS}
${XOS}:
	make -C pl_kernels/ PLATFORM=${PLATFORM} FREQ_HZ=${FREQ_HZ} PLIOW=${PLIOW} all

host:  ${HOST_EXE}
${HOST_EXE}: $(HOST_DIR)/*.cpp
	make -C $(HOST_DIR) AIE_DIR=../$(AIE_DIR)

package:  package_${TARGET}
package_${TARGET}: ${LIBADF} ${XCLBIN} ${HOST_EXE}
	${VCC} -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_dir data \
		--package.sd_file addon_files/embedded_exec.sh \
		--package.sd_file addon_files/xrt.ini \
		--package.sd_file addon_files/newdir \
		--package.sd_file ${HOST_EXE} ${XCLBIN} ${LIBADF}
	zip sd_card_$(shell date +%y%m%d_%H%M)_DualSSR.zip sd_card.img



run_hw_emu:
	$(EMU_CMD)

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* data _ide \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil *bin *BIN *.bif launch_hw_emu.sh cfg emu_qemu_scripts \
				 *.json  *.txt *.wdb *.wcfg *.pdi v++.package_summary sim qemu_dts_files sd_card sd_card.img \
				 dtb_creation.sh .ipcache *summary sd_card_*.zip
	make -C ${AIE_DIR} cleanall
	make -C ${KERNELS_DIR} clean
	make -C ${HOST_DIR} clean
