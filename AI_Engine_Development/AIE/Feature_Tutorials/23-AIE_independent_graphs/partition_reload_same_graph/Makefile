
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
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
HOST_EXE0 = pr0_gmio/host0.exe
HOST_EXE1 = pr1_rtp/host1.exe
HOST_EXE2 = pr2_perf/host2.exe
HOST_ALL = host.exe
HOST_EXE = ${HOST_EXE0} ${HOST_EXE1} ${HOST_EXE2} ${HOST_ALL}

LIBADF   = pr0_gmio/libadf.a pr1_rtp/libadf.a pr2_perf/libadf.a

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
	make -C pr0_gmio aie TARGET=${TARGET} ARCH=${ARCH} PLATFORM=${PLATFORM}
	make -C pr1_rtp aie TARGET=${TARGET} ARCH=${ARCH} PLATFORM=${PLATFORM}
	make -C pr2_perf aie TARGET=${TARGET} ARCH=${ARCH} PLATFORM=${PLATFORM}

host: ${HOST_EXE}
${HOST_EXE}:
	make -C pr0_gmio host
	make -C pr1_rtp host
	make -C pr2_perf host
	make -C sw

xsa: ${XSA}
${XSA}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

kernels: ${XOS}
${XOS}: 
	$(MAKE) -C pl_kernels/ PLATFORM=${PLATFORM}

package: package_${TARGET}
package_${TARGET}: ${LIBADF} ${XSA} ${HOST_EXE} ${HOST_ALL} pl.xclbin pr0.xclbin pr1.xclbin pr2.xclbin
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
		--package.sd_file pl.xclbin \
		--package.sd_file pr0.xclbin \
		--package.sd_file pr1.xclbin \
		--package.sd_file pr2.xclbin \
		--package.sd_file embedded_exec.sh \
		${XSA} \
		${LIBADF}

package_pl: pl.xclbin
pl.xclbin:
	${VCC} -p -s -t ${TARGET} -f ${PLATFORM} \
		--config package_pl_only.cfg \
		--output pl.xclbin \
		${XSA}

package_pr0: pr0.xclbin
pr0.xclbin:
	${VCC} -p -s -t ${TARGET} -f ${PLATFORM} \
		--package.defer_aie_run \
		--package.aie_overlay ./pr0_gmio/libadf.a \
		--output pr0.xclbin \
		${XSA}

package_pr1: pr1.xclbin
pr1.xclbin:
	${VCC} -p -s -t ${TARGET} -f ${PLATFORM} \
		--package.defer_aie_run \
		--package.aie_overlay ./pr1_rtp/libadf.a \
		--output pr1.xclbin \
		${XSA}

package_pr2: pr2.xclbin
pr2.xclbin:
	${VCC} -p -s -t ${TARGET} -f ${PLATFORM} \
		--package.defer_aie_run \
		--package.aie_overlay ./pr2_perf/libadf.a \
		--output pr2.xclbin \
		${XSA}

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil aiesimu* x86simu* *.xclbin *.pdi *.bif sd_card* *summary BOOT.BIN *.bin .ipcache
	$(MAKE) -C pl_kernels clean
	$(MAKE) -C sw clean
	$(MAKE) -C pr0_gmio clean
	$(MAKE) -C pr1_rtp clean
	$(MAKE) -C pr2_perf clean
