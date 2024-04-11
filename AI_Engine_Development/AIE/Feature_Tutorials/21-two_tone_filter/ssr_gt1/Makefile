# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

PLATFORM_VER	:= xilinx_vck190_base_202320_1
PLATFORM	:= platform=${PLATFORM_REPO_PATHS}/${PLATFORM_VER}/${PLATFORM_VER}.xpfm
PROJECT_PATH	:= $(shell pwd)
SIM_OUTPUT  	:= aiesimulator_output
LOG_FILE    	:= log

LOG_FIR2	:= log_fir2
LOG_FFT2	:= log_fft2
LOG_ALL2	:= log_all2

FIR2_APP    := fir2_app.cpp
FFT2_APP    := fft2_app.cpp
ALL2_APP    := all2_app.cpp
APP_FILE	:= ${ALL2_APP}

WORK_ALL2 	:= ${PROJECT_PATH}/build/hw/Work_all2
WORK_FIR2   := ${PROJECT_PATH}/build/hw/Work_fir2
WORK_FFT2   := ${PROJECT_PATH}/build/hw/Work_fft2

FIR2_LIB	:= libadf_fir2.a
FFT2_LIB	:= libadf_fft2.a
ALL2_LIB	:= libadf_all2.a

CHECK_FIFO      := --evaluate-fifo-depth -Xrouter=disablePathBalancing

SRC_PATH	:= src
TARGET		:= hw
SIM_FIFO        := false

WORK_AIESIM	:= ${PROJECT_PATH}/build/hw/Work_all2
WORK_X86SIM	:= ${PROJECT_PATH}/build/x86sim/Work_all2
APP_NAME	:= all2

DSPLIB_INCLUDE 	:= --include=${DSPLIB_ROOT}/L2/include/aie \
		     --include=${DSPLIB_ROOT}/L1/include/aie \
		     --include=${DSPLIB_ROOT}/L1/src/aie

define genConfig
${PLATFORM}
work_dir=${PROJECT_PATH}/build/$(TARGET_AIE)/Work_${APP_NAME}
include=${PROJECT_PATH}/src
include=${PROJECT_PATH}/data
input_files=${PROJECT_PATH}/src/${APP_FILE}
target=$(TARGET_AIE)
[aie]
Xchess=-std=c++17
xlopt=1
pl-freq=500
Xmapper=BufferOptLevel9

endef

gen_file: all2_x86sim all2_hw fir2_hw fft2_hw
all2_x86sim: export TARGET_AIE=x86sim
all2_x86sim: export APP_FILE=${ALL2_APP}
all2_x86sim: export APP_NAME=all2
all2_x86sim: export genConfig:=$(genConfig)
all2_x86sim:
	@echo "$${genConfig}" > aie_config_x86sim.cfg

all2_hw: export TARGET_AIE=hw
all2_hw: export APP_FILE=${ALL2_APP}
all2_hw: export APP_NAME=all2
all2_hw: export genConfig:=$(genConfig)
all2_hw:
	@echo "$${genConfig}" > aie_config_hw.cfg

fir2_hw: export TARGET_AIE=hw
fir2_hw: export APP_FILE=${FIR2_APP}
fir2_hw: export APP_NAME=fir2
fir2_hw: export genConfig:=$(genConfig)
fir2_hw:
	@echo "$${genConfig}" > aie_config_fir2lib_hw.cfg

fft2_hw: export TARGET_AIE=hw
fft2_hw: export APP_FILE=${FFT2_APP}
fft2_hw: export APP_NAME=fft2
fft2_hw: export genConfig:=$(genConfig)
fft2_hw:
	@echo "$${genConfig}" > aie_config_fft2lib_hw.cfg

ifeq (${SIM_FIFO}, true)
	AIE_FLAGS := ${AIE_FLAGS} ${CHECK_FIFO}
endif

#AIE_FLAGS_FIR2    := --workdir=${WORK_FIR2} ${AIE_FLAGS}
#AIE_FLAGS_FFT2    := --workdir=${WORK_FFT2} ${AIE_FLAGS}
#AIE_FLAGS_ALL2    := --workdir=${WORK_ALL2} ${AIE_FLAGS}

.PHONY: help clean all_fir2 all_fft2 all_all2

help:
	@echo "Makefile Usage:"
	@echo ""
	@echo "  make all_fir2"
	@echo "      Command to generate everything for the FIR portion of the design"
	@echo ""
	@echo "  make all_fft2"
	@echo "      Command to generate everything for the FFT portion of the design"
	@echo ""
	@echo "  make all_all2"
	@echo "      Command to generate everything for the combined FIR/FFT design"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all_all2: 	${ALL2_LIB} profile_all2 throughput_all2
