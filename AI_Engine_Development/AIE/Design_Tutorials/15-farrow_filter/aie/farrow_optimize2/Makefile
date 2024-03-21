#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

AIE_SIM_ONLY      ?= true

PLATFORM_USE	  := xilinx_vck190_base_202320_1
PLATFORM          := ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

MY_APP             = farrow_app.cpp
MY_SOURCES         = ${MY_APP} farrow_graph.h farrow_kernel.cpp farrow_kernel.h
LOG_FILE           = log
OTHER_OPTS         = --aie.pl-freq=625.0 --aie.Xchess="-std=c++17"
ALL_INCLUDE       := --include=${PWD}
AIE_FLAGS         := --aie.verbose ${ALL_INCLUDE} ${MY_APP} ${OTHER_OPTS} --platform=${PLATFORM} --aie.Xmapper=BufferOptLevel9 --aie.xlopt=1

ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.Xpreproc="-DAIE_SIM_ONLY"
endif

.PHONY: clean help compile sim

help::
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to generate everything for this design"
	@echo ""
	@echo "  make compile"
	@echo "      Run AIE compiler and build the design"
	@echo ""
	@echo "  make sim"
	@echo "      Run AIE simulator"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all          : compile sim get_II check_sim_output_aie

x86all       : x86compile x86sim check_sim_output_x86

compile:	${MY_SOURCES}
	v++ -c --mode aie --target=hw ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

x86compile:
	v++ -c --mode aie --target=x86sim ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

sim:
	aiesimulator --profile --online -wdb -text -i=../.. 2>&1 | tee -a ${LOG_FILE}

x86sim:
	x86simulator 2>&1 -i=../.. | tee -a ${LOG_FILE}

check_sim_output_aie:
	matlab -batch "check_sim_output('hw')"

check_sim_output_x86:
	matlab -batch "check_sim_output('x86sim')"

throughput:
	@../../scripts/throughput.py aiesimulator_output/data/sig_o.txt --iscomplex

get_II:
	@../../scripts/get_loop_II.py Work/aie/

clean:
	@rm -rf Work* .Xil function_wdb_dir .crashReporter .AIE_SIM_CMD_LINE_OPTIONS AIECompiler.log xcd.log log aiesimulator_output x86simulator_output diff_output pl_sample_count* tmp vitis_analyzer.* xsc_report.log diag_report.log libadf.a sol.db t.txt Map_Report.csv AIESimulator.log
	@rm -rf ISS_RPC_SERVER_PORT system_flat.wcfg system.wcfg system.wdb tmp.vcd.vcd trdata.aiesim vcdanalyze.log vitis_analyzer_pid* logs VCDAnalyze.log