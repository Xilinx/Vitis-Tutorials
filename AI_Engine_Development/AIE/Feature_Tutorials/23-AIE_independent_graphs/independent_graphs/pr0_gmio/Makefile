
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

MODE	 = linux
XSA	 = ${ARCH}_base_graph_${TARGET}.xsa
HOST_EXE = host.exe

GRAPH    = aie/graph.cpp
LIBADF  = libadf.a
AIE_CMPL_CMD = v++ -c --mode aie --platform=${PLATFORM} --config aie.cfg --include="./aie" ${GRAPH} 
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work 

VCC      = v++
VPP_SPEC =system.cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}  
LDCLFLAGS=

.PHONY: clean

aie: ${LIBADF}
${LIBADF}: aie/*
	${AIE_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

host: ${HOST_EXE}
${HOST_EXE}: ${GRAPH} 
	$(MAKE) -C sw/

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       *.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil  aiesimu* x86simu* foo.vcd pl_sample_counts
	$(MAKE) -C sw clean
