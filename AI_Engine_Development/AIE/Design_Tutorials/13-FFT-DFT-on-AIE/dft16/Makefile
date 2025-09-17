#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

AIE_SIM_ONLY	  := false
SIM_FIFO          := false

MY_APP            := dft16_app
MY_SOURCES        := ${MY_APP}.cpp dft16_graph.h dft_1xN_widget_graph.h dft_1xN_widget.h dft_1xN_widget.cpp \
	             twiddle_dft16.h

PLATFORM_USE      := xilinx_vck190_base_202520_1
PLATFORM          ?= ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

AIE_OUTPUT        := libadf.a

AIE_FLAGS         := --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif
ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.Xpreproc="-DAIE_SIM_ONLY"
endif

all:	${AIE_OUTPUT} sim throughput_ok

${AIE_OUTPUT}:	${MY_SOURCES}
	v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} 2>&1| tee log

x86com:
	v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} 2>&1| tee log

sim:
	aiesimulator 2>&1| tee -a log

x86sim:
	x86simulator 2>&1| tee -a log

trace:
	aiesimulator --online -wdb -text 2>&1| tee -a log

profile:
	aiesimulator --online -wdb -text --profile --profile 2>&1| tee -a log

throughput_ok:
	@../python/check_throughput.py PLIO_dft_o_0 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_1 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_2 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_3 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_4 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_5 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_6 4309.0 5.0
	@../python/check_throughput.py PLIO_dft_o_7 4309.0 5.0

clean:
	rm -rf Work Work* ${WORK_DIR}
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf x86simulator_output
	rm -rf log log* ${LOG_FILE}
	rm -rf libadf.a *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* vitis_design_wrapper* function_wdb_dir*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter* .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir

