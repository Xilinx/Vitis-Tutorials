#
# Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#


# AIE compiler tools
AIE_TOP_DIR = .
AIE_BUILD_DIR = .
AIE_SRC_DIR = src
AIE_OUT_DIR = ${AIE_BUILD_DIR}/Work
AIE_DATA_DIR = ${AIE_BUILD_DIR}/../data


AIE_GRAPH_FILES = ${AIE_TOP_DIR}/$(AIE_SRC_DIR)/SSRgraph.cpp


# AIE compiler settings
AIE_FLAGS = -v --Xrouter=dmaFIFOsInFreeBankOnly
AIE_FLAGS += -include="${XILINX_VITIS}/aietools/include" -include="${AIE_TOP_DIR}/" -include="${AIE_TOP_DIR}/${AIE_SRC_DIR}" -include="${AIE_TOP_DIR}/data"
AIE_FLAGS += -workdir=$(AIE_OUT_DIR)
AIE_FLAGS += --enable-ecc-scrubbing=false

ifdef XSA
	AIE_FLAGS += -platform ${XSA}
endif

X86_FLAGS := $(AIE_FLAGS)

# Event trace
AIE_FLAGS += --event-trace=runtime
AIE_FLAGS += --num-trace-streams=16 #default 16
AIE_FLAGS +=  --event-trace-port=gmio #default gmio
# AIE_FLAGS += --trace-plio-width=128 #default 64
AIE_FLAGS += --broadcast-enable-core=true #default true
AIE_FLAGS += --xlopt=0


#
# primary build targets
#

all: clean aie aiesim

all-x86: clean x86 x86sim-deadlock x86viz

x86:
	aiecompiler -target=x86sim $(X86_FLAGS) $(AIE_GRAPH_FILES) -platform=$(PLATFORM)

aie:
	aiecompiler -target=hw $(AIE_FLAGS) $(AIE_GRAPH_FILES) -platform=$(PLATFORM)

aie-post:
	aiecompiler

aiesim:
	aiesimulator --pkg-dir=Work --dump-vcd=foo  --profile --options-file=aiesim-options.cfg



x86sim-deadlock:
	x86simulator --pkg-dir=Work --stop-on-deadlock

x86sim:
	x86simulator --pkg-dir=Work --stop-on-deadlock --dump

x86viz:
	dot -Tpng x86simulator_output/simulator_state_post_analysis.dot  > simulator_state_post_analysis.png
	firefox simulator_state_post_analysis.png &

cleanall: clean
	rm -rf $(AIE_OUT_DIR) *.log libadf.a x86simulator_output aiesimulator_output *.png pl_sample_count* .Xil .AIE_SIM_CMD_LINE_OPTIONS

clean:
	rm -rf aie_trace*.txt aie_profile_edge.csv aie_event_runtime_config.json xrt.run_summary summary.csv foo.vcd data temp *csv *json *db ISS_RPC_SERVER_PORT


.PHONY: all clean cleanall aie aiesim aiesim-nodeadlock aieviz




.PHONY: incremental
incremental: clean all
