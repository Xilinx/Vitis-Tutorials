
#Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

TARGET   = hw
ARCH = aie

ifeq ($(ARCH), aie)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202520_1/xilinx_vck190_base_202520_1.xpfm
else ifeq ($(ARCH), aie-ml)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202520_1/xilinx_vek280_base_202520_1.xpfm
else
$(error ARCH is not supported)
endif

XSA   = aie_base_graph_${TARGET}.xsa
HOST_EXE = pr1_rtp/host1.exe

LIBADF   = pr1_rtp/libadf.a 

XOS      = $(subst .cpp,.xo,$(wildcard pl_kernels/*.cpp)) 
VCC      = v++
VPP_SPEC =system.cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}  
LDCLFLAGS=

.PHONY: clean
all: package
run: all 
sd_card: all

aie: ${LIBADF}
${LIBADF}: 
	make -C pr1_rtp aie TARGET=${TARGET} ARCH=${ARCH} PLATFORM=${PLATFORM}

host: ${HOST_EXE}
${HOST_EXE}:
	make -C pr1_rtp host

xsa: ${XSA}
${XSA}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

kernels: ${XOS}
${XOS}: 
	$(MAKE) -C pl_kernels/ PLATFORM=${PLATFORM}

package: package_${TARGET}
package_${TARGET}: ${LIBADF} ${XSA} ${HOST_EXE} pl.xclbin pr1.xclbin 
	${VCC} -p -s -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_file ${HOST_EXE} \
		--package.sd_file pl.xclbin \
		--package.sd_file pr1.xclbin \
		--package.sd_file embedded_exec.sh \
		${XSA} \
		${LIBADF}

package_pl: pl.xclbin
pl.xclbin:
	${VCC} -p -s -t ${TARGET} -f ${XSA} \
		--config package_pl_only.cfg \
		--output pl.xclbin 

package_pr1: pr1.xclbin
pr1.xclbin:
	${VCC} -p -s -t ${TARGET} -f ${XSA} \
		--package.defer_aie_run \
		--package.aie_overlay ./pr1_rtp/libadf.a \
		--output pr1.xclbin 

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil aiesimu* x86simu* *.xclbin *summary *.bif *.pdi *.BIN *.bin sd_card* .ipcache
	$(MAKE) -C pl_kernels clean
	$(MAKE) -C pr1_rtp clean
