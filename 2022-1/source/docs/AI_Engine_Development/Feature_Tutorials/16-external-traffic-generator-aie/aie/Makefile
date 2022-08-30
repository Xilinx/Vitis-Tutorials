#© Copyright 2022 Xilinx, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#

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
AIESIM_ETG := $(AIESIM) & python3 ../TrafficGenerator/Python/xtg_aie.py
X86SIM_ETG := $(X86SIM) & python3 ../TrafficGenerator/Python/xtg_aie.py
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
	../check_op 1 data/s2mm_ref.txt x86simulator_output/data/DataOut1.txt
else
	$(AIESIM)
	../check_op 1 data/s2mm_ref.txt aiesimulator_output/data/DataOut1.txt
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
