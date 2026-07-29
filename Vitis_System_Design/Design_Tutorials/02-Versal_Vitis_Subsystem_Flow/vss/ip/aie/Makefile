#
# Copyright (C) 2020-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# Author Derek Hagen

ECHO              := @echo

PART              ?= xcvc1902-vsva2197-2MP-e-S
FREQHZ            ?= 625000000

TARGET            ?= hw

MY_APP            := datamove_app
export AIE_WS     := ${MY_APP}_ws
AIE_SRC           := ./src
AIE_GRAPHS        := ${AIE_SRC}/graphs
AIE_KERNELS       := ${AIE_SRC}/kernels
AIE_CFG           := ./aie_common.cfg
AIE_OUTPUT        := ${MY_APP}_libadf.a

MAX_CYCLES        := 10000
FLAG              = true
CONSTRAINT_FILE   = ${AIE_SRC}/${MY_APP}.aiecst
TEST_BENCH        = ${AIE_SRC}/${MY_APP}.cpp
SIM_OUTPUT        = ${AIE_WS}/aiesimulator_output
WORK_DIR          = ${AIE_WS}/Work
LOG_FILE          = ${AIE_WS}/aiecompiler.log
SIM_LOG_FILE      = ${AIE_WS}/aiesimulator.log
#AIE_OPTS          += --use-phy-shim=true
#AIE_OPTS          += --platform=${XPFM}
AIE_OPTS          :=  --part=${PART}
AIE_OPTS          +=  --target=${TARGET}
AIE_OPTS          +=  --aie.workdir=${WORK_DIR}
AIE_OPTS          +=  --aie.output-archive ${AIE_OUTPUT}
AIE_OPTS          +=  --aie.constraints=${CONSTRAINT_FILE}
#AIE_OPTS          +=  --aie.event-trace=runtime
#AIE_OPTS          +=  --freqhz=${FREQHZ}
#AIE_DSPLIB        := -include=$(DSPLIB_ROOT)/L1/include/aie
#AIE_DSPLIB        += -include=$(DSPLIB_ROOT)/L1/src/aie
#AIE_DSPLIB        += -include=$(DSPLIB_ROOT)/L2/include/aie
AIE_INCLUDE       := -include=${AIE_SRC}
AIE_INCLUDE       += -include=${AIE_GRAPHS}
AIE_INCLUDE       += -include=${AIE_KERNELS}

# Use to pass arguments to preprocessing # AIE_XFLAGS        := --Xpreproc="-DNB_FFT=${NB_FFT}"

# added to aie_common.cfg # OTHER_OPTS        := verbose=true
# added to aie_common.cfg # OTHER_OPTS        += kernel-linting=true
# added to aie_common.cfg # #OTHER_OPTS        += -Xchess="-std=c++17"
# added to aie_common.cfg # OTHER_OPTS        += xlopt=1
# added to aie_common.cfg # OTHER_OPTS        += Xmapper=BufferOptLevel7
# added to aie_common.cfg # OTHER_OPTS        += pl-register-threshold=125
# added to aie_common.cfg # OTHER_OPTS        += log-level=5
# added to aie_common.cfg # OTHER_OPTS        += test-iterations=-1
# added to aie_common.cfg # OTHER_OPTS        += pl-axi-lite=false


TIME_LOG          = ${AIE_WS}/timelog

#AIE_OUTPUT        := libadf.a

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
.PHONY: allx86
allx86       : clean x86compile x86sim

#${AIE_OUTPUT}: ${AIE_SRC}/* ${AIE_GRAPHS}/* ${AIE_KERNELS}/*
#	$(MAKE) clean
#	aiecompiler -target=hw -v --use-phy-shim=${FLAG} --constraints=${CONSTRAINT_FILE} -include=${AIE_SRC} -include=${AIE_GRAPHS} -include=${AIE_KERNELS}  ${TEST_BENCH} ${OTHER_OPTS} -Xmapper=${MAP_OPT} --platform=${XPFM} -workdir=${WORK_DIR} ${PL_FREQ} |& tee ${LOG_FILE}

${AIE_WS}:
	mkdir ${AIE_WS}

#${AIE_CFG}: ${AIE_SRC}/* ${AIE_GRAPHS}/* ${AIE_WS}
#	$(ECHO) "# Auto generated from Makefile" > $@
#	$(ECHO) "# Override settings from command line with <PARAM>=<value>" >> $@
#	$(ECHO) $(addprefix include=, ${AIE_SRC}) >> $@
#	$(ECHO) $(addprefix include=, ${AIE_GRAPHS}) >> $@
#	$(ECHO) $(addprefix include=, ${AIE_KERNELS}) >> $@
#	$(ECHO) "target=${TARGET}" >> $@
#	$(ECHO) "part=${PART}" >> $@
#	$(ECHO) "freqhz=${FREQHZ}" >> $@
#	$(ECHO) "" >> $@
#	$(ECHO) "[aie]" >> $@
#	$(ECHO) "verbose=true" >> $@
#	$(ECHO) "workdir=${WORK_DIR}" >> $@
#	$(ECHO) "constraints=${CONSTRAINT_FILE}" >> $@
#	$(ECHO) "kernel-linting=true" >> $@
#	$(ECHO) "xlopt=1" >> $@
#	$(ECHO) "Xmapper=BufferOptLevel7" >> $@
#	$(ECHO) "pl-register-threshold=125" >> $@
#	$(ECHO) "log-level=5" >> $@
#	$(ECHO) "test-iterations=-1" >> $@
#	$(ECHO) "pl-axi-lite=false" >> $@


${AIE_OUTPUT}: ${AIE_WS} ${AIE_SRC}/* ${AIE_GRAPHS}/* ${AIE_KERNELS}/*
	v++ -c --mode aie \
		${AIECC_INCLUDES} \
		${AIE_OPTS} \
		${AIE_XFLAGS} \
		--config ${AIE_CFG} \
		${TEST_BENCH} \
		|& tee ${LOG_FILE}


.PHONY: sim
sim:
	aiesimulator --pkg-dir=$(WORK_DIR) -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES) |& tee -a $(SIM_LOG_FILE)

.PHONY: sim_analyzer
sim_analyzer:
	aiesimulator --pkg-dir=$(WORK_DIR) --profile --online -wdb -text -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES) |& tee -a $(SIM_LOG_FILE)

.PHONY: analyze
analyze:
	vitis_analyzer -report system.wdb

.PHONY: get_op
get_op:
	grep -v T ${SIM_OUTPUT}/data/output0.txt > ${SIM_OUTPUT}/data/output0_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output1.txt > ${SIM_OUTPUT}/data/output1_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output2.txt > ${SIM_OUTPUT}/data/output2_raw.txt
	grep -v T ${SIM_OUTPUT}/data/output3.txt > ${SIM_OUTPUT}/data/output3_raw.txt

.PHONY: x86sim
x86sim:
	x86simulator --pkg-dir=$(WORK_DIR) |& tee -a $(SIM_LOG_FILE)

clean:
	rm -rf Work Work* $(AIE_WS)
	rm -rf .Xil
	rm -rf aiesimulator_output aiesimulator.log
	rm -rf aiesimulator_output
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS ISS_RPC_SERVER_PORT pl_sample_counts
	rm -rf log log* $(LOG_FILE)
	rm -rf ${AIE_OUTPUT} *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.wdb *.wcfg

.PHONY: timestamp
timestamp:
	@echo TIME: >> $(TIME_LOG);\
	date +%s >> $(TIME_LOG)
