#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

AIE_SIM_ONLY	  := false
SIM_FIFO          := false

MY_APP            := fft32_dsplib_app
MY_SOURCES        := ${MY_APP}.cpp fft32_dsplib_graph.h

PLATFORM_USE      := xilinx_vck190_base_202410_1
PLATFORM          ?= ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --evaluate-fifo-depth -Xrouter=disablePathBalancing

AIE_OUTPUT        := libadf.a

DSPLIB_OPTS 	  := --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/src/aie

AIE_FLAGS         := --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT} ${DSPLIB_OPTS}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif
ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.Xpreproc="-DAIE_SIM_ONLY"
endif

all: 	${AIE_OUTPUT} sim

${AIE_OUTPUT}:	 ${MY_SOURCES}
	v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} |& tee log

x86com:
	v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} |& tee log

sim:
	aiesimulator |& tee -a log

x86sim:
	x86simulator |& tee -a log

trace:
	aiesimulator --online -wdb -text |& tee -a log

profile:
	aiesimulator --online -wdb -text --profile |& tee -a log

vcd:
	aiesimulator --dump-vcd=hough_tile |& tee -a log

throughput:
	@../python/throughput.py aiesimulator_output/data/sig0_o.txt --iscomplex

loopII:
	@../python/get_loop_II.py Work/aie

clean:
	rm -rf Work Work* ${WORK_DIR}
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf x86simulator_output
	rm -rf log log* ${LOG_FILE}
	rm -rf libadf.a *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_count* vitis_design_wrapper* function_wdb_dir* trdata.aiesim*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter* .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir

