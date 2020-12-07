
TARGET   = hw_emu
PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_es1_base_202020_1/xilinx_vck190_es1_base_202020_1.xpfm
XCLBIN   = vck190_aie_base_graph_${TARGET}.xclbin
HOST_EXE = host.exe
ROOTFS   = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2020.2/rootfs.ext4 
IMAGE    = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2020.2/Image 

GRAPH    = src/graph.cpp
LIBADF  = libadf.a
AIE_CMPL_CMD = aiecompiler -platform=${PLATFORM} -include="./src" -include="./data" -include="./src/kernels" -include="./" --pl-axi-lite=true -workdir=./Work ${GRAPH}
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work

##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################


XOS      = $(subst .cpp,.xo,$(wildcard pl_kernels/*.cpp)) 
VCC      = v++
VPP_SPEC =system.cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}  

.PHONY: clean

all: ${XCLBIN} ${HOST_EXE} package

${LIBADF}: ${GRAPH}
	${AIE_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

xclbin: ${XCLBIN}
${XCLBIN}: ${LIBADF} ${VPP_SPEC} ${XOS} 
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} --vivado.prop fileset.sim_1.xsim.simulate.runtime=300us   \
	       -t ${TARGET} ${VPP_FLAGS} -o $@

${XOS}: 
	make -C pl_kernels/
	
${HOST_EXE}: sw/*.cpp
	make -C sw/

package: package_${TARGET}
package_${TARGET}: ${LIBADF} ${XCLBIN} ${HOST_EXE} 
	${VCC} -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_dir data \
		--package.sd_file ${HOST_EXE} ${XCLBIN} ${LIBADF}

run_hw_emu: launch_hw_emu.sh
launch_hw_emu.sh: package_hw_emu
	./launch_hw_emu.sh -add-env VITIS_LAUNCH_WAVEFORM_BATCH=1

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil
	make -C pl_kernels clean
	make -C sw clean
