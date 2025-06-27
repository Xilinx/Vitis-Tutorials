
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

HOST_EXE = host.exe

GRAPH    = aie/graph.cpp
GRAPH2    = aie2/graph.cpp
LIBADF  = libadf.a
LIBADF2  = libadf2.a
AIE_CMPL_CMD = v++ -c --mode aie --platform=${PLATFORM} --config aie.cfg --include="./aie" --aie.output-archive=${LIBADF} ${GRAPH} 
AIE_CMPL_CMD2 = v++ -c --mode aie --platform=${PLATFORM} --config aie.cfg --include="./aie2" --aie.output-archive=${LIBADF2} ${GRAPH2} 
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work 

.PHONY: clean

aie: ${LIBADF}
${LIBADF}: aie/*
	${AIE_CMPL_CMD}

aie2: ${LIBADF2}
${LIBADF2}: aie2/*
	${AIE_CMPL_CMD2}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

host: ${HOST_EXE}
${HOST_EXE}: ${GRAPH} 
	$(MAKE) -C sw/

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       *.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil  aiesimu* x86simu* *.log Work *.csv *.exe libadf2.a libadf2.a 
	$(MAKE) -C sw clean