all_fir2: 	${FIR2_LIB} profile_fir2 throughput_fir2
all_fft2: 	${FFT2_LIB} profile_fft2 throughput_fft2

all_aiecom_sim: aiecompile aiesim_all2

aiecompile: genConfigFile ${ALL2_LIB}

genConfigFile: gen_file


#${FIR2_LIB}:	fir2_graph.h ${FIR2_APP}
#	aiecompiler --output-archive ${FIR2_LIB} ${FIR2_APP} ${AIE_FLAGS_FIR2} |& tee ${LOG_FIR2}

${FIR2_LIB}:	./${SRC_PATH}/fir2_graph.h ./${SRC_PATH}/${FIR2_APP}
	v++ -c --mode aie --config aie_config_fir2lib_hw.cfg ${DSPLIB_INCLUDE}

${FFT2_LIB}:	./${SRC_PATH}/fft2_graph.h ./${SRC_PATH}/${FFT2_APP}
	v++ -c --mode aie --config aie_config_fft2lib_hw.cfg ${DSPLIB_INCLUDE}

${ALL2_LIB}:	./${SRC_PATH}/all2_graph.h ./${SRC_PATH}/${ALL2_APP}
ifeq ($(TARGET), x86sim)
	v++ -c --mode aie --config aie_config_x86sim.cfg ${DSPLIB_INCLUDE}  
else ifeq ($(TARGET), hw)
	v++ -c --mode aie --config aie_config_hw.cfg ${DSPLIB_INCLUDE}
endif

sim_fir2:
	aiesimulator --pkg-dir=${WORK_FIR2} -o aiesimulator_fir2 |& tee -a ${LOG_FIR2}

sim_fft2:
	aiesimulator --pkg-dir=${WORK_FFT2} -o aiesimulator_fft2 |& tee -a ${LOG_FFT2}u

aiesim_all2:
	aiesimulator --pkg-dir=${WORK_AIESIM} -o aiesimulator_all2 |& tee -a ${LOG_ALL2}

x86sim_all2:
	x86simulator --pkg-dir=${WORK_X86SIM} --dump |& tee -a ${LOG_FILE}

profile_fir2:
	aiesimulator --pkg-dir=${WORK_FIR2} -o aiesimulator_fir2 --profile --online -wdb -ctf |& tee -a ${LOG_FIR2}

profile_fft2:
	aiesimulator --pkg-dir=${WORK_FFT2} -o aiesimulator_fft2 --profile --online -wdb -ctf |& tee -a ${LOG_FFT2}

profile_all2:
	aiesimulator --pkg-dir=${WORK_AIESIM} -o aiesimulator_all2 --profile --online -wdb -ctf |& tee -a ${LOG_ALL2} 

throughput_fft2:
	@../scripts/throughput.py aiesimulator_fft2/data/fft_o_0.txt --iscomplex
	@../scripts/throughput.py aiesimulator_fft2/data/fft_o_1.txt --iscomplex
	@../scripts/throughput.py aiesimulator_fft2/data/fft_o_2.txt --iscomplex
	@../scripts/throughput.py aiesimulator_fft2/data/fft_o_3.txt --iscomplex

throughput_fir2:
	@../scripts/throughput.py aiesimulator_fir2/data/fir_o_0.txt --iscomplex
	@../scripts/throughput.py aiesimulator_fir2/data/fir_o_1.txt --iscomplex
	@../scripts/throughput.py aiesimulator_fir2/data/fir_o_2.txt --iscomplex
	@../scripts/throughput.py aiesimulator_fir2/data/fir_o_3.txt --iscomplex

throughput_all2:
	@../scripts/throughput.py aiesimulator_all2/data/fir_o_0.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fir_o_1.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fir_o_2.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fir_o_3.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fft_o_0.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fft_o_1.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fft_o_2.txt --iscomplex
	@../scripts/throughput.py aiesimulator_all2/data/fft_o_3.txt --iscomplex

analyze:
	vitis_analyzer aiesimulator_all2/default.aierun_summary

clean:
	rm -rf Work* libadf*
	rm -rf .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	rm -rf aiesimulator* x86simulator*
	rm -rf log*
	rm -rf *.xpe *.elf *.db *.soln Map_* xnw* *.lp *.log .xil .Xil *.lp *.db *.log *.exe *.vcd *.json
	rm -rf vitis_analyzer* function_wdb_dir vitis_design_wrapper*
	rm -rf temp ISS_RPC_SERVER_PORT
	rm -rf system*.* pl_sample_count*
