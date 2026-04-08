
#Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

TARGET   = hw
ARCH = aie

ifeq ($(ARCH), aie)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202610_1/xilinx_vck190_base_202610_1.xpfm
else ifeq ($(ARCH), aie-ml)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202610_1/xilinx_vek280_base_202610_1.xpfm
else
$(error ARCH is not supported)
endif

XSA   = aie_base_graph_${TARGET}.xsa
HOST_EXE0 = pr0_gmio/host0.exe
HOST_EXE1 = pr1_rtp/host1.exe
HOST_EXE2 = pr2_perf/host2.exe
HOST_ALL = host.exe
HOST_EXE = ${HOST_EXE0} ${HOST_EXE1} ${HOST_EXE2} ${HOST_ALL}

GRAPH    = aie/graph.cpp
LIBADF   = pr0_gmio/libadf.a pr1_rtp/libadf.a pr2_perf/libadf.a
AIE_CMPL_CMD = v++ -c --mode aie --platform=${PLATFORM} --config aie.cfg --include="./aie" --work_dir=./Work ${GRAPH}
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work
EMU_CMD = ./launch_hw_emu.sh

##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
##########################################################################################################################################################

XOS      = $(subst .cpp,.xo,$(wildcard pl_kernels/*.cpp)) 
VCC      = v++
VPP_SPEC =system.cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}  
LDCLFLAGS=

.PHONY: clean

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
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202610_1)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2025.2/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2025.2/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux)

###

all: package
run: all 
sd_card: all

aie: ${LIBADF}
${LIBADF}: 
	make -C pr0_gmio aie
	make -C pr1_rtp aie
	make -C pr2_perf aie

host: ${HOST_EXE}
${HOST_EXE}:
	make -C pr0_gmio host
	make -C pr1_rtp host
	make -C pr2_perf host
	make -C sw

xsa: guard-PLATFORM_REPO_PATHS ${XSA}
${XSA}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

kernels: guard-PLATFORM_REPO_PATHS ${XOS}
${XOS}: 
	$(MAKE) -C pl_kernels/ PLATFORM=${PLATFORM}

package: guard-ROOTFS guard-IMAGE guard-PLATFORM_REPO_PATHS package_${TARGET}
package_${TARGET}: ${LIBADF} ${XSA} ${HOST_EXE} 
	${VCC} -p -s -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_file ${HOST_EXE0} \
		--package.sd_file ${HOST_EXE1} \
		--package.sd_file ${HOST_EXE2} \
		--package.sd_file ${HOST_ALL} \
		--package.sd_file embedded_exec.sh \
		${XSA} \
		${LIBADF}

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil aiesimu* x86simu*
	$(MAKE) -C pl_kernels clean
	$(MAKE) -C sw clean
