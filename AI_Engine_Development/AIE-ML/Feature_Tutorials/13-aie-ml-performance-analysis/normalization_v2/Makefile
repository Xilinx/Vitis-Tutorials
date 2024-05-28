
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

TARGET   = hw
PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202410_1/xilinx_vek280_base_202410_1.xpfm
XSA   = vek280_aie_base_graph_${TARGET}.xsa
HOST_EXE = host.exe

GRAPH    = aie/graph.cpp
LIBADF  = libadf.a
AIE_CMPL_CMD = v++ -c --mode aie --platform=${PLATFORM} --include="./aie" --work_dir=./Work ${GRAPH} --aie.pl-freq=312.5 --aie.xlopt=0 2>&1 | tee log.txt
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work --dump-vcd=foo --profile --end-wait-time=120

##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################


XOS      = $(subst .cpp,.xo,$(wildcard pl_kernels/*.cpp)) 
VCC      = v++
VPP_SPEC =system.cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}  

.PHONY: clean

all: ${XSA} ${HOST_EXE} package
run: all
sd_card: all

aie: ${LIBADF}
${LIBADF}: ${GRAPH}
	${AIE_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

xclbin: ${XSA}
${XSA}: ${LIBADF} ${VPP_SPEC} ${XOS} ${GRAPH_XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF}  \
	       -t ${TARGET} ${VPP_FLAGS} -o $@

kernels: ${XOS}
${XOS}: 
	make -C pl_kernels/
	
host: ${HOST_EXE}
${HOST_EXE}: sw/*.cpp
	make -C sw/

package: package_${TARGET}
package_${TARGET}: ${XSA} ${LIBADF} ${HOST_EXE}
	${VCC} -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_dir data \
		--package.sd_file embedded_exec.sh \
		--package.sd_file ${HOST_EXE} ${XSA} ${LIBADF} 

run_hw_emu: launch_hw_emu.sh
launch_hw_emu.sh: package_hw_emu
	./launch_hw_emu.sh 

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vek280_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil
	make -C pl_kernels clean
	make -C sw clean
