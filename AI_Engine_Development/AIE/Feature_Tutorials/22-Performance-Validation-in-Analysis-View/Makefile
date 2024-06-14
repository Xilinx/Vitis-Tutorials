#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#

include ./common.mk


#########################################
#
# SYSTEM FILENAMES
#
########################################
XCLBIN   := ${PNAME}.xsa
HOST_EXE := host.exe
HOST_API_EXE := host_api.exe
LIBADF  := $(AIE_DIR)/libadf.a


##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################

KERNEL_CPP := $(KERNELS_DIR)/gen2s_$(DATAW)_$(PLIOW).cpp \
				$(KERNELS_DIR)/s2mm_$(DATAW)_$(PLIOW).cpp
XO_FILES := $(subst .cpp,.xo,$(KERNEL_CPP))

XOS      = $(XO_FILES)
VCC      = v++
VPP_SPEC = $(ADDON_DIR)/system_$(strip $(NAntenna)).cfg
VPP_FLAGS =--save-temps --verbose --config ${VPP_SPEC}

ifeq ($(strip $(ENABLE_TRACE)),true)
	VPP_FLAGS += --profile.data all:all:all
endif

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
	$(call check_defined, REPO, Set your where you downloaded your platform)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2023.1/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2023.1/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux)

###
.PHONY: all_hw all_hw_emu run upd_host_hw aie postaie data aiesim aieviz aiesim-fifo compareaie x86 x86sim comparex86


######################################################################
#
#   Full chains
#
######################################################################

build_hw:
	make TARGET=hw  clean aie kernels xclbin host package

build_hw_emu:
	make TARGET=hw_emu  clean aie kernels xclbin host package

launch_hw_emu:
	./launch_hw_emu.sh -add-env AIE_COMPILER_WORKDIR=$(AIE_DIR)/Work -aie-sim-options $(ADDON_DIR)/aiesim_options.txt -run-app embedded_exec.sh

run_hw_emu:
	make build_hw_emu launch_hw_emu

upd_host_hw:
	make TARGET=hw host package

######################################################################
#
#   AIE Compilation/Simulation
#
######################################################################
aie:
	make -C $(AIE_DIR) aie

postaie:
	make -C $(AIE_DIR) aie  XSA=../${PNAME}.xsa

data:
	make -C $(AIE_DIR)  data

aiesim:
	make -C $(AIE_DIR)  aiesim

aieviz:
	make -C $(AIE_DIR)  aieviz

simviz:
	make -C $(AIE_DIR)  simviz

x86:
	make -C $(AIE_DIR) x86

x86sim:
	make -C $(AIE_DIR) x86sim

######################################################################
#
#   PL Kernels compilation
#
######################################################################

kernels: guard-PLATFORM_REPO_PATHS ${XOS}
${XOS}:
	make -C pl_kernels/ PLATFORM=${PLATFORM}  all


######################################################################
#
#   System Link
#
######################################################################

xclbin: guard-PLATFORM_REPO_PATHS ${XCLBIN}
${XCLBIN}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

$(VPP_SPEC):
	make -C $(ADDON_DIR) all

######################################################################
#
#   PS host code compilation
#
######################################################################

host: guard-CXX guard-SDKTARGETSYSROOT ${HOST_EXE}
${HOST_EXE}: $(HOST_DIR)/*.cpp
	make -C $(HOST_DIR) AIE_DIR=$(AIE_DIR)  all


######################################################################
#
#   Packaging --> SD Card Image
#
######################################################################

package: guard-ROOTFS guard-IMAGE guard-PLATFORM_REPO_PATHS package_${TARGET}
package_${TARGET}: ${LIBADF} ${XCLBIN} ${HOST_EXE}
	${VCC} -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_dir $(AIE_DIR)/data \
		--package.sd_file $(ADDON_DIR)/embedded_exec.sh \
		--package.sd_file $(ADDON_DIR)/xrt.ini \
		--package.sd_file $(ADDON_DIR)/newdir \
		--package.sd_file $(ADDON_DIR)/newdir_api \
		--package.sd_file ${HOST_EXE} ${XCLBIN} ${LIBADF} \
		--package.sd_file ${HOST_API_EXE} 
	zip sd_card.zip sd_card.img


######################################################################
#
#   Clean functions
#
######################################################################

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.*  \
	    	vek280*.xclbin* vck190*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	    	ilpProblem* sol.db drivers .Xil *bin *BIN *.bif launch_hw_emu.sh cfg emu_qemu_scripts \
			[!d]*.json  *.txt *.wdb *.wcfg *.pdi v++.package_summary sim qemu_dts_files sd_card sd_card.img \
			sd_card.zip dtb_creation.sh .ipcache *summary *.sh .AIE_SIM_CMD_LINE_OPTIONS .crashReporter logs .wsdata
	make -C ${AIE_DIR} clean
	make -C ${KERNELS_DIR} clean
	make -C ${HOST_DIR} clean
	# make -C $(ADDON_DIR) clean

clean-all:
	make clean
	make -C ${AIE_DIR} clean-all

soft-clean:
	make -C ${AIE_DIR} soft-clean
