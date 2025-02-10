
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

TARGET   = hw
ARCH = aie

ifeq ($(ARCH), aie)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202510_1/xilinx_vck190_base_202510_1.xpfm
else ifeq ($(ARCH), aie-ml)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202510_1/xilinx_vek280_base_202510_1.xpfm
else
$(error ARCH is not supported)
endif

HOST_EXE = host.exe

GRAPH    = aie/graph.cpp
LIBADF   = libadf.a
AIE_CMPL_CMD = v++ -c --mode aie --platform=${PLATFORM} --config aie.cfg --include="./aie" --work_dir=./Work ${GRAPH} 
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work

.PHONY: clean

aie: ${LIBADF}
${LIBADF}: ${GRAPH}
	${AIE_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

host: ${HOST_EXE}
${HOST_EXE}: sw/*.cpp
	$(MAKE) -C sw/

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil aiesimu* x86simu* pl_sample_counts throughput_info.json
	$(MAKE) -C sw clean
