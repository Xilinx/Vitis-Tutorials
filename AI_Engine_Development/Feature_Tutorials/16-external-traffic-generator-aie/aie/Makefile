
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# Makefile input options if not defined from top-level stage

# TARGET --> sw_emu | hw_emu
TARGET ?= sw_emu

# EXTIO --> true | false
EXTIO ?= true

# TRAFFIC_GEN --> PYTHON | CPP
TRAFFIC_GEN ?= CPP

# File names and locations
GRAPH := src/graph.cpp
GRAPH_O := libadf.a
PYTHONPATH += ${XILINX_VIVADO}/data/emulation/hw_em/lib/python:${XILINX_VIVADO}/data/emulation/python/xtlm_ipc:${XILINX_VIVADO}/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/python

LD_LIBRARY_PATH:=$(XILINX_VIVADO)/data/simmodels/xsim/2023.1/lnx64/9.3.0/ext/protobuf/:$(XILINX_VIVADO)/lib/lnx64.o/Default:$(XILINX_VIVADO)/lib/lnx64.o/:$(LD_LIBRARY_PATH)
# Command-line options
AIECC := aiecompiler

AIE_INCLUDE_FLAGS := -include="$(XILINX_VITIS)/aietools/include" -include="./src" -include="./data" -include="./src/kernels" -include="./"
AIE_FLAGS := $(AIE_INCLUDE_FLAGS) --xlopt=0 -workdir=./Work

# External IO for External Traffic Generator
ifeq (${EXTIO}, true)
	AIE_FLAGS += --Xpreproc=-DEXTERNAL_IO
endif

AIESIM := aiesimulator --pkg-dir=./Work
X86SIM := x86simulator --pkg-dir=./Work

ifeq (${TRAFFIC_GEN}, PYTHON)
#For Python External Traffic Generator
AIESIM_ETG := $(AIESIM) & ${PYTHON_INSTALL}/python3 ../TrafficGenerator/Python/xtg_aie.py
X86SIM_ETG := $(X86SIM) & ${PYTHON_INSTALL}/python3 ../TrafficGenerator/Python/xtg_aie.py
else
# For C++ Traffic Generator
AIESIM_ETG := $(AIESIM) & ../TrafficGenerator/Cpp/chain
X86SIM_ETG := $(X86SIM) & ../TrafficGenerator/Cpp/chain
endif


.ONESHELL:
.PHONY: clean all aie disp

###
# Guarding Checks. Do not modify.
###
check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202120_1)

disp:
	echo "TARGET = '"${TARGET}"'"
	echo "EXTIO = '"${EXTIO}"'"
	echo "TRAFFIC_GEN = '"${TRAFFIC_GEN}"'"

aie:  $(GRAPH_O)

$(GRAPH_O): $(GRAPH)
ifeq (${TARGET}, sw_emu)
	$(AIECC) --target=x86sim $(AIE_FLAGS) $(GRAPH)
else
	$(AIECC) --target=hw $(AIE_FLAGS) $(GRAPH)
endif



aiesim: $(GRAPH_O)
ifeq (${EXTIO}, false)
ifeq (${TARGET}, sw_emu)
	$(X86SIM)
	../check_op 1 data/polar_clip_in_reim_ref.txt ./x86simulator_output/data/output1.txt
else
	$(AIESIM)
	../check_op 1 data/polar_clip_in_reim_ref.txt ./aiesimulator_output/data/output1.txt
endif
else  # Use External Traffic Generator
ifeq (${TARGET}, sw_emu)
	$(X86SIM_ETG)
	../check_op 2 data/s2mm_ref.txt DataOut1.txt
else
	$(AIESIM_ETG)
	../check_op 2 data/s2mm_ref.txt DataOut1.txt
endif
endif

#####################################################


clean:
	rm -rf Work *_output *.log libadf.a *.vcd pl_sample_counts ISS_RPC_SERVER_PORT *.txt .Xil .AIE_SIM_CMD_LINE_OPTIONS
