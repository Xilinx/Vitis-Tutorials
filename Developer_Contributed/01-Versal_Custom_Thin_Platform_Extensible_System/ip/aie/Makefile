 #
 # Copyright 2021 Xilinx, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

ECHO := @echo

TARGET            ?= hw

TARGET_HW         := hw
AIE_WS            := .
AIE_SRC           := ${AIE_WS}/src
AIE_GRAPHS        := ${AIE_SRC}/graphs
AIE_KERNELS       := ${AIE_SRC}/kernels
MAP_OPT           := BufferOptLevel7
MAX_CYCLES        := 10000
FLAG              = true
#REF_FILE          = ./data/dpd_out_ref.txt
DEVICE_FILE       = VC1902
MY_APP            := datamove_app
TEST_BENCH        = ${AIE_SRC}/${MY_APP}.cpp
SIM_OUTPUT        = ${AIE_WS}/aiesimulator_output
WORK_DIR          = ${AIE_WS}/Work
LOG_FILE          = ${AIE_WS}/log
OTHER_OPTS        = --kernel-linting -Xchess="-std=c++17" -xlopt=2 --genArchive --pl-register-threshold=125 --log-level=5 --test-iterations=-1 --pl-axi-lite=false
#PL_FREQ           = --pl-freq=lte_0:153.6 --pl-freq=lte_1:153.6 --pl-freq=tx_chain0.pccfr_pl:307.2 --pl-freq=tx_chain0.wcfr_pl:307.2 --pl-freq=tx_chain0.dpd_out0:500.0
PL_FREQ           = --pl-freq=500.0

TIME_LOG          = ${AIE_WS}/timelog
CONSTRAINT_FILE   = ${AIE_SRC}/${MY_APP}.aiecst

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

${AIE_OUTPUT}: ${AIE_SRC}/* ${AIE_GRAPHS}/* ${AIE_KERNELS}/*
	$(MAKE) clean
	aiecompiler -target=${TARGET_HW} --dataflow -v --use-phy-shim=${FLAG} --constraints=${CONSTRAINT_FILE} -include=${AIE_SRC} -include=${AIE_GRAPHS} -include=${AIE_KERNELS}  ${TEST_BENCH} ${OTHER_OPTS} -Xmapper=${MAP_OPT} --device=${DEVICE_FILE} -workdir=${WORK_DIR} ${PL_FREQ} |& tee ${LOG_FILE}

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
