#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

SIM_FIFO          := false

MY_APP            := firbank_app
MY_SOURCES        := ${MY_APP}.cpp firbank_graph.h

PART              := xcve2802-vsvh1760-2MP-e-S

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

DSPLIB_OPTS 	  := --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/vss/common \
		     --include=${DSPLIB_ROOT}/L1/include/vss/vss_fft_ifft_1d \
		     --include=${DSPLIB_ROOT}/L1/src/aie

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${DSPLIB_OPTS} --part=${PART} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif

.PHONY: clean x86com x86sim sim profile throughput

all:		compile profile throughput check_sim_output_aie

x86all: 	x86com x86sim check_sim_output_x86

gen_vectors:
	matlab -batch "gen_vectors"

compile:	${MY_SOURCES}
	time v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} 2>&1 |  tee log

x86com:
	time v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} 2>&1 |  tee log

sim:
	time aiesimulator 2>&1 |  tee -a log

x86sim:
	time x86simulator 2>&1 |  tee -a log

profile:
	time aiesimulator --profile --online -wdb -text 2>&1 |  tee -a log

trace:
	time aiesimulator --online -wdb -text 2>&1 |  tee -a log

throughput:
	@for z in `seq 0 3`; do \
		../../scripts/throughput.py aiesimulator_output/data/filterbank_o_$${z}.txt --iscomplex;\
	done;


check_sim_output_aie:
	matlab -batch "check_sim_output('hw')"

check_sim_output_x86:
	matlab -batch "check_sim_output('x86sim')"

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_*
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir .wsdata _ide

