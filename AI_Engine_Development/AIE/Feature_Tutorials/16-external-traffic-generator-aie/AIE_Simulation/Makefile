
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# Examples of make calls
#
# make TARGET=x86sim EXTIO=false aie x86sim PLATFORM=</path/to/vck190_base pfm>
#   --> standard X86 simulation
#
# make EXTIO=false aie aiesim PLATFORM=</path/to/vck190_base pfm>
#   --> standard AIE simulation
#
# make TARGET=x86sim EXTIO=true PLATFORM=</path/to/vck190_base pfm> TRAFFIC_GEN=<PYTHON/CPP/MATLAB> PYTHON_INSTALL=<> aie x86sim 
#   -->  X86 simulation using a Python/MATLAB?CPP based External Traffic Generator
#
# make EXTIO=true PLATFORM=</path/to/vck190_base pfm> TRAFFIC_GEN=PYTHON PYTHON_INSTALL=<> aie aiesim
#   -->  AIE simulation using a Python/MATLAB?CPP based External Traffic Generator
#

PLATFORM := xilinx_vck190_base_202320_1

# Makefile input options if not defined from top-level stage

TARGET ?= hw

# EXTIO --> true | false
# true: simulation needs an external traffic Generator
# false: simulation will use provided data files
EXTIO ?= true

# TRAFFIC_GEN --> PYTHON | CPP | MATLAB
# PYTHON: A Python based traffic generator is used
# CPP: A C++ traffic generator is used
# MATLAB: A Matlab based traffic generator is used
PYTHON_INSTALL := $(XILINX_VIVADO)/tps/lnx64/python-3.8.3/bin/

# Full flows
all_aiesim: aie 
all_x86sim: aie 

## File names and locations
BASE_DIR := $(PWD)
GRAPH := $(BASE_DIR)/aie/src/graph.cpp
GRAPH_O := $(BASE_DIR)/aie/libadf.a

.ONESHELL:
.PHONY: clean aie aiesim 
.PHONY: all_aiesim all_x86sim

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


# Makefile input options if not defined from top-level stage

# TARGET --> sw_emu | hw_emu
TARGET ?= hw

# EXTIO --> true | false
EXTIO ?= true

# TRAFFIC_GEN --> PYTHON | CPP | MATLAB
TRAFFIC_GEN ?= PYTHON 

# File names and locations
GRAPH := ./aie/src/graph.cpp
GRAPH_O := libadf.a
PYTHONPATH=$XILINX_VIVADO/data/emulation/python/xtlm_ipc_v2/

# Command-line options
AIECC := v++ -c --mode aie

AIE_INCLUDE_FLAGS := --include="$(XILINX_VITIS)/aietools/include" --include="./aie/src" --include="./data" --include="./src/kernels" --include="./"
AIE_FLAGS := $(AIE_INCLUDE_FLAGS) --aie.xlopt=0 --aie.workdir=./Work --platform $(PLATFORM)

# External IO for External Traffic Generator
ifeq (${EXTIO}, true)
	AIE_FLAGS += --aie.Xpreproc=-DEXTERNAL_IO
endif

# set the AIE Flags
AIESIM := aiesimulator --pkg-dir=./Work --dump-vcd=foo
X86SIM := x86simulator --pkg-dir=./Work

ifeq (${TARGET}, x86sim)
OUTPUT_DIR := x86simulator_output
else
OUTPUT_DIR := aiesimulator_output
endif

ifeq (${TRAFFIC_GEN}, PYTHON)
#For Python External Traffic Generator
AIESIM_ETG := ./scripts/etg_py.sh & $(AIESIM)
X86SIM_ETG := ./scripts/etg_py.sh & $(X86SIM)
endif
ifeq (${TRAFFIC_GEN}, CPP)
# For C++ Traffic Generator
AIESIM_ETG := ./scripts/etg_cpp.sh & $(AIESIM)
X86SIM_ETG := ./scripts/etg_cpp.sh & $(X86SIM)
endif
ifeq (${TRAFFIC_GEN}, MATLAB)
#For MATLAB External Traffic Generator
AIESIM_ETG := ./scripts/etg_matlab.sh & $(AIESIM)
X86SIM_ETG := ./scripts/etg_matlab.sh & $(X86SIM)
endif

.ONESHELL:
.PHONY: clean all aie disp

###
# Guarding Checks. Do not modify.
###

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202120_1)

disp:
	@echo "TARGET = '"${TARGET}"'"
	@echo "EXTIO = '"${EXTIO}"'"
ifeq (${EXTIO}, true)
	@echo "TESTBENCH = '"${TRAFFIC_GEN} based External Traffic"'"
else
	@echo "TESTBENCH = '"PLIO File Based"'"
endif

aie:  $(GRAPH_O) 

$(GRAPH_O): $(GRAPH)
ifeq (${TARGET}, x86sim)
	$(AIECC) --target=x86sim $(AIE_FLAGS) $(GRAPH)
else
	$(AIECC) --target=hw $(AIE_FLAGS) $(GRAPH)
endif

x86sim: $(GRAPH_O) 
ifeq (${EXTIO}, false)
	$(X86SIM)
else  # Use External Traffic Generator
	$(X86SIM_ETG)
endif

aiesim: $(GRAPH_O)
ifeq (${EXTIO}, false)
	$(AIESIM)
else
	$(AIESIM_ETG)
endif
#####################################################

run_x86sim: disp aie x86sim #compare
run_aiesim: disp aie aiesim #compare

compare: 
ifeq (${EXTIO}, true)
	@diff -w ./data/interpolator_golden.txt ./out_interpolator_data.txt; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
	    echo "PASSED: Interpolator validated successfully"; \
	else \
	    echo "ERROR: Interpolator validation failed"; \
	fi
	@diff -w ./data/classifier_golden.txt ./out_classifier_data.txt; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
	    echo "PASSED: Classifier validated successfully"; \
	else \
	    echo "ERROR: Classifier validation failed"; \
	fi
endif
#####################################################

clean:
	rm -rf Work *_output *.log libadf.a *.vcd pl_sample_counts ISS_RPC_SERVER_PORT *.txt .Xil .AIE_SIM_CMD_LINE_OPTIONS
