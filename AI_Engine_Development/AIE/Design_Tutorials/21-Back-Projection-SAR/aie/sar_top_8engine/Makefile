#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

AIE_SIM_ONLY      := false

SIM_FIFO          := false

USE_JSON          := false

USE_EVENTS        := false

MY_APP            := sar_top_8engine_app
MY_SOURCES        := ${MY_APP}.cpp ${MY_APP}.aiecst ${MY_APP}_aie_routed.aiecst sar_top_8engine_cfg.h \
                  ../sar_top_1engine/sar_top_1engine_graph.h \
		  ../ifft2k_async/ifft2k_async_graph.h ../ifft2k_async/ifft2k_async.h \
                  ../ifft2k_async/ifft2k_async.cpp ../ifft2k_async/ifft2k_async_twiddles_internal.h \
                  ../ifft2k_async/ifft2k_async_twiddles_external.h \
	          ../sar_engine/sar_engine_graph.h \
		  ../diff3dsq/diff3dsq_graph.h ../diff3dsq/diff3dsq.h ../diff3dsq/diff3dsq.cpp \
		  ../sqrt_lib/sqrt_lib_graph.h \
		  ../dR_comp/dR_comp_graph.h ../dR_comp/dR_comp.h ../dR_comp/dR_comp.cpp \
		  ../fmod_floor/fmod_floor_graph.h ../fmod_floor/fmod_floor.h ../fmod_floor/fmod_floor.cpp \
		  ../expjx/expjx_graph.h ../expjx/expjx.h ../expjx/expjx.cpp \
		  ../cos_lib/cos_lib_graph.h ../cos_lib/cos_lib_lut_init.h \
		  ../sin_lib/sin_lib_graph.h ../sin_lib/sin_lib_lut_init.h \
		  ../bp_update/bp_update_graph.h ../bp_update/bp_update.h ../bp_update/bp_update.cpp \
		  ../bp_update/input_combine.h ../bp_update/input_combine.cpp \
	          ../interp1/interp1_graph.h ../interp1/interp1.h ../interp1/interp1.cpp


PLATFORM_USE      := xilinx_vck190_base_202610_1
PLATFORM          ?= ${PLATFORM_REPO_PATHS}/${PLATFORM_USE}/${PLATFORM_USE}.xpfm

CHECK_FIFO        := --aie.evaluate-fifo-depth --aie.Xrouter=disablePathBalancing

DSPLIB_OPTS 	  := --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/src/aie \
	             --include=../sar_top_1engine \
	             --include=../sar_engine \
		     --include=../ifft2k_async \
		     --include=../range_gen \
		     --include=../diff3dsq \
		     --include=../sqrt_lib \
		     --include=../dR_comp \
		     --include=../fmod_floor \
		     --include=../expjx \
		     --include=../cos_lib \
		     --include=../sin_lib \
		     --include=../bp_update \
		     --include=../interp1

AIE_OUTPUT := libadf.a

AIE_FLAGS :=	${DSPLIB_OPTS} --platform=${PLATFORM} ${MY_APP}.cpp --aie.output-archive=${AIE_OUTPUT}
AIE_SIM_FLAGS :=    --hang-detect-time=2200000 --display-run-interval=10000

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif
ifeq (${AIE_SIM_ONLY},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.Xpreproc="-DAIE_SIM_ONLY"
endif
ifeq (${USE_JSON},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.constraints=${MY_APP}_aie_routed.aiecst
else
	AIE_FLAGS := ${AIE_FLAGS} --aie.constraints=${MY_APP}.aiecst
endif
ifeq (${USE_EVENTS},true)
	AIE_FLAGS := ${AIE_FLAGS} --aie.graph-iterator-event --aie.event-trace=runtime
endif

.PHONY: help clean x86com x86sim sim profile throughput

help::
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to generate everything for this design"
	@echo ""
	@echo "  make compile"
	@echo "      Run AIE compiler and build the design"
	@echo ""
	@echo "  make profile"
	@echo "      Run AIE simulator with profiling and generate outputs suitable for Vitis Analyzer"
	@echo ""
	@echo "  make analyze"
	@echo "      Run Vitis Analyzer to inspect results"
	@echo ""
	@echo "  make throughput"
	@echo "      Evaluate the achieved throughput at the output"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all:		${AIE_OUTPUT}

x86all: 	clean x86com x86sim

${AIE_OUTPUT}:	${MY_SOURCES}
	v++ --compile --config aie.cfg --mode aie --target=hw ${AIE_FLAGS} 2>&1| tee log

x86com:
	v++ --compile --config aie.cfg --mode aie --target=x86sim ${AIE_FLAGS} 2>&1| tee log

sim:
	aiesimulator ${AIE_SIM_FLAGS} 2>&1| tee -a log

x86sim:
	x86simulator 2>&1| tee -a log

profile:
	aiesimulator ${AIE_SIM_FLAGS} --online -wdb -text --profile 2>&1| tee -a log

trace:
	aiesimulator ${AIE_SIM_FLAGS} --online -wdb -text 2>&1| tee -a log

trace2:
	aiesimulator ${AIE_SIM_FLAGS} --dump-vcd=dumpvcd 2>&1| tee -a log

clean:
	rm -rf .Xil Work libadf.a
	rm -rf aiesimulator_output* aiesimulator*.log
	rm -rf x86simulator_output*
	rm -rf log log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* pl_sample_counts* pl_sample_count_* _ide* vfs_work
	rm -rf temp ISS_RPC_SERVER_PORT .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf system*.* trdata.aiesim function_wdb_dir .wsdata .ipynb_checkpoints

