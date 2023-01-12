# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


ECHO              := @echo

AIE_WS            := .
AIE_SRC           := ${AIE_WS}/src
AIE_GRAPHS        := ${AIE_SRC}/graphs
AIE_KERNELS       := ${AIE_SRC}/kernels
MAP_OPT           := BufferOptLevel7
MAX_CYCLES        := 10000
FLAG              = true
#REF_FILE          = ./data/dpd_out_ref.txt
DEVICE_FILE       = VC1902
MY_APP            := fir_graph
TEST_BENCH        = ${AIE_SRC}/${MY_APP}.cpp
SIM_OUTPUT        = ${AIE_WS}/aiesimulator_output
WORK_DIR          = ${AIE_WS}/Work
LOG_FILE          = ${AIE_WS}/log
OTHER_OPTS        = --kernel-linting  -xlopt=2 --genArchive --pl-register-threshold=125 --log-level=5 --test-iterations=-1 --pl-axi-lite=false
#PL_FREQ           = --pl-freq=lte_0:153.6 --pl-freq=lte_1:153.6 --pl-freq=tx_chain0.pccfr_pl:307.2 --pl-freq=tx_chain0.wcfr_pl:307.2 --pl-freq=tx_chain0.dpd_out0:500.0
PL_FREQ           = --pl-freq=500.0

TIME_LOG          = ${AIE_WS}/timelog
CONSTRAINT_FILE   = ${AIE_SRC}/${MY_APP}.aiecst

AIE_INCLUDES      = -include="$(XILINX_VITIS)/aietools/include" \
	                  -include="$(DSPLIB_ROOT)/L1/include/aie" \
	                  -include="$(DSPLIB_ROOT)/L1/src/aie" \
	                  -include="$(DSPLIB_ROOT)/L1/tests/aie/inc" \
	                  -include="$(DSPLIB_ROOT)/L1/tests/aie/src" \
	                  -include="$(DSPLIB_ROOT)/L2/include/aie" \
	                  -include="$(DSPLIB_ROOT)/L2/tests/aie/common/inc" \
                    -include="${AIE_GRAPHS}" \
                    -include="${AIE_SRC}"

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

${AIE_OUTPUT}: ${AIE_SRC}/* ${AIE_GRAPHS}/*
	$(MAKE) clean
	aiecompiler -target=hw ${AIE_INCLUDES} ${TEST_BENCH} --device=${DEVICE_FILE} -workdir=${WORK_DIR} |& tee ${LOG_FILE}

x86compile:
	aiecompiler --dataflow -v -include=$(COMM_INC) -include=$(COMM_SRC) -include=$(FILTERS_INC) -include=$(FILTERS_SRC) -include=$(RADIO_INC) -include=$(RADIO_SRC)  $(TEST_BENCH) $(OTHER_OPTS) --device=$(DEVICE_FILE) -Xchess="gain:cpp.define=USER_PARAMS" -workdir=$(WORK_DIR) --x86Sim |& tee $(LOG_FILE)

sim:
	aiesimulator --pkg-dir=$(WORK_DIR) -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES) |& tee -a $(LOG_FILE)

sim_analyzer:
	aiesimulator --pkg-dir=$(WORK_DIR) --profile --online -wdb -ctf -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES) |& tee -a $(LOG_FILE)

analyze:
	vitis_analyzer -report system.wdb

get_op:
	grep -v T ${SIM_OUTPUT}/data/output0.txt > ${SIM_OUTPUT}/data/output0_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output1.txt > ${SIM_OUTPUT}/data/output1_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output2.txt > ${SIM_OUTPUT}/data/output2_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output3.txt > ${SIM_OUTPUT}/data/output3_raw.txt

x86sim:
	x86simulator --pkg-dir=$(WORK_DIR) |& tee -a $(LOG_FILE)

clean:
	rm -rf Work Work* $(WORK_DIR)
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf aiesimulator_output
	rm -rf log log* $(LOG_FILE)
	rm -rf libadf.a *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe

timestamp:
	@echo TIME: >> $(TIME_LOG);\
	date +%s >> $(TIME_LOG)
