
#Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

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
${HOST_EXE}: ${GRAPH} 
	$(MAKE) -C sw/

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       vck190_aie_base*.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil aiesimu* x86simu* AIECompiler.log Map_Report.csv sol.db Work *.exe
	$(MAKE) -C sw clean
