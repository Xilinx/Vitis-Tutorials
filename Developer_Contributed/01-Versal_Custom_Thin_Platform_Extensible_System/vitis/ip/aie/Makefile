#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO              := @echo

BOARD_NAME        ?= vck190
PLATFORM_CUSTOM   ?= ${BOARD_NAME}_thin

XPFM              := ../../xpfm_export/build/platform/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}/${PLATFORM_CUSTOM}.xpfm
TARGET            ?= hw

AIE_WS            := .
AIE_SRC           := ${AIE_WS}/src
AIE_GRAPHS        := ${AIE_SRC}/graphs
AIE_KERNELS       := ${AIE_SRC}/kernels
MAX_CYCLES        := 10000
FLAG              = true
MY_APP            := datamove_app
CONSTRAINT_FILE   = ${AIE_SRC}/${MY_APP}.aiecst
TEST_BENCH        = ${AIE_SRC}/${MY_APP}.cpp
SIM_OUTPUT        = ${AIE_WS}/aiesimulator_output
WORK_DIR          = ${AIE_WS}/Work
LOG_FILE          = ${AIE_WS}/aiecompiler.log
SIM_LOG_FILE      = ${AIE_WS}/aiesimulator.log
AIE_OPTS          := -target=${TARGET}
#AIE_OPTS          += --dataflow
AIE_OPTS          += -v
#AIE_OPTS          += --use-phy-shim=true
AIE_OPTS          += --platform=${XPFM}
AIE_OPTS          +=  --workdir=${WORK_DIR}
AIE_OPTS          +=  --constraints=${CONSTRAINT_FILE}
#AIE_DSPLIB        := -include=$(DSPLIB_ROOT)/L1/include/aie
#AIE_DSPLIB        += -include=$(DSPLIB_ROOT)/L1/src/aie
#AIE_DSPLIB        += -include=$(DSPLIB_ROOT)/L2/include/aie
AIE_INCLUDE       := -include=${AIE_SRC}
AIE_INCLUDE       += -include=${AIE_GRAPHS}
AIE_INCLUDE       += -include=${AIE_KERNELS}
# Use to pass arguments to preprocessing # AIE_XFLAGS        := --Xpreproc="-DNB_FFT=${NB_FFT}"
OTHER_OPTS        += --kernel-linting
#OTHER_OPTS        += -Xchess="-std=c++17"
OTHER_OPTS        += -xlopt=2
OTHER_OPTS        += -Xmapper=BufferOptLevel7
OTHER_OPTS        += --pl-freq=500.0
OTHER_OPTS        += --pl-register-threshold=125
OTHER_OPTS        += --log-level=5
OTHER_OPTS        += --test-iterations=-1
OTHER_OPTS        += --pl-axi-lite=false


TIME_LOG          = ${AIE_WS}/timelog

AIE_OUTPUT        := libadf.a

.PHONY: help clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make sim_analyzer"
	$(ECHO) "      Run AIE simulator and generate outputs suitable for Vitis Analyzer"
	$(ECHO) ""
	$(ECHO) "  make get_op"
	$(ECHO) "      Strip simulator output data from timestamps and copy to new file"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all          : ${AIE_OUTPUT}
allx86       : clean x86compile x86sim

#${AIE_OUTPUT}: ${AIE_SRC}/* ${AIE_GRAPHS}/* ${AIE_KERNELS}/*
#	$(MAKE) clean
#	aiecompiler -target=hw -v --use-phy-shim=${FLAG} --constraints=${CONSTRAINT_FILE} -include=${AIE_SRC} -include=${AIE_GRAPHS} -include=${AIE_KERNELS}  ${TEST_BENCH} ${OTHER_OPTS} -Xmapper=${MAP_OPT} --platform=${XPFM} -workdir=${WORK_DIR} ${PL_FREQ} |& tee ${LOG_FILE}

${AIE_OUTPUT}: ${AIE_SRC}/* ${AIE_GRAPHS}/*
#${AIE_KERNELS}/*
	$(MAKE) clean
	aiecompiler ${AIECC_INCLUDES} \
		${AIE_INCLUDE} \
		${AIE_DSPLIB} \
		${AIE_OPTS} \
		${AIE_XFLAGS} \
		${TEST_BENCH} \
		${OTHER_OPTS} \
		|& tee ${LOG_FILE}


sim:
	aiesimulator --pkg-dir=$(WORK_DIR) -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES) |& tee -a $(SIM_LOG_FILE)

sim_analyzer:
	aiesimulator --pkg-dir=$(WORK_DIR) --profile --online -wdb -ctf -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES) |& tee -a $(SIM_LOG_FILE)

analyze:
	vitis_analyzer -report system.wdb

get_op:
	grep -v T ${SIM_OUTPUT}/data/output0.txt > ${SIM_OUTPUT}/data/output0_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output1.txt > ${SIM_OUTPUT}/data/output1_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output2.txt > ${SIM_OUTPUT}/data/output2_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output3.txt > ${SIM_OUTPUT}/data/output3_raw.txt

x86sim:
	x86simulator --pkg-dir=$(WORK_DIR) |& tee -a $(SIM_LOG_FILE)

clean:
	rm -rf Work Work* $(WORK_DIR)
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf aiesimulator_output
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS ISS_RPC_SERVER_PORT pl_sample_counts
	rm -rf log log* $(LOG_FILE)
	rm -rf libadf.a *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.wdb *.wcfg

timestamp:
	@echo TIME: >> $(TIME_LOG);\
	date +%s >> $(TIME_LOG)
